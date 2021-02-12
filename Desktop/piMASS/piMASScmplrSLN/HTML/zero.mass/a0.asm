
0x00002470 nop; //----------------
0x00002488 nop; //----------------
0x000024a0 move.l 0x2140,A0		; //10,
0x000024b8 move.l 0x2220,A1		; //0,
0x000024d0 move.b (A0),(A1)
0x000024e8 move.l 0x21a0,A0		; //20,
0x00002500 move.b (A0),D0
0x00002518 move.l 0x2220,A0		; //0,
0x00002530 move.b (A0),D1
0x00002548 add.b D0,D1
0x00002560 move.l 0x2220,A0		; //0,
0x00002578 move.b D1,(A0)
0x00002590 nop; //----------------
0x000025a8 move.l 0x21a0,A0		; //20,
0x000025c0 move.l 0x2250,A1		; //0,
0x000025d8 move.b (A0),(A1)
0x000025f0 xor.q D0,D0
0x00002608 move.l 0x2220,A0		; //0,
0x00002620 move.b (A0),D0
0x00002638 xor.q D1,D1
0x00002650 move.l 0x2250,A0		; //0,
0x00002668 move.b (A0),D1
0x00002680 mul.w D0,D1
0x00002698 move.l 0x2250,A0		; //0,
0x000026b0 move.w D1,(A0)
0x000026c8 xor.q D0,D0
0x000026e0 move.l 0x21a0,A0		; //20,
0x000026f8 move.b (A0),D0
0x00002710 xor.q D1,D1
0x00002728 move.l 0x2250,A0		; //0,
0x00002740 move.w (A0),D1
0x00002758 mul.l D0,D1
0x00002770 move.l 0x2250,A0		; //0,
0x00002788 move.l D1,(A0)
0x000027a0 xor.q D0,D0
0x000027b8 move.l 0x2140,A0		; //10,
0x000027d0 move.b (A0),D0
0x000027e8 xor.q D1,D1
0x00002800 move.l 0x2250,A0		; //0,
0x00002818 move.l (A0),D1
0x00002830 div.q D0,D1
0x00002848 move.l 0x2250,A0		; //0,
0x00002860 move.q D1,(A0)
0x00002878 move.l 0x2140,A0		; //10,
0x00002890 move.l 0x2280,A1		; //0,
0x000028a8 move.b (A0),(A1)
0x000028c0 move.l 0x2250,A0		; //0,
0x000028d8 move.q (A0),D0
0x000028f0 xor.q D1,D1
0x00002908 move.l 0x2280,A0		; //0,
0x00002920 move.b (A0),D1
0x00002938 add.q D0,D1
0x00002950 move.l 0x2280,A0		; //0,
0x00002968 move.q D1,(A0)
0x00002980 nop; //----------------
0x00002998 move.l 0x2280,A0		; //0,
0x000029b0 move.l 0x2310,A1		; //0,
0x000029c8 move.q (A0),(A1)
0x000029e0 xor.q D0,D0
0x000029f8 move.l 0x22d0,A0		; //4,
0x00002a10 move.b (A0),D0
0x00002a28 move.l 0x2310,A0		; //0,
0x00002a40 move.q (A0),D1
0x00002a58 rem.q D0,D1
0x00002a70 move.l 0x2310,A0		; //0,
0x00002a88 move.q D1,(A0)
0x00002aa0 nop; //----------------
0x00002ab8 move.q A7,D7
0x00002ad0 move.l 0x2360,-(A7)
0x00002ae8 move.l 0x21c0,-(A7)
0x00002b00 move.l 0x2390,A0		; //9296,
0x00002b18 jsr print
0x00002b30 move.q D7,A7
0x00002b48 nop; //----------------
0x00002b60 nop; //----------------
0x00002b78 nop; //----------------
0x00002b90 move.q A7,D7
0x00002ba8 move.l 0x23f0,-(A7)
0x00002bc0 move.l 0x2320,-(A7)
0x00002bd8 move.l 0x2440,A0		; //0,
0x00002bf0 jsr fps
0x00002c08 move.l 0x2420,-(A7)
0x00002c20 move.l 0x2450,A0		; //-1,
0x00002c38 jsr print
0x00002c50 move.q D7,A7
0x00002c68 nop; //----------------
0x00002c80 nop; //----------------