
0x00002850 nop; //----------------
0x00002868 nop; //----------------
0x00002880 move.l 0x21e0,A0		; //0,
0x00002898 jsr ix
0x000028b0 move.l 0x21e0,A0		; //0,
0x000028c8 move.l 0x2210,A1		; //0,
0x000028e0 move.L (A0),(A1)
0x000028f8 xor.q D0,D0
0x00002910 move.l 0x2140,A0		; //640,
0x00002928 move.w (A0),D0
0x00002940 move.l 0x2210,A0		; //0,
0x00002958 move.L (A0),D1
0x00002970 extL.Q D1
0x00002988 mul.Q D0,D1
0x000029a0 move.l 0x2210,A0		; //0,
0x000029b8 move.Q D1,(A0)
0x000029d0 move.l 0x2220,A0		; //0,
0x000029e8 jsr iw
0x00002a00 move.l 0x2220,A0		; //0,
0x00002a18 move.L (A0),D0
0x00002a30 extL.Q D0
0x00002a48 move.l 0x2210,A0		; //0,
0x00002a60 move.Q (A0),D1
0x00002a78 div.Q D0,D1
0x00002a90 move.l 0x2210,A0		; //0,
0x00002aa8 move.Q D1,(A0)
0x00002ac0 nop; //----------------
0x00002ad8 move.l 0x2140,A0		; //640,
0x00002af0 move.l 0x22b0,A1		; //0,
0x00002b08 move.w (A0),(A1)
0x00002b20 move.l 0x22c0,A0		; //0,
0x00002b38 jsr iy
0x00002b50 move.l 0x22c0,A0		; //0,
0x00002b68 move.L (A0),D0
0x00002b80 extL.Q D0
0x00002b98 xor.q D1,D1
0x00002bb0 move.l 0x22b0,A0		; //0,
0x00002bc8 move.w (A0),D1
0x00002be0 mul.Q D0,D1
0x00002bf8 move.l 0x22b0,A0		; //0,
0x00002c10 move.Q D1,(A0)
0x00002c28 move.l 0x22d0,A0		; //0,
0x00002c40 jsr ih
0x00002c58 move.l 0x22d0,A0		; //0,
0x00002c70 move.L (A0),D0
0x00002c88 extL.Q D0
0x00002ca0 move.l 0x22b0,A0		; //0,
0x00002cb8 move.Q (A0),D1
0x00002cd0 div.Q D0,D1
0x00002ce8 move.l 0x22b0,A0		; //0,
0x00002d00 move.Q D1,(A0)
0x00002d18 nop; //----------------
0x00002d30 move.l 0x23a0,A0		; //0,
0x00002d48 jsr ia
0x00002d60 move.l 0x23a0,A0		; //0,
0x00002d78 move.l 0x23d0,A1		; //0,
0x00002d90 move.L (A0),(A1)
0x00002da8 xor.q D0,D0
0x00002dc0 move.l 0x2380,A0		; //1,
0x00002dd8 move.b (A0),D0
0x00002df0 move.l 0x23d0,A0		; //0,
0x00002e08 move.L (A0),D1
0x00002e20 sub.L D0,D1
0x00002e38 move.l 0x23d0,A0		; //0,
0x00002e50 move.L D1,(A0)
0x00002e68 nop; //----------------
0x00002e80 move.l 0x23d0,A0		; //0,
0x00002e98 move.l 0x2440,A1		; //0,
0x00002eb0 move.L (A0),(A1)
0x00002ec8 xor.q D0,D0
0x00002ee0 move.l 0x2400,A0		; //1000,
0x00002ef8 move.w (A0),D0
0x00002f10 move.l 0x2440,A0		; //0,
0x00002f28 move.L (A0),D1
0x00002f40 extL.Q D1
0x00002f58 mul.Q D0,D1
0x00002f70 move.l 0x2440,A0		; //0,
0x00002f88 move.Q D1,(A0)
0x00002fa0 move.l 0x2490,A0		; //0,
0x00002fb8 jsr in
0x00002fd0 move.l 0x2490,A0		; //0,
0x00002fe8 move.l 0x24c0,A1		; //0,
0x00003000 move.L (A0),(A1)
0x00003018 xor.q D0,D0
0x00003030 move.l 0x2380,A0		; //1,
0x00003048 move.b (A0),D0
0x00003060 move.l 0x24c0,A0		; //0,
0x00003078 move.L (A0),D1
0x00003090 sub.L D0,D1
0x000030a8 move.l 0x24c0,A0		; //0,
0x000030c0 move.L D1,(A0)
0x000030d8 nop; //----------------
0x000030f0 move.l 0x24c0,A0		; //0,
0x00003108 move.l 0x2530,A1		; //0,
0x00003120 move.L (A0),(A1)
0x00003138 xor.q D0,D0
0x00003150 move.l 0x24f0,A0		; //10000,
0x00003168 move.w (A0),D0
0x00003180 move.l 0x2530,A0		; //0,
0x00003198 move.L (A0),D1
0x000031b0 extL.Q D1
0x000031c8 mul.Q D0,D1
0x000031e0 move.l 0x2530,A0		; //0,
0x000031f8 move.Q D1,(A0)
0x00003210 move.l 0x2540,A0		; //0,
0x00003228 jsr mlb
0x00003240 move.l 0x2540,A0		; //0,
0x00003258 move.l 0x2570,A1		; //0,
0x00003270 move.L (A0),(A1)
0x00003288 move.l 0x2440,A0		; //0,
0x000032a0 move.Q (A0),D0
0x000032b8 move.l 0x2570,A0		; //0,
0x000032d0 move.L (A0),D1
0x000032e8 extL.Q D1
0x00003300 add.Q D0,D1
0x00003318 move.l 0x2570,A0		; //0,
0x00003330 move.Q D1,(A0)
0x00003348 move.l 0x2530,A0		; //0,
0x00003360 move.Q (A0),D0
0x00003378 move.l 0x2570,A0		; //0,
0x00003390 move.Q (A0),D1
0x000033a8 add.Q D0,D1
0x000033c0 move.l 0x2570,A0		; //0,
0x000033d8 move.Q D1,(A0)
0x000033f0 nop; //----------------
0x00003408 nop; //----------------
0x00003420 nop; //----------------
0x00003438 nop; //----------------
0x00003450 nop; //----------------
0x00003468 move.q A7,D7
0x00003480 move.l 0x26d0,-(A7)
0x00003498 move.l 0x2180,-(A7)
0x000034b0 move.l 0x2230,-(A7)
0x000034c8 move.l 0x2580,-(A7)
0x000034e0 move.l 0x2720,-(A7)
0x000034f8 move.l 0x2760,A0		; //-1,
0x00003510 jsr print
0x00003528 move.q D7,A7
0x00003540 nop; //----------------
0x00003558 nop; //----------------
0x00003570 nop; //----------------
0x00003588 nop; //----------------
0x000035a0 nop; //----------------