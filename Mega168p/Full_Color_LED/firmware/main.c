/**
 * @file main.c
 * @brief フルカラーLEDをタイマーで（簡易的に）制御するテスト
 *
 * @par 回路
 * LEDをマクロで指定の場所に抵抗を介して接続\n
 * PORTCの0〜4番ピンにスイッチを接続、押すとGNDにつながるように（内部でプルアップ済み）\n
 * @waning
 * LEDはマクロでポート指定しているがそれを変えてもDDRを設定しないといけない。\n
 * @author cormoran
 * @par License
 * MIT Lisense
 * @par
 * (C) 2015 cormoran ALL rights reserved.
 */



#define F_CPU 8000000L


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define LEDPORT PORTD
#define LED_R   PD5
#define LED_G   PD6
#define LED_B   PD7

///@warning 色のRGBは適当
enum COLOR_NAME {BLACK=0,RED=1,GREEN,YELLOW,BLUE,PURPLE,AQUA,WHITE};
unsigned char COLORS[][3] = {
    {  0,  0,  0},
    {255,  0,  0},
    {  0,255,  0},
    {255,255,  0},
    {  0,  0,255},
    {255,  0,255},
    {  0,255,255},
    {255,255,255}
};

unsigned char  led_cnt   = 0;
unsigned char *led_color = COLORS[WHITE];

///Timer0のオーバーフロー割り込み
///@note 超簡易実装なのでこれでうまく色が出ているのか不明
///@note よくわからないがなんかうまくいったっぽい
ISR(TIMER0_OVF_vect){
    unsigned char nxLEDPORT=LEDPORT;
    led_cnt++;
    if(led_cnt==0)
        nxLEDPORT &= ~_BV(LED_R) & ~_BV(LED_G) & ~_BV(LED_B);

    if(led_cnt>=led_color[0])
        nxLEDPORT |= _BV(LED_R) ;
    if(led_cnt>=led_color[1])
        nxLEDPORT |= _BV(LED_G) ;
    if(led_cnt>=led_color[2])
        nxLEDPORT |= _BV(LED_B) ;
    LEDPORT=nxLEDPORT;
}

///Timer0の初期化
void Timer0_init(){
    TCCR0A  = 0 ;         // Timer0は標準動作
    TCCR0B  = _BV(CS01);  // 前置分周1/8
                          // F_CPU/8 の周波数でカウントする
                          // 8bitタイマなので割り込み周期は1/(F_CPU/8/256)
    
    TIMSK0  = _BV(TOIE0); // Timer0カウンタ溢れ割り込み許可
}

void init(){
    DDRB  = 0xff;  //PORTBは全出力
    DDRC  = 0;     //PORTCは全入力
    PORTC = 0xff;  //PORTCの下位4bitピンはプルアップする
    Timer0_init();
    sei();         //全割り込み許可
}

///無限に、色を順に変える
void in_turn(){
    unsigned char num=0;
    while(1){
        _delay_ms(1000);
        led_color =COLORS[num++];
        if(num>=8)num=0;
    }
}

int main(void)
{
    init();
    
    //in_turn();
    
    while(1){
        unsigned char num=0;
        _delay_ms(100);
        
        //プルアップなのでLowでスイッチ押下
        if(!(PINC & _BV(PC5)))num+=1<<3;
        if(!(PINC & _BV(PC4)))num+=1<<2;
        if(!(PINC & _BV(PC3)))num+=1<<1;
        if(!(PINC & _BV(PC2)))num+=1<<0;
        
    }
    return 0;
}
