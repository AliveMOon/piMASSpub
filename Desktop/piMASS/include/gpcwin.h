#ifndef GPCWIN_H
#define GPCWIN_H

//#include "piMASS.h"
//#include "gpcSRC.h"
#include "gpcres.h"

class gpcCRS;
class gpdADR;
class gpcTHRD_DRW;
//class gpcRES;

class InitError : public std::exception
{
    std::string msg;
public:
    InitError();
    InitError( const std::string & );
    virtual ~InitError() throw();
    virtual const char * what() const throw();
};

#define gpdNdiv gpmN(apCRS)
class gpcGLSL
{
public:
	GLint	isSUCC, nLOG,
			PrgID, nT, nU,

			ATvxID, ATuvID,

			aTexID[GL_ACTIVE_TEXTURE-GL_TEXTURE0],
			aUniID[0x10];

	GLuint	vrtxID,
			frgID;
	gpcLZY	vrtxLOG, vrtxSRC,
			frgLOG, frgSRC,
			lnkLOG;
	U4 nSUCC;
	I8x2	ID;
private:
	GLint GLSLvrtx( const char* pSvrtx );
	GLint GLSLfrg( const char* pSfrg );
	GLint GLSLlnk( const char** ppUlst = NULL );

	gpcGLSL( const I8x2& an, const char* pSfrg, const char* pSvrtx )
	{
		gpmCLR;
		ID = an;
		if( GLSLvrtx( pSvrtx ) != GL_TRUE )
			return;

		if( GLSLfrg( pSfrg ) != GL_TRUE )
			return;

		GLSLlnk();
	}
public:
	GLint fndTX( const char* p_txt )
	{
		if( nT )
			return nT;

		if( PrgID < 1 )
			return nT;

		char sTX[0x100];
		nT = 0;
		for( U4 i = 0, e = gpmN(aTexID); i < e; i++ )
		{
			sprintf( sTX, "%s%d", p_txt, i );
			aTexID[i] = glGetUniformLocation( PrgID, sTX );
			if( aTexID[i] > -1 )
				nT = i+1;
		}

		return nT;
	}
	gpcGLSL* pNEW( const I8x2& an, const char* pF, const char* pV, const char* pATvx = "v_vx", const char* pATuv = "v_uv", const char* pATtx = "tex"  )
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

			return pTHIS->pNEW( an, pF, pV );
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
};
class gpcGL
{
public:
	GLint	oPrgID,
			ATvxID, ATuvID,
			gVxSucc,
			gFrSucc, aTexID[0x8],
			gPrgSucc, aUniID[0x8],
			gSucc;
	GLuint	tmpID,
			gProgID,
			gVxSID,
			gFrSID,
			VaID,
			VXbID,
			IXbID;

	SDL_GLContext	gCntxt;
	GLenum			glewErr;


	gpcLZY	VxSlog,
			VxSsrc,
			FrSlog,
			frSsrc,
			Lnklog;

	SDL_Renderer* pRNDR;
	I4x2	luXY,	trgWHpx,
			boxXY, 	boxWH,
			aTXwh[0x10];

	gpcPIC* pPICrtx;
	SDL_Texture	*pRTX, *pTRG,
				*pTXchar,
				*pTXiso;

	gpcGLSL		**ppGLSL, *pGLSL;
	U4			iGLSL, eGLSL, nGLSL;

	gpcGL* GLSLset( const gpcALU& alu, const char* pF = NULL, const char* pV = NULL );
	gpcGL* GLSLset( const I8x2& an, const char* pF = NULL, const char* pV = NULL  );
	~gpcGL()
	{
		SDL_GL_DeleteContext( gCntxt );
	}

	gpcGL( gpcWIN& win );
	gpcGL* glSETtrg( gpcPIC* pT, I4x2 wh, bool bC = true, bool bD = true );
	gpcGL* TRG( SDL_Renderer* pSDLrndr, I4x2 lXY, const I4x2& tWH, float ms, bool bCLR = true, bool bDEP = true )
	{
		if( !this )
			return NULL;

		if( pRNDR != pSDLrndr )
			pRNDR = pSDLrndr;

		if( !pRNDR )
			return NULL;

		glGetIntegerv( GL_CURRENT_PROGRAM, &oPrgID );
		if( pRTX )
		{
			SDL_SetRenderTarget( pRNDR, NULL );
			SDL_RenderCopy( pRNDR, pRTX, NULL, NULL ); //&t2Rect );

			int w = 0, h = 0, acc = 0;
			U4 frm;
			SDL_QueryTexture( pRTX, &frm, &acc, &w, &h );
			if( pPICrtx )
			{
				if( !pPICrtx->pSRF )
					pPICrtx->pSRF = SDL_CreateRGBSurface( 0, w, h, 32, 0,0,0,0 ); // rmask, gmask,bmask, amask );
				if( pPICrtx->pSRF )
					SDL_RenderReadPixels( pRNDR, NULL, 0, pPICrtx->pSRF->pixels, pPICrtx->pSRF->pitch );
				pPICrtx->pREF = NULL;
			}
			pPICrtx = NULL;
			pRTX = NULL;
		}

		if( pTRG )
		if( trgWHpx != tWH )
		{
			gpmSDL_FreeTX( pTRG );
			pTRG = SDL_CreateTexture( pRNDR, SDL_PIXELFORMAT_BGRA8888, SDL_TEXTUREACCESS_TARGET, tWH.x, tWH.y );
			if( !pTRG )
				return NULL;
			//pTXchar = SDL_CreateTextureFromSurface( pRNDR, pSRFchar );
		}

		luXY = lXY;
		trgWHpx = tWH;

		GLbitfield b = 0;
		if( bCLR )
			b |= GL_COLOR_BUFFER_BIT;
		if( bDEP )
			b |= GL_DEPTH_BUFFER_BIT;

		ms = sin( ms/1000.0 )+1.0;
		glClearColor( ms*0.13, 0.0f, ms*0.3, 0.0f );
		glClearDepth(1.0);
		glClear( b );

		//glEnable(GL_TEXTURE_2D);
		//SDL_SetRenderTarget( pRNDR, pTRG );
		//char* pERR = (char*)SDL_GetError();

		return this;
	}
	GLuint IBOnew( const GLuint* pD, U4 nD ) {
		//Create IBO
		glGenBuffers( 1, &IXbID );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IXbID );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, nD * sizeof(*pD), pD, GL_STATIC_DRAW );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
		return IXbID;
	}
	GLuint VBOnew( const GLfloat* pD, U4 nD, U4 nX ) {
		//Create VBO
		glGenBuffers( 1, &VXbID );
		glBindBuffer( GL_ARRAY_BUFFER, VXbID );
		glBufferData( GL_ARRAY_BUFFER, nX * nD * sizeof(*pD), pD, GL_STATIC_DRAW );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		return VXbID;
	}



	gpcGL* glSETbox( I4x2 xy, const I4x2& wh )
	{
		if( this ? !wh.area() : true )
			return NULL;

		Fx4 aV4[4];
		glUseProgram( gProgID );
		if( boxXY == xy && boxWH == wh )
			return this;

		if( boxWH.area() )
			glDeleteBuffers(1,&VXbID);

		xy -= luXY;

		aV4[0].aF2[1] = 0;
		aV4[1].aF2[1] = Fx2( 1, 0 );
		aV4[2].aF2[1] = Fx2( 1, 1 );
		aV4[3].aF2[1] = Fx2( 0, 1 );

		aV4[0].aF2[0] = xy;
		aV4[1].aF2[0] = xy + I4x2( wh.x, 0 );
		aV4[2].aF2[0] = xy + wh;
		aV4[3].aF2[0] = xy + I4x2( 0, wh.y );

		if( pPICrtx )
			aV4[0].div( Fx4( pPICrtx->txWH.a4x2[1], I4x2(1,1) ), gpmN(aV4) );
		else
			aV4[0].div( Fx4( trgWHpx, I4x2(1,1) ), gpmN(aV4) );

		if( VXbID > 0 )
			glDeleteBuffers(1,  &VXbID );

		VBOnew( (float*)aV4, 4, 4 );
		boxXY = xy;
		boxWH = wh;

		return this;
	}
	gpcGL* glSETbox( const I4x4& w )
	{
		return glSETbox( w.a4x2[0], w.a4x2[1] );
	}
	gpcGL* glSETbox( I4x4 xyWH, const I4x4 divPX, const I4x2& frm )
	{
		xyWH &= divPX.a4x2[1];
		xyWH /= frm;
		xyWH.a4x2[0] += divPX.a4x2[0];
		return glSETbox( xyWH.a4x2[0], xyWH.a4x2[1] );
	}
	gpcGL* glSETcnl( U4 i, Fx4 xyzw )
	{
		if( !this )
			return NULL;

		if( aUniID[4] > -1 )
			glUniform4fv( aUniID[4]+i, 1, (GLfloat*)&xyzw );

		return this;
	}
	gpcGL* glSETcnl( U4 i, Fx4* pXYZW, U4 n )
	{
		if( !this )
			return NULL;

		if( aUniID[4] > -1 )
			glUniform4fv( aUniID[4]+i, n, (GLfloat*)pXYZW );

		return this;
	}
	gpcGL* glSETtx( U4 i, SDL_Texture* pTX, I4x2 wh )
	{
		if( !this )
			return NULL;

		if( aTexID[i] < 0 )
			return this;
		if( !pTX )
			return this;

		glUniform1i( aTexID[i], i );
		glActiveTexture( GL_TEXTURE0+i );
		glEnable(GL_TEXTURE_2D);
		SDL_GL_BindTexture( pTX, NULL, NULL );

		if( aUniID[3] > -1 )
			glUniform2f( aUniID[3]+i, (float)wh.x, (float)wh.y );

		aTXwh[i] = wh;
		return this;
	}
	gpcGL* glSETtx( U4 msk, gpcPIC** ppPIC )
	{
		if( !this )
			return NULL;
		SDL_Surface* pSURF;
		for( I4 i = 0; msk; i++, msk>>=1 )
		{
			if( (msk&1) ? !ppPIC[i] : true )
				continue;

			glSETtx( i, ppPIC[i]->surDRWtx(pRNDR), ppPIC[i]->txWH.a4x2[1] );
		}
		return this;
	}
	gpcGL* glDONE(){ glUseProgram(0); return this; }
	gpcGL* glDRW( I4x2 xy, I4x2 wh ) {
		if( !this )
			return NULL;

		if( aUniID[0] > -1 )
			glUniform2f( aUniID[0], (float)trgWHpx.x, (float)trgWHpx.y );
		if( aUniID[1] > -1 )
			glUniform2f( aUniID[1], (float)xy.x, (float)xy.y );
		if( aUniID[2] > -1 )
			glUniform2f( aUniID[2], (float)wh.x, (float)wh.y );

		glBindBuffer( GL_ARRAY_BUFFER, VXbID );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IXbID );

		glVertexAttribPointer( ATvxID, 2, GL_FLOAT, GL_FALSE, sizeof(Fx4), 0 );
		glEnableVertexAttribArray( ATvxID );
		glVertexAttribPointer( ATuvID, 2, GL_FLOAT, GL_FALSE, sizeof(Fx4), gpmGLBOFF(sizeof(Fx2)) );
		glEnableVertexAttribArray( ATuvID );

		glDrawElements( GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL );

		glDisableVertexAttribArray( ATvxID );
		glDisableVertexAttribArray( ATuvID );

		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
		glBindVertexArray( 0 );

		return this;
	}
	gpcGL* SWP( SDL_Window* pWIN )
	{
		if( !this )
			return NULL;

		SDL_GL_SwapWindow( pWIN );

		if( oPrgID < 0 )
			return this;

		glUseProgram(oPrgID);
		SDL_RenderClear( pRNDR );

		return this;
	}

	GLuint GLSLvrtx( const char* pS = NULL );
	GLuint GLSLfrg( const char* pS = NULL );
	GLuint GLSLlnk( ) {
		U8 s;
		gProgID = glCreateProgram();
		glAttachShader( gProgID, gVxSID );
		glAttachShader( gProgID, gFrSID );
		glLinkProgram( gProgID );


		gSucc = GL_FALSE;
		glGetProgramiv( gProgID, GL_LINK_STATUS, &gSucc );
		if( gSucc != GL_TRUE )
		{
			int infoLogLength = 0;
			int maxLength = infoLogLength;

			//Get info string length
			glGetProgramiv( gProgID, GL_INFO_LOG_LENGTH, &maxLength );

			//Allocate string
			if( maxLength )
			{
				Lnklog.lzyADD( NULL, maxLength, s = -1, -1 );
				//char* infoLog = new char[ maxLength ];

				//Get info log
				glGetProgramInfoLog( gProgID, maxLength, &infoLogLength, (char*)(Lnklog.p_alloc+s) );
				if( infoLogLength > 0 )
				{
					Lnklog.n_load = s+infoLogLength;
				} else
					Lnklog.n_load = s;

			}
			return -1;
		}

		if( gVxSID )
		{
			glDetachShader( gProgID, gVxSID );
			glDeleteShader( gVxSID );
		}
		if( gFrSID )
		{
			glDetachShader( gProgID, gFrSID );
			glDeleteShader( gFrSID );
		}

		ATvxID = glGetAttribLocation( gProgID, "v_vx" );
		if( ATvxID < 0 )
		{
			return gProgID;
		}
		ATuvID = glGetAttribLocation( gProgID, "v_uv" );


		aTexID[0] = glGetUniformLocation( gProgID, "tex0" );
		aTexID[1] = glGetUniformLocation( gProgID, "tex1" );
		aTexID[2] = glGetUniformLocation( gProgID, "tex2" );
		aTexID[3] = glGetUniformLocation( gProgID, "tex3" );
		gPrgSucc = GL_TRUE;
		return gProgID;

	}







};
class gpcWIN
{
	public:
		gpcMASS*		piMASS;
		gpcCRS			*apCRS[4];

