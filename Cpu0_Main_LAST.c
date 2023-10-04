/**********************************************************************************************************************
 * \file Cpu0_Main.c
 * \copyright Copyright (C) Infineon Technologies AG 2019
 * 
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of 
 * business you are acting and (ii) Infineon Technologies AG or its licensees. If and as long as no such terms of use
 * are agreed, use of this file is subject to following:
 * 
 * Boost Software License - Version 1.0 - August 17th, 2003
 * 
 * Permission is hereby granted, free of charge, to any person or organization obtaining a copy of the software and 
 * accompanying documentation covered by this license (the "Software") to use, reproduce, display, distribute, execute,
 * and transmit the Software, and to prepare derivative works of the Software, and to permit third-parties to whom the
 * Software is furnished to do so, all subject to the following:
 * 
 * The copyright notices in the Software and this entire statement, including the above license grant, this restriction
 * and the following disclaimer, must be included in all copies of the Software, in whole or in part, and all 
 * derivative works of the Software, unless such copies or derivative works are solely in the form of 
 * machine-executable object code generated by a source language processor.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE 
 * COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN 
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
 * IN THE SOFTWARE.
 *********************************************************************************************************************/
#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"

#define PORT00_BASE     (0xF003A000)
#define PORT00_OMR      (*(volatile unsigned int*)(PORT00_BASE + 0x04))
#define PORT00_IOCR4    (*(volatile unsigned int*)(PORT00_BASE + 0x14))
#define PORT00_IN       (*(volatile unsigned int*)(PORT00_BASE + 0x24))


// FOR RGB LED control
#define PORT02_BASE     (0xF003A200)
#define PORT02_IOCR4    (*(volatile unsigned int*)(PORT02_BASE + 0x14))
#define PORT02_OMR      (*(volatile unsigned int*)(PORT02_BASE + 0x04))
#define PORT02_IN       (*(volatile unsigned int*)(PORT02_BASE + 0x24))
/* volatile 타입으로 선언하는 이유는 동일한 메모리에 1/0을 반복적으로 쓰기를 하므로 컴파일러가 동일한 동작을 무의미하게 반복한다고 판단하여 '최적화'를 실행할 수 있으므로 이를 방지하기 위하여 'volatile' 형으로 선언함 */

// FOR LED
#define PORT10_BASE     (0xF003B000)
#define PORT10_IOCR0    (*(volatile unsigned int*)(PORT10_BASE + 0x10))     /* PORT10_IOCR0의 주소를 이용한 주소의 내용 (변수 아님) */
#define PORT10_OMR      (*(volatile unsigned int*)(PORT10_BASE + 0x04))     /* PORT10_OMR의 주소를 이용한 주소의 내용 (변수 아님)   */
#define PORT10_IOCR4    (*(volatile unsigned int*)(PORT10_BASE + 0x14))
#define PORT10_IN       (*(volatile unsigned int*)(PORT10_BASE + 0x24))

#define P3              3
#define P4              4
#define P6              6
#define P7              7
#define PC0      3
#define PC1             11
#define PC2             19
#define PC3             27
#define PC4             3
#define PC5             11
#define PC6             19
#define PC7             27
#define PS1    1
#define PS2    2
#define PS3             3
#define PS4             4
#define PS5             5
#define PS6             6
#define PS7             7
#define PCL1   17
#define PCL2   18
#define PCL3            19
#define PCL4            20
#define PCL5            21
#define PCL6            22
#define PCL7            23

#define P10_R0_PC3             27
#define P10_R4_PC5             11
#define P02_R4_PC7             27
#define P10_R0_PCL3            19
#define P10_R4_PCL5            21
#define P02_R4_PCL7            23
#define P10_R0_PS3             3
#define P10_R4_PS5             5
#define P02_R4_PS7             7


// FOR SWITCH
#define PORT02_IOCR0    (*(volatile unsigned int*)(PORT02_BASE + 0x10))
#define PORT02_IN       (*(volatile unsigned int*)(PORT02_BASE + 0x24))

#define P02_R0_PC0             3
#define P02_R0_PC1             11
#define P0                     0
#define P1                     1


/* SCU Registers */
#define SCU_BASE            (0xF0036000)
#define SCU_WDT_CPU0CON0    (*(volatile unsigned int*)(SCU_BASE + 0x100))
#define SCU_WDTSCON0  (*(volatile unsigned int*)(SCU_BASE + 0x0F0))
#define SCU_EICR0           (*(volatile unsigned int*)(SCU_BASE + 0x210))
#define SCU_EICR1     (*(volatile unsigned int*)(SCU_BASE + 0x214))// ERS2, 3
#define SCU_IGCR0     (*(volatile unsigned int*)(SCU_BASE + 0x22C))

