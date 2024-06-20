/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

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

#include "PeripheralPins.h"
#include "gd32xxyy.h"

/*  void pin_function(PinName pin, int function);
    configure the speed, mode,and remap function of pins
    the parameter function contains the configuration information,show as below
    bit 0:2   gpio mode
    bit 3:8   remap
    bit 9:10  gpio speed
    bit 11:15 adc  /timer channel
*/
const int GD_GPIO_PULL_UP_DOWN[] = {
    GPIO_PUPD_NONE,             /* 0 */
    GPIO_PUPD_PULLUP,           /* 1 */
    GPIO_PUPD_PULLDOWN,         /* 2 */
};

const int GD_GPIO_OUTPUT_MODE[] = {
    GPIO_OTYPE_PP,             /* 0 */
    GPIO_OTYPE_OD,             /* 1 */
};

const int GD_GPIO_AF[] = {
    GPIO_AF_0,             /* 0 */
    GPIO_AF_1,             /* 1 */
    GPIO_AF_2,             /* 2 */
    GPIO_AF_3,             /* 3 */
    GPIO_AF_4,             /* 4 */
    GPIO_AF_5,             /* 5 */
    GPIO_AF_6,             /* 6 */
    GPIO_AF_7,             /* 7 */
    GPIO_AF_9,             /* 8 */
    GPIO_AF_11             /* 9 */
};

/* pin descriptions only reference the index in the array, so
 * to get e.g. AF11 one must give it index = 9. provide 
 * convenience macros here.
 * for all other arrays, the value is also equivalent to the index,
 * so there doesn't need to be anything done more.
 */
#define IND_GPIO_AF_0 0
#define IND_GPIO_AF_1 1
#define IND_GPIO_AF_2 2
#define IND_GPIO_AF_3 3
#define IND_GPIO_AF_4 4
#define IND_GPIO_AF_5 5
#define IND_GPIO_AF_6 6
#define IND_GPIO_AF_7 7
#define IND_GPIO_AF_9 8
#define IND_GPIO_AF_11 9

/* GPIO MODE */
const int GD_GPIO_MODE[] = {
    GPIO_MODE_INPUT,             /* 0 */
    GPIO_MODE_OUTPUT,            /* 1 */
    GPIO_MODE_AF,                /* 2 */
    GPIO_MODE_ANALOG,            /* 3 */
};

/* GPIO SPEED */
const int GD_GPIO_SPEED[] = {
    GPIO_OSPEED_2MHZ,             /* 0 */
    GPIO_OSPEED_10MHZ,            /* 1 */
    0,                            /* 2 (unused) */
    GPIO_OSPEED_50MHZ,            /* 3 */
};

