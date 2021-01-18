#include "gpccrs.h"
#include "gpsGLSL.h"

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
		if(bSTDcout){std::cout <<std::endl << "gpcGL init error" <<std::endl;}
		return;
	}
	glewExperimental = GL_TRUE;
	glewErr = glewInit();
	if( glewErr != GLEW_OK )
	{
		if(bSTDcout){std::cout <<std::endl << "gpcGL GLEW_NOK error" <<std::endl;}
		return;
	}

	glGetIntegerv( GL_CURRENT_PROGRAM, &oPrgID );

	trgWHpx = win.winSIZ.a4x2[1];
	pTRG = SDL_CreateTexture(
								win.pSDLrndr, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
								trgWHpx.x, trgWHpx.y
							);
	gVxSucc = GL_FALSE;

	pTXchar = SDL_CreateTextureFromSurface( win.pSDLrndr, win.pSRFchar );
	if( pTXchar )
		if(bSTDcout){std::cout << "char" << (void*)win.pSRFchar <<std::endl;}
	else
		if(bSTDcout){std::cout << SDL_GetError() <<std::endl;}

	pTXiso = SDL_CreateTextureFromSurface( win.pSDLrndr, win.pSRFiso );
	if( pTXiso )
		if(bSTDcout){std::cout << "char" << (void*)win.pSRFiso <<std::endl;}
	else
		if(bSTDcout){std::cout << SDL_GetError() <<std::endl;}

	/*glGenSamplers( 1, aSMPid );
	glSamplerParameteri(aSMPid[0], GL_TEXTURE_WRAP_S, GL_REPEAT);
	glSamplerParameteri(aSMPid[0], GL_TEXTURE_WRAP_T, GL_REPEAT);
	glSamplerParameteri(aSMPid[0], GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glSamplerParameteri(aSMPid[0], GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
	glSamplerParameterf(aSMPid[0], GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);*/

}



gpcGL* gpcGL::glSETtrg( gpcPIC* pT, I4x2 wh, bool bCLR, bool bDEP ) {
	if( this ? !pRNDR : true )
		return NULL;

	if(pT)
	{
		if( pT->txWH.a4x2[1] != wh )
			gpmSDL_FreeTX( pT->pRTX );

		if(!pT->pRTX)
		{
			pT->pRTX = SDL_CreateTexture( pRNDR, SDL_PIXELFORMAT_BGRA8888, SDL_TEXTUREACCESS_TARGET, wh.x, wh.y );
			if(!pT->pRTX)
				return NULL;
			pT->txWH.a4x2[1] = wh;
		}

		if(pRTX!=pT->pRTX)
		{
			if(pPICrtx)
			{
				if(!pPICrtx->pSRF)
				{
					int w=0, h=0, acc=0;
					U4 frm;
					SDL_QueryTexture( pRTX, &frm, &acc, &w, &h );
					pPICrtx->pSRF = SDL_CreateRGBSurface( 0, w, h, 32, 0,0,0,0 );
				}
				if( pPICrtx->pSRF )
					SDL_RenderReadPixels(pRNDR, NULL, 0, pPICrtx->pSRF->pixels, pPICrtx->pSRF->pitch );
				pPICrtx->pREF = NULL;
			}
			SDL_SetRenderTarget(pRNDR,NULL);
		}
		pPICrtx=pT;
		SDL_SetRenderTarget(pRNDR,pRTX=pPICrtx->pRTX);
	} else {
		pPICrtx=NULL;
		SDL_SetRenderTarget(pRNDR,pRTX=NULL);
	}

	GLbitfield b = GL_STENCIL_BUFFER_BIT;
	if( bCLR )
		b |= GL_COLOR_BUFFER_BIT;
	if( bDEP )
		b |= GL_DEPTH_BUFFER_BIT;

	glClearColor( 0.0f, 0.0f, 0.25, 1.0f );
	glClearDepth((GLclampd)1.0);
	glClear( b );
	glViewport( 0, 0, wh.x, wh.y );
	return this;
}

GLint gpcGLSL::GLSLvtx( const char* pSvrtx ) {
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
			vtxLOG.lzyADD( NULL, nLOG, s = 0, 0 );
			glGetShaderInfoLog( vrtxID, nLOG, &nLOG, (char*)vtxLOG.p_alloc );
		}
	} else {
		pVTX = (char*)pSvrtx;
		vtxLOG.lzyRST();
		vtxSRC.lzyFRMT( s = -1, "%s", pSvrtx );
		nSUCC |= 1;
	}

	return isSUCC;
}
GLint gpcGLSL::GLSLfrg( const char* pSfrg ) {
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
		return isSUCC;
	}

	pFRG = (char*)pSfrg;
	frgLOG.lzyRST();
	frgSRC.lzyFRMT( s = -1, "%s", pSfrg );
	nSUCC |= 2;
	return isSUCC;
}

