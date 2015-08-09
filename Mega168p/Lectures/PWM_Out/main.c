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

void IO_init(){
  DDRD|=_BV(PD6);//PD6のみ出力
}

void Timer0_PWM_init(){
  TCCR0A |= _BV(WGM01)|_BV(WGM00); //8bit高速PWM
  TCCR0A |= _BV(COM0A1); //OC0Aは非反転動作(OC0Bは切断)

  TCCR0B |= _BV(CS02); //1024分周(システムクロック?で動作 今回1MHz/1024=1kHz)

  OCR0A=0;
}


int main(void){
  IO_init();
  Timer0_PWM_init();

  unsigned char duty=0;
  
  while(1){

    OCR0A = duty;
    
    duty+=10; //溢れたら勝手に0の方へ戻る
    
    _delay_ms(500);
    
  }
  
  return 0;
}
