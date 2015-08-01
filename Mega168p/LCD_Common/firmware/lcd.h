/*
 * File:   lcd.h
 * Author: cormoran
 *
 * Created on 2014/09/05
 * Modified for AVR  on 2015/02/22.
 *
 */


#ifndef LCD_H
#define	LCD_H

#ifdef	__cplusplus
extern "C" {
#endif
    //#########################################################################//
    //#########################################################################//
    
    /*クロック設定*/
#ifndef F_CPU
#define F_CPU 8000000 // delay関数のために必要　システムクロックを設定
#endif

#include <avr/io.h>
#include <util/delay.h>

    /* ポート設定
     * LCDとの通信は7ピン使う。RS,RW,Eは任意
     * DataBitはLCDとの送受信に使うことがあり、入出力の切り替えが必要なので扱いやすいように同一ポートから4つ選ぶようにしている
     */
    
#define b_SET(x,y) ((x)|= (1<<(y)))
#define b_CLR(x,y) ((x)&=~(1<<(y)))
    
#define RS_PORT PORTB
#define RS_BIT  PB4
#define RW_PORT PORTB
#define RW_BIT  PB5
#define E_PORT  PORTB
#define E_BIT   PB6

#define DB_TRIS_lcd DDRB //DataBitに使うボートのIO設定レジスタ
#define DB_lcd PORTB      //DataBitに使うポートの値読み、設定レジスタ
#define TRIS_mask_lcd 0x0F//つかうDBポートに合わせたマスク
                          //例: 0xF0でR?4/5/6/7 , 0x0FでR?0/1/2/3がDBポートとして使われる （?=A,Bなど）

    
    /*その他設定*/
    //#define USE_BUSYFLG //動作しないのでコメントアウト必須
    void lcd_put_char(char data);
    
    //#########################################################################//
    //#########################################################################//
    
    /*その他のマクロ*/
#define E_Set_Edge (b_SET(E_PORT,E_BIT),b_CLR(E_PORT,E_BIT)) //多分220ns highになってないといけないのでクロックが高すぎる場合は対策必要
#define RSW_Set_W_CMD (b_CLR(RS_PORT,RS_BIT),b_CLR(RW_PORT,RW_BIT))
#define RSW_Set_W_DATA (b_SET(RS_PORT,RS_BIT),b_CLR(RW_PORT,RW_BIT))
#define RSW_Set_R_CMD (b_CLR(RS_PORT,RS_BIT),b_SET(RW_PORT,RW_BIT))
#define RSW_Set_R_DATA (b_SET(RS_PORT,RS_BIT),b_SET(RW_PORT,RW_BIT))
    
#define DB_ReadData_lcd ((DB_lcd & TRIS_mask_lcd)<<4) //上位4bitにデータが入り下位4bitは0000となるように調節する 4/5/6/7ポートなら<<0、0/1/2/3ポートなら<<4
#define DB_SetData_lcd(data)  ( ( (data&0xF0)  >>4 )  |  (DB_lcd & ~TRIS_mask_lcd)  ) //上位4bitに送信データが入っているのでそれがDBポートに出るように調節する4/5/6/7ポートなら>>0、0/1/2/3ポートなら>>4
    
    /*変数*/
    unsigned char TRIS_INI ;  //ポートBの入出力初期設定格納
    
    /*関数宣言*/
    
    void lcd_ShowString(unsigned char* strdata);
    void lcd_MoveLine(unsigned char line);
    void lcd_ClearDisplay(void);
    void lcd_CursorAtHome(void);
    void lcd_DisplayShift_L(int n);
    void lcd_DisplayShift_R(int n);
    void lcd_CursorShift_L(int n);
    void lcd_CursorShift_R(int n);
    void lcd_Set_DisplayPosition(char address);
    
    void lcd_EntryMode_Set(char i_d,char shift);
    void lcd_DisplayControl(char display,char cursor,char blink);
    void lcd_FunctionSet(char datalen,char linenum,char font);
    
    void lcd_setDB(unsigned char data);
    void lcd_init(void);
    void lcd_send_initcmd(unsigned char cmd);
    void lcd_send_cmd(unsigned char cmd);
    void lcd_send_data(unsigned char data);
    unsigned char lcd_ready(void);
    
    
#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */
