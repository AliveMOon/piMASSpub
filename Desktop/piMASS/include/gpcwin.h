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
class gpcWINgl
{
public:
	GLint	gVxPos2Loc,
			gVxSucc,
			gFrSucc,
			gSucc;
	GLuint	tmpID,
			gProgID,
			gVxS,
			gFrS,
			gVBO,
			gIBO;

	SDL_GLContext	gCntxt;
	GLenum			glewErr;

	gpcLZY	VxSlog,
			VxSsrc,
			FrSlog,
			frSsrc,
			Lnklog;

	gpcWINgl( gpcWIN& win );


	GLuint VxScmp( const char* pS )
	{
		U8 s = -1, nS = gpmSTRLEN(pS);
		if( !nS )
			return gVxS;

		if( VxSsrc.n_load ? VxSsrc.n_load == nS : false )
		if( gpmMEMCMP( VxSsrc.p_alloc, pS, VxSsrc.n_load ) == VxSsrc.n_load )
				return gVxS;

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
			return gVxS;
		}

		//VxSsrc.lzyRST();
		VxSsrc.lzyINS( (U1*)pS, nS, s = 0, -1 );
		glDetachShader( gProgID, gVxS );
		glDeleteShader( gVxS );
		gVxSucc = gSucc;
		gVxS = tmpID;
		return gVxS;

	}
	GLuint FrScmp( const char* pS )
	{
		U8 s = -1, nS = gpmSTRLEN(pS);
		if( !nS )
			return gVxS;

		if( frSsrc.n_load ? frSsrc.n_load == nS : false )
		if( gpmMEMCMP( frSsrc.p_alloc, pS, frSsrc.n_load ) == frSsrc.n_load )
				return gFrS;

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
			return gFrS;
		}

		//frSsrc.lzyRST();
		frSsrc.lzyINS( (U1*)pS, nS, s = 0, -1 );
		glDetachShader( gProgID, gFrS );
		glDeleteShader( gFrS );
		gFrSucc = gSucc;
		gFrS = tmpID;
		return gFrS;

	}

	GLuint VxFrLink( )
	{
		U8 s;
		glAttachShader( gProgID, gVxS );
		glAttachShader( gProgID, gFrS );
		glLinkProgram( gProgID );
		gSucc = GL_FALSE;
		glGetProgramiv( gProgID, GL_COMPILE_STATUS, &gSucc );
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
		return gProgID;

	}

	GLuint VBOnew( GLfloat* pD, U4 nD, U4 nX )
	{
		//Create VBO
		glGenBuffers( 1, &gVBO );
		glBindBuffer( GL_ARRAY_BUFFER, gVBO );
		glBufferData( GL_ARRAY_BUFFER, nX * nD * sizeof(*pD), pD, GL_STATIC_DRAW );
		return gVBO;
	}

	GLuint IBOnew( GLuint* pD, U4 nD )
	{
		//Create IBO
		glGenBuffers( 1, &gIBO );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gIBO );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, nD * sizeof(*pD), pD, GL_STATIC_DRAW );
		return gIBO;
	}

	void rndr()
	{
		if(!this)
			return;
		//glClearColor( 0.f, 0.f, 0.f, 1.f );
		//glClear( GL_COLOR_BUFFER_BIT );
		GLdouble model[16],
				 proj[16];

		glGetDoublev( GL_MODELVIEW_MATRIX, model );
		glGetDoublev( GL_PROJECTION_MATRIX, proj );

		//glGetDoublev(GL_PROJECTION_MATRIX,projectionMatri x);

		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();

		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();

		//Bind program
		glBegin( GL_QUADS );
			 glVertex2f( -0.5f, -0.5f );
            glVertex2f( 0.5f, -0.5f );
            glVertex2f( 0.5f, 0.5f );
            glVertex2f( -0.5f, 0.5f );

/*			glUseProgram( gProgID );
			glBindBuffer( GL_ARRAY_BUFFER, gVBO );
			glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gIBO );
			glDrawElements( GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL );
			glUseProgram( 0 );*/

		glEnd();



	}
	~gpcWINgl()
	{
		SDL_GL_DeleteContext( gCntxt );
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
						SRCxycr, SRCin;

		gpcWINgl		*pGL;

		SDL_Window		*pSDLwin;
		SDL_Renderer	*pSDLrndr;
		SDL_Texture		*pTXrndr,
						*pTXsnd,
						*pTXchar;

		SDL_Surface		*pSRFload,
						*pSRFchar,
						*pSRFwin,
						*pSRFsnd;
		gpcPIC			*pPICbg;

		SDL_Event 		ev;
		SDL_Rect		chrPIC;

		U1		sHOST[0x100],
				sUSER[0x100],
				gpsKEYbuff[0x100],
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
		I4x4 winDIV;


public:
		~gpcWIN();
		gpcWIN( char* pPATH, char* pFILE, char* sNAME, gpcMASS* piMASS ); //, char* pPATH, char* pFILE ); //, I4x4& siz );

		gpcLZY* putLIST( gpcLZY* pLZY, gpeNET4 nt4, U4 i, U4 j, char* pPRMPT = NULL )
		{
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
		gpcLZY* putLIST( gpcLZY* pLZY, gpeNET4 nt4, I8x4 an, char* pPRMPT = NULL )
		{
			if( !this )
				return pLZY;
			U4x2 zn;
			U4* pM = piMASS->pM( zn );
			if( !pM )
				return pLZY;

			I8x4 lurdZN = an.lurd();
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
		bool bINIThu()
		{
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
		SDL_Rect	wDIV( U1 iDIV );
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

		U4x2 chrWH( void )
		{
			return U4x2( chrPIC.w, chrPIC.h );
		}

		I4x2 wFRM( U1 iDIV )
		{
			SDL_Rect div = wDIV( iDIV );
			return I4x2( div.w/chrPIC.w, div.h/chrPIC.h );
		}
		U1 onDIVf( const I4x2& mXY )
		{
			SDL_Rect dim;
			for( U4 i = 0; i < 4; i++ )
			{
				dim = wDIV(i);
				if( dim.x > mXY.x || dim.y > mXY.y )
					continue;
				if( mXY.x-dim.x >= dim.w || mXY.y-dim.y >= dim.h  )
					continue;
				return i;
			}
			return (mXY.x/winDIV.x) | ((mXY.y/winDIV.y)<<1);
		}
		I4x2& winWH()
		{
			return winDIV.a4x2[1];
		}
		void	WINrun( const char* pWELLCOME );
		bool	WINvar( gpcREG& out, gpeALF alf );

};

#endif // GPCWIN_H