#define LCK                 1
#define ENDINIT             0
#define INP0          12
#define INP1          28
#define EIEN0         11
#define EIEN1         27
#define FEN0          8
#define REN0          9
#define FEN1          24
#define EXIS0         4
#define EXIS1         20
#define IGP0          14
#define IGP1          30

/* SRC Registers */
#define SRC_BASE            (0xF0038000)
#define SRC_CCU60_SR0        (*(volatile unsigned int*)(SRC_BASE + 0x420))
#define SRC_SCUERU0  (*(volatile unsigned int*)(SRC_BASE + 0xCD4))
#define SRC_SCUERU1  (*(volatile unsigned int*)(SRC_BASE + 0xCD8))

#define TOS                 11
#define SRE                 10
#define SRPN                0

/* volatile 타입으로 선언하는 이유는 동일한 메모리에 1/0을 반복적으로 쓰기를 하므로 컴파일러가 동일한 동작을 무의미하게 반복한다고 판단하여 '최적화'를 실행할 수 있으므로 이를 방지하기 위하여 'volatile' 형으로 선언함 */

#define P10_R0_PC1             11
#define P10_R0_PC2             19
#define P10_R0_PCL1            17
#define P10_R0_PCL2            18
#define P10_R0_PS1             1
#define P10_R0_PS2             2

// GTM
#define GTM_BASE                    (0xF0100000)

#define GTM_CLC                     (*(volatile unsigned int*)(GTM_BASE + 0x9FD00))
#define GTM_TOUTSEL0                (*(volatile unsigned int*)(GTM_BASE + 0x9FD30))
#define DISS                        1
#define DISR                        0
#define SEL3                        6

//GTM CMU 설정
#define GTM_CMU_FXCLK_CTRL          (*(volatile unsigned int*)(GTM_BASE + 0x00344))
#define FXCLK_SEL                   0
#define GTM_CMU_CLK_EN              (*(volatile unsigned int*)(GTM_BASE + 0x00300))
#define EN_FXCLK                    22


// GTM - TOM0 - TGC0 설정
#define GTM_TOM0_TGC1_GLB_CTRL      (*(volatile unsigned int*)(GTM_BASE + 0x08230))
#define GTM_TOM0_TGC1_ENDIS_CTRL    (*(volatile unsigned int*)(GTM_BASE + 0x08270))
#define GTM_TOM0_TGC1_OUTEN_CTRL    (*(volatile unsigned int*)(GTM_BASE + 0x08278))
#define GTM_TOM0_TGC1_FUPD_CTRL     (*(volatile unsigned int*)(GTM_BASE + 0x08238))
#define GTM_TOM0_CH11_CTRL          (*(volatile unsigned int*)(GTM_BASE + 0x082C0))
#define GTM_TOM0_CH11_SR0           (*(volatile unsigned int*)(GTM_BASE + 0x082C4))
#define GTM_TOM0_CH11_SR1           (*(volatile unsigned int*)(GTM_BASE + 0x082C8))

#define GTM_TOM0_TGC0_GLB_CTRL      (*(volatile unsigned int*)(GTM_BASE + 0x08030))
#define HOST_TRIG                   0
#define UPEN_CTRL1                  18
#define UPEN_CTRL2                  20
#define GTM_TOM0_TGC0_FUPD_CTRL     (*(volatile unsigned int*)(GTM_BASE + 0x08038))
#define RSTCN0_CH1                  18
#define FUPD_CTRL1                  2
#define RSTCN0_CH2                  20
#define FUPD_CTRL2                  4
#define GTM_TOM0_TGC0_ENDIS_CTRL    (*(volatile unsigned int*)(GTM_BASE + 0x08070))
#define ENDIS_CTRL1                 2
#define ENDIS_CTRL2                 4
#define GTM_TOM0_TGC0_OUTEN_CTRL    (*(volatile unsigned int*)(GTM_BASE + 0x08078))
#define OUTEN_CTRL1                 2
#define OUTEN_CTRL2                 4


// GTM - TOM0 - Channel 1 설정
#define GTM_TOM0_CH1_CTRL           (*(volatile unsigned int*)(GTM_BASE + 0x08040))
#define SL                          11
#define CLK_SRC_SR                  12
#define GTM_TOM0_CH1_SR0            (*(volatile unsigned int*)(GTM_BASE + 0x08044))
#define GTM_TOM0_CH1_SR1            (*(volatile unsigned int*)(GTM_BASE + 0x08048))