		I4x4			winID,
						winSIZ,
						mouseXY, mouseW,
						SRCxycr, SRCin, chrTX;


		gpcGL		*pGL;

		SDL_Window		*pSDLwin;
		SDL_Renderer	*pSDLrndr;

		SDL_Texture		*pTXrndr,
						*pTXsnd,
						*pTXize;

		SDL_Surface		*pSRFload,
						*pSRFchar,

						*pSRFiso,

						*pSRFwin,
						*pSRFsnd;
		gpcPIC			*pPICbg;

		SDL_Event 		ev;
		SDL_Rect		chrPIC;

		U1		sHOST[0x100],
				sUSER[0x100],
				gpsKEYbuff[0x10000],
				*pHOST, *pUSER,
				*gppKEYbuff,
				*gppMOUSEbuff;

		char 	gpsMAINpub[0x100],
				gpsTITLEpub[0x100],
				gpsMASSname[0x100],
				gpsMASSpath[gpdMAX_PATH], *gppMASSfile;


		I8	gpnEVENT = 0,
			gpnTITLE = 0;

		U4	aKT[0x200],
			srcDIV, dstDIV, msEVNT;
		U4x2 	onDIV; // = srcDIV;
		U4x4	mSEC = 0, // x: new // y: prev // z: elapse // w: fps
				nJDOIT;
		I4 nMOV, nIRQ,  nMB, nMBB, nF;

