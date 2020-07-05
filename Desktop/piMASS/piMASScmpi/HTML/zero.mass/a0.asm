
 0x0000 move.l	A7,D0	
 0x0001 move.l	0x00000000,-(A7)	;a
 0x0002 jsr entryOBJ2A0
 0x0003 move.l	A0,-(A6)	
 0x0004 nop ;-------------------------
 0x0005 move.l	A7,D0	
 0x0006 move.l	0x00000001,-(A7)	;0x0a
 0x0007 jsr entryOBJ2A0
 0x0008 move.l	A0,(A5)+	
 0x0009 nop ;-------------------------
 0x000a move.l	-1(A5),A1	
 0x000b move.l	(A1),D0	
 0x000c move.l	(A6),A0	
 0x000d move.l	D0,(A0)	
 0x000e nop ;-------------------------
 0x000f nop ;-------------------------
 0x0010 move.l	A7,D0	
 0x0011 move.l	0x00000002,-(A7)	;hell
 0x0012 move.l	0x00000003,-(A7)	;o
 0x0013 jsr entryOBJ2A0
 0x0014 move.l	A0,-(A6)	
 0x0015 nop ;-------------------------
 0x0016 move.l	A7,D0	
 0x0017 move.l	0x00000000,-(A7)	;a
 0x0018 jsr entryOBJ2A0
 0x0019 move.l	A0,(A5)+	
 0x001a nop ;-------------------------
 0x001b move.l	-1(A5),A1	
 0x001c move.l	(A6),A0	
 0x001d move.l	(A1),(A0)	
 0x001e nop ;-------------------------
 0x001f move.l	A7,D0	
 0x0020 move.l	0x00000004,-(A7)	;iz
 0x0021 move.l	0x00000005,-(A7)	;e
 0x0022 jsr entryOBJ2A0
 0x0023 move.l	A0,(A5)+	
 0x0024 nop ;-------------------------
 0x0025 move.l	A7,D0	
 0x0026 move.l	0x00000006,-(A7)	;ez
 0x0027 move.l	0x00000007,-(A7)	;i
 0x0028 jsr entryOBJ2A0
 0x0029 move.l	A0,(A5)+	
 0x002a nop ;-------------------------
 0x002b move.l	-2(A5),A1	
 0x002c move.l	(A1),D0	
 0x002d move.l	-1(A5),A1	
 0x002e MUL.l	(A1),D0		; *
 0x002f move.l	(A6),A0	
 0x0030 SUB.l	D0,(A0)			; -
 0x0031 nop ;-------------------------
 0x0032 move.l	A7,D0	
 0x0033 move.l	0x00000008,-(A7)	;b
 0x0034 move.l	0x00000000,-(A7)	;a
 0x0035 jsr entryOBJ2A0
 0x0036 move.l	A0,(A5)+	
 0x0037 nop ;-------------------------
 0x0038 move.l	-1(A5),A1	
 0x0039 move.l	(A1),D0	
 0x003a move.l	(A6),A0	
 0x003b ADD.l	D0,(A0)			; + 
 0x003c nop ;-------------------------
 0x003d move.l	A7,D0	
 0x003e move.l	0x00000009,-(A7)	;c
 0x003f move.l	0x0000000a,-(A7)	;d
 0x0040 jsr entryOBJ2A0
 0x0041 move.l	A0,(A5)+	
 0x0042 nop ;-------------------------
 0x0043 move.l	A7,D0	
 0x0044 move.l	0x00000005,-(A7)	;e
 0x0045 jsr entryOBJ2A0
 0x0046 move.l	A0,(A5)+	
 0x0047 nop ;-------------------------
 0x0048 move.l	A7,D0	
 0x0049 move.l	0x0000000b,-(A7)	;0x64
 0x004a jsr entryOBJ2A0
 0x004b move.l	A0,(A5)+	
 0x004c nop ;-------------------------
 0x004d nop ;-------------------------
 0x004e move.l	A5,-(A7)	
 0x004f nop ;-------------------------
 0x0050 nop ;-------------------------
 0x0051 XOR.l	D0,D0			; ^
 0x0052 move.l	D0,-(A6)	
 0x0053 nop ;-------------------------
 0x0054 move.l	A7,D0	
 0x0055 move.l	0x0000000c,-(A7)	;0xc8
 0x0056 jsr entryOBJ2A0
 0x0057 move.l	A0,(A5)+	
 0x0058 nop ;-------------------------
 0x0059 move.l	-1(A5),A1	
 0x005a move.l	(A6),A0	
 0x005b move.l	(A1),(A0)	
 0x005c nop ;-------------------------
 0x005d move.l	A7,D0	
 0x005e move.l	0x0000000d,-(A7)	;f
 0x005f jsr entryOBJ2A0
 0x0060 move.l	A0,(A5)+	
 0x0061 nop ;-------------------------
 0x0062 move.l	A7,D0	
 0x0063 move.l	0x0000000e,-(A7)	;g
 0x0064 jsr entryOBJ2A0
 0x0065 move.l	A0,(A5)+	
 0x0066 nop ;-------------------------
 0x0067 move.l	-2(A5),A1	
 0x0068 move.l	(A1),D0	
 0x0069 move.l	-1(A5),A1	
 0x006a MUL.l	(A1),D0		; *
 0x006b move.l	(A6),A0	
 0x006c ADD.l	D0,(A0)			; + 
 0x006d nop ;-------------------------
 0x006e move.l	A7,D0	
 0x006f move.l	0x0000000f,-(A7)	;h
 0x0070 jsr entryOBJ2A0
 0x0071 move.l	A0,(A5)+	
 0x0072 nop ;-------------------------
 0x0073 move.l	-1(A5),A1	
 0x0074 move.l	(A1),D0	
 0x0075 move.l	(A6),A0	
 0x0076 ADD.l	D0,(A0)			; + 
 0x0077 nop ;-------------------------
 0x0078 nop ;-------------------------
 0x0079 move.l	(A7)+,A5	
 0x007a nop ;-------------------------
 0x007b move.l	(A6)+,(A5)+	
 0x007c nop ;-------------------------
 0x007d move.l	-4(A5),A1	
 0x007e move.l	(A1),D0	
 0x007f move.l	-3(A5),A1	
 0x0080 MUL.l	(A1),D0		; *
 0x0081 move.l	-2(A5),A1	
 0x0082 DIV.l	(A1),D0			; /
 0x0083 move.l	-1(A5),A1	
 0x0084 MUL.l	(A1),D0		; *
 0x0085 move.l	(A6),A0	
 0x0086 SUB.l	D0,(A0)			; -
 0x0087 nop ;-------------------------
 0x0088 move.l	A7,D0	
 0x0089 move.l	0x00000007,-(A7)	;i
 0x008a jsr entryOBJ2A0
 0x008b move.l	A0,(A5)+	
 0x008c nop ;-------------------------
 0x008d move.l	-1(A5),A1	
 0x008e move.l	(A1),D0	
 0x008f move.l	(A6),A0	
 0x0090 ADD.l	D0,(A0)			; + 
 0x0091 nop ;-------------------------
 0x0092 nop ;-------------------------
 0x0093 move.l	A7,D0	
 0x0094 move.l	0x00000010,-(A7)	;mini
 0x0095 jsr entryOBJ2A0
 0x0096 move.l	A0,-(A6)	
 0x0097 nop ;-------------------------
 0x0098 nop ;-------------------------
 0x0099 move.l	A5,-(A7)	
 0x009a nop ;-------------------------
 0x009b nop ;-------------------------
 0x009c XOR.l	D0,D0			; ^
 0x009d move.l	D0,-(A6)	
 0x009e nop ;-------------------------
 0x009f move.l	A7,D0	
 0x00a0 move.l	0xffffffb1,-(A7)	;"ize = %d\r\n"
 0x00a1 jsr entryOBJ2A0
 0x00a2 move.l	A0,(A5)+	
 0x00a3 nop ;-------------------------
 0x00a4 move.l	-1(A5),A1	
 0x00a5 move.l	(A1),D0	
 0x00a6 move.l	(A6),A0	
 0x00a7 move.l	D0,(A0)	
 0x00a8 nop ;-------------------------
 0x00a9 nop ;-------------------------
 0x00aa move.l	A7,D0	
 0x00ab move.l	0x00000012,-(A7)	;hello
 0x00ac jsr entryOBJ2A0
 0x00ad move.l	A0,(A5)+	
 0x00ae nop ;-------------------------
 0x00af move.l	-1(A5),A1	
 0x00b0 move.l	(A1),D0	
 0x00b1 move.l	(A6),A0	
 0x00b2 move.l	D0,(A0)	
 0x00b3 nop ;-------------------------
 0x00b4 nop ;-------------------------
 0x00b5 jsr	print
 0x00b6 move.l	(A7)+,A5	
 0x00b7 nop ;-------------------------
 0x00b8 move.l	(A6)+,(A5)+	
 0x00b9 nop ;-------------------------