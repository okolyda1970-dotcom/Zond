/**
  ******************************************************************************
  * @file    regEdit.c
  * @brief   Драйвер синтезатора MAX2870 — плата зонда (STM32F010)
  * @version 2.0
  *
  * Совместимость: API v1.0 сохранён полностью — main.c зонда не меняется.
  * Новое: единый MODULUS, честные границы делителей, расчёт частоты,
  *        аттенюатор теперь следует за командой, заготовка под lock detect.
  ******************************************************************************
  */

#include "main.h"
#include "regEdit.h"

#define SIZEA 4   /* байт на регистр при записи по SPI */

/* ------------------------- тени регистров --------------------------------- */
structR0 tipDef_0;
structR1 tipDef_1;
structR2 tipDef_2;
structR3 tipDef_3;
structR4 tipDef_4;
structR5 tipDef_5;

/* контейнеры протокола дисплей -> зонд */
remote1  txRem1;
remote2  txRem2;

/* --------------------------- внешние данные ------------------------------- */
extern uint32_t regRx1;     /* контейнерное слово 1 из UART (main.c)   */
extern uint32_t regRx2;     /* контейнерное слово 2 из UART (main.c)   */

extern uint8_t pData0[4];   /* побайтовые копии слов регистров (main.c) */
extern uint8_t pData1[4];
extern uint8_t pData2[4];
extern uint8_t pData3[4];
extern uint8_t pData4[4];
extern uint8_t pData5[4];

extern SPI_HandleTypeDef hspi1;

/* setAtt() живёт в main.c (ATT_1_GPIO_Port / ATT_1_Pin).
 * Драйвер её просто вызывает. Если захотите перенести в драйвер —
 * удалите определение из main.c, иначе будет двойное определение. */
extern void setAtt(void);

/* ========================================================================== */
/*  SPI: запись всех шести регистров                                          */
/* ========================================================================== */

void spiAdf(void)
{
    /* MAX2870: строго R5 -> R4 -> R3 -> R2 -> R1 -> R0,
       данные тактуются при LE = 0, защёлка по фронту LE. */
    uint8_t* order[6] = { pData5, pData4, pData3, pData2, pData1, pData0 };

    for (uint8_t r = 0; r < 6; r++)
    {
        LL_GPIO_ResetOutputPin(LE_1_GPIO_Port, LE_1_Pin);
        HAL_SPI_Transmit(&hspi1, order[r], SIZEA, 10);
        LL_GPIO_SetOutputPin(LE_1_GPIO_Port, LE_1_Pin);
    }

    /* v1.0: здесь ещё мигал LED_1 на каждый регистр. Убрано:
       теперь LED_1 мигает один раз на UART-кадр в HAL_UART_RxCpltCallback. */
}

/* ========================================================================== */
/*  Дефолтные значения                                                        */
/* ========================================================================== */

