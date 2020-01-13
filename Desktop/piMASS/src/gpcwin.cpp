//#include "gpcwin.h"
#include "gpccrs.h"
//#include "gpcSRC.h"
extern U1 gpsKEYMAP[];


gpcWIN::~gpcWIN()
{
	for( U1 i = 0, e = gpmN(apTall); i < e; i++ )
	{
		if( apT[i] )
		{
			aT[i].join();
			apT[i] = NULL;
		}
		gpmDEL( apTall[i] );
	}

	gpmDEL( pSYNwin );
    gpmDEL( pSYNgt );
    if( pSRFload != pSRFchar )
		gpmSDL_FreeSRF( pSRFload );
	gpmSDL_FreeSRF( pSRFchar );
	gpmSDL_FreeSRF( pSRFiso );
	gpmSDL_FreeSRF( pSRFsnd );


	gpmDEL(pGL);
    SDL_DestroyWindow( pSDLwin );
    SDL_DestroyRenderer( pSDLrndr );
}


InitError::InitError() :
    exception(),
    msg( SDL_GetError() )
{
}

InitError::InitError( const std::string & m ) :
    exception(),
    msg( m )
{
}

InitError::~InitError() throw()
{
}

const char * InitError::what() const throw()
{
    return msg.c_str();
}

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
"	vec2 ac = (rgba.rg/crl.xz);																						\n"
"	return vec4( 																								\n"
"					floor( vec2(fract(ac.x)*crl.x, ac.x)	),							// char					\n"
"					floor( vec2(fract(ac.y)*crl.z, ac.y)	)  							// LUT					\n"
"				) / vec4( crl.xy, lwh ) + vec2(0.0, rgba.b*0.25).xyxx;												\n"
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
/*"																										\n"
"	cl	= cr_lut( 																						\n"
"					texture2D( gpdTXut, frm0 + off0 + vec2( 0, 1.0/2.0 ) ), 							\n"
"					gpdTXcrl, gpdTXwh );																\n"
"	vec4 FR = texture2D( gpdTXlo, cl.xy+Ain+t2x3*vec2( 0, 2 ) );										\n"
"	 																									\n"*/
"	// -1 0																								\n"
"	// +-+-+																							\n"
"	// |B|A| 0																							\n"
"	// +-+-+																							\n"
"	// |D|C| 1																							\n"
"	// +-+-+																							\n"
"	// |F|E| 2																							\n"
"	// +-+-+																							\n"
"	// FRM -------------------------------------------------------										\n"
"	vec4 A = texture2D( gpdTXut, frm0 + off0+vec2(0.0,0.5) )*0x100, cl;						\n"
"	if( A.r > 0 )																						\n"
"	{																									\n"
"		cl	= cr_lut( A, gpdTXcrl, gpdTXwh );															\n"
"		gl_FragColor += texture2D( gpdTXlo, cl.xy+Ain+t2x3*vec2( 0, 2 ) )*texture2D( gpdTXlo, cl.zw );		\n"
"	}																									\n"

"	// B --------------------------------------------------------										\n"
"	A = texture2D( gpdTXut, frm0 + off0 + off2x3*vec2( -1, 0 ) )*0x100;						\n"
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
"	// A --------------------------------------------------------										\n"
"	A = texture2D( gpdTXut, frm0 + off0 ) *0x100;										\n"
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
"																										\n"
"	// D --------------------------------------------------------										\n"
"	A = texture2D( gpdTXut, frm0 + off0 + off2x3*vec2(-1, 1 ) )*0x100;									\n"
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
"	// C --------------------------------------------------------										\n"
"	A = texture2D( gpdTXut, frm0 + off0 + off2x3*vec2( 0, 1 ) )*0x100;									\n"
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

"	// F --------------------------------------------------------										\n"
"	A = texture2D( gpdTXut, frm0 + off0 + off2x3*vec2( -1, 2 ) )*0x100;							\n"
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
"	// E --------------------------------------------------------										\n"
"	A = texture2D( gpdTXut, frm0 + off0 + off2x3*vec2( 0, 2 ) )*0x100;									\n"
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


"}\n";

char gpsSHDRisoFRG[] =
"#version 120																		\n"
"varying vec2 fr_uv;																\n"
"uniform sampler2D tex0;						// MINI_CHAR_xXy_zXw.png			\n"
"uniform sampler2D tex1;						// BackGround						\n"
"uniform sampler2D tex2;						// MINI 	ABGR?					\n"
"											// U4		XYZW					\n"
"											//			cFcA 	\n"
"uniform sampler2D tex3;						// CUBE												\n"
"uniform vec2 tgPX;																	\n"
"uniform vec2 DIVxy;																	\n"
"uniform vec2 FRMwh;																	\n"
"uniform vec2 aTX[8];\n"

"#define gpdTX_lo 	tex0\n"
"#define gpdTX_loCRL	vec3( aTX[0], 4 )\n"
"#define gpdTX_loWH	vec2( 128, 1024 )\n"

"#define gpdTX_bg tex1\n"
"#define gpdTX_ut tex2\n"

"#define gpdTX_hi tex3\n"
"#define gpdTX_hiCRL	vec3( aTX[3], 4 )\n"
"#define gpdTX_hiWH	vec2( 256, 1536 )\n"

