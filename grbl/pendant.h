/*
  pendant.h -  roy's handcraft pendant controller

  Copyright (c) 2015-2016 Yong GE
  
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

#ifndef pendant_h
#define pendant_h

/* Which analog pin we want to read from.  The pins are labeled "ADC0"
   "ADC1" etc on the pinout in the data sheet.  In this case ADC_PIN
   being 0 means we want to use ADC0.  On the ATmega328P this is also
   the same as pin PC0 */

#define ADC_PIN    4

#define KEY_IDLE   0
#define KEY_X_L    1
#define KEY_X_R    2
#define KEY_Y_F    3
#define KEY_Y_B    4
#define KEY_Z_U    5
#define KEY_Z_D    6

void  pendant_init(void);
char  get_key(void);

#endif
