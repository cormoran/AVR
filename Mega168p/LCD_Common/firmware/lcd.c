/*
 * File:   lcd.c
 * Author: cormoran
 *
 * Created on 2014/09/05
 * Moved from PIC on 2015/02/22
 *
 * LCD SC1602BS*B　のためのライブラリ PICからAVR用に移植
 *   Data4bitで通信
 * busyflgを使った待機は動作しない
 */

#include "lcd.h"
#include <stdio.h> //putchとセットでprintf使用可能


//printf関数でlcdにデータを送るための関数
void lcd_put_char(char data) {
    lcd_send_data(data);      // LCD への一文字表示関数
}



/************************************************************
 *制御用関数
 *lcd_ShowString(unsigned char* strdata)
 *lcd_send_data(unsigned char data)
 *lcd_MoveLine(unsigned char line)
 *lcd_ClearDisplay(void)
 *lcd_CursorAtHome(void)
 *lcd_DisplayShift_L(int n)
 *lcd_DisplayShift_R(int n)
 *lcd_CursorShift_L(int n)
 *lcd_CursorShift_R(int n)
 *lcd_Set_DisplayPosition(char address)
 *
 ************************************************************/

/*NULL文字まで文字データを送る関数*/
void lcd_ShowString(unsigned char* strdata)
{
    while(*strdata){
        lcd_send_data(*strdata++);
    }
}

/*lcdに1byte文字データを送る関数*/
void lcd_send_data(unsigned char data){
    lcd_setDB(data); //上位4bitを送信
    RSW_Set_W_DATA;
    E_Set_Edge;
    lcd_setDB(data<<4); //下位4bitを送信
    E_Set_Edge;
#ifdef USE_BUSYFLG
    while( lcd_ready() & 0x80); //busy flgがclearされるまで待つ
#else
    _delay_us(50);
#endif
}

/*1or2行目の先頭にカーソルを移動　line = 1 or 2*/
void lcd_MoveLine(unsigned char line)
{
    if(line==1) lcd_Set_DisplayPosition(0x00);
    else if(line==2) lcd_Set_DisplayPosition(0x40);
}
/*表示をクリアしてカーソルは0番地に*/
void lcd_ClearDisplay(void){
    lcd_send_cmd(0x01);
    _delay_ms(2);
}
/*カーソルを0番地に、表示領域も0番地からに設定*/
void lcd_CursorAtHome(void)
{
    lcd_send_cmd(0x02);
    _delay_ms(2);
}


/*display/Cursor Shift関数群
 　指定数値だけDisplay(=表示番地)をずらす
 　　　　　　　Cursor (=入力カーソル位置)をずらす
 */
void lcd_DisplayShift_L(int n){
    int i;
    for (i=0;i<n;i++){
        lcd_send_cmd(0x18);
    }
}
void lcd_DisplayShift_R(int n){
    int i;
    for (i=0;i<n;i++){
        lcd_send_cmd(0x1C);
    }
}
void lcd_CursorShift_L(int n){
    int i;
    for (i=0;i<n;i++){
        lcd_send_cmd(0x10);
    }
}
void lcd_CursorShift_R(int n){
    int i;
    for (i=0;i<n;i++){
        lcd_send_cmd(0x14);
    }
}

/*表示番地を変更する*/
void lcd_Set_DisplayPosition(char address){
    lcd_send_cmd(0x80 | address);
}



/************************************************************
 *送受信系関数
 *lcd_setDB
 *lcd_send_cmd
 *lcd_send_initcmd
 *lcd_ready
 *
 ************************************************************/

/*DB用ポートの出力値をセット上位4bit使用*/
void lcd_setDB(unsigned char data)
{
    DB_lcd =DB_SetData_lcd(data);
}

/*8bit制御コマンド送信関数 busy flgで待機したかったが動かないのでdelay*/
void lcd_send_cmd(unsigned char cmd){
    lcd_setDB(cmd); //上位4bitを送信
    RSW_Set_W_CMD;
    E_Set_Edge;
    lcd_setDB(cmd<<4); //下位4bitを送信
    E_Set_Edge;
    
#ifdef USE_BUSYFLG
    while( lcd_ready() & 0x80); //busy flgがclearされるまで待つ
#else
    _delay_us(50);
#endif
    
}

