/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#define F_CPU 8000000L

#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
int main(void)
{
    
    DDRB=0xff;
    lcd_init();
    
    lcd_ShowString("Hello World!");
    _delay_ms(2000);
    while(1){
        _delay_ms(1000);
        lcd_ClearDisplay();
        printf("LCD Display Test");
        _delay_ms(1000);
        lcd_ClearDisplay();
        printf("A!#$&'()'@+*<>?}");
    }
    
    return 0;   /* never reached */
}