"vec2 ac_more( vec4 A )	\n"
"{	\n"
"	return A.rg; //+vec2(A.g*0x100,0);																			\n"
"}																												\n"
"vec4 cr_lut( vec2 ac, vec3 crl, vec2 lwh ) 							// crl.xyz ColRowLut // lwh LutWH		\n"
"{																					\n"
"	ac /= crl.xz; 																	\n"
"	return vec4( 																	\n"
"						floor( vec2(fract(ac.x)*crl.x, ac.x) ), 	// char				\n"
"						floor( vec2(fract(ac.y)*crl.z, ac.y) )		// LUT		  		\n"
"					)																\n"
"			/ vec4(	crl.xy, lwh );																\n"
"}	\n"
"																					\n"
"void main()																		\n"
"{																					\n"
"	vec2	frm1 = fr_uv*FRMwh,														\n"
"			frm0 = frm1/aTX[2],								// atx[2] == TXwh		\n"
"			Ain = fract(frm1)/aTX[0],						// atx[0] == mnCR		\n"
"			off0 = vec2( 1.0/4.0, 1.0/8.0 );										\n"
"	if( DIVxy.x < 1 )							\n"
" 		off0.x = 0.0;							\n"
"	if( DIVxy.y < 1 )							\n"
" 		off0.y = 0.0;							\n"
"	vec4	A = texture2D( gpdTX_ut, frm0 + off0 )*0x100,	\n"
"			B;								\n"
"																						\n"
"	gl_FragColor = vec4( texture2D( gpdTX_bg, fr_uv ).rgb, 0 );				// BG		\n"
"	gl_FragColor *= gl_FragColor;											// BG		\n"
"	vec4	fr		= cr_lut( A.ba+vec2(0xb0,0), gpdTX_loCRL, gpdTX_loWH ),				\n"
"			fr_rgb	= texture2D( gpdTX_lo, fr.zw ),										\n"
"			cl		= cr_lut( A.rg, gpdTX_loCRL, gpdTX_loWH ),							\n"
"			cl_rgb	= texture2D( gpdTX_lo, cl.zw );										\n"
"	if( A.b != 0 )																		\n"
"		gl_FragColor += texture2D( gpdTX_lo, fr.xy+Ain )*fr_rgb;						\n"
"																						\n"
"	if( A.r != 0 ) {																	\n"
"		if( A.r <= 0x60 )																\n"
"			B = texture2D( gpdTX_lo, cl.xy+Ain )*cl_rgb;								\n"
"		else {																			\n"
"			cl.zw = (A.rr - vec2( 0x20, -0x20 ))/0x10;									\n"
"																						\n"
"			cl.z = texture2D( gpdTX_lo,													\n"
"											floor(vec2(fract(cl.z)*0x10, cl.z)) 		\n"
"											/ gpdTX_loWH								\n"
"								).a;													\n"
"			cl.w = texture2D( gpdTX_lo,													\n"
"											floor(vec2(fract(cl.w)*0x10, cl.w)) 		\n"
"											/ gpdTX_loWH								\n"
"								).a;													\n"
"			cl.zw *= 0x100;																\n"
"			if( cl.z >= 0x41 &&  cl.z <= 0x5a )											\n"
"				cl.w += 0x68;															\n"
"			else																		\n"
"				cl.w += 0x60;															\n"
"																						\n"
"			cl.zw /= 8;																	\n"
"			cl = vec4( 																					\n"
"							floor(vec2(fract(cl.z)*0x8, cl.z)),											\n"
"							floor(vec2(fract(cl.w)*0x8, cl.w))											\n"
"						) / vec2( 8, 32 ).xyxy + Ain.xyxy;												\n"
"			B = max( texture2D(gpdTX_lo, cl.xy ), texture2D(gpdTX_lo, cl.zw ) ) * cl_rgb;				\n"
"		}																								\n"
"																										\n"
"		if( (B.r+B.g+B.b) > 1.0 )																		\n"
"		{																								\n"
"			gl_FragColor = B;																			\n"
//"			return;																						\n"
"		} else																							\n"
"			gl_FragColor += B;																			\n"
"	}																									\n"
"																										\n"
"																										\n"
"	vec4 frnt = vec4(0.0,0.0,0.0,0.0);																	\n"
"	frm1 *= vec2(2.0,3.0);	// (frm1 = fr_uv * FRMwh) *= 2;												\n"
"	frm0 *= vec2(2.0,3.0);	// frm0 = fr_uv * FRMwh*2 / aTX[2];		// aTX[2] == TXwh					\n"
//"	off0 *= 2.0;																						\n"
//"	off0.y += 1.0/3.0;																					\n"
"	vec2	off2x3 = 1.0/aTX[2],										// aTX[2] == TXwh				\n"
"			t2x3 = 1.0/(aTX[3]*vec2( 2.0, 3.0 ));						// aTX[3] == mn_iso_CR			\n"
"	Ain = t2x3*fract(frm1);												\n"
"																										\n"
"	A = texture2D( gpdTX_ut, frm0 + off0 )*0x100;														\n"
"	cl		= cr_lut( ac_more(A), gpdTX_hiCRL, gpdTX_loWH );											\n"
"	A 		= texture2D( gpdTX_hi, cl.xy+Ain+t2x3*vec2( 0, 2 ) );										\n"
"	if( A.g*A.a > 0.75 )																				\n"
"	{	gl_FragColor = A.ggga*texture2D( gpdTX_lo, cl.zw ); return; }									\n"
"																										\n"
"	A = texture2D( gpdTX_ut, frm0 + off0 + off2x3*vec2( -1, 0 ) )*0x100;								\n"
"	cl		= cr_lut( ac_more(A), gpdTX_hiCRL, gpdTX_loWH );											\n"
"	A 		= texture2D( gpdTX_hi, cl.xy+Ain+t2x3*vec2( 1, 2 ) );										\n"
"	if( A.g*A.a > 0.75 )																				\n"
"	{	gl_FragColor = A.ggga*texture2D( gpdTX_lo, cl.zw ); return; }									\n"
"																										\n"
"																										\n"
"	A = texture2D( gpdTX_ut, frm0 + off0 + off2x3*vec2(  0, 1 ) )*0x100;								\n"
"	cl		= cr_lut( ac_more(A), gpdTX_hiCRL, gpdTX_loWH );											\n"
"	A 		= texture2D( gpdTX_hi, cl.xy+Ain+t2x3*vec2( 0, 1 ) );										\n"
"	if( A.g*A.a > 0.75 )																				\n"
"	{	gl_FragColor = A.ggga*texture2D( gpdTX_lo, cl.zw ); return; }									\n"
"																										\n"
"	A = texture2D( gpdTX_ut, frm0 + off0 + off2x3*vec2( -1, 1 ) )*0x100;								\n"
"	cl		= cr_lut( ac_more(A), gpdTX_hiCRL, gpdTX_loWH );											\n"
"	A 		= texture2D( gpdTX_hi, cl.xy+Ain+t2x3*vec2( 1, 1 ) );										\n"
"	if( A.g*A.a > 0.75 )																				\n"
"	{	gl_FragColor = A.ggga*texture2D( gpdTX_lo, cl.zw ); return; }									\n"
"																										\n"
"																										\n"
"	A = texture2D( gpdTX_ut, frm0 + off0 + off2x3*vec2(  0, 2 ) )*0x100;								\n"
"	cl		= cr_lut( ac_more(A), gpdTX_hiCRL, gpdTX_loWH );											\n"
"	A 		= texture2D( gpdTX_hi, cl.xy+Ain+t2x3*vec2( 0, 0 ) );										\n"
"	if( A.g*A.a > 0.75 )																				\n"
"	{	gl_FragColor = A.ggga*texture2D( gpdTX_lo, cl.zw ); return; }									\n"
"																										\n"
"	A = texture2D( gpdTX_ut, frm0 + off0 + off2x3*vec2( -1, 2 ) )*0x100;								\n"
"	cl		= cr_lut( ac_more(A), gpdTX_hiCRL, gpdTX_loWH );											\n"
"	A 		= texture2D( gpdTX_hi, cl.xy+Ain+t2x3*vec2( 1, 0 ) );										\n"
"	if( A.g*A.a > 0.75 )																				\n"
"	{	gl_FragColor = A.ggga*texture2D( gpdTX_lo, cl.zw ); return; }									\n"
"																										\n"

"	gl_FragColor += frnt;																				\n"
"}																					\n\0"	// ScHöNT elftárs // ki kapcsoltuk a lentit

"	B = texture2D( gpdTX_ut, frm0 + off0 + off_in*vec2( -1, 0 ) )*0x100;																	\n"
"	vec4	C = texture2D( gpdTX_ut, frm0 + off0 + off_in*vec2(  0, 1 ) )*0x100,															\n"
"			D = texture2D( gpdTX_ut, frm0 + off0 + off_in*vec2( -1, 1 ) )*0x100,																\n"
"			E = texture2D( gpdTX_ut, frm0 + off0 + off_in*vec2(  0, 2 ) )*0x100,																\n"
"			F = texture2D( gpdTX_ut, frm0 + off0 + off_in*vec2( -1, 2 ) )*0x100;																\n"
"	off_in = vec2(8, 32/3.0)/vec2(128, 1024);																								\n"
"																																			\n"

"	cl		= cr_lut( ac_more(E), gpdTX_hiCRL, gpdTX_loWH );											\n"
"	E 		= texture2D( gpdTX_hi, cl.xy+Ain+off_in*vec2( 0, -2 ));	\n"
"	if( E.g*E.a > 0.75 )	\n"
"		frnt += E.ggga*texture2D( gpdTX_hi, cl.zw );	\n"
"		\n"
"	cl		= cr_lut( ac_more(B), gpdTX_hiCRL, gpdTX_loWH );											\n"
"	B		= texture2D( gpdTX_hi, cl.xy+Ain+off_in*vec2( 1, 0 ));	\n"
"	if( B.g*B.a > 0.75 )	\n"
"		frnt += B.ggga*texture2D( gpdTX_hi, cl.zw );	\n"
"	if( B.a >= E.a )		\n"
"		E = B;	\n"
"			\n"
"	cl		= cr_lut( ac_more(C), gpdTX_hiCRL, gpdTX_loWH );											\n"
"	C		= texture2D( gpdTX_hi, cl.xy+Ain+off_in*vec2( 0, -1 ));	\n"
"	if( C.g*C.a > 0.75 )	\n"
"		frnt += C.ggga*texture2D( gpdTX_hi, cl.zw );	\n"
"	if( C.a >= E.a )		\n"
"		E = C;	\n"
"		\n"
"	cl		= cr_lut( ac_more(F), gpdTX_hiCRL, gpdTX_loWH );											\n"
"	F		= texture2D( gpdTX_hi, cl.xy+Ain+off_in*vec2( 1, -2 ));	\n"
"	if( F.g*F.a > 0.75 )	\n"
"		frnt += F.ggga*texture2D( gpdTX_hi, cl.zw );	\n"
"	if( F.a >= E.a )		\n"
"		E = F;	\n"
"		\n"

