#include "main.h"
#include "regEdit.h"

#define SIZEA 4

structR0 tipDef_0;
structR1 tipDef_1;
structR2 tipDef_2;
structR3 tipDef_3;
structR4 tipDef_4;
structR5 tipDef_5;
remote1 txRem1;
remote2 txRem2;

extern uint32_t regRx1;
extern uint32_t regRx2;
extern uint8_t pData0[4];
extern uint8_t pData1[4];
extern uint8_t pData2[4];
extern uint8_t pData3[4];
extern uint8_t pData4[4];
extern uint8_t pData5[4];
extern SPI_HandleTypeDef hspi1;

void spiAdf(void)
{
    LL_GPIO_ResetOutputPin(LE_1_GPIO_Port, LE_1_Pin);
    HAL_SPI_Transmit(&hspi1, pData5, SIZEA, 10);
    LL_GPIO_SetOutputPin(LE_1_GPIO_Port, LE_1_Pin);

    LL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin);
    LL_GPIO_ResetOutputPin(LE_1_GPIO_Port, LE_1_Pin);
    HAL_SPI_Transmit(&hspi1, pData4, SIZEA, 10);
    LL_GPIO_SetOutputPin(LE_1_GPIO_Port, LE_1_Pin);

    LL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin);
    LL_GPIO_ResetOutputPin(LE_1_GPIO_Port, LE_1_Pin);
    HAL_SPI_Transmit(&hspi1, pData3, SIZEA, 10);
    LL_GPIO_SetOutputPin(LE_1_GPIO_Port, LE_1_Pin);

    LL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin);
    LL_GPIO_ResetOutputPin(LE_1_GPIO_Port, LE_1_Pin);
    HAL_SPI_Transmit(&hspi1, pData2, SIZEA, 10);
    LL_GPIO_SetOutputPin(LE_1_GPIO_Port, LE_1_Pin);

    LL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin);
    LL_GPIO_ResetOutputPin(LE_1_GPIO_Port, LE_1_Pin);
    HAL_SPI_Transmit(&hspi1, pData1, SIZEA, 10);
    LL_GPIO_SetOutputPin(LE_1_GPIO_Port, LE_1_Pin);

    LL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin);
    LL_GPIO_ResetOutputPin(LE_1_GPIO_Port, LE_1_Pin);
    HAL_SPI_Transmit(&hspi1, pData0, SIZEA, 10);
    LL_GPIO_SetOutputPin(LE_1_GPIO_Port, LE_1_Pin);
}

void defoultSet(void)
{
    tipDef_0.adres = 0;
    tipDef_0.frac = 0;
    tipDef_0.integ = 128;
    tipDef_0.reserved = 0;

    tipDef_1.adres = 0b001;
    tipDef_1.modulsValue = 1000;
    tipDef_1.phaseValue = 1;
    tipDef_1.prescaler = 1;
    tipDef_1.fazeAdj = 0;
    tipDef_1.reserved = 0;

    tipDef_2.adres = 0b010;
    tipDef_2.counterReset = 0;
    tipDef_2.CP_freeState = 0;
    tipDef_2.powerDown = 0;
    tipDef_2.PD_polarity = 1;
    tipDef_2.ldp = 0;
    tipDef_2.ldf = 0;
    tipDef_2.chargePumpCurrent = 0b0001;
    tipDef_2.doubleBufferR4 = 0;
    tipDef_2.R_counter = 1;
    tipDef_2.Rdiv2 = 0;
    tipDef_2.referenceDoubler = 0;
    tipDef_2.muxOut = 0;
    tipDef_2.noiseMode = 0b00;
    tipDef_2.reserved = 0;

    tipDef_3.adres = 0b011;
    tipDef_3.clockDividerValue = 0;
    tipDef_3.CLK_divMode = 0b01;
    tipDef_3.reserved3 = 0;
    tipDef_3.CSR = 0;
    tipDef_3.reserved2 = 0;
    tipDef_3.chargeCancel = 0;
    tipDef_3.ABP = 0;
    tipDef_3.bandSelect = 0;
    tipDef_3.reserved = 0;

    tipDef_4.adres = 0b100;
    tipDef_4.outputPower = 0b00;
    tipDef_4.RFoutputEnable = 1;
    tipDef_4.AUXoutputPower = 0;
    tipDef_4.AUXoutputEnable = 0;
    tipDef_4.AUXoutputSelect = 0;
    tipDef_4.MTLD = 0;
    tipDef_4.VCOpowerDown = 0;
    tipDef_4.bandSelectclockDividerValue = 200;
    tipDef_4.RFdividerSelect = 0b110;
    tipDef_4.FBselect = 1;
    tipDef_4.reserved = 0;

    tipDef_5.adres = 0b101;
    tipDef_5.reserved4 = 0;
    tipDef_5.reserved3 = 0b11;
    tipDef_5.reserved2 = 0;
    tipDef_5.LDpinMode = 0b11;
    tipDef_5.reserved = 0;

    txRem1.intValue = 125;
    txRem1.frac_Value = 0;
    txRem1.muxOut = 0;
    txRem1.reserved = 0;

    txRem2.modValue = 1000;
    txRem2.chargePampCurrent = 0b0010;
    txRem2.outPower = 0b01;
    txRem2.rfDivider = 0b010;
    txRem2.ldPinMod = 0b11;
    txRem2.attenuator1 = 0;
    txRem2.attenuator2 = 0;
    txRem2.attenuator3 = 0;
    txRem2.reserved = 0;
}