/* ADC PinMap */
const PinMap PinMap_ADC[] = {
    {PORTA_0,  ADC0, GD_PIN_FUNC_ANALOG_CH(0)},    /* ADC0_IN0 */
    {PORTA_1,  ADC0, GD_PIN_FUNC_ANALOG_CH(1)},    /* ADC0_IN1 */
    {PORTA_2,  ADC0, GD_PIN_FUNC_ANALOG_CH(2)},    /* ADC0_IN2 */
    {PORTA_3,  ADC0, GD_PIN_FUNC_ANALOG_CH(3)},    /* ADC0_IN3 */
    {PORTA_4,  ADC0, GD_PIN_FUNC_ANALOG_CH(4)},    /* ADC0_IN4 */
    {PORTA_5,  ADC0, GD_PIN_FUNC_ANALOG_CH(5)},    /* ADC0_IN5 */
    {PORTA_6,  ADC0, GD_PIN_FUNC_ANALOG_CH(6)},    /* ADC0_IN6 */
    {PORTA_7,  ADC0, GD_PIN_FUNC_ANALOG_CH(7)},    /* ADC0_IN7 */
    {PORTB_0,  ADC0, GD_PIN_FUNC_ANALOG_CH(8)},    /* ADC0_IN8 */
    {PORTB_1,  ADC0, GD_PIN_FUNC_ANALOG_CH(9)},    /* ADC0_IN9 */
    {PORTC_0,  ADC0, GD_PIN_FUNC_ANALOG_CH(10)},   /* ADC0_IN10 */
    {PORTC_1,  ADC0, GD_PIN_FUNC_ANALOG_CH(11)},   /* ADC0_IN11 */
    {PORTC_2,  ADC0, GD_PIN_FUNC_ANALOG_CH(12)},   /* ADC0_IN12 */
    {PORTC_3,  ADC0, GD_PIN_FUNC_ANALOG_CH(13)},   /* ADC0_IN13 */
    {PORTC_4,  ADC0, GD_PIN_FUNC_ANALOG_CH(14)},   /* ADC0_IN14 */
    {PORTC_5,  ADC0, GD_PIN_FUNC_ANALOG_CH(15)},   /* ADC0_IN15 */
    {PORTA_0,  ADC1, GD_PIN_FUNC_ANALOG_CH(0)},    /* ADC1_IN0 */
    {PORTA_1,  ADC1, GD_PIN_FUNC_ANALOG_CH(1)},    /* ADC1_IN1 */
    {PORTA_2,  ADC1, GD_PIN_FUNC_ANALOG_CH(2)},    /* ADC1_IN2 */
    {PORTA_3,  ADC1, GD_PIN_FUNC_ANALOG_CH(3)},    /* ADC1_IN3 */
    {PORTA_4,  ADC1, GD_PIN_FUNC_ANALOG_CH(4)},    /* ADC1_IN4 */
    {PORTA_5,  ADC1, GD_PIN_FUNC_ANALOG_CH(5)},    /* ADC1_IN5 */
    {PORTA_6,  ADC1, GD_PIN_FUNC_ANALOG_CH(6)},    /* ADC1_IN6 */
    {PORTA_7,  ADC1, GD_PIN_FUNC_ANALOG_CH(7)},    /* ADC1_IN7 */
    {PORTB_0,  ADC1, GD_PIN_FUNC_ANALOG_CH(8)},    /* ADC1_IN8 */
    {PORTB_1,  ADC1, GD_PIN_FUNC_ANALOG_CH(9)},    /* ADC1_IN9 */
    {PORTC_0,  ADC1, GD_PIN_FUNC_ANALOG_CH(10)},   /* ADC1_IN10 */
    {PORTC_1,  ADC1, GD_PIN_FUNC_ANALOG_CH(11)},   /* ADC1_IN11 */
    {PORTC_2,  ADC1, GD_PIN_FUNC_ANALOG_CH(12)},   /* ADC1_IN12 */
    {PORTC_3,  ADC1, GD_PIN_FUNC_ANALOG_CH(13)},   /* ADC1_IN13 */
    {PORTC_4,  ADC1, GD_PIN_FUNC_ANALOG_CH(14)},   /* ADC1_IN14 */
    {PORTC_5,  ADC1, GD_PIN_FUNC_ANALOG_CH(15)},   /* ADC1_IN15 */
    {PORTA_0,  ADC2, GD_PIN_FUNC_ANALOG_CH(0)},    /* ADC2_IN0 */
    {PORTA_1,  ADC2, GD_PIN_FUNC_ANALOG_CH(1)},    /* ADC2_IN1 */
    {PORTA_2,  ADC2, GD_PIN_FUNC_ANALOG_CH(2)},    /* ADC2_IN2 */
    {PORTA_3,  ADC2, GD_PIN_FUNC_ANALOG_CH(3)},    /* ADC2_IN3 */
    {PORTC_0,  ADC2, GD_PIN_FUNC_ANALOG_CH(10)},   /* ADC2_IN10 */
    {PORTC_1,  ADC2, GD_PIN_FUNC_ANALOG_CH(11)},   /* ADC2_IN11 */
    {PORTC_2,  ADC2, GD_PIN_FUNC_ANALOG_CH(12)},   /* ADC2_IN12 */
    {PORTC_3,  ADC2, GD_PIN_FUNC_ANALOG_CH(13)},   /* ADC2_IN13 */
    {ADC_TEMP, ADC, GD_PIN_FUNC_ANALOG_CH(16)},    /* ADC_IN16 */
    {ADC_VREF, ADC, GD_PIN_FUNC_ANALOG_CH(17)},    /* ADC_IN17 */
    {NC,   NC,    0}
};

/* DAC PinMap */
const PinMap PinMap_DAC[] = {
    {PORTA_4,  DAC, GD_PIN_FUNC_ANALOG_CH(0)},     /* DAC_OUT0 */
    {PORTA_5,  DAC, GD_PIN_FUNC_ANALOG_CH(1)},     /* DAC_OUT1 */
    {NC,   NC,    0}
};

