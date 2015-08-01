/*
 * file   : main.c
 * date   : 2015-07-17
 * author : Kenya Ukai
 *
 * 何?
 *     LEDを点滅させるサンプルプログラム
 * 接続
 *     PB0にLED 方向は問わない
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

int main(void){
  
  DDRB = 0b00000001;    //PB0を出力に（他は入力）
  
  while(1){
    
    PORTB ^= (1<<PB0); //PB0の値を反転する
    _delay_ms(500);    //500ms待機
    
  }
  
  return 0;
}
