
0x00002960 nop; //----------------
0x00002978 nop; //----------------
0x00002990 move.l 0x2250,A0		; //0,
0x000029a8 jsr ix
0x000029c0 move.l 0x2250,A0		; //0,
0x000029d8 move.l 0x2280,A1		; //0,
0x000029f0 move.L (A0),(A1)
0x00002a08 xor.q D0,D0
0x00002a20 move.l 0x2210,A0		; //640,
0x00002a38 move.w (A0),D0
0x00002a50 move.l 0x2280,A0		; //0,
0x00002a68 move.L (A0),D1
0x00002a80 extL.Q D1
0x00002a98 mul.Q D0,D1
0x00002ab0 move.l 0x2280,A0		; //0,
0x00002ac8 move.Q D1,(A0)
0x00002ae0 move.l 0x2290,A0		; //0,
0x00002af8 jsr iw
0x00002b10 move.l 0x2290,A0		; //0,
0x00002b28 move.L (A0),D0
0x00002b40 extL.Q D0
0x00002b58 move.l 0x2280,A0		; //0,
0x00002b70 move.Q (A0),D1
0x00002b88 div.Q D0,D1
0x00002ba0 move.l 0x2280,A0		; //0,
0x00002bb8 move.Q D1,(A0)
0x00002bd0 nop; //----------------
0x00002be8 move.l 0x22e0,A0		; //960,
0x00002c00 move.l 0x2360,A1		; //0,
0x00002c18 move.w (A0),(A1)
0x00002c30 move.l 0x2370,A0		; //0,
0x00002c48 jsr iy
0x00002c60 move.l 0x2370,A0		; //0,
0x00002c78 move.L (A0),D0
0x00002c90 extL.Q D0
0x00002ca8 xor.q D1,D1
0x00002cc0 move.l 0x2360,A0		; //0,
0x00002cd8 move.w (A0),D1
0x00002cf0 mul.Q D0,D1
0x00002d08 move.l 0x2360,A0		; //0,
0x00002d20 move.Q D1,(A0)
0x00002d38 move.l 0x2380,A0		; //0,
0x00002d50 jsr ih
0x00002d68 move.l 0x2380,A0		; //0,
0x00002d80 move.L (A0),D0
0x00002d98 extL.Q D0
0x00002db0 move.l 0x2360,A0		; //0,
0x00002dc8 move.Q (A0),D1
0x00002de0 div.Q D0,D1
0x00002df8 move.l 0x2360,A0		; //0,
0x00002e10 move.Q D1,(A0)
0x00002e28 nop; //----------------
0x00002e40 move.l 0x2450,A0		; //0,
0x00002e58 jsr ia
0x00002e70 move.l 0x2450,A0		; //0,
0x00002e88 move.l 0x2480,A1		; //0,
0x00002ea0 move.L (A0),(A1)
0x00002eb8 xor.q D0,D0
0x00002ed0 move.l 0x2430,A0		; //2,
0x00002ee8 move.b (A0),D0
0x00002f00 move.l 0x2480,A0		; //0,
0x00002f18 move.L (A0),D1
0x00002f30 sub.L D0,D1
0x00002f48 move.l 0x2480,A0		; //0,
0x00002f60 move.L D1,(A0)
0x00002f78 nop; //----------------
0x00002f90 move.l 0x2480,A0		; //0,
0x00002fa8 move.l 0x24f0,A1		; //0,
0x00002fc0 move.L (A0),(A1)
0x00002fd8 xor.q D0,D0
0x00002ff0 move.l 0x24b0,A0		; //1000,
0x00003008 move.w (A0),D0
0x00003020 move.l 0x24f0,A0		; //0,
0x00003038 move.L (A0),D1
0x00003050 extL.Q D1
0x00003068 mul.Q D0,D1
0x00003080 move.l 0x24f0,A0		; //0,
0x00003098 move.Q D1,(A0)
0x000030b0 move.l 0x2580,A0		; //0,
0x000030c8 jsr in
0x000030e0 move.l 0x2580,A0		; //0,
0x000030f8 move.l 0x25b0,A1		; //0,
0x00003110 move.L (A0),(A1)
0x00003128 xor.q D0,D0
0x00003140 move.l 0x2560,A0		; //3,
0x00003158 move.b (A0),D0
0x00003170 move.l 0x25b0,A0		; //0,
0x00003188 move.L (A0),D1
0x000031a0 sub.L D0,D1
0x000031b8 move.l 0x25b0,A0		; //0,
0x000031d0 move.L D1,(A0)
0x000031e8 nop; //----------------
0x00003200 move.l 0x25b0,A0		; //0,
0x00003218 move.l 0x2620,A1		; //0,
0x00003230 move.L (A0),(A1)
0x00003248 xor.q D0,D0
0x00003260 move.l 0x25e0,A0		; //10000,
0x00003278 move.w (A0),D0
0x00003290 move.l 0x2620,A0		; //0,
0x000032a8 move.L (A0),D1
0x000032c0 extL.Q D1
0x000032d8 mul.Q D0,D1
0x000032f0 move.l 0x2620,A0		; //0,
0x00003308 move.Q D1,(A0)
0x00003320 move.l 0x2630,A0		; //0,
0x00003338 jsr mlb
0x00003350 move.l 0x2630,A0		; //0,
0x00003368 move.l 0x2660,A1		; //0,
0x00003380 move.L (A0),(A1)
0x00003398 move.l 0x24f0,A0		; //0,
0x000033b0 move.Q (A0),D0
0x000033c8 move.l 0x2660,A0		; //0,
0x000033e0 move.L (A0),D1
0x000033f8 extL.Q D1
0x00003410 add.Q D0,D1
0x00003428 move.l 0x2660,A0		; //0,
0x00003440 move.Q D1,(A0)
0x00003458 move.l 0x2620,A0		; //0,
0x00003470 move.Q (A0),D0
0x00003488 move.l 0x2660,A0		; //0,
0x000034a0 move.Q (A0),D1
0x000034b8 add.Q D0,D1
0x000034d0 move.l 0x2660,A0		; //0,
0x000034e8 move.Q D1,(A0)
0x00003500 nop; //----------------
0x00003518 move.q A7,D7
0x00003530 move.l 0x2390,-(A7)
0x00003548 move.l 0x26b0,-(A7)
0x00003560 move.l 0x26e0,-(A7)
0x00003578 move.l 0x21b0,-(A7)
0x00003590 move.l 0x22a0,-(A7)
0x000035a8 move.l 0x2710,-(A7)
0x000035c0 move.l 0x2750,-(A7)
0x000035d8 move.l 0x27b0,A0		; //-1,
0x000035f0 jsr fnd
0x00003608 move.q D7,A7
0x00003620 nop; //----------------
0x00003638 nop; //----------------
0x00003650 move.q A7,D7
0x00003668 move.l 0x2810,-(A7)
0x00003680 move.l 0x2250,A0		; //0,
0x00003698 jsr ix
0x000036b0 move.l 0x21d0,-(A7)
0x000036c8 move.l 0x2370,A0		; //0,
0x000036e0 jsr iy
0x000036f8 move.l 0x2300,-(A7)
0x00003710 move.l 0x2450,A0		; //0,
0x00003728 jsr ia
0x00003740 move.l 0x23f0,-(A7)
0x00003758 move.l 0x2580,A0		; //0,
0x00003770 jsr in
0x00003788 move.l 0x2520,-(A7)
0x000037a0 move.l 0x21b0,-(A7)
0x000037b8 move.l 0x22a0,-(A7)
0x000037d0 move.l 0x2290,A0		; //0,
0x000037e8 jsr iw
0x00003800 move.l 0x2230,-(A7)
0x00003818 move.l 0x2380,A0		; //0,
0x00003830 jsr ih
0x00003848 move.l 0x2320,-(A7)
0x00003860 move.l 0x2630,A0		; //0,
0x00003878 jsr mlb
0x00003890 move.l 0x23b0,-(A7)
0x000038a8 move.l 0x2390,-(A7)
0x000038c0 move.l 0x2670,-(A7)
0x000038d8 move.l 0x2890,A0		; //-1,
0x000038f0 jsr print
0x00003908 move.q D7,A7
0x00003920 nop; //----------------
0x00003938 nop; //----------------
0x00003950 nop; //----------------
0x00003968 nop; //----------------