void defoultSet(void)
{
    /* R0: INT=128, FRAC=0 -> VCO 3200 МГц при F_PFD 25 МГц */
    tipDef_0.adres    = 0;
    tipDef_0.frac     = 0;
    tipDef_0.integ    = 128;
    tipDef_0.reserved = 0;

    /* R1: MOD — из единого макроса, PHASE=1, prescaler dual 8/9 */
    tipDef_1.adres       = 0b001;
    tipDef_1.modulsValue = MAX2870_MODULUS;
    tipDef_1.phaseValue  = 1;
    tipDef_1.prescaler   = 1;
    tipDef_1.fazeAdj     = 0;
    tipDef_1.reserved    = 0;

    /* R2: CP 1.25 мА (код 0001), R=1, полярность +, MUXOUT=0 (перезапишется) */
    tipDef_2.adres            = 0b010;
    tipDef_2.counterReset     = 0;
    tipDef_2.CP_freeState     = 0;
    tipDef_2.powerDown        = 0;
    tipDef_2.PD_polarity      = 1;
    tipDef_2.ldp              = 0;
    tipDef_2.ldf              = 0;
    tipDef_2.chargePumpCurrent= 0b0001;
    tipDef_2.doubleBufferR4   = 0;
    tipDef_2.R_counter        = 1;
    tipDef_2.Rdiv2            = 0;
    tipDef_2.referenceDoubler = 0;
    tipDef_2.muxOut           = 0;
    tipDef_2.noiseMode        = 0b00;   /* low noise */
    tipDef_2.reserved         = 0;

    /* R3: CLK div в режиме band select (01), делитель 0 */
    tipDef_3.adres             = 0b011;
    tipDef_3.clockDividerValue = 0;
    tipDef_3.CLK_divMode       = 0b01;
    tipDef_3.reserved3         = 0;
    tipDef_3.CSR               = 0;
    tipDef_3.reserved2         = 0;
    tipDef_3.chargeCancel      = 0;
    tipDef_3.ABP               = 0;
    tipDef_3.bandSelect        = 0;
    tipDef_3.reserved          = 0;

    /* R4: RF вкл, делитель /64, FB = fundamental, band sel clk = 200 */
    tipDef_4.adres                       = 0b100;
    tipDef_4.outputPower                 = 0b00;
    tipDef_4.RFoutputEnable              = 1;
    tipDef_4.AUXoutputPower              = 0;
    tipDef_4.AUXoutputEnable             = 0;
    tipDef_4.AUXoutputSelect             = 0;
    tipDef_4.MTLD                        = 0;
    tipDef_4.VCOpowerDown                = 0;
    tipDef_4.bandSelectclockDividerValue = 200;
    tipDef_4.RFdividerSelect             = MAX2870_DIV_64;
    tipDef_4.FBselect                    = 1;
    tipDef_4.reserved                    = 0;

    /* R5: LD pin = digital lock detect (11), [20:19] = 11 по даташиту */
    tipDef_5.adres     = 0b101;
    tipDef_5.reserved4 = 0;
    tipDef_5.reserved3 = 0b11;
    tipDef_5.reserved2 = 0;
    tipDef_5.LDpinMode = 0b11;
    tipDef_5.reserved  = 0;

    /* контейнеры протокола (эхо-режим дисплея) */
    txRem1.intValue   = 125;
    txRem1.frac_Value = 0;
    txRem1.muxOut     = 0;
    txRem1.reserved   = 0;

    txRem2.modValue          = MAX2870_MODULUS;
    txRem2.chargePampCurrent = 0b0010;
    txRem2.outPower          = 0b01;
    txRem2.rfDivider         = MAX2870_DIV_2;
    txRem2.ldPinMod          = 0b11;
    txRem2.attenuator1       = 0;
    txRem2.attenuator2       = 0;
    txRem2.attenuator3       = 0;
    txRem2.reserved          = 0;
}

/* ========================================================================== */
/*  Сборка слов регистров (побитно по Table 9 даташита MAX2870)               */
/* ========================================================================== */

uint32_t initReg0(void)
{
    return (tipDef_0.adres    << 0)
         | (tipDef_0.frac     << 3)
         | (tipDef_0.integ    << 15)
         | (tipDef_0.reserved << 31);
}

uint32_t initReg1(void)
{
    return (tipDef_1.adres       << 0)
         | (tipDef_1.modulsValue << 3)
         | (tipDef_1.phaseValue  << 15)
         | (tipDef_1.prescaler   << 27)
         | (tipDef_1.fazeAdj     << 28)
         | (tipDef_1.reserved    << 29);
}

uint32_t initReg2(void)
{
    return (tipDef_2.adres             << 0)
         | (tipDef_2.counterReset      << 3)
         | (tipDef_2.CP_freeState      << 4)
         | (tipDef_2.powerDown         << 5)
         | (tipDef_2.PD_polarity       << 6)
         | (tipDef_2.ldp               << 7)
         | (tipDef_2.ldf               << 8)
         | (tipDef_2.chargePumpCurrent << 9)
         | (tipDef_2.doubleBufferR4    << 13)
         | (tipDef_2.R_counter         << 14)
         | (tipDef_2.Rdiv2             << 24)
         | (tipDef_2.referenceDoubler  << 25)
         | (tipDef_2.muxOut            << 26)
         | (tipDef_2.noiseMode         << 29)
         | (tipDef_2.reserved          << 31);
}

