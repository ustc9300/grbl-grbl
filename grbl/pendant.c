/*
  pendant.c - roy's handcraft pendant control
  
  Copyright (c) 2016-2017  Yong GE
  
  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "grbl.h"

static uint16_t adc_read(uint8_t adcx);
static char val2key(uint16_t input);

// do some initialization
void pendant_init(void)
{
    return;
}

static char oldkey = KEY_IDLE;
char get_key(void) 
{
    char key = 0;
    key = val2key(adc_read(ADC_PIN));   // convert into key press	
    if (key != oldkey)	          // if keypress is detected
    {
#if 0
        delay(50);                // wait for debounce time
#endif
        key = val2key(adc_read(ADC_PIN));  // read the value from the sensor  
    }
    oldkey = key;
    return key;
}

// Convert ADC value to direction
static char val2key(uint16_t input)
{   
    if       (input < 100) return KEY_IDLE;
    else  if (input < 300) return KEY_X_L;
    else  if (input < 500) return KEY_X_R;
    else  if (input < 700) return KEY_Y_F;
    else  if (input < 900) return KEY_Y_B;    
    else  if (input < 700) return KEY_Z_U;
    else  if (input < 900) return KEY_Z_D;    
    else  return KEY_IDLE;
}

/*
  This function just keeps the reading code out of the loop itself.
  It takes the analog pin number as a parameter and returns the
  analog reading on that pin as a result.
  Look for its definition below main. 
*/
static uint16_t adc_read(uint8_t adcx)
{
    /* Enable the ADC */
    ADCSRA |= _BV(ADEN);

    /* adcx is the analog pin we want to use.  ADMUX's first few bits are
       the binary representations of the numbers of the pins so we can
       just 'OR' the pin's number with ADMUX to select that pin.
       We first zero the four bits by setting ADMUX equal to its higher
       four bits. */
    /*
    ADMUX &= 0xF0;
    */
    ADMUX &= 0x40;
    ADMUX |= adcx;

    /* This starts the conversion. */
    ADCSRA |= _BV(ADSC);

    /* This is an idle loop that just wait around until the conversion
       is finished.  It constantly checks ADCSRA's ADSC bit, which we just
       set above, to see if it is still set.  This bit is automatically
       reset (zeroed) when the conversion is ready so if we do this in
       a loop the loop will just go until the conversion is ready. */
    while ( (ADCSRA & _BV(ADSC)) );

    /* Finally, we return the converted value to the calling function. */
    return ADC;
}
