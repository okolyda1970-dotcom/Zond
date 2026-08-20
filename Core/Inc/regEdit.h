/*
 * regEdit.h
 *
 *  Created on: Sep 26, 2024
 *      Author: kolyada
 */

#ifndef INC_REGEDIT_H_
#define INC_REGEDIT_H_

#include "main.h"

// === ПРОТОТИПЫ ФУНКЦИЙ ===
void defoultSet(void);
void spiAdf(void);
uint32_t initReg0(void);
uint32_t initReg1(void);
uint32_t initReg2(void);
uint32_t initReg3(void);
uint32_t initReg4(void);
uint32_t initReg5(void);
uint32_t remoteInit1(void);
uint32_t remoteInit2(void);
void setRegistr(void);
void bitOperation(void);

// === СТРУКТУРЫ РЕГИСТРОВ MAX2870 ===

typedef struct __structR0
{
    uint8_t reserved : 1;
    uint16_t integ : 16;
    uint16_t frac : 12;
    uint8_t adres : 3;
} structR0;

typedef struct __structR1
{
    uint8_t reserved : 4;
    uint8_t fazeAdj : 1;
    uint8_t prescaler : 1;
    uint16_t phaseValue : 12;
    uint16_t modulsValue : 12;
    uint8_t adres : 3;
} structR1;

typedef struct __structR2
{
    uint8_t reserved : 1;
    uint8_t noiseMode : 2;
    uint8_t muxOut : 3;
    uint8_t referenceDoubler : 1;
    uint8_t Rdiv2 : 1;
    uint16_t R_counter : 10;
    uint8_t doubleBufferR4 : 1;
    uint8_t chargePumpCurrent : 4;
    uint8_t ldf : 1;
    uint8_t ldp : 1;
    uint8_t PD_polarity : 1;
    uint8_t powerDown : 1;
    uint8_t CP_freeState : 1;
    uint8_t counterReset : 1;
    uint8_t adres : 3;
} structR2;

typedef struct __structR3
{
    uint16_t reserved : 8;
    uint8_t bandSelect : 1;
    uint8_t ABP : 1;
    uint8_t chargeCancel : 1;
    uint8_t reserved2 : 1;
    uint8_t CSR : 1;
    uint8_t reserved3 : 1;
    uint8_t CLK_divMode : 2;
    uint16_t clockDividerValue : 12;
    uint8_t adres : 3;
} structR3;

typedef struct __structR4
{
    uint8_t reserved : 8;
    uint8_t FBselect : 1;
    uint8_t RFdividerSelect : 3;
    uint8_t bandSelectclockDividerValue : 8;
    uint8_t VCOpowerDown : 1;
    uint8_t MTLD : 1;
    uint8_t AUXoutputSelect : 1;
    uint8_t AUXoutputEnable : 1;
    uint8_t AUXoutputPower : 2;
    uint8_t RFoutputEnable : 1;
    uint8_t outputPower : 2;
    uint8_t adres : 3;
} structR4;

typedef struct __structR5
{
    uint8_t reserved : 8;
    uint8_t LDpinMode : 2;
    uint8_t reserved2 : 1;
    uint8_t reserved3 : 2;
    uint16_t reserved4 : 16;
    uint8_t adres : 3;
} structR5;

// === СТРУКТУРЫ ДЛЯ ОБМЕНА ПО UART ===

typedef struct __remote1
{
    uint8_t reserved : 1;
    uint8_t muxOut : 3;
    uint16_t frac_Value : 12;
    uint16_t intValue : 16;
} remote1;

typedef struct __remote2
{
    uint16_t reserved : 6;
    uint8_t attenuator3 : 1;
    uint8_t attenuator2 : 1;
    uint8_t attenuator1 : 1;
    uint8_t rfDivider : 3;
    uint8_t ldPinMod : 2;
    uint8_t outPower : 2;
    uint8_t chargePampCurrent : 4;
    uint16_t modValue : 12;
} remote2;

// === ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ ===
extern structR0 tipDef_0;
extern structR1 tipDef_1;
extern structR2 tipDef_2;
extern structR3 tipDef_3;
extern structR4 tipDef_4;
extern structR5 tipDef_5;
extern remote1 txRem1;
extern remote2 txRem2;
extern FlagStatus flagErrorFwc;   // <-- ДЛЯ ПРОВЕРКИ ОШИБОК

#endif /* INC_REGEDIT_H_ */
