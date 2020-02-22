#include "gpccrs.h"
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

	trgWHpx = win.winSIZ.a4x2[1];
	pTRG = SDL_CreateTexture(
									win.pSDLrndr, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
									trgWHpx.x, trgWHpx.y
								);
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

char gpsGLSLvx[] =
"#version 120																\n"
"attribute	vec2	v_vx;													\n"
"attribute	vec2	v_uv;													\n"
"varying	vec2	fr_uv;													\n"
"void main()																\n"
"{																			\n"
"	gl_Position			= vec4( v_vx*vec2(2.0,-2.0)+vec2(-1.0,1.0), 0, 1);	\n"
"	fr_uv				= v_uv;												\n"
"}																			\n\0";
char gpsGLSLfrgREF[] =
"#version 120																\n"
"varying vec2 fr_uv;														\n"
"uniform sampler2D	tex0;					// MINI_CHAR_xXy_zXw.png		\n"
"uniform vec4 		aCNL[8];				// CNL							\n"
"void main()																\n"
"{																			\n"
"	gl_FragColor = texture2D( tex0, fr_uv )*aCNL[0];						\n"
"	if( gl_FragColor.a < (1/0x100) )										\n"
" 		discard;															\n"
"}																			\n"

"\n\0";

char gpsGLSLfrgISO[] =
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
"	}																									\n"
"	\n"
"	if( B < 0.5 )																						\n"
"	{																									\n"
"		if( gl_FragColor.a < 0.33 )																						\n"
" 			discard;\n"
" 		return;\n"
"	}																									\n"
"	gl_FragColor += Bc/(6+B+gl_FragColor.a);											\n"

"}\n";

GLint gpcGLSL::GLSLvrtx( const char* pSvrtx )
{
	if( !pSvrtx )
		pSvrtx = gpsGLSLvx;
	U8 s;
	nSUCC &= ~1;
	isSUCC = GL_FALSE;

	vrtxID = glCreateShader( GL_VERTEX_SHADER );
	glShaderSource( vrtxID, 1, &pSvrtx, 0 );
	glCompileShader( vrtxID );
	glGetShaderiv( vrtxID, GL_COMPILE_STATUS, &isSUCC );
	if( isSUCC != GL_TRUE )
	{
		//Get info string length
		glGetShaderiv( vrtxID, GL_INFO_LOG_LENGTH, &nLOG );
		if( nLOG )
		{
			vrtxLOG.lzyADD( NULL, nLOG, s = 0, 0 );
			glGetShaderInfoLog( vrtxID, nLOG, &nLOG, (char*)vrtxLOG.p_alloc );
		}
	} else {
		frgLOG.lzyRST();
		vrtxSRC.lzyFRMT( s = -1, "%s", pSvrtx );
		nSUCC |= 1;
	}

	return isSUCC;
}
GLint gpcGLSL::GLSLfrg( const char* pSfrg )
{
	if( !pSfrg )
		pSfrg = gpsGLSLfrgISO;
	U8 s;
	nT = 0;
	nSUCC &= ~2;
	isSUCC = GL_FALSE;

	frgID = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( frgID, 1, &pSfrg, 0 );
	glCompileShader( frgID );
	glGetShaderiv( frgID, GL_COMPILE_STATUS, &isSUCC );
	if( isSUCC != GL_TRUE )
	{
		//Get info string length
		glGetShaderiv( frgID, GL_INFO_LOG_LENGTH, &nLOG );
		if( nLOG )
		{
			frgLOG.lzyADD( NULL, nLOG, s = 0, 0 );
			glGetShaderInfoLog( frgID, nLOG, &nLOG, (char*)(frgLOG.p_alloc) );
		}
	} else {
		frgLOG.lzyRST();
		frgSRC.lzyFRMT( s = -1, "%s", pSfrg );
		nSUCC |= 2;
	}
	return isSUCC;
}

