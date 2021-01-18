char gpsSHDRisoFRG1x2[] =
"#version 120																									\n"
"varying vec2 fr_uv;																							\n"
"uniform sampler2D tex0;					// MINI_CHAR_xXy_zXw.png											\n"
"uniform sampler2D tex1;					// BackGround														\n"
"uniform sampler2D tex2;					// MINI 	ABGR?													\n"
"											// U4		XYZW													\n"
"											//			cFcA 													\n"
"//uniform sampler2D tex3;					// CUBE																\n"
"uniform vec2 tgPX;																								\n"
"uniform vec2 DIVxy;																							\n"
"uniform vec2 FRMwh;																							\n"
"uniform vec2 aTX[8];																							\n"
"																												\n"
"#define gpdTXlo 	tex0																						\n"
"#define gpdTXcrl	vec3( aTX[0], 4 )		// aTX[0] = vec2( 32, 32 )											\n"
"#define gpdTXwh	vec2( 1024, 1536 )																			\n"
"																												\n"
"#define sb 0.1																									\n"
"#define gpdTXbg tex1																							\n"
"#define gpdTXut tex2																							\n"
"																												\n"
"vec4 cr_lut( vec4 rgba, vec3 crl, vec2 lwh ) 				// crl.xyz ColRowLut 	// lwh LutWH				\n"
"{																												\n"
"	vec2 ac = (rgba.rg/crl.xz);																					\n"
"	return vec4( 																								\n"
"					floor( vec2(fract(ac.x)*crl.x, ac.x)	),							// char					\n"
"					floor( vec2(fract(ac.y)*crl.z, ac.y)	)  							// LUT					\n"
"				) / vec4( crl.xy, lwh ) + vec2(0.0, rgba.b*0.25).xyxx;											\n"
"}																												\n"
"																												\n"
"void main()																									\n"
"{																										\n"
"	vec2	frm1 = fr_uv*FRMwh,																			\n"
"			frm0 = frm1/aTX[2],								// atx[2] == TXwh							\n"
"			t2x3 = vec2( 1.0/2.0, 1.0/3.0 )/gpdTXcrl.xy,	// aTX[0] == mn_iso_CR_32x32				\n"
"			t1x1 = t2x3*vec2( 2.0, 3.0 ),																\n"
"			Ain = fract(frm1)*t2x3,							// atx[0] == mnCR							\n"
"			off0 = vec2( 1.0/2.0, 1.0/4.0 ),															\n"
"			off2x3 = 1.0/aTX[2];							// aTX[3] == mn_iso_CR						\n"
"																										\n"
"	if( DIVxy.x < 1 )																					\n"
" 		off0.x = 0.0;																					\n"
"	if( DIVxy.y < 1 )																					\n"
" 		off0.y = 0.0;																					\n"
"																										\n"
"	gl_FragColor = vec4( texture2D( gpdTXbg, fr_uv ).rgb, 0 );				// BG						\n"
"	gl_FragColor *= gl_FragColor;																		\n"
"	// -1 0																								\n"
"	// +-+-+																							\n"
"	// |B|A| 0																							\n"
"	// +-+-+																							\n"
"	// |B|A| 1																							\n"
"	// +-+-+																							\n"
"	// |F|E| 2																							\n"
"	// +-+-+																							\n"
"	// FRM -------------------------------------------------------										\n"
"	vec4 A = texture2D( gpdTXut, frm0 + off0+vec2(0.0,0.5) )*0x100, cl;						\n"
"	if( A.r > 0 )																						\n"
"	{																									\n"
"		cl	= cr_lut( A, gpdTXcrl, gpdTXwh );															\n"
"		gl_FragColor += texture2D( gpdTXlo, cl.xy+Ain+t2x3*vec2( 0, 2 ) )*texture2D( gpdTXlo, cl.zw );	\n"
"	}																									\n"

"	// C --------------------------------------------------------										\n"
"	A = texture2D( gpdTXut, frm0 + off0 + off2x3*vec2( 0, -1 ) )*0x100;									\n"
"	if( A.r > 0 )																						\n"
"	{																									\n"
"		cl	= cr_lut( A, gpdTXcrl, gpdTXwh );															\n"
"		A 	= texture2D( gpdTXlo, cl.xy+Ain+t2x3*vec2( 0, 2 ) );										\n"
"		if( cl.w >= 1.0-sb )																				\n"
"		{																								\n"
"			cl.w = fract(cl.w);																			\n"
"			gl_FragColor += texture2D( gpdTXlo, cl.zw )*(1.0-A.g);										\n"
"		}																								\n"
"		else if( A.a > (1.0/256) )																		\n"
"		if( A.g*A.a > 0.5 )																			\n"
"		{																								\n"
"			gl_FragColor = ((A-0.5)*2.0).ggga*texture2D( gpdTXlo, cl.zw );											\n"
"		} else 																							\n"
"			gl_FragColor = A;																			\n"
"	}																									\n"