/* I2C_SDA PinMap */
const PinMap PinMap_I2C_SDA[] = {
    {PORTB_11, I2C1, GD_PIN_FUNCTION4(PIN_MODE_AF, PIN_OTYPE_OD, PIN_PUPD_PULLUP, IND_GPIO_AF_0)}, /* I2C1_SDA */
    {PORTB_7,  I2C0, GD_PIN_FUNCTION4(PIN_MODE_AF, PIN_OTYPE_OD, PIN_PUPD_PULLUP, IND_GPIO_AF_0)}, /* I2C0_SDA */
    {PORTB_9,  I2C0, GD_PIN_FUNCTION4(PIN_MODE_AF, PIN_OTYPE_OD, PIN_PUPD_PULLUP, IND_GPIO_AF_0)}, /* I2C0_SDA */
    {NC,   NC,    0}
};

/* I2C_SCL PinMap */
const PinMap PinMap_I2C_SCL[] = {
    {PORTB_10, I2C1, GD_PIN_FUNCTION4(PIN_MODE_AF, PIN_OTYPE_OD, PIN_PUPD_PULLUP, IND_GPIO_AF_0)}, /* I2C1_SCL */
    {PORTB_6,  I2C0, GD_PIN_FUNCTION4(PIN_MODE_AF, PIN_OTYPE_OD, PIN_PUPD_PULLUP, IND_GPIO_AF_0)}, /* I2C0_SCL */
    {PORTB_8,  I2C0, GD_PIN_FUNCTION4(PIN_MODE_AF, PIN_OTYPE_OD, PIN_PUPD_PULLUP, IND_GPIO_AF_0)}, /* I2C0_SCL */
    {NC,   NC,    0}
};