// GTM - TOM0 - Channel 2 설정
#define GTM_TOM0_CH2_CTRL           (*(volatile unsigned int*)(GTM_BASE + 0x08080))
#define GTM_TOM0_CH2_SR0            (*(volatile unsigned int*)(GTM_BASE + 0x08084))
#define GTM_TOM0_CH2_SR1            (*(volatile unsigned int*)(GTM_BASE + 0x08088))

// TOUT 설정
#define GTM_TOUTSEL6                (*(volatile unsigned int*)(GTM_BASE + 0x9FD48))
#define SEL7                        14
#define SEL8                        16

#define UPEN_CTRL3                  22
#define ENDIS_CTRL3                 6
#define OUTEN_CTRL3                 6
#define RSTCN0_CH3                  22
#define FUPD_CTRL3                  6


/* VADC Registers */
#define VADC_BASE       (0xF0020000)
#define VADC_CLC        (*(volatile unsigned int*)(VADC_BASE + 0x000))                  // CLC register 설정 enable(DISR) 및 enable 확인(DISS)
#define DISS            1
#define DISR            0
#define VADC_G4ARBPR    (*(volatile unsigned int*)(VADC_BASE + 0x1484))                 // G4ARBPR register 설정
#define PRIO0           0
#define CSM0            3
#define ASEN0           24
#define VADC_G4QMR0     (*(volatile unsigned int*)(VADC_BASE + 0x1504))
#define ENGT            0
#define TREV            9
#define FLUSH           10
#define VADC_G4ARBCFG   (*(volatile unsigned int*)(VADC_BASE + 0x1480))
#define ANONC           0
#define VADC_G4ICLASS0  (*(volatile unsigned int*)(VADC_BASE + 0x14A0))
#define STCS            0
#define CMS             8
#define VADC_G4CHCTR7   (*(volatile unsigned int*)(VADC_BASE + 0x161C))
#define ICLSEL          0
#define RESREG          16
#define RESPOS          21
#define VADC_G4QINR0    (*(volatile unsigned int*)(VADC_BASE + 0x1510))
#define REQCHNR         0
#define RF              5
#define VADC_G4RES1     (*(volatile unsigned int*)(VADC_BASE + 0x1704))
#define RESULT          0
#define VF              31

/* CCU60 Registers */
#define CCU60_BASE          (0xF0002A00)
#define CCU60_CLC           (*(volatile unsigned int*)(CCU60_BASE + 0x00))    // CCU60 모듈을 enable하기 위한 레지스터(CCU60_CLC)
#define CCU60_T12           (*(volatile unsigned int*)(CCU60_BASE + 0x20))    // T12 counter register를 통해 현재 카운트 값을 읽음.  counter를 사용하기 위해서 초기화(reset) 필요. 리셋값 0x00
#define CCU60_T12PR         (*(volatile unsigned int*)(CCU60_BASE + 0x24))    // T12 Period register에 period value값을 계산하여서 저장
#define CCU60_TCTR0         (*(volatile unsigned int*)(CCU60_BASE + 0x70))    //
#define CCU60_TCTR2         (*(volatile unsigned int*)(CCU60_BASE + 0x74))
#define CCU60_TCTR4         (*(volatile unsigned int*)(CCU60_BASE + 0x78))
#define CCU60_INP           (*(volatile unsigned int*)(CCU60_BASE + 0xAC))
#define CCU60_IEN           (*(volatile unsigned int*)(CCU60_BASE + 0xB0))

#define CCU61_BASE          (0xF0002B00)
#define CCU61_CLC           (*(volatile unsigned int*)(CCU61_BASE + 0x00))
#define CCU61_T12           (*(volatile unsigned int*)(CCU61_BASE + 0x20))
#define CCU61_T12PR         (*(volatile unsigned int*)(CCU61_BASE + 0x24))
#define CCU61_TCTR0         (*(volatile unsigned int*)(CCU61_BASE + 0x70))
#define CCU61_TCTR2         (*(volatile unsigned int*)(CCU61_BASE + 0x74))
#define CCU61_TCTR4         (*(volatile unsigned int*)(CCU61_BASE + 0x78))
#define CCU61_INP           (*(volatile unsigned int*)(CCU61_BASE + 0xAC))
#define CCU61_IEN           (*(volatile unsigned int*)(CCU61_BASE + 0xB0))
#define DISS                1
#define DISR                0
#define CTM                 7
#define T12PRE              3
#define T12CLK              0
#define T12STR              6
#define T12RS               1
#define INPT12              10
#define ENT12PM             7
#define T12SSC              0
#define T12RR               0
#define T12RS               1
#define T12RES              2

