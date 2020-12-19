
0x000023d0 nop; //----------------
0x000023e8 nop; //----------------
0x00002400 move.l 0x2140,A0		; //10,
0x00002418 move.l 0x2220,A1		; //0,
0x00002430 move.b (A0),(A1)
0x00002448 move.l 0x21a0,A0		; //20,
0x00002460 move.b (A0),D0
0x00002478 move.l 0x2220,A0		; //0,
0x00002490 move.b (A0),D1
0x000024a8 add.b D0,D1
0x000024c0 move.l 0x2220,A0		; //0,
0x000024d8 move.b D1,(A0)
0x000024f0 nop; //----------------
0x00002508 move.l 0x21a0,A0		; //20,
0x00002520 move.l 0x2250,A1		; //0,
0x00002538 move.b (A0),(A1)
0x00002550 xor.q D0,D0
0x00002568 move.l 0x2220,A0		; //0,
0x00002580 move.b (A0),D0
0x00002598 xor.q D1,D1
0x000025b0 move.l 0x2250,A0		; //0,
0x000025c8 move.b (A0),D1
0x000025e0 mul.w D0,D1
0x000025f8 move.l 0x2250,A0		; //0,
0x00002610 move.w D1,(A0)
0x00002628 xor.q D0,D0
0x00002640 move.l 0x21a0,A0		; //20,
0x00002658 move.b (A0),D0
0x00002670 xor.q D1,D1
0x00002688 move.l 0x2250,A0		; //0,
0x000026a0 move.w (A0),D1
0x000026b8 mul.l D0,D1
0x000026d0 move.l 0x2250,A0		; //0,
0x000026e8 move.l D1,(A0)
0x00002700 xor.q D0,D0
0x00002718 move.l 0x2140,A0		; //10,
0x00002730 move.b (A0),D0
0x00002748 xor.q D1,D1
0x00002760 move.l 0x2250,A0		; //0,
0x00002778 move.l (A0),D1
0x00002790 div.q D0,D1
0x000027a8 move.l 0x2250,A0		; //0,
0x000027c0 move.q D1,(A0)
0x000027d8 move.l 0x2140,A0		; //10,
0x000027f0 move.l 0x2280,A1		; //0,
0x00002808 move.b (A0),(A1)
0x00002820 move.l 0x2250,A0		; //0,
0x00002838 move.q (A0),D0
0x00002850 xor.q D1,D1
0x00002868 move.l 0x2280,A0		; //0,
0x00002880 move.b (A0),D1
0x00002898 add.q D0,D1
0x000028b0 move.l 0x2280,A0		; //0,
0x000028c8 move.q D1,(A0)
0x000028e0 nop; //----------------
0x000028f8 move.l 0x2280,A0		; //0,
0x00002910 move.l 0x2310,A1		; //0,
0x00002928 move.q (A0),(A1)
0x00002940 xor.q D0,D0
0x00002958 move.l 0x22d0,A0		; //4,
0x00002970 move.b (A0),D0
0x00002988 move.l 0x2310,A0		; //0,
0x000029a0 move.q (A0),D1
0x000029b8 rem.q D0,D1
0x000029d0 move.l 0x2310,A0		; //0,
0x000029e8 move.q D1,(A0)
0x00002a00 nop; //----------------
0x00002a18 move.q A7,D7
0x00002a30 move.l 0x2360,-(A7)
0x00002a48 move.l 0x21c0,-(A7)
0x00002a60 move.l 0x2390,A0		; //-1,
0x00002a78 jsr print
0x00002a90 move.q D7,A7
0x00002aa8 nop; //----------------
0x00002ac0 nop; //----------------
0x00002ad8 nop; //----------------