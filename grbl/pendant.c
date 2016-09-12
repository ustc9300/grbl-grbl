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
static uint8_t val2key(uint16_t input);

// do some initialization
void pendant_init()
{
    return;
}

static uint8_t oldkey = KEY_IDLE;
uint8_t get_key() 
{
    uint8_t key = 0;
    key = val2key(adc_read(ADC_PIN));   // convert into key press	
    if (key != oldkey)	          // if keypress is detected
    {
        _delay_ms(50.0);                // wait for debounce time
        key = val2key(adc_read(ADC_PIN));  // read the value from the sensor  
    }
    oldkey = key;
    return key;
}

// Convert ADC value to direction
static uint8_t val2key(uint16_t input)
{  

    if       (input < 100) return KEY_ERROR;
    else  if (input < 300) return KEY_X_L;
    else  if (input < 400) return KEY_X_R;
    else  if (input < 550) return KEY_Y_F;
    else  if (input < 700) return KEY_Y_B;    
    else  if (input < 850) return KEY_Z_D;
    else  if (input <1000) return KEY_Z_U;    
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
    uint8_t  low, high;
    uint16_t val;

    /* Enable the ADC  and the set prescale betwenen system clock and ADC clock */
    ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);

    /* adcx is the analog pin we want to use.  ADMUX's first few bits are
       the binary representations of the numbers of the pins so we can
       just 'OR' the pin's number with ADMUX to select that pin.
       We first zero the four bits by setting ADMUX equal to its higher
       four bits. */
  
    // arduino defualt use AREF
    ADMUX = _BV(REFS0) | ( adcx & 0x0F );

    /* This starts the conversion. */
    ADCSRA |= _BV(ADSC);

    /* This is an idle loop that just wait around until the conversion
       is finished.  It constantly checks ADCSRA's ADSC bit, which we just
       set above, to see if it is still set.  This bit is automatically
       reset (zeroed) when the conversion is ready so if we do this in
       a loop the loop will just go until the conversion is ready. */
#if 1
    while ( (ADCSRA & _BV(ADSC)) );
#else
    // an alternative way to wait for ADC, same result with above line
    while ( !(ADCSRA & _BV(ADIF)) );
    ADCSRA |= _BV(ADIF);
#endif

    // we have to read ADCL first; doing so locks both ADCL
    // and ADCH until ADCH is read.  reading ADCL second would
    // cause the results of each conversion to be discarded,
    // as ADCL and ADCH would be locked when it completed.
    low  = ADCL;
    high = ADCH;

    val = (high << 8) | low;

    /* Frinally, we return the converted value to the calling function. */
    return val;
}

