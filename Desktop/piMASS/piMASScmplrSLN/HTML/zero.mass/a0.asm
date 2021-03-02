
0x000024a0 move.l 0x2140,A0		; //0,
0x000024b8 jsr ix
0x000024d0 nop; //----------------
0x000024e8 nop; //----------------
0x00002500 move.l 0x2140,A0		; //0,
0x00002518 move.l 0x2210,A1		; //0,
0x00002530 move.L (A0),(A1)
0x00002548 xor.q D0,D0
0x00002560 move.l 0x2190,A0		; //20,
0x00002578 move.b (A0),D0
0x00002590 move.l 0x2210,A0		; //0,
0x000025a8 move.L (A0),D1
0x000025c0 add.L D0,D1
0x000025d8 move.l 0x2210,A0		; //0,
0x000025f0 move.L D1,(A0)
0x00002608 nop; //----------------
0x00002620 move.l 0x2190,A0		; //20,
0x00002638 move.l 0x2240,A1		; //0,
0x00002650 move.b (A0),(A1)
0x00002668 move.l 0x2210,A0		; //0,
0x00002680 move.L (A0),D0
0x00002698 extL.Q D0
0x000026b0 xor.q D1,D1
0x000026c8 move.l 0x2240,A0		; //0,
0x000026e0 move.b (A0),D1
0x000026f8 mul.Q D0,D1
0x00002710 move.l 0x2240,A0		; //0,
0x00002728 move.Q D1,(A0)
0x00002740 xor.q D0,D0
0x00002758 move.l 0x2190,A0		; //20,
0x00002770 move.b (A0),D0
0x00002788 move.l 0x2240,A0		; //0,
0x000027a0 move.Q (A0),D1
0x000027b8 mul.Q D0,D1
0x000027d0 move.l 0x2240,A0		; //0,
0x000027e8 move.Q D1,(A0)
0x00002800 move.l 0x2140,A0		; //0,
0x00002818 move.L (A0),D0
0x00002830 extL.Q D0
0x00002848 move.l 0x2240,A0		; //0,
0x00002860 move.Q (A0),D1
0x00002878 div.Q D0,D1
0x00002890 move.l 0x2240,A0		; //0,
0x000028a8 move.Q D1,(A0)
0x000028c0 move.l 0x2140,A0		; //0,
0x000028d8 move.l 0x2270,A1		; //0,
0x000028f0 move.L (A0),(A1)
0x00002908 move.l 0x2240,A0		; //0,
0x00002920 move.Q (A0),D0
0x00002938 move.l 0x2270,A0		; //0,
0x00002950 move.L (A0),D1
0x00002968 extL.Q D1
0x00002980 add.Q D0,D1
0x00002998 move.l 0x2270,A0		; //0,
0x000029b0 move.Q D1,(A0)
0x000029c8 nop; //----------------
0x000029e0 move.l 0x2270,A0		; //0,
0x000029f8 move.l 0x2300,A1		; //0,
0x00002a10 move.Q (A0),(A1)
0x00002a28 xor.q D0,D0
0x00002a40 move.l 0x22c0,A0		; //4,
0x00002a58 move.b (A0),D0
0x00002a70 move.l 0x2300,A0		; //0,
0x00002a88 move.Q (A0),D1
0x00002aa0 rem.Q D0,D1
0x00002ab8 move.l 0x2300,A0		; //0,
0x00002ad0 move.Q D1,(A0)
0x00002ae8 nop; //----------------
0x00002b00 move.q A7,D7
0x00002b18 move.l 0x2350,-(A7)
0x00002b30 move.l 0x21b0,-(A7)
0x00002b48 move.l 0x2380,A0		; //9344,
0x00002b60 jsr print
0x00002b78 move.q D7,A7
0x00002b90 nop; //----------------
0x00002ba8 nop; //----------------
0x00002bc0 nop; //----------------
0x00002bd8 move.q A7,D7
0x00002bf0 move.l 0x2420,-(A7)
0x00002c08 move.l 0x2310,-(A7)
0x00002c20 move.l 0x2470,A0		; //0,
0x00002c38 jsr fps
0x00002c50 move.l 0x2450,-(A7)
0x00002c68 move.l 0x2480,A0		; //-1,
0x00002c80 jsr print
0x00002c98 move.q D7,A7
0x00002cb0 nop; //----------------
0x00002cc8 nop; //----------------