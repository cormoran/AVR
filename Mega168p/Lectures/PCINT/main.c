/*
 * File   : main.c
 * date   : 2015-07-17
 * author : Kenya Ukai
 *
 * 動作未検証
 *
 * 何?
 *     スイッチの入力を、入力変化割り込みで取得するサンプル
 * 接続
 *     PB0にLED 方向は問わない
 *     PB1(PCINT1)にスイッチ(押すと直接GNDにつながるように)
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
#include<avr/interrupt.h>

ISR(PCINT0_vect){
  if(PINB & _BV(PB1) == 0){
    PORTB ^= (1<<PB0); //PB0の値を反転する
    _delay_ms(10);
  }

}


void io_init(){
  DDRB = 0b00000001;    //PB0を出力に（他は入力PB1も入力）
  PORTB= 0b00000010;    //PB1をプルアップ  
}

void pin_change_init(){
  PCMSK0 = _BV(PCINT1);
  PCICR  |= _BV(PCIE0);
}




int main(void){
  io_init();
  pin_change_init();
  sei();//全割り込み許可
  
  while(1){

    
  }
  
  return 0;
}
