/**
  ******************************************************************************
  * @file    regEdit.h
  * @brief   Драйвер синтезатора MAX2870 — плата зонда (STM32F010)
  * @version 2.0
  *
  * Что нового в v2.0:
  *  - единый модуль дробной части MAX2870_MODULUS (раньше 1000 была зашита
  *    в двух местах и могла разъехаться с txRem2.modValue);
  *  - max2870_SetFrequencyMHz() — расчёт INT/FRAC и делителя с ТОЧНЫМИ
  *    границами 68.75 / 137.5 / 275 / 550 / 1100 / 2200 МГц;
  *  - max2870_ActualFrequencyMHz() — какая частота реально установлена;
  *  - max2870_IsLocked() — чтение захвата, если MUXOUT заведён на вход МК;
  *  - setRegistr() теперь дёргает setAtt() — аттенюатор следует за командой.
  *
  * API полностью совместим с v1.0 — main.c зонда перекомпилируется без правок.
  ******************************************************************************
  */

#ifndef REGEDIT_H
#define REGEDIT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include <stdint.h>

/* ========================================================================== */
/*  Параметры — единый источник правды                                        */
/* ========================================================================== */

/* Модуль дробного N-делителя.
 * ВАЖНО: на дисплейной стороне (main.cpp -> calculateRegisters) замените
 * зашитое "* 1000.0" на это же число, иначе FRAC в команде и MOD в R1
 * разъедутся — и частота будет молча "плыть". */
#define MAX2870_MODULUS        1000U

/* Частота PFD, МГц: опора 25 МГц, R-счётчик = 1, удвоитель выключен */
#define MAX2870_F_PFD_MHZ      25.0f

/* Диапазон выходной частоты, МГц (окно ГУН 2.2...4.4 ГГц) */
#define MAX2870_F_MIN_MHZ      34.375f
#define MAX2870_F_MAX_MHZ      4400.0f

/* Коды RF-делителя — биты R4[22:20] */
#define MAX2870_DIV_1          0b000
#define MAX2870_DIV_2          0b001
#define MAX2870_DIV_4          0b010
#define MAX2870_DIV_8          0b011
#define MAX2870_DIV_16         0b100
#define MAX2870_DIV_32         0b101
#define MAX2870_DIV_64         0b110
#define MAX2870_DIV_128        0b111

/* Пин lock detect (MUXOUT в режиме LD, R2[28:26] = 010).
 * Если провод заведён на вход МК — раскомментируйте и укажите пин,
 * тогда max2870_IsLocked() начнёт возвращать 0/1 вместо 0xFF. */
/* #define MAX2870_LD_GPIO_Port   GPIOB */
/* #define MAX2870_LD_Pin         GPIO_PIN_0 */

/* ========================================================================== */
/*  Битовые структуры регистров R0...R5 (MAX2870, Table 9)                    */
/* ========================================================================== */

typedef struct
{
    uint32_t adres     : 3;   /* [2:0]   адрес регистра, для R0 = 000   */
    uint32_t frac      : 12;  /* [14:3]  FRAC                           */
    uint32_t integ     : 16;  /* [30:15] INT                            */
    uint32_t reserved  : 1;   /* [31]                                   */
} structR0;

typedef struct
{
    uint32_t adres        : 3;  /* [2:0]   адрес регистра, для R1 = 001 */
    uint32_t modulsValue  : 12; /* [14:3]  MOD                          */
    uint32_t phaseValue   : 12; /* [26:15] PHASE                        */
    uint32_t prescaler    : 1;  /* [27]    1 = dual (8/9)               */
    uint32_t fazeAdj      : 1;  /* [28]    phase adjust                 */
    uint32_t reserved     : 3;  /* [31:29]                              */
} structR1;

typedef struct
{
    uint32_t adres            : 3;  /* [2:0]   адрес, для R2 = 010      */
    uint32_t counterReset     : 1;  /* [3]                              */
    uint32_t CP_freeState     : 1;  /* [4]    three-state CP            */
    uint32_t powerDown        : 1;  /* [5]                              */
    uint32_t PD_polarity      : 1;  /* [6]    1 = positive              */
    uint32_t ldp              : 1;  /* [7]    lock detect precision     */
    uint32_t ldf              : 1;  /* [8]    lock detect function      */
    uint32_t chargePumpCurrent: 4;  /* [12:9]                           */
    uint32_t doubleBufferR4   : 1;  /* [13]                             */
    uint32_t R_counter        : 10; /* [23:14]                          */
    uint32_t Rdiv2            : 1;  /* [24]                             */
    uint32_t referenceDoubler : 1;  /* [25]                             */
    uint32_t muxOut           : 3;  /* [28:26] 010 = lock detect        */
    uint32_t noiseMode        : 2;  /* [30:29] 00 = low noise           */
    uint32_t reserved         : 1;  /* [31]                             */
} structR2;