/*4bit制御コマンド送信関数 上位4bitを送信　別途delayで待機が必要*/
void lcd_send_initcmd(unsigned char cmd){
    //DBにセットしてそれを読ませる
    lcd_setDB(cmd);
    RSW_Set_W_CMD;
    E_Set_Edge;
}

/*lcdからデータを受信する関数　busyflgを得る目的*/
unsigned char lcd_ready(void){
    char inData=0;
    DB_TRIS_lcd=TRIS_mask_lcd | TRIS_INI;//入力にする
    RSW_Set_R_CMD;               //lcdから読み取り
    
    b_SET(E_PORT,E_BIT);
    inData=DB_ReadData_lcd;	//入力　上位bit
    b_CLR(E_PORT,E_BIT);
    b_SET(E_PORT,E_BIT);
    inData|=DB_ReadData_lcd>>4;
    b_CLR(E_PORT,E_BIT);
    DB_TRIS_lcd=TRIS_INI;//出力に戻す
    return inData;
}


/************************************************************
 *初期設定用関数
 *lcd_init
 *lcd_FunctionSet
 *lcd_EntryMode_Set
 *lcd_DisplayControl
 *
 ************************************************************/


void lcd_init(void){
    fdev_setup_stream(stdout,lcd_put_char, NULL, _FDEV_SETUP_WRITE);
    /*IO 設定バックアップ*/
    TRIS_INI=DB_TRIS_lcd | TRIS_mask_lcd;
    
    /*初期化*/
    _delay_ms(15);
    lcd_send_initcmd(0x30);//0b0011 0000 8bitモードに設定
    _delay_ms(5);
    lcd_send_initcmd(0x30);
    _delay_us(120);
    lcd_send_initcmd(0x30);
    _delay_us(120);
    //ここからbusyflgでチェックできる
    lcd_send_initcmd(0x20);//0b0010 0000 4bitモードに設定
    _delay_us(50);
    lcd_FunctionSet(0,1,1); //0b0010 1000 4bitモード/2行表示/フォント5*10dotsに設定
    //lcd_FunctionSet(0,1,0);//lcd_send_cmd(0x2E);//フォント5*7dotsの場合
    lcd_DisplayControl(0,0,0);//ディスプレイなどをOFFに
    lcd_ClearDisplay();
    
    lcd_EntryMode_Set(1,0);//エントリーモード設定 Increment,display shift off
    lcd_DisplayControl(1,0,0);//ディスプレイをON、カーソル非表示、点滅OFF
}

/*datalen : 1/0 8bit     / 4bit
 linenum : 1/0 2line    / 1line
 font    : 1/0 5*10dots / 5*7dots */
void lcd_FunctionSet(char datalen,char linenum,char font){
    unsigned char cmd=0x20;
    if(datalen)cmd|=0x01<<4;
    if(linenum)cmd|=0x01<<3;
    if(font)cmd|=0x01<<2;
    lcd_send_cmd(cmd);
}

/*EntryMode設定
 * i_d : cursor move direction
 *         1 increment
 *         0 decrement
 *shift:   1 with display shift
 *         0 not shift the display  */
void lcd_EntryMode_Set(char i_d,char shift){
    unsigned char cmd=0x04;
    if(i_d)cmd|=0x02;
    if(shift)cmd|=0x01;
    lcd_send_cmd(cmd);
}

/*Display表示設定
 * display : 0 or 1 OFF/ON
 * cursor  : 0 or 1 cursor hide/Show
 * blink   : 0 or 1 blink of cursor posistion OFF/ON
 */
void lcd_DisplayControl(char display,char cursor,char blink){
    unsigned char cmd=0x08;
    if(display)cmd|=0x04;
    if(cursor)cmd|=0x02;
    if(blink)cmd|=0x01;
    lcd_send_cmd(cmd);
}