IfxCpu_syncEvent g_cpuSyncEvent = 0;

void init_GTM_TOM0_PWM(void);
void init_LED(void);
void unlock_Safety_Critical_Reg(void);
void lock_Safety_Critical_Reg(void);
void init_VADC(void);
void VADC_startConversion(void);
unsigned int VADC_readResult(void);
void init_Switch(void);
void init_RGB(void);
void init_CCU60(void);
void init_Buzzer(void);
void init_ERU(void);
void usonicTrigger(void);
void initCCU61(void);
void initUSonic(void);
#define PWM_FREQ        6250000

unsigned int rotation = 0;
unsigned int count = 0;
int num_tones = 12;
int tones[] = {261, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494};

volatile uint32 range;
volatile uint8 range_valid_flag = 0;

int core0_main(void)
{
    IfxCpu_enableInterrupts();
    
    /* !!WATCHDOG0 AND SAFETY WATCHDOG ARE DISABLED HERE!!
     * Enable the watchdogs and service them periodically if it is required
     */
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());
    
    /* Wait for CPU sync event */
    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 1);

    unsigned int adcResult;

    /* Initialization */
    init_LED();
    init_GTM_TOM0_PWM();
    init_RGB();
    init_Switch();
    init_CCU60();
    init_Buzzer();
    init_ERU();

    init_VADC();                // Initialize VADC


    while(1)
    {
        VADC_startConversion();
        adcResult = VADC_readResult();

        GTM_TOM0_CH1_SR1 = 12500*(adcResult+1)/4096 - 1;       // ADC 출력값이 0~4095이므로 이값으로 그대로 연산하면 0이 곱해지며, 이때 -1항과의 연산에서 음수가 발생하므로, ADC 출력값을 0~4095 >> 1~4096으로 변경해서 연산함
        GTM_TOM0_CH2_SR1 = 12500*(adcResult+1)/4096 - 1;

        if((PORT02_IN & (1<<P1)) == 0) // 스위치 출렀을 때 low (스위치가 pull up 방식이어서...) 가되고  켜짐
        {
            PORT10_IOCR0 &= ~((0x1F) << P10_R0_PC1);
            PORT10_IOCR0 |= ((0x10) << P10_R0_PC1);
            PORT10_OMR |= (1<<P10_R0_PCL1);
        }
        else
        {
            PORT10_IOCR0 &= ~((0x1F) << P10_R0_PC1);
            PORT10_IOCR0 |= ((0x11) << P10_R0_PC1);
        }

    }

    return (1);
}

/*LED*/
__interrupt( 0x0A ) __vector_table( 0 )
void CCU60_T12_ISR(void)
{
    if(count == 0){
        PORT10_OMR |= ((1<<P10_R0_PCL3)|(1<<P10_R0_PS3));
    }
    else if(count == 1){
        PORT10_OMR |= ((1<<P10_R4_PCL5)|(1<<P10_R4_PS5));
    }
    else
    {
        PORT02_OMR |= ((1<<P02_R4_PCL7)|(1<<P02_R4_PS7));
        count = 0;
    }
    count++;
}

__interrupt(0x0B) __vector_table(0)
void ERU_ISR_SW2(void)
{
    PORT10_OMR |= ((1<<PCL1) | (1<<PS1));

    for(int i = 0 ; i < num_tones ; i++)
    {
        int pwm_cnt = PWM_FREQ / tones[i];

        GTM_TOM0_CH11_SR0 = pwm_cnt;
        GTM_TOM0_CH11_SR1 = pwm_cnt / 2;

        for(int j = 0 ; j < 1200000 ; j++) ;
    }

//   for(int j = 0 ; j < 5000000 ; j++) ;
    GTM_TOM0_CH11_SR1 = 0;

}


