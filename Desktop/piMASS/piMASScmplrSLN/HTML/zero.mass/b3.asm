
0x00002470 move.l 0x21a0,A0		; //0,
0x00002488 jsr ix
0x000024a0 move.l 0x21a0,A0		; //0,
0x000024b8 move.l 0x21d0,A1		; //0,
0x000024d0 move.L (A0),(A1)
0x000024e8 xor.q D0,D0
0x00002500 move.l 0x2160,A0		; //1640,
0x00002518 move.w (A0),D0
0x00002530 move.l 0x21d0,A0		; //0,
0x00002548 move.L (A0),D1
0x00002560 extL.Q D1
0x00002578 mul.Q D0,D1
0x00002590 move.l 0x21d0,A0		; //0,
0x000025a8 move.Q D1,(A0)
0x000025c0 move.l 0x21e0,A0		; //0,
0x000025d8 jsr iw
0x000025f0 move.l 0x21e0,A0		; //0,
0x00002608 move.L (A0),D0
0x00002620 extL.Q D0
0x00002638 move.l 0x21d0,A0		; //0,
0x00002650 move.Q (A0),D1
0x00002668 div.Q D0,D1
0x00002680 move.l 0x21d0,A0		; //0,
0x00002698 move.Q D1,(A0)
0x000026b0 nop; //----------------
0x000026c8 move.l 0x2230,A0		; //1480,
0x000026e0 move.l 0x22b0,A1		; //0,
0x000026f8 move.w (A0),(A1)
0x00002710 move.l 0x22c0,A0		; //0,
0x00002728 jsr iy
0x00002740 move.l 0x22c0,A0		; //0,
0x00002758 move.L (A0),D0
0x00002770 extL.Q D0
0x00002788 xor.q D1,D1
0x000027a0 move.l 0x22b0,A0		; //0,
0x000027b8 move.w (A0),D1
0x000027d0 mul.Q D0,D1
0x000027e8 move.l 0x22b0,A0		; //0,
0x00002800 move.Q D1,(A0)
0x00002818 move.l 0x22d0,A0		; //0,
0x00002830 jsr ih
0x00002848 move.l 0x22d0,A0		; //0,
0x00002860 move.L (A0),D0
0x00002878 extL.Q D0
0x00002890 move.l 0x22b0,A0		; //0,
0x000028a8 move.Q (A0),D1
0x000028c0 div.Q D0,D1
0x000028d8 move.l 0x22b0,A0		; //0,
0x000028f0 move.Q D1,(A0)
0x00002908 nop; //----------------
0x00002920 move.q A7,D7
0x00002938 move.l 0x2320,-(A7)
0x00002950 move.l 0x21a0,A0		; //0,
0x00002968 jsr ix
0x00002980 move.l 0x2120,-(A7)
0x00002998 move.l 0x22c0,A0		; //0,
0x000029b0 jsr iy
0x000029c8 move.l 0x2250,-(A7)
0x000029e0 move.l 0x23d0,A0		; //0,
0x000029f8 jsr ia
0x00002a10 move.l 0x2390,-(A7)
0x00002a28 move.l 0x23e0,A0		; //0,
0x00002a40 jsr in
0x00002a58 move.l 0x23b0,-(A7)
0x00002a70 move.l 0x2100,-(A7)
0x00002a88 move.l 0x21f0,-(A7)
0x00002aa0 move.l 0x21e0,A0		; //0,
0x00002ab8 jsr iw
0x00002ad0 move.l 0x2180,-(A7)
0x00002ae8 move.l 0x22d0,A0		; //0,
0x00002b00 jsr ih
0x00002b18 move.l 0x2270,-(A7)
0x00002b30 move.l 0x23f0,A0		; //-1,
0x00002b48 jsr print
0x00002b60 move.q D7,A7
0x00002b78 nop; //----------------
0x00002b90 nop; //----------------
0x00002ba8 nop; //----------------