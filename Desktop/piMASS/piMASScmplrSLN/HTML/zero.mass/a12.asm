
0x000023b0 move.l 0x2180,A0		; //0,
0x000023c8 jsr in
0x000023e0 move.l 0x2180,A0		; //0,
0x000023f8 move.l 0x21b0,A1		; //0,
0x00002410 move.L (A0),(A1)
0x00002428 xor.q D0,D0
0x00002440 move.l 0x2160,A0		; //10,
0x00002458 move.b (A0),D0
0x00002470 move.l 0x21b0,A0		; //0,
0x00002488 move.L (A0),D1
0x000024a0 cmp.L D0,D1
0x000024b8 seq.L D1
0x000024d0 and.L 0x1,D1
0x000024e8 move.l 0x21b0,A0		; //0,
0x00002500 move.L D1,(A0)
0x00002518 nop; //----------------
0x00002530 move.l 0x2180,A0		; //0,
0x00002548 jsr in
0x00002560 move.l 0x2180,A0		; //0,
0x00002578 move.l 0x2220,A1		; //0,
0x00002590 move.L (A0),(A1)
0x000025a8 xor.q D0,D0
0x000025c0 move.l 0x2160,A0		; //10,
0x000025d8 move.b (A0),D0
0x000025f0 move.l 0x2220,A0		; //0,
0x00002608 move.L (A0),D1
0x00002620 cmp.L D0,D1
0x00002638 seq.L D1
0x00002650 and.L 0x1,D1
0x00002668 move.l 0x2220,A0		; //0,
0x00002680 move.L D1,(A0)
0x00002698 nop; //----------------
0x000026b0 move.l 0x22b0,A0		; //0,
0x000026c8 jsr ia
0x000026e0 move.l 0x22b0,A0		; //0,
0x000026f8 move.l 0x22e0,A1		; //0,
0x00002710 move.L (A0),(A1)
0x00002728 xor.q D0,D0
0x00002740 move.l 0x2290,A0		; //1,
0x00002758 move.b (A0),D0
0x00002770 move.l 0x22e0,A0		; //0,
0x00002788 move.L (A0),D1
0x000027a0 cmp.L D0,D1
0x000027b8 sne.L D1
0x000027d0 and.L 0x1,D1
0x000027e8 move.l 0x22e0,A0		; //0,
0x00002800 move.L D1,(A0)
0x00002818 nop; //----------------
0x00002830 move.l 0x22e0,A0		; //0,
0x00002848 move.l 0x2310,A1		; //0,
0x00002860 move.L (A0),(A1)
0x00002878 xor.q D0,D0
0x00002890 move.l 0x2160,A0		; //10,
0x000028a8 move.b (A0),D0
0x000028c0 move.l 0x2310,A0		; //0,
0x000028d8 move.L (A0),D1
0x000028f0 extL.Q D1
0x00002908 mul.Q D0,D1
0x00002920 move.l 0x2310,A0		; //0,
0x00002938 move.Q D1,(A0)
0x00002950 move.l 0x2220,A0		; //0,
0x00002968 move.l 0x2340,A1		; //0,
0x00002980 move.L (A0),(A1)
0x00002998 move.l 0x2310,A0		; //0,
0x000029b0 move.Q (A0),D0
0x000029c8 move.l 0x2340,A0		; //0,
0x000029e0 move.L (A0),D1
0x000029f8 extL.Q D1
0x00002a10 add.Q D0,D1
0x00002a28 move.l 0x2340,A0		; //0,
0x00002a40 move.Q D1,(A0)
0x00002a58 nop; //----------------
0x00002a70 nop; //----------------