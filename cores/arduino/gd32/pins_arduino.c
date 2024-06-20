/*
  Copyright (c) 2011 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "pins_arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

uint32_t PinName_to_digital(PinName p)
{
    uint32_t i = DIGITAL_PINS_NUM;
    p &= ~ALTMASK;
    if (GD_PORT_GET(p) <= (PORTEND - 1)) {
        for (i = 0; i < DIGITAL_PINS_NUM; i++) {
            if (digital_pins[i] == p) {
                break;
            }
        }
    }
    return i;
}

PinName analog_pin_to_PinName(uint32_t pin)
{
    PinName pn = DIGITAL_TO_PINNAME(ANALOG_PINS_TO_DIGITAL(pin));
    if (pn == NC) {
        switch (pin) {
#if defined(ADC_CHANNEL_TEMPSENSOR) || defined(ADC_CHANNEL_TEMPSENSOR_ADC1)
            case ATEMP:
                pn = ADC_TEMP;
                break;
#endif
#ifdef AVREF
            case AVREF:
                pn = ADC_VREF;
                break;
#endif
            default:
                break;
        }
    }
    return pn;
}

bool  pin_in_analog_pins(uint32_t pin)
{
    bool ret = false;
#if ANALOG_PINS_NUM > 0
#ifndef ANALOG_PINS_LAST
    pin &= ~ALTMASK;
    ret = (pin >= A0) && (pin < (A0 + ANALOG_PINS_NUM));
#else
    for (uint32_t i = 0; i < ANALOG_PINS_NUM; i++) {
        if (analog_pins[i] == pin) {
            ret = true;
            break;
        }
    }
#endif /* ANALOG_PINS_LAST */
#endif /* ANALOG_PINS_NUM > 0 */
    return ret;
}

uint32_t digital_pin_to_analog(uint32_t pin)
{
    uint32_t ret = ANALOG_PINS_NUM;
#if ANALOG_PINS_NUM > 0
#ifndef ANALOG_PINS_LAST
    pin &= ~ALTMASK;
    ret = pin - A0;
#else
    for (uint32_t i = 0; i < ANALOG_PINS_NUM; i++) {
        if (analog_pins[i] == pin) {
            ret = i;
            break;
        }
    }
#endif /* ANALOG_PINS_LAST */
#endif /* ANALOG_PINS_NUM > 0 */
    return ret;
}

#ifdef __cplusplus
}
#endif
