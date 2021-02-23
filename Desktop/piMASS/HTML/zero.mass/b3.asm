
0x000028d0 nop; //----------------
0x000028e8 nop; //----------------
0x00002900 move.l 0x2250,A0		; //0,
0x00002918 jsr ix
0x00002930 move.l 0x2250,A0		; //0,
0x00002948 move.l 0x2280,A1		; //0,
0x00002960 move.L (A0),(A1)
0x00002978 xor.q D0,D0
0x00002990 move.l 0x2210,A0		; //480,
0x000029a8 move.w (A0),D0
0x000029c0 move.l 0x2280,A0		; //0,
0x000029d8 move.L (A0),D1
0x000029f0 extL.Q D1
0x00002a08 mul.Q D0,D1
0x00002a20 move.l 0x2280,A0		; //0,
0x00002a38 move.Q D1,(A0)
0x00002a50 move.l 0x2290,A0		; //0,
0x00002a68 jsr iw
0x00002a80 move.l 0x2290,A0		; //0,
0x00002a98 move.L (A0),D0
0x00002ab0 extL.Q D0
0x00002ac8 move.l 0x2280,A0		; //0,
0x00002ae0 move.Q (A0),D1
0x00002af8 div.Q D0,D1
0x00002b10 move.l 0x2280,A0		; //0,
0x00002b28 move.Q D1,(A0)
0x00002b40 nop; //----------------
0x00002b58 move.l 0x22e0,A0		; //800,
0x00002b70 move.l 0x2360,A1		; //0,
0x00002b88 move.w (A0),(A1)
0x00002ba0 move.l 0x2370,A0		; //0,
0x00002bb8 jsr iy
0x00002bd0 move.l 0x2370,A0		; //0,
0x00002be8 move.L (A0),D0
0x00002c00 extL.Q D0
0x00002c18 xor.q D1,D1
0x00002c30 move.l 0x2360,A0		; //0,
0x00002c48 move.w (A0),D1
0x00002c60 mul.Q D0,D1
0x00002c78 move.l 0x2360,A0		; //0,
0x00002c90 move.Q D1,(A0)
0x00002ca8 move.l 0x2380,A0		; //0,
0x00002cc0 jsr ih
0x00002cd8 move.l 0x2380,A0		; //0,
0x00002cf0 move.L (A0),D0
0x00002d08 extL.Q D0
0x00002d20 move.l 0x2360,A0		; //0,
0x00002d38 move.Q (A0),D1
0x00002d50 div.Q D0,D1
0x00002d68 move.l 0x2360,A0		; //0,
0x00002d80 move.Q D1,(A0)
0x00002d98 nop; //----------------
0x00002db0 move.l 0x2450,A0		; //0,
0x00002dc8 jsr ia
0x00002de0 move.l 0x2450,A0		; //0,
0x00002df8 move.l 0x2480,A1		; //0,
0x00002e10 move.L (A0),(A1)
0x00002e28 xor.q D0,D0
0x00002e40 move.l 0x2430,A0		; //2,
0x00002e58 move.b (A0),D0
0x00002e70 move.l 0x2480,A0		; //0,
0x00002e88 move.L (A0),D1
0x00002ea0 sub.L D0,D1
0x00002eb8 move.l 0x2480,A0		; //0,
0x00002ed0 move.L D1,(A0)
0x00002ee8 nop; //----------------
0x00002f00 move.l 0x2480,A0		; //0,
0x00002f18 move.l 0x24f0,A1		; //0,
0x00002f30 move.L (A0),(A1)
0x00002f48 xor.q D0,D0
0x00002f60 move.l 0x24b0,A0		; //1000,
0x00002f78 move.w (A0),D0
0x00002f90 move.l 0x24f0,A0		; //0,
0x00002fa8 move.L (A0),D1
0x00002fc0 extL.Q D1
0x00002fd8 mul.Q D0,D1
0x00002ff0 move.l 0x24f0,A0		; //0,
0x00003008 move.Q D1,(A0)
0x00003020 move.l 0x2580,A0		; //0,
0x00003038 jsr in
0x00003050 move.l 0x2580,A0		; //0,
0x00003068 move.l 0x25b0,A1		; //0,
0x00003080 move.L (A0),(A1)
0x00003098 xor.q D0,D0
0x000030b0 move.l 0x2560,A0		; //3,
0x000030c8 move.b (A0),D0
0x000030e0 move.l 0x25b0,A0		; //0,
0x000030f8 move.L (A0),D1
0x00003110 sub.L D0,D1
0x00003128 move.l 0x25b0,A0		; //0,
0x00003140 move.L D1,(A0)
0x00003158 nop; //----------------
0x00003170 move.l 0x25b0,A0		; //0,
0x00003188 move.l 0x2620,A1		; //0,
0x000031a0 move.L (A0),(A1)
0x000031b8 xor.q D0,D0
0x000031d0 move.l 0x25e0,A0		; //10000,
0x000031e8 move.w (A0),D0
0x00003200 move.l 0x2620,A0		; //0,
0x00003218 move.L (A0),D1
0x00003230 extL.Q D1
0x00003248 mul.Q D0,D1
0x00003260 move.l 0x2620,A0		; //0,
0x00003278 move.Q D1,(A0)
0x00003290 move.l 0x2630,A0		; //0,
0x000032a8 jsr mlb
0x000032c0 move.l 0x2630,A0		; //0,
0x000032d8 move.l 0x2660,A1		; //0,
0x000032f0 move.L (A0),(A1)
0x00003308 move.l 0x24f0,A0		; //0,
0x00003320 move.Q (A0),D0
0x00003338 move.l 0x2660,A0		; //0,
0x00003350 move.L (A0),D1
0x00003368 extL.Q D1
0x00003380 add.Q D0,D1
0x00003398 move.l 0x2660,A0		; //0,
0x000033b0 move.Q D1,(A0)
0x000033c8 move.l 0x2620,A0		; //0,
0x000033e0 move.Q (A0),D0
0x000033f8 move.l 0x2660,A0		; //0,
0x00003410 move.Q (A0),D1
0x00003428 add.Q D0,D1
0x00003440 move.l 0x2660,A0		; //0,
0x00003458 move.Q D1,(A0)
0x00003470 nop; //----------------
0x00003488 move.q A7,D7
0x000034a0 move.l 0x2390,-(A7)
0x000034b8 move.l 0x26b0,-(A7)
0x000034d0 move.l 0x26e0,-(A7)
0x000034e8 move.l 0x21b0,-(A7)
0x00003500 move.l 0x22a0,-(A7)
0x00003518 move.l 0x2710,-(A7)
0x00003530 move.l 0x2750,-(A7)
0x00003548 move.l 0x27b0,A0		; //-1,
0x00003560 jsr fnd
0x00003578 move.q D7,A7
0x00003590 nop; //----------------
0x000035a8 nop; //----------------
0x000035c0 move.q A7,D7
0x000035d8 move.l 0x2810,-(A7)
0x000035f0 move.l 0x2250,A0		; //0,
0x00003608 jsr ix
0x00003620 move.l 0x21d0,-(A7)
0x00003638 move.l 0x2370,A0		; //0,
0x00003650 jsr iy
0x00003668 move.l 0x2300,-(A7)
0x00003680 move.l 0x2450,A0		; //0,
0x00003698 jsr ia
0x000036b0 move.l 0x23f0,-(A7)
0x000036c8 move.l 0x2580,A0		; //0,
0x000036e0 jsr in
0x000036f8 move.l 0x2520,-(A7)
0x00003710 move.l 0x21b0,-(A7)
0x00003728 move.l 0x22a0,-(A7)
0x00003740 move.l 0x2290,A0		; //0,
0x00003758 jsr iw
0x00003770 move.l 0x2230,-(A7)
0x00003788 move.l 0x2380,A0		; //0,
0x000037a0 jsr ih
0x000037b8 move.l 0x2320,-(A7)
0x000037d0 move.l 0x2630,A0		; //0,
0x000037e8 jsr mlb
0x00003800 move.l 0x23b0,-(A7)
0x00003818 move.l 0x2390,-(A7)
0x00003830 move.l 0x2670,-(A7)
0x00003848 move.l 0x2890,A0		; //-1,
0x00003860 jsr print
0x00003878 move.q D7,A7
0x00003890 nop; //----------------
0x000038a8 nop; //----------------
0x000038c0 nop; //----------------