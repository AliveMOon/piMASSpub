
0x000024e0 nop; //----------------
0x000024f8 nop; //----------------
0x00002510 move.l 0x2160,A0		; //10,
0x00002528 move.l 0x2290,A1		; //0,
0x00002540 move.b (A0),(A1)
0x00002558 move.l 0x21e0,A0		; //20,
0x00002570 move.b (A0),D0
0x00002588 move.l 0x2290,A0		; //0,
0x000025a0 move.b (A0),D1
0x000025b8 add.b D0,D1
0x000025d0 move.l 0x2290,A0		; //0,
0x000025e8 move.b D1,(A0)
0x00002600 nop; //----------------
0x00002618 move.l 0x21e0,A0		; //20,
0x00002630 move.l 0x22d0,A1		; //0,
0x00002648 move.b (A0),(A1)
0x00002660 xor.q D0,D0
0x00002678 move.l 0x2290,A0		; //0,
0x00002690 move.b (A0),D0
0x000026a8 xor.q D1,D1
0x000026c0 move.l 0x22d0,A0		; //0,
0x000026d8 move.b (A0),D1
0x000026f0 mul.w D0,D1
0x00002708 move.l 0x22d0,A0		; //0,
0x00002720 move.w D1,(A0)
0x00002738 xor.q D0,D0
0x00002750 move.l 0x21e0,A0		; //20,
0x00002768 move.b (A0),D0
0x00002780 xor.q D1,D1
0x00002798 move.l 0x22d0,A0		; //0,
0x000027b0 move.w (A0),D1
0x000027c8 mul.l D0,D1
0x000027e0 move.l 0x22d0,A0		; //0,
0x000027f8 move.l D1,(A0)
0x00002810 xor.q D0,D0
0x00002828 move.l 0x2160,A0		; //10,
0x00002840 move.b (A0),D0
0x00002858 xor.q D1,D1
0x00002870 move.l 0x22d0,A0		; //0,
0x00002888 move.l (A0),D1
0x000028a0 div.q D0,D1
0x000028b8 move.l 0x22d0,A0		; //0,
0x000028d0 move.q D1,(A0)
0x000028e8 move.l 0x2160,A0		; //10,
0x00002900 move.l 0x2310,A1		; //0,
0x00002918 move.b (A0),(A1)
0x00002930 move.l 0x22d0,A0		; //0,
0x00002948 move.q (A0),D0
0x00002960 xor.q D1,D1
0x00002978 move.l 0x2310,A0		; //0,
0x00002990 move.b (A0),D1
0x000029a8 add.q D0,D1
0x000029c0 move.l 0x2310,A0		; //0,
0x000029d8 move.q D1,(A0)
0x000029f0 nop; //----------------
0x00002a08 move.l 0x2310,A0		; //0,
0x00002a20 move.l 0x23d0,A1		; //0,
0x00002a38 move.q (A0),(A1)
0x00002a50 xor.q D0,D0
0x00002a68 move.l 0x2380,A0		; //4,
0x00002a80 move.b (A0),D0
0x00002a98 move.l 0x23d0,A0		; //0,
0x00002ab0 move.q (A0),D1
0x00002ac8 rem.q D0,D1
0x00002ae0 move.l 0x23d0,A0		; //0,
0x00002af8 move.q D1,(A0)
0x00002b10 nop; //----------------
0x00002b28 move.q A7,D7
0x00002b40 move.l 0x2440,-(A7)
0x00002b58 move.l 0x2200,-(A7)
0x00002b70 move.l 0x2480,A0		; //-1,
0x00002b88 jsr print
0x00002ba0 move.q D7,A7
0x00002bb8 nop; //----------------
0x00002bd0 nop; //----------------
0x00002be8 nop; //----------------