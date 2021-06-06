#pragma once
#ifndef gpsGLSL_H
#define gpsGLSL_H
#define gpdGUIfrDEPTH "	float f = gl_DepthRange.far, n = gl_DepthRange.near; gl_FragDepth = (f-n)*0.85+n; \n"

#define gpdGUIfr3Dc "	gl_FragColor = vec4(fr_uv,1.0);											\n"
#define gpdGUIfr3Dc0 "	gl_FragColor = texture2D( tex0, fr_uv.xy );								\n"
#define gpdGUIfr3Dc1 "	gl_FragColor = texture2D( tex1, fr_uv.xy );								\n"
#define gpdGUIfr3Dc2 "	gl_FragColor = texture2D( tex2, fr_uv.xy );								\n"
#define gpdGUIfr3Dc3 "	gl_FragColor = texture2D( tex3, fr_uv.xy );								\n"
#define gpdGUIfr3Dc4 "	gl_FragColor = texture2D( tex4, fr_uv.xy );								\n"
#define gpdGUIfr3Dc5 "	gl_FragColor = texture2D( tex5, fr_uv.xy );								\n"
#define gpdGUIfr3Dc6 "	gl_FragColor = texture2D( tex6, fr_uv.xy );								\n"
#define gpdGUIfr3Dc7 "	gl_FragColor = texture2D( tex7, fr_uv.xy );								\n"
#define gpdGUIfr3Dc8 "	gl_FragColor = texture2D( tex8, fr_uv.xy );								\n"

#define gpdGUIfr3Du "	gl_FragColor = vec4(fr_up,1.0);											\n"
#define gpdGUIfr3Dp "	gl_FragColor = vec4(fr_ps.xyy,1.0);										\n"

#define gpdGUIfr3Dd "	float f = gl_DepthRange.far, n = gl_DepthRange.near; gl_FragDepth = ((f-n)*fr_uv.z+n+f)/2.0;	\n"
#define gpdSTATICoff static const char  //static const

///-------------------------------------------------
///					 wrkDIST * imgSIZE
///		fokLEN	 =	-------------------
///					 objSIZE + imgSIZE
///
///		imgSIZE: SONY IMX219 1/4" 3.67mm x 2.76mm px1.12um
///-------------------------------------------------

gpdSTATICoff gpsGLSLvx[] = //{
"#version 120																\n"
"attribute	vec2	v_vx;													\n"
"attribute	vec2	v_uv;													\n"
"varying	vec2	fr_uv;													\n"
"void main()																\n"
"{																			\n"
"	gl_Position	= vec4( v_vx*vec2(2.0,-2.0)+vec2(-1.0,1.0), -0.9, 1.0);		\n"
"	fr_uv		= v_uv;														\n"
"}																			\n\0";
//};
gpdSTATICoff gpsGLSLfrgREF[] = //{
"#version 120																\n"
"varying vec2 fr_uv;														\n"
"uniform sampler2D	tex0;					// MINI_CHAR_xXy_zXw.png		\n"
"uniform vec4 		aCNL[8];				// CNL							\n"
"void main()																\n"
"{																			\n"
"	gl_FragColor = texture2D( tex0, fr_uv )*aCNL[0];						\n"
"	if( gl_FragColor.a < (1/0x100) )										\n"
" 		discard;															\n"
	gpdGUIfrDEPTH
"}																			\n"
"\n\0";
//};
gpdSTATICoff gpsGLSLfrgLINE[] = // {
"#version 120															\n"
"varying vec2 fr_uv;													\n"
"uniform sampler2D	tex0;					// MINI_CHAR_xXy_zXw.png	\n"
"uniform vec4 		aCNL[8];				// CNL						\n"
"void main()															\n"
"{																		\n"
"	gl_FragColor = aCNL[0];												\n"
	gpdGUIfrDEPTH
"}																		\n"
"\n\0";
//};

gpdSTATICoff gpsGLSLfrgISOo[] = //{
"#version 120																							\n"
"varying vec2 fr_uv;																					\n"
"uniform sampler2D tex0;					// MINI_CHAR_xXy_zXw.png									\n"
"uniform sampler2D tex1;					// BackGround												\n"
"uniform sampler2D tex2;					// MINI 	ABGR?											\n"
"											// U4		XYZW											\n"
"											//			cFcA 											\n"
"//uniform sampler2D tex3;					// CUBE														\n"
"uniform vec2 tgPX;																						\n"
"uniform vec2 DIVxy;																					\n"
"uniform vec2 FRMwh;																					\n"
"uniform vec2 aTX[8];																					\n"
"																										\n"
"#define gpdTXlo 	tex0																				\n"
"#define gpdTXcrl	vec3( aTX[0], 4 )		// aTX[0] = vec2( 32, 32 )									\n"
"#define gpdTXwh	vec2( 1024, 1536 )																	\n"
"#define gpdTXiz	vec2( 0, 3.0/1536 )																	\n"
"																										\n"
"#define sb 0.1																							\n"
"#define gpdTXbg tex1																					\n"
"#define gpdTXut tex2																					\n"
"																										\n"
"vec4 cr_lut( vec4 rgba, vec3 crl, vec2 lwh ) 				// crl.xyz ColRowLut 	// lwh LutWH		\n"
"{																										\n"
"	vec2 ac = (rgba.rg/crl.xz);																			\n"
"	vec4 o = vec4( 																						\n"
"					floor( vec2( fract(ac.x)*crl.x, ac.x )	),							// char			\n"
"					floor( vec2( fract(ac.y)*crl.z, ac.y+(1.0/3.0) )	)  				// LUT			\n"
"				) / vec4( crl.xy, lwh ) + vec2(0.0, rgba.b*0.25).xyxx;									\n"
"	if( ac.y > crl.z )																					\n"
"		o.w += 1.0;																						\n"
"	return o;																							\n"
"}																										\n"
"																										\n"
"void main()																							\n"
"{																										\n"
"	vec2	frm1 = fr_uv*FRMwh,																			\n"
"			frm0 = frm1/aTX[2],								// atx[2] == TXwh							\n"
"			t2x3 = vec2( 1.0/2.0, 1.0/3.0 )/gpdTXcrl.xy,	// aTX[0] == mn_iso_CR_32x32				\n"
"			Ain = fract(frm1)*t2x3,							// atx[0] == mnCR							\n"
"			off0 = vec2( 1.0/2.0, 1.0/4.0 ),															\n"
"			off2x3 = 1.0/aTX[2];							// aTX[3] == mn_iso_CR						\n"
"																										\n"
"	if( DIVxy.x < 1 )																					\n"
" 		off0.x = 0.0;																					\n"
"	if( DIVxy.y < 1 )																					\n"
" 		off0.y = 0.0;																					\n"
"	gl_FragColor = vec4( texture2D( gpdTXbg, fr_uv ).rgb, 0 );				// BG						\n"
"	gl_FragColor *= gl_FragColor*0.25;																	\n"
"	// FRM -------------------------------------------------------\n"
"	float B = 0, Aga; 	\n"
"	vec4 A = texture2D( gpdTXut, frm0 + off0+vec2(0.0,0.5) )*0x100, cl, Bc = vec4(0,0,0,0);				\n"
"	if( A.r > 0 )																						\n"
"	{																									\n"
"		cl	= cr_lut( A, vec3( aTX[0]*vec2(1,3), 4 ), gpdTXwh );										\n"
"		gl_FragColor += texture2D( gpdTXlo, cl.xy+Ain+vec2(0,5.0/32.0) )*texture2D( gpdTXlo, cl.zw );	\n"
"	}																									\n"
"	off0.y += off2x3.y;																					\n"
"	// -1 0		//  0 1																					\n"
"	// +-+-+	// +-+-+																				\n"
"	// |D|C| -1	// |E|F| 0																				\n"
"	// +-+-+	// +-+-+																				\n"
"	// |B|A| 0	// |A|B| 1																				\n"
"	// +-+-+	// +-+-+																				\n"
"	// |F|E| 1	// |C|D| 2																				\n"
"	// +-+-+	// +-+-+																				\n"
"	// C --------------------------------------------------------										\n"
"	A = texture2D( gpdTXut, frm0 + off0 + off2x3*vec2( 0, -1 ) )*0x100;									\n"
"	cl	= cr_lut( A, gpdTXcrl, gpdTXwh );																\n"		/// !!!! ------------
"	if( A.r > 0 )																						\n"
"	{																									\n"
"		A 	= texture2D( gpdTXlo, cl.xy+Ain+t2x3*vec2( 0, 2 )-gpdTXiz );								\n"
"		Aga = A.g*A.a; 																					\n"
"		if( cl.w >= 1.0 )																				\n"
"		{																								\n"
"			cl.w = fract(cl.w);																			\n"
"			B += (1.0-Aga);																				\n"
"			Bc += texture2D( gpdTXlo, cl.zw );															\n"
"		}																								\n"
"		else if( A.a > (1.0/256) )\n"
"		{\n"
"			if( Aga > 0.5 )																			\n"
"			{\n"
"				Aga = (Aga-0.5)*2.0;\n"
"				if( Aga > 0.9 )\n"
"				{\n"
"					B += Aga;\n"
"					Bc += texture2D( gpdTXlo, cl.zw );\n"
"				}			\n"
"			} //else																		\n"
"			gl_FragColor = max( gl_FragColor, Aga*texture2D( gpdTXlo, cl.zw ));							\n"
"		}			\n"
"	}																									\n"
"	else if( cl.w >= 1.0 )																				\n"
"		{																								\n"
"			cl.w = fract(cl.w);																			\n"
"			B += 1;																						\n"
"			Bc += texture2D( gpdTXlo, cl.zw );															\n"
"		}																								\n"
"	// D --------------------------------------------------------										\n"
"	A = texture2D( gpdTXut, frm0 + off0 + off2x3*vec2(-1, -1 ) )*0x100;									\n"
"	if( A.r > 0 )																						\n"
"	{																									\n"
"		cl	= cr_lut( A, gpdTXcrl, gpdTXwh );															\n"
"		A 	= texture2D( gpdTXlo, cl.xy+Ain+t2x3*vec2( 1, 2 )-gpdTXiz );										\n"
"		Aga = A.g*A.a; 		\n"
"		if( cl.w >= 2.0 )																				\n"
"		{																								\n"
"			cl.w = fract(cl.w);\n"
"			B += (1.0-Aga);\n"
"			Bc += texture2D( gpdTXlo, cl.zw );										\n"
"		}																								\n"
"		else if( A.a > (1.0/256) )\n"
"		{\n"
"			if( Aga > 0.5 )																			\n"
"			{\n"
"				Aga = (Aga-0.5)*2.0;\n"
"				if( Aga > 0.9 )\n"
"				{\n"
"					B += Aga;\n"
"					Bc += texture2D( gpdTXlo, cl.zw );\n"
"				}			\n"
"			} //else																		\n"
"			gl_FragColor = max( gl_FragColor, Aga*texture2D( gpdTXlo, cl.zw ));							\n"
"		}																			\n"
"	}																									\n"
"	// B --------------------------------------------------------										\n"
"	A = texture2D( gpdTXut, frm0 + off0 + off2x3*vec2( -1, 0 ) )*0x100;						\n"
"	if( A.r > 0 )																						\n"
"	{																									\n"
"		cl	= cr_lut( A, gpdTXcrl, gpdTXwh );															\n"
"		A 	= texture2D( gpdTXlo, cl.xy+Ain+t2x3*vec2( 1, 1 )-gpdTXiz );										\n"
"		Aga = A.g*A.a; 		\n"
"		if( cl.w >= 2.0 )																				\n"
"		{																								\n"
"			cl.w = fract(cl.w);\n"
"			B += (1.0-Aga);\n"
"			Bc += texture2D( gpdTXlo, cl.zw );										\n"
"		}																								\n"
"		else if( A.a > (1.0/256) )\n"
"		{\n"
"			if( Aga > 0.5 )																			\n"
"			{\n"
"				Aga = (Aga-0.5)*2.0;\n"
"				if( Aga > 0.9 )\n"
"				{\n"
"					B += Aga;\n"
"					Bc += texture2D( gpdTXlo, cl.zw );\n"
"				}			\n"
"			} // else																		\n"
"			gl_FragColor = max( gl_FragColor, Aga*texture2D( gpdTXlo, cl.zw ));							\n"
"		}																			\n"
"	}																									\n"
"	// A --------------------------------------------------------										\n"
"	A = texture2D( gpdTXut, frm0 + off0 ) *0x100;														\n"
"	cl	= cr_lut( A, gpdTXcrl, gpdTXwh );															\n"		/// !!!! ------------
"	if( A.r > 0 )																						\n"
"	{																									\n"
"		A 	= texture2D( gpdTXlo, cl.xy+Ain+t2x3*vec2( 0, 1 )-gpdTXiz );										\n"
"		Aga = A.g*A.a; 		\n"
"		if( cl.w >= 1.0 )																				\n"
"		{																								\n"
"			cl.w = fract(cl.w);\n"
"			B += (1.0-Aga);\n"
"			Bc += texture2D( gpdTXlo, cl.zw );										\n"
"		}																								\n"
"		else if( A.a > (1.0/256) )\n"
"		{\n"
"			if( Aga > 0.5 )																			\n"
"			{\n"
"				Aga = (Aga-0.5)*2.0;\n"
"				if( Aga > 0.9 )\n"
"				{\n"
"					B += Aga;\n"
"					Bc += texture2D( gpdTXlo, cl.zw );\n"
"				}			\n"
"			}  //else																\n"
"			gl_FragColor = max( gl_FragColor, Aga*texture2D( gpdTXlo, cl.zw ));		\n"
"		}																			\n"
"	}																				\n"
"	else if( cl.w >= 1.0 )															\n"
"		{																			\n"
"			cl.w = fract(cl.w);														\n"
"			B += 1.0;																\n"
"			Bc += texture2D( gpdTXlo, cl.zw );										\n"
"		}																			\n"
"																					\n"
"	// E --------------------------------------------------------					\n"
"	A = texture2D( gpdTXut, frm0 + off0 + off2x3*vec2( 0, 1 ) )*0x100;				\n"
"	if( A.r > 0 )																	\n"
"	{																				\n"
"		cl	= cr_lut( A, gpdTXcrl, gpdTXwh );										\n"
"		A 	= texture2D( gpdTXlo, cl.xy+Ain+t2x3*vec2( 0, 0 )-gpdTXiz );			\n"
"		Aga = A.g*A.a; 																\n"
"		if( cl.w >= 2.0 )															\n"
"		{																			\n"
"			cl.w = fract(cl.w);														\n"
"			B += (1.0-Aga);															\n"
"			Bc += texture2D( gpdTXlo, cl.zw );										\n"
"		}																			\n"
"		else if( A.a > (1.0/256) )													\n"
"		{																			\n"
"			if( Aga > 0.5 )															\n"
"			{																		\n"
"				Aga = (Aga-0.5)*2.0;												\n"
"				if( Aga > 0.9 )														\n"
"				{																	\n"
"					B += Aga;														\n"
"					Bc += texture2D( gpdTXlo, cl.zw );								\n"
"				}																	\n"
"			}  																		\n"
"			gl_FragColor = max( gl_FragColor, Aga*texture2D( gpdTXlo, cl.zw ));		\n"
"		}																			\n"
"	}																				\n"
"																					\n"
"	// F --------------------------------------------------------					\n"
"	A = texture2D( gpdTXut, frm0 + off0 + off2x3*vec2( -1, 1 ) )*0x100;				\n"
"	if( A.r > 0 )																	\n"
"	{																				\n"
"		cl	= cr_lut( A, gpdTXcrl, gpdTXwh );										\n"
"		A = texture2D( gpdTXlo, cl.xy+Ain+t2x3*vec2( 1, 0 )-gpdTXiz );				\n"
" 		Aga = A.g*A.a; 																\n"
"		if( cl.w >= 2.0 )															\n"
"		{																			\n"
"			cl.w = fract(cl.w);														\n"
"			B += (1.0-Aga);															\n"
"			Bc += texture2D( gpdTXlo, cl.zw );										\n"
"		}																			\n"
"		else if( A.a > (1.0/256) )\n"
"		{\n"
"			if( Aga > 0.5 )																			\n"
"			{\n"
"				Aga = (Aga-0.5)*2.0;\n"
"				if( Aga > 0.9 )\n"
"				{\n"
"					B += Aga;\n"
"					Bc += texture2D( gpdTXlo, cl.zw );\n"
"				}			\n"
"			}  //else																		\n"
"			gl_FragColor = max( gl_FragColor, Aga*texture2D( gpdTXlo, cl.zw ));							\n"
"		}																				\n"
"	}																					\n"
"																						\n"
"	if( B < 0.5 )																		\n"
"	{																					\n"
"		if( gl_FragColor.a < 0.33 )														\n"
" 			discard;																	\n"
" 		return;																			\n"
"	}																					\n"
"	gl_FragColor += Bc/(6+B+gl_FragColor.a);											\n"
	gpdGUIfrDEPTH
"}\n";
//};

gpdSTATICoff gpsGLSLfrgISO[] = //{
"#version 120																							\n"
"varying vec2 fr_uv;																					\n"
"uniform sampler2D tex0;					// MINI_CHAR_xXy_zXw.png									\n"
"uniform sampler2D tex1;					// BackGround												\n"
"uniform sampler2D tex2;					// MINI 	ABGR?											\n"
"										// U4		XYZW											\n"
"										//			cFcA 											\n"
"//uniform sampler2D tex3;				// CUBE														\n"
"uniform vec2 tgPX;																						\n"
"uniform vec2 DIVxy;																					\n"
"uniform vec2 FRMwh;																					\n"
"uniform vec2 aTX[8];\n"
"#define gpdTXmin	tex0																				\n"
"#define gpdTXbg		tex1																					\n"
"#define gpdTXutf	tex2\n"
"#define gpdTXcrl	vec3( aTX[0], 4 )		// aTX[0] = vec2( 32, 32 )									\n"
"#define gpdTXwh	vec2( 1024, 1536 )																	\n"
"#define gpdTXiz	vec2( 0, 3.5/1536 )	\n"
"vec4 cr_lut( vec4 rgba, vec3 crl, vec2 lwh ) 				// crl.xyz ColRowLut 	// lwh LutWH		\n"
"{\n"
"	// MINI\n"
"	// U4		\n"
"	// XYZW\n"
"	// ABGR?\n"
"	// cFcA\n"
"	vec2 ac = (rgba.rg/crl.xz);																			\n"
"	vec4 o = vec4( 																						\n"
"					floor( vec2( fract(ac.x)*crl.x, ac.x )	),							// char			\n"
"					floor( vec2( fract(ac.y)*crl.z, ac.y )	) +(1.0/3.0) 				// LUT			\n"
"				) / vec4( crl.xy, lwh ) + vec2(0.0, rgba.b*0.25).xyxx;									\n"
"	if( ac.y > crl.z )																					\n"
"		o.w += 1.0;																						\n"
"	return o;																							\n"
"}\n"
"void main()																							\n"
"{\n"
"	const vec4 m0p13 = vec4(-1,0,1, 1.0/3.0);\n"
"	vec2	frm1 = fr_uv*FRMwh,																			\n"
"			frm0 = frm1/aTX[2],								// atx[2] == TXwh							\n"
"			t2x3 = vec2( 1.0/2.0, m0p13.w )/gpdTXcrl.xy,	// aTX[0] == mn_iso_CR_32x32				\n"
"			Ain = fract(frm1)*t2x3,							// atx[0] == mnCR							\n"
"			off0 = vec2( 1.0/2.0, 1.0/4.0 ),															\n"
"			off2x3 = 1.0/aTX[2];							// aTX[3] == mn_iso_CR	\n"
"			\n"
"	gl_FragColor = vec4( texture2D( gpdTXbg, fr_uv ).rgb, 0.25 );	// BG						\n"
"	\n"
"	if( DIVxy.x < 1 )																					\n"
" 		off0.x = 0.0;																					\n"
"	if( DIVxy.y < 1 )																					\n"
" 		off0.y = 0.0;																					\n"
"	\n"
"	// FRM -------------------------------------------------------------------------------\n"
"	vec4	O, A, B,\n"
"			C, D,\n"
"			E,F = texture2D( gpdTXutf, frm0 + off0+vec2(0.0,0.5) )*0x100,\n"
"			cl;\n"
"	if( F.r > 0 ) {																									\n"
"		cl = cr_lut( F, gpdTXcrl*vec3(1,3,1), gpdTXwh );\n"
"		F = texture2D( gpdTXmin, cl.xy+Ain+vec2(0,5.0/32.0) );\n"
"	}\n"
"	// UTF		// MIN		\n"
"	// -1 0		t2x3*vec2	// UT		// MIN														\n"
"	// +-+-+	+--+--+		// -1 0		//  0 1																				\n"
"	// |A|B|-1	|00|10|		// +-+-+	// +-+-+																		\n"
"	// +-+-+	+--+--+		// |D|C| -1	// |E|F| 0																	\n"
"	// |C|D|0	|01|11|		// +-+-+	// +-+-+																			\n"
"	// +-+-+	+--+--+		// |B|A| 0	// |A|B| 1																		\n"
"	// |E|F|1	|02|12|		// +-+-+	// +-+-+																		\n"
"	// +-+-+	+--+--+     // |F|E| 1	// |C|D| 2	\n"
"	// MINI ----------------// +-+-+	// +-+-+	--------------------------------------\n"
"	if( F.a < 0.25 ) {\n"
"		E = texture2D( gpdTXutf, frm0 + off0 )*0x100;\n"
"			cl = cr_lut( E, gpdTXcrl, gpdTXwh );\n"
"		if( E.r > 0 ) {\n"
"			E = texture2D( gpdTXmin, cl.xy+Ain+t2x3*vec2( 0, 2 )-gpdTXiz );\n"
"		} else \n"
"			E = m0p13.yyyy;\n"
"		if( cl.w >= 1.0 ){\n"
"			// ki van jel�lve\n"
"			if( E.a>(16.0/255.0) )\n"
"				F = texture2D( gpdTXmin, cl.zw );\n"
"			else\n"
"				F = m0p13.zzzz*(1.0-E.a);\n"
"		} else {\n"
"			E *= texture2D( gpdTXmin, cl.zw );\n"
"			\n"
"			A = texture2D( gpdTXutf, frm0 + off0 + off2x3*vec2( 0, 2) )*0x100;	\n"
"			B = texture2D( gpdTXutf, frm0 + off0 + off2x3*vec2(-1, 2) )*0x100;	\n"
"			C = texture2D( gpdTXutf, frm0 + off0 + off2x3*vec2( 0, 1) )*0x100;	\n"
"			D = texture2D( gpdTXutf, frm0 + off0 + off2x3*vec2(-1, 1) )*0x100;	\n"
"			F = texture2D( gpdTXutf, frm0 + off0 + off2x3*vec2(-1, 0) )*0x100;	\n"
"			if( A.r > 0 ) {\n"
"				cl = cr_lut( A, gpdTXcrl, gpdTXwh );\n"
"				A = texture2D( gpdTXmin, cl.xy+Ain+t2x3*vec2( 0, 0 )-gpdTXiz )*texture2D( gpdTXmin, cl.zw );\n"
"			} else \n"
"				A = m0p13.yyyy; \n"
"			if( B.r > 0 ) {\n"
"				cl = cr_lut( B, gpdTXcrl, gpdTXwh );\n"
"				B = texture2D( gpdTXmin, cl.xy+Ain+t2x3*vec2( 1, 0 )-gpdTXiz )*texture2D( gpdTXmin, cl.zw );\n"
"			} else \n"
"				B = m0p13.yyyy; \n"
"			\n"
"			if( C.r > 0 ) {\n"
"				cl = cr_lut( C, gpdTXcrl, gpdTXwh );\n"
"				C = texture2D( gpdTXmin, cl.xy+Ain+t2x3*vec2( 0, 1 )-gpdTXiz )*texture2D( gpdTXmin, cl.zw );\n"
"			} else\n"
"				C = m0p13.yyyy;\n"
"			\n"
"			if( D.r > 0 ) {\n"
"				cl = cr_lut( D, gpdTXcrl, gpdTXwh );\n"
"				D = texture2D( gpdTXmin, cl.xy+Ain+t2x3*vec2( 1, 1 )-gpdTXiz )*texture2D( gpdTXmin, cl.zw );\n"
"			} else \n"
"				D = m0p13.yyyy; \n"
"			\n"
"			if( F.r > 0 ) {\n"
"				cl = cr_lut( F, gpdTXcrl, gpdTXwh );\n"
"				F = texture2D( gpdTXmin, cl.xy+Ain+t2x3*vec2( 1, 2 )-gpdTXiz )*texture2D( gpdTXmin, cl.zw );\n"
"			} else\n"
"				F = m0p13.yyyy; \n"
"			\n"
"			F = max(max(max(A,B),C), \n"
"					max(max(D,E),F));\n"
"			\n"
"		}\n"
"	} else \n"
"		F *= texture2D( gpdTXmin, cl.zw );\n"
"	\n"
"	//F.a *= dot(F.rgb, m0p13.www );\n"
" 	if( F.a < 1.0/255.0 ) \n"
"		discard; \n"
"	gl_FragColor = gl_FragColor*gl_FragColor.a + F*F.a;										\n"
	gpdGUIfrDEPTH
