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
  ADMUX |= _BV(REFS0);    // 0b01 基準電圧をAVCCにする
  DDRD|=_BV(PD6);//PD6のみ出力
}

void Timer0_PWM_init(){
  TCCR0A |= _BV(WGM01)|_BV(WGM00); //8bit高速PWM
  TCCR0A |= _BV(COM0A1); //OC0Aは非反転動作(OC0Bは切断)

  TCCR0B |= _BV(CS02); //1024分周(システムクロック?で動作 今回1MHz/1024=1kHz)

  OCR0A=0;
}


unsigned short AD_Convert(unsigned char pin){
  ADMUX= (ADMUX & 0xf0) | (pin & 0x0f);
  ADCSRA|=_BV(ADSC);
  loop_until_bit_is_set(ADCSRA, ADSC);  
  return ADC;
}


int main(void){
  IO_init();
  AD_init();
  Timer0_PWM_init();
  
  while(1){

    OCR0A = AD_Convert(0);
    
    _delay_ms(10);
  }
  
  return 0;
}