typedef struct
{
    uint32_t adres             : 3;  /* [2:0]   адрес, для R3 = 011      */
    uint32_t clockDividerValue : 12; /* [14:3]                          */
    uint32_t CLK_divMode       : 2;  /* [16:15] 01 = band select clock  */
    uint32_t reserved3         : 1;  /* [17]                            */
    uint32_t CSR               : 1;  /* [18]                            */
    uint32_t reserved2         : 2;  /* [20:19]                         */
    uint32_t chargeCancel      : 1;  /* [21]                            */
    uint32_t ABP               : 1;  /* [22]    0 = 6 ns, 1 = 3 ns      */
    uint32_t bandSelect        : 1;  /* [23]    1 = reserved/high       */
    uint32_t reserved          : 8;  /* [31:24]                         */
} structR3;

typedef struct
{
    uint32_t adres                    : 3;  /* [2:0]  адрес, для R4 = 100 */
    uint32_t outputPower              : 2;  /* [4:3]  RF out power        */
    uint32_t RFoutputEnable           : 1;  /* [5]                        */
    uint32_t AUXoutputPower           : 2;  /* [7:6]                      */
    uint32_t AUXoutputEnable          : 1;  /* [8]                        */
    uint32_t AUXoutputSelect          : 1;  /* [9]                        */
    uint32_t MTLD                     : 1;  /* [10]                       */
    uint32_t VCOpowerDown             : 1;  /* [11]                       */
    uint32_t bandSelectclockDividerValue : 8; /* [19:12]                  */
    uint32_t RFdividerSelect          : 3;  /* [22:20] см. MAX2870_DIV_*  */
    uint32_t FBselect                 : 1;  /* [23]  1 = fundamental      */
    uint32_t reserved                 : 8;  /* [31:24]                    */
} structR4;

typedef struct
{
    uint32_t adres     : 3;   /* [2:0]   адрес, для R5 = 101            */
    uint32_t reserved4 : 16;  /* [18:3]                                 */
    uint32_t reserved3 : 2;   /* [20:19] по даташиту = 11               */
    uint32_t reserved2 : 1;   /* [21]                                   */
    uint32_t LDpinMode : 2;   /* [23:22] 11 = digital lock detect       */
    uint32_t reserved  : 8;   /* [31:24]                                */
} structR5;

/* ========================================================================== */
/*  «Контейнерные» слова протокола дисплей -> зонд                            */
/* ========================================================================== */

/* Это НЕ регистры чипа! Это упаковка протокола:
 * слово 1: [0] reserved, [3:1] MUXOUT, [15:4] FRAC, [31:16] INT
 * слово 2: [5:0] reserved, [8:6] аттенюатор, [11:9] RF div, [13:12] LD,
 *          [15:14] out power, [19:16] charge pump, [31:20] MOD
 * Зонд разбирает их в bitOperation() и пересобирает настоящие регистры. */
typedef struct
{
    uint32_t reserved    : 1;
    uint32_t muxOut      : 3;
    uint32_t frac_Value  : 12;
    uint32_t intValue    : 16;
} remote1;

typedef struct
{
    uint32_t reserved         : 6;
    uint32_t attenuator3      : 1;
    uint32_t attenuator2      : 1;
    uint32_t attenuator1      : 1;
    uint32_t rfDivider        : 3;
    uint32_t ldPinMod         : 2;
    uint32_t outPower         : 2;
    uint32_t chargePampCurrent: 4;
    uint32_t modValue         : 12;
} remote2;

/* ========================================================================== */
/*  API                                                                       */
/* ========================================================================== */

/* Тени регистров и контейнеры (определены в regEdit.c) */
extern structR0 tipDef_0;
extern structR1 tipDef_1;
extern structR2 tipDef_2;
extern structR3 tipDef_3;
extern structR4 tipDef_4;
extern structR5 tipDef_5;
extern remote1  txRem1;
extern remote2  txRem2;

void     spiAdf(void);         /* запись R5...R0 по SPI (LE-строб)      */
void     defoultSet(void);     /* дефолтные значения всех полей         */

uint32_t initReg0(void);       /* сборка слова регистра R0              */
uint32_t initReg1(void);
uint32_t initReg2(void);
uint32_t initReg3(void);
uint32_t initReg4(void);
uint32_t initReg5(void);

uint32_t remoteInit1(void);    /* контейнерные слова протокола          */
uint32_t remoteInit2(void);    /* (для совместимости с дисплеем)        */

void     bitOperation(void);   /* regRx1/regRx2 -> txRem1/txRem2        */
void     setRegistr(void);     /* txRem -> tipDef + setAtt()            */

/* ---- новое в v2.0 ---------------------------------------------------- */
uint8_t  max2870_SetFrequencyMHz(float fMHz);   /* 0 = OK, 1 = вне диапазона */
float    max2870_ActualFrequencyMHz(void);      /* фактическая частота   */
uint8_t  max2870_IsLocked(void);                /* 0/1, 0xFF = не заведён */

#ifdef __cplusplus
}
#endif

#endif /* REGEDIT_H */