"	if( F.a >= E.a )		\n"
"		E = F;	\n"
"			\n"
"	cl		= cr_lut( ac_more(D), gpdTX_hiCRL, gpdTX_loWH );											\n"
"	D		= texture2D( gpdTX_hi, cl.xy+Ain+off_in*vec2(-1, -1 ));	\n"
"	if( D.g*D.a > 0.75 )	\n"
"		frnt += D.ggga*texture2D( gpdTX_hi, cl.zw );	\n"
"	if( D.a >= E.a )		\n"
"		E = D;	\n"
"			\n"
"	gl_FragColor += frnt+E;	\n"
"		\n"
"}\n\0";
char gpsSHDRiso[] =
	"#version 120																	\n"
"varying vec2 fr_uv;															\n"
"uniform sampler2D tex0;						// MINI_CHAR_xXy_zXw.png		\n"
"uniform sampler2D tex1;						// BackGround					\n"
"uniform sampler2D tex2;						// MINI 	ABGR?				\n"
	"										// U4		XYZW				\n"
	"										//			cFcA \n"
"uniform sampler2D tex3;						// CUBE											\n"
"uniform vec2 tgPX;																\n"
"uniform vec2 DIVxy;																\n"
"uniform vec2 FRMwh;																\n"
"uniform vec2 aTX[8];\n"
"vec2 ac_more( vec4 A )\n"
"{\n"
	"return A.rg; //+vec2(A.g*0x100,0);																	\n"
"}															\n"
"vec4 cr_lut( vec2 ac, vec3 crl, vec2 lwh ) 							// crl.xyz ColRowLut // lwh LutWH															\n"
"{																				\n"
	"ac /= crl.xz; 														\n"
	"return vec4( 																\n"
	"					floor( vec2(fract(ac.x)*crl.x, ac.x) ), 	// char			\n"
	"					floor( vec2(fract(ac.y)*crl.z, ac.y) )		// LUT		  	\n"
	"				)															\n"
	"		/ vec4(	crl.xy, lwh );															\n"
"}\n"
	"																			\n"
"void main()																	\n"
"{																				\n"
	"vec2	frm1 = fr_uv*FRMwh,													\n"
	"		Ain = fract(frm1)/aTX[0],										\n"
	"		frm0 = frm1/aTX[2],													\n"
	"		off0 = vec2( 1.0/4.0, 1.0/6.0 );								\n"
	"if( DIVxy.x < 1 )						\n"
 "		off0.x = 0.0;						\n"
	"if( DIVxy.y < 1 )						\n"
 "		off0.y = 0.0;						\n"
	"vec4	A = texture2D( tex2, frm0 + off0 )*0x100,\n"
	"		B;							\n"
	"																			\n"
	"gl_FragColor = vec4( texture2D( tex1, fr_uv ).rgb, 0 );			// BG	\n"
	"vec4	fr		= cr_lut( A.ba+vec2(0xb0,0), vec3(8, 32, 4), vec2( 128, 1024 ) ),						\n"
	"		fr_rgb	= texture2D( tex0, fr.zw ),									\n"
	"		cl		= cr_lut( A.rg, vec3(8, 32, 4), vec2( 128, 1024 ) ),										\n"
	"		cl_rgb	= texture2D( tex0, cl.zw );									\n"
	"if( A.b != 0 )															\n"
	"	gl_FragColor += texture2D( tex0, fr.xy+Ain )*fr_rgb;					\n"
	"																			\n"
	"if( A.r != 0 ) {																			\n"
	"	if( A.r <= 0x60 )															\n"
	"		B = texture2D( tex0, cl.xy+Ain )*cl_rgb;					\n"
	"	else {																		\n"
	"		cl.zw = (A.rr - vec2( 0x20, -0x20 ))/0x10;							\n"
	"																				\n"
	"		cl.z = texture2D( tex0,													\n"
	"										floor(vec2(fract(cl.z)*0x10, cl.z)) 	\n"
	"										/ vec2( 128, 1024 )					\n"
	"							).a;												\n"
	"		cl.w = texture2D( tex0,													\n"
	"										floor(vec2(fract(cl.w)*0x10, cl.w)) 	\n"
	"										/ vec2( 128, 1024 )					\n"
	"							).a;												\n"
	"		cl.zw *= 0x100;															\n"
	"		if( cl.z >= 0x41 &&  cl.z <= 0x5a )										\n"
	"			cl.w += 0x68;														\n"
	"		else																	\n"
	"			cl.w += 0x60;														\n"
	"																				\n"
	"		cl.zw /= 8;																\n"
	"		cl = vec4( 																			\n"
	"						floor(vec2(fract(cl.z)*0x8, cl.z)),									\n"
	"						floor(vec2(fract(cl.w)*0x8, cl.w))									\n"
	"					) / vec2( 8, 32 ).xyxy + Ain.xyxy;								\n"
	"		B = max( texture2D(tex0, cl.xy ), texture2D(tex0, cl.zw ) ) * cl_rgb;	\n"
	"	}\n"
	"	\n"
	"	if( (B.r+B.g+B.b) > 1.0 )\n"
	"	{\n"
	"		gl_FragColor = B;\n"
	"		return;\n"
	"	}\n"
	"	\n"
	"	gl_FragColor += B;\n"
	"}																\n"
	"																\n"
	"																\n"
	"frm0 *= 2.0;	// frm0 = fr_uv * FRMwh*2 / aTX[2]																\n"
	"off0 *= 2.0;																\n"
	//"off0.y += 1.0/3.0;																\n"
	"vec2 off_in = 1.0/aTX[2];																\n"
	"A = texture2D( tex2, frm0 + off0 )*0x100;																\n"
	"B = texture2D( tex2, frm0 + off0 + off_in*vec2( -1, 0 ) )*0x100;																\n"
	"gl_FragColor += A/0x100;		\n"
	"vec4	C = texture2D( tex2, frm0 + off0 + off_in*vec2(  0, 1 ) )*0x100,\n"
	"		D = texture2D( tex2, frm0 + off0 + off_in*vec2( -1, 1 ) )*0x100,\n"
	"		E = texture2D( tex2, frm0 + off0 + off_in*vec2(  0, 2 ) )*0x100,\n"
	"		F = texture2D( tex2, frm0 + off0 + off_in*vec2( -1, 2 ) )*0x100;\n"
	"frm1 *= 2.0;\n"
	"Ain = fract(frm1)/aTX[3];\n"
	"off_in = vec2(8, 32/3.0)/vec2(128, 1024);\n"
	"\n"
	"vec4	frnt = vec4(0.0,0.0,0.0,0.0);\n"
	"cl		= cr_lut( ac_more(E), vec3(8, 32, 4), vec2( 128, 1024 ) );										\n"
	"E 		= texture2D( tex0, cl.xy+Ain+off_in*vec2( 0, -2 ));\n"
	"if( E.g*E.a > 0.75 )\n"
	"	frnt += E.ggga*texture2D( tex0, cl.zw );\n"
	"\n"
	"cl		= cr_lut( ac_more(B), vec3(8, 32, 4), vec2( 128, 1024 ) );										\n"
	"B		= texture2D( tex0, cl.xy+Ain+off_in*vec2( 1, 0 ));\n"
	"if( B.g*B.a > 0.75 )\n"
	"	frnt += B.ggga*texture2D( tex0, cl.zw );\n"
	"if( B.a >= E.a )	\n"
	"	E = B;\n"
	"	\n"
	"cl		= cr_lut( ac_more(C), vec3(8, 32, 4), vec2( 128, 1024 ) );										\n"
	"C		= texture2D( tex0, cl.xy+Ain+off_in*vec2( 0, -1 ));\n"
	"if( C.g*C.a > 0.75 )\n"
	"	frnt += C.ggga*texture2D( tex0, cl.zw );\n"
	"if( C.a >= E.a )	\n"
	"	E = C;\n"
	"\n"
	"cl		= cr_lut( ac_more(F), vec3(8, 32, 4), vec2( 128, 1024 ) );										\n"
	"F		= texture2D( tex0, cl.xy+Ain+off_in*vec2( 1, -2 ));\n"
	"if( F.g*F.a > 0.75 )\n"
	"	frnt += F.ggga*texture2D( tex0, cl.zw );\n"
	"if( F.a >= E.a )	\n"
	"	E = F;\n"
	"\n"
	"cl		= cr_lut( ac_more(A), vec3(8, 32, 4), vec2( 128, 1024 ) );										\n"
	"A 		= texture2D( tex0, cl.xy+Ain );\n"
	"if( A.g*A.a > 0.75 )\n"
	"	frnt += A.ggga*texture2D( tex0, cl.zw );\n"
	"if( F.a >= E.a )	\n"
	"	E = F;\n"
	"	\n"
	"cl		= cr_lut( ac_more(D), vec3(8, 32, 4), vec2( 128, 1024 ) );										\n"
	"D		= texture2D( tex0, cl.xy+Ain+off_in*vec2(-1, -1 ));\n"
	"if( D.g*D.a > 0.75 )\n"
	"	frnt += D.ggga*texture2D( tex0, cl.zw );\n"
	"if( D.a >= E.a )	\n"
	"	E = D;\n"
	"	\n"
	"gl_FragColor += frnt+E;\n"
	"\n"
