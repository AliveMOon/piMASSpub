a = 10;
 b = 20;
 c = a+b
  * (a + b)
  *b/a
 ,
 d = c%4;

   0x2100 a= 10,
//0x2110 10= 0x0a,
  0x2120 b= 20,
//0x2130 20= 0x14,
  0x2140 c= 0,
//0x2170 ,
  0x2190 d= 0,
//0x21b0 4= 0x04,

0x000021c0 xor.q D0,D0
0x000021d8 move.l 0x2110,A0		; //10,
0x000021f0 move.b (A0),D0
0x00002208 move.l 0x2100,A0		; //0,
0x00002220 move.L D0,(A0)
0x00002238 nop; //----------------
0x00002250 xor.q D0,D0
0x00002268 move.l 0x2130,A0		; //20,
0x00002280 move.b (A0),D0
0x00002298 move.l 0x2120,A0		; //0,
0x000022b0 move.L D0,(A0)
0x000022c8 nop; //----------------
0x000022e0 move.l 0x0,A0		; //0,
0x000022f8 move.L D0,(A0)
0x00002310 nop; //----------------
0x00002328 move.l 0x2120,A0		; //0,
0x00002340 move.L (A0),D0
0x00002358 move.l 0x2120,A0		; //0,
0x00002370 move.L (A0),D1
0x00002388 mul.L D1,D0
0x000023a0 move.l 0x2100,A0		; //0,
0x000023b8 move.L (A0),D1
0x000023d0 div.L D1,D0
0x000023e8 move.l 0x2180,A0		; //0,
0x00002400 move.L D0,(A0)
0x00002418 nop; //----------------
0x00002430 move.l 0x0,A0		; //0,
0x00002448 move.L D0,(A0)
0x00002460 nop; //----------------
0x00002478 move.l 0x2140,A0		; //0,
0x00002490 move.L (A0),D0
0x000024a8 move.l 0x21b0,A0		; //4,
0x000024c0 xor.q D1,D1
0x000024d8 move.b (A0),D1
0x000024f0 rem.L D1,D0
0x00002508 move.l 0x21a0,A0		; //0,
0x00002520 move.L D0,(A0)
0x00002538 nop; //----------------
0x00002550 xor.q D0,D0
0x00002568 move.l 0x21a0,A0		; //0,
0x00002580 move.L (A0),D0
0x00002598 move.l 0x2190,A0		; //0,
0x000025b0 move.L D0,(A0)
0x000025c8 nop; //----------------
0x000025e0 move.l 0x2180,A0		; //0,
0x000025f8 move.L (A0),D0
0x00002610 move.l 0x2190,A0		; //0,
0x00002628 move.L (A0),D1
0x00002640 stk.L D1,D0
0x00002658 move.l 0x0,A0		; //0,
0x00002670 move.L D0,(A0)
0x00002688 nop; //----------------