/* PWM PinMap */
const PinMap PinMap_PWM[] = {
    {PORTA_0,  TIMER1,  GD_PIN_FUNC_PWM_2(0)},     /* TIMER1_CH0 */
    {PORTA_0,  TIMER4,  GD_PIN_FUNC_PWM_2(0)},     /* TIMER4_CH0 */
    {PORTA_1,  TIMER1,  GD_PIN_FUNC_PWM_2(1)},     /* TIMER1_CH1 */
    {PORTA_1,  TIMER4,  GD_PIN_FUNC_PWM_2(1)},     /* TIMER4_CH1 */
    {PORTA_2,  TIMER1,  GD_PIN_FUNC_PWM_2(2)},     /* TIMER1_CH2 */
    {PORTA_2,  TIMER4,  GD_PIN_FUNC_PWM_2(2)},     /* TIMER4_CH2 */
    {PORTA_2,  TIMER8,  GD_PIN_FUNC_PWM_2(0)},     /* TIMER8_CH0 */
    {PORTA_3,  TIMER1,  GD_PIN_FUNC_PWM_2(3)},     /* TIMER1_CH3 */
    {PORTA_3,  TIMER4,  GD_PIN_FUNC_PWM_2(3)},     /* TIMER4_CH3 */
    {PORTA_3,  TIMER8,  GD_PIN_FUNC_PWM_2(1)},     /* TIMER8_CH1 */
    {PORTA_6,  TIMER2,  GD_PIN_FUNC_PWM_2(0)},     /* TIMER2_CH0 */
    {PORTA_6,  TIMER12, GD_PIN_FUNC_PWM_2(0)},     /* TIMER12_CH0 */
    {PORTA_7,  TIMER2,  GD_PIN_FUNC_PWM_2(1)},     /* TIMER2_CH1 */
    {PORTA_7,  TIMER7,  GD_PIN_FUNC_PWM_2(0)},     /* TIMER7_CH0_ON */
    {PORTA_7,  TIMER13, GD_PIN_FUNC_PWM_2(0)},     /* TIMER13_CH0 */
    {PORTA_7,  TIMER0,  GD_PIN_FUNC_PWM_2(0)},     /* TIMER0_CH0_ON */
    {PORTB_0,  TIMER2,  GD_PIN_FUNC_PWM_2(2)},     /* TIMER2_CH2 */
    {PORTB_0,  TIMER7,  GD_PIN_FUNC_PWM_2(1)},     /* TIMER7_CH1_ON */
    {PORTB_0,  TIMER0,  GD_PIN_FUNC_PWM_2(1)},     /* TIMER0_CH1_ON */
    {PORTB_1,  TIMER2,  GD_PIN_FUNC_PWM_2(3)},     /* TIMER2_CH3 */
    {PORTB_1,  TIMER7,  GD_PIN_FUNC_PWM_2(2)},     /* TIMER7_CH2_ON */
    {PORTB_1,  TIMER0,  GD_PIN_FUNC_PWM_2(2)},     /* TIMER0_CH2_ON */
    {PORTB_10, TIMER1,  GD_PIN_FUNC_PWM_2(2)},     /* TIMER1_CH2 */
    {PORTB_11, TIMER1,  GD_PIN_FUNC_PWM_2(3)},     /* TIMER1_CH3 */
    {PORTB_13, TIMER0,  GD_PIN_FUNC_PWM_2(0)},     /* TIMER0_CH0_ON */
    {PORTB_14, TIMER0,  GD_PIN_FUNC_PWM_2(1)},     /* TIMER0_CH1_ON */
    {PORTB_14, TIMER11, GD_PIN_FUNC_PWM_2(0)},     /* TIMER11_CH0 */
    {PORTB_15, TIMER0,  GD_PIN_FUNC_PWM_2(2)},     /* TIMER0_CH2_ON */
    {PORTB_15, TIMER11, GD_PIN_FUNC_PWM_2(1)},     /* TIMER11_CH1 */
    {PORTC_6,  TIMER7,  GD_PIN_FUNC_PWM_2(0)},     /* TIMER7_CH0 */
    {PORTC_6,  TIMER2,  GD_PIN_FUNC_PWM_2(0)},     /* TIMER2_CH0 */
    {PORTC_7,  TIMER7,  GD_PIN_FUNC_PWM_2(1)},     /* TIMER7_CH1 */
    {PORTC_7,  TIMER2,  GD_PIN_FUNC_PWM_2(1)},     /* TIMER2_CH1 */
    {PORTC_8,  TIMER7,  GD_PIN_FUNC_PWM_2(2)},     /* TIMER7_CH2 */
    {PORTC_8,  TIMER2,  GD_PIN_FUNC_PWM_2(2)},     /* TIMER2_CH2 */
    {PORTC_9,  TIMER7,  GD_PIN_FUNC_PWM_2(3)},     /* TIMER7_CH3 */
    {PORTC_9,  TIMER2,  GD_PIN_FUNC_PWM_2(3)},     /* TIMER2_CH3 */
    {PORTA_8,  TIMER0,  GD_PIN_FUNC_PWM_2(0)},     /* TIMER0_CH0 */
    {PORTA_9,  TIMER0,  GD_PIN_FUNC_PWM_2(1)},     /* TIMER0_CH1 */
    {PORTA_10, TIMER0,  GD_PIN_FUNC_PWM_2(2)},     /* TIMER0_CH2 */
    {PORTA_11, TIMER0,  GD_PIN_FUNC_PWM_2(3)},     /* TIMER0_CH3 */
    {PORTA_15, TIMER1,  GD_PIN_FUNC_PWM_2(0)},     /* TIMER1_CH0 */
    {PORTB_3,  TIMER1,  GD_PIN_FUNC_PWM_2(1)},     /* TIMER1_CH1 */
    {PORTB_4,  TIMER2,  GD_PIN_FUNC_PWM_2(0)},     /* TIMER2_CH0 */
    {PORTB_5,  TIMER2,  GD_PIN_FUNC_PWM_2(1)},     /* TIMER2_CH1 */
    {PORTB_6,  TIMER3,  GD_PIN_FUNC_PWM_2(0)},     /* TIMER3_CH0 */
    {PORTB_7,  TIMER3,  GD_PIN_FUNC_PWM_2(1)},     /* TIMER3_CH1 */
    {PORTB_8,  TIMER3,  GD_PIN_FUNC_PWM_2(2)},     /* TIMER3_CH2 */
    {PORTB_8,  TIMER9,  GD_PIN_FUNC_PWM_2(0)},     /* TIMER9_CH0 */
    {PORTB_9,  TIMER3,  GD_PIN_FUNC_PWM_2(3)},     /* TIMER3_CH3 */
    {PORTB_9,  TIMER10, GD_PIN_FUNC_PWM_2(0)},     /* TIMER10_CH0 */
    {NC,   NC,    0}
};

