
0x000023e0 nop; //----------------
0x000023f8 nop; //----------------
0x00002410 nop; //----------------
0x00002428 nop; //----------------
0x00002440 nop; //----------------
0x00002458 move.q A7,D7
0x00002470 move.l 0xe,-(A7)
0x00002488 move.l 0x10,-(A7)
0x000024a0 move.l 0x2310,A0		; //-1,
0x000024b8 jsr entry
0x000024d0 move.q D7,A7
0x000024e8 nop; //----------------
0x00002500 nop; //----------------
0x00002518 nop; //----------------