GLint gpcGLSL::GLSLlnk( const char** ppUlst )
{

	if( (nSUCC&0x7) == 0x7 )
		return GL_TRUE;

	if( (nSUCC&0x7) != 3 )
		return GL_FALSE;

	glUseProgram(0);
	if( PrgID > 0 )
		glDeleteProgram(PrgID);

	PrgID = glCreateProgram();
	if( PrgID < 1 )
		return GL_FALSE;

	U8 s;
	glAttachShader( PrgID, vrtxID );
	glAttachShader( PrgID, frgID );
	glLinkProgram( PrgID );
	glGetProgramiv( PrgID, GL_LINK_STATUS, &isSUCC );
	if( isSUCC != GL_TRUE )
	{
		//Get info string length
		glGetShaderiv( PrgID, GL_INFO_LOG_LENGTH, &nLOG );
		if( nLOG )
		{
			lnkLOG.lzyADD( NULL, nLOG, s = 0, 0 );
			glGetShaderInfoLog( PrgID, nLOG, &nLOG, (char*)(lnkLOG.p_alloc) );
		}
	} else {
		lnkLOG.lzyRST();
		nSUCC |= 0x4;
	}

	glDetachShader( PrgID, vrtxID );
	glDeleteShader( vrtxID );
	glDetachShader( PrgID, frgID );
	glDeleteShader( frgID );

	aUniID[0] = glGetUniformLocation( PrgID, "tgPX" 	);
	aUniID[1] = glGetUniformLocation( PrgID, "DIVxy" 	);
	aUniID[2] = glGetUniformLocation( PrgID, "FRMwh" 	);
	aUniID[3] = glGetUniformLocation( PrgID, "aTX" 		);
	aUniID[4] = glGetUniformLocation( PrgID, "aCNL"		);
	nU = 4;

	if( !ppUlst )
		return GL_TRUE;

	for( U4 i = 0, e = gpmN(aUniID); nU < e; i++ )
	{
		if( ppUlst[i] ? !*ppUlst[i] : true )
			break;

		aUniID[nU] = glGetUniformLocation( PrgID, ppUlst[i] );
		if( aUniID[nU] < 0 )
			continue;
		nU++;
	}

	return GL_TRUE;
}

gpcGL* gpcGL::GLSLset( const I8x2& an, const char* pF, const char* pV )
{
	if( pGLSL  ? pGLSL->an == an : false )
		return this;

	pGLSL = NULL;
	eGLSL = nGLSL;
    for( iGLSL = 0; iGLSL < nGLSL; iGLSL++ )
    {
		if( !ppGLSL[iGLSL] )
		{
			if( eGLSL > iGLSL )
				eGLSL = iGLSL;
			continue;
		}

        if( ppGLSL[iGLSL]->an != an )
			continue;

		pGLSL = ppGLSL[iGLSL];
		break;
    }
	if( iGLSL > eGLSL )
		iGLSL = eGLSL;

	if( !pGLSL )
	{
	    if( iGLSL >= nGLSL )
        {
			iGLSL = nGLSL;
			gpcGLSL **ppKILL = ppGLSL;
			nGLSL += iGLSL + 0x4;
			ppGLSL = new gpcGLSL*[nGLSL];
			gpmMcpyOF( ppGLSL, ppKILL, iGLSL );
			gpmDELary(ppKILL);
			gpmZn( ppGLSL+iGLSL, nGLSL-iGLSL );
        }
		ppGLSL[iGLSL] = ppGLSL[iGLSL]->pNEW( an, pF, pV );
		if( !ppGLSL[iGLSL] )
			return NULL;
		pGLSL = ppGLSL[iGLSL];
	}

	gProgID = pGLSL->PrgID;
	gpmMcpyOF( aTexID, pGLSL->aTexID, gpmN(aTexID) );
	gpmMcpyOF( aUniID, pGLSL->aUniID, gpmN(aUniID) );
	ATvxID = pGLSL->ATvxID;
	ATuvID = pGLSL->ATuvID;
	return this;
}