"	// D --------------------------------------------------------										\n"
"	A = texture2D( gpdTXut, frm0 + off0 + off2x3*vec2(-1, -1 ) )*0x100;									\n"
"	if( A.r > 0 )																						\n"
"	{																									\n"
"		cl	= cr_lut( A, gpdTXcrl, gpdTXwh );															\n"
"		A 	= texture2D( gpdTXlo, cl.xy+Ain+t2x3*vec2( 1, 2 ) );										\n"
"		if( cl.w >= 1.0-sb )																				\n"
"		{																								\n"
"			cl.w = fract(cl.w);																			\n"
"			gl_FragColor += texture2D( gpdTXlo, cl.zw )*(1.0-A.g);										\n"
"		}																								\n"
"		else if( A.a > (1.0/256) )																		\n"
"		if( A.g*A.a > 0.5 )																			\n"
"		{																								\n"
"			gl_FragColor = ((A-0.5)*2.0).ggga*texture2D( gpdTXlo, cl.zw );											\n"
"		} else 																							\n"
"			gl_FragColor = A;																			\n"
"	}																									\n"


"	// B --------------------------------------------------------										\n"
"	A = texture2D( gpdTXut, frm0 + off0 + off2x3*vec2( -1, 0 ) )*0x100;						\n"
"	if( A.r > 0 )																						\n"
"	{																									\n"
"		cl	= cr_lut( A, gpdTXcrl, gpdTXwh );															\n"
"		A 	= texture2D( gpdTXlo, cl.xy+Ain+t2x3*vec2( 1, 1 ) );										\n"
"		if( cl.w >= 1.0-sb )																				\n"
"		{																								\n"
"			cl.w = fract(cl.w);																			\n"
"			gl_FragColor += texture2D( gpdTXlo, cl.zw )*(1.0-A.g);										\n"
"		}																								\n"
"		else if( A.a > (1.0/256) )																		\n"
"		if( A.g*A.a > 0.5 )																			\n"
"		{																								\n"
"			gl_FragColor = ((A-0.5)*2.0).ggga*texture2D( gpdTXlo, cl.zw );											\n"
"		} else 																							\n"
"			gl_FragColor = A;																			\n"
"	}																									\n"

"	// A --------------------------------------------------------										\n"
"	A = texture2D( gpdTXut, frm0 + off0 ) *0x100;														\n"
"	if( A.r > 0 )																						\n"
"	{																									\n"
"		cl	= cr_lut( A, gpdTXcrl, gpdTXwh );															\n"
"		A 	= texture2D( gpdTXlo, cl.xy+Ain+t2x3*vec2( 0, 1 ) );										\n"
"		if( cl.w >= 1.0-sb )																				\n"
"		{																								\n"
"			cl.w = fract(cl.w);																			\n"
"			gl_FragColor += texture2D( gpdTXlo, cl.zw )*(1.0-A.g);										\n"
"		}																								\n"
"		else if( A.a > (1.0/256) )																		\n"
"		if( A.g*A.a > 0.5 )																			\n"
"		{																								\n"
"			gl_FragColor = ((A-0.5)*2.0).ggga*texture2D( gpdTXlo, cl.zw );											\n"
"		} else 																							\n"
"			gl_FragColor = A;																			\n"
"	}																									\n"
"																										\n"

"	// E --------------------------------------------------------										\n"
"	A = texture2D( gpdTXut, frm0 + off0 + off2x3*vec2( 0, 1 ) )*0x100;									\n"
"	if( A.r > 0 )																						\n"
"	{																									\n"
"		cl	= cr_lut( A, gpdTXcrl, gpdTXwh );															\n"
"		A 	= texture2D( gpdTXlo, cl.xy+Ain+t2x3*vec2( 0, 0 ) );										\n"
"		if( cl.w >= 1.0-sb )																				\n"
"		{																								\n"
"			cl.w = fract(cl.w);																			\n"
"			gl_FragColor += texture2D( gpdTXlo, cl.zw )*(1.0-A.g);										\n"
"		}																								\n"
"		else if( A.a > (1.0/256) )																		\n"
"		if( A.g*A.a > 0.5 )																			\n"
"		{																								\n"
"			gl_FragColor = ((A-0.5)*2.0).ggga*texture2D( gpdTXlo, cl.zw );											\n"
"		} else 																							\n"
"			gl_FragColor = A;																			\n"
"	}																									\n"

"	// F --------------------------------------------------------										\n"
"	A = texture2D( gpdTXut, frm0 + off0 + off2x3*vec2( -1, 1 ) )*0x100;							\n"
"	if( A.r > 0 )																						\n"
"	{																									\n"
"		cl	= cr_lut( A, gpdTXcrl, gpdTXwh );														\n"
"		A = texture2D( gpdTXlo, cl.xy+Ain+t2x3*vec2( 1, 0 ) );												\n"
" 		gl_FragColor += texture2D( gpdTXlo, frm1);															\n"
"		if( cl.w >= 1.0-sb )																					\n"
"		{																									\n"
"			cl.w = fract(cl.w);																				\n"
"			gl_FragColor += texture2D( gpdTXlo, cl.zw )*(1.0-A.g);											\n"
"		}																									\n"
"		else if( A.a > (1.0/256.0) )																		\n"
"		if( A.g*A.a > 0.5 )																				\n"
"		{																									\n"
"			gl_FragColor = ((A-0.5)*2.0).ggga*texture2D( gpdTXlo, cl.zw );												\n"
"		} else 																								\n"
"			gl_FragColor = A;																				\n"
"	}																									\n"



"}\n";