"}\n";
//};

gpdSTATICoff gpsGLSLfrgISOaa[] = //{
"#version 120																							\n"
"varying vec2 fr_uv;																					\n"
"uniform sampler2D tex0;					// MINI_CHAR_xXy_zXw.png									\n"
"uniform sampler2D tex1;					// BackGround												\n"
"uniform sampler2D tex2;					// MINI 	ABGR?											\n"
"										// U4		XYZW											\n"
"										//			cFcA 											\n"
"//uniform sampler2D tex3;				// CUBE														\n"
"uniform vec2 tgPX;																						\n"
"uniform vec2 DIVxy;																					\n"
"uniform vec2 FRMwh;																					\n"
"uniform vec2 aTX[8];\n"
"#define gpdTXmin	tex0																				\n"
"#define gpdTXbg		tex1																					\n"
"#define gpdTXutf	tex2\n"
"#define gpdTXcrl	vec3( aTX[0], 4 )		// aTX[0] = vec2( 32, 32 )									\n"
"#define gpdTXwh	vec2( 1024, 1536 )																	\n"
"#define gpdTXiz	vec2( 0, 3.5/1536.0 )	\n"
"vec4 cr_lut( vec4 rgba, vec3 crl, vec2 lwh ) 				// crl.xyz ColRowLut 	// lwh LutWH		\n"
"{\n"
"	// MINI\n"
"	// U4		\n"
"	// XYZW\n"
"	// ABGR?\n"
"	// cFcA\n"
"	vec2 ac = (rgba.rg/crl.xz);																			\n"
"	vec4 o = vec4( 																						\n"
"					floor( vec2( fract(ac.x)*crl.x, ac.x )	),							// char			\n"
"					floor( vec2( fract(ac.y)*crl.z, ac.y+(1.0/3.0) )	)  				// LUT			\n"
"				) / vec4( crl.xy, lwh ) + vec2(0.0, rgba.b*0.25).xyxx;									\n"
"	if( ac.y > crl.z )																					\n"
"		o.w += 1.0;																						\n"
"	return o;																							\n"
"}\n"
"void main()																							\n"
"{\n"
"	const vec4 m0p13 = vec4(-1,0,1, 1.0/3.0);\n"
"	const vec3 aa = vec3( 1.0/1024.0, 1.0/1536.0, 0 );\n"
"	vec2	frm1 = fr_uv*FRMwh,																			\n"
"			frm0 = frm1/aTX[2],								// atx[2] == TXwh							\n"
"			t2x3 = vec2( 1.0/2.0, m0p13.w )/gpdTXcrl.xy,	// aTX[0] == mn_iso_CR_32x32				\n"
"			Ain = fract(frm1)*t2x3,							// atx[0] == mnCR							\n"
"			off0 = vec2( 1.0/2.0, 1.0/4.0 ),															\n"
"			off2x3 = 1.0/aTX[2];							// aTX[3] == mn_iso_CR	\n"
"			\n"
"	gl_FragColor = vec4( texture2D( gpdTXbg, fr_uv ).rgb, 0.25 );	// BG						\n"
"	\n"
"	if( DIVxy.x < 1 )																					\n"
" 		off0.x = 0.0;																					\n"
"	if( DIVxy.y < 1 )																					\n"
" 		off0.y = 0.0;																					\n"
"	\n"
"	// FRM -------------------------------------------------------------------------------\n"
"	vec4	O, A, B,\n"
"			C, D,\n"
"			E,F = texture2D( gpdTXutf, frm0 + off0+vec2(0.0,0.5) )*0x100,\n"
"			cl, cl2, ec;\n"
"	if( F.r > 0 ) {																									\n"
"		cl = cr_lut( F, gpdTXcrl*vec3(1,3,1), gpdTXwh );\n"
"		F = texture2D( gpdTXmin, cl.xy+Ain+vec2(0,5.0/32.0) );\n"
"	}\n"
"	// UTF		// MIN		\n"
"	// -1 0		t2x3*vec2	// UT		// MIN														\n"
"	// +-+-+	+--+--+		// -1 0		//  0 1																				\n"
"	// |A|B|-1	|00|10|		// +-+-+	// +-+-+																		\n"
"	// +-+-+	+--+--+		// |D|C| -1	// |E|F| 0																	\n"
"	// |C|D|0	|01|11|		// +-+-+	// +-+-+																			\n"
"	// +-+-+	+--+--+		// |B|A| 0	// |A|B| 1																		\n"
"	// |E|F|1	|02|12|		// +-+-+	// +-+-+																		\n"
"	// +-+-+	+--+--+     // |F|E| 1	// |C|D| 2	\n"
"	// MINI ----------------// +-+-+	// +-+-+	--------------------------------------\n"
"	if( F.a < 0.25 ) {\n"
"		E = texture2D( gpdTXutf, frm0 + off0 )*0x100;\n"
"			cl = cr_lut( E, gpdTXcrl, gpdTXwh );\n"
"		if( E.r > 0 ) {\n"
"			E = texture2D( gpdTXmin, cl.xy+Ain+t2x3*vec2( 0, 2 )-gpdTXiz );\n"
"			ec = texture2D( gpdTXmin, cl.zw ); \n"
"		} else \n"
"			E = ec = m0p13.yyyy;\n"
"		if( cl.w >= 1.0 ){\n"
"			// ki van jel�lve\n"
"			//if( E.a>(1.0/255.0) )\n"
"			//	F = texture2D( gpdTXmin, cl.zw );\n"
"			//else\n"
"				F = m0p13.zzzz*(1.0-E.a);\n"
"		} else {\n"
"			E *= ec;\n"
"			A = texture2D( gpdTXutf, frm0 + off0 + off2x3*vec2( 0, 2) )*0x100;	\n"
"			B = texture2D( gpdTXutf, frm0 + off0 + off2x3*vec2(-1, 2) )*0x100;	\n"
"			C = texture2D( gpdTXutf, frm0 + off0 + off2x3*vec2( 0, 1) )*0x100;	\n"
"			D = texture2D( gpdTXutf, frm0 + off0 + off2x3*vec2(-1, 1) )*0x100;	\n"
"			F = texture2D( gpdTXutf, frm0 + off0 + off2x3*vec2(-1, 0) )*0x100;	\n"
"			if( A.r > 0 ) {\n"
"				cl2 = cr_lut( A, gpdTXcrl, gpdTXwh );\n"
"				A = texture2D( gpdTXmin, cl2.xy+Ain+t2x3*vec2( 0, 0 )-gpdTXiz )*texture2D( gpdTXmin, cl2.zw );\n"
"			} else \n"
"				A = texture2D( gpdTXmin, cl.xy+Ain+t2x3*vec2( 0, 2 )-gpdTXiz+aa.xz )*ec; \n"
"			if( B.r > 0 ) {\n"
"				cl2 = cr_lut( B, gpdTXcrl, gpdTXwh );\n"
"				B = texture2D( gpdTXmin, cl2.xy+Ain+t2x3*vec2( 1, 0 )-gpdTXiz )*texture2D( gpdTXmin, cl2.zw );\n"
"			} else \n"
"				B = texture2D( gpdTXmin, cl.xy+Ain+t2x3*vec2( 0, 2 )-gpdTXiz+aa.xy )*ec;\n"
"			\n"
"			if( C.r > 0 ) {\n"
"				cl2 = cr_lut( C, gpdTXcrl, gpdTXwh );\n"
"				C = texture2D( gpdTXmin, cl2.xy+Ain+t2x3*vec2( 0, 1 )-gpdTXiz )*texture2D( gpdTXmin, cl2.zw );\n"
"			} else\n"
"				C = texture2D( gpdTXmin, cl.xy+Ain+t2x3*vec2( 0, 2 )-gpdTXiz+aa.zy )*ec;\n"
"			\n"
"			if( D.r > 0 ) {\n"
"				cl2 = cr_lut( D, gpdTXcrl, gpdTXwh );\n"
"				D = texture2D( gpdTXmin, cl2.xy+Ain+t2x3*vec2( 1, 1 )-gpdTXiz )*texture2D( gpdTXmin, cl2.zw );\n"
"			} else \n"
"				D = texture2D( gpdTXmin, cl.xy+Ain+t2x3*vec2( 0, 2 )-gpdTXiz-aa.xz )*ec; \n"
"			\n"
"			if( F.r > 0 ) {\n"
"				cl2 = cr_lut( F, gpdTXcrl, gpdTXwh );\n"
"				F = texture2D( gpdTXmin, cl2.xy+Ain+t2x3*vec2( 1, 2 )-gpdTXiz )*texture2D( gpdTXmin, cl2.zw );\n"
"			} else\n"
"				F = texture2D( gpdTXmin, cl.xy+Ain+t2x3*vec2( 0, 2 )-gpdTXiz-aa.xy )*ec; \n"
"			\n"
"			F = max(max(max(A,B),C), \n"
"					max(max(D,E),F));\n"
"			\n"
"		}\n"
"	} else \n"
"		F *= texture2D( gpdTXmin, cl.zw );\n"
"	\n"
"	//F.a *= dot(F.rgb, m0p13.www );\n"
" 	if( F.a < 1.0/255.0 ) \n"
"		discard; \n"
"	gl_FragColor = gl_FragColor*gl_FragColor.a + F*F.a;										\n"
	gpdGUIfrDEPTH
"}\n";
//};



#endif
