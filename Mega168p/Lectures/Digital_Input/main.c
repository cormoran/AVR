/*
 * file   : main.c
 * date   : 2015-07-17
 * author : Kenya Ukai
 *
 * 何?
 *     スイッチの状態を読み取るサンプルプログラム
 *     押している間LEDが点滅
 * 接続
 *     PB0にLED(マイコン側アノード(+))
 *     PB1にスイッチ(押すと直接GNDにつながるように)
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
  
  DDRB = 0b00000001;    //PB0を出力に（他は入力PB1も入力）
  PORTB= 0b00000010;    //PB1をプルアップ
  
  while(1){
    if(PINB & (1<<PB1)){
      //スイッチが押されていない時
      
    }
    else{
      //スイッチが押された時
      PORTB ^= (1<<PB0); //PB0の値を反転する
      _delay_ms(100);    //100ms待機 : 連続反応対策
    }
  }
  
  return 0;
}
