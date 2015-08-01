/*
 * file   : main.c
 * date   : 2015-07-17
 * author : Kenya Ukai
 *
 * 何?
 *
 * 接続
 *
 * Fuse
 *     l:0x62 h:0xDF ex:0xF9
 */

//以下は自分の開発環境用

#ifndef __AVR_ATmega168P__
#define __AVR_ATmega168P__
#endif

//ここから一般向け


#define F_CPU 1000000 //1MHz
#include<avr/io.h>
#include<util/delay.h>

void AD_init(){
    ADCSRA |= _BV(ADEN);
}


void IO_init(){
  DDRB = 0b00000001;    //PB0を出力に（他は入力）
  DDRC = 0b00000000;    //（サンプルなので一応明示的に）PC0を入力に(ADC用)
}


int AD_Convert(char pin){
  ADMUX=pin;
  ADCSRA|=_BV(ADSC);
  loop_until_bit_is_set(ADCSRA, ADSC);  
  return ADC;
}

int main(void){
  IO_init();
  AD_init();
  
  while(1){

    if(AD_Convert()>=512){
      PORTB|=_BV(PB0);
    }
    else {
      PORTB&=~_BV(PB0);
    }

    _delay_ms(10);
  }
  
  return 0;
}