		bool	bSHIFT, bCTRL,
				bALT, abALT[2];
		U1		bSW;

		gpcLZY	winPUB;
		U8		iPUB;

		U1	sGTpub[0x10000],
			sGTbuff[gpdRECVn];
		/// SYNC MAP --------------
		gpcLZY *pSYNwin, *pSYNgt;
		U4		msSYN, msSYNrst;


		/// RUN MAP --------------
		U4	*pM, *pC, *pR, mZ, mN, mZN;

		std::thread		aT[gpdNdiv]; //4];
		gpcTHRD_DRW		*apT[gpdNdiv], *apTall[gpdNdiv];
protected:
private:
		I4x4 winDIVpx, winDIVcr;


public:
		~gpcWIN();
		gpcWIN( char* pPATH, char* pFILE, char* sNAME, gpcMASS* piMASS ); //, char* pPATH, char* pFILE ); //, I4x4& siz );

		gpcLZY* putLIST( gpcLZY* pLZY, gpeNET4 nt4, U4 i, U4 j, char* pPRMPT = NULL ) {
			if( !this )
				return pLZY;
			U4x2 zn;
			U4* pM = piMASS->pM( zn );
			if( !pM )
				return pLZY;

			if( i > j )
			{
				U4 t = i;
				i = j;
				j = t;
			}

            U8 s = zn.area(), b = -1;
            if( !j )
				j = s;
            else if( j > s )
				j = s;

            gpcSYNC syn( nt4, 0, mSEC.x, INVALID_SOCKET, 0 );
			pLZY = pLZY->lzyADD( &syn, sizeof(syn), b );

            U4 xFND, n = 0, z = zn.x;
            U1 sBUFF[0x20], *pNUM;
            for( ; i < j; i++ )
            {
				if( !( xFND = pM[i] ) )
					continue;
				gpcSRC* pSRC = piMASS->SRCfnd( xFND );
				if( !pSRC )
					continue;
				pNUM = sBUFF+gpfALF2STR( sBUFF, (i%z)+1 );
				sprintf( (char*)pNUM, "%d\t", i/z );
                pLZY = pLZY->lzyFRMT( s=-1, "%s%s: "
											gpdPUB
											"\r\n%s\r\n", n ? "" : "\r\n", sBUFF, (char*)pSRC->pPUB() );
                n++;

            }
			if( pPRMPT )
				pLZY = pLZY->lzyFRMT( s=-1, "\r\n%s", pPRMPT );

            return pLZY->SYNrdy(b);
		}
		gpcLZY* putLIST( gpcLZY* pLZY, gpeNET4 nt4, I8x4 an, char* pPRMPT = NULL ) {
			if( !this )
				return pLZY;
			U4x2 zn;
			U4* pM = piMASS->pM( zn );
			if( !pM )
				return pLZY;
			I8x4 lurdZN = an;
			if( lurdZN.x && !lurdZN.z )
				lurdZN.a8x2[1] = lurdZN.a8x2[0];
			else
				lurdZN = an.lurd();

			if( lurdZN.x > 0 )
				lurdZN.x--;
			else
				lurdZN = I8x4( 0, 0, zn.x, zn.y );

			U8 i = 0, j = zn.area(), b = -1, s;

            gpcSYNC syn( nt4, 0, mSEC.x, INVALID_SOCKET, 0 );
			pLZY = pLZY->lzyADD( &syn, sizeof(syn), b );

            U4 xFND, n = 0, z = zn.x;
            U1 sBUFF[0x20], *pNUM;
            for( ; i < j; i++ )
            {
				an.x = i%z;
				an.y = i/z;
				if( an.x < lurdZN.x || an.y < lurdZN.y )
					continue;
				if( an.x >= lurdZN.z || an.y > lurdZN.w )
					continue;

				if( !( xFND = pM[i] ) )
					continue;

				gpcSRC* pSRC = piMASS->SRCfnd( xFND );
				if( !pSRC )
					continue;
				pNUM = sBUFF+gpfALF2STR( sBUFF, an.x+1 );
				sprintf( (char*)pNUM, "%d\t", an.y );
                pLZY = pLZY->lzyFRMT( s=-1, "%s%s: "
											gpdPUB
											"\r\n%s\r\n", n ? "" : "\r\n", sBUFF, (char*)pSRC->pPUB() );
                n++;

            }
			if( pPRMPT )
				pLZY = pLZY->lzyFRMT( s=-1, "\r\n%s", pPRMPT );

            return pLZY->SYNrdy(b);
		}
		gpcLZY* putMINI( gpcLZY* pLZY, gpeNET4 nt4, I8x4 an, char* pPRMPT = NULL ) {
			if( !this )
				return pLZY;
			U4x2 zn;
			U4* pM = piMASS->pM( zn );
			if( !pM )
				return pLZY;
			I8x4 lurdZN = an;
			if( lurdZN.x && !lurdZN.z )
				lurdZN.a8x2[1] = lurdZN.a8x2[0];
			else
				lurdZN = an.lurd();

			if( lurdZN.x > 0 )
				lurdZN.x--;
			else
				lurdZN = I8x4( 0, 0, zn.x, zn.y );

			U8 i = 0, j = zn.area(), b = -1, s;

            gpcSYNC syn( nt4, 0, mSEC.x, INVALID_SOCKET, 0 );
			pLZY = pLZY->lzyADD( &syn, sizeof(syn), b );

            U4 xFND, n = 0, z = zn.x;
            U1 sBUFF[0x20], *pNUM;
            for( ; i < j; i++ )
            {
				an.x = i%z;
				an.y = i/z;
				if( an.x < lurdZN.x || an.y < lurdZN.y )
					continue;
				if( an.x >= lurdZN.z || an.y > lurdZN.w )
					continue;

				if( !( xFND = pM[i] ) )
					continue;

				gpcSRC* pSRC = piMASS->SRCfnd( xFND );
				if( !pSRC )
					continue;
				pNUM = sBUFF+gpfALF2STR( sBUFF, an.x+1 );
				sprintf( (char*)pNUM, "%d\t", an.y );
                pLZY = pLZY->lzyFRMT( s=-1, "%s%s: "
											gpdPUB
											"\r\n%s\r\n", n ? "" : "\r\n", sBUFF, (char*)pSRC->pSRCstart( false ) );
                n++;

            }
			if( pPRMPT )
				pLZY = pLZY->lzyFRMT( s=-1, "\r\n%s", pPRMPT );

            return pLZY->SYNrdy(b);
		}
		bool bINI_hst_usr() {
			if( !this )
				return false;
			if( pHOST <= sHOST )
			{
				//pCOMP +=
				pHOST += gethostname( (char*)sHOST, sizeof(sHOST) );
			}
			if( pUSER <= sUSER )
			{
				char* pU = getlogin();
				if( !pU )
				{
					struct passwd *pw = getpwuid(getuid());
					pU = strrchr( pw->pw_dir, '/' );
					if( pU ? *pU == '/' : false )
						pU++;
				}

				//__uid_t id = getuid();
				//pCOMP +=
				pUSER += sprintf( (char*)sUSER, "%s", pU ); //id ? getlogin() : "root" );
				//pUSER += getlogin_r( (char*)sUSER, sizeof(sUSER) );
			}
			return true;
		}
		I4x2 wFRM( U1 iDIV ) {
			I4x2 whCR = winSIZ.a4x2[1]/chrTX.a4x2[1]; // I4x2( (int*)&chrPIC.w );
			return whCR;
		}
		I4x4 wDIVcr( U1 iDIV )
		{
			//SDL_Rect
			I4x4 div;

			switch( iDIV%4)
			{
				case 0:
					div.z = (bSW&0x2) ? winDIVcr.x : winDIVcr.z;
					div.w = (bSW&0xc) ? winDIVcr.y : winDIVcr.w;

					div.x =
					div.y = 0;
					break;
				case 1:
					div.z = (bSW&0x2) ? winDIVcr.z-winDIVcr.x : 0;
					div.w = (bSW&0x8) ? winDIVcr.y : winDIVcr.w;

					div.x = winDIVcr.z - div.z;
					div.y = 0;
					break;
				case 2:
					if( bSW&0x4 )
					{
						div.z = (bSW&0xa) ? winDIVcr.x : winDIVcr.z;
						div.w = winDIVcr.w-winDIVcr.y;
					} else {
						div.z = div.w = 0;
					}

					div.x = 0;
					div.y = winDIVcr.w-div.w;
					break;
				case 3:
					if( bSW&0x8 )
					{
						div.z = (bSW&0x4) ? winDIVcr.x : winDIVcr.z;
						div.w = winDIVcr.y;
					} else {
						div.z = div.w = 0;
					}

					div.x = winDIVcr.z-div.z;
					div.y = winDIVcr.w-div.w;
					break;
			}
			return div;
		}