"}\n";

char gpsSHDRvx[] =
	"#version 120										\n"
	"attribute	vec2	v_vx;							\n"
	"attribute	vec2	v_uv;							\n"
	"varying	vec2	fr_uv;							\n"
	"void main()										\n"
	"{																	\n"
	"	gl_Position			= vec4( v_vx*vec2(2.0,-2.0)+vec2(-1.0,1.0), 0, 1);		\n"
	"	fr_uv				= v_uv;										\n"
	"}													\n\0";
char gpsSHDR[] =
	"#version 120																	\n"
	"varying vec2 fr_uv;															\n"
	"uniform sampler2D tex0;						// MINI_CHAR_xXy_zXw.png		\n"
	"uniform sampler2D tex1;						// BackGround					\n"
	"uniform sampler2D tex2;						// MINI 	ABGR?				\n"
	"												// U4		XYZW				\n"
	"												//			cFcA 				\n"
	"uniform vec2 tgPX;																\n"
	"uniform vec2 DIVxy;																\n"
	"uniform vec2 FRMwh;																\n"
	"uniform vec2 aTX[8];															\n"
	"vec4 cr_lut( vec2 ac )															\n"
	"{																				\n"
	"	ac /= vec2(8.0,4.0);														\n"
	"	return vec4( 																\n"
	"						floor( vec2(fract(ac.x)*8.0, ac.x) ), 	// char			\n"
	"						floor( vec2(fract(ac.y)*4.0, ac.y) )	// LUT		  	\n"
	"					)															\n"
	"			/ vec4(																\n"
	"						8.0,32.0,												\n"
	"						128.0,1024.0											\n"
	"					);															\n"
	"}																				\n"
	"void main()																	\n"
	"{																				\n"
	"	vec2	frm1 = fr_uv*FRMwh,													\n"
	"			big_in = fract(frm1)/aTX[0],										\n"
	"			frm0 = frm1/aTX[2],													\n"
	"			off0 = vec2( 1.0/4.0, 1.0/6.0 );								\n"
	"	if( DIVxy.x < 1 )						\n"
	" 		off0.x = 0.0;						\n"
	"	if( DIVxy.y < 1 )						\n"
	" 		off0.y = 0.0;						\n"
	"	vec4	big = texture2D( tex2, frm0 + off0 )*0x100;							\n"
	"																				\n"
	"	gl_FragColor = vec4( texture2D( tex1, fr_uv ).rgb, 0.0 );			// BG	\n"
	"	vec4	fr			= cr_lut( big.ba+vec2(0xb0,0) ),						\n"
	"			fr_rgb	= texture2D( tex0, fr.zw ),									\n"
	"			cl			= cr_lut(big.rg),										\n"
	"			cl_rgb	= texture2D( tex0, cl.zw );									\n"
	"	if( big.b != 0 )															\n"
	"		gl_FragColor += texture2D( tex0, fr.xy+big_in )*fr_rgb;					\n"
	"																				\n"
	"	if( big.r != 0 )															\n"
	"	{																			\n"
	"		if( big.r <= 0x60 )															\n"
	"			gl_FragColor += texture2D( tex0, cl.xy+big_in )*cl_rgb;					\n"
	"		else {																		\n"
	"			cl.zw = (big.rr - vec2( 0x20, -0x20 ))/0x10;							\n"
	"																					\n"
	"			cl.z = texture2D( tex0,													\n"
	"											floor(vec2(fract(cl.z)*0x10, cl.z)) 	\n"
	"											/ vec2( 128.0, 1024 )					\n"
	"								).a;												\n"
	"			cl.w = texture2D( tex0,													\n"
	"											floor(vec2(fract(cl.w)*0x10, cl.w)) 	\n"
	"											/ vec2( 128.0, 1024 )					\n"
	"								).a;												\n"
	"			cl.zw *= 0x100;															\n"
	"			if( cl.z >= 0x41 &&  cl.z <= 0x5a )										\n"
	"				cl.w += 0x68;														\n"
	"			else																	\n"
	"				cl.w += 0x60;														\n"
	"																					\n"
	"			cl.zw /= 8;																\n"
	"			cl = vec4( 																			\n"
	"							floor(vec2(fract(cl.z)*0x8, cl.z)),									\n"
	"							floor(vec2(fract(cl.w)*0x8, cl.w))									\n"
	"						) / vec2( 8.0, 32.0 ).xyxy + big_in.xyxy;								\n"
	"			gl_FragColor += max( texture2D(tex0, cl.xy ), texture2D(tex0, cl.zw ) ) * cl_rgb;	\n"
	"		}																						\n"
	"	}																			\n"
	"} \n\0";


char gpsSHDRfr_ref[] =
	"#version 120																		\n"
	"varying vec2 v_uv;																	\n"
	"uniform sampler2D tex0;															\n"
	"uniform sampler2D tex1;															\n"
	"uniform sampler2D tex2;															\n"
	"uniform vec2 crsFRxy;																\n"
	"																					\n"
	"void main()																		\n"
	"{																					\n"
	"	vec4 mini = texture2D(tex0, v_uv)*256.0;										\n"
	"	float 	c = mini.r,			cc = mini.g, 										\n"
	"			f = mini.b+0xb0,	fc = mini.a; 										\n"
	"	vec2 	uv = fract(v_uv*crsFRxy)/vec2(8.0,32.0),								\n"
	"			f_uv =	vec2( 															\n"
	"							floor(fract(f/8.0)*8.0)/8.0,							\n"
	"							floor(f/8.0)/32.0										\n"
	"						) 															\n"
	"					+ uv, 															\n"
	"			fc_uv = vec2( 															\n"
	"							floor(fract(fc/4.0)*4.0)/128.0,							\n"
	"							floor(fc/4.0)/1024.0									\n"
	"						),															\n"
	"			c_uv =	vec2( 															\n"
	"							floor(fract(c/8.0)*8.0)/8.0,							\n"
	"							floor(c/8.0)/32.0										\n"
	"						) 															\n"
	"					+ uv, 															\n"
	"			cc_uv = vec2( 															\n"
	"							floor(fract(cc/4.0)*4.0)/128.0,							\n"
	"							floor(cc/4.0)/1024.0									\n"
	"						);															\n"
	"																					\n"
	"	gl_FragColor =	 																\n"
	"					min( f, 1 ) * texture2D(tex1, f_uv ) * texture2D(tex1, fc_uv )	\n"
	"					+min( c, 1 ) * texture2D(tex1, c_uv ) * texture2D(tex1, cc_uv )	\n"
	"					+vec4( texture2D( tex2, v_uv ).rgb, 0.0 );						\n"
	"}	\n";