/* UART_TX PinMap */
const PinMap PinMap_UART_TX[] = {
    {PORTA_2,  USART1, 7},                         /* USART1_TX */
    {PORTB_10, USART2, 7},                         /* USART2_TX */
    {PORTA_9,  USART0, 7},                         /* USART0_TX */
    {PORTC_10, USART2, 7},                         /* USART2_TX */
    {PORTB_6,  USART0, 7},                         /* USART0_TX */
    {PORTC_10, UART3, 7},                          /* UART3_TX */
    {PORTC_12, UART4, 7},                          /* UART4_TX  */
    {NC,   NC,    0}
};

/* UART_RX PinMap */
const PinMap PinMap_UART_RX[] = {
    {PORTA_3,  USART1, 1},                         /* USART1_RX */
    {PORTB_11, USART2, 1},                         /* USART2_RX */
    {PORTA_10, USART0, 1},                         /* USART0_RX */
    {PORTC_11, USART2, 1},                         /* USART2_RX */
    {PORTB_7,  USART0, 1},                         /* USART0_RX */
    {PORTC_11, UART3, 1},                          /* UART3_RX */
    {PORTD_2,  UART4, 1},                          /* UART4_RX */
    {NC,   NC,    0}
};

/* UART_RTS PinMap */
const PinMap PinMap_UART_RTS[] = {
    {PORTA_1,  USART1, 7},                         /* USART1_RTS */
    {PORTB_14, USART2, 7},                         /* USART2_RTS */
    {PORTA_12, USART0, 7},                         /* USART0_RTS */
    {NC,   NC,    0}
};

/* UART_CTS PinMap */
const PinMap PinMap_UART_CTS[] = {
    {PORTA_0,  USART1, 7},                         /* USART1_CTS */
    {PORTB_13, USART2, 7},                         /* USART2_CTS */
    {PORTA_11, USART0, 7},                         /* USART0_CTS */
    {NC,   NC,    0}
};

/* SPI_MOSI PinMap */
const PinMap PinMap_SPI_MOSI[] = {
    {PORTA_7,  SPI0, 7},                           /* SPI0_MOSI */
    {PORTB_15, SPI1, 7},                           /* SPI1_MOSI */
    {PORTC_12, SPI2, 7},                           /* SPI2_MOSI  */
    {PORTB_5,  SPI2, 7},                           /* SPI2_MOSI */
    {PORTB_5,  SPI0, 7},                           /* SPI0_MOSI */
    {NC,   NC,    0}
};

/* SPI_MISO PinMap */
const PinMap PinMap_SPI_MISO[] = {
    {PORTA_6,  SPI0, 7},                           /* SPI0_MISO */
    {PORTB_14, SPI1, 7},                           /* SPI1_MISO */
    {PORTC_11, SPI2, 7},                           /* SPI2_MISO */
    {PORTB_4,  SPI2, 7},                           /* SPI2_MISO */
    {PORTB_4,  SPI0, 7},                           /* SPI0_MISO */
    {NC,   NC,    0}
};

/* SPI_SCLK PinMap */
const PinMap PinMap_SPI_SCLK[] = {
    {PORTA_5,  SPI0, 7},                           /* SPI0_SCK */
    {PORTB_13, SPI1, 7},                           /* SPI1_SCK */
    {PORTC_10, SPI2, 7},                           /* SPI2_SCK */
    {PORTB_3,  SPI0, 7},                           /* SPI0_SCK */
    {NC,   NC,    0}
};

/* SPI_SSEL PinMap */
const PinMap PinMap_SPI_SSEL[] = {
    {PORTA_4,  SPI0, 7},                           /* SPI0_NSS */
    {PORTA_4,  SPI2, 7},                           /* SPI2_NSS */
    {PORTB_12, SPI1, 7},                           /* SPI1_NSS */
    {PORTA_15, SPI2, 7},                           /* SPI2_NSS */
    {PORTA_15, SPI0, 7},                           /* SPI0_NSS */
    {NC,   NC,    0}
};

/* CAN_RD PinMap */
const PinMap PinMap_CAN_RD[] = {
    {PORTA_11, CAN0, 7},                           /* CAN0_RX */
    {PORTB_8,  CAN0, 7},                           /* CAN0_RX */
    {NC,   NC,    0}
};

/* CAN_TD PinMap */
const PinMap PinMap_CAN_TD[] = {
    {PORTA_12, CAN0, 7},                           /* CAN0_TX */
    {PORTB_9,  CAN0, 7},                           /* CAN0_TX */
    {NC,   NC,    0}
};

