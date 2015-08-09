AVR Microcontroller Projects and Libraries
==========================================
AVRマイコン向けのプロジェクトやライブラリなど。  
少しずつ増やしていきます。

開発環境
---------
主にMacOS X CrossPack for AVR (20131216) の avr-gcc  
avr-libc version 1.8.0  
avrdude  version 6.0.1  
たまに Windows8.1 Atmel studio 6 になるかもしれない

使い方
---------
Makefileがあるところで make するとコンパイルできる。  
.hexファイルがつけてある場合はそれを使って直接書き込んでも良い。  
fuseビット設定はMakefile内に書いてある。

Makefileでライタ等を設定すれば  
`make install`  
でfuse書き込み、プログラム書き込みをしてくれるようになる。


ライセンス
---------
MIT Lisense

hexファイルに関して、avr-libcのライセンス表記はLISENSE_avr_libc にあります。