char gpsSHDRfrOLD[] =
	"#version 120\n"
	"varying vec2 v_uv;\n"
	"uniform sampler2D tex0;\n"
	"uniform sampler2D tex1;\n"
	"void main()\n"
	"{\n"
	"	gl_FragColor =	max( texture2D(tex0, v_uv),\n"
	"						 texture2D(tex1, v_uv) );\n"
	//"	gl_FragColor.a = 1.0;\n"
	"}\n\0";
//VBO data
static const GLfloat aVxD[] =
{
	-1.0f, -1.0f,
	 1.0f, -1.0f,
	 1.0f,  1.0f,
	-1.0f,  1.0f,
};
static const GLfloat aVX4[] =
{
	-1.0f, -1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,  1.0f,
};

//IBO data
static const GLuint aIxD[] = //{ 3, 2, 1, 0, };
								{ 0, 1, 2, 3 };

gpcGL::gpcGL( gpcWIN& win )
{
	gpmCLR;
	oPrgID = -1;
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
	gCntxt = SDL_GL_CreateContext( win.pSDLwin );
	if( !gCntxt )
	{
		cout << endl << "gpcGL init error" << endl;
		return;
	}
	glewExperimental = GL_TRUE;
	glewErr = glewInit();
	if( glewErr != GLEW_OK )
	{
		cout << endl << "gpcGL GLEW_NOK error" << endl;
		return;
	}

	pTRG = SDL_CreateTexture(
									win.pSDLrndr, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
									win.winSIZ.z, win.winSIZ.w
								);
	trgWH = win.winSIZ.a4x2[1];
	gVxSucc = GL_FALSE;

	pTXchar = SDL_CreateTextureFromSurface( win.pSDLrndr, win.pSRFchar );
	if( pTXchar )
		cout << "char" << (int)win.pSRFchar << endl;
	else
		cout << SDL_GetError() << endl;

	pTXiso = SDL_CreateTextureFromSurface( win.pSDLrndr, win.pSRFiso );
	if( pTXiso )
		cout << "char" << (int)win.pSRFiso << endl;
	else
		cout << SDL_GetError() << endl;
}