uint32_t initReg0(void)
{
    return (tipDef_0.adres << 0) | (tipDef_0.frac << 3) | (tipDef_0.integ << 15) | (tipDef_0.reserved << 31);
}

uint32_t initReg1(void)
{
    return (tipDef_1.adres << 0) | (tipDef_1.modulsValue << 3) | (tipDef_1.phaseValue << 15)
           | (tipDef_1.prescaler << 27) | (tipDef_1.fazeAdj << 28) | (tipDef_1.reserved << 29);
}

uint32_t initReg2(void)
{
    return (tipDef_2.adres << 0) | (tipDef_2.counterReset << 3) | (tipDef_2.CP_freeState << 4)
           | (tipDef_2.powerDown << 5) | (tipDef_2.PD_polarity << 6) | (tipDef_2.ldp << 7) | (tipDef_2.ldf << 8)
           | (tipDef_2.chargePumpCurrent << 9) | (tipDef_2.doubleBufferR4 << 13) | (tipDef_2.R_counter << 14)
           | (tipDef_2.Rdiv2 << 24) | (tipDef_2.referenceDoubler << 25) | (tipDef_2.muxOut << 26)
           | (tipDef_2.noiseMode << 29) | (tipDef_2.reserved << 31);
}

uint32_t initReg3(void)
{
    return (tipDef_3.adres << 0) | (tipDef_3.clockDividerValue << 3) | (tipDef_3.CLK_divMode << 15)
           | (tipDef_3.reserved3 << 17) | (tipDef_3.CSR << 18) | (tipDef_3.reserved2 << 19)
           | (tipDef_3.chargeCancel << 21) | (tipDef_3.ABP << 22) | (tipDef_3.bandSelect << 23)
           | (tipDef_3.reserved << 24);
}

uint32_t initReg4(void)
{
    return (tipDef_4.adres << 0) | (tipDef_4.outputPower << 3) | (tipDef_4.RFoutputEnable << 5)
           | (tipDef_4.AUXoutputPower << 6) | (tipDef_4.AUXoutputEnable << 8) | (tipDef_4.AUXoutputSelect << 9)
           | (tipDef_4.MTLD << 10) | (tipDef_4.VCOpowerDown << 11) | (tipDef_4.bandSelectclockDividerValue << 12)
           | (tipDef_4.RFdividerSelect << 20) | (tipDef_4.FBselect << 23) | (tipDef_4.reserved << 24);
}

uint32_t initReg5(void)
{
    return (tipDef_5.adres << 0) | (tipDef_5.reserved4 << 3) | (tipDef_5.reserved3 << 19)
           | (tipDef_5.reserved2 << 21) | (tipDef_5.LDpinMode << 22) | (tipDef_5.reserved << 24);
}

uint32_t remoteInit1(void)
{
    return (txRem1.reserved << 0) | (txRem1.muxOut << 1) | (txRem1.frac_Value << 4) | (txRem1.intValue << 16);
}

uint32_t remoteInit2(void)
{
    return (txRem2.reserved << 0) | (txRem2.attenuator3 << 6) | (txRem2.attenuator2 << 7)
           | (txRem2.attenuator1 << 8) | (txRem2.rfDivider << 9) | (txRem2.ldPinMod << 12)
           | (txRem2.outPower << 14) | (txRem2.chargePampCurrent << 16) | (txRem2.modValue << 20);
}

void bitOperation(void)
{
    txRem1.reserved = 1 & (regRx1 >> 0);
    txRem1.muxOut = 7 & (regRx1 >> 1);
    txRem1.frac_Value = 0xFFF & (regRx1 >> 4);
    txRem1.intValue = 0xFFFF & (regRx1 >> 16);

    txRem2.modValue = 0xFFF & (regRx2 >> 20);
    txRem2.chargePampCurrent = 0xF & (regRx2 >> 16);
    txRem2.outPower = 0x3 & (regRx2 >> 14);
    txRem2.ldPinMod = 0x3 & (regRx2 >> 12);
    txRem2.rfDivider = 0x7 & (regRx2 >> 9);
    txRem2.attenuator1 = 0x1 & (regRx2 >> 8);
    txRem2.attenuator2 = 0x1 & (regRx2 >> 7);
    txRem2.attenuator3 = 0x1 & (regRx2 >> 6);
    txRem2.reserved = 0x3F & (regRx2 >> 0);
}

void setRegistr(void)
{
    tipDef_0.integ = txRem1.intValue;
    tipDef_0.frac = txRem1.frac_Value;
    tipDef_2.muxOut = txRem1.muxOut;

    tipDef_4.RFdividerSelect = txRem2.rfDivider;
    tipDef_5.LDpinMode = txRem2.ldPinMod;
    tipDef_4.outputPower = txRem2.outPower;
    tipDef_2.chargePumpCurrent = txRem2.chargePampCurrent;
    tipDef_1.modulsValue = txRem2.modValue;
}

// ============================================================
// fricvancy() УДАЛЕНА ИЗ ЭТОГО ФАЙЛА
// Она осталась в main.c, как было изначально
// ============================================================
