/**
 * @file main.c
 * @brief
 * @author cormoran
 * @par License
 * MIT Lisense
 * @par
 * (C) 2015 cormoran ALL rights reserved.
 */

/** @mainpage
 * PWM信号を任意の周波数で出力するテスト \n
 * OC1BにPWMが出力される \n
 * ADC1の入力電圧によって出力の周波数が変化する。Duty比は50%で一定
 * 真ん中あたりの周波数で圧電サウンダが使える
 */


#define F_CPU 8000000L


#include <avr/io.h>
#include <util/delay.h>

#define bSET(x) (1<<(x))     // bit x のみが1
#define bCLR(x) (~(1<<(x)))  // bit x のみが0
#define is_SET(x,y) ((x) & (1<<(y)))

/**
 * MR1の設定(PWM) \n
 * 出力PWMは  周波数 clk / 8 * OCR1A \n
 *           Duty  High:Low = OCR1B : OCR1A
 *
 */
void TMR1_init(){
    
    TCNT1=0;
    TCCR1A = 0;
    TCCR1B = 0;
    
    TCCR1A |= bSET(WGM11) | bSET(WGM10)  ; // 0b(11)11 OCR1AをTOPをする高速PWM
    TCCR1B |= bSET(WGM13) | bSET(WGM12)  ; // 上の括弧部分
    
    TCCR1A |= bSET(COM1B1) ; // 0b10    OC1B出力は比較一致でLow,BottomでHigh
                             //         (ちなみに)OC1Aは無効 0b00
    
    
    ICR1 =0xffff;
    
    OCR1A=0x00ff;  //タイマのトップ値の設定
    
    OCR1B=0x00ff;     //比較一致の値
    
    //割り込みは不可（多分デフォルト？）
    TCCR1B |= bSET(CS11) ;   // 0b010   Timer1のクロックはclkを8分周 start
}

void init(){
    TMR1_init();
    DDRB = 0xff;//全てのポートを出力に
    DDRC = 0xff & bCLR(PC5) & bCLR(PC4) & bCLR(PC3) & bCLR(PC2);
    PORTC=  bSET(PC5) | bSET(PC4) | bSET(PC3) | bSET(PC2);
    PORTB=0;
}

int main(void)
{
    init();
    OCR1A=1000;
    OCR1B=500 ;
    
    while(1){
      _delay_ms(1);
      PORTD^=0b01100000;
      if(PINC & bSET(PC5) ==0 ){
        OCR1A=1000;
        OCR1B=500 ;
      }
      if(PINC & bSET(PC4) ==0 ){
        OCR1A=2000;
        OCR1B=100 ;
      }
      if(PINC & bSET(PC3) ==0 ){
        OCR1A=300;
        OCR1B=150 ; 
      }
    }
    
    return 0;   /* never reached */
}