U1 gpsHUNtx[] =
" A       E   I  "
"UOoO  O   U U   "
// 0x20 -------------------
" a       e   i  "
"uUuo  o   u u   "
" '       '   '  "
":\"\"'  :   ' :   "
// 0x60 -------------------
" '       '   '  "
":\"\"'  :   ' :   "
// 0x80 -------------------
"0123456789abcdef";
gpcWIN::gpcWIN( char* pPATH, char* pFILE, char* sNAME, gpcMASS* piM )  {
	//ctor
	gpmCLR;
	piMASS = piM;
	SDL_DisplayMode sdlDM;
	SDL_GetCurrentDisplayMode( 0, &sdlDM );
	winSIZ.x = (sdlDM.w*7)/8;
	winSIZ.y = sdlDM.h-64;

	winSIZ.a4x2[1] = winSIZ.a4x2[0] / (gpdSIZ2CR*2);

	winDIVcr.a4x2[0] = winSIZ.a4x2[1];
	winDIVcr.a4x2[1] = winDIVcr.a4x2[0]*2;

	winSIZ.a4x2[1] = winDIVcr.a4x2[1] & gpdSIZ2CR;

	if( winSIZ.a4x2[0] != winSIZ.a4x2[1] )
		winSIZ.a4x2[0] = winSIZ.a4x2[1];

	winDIVpx = winSIZ / I4x4( 2,2,1,1 );

	pSDLwin = SDL_CreateWindow(	"Custom shader with SDL2 renderer!", SDL_WINDOWPOS_CENTERED,
								SDL_WINDOWPOS_CENTERED, winSIZ.z, winSIZ.w, SDL_WINDOW_RESIZABLE|SDL_WINDOW_OPENGL );

	SDL_SetHint( SDL_HINT_RENDER_DRIVER, "1" );

	pSDLrndr = SDL_CreateRenderer(	pSDLwin, -1,
									SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

	/*if(
		winID.x = SDL_CreateWindowAndRenderer(	winSIZ.z, winSIZ.w,
												SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE|SDL_WINDOW_OPENGL,
												&pSDLwin, &pSDLrndr ) != 0
	)
        throw InitError();
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1" );
	*/

	if( !(pSRFwin = SDL_GetWindowSurface( pSDLwin )) )
		throw InitError();

	if( !pPATH )
		return;

	gpmSTRCPY( gpsMASSname, sNAME );
	gpmSTRCPY( gpsMASSpath, pPATH );
	gppMASSfile = gpsMASSpath+(pFILE-pPATH);

	pSRFload = IMG_Load( gpsMASSpath );
	pSRFchar = pSRFload;
	if( pSRFchar != pSRFload )
		gpmSDL_FreeSRF( pSRFload );


	chrPIC.x = 8; //*4;
	chrPIC.y = 32; //*4;
	chrPIC.w = pSRFchar->w/chrPIC.x;
	chrPIC.h = pSRFchar->h/chrPIC.y;
	U1x4* pC = (U1x4*)(pSRFchar->pixels + 4*pSRFchar->pitch);
	for( U4 i = 0; i < 0x80; i++ )
	{
		pC[(i>>4)*pSRFchar->w + (i&0xf)] = 0x01010101*(gpsHUNtx[i]-' ');
	}

	gpmSTRCPY( gppMASSfile, gpsMINI_ISO );
	pSRFiso = IMG_Load( gpsMASSpath );


	gppKEYbuff = gpsKEYbuff;
	gppMOUSEbuff = gpsKEYbuff;

	pHOST = sHOST;
	pUSER = sUSER;

	pGL = true ? new gpcGL( *this ) : NULL;
	if( !pGL ) //? !pGL->pTXback : true )
		return;

	pGL->VxScmp( gpsSHDRvx );
	if( pGL->gVxSucc != GL_TRUE )
		return;

	//pGL->FrgSHDRcmpi( gpsSHDR );
	//pGL->FrgSHDRcmpi( gpsSHDRiso );
	pGL->FrgSHDRcmpi( gpsSHDRisoFRG1x2 );
	if( pGL->gFrSucc != GL_TRUE )
		return;

	pGL->VxFrLink();
	if( pGL->gPrgSucc != GL_TRUE )
		return;

	//pGL->VBOnew( aVxD, gpmN(aVxD)/2, 2 );
	pGL->IBOnew( aIxD, gpmN(aIxD) );

}
void gpcWIN::gpeWINresize( void )
{
	for( U1 i = 0; i < 4; i++ )
	{
		if( apT[i] )
		{
			aT[i].join();
			apT[i] = NULL;
		}

	}

	if( !(pSRFwin = SDL_GetWindowSurface( pSDLwin )) )
		throw InitError();
	SDL_GetWindowSize( pSDLwin, &winSIZ.x, &winSIZ.y );

	winSIZ.a4x2[1] = winSIZ.a4x2[0] / (gpdSIZ2CR*2);

	winDIVcr.a4x2[0] = winSIZ.a4x2[1];
	winDIVcr.a4x2[1] = winDIVcr.a4x2[0]*2;

	winSIZ.a4x2[1] = winDIVcr.a4x2[1] & gpdSIZ2CR;

	if( winSIZ.a4x2[0] != winSIZ.a4x2[1] )
	{
		winSIZ.a4x2[0] = winSIZ.a4x2[1];
		SDL_SetWindowSize( pSDLwin, winSIZ.z, winSIZ.w );
	}
	winDIVpx = winSIZ / I4x4( 2,2,1,1 );

	if( !pGL )
		return;

	/*gpmSDL_FreeTX( pGL->pTRG );
    pGL->pTRG = SDL_CreateTexture( pSDLrndr, SDL_PIXELFORMAT_BGRA8888, SDL_TEXTUREACCESS_TARGET, winSIZ.z, winSIZ.w );
	if( pGL->pTRG )
		return;

	gpmDEL(pGL);*/

}


void gpcWIN::WINrun( const char* pWELLCOME )
{
	U4 scan, bug = 0, nBUG;
	U1 aXY[] = "00", c = 0;

	gppKEYbuff = ( gppMOUSEbuff +  sprintf( (char*)gppMOUSEbuff,pWELLCOME ) );

	while( gppKEYbuff ) {
		mSEC.y = mSEC.x;
		mSEC.x = SDL_GetTicks();
		mSEC.z = mSEC.x-mSEC.y;
		mSEC.w = 1000/max( 1, mSEC.z);


		gpcCRS& crs = *apCRS[srcDIV]; // ? *apCRS[srcDIV] : main_crs;
		reSCAN();

		if(
			  (gppKEYbuff != gpsKEYbuff)
			|| nIRQ
			|| ((mSEC.x-nJDOIT.z) > gpdJDOIT_tOUT)
		) {

			for( U1 i = 0; i < 4; i++ )
			{
				if( apT[i] )
				{
					aT[i].join();
					apT[i] = NULL;
				}

			}

			gpcPIC* pPIC = pGL ? piMASS->PIC.PIC( I8x2(gpeALF_TRG,0x10) ) : NULL;
			if( pPIC )
			{
				SDL_Texture* pBGtx = (pPICbg ? pPICbg->surDRWtx(pSDLrndr) : NULL);
				pGL->TRG( pSDLrndr, 0, winSIZ.a4x2[0], mSEC.x );
				for( U1 i = 0; i < 4; i++ )
				{
					if( i ? !(bSW&(1<<i)) : false )
						continue;

					I4x4 w = wDIVpx(i);
					I4x2 FRMwh = apCRS[i]->gtFRMwh();

					/*pGL->SET_box( w.a4x2[0], w.a4x2[1] ) //aVX4 )
					->SET_tx( 0, pGL->pTXchar, I4x2(8,32) )
					->SET_tx( 1,
									pBGtx,
									(pPICbg ? pPICbg->txWH.a4x2[0] 		: I4x2(1280,960))
							)
					->SET_tx( 2, pPIC->pTX, pPIC->txWH.a4x2[0] )
					->SET_tx( 3, pGL->pTXiso, I4x2(32,32) )
					->DRW( w.a4x2[0], FRMwh );*/

					pGL->SET_box( w.a4x2[0], w.a4x2[1] ) //aVX4 )
					->SET_tx( 0, pGL->pTXiso, I4x2(32,32) )													// tex0 -- CHAR set
					->SET_tx( 1,
									pBGtx,
									(pPICbg ? pPICbg->txWH.a4x2[0] 		: I4x2(1280,960))
							)																				// tex1 -- BG background
					->SET_tx( 2, pPIC->pTX, pPIC->txWH.a4x2[0] )											// tex2 -- MINIiso
					->DRW( w.a4x2[0], FRMwh );
				}

				pGL->SWP( pSDLwin );
			}

			/*if( pPIC )
			if( pGL->v_uvID > -1 ) {
				I4x4 w = wDIV(0);
				//pPIC->unLOCK();
				SDL_Texture* pBGtx = (pPICbg ? pPICbg->surDRWtx(pSDLrndr) : NULL);

				if( pBGtx == pGL->pTXchar )
					pBGtx = NULL;

				pGL->TRG( pSDLrndr, 0, winSIZ.a4x2[0], mSEC.x )
				->SET_box( w.a4x2[0], w.a4x2[1] ) //aVX4 )
				->SET_tx( 0, pGL->pTXchar, I4x2(8,32) )
				->SET_tx( 1,
								pBGtx,
								(pPICbg ? pPICbg->txWH.a4x2[0] 		: I4x2(1280,960))
						)
				->SET_tx( 2, pPIC->pTX, pPIC->txWH.a4x2[0] )
				->DRW( apCRS[0]->gtFRMxy(), apCRS[0]->gtFRMwh() );

				pGL->SWP( pSDLwin );
			}*/

			*gppKEYbuff = 0;
			U1 iRDY = 0x10;


			nJDOIT.z = mSEC.x;
			if( piMASS )
			{

				U1	*pS = gppMOUSEbuff,
					*pE = pS;

				if( gppKEYbuff == pS ) //nMAG )
				{
					// nincsen begépelve semmi
					// mondjuk ZOOM, stb..?
					iRDY = crs.id;
					crs.CRSins( *piMASS, gppKEYbuff, pS );
					//crs.miniRDY( *this, srcDIV, *piMASS, gppKEYbuff, pS, pPIC, pSDLrndr );
					pS = gppKEYbuff;
				} else {
					iRDY = crs.id;
					while( pE < gppKEYbuff ) {
						switch( *pE )
						{
							case '\v': {
								crs.CRSins( *piMASS, pE, pS );
								//crs.miniRDY( *this, srcDIV, *piMASS, pE, pS, pPIC, pSDLrndr );
								pS = pE+1;
								// tehát ha bent van ki kell lépni a szerkeszttett cellából
								crs.CRSbEDswitch();
							} break;
							case '\t': {
								if( crs.CRSbEDget() )
									break;

								crs.CRSins( *piMASS, pE, pS );
								//crs.miniRDY( *this, srcDIV, *piMASS, pE, pS, pPIC, pSDLrndr );
								if( *pE == '\r' )
								if( pE[1] == '\n' )
									pE++;

								pS = pE+1;
								crs.CRSstpCL( *this, *piMASS, 3, bSHIFT );
							} break;
							case '\r':
							case '\n': {
								if( crs.CRSbEDget() )
									break;


								crs.CRSins( *piMASS, pE, pS );
								//crs.miniRDY( *this, srcDIV, *piMASS, pE, pS, pPIC, pSDLrndr );
								if( *pE == '\r' )
								if( pE[1] == '\n' )
									pE++;

								pS = pE+1;
								crs.CRSstpCL( *this, *piMASS, 5, bSHIFT );
							} break;


							case 2:			// left
							case 3:			// right
							case 4:			// up
							case 5:	{ 		// down

								crs.CRSins( *piMASS, pE, pS );
								//crs.miniRDY( *this, srcDIV, *piMASS, pE, pS, pPIC, pSDLrndr );
								pS = pE+1;
								if( !crs.CRSbEDget() )
								{
									crs.CRSstpCL( *this, *piMASS, *pE, bSHIFT );

									break;
								}

								//------------------------------------
								//
								//			CRS MOVE
								//
								//------------------------------------
								crs.CRSstpED(
												*this, *piMASS,
												*pE, bSHIFT // (1&(aKT[SDL_SCANCODE_LSHIFT]|aKT[SDL_SCANCODE_RSHIFT]))
											);

							} break;
						}
						pE++;
					}

				}

				if( pS < gppKEYbuff )
					crs.CRSins( *piMASS, gppKEYbuff, pS );

				crs.miniRDYgl( *this, *piMASS, pPIC, pSDLrndr );

				gppKEYbuff = gppMOUSEbuff;
				*gppKEYbuff = 0;

				for( U1 i = 0; i < 4; i++ )
				{
					if( crs.id == i )
						continue;
					if( bSW&(1<<i) )
						apCRS[i]->miniRDYgl( *this, *piMASS, pPIC, pSDLrndr );
				}
			} else {
				crs.miniINS( gppKEYbuff, gppMOUSEbuff, gpsKEYbuff );
			}


			if( !pPIC->unLOCK() )
			for( U1 i = 0; i < 4; i++ )
			{
				if( crs.id != i )
					apCRS[i]->miniRDY2( *this, srcDIV, *piMASS, gppKEYbuff, gppKEYbuff );

				if( pSDLrndr)
					apCRS[i]->miniDRWtx( *this, srcDIV, onDIV.x, dstDIV, SRCxycr, bSHIFT );
				else
					apCRS[i]->miniDRW( *this, srcDIV, onDIV.x, dstDIV, SRCxycr, bSHIFT );
				//cout <<  (int)i << ":" << (SDL_GetTicks()-mSEC.x) << " " ;
			}
			//if( pSDLrndr)
			//	SDL_RenderPresent( pSDLrndr );

			//SDL_UpdateWindowSurface( pSDLwin );
			//cout << "s" << SDL_GetTicks() << endl;
		}


		//nMB = SDL_GetMouseState( &mouseXY.x, &mouseXY.y );

		gppMOUSEbuff = gppKEYbuff = piMASS->justDOit( *this );

		if(
			(
				nMOV =	abs( mouseXY.z-mouseXY.x)+abs( mouseXY.w-mouseXY.y)	// pntr pos
						+abs(nMBB-nMB)										// mBUTTON
						+abs( mouseW.z-mouseW.x)+abs( mouseW.w-mouseW.y)	// mWheel
						+nF
						+nIRQ
			) > 0
		) {
			gppKEYbuff += sprintf( (char*)gppKEYbuff, "move" );
			gppMOUSEbuff = gppKEYbuff;

			*gpsMAINpub = 0;
			onDIV.y = onDIV.x;
			onDIV.x = onDIVf( mouseXY.a4x2[0] );
			if( apCRS[onDIV.x] )
			{
				SRCxycr = apCRS[onDIV.x]->scnZNCR(	*this, *piMASS, mouseXY.a4x2[0] );

				char *pE = gpsMAINpub + gpfALF2STR( (U1*)gpsMAINpub, apCRS[onDIV.x]->scnZN.x+1 );
				pE += sprintf( pE, "%d", apCRS[onDIV.x]->scnZN.y );
				SRCin = apCRS[onDIV.x]->scnIN;

				if( (nMBB&1) )
				if( !(nMB&1) )
				{
					// RELEASE leftMOUSE button
					// SELECT
					if( bCTRL )
					{
						//------------------------------
						//
						// 		AN INSERT
						//
						//---------------------
						gppKEYbuff += sprintf( (char*)gppKEYbuff, "%s%s",
													bSHIFT ? "#":"",
													gpsMAINpub );
					}
					else if( bSHIFT && ( srcDIV != onDIV.x ) )
					{
						if( dstDIV != onDIV.x )
                        {
							dstDIV = onDIV.x;
                        }
						apCRS[srcDIV]->CRSsel(
												*this, *apCRS[onDIV.x], *piMASS,
												true
											);
					} else {
						if( onDIV.x != srcDIV )
						{
							U4 tmp = srcDIV;
							srcDIV = onDIV.x;
							dstDIV = tmp;
						}

                        /*if(( srcDIV != onDIV.x ) && ( dstDIV != onDIV.x ) )
                        {
							dstDIV = onDIV.x;
                        }*/
						apCRS[srcDIV]->CRSsel(
												*this, *apCRS[srcDIV], *piMASS,
												bSHIFT //, bSHIFT ? 4 : srcDIV
											);
					}


				}
			}
			*gppKEYbuff = 0;

			mouseXY.z=mouseXY.x;
			mouseXY.w=mouseXY.y;
			mouseW.z=mouseW.x;
			mouseW.w=mouseW.y;
			nMBB = nMB;

			if( nF )
				nF = 0;
			nIRQ = 0;
			//SDL_SetWindowTitle( pSDLwin, gpsTITLEpub );

		} else {
			char *pE = gpsMAINpub + gpfALF2STR( (U1*)gpsMAINpub, apCRS[onDIV.x]->scnZN.x+1 );

			pE += sprintf( pE, "%d", apCRS[onDIV.x]->scnZN.y );
			SRCin = apCRS[onDIV.x]->scnIN;

			sprintf(
						gpsTITLEpub,
						"-= piMASS%d::%s"
						" x:%d y:%d, onDIV: %d"
						" xycr:%s AN:%s"
						" IN %s %0.2f %0.2f"
						" wx:%d wy:%d"
						" %d F%d =-"
						" %d, %d",

						mSEC.w,
						gpsMASSname,
						mouseXY.x, mouseXY.y, onDIV.x,
						SRCxycr.str(gpsMAINpub+0x40), gpsMAINpub,
						SRCin.str(gpsMAINpub+0x80),
						(float)SRCin.x/SRCin.z, (float)SRCin.y/SRCin.w,
						mouseW.x, mouseW.y,
						nMB, nF,
						bug, nBUG
					);
		}


		//while( SDL_PollEvent( &ev ) )
		//if( SDL_PeepEvents( &ev, 1, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT ) )
		/*if( mSEC.x-msEVNT < gpdEV_tOUT )
			continue;*/

		SDL_SetWindowTitle( pSDLwin, gpsTITLEpub );
		nMB = SDL_GetMouseState( &mouseXY.x, &mouseXY.y );

		gpnTITLE++;
		msEVNT = mSEC.x;

		U4 tout = gpdSDL_tOUT;
		if( !SDL_WaitEventTimeout( &ev, tout ) )
			continue;
		gpnTITLE = gpnEVENT;

		do { /// EVENTS --------------------------------------------------
			tout = 0;
			gpnEVENT++;
			switch( ev.type )
			{
				case SDL_MOUSEWHEEL: {
						if( bCTRL )
						{
							if( ev.wheel.y )
							{
								//------------------------------
								//
								// 		WHEEL ZOOM
								//
								//---------------------
								I4 mag = apCRS[onDIV.x]->gtFRMwh().x - ev.wheel.y*2;

								apCRS[onDIV.x]->stFRMwh(
															*this,
															mag, 0,
															mag
														);
								nIRQ++;
								break;
							}
							break;
						}
						//------------------------------
						//
						// 		WHEEL SCROLL
						//
						//---------------------
						if( bSHIFT )
							apCRS[onDIV.x]->addFRMxy( ev.wheel.y );
						else
							apCRS[onDIV.x]->addFRMxy( 0, ev.wheel.y );

						nIRQ++;

						mouseW.x += ev.wheel.x;
						mouseW.y += ev.wheel.y;
					} break;
				case SDL_QUIT:
					gppKEYbuff = NULL;
					continue;
				case SDL_KEYDOWN:
					aKT[ev.key.keysym.scancode] = ev.key.timestamp|1;
					reSCAN();
					break;
				case SDL_KEYUP: {
						aKT[ev.key.keysym.scancode] = ev.key.timestamp;
						aKT[ev.key.keysym.scancode] &= ~1;

						scan = ev.key.keysym.scancode&0xff;
						// az SDL scan codjábol csinál egy 0x20*y öszeget
						// ami a táblázatban a kívánt karakterre fog mutatni
						scan = (scan%0x10) + (scan/0x10)*0x20;

						// SHIFT & ALT modosíthatja
						// tehát 0x800-as táblázatban tud válogatni
						scan |= reSCAN();
						/*if( bSHIFT )
							scan |= 0x200;
						if( bALT )
							scan |= 0x400;*/

						// a táblázat első 0x10 / 16 a vezérlő kód
						// ' ' sima ASCII
						// - :"' - ékezetes betűk
						// fF azok a felső funkció gombok f1f2f3f4 etc...
						// _ kurzor nyilak
						// / azok a szeparátor azaz enter tab cell etc...

						aXY[0] = c = gpsKEYMAP[scan];
						aXY[1] = gpsKEYMAP[scan+0x10];
					} break;
				case SDL_WINDOWEVENT:
					if( ev.window.event != SDL_WINDOWEVENT_RESIZED )
						break;
					SDL_GetWindowSize( pSDLwin, &winSIZ.x, &winSIZ.y );
					if( !(abs( winSIZ.z-winSIZ.x)+abs( winSIZ.w-winSIZ.y))	)
						break;

					gpeWINresize();

					winSIZ.a4x2[1] = winSIZ.a4x2[0];

			}
			if( c == 'x' )
				continue;

			if( c )
			{
				char *pUTF8 = NULL;
				switch( c )
				{
					case ' ': {
							if( bCTRL )
							{
								switch( aXY[1] )
								{
									case 'v':
									case 'V':
										// paste
										gppKEYbuff += sprintf( (char*)gppKEYbuff, "%s", SDL_GetClipboardText());
										aXY[1] = 0;
										break;
									case 'c':
									case 'C':
										if( crs.gtUTF8( gppKEYbuff ) > gppKEYbuff )
											SDL_SetClipboardText( (char*)gppKEYbuff );

										*gppKEYbuff = aXY[1] = 0;
										break;
									case 'x':
									case 'X':
										if( crs.gtUTF8( gppKEYbuff ) > gppKEYbuff )
											SDL_SetClipboardText( (char*)gppKEYbuff );

										*gppKEYbuff = 0;
										aXY[1] = 0x7e;
										break;
									case 's':
										strcpy( gppMASSfile, gpsMASSname );
										piMASS->SRCsave( (U1*)gpsMASSpath, (U1*)gppMASSfile );
										*gppKEYbuff = aXY[1] = 0;
										break;
									case 'S':
										// na mencsük ki ami van
										{
											char* pFILE = gppMASSfile+sprintf( gppMASSfile, "HTML/" );
											abALT[0] = 1&aKT[SDL_SCANCODE_LALT];
											abALT[1] = 1&aKT[SDL_SCANCODE_RALT];
											bALT = abALT[0]|abALT[1];

											//bool bALT = ( 1 & (aKT[SDL_SCANCODE_LALT]|aKT[SDL_SCANCODE_RALT]) );
											//strcpy( gppMASSfile, gpsMASSname );
											piMASS->HTMLsave( (U1*)gpsMASSpath, (U1*)pFILE, (U1*)gpsMASSname, bALT );
										}
										*gppKEYbuff = aXY[1] = 0;
										break;
								}

							}
							if( !aXY[1] )
								break;
							*gppKEYbuff = aXY[1];
							gppKEYbuff++;
						} break;
					// enter tab izé bizé
					case '/': {
							switch( aXY[1] )
							{
								case 'e':
								case 'E':
									*gppKEYbuff = '\v';
									gppKEYbuff++;
									break;
								case 't':
								case 'T':
									*gppKEYbuff = '\t';
									gppKEYbuff++;
									break;
								case 'b':
								case 'B':
									*gppKEYbuff = '\b';
									gppKEYbuff++;
									break;
								case 'd':
								case 'D':
									*gppKEYbuff = 0x7f;
									gppKEYbuff++;
									break;
								case 'n':
								case 'N':
									gppKEYbuff += sprintf( (char*)gppKEYbuff, "\r\n " );
									break;
							}
						} break;
					// cursor nyilak
					case '_': {
							switch( aXY[1] )
							{
								case 'l':
								case 'L':
									*gppKEYbuff = 2;
									gppKEYbuff++;
									break;
								case 'r':
								case 'R':
									*gppKEYbuff = 3;
									gppKEYbuff++;
									break;

								case 'u':
								case 'U':
									*gppKEYbuff = 4;
									gppKEYbuff++;
									break;
								case 'd':
								case 'D':
									*gppKEYbuff = 5;
									gppKEYbuff++;
									break;

							}
						} break;
					// ékezetek ----------------
					case '\'': {
							switch( aXY[1] ) {
								case 'A':
									pUTF8 = "\xc3\x81";
									break;
								case 'a':
									pUTF8 = "\xc3\xa1";
									break;
								case 'E':
									pUTF8 = "\xc3\x89";
									break;
								case 'e':
									pUTF8 = "\xc3\xa9";
									break;
								case 'I':
									pUTF8 = "\xc3\x8d";
									break;
								case 'i':
									pUTF8 = "\xc3\xad";
									break;
								case 'O':
									pUTF8 = "\xc3\x93";
									break;
								case 'o':
									pUTF8 = "\xc3\xb3";
									break;
								case 'U':
									pUTF8 = "\xc3\x9a";
									break;
								case 'u':
									pUTF8 = "\xc3\xba";
									break;
								default:
									pUTF8 = "?";
							}
							gppKEYbuff += sprintf( (char*)gppKEYbuff, "%s", pUTF8 );
						} break;
					case '\"': {
							switch( aXY[1] ) {
								case 'O':
									pUTF8 = "\xc5\x90";
									break;
								case 'o':
									pUTF8 = "\xc5\x91";
									break;
								case 'U':
									pUTF8 = "\xc5\xb0";
									break;
								case 'u':
									pUTF8 = "\xc5\xb1";
									break;
								default:
									pUTF8 = "?";
							}
							gppKEYbuff += sprintf( (char*)gppKEYbuff, "%s", pUTF8 );
						}break;
					case ':': {
							switch( aXY[1] ) {
								case 'O':
									pUTF8 = "\xc3\x96";
									break;
								case 'o':
									pUTF8 = "\xc3\xb6";
									break;
								case 'U':
									pUTF8 = "\xc3\x90";
									break;
								case 'u':
									pUTF8 = "\xc3\xbc";
									break;
								default:
									pUTF8 = "?";
							}
							gppKEYbuff += sprintf( (char*)gppKEYbuff, "%s", pUTF8 );
						} break;
					case '=': {
							// € EURO izé
							switch( aXY[1] )
							{
								case 'C':
								case 'c':
										pUTF8 = "\u20AC";
										break;
								default:
									pUTF8 = "?";
							}
							gppKEYbuff += sprintf( (char*)gppKEYbuff, "%s", pUTF8 );
						} break;
					case 'f':
					case 'F': {
							//------------------------------
							//
							// 		PANEL SWITCH
							//
							//---------------------
							nF = aXY[1] >= 'a' ?	((aXY[1]-'a')+10) :
													aXY[1]-'0';
							U1 div = nF-1, msk = (0x1<<(div));
							if( div < 4 )
							{
								if( !div )
								{
									//onDIV.x = 0;

									if( bSHIFT )
									{
										dstDIV = srcDIV = 0;
										bSW = 1;
										div = 4;
									} else {
										if( !srcDIV )
										{
											srcDIV = dstDIV;
											dstDIV = 0;
										} else {
											dstDIV = srcDIV;
											srcDIV = 0;
										}
									}
								}
								else if( div < 4 )
								{
									if( bSHIFT )
									{
										if( srcDIV == div )
										{
											srcDIV = dstDIV;
											dstDIV = 0;
										}
										else if( dstDIV == div )
										{
											dstDIV = 0;
										}
									}
									else if( srcDIV == div )
									{
										srcDIV = dstDIV;
										dstDIV = div;
									} else {
										if( bSW&msk)
										{
											dstDIV = srcDIV;
											srcDIV = div;
										} else
											dstDIV = div;

									}

									//U1 msk = (0x1<<(div));

									if( bSW&msk )
									{
										if( bSHIFT )
										{
											div = 4;
											bSW = (bSW&(~msk));
										}
									} else {
										bSW |= msk;
									}
								}

								bSW |= 1;
								if( div < 4 )
								{
									if( !apCRS[div] )
											apCRS[div] = new gpcCRS( *this, div );
								}
								for( U1 i = 0, sw = bSW; i < 4; i++, sw >>= 1 )
								{
									if( !(sw&1) )
										continue;

									if( !apCRS[i] )
										continue;

									apCRS[i]->stFRMwh( *this, apCRS[i]->gtFRMwh().x, 0 );
								}
								break;
							}



							switch( nF )
							{
								case 5:{	/// F5 ----- COPY
								} break;
								case 6:{	/// F6 ----- MOVE
								} break;
								case 7:{	/// F7 ----- NEW
								} break;
								case 8:{	/// F8 ----- DELETE
								} break;

								case 9:{	/// F9 ----- ?
								} break;
								case 10:{	/// F10 ----- ?
								} break;
								case 11:{	/// F11 ----- ?
								} break;
								case 12:{	/// F12 ----- ?
								} break;

							}



						} break;
					default:
						gppKEYbuff += sprintf( (char*)gppKEYbuff, "%s", aXY );
				}

				c = 0;
			}
		}
		while( SDL_WaitEventTimeout( &ev, tout ) );
		//while( SDL_PollEvent( &ev ) );


	}

}