uint32_t initReg3(void)
{
    return (tipDef_3.adres             << 0)
         | (tipDef_3.clockDividerValue << 3)
         | (tipDef_3.CLK_divMode       << 15)
         | (tipDef_3.reserved3         << 17)
         | (tipDef_3.CSR               << 18)
         | (tipDef_3.reserved2         << 19)
         | (tipDef_3.chargeCancel      << 21)
         | (tipDef_3.ABP               << 22)
         | (tipDef_3.bandSelect        << 23)
         | (tipDef_3.reserved          << 24);
}

uint32_t initReg4(void)
{
    return (tipDef_4.adres                       << 0)
         | (tipDef_4.outputPower                 << 3)
         | (tipDef_4.RFoutputEnable              << 5)
         | (tipDef_4.AUXoutputPower              << 6)
         | (tipDef_4.AUXoutputEnable             << 8)
         | (tipDef_4.AUXoutputSelect             << 9)
         | (tipDef_4.MTLD                        << 10)
         | (tipDef_4.VCOpowerDown                << 11)
         | (tipDef_4.bandSelectclockDividerValue << 12)
         | (tipDef_4.RFdividerSelect             << 20)
         | (tipDef_4.FBselect                    << 23)
         | (tipDef_4.reserved                    << 24);
}

uint32_t initReg5(void)
{
    return (tipDef_5.adres     << 0)
         | (tipDef_5.reserved4 << 3)
         | (tipDef_5.reserved3 << 19)
         | (tipDef_5.reserved2 << 21)
         | (tipDef_5.LDpinMode << 22)
         | (tipDef_5.reserved  << 24);
}

/* ========================================================================== */
/*  Контейнерные слова протокола (формат дисплея, НЕ регистры чипа!)          */
/* ========================================================================== */

uint32_t remoteInit1(void)
{
    /* ВНИМАНИЕ: биты [3:1] здесь — MUXOUT, а не адрес регистра.
       В чип это слово слать нельзя — только через bitOperation(). */
    return (txRem1.reserved   << 0)
         | (txRem1.muxOut     << 1)
         | (txRem1.frac_Value << 4)
         | (txRem1.intValue   << 16);
}

uint32_t remoteInit2(void)
{
    return (txRem2.reserved          << 0)
         | (txRem2.attenuator3       << 6)
         | (txRem2.attenuator2       << 7)
         | (txRem2.attenuator1       << 8)
         | (txRem2.rfDivider         << 9)
         | (txRem2.ldPinMod          << 12)
         | (txRem2.outPower          << 14)
         | (txRem2.chargePampCurrent << 16)
         | (txRem2.modValue          << 20);
}

/* ========================================================================== */
/*  Разбор команды от дисплея                                                 */
/* ========================================================================== */

void bitOperation(void)
{
    txRem1.reserved   = 0x1    & (regRx1 >> 0);
    txRem1.muxOut     = 0x7    & (regRx1 >> 1);
    txRem1.frac_Value = 0xFFF  & (regRx1 >> 4);
    txRem1.intValue   = 0xFFFF & (regRx1 >> 16);

    txRem2.modValue          = 0xFFF & (regRx2 >> 20);
    txRem2.chargePampCurrent = 0xF   & (regRx2 >> 16);
    txRem2.outPower          = 0x3   & (regRx2 >> 14);
    txRem2.ldPinMod          = 0x3   & (regRx2 >> 12);
    txRem2.rfDivider         = 0x7   & (regRx2 >> 9);
    txRem2.attenuator1       = 0x1   & (regRx2 >> 8);
    txRem2.attenuator2       = 0x1   & (regRx2 >> 7);
    txRem2.attenuator3       = 0x1   & (regRx2 >> 6);
    txRem2.reserved          = 0x3F  & (regRx2 >> 0);
}

