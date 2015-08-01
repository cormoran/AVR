/**
 * @file main.c
 * @brief
 * @author cormoran
 * @data 2015/02/24
 *
 * Copyright (C) 2015 cormoran All rights reserved.
 */

#define bSET(x) (1<<(x))     // bit x のみが1
#define bCLR(x) (~(1<<(x)))  // bit x のみが0
#define is_SET(x,y) ((x) & (1<<(y)))

#define F_CPU 8000000L   ///delay と UART に必要

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "UART.h"


char *sentence = "Hello World! This program is the uart communication test from avr. You can send the sentence whose length is over the send buffer size , but if then , it takes more time than if it isn't.";


void init(){
    DDRB = 0xff;
    DDRC = 0xff;
    DDRD = 0xff & bCLR(PD0) &bCLR(PD1);  //ここ必要か不明
    PORTD=0;
    
    UART_init();
    sei();
}


int main(void)
{
    init();
    while(!UART_is_received());
    
    while(1){
        _delay_ms(2000);
        UART_puts(sentence);

	//受信バッファをすべて読む
        while( UART_is_received() ){
            printf("received");
            PORTB = UART_getchar();
        }
        PORTC^=bSET(PC5);
        
    }
     
    
    return 0;
}