		I4x4 wDIVpx( U1 iDIV )
		{
			//SDL_Rect
			I4x4 div = wDIVcr(iDIV);
			div &= gpdSIZ2CR;
			return div;
		}
		I4x2 wDIVcrLAY()
		{
			return winDIVcr.a4x2[1];
		}
		I4x2 wDIVcrALLOCK()
		{
			return winDIVcr.a4x2[1]&gpdCRall;
		}
		void		gpeWINresize( void );


		U4 reSCAN( void )
		{
			bSHIFT = 1&(aKT[SDL_SCANCODE_LSHIFT]|aKT[SDL_SCANCODE_RSHIFT]);
			bCTRL = 1&(aKT[SDL_SCANCODE_LCTRL]|aKT[SDL_SCANCODE_RCTRL]);
			abALT[0] = 1&aKT[SDL_SCANCODE_LALT];
			abALT[1] = 1&aKT[SDL_SCANCODE_RALT];
			bALT = abALT[0]|abALT[1];

			return (((U4)bSHIFT)<<9)|((U4)bALT<<10);
		}

		U4x2 chrWH( void ) {
			return U4x2( chrPIC.w, chrPIC.h );
		}


		U1 onDIVf( const I4x2& mXY ) {
			SDL_Rect dim;
			for( U4 i = 0; i < 4; i++ )
			{
				dim = wDIVpx(i).xyWH;
				if( dim.x > mXY.x || dim.y > mXY.y )
					continue;
				if( mXY.x-dim.x >= dim.w || mXY.y-dim.y >= dim.h  )
					continue;
				return i;
			}
			return (mXY.x/winDIVpx.x) | ((mXY.y/winDIVpx.y)<<1);
		}
		I4x2& winWHpx()
		{
			return winDIVpx.a4x2[1];
		}
		void	WINrun( const char* pWELLCOME );
		bool	WINvar( gpcREG& out, gpeALF alf );

};

#endif // GPCWIN_H