void setRegistr(void)
{
    tipDef_0.integ  = txRem1.intValue;
    tipDef_0.frac   = txRem1.frac_Value;
    tipDef_2.muxOut = txRem1.muxOut;

    tipDef_4.RFdividerSelect   = txRem2.rfDivider;
    tipDef_5.LDpinMode         = txRem2.ldPinMod;
    tipDef_4.outputPower       = txRem2.outPower;
    tipDef_2.chargePumpCurrent = txRem2.chargePampCurrent;
    tipDef_1.modulsValue       = txRem2.modValue;

    setAtt();   /* v2.0: аттенюатор следует за битами команды */
}

/* ========================================================================== */
/*  Новое в v2.0: расчёт частоты                                              */
/* ========================================================================== */

/* Код делителя для частоты. Границы — ровно по окну ГУН 2.2...4.4 ГГц:
 *   34.375...68.75   -> /64        550...1100  -> /4
 *   68.75...137.5    -> /32        1100...2200 -> /2
 *   137.5...275      -> /16        2200...4400 -> /1
 *   275...550        -> /8                                             */
static uint8_t max2870_DividerCode(float fMHz)
{
    if (fMHz <= 68.75f)  return MAX2870_DIV_64;
    if (fMHz <= 137.5f)  return MAX2870_DIV_32;
    if (fMHz <= 275.0f)  return MAX2870_DIV_16;
    if (fMHz <= 550.0f)  return MAX2870_DIV_8;
    if (fMHz <= 1100.0f) return MAX2870_DIV_4;
    if (fMHz <= 2200.0f) return MAX2870_DIV_2;
    return MAX2870_DIV_1;
}

/* Заполняет INT/FRAC (R0) и делитель (R4) под нужную частоту.
 * Возврат: 0 = OK, 1 = частота вне диапазона (регистры не тронуты).
 * После вызова — как обычно: shift(); spiAdf(); */
uint8_t max2870_SetFrequencyMHz(float fMHz)
{
    if (fMHz < MAX2870_F_MIN_MHZ || fMHz > MAX2870_F_MAX_MHZ)
        return 1;

    uint8_t  code    = max2870_DividerCode(fMHz);
    uint32_t divider = 1UL << (code & 0x07);          /* 000->1 ... 111->128 */

    float    vco = fMHz * (float)divider;
    float    n   = vco / MAX2870_F_PFD_MHZ;
    uint32_t i   = (uint32_t)n;
    uint32_t fr  = (uint32_t)((n - (float)i) * (float)MAX2870_MODULUS + 0.5f);

    if (fr >= MAX2870_MODULUS)   /* округление вверх через границу */
    {
        fr -= MAX2870_MODULUS;
        i  += 1U;
    }

    tipDef_0.integ           = i;
    tipDef_0.frac            = fr;
    tipDef_4.RFdividerSelect = code;
    return 0;
}

/* Какая частота стоит СЕЙЧАС (по теням регистров), МГц */
float max2870_ActualFrequencyMHz(void)
{
    uint32_t divider = 1UL << (tipDef_4.RFdividerSelect & 0x07);
    float vco = ((float)tipDef_0.integ
              + (float)tipDef_0.frac / (float)MAX2870_MODULUS)
              * MAX2870_F_PFD_MHZ;
    return vco / (float)divider;
}

/* Захват ГУН: 1 = locked, 0 = нет, 0xFF = пин MUXOUT не заведён на МК */
uint8_t max2870_IsLocked(void)
{
#if defined(MAX2870_LD_GPIO_Port) && defined(MAX2870_LD_Pin)
    return (HAL_GPIO_ReadPin(MAX2870_LD_GPIO_Port, MAX2870_LD_Pin) == GPIO_PIN_SET)
           ? 1U : 0U;
#else
    return 0xFFU;
#endif
}