GLuint gpcGL::GLSLvrtx( const char* pS ) {
	if( !pS )
		pS = gpsGLSLvx;
	U8 s = -1, nS = gpmSTRLEN(pS);
	if( !nS )
		return gVxSID;

	if( VxSsrc.n_load ? VxSsrc.n_load == nS : false )
	if( gpmMcmp( VxSsrc.p_alloc, pS, VxSsrc.n_load ) == VxSsrc.n_load )
			return gVxSID;

	//VxSlog.lzyRST();
	//VxSsrc.lzyRST();

	tmpID = glCreateShader( GL_VERTEX_SHADER );
	glShaderSource( tmpID, 1, &pS, NULL );
	glCompileShader( tmpID );
	gSucc = GL_FALSE;
	glGetShaderiv( tmpID, GL_COMPILE_STATUS, &gSucc );
	if( gSucc != GL_TRUE )
	{
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		//Get info string length
		glGetShaderiv( tmpID, GL_INFO_LOG_LENGTH, &maxLength );

		//Allocate string
		if( maxLength )
		{
			VxSlog.lzyADD( NULL, maxLength, s, -1 );
			//char* infoLog = new char[ maxLength ];

			//Get info log
			glGetShaderInfoLog( tmpID, maxLength, &infoLogLength, (char*)(VxSlog.p_alloc+s) );
			if( infoLogLength > 0 )
			{
				VxSlog.n_load = s+infoLogLength;
			} else
				VxSlog.n_load = s;
		}

		//Deallocate string
		//delete[] infoLog;
		return gVxSID;
	}

	//VxSsrc.lzyRST();
	VxSsrc.lzyINS( (U1*)pS, nS, s = 0, -1 );
	if( gVxSID )
	{
		glDetachShader( gProgID, gVxSID );
		glDeleteShader( gVxSID );
	}
	gVxSucc = gSucc;
	gVxSID = tmpID;
	return gVxSID;

}
GLuint  gpcGL::GLSLfrg( const char* pS ) {
	if(!pS)
		pS = gpsGLSLfrgISO;
	U8 s = -1, nS = gpmSTRLEN(pS);
	if( !nS )
		return gFrSID;

	if( frSsrc.n_load ? frSsrc.n_load == nS : false )
	if( gpmMcmp( frSsrc.p_alloc, pS, frSsrc.n_load ) == frSsrc.n_load )
			return gFrSID;

	//FrSlog.lzyRST();
	//frSsrc.lzyRST();

	tmpID = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( tmpID, 1, &pS, NULL );
	glCompileShader( tmpID );
	gSucc = GL_FALSE;
	glGetShaderiv( tmpID, GL_COMPILE_STATUS, &gSucc );
	if( gSucc != GL_TRUE )
	{
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		//Get info string length
		glGetShaderiv( tmpID, GL_INFO_LOG_LENGTH, &maxLength );

		//Allocate string
		if( maxLength )
		{
			FrSlog.lzyADD( NULL, maxLength, s, -1 );
			//char* infoLog = new char[ maxLength ];

			//Get info log
			glGetShaderInfoLog( tmpID, maxLength, &infoLogLength, (char*)(FrSlog.p_alloc+s) );
			if( infoLogLength > 0 )
			{
				FrSlog.n_load = s+infoLogLength;
			} else
				FrSlog.n_load = s;
		}

		//Deallocate string
		//delete[] infoLog;
		return gFrSID;
	}

	//frSsrc.lzyRST();
	frSsrc.lzyINS( (U1*)pS, nS, s = 0, -1 );
	if( gFrSID )
	{
		glDetachShader( gProgID, gFrSID );
		glDeleteShader( gFrSID );
	}
	gFrSucc = gSucc;
	gFrSID = tmpID;
	return gFrSID;

}
