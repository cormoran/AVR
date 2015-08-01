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

/*
  A/D変換 ADC
  10bit = 2^10 =1024 段階
  基準電圧が1023 GND が0
  
  ADMUX
  7 REFS1
  6 REFS0 : 基準電圧設定(reference select bit)
  5 ADLAR : 左揃え選択(ADC Left Adjust Result)
  4 NONE
  3 MUX3  
  2 MUX2
  1 MUX1
  0 MUX0  : チャンネル選択

  ADCSRA
  7 ADEN  : ADCモジュールON/OFF(ADC Enable)
  6 ADSC  : ADC開始(ADC start coversion)
  5 ADATE : ADC自動変換許可
  4 ADIF  : ADC完了割り込みフラグ
  3 ADIE  : ADC完了割り込み許可
  2 ADPS2 
  1 ADPS1
  0 ADPS0 : ADC動作クロック選択

  ADCSRB
  7 N
  6 ACME (アナログ比較用)
  5 N
  4 N
  3 N
  2 ADTS2 
  1 ADTS1
  0 ADTS0 : ADC自動起動要因選択

  ADCH,ADCL ADCデータレジスタ
  注意:ADLARの値によって格納のされ方が変わる(右揃えor左揃え)

  DIDR0 : ADCに使うピンのDIを停止して消費電力削減する設定用
  
 */


#define F_CPU 1000000 //1MHz
#include<avr/io.h>
#include<util/delay.h>

void AD_init(){
  
    ADMUX |= _BV(REFS0) ;    // 0b01 基準電圧をAVCCにする
                              // ADCにADC0ピンを接続(default)
    
    ADCSRA |= _BV(ADEN);    // A/D変換使用許可
                              // (ちなみに)入力クロックはCK/2
    
    DIDR0  = _BV(ADC1D);     // ADC1のポートのデジタル入力禁止（省電力のため推奨なくてもいい）   
}

void IO_init(){
  DDRB = 0b00000001;    //PB0を出力に（他は入力）
  DDRC = 0b00000000;    //（サンプルなので一応明示的に）PC0を入力に(ADC用)
}

int AD_Convert(){
  ADCSRA|=_BV(ADSC); //変換開始

  loop_until_bit_is_set(ADCSRA, ADSC);
  // while(ADCSRA & (1<<ADSC)); //と同じ意味
  
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