void init_CCU60(void)
{
    /* CCU60 T12 Setting */

    unlock_Safety_Critical_Reg();

    CCU60_CLC &= ~(1 << DISR);                     // Enable CCU60 Module

    lock_Safety_Critical_Reg();

    while((CCU60_CLC & (1 << DISS)) != 0);       // Wait until module is enabled

    CCU60_TCTR0 &= ~((0x7) << T12CLK);           // f_T12 = f_CCU6 / prescaler
    CCU60_TCTR0 |= ((0x3) << T12CLK);            // F_CCU6 = 100 MHz, prescaler = 2048
    CCU60_TCTR0 |= (1 << T12PRE);                // f_T12 = 48828 Hz

    CCU60_TCTR0 &= ~(1 << CTM);                  // T12 always counts up and continues counting
                                                 // from zero after reaching the period value

    CCU60_T12PR = 24414 - 1;                     // Interrupt freq. = f_T12 / (period value  + 1)
    CCU60_TCTR4 |= (1 << T12STR);                // Interrupt freq. = 2 Hz

    CCU60_T12 = 0;                               // Clear T12 counting value

    /* CCU60 T12 Interrupt Setting */
    CCU60_INP &= ~((0x3) << INPT12);             // Service Request output SR0 is selected

    CCU60_IEN |= (1 << ENT12PM);                 // Enable Interrupt for T12 Period-Match

    /* SRC Interrupt Setting For CCU60 */
    SRC_CCU60_SR0 &= ~((0xFF) << SRPN);           // Set Priority : 0x0A
    SRC_CCU60_SR0 |= ((0x0A) << SRPN);

    SRC_CCU60_SR0 &= ~((0x3) << TOS);             // CPU0 services

    SRC_CCU60_SR0 |= (1 << SRE);                  // Service Request is enabled

    /* CCU60 T12 Start */
    CCU60_TCTR4 = (1 << T12RS);                  // T12 starts counting
}


/* Initialize LED (RED & BLUE) */
void init_LED(void)
{
    /* reset PC1 & PC2 in IOCR0 */
    PORT10_IOCR0 &= ~((0x1F) << P10_R0_PC1);
    PORT10_IOCR0 &= ~((0x1F) << P10_R0_PC2);

    /* set PC1 & PC2 with push-pull (2b10000) */
    PORT10_IOCR0 |= ((0x11) << P10_R0_PC1);           // TOUT3 --> PC1 : 10011b로 설정 (3 포트)   red    PWM OUTPUT을 입력으로 받아서 LED 켜짐
    PORT10_IOCR0 |= ((0x11) << P10_R0_PC2);           // TOUT4 --> PC1 : 10100b로 설정 (4 포트)   blue   PWM OUTPUT을 입력으로 받아서 LED 켜짐
}

void init_RGB(void){
    /* reset PC3 in IOCR0, PC5,PC7 in IOCR4 */
    PORT10_IOCR0 &= ~((0x1F) << P10_R0_PC3);
    PORT10_IOCR4 &= ~((0x1F) << P10_R4_PC5);
    PORT02_IOCR4 &= ~((0x1F) << P02_R4_PC7);

    /* set PC3 in IOCR0, 5, 7 in IOCR4 with push-pull (2b10000) */
    PORT10_IOCR0 |= ((0x10) << P10_R0_PC3);
    PORT10_IOCR4 |= ((0x10) << P10_R4_PC5);
    PORT02_IOCR4 |= ((0x10) << P02_R4_PC7);
}

void init_Switch(void){
    // reset PC0, PC1 in IOCR0
    PORT02_IOCR0 &= ~((0X1F) << P02_R0_PC0);
    PORT02_IOCR0 &= ~((0X1F) << P02_R0_PC1);
    // set PC0, PC1 with push-pull(2b0xx10)
    PORT02_IOCR0 |= ((0x2) << P02_R0_PC0);
    PORT02_IOCR0 |= ((0x2) << P02_R0_PC1);
}

void init_Buzzer(void)
{
    PORT02_IOCR0 &= ~((0x1F) << PC3);           // PORT02.3 : Alternate output function 1 (push-pull)
    PORT02_IOCR0 |= ((0x11) << PC3);            // PORT02.3 : GTM_TOUT3
}


void init_VADC(void){
    /* VADC Enable */
    unlock_Safety_Critical_Reg();
    VADC_CLC &= ~(1 << DISR);                 // Enable VADC Module, DISR = 0
    lock_Safety_Critical_Reg();
    while((VADC_CLC & (1 << DISS)) != 0);     // Wait until module is enabled, DISS가 0 인지 재확인

    VADC_G4ARBPR |= ((0x3) << PRIO0);         // Highest Priority for Request Source 0,  011b를 입력
    VADC_G4ARBPR &= ~(1 << CSM0);             // Conversion Start Mode : Wait-for-start mode
    VADC_G4ARBPR |= (1 << ASEN0);             // Arbitration Source Input 0 Enable  1 입력

    VADC_G4QMR0  &= ~((0x3) << ENGT);         // Enable Conversion Requests
    VADC_G4QMR0  |= ((0x1) << ENGT);

    VADC_G4QMR0  |= (1 << FLUSH);             // Clear all Queue Entries

    VADC_G4ARBCFG |= ((0x3) << ANONC);        // Analog Converter : Normal Operation

    VADC_G4ICLASS0 &= ~((0x7) << CMS);        // Group-specific Class 0
                                              // Conversion Mode : Standard Conversion (12-bit)

    /* VADC Group 4 Channel 7 Setting */
    VADC_G4CHCTR7 |= (1 << RESPOS);           // Read Results Right-aligned
    VADC_G4CHCTR7 &= ~((0xF) << RESREG);      // Store Result in Group Result Register G4RES1
    VADC_G4CHCTR7 |= (1 << RESREG);
    VADC_G4CHCTR7 &= ~((0x3) << ICLSEL);      // Use Group-specific Class 0
}

