/*
    Copyright (c) 2021, CommunityGD32Cores

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#ifndef PERIPHERALNAMES_H
#define PERIPHERALNAMES_H

#include "gd32xxyy.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ADC_0 = (int)ADC0,
    ADC_1 = (int)ADC1,
    ADC_2 = (int)ADC2
} ADCName;

typedef enum {
    DAC_0 = (int)DAC
} DACName;

typedef enum {
    UART_3 = (int)UART3,
    UART_4 = (int)UART4,
    UART_0 = (int)USART0,
    UART_1 = (int)USART1,
    UART_2 = (int)USART2
} UARTName;

typedef enum {
    SPI_0 = (int)SPI0,
    SPI_1 = (int)SPI1,
    SPI_2 = (int)SPI2
} SPIName;

typedef enum {
    I2C_0 = (int)I2C0,
    I2C_1 = (int)I2C1
} I2CName;

typedef enum {
    PWM_0 = (int)TIMER0,
    PWM_1 = (int)TIMER1,
    PWM_2 = (int)TIMER2,
    PWM_3 = (int)TIMER3,
    PWM_4 = (int)TIMER4,
    PWM_7 = (int)TIMER7,
    PWM_8 = (int)TIMER8,
    PWM_9 = (int)TIMER9,
    PWM_10 = (int)TIMER10,
    PWM_11 = (int)TIMER11,
    PWM_12 = (int)TIMER12,
    PWM_13 = (int)TIMER13
} PWMName;


#ifdef __cplusplus
}
#endif

#endif
