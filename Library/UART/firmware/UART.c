/**
 * @file UART.c
 * @brief UART通信ライブラリ for ATmega168p
 * @author cormoran
 * @date 2015/02/24
 *
 * Copyright (C) 2015 cormoran All rights reserved.
 */


///x bit目のみ1
#define bSET(x) (1<<(x))
///x bit目のみ0
#define bCLR(x) (~(1<<(x)))
///x の y bit目が1かどうか
#define is_SET(x,y) ((x) & (1<<(y)))

#include "UART.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#ifdef USE_STDIO
#include <stdio.h>
#endif


volatile signed char rcvbuf[RCVBUF_SIZE]={};
volatile signed char sndbuf[SNDBUF_SIZE]={};
volatile unsigned char sndbuf_cnt_w=0,sndbuf_cnt_r=0;
volatile unsigned char rcvbuf_cnt_w=0,rcvbuf_cnt_r=0;

volatile char sndbuf_over_flg=0;//バッファ溢れを防ぐため

#ifdef USE_STDIO
static FILE uart_out = FDEV_SETUP_STREAM(UART_putchar, NULL,_FDEV_SETUP_WRITE);
static FILE uart_in = FDEV_SETUP_STREAM(NULL,UART_getchar,_FDEV_SETUP_READ);
#endif


/*
 * UARTモジュールの初期化
 */
void UART_init(){
    
#ifdef USE_STDIO
    stdout=&uart_out;
    stderr=&uart_out;
    stdin =&uart_in;
#endif
    
    UCSRB  = 0 ;
    UCSRC  = 0 ;
    
    UCSRB |= bSET(RXCIE) | bSET(UDRIE);  //受信完了、送信準備完了割り込み許可
    UCSRC |= bSET(UCSZ1) | bSET(UCSZ0);  // 0b(0)11 DataBit長 8bit
    //非同期通信、パリティ検査禁止
    UBRR   = F_CPU/16/BAUD - 1 ;           //ボーレート設定
    UCSRB |= bSET(RXEN) | bSET(TXEN);    //RX,TXピンを有効化、送受信許可
}



///受信完了時割り込み
ISR(USART_RX_vect){
    rcvbuf[ (int)rcvbuf_cnt_w++ ] = UDR ;
    if( rcvbuf_cnt_w >= RCVBUF_SIZE ) rcvbuf_cnt_w = 0 ;
}


///送信準備完了時割り込み
ISR(USART_UDRE_vect){
    if( sndbuf_over_flg || sndbuf_cnt_r != sndbuf_cnt_w ){
        UDR = sndbuf[ (int)sndbuf_cnt_r++ ];
        if( sndbuf_cnt_r >= SNDBUF_SIZE ) sndbuf_cnt_r = 0;
        sndbuf_over_flg = 0;
    }
    else UCSRB &= bCLR(UDRIE);
}


/**
 * １文字転送関数 <br>
 * @attention 送信バッファが溢れる場合は待機する
 * @param ch 送信するASCII文字
 */
void UART_putchar(char ch){
    while(sndbuf_over_flg);
    UCSRB &= bCLR(UDRIE);
    sndbuf[ (int)sndbuf_cnt_w++ ] = ch ;
    if( sndbuf_cnt_w >= SNDBUF_SIZE ) sndbuf_cnt_w = 0 ;
    if(sndbuf_cnt_w==sndbuf_cnt_r)sndbuf_over_flg=1;
    UCSRB |= bSET(UDRIE);
}

/**
 * １文字転送関数（ノンブロッキング版）<br>
 * 送信バッファから溢れるものは捨てられる<br>
 * @param ch 送信するASCII文字
 */
void UART_putchar_nb(char ch){
    UCSRB &= bCLR(UDRIE);
    if(sndbuf_over_flg){UCSRB|=bSET(UDRIE);return;}
    sndbuf[ (int)sndbuf_cnt_w++ ] = ch ;
    if( sndbuf_cnt_w >= SNDBUF_SIZE ) sndbuf_cnt_w = 0 ;
    if(sndbuf_cnt_w==sndbuf_cnt_r)sndbuf_over_flg=1;
    UCSRB |= bSET(UDRIE);
}

/**
 * １文字受信関数<br>
 * @attention 受信する文字がない場合は待機する
 * @return 受信したASCII文字
 */
char UART_getchar(){
    while(rcvbuf_cnt_r == rcvbuf_cnt_w); //受信するまで待機
    UCSRB &= bCLR(RXCIE) ;
    char ret = rcvbuf[(int)rcvbuf_cnt_r++];
    if(rcvbuf_cnt_r>=RCVBUF_SIZE)rcvbuf_cnt_r=0;
    UCSRB |= bSET(RXCIE) ;
    return ret;
}

/**
 * １文字受信関数（ノンブロッキング版）<br>
 * 受信する文字がない場合は -1 を返す<br>
 * @return 受信したASCII文字 or -1
 */
char UART_getchar_nb(){
    char ret = -1;
    UCSRB &= bCLR(RXCIE) ;
    if(rcvbuf_cnt_r!=rcvbuf_cnt_w){
        ret = rcvbuf[(int)rcvbuf_cnt_r++];
        if(rcvbuf_cnt_r>=RCVBUF_SIZE)rcvbuf_cnt_r=0;
    }
    UCSRB |= bSET(RXCIE) ;
    return ret;
}

/**
 * 文字列送信関数<br>
 * NULL文字まで送信バッファに入れる<br>
 * 内部では UART_putchar を使用<br>
 * @param *str 送信する文字列のアドレス
 */
void UART_puts(char *str){
    while(*str)UART_putchar(*(str++));
    UART_putchar('\r');
    UART_putchar('\n');
}

/**
 * 受信バッファにデータがあるかどうかを返す関数 <br>
 * @return データがあれば 1 、なければ 0
 */
char UART_is_received(){
    return rcvbuf_cnt_r!=rcvbuf_cnt_w;
}

/**
 * 送信バッファの残りサイズを返す関数<br>
 * 使わないかもしれない...<br>
 * @return 送信バッファの残りサイズ
 */
char UART_get_left_of_snd_buffer(){
    char ret = sndbuf_cnt_r - sndbuf_cnt_w ;
    while(ret<0)ret+=SNDBUF_SIZE;
    return ret;
}

/**
 * 永遠に受信データをオウム返しする関数<br>
 * @warning 内部で無限ループする
 */
void UART_Parrot(){
    char data;
    while(1){
      while( !(UCSRA & bSET(RXC)) );
      data = UDR;
      while( !(UCSRA & bSET(UDRE)) );
      UDR = data;
    }
}