void VADC_startConversion(void){
    /* No fill and Start Queue */
    VADC_G4QINR0 &= ~(0x1F);
    VADC_G4QINR0 |= (0x07);                  // Request Channel Number : 7

    VADC_G4QINR0 &= ~(1 << RF);              // No fill : it is converted once
//    VADC_G4QINR0 |= (1 << RF);               // automatic refill

    VADC_G4QMR0 |= (1 << TREV);              // Generate a Trigger Event
}

unsigned int VADC_readResult(void)
{
    unsigned int result;

    while((VADC_G4RES1 & (1 << VF)) == 0);          // Wait until New Result Available

    result = (VADC_G4RES1 & ((0xFFFF) << RESULT));  // Read Result

    return result;
}

void unlock_Safety_Critical_Reg(void)
{
    /* Password Access to unlock WDTSCON0 */
    SCU_WDT_CPU0CON0 = ((SCU_WDT_CPU0CON0 ^ 0xFC) & ~(1 << LCK)) | (1 << ENDINIT);
    while((SCU_WDT_CPU0CON0 & (1 << LCK)) != 0);

    /* Modify Access to clear ENDINIT bit */
    SCU_WDT_CPU0CON0 = ((SCU_WDT_CPU0CON0 ^ 0xFC) | (1 << LCK)) & ~ (1 << ENDINIT);
    while((SCU_WDT_CPU0CON0 & (1 << LCK)) == 0);
}

void lock_Safety_Critical_Reg(void)
{
    /* Password Access to unlock WDTSCON0 */
    SCU_WDT_CPU0CON0 = ((SCU_WDT_CPU0CON0 ^ 0xFC) & ~(1 << LCK)) | (1 << ENDINIT);
    while((SCU_WDT_CPU0CON0 & (1 << LCK)) != 0);

    /* Modify Access to clear ENDINIT bit */
    SCU_WDT_CPU0CON0 = ((SCU_WDT_CPU0CON0 ^ 0xFC) | (1 << LCK)) | (1 << ENDINIT);
    while((SCU_WDT_CPU0CON0 & (1 << LCK)) == 0);
}

/* Initialize External Request Unit (ERU) */

void init_ERU(void)
{
    SCU_WDTSCON0 = ((SCU_WDTSCON0 ^ 0xFC) & ~(1 << LCK)) | (1 << ENDINIT);
    while((SCU_WDTSCON0 & (1<< LCK)) !=0);

    // Modify Aceess to clear ENDINIT bit
    SCU_WDTSCON0 = ((SCU_WDTSCON0 ^ 0xFC) | (1 << LCK)) & ~ (1 << ENDINIT);
    while((SCU_WDTSCON0 &(1 << LCK))==0);

//SW1
    SCU_EICR1 &= ~((0x7) << EXIS0);         // External input 1 selected
    SCU_EICR1 |=  ((0x1) << EXIS0);

    SCU_EICR1 |= (1<< FEN0);                // Falling edge enable

    SCU_EICR1 |=((0x1) << EIEN0);           // The trigger event is enabled

    SCU_EICR1 &= ~((0x7) << INP0);          // An event from input ETL 2 triggers output OGU 0

    SCU_IGCR0 &= ~((0x3) << IGP0);          // IOUT(0) is activated in response to a trigger event
    SCU_IGCR0 |= ((0x1) << IGP0);           // The pattern is not considered

//SW2
    SCU_EICR1 &= ~((0x7) << EXIS1);         // External input 2 selected
    SCU_EICR1 |=  ((0x2) << EXIS1);

    SCU_EICR1 |= (1<< FEN1);                // Falling edge enable

    SCU_EICR1 |=((0x1) << EIEN1);           // The trigger event is enabled

    SCU_EICR1 &= ~((0x7) << INP1);          // An event from input ETL 2 triggers output OGU 1
    SCU_EICR1 |=((0x1) << INP1);           // The trigger event is enabled

    SCU_IGCR0 &= ~((0x3) << IGP1);          // IOUT(0) is activated in response to a trigger event
    SCU_IGCR0 |= ((0x1) << IGP1);           // The pattern is not considered


    SCU_WDTSCON0 = ((SCU_WDTSCON0 ^ 0xFC) & ~(1 << LCK)) | (1 << ENDINIT);
    while((SCU_WDTSCON0 & (1<< LCK)) !=0);

    // Modify Aceess to set ENDINIT bit
    SCU_WDTSCON0 = ((SCU_WDTSCON0 ^ 0xFC) | (1 << LCK)) | (1 << ENDINIT);
    while((SCU_WDTSCON0 &(1 << LCK))==0);

    SRC_SCUERU0 &= ~((0xFF)<<SRPN);          // Set Priority : 0x0A
    SRC_SCUERU0 |= ((0x0A)<<SRPN);

    SRC_SCUERU0 &= ~((0x3) << TOS);          // CPU0 Services

    SRC_SCUERU0 |= (1<< SRE);                // Service Request is enabled


    SRC_SCUERU1 &= ~((0xFF)<<SRPN);          // Set Priority : 0x0B
    SRC_SCUERU1 |= ((0x0B)<<SRPN);

    SRC_SCUERU1 &= ~((0x3) << TOS);          // CPU0 Services

    SRC_SCUERU1 |= (1<< SRE);                // Service Request is enabled

}

void init_GTM_TOM0_PWM(void)
{

    /* GTM Enable */
    unlock_Safety_Critical_Reg();
    GTM_CLC &= ~(1 << DISR);                        // Enable GTM Module
    lock_Safety_Critical_Reg();
    while((GTM_CLC & (1 << DISS)) != 0);            // Wait until module is enabled

    /* GTM Clock Setting */
    GTM_CMU_FXCLK_CTRL &= ~((0xF) << FXCLK_SEL);    // Input clock of CMU_FXCLK : select CMU_GCLK_EN (0000b)

    GTM_CMU_CLK_EN &= ~((0x3) << EN_FXCLK);         // Enable all CMU_FXCLK
    GTM_CMU_CLK_EN |= ((0x2) << EN_FXCLK);

    /* GTM TOM0 PWM Setting */
    GTM_TOM0_TGC0_GLB_CTRL &= ~((0x3) << UPEN_CTRL1);   // TOM0 channel 1 enable update of
    GTM_TOM0_TGC0_GLB_CTRL |= ((0x2) << UPEN_CTRL1);    // register CM0, CM1, CLK_SRC

    GTM_TOM0_TGC0_GLB_CTRL &= ~((0x3) << UPEN_CTRL2);   // TOM0 channel 2 enable update of
    GTM_TOM0_TGC0_GLB_CTRL |= ((0x2) << UPEN_CTRL2);    // register CM0, CM1, CLK_SRC

    GTM_TOM0_TGC0_FUPD_CTRL &= ~((0x3) << FUPD_CTRL1);  // Enable force update of TOM0 channel 1
    GTM_TOM0_TGC0_FUPD_CTRL |= ((0x2) << FUPD_CTRL1);

    GTM_TOM0_TGC0_FUPD_CTRL &= ~((0x3) << FUPD_CTRL2);  // Enable force update of TOM0 channel 1
    GTM_TOM0_TGC0_FUPD_CTRL |= ((0x2) << FUPD_CTRL2);

    GTM_TOM0_TGC0_FUPD_CTRL &= ~((0x3) << RSTCN0_CH1);  // Reset CN0 of TOM0 channel 1 on force update
    GTM_TOM0_TGC0_FUPD_CTRL |= ((0x2) << RSTCN0_CH1);

    GTM_TOM0_TGC0_FUPD_CTRL &= ~((0x3) << RSTCN0_CH2);  // Reset CN0 of TOM0 channel 1 on force update
    GTM_TOM0_TGC0_FUPD_CTRL |= ((0x2) << RSTCN0_CH2);

    GTM_TOM0_TGC0_ENDIS_CTRL &= ~((0x3) << ENDIS_CTRL1); // Enable channel 1 on an update trigger
    GTM_TOM0_TGC0_ENDIS_CTRL |= ((0x2) << ENDIS_CTRL1);
    GTM_TOM0_TGC0_OUTEN_CTRL &= ~((0x3) << OUTEN_CTRL1); // Enable channel 1 output on an update trigger
    GTM_TOM0_TGC0_OUTEN_CTRL |= ((0x2) << OUTEN_CTRL1);

    GTM_TOM0_TGC0_ENDIS_CTRL &= ~((0x3) << ENDIS_CTRL2); // Enable channel 1 on an update trigger
    GTM_TOM0_TGC0_ENDIS_CTRL |= ((0x2) << ENDIS_CTRL2);
    GTM_TOM0_TGC0_OUTEN_CTRL &= ~((0x3) << OUTEN_CTRL2); // Enable channel 1 output on an update trigger
    GTM_TOM0_TGC0_OUTEN_CTRL |= ((0x2) << OUTEN_CTRL2);

    /*buzzer*/
    GTM_TOM0_TGC1_GLB_CTRL &= ~((0x3) << UPEN_CTRL3);   // TOM0 channel 11 enable update of
    GTM_TOM0_TGC1_GLB_CTRL |= ((0x2) << UPEN_CTRL3);    // register CM0, CM1, CLK_SRC

    GTM_TOM0_TGC1_FUPD_CTRL &= ~((0x3) << FUPD_CTRL3);  // Enable force update of TOM0 channel 11
    GTM_TOM0_TGC1_FUPD_CTRL |= ((0x2) << FUPD_CTRL3);
    GTM_TOM0_TGC1_FUPD_CTRL &= ~((0x3) << RSTCN0_CH3);  // Reset CN0 of TOM0 channel 11 on force update
    GTM_TOM0_TGC1_FUPD_CTRL |= ((0x2) << RSTCN0_CH3);

    GTM_TOM0_TGC1_ENDIS_CTRL &= ~((0x3) << ENDIS_CTRL3); // Enable channel 11 on an update trigger
    GTM_TOM0_TGC1_ENDIS_CTRL |= ((0x2) << ENDIS_CTRL3);
    GTM_TOM0_TGC1_OUTEN_CTRL &= ~((0x3) << OUTEN_CTRL3); // Enable channel 11 output on an update trigger
    GTM_TOM0_TGC1_OUTEN_CTRL |= ((0x2) << OUTEN_CTRL3);

    GTM_TOM0_CH11_CTRL |= (1 << SL);                    // High signal level for duty cycle

    GTM_TOM0_CH11_CTRL &= ~((0x7) << CLK_SRC_SR);       // Clock source : CMU_FXCLK(1) = 6250 kHz
    GTM_TOM0_CH11_CTRL |= (1 << CLK_SRC_SR);
    GTM_TOM0_CH11_SR0 = 12500 - 1;                      // PWM freq. = 6250 kHz / 12500 = 500 Hz
    GTM_TOM0_CH11_SR1 = 0;                              // Duty cycle = 0

    GTM_TOUTSEL0 &= ~((0x3) << SEL3);                   // TOUT3 : TOM0 channel 11

    GTM_TOM0_TGC1_GLB_CTRL |= (1 << HOST_TRIG);
    /*end of buzzer*/

    GTM_TOM0_CH1_CTRL |= (1 << SL);                     // High signal level for duty cycle

    GTM_TOM0_CH1_CTRL &= ~((0x7) << CLK_SRC_SR);        // Clock source : CMU_FXCLK(1) = 6250 kHz
    GTM_TOM0_CH1_CTRL |= (1 << CLK_SRC_SR);

    GTM_TOM0_CH2_CTRL |= (1 << SL);                     // High signal level for duty cycle

    GTM_TOM0_CH2_CTRL &= ~((0x7) << CLK_SRC_SR);        // Clock source : CMU_FXCLK(1) = 6250 kHz
    GTM_TOM0_CH2_CTRL |= (1 << CLK_SRC_SR);

    GTM_TOM0_CH1_SR0 = 12500 - 1;                       // PWM freq. = 6250 kHz / 12500 = 500 Hz
    GTM_TOM0_CH2_SR0 = 12500 - 1;                       // PWM freq. = 6250 kHz / 12500 = 500 Hz
//    GTM_TOM0_CH1_SR1 = 0;                               // Duty cycle = 0
//    GTM_TOM0_CH1_SR1 = 3125-1;                               // Duty cycle = 25
//    GTM_TOM0_CH1_SR1 = 6250-1;                          // Duty cycle = 50
    GTM_TOM0_CH1_SR1 = 0;                       // Duty cycle = 100
    GTM_TOM0_CH2_SR1 = 0;                       // Duty cycle = 100

    GTM_TOUTSEL6 &= ~((0x3) << SEL7);                   // TOUT103 : TOM0 channel 1
    GTM_TOUTSEL6 &= ~((0x3) << SEL8);                   // TOUT103 : TOM0 channel 2


    GTM_TOM0_TGC0_GLB_CTRL |= (1 << HOST_TRIG);         // Trigger request signal to update
}
