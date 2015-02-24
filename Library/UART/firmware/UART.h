/**
 * @file UART.h
 * @brief UART通信ライブラリ for ATmega168p
 * @par 使い方
 * はじめに UART_init()を呼ぶ <br>
 * main関数でsei()で割り込み許可が必要 <br>
 * ヘッダをインクルードする前に F_CPU を定義しておく
 *
 * @author cormoran
 * @date 2015/02/24
 * @par
 * Copyright (C) 2015 cormoran All rights reserved.
 */



#ifndef __USART_Test__UART__
#define __USART_Test__UART__

#ifndef F_CPU
#define F_CPU 1000000L
#endif


/// ボーレート
#define BAUD 9600
//#define BAUD 112500

//バッファサイズ
#define RCVBUF_SIZE 25
#define SNDBUF_SIZE 25
///stdio.hの関数を使わないならコメントアウト
#define USE_STDIO

///@name レジスタマクロ
///@{
#define UCSRA UCSR0A
#define UCSRB UCSR0B
#define UCSRC UCSR0C
#define UBRR  UBRR0
#define UDR   UDR0
///@}
///@name ビットマクロ
///@{
#define RXCIE RXCIE0
#define UDRIE UDRIE0
#define UCSZ1 UCSZ01
#define UCSZ0 UCSZ00
#define RXEN  RXEN0
#define TXEN  TXEN0
#define RXC   RXC0
#define UDRE  UDRE0

///@}

void UART_init();

void UART_putchar(char ch);

char UART_getchar();

char UART_getchar_nb();
void UART_puts(char *str);

char UART_is_received();
char UART_get_left_of_snd_buffer();
void UART_Parrot();


#endif /* defined(__USART_Test__UART__) */