GLint gpcGLSL::GLSLlnk( const char** ppUlst ) {

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
	nU = 5;

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
gpcGLSL* gpcGLSL::pNEW( const I8x2& an, const char* pF, const char* pV, const char* pATvx, const char* pATuv, const char* pATtx )
{
	if( !this )
	{
		gpcGLSL* pTHIS = new gpcGLSL( an, pF, pV );
		if( !pTHIS )
			return NULL;

		if( (pTHIS->nSUCC&0x7) != 0x7 )
		{
			gpmDEL(pTHIS);
			return NULL;
		}

		return pTHIS->pNEW( an, pF, pV, pATvx, pATuv, pATtx );
	}
	fndTX( pATtx );
	// ATTRIBUTES ----------------------
	ATvxID = glGetAttribLocation( PrgID, pATvx );
	if( ATvxID < 0 )
		return this;
	nSUCC |= 8;

	ATuvID = glGetAttribLocation( PrgID, pATuv );
	if( ATuvID < 0 )
		return this;

	nSUCC |= 0x10;
	return this;
}
gpcGL* gpcGL::GLSLset( const gpcALU& alu, const char* pF, const char* pV ) {
	if( !this )
		return this;

	I8x2 an(0,14);
	if(alu.bSTR())
	{
		an = (U1*)alu.pDAT;
		an.num = gpfSTR2U8( (U1*)alu.pDAT+an.num );
	} else {
		an.x = alu.u8();
		an.num = 0;
	}

	return GLSLset( an, pF, pV  );
}
gpcGL* gpcGL::GLSLset( const I8x2& an, const char* pF, const char* pV ) {
	if( !this )
		return NULL;

	if( pGLSL ? pGLSL->ID != an : true )
		pGLSL = NULL;

	if( pGLSL )
	if( pF || pV )
		pGLSL = NULL;

	if( !pGLSL )
	{
		eGLSL = nGLSL;
		for( iGLSL = 0; iGLSL < nGLSL; iGLSL++ )
		{
			if( !ppGLSL[iGLSL] )
			{
				if( eGLSL > iGLSL )
					eGLSL = iGLSL;
				continue;
			}

			if( ppGLSL[iGLSL]->ID != an )
				continue;

			pGLSL = ppGLSL[iGLSL];
			break;
		}
		if( iGLSL > eGLSL )
			iGLSL = eGLSL;
	}

	gpcGLSL* pKILL = NULL;
	if( pGLSL )
	{
		U4 nCMP = 0;
		//if( pGLSL->pVTX != pV )
		{
			nCMP = gpmSTRLEN(pV);
			if( nCMP )
			if( nCMP != pGLSL->vtxSRC.n_load )
				pKILL = pGLSL;
			else if( gpmMcmp( pGLSL->vtxSRC.p_alloc, pV, nCMP ) != nCMP )
				pKILL = pGLSL;
		}

		if( !pKILL )
		//if( pGLSL->pFRG != pF )
		{
			nCMP = gpmSTRLEN(pF);
			if( nCMP )
			if( nCMP != pGLSL->frgSRC.n_load )
				pKILL = pGLSL;
			else if( gpmMcmp( pGLSL->frgSRC.p_alloc, pF, nCMP ) != nCMP )
				pKILL = pGLSL;
		}

		if( pKILL )
		{
			pGLSL = ((gpcGLSL*)NULL)->pNEW( an, pF, pV );
			if( pGLSL )
			{
				ppGLSL[iGLSL] = pGLSL;
				gpmDEL(pKILL);
			}
		}

	} else {
	    if( iGLSL >= nGLSL )
        {
			iGLSL = nGLSL;
			gpcGLSL **ppKILL = ppGLSL;
			nGLSL += iGLSL + 0x4;
			ppGLSL = new gpcGLSL*[nGLSL];
			gpmMcpyOF( ppGLSL, ppKILL, iGLSL );
			gpmDELary(ppKILL);
			gpmZnOF( ppGLSL+iGLSL, nGLSL-iGLSL );
        }
		ppGLSL[iGLSL] = ppGLSL[iGLSL]->pNEW( an, pF, pV );
		if( !ppGLSL[iGLSL] )
			return NULL;
		pGLSL = ppGLSL[iGLSL];
	}
	if( !pGLSL )
		return NULL;

	gProgID = pGLSL->PrgID;
	gpmMcpyOF( aTexID, pGLSL->aTexID, gpmN(aTexID) );
	gpmMcpyOF( aUniID, pGLSL->aUniID, gpmN(aUniID) );
	ATvxID = pGLSL->ATvxID;
	ATuvID = pGLSL->ATuvID;
	return this;
}

GLuint gpcGL::GLSLvtx( const char* pS ) {
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
