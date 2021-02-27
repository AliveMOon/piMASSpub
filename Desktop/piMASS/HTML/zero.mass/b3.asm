
0x00002890 nop; //----------------
0x000028a8 nop; //----------------
0x000028c0 move.l 0x2250,A0		; //0,
0x000028d8 jsr ix
0x000028f0 move.l 0x2250,A0		; //0,
0x00002908 move.l 0x2280,A1		; //0,
0x00002920 move.L (A0),(A1)
0x00002938 xor.q D0,D0
0x00002950 move.l 0x2210,A0		; //480,
0x00002968 move.w (A0),D0
0x00002980 move.l 0x2280,A0		; //0,
0x00002998 move.L (A0),D1
0x000029b0 extL.Q D1
0x000029c8 mul.Q D0,D1
0x000029e0 move.l 0x2280,A0		; //0,
0x000029f8 move.Q D1,(A0)
0x00002a10 move.l 0x2290,A0		; //0,
0x00002a28 jsr iw
0x00002a40 move.l 0x2290,A0		; //0,
0x00002a58 move.L (A0),D0
0x00002a70 extL.Q D0
0x00002a88 move.l 0x2280,A0		; //0,
0x00002aa0 move.Q (A0),D1
0x00002ab8 div.Q D0,D1
0x00002ad0 move.l 0x2280,A0		; //0,
0x00002ae8 move.Q D1,(A0)
0x00002b00 nop; //----------------
0x00002b18 move.l 0x22e0,A0		; //800,
0x00002b30 move.l 0x2360,A1		; //0,
0x00002b48 move.w (A0),(A1)
0x00002b60 move.l 0x2370,A0		; //0,
0x00002b78 jsr iy
0x00002b90 move.l 0x2370,A0		; //0,
0x00002ba8 move.L (A0),D0
0x00002bc0 extL.Q D0
0x00002bd8 xor.q D1,D1
0x00002bf0 move.l 0x2360,A0		; //0,
0x00002c08 move.w (A0),D1
0x00002c20 mul.Q D0,D1
0x00002c38 move.l 0x2360,A0		; //0,
0x00002c50 move.Q D1,(A0)
0x00002c68 move.l 0x2380,A0		; //0,
0x00002c80 jsr ih
0x00002c98 move.l 0x2380,A0		; //0,
0x00002cb0 move.L (A0),D0
0x00002cc8 extL.Q D0
0x00002ce0 move.l 0x2360,A0		; //0,
0x00002cf8 move.Q (A0),D1
0x00002d10 div.Q D0,D1
0x00002d28 move.l 0x2360,A0		; //0,
0x00002d40 move.Q D1,(A0)
0x00002d58 nop; //----------------
0x00002d70 move.l 0x2450,A0		; //0,
0x00002d88 jsr ia
0x00002da0 move.l 0x2450,A0		; //0,
0x00002db8 move.l 0x2480,A1		; //0,
0x00002dd0 move.L (A0),(A1)
0x00002de8 xor.q D0,D0
0x00002e00 move.l 0x2430,A0		; //2,
0x00002e18 move.b (A0),D0
0x00002e30 move.l 0x2480,A0		; //0,
0x00002e48 move.L (A0),D1
0x00002e60 sub.L D0,D1
0x00002e78 move.l 0x2480,A0		; //0,
0x00002e90 move.L D1,(A0)
0x00002ea8 nop; //----------------
0x00002ec0 move.l 0x2480,A0		; //0,
0x00002ed8 move.l 0x24f0,A1		; //0,
0x00002ef0 move.L (A0),(A1)
0x00002f08 xor.q D0,D0
0x00002f20 move.l 0x24b0,A0		; //1000,
0x00002f38 move.w (A0),D0
0x00002f50 move.l 0x24f0,A0		; //0,
0x00002f68 move.L (A0),D1
0x00002f80 extL.Q D1
0x00002f98 mul.Q D0,D1
0x00002fb0 move.l 0x24f0,A0		; //0,
0x00002fc8 move.Q D1,(A0)
0x00002fe0 move.l 0x2580,A0		; //0,
0x00002ff8 jsr in
0x00003010 move.l 0x2580,A0		; //0,
0x00003028 move.l 0x25b0,A1		; //0,
0x00003040 move.L (A0),(A1)
0x00003058 xor.q D0,D0
0x00003070 move.l 0x2560,A0		; //3,
0x00003088 move.b (A0),D0
0x000030a0 move.l 0x25b0,A0		; //0,
0x000030b8 move.L (A0),D1
0x000030d0 sub.L D0,D1
0x000030e8 move.l 0x25b0,A0		; //0,
0x00003100 move.L D1,(A0)
0x00003118 nop; //----------------
0x00003130 move.l 0x25b0,A0		; //0,
0x00003148 move.l 0x2620,A1		; //0,
0x00003160 move.L (A0),(A1)
0x00003178 xor.q D0,D0
0x00003190 move.l 0x25e0,A0		; //10000,
0x000031a8 move.w (A0),D0
0x000031c0 move.l 0x2620,A0		; //0,
0x000031d8 move.L (A0),D1
0x000031f0 extL.Q D1
0x00003208 mul.Q D0,D1
0x00003220 move.l 0x2620,A0		; //0,
0x00003238 move.Q D1,(A0)
0x00003250 move.l 0x2630,A0		; //0,
0x00003268 jsr mlb
0x00003280 move.l 0x2630,A0		; //0,
0x00003298 move.l 0x2660,A1		; //0,
0x000032b0 move.L (A0),(A1)
0x000032c8 move.l 0x24f0,A0		; //0,
0x000032e0 move.Q (A0),D0
0x000032f8 move.l 0x2660,A0		; //0,
0x00003310 move.L (A0),D1
0x00003328 extL.Q D1
0x00003340 add.Q D0,D1
0x00003358 move.l 0x2660,A0		; //0,
0x00003370 move.Q D1,(A0)
0x00003388 move.l 0x2620,A0		; //0,
0x000033a0 move.Q (A0),D0
0x000033b8 move.l 0x2660,A0		; //0,
0x000033d0 move.Q (A0),D1
0x000033e8 add.Q D0,D1
0x00003400 move.l 0x2660,A0		; //0,
0x00003418 move.Q D1,(A0)
0x00003430 nop; //----------------
0x00003448 move.q A7,D7
0x00003460 move.l 0x2390,-(A7)
0x00003478 move.l 0x26b0,-(A7)
0x00003490 move.l 0x26e0,-(A7)
0x000034a8 move.l 0x21b0,-(A7)
0x000034c0 move.l 0x22a0,-(A7)
0x000034d8 move.l 0x2710,-(A7)
0x000034f0 move.l 0x2770,A0		; //-1,
0x00003508 jsr fnd
0x00003520 move.q D7,A7
0x00003538 nop; //----------------
0x00003550 nop; //----------------
0x00003568 move.q A7,D7
0x00003580 move.l 0x27d0,-(A7)
0x00003598 move.l 0x2250,A0		; //0,
0x000035b0 jsr ix
0x000035c8 move.l 0x21d0,-(A7)
0x000035e0 move.l 0x2370,A0		; //0,
0x000035f8 jsr iy
0x00003610 move.l 0x2300,-(A7)
0x00003628 move.l 0x2450,A0		; //0,
0x00003640 jsr ia
0x00003658 move.l 0x23f0,-(A7)
0x00003670 move.l 0x2580,A0		; //0,
0x00003688 jsr in
0x000036a0 move.l 0x2520,-(A7)
0x000036b8 move.l 0x21b0,-(A7)
0x000036d0 move.l 0x22a0,-(A7)
0x000036e8 move.l 0x2290,A0		; //0,
0x00003700 jsr iw
0x00003718 move.l 0x2230,-(A7)
0x00003730 move.l 0x2380,A0		; //0,
0x00003748 jsr ih
0x00003760 move.l 0x2320,-(A7)
0x00003778 move.l 0x2630,A0		; //0,
0x00003790 jsr mlb
0x000037a8 move.l 0x23b0,-(A7)
0x000037c0 move.l 0x2390,-(A7)
0x000037d8 move.l 0x2670,-(A7)
0x000037f0 move.l 0x2850,A0		; //-1,
0x00003808 jsr print
0x00003820 move.q D7,A7
0x00003838 nop; //----------------
0x00003850 nop; //----------------
0x00003868 nop; //----------------