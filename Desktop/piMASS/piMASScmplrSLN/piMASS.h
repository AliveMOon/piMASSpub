#ifndef piMASS_H_INCLUDED
#define piMASS_H_INCLUDED
//~ MIT License
//
//~ Copyright (c) 2019 AliveMOon
//
//~ Permission is hereby granted, free of charge, to any person obtaining a copy
//~ of this software and associated documentation files (the "Software"), to deal
//~ in the Software without restriction, including without limitation the rights
//~ to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//~ copies of the Software, and to permit persons to whom the Software is
//~ furnished to do so, subject to the following conditions:
//
//~ The above copyright notice and this permission notice shall be included in all
//~ copies or substantial portions of the Software.
//
//~ THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//~ IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//~ FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//~ AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//~ LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//~ OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//~ SOFTWARE.

//#define gpdDEBUG
//? #define gpdNEW_CMPLR

#define bSTDcout true //false //true
#define bSTDcout_V4l2 (bSTDcout&false) //true) // false)//
#define bSTDcout_jsr false //(bSTDcout&true)
#define bSTDcout_slmp (bSTDcout&true)
#define bSTDcoutCMP true //false //true
//#define stdON

#include "mysys.h"
#define piMASS_DEBUG 1
#include <exception>
#include <mysys.h>
#include <errno.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>

#include <inttypes.h>

#include <stdarg.h>

#include <setjmp.h>
#include <math.h>
#include <thread>

#ifdef _WIN64

	#pragma once
	#define _CRT_SECURE_NO_WARNINGS
	#pragma warning( push )
	#pragma warning( disable : 4995 )

	#include <stdint.h>
	#include <assert.h>


	#include <string>
	#include <stdlib.h>
	#include <io.h>
	#include <direct.h>
	#include <malloc.h>
	#include <memory.h>
	#include <WinSock2.h>
	#include <ws2tcpip.h>
	#include "camU.h"
	//#include <ipmib.h>
	#include <iphlpapi.h>
	#include <fcntl.h>


	#define bzero( a, n ) ( (void*)memset( a, 0, n ) )
	#define gpdMAX_PATH _MAX_PATH
	//#define close( h ){ if( h != INVALID_SOCKET ){ closesocket( h ); h = INVALID_SOCKET; } }
	#define ace _access
	#define mkd( a, b ) ( _mkdir( a ) )
	#define usleep Sleep
	#define sleep Sleep
	#define gpmALLOC( n ) ((U1*)_mm_malloc( n, 0x10 ))
	#define gpmFREE( p ) _aligned_free( p )
	//#define gpmFD_CLOSE( h ){ if( h ){ CloseHandle( h ); h = 0; } }
	#define gpmFD_CLOSE( h ){ if( h ){ fclose( h ); h = 0; } }
	#define gpmFF_CLOSE( h ){ if( h ){ _findclose( h ); h = -1L; } }

	#pragma comment (lib, "iphlpapi.lib")
	#pragma comment (lib, "ws2_32.lib")
	#define gpdU4x2nSTR 1
	#define gpdCAMu gpcCAMubi // gpcCAMx64

	extern "C" {
		#include <jpeglib.h>
	}

	#define GLEW_STATIC
	#include <GL/glew.h>

	//#define GL_GLES_PROTOTYPES 0
	//#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
	#define gpdGT_LIST_tOUT 7
	#define gpdTCP_tOUT		1000/50
	#define gpmSTRiSTR strstr
#else

	//#include <opencv2/core/core.hpp>
	//#include <opencv2/highgui/highgui.hpp>
	#include <jpeglib.h>

	#define GLEW_STATIC
    #include <GL/glew.h>
	#ifdef gpdSYSpi
		#include <raspicam/raspicam.h>
		//#include <raspicam/raspicam_cv.h>
		//#include <wiringPi.h>
		#define gpdU4x2nSTR 2
		#define gpdCAMu raspicam::RaspiCam
        #define gpdGT_LIST_tOUT 3
        #define GL_GLES_PROTOTYPES 0
	#else
        //#include <GLFW/glfw3.h>
	    #define GL_GLES_PROTOTYPES 0
		#define gpdU4x2nSTR 1

        #include <camU.h>
        #define gpdCAMu gpcCAMubi

        #define gpdGT_LIST_tOUT 7
	#endif // gpdSYSpi

	#include <unistd.h> // for usleep()

	#include <sys/socket.h>
	#include <sys/ioctl.h>
	#include <sys/select.h>
	#include <sys/stat.h>

	#include <netdb.h>
	#include <fcntl.h>
	#include <ifaddrs.h>
	#include <netinet/in.h>
	#include <netinet/tcp.h>
	#include <arpa/inet.h>
	#include <ifaddrs.h>
	#include <X11/X.h>
	#include <X11/Xlib.h>
	#include <X11/Xutil.h>
	#include "X11/bitmaps/gray"
	#include <X11/Xatom.h>
	//#include <X11/Xmu/CurUtil.h>
	#include <X11/Xcursor/Xcursor.h>
	#include <linux/limits.h>
	//#include <bits/stdc++.h>

	//#include <GL/glew.h>

	#include <pwd.h>

	typedef int			SOCKET;
	typedef sockaddr	SOCKADDR;
	typedef sockaddr_in	SOCKADDR_IN; //struct	sockaddr_in	addrinfo;

	#define ace access
	#define mkd( a, b ) ( mkdir( a,b ) )
	#define gpmALLOC( n ) ((U1*)memalign( 0x10, n ))
	#define gpmFREE( p ) free( p )
	#define gpmFD_CLOSE( h ){ if( h ){ fclose( h ); h = 0; } }
	#define gpmFF_CLOSE( h ){ if( h ){ _findclose( h ); h = -1L; } }

	#define gpdMAX_PATH (PATH_MAX/2)

	#define gpdEV_tOUT		7
	#define gpdTCP_tOUT		1000/20
	/*SOCKET inline gpfSOC_CLOSE(SOCKET& h)
	{
		if (h == INVALID_SOCKET)
			return INVALID_SOCKET;

		close(h);
		h = INVALID_SOCKET;
		return h;
	}*/
	#define gpmSTRiSTR strcasestr
#endif


#define gpdSYNmSEC		333
#define gpdRECVn 		(0x30000/0x4) 	//0x4000*4	//	(0x30000/0x10)
#define gpdHUDn			(gpdRECVn/0x4)

#define gpdWIN_WIDTH	480	//(sdlDM.w*4)/8			//480	//(sdlDM.w*4)/8			//
#define gpdWIN_HEIGHT	800	//(winSIZ.x*6)/8			//800	//(winSIZ.x*6)/8		//

#define gpdRPI_WIDTH	640	//960 	//1280 	//640	//1280	//320	//	Allowable widths: 320, 640, 1280
#define gpdRPI_HEIGHT	320	//480 	//960 	//480 // 960	//240	//	Allowable heights: 240, 480, 960

#define gpdJDOIT_tOUT	3

#define gpdSDL_tOUT		5
#define gpmSDL_FreeSRF( h ){ if( h ){ SDL_FreeSurface( h ); h = NULL; } }
#define gpmSDL_FreeTX( h ){ if( h ){ SDL_DestroyTexture( h ); h = NULL; } }

#define gpdBLT 	SDL_BlitSurface //SDL_BlitSurface	//SDL_LowerBlit
#define gpdBLTs	SDL_BlitScaled 	//SDL_SoftStretch //SDL_BlitScaled	//SDL_LowerBlitScaled

#define gpdBLTstx( st, sxy, dt, dxy ) 	SDL_RenderCopy( dt, st, sxy,  dxy ) 	//SDL_SoftStretch //SDL_BlitScaled	//SDL_LowerBlitScaled
#define gpdBLTtx( st, sxy, dt, dxy )  	SDL_RenderCopy( dt, st, sxy,  dxy )  //SDL_BlitSurface	//SDL_LowerBlit

/// sudo ln -s "/media/alivemoon/UUI/rob_dir/" /robo

/// mkdir /piMASS
/// sudo ln -s /home/alivemoon/Asztal/piMASScmplrSLNpi4/mini_ISO_32x32_1024x1536_3.png /piMASS/mini_char.png

/// sudo cp ~/Asztal/piMASScmplrSLNpi4/bin/Debug/piMASS /usr/sbin/pimass
//#define gpdGT_LIST_tOUT 3
#define gpdPICbg "/mnt/ram/bg.png"
#define gpsMINI_CHAR "mini_char_16x32_8x32.png" //"mini_char_64x512.png"	// "mini_char_16x32.png" //
#define gpsMINI_ISO "mini_ISO_32x32_1024x1536_3.png"




#include <SDL.h>			//-lSDL2
#include <SDL_image.h>
#include <SDL_opengl.h>
#include <SDL_opengles2.h>


// Include GLFW
//#include <glfw3.h>


/*#include "gpcSCHL.h"
#include "gpcOPCD.h"*/

//using namespace std;

#include <UIF1248etc.h>
#include <gpeTYP.h>

enum gpeLX:U8
{
    gpeZERO,
    gpeU4x2nSTR = sizeof(U8)/sizeof(void*),
   // gpeMXPATH = gpmPAD( gpdMAX_PATH, 0x10 ) , //0x10*0x11,

    //gpeSYNmsec = 333,


};


class float2;
class float3;
class float4;		// ez a F4 négyes singleP vektor
class float4x4;		// ja ez itt a matrix lenne
class F2;
class F4;
class F4x4;
class D4;
class U2x4;
class U4x4;
class U8x2;
class U8x4;
class I4x2;
class I4x4;
class I8x2;
class I8x4;
class gpcMASS;
class gpcALU;
//#define gpm_bool( b, p ) if( b ){ (p) }

#define gpdGT_NoDALEY 1 	//0 //
#define INVALID_SOCKET  (SOCKET)(~0)
#define SOCKET_ERROR            (-1)


#define frac( a ) ( (a)-floor((a)) )
#define ABC_ xyz_

class F {
public:
	float f;
	F() {}
	F( float i ) { f = i; }
	F( double i ) { f = i; }
	F( U8 i ) { f = i; }
	F( I8 i ) { f = i; }
	F( int i ) { f = i; }
	bool operator != ( F b ) { return f != b.f; }
	bool operator == ( F b ) { return f == b.f; }

	F& operator *= ( const F& b ) { f*=b.f; return *this; }
	F& operator &= ( const F& b ) { f =frac(f*b.f); return *this; }
	F& operator /= ( const F& b ) { f/=b.f; return *this; }
	F& operator %= ( const F& b ) { f =frac(f/b.f); return *this; }
	float operator % ( const F& b ) { return frac(f/b.f); }
	float operator / ( const F& b ) { return f/b.f; }


	F& operator += ( const F& b ) { f+=b.f; return *this; }
	F& operator |= ( const F& b ) { f =frac(f+b.f); return *this; }
	F& operator -= ( const F& b ) { f-=b.f; return *this; }
};
class D {
public:
	double d;
	D() {};
	D( float i ) { d = i; }
	D( double i ) { d = i; }
	D( U8 i ) { d = i; }
	D( I8 i ) { d = i; }
	D( int i ) { d = i; }
	bool operator != ( D b ) { return d != b.d; }
	bool operator == ( D b ) { return d == b.d; }

	D& operator *= ( const D& b ) { d*=b.d; return *this; }
	D& operator &= ( const D& b ) { d =frac(d*b.d); return *this; }
	D& operator /= ( const D& b ) { d/=b.d; return *this; }
	D& operator %= ( const D& b ) { d =frac(d/b.d); return *this; }
	double operator % ( const D& b ) { return frac(d/b.d); }
	double operator / ( const D& b ) { return d/b.d; }

	D& operator += ( const D& b ) { d+=b.d; return *this; }
	D& operator |= ( const D& b ) { d =frac(d+b.d); return *this; }
	D& operator -= ( const D& b ) { d-=b.d; return *this; }
};



#define gpmMAX( a, b ) ( a > b ? a : b )
#define gpmMIN( a, b ) ( a < b ? a : b )
#define PI acos(-1.0)
#define PIp2 (PI/2.0)
#define PI2 (PI*2.0)
#define COS5 0.99619469809174553229501040247389
#define SIN5 0.08715574274765817355806427083747
#define SIN1 0.0174524064373
#define RAD5 (5.0*PIp180)
#define COSSIN45 0.70710678118654752440084436210485
#define gpdSQRT2 1.4142135623730950488016887242097
#define gpdSQRT3 1.7320508075688772935274463415059f
#define gpmNINCS( s, v )	strspn( (char*)(s), (char*)(v) )
#define gpmSTR2D( p )	strtod( (char*)(p), (char**)&(p) )
#define gpmSTR2U8( p, radx )	strtol( (char*)(p), (char**)&(p), (radx) )
#define gpdVAN	strcspn
#define gpmPAD( n, p ) ( (n) + (((n)%(p)) ? ((p)-((n)%(p))) : 0) )
#define gpmCLR	if( this ) bzero( this, sizeof(*this) )
#define gpmN( p ) ( sizeof(p)/sizeof(*p) )
#define gpmZ( p ) bzero( &p, sizeof(p) )
#define gpmZn( p, n ) bzero( (p), (n) )
#define gpmZnOF( p, n ) bzero( (p), (n)*sizeof(*(p)) )

#define gpmGLBOFF(a) (void*)(a)

#define gpmOFF( st, m ) ( (size_t)(&((st*)NULL)->m) )
#define gpmOFFOFF(st, m, n ) (gpmOFF(st, n)-gpmOFF(st, m))

#define gpmSTRCPY( d, s ) \
			(												\
				( (d)&&(s)&&(((char*)(d))!=((char*)(s))) ) 	\
				? ( strcpy( ((char*)(d)), ((char*)(s)) ) ) 	\
				: ( (char*)(d) ) 							\
			)

#define gpmALF2STR( d, alf ) \
			(												\
				(d) ? gpfALF2STR( (char*)(d), (gpeALF)alf )	\
					: 0 									\
			)
#define gpmMcpy( d, s, n ) \
			((U1*)(											\
				(											\
				 	(n)										\
					&&(d)									\
					&&(s)									\
					&&( ((U1*)(d)) != ((U1*)(s)) )			\
				)											\
				? ( memcpy( (d), (s), (n) ) )				\
				: ( d )  									\
			))
/// WARNING ez beszorozza a sizeof()-val
#define gpmMcpyOF( d, s, n ) gpmMcpy( (d), (s), ((n)*sizeof(*(d))) )

#define gpmSTRnCPY( d, s, n ) \
			(														\
				( (n)&&(d)&&(s)&&(((char*)(d))!=((char*)(s))) ) 	\
				? ( (U1*)strncpy( (char*)(d), (char*)(s), (n) ) )  		\
				: ( (U1*)d )  									\
			)

#define gpmSTRLEN( s ) ((s)? ( *(char*)s ? strlen((char*)(s)) : 0 ) : 0)
#define gpmVAN( d, v, l ) gpfVAN( (U1*)(d), (U1*)v, l )

#define gpmDEL( p ){ if( (p) ){ delete (p); (p) = NULL; } }
#define gpmDELary( p ){ if( (p) ){ delete[] (p); (p) = NULL; } }
#define gpmMcmp gp_memcmp
#define gpmMcmpOF(d,s,n) (gpmMcmp( (d), (s), (n)*sizeof(*d) )/sizeof(*d))
#define gpmUnB( u )		( abs(u)>0x7fFF ? (abs(u)>0x7fffFFFF ? 8 : 4) : (abs(u)>0x7f ? 2 : 1)  )
#define gpmFnB( f )		( abs(f)>0xffFFFF ? 8 : 4)		// float 23bit felbontású
#define gpmSHnB( b )	( b>2 ? (b>4 ? 3 : 2) : (b>1 ? 1 : 0)  )
#define gpdPUB "+--- --  -   "
//#define gpdSIZ2CR  I4x2(6,9)
#define gpdSIZ2CR  I4x2(4,6)
#define gpdCRall I4x2(1,2)

#define ms2sec 1000
#define gpdROBlim 100
#define mmX(a) ((a)*100)
#define degX(a) ((a)*100)
#define zsIO 20
#define zsIN 400
#define zsINin (zsIN-zsIO)
#define zsIN100 mmX(zsIN)
#define zsOU (zsIN+zsIO)
#define zsOU100 mmX(zsOU)


#define gpfVnN( d, v ) gpfVANnNINCS( (U1*)(d), (U1*)(v))
double inline gpfRAMADZSAN( double a, double b )
{
	if(a==0.0)
		return 4*b;
	if(b==0.0)
		return 4*a;
	if( a==b )
		return 2.0*a*PI;

	return PI*(3.0*(a+b)-sqrt((3.0*a + b)*(a + 3.0*b)));
}
GLenum inline gpfGLerr( const char* pERR = "" ) {
	GLenum e = glGetError();
	if( !e )
		return 0;
	std::cout << std::hex << e << pERR <<  std::endl;
	return e;
}
//#define gpmbABC( c ) (c < 0x80 ? gpaALFsub[c] : true)
SOCKET inline gpfSOC_CLOSE( SOCKET& h )
{
	if( h == INVALID_SOCKET )
		return INVALID_SOCKET;

	close( h );
	h = INVALID_SOCKET;
	return h;
}

inline U1* gpfSTR( U1* pSTR, const U1* pU ) {
	if(pSTR == pU)	/// ha mindkető NULL akkor is kész
		return pSTR;

	U8 nSTR = gpmSTRLEN(pSTR), nU = gpmSTRLEN( pU );
	if( !nU )
	{
		gpmDELary(pSTR);
		return pSTR;
	}
	if( nU < nSTR )
	{
		gpmMcpy( pSTR, pU, nU );
		pSTR[nU] = 0;
		return pSTR;
	}
	pSTR = new U1[nU+1];
	gpmMcpy( pSTR, pU, nU );
	pSTR[nU] = 0;
	return pSTR;
}
U8 inline gpfABCvan( const U1* p_str, const U1* pE, U8& nUTF8, const U1* gpaALFsub ) {
	/// a viszatérési érték nBYTE, nLEN az UTF(
	nUTF8 = 0;
	if( (p_str < pE) ? !*p_str : true )
		return 0;

	U1* pS = (U1*)p_str;
	while( pS < pE )
	{
		if( (*pS)&0x80 )
			return pS-p_str;

		if( gpaALFsub[*pS] )
			return pS-p_str;

		nUTF8++;
		pS++;
	}
	return pS-p_str;
}

U8 inline gpfABCnincs( const U1* p_str, const U1* pE, U8& nUTF8, const U1* gpaALFsub ) {
	/// a viszatérési érték nBYTE, nLEN az UTF(
	nUTF8 = 0;
	if( (p_str < pE) ? !*p_str : true )
		return 0;

	U1* pS = (U1*)p_str;
	while( pS < pE )
	{
		if( (*pS)&0x80 )
		{
			if( (*pS)&0x40 )	// UTF8-nál az első kódnál van 0x110yYYYY a többi már 0x10xxXXXX
				nUTF8++;
			pS++;
			continue;
		}
		if( !gpaALFsub[*pS] )
			break;

		nUTF8++;
		pS++;
	}
	return pS-p_str;
}
U8 inline gpfABC_H_nincs( const U1* p_str, const U1* pE, U8& nUTF8, const char* gpaALF_H_sub ) {
	/// a viszatérési érték nBYTE, nLEN az UTF(
	nUTF8 = 0;
	if( (p_str < pE) ? !*p_str : true )
		return 0;

	U1* pS = (U1*)p_str;
	while( pS < pE )
	{
		if( (*pS)&0x80 )
		{
			if( (*pS)&0x40 )	// UTF8-nál az első kódnál van 0x110yYYYY a többi már 0x10xxXXXX
				nUTF8++;
			pS++;
			continue;
		}
		if( !gpaALF_H_sub[*pS] )
			break;

		nUTF8++;
		pS++;
	}
	return pS-p_str;
}
#define gpdSTR_READ( x ) ( (x) ? (x) : "" )

#ifndef MAKE_ID
	#define MAKE_ID( a,b,c,d )					\
		(									\
			  (((U4)d)<<24)					\
			| (((U4)c)<<16)					\
			| (((U4)b)<< 8)					\
			| ((U4)a)						\
		)
#endif

//#define stdOFF      0
#define stdRESET	"\033[0m"

#define stdBLACK	"\033[1;30m"
#define stdRED		"\033[1;31m"
#define stdGREEN	"\033[1;32m"
#define stdYELLOW	"\033[1;33m"
#define stdBLUE		"\033[1;34m"
#define stdPURPLE	"\033[1;35m"
#define stdCYAN		"\033[1;36m"
#define stdWHITE	"\033[1;36m"

#define stdALU		stdYELLOW
#define stdMINI		stdBLUE
#define stdRUN		stdPURPLE
#define stdCMPLR	stdRED
#define stdDBG		stdCYAN
#define stdBREAK	stdGREEN
#define stdRDY		stdCYAN

enum gpeCLR: U1
{
	gpeCLR_black,	gpeCLR_red, 	gpeCLR_green,	gpeCLR_blue,
	gpeCLR_violet,	gpeCLR_red2, 	gpeCLR_green2,	gpeCLR_blue2,
	gpeCLR_brown,	gpeCLR_orange,	gpeCLR_yellow,  gpeCLR_cyan,
	gpeCLR_gray,	gpeCLR_gray2,	gpeCLR_grey3,	gpeCLR_white,

};
enum gpeLZYset : U1
{
	gpeLZYoff,
	gpeLZYwipSTK,
	gpeLZYwip,
	gpeLZYxN,
	gpeLZYxSOF,
};
enum gpeWIP : U1
{
	gpeWIP_init,
	gpeWIP_busy,
	gpeWIP_done,
	gpeWIP_go,
	gpeWIP_idle,
	gpeWIP_esc,
	gpeWIP_die,
};


enum gpeFD {
	gpeFDrcv,
	gpeFDsnd,
	gpeFDexcp,
	gpeFDn,
};




#include "gpeALF.h"

inline void* gpfMset( void* pD, size_t n, void* pS, size_t nS ) {
	if( !n || !nS )
		return pD;

	if( nS == 1 )
		return memset( pD, *(U1*)pS, n );

	if( pD != pS )
		memcpy( pD, pS, nS );

	if( n < 2 )
		return pD;
	U1 	*p_a = (U1*)pD,
		*p_b = p_a+nS,
		*p_e = p_a+n*nS;

	size_t a,b;
	while( p_b < p_e )
	{
		a = p_b-p_a;
		b = p_e-p_b;
		if( a > b )
			a = b;

        memcpy( p_b, p_a, a );
        p_b += a;
	}
	return pD;
}
#define gpmMsetOF( d, n, s ) gpfMset((d), (n), (s), sizeof(*(s))  )
int inline gpfACE( const char* p_file, I4 mode ) {
	//	00	Existence only
	//	02	Write-only
	//	04	Read-only
	//	06	Read and write
	if( !p_file )
		return -1; // ha negatív nem elérhetõ

	int io = ace( p_file, mode );
	return io;
}
#define gpmACE( p, m ) gpfACE( (char*)(p), (m) )
bool inline gpfMKDR( char* p_buff, const char* p_new ) {
	if( p_new ? !*p_new : true )
		return false;
	int mi;
	char	*p_mkdir = p_buff+sprintf( p_buff, p_new ),
			*p_end = p_mkdir, *p_last = p_mkdir,
			*p_first = NULL;
	while( p_mkdir = strrchr( p_buff, '/') )
	{
		*(p_last = p_mkdir) = 0;
		if( !p_first )
			p_first = p_last;

		if( gpfACE( p_buff, 0 ) > -1 )
			break;

		mi = //mkdir
				mkd( p_buff, S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH );
		if( mi > -1 )
			break;

	}
	while( p_last < p_first )
	{
		*p_last = '/'; // \\';
		p_last += gpmSTRLEN( p_last );
		if( gpfACE( p_buff, 0 ) > -1 )
			break;
		mi = //mkdir
				mkd( p_buff, S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH );

	}
	return p_last == p_first;
}
inline char* gpfP2F( char* p2P, char* p2F, const char* pS, char c = '/' ) {
	char* pC = strrchr( (char*)pS, c );

	if( !pC )
		strcpy( (p2P+=sprintf(p2P,".%c",c)), pS ); // nincsen path
	else {
		pC++;
		p2P = strcpy( p2P, pS ) + (pC-pS);
	}

	strcpy( p2F, p2P );
	*p2P = 0;
	return p2P;
}

inline U8 gp_memcmp( const void* pA, const void* pB, size_t n ) {
	if( !pA || !pB )
		return 0;

	if( pA == pB )
		return n;

	U1	*pAu1 = (U1*)pA,
		*pBu1 = (U1*)pB;
	size_t i = 0;
	while( n-i > 8 )
	{
		if( *(U8*)(pAu1+i) != *(U8*)(pBu1+i) )
			break;
		i += 8;
	}
	while( n-i > 4 )
	{
		if( *(U4*)(pAu1+i) != *(U4*)(pBu1+i) )
			break;
		i += 4;
	}
	while( n-i > 2 )
	{
		if( *(U2*)(pAu1+i) != *(U2*)(pBu1+i) )
			break;
		i += 2;
	}
	while( i < n )
	{
		if( pAu1[i] != pBu1[i] )
			return i;
		i++;
	}
	return n;

}

inline size_t gpfMM( U1* pA, size_t nA, U1* pB, size_t nB, U1** ppFND = NULL ) {
	/// ha talált kissebb lesz mint az nA
	size_t nFND, nF = 0;
	if( nB > nA )
		return nA;
	// nem cserélgetünk megtévesztő
	U1* pT = pA;

	for( U1 *pE = pA+(nA-nB); pA <= pE; pA++ )
	{
		if( *pA != *pB )
			continue;
		nFND = gp_memcmp( pA, pB, nB );
		if( nFND < nB )
		{
			if( !ppFND )
				continue;

			if( nF < nFND )
			{
				nF = nFND;
				*ppFND = pA;	// azért meg tudja mondani hol talált hasonlót
			}
			continue;
		}

		return pA-pT;
	}
	return nA;
}

inline U2 swp2( const void* p ) {
	register U2 u = *(U2*)p;
	return    (u<<8)|(u>>8);
}
inline U4 swp4( const void* p ) {
	register U4 u = *(U4*)p,
	uu =	 (u<<24)
			|((u&0xFF000000)>>24)
			|((u&0xFF0000)>>8)
			|((u&0xFF00)<<8);
	return uu;
}

class UTF8 {
	U1* pU;
public:
	UTF8( void* pVOID ){
		pU = (U1*)pVOID;
	};
	U1 operator = ( U1 ascii )
	{
		if( this ? !pU : true )
			return 0;

		*pU = ascii;
		return ascii;
	}
	U1* pBK( void )
	{
		if( this ? !pU : true )
			return NULL;

		while( *pU & 0x80 )
        {
			if( (*pU & 0xc0) == 0xc0 )
				return pU;
			pU--;
        }
		return pU;
	}
	U1* pNX( void )
	{
		if( !pBK() )
			return NULL;

		pU++;
		while( *pU & 0x80 )
        {
			if( (*pU & 0xc0) == 0xc0 )
				return pU;
			pU++;
        }

		return pU;
	}

	U1* operator ++ ()		// ++pUTF8
	{
		return pNX();
	}

	U1* operator ++ ( int )
	{
		// post
		U1* pO = pU;
		pNX();
		return pO;
	}

	UTF8& operator += ( U8 n ) {
		if( pBK() ) {
			for( U8 i = 0; i < n; i++ ) {
				if( !*pU )
					break;

				pU++;
				while( *pU & 0x80 ) {
					if( (*pU & 0xc0) == 0xc0 )
						break;
					pU++;
				}
			}
		}
		return *this;
	}

	UTF8& operator -= ( U8 n ) {
		if( pBK() ) {
			for( U8 i = n; i; i++ ) {
				if( !*pU )
					break;

				pU++;
				pBK();
			}
		}
		return *this;
	}

};
enum gpeISA : I1 {
	gpeISA_nop,
	gpeISA_u8,
	gpeISA_i8,
	gpeISA_d8,
	gpeISA_an,
	gpeISA_anFUN,
	gpeISA_var,
	gpeISA_FUN,
	gpeISA_dim,
	gpeISA_not 		= '!',
	gpeISA_str,		// 		= '\"'
	gpeISA_tag, 	// 		= '#',
	gpeISA_dollar,	//		= '$'
	gpeISA_rem,		// 		= '%',
	gpeISA_and,		// 		= '&',
	gpeISA_brkB 	= '(',
	gpeISA_brkE,	//		= ')',
	gpeISA_mul,		//		= '*',
	gpeISA_plus,	//		= '+',
	gpeISA_col,		//		= ',',
	gpeISA_sub,		//		= '-',
	gpeISA_dot,		//		= '.',
	gpeISA_div,		//		= '/',

	gpeISA_row 		= ';',
	gpeISA_litl 	= '<',
	gpeISA_assign 	= '=',
	gpeISA_gret 	= '>',
	gpeISA_exp 		= 'E',
	gpeISA_root		= 'R',
	gpeISA_brkDB 	= '[',
	gpeISA_brkDE 	= ']',
	gpeISA_equ		= 'e',
	gpeISA_trg		= 'x',
	gpeISA_blkB 	= '{',
	gpeISA_or 		= '|',
	gpeISA_blkE		= '}',

};

class U1x4 {
public:
    union
    {
        struct
        {
            U1 x,y,z,w;
        };
         struct
        {
            U1 R,G,B,A;
        };
        struct
        {
            U1 aXYZW[4];
        };
        struct
        {
            gpeISA aISA[4];
        };
        struct
        {
            U4 u4;
        };
    };
    U1x4(){};
    bool operator != ( U1x4 b ) { return u4 != b.u4; }
	bool operator == ( U1x4 b ) { return u4 == b.u4; }

    U1x4& operator *= ( const U1x4& b ) { x*=b.x; y*=b.y; z*=b.z; w*=b.w; return *this; }
	U1x4& operator &= ( const U1x4& b ) { u4 &= b.u4; return *this; }
	U1x4& operator /= ( const U1x4& b ) {
		x = b.x ? x/b.x : 0xff;
		y = b.y ? y/b.y : 0xff;
		z = b.z ? y/b.z : 0xff;
		w = b.w ? y/b.w : 0xff;
		return *this;
	}
	U1x4& operator %= ( const U1x4& b ) {
		x = b.x ? x%b.x : 0x0;
		y = b.y ? y%b.y : 0x0;
		z = b.z ? y%b.z : 0x0;
		w = b.w ? y%b.w : 0x0;
		return *this;
	}

	U1x4& operator += ( const U1x4& b ) { x+=b.x; y+=b.y; z+=b.z; w+=b.w; return *this; }
	U1x4& operator |= ( const U1x4& b ) { u4 |= b.u4; return *this; }
	U1x4& operator -= ( const U1x4& b ) { x-=b.x; y-=b.y; z-=b.z; w-=b.w; return *this; }

    U1x4& null() {
		u4 = 0;
		return *this;
    }
    U1x4( U4 b ) {
        u4 = b;
    }
    U1x4( U1 _x, U1 _y, U1 _z = 0, U1 _w = 0 ) { x=_x; y=_y; z=_z; w=_w; }
    U1x4& str2time( U1* p_str, U1* p_end, U1** pp_str = NULL );
    U4 area_xy() {
		U4 a = x;
		a *= y;
		return a;
    }
    U4 area_yz() {
		U4 a = y;
		a *= z;
		return a;
    }
    U4 area_zw() {
		U4 a = z;
		a *= w;
		return a;
    }
    U4 area() {
		U4 a = x;
		a *= y;
		a *= z;
		a *= w;
		return a;
    }
    U4 area_xyz() {
		U4 a = x;
		a *= y;
		a *= z;
		return a;
    }
	U1 mn() const {
		U1 o = x;
		if( o > y )
			o = y;
		if( o > z )
			o = z;
		if( o >= w )
			return w;

		return o;
	}
	U1 mx() const {
		U1 o = x;
		if( o < y )
			o = y;
		if( o < z )
			o = z;
		if( o <= w )
			return w;

		return o;
	}

	U1 mn_xyz() const {
		U1 o = x;
		if( o > y )
			o = y;
		if( o >= z )
			return z;

		return o;
	}
	U1 mx_xyz() const {
		U1 o = x;
		if( o < y )
			o = y;
		if( o <= z )
			return z;

		return o;
	}

    U1x4& mx( U1x4 xyzw ) {
		if( x < xyzw.x )
			x = xyzw.x;
		if( y < xyzw.y )
			y = xyzw.y;
		if( z < xyzw.z )
			z = xyzw.z;
		if( w < xyzw.w )
			w = xyzw.w;

		return *this;
    }
    U1x4& mn( U1x4 xyzw ) {
		if( x > xyzw.x )
			x = xyzw.x;
		if( y > xyzw.y )
			y = xyzw.y;
		if( z > xyzw.z )
			z = xyzw.z;
		if( w > xyzw.w )
			w = xyzw.w;

		return *this;
    }
	// typ:
	/// x[7s,6f,5r,4str : 3-0 nBYTE = 1<<(x&0xf) ]
	/// yz[ dimXY ] 	, w nBYTE //= 1<<(x&0xf)
	U1x4& typ() {
		if( w )
			return *this;

		if( !y ) y = 1;
		if( !z ) z = 1;
		if( x&0x40 )	// float or double
			x = ((x&0xf) < 2 ) ? 0xc2 : 0xc3;

		w = 1<<(x&0xf);	//
		return *this;
	}
	U1x4& typMX( U1x4 tp ) {
		U1	sh1 = x&0xf;
		if( sh1 < tp.x&0xf ) 	// sh: 0B?1 1WH2 2LF4 3QD8
			sh1 = tp.x&0xf;
		tp.x = (tp.x|x)&0xf0;

		if( tp.x&0x40 )
		{
			tp.x |= 0x80;	// float akor signed
			if( sh1 < 2 )
				sh1 = 2;	// ha float most 4b kisebb nem lehet
		}

		x = tp.x|gpmMIN(3,sh1);
		w = 1<<(x&0xf);

		// legalább 1x1 es valamikből áll
		if( y < tp.y )
			y = tp.y;
		if( z < tp.z )
			z = tp.z;

		return *this;
	}
	U1* typ2str( U1* pBUFF );	// x[7s,6f,5r,4str : 3-0 nBYTE = 1<<(x&0xf) ] // yz dimxy
	U1x4* pos( I4x2 pxy, const I4x4& whp );
	U1x4* frmFILL( I4x2 cr, U1x4 u, I4x4 whp );
	U1x4* frmBRDR( I4x2 cr, gpeCLR clr, U1 flg, I4x4 whp  );

	U1x4* print( U1* p_str, gpeCLR clr ) {
		if( this ? !p_str : true )
			return this;

		U1x4* p4 = this;
		while( *p_str )
		{
			p4->u4 = 0;
			p4->B = clr;
			p4->A = *p_str;
			p4++;
			p_str++;
		}
        return p4;
	}
	U1x4* zyxw( void* pV, U4 n ) {
		if( !this )
			return NULL;

		gpmMcpyOF( this, pV, n );
		for( U4 i = 0; i < n; i++ )
			this[i].swpZX();

		return this;
	}
	U1x4* zwxy( void* pV, U4 n ) {
		U1x4 *p1 = this,
			 *p2 = (U1x4*)pV;
		for( U4 i = 0; i < n; i++ )
		{
			p1[i].x = p2[i].z;
			p1[i].y = p2[i].w;
			p1[i].z = p2[i].x;
			p1[i].w = p2[i].y;
		}
		return this;
	}
	U1x4* wzyx( const void* pV, U4 n ) {
		U1x4 *p1 = this;
		U4* pU =(U4*)pV;
		for( U4 i = 0; i < n; i++ )
			p1[i].u4 = swp4(pU+i);
		return this;
	}

	U1x4* AND( void* pV, U4 n ) {
		if( this ? !n : true )
			return this;

		U4 i = 0;
		for( U4	ni = n>>1;
				i<ni;
				i++ )
			((U8*)this)[i] &= ((U8*)pV)[i];

		if( !(n&1) )
			return this;

		i <<= 1;
		((U4*)this)[i] &= ((U4*)pV)[i];
		return this;
	}
	U1x4* OR( void* pV, U4 n ) {
		if( this ? !n : true )
			return this;

		U4 i = 0;
		for( U4	ni = n>>1;
				i<ni;
				i++ )
			((U8*)this)[i] |= ((U8*)pV)[i];

		if( !(n&1) )
			return this;

		i <<= 1;
		((U4*)this)[i] |= ((U4*)pV)[i];
		return this;
	}
	/// 13
	U1x4& swpYX() {
		U1 t = x;
		x = y;
		y = t;
		return *this;
	}
	/// 25
	U1x4& swpZX() {
		U1 t = x;
		x = z;
		z = t;
		return *this;
	}
	/// 36
	U1x4& swpZY() {
		U1 t = y;
		y = z;
		z = t;
		return *this;
	}

	/// 49
	U1x4& swpWX() {
		U1 t = x;
		x = w;
		w = t;
		return *this;
	}

	/// 5A
	U1x4& swpWY() {
		U1 t = y;
		y = w;
		w = t;
		return *this;
	}
	/// 6C
	U1x4& swpWZ() {
		U1 t = z;
		z = w;
		w = t;
		return *this;
	}

	U1x4 srt3() {
		if( !(u4&0xffffff) )
			return 0;

		U1x4 s = *this;
		s.w = 1;
		if(s.y>s.x)
		{
			s.swpYX();
			s.w = 0x2;
		}
		if(s.z>s.x)
		{
			s.swpZX();
			s.w |= 0x04;
		}
		if(s.z>s.y)
		{
			s.swpZY();
			s.w |= 0x08;
		}

		s.x = (s.x&0xf0)|s.w;
		s.y = (s.y&0xf0)|(s.z>>4);
		return s;
	}
	U1x4& swp3( U1 s ) {
		if(s&1)
		{
			if( s&0x08 )
				swpZY();
			if( s&0x04 )
				swpZX();
			if( s&0x02 )
				swpYX();
		}

		return *this;
	}

	U1 srt4() {
		if( !u4 )
			return 0;

		U1 o = 0;
		U1x4 s = *this;
		s.w = 1;
		if(s.y>s.x)
		{
			s.swpYX();
			o |= 0x2;
		}
		if(s.z>s.x)
		{
			s.swpZX();
			o |= 0x04;
		}
		if(s.z>s.y)
		{
			s.swpZY();
			o |= 0x8;
		}

		/// srt3
		if( !w )
			return o;

		if(s.w>s.x)
		{
			s.swpWX();
			o |= 0x10;
		}

		if(s.w>s.y)
		{
			s.swpWY();
			o |= 0x20;
		}

		if(s.w>s.z)
		{
			s.swpWZ();
			o |= 0x40;
		}

		return o;
	}


	U1x4 operator & ( U4 b ) const { return u4&b; }
	U1x4 operator | ( U4 b ) const { return u4|b; }
	U1x4 operator ^ ( U4 b ) const { return u4^b; }
	U1x4 operator % ( const U1x4& b ) { return U1x4( x?frac(x%b.x):0, y?frac(y%b.y):0, z?frac(z%b.z):0, w?frac(w%b.w):0 ); }
	U1x4 operator / ( const U1x4& b ) { return U1x4( x?x/b.x:0xff, y?y/b.y:0xff, z?z/b.z:0xff, w?w/b.w:0xff ); }


	U1x4 operator & ( U1 b ) const {
		if( !b )
			return 0;

		return u4&(0x1010101*(U4)b);
	}
	U1x4 operator | ( U1 b ) const {
		if( !b )
			return 0;

		return u4|(0x1010101*(U4)b);
	}
	U1x4 operator ^ ( U1 b ) const {
		return u4^(0x1010101*(U4)b);
	}


	U1x4 operator << ( I1 b ) const {
		if( !b )
			return *this;
		if( b < 8 )
			return U1x4( x<<b, y<<b, z<<b, w<<b );
		return 0;
	}
	U1x4 operator << ( const U1x4& b ) const {
		return U1x4( x<<b.x, y<<b.y, z<<b.z, w<<b.w );
	}
	U1x4 operator >> ( I1 b ) const {
		if( !b )
			return *this;
		if( b < 8 )
			return U1x4( x>>b, y>>b, z>>b, w>>b );
		return 0;
	}
	U1x4 operator >> ( const U1x4& b ) const {
		return U1x4( x>>b.x, y>>b.y, z>>b.z, w>>b.w );
	}
	U1x4& operator >>= ( U1 b ) {
		if( !b )
			return *this;
		if( b > 7 )
			return null();
		x >>= b;
		y >>= b;
		z >>= b;
		w >>= b;
		return *this;
	}
	U1x4& operator <<= ( U1 b ) {
		if( !b )
			return *this;
		if( b > 7 )
			return null();
		x <<= b;
		y <<= b;
		z <<= b;
		w <<= b;
		return *this;
	}

	U4 bugU1( I4x2* pR, U4* pMSK, I4 mom, I4 b, I4* pD, U4 n, I4 rg = 0, U4 nX = 0 );
	U4 bugW( I4x2* pR, U4* pMSK, I4 mom, I4 b, I4* pD, U4 n, U4 nX = 0 );

	U1x4* cpyX( U1x4 *p_s,
				U4 dw, 		U4 dwh,
				U4 dsx = 1,	U4 dsy = 0,
				U4 ssx = 1,	U4 ssy = 0 ) {
		U1x4* p_d = this;
		for( U1x4*	p_de = p_d+dwh;

					p_d < p_de;

					p_d += dsy,
					p_s += ssy )
		for( U4 d = 0, s = 0; d < dw; d += dsx, s += ssx )
				p_d[d] = p_s[s];

		return this;

	}
	int hstX( I4x4* pH, int p, int shr, int w, int h = 0 );
	int hstXrgb( I4x4* pH, int p, int shr, int w, int h = 0 );
	U1x4 xyz0() const { return U1x4(x,y,z); }
	U1x4 xy00() const { return U1x4(x,y); }
	U1x4& e8xyz_( const U1x4& b ) {
		U4 bb = b.x;
		x = (bb*bb)>>8;
		bb = b.y;
		y = (bb*bb)>>8;
		bb = b.y;
		z = (bb*bb)>>8;
		return *this;
	}
	U1x4& e8( const U1x4& b ) {
		U4 bb = b.x;
		x = (bb*bb)>>8;
		bb = b.y;
		y = (bb*bb)>>8;
		bb = b.y;
		z = (bb*bb)>>8;
		bb = b.w;
		w = (bb*bb)>>8;
		return *this;
	}
};

class I1x4 {
public:
    union
    {
        struct
        {
           I1 x,y,z,w;
        };
        struct
        {
            I1 aXYZW[4];
        };
        struct
        {
            gpeISA aISA[4];
        };
        struct
        {
            U4 u4;
        };
    };
    I1x4& null()
    {
		u4 = 0;
		return *this;
    }
    I1x4(){};
    I1x4( U4 b )
    {
        u4 = b;
    }
    I1x4( I1 _x, I1 _y, I1 _z = 0, I1 _w = 0 )
    {
        x = _x; y = _y; z = _z; w = _w;
    }
    I1x4& operator = ( U4 b )
    {
		u4 = b;
		return *this;
    }
    //U1x4& str2time( U1* p_str, U1* p_end, U1** pp_str = NULL );
    I4 area_xy()
    {
		I8 a = x;
		a *= y;
		return a;
    }
    I4 area_yz()
    {
		I8 a = y;
		a *= z;
		return a;
    }
    I4 area_zw()
    {
		I8 a = z;
		a *= w;
		return a;
    }
    I4 area()
    {
		I8 a = x;
		a *= y;
		a *= z;
		a *= w;
		return a;
    }
    I4 area_xyz()
    {
		I8 a = x;
		a *= y;
		a *= z;
		return a;
    }

};


class U2x2 {
public:
	union
	{
		struct { U2 x,y; };
		struct { U2 i,n; };
		struct { U2 prt,srf; };
		struct { U1x4 aCLR[2]; };
		struct { U4 u4; };
	};

    U2x2(){};
    U2x2( U2 _x, U2 _y = 0 )
    {
        x = _x; y = _y;
    }

	U2x2& null( void )
	{
		u4 = 0;
		return *this;
	}
	U2x2& operator += ( U2 u )
	{
		x += u;
		y += u;
		return *this;
	}
	U2x2& operator += ( I2 i )
	{
		x += i;
		y += i;
		return *this;
	}
	U2x2& operator += ( float f )
	{
		x += f;
		y += f;
		return *this;
	}

	U2x2& operator -= ( U2 u )
	{
		x -= u;
		y -= u;
		return *this;
	}
	U2x2& operator -= ( I2 i )
	{
		x -= i;
		y -= i;
		return *this;
	}
	U2x2& operator -= ( float f )
	{
		x -= f;
		y -= f;
		return *this;
	}
	U2x2& operator = ( int i )
	{
		if( !i )
			return null();
		if( i < 0 )
			i *= -1;
		x = y = i;
		return *this;
	}

	//U2x2& operator = ( const I4x2& b );


	U2x2 operator + (const U4 b) const
	{
		return U2x2( x+b, y+b );
	}
	U2x2 operator + (const U2x2 b) const
	{
		return U2x2( x+b.x, y+b.y );
	}

	U2x2 operator - (const U4 b) const
	{
		return U2x2( x-b, y-b );
	}
	U2x2 operator - (const U2x2 b) const
	{
		return U2x2( x-b.x, y-b.y );
	}



	U4 operator * (const U2x2& b) const
	{
		return (U4)x*b.x + (U4)y*b.y;
	}
	U2x2 operator & (const U2x2& b) const
	{
		return U2x2( x*b.x, y*b.y );
	}
	U2x2& operator *= ( U2 i )
	{
		if( !i )
			return null();
		if( i == 1 )
			return *this;

		x*=i;
		y*=i;

		return *this;
	}
	U2x2& operator /= ( U2 i )
	{
		if( i == 1 )
			return *this;

		if( !i )
		{
			x = y = 0xffffffff;
			return *this;
		}

		x/=i;
		y/=i;

		return *this;
	}
	U2x2& operator %= ( U2 i )
	{
		if( i == 1 )
			return null();

		if( !i )
			return *this;
		x%=i;
		y%=i;
		return *this;
	}


	U4 sum() const 		{ return (!this?0:(U4)x+y); }
	U4 area() const 	{ return (!this?0:(U4)x*y); }
	U4 are_sum() const 	{ return area()+sum(); }
	U4 qlen() const 	{ return (!this?0:((U4)x*x+(U4)y*y)); }
	U2 mn() const 		{ return (!this?0:(x < y ? x:y)); }
	U2 mx() const 		{ return (!this?0:(x > y ? x:y)); }

	U2x2 abs( void ) const
	{
		return U2x2( x<0?-x:x, y<0?-y:y );
	}
	U2x2& mx( U2x2 b )
	{
		if( x < b.x )
			x = b.x;
		if( y < b.y )
			y = b.y;
		return *this;
	}

	U2x2& mn( const U2x2& b )
	{
		if( x > b.x )
			x = b.x;
		if( y > b.y )
			y = b.y;
		return *this;
	}


};

class U2x4 {
public:
    union
    {
        struct
        {
            U2 x,y,z,w;
        };
        struct
        {
			U2x2 a2x2[2];
		};
        struct
        {
            U2 aXYZW[4];
        };
    };

    U2x4(){};
    U2x4& null( void )
	{
		gpmCLR;
		return *this;
	}
    U2x4( U2 _x, U2 _y = 0, U2 _z = 0, U2 _w = 0 )
    {
        x = _x; y = _y; z = _z; w = _w;
    }
	//U2x4( const I4x2 _xy, I4x2* p_zw = NULL );
    U2x4( U2x2& _xy, U2x2* p_zw = NULL )
    {
        a2x2[0] = _xy;
        a2x2[1] = p_zw ? *p_zw : _xy;
    }
    U2x4( U2x2& _xy, U2x2& _zw )
    {
        a2x2[0] = _xy;
        a2x2[1] = _zw;
    }
    U2x4& operator = ( U2 b )
    {
        x = y = z = w = b;
    }
    U2x4& operator = ( I2 b )
    {
        x = y = z = w = abs(b);
    }
    U2x4& operator = ( const U2x2& b )
    {
		a2x2[1] = a2x2[0] = b;
		return *this;
    }
    U2x4& operator = ( const I4x2& b );


    U2x4& str2date( U1* p_str, U1* p_end, U1** pp_str = NULL );
    char* pSTR( char* pBUFF, const char* pSP = ", "  )
    {
		sprintf( pBUFF, "%d%s%d%s%d%s%d%s", x,pSP,y,pSP,z,pSP,w,pSP );
		return pBUFF;
    }


    U2x4 zwxy( void )  const
    {
		return U2x4( z,w, x,y );
    }

	U2 tree_add( U2 u2, U2 n_t )
	{
		// x érték, y mama, z kicsiegyenlõ, w nagyobb
		this[n_t].null().x = u2;
		if( !n_t )
			return 1;

		U2 i = 0;
		while( i < n_t )
		{
			if( this[i].x == u2 )
				return n_t;

			if( this[i].x < u2 )
			{
				if( !this[i].z )
				{
					this[i].z = n_t;
					this[n_t].y = i;
					n_t++;
					return n_t;
				}
				i = this[i].z;
				continue;
			}
			if( !this[i].w )
			{
				this[i].w = n_t;
				this[n_t].y = i;
				n_t++;
				return n_t;
			}
			i = this[i].w;
		}
		return n_t;
	}
	U2 tree_fnd( U2 u2, U4 n_t )
	{
		// figyelem/WARNING
		// visza térési érték, az, ameddig eljutott a fában
		// kell még egy ellenörzés, hogy tényleg azonos e a két elem
		U2 i = 0;
		while( i < n_t )
		{
			if( this[i].x == u2 )
				return i;

			if( this[i].x < u2 )
			{
				if( !this[i].z )
					return i;

				i = this[i].z;
				continue;
			}
			if( !this[i].w )
				return i;

			i = this[i].w;
		}
		return n_t;
	}


};

class U4x2 {
public:
	union {
		struct { U4 x,y; };
		struct { U4 i,n; };
		struct { gpeALFu4 a4; U4 n4; };
		struct { U1x4 aU1x4[2]; };
		struct { U2 xl,xh,prt,srf; };
		struct { gpeALF var; };
		struct { U8 u8; };
		struct { U1* apSTR[gpeU4x2nSTR]; };
		struct { double d8; };
	};

    U4x2(){};
    U4x2( U4 _x, U4 _y = 0 ) { x = _x; y = _y; }
    U4x2( const I8x2& an );
    U4x2( const I4x2& i42 );
	U4x2( const SDL_Surface& srf ) { x = srf.w; y = srf.h; }
	U4x2& operator ++() { ++x;++y; return *this; }
	U4x2& operator --() { --x;--y; return *this; }

    U4x2& operator = ( const SDL_Surface& srf ) {
		x = srf.w;
		y = srf.h;
		return *this;
	}
	U4x2& operator = ( const I8x2& an );
	// cnt = fract * U4x2(1, w);
    U4 strALF4N( U1* pBUFF ) {
		U4 n = gpfALF2STR( pBUFF, a4 );
		return n+sprintf( (char*)(pBUFF+n), "%d", n4 );
    }
	U1* pSTRalf4n( U1* pBUFF ) { return pBUFF + gpfALF2STR( pBUFF, a4 ); }
    U1* strVAR( U1* pBUFF ) {
		gpfALF2STR( pBUFF, var );
		return pBUFF;
    }
	U1* str( U1* pBUFF, const char* pSP = ", " ) {
		sprintf( (char*)pBUFF, "%d%s%d%s", x,pSP,y,pSP );
		return pBUFF;
    }
	U4x2& cnt2fract(U4 w, U8 cnt) {
		U1 lg = log2(w * w);
		w = 1<<(lg/2);
		U8 X = w * w;
		cnt %= X;
		null();

		while( cnt )
		{
			w >>= 1;
			switch(cnt&3)
			{
				case 1:
					x += w;
					y += w;
					break;
				case 2:
					x += w;
					break;
				case 3:
					y += w;
					break;
			}
			cnt >>= 2;
		}
		return *this;
	}
	U4x2& null( void ) { gpmCLR; return *this; }
	U4x2& operator += ( const U4x2& b ) { x += b.x; y += b.y; return *this; }
	U4x2& operator -= ( const U4x2& b ) { x -= b.x; y -= b.y; return *this; }
	U4x2& operator &= ( const U4x2& u ) { x *= u.x; y *= u.y; return *this; }
	U4x2& operator /= ( const U4x2& u ) {
		if( u.x )
			x /= u.x;
		else
			x = 0xffffFFFF;

		if( u.y )
			y /= u.y;
		else
			y = 0xffffFFFF;
		return *this;
	}
	U4x2& operator %= ( const U4x2& u ) {
		if( u.x )
			x %= u.x;
		else
			x = 0;

		if( u.y )
			y %= u.y;
		else
			y = 0;
		return *this;
	}
	U4x2& operator += ( U4 u ) { x += u; y += u; return *this; }
	U4x2& operator += ( I4 i )		{ x += i; y += i; return *this; }
	U4x2& operator += ( float f )	{ x += f; y += f; return *this; }
	U4x2& operator -= ( U4 u ) 		{ x -= u; y -= u; return *this; }
	U4x2& operator -= ( I4 i ) 		{ x -= i; y -= i; return *this; }
	U4x2& operator -= ( float f ) 	{ x -= f; y -= f; return *this; }
	U4x2& operator = ( int i ) {
		if( !i )
			return null();

		x = y = i;
		return *this;
	}

	U4x2& operator = ( const I4x2& b );


	U4x2 operator + (const U4 b) const		{ return U4x2( x+b, y+b );		}
	U4x2 operator + (const U4x2 b) const	{ return U4x2( x+b.x, y+b.y );	}

	U4x2 operator - (const U4 b) const 		{ return U4x2( x-b, y-b );		}
	U4x2 operator - (const U4x2 b) const	{ return U4x2( x-b.x, y-b.y );	}

	bool operator != ( const U4x2& b ) const {
		if( x != b.x )
			return true;
		return y != b.y;
	}
	bool operator == ( const U4x2& b ) const { return !(*this!=b); }

	U8 operator * (const U4x2& b) const		{ return (U8)x*b.x + (U8)y*b.y;	}
	U4x2 operator & (const U4x2& b) const	{ return U4x2( x*b.x, y*b.y );	}
	U4x2 operator / (const U4x2& b) const	{ return U4x2( b.x ? x/b.x : 0xffffffff, b.y ? y/b.y : 0xffffffff ); }
	U4x2 operator % (const U4x2& b) const { return U4x2( b.x ? x%b.x : 0, b.y ? y%b.y : 0 ); }
	U4x2& operator *= ( U4 i ) {
		if( !i )
			return null();
		if( i == 1 )
			return *this;

		x*=i;
		y*=i;

		return *this;
	}
	U4x2& operator /= ( U4 i ) {
		if( i == 1 )
			return *this;

		if( !i )
		{
			x = y = 0xffffffff;
			return *this;
		}

		x/=i;
		y/=i;

		return *this;
	}
	U4x2& operator %= ( U4 i ) {
		if( i == 1 )
			return null();

		if( !i )
			return *this;


		x%=i;
		y%=i;

		return *this;
	}

	U8 sum() const 		{ return (!this?0:(U8)x+y); }
	U8 area() const 	{ return (!this?0:(U8)x*y); }
	U8 are_sum() const 	{ return area()+sum(); }
	U8 qlen() const 	{ return (!this?0:(U8)x*x+(U8)y*y); }
	U4 mn() const 		{ return (!this?0:(x<y?x:y)); }
	U4 mx() const 		{ return (!this?0:(x>y?x:y)); }

	U4x2 abs( void ) const { return U4x2( x<0?-x:x, y<0?-y:y ); }
	U4x2& mx( U4x2 b ) {
		if( x < b.x )
			x = b.x;
		if( y < b.y )
			y = b.y;
		return *this;
	}

	U4x2& mn( const U4x2& b ) {
		if( x > b.x )
			x = b.x;
		if( y > b.y )
			y = b.y;
		return *this;
	}
	U4x2& mx( I4x2 b );
	U4x2& mn( I4x2 b );

	U4 average( U4 n ) {
		// vigyázz ez sorrendezi az értékeket
		if( !this || n < 1 )
			return 0;

		if( n < 2 )
			return this[0].y;
		else if( n < 3 )
			return (this[1].y+this[0].y)/2;


		I8 avgr = 0;
		for( U4 j = 0; j < n; j++ )
			avgr += this[j].y;

		return avgr / n;
	}
	U4 median( U4 n, U4x2* p_tree, bool b_inc = false ) {
		// b_inc == true - incrementált növekvő sorban leszenk
		// b_inc == false - dekrementáslt csökkenő sorban leszenk (nem definiálod akkor ez, azaz csökenő )
		if( !this || n < 1 )
			return 0;

		if( n < 2 )
			return this->y;

		U4	i, j, l, r;
		U4x2 x;

		r = n;
		while( r >= 1 ) {
			// az öszes elem számát "r" elosztom 2-tővel kerekítés nélkül
			// ezzel tudom, hogy a soron következő szint meddig csökkenhet "l" lesz a küszöb
			l = ldiv( r, 2 ).quot;
			while( r > 0 ) {
				// a következő elemet berakom az x-be
				x = this[r-1];

				i = r;
				if ( i*2 <= n ) {
					// i mutatja majd azt a helyet ahonva az x et be akarnám rakni
					while( i*2 <= n ) {
						j = i*2;
						// azt jelenti, hogy az i nek van ága
						if( j+1 <= n )
						if( p_tree[j+1].y < p_tree[j].y )
							j++; // azt jelenti, hogy két ága is volt, és a magasabb indexün kissebb volt az érték

						if( x.y > p_tree[j].y ) {
							// azt jelenti hogy az x nagyobb volt mint az ág ezért lejebb rakom a tartalmát
							p_tree[i] = p_tree[j];
							// és következő ciklusban az ágról akarom folytatni
							i = j;
						} else {
							// azt jelenti, nincs ennél magasabb szám az ágakon
							break;
						}
					}//while
					p_tree[i] = x;
				} else {
					p_tree[r] = x;
				}

				// r-et csökkentem jöhet a következő elem
				r--;
			}
		}
		l = ldiv( r, 2).quot;
		r = n;
		i = 1;
		while ( r >= 1 ) {
			x = p_tree[r];
			p_tree[r] = p_tree[1];
			r--;
			l = 1;
			while ( l <= r ) {
				i = l*2;
				if ( i <= r ) {
					if( i+1 <= r )
					if( p_tree[i+1].y < p_tree[i].y )
						i++;

					if( x.y > p_tree[i].y ) {
						p_tree[l] = p_tree[i];
					} else {
						p_tree[l] = x;
						break;
					}
				} else {
					p_tree[l] = x;
					break;
				}
				l = i;
			}//while

		}

		if( b_inc ) {
			for( U4 i = 0; i < n; i++ )
				this[i] = p_tree[n-i];
			if( n < 3 )
				return average( n );
			return this[n/2].y;
		}

		gpmMcpyOF( this, p_tree+1, n );
		if( n < 3 )
			return average( n );

		return this[n/2].y;
	}
};



class U4x4 {
public:
    union {
        struct { U4 x,y,z,w; };
        struct { U4x2 a4x2[2]; };
        struct { U4 aXYZW[4]; };
        struct { U2 aU2[8]; };
        struct { U1x4 aRGBAx4[4]; };
    };

    U4x4(){};
    U4x4& null( void ) { gpmCLR; return *this; }
    U4x4( U4 _x, U4 _y = 0, U4 _z = 0, U4 _w = 0 ) { x = _x; y = _y; z = _z; w = _w; }
	U4x4( const I4x2 _xy, I4x2* p_zw = NULL );
    U4x4( const U4x2 _xy, U4 _z = 0, U4 _w = 0 ) {
        a4x2[0] = _xy;
        z = _z;
        w = _w;
    }
    U4x4( U4x2& _xy, U4x2* p_zw = NULL ) {
        a4x2[0] = _xy;
        a4x2[1] = p_zw ? *p_zw : _xy;
    }
    U4x4( U4x2& _xy, U4x2& _zw ) {
        a4x2[0] = _xy;
        a4x2[1] = _zw;
    }
    U4x4& operator = ( U4 b ) { x = y = z = w = b; return *this; }
    U4x4& operator = ( I4 b ) { x = y = z = w = abs(b); return *this; }
    U4x4& operator = ( const U4x2& b ) { a4x2[1] = a4x2[0] = b; return *this; }
    U4x4& operator = ( const I4x2& b );


    U4x4& str2date( U1* p_str, U1* p_end, U1** pp_str = NULL );
    char* pSTR( char* pBUFF, const char* pSP = ", "  ) {
		sprintf( pBUFF, "%d%s%d%s%d%s%d%s", x,pSP,y,pSP,z,pSP,w,pSP );
		return pBUFF;
    }
	U4x4 xyz_HSTaqi( U4 n, U4 a ) {
		/// HISTI listában meg lehet keresni
		/// hol jön össze elég DARAB
		U4x4 aqu(0), aqui(0), *pH = this;
		for( int i = 0, j = 0; i < n; i++, j=0 ) {
			aqu += pH[i];
			if( aqu.x < a ) {
				aqui.x = i;
				j++;
			}
			if( aqu.y < a ) {
				aqui.y = i;
				j++;
			}
			if( aqu.z < a ) {
				aqui.z = i;
				j++;
			}
			if( !j )
				break;
		}
		return aqui;
	}
    bool operator != ( const U4x4& b ) const {
		if( a4x2[0] != b.a4x2[0] )
			return true;
		return a4x2[1] != b.a4x2[1];
	}
	bool operator == ( const U4x4& b ) const {
		return !(*this!=b);
	}


	U4x4& operator += ( const U4x4& b ) {
		a4x2[0] += b.a4x2[0];
		a4x2[1] += b.a4x2[1];
		return *this;
	}
	U4x4& operator -= ( const U4x4& b ) {
		a4x2[0] -= b.a4x2[0];
		a4x2[1] -= b.a4x2[1];
		return *this;
	}

	U4x4& operator += ( const U4x2& b ) {
		a4x2[0] += b;
		a4x2[1] += b;
		return *this;
	}
	U4x4& operator -= ( const U4x2& b ) {
		a4x2[0] -= b;
		a4x2[1] -= b;
		return *this;
	}

	U4x4& operator += ( U4 u ) {
		a4x2[0] += u;
		a4x2[1] += u;
		return *this;
	}
	U4x4& operator += ( I4 i ) {
		a4x2[0] += i;
		a4x2[1] += i;
		return *this;
	}
	U4x4& operator += ( float f ) {
		a4x2[0] += f;
		a4x2[1] += f;
		return *this;
	}

	U4x4& operator -= ( U4 u ) {
		a4x2[0] -= u;
		a4x2[1] -= u;
		return *this;
	}
	U4x4& operator -= ( I4 i ) {
		a4x2[0] -= i;
		a4x2[1] -= i;
		return *this;
	}
	U4x4& operator -= ( float f ) {
		a4x2[0] -= f;
		a4x2[1] -= f;
		return *this;
	}

    U4x4 zwxy( void )  const {
		return U4x4( z,w, x,y );
    }
    U4x4& xyz_( U4x4 b ) {
		a4x2[0] = b.a4x2[0];
		z = b.z;
		return *this;
	}
	U4x4 xyz0( void ) const {
		U4x4 o = *this;
		o.w = 0;
		return o;
	}

	U4 tree_add( U4 u4, U4 n_t ) {
		// x érték, y mama, z kicsiegyenlõ, w nagyobb
		this[n_t].null().x = u4;
		if( !n_t )
			return 1;

		U4 i = 0;
		while( i < n_t )
		{
			if( this[i].x == u4 )
				return n_t;

			if( this[i].x < u4 )
			{
				if( !this[i].z )
				{
					this[i].z = n_t;
					this[n_t].y = i;
					n_t++;
					return n_t;
				}
				i = this[i].z;
				continue;
			}
			if( !this[i].w )
			{
				this[i].w = n_t;
				this[n_t].y = i;
				n_t++;
				return n_t;
			}
			i = this[i].w;
		}
		return n_t;
	}
	U4 tree_fnd( U4 u4, U4 n_t ) {
		// figyelem/WARNING
		// visza térési érték, az, ameddig eljutott a fában
		// kell még egy ellenörzés, hogy tényleg azonos e a két elem
		U4 i = 0;
		while( i < n_t )
		{
			if( this[i].x == u4 )
				return i;

			if( this[i].x < u4 )
			{
				if( !this[i].z )
					return i;

				i = this[i].z;
				continue;
			}
			if( !this[i].w )
				return i;

			i = this[i].w;
		}
		return n_t;
	}

	U8 sum( void ) const { return (I8)x + y + z + w; }
	U8 volume( void ) const { return x*y*z*w; }

	U4	dctADD( const void* pV, U4& m, U4x4& w );
	U4  dctFND( U1* p_src, U4x4& w );

	U4	dctADDn( void* pBIN, U4& m, U4x4& w );
	U4  dctFNDn( void* pBIN, U4x4& w );

	U4 coreC() const { return x>>4; }
	gpeCsz coreCsz() const { return (gpeCsz)(x&0xf); }
	U4 coreARY( U4 sz=1 ) {
		if( y )
			return sz*y;
		return sz*(y = a4x2[1].area());
	}
};

class U8x4 {
public:
    union
    {
        struct { U8 x,y,z,w; };
        struct { U8 aXYZW[4]; };
        struct { gpeALF labe; U8 mom, up, nx; };
    };
    U8x4(){};
    U8x4( U8 _x, U8 _y = 0, U8 _z = 0, U8 _w = 0 ) { x = _x; y = _y; z = _z; w = _w; }
    U8x4& operator = ( U8 b ) {
		gpmCLR;
        x = b;
        return *this;
    }
    U8x4& operator = ( I8 b ) {
		gpmCLR;
        x = abs(b);
        return *this;
    }
    U8x4& null( void ) {
		gpmCLR;
		return *this;
	}
    U8 tree_add( U8 u8, U8 n_t ) {
		// x érték, y mama, z kicsiegyenlõ, w nagyobb
		this[n_t].null().x = u8;
		if( !n_t )
			return 1;

		U8 i = 0;
		while( i < n_t )
		{
			if( this[i].x == u8 )
				return n_t;

			if( this[i].x < u8 )
			{
				if( !this[i].z )
				{
					this[i].z = n_t;
					this[n_t].y = i;
					return n_t+1;
				}
				i = this[i].z;
				continue;
			}
			if( !this[i].w )
			{
				this[i].w = n_t;
				this[n_t].y = i;
				return n_t+1;
			}
			i = this[i].w;
		}
		return n_t;
	}
	U8 tree_fnd( U8 u8, U8 n_t ) {
		// figyelem/WARNING
		// visza térési érték, az, ameddig eljutott a fában
		// kell még egy ellenörzés, hogy tényleg azonos e a két elem
		U8 i = 0;
		while( i < n_t )
		{
			if( this[i].x == u8 )
				return i;

			if( this[i].x < u8 )
			{
				if( !this[i].z )
					return i;

				i = this[i].z;
				continue;
			}
			if( !this[i].w )
				return i;

			i = this[i].w;
		}
		return n_t;
	}
	U8 tree_fndHARD( U8 u8, U8 n_t ) {
		// figyelem/WARNING
		// visza térési érték, az, ameddig eljutott a fában
		// kell még egy ellenörzés, hogy tényleg azonos e a két elem

		// szigorú
		U8 i = 0;
		while( i < n_t )
		{
			if( this[i].x == u8 )
				return i;

			if( this[i].x < u8 )
			{
				if( !this[i].z )
					break;

				i = this[i].z;
				continue;
			}
			if( !this[i].w )
				break;

			i = this[i].w;
		}
		return n_t;
	}
};
typedef enum gpeDRCos:U4 {
		gpeDRCos_POSx,	gpeDRCos_POSy, 	gpeDRCos_POSz,
		gpeDRCos_ABCa,	gpeDRCos_ABCb, 	gpeDRCos_ABCc,
		gpeDRCos_L1,	gpeDRCos_L2,	gpeDRCos_L3,
		gpeDRCos_L4,	gpeDRCos_L5,	gpeDRCos_L6,
		gpeDRCos_posx, 	gpeDRCos_posy, 	gpeDRCos_posz,
		gpeDRCos_abcA, 	gpeDRCos_abcB, 	gpeDRCos_abcC,
		gpeDRCos_l1,	gpeDRCos_l2,	gpeDRCos_l3,
		gpeDRCos_l4,	gpeDRCos_l5,	gpeDRCos_l6,
		gpeDRCos_GRPx,	gpeDRCos_GRPy, 	gpeDRCos_GRPz,
		gpeDRCos_27,	gpeDRCos_28,	gpeDRCos_29,
		gpeDRCos_30,	gpeDRCos_31,	gpeDRCos_32,
		gpeDRCos_33,	gpeDRCos_34,	gpeDRCos_35,
		gpeDRCos_prgA,	gpeDRCos_prgB, 	gpeDRCos_prgC, gpeDRCos_prgD,
		gpeDRCos_drpX,	gpeDRCos_drpY, 	gpeDRCos_drpZ, gpeDRCos_drpUP, gpeDRCos_drpT, gpeDRCos_drpW,

		gpeDRCos_NONS,
	} gpeDRCos;
typedef enum gpeZS:U4 {
	gpeZS_null, // = 0,
	gpeZS_iDIR = MAKE_ID( 'i', 'D', 'I', 'R'	),
	gpeZS_oDIR = MAKE_ID( 'o', 'D', 'I', 'R'	),
	gpeZS_tDIR = MAKE_ID( 't', 'D', 'I', 'R'	),

	gpeZS_BILL = MAKE_ID( 'B', 'I', 'L', 'L'	),
	gpeZS_JOHN = MAKE_ID( 'J', 'O', 'H', 'N'	),

	gpeZS_DIR0 = MAKE_ID( 'D', 'I', 'R',  0 	),
	gpeZS_iPOS = MAKE_ID( 'i', 'P', 'O', 'S'	),
	gpeZS_oPOS = MAKE_ID( 'o', 'P', 'O', 'S'	),
	gpeZS_tPOS = MAKE_ID( 't', 'P', 'O', 'S'	),
	gpeZS_POS0 = MAKE_ID( 'P', 'O', 'S',  0		),
	// alias
	gpeZS_XYZ0 = MAKE_ID( 'X', 'Y', 'Z',  0		),
	gpeZS_ABC0 = MAKE_ID( 'A', 'B', 'C',  0 	),

	gpeZS_iA13 = MAKE_ID( 'i', 'A', '1', '3'	),
	gpeZS_oA13 = MAKE_ID( 'o', 'A', '1', '3'	),

	gpeZS_AXI1 = MAKE_ID( 'A', 'X', 'I', '1'	),
	gpeZS_AXI2 = MAKE_ID( 'A', 'X', 'I', '2'	),
	gpeZS_AXI3 = MAKE_ID( 'A', 'X', 'I', '3'	),

	gpeZS_iA46 = MAKE_ID( 'i', 'A', '4', '5'	),
	gpeZS_oA46 = MAKE_ID( 'o', 'A', '4', '6'	),

	gpeZS_AXI4 = MAKE_ID( 'A', 'X', 'I', '4'	),
	gpeZS_AKI5 = MAKE_ID( 'A', 'X', 'I', '5'	),
	gpeZS_AXI6 = MAKE_ID( 'A', 'X', 'I', '6'	),

	gpeZS_AXIS = MAKE_ID( 'A', 'X', 'I', 'S'	),
	gpeZS_LINK = MAKE_ID( 'L', 'I', 'N', 'K'	),


	gpeZS_idir = MAKE_ID( 'i', 'd', 'i', 'r'	),
	gpeZS_odir = MAKE_ID( 'o', 'd', 'i', 'r'	),
	gpeZS_tdir = MAKE_ID( 't', 'd', 'i', 'r'	),
	gpeZS_dir0 = MAKE_ID( 'd', 'i', 'r',  0		),
	gpeZS_ipos = MAKE_ID( 'i', 'p', 'o', 's'	),
	gpeZS_opos = MAKE_ID( 'o', 'p', 'o', 's'	),
	gpeZS_tpos = MAKE_ID( 't', 'p', 'o', 's'	),
	gpeZS_pos0 = MAKE_ID( 'p', 'o', 's',  0		),
	// alias
	gpeZS_xyz0 = MAKE_ID( 'x', 'y', 'z',  0		),
	gpeZS_abc0 = MAKE_ID( 'a', 'b', 'c',  0 	),

	gpeZS_ia13 = MAKE_ID( 'i', 'a', '1', '3'	),
	gpeZS_ia46 = MAKE_ID( 'i', 'a', '4', '5'	),
	gpeZS_oa13 = MAKE_ID( 'o', 'a', '1', '3'	),
	gpeZS_oa46 = MAKE_ID( 'o', 'a', '4', '6'	),

	gpeZS_CTRL = MAKE_ID( 'C', 'T', 'R', 'L'	),
	gpeZS_iCTR = MAKE_ID( 'i', 'C', 'T', 'R'	),
	gpeZS_oCTR = MAKE_ID( 'o', 'C', 'T', 'R'	),

} gpeZS;
class gpcVAR{
public:
	gpeALF alf;
	gpeCsz typ;
	gpcVAR( gpeALF a, gpeCsz t ){
		alf = a;
		typ = t;
	}

};
class I4x2 {
public:
	union{
		struct{ int x,y; };
		struct{ int i,n; };
		struct{ div_t qr; };
		struct{ U8 u8; };
		struct{	gpeOPtyp	typ;
				U1			wh;
				gpeOPid		pst, pre;
				I4 iCD;
		};
	};

    I4x2(){};
    I4x2( I4 _x, I4 _y = 0 ) { x = _x; y = _y; }
	I4x2( U4x2 b ) { *this = b; }
    I4x2( const SDL_Surface& srf ) { x = srf.w; y = srf.h; }
    I4x2& operator = ( const SDL_Surface& srf ) {
		x = srf.w;
		y = srf.h;
		return *this;
	}
    I4x2( U8x2 b );
    I4x2( I8x2 b );
    I4x2( int* pI ) {
		if( !pI )
		{
			gpmCLR;
			return;
		}
        x = pI[0];
        y = pI[1];
    }
    size_t str( char* pBUFF, const char* pSeP = ", ", const char* pENT = ""  ) {
		return sprintf( pBUFF, "%d%s%d%s%s", x, pSeP, y, pSeP, pENT );
    }
    char* pSTR( char* pBUFF, const char* pSeP = ", ", const char* pENT = ""  ) {
		str( pBUFF, pSeP, pENT );
		return pBUFF;
    }
	I4x2& snail( I4 i ) {
		if( !i )
			return null();

		bool bS = i < 0; if(bS) i *= -1;

		I4	sq = floor(sqrt(i)),
			i0 = sq*sq,

			sq2 = sq+1,
			i1 = sq2*sq,

			i2 =sq2*sq2;
		// i=6 sq=2 i0=4 sq2=3 i1=6 i2=9


		if( i0&1 ) {
			*this = sq2/2;	//i=3,2,1 xy=2/2=1
			// páratlan
            if( i<i1) {
				x -= i1-i;	//i=1 x=1-(2-1)= 0
				y -= i2-i1;	//i=1 y=1-(4-2)=-1
            } else {
				//x=x			//i=3;2 x=1
				y -= i2-i;		//i=3;2 y=1-(4-3)=0
            }
		} else {
			// páros
			*this = sq/2;	//i=5;4 xy=2/2=1
			if( i<i1) {
				x -= i-i0;	//i=5;4 x=1-(5-4)= 0
				//y=y		//i=5;4 y=1
            } else {
				x -= i1-i0;	//i=6 x=1-(6-4)=1-2=-1
				y -= i-i1;	//i=6 x=1-(6-6) = 1
            }
		}

		// páratlan i0
		// i=1 sq=1 i0=1 sq2=2 i1=2 i2=4 x= 0 y=-1
		// i=2 sq=1 i0=1 sq2=2 i1=2 i2=4 x= 1 y=-1
		// i=3 sq=1 i0=1 sq2=2 i1=2 i2=4 x= 1 y= 0
		// páros i0
		// i=4 sq=2 i0=4 sq2=3 i1=6 i2=9 x= 1 y= 1
		// i=5 sq=2 i0=4 sq2=3 i1=6 i2=9 x= 0 y= 1
		// i=6 sq=2 i0=4 sq2=3 i1=6 i2=9 x=-1 y= 1
		if( bS )
			*this *= -1;

		return *this;
	}
	// cnt = fract * U42(1, w);
	I4x2& cnt2fract(U4 w, U8 cnt) {
		U1 lg = log2(w * w);
		w = 1<<(lg/2);
		U8 X = w * w;
		cnt %= X;
		null();

		while( cnt )
		{
			w >>= 1;
			switch(cnt&3)
			{
				case 1:
					x += w;
					y += w;
					break;
				case 2:
					x += w;
					break;
				case 3:
					y += w;
					break;
			}
			cnt >>= 2;
		}
		return *this;
	}

	I8x2 operator * ( I8 b ) const;
	I8x2 operator * ( int b ) const; ///? { return *this * (I8)b };
	I8x2 operator * ( float b ) const; // { return *this * (I8)b };
	I8x2 operator * ( double b ) const; // { return *this * (I8)b };
	I4x2 operator / ( int b ) const {
		if( !b )
			return I4x2( 0x7fffffff, 0x7fffffff );
		if( b == 1 )
			return *this;
		if( b == -1 )
			return I4x2( -x, -y );

		return I4x2( x/b, y/b );
	}
	I4x2 operator % ( int b ) const {
		if( ( b > 0 ? b : -b ) < 2 )
			return 0;

		return I4x2( x%b, y%b );
	}


	I8 operator * (const I4x2& b) const { return (I8)x*b.x + (I8)y * b.y; }


	I4x2 operator & (U8 b) const {
		if( !b )
			return 0;

		I4x2 a(0);
		a.u8 = u8&b;
		return a;
	}

	I4x2 operator >> (int b) const {
		if(!b)
			return *this;

		return I4x2( x>>b,  y>>b );
	}

	I8x2 operator & (const I4x2& b ) const;

	I4x2 operator % (const I4x2& b) const { return I4x2( b.x ? x/b.x : 0,  b.y ? y/b.y : 0 ); }
	I4x2 operator / (const I4x2& b) const { return I4x2( b.x ? x/b.x : 0x7fffffff ,  b.y ? y/b.y : 0x7fffffff ); }
	I4x2 operator / (const U4x2& b) const { return I4x2( b.x ? x/b.x : 0x7fffffff,  b.y ? y/b.y : 0x7fffffff ); }
	I4x2 operator / (const I8x2& b) const;
	I8x2 operator + (const I8x2& b) const;
	I8x2 operator - (const I8x2& b) const;

    I4x2& operator ++() { ++x; ++y; return *this; }
    I4x2& operator --() { --x; --y; return *this; }

	I4x2 operator + ( int b ) const { return I4x2( x+b,  y+b ); }
	I4x2 operator + (const I4x2& b) const { return I4x2( x+b.x,  y+b.y ); }
	I4x2 operator + (const U4x2& b) const { return I4x2( x+b.x,  y+b.y ); }

	I4x2 operator - ( int b ) const { return I4x2( x-b,  y-b ); }

	I4x2 operator - (const I4x2& b) const { return I4x2( x-b.x,  y-b.y ); }
	I4x2 operator - (const U4x2& b) const { return I4x2( x-b.x,  y-b.y ); }
	bool operator != ( const I4x2& b ) const {
		if( x != b.x )
			return true;
		return y != b.y;
	}
	bool operator == ( const I4x2& b ) const {
		return !(*this!=b);
	}

	I4x2& operator *= ( I4 i ) {
		if( !i )
			return null();
		if( i == 1 )
			return *this;

		x*=i;
		y*=i;

		return *this;
	}
	I4x2& operator /= ( I4 i ) {
		if( i == 1 )
			return *this;

		if( !i )
		{
			x = y = 0x7fffffff;
			return *this;
		}

		x/=i;
		y/=i;

		return *this;
	}
	I4x2& operator %= ( I4 i ) {
		if( i == 1 )
			return null();

		if( !i )
			return *this;


		x%=i;
		y%=i;

		return *this;
	}
	I4x2& operator <<= ( I4 i ) { x<<=i; y<<=i; return *this; }
	I4x2& operator >>= ( I4 i ) { x>>=i; y>>=i; return *this; }
	I4x2& operator &= ( I4 i ) { x&=i; y&=i; return *this; }
	I4x2& operator |= ( I4 i ) { x|=i; y|=i; return *this; }


	I4x2& null( void ) { gpmCLR; return *this; }


	I4x2& operator += ( const U4x2& u ) {
		I4x2 b = u;
		x += b.x;
		y += b.y;
		return *this;
	}
	I4x2& operator -= ( const U4x2& u ) {
		I4x2 b = u;
		x -= b.x;
		y -= b.y;
		return *this;
	}


	I4x2& operator &= ( const I4x2& i ) {
		x *= i.x;
		y *= i.y;
		return *this;
	}
	I4x2& operator /= ( const I4x2& i ) {
		if( i.x )
			x /= i.x;
		else
			x = 0x7fffFFFF;

		if( i.y )
			y /= i.y;
		else
			y = 0x7fffFFFF;
		return *this;
	}
	I4x2& operator %= ( const I4x2& i ) {
		if( i.x )
			x %= i.x;
		else
			x = 0;

		if( i.y )
			y %= i.y;
		else
			y = 0;
		return *this;
	}


	I4x2& operator += ( const I4x2& b ) {
		x += b.x;
		y += b.y;
		return *this;
	}
	I4x2& operator -= ( const I4x2& b ) {
		x -= b.x;
		y -= b.y;
		return *this;
	}
	I4x2& operator += ( U4 u ) {
		if( u > 0x7fffFFFF )
			u = 0x7fffFFFF;
		x += u;
		y += u;
		return *this;
	}
	I4x2& operator += ( I4 i ) {
		if( i < 0 )
			i *= -1;
		x += i;
		y += i;
		return *this;
	}
	I4x2& operator += ( float f ) {
		x += f;
		y += f;
		return *this;
	}

	I4x2& operator -= ( U4 u ) {
		if( u > 0x7fffFFFF )
			u = 0x7fffFFFF;
		x -= u;
		y -= u;
		return *this;
	}
	I4x2& operator -= ( I4 i ) {
		if( i < 0 )
			i *= -1;
		x -= i;
		y -= i;
		return *this;
	}
	I4x2& operator -= ( float f ) {
		x -= f;
		y -= f;
		return *this;
	}
	I4x2& operator = ( int i ) {
		if( !i )
			return null();

		x = y = i;
		return *this;
	}
	U4 strALF4N( U1* pBUFF ) {
		U4 nn = gpfALF2STR( pBUFF, x );
		return nn+sprintf( (char*)(pBUFF+nn), "%d", y );
    }
	U1* pSTRalf4n( U1* pBUFF ) {
		//sprintf( (char*)(pBUFF+gpfALF2STR( pBUFF, x )), "%d", y );
		return pBUFF + strALF4N( pBUFF );
    }

	I4x2& operator = ( const U4x2& b ) {
		x = b.x > 0x7fffFFFF ? 0x7fffFFFF : b.x;
		y = b.y > 0x7fffFFFF ? 0x7fffFFFF : b.y;
		return *this;
	}

	I4x2& operator = ( const U8x2& b );
	I4x2& operator = ( const I8x2& b );

	I4x2 abs( void ) const { return I4x2( x<0?-x:x, y<0?-y:y ); }

	I8 sum( void ) const { return (I8)x+y; }
	I8 area( void ) const { return x*y; }
	U8 are_sum( void ) const { return abs().area()+abs().sum(); }

	I4x4 xxxy() const;

	I8 qlen (void ) const { return x*x + y*y; }

	I4 mn() { return x < y ? x:y; }
	I4 mx() { return x > y ? x:y; }


	I4x2 MX( const I4x2 b ) const { return  this ? I4x2( x>b.x?x:b.x, y>b.y?y:b.y ) : I4x2(0); }
	I4x2 MN( const I4x2 b ) const { return  this ? I4x2( x<b.x?x:b.x, y<b.y?y:b.y ) : I4x2(0); }
	I4x2& mx( const I4x2 b ) {
		if( x < b.x )
			x = b.x;
		if( y < b.y )
			y = b.y;
		return *this;
	}

	I4x2& mn( const I4x2 b ) {
		if( x > b.x )
			x = b.x;
		if( y > b.y )
			y = b.y;
		return *this;
	}

	I4x2& swp() {
		int t = x;
		x = y;
		y = t;
		return *this;
	}
	I4x2& YdivQR( int b ) {
		if( b<2 ? true : b < -1 )
		{
			x = y;
			y = !b ? ~0 : 0;
			return *this;
		}
		/// ennek az a lényege
		/// az ALU egyébként is kiszámolja a hányadost
		/// és a maradékot egy lépésben
		qr = div(y,b);
		return *this;
	}
	I4x2& YdivRQ( int b ) {
		return YdivQR(b).swp();
	}
	I4x2& XdivQR( int b ) {
		y = x;
		return YdivQR(b);
	}
	I4x2& XdivRQ( int b ) {
		y = x;
		return YdivQR(b).swp();
	}

	I4 average( I4 n ) {
		// vigyázz ez sorrendezi az értékeket
		if( !this || n < 1 )
			return 0;

		if( n < 2 )
			return this[0].y;
		else if( n < 3 )
			return (this[1].y+this[0].y)/2;


		I8 avgr = 0;
		for( U4 j = 0; j < n; j++ )
			avgr += this[j].y;

		return avgr / n;
	}
	I4 median( U4 n, I4x2* p_tree, bool b_inc = false ) {
		// b_inc == true - incrementált növekvő sorban leszenk
		// b_inc == false - dekrementáslt csökkenő sorban leszenk (nem definiálod akkor ez, azaz csökenő )
		if( !this || n < 1 )
			return 0;

		if( n < 2 )
			return this->y;

		U4	i, j, l, r;
		I4x2 x;

		r = n;
		while( r >= 1 ) {
			// az öszes elem számát "r" elosztom 2-tővel kerekítés nélkül
			// ezzel tudom, hogy a soron következő szint meddig csökkenhet "l" lesz a küszöb
			l = ldiv( r, 2 ).quot;
			while( r > 0 ) {
				// a következő elemet berakom az x-be
				x = this[r-1];

				i = r;
				if ( i*2 <= n ) {
					// i mutatja majd azt a helyet ahonva az x et be akarnám rakni
					while( i*2 <= n ) {
						j = i*2;
						// azt jelenti, hogy az i nek van ága
						if( j+1 <= n )
						if( p_tree[j+1].y < p_tree[j].y )
							j++; // azt jelenti, hogy két ága is volt, és a magasabb indexün kissebb volt az érték

						if( x.y > p_tree[j].y ) {
							// azt jelenti hogy az x nagyobb volt mint az ág ezért lejebb rakom a tartalmát
							p_tree[i] = p_tree[j];
							// és következő ciklusban az ágról akarom folytatni
							i = j;
						} else {
							// azt jelenti, nincs ennél magasabb szám az ágakon
							break;
						}
					}//while
					p_tree[i] = x;
				} else {
					p_tree[r] = x;
				}

				// r-et csökkentem jöhet a következő elem
				r--;
			}
		}
		l = ldiv( r, 2).quot;
		r = n;
		i = 1;
		while ( r >= 1 ) {
			x = p_tree[r];
			p_tree[r] = p_tree[1];
			r--;
			l = 1;
			while ( l <= r ) {
				i = l*2;
				if ( i <= r ) {
					if( i+1 <= r )
					if( p_tree[i+1].y < p_tree[i].y )
						i++;

					if( x.y > p_tree[i].y ) {
						p_tree[l] = p_tree[i];
					} else {
						p_tree[l] = x;
						break;
					}
				} else {
					p_tree[l] = x;
					break;
				}
				l = i;
			}//while

		}

		if( b_inc ) {
			for( U4 i = 0; i < n; i++ )
				this[i] = p_tree[n-i];
			if( n < 3 )
				return average( n );
			return this[n/2].y;
		}

		gpmMcpyOF( this, p_tree+1, n );
		if( n < 3 )
			return average( n );

		return this[n/2].y;
	}

	I4x2 left()		{ return I4x2(-y,x);	}
	I4x2 right()	{ return I4x2(y,-x);	}
	I4x2 SCRlft()	{ return right();		}
	I4x2 SCRrig()	{ return left();		}
};



class I4x4 {
public:
	union {
		struct { I4 x,y,z,w; };
		struct { I4 r,g,b,a; };
		struct { I4 i,n,zz,ww; };
		struct {	gpeOPid pre, pst;	// 0, 1
					gpeCsz	cID;		// 2,
					U1		d0;			// 3,
					U4		op;			// 4,
					I4		aOB[2];		// 8, 12; //0x10
        };
		struct { I4 A,B,C,D; };
		struct { I4 aXYZW[4]; };
		struct { gpeZS aZS[4]; };
		struct { I4x2 a4x2[2]; };

		struct { SDL_Rect xyWH; };

        struct { U4x2 au4x2[2]; };

	};
	U1x4 opw() { return (U1x4)op; }
	I4x4(){};
    I4x4( I4 _x, I4 _y = 0, I4 _z = 0, I4 _w = 0 ) { x = _x; y = _y; z = _z; w = _w; }
	I4x4( I4x2 _xy, I4x2 _zw ) {
        a4x2[0] = _xy;
        a4x2[1] = _zw;
    }
    I4x4( U4x2 _xy, U4x2 _zw ) {
        a4x2[0] = _xy;
        a4x2[1] = _zw;
    }
    I4x4( SDL_Rect rect ) { xyWH = rect; }
    //I4x4( const I4x4 b ) { x = b.x; y = b.y; z = b.z; w = b.w; }
    I4x4( const I8x4 b );
	//I4x4( const I8x4& b );
	I4x4( const F4 f4 );
	I4x4& operator = ( const I4x4* pB ) {
		gpmMcpy( this, pB, sizeof(*this) );
		return *this;
	}
	I4x4& operator = ( const U1x4 b ) { x = b.x; y = b.y; z = b.z; w = b.w; return *this; }
    I4x4& operator = ( const F4 f4 );
    I4x4& operator = ( I4x2 b ) { a4x2[1] = a4x2[0] = b; return *this; }
    I4x4& operator ++() { ++a4x2[0]; ++a4x2[1]; return *this; }
	I4x4& operator --() { --a4x2[0]; --a4x2[1]; return *this; }
    size_t str( char* pBUFF, const char* pSeP = ", ", const char* pENT = ""   ) {
		return sprintf( pBUFF, "%d%s%d%s%d%s%d%s%s", x,pSeP,y,pSeP,z,pSeP,w,pSeP, pENT );
    }
	char* pSTR( char* pBUFF, const char* pSeP = ", ", const char* pENT = ""   ) {
		str( pBUFF, pSeP, pENT );
		//sprintf( pBUFF, "%d%s%d%s%d%s%d%s%s", x,pSeP,y,pSeP,z,pSeP,w,pSeP, pENT );
		return pBUFF;
    }


	I4x4 mx_mx( I4 r ) {
		I8 m = abs0().mx().x;
		if( !m )
			return 0;
		m = abs0().mx().x;
		return I4x4( ((I8)x*(I8)r)/m, ((I8)y*(I8)r)/m, ((I8)z*(I8)r)/m, w );
	}

	I4x4 lim_mx( I4 r ) {
		I8 l = abs0().mx().x;
		if( l < r )
			return *this;

		return mx_mx(r);
	}
	I4x4 lim_mx( const I4x4& b, I4 r ) const {
		I4x4 v4 = b-xyz0();
		I8 l = v4.abs0().mx().x;
		if( l < r )
			return b;
		v4 = v4.lim_mx(r);
		v4.w = b.w;
		return v4+xyz0();
	}


	I4x4 len_mx( I4 r ) {
		I8 l = abs0().mx().x;
		if( !l )
			return 0;
		l = sqrt(l);
		return I4x4( ((I8)x*(I8)r)/l, ((I8)y*(I8)r)/l, ((I8)z*(I8)r)/l, w );
	}

	I4x4 len_xyz( I4 r ) {
		I8 l = qlen_xyz();
		if( !l )
			return 0;
		l = sqrt(l);
		return I4x4( ((I8)x*(I8)r)/l, ((I8)y*(I8)r)/l, ((I8)z*(I8)r)/l, w );
	}
	I4x4 lim_xyz( I4 r ) {
		I8 l = qlen_xyz();
		if( l < r*r )
			return *this;

		return len_xyz(r);
	}
	I4x4 lim_xyz( const I4x4& b, I4 r ) const {
		I4x4 d = b-xyz0();
		I8 l = d.qlen_xyz();
		if( l < r*r )
			return b;
		d = d.len_xyz(r);
		d.w = b.w;
		return xyz0()+d;
	}

    bool operator != ( const I4x4& b ) const {
		if( a4x2[0] != b.a4x2[0] )
			return true;
		return a4x2[1] != b.a4x2[1];
	}
	bool operator == ( const I4x4& b ) const { return !(*this!=b); }



	I4x4& operator += ( const I4x4& b ) {
		a4x2[0] += b.a4x2[0];
		a4x2[1] += b.a4x2[1];
		return *this;
	}
	I4x4& operator -= ( const I4x4& b ) {
		a4x2[0] -= b.a4x2[0];
		a4x2[1] -= b.a4x2[1];
		return *this;
	}

	I4x4& operator &= ( const I4x4& b ) {
		a4x2[0] &= b.a4x2[0];
		a4x2[1] &= b.a4x2[1];
		return *this;
	}
	I4x4& operator %= ( const I4x4& b ) {
		a4x2[0] %= b.a4x2[0];
		a4x2[1] %= b.a4x2[1];
		return *this;
	}
	I4x4& operator /= ( const I4x4& b ) {
		a4x2[0] /= b.a4x2[0];
		a4x2[1] /= b.a4x2[1];
		return *this;
	}




	I4x4& operator += ( const I4x2& b ) {
		a4x2[0] += b;
		a4x2[1] += b;
		return *this;
	}
	I4x4& operator -= ( const I4x2& b ) {
		a4x2[0] -= b;
		a4x2[1] -= b;
		return *this;
	}

	I4x4& operator &= ( const I4x2& b ) {
		a4x2[0] &= b;
		a4x2[1] &= b;
		return *this;
	}
	I4x4& operator %= ( const I4x2& b ) {
		a4x2[0] %= b;
		a4x2[1] %= b;
		return *this;
	}
	I4x4& operator /= ( const I4x2& b ) {
		a4x2[0] /= b;
		a4x2[1] /= b;
		return *this;
	}




	I4x4& operator += ( U4 u ) {
		a4x2[0] += u;
		a4x2[1] += u;
		return *this;
	}
	I4x4& operator += ( I4 i ) {
		a4x2[0] += i;
		a4x2[1] += i;
		return *this;
	}
	I4x4& operator += ( float f ) {
		a4x2[0] += f;
		a4x2[1] += f;
		return *this;
	}

	I4x4& operator -= ( U4 u ) {
		a4x2[0] -= u;
		a4x2[1] -= u;
		return *this;
	}
	I4x4& operator -= ( I4 i ) {
		a4x2[0] -= i;
		a4x2[1] -= i;
		return *this;
	}
	I4x4& operator -= ( float f ) {
		a4x2[0] -= f;
		a4x2[1] -= f;
		return *this;
	}


	I4x4& operator *= ( U4 u ) {
		a4x2[0] *= u;
		a4x2[1] *= u;
		return *this;
	}
	I4x4& operator *= ( I4 i ) {
		a4x2[0] *= i;
		a4x2[1] *= i;
		return *this;
	}
	I4x4& operator *= ( float f ) {
		a4x2[0] *= (I4)f;
		a4x2[1] *= (I4)f;
		return *this;
	}

	I4x4& operator %= ( U4 u ) {
		a4x2[0] %= u;
		a4x2[1] %= u;
		return *this;
	}
	I4x4& operator %= ( I4 i ) {
		a4x2[0] %= i;
		a4x2[1] %= i;
		return *this;
	}
	I4x4& operator %= ( float f ) {
		a4x2[0] %= (I4)f;
		a4x2[1] %= (I4)f;
		return *this;
	}

	I4x4& operator /= ( U4 u ) {
		a4x2[0] /= u;
		a4x2[1] /= u;
		return *this;
	}
	I4x4& operator /= ( I4 i ) {
		a4x2[0] /= i;
		a4x2[1] /= i;
		return *this;
	}
	I4x4& operator /= ( float f ) {
		a4x2[0] /= (I4)f;
		a4x2[1] /= (I4)f;
		return *this;
	}

	I4x4& operator <<= ( I4 i ) {
		a4x2[0] <<= i;
		a4x2[1] <<= i;
		return *this;
	}
	I4x4& operator >>= ( I4 i ) {
		a4x2[0] >>= i;
		a4x2[1] >>= i;
		return *this;
	}

	I4x4& operator &= ( I4 i ) {
		a4x2[0] &= i;
		a4x2[1] &= i;
		return *this;
	}
	I4x4& operator |= ( I4 i ) {
		a4x2[0] |= i;
		a4x2[1] |= i;
		return *this;
	}

	I4x4 operator + (const I4x4& b) const { return I4x4( x+b.x, y+b.y, z+b.z, w+b.w ); }
	I4x4 operator - (const I4x4& b) const { return I4x4( x-b.x, y-b.y, z-b.z, w-b.w ); }

	I8 operator * (const I4x4& b) const { return a4x2[0]*b.a4x2[0] + a4x2[1]*b.a4x2[1]; }
	U8 qlen (void ) const { return (*this) * (*this); }
	I8x4 operator & (const I4x4& b ) const;
	I4x4 operator % (const I4x4& b) const { return I4x4( a4x2[0]%b.a4x2[0], a4x2[1]%b.a4x2[1] ); }
	I4x4 operator / (const I4x4& b) const { return I4x4( a4x2[0]/b.a4x2[0], a4x2[1]/b.a4x2[1] ); }
	I4x4 operator / (const U4x4& b) const { return I4x4( a4x2[0]/b.a4x2[0], a4x2[1]/b.a4x2[1] ); }



	I8x4 operator * ( int b ) const;
	I8x4 operator * ( I8 b ) const;
	I8x4 operator * ( float b ) const;
	I8x4 operator * ( double b ) const;

	I4x4 operator / ( int b ) const { return I4x4( a4x2[0]/b, a4x2[1]/b ); }
	I4x4 operator % ( int b ) const { return I4x4( a4x2[0]%b, a4x2[1]%b ); }

	U8 operator * (const I4x2& b) const { return a4x2[0]*b + a4x2[1]*b; }

	I8x4 operator & (const I4x2& b) const;
	I8x4 operator & (const U4x2& b) const;

	I4x2 lurdWH(){ return a4x2[1]-a4x2[0];};
	I4x4 lurd( void ) {
		// left - up - right - down
		I4x4  lurd = *this;
		if( x > z )
		{
			lurd.x = z;
			lurd.z = x;
		}
		if( y > w )
		{
			lurd.y = w;
			lurd.w = y;
		}
		return lurd;
	}

	I4x4& null( void ) { gpmCLR; return *this; }
	I8 sum( void ) const { return (I8)x + y + z + w; }
	I8 volume( void ) const { return x*y*z*w; }

	I4x4 abs( void ) const { return I4x4( x<0 ? -x: x, y<0 ? -y: y, z<0 ? -z: z, w<0 ? -w: w ); }
	I4x4 abs0( void ) const { return I4x4( x<0 ? -x: x, y<0 ? -y: y, z<0 ? -z: z ); }
	I4x4 zwxy( void ) const { return I4x4( z, w, x, y ); }
	I4x4& xyz_( I4x4 b ) { a4x2[0] = b.a4x2[0]; z = b.z; return *this; }
	I4x4 xy00( void ) const { return I4x4(x,y); }
	I4x4 xyz0( void ) const { return I4x4(x,y,z); }
	/// 13
	I4x4& swpYX() {
		float t = x;
		x = y;
		y = t;
		return *this;
	}
	/// 25
	I4x4& swpZX() {
		float t = x;
		x = z;
		z = t;
		return *this;
	}
	/// 36
	I4x4& swpZY() {
		float t = y;
		y = z;
		z = t;
		return *this;
	}

	/// 49
	I4x4& swpWX() {
		float t = x;
		x = w;
		w = t;
		return *this;
	}

	/// 5A
	I4x4& swpWY() {
		float t = y;
		y = w;
		w = t;
		return *this;
	}
	/// 6C
	I4x4& swpWZ() {
		float t = z;
		z = w;
		w = t;
		return *this;
	}

	I4x4 srt3() {
		if( qlen_xyz() == 0.0 )
			return 0;

		I4x4 s = *this;
		s.w = 1;
		if(s.y>s.x)
		{
			s.swpYX();
			s.w += 2;
		}
		if(s.z>s.x)
		{
			s.swpZX();
			s.w += 4;
		}
		if(s.z>s.y)
		{
			s.swpZY();
			s.w += 8;
		}

		return s;
	}
	I4x4& swp3( U1 s ) {
		if(s&1)
		{
			if( s&0x08 )
				swpZY();
			if( s&0x04 )
				swpZX();
			if( s&0x02 )
				swpYX();
		}

		return *this;
	}
	I4x4* index( U4 n_i ) {
		if( !this )
		{
			I4x4* p_this = new I4x4[n_i+1];
			if( !p_this )
				return NULL;

			return p_this->index( n_i );
		}

		for( I4 i = 0; i < n_i; i++ )
		{
			this[i].x = i;
			this[i].y = 0;
		}
		return this;
	}
	U4 histi( U1* p_src, U4 n_src ) {
		for( U4 i = 0; i < n_src; i++ )
			this[p_src[i]].y++;

		U2 mx = 0x100;
		for( U2 i = 0; i < 0x100; i++ )
		{
			if( !this[i].y )
				continue;
			mx = i;
		}
		return mx;
	}
	I4x4 xyz_HSTaqi( U4 n, U4 a ) {
		/// HISTI listában meg lehet keresni
		/// hol jön össze elég DARAB
		I4x4 aqu(0), aqui(0), *pH = this;
		for( int i = 0, j = 0; i < n; i++, j=0 ) {
			aqu += pH[i];
			if( aqu.x < a ) {
				aqui.x = i;
				j++;
			}
			if( aqu.y < a ) {
				aqui.y = i;
				j++;
			}
			if( aqu.z < a ) {
				aqui.z = i;
				j++;
			}
			if( !j )
				break;
		}
		return aqui;
	}

	I4 median( U4 n, bool b_inc = false ) {
		// b_inc == true - incrementált növekvő sorban leszenk
		// b_inc == false - dekrementáslt csökkenő sorban leszenk (nem definiálod akkor ez, azaz csökenő )
		if( !this || n < 1 )
			return 0;

		if( n < 2 )
			return this->y;

		U4	i, j, l, r;
		I4x4	*p_tree = new I4x4[n+1],
				x;

		r = n;
		while( r >= 1 )
		{
			// az öszes elem számát "r" elosztom 2-tővel kerekítés nélkül
			// ezzel tudom, hogy a soron következő szint meddig csökkenhet "l" lesz a küszöb
			l = ldiv( r, 2 ).quot;
			while( r > 0 )
			{
				// a következő elemet berakom az x-be
				x = this[r-1];

				i = r;
				if ( i*2 <= n )
				{
					// i mutatja majd azt a helyet ahonva az x et be akarnám rakni
					while( i*2 <= n )
					{
						j = i*2;
						// azt jelenti, hogy az i nek van ága
						if( j+1 <= n )
						if( p_tree[j+1].y < p_tree[j].y )
							j++; // azt jelenti, hogy két ága is volt, és a magasabb indexün kissebb volt az érték

						if( x.y > p_tree[j].y )
						{
							// azt jelenti hogy az x nagyobb volt mint az ág ezért lejebb rakom a tartalmát
							p_tree[i] = p_tree[j];
							// és következő ciklusban az ágról akarom folytatni
							i = j;
						} else {
							// azt jelenti, nincs ennél magasabb szám az ágakon
							break;
						}
					}//while
					p_tree[i] = x;
				} else {
					p_tree[r] = x;
				}

				// r-et csökkentem jöhet a következő elem
				r--;
			}
		}
		l = ldiv( r, 2).quot;
		r = n;
		i = 1;
		while ( r >= 1 )
		{
			x = p_tree[r];
			p_tree[r] = p_tree[1];
			r--;
			l = 1;
			while ( l <= r )
			{
				i = l*2;
				if ( i <= r )
				{
					if ( i+1 <= r )
						if ( p_tree[i+1].y < p_tree[i].y )
							i++;

					if ( x.y > p_tree[i].y )
					{
						p_tree[l] = p_tree[i];
					} else {
						p_tree[l] = x;
						break;
					}
				} else {
					p_tree[l] = x;
					break;
				}
				l = i;
			}//while

		}

		if( b_inc )
		{
			for( U4 i = 0; i < n; i++ )
			{
				this[i] = p_tree[n-i];
			}
			gpmDELary( p_tree );
			if( n < 3 )
				return average( n );

			return this[n/2].y;
		}

		I4x4* p_src = p_tree+1;
		for( U4 i = 0; i < n; i++ )
		{
			this[i] = p_src[i];
		}
		gpmDELary( p_tree );
		if( n < 3 )
			return average( n );

		return this[n/2].y;
	}
	I4 average( U4 n ) {
		// vigyázz ez sorrendezi az értékeket
		if( !this || n < 1 )
			return 0;

		if( n < 2 )
		{
			return this[0].y;
		}
		else if( n < 3 )
		{
			return (this[1].y+this[0].y)/2;
		}

		I8 avgr = 0;
		for( U4 j = 0; j < n; j++ )
			avgr += this[j].y;

		return avgr / n;
	}

    I8 qlen_xyz() const { return (I8)x*x + (I8)y*y + (I8)z*z; }
	I4x4 TSrBALL( I4x4 T, I8 r );
	I4x4 TSrBOX( I4x4 T, I8 r );
	I4x4 ABC2xyz( I4x4 txyz, const I4x4& iABC ) const;
	I4x4 mxR( I8 r );
	I4x2 mx() {
		I4x2 o(x,0);
		if( o.x < y )
		{
			o.x = y;
			o.y = 1;
		}
		if( o.x < z )
		{
			o.x = z;
			o.y = 2;
		}
		if( o.x < w )
		{
			o.x = w;
			o.y = 3;
		}
		return o;
	}
	I4x2 mn() {
		I4x2 o(x,0);
		if( o.x > y )
		{
			o.x = y;
			o.y = 1;
		}
		if( o.x > z )
		{
			o.x = z;
			o.y = 2;
		}
		if( o.x > w )
		{
			o.x = w;
			o.y = 3;
		}
		return o;
	}
	I4x4& swpZWXY() {
		I4x2 t = a4x2[1];
		a4x2[1] = a4x2[0];
		a4x2[0] = t;
		return *this;
	}
	//I4x4 chkABC( const I4x4& b, float dim, float mm = mmX(100) ) const;
	I4x4 mmABC( const I4x4& b, float alf, float mm = mmX(100) ) const;

	double dropRAD(const I4x4 T, const I4x4 up, I8 d, I8 ti, I8 tn ) const;
	I4x4 drop( const I4x4 T, const I4x4 up, I8 d, I8 ti, I8 tn ) const;


};


class I8x2 {
public:
	union{
		struct { I8 x,y; };
		struct { gpeALF	alf; I8 num; };
		struct { gpeALF	a, b; };
		struct { I4x2 i4x2[2]; };
		struct { U4x2 u4x2[2]; };
		struct { U8 ux,uy; };
		struct { double dx,dy; };
	};

    I8x2(){};
    I8x2( gpeALF a, I4x2 b ){
    	alf = a;
    	i4x2[1] = b;
    };
    I8x2( I4 _x, I4 _y = 0 ) { x = _x; y = _y; }
    I8x2( U8 _x, U8 _y = 0 ) { x = _x; y = _y; }
    I8x2( I8 _x, I8 _y = 0 ) { x = _x; y = _y; }
    I8x2( gpeALF a, I8 n = 0 ) { alf = a; num = n; }
    I8x2( const U4x2 b ) { x = b.x; y = b.y; }
	I8x2( const I4x2 b ) { x = b.x; y = b.y; }
	I8x2( U8* pB ) {
		gpmMcpyOF( this, pB, 1 );
	}
    I8x2& operator = ( const U1* pS );
    I8x2& operator = ( const char* pS );

    I8x2& operator += ( const I8x2& b ) { x += b.x; y += b.y; return *this; }
    I8x2& operator -= ( const I8x2& b ) { x -= b.x; y -= b.y; return *this; }

    I8x2& operator += ( const I4x2& b ) { x += b.x; y += b.y; return *this; }
    I8x2& operator -= ( const I4x2& b ) { x -= b.x; y -= b.y; return *this; }


	// cnt = fract * U42(1, w);
	I8x2& cnt2fract( U4 w, U8 cnt ) {
		U1 lg = log2(w * w);
		w = 1<<(lg/2);
		U8 X = w * w;
		cnt %= X;
		null();

		while( cnt ) {
			w >>= 1;
			switch(cnt&3) {
				case 1:
					x += w;
					y += w;
					break;
				case 2:
					x += w;
					break;
				case 3:
					y += w;
					break;
			}
			cnt >>= 2;
		}
		return *this;
	}

	I8 operator * (const I4x2& b) const { return x*(I8)b.x + y*(I8)b.y; }
	I8 operator * (const I8x2& b) const { return x*b.x + y * b.y; }
	I8x2 operator & (const I4x2& b) const { return I8x2( x*b.x,  y*b.y ); }
	I8x2 operator & (const I8x2& b) const { return I8x2( x*b.x,  y*b.y ); }
	I8x2 operator & (const U8* pB ) const { return I8x2( x*pB[0],  y*pB[1] ); }


	I8x2 operator % (const U4x2& b) const {
		return I8x2( b.x ? x%b.x : 0x7FFFffffFFFFffff ,  b.y ? y%b.y : 0x7FFFffffFFFFffff );
	}
	I8x2 operator % (const I4x2& b) const {
		return I8x2( b.x ? x%b.x : 0x7FFFffffFFFFffff ,  b.y ? y%b.y : 0x7FFFffffFFFFffff );
	}
	I8x2 operator % (const I8x2& b) const {
		return I8x2( b.x ? x%b.x : 0x7FFFffffFFFFffff ,  b.y ? y%b.y : 0x7FFFffffFFFFffff );
	}

	I8x2 operator / (const U4x2& b) const {
		return I8x2( b.x ? x/b.x : 0x7FFFffffFFFFffff,  b.y ? y/b.y : 0x7FFFffffFFFFffff );
	}
	I8x2 operator / (const I4x2& b) const {
		return I8x2( b.x ? x/b.x : 0x7FFFffffFFFFffff ,  b.y ? y/b.y : 0x7FFFffffFFFFffff );
	}
	I8x2 operator / (const I8x2& b) const {
		return I8x2( b.x ? x/b.x : 0x7FFFffffFFFFffff ,  b.y ? y/b.y : 0x7FFFffffFFFFffff );
	}

	I8x2 operator & (const U4x2& b) const { return I8x2( x*b.x,  y*b.y ); }
	I8x2 operator + (const I4x2& b) const { return I8x2( x+b.x,  y+b.y ); }
	I8x2 operator + (const I8x2& b) const { return I8x2( x+b.x,  y+b.y ); }

	I8x2 operator + (const U4x2& b) const { return I8x2( x+b.x,  y+b.y ); }
	I8x2 operator + (const U8* pB ) const { return I8x2( x+pB[0], y+pB[1] ); }

	I8x2 operator - (const I4x2& b) const { return I8x2( x-b.x,  y-b.y ); }
	I8x2 operator - (const I8x2& b) const { return I8x2( x-b.x,  y-b.y ); }

	I8x2 operator - (const U4x2& b) const { return I8x2( x-b.x,  y-b.y ); }
	I8x2 operator - (const U8* pB ) const { return I8x2( x-pB[0], y-pB[1]); }

	I8x2 operator * ( const U4 b ) const { return *this&U4x2(b,b); }
	I8x2 operator & ( const U4 b ) const { return I8x2( x&b, y&b ); }
	I8x2 operator | ( const U4 b ) const { return I8x2( x|b, y|b ); }
	I8x2 operator + ( const U4 b ) const { return *this+U4x2(b,b); }
	I8x2 operator - ( const U4 b ) const { return *this-U4x2(b,b); }
	I8x2 operator % ( const U4 b ) const { return *this%U4x2(b,b); }
	I8x2 operator / ( const U4 b ) const { return *this/U4x2(b,b); }

	I8x2 operator * ( const I4 b ) const { return *this&I4x2(b,b); }
	I8x2 operator & ( const I4 b ) const { return I8x2( x&b, y&b ); }
	I8x2 operator | ( const I4 b ) const { return I8x2( x|b, y|b ); }
	I8x2 operator + ( const I4 b ) const { return *this+I4x2(b,b); }
	I8x2 operator - ( const I4 b ) const { return *this-I4x2(b,b); }
	I8x2 operator % ( const I4 b ) const { return *this%I4x2(b,b); }
	I8x2 operator / ( const I4 b ) const { return *this/I4x2(b,b); }

	I8x2 operator * ( const U8 b ) const { return *this&I8x2(b,b); }
	I8x2 operator & ( const U8 b ) const { return I8x2( x&b, y&b ); }
	I8x2 operator | ( const U8 b ) const { return I8x2( x|b, y|b ); }
	I8x2 operator + ( const U8 b ) const { return *this+I8x2(b,b); }
	I8x2 operator - ( const U8 b ) const { return *this-I8x2(b,b); }
	I8x2 operator % ( const U8 b ) const { return *this%I8x2(b,b); }
	I8x2 operator / ( const U8 b ) const { return *this/I8x2(b,b); }

	I8x2 operator * ( const I8 b ) const { return *this&I8x2(b,b); }
	I8x2 operator & ( const I8 b ) const { return I8x2( x&b, y&b ); }
	I8x2 operator | ( const I8 b ) const { return I8x2( x|b, y|b ); }
	I8x2 operator + ( const I8 b ) const { return *this+I8x2(b,b); }
	I8x2 operator - ( const I8 b ) const { return *this-I8x2(b,b); }
	I8x2 operator % ( const I8 b ) const { return *this%I8x2(b,b); }
	I8x2 operator / ( const I8 b ) const { return *this/I8x2(b,b); }

	bool operator != ( const I4x2& b ) const {
		if( x != b.x )
			return true;
		return y != b.y;
	}
	bool operator != ( const I8x2& b ) const {
		if( x != b.x )
			return true;
		return y != b.y;
	}
	bool operator == ( const I4x2& b ) const { return !(*this!=b); }
	bool operator == ( const I8x2& b ) const { return !(*this!=b); }

	I8x2& operator *= ( I8 i ) {
		if( !i )
			return null();
		if( i == 1 )
			return *this;

		x*=i;
		y*=i;

		return *this;
	}
	I8x2& operator /= ( I8 i ) {
		if( i == 1 )
			return *this;

		if( !i )
		{
			x = y = 0x7FFFffffFFFFffff;
			return *this;
		}

		x/=i;
		y/=i;

		return *this;
	}
	I8x2& operator %= ( I8 i ) {
		if( i == 1 )
			return null();

		if( !i )
			return *this;


		x%=i;
		y%=i;

		return *this;
	}
	I8x2& null( void ) {
		gpmCLR;
		return *this;
	}

	I8 are_sum() const { return abs().area()+abs().sum(); }

	I8 sum() const	{ return !this ? 0 : x+y; }
	I8 area() const	{ return !this ? 0 : x*y; }
	I8 qlen() const	{ return !this ? 0 : (x*x + y*y); }
	I8 mn() const	{ return !this ? 0 : (x<y ? x:y); }
	I8 mx() const	{ return !this ? 0 : (x>y ? x:y); }

	I8x2 abs( void ) const { return I8x2( x<0?-x:x, y<0?-y:y ); }

	gpeTYP cdrMILLnum( const char* pS, U4 nS );
	gpeTYP cdrMILLalf( const char* pS, U4 nS );

	gpeCsz gpCszNUM( const char* pS, U4 nS );
	gpeCsz gpCszALF( const char* pS, U4 nS );
	//U8
	size_t ab2str( void* p_b, const void* p_p = NULL ) {
		if( !p_b )
			return 0;
		char	*p_buff = (char*)p_b,
				*p_post = (char*)p_p;
		if( !this ) {
			*p_buff = 0;
			return 0;
		}
		char* p_begin = p_buff;
		p_buff += gpfALF2STR( p_buff, a );
		if( p_post )
			p_buff += sprintf( (char*)p_buff, "%s", p_post );
		p_buff += gpfALF2STR( p_buff, b );
		return p_buff-p_begin;
	}
	size_t an2str( void* p_b, const U1* p_p = NULL, bool b_hex = false, bool b0x0 = false ) {
		if( !p_b )
			return 0;
		char	*p_buff = (char*)p_b,
				*p_post = (char*)p_p;
		if( !this ) {
			*p_buff = 0;
			return 0;
		}
		char *p_begin = p_buff;
		p_buff += gpfALF2STR( p_buff, alf );

		I8 nn = num;
		if( nn < 0)
		{
			nn = -nn;
			b_hex = b0x0 = true;
		}
		//_strupr( p_begin );
		const char* pPAT	= b_hex
					? ( b0x0	? "0x%0.8llx"
								: "%llx" )
					: "%lld";
		p_buff += sprintf( p_buff, pPAT, nn );
		if( p_post )
			p_buff += sprintf( p_buff, "%s", p_post );

		return p_buff-p_begin;
	}

	int alfFND( gpeALF af, int n );
	int alfN( gpeALF af, int n );
	int alfRIG( gpeALF af, int n, int r );
	int aALFvan( const gpeALF* aALF, int n, int nA );

	I8x2* alfCON( char* pOUT, int nAT ) {
		I8x2* pAT = this;
		if( !this )
			nAT = 0;

		int at = 0;
		for( ; at < nAT; at++ ) {
			switch(pAT[at].alf){
				case gpeALF_PLUS:
					sprintf( pOUT, "+%lld", pAT[at].num );
					break;
				case gpeALF_CM:
					sprintf( pOUT, ",%lld", pAT[at].num );
					break;
				case gpeALF_MRK:
					sprintf( pOUT, "\"%lld\r\n", pAT[at].num);
					break;
				default:
					pAT[at].an2str( pOUT );
					break;
			}
			std::cout << at <<"."<< pOUT << " ";
		}
		std::cout << "nAT:" << at << std::endl;
		return this;
	}
};


class I8x4 {
public:
    union {
        struct { I8 x,y,z,w; };
        struct { I8 A,B,C,D; };
        struct { I8 aXYZW[4]; };
        struct { I8x2 a8x2[2]; };
        struct { gpeALF labe; I8 mom, up, nx; };
        struct { I4x4 ai4x4[2]; };
        struct { U4x4 au4x4[2]; };
        struct { U4x4 af4x4[2]; };
        ///-----------------------------------------
		///
		///				-= piMASS cmplr =-
		///
		///-----------------------------------------
         /// src/mn/dct
        struct { I4 iS,nS,dctID,typ,px,py, clr,res; };
    };
	///-----------------------------------------
	///
	///				-= piMASS cmplr =-
	///
	///-----------------------------------------
    gpeCLR MNclr() { return (gpeCLR)((U1x4)clr).B; }
	gpeTYP MNtyp() { return (gpeTYP)typ; }


    I8x4(){};
    I8x4( I8 _x, I8 _y = 0, I8 _z = 0, I8 _w = 0 ) { x = _x; y = _y; z = _z; w = _w; }
    I8x4( const I8x2& xy, const I8x2& zw ) { a8x2[0] = xy; a8x2[1] = zw; }
    I8x4( const I4x4& b ){ x = b.x; y = b.y; z = b.z; w = b.w; };
    I8x4& operator = ( const I4x4& b ){ x = b.x; y = b.y; z = b.z; w = b.w; return *this; };

    I8x4& operator += ( const I8x4& b ) {
		a8x2[0] += b.a8x2[0];
		a8x2[1] += b.a8x2[1];
		return *this;
    }
    I8x4& operator -= ( const I8x4& b ) {
		a8x2[0] -= b.a8x2[0];
		a8x2[1] -= b.a8x2[1];
		return *this;
    }

	I8x4& operator += ( const I4x4& b ) {
		a8x2[0] += b.a4x2[0];
		a8x2[1] += b.a4x2[1];
		return *this;
    }
    I8x4& operator -= ( const I4x4& b ) {
		a8x2[0] -= b.a4x2[0];
		a8x2[1] -= b.a4x2[1];
		return *this;
    }

    I8x4& operator *= ( I8 b ) {
		if( !b )
		{
			gpmCLR;
			return *this;
		}
		if( b == 1 )
			return *this;
		x *= b;
        y *= b;
        z *= b;
        w *= b;
        return *this;
    }
    I8x4& operator /= ( I8 b ) {
		if( !b )
		{
			x = y = z = w = 0x7FFFffffFFFFffff;
			return *this;
		}
		if( b == 1 )
			return *this;
		x /= b;
        y /= b;
        z /= b;
        w /= b;
        return *this;
    }
    I8x4& operator %= ( I8 b ) {
		if( !b )
		{
			x = y = z = w = 0x7FFFffffFFFFffff;
			return *this;
		}
		if( (b ? b : -b) == 1 )
		{
			gpmCLR;
			return *this;
		}
		x %= b;
        y %= b;
        z %= b;
        w %= b;
        return *this;
    }
    I8x4& operator = ( U8 b ) {
		gpmCLR;
        x = gpmMIN( b, 0x7FFFffffFFFFffff );
		return *this;
    }
    I8x4& operator = ( I8 b ) {
        gpmCLR;
        x = b;
		return *this;
    }
    I8x4& null( void ) {
		gpmCLR;
		return *this;
	}
	I8x4& AB( U1* pA, U1* pB, U1** ppA = NULL, U1** ppB = NULL );

	I8 qlen_xyz() { return x*x+y*y+z*z; }

	I8x4 operator * ( const U4 b ) const { return I8x4( a8x2[0]*b, a8x2[1]*b ); }
	I8x4 operator & ( const U4 b ) const { return I8x4( a8x2[0]&b, a8x2[1]&b ); }
	I8x4 operator | ( const U4 b ) const { return I8x4( a8x2[0]|b, a8x2[1]|b ); }
	I8x4 operator + ( const U4 b ) const { return I8x4( a8x2[0]+b, a8x2[1]+b ); }
	I8x4 operator - ( const U4 b ) const { return I8x4( a8x2[0]-b, a8x2[1]-b ); }
	I8x4 operator % ( const U4 b ) const { return I8x4( a8x2[0]%b, a8x2[1]%b ); }
	I8x4 operator / ( const U4 b ) const { return I8x4( a8x2[0]/b, a8x2[1]/b ); }

	I8x4 operator * ( const I4 b ) const { return I8x4( a8x2[0]*b, a8x2[1]*b ); }
	I8x4 operator & ( const I4 b ) const { return I8x4( a8x2[0]&b, a8x2[1]&b ); }
	I8x4 operator | ( const I4 b ) const { return I8x4( a8x2[0]|b, a8x2[1]|b ); }
	I8x4 operator + ( const I4 b ) const { return I8x4( a8x2[0]+b, a8x2[1]+b ); }
	I8x4 operator - ( const I4 b ) const { return I8x4( a8x2[0]-b, a8x2[1]-b ); }
	I8x4 operator % ( const I4 b ) const { return I8x4( a8x2[0]%b, a8x2[1]%b ); }
	I8x4 operator / ( const I4 b ) const { return I8x4( a8x2[0]/b, a8x2[1]/b ); }

	I8x4 operator * ( const U8 b ) const { return I8x4( a8x2[0]*b, a8x2[1]*b ); }
	I8x4 operator & ( const U8 b ) const { return I8x4( a8x2[0]&b, a8x2[1]&b ); }
	I8x4 operator | ( const U8 b ) const { return I8x4( a8x2[0]|b, a8x2[1]|b ); }
	I8x4 operator + ( const U8 b ) const { return I8x4( a8x2[0]+b, a8x2[1]+b ); }
	I8x4 operator - ( const U8 b ) const { return I8x4( a8x2[0]-b, a8x2[1]-b ); }
	I8x4 operator % ( const U8 b ) const { return I8x4( a8x2[0]%b, a8x2[1]%b ); }
	I8x4 operator / ( const U8 b ) const { return I8x4( a8x2[0]/b, a8x2[1]/b ); }

	I8x4 operator * ( const I8 b ) const { return I8x4( a8x2[0]*b, a8x2[1]*b ); }
	I8x4 operator & ( const I8 b ) const { return I8x4( a8x2[0]&b, a8x2[1]&b ); }
	I8x4 operator | ( const I8 b ) const { return I8x4( a8x2[0]|b, a8x2[1]|b ); }
	I8x4 operator + ( const I8 b ) const { return I8x4( a8x2[0]+b, a8x2[1]+b ); }
	I8x4 operator - ( const I8 b ) const { return I8x4( a8x2[0]-b, a8x2[1]-b ); }
	I8x4 operator % ( const I8 b ) const { return I8x4( a8x2[0]%b, a8x2[1]%b ); }
	I8x4 operator / ( const I8 b ) const { return I8x4( a8x2[0]/b, a8x2[1]/b ); }


	I8 operator * ( const I8x4& b ) const {
		return x*b.x + y*b.y + z*b.z + w*b.w;
	}
	I8x4 operator & ( const I8x4& b ) const {
		return I8x4( x*b.x, y*b.y, z*b.z, w*b.w );
	}
	I8x4 operator + ( const I8x4& b ) const {
		return I8x4( a8x2[0]+b.a8x2[0], a8x2[1]+b.a8x2[1] );
	}
	I8x4 operator - ( const I8x4& b ) const {
		return I8x4( a8x2[0]-b.a8x2[0], a8x2[1]-b.a8x2[1] );
	}
	I8x4 operator / ( const I8x4& b ) const {
		return I8x4( a8x2[0]/b.a8x2[0], a8x2[1]/b.a8x2[1] );
	}
	I8x4 operator % ( const I8x4& b ) const {
		return I8x4( a8x2[0]%b.a8x2[0], a8x2[1]%b.a8x2[1] );
	}

	I8x4 operator & ( const I4x2& b ) const {
		return I8x4( a8x2[0]&b, a8x2[1]&b );
	}
	I8x4 operator + ( const I4x2& b ) const {
		return I8x4( a8x2[0]+b, a8x2[1]+b );
	}
	I8x4 operator - ( const I4x2& b ) const {
		return I8x4( a8x2[0]-b, a8x2[1]-b );
	}
	I8x4 operator / ( const I4x2& b ) const {
		return I8x4( a8x2[0]/b, a8x2[1]/b );
	}
	I8x4 operator % ( const I4x2& b ) const {
		return I8x4( a8x2[0]%b, a8x2[1]%b );
	}



    I8 tree_add( gpeALF alf, I8 n_t ) {
		return tree_add( (I8)alf, n_t );
    }
    I8 tree_add( I8 i8, I8 n_t ) {
		// x érték, y mama, z kicsiegyenlõ, w nagyobb
		this[n_t].null().x = i8;
		if( !n_t )
			return 1;

		I8 i = 0;
		while( i < n_t )
		{
			if( this[i].x == i8 )
				return n_t;

			if( this[i].x < i8 )
			{
				if( !this[i].z )
				{
					this[i].z = n_t;
					this[n_t].y = i;
					n_t++;
					return n_t;
				}
				i = this[i].z;
				continue;
			}
			if( !this[i].w )
			{
				this[i].w = n_t;
				this[n_t].y = i;
				n_t++;
				return n_t;
			}
			i = this[i].w;
		}
		return n_t;
	}
	I8 tree_fnd( gpeALF alf, I8 n_t ) {
		return tree_fnd( (I8)alf, n_t );
    }
	I8 tree_fnd( I8 i8, I8 n_t ) {
		// figyelem/WARNING
		// visza térési érték, az, ameddig eljutott a fában
		// kell még egy ellenörzés, hogy tényleg azonos e a két elem
		I8 i = 0;
		while( i < n_t )
		{
			if( this[i].x == i8 )
				return i;

			if( this[i].x < i8 )
			{
				if( !this[i].z )
					return i;

				i = this[i].z;
				continue;
			}
			if( !this[i].w )
				return i;

			i = this[i].w;
		}
		return n_t;
	}
	I8x4 lurd( void ) {
		// left - up - right - down
		I8x4  lurd = *this;
		if( x > z )
		{
			lurd.x = z;
			lurd.z = x;
		}
		if( y > w )
		{
			lurd.y = w;
			lurd.w = y;
		}
		return lurd;
	}
	I8x4 abs( void ) const {
		return I8x4( x<0 ? -x: x, y<0 ? -y: y, z<0 ? -z: z, w<0 ? -w: w );
	}
	I8x4 abs0( void ) const {
		return I8x4( x<0 ? -x: x, y<0 ? -y: y, z<0 ? -z: z );
	}

	/*I8x4 ABS() const
	{
		return I8x4( a8x2[0].abs(), a8x2[1].abs() );
	}*/
	I8x2 mx() {
		I8x2 o(x,(I8)0);
		if( o.x < y )
		{
			o.x = y;
			o.y = 1;
		}
		if( o.x < z )
		{
			o.x = z;
			o.y = 2;
		}
		if( o.x < w )
		{
			o.x = w;
			o.y = 3;
		}
		return o;
	}
	I8x2 mn() {
		I8x2 o(x,(I8)0);
		if( o.x > y )
		{
			o.x = y;
			o.y = 1;
		}
		if( o.x > z )
		{
			o.x = z;
			o.y = 2;
		}
		if( o.x > w )
		{
			o.x = w;
			o.y = 3;
		}
		return o;
	}

};

class F2 {
public:
    float x,y;

    F2(){};

    F2( I4 _x, I4 _y = 0 ) { x = _x; y = _y; }
    F2( float _x, float _y = 0.0 ) { x = _x; y = _y; }
    F2( float* pF ) { gpmMcpyOF( this, pF, 1 ); }
    F2( I4x2 xy ) { x = xy.x; y = xy.y; }
    bool operator != ( const F2& b ) {
		if( x != b.x )
			return true;
		if( y != b.y )
			return true;

		return false;
    }
    bool operator == ( const F2& b ) { return !(*this!=b); }

    F2& operator = ( I4 b ) { x = b; y = 0; return *this; }

    F2& operator = ( const I4x2& xy ) { x = xy.x; y = xy.y; return *this; }

	F2& operator += ( const float b ) { x += b; y += b; return *this; }
    F2& operator -= ( const float b ) { x -= b; y -= b; return *this; }
    F2& operator *= ( const float b ) { x *= b; y *= b; return *this; }
    F2& operator %= ( const float b ) { x = frac(x/b); y = frac(y/b); return *this; }
    F2& operator /= ( const float b ) { x /= b; y /= b; return *this; }


    F2& operator += ( const F2& xy ) { x += xy.x; y += xy.y; return *this; }
    F2& operator -= ( const F2& xy ) { x -= xy.x; y -= xy.y; return *this; }
    F2& operator &= ( const F2& xy ) { x *= xy.x; y *= xy.y; return *this; }
    F2& operator %= ( const F2& xy ) { x = frac(x/xy.x); y = frac(y/xy.y); return *this; }
    F2& operator /= ( const F2& xy ) { x /= xy.x; y /= xy.y; return *this; }

	F2& sXY( const char* p_str, char** pp_str ); /// gpcGLobj.cpp
	F2& swpXY( const void* pV );

    double sum( void ) const { return x+y; }
    double qlen( void ) const { return x*x+y*y; }
    F2 norm( void ) const {
        float l = sqrt(qlen());
		return F2( x/l, y/l );
	}
	float dot( const F2& b ) const { return x*b.x + y*b.y; }

	F2 operator + ( float b ) const { return F2( x+b, y+b ); }
	F2 operator - ( float b ) const { return F2( x-b, y-b ); }
	F2 operator * ( float b ) const { return F2( x*b, y*b ); }
	F2 operator / ( float b ) const { return F2( x/b, y/b ); }

	F2 operator + ( const F2& b ) const { return F2( x+b.x, y+b.y ); }
	F2 operator - ( const F2& b ) const { return F2( x-b.x, y-b.y ); }
	float operator * ( const F2& b ) const { return x*b.x+y*b.y; }
	F2 operator & ( const F2& b ) const { return F2( x*b.x, y*b.y ); }
	F2 operator / ( const F2& b ) const { return F2( x/b.x, y/b.y ); }
	F2& add( F2 b, U4 n ) {
		for( U4 i = 0; i < n; i++ )
			this[i] += b;

		return *this;
	}
	F2& sub( F2 b, U4 n ) {
		for( U4 i = 0; i < n; i++ )
			this[i] -= b;

		return *this;
	}
	F2& mul( F2 b, U4 n ) {
		for( U4 i = 0; i < n; i++ )
			this[i] &= b;

		return *this;
	}
	F2& div( F2 b, U4 n ) {
		for( U4 i = 0; i < n; i++ )
			this[i] /= b;

		return *this;
	}

	F2 left() { return F2(-y,x); }
	F2 right() { return F2(y,-x); }
	F2 SCRlft() { return right(); }
	F2 SCRrig() { return left(); }
};

class F4 {
public:
	union {
		struct { float x,y,z,w; };
		struct { float A,B,C,D; };
		struct { float ry,rp,rr,r0; };
		struct { float aXYZW[4]; };
		struct { U4 uXYZW[4]; };
		struct { U1x4 aU1x4[4]; };
		struct { F2	aF2[2]; };
	};

    F4(){};
    F4( I4 _x, I4 _y=0, I4 _z=0, I4 _w=0 ) { x = _x; y = _y; z = _z; w = _w; }
    F4( float _x, float _y = 0.0, float _z = 0.0, float _w = 0.0 ) { x = _x; y = _y; z = _z; w = _w; }
    F4( double _x, double _y = 0.0, double _z = 0.0, double _w = 0.0 ) { x = _x; y = _y; z = _z; w = _w; }
    F4( float* pF ) { gpmMcpyOF( this, pF, 1 ); }
    F4( I4x2 xy, I4x2 zw = 0 ) { aF2[0] = xy; aF2[1] = zw; }
    F4( const I4x4 i4 ) { aF2[0] = i4.a4x2[0]; aF2[1] = i4.a4x2[1]; }
	F4& operator = ( const I4x4 i4 ) { aF2[0] = i4.a4x2[0]; aF2[1] = i4.a4x2[1]; return *this; }
    size_t str( char* pBUFF, const char* pSeP = ", ", const char* pENT = ""  ) {
		return sprintf( pBUFF, "%8.2f%s%8.2f%s%8.2f%s%8.2f%s%s", x, pSeP, y, pSeP, z, pSeP, w, pSeP, pENT );
    }
    char* pSTR( char* pBUFF, const char* pSeP = ", ", const char* pENT = ""  ) { str( pBUFF, pSeP, pENT ); return pBUFF; }


	F4 sqrt() const { return F4(sqrtf(x),sqrtf(y),sqrtf(z),sqrtf(w)); }
    F4 sqrt_xyz0() const { return F4(sqrtf(x),sqrtf(y),sqrtf(z)); }
    double sum( void ) const		{ return x+y+z+w; }
    double sum_xyz( void ) const	{ return x+y+z; }
    double qlen( void ) const		{ return x*x+y*y+z*z+w*w; }
    double qlen_xyz( void ) const	{ return x*x+y*y+z*z; }

    F4 N3( void ) const { /// NORMALIZE
        float l = sqrtf(qlen_xyz());
		return F4( x/l, y/l, z/l );
	}
	float dot_xyz( F4 b ) const { return x*b.x + y*b.y + z*b.z; }
    F4 X3( F4 b ) const {	/// CROSS PRODUCT
		return F4(
						y * b.z - z * b.y,
						z * b.x - x * b.z,
						x * b.y - y * b.x
				);
	}
	F4 xyz1() const { return F4(x,y,z,1.0);}

	F4  cos() const { return F4(  cosf(x), cosf(y), cosf(z), cosf(w)); };
	F4  sin() const { return F4(  sinf(x), sinf(y), sinf(z), sinf(w)); };
	F4 acos() const { return F4( acosf(x),acosf(y),acosf(z),acosf(w)); };
	F4 asin() const { return F4( asinf(x),asinf(y),asinf(z),asinf(w)); };
	F4 operator	+ ( const float b ) const { return F4( x+b, y+b, z+b, w+b ); }
	F4 operator	- ( const float b ) const { return F4( x-b, y-b, z-b, w-b ); }
	F4 operator	* ( const float b ) const { return F4( x*b, y*b, z*b, w*b ); }
	F4 operator	% ( const float b ) const { return F4( frac(x/b), frac(y/b), frac(z/b), frac(w/b) ); }
	F4 operator	/ ( const float b ) const { return F4( x/b, y/b, z/b, w/b ); }


	F4 operator	+ ( const F4& b ) const { return F4( x+b.x, y+b.y, z+b.z, w+b.w ); }
	F4 operator	- ( const F4& b ) const { return F4( x-b.x, y-b.y, z-b.z, w-b.w ); }
	float operator * ( const F4& b ) const { return x*b.x + y*b.y + z*b.z + w*b.w; }
	F4 operator	& ( const F4& b ) const { return F4( x*b.x, y*b.y, z*b.z, w*b.w ); }
	F4 operator	/ ( const F4& b ) const { return F4( x/b.x, y/b.y, z/b.z, w/b.w ); }

	bool operator != ( const F4& b ) {
		if( aF2[0] != b.aF2[0] )
			return true;
		if( aF2[1] != b.aF2[1] )
			return true;

		return false;
    }
    bool operator == ( const F4& b ) { return !(*this!=b); }

    F4& operator = ( const I4x2& b ) { aF2[0] = aF2[1] = b; return *this; }

    F4& operator += ( const float b ) { aF2[0] += b; aF2[1] += b; return *this; }
    F4& operator -= ( const float b ) { aF2[0] -= b; aF2[1] -= b; return *this; }
    F4& operator *= ( const float b ) { aF2[0] *= b; aF2[1] *= b; return *this; }
    F4& operator /= ( const float b ) { aF2[0] /= b; aF2[1] /= b; return *this; }

    F4& operator += ( const F2& b ) { aF2[0] += b; aF2[1] += b; return *this; }
    F4& operator -= ( const F2& b ) { aF2[0] -= b; aF2[1] -= b; return *this; }
    F4& operator &= ( const F2& b ) { aF2[0] &= b; aF2[1] &= b; return *this; }
    F4& operator /= ( const F2& b ) { aF2[0] /= b; aF2[1] /= b; return *this; }

	F4& operator += ( const F4& b ) { aF2[0] += b.aF2[0]; aF2[1] += b.aF2[1]; return *this; }
    F4& operator -= ( const F4& b ) { aF2[0] -= b.aF2[0]; aF2[1] -= b.aF2[1]; return *this; }
    F4& operator *= ( const F4& b ) {
		double a = aF2[0] * b.aF2[0];
		a += aF2[1] * b.aF2[1];

		x = y = z = w = a;
		return *this;
    }
    F4& operator &= ( const F4& b ) { aF2[0] &= b.aF2[0]; aF2[1] &= b.aF2[1]; return *this; }
    F4& operator %= ( const F4& b ) { aF2[0] %= b.aF2[0]; aF2[1] %= b.aF2[1]; return *this; }
    F4& operator /= ( const F4& b ) { aF2[0] /= b.aF2[0]; aF2[1] /= b.aF2[1]; return *this; }

	F4& operator *= ( const F4x4& b );
	F4 operator * ( const F4x4& b ) const;
	F4& operator /= ( const F4x4& b );
	F4 operator / ( const F4x4& b ) const;

	F4& xyz_( F4 b ) { aF2[0] = b.aF2[0]; z = b.z; return *this; }
	F4	xyz0() const { return F4(x,y,z); }
	F4&	abLOAD( char* pS, U4 nS, gpeALF alfS, U1** ppV, size_t* pN );
	F4&	sXYZW( const char* p_str, char** pp_str ); /// gpcGLobj.cpp
	F4&	swpXYZ0( const void* pV );

	F4 abs() const { return F4(x>=0.0?x:-x,y>=0.0?y:-y,z>=0.0?z:-z,w>=0.0?w:-w); }
	F4& add( F4 b, U4 n ) {
		for( U4 i = 0; i < n; i++ )
			this[i] += b;

		return *this;
	}
	F4& sub( F4 b, U4 n ) {
		for( U4 i = 0; i < n; i++ )
			this[i] -= b;

		return *this;
	}
	F4& mul( F4 b, U4 n ) {
		for( U4 i = 0; i < n; i++ )
			this[i] &= b;

		return *this;
	}
	F4& div( F4 b, U4 n ) {
		for( U4 i = 0; i < n; i++ )
			this[i] /= b;

		return *this;
	}

	/// 13
	F4& swpYX() {
		float t = x;
		x = y;
		y = t;
		return *this;
	}
	/// 25
	F4& swpZX() {
		float t = x;
		x = z;
		z = t;
		return *this;
	}
	/// 36
	F4& swpZY() {
		float t = y;
		y = z;
		z = t;
		return *this;
	}

	/// 49
	F4& swpWX() {
		float t = x;
		x = w;
		w = t;
		return *this;
	}

	/// 5A
	F4& swpWY() {
		float t = y;
		y = w;
		w = t;
		return *this;
	}
	/// 6C
	F4& swpWZ() {
		float t = z;
		z = w;
		w = t;
		return *this;
	}

	F4 srt3() {
		if( qlen_xyz() == 0.0 )
			return 0;

		F4 s = *this;
		s.w = 1;
		if(s.y>s.x)
		{
			s.swpYX();
			s.w += 2;
		}
		if(s.z>s.x)
		{
			s.swpZX();
			s.w += 4;
		}
		if(s.z>s.y)
		{
			s.swpZY();
			s.w += 8;
		}

		return s;
	}
	F4& swp3( U1 s ) {
		if(s&1)
		{
			if( s&0x08 )
				swpZY();
			if( s&0x04 )
				swpZX();
			if( s&0x02 )
				swpYX();
		}

		return *this;
	}

	F4& col4x4( const float* pF ) {
		x = pF[0];
		y = pF[4];
		z = pF[8];
		w = pF[12];
		return *this;
	}
	F4& col4x3( const float* pF ) {
		x = pF[0];
		y = pF[4];
		z = pF[8];
		w = 0.0;
		return *this;
	}

	F4 abs0( void ) const { return F4( x<0 ? -x: x, y<0 ? -y: y, z<0 ? -z: z ); }

	float mx() { float m = srt3().x; return m < w ? w : m; }
	float mn() { float m = srt3().z; return m > w ? w : m; }

	//I84 gr2cr_1m( void ) const
	F4& gr2cr( const I4x2 xy, double rw ) {

		double	rx = ::PI*((double(xy.x)/(rw*2)) + 0.25),
				ry = ::PI*((double(xy.y)/(rw*2)) + 0.25),
				ctgX = ::cos(rx)/::sin(rx),
				ctgY = ::cos(ry)/::sin(ry),
				ctgXY2 = ctgX*ctgX + ctgY*ctgY,
				r0 = rw / ::sqrt(1.0+ctgXY2);

		ctgX *= r0;
		ctgY *= r0;
		ctgXY2 = ctgX*ctgX + ctgY*ctgY;
		r0 = ::sqrt( rw*rw - ctgXY2 );

		x = ctgX;
		y = ctgY;
		z = r0;

		w = ::sqrt(qlen_xyz());
		return *this;
	}


};

//#define _11 x.x
//#define _12 x.y
//#define _13 x.z
//#define _14 x.w
//
//#define _21 y.x
//#define _22 y.y
//#define _23 y.z
//#define _24 y.w
//
//#define _31 z.x
//#define _32 z.y
//#define _33 z.z
//#define _34 z.w
//
//#define _41 t.x
//#define _42 t.y
//#define _43 t.z
//#define _44 t.w

class F4x4 {
public:
	F4 x,y,z,t;

    F4x4(){};
    F4x4( float a ){
		gpmCLR;
		if( a==0.0 )
		return; x.x = y.y = z.z = t.w = a;
	};
	F4x4( const F4 _x, const F4 _y, const F4 _z, const F4 _t ) {
		x = _x; y = _y; z = _z; t = _t;
	}
	F4x4( const F4* pA ) {
		gpmMcpy( this, pA, sizeof(*this) );
	}
	F4x4( float* pA ) {
		gpmMcpy( this, pA, sizeof(*this) );
	}

	size_t str( char* pBUFF, const char* pSeP = ", ", const char* pENT = ""  ) {
		char* pB = pBUFF;
		pB += x.str( pB, pSeP, pENT );
		pB += y.str( pB, pSeP, pENT );
		pB += z.str( pB, pSeP, pENT );
		pB += t.str( pB, pSeP, pENT );
		return pB-pBUFF;
    }
	F4x4& latR( F4 e, F4 c, F4 u );

	F4x4& operator = ( float a ) {
		gpmCLR;
		if( a == 0.0 ) return *this;
		x.x = y.y = z.z = t.w = a;
		return *this;
	};

	F4x4& operator = ( const F4& a ) {
		gpmCLR;
		x.x = a.x;
		y.y = a.y;
		z.z = a.z;
		t.w = a.w;
		return *this;
	};

	F4x4& operator = ( const F4* pA ) {
		gpmMcpy( this, pA, sizeof(*this) );
		return *this;
	};
	F4 qlen() const {
		return F4( x.qlen(), y.qlen(), z.qlen(), t.qlen() );
	}
	F4 qlen_xyz() const {
		return F4( x.qlen_xyz(), y.qlen_xyz(), z.qlen_xyz(), t.qlen_xyz() );
	}

	F4x4& operator += ( const F4& b ) {
		F4 l = qlen().sqrt();
		x *= (l.x+b.x)/l.x;
		y *= (l.y+b.y)/l.y;
		z *= (l.z+b.z)/l.z;
		t *= (l.w+b.w)/l.w;
		return *this;
	};
	F4x4& operator -= ( const F4& b ) {
		F4 l = qlen().sqrt();
		x *= (l.x-b.x)/l.x;
		y *= (l.y-b.y)/l.y;
		z *= (l.z-b.z)/l.z;
		t *= (l.w-b.w)/l.w;
		return *this;
	};
	F4x4& operator &= ( const F4& b ) {
		x *= b.x;
		y *= b.y;
		z *= b.z;
		t *= b.w;
		return *this;
	};
	F4x4& operator %= ( const F4& b ) {
		x %= b.x;
		y %= b.y;
		z %= b.z;
		t %= b.w;
		return *this;
	};
	F4x4& operator /= ( const F4& b ) {
		x /= b.x;
		y /= b.y;
		z /= b.z;
		t /= b.w;
		return *this;
	};
	F4x4 T4x4() {
		F4x4 c;
		c.x.col4x4(&x.x);
		c.y.col4x4(&x.y);
		c.z.col4x4(&x.z);
		c.t.col4x4(&x.w);
		return c;
	}
	F4x4 T3x3() {
		F4x4 c = *this;
		c.x.col4x3(&x.x);
		c.y.col4x3(&x.y);
		c.z.col4x3(&x.z);
		return c;
	}
	F4x4& operator *= ( const F4x4& b ) {
        F4 v;
		F4x4 c = *this;

		v.col4x4(&b.x.x);
		x.x = c.x*v;
		y.x = c.y*v;
		z.x = c.z*v;
		t.x = c.t*v;

		v.col4x4(&b.x.y);
		x.y = c.x*v;
		y.y = c.y*v;
		z.y = c.z*v;
		t.y = c.t*v;

		v.col4x4(&b.x.z);
		x.z = c.x*v;
		y.z = c.y*v;
		z.z = c.z*v;
		t.z = c.t*v;

		v.col4x4(&b.x.w);
		x.w = c.x*v;
		y.w = c.y*v;
		z.w = c.z*v;
		t.w = c.t*v;
		return *this;
	}
	F4x4 operator * ( const F4x4& b ) const {
        F4 v;
		F4x4 c;

		v.col4x4(&b.x.x);
		c.x.x = x*v;
		c.y.x = y*v;
		c.z.x = z*v;
		c.t.x = t*v;

		v.col4x4(&b.x.y);
		c.x.y = x*v;
		c.y.y = y*v;
		c.z.y = z*v;
		c.t.y = t*v;

		v.col4x4(&b.x.z);
		c.x.z = x*v;
		c.y.z = y*v;
		c.z.z = z*v;
		c.t.z = t*v;

		v.col4x4(&b.x.w);
		c.x.w = x*v;
		c.y.w = y*v;
		c.z.w = z*v;
		c.t.w = t*v;
		return c;
	}
	F4x4 operator ^ ( const F4x4& b ) const {
		F4x4 o(1.0);
		o.x = b.x*x.x + b.y*x.y + b.z*x.z;
		o.y = b.x*y.x + b.y*y.y + b.z*y.z;
		o.z = b.x*z.x + b.y*z.y + b.z*z.z;
		o.t = b.x*t.x + b.y*t.y + b.z*t.z + b.t;
		return o;
	}



	F4x4 operator * ( const float b ) const {
		F4x4 o;
		o.x = x*b;
		o.y = y*b;
		o.z = z*b;
		o.t = t*b;
		return o;
	}
	F4x4 operator + ( const float b ) const {
		F4x4 c = *this;
		c.x += b;
		c.y += b;
		c.z += b;
		c.t += b;
		return c;
	}
	F4x4 operator - ( const float b ) const {
		F4x4 c = *this;
		c.x -= b;
		c.y -= b;
		c.z -= b;
		c.t -= b;
		return c;
	}



	F4x4 operator * ( const F4& b ) const {
		F4x4 o;
		o.x = x*b.x;
		o.y = y*b.y;
		o.z = z*b.z;
		o.t = t*b.w;
		return o;
	}
	F4 dot( const F4x4& b ) const {
		F4 o;
		o.x = x*b.x;
		o.y = y*b.y;
		o.z = z*b.z;
		o.w = t*b.t;
		return o;
	}
	F4x4 operator + ( const F4x4& b ) const {
		F4x4 c = *this;
		c.x += b.x;
		c.y += b.y;
		c.z += b.z;
		c.t += b.t;
		return c;
	}
	F4x4 operator - ( const F4x4& b ) const {
		F4x4 c = *this;
		c.x -= b.x;
		c.y -= b.y;
		c.z -= b.z;
		c.t -= b.t;
		return c;
	}

	F4x4& a( float r, float toR = 1.0 ) {
		if( r == 0.0 )
			return *this = 1.0;
		r /= toR;
		float cr = cos(r), sr = sin(r);
		gpmCLR;

		t.w = x.x = 1.0;
		y.y =  cr; y.z = sr;
		z.y = -sr; z.z = cr;
		return *this;
	}
	F4x4& b( float r, float toR = 1.0 ) {
		if( r == 0.0 )
			return *this = 1.0;
		r /= toR;
		float cr = cos(r), sr = sin(r);
		gpmCLR;
		x.x = cr; x.z = -sr;
		t.w = y.y = 1.0;
		z.x = sr; z.z =  cr;
		return *this;
	}
	F4x4& c( float r, float toR = 1.0 ) {
		if( r == 0.0 )
			return *this = 1.0;
		r /= toR;
		float cr = cos(r), sr = sin(r);
		gpmCLR;
		x.x = cr; x.y = sr;
		y.x =-sr; y.y = cr;
		t.w = z.z = 1.0;
		return *this;
	}
	F4x4& mxABC( F4 abc, float toR = 1.0 ) {
		a( abc.A, toR );
		F4x4 bc;
		bc.b( abc.B, toR );
		*this *= bc;
		bc.c( abc.C, toR );
		*this *= bc;
		return *this;
	}




//	F4x4& ypr( F4 ypr, float toR = 1.0 ) {
//		ypr /= toR;
//		float	cy = cosf(ypr.x), sy = sinf(ypr.x),
//				cp = cosf(ypr.y), sp = sinf(ypr.y),
//				cr = cosf(ypr.z), sr = sinf(ypr.z);
//
//		_11 =  cr*cy + sr*sp*sy;	_12 = sr*cp;	_13 = -cr*sy + sr*sp*cy;	_14 = 0.0f;
//		_21 = -sr*cy + cr*sp*sy;	_22 = cr*cp;	_23 = sr*sy + cr*sp*cy;		_24 = 0.0f;
//		_31 =  cp*sy;				_32 = -sp;		_33 = cp*cy;				_34 = 0.0f;
//
//		_41 = _42 = _43 = 0.0f;				_44 = 1.0f;
//
//		return *this;
//	}
//	F4x4& AXr( F4 vec, float rad ) {
//		double	l = sqrt(vec.qlen());
//		float	CR = cosf(rad), SR = sinf(rad), T = 1-CR,
//				X = vec.x/l, Y = vec.y/l, Z = vec.z/l,
//				X2 = X*X, Y2 = Y*Y, Z2 = Z*Z;
//
//		_11 = T*X2+CR;		_12 = T*X*Y+SR*Z; 	_13 = T*X*Z-SR*Y;
//		_21 = T*X*Y-SR*Z;	_22 = T*Y2+CR;		_23 = T*Y*Z+SR*X;
//		_31 = T*X*Z+SR*Y; 	_32 = T*Y*Z-SR*X;	_33 = T*Z2+CR;
//
//		_14 = _24 = _34 = _41 = _42 = _43 = 0.0f;
//		_44 = 1.0f;
//
//		return *this;
//	}

	F4x4& ypr( F4 ypr, float toR = 1.0 ) {
		ypr /= toR;
		float	cy = cosf(ypr.x), sy = sinf(ypr.x),
				cp = cosf(ypr.y), sp = sinf(ypr.y),
				cr = cosf(ypr.z), sr = sinf(ypr.z);

		x.x =  cr*cy + sr*sp*sy;	x.y = sr*cp;	x.z = -cr*sy + sr*sp*cy;	x.w = 0.0f;
		y.x = -sr*cy + cr*sp*sy;	y.y = cr*cp;	y.z = sr*sy + cr*sp*cy;		y.w = 0.0f;
		z.x =  cp*sy;				z.y = -sp;		z.z = cp*cy;				z.w = 0.0f;

		t.x = t.y = t.z = 0.0f;				t.w = 1.0f;

		return *this;
	}
	F4x4& AXr( F4 vec, float rad ) {
		double	l = sqrt(vec.qlen());
		float	CR = cosf(rad), SR = sinf(rad), T = 1-CR,
				X = vec.x/l, Y = vec.y/l, Z = vec.z/l,
				X2 = X*X, Y2 = Y*Y, Z2 = Z*Z;

		x.x = T*X2+CR;		x.y = T*X*Y+SR*Z; 	x.z = T*X*Z-SR*Y;
		y.x = T*X*Y-SR*Z;	y.y = T*Y2+CR;		y.z = T*Y*Z+SR*X;
		z.x = T*X*Z+SR*Y; 	z.y = T*Y*Z-SR*X;	z.z = T*Z2+CR;

		x.w = y.w = z.w = t.x = t.y = t.z = 0.0f;
		t.w = 1.0f;

		return *this;
	}

	F4 eulABC() {

		F4 abc(0.0, acos(x.z)-(PI/2.0) ), N;	// B
        if( fabs(x.z) > 0.9998f )
        {
			// nagyon erös a B
			abc.C = acos(y.y);
			if( y.x > 0.0 )
				abc.C = -abc.C;
			// csak BC tekerés kell
			return abc;
        }

		N = F4(0,0,1).X3(x).N3();
		abc.C = acos(N.y);
		if( N.x > 0.0 )
			abc.C = -abc.C;

		abc.A = acos( N*z );
		if( z.z < 0.0 )
			abc.A = -abc.A;

		abc.A -= (PI/2.0);
		return abc;
	}

	F4 eulYPR() {
		F4 ypr(0.0);
		if( fabs(y.x) > 0.998f )
		{
			ypr.ry = atan2( x.z, z.z );
			ypr.rp = PI/2.0;
			ypr.rr = 0;
			return ypr;
		}

		ypr.ry = atan2( -z.x, x.x );
		ypr.rp = atan2( -y.z, y.y );
		ypr.rr = asin(y.x);
		return ypr;
	}


	F4x4 lerp_xyz( const F4x4& b, float ab ) const {
		if( ab <= 0.0002f )
			return *this;
		if( ab >= 0.9998f )
			return b;

		F4x4 axr, o = *this;

		F4	No = x.N3(),
			Ni = b.x.N3(), R;
		float c = Ni*No, a = acos(c)*ab;
		if( a>0.01f && a<(PI*0.99f) )
		{
			R = No.X3(Ni);
			axr.AXr( R, a );
			o *= axr;
		}

		No = o.y.N3();
		Ni = b.y.N3();
		c = Ni*No;
		a = acos(c)*ab;
		if( a>0.01f && a<(PI*0.99f) )
		{
			R = No.X3(Ni);
			axr.AXr( R, a );
			o *= axr;
		}

		No = o.z.N3();
		Ni = b.z.N3();
		c = Ni*No;
		a = acos(c)*ab;
		if( a>0.01f && a<(PI*0.99f) )
		{
			R = No.X3(Ni);
			axr.AXr( R, a );
			o *= axr;
		}

		return o;
	}

	F4x4 lerp_zyx( const F4x4& b, float ab ) const {
		if( ab <= 0.0002f )
			return *this;
		if( ab >= 0.9998f )
			return b;

		F4x4 axr, o = *this;

		F4	No = z.N3(),
			Ni = b.z.N3(), R;
		float c = Ni*No, a = acos(c)*ab;
		if( a>0.01f && a<(PI*0.99f) )
		{
			R = No.X3(Ni);
			axr.AXr( R, a );
			o *= axr;
		}

		No = o.y.N3();
		Ni = b.y.N3();
		c = Ni*No;
		a = acos(c)*ab;
		if( a>0.01f && a<(PI*0.99f) )
		{
			R = No.X3(Ni);
			axr.AXr( R, a );
			o *= axr;
		}

		No = o.x.N3();
		Ni = b.x.N3();
		c = Ni*No;
		a = acos(c)*ab;
		if( a>0.01f && a<(PI*0.99f) )
		{
			R = No.X3(Ni);
			axr.AXr( R, a );
			o *= axr;
		}

		return o;
	}

	F4x4& lat( F4x4 e, F4x4 c, F4x4 u );
};

class D4 {
public:
    double x,y,z,w;
    D4(){};
    D4( I4 _x, I4 _y, I4 _z, I4 _w ) {
        x = _x; y = _y; z = _z; w = _w;
    }
    D4( double _x, double _y = 0.0, double _z = 0.0, double _w = 0.0 ) {
        x = _x; y = _y; z = _z; w = _w;
    }
    D4( double* pD ) {
        gpmMcpyOF( this, pD, 1 );
    }
    double sum( void ) const {
        return x+y+z+w;
    }
    double sum_xyz( void ) const {
        return x+y+z;
    }
    double qlen( void ) const {
        return x*x+y*y+z*z+w*w;
    }
    double qlen_xyz( void ) const {
        return x*x+y*y+z*z;
    }
    D4 N3( void ) const {
        double l = sqrt(qlen_xyz());
		return D4( x/l, y/l, z/l );
	}
	double dot_xyz( D4 b ) const {
        return x*b.x + y*b.y + z*b.z;
	}
    D4 X3( D4 b ) const {
		return D4(
						y * b.z - z * b.y,
						z * b.x - x * b.z,
						x * b.y - y * b.x
				);
	}
	D4 operator + ( const D4& b ) const {
        return D4( x+b.x, y+b.y, z+b.z, w+b.w );
	}
	D4 operator - ( const D4& b ) const {
        return D4( x-b.x, y-b.y, z-b.z, w-b.w );
	}
};

typedef enum gpeNET2:U4 {
	gpcNET2_NULL, // = 0,
	gpeNET2_D8	= MAKE_ID( 'D', '8', 0, 0 ),
	gpeNET2_F4	= MAKE_ID( 'F', '4', 0, 0 ),

	gpeNET2_I1	= MAKE_ID( 'I', '1', 0, 0 ),
	gpeNET2_I2	= MAKE_ID( 'I', '2', 0, 0 ),
	gpeNET2_I4	= MAKE_ID( 'I', '4', 0, 0 ),
	gpeNET2_I8	= MAKE_ID( 'I', '8', 0, 0 ),

	gpeNET2_U1	= MAKE_ID( 'U', '1', 0, 0 ),
	gpeNET2_U2	= MAKE_ID( 'U', '2', 0, 0 ),
	gpeNET2_U4	= MAKE_ID( 'U', '4', 0, 0 ),
	gpeNET2_U8	= MAKE_ID( 'U', '8', 0, 0 ),

} gpeNET2;
typedef enum gpeNET4:U4 {
	gpeNET4_null, // = 0,
	gpeNET4_U11	= MAKE_ID( sizeof(U1)		, 'U', '1', '1' ),
	gpeNET4_I11	= MAKE_ID( sizeof(I1)		, 'I', '1', '1' ),

	gpeNET4_U21	= MAKE_ID( sizeof(U2)		, 'U', '2', '1' ),
	gpeNET4_I21	= MAKE_ID( sizeof(I2)		, 'I', '2', '1' ),
	gpeNET4_H21	= MAKE_ID( 2, 'H', '4', '1' ),

	gpeNET4_U41	= MAKE_ID( sizeof(U4)		, 'U', '4', '1' ),
	gpeNET4_I41	= MAKE_ID( sizeof(I4)		, 'I', '4', '1' ),
	gpeNET4_F41	= MAKE_ID( sizeof(float)	, 'F', '4', '1' ),

	gpeNET4_ALF	= MAKE_ID( sizeof(gpeALF)	, 'A', 'L', 'F' ),
	gpeNET4_U81	= MAKE_ID( sizeof(U8)		, 'U', '8', '1' ),
	gpeNET4_I81	= MAKE_ID( sizeof(I8)		, 'I', '8', '1' ),
	gpeNET4_D81	= MAKE_ID( sizeof(double)	, 'D', '8', '1' ),

	// x2
	gpeNET4_U12	= MAKE_ID( 2, 'U', '1', '2' ),
	gpeNET4_I12	= MAKE_ID( 2, 'I', '1', '2' ),

	gpeNET4_U22	= MAKE_ID( 4, 'U', '2', '2' ),
	gpeNET4_I22	= MAKE_ID( 4, 'I', '2', '2' ),
	gpeNET4_H22	= MAKE_ID( 4, 'H', '4', '2' ),

	gpeNET4_U42	= MAKE_ID( sizeof(U4x2)		, 'U', '4', '2' ),
	gpeNET4_I42	= MAKE_ID( sizeof(I4x2)		, 'I', '4', '2' ),
	gpeNET4_F42	= MAKE_ID( sizeof(I4x2)		, 'F', '4', '2' ),

	gpeNET4_AN	= MAKE_ID( 16, 'U', '8', '2' ),
	gpeNET4_U82	= MAKE_ID( 16, 'U', '8', '2' ),
	gpeNET4_I82	= MAKE_ID( 16, 'I', '8', '2' ),
	gpeNET4_D82	= MAKE_ID( 16, 'D', '8', '2' ),


	// x3
	gpeNET4_RGB	= MAKE_ID( 3, 'R', 'G', 'B' ),
	gpeNET4_U13	= MAKE_ID( 3, 'U', '1', '3' ),
	gpeNET4_I13	= MAKE_ID( 3, 'I', '1', '3' ),

	gpeNET4_U23	= MAKE_ID( 6, 'U', '2', '3' ),
	gpeNET4_I23	= MAKE_ID( 6, 'I', '2', '3' ),
	gpeNET4_H23	= MAKE_ID( 6, 'H', '4', '3' ),

	gpeNET4_U43	= MAKE_ID( 12, 'U', '4', '3' ),
	gpeNET4_I43	= MAKE_ID( 12, 'I', '4', '3' ),
	gpeNET4_F43	= MAKE_ID( 12, 'F', '4', '3' ),

	gpeNET4_U83	= MAKE_ID( 24, 'U', '8', '3' ),
	gpeNET4_I83	= MAKE_ID( 24, 'I', '8', '3' ),
	gpeNET4_D83	= MAKE_ID( 24, 'D', '8', '3' ),

	// x4
	gpeNET4_CLR	= MAKE_ID( 4, 'C', 'L', 'R' ),
	gpeNET4_U14	= MAKE_ID( sizeof(U1x4)		, 'U', '1', '4' ),
	gpeNET4_I14	= MAKE_ID( 4, 'I', '1', '4' ),

	gpeNET4_U24	= MAKE_ID( 8, 'U', '2', '4' ),
	gpeNET4_I24	= MAKE_ID( 8, 'I', '2', '4' ),
	gpeNET4_H24	= MAKE_ID( 8, 'H', '4', '4' ),

	gpeNET4_U44	= MAKE_ID( sizeof(U4x4)		, 'U', '4', '4' ),
	gpeNET4_I44	= MAKE_ID( sizeof(I4x2)		, 'I', '4', '4' ),
	gpeNET4_F44	= MAKE_ID( sizeof(F4)		, 'F', '4', '4' ),

	gpeNET4_U84	= MAKE_ID( sizeof(U8x4)		, 'U', '8', '4' ),
	gpeNET4_I84	= MAKE_ID( sizeof(I8x4)		, 'I', '8', '4' ),
	gpeNET4_D84	= MAKE_ID( sizeof(D4)		, 'D', '8', '4' ),

	gpeNET4_RES = MAKE_ID( 32 //sizeof(gpcREStrs)
							, 'R', 'E', 'S' ),

	gpeNET4_MAS = MAKE_ID( sizeof(void*), 'M', 'A', 'S' ),


	gpeNET4_0EYE	= MAKE_ID( 0, 'E', 'Y', 'E' ),
	gpeNET4_0HUD	= MAKE_ID( 0, 'H', 'U', 'D' ),

	gpeNET4_0LST	= MAKE_ID( 0, 'L', 'S', 'T' ),
	gpeNET4_0NYL	= MAKE_ID( 0, 'N', 'Y', 'L' ),
	gpeNET4_0PIC	= MAKE_ID( 0, 'P', 'I', 'C' ),
	gpeNET4_0SRC	= MAKE_ID( 0, 'S', 'R', 'C' ),
	gpeNET4_0SYN	= MAKE_ID( 0, 'S', 'Y', 'N' ),



	gpeNET4_PREV	= MAKE_ID( 'P', 'R', 'E', 'V' ),
} gpeNET4;


class gpcCMPL;


class gpcSYNC {
protected:
	gpeNET4	typ4;
	U4		nb;
public:
	U4		id;
	U4		ms;
	SOCKET	iGT;

	gpcSYNC( gpeNET4 t, U8 i, U4 s,	SOCKET	sck, // = INVALID_SOCKET
									U4 		n ) // = 0 )
	{
        typ4 = t;
        id = i;
        ms = s;
        nb = n ? n : sizeof(*this);
        iGT = sck;
	}
	gpeNET4 typ()
	{
		return typ4;
	}
	U4 nS()
	{
		return nb/sizeof(*this);
	}
	U4 nB()
	{
		return nb;
	}
	U4 nB( U4 n )
	{
		if( !this )
			return 0;

		if( n < sizeof(*this) )
			n = sizeof(*this);

		nb = n;
		return nb;
	}
	bool operator != ( const gpcSYNC b ) const
	{
		if( typ4 != b.typ4 )
			return true;

		return id != b.id;
	}

	bool operator == ( const gpcSYNC b ) const
	{
		return !(*this != b);
	}

	bool bSW( U8 sw )
	{
		switch( typ4 )
		{
			case gpeNET4_0SRC:
				if( sw&1 )
					return false; // ha azonos file csak akkor engedi a src-t elküldeni, különben hagyja a continue-t

				return true;
			default:
				break;
		}
		return false;
	}
};

#define gpdLZYallGT 4
//#define gpmLZYvali( a, b ) ((a*)( (b) ? (b)->p_alloc : NULL ))
#define gpmLZYvaliPAD( a, b, n ) ((a*)( (b) ? ((b)->p_alloc ? (b)->p_alloc+(n) : NULL ) : NULL ))
#define gpmLZYvali( a, b ) gpmLZYvaliPAD( a, (b), 0 )

#define gpmLZYloadPAD( p, t, n ) ((p) ? ((p->n_load) ? (((p->n_load)-(n))/sizeof(t)) : 0 ) : 0 )
#define gpmLZYload( p, t ) gpmLZYloadPAD( (p), t, (U8)0 )

class gpcROBnD;
class gpcZSnD;

typedef enum gpeAT:U4{
	gpeAT_no,
	gpeAT_ok,
	gpeAT_at,
	gpeAT_err,
	gpeAT_cmti,
	gpeAT_clip,
	gpeAT_ring,
	gpeAT_miss,
	gpeAT_voice,
	gpeAT_call,
	gpeAT_begin,
	gpeAT_end,
	gpeAT_mrk,
	gpeAT_creg,


	gpeAT_noQ,
	gpeAT_okQ,
	gpeAT_atQ,
	gpeAT_errQ,
	gpeAT_cmtiQ,
	gpeAT_clipQ,
	gpeAT_ringQ,
	gpeAT_missQ,
	gpeAT_voiceQ,
	gpeAT_callQ,
	gpeAT_beginQ,
	gpeAT_endQ,
	gpeAT_mrkQ,
	gpeAT_cregQ,


	gpeAT_noP,
	gpeAT_okP,
	gpeAT_atP,
	gpeAT_errP,
	gpeAT_cmtiP,
	gpeAT_clipP,
	gpeAT_ringP,
	gpeAT_missP,
	gpeAT_voiceP,
	gpeAT_callP,
	gpeAT_beginP,
	gpeAT_endP,
	gpeAT_mrkP,
	gpeAT_cregP,


	gpeAT_noA,
	gpeAT_okA,
	gpeAT_atA,
	gpeAT_errA,
	gpeAT_cmtiA,
	gpeAT_clipA,
	gpeAT_ringA,
	gpeAT_missA,
	gpeAT_voiceA,
	gpeAT_callA,
	gpeAT_beginA,
	gpeAT_endA,
	gpeAT_mrkA,
	gpeAT_cregA,


	gpeAT_noPP,
	gpeAT_okPP,
	gpeAT_atPP,
	gpeAT_errPP,
	gpeAT_cmtiPP,
	gpeAT_clipPP,
	gpeAT_ringPP,
	gpeAT_missPP,
	gpeAT_voicePP,
	gpeAT_callPP,
	gpeAT_beginPP,
	gpeAT_endPP,
	gpeAT_mrkPP,
	gpeAT_cregPP,


	gpeAT_N,
} gpeAT;
class gpcLZY {
public:
	union {
		struct{
			U1* p_alloc, aXYZW[4], aSET[4];

			U8 //size_t // ha x32 CPU-n size_t 20 byte lesz és nem 32 azaz nem 16B aligned
				n_load, n_alloc;
		};
		struct{
			U8		*p_a8;
			U4		type;	/// x[7s,6f,5r,4str : 3-0 nBYTE = 1<<(x&0xf) ]
							/// yz[ dimXY ] 	, nBYTE = 1<<(x&0xf)
			gpeWIP	aWIP[4];
			U8		n_l8, n_a8;
		};
	};
	gpcLZY* null(){
		if(!this)
			return this;

		gpmDELary(p_alloc);
		gpmCLR;
		return this;
	}
	gpcLZY( void ) { gpmCLR; }
	gpcLZY( U1 n ) {
		gpmCLR;

		if( n < 1 )
			n = 4;
		aWIP[gpeLZYwip] = gpeWIP_done,
		aSET[gpeLZYxN] = n;
	}
	gpcLZY( gpcVAR* pALF, U4 n );
	gpcLZY( const gpcLZY& b );
 	~gpcLZY() { gpmFREE( p_alloc ); }
	size_t nLD( size_t n = 1 ) const { return this ? n_load/n : 0; }
	U8 nSUM(){
		U8 sum = nLD();
		if( !sum )
			return 0;

		sum = sum*sum/2;
		for( U8 i = 0, e = nLD(); i < e; i++ ){
			sum += p_alloc[i];
		}
		return sum;
	}

	U1x4& typ() { return *(U1x4*)&type; }
	U4x4* pMAP( U4 nX, U4 nLIM, size_t nBYTE ) {
		if( !this )
			return NULL;

		size_t nNEED = nX*(sizeof(U4x4) + nLIM*nBYTE);
		if( n_alloc >= nNEED )
			return (U4x4*)p_alloc;

		U8 iS = 0;
		lzyINS( NULL, nNEED-n_alloc, iS, -1, 2 );
		n_load = iS;
		return (U4x4*)p_alloc;
	}
	U4x4* xFND( U4 x, U4 nX, U4 nLIM, size_t nBYTE = sizeof(U1x4) ) {
		if( !this )
			return NULL;

		U4x4* pM = pMAP( nX, nLIM, nBYTE );
		if( !pM )
			return NULL;
		size_t	nM = n_load/sizeof(*pM),
				iM = nM,
				xM = sizeof(*pM)*nX;
		if( nLIM != ((n_alloc-xM)/(nX*nBYTE)) )
			nLIM = ((n_alloc-xM)/(nX*nBYTE));
				//;
				//nO = sizeof(*pO);
		//pU1x4 = p_alloc + xM;
		for( U4 i = 0; i < nM; i++ ) {
			if( pM[i].x != x )
				continue;
			if( !i )
				return pM;

			if( i < nLIM )
			{
				i--;
				U4x4 t = pM[i];
				pM[i] = pM[i+1];
				pM[i+1] = t;
				return pM+i;
			}

			U4x4 t = pM[nLIM-1];
			t.y = 0;
			pM[nLIM-1].xyz_( pM[i] );
			pM[i].xyz_(t);
			pM[nLIM-1].y = 0;
			i = nLIM-1;
			return pM+i;
		}
		if( nM >= nX )
			return NULL; // nincsen több hely

		pM[iM=nM] = U4x4( x, 0, 0, gpmMIN(iM,nLIM) );
		nM++;
		n_load = sizeof(*pM)*nM;

		if( !iM )
			return pM;

		if( iM < nLIM ) {
			iM--;
			U4x4 t = pM[iM];
			pM[iM] = pM[iM+1];
			pM[iM+1] = t;
			return pM+iM;
		}

		U4x4 t = pM[nLIM-1];
		t.y = 0;
		pM[nLIM-1].xyz_( pM[iM] );
		pM[iM].xyz_(t);

		pM[iM = nLIM-1].y = 0;
		return pM+iM;
	}


	U4 nPC( void );

	void wait( void ) {
		if( !this )
			return;

		while( aWIP[gpeLZYwip] != gpeWIP_done )
			usleep(100);

	}
	void DO(void) { aWIP[gpeLZYwip] = aWIP[gpeLZYwipSTK]; }


	gpcLZY* syncADD( gpcSYNC s, U4& ms ) {
		if( !s.typ() )
			return this;

		gpcSYNC* pSYNC = this ? (gpcSYNC*)p_alloc : NULL;

		if( pSYNC )
		for( U4 i = 0, e = n_load/sizeof(*pSYNC); i < e; i++ )
		{
			if( pSYNC[i] != s )
				continue;

			if( pSYNC[i].ms >= s.ms )
				return this;

			pSYNC[i].ms = s.ms;
			pSYNC[i].iGT = s.iGT;
			//s.typ = gpeNET4_null;
			if( ms < s.ms )
				ms = s.ms;
			return this;
		}

		U8 i = -1;
		gpcLZY* pOUT = lzyADD( &s, sizeof(s), i );

		if( ms < s.ms )
			ms = s.ms;
		return pOUT;
	}
	gpcLZY* SYNrdy( U4 strt ) {
		if( this ? (strt > n_load) : true )
			return this;

		gpcSYNC* pSYN = (gpcSYNC*)(p_alloc+strt);
		pSYN->nB( n_load-strt );
		if( pSYN->nB() > sizeof(*pSYN) ) //pSYN->nS() >= 1 )
			return this;

		if( strt )
		{
			n_load = strt;
			return this;
		}

		delete this;
		return NULL;

	}
	gpcLZY* putZN( U1* pDAT, U4 nDAT, gpeNET4 nt4, U4 zn, U4 ms ) {
		U8 s = -1, b;
		gpcSYNC syn( nt4, 0, ms, INVALID_SOCKET, 0 );

		gpcLZY* pOUT = this->lzyADD( &syn, sizeof(syn), s = -1 );
		b = s;
		pOUT = pOUT->lzyADD( &zn, sizeof(zn), s = -1 );

		if( pDAT ? nDAT : false )
			pOUT = pOUT->lzyADD( pDAT, nDAT, s = -1 );

		return pOUT->SYNrdy(b);
	}
	gpcLZY* putPIC( U1* pDAT, U4 nDAT, U1* pNAME, U4 ms ) {
		if( pDAT ? !nDAT : true )
			return this;

		U8 s = -1, b;
		gpcSYNC syn( gpeNET4_0PIC, 0, ms, INVALID_SOCKET, 0 );

		gpcLZY* pOUT = this->lzyADD( &syn, sizeof(syn), s = -1 );
		b = s;
		if( !pNAME )
			pNAME = (U1*)"ize.pic";
		pOUT = pOUT->lzyADD( pNAME, gpmSTRLEN(pNAME)+1, s = -1 );
		pOUT = pOUT->lzyADD( pDAT, nDAT, s = -1 );

		return pOUT->SYNrdy(b);
	}
	gpcLZY* putSYN( gpcLZY* pOUT, U4 ms, SOCKET iGT, U8 bSW ) {
		if( this ? ( p_alloc ? !n_load : true ) : true )
			return pOUT;

		U8 s = -1, b;
		gpcSYNC syn( gpeNET4_0SYN, 0, ms, INVALID_SOCKET, 0 ),
				*pSYN = (gpcSYNC*)p_alloc;

		pOUT = pOUT->lzyADD( &syn, sizeof(syn), s = -1 );
		b = s;
		if( iGT > -1 )
		{
			for( U4 i = 0, e = n_load/sizeof(syn); i < e; i++ )
			{
				if( pSYN[i].ms < ms )
					continue;
				if( pSYN[i].iGT == iGT )
					continue;
				if( pSYN[i].bSW( bSW ) )
					continue;

				pOUT = pOUT->lzyADD( pSYN+i, sizeof(syn), s = -1 );
			}
		}
		else for( U4 i = 0, e = n_load/sizeof(syn); i < e; i++ )
		{
			if( pSYN[i].ms < ms )
				continue;
			if( pSYN[i].bSW( bSW ) )
					continue;

			pOUT = pOUT->lzyADD( pSYN+i, sizeof(syn), s = -1 );
		}

		return pOUT->SYNrdy(b);
	}

	gpcLZY* qEVENT(void);

	gpcLZY* lzy_strict( void ) {
		if( this ? !n_load : true )
			return this;

		if( n_load < n_alloc )
		{
			U8 new_alloc = gpmPAD( n_load, 0x10 );
			if( !new_alloc )
			{
				(*p_alloc) = n_load = 0;
				return this;
			}
			else if( n_alloc > new_alloc )
			{
				U1* p_kill = p_alloc;
				p_alloc = gpmALLOC( n_alloc+0x10 ); //, 0x10 );
				memcpy( p_alloc, p_kill, n_load );
				gpmFREE( p_kill );
				n_alloc = new_alloc;
			}
		}
		p_alloc[ n_load ] = 0;
		return this;
	}
	gpcLZY* lzyRST( void ) {
		if( this ? !n_load : true )
			return this;

		if( n_load*2 < n_alloc )
		{
			U8 new_alloc = gpmPAD( ((n_alloc*3)/4), 0x10 );
			if( !new_alloc ) {
				(*p_alloc) = n_load = 0;
				return this;
			}
			else if( n_alloc > new_alloc ) {
				gpmFREE( p_alloc );
				n_alloc = new_alloc;
				p_alloc = gpmALLOC( n_alloc+0x10 );
			}
		}
		(*p_alloc) = n_load = 0;
		return this;
	}

	gpcLZY* lzyADD( const void* p_void, U8 n_byte, U8& iSTRT, U1 n = 0 ) {
		if( !n_byte )
			return this;

		if( !this ) {
			iSTRT = 0;
			gpcLZY* p_lazy = new gpcLZY( n );
			p_lazy->n_alloc = gpmPAD( n_byte*p_lazy->aSET[gpeLZYxN], 0x10 );
			p_lazy->p_alloc = gpmALLOC( p_lazy->n_alloc+0x10 ); //, 0x10 );
			if( !p_void )
			{
				p_lazy->p_alloc[p_lazy->n_load = n_byte] = 0;
				return p_lazy;
			}

			((U1*)memcpy( p_lazy->p_alloc, p_void, n_byte ))[n_byte] = 0;
			p_lazy->n_load = n_byte;
			return p_lazy;
		}
		if( n > 0x80 )
			n = 4;
		if( !n )
		{
			if( !aSET[gpeLZYxN] )
				aSET[gpeLZYxN] = 4;

			n = aSET[gpeLZYxN];
		}
		else if( !aSET[gpeLZYxN] )
				aSET[gpeLZYxN] = n;


		if( iSTRT > n_load )
			iSTRT = n_load;
		else
			n_load = iSTRT;

		if( p_alloc ? (n_load+n_byte > n_alloc) : true ) {
			U1* p_kill = p_alloc;
			n_alloc = gpmPAD( (n_load+n_byte*n), 0x10 );
            p_alloc = gpmALLOC( n_alloc+0x10 ); //, 0x10 );
			if( p_kill )
			{
				gpmMcpy( p_alloc, p_kill, n_load );
				gpmFREE( p_kill );
			} else
				n_load = 0;
		}

		if( p_void )
			gpmMcpy( p_alloc+n_load, p_void, n_byte );

		n_load += n_byte;
		p_alloc[n_load] = 0;
		return this;
	}
	void* pVALID( gpcLZY* pLZY, void* pTHIS = NULL ) {
		if( !this )
			return NULL; // ha nincsen thisLZY akkor nem lehet valós pU2

		if( pTHIS != (void*)p_alloc )
			pTHIS = NULL;
		else if( pLZY->n_load > n_load )
			pTHIS = NULL;	// pLZY nagyobb akkor sem jó pLZY-ben több az adat

		if( pTHIS )
			return (void*)p_alloc; // tehát ha nem lett NULL akor VALID

		if( pLZY->n_load <= n_load )
			return (void*)p_alloc;

		/// pLZY töltve lett, a fölét a IDE(this) is feltöltjük
		U8 STRT = -1;
		lzyADD( pLZY->p_alloc+n_load, pLZY->n_load-n_load, STRT );
		return (void*)p_alloc;
	}

	gpcLZY* lzyPLUS(  const gpcLZY* p_b, U8& iSTRT ) {
		return lzyADD( p_b->p_alloc, p_b->n_load, iSTRT, ( (p_b->n_load<=0x40) ? 0xf : 0x3 ) );
	}
	gpcLZY* lzySUB( U8& iSTRT, U8 n_sub ) {
		if( this ? !n_sub : true )
		{
			iSTRT = 0;
			return this;
		}
		if( !n_sub )
		{
			iSTRT = n_load;
			return this;
		}
		if( iSTRT > n_load )
		{
			iSTRT = n_load;
			return this;
		}
		if( iSTRT+n_sub >= n_load )
		{
			n_sub = n_load-iSTRT;
		} else {
			U8 n_hi = n_load-(iSTRT+n_sub);
			memcpy( p_alloc+iSTRT, p_alloc+iSTRT+n_sub, n_hi );
		}

		if( n_load > n_sub)
			n_load -= n_sub;
		else
			n_load = 0;

		if( p_alloc )
			p_alloc[n_load] = 0;
		return this;
	}
	gpcLZY* lzy_exp_rand( U8& iSTRT, U8 n_sub, U8 n_add, U1 n = 0 ) {
		if( iSTRT <= (this ? n_load : 0) )
			return lzy_exp( iSTRT, n_sub,n_add, n );

		U8 pls = iSTRT-n_load;
		iSTRT = n_load;
		if( !this )
			return lzyADD( NULL, n_add + pls, iSTRT, n );

		return lzy_exp( iSTRT, n_sub, n_add + pls, n );
	}
	gpcLZY* lzy_exp( U8& iSTRT, U8 n_sub, U8 n_add, U1 n = 0 ) {
		if( !this )
		{
			return lzyADD( NULL, n_add, iSTRT, n );
		}

		if( !n )
		{
			if( !aSET[gpeLZYxN] )
				aSET[gpeLZYxN] = 4;

			n = aSET[gpeLZYxN];
		}
		else if( !aSET[gpeLZYxN] )
				aSET[gpeLZYxN] = n;

		if( iSTRT > n_load )
		{
			iSTRT = n_load;
			n_sub = 0;
		}
		else if( iSTRT+n_sub >= n_load )
		{
			n_sub = n_load-iSTRT;
		}
		U8 src_hi = iSTRT+n_sub,
			dst_hi = iSTRT+n_add,
			n_hi = n_load-src_hi,
			new_load = iSTRT + n_add + n_hi;
		U1* p_kill = NULL;

		if( new_load > n_alloc )
		{
			n_alloc += gpmPAD( (n_add*n), 0x10 );
			p_kill = p_alloc;
			p_alloc = gpmALLOC( n_alloc+0x10 ); //, 0x10 );
			memcpy( p_alloc, p_kill, iSTRT );
			memcpy( p_alloc+dst_hi, p_kill+src_hi, n_hi );
			n_load = new_load;
			p_alloc[n_load] = 0;
			gpmFREE( p_kill );
			return this;
		}

		if( n_hi )
		if( n_load <= dst_hi ) {
			// nincsen átfedésben mehet sima kopival
			memcpy( p_alloc+dst_hi, (p_kill?p_kill:p_alloc)+src_hi, n_hi );
		}
		else if( dst_hi < src_hi )
		{
			memcpy( p_alloc + dst_hi, p_alloc + src_hi, n_hi );
		}
		else if( dst_hi > src_hi )
		{
			// dst_hi == src_hi akkor a memcpy úgy sem másolna
			U8 a_buff[32];
			U8 n_stp = sizeof(a_buff), i = n_hi;
			U1	*p_s = p_alloc+src_hi,
				*p_d = p_alloc+dst_hi;
			if( n_stp > n_hi )
				n_stp = n_hi;
			while( i >= n_stp )
			{
				i -= n_stp;
				memcpy( a_buff, p_s+i, n_stp );
				memcpy( p_d+i, a_buff, n_stp );
			}

			if( i )
			{
				memcpy( a_buff, p_s, i );
				memcpy( p_d, a_buff, i );
			}
		}

		//memcpy( p_alloc+start, p_u1, n_u1 );
		if( !p_alloc )
			return this;

		n_load = new_load;
		p_alloc[n_load] = 0;
		return this;
	}

	gpcLZY* lzyINS( const U1* p_u1, U8 n_u1, U8& iSTRT, U8 n_sub, U1 n = 0 ) {
		if( !this )
			return lzyADD( p_u1, n_u1, iSTRT, n );
		lzy_exp( iSTRT, n_sub,  n_u1, n );
		gpmMcpyOF( p_alloc+iSTRT, p_u1, n_u1 );
		return this;
	}
	gpcLZY& operator = ( const gpcLZY& b ) {
		U8 s = 0;
		lzyINS( b.p_alloc, b.nLD(), s, -1 );
		return *this;
	}
	gpcLZY* operator += ( const gpcLZY& plus ) {
		U8 s = -1;
		return lzyADD( plus.p_alloc, plus.nLD(), s );
	}
	gpcLZY* lzyDIR( const char* p_file, U8& iSTRT );
	gpcLZY* lzyRD( const char* p_file, U8& iSTRT, U1 n = 0 ) {
		/// READ FILE
		if( !p_file )
			return this;

		if( !this )
			iSTRT = 0;

		if( gpfACE(p_file, 4) < 0 )
			return this;

		FILE* p_f = fopen( p_file, "rb" );
		if( !p_f )
			return this;

#ifdef _WIN64
		fpos_t n_byte = 0;
		fseek(p_f, 0, SEEK_END);
		fgetpos(p_f, &n_byte);
		fseek(p_f, 0, SEEK_SET);
#else
		fpos_t fp; // = (fpos_t)0;
		U8 n_byte = 0;
		fseek(p_f, 0, SEEK_END );
		fgetpos(p_f, &fp);
		n_byte = fp.__pos;
		fseek(p_f, 0, SEEK_SET);
#endif
		gpcLZY* p_lazy = this;

		if( n_byte > 0 )
		{
			p_lazy = p_lazy->lzyADD( NULL, n_byte, iSTRT, n );
			if( !p_lazy )
				goto szasz;
			U8 n;
			U1* p_l = p_lazy->p_alloc + iSTRT, *p_e = p_l+n_byte;
			while( p_l < p_e )
			{
				n = fread( p_l, 1, n_byte, p_f );
				if( !n )
					break;
				p_l += n;
				n_byte -= n;
			}
			p_lazy->n_load = p_l-p_lazy->p_alloc;
		}

	szasz:
		gpmFD_CLOSE(p_f);

		return p_lazy;
	}
	gpcLZY* lzyWR( const char* p_file, bool b_over = true, bool bADD = false ) {
		/// WRITE FILE
		if( this ? !n_load : true )
			return this;
		if( bADD )
			b_over = true;

		if( !b_over )
		if( gpfACE(p_file, 4) < 0 )
			return this;

		char s_buff[gpdMAX_PATH];
		FILE* p_f = fopen( p_file, bADD ? "ab":"wb" );
		U8 n_w = 0, n_err = 0, W0 = 0;

		if( !p_f )
		{
			if( !gpfMKDR( s_buff, p_file ) )
				goto close;
			p_f = fopen(p_file, bADD ? "ab":"wb" );
			if( !p_f )
				goto close;
		}

		n_w = fwrite( p_alloc, 1, n_load, p_f );
		if( n_w == n_load )
			goto close;
		if( !n_w )
			goto close;

		while( n_w < n_load )
		{
			if( n_err > 3 )
				return this;

			W0 = fwrite( p_alloc+n_w, 1, n_load-n_w, p_f );
			if( !W0 )
			{
				n_err++;
				sleep(0);
				continue;
			}
			n_err = 0;
			n_w += W0;
		}

close:
		gpmFD_CLOSE(p_f);
szasz:
		return this;
	}

	gpcLZY* lzyFRMT( U8& iSTRT, const char* p_format, ... );
	gpcLZY* lzyHEXb( U8& iSTRT, U1* pBIN, U4 nBIN );
	gpcLZY* lzyHEXw( U8& iSTRT, U1* pBIN, U4 nBIN );
	gpcLZY* lzyHEXl( U8& iSTRT, U1* pBIN, U4 nBIN, bool bCOM = true );
	gpcLZY* lzyROBnDstat( U8& iSTRT, gpcROBnD& RnD, U1 i = 3, char* pPP = "//" );
	gpcLZY* lzyZSnDstat( U8& iSTRT, gpcZSnD& zs, U1 i = 3 );
	gpcLZY* lzy_reqCLOSE( void ) {
		if( !this )
			return this;

		aSET[gpeLZYoff] = 1;
		return this;
	}
	bool qCLOSE( void ) {
		if( !this )
			return false;
		return aSET[gpeLZYoff];
	}
	U4 tree_fnd( U4 id, U4& n );
	gpcLZY* tree_add( U4 id, U4& n );
	U8 tree_fnd( U8 id, U8& n );
	gpcLZY* tree_add( U8 id, U8& n );
	I8 tree_fnd( I8 id, I8& n );
	gpcLZY* tree_add( I8 id, I8& n );

	gpcCMPL* pPC( U4 pc, U1* pS = NULL );
	gpcCMPL* pSPARE( U4 pc, gpeALF sw = gpeALF_null , U1* pS = NULL );
	U1* Ux( I8 iPC, U4 n, bool bZ = true, U4 stp = 0 ) {
		if( !this )
			return NULL;

		if( iPC < 0 )
			iPC *= -1;
		if( !stp )
			stp = n;
		U8	e = iPC*stp + n;
		if( e <= nLD() )
			 return p_alloc + iPC*stp; //n;

		U8 s = -1, ee = e+n*3;

		lzyADD( NULL, ee-nLD(), s );
		if( bZ )
			gpmZn( p_alloc+s, nLD()-s );
		if( nLD() > e )
			n_load = e;

        return p_alloc + iPC*stp; //*n;//+e-n*2;
	}
	I4x4* pINST( U4 pc ) { return (I4x4*)Ux( pc, sizeof(I4x4) ); }
	I4x4& INST( U4 pc,	gpeOPid op = gpeOPid_nop, gpeCsz iC = gpeCsz_OFF,
						gpeEA s0 = gpeEA_OFF, 	U1 sn = 0,	U1 si = 0,
						gpeEA d0 = gpeEA_OFF,	U1 dn = 0,	U1 di = 0 ) {
		I4x4 &ins = *(I4x4*)Ux( pc, sizeof(I4x4) );
		ins.null();
		ins.cID = iC;
		ins.op = U1x4( op, ((U1)s0<<3)|(sn&7), ((U1)d0<<3)|(dn&7), (di<<4)|(si&7) ).u4;
		return ins;
	}
	int nAT( const char* pSat, int nSat = 0, const char* pFILT = " \r\n\t:+," );
	gpcLZY* utf8( U4 u, U1 trans = '\a' ) {
		if( !u )
			return this;
		gpcLZY* pLZY = this;
		U1 aU[8], nU = 0;
		if( trans ? u == trans : false ) {
			nU = 2;
			aU[1] = (u&0x3f)|0x80; u>>=6;
			aU[0] = (u&0x1f)|0xc0;
		}
		else if( u < 0x80 ) {
			nU = 1;
			aU[0] = u;
		}
		else if( u < 0x800 ) {
			nU = 2;
			aU[1] = (u&0x3f)|0x80; u>>=6;
			aU[0] = (u&0x1f)|0xc0;
		}
		else if( u < 0x1000 ) {
			nU = 3;
			aU[2] = (u&0x3f)|0x80; u>>=6;
			aU[1] = (u&0x3f)|0x80; u>>=6;
			aU[0] = (u&0x0f)|0xe0;
		}
		else {
			nU = 4;
			aU[3] = (u&0x3f)|0x80; u>>=6;
			aU[2] = (u&0x3f)|0x80; u>>=6;
			aU[1] = (u&0x3f)|0x80; u>>=6;
			aU[0] = (u&0x07)|0xf0;
		}
		U8 s=-1;
		return pLZY->lzyADD( aU, nU, s );
	}
	U1* pU1n( int i = 0, int n = 1 ) {
		if( !this )
			return NULL;
		if( !i )
			return p_alloc;

		i *= n;
		if( i >= nLD() )
			return NULL;

		return p_alloc+i;
	}

	U1x4* pU1x4( U4x4& m, U4 nX ) {
		if( !this )
			return NULL;

		U4 nLIM = (n_alloc-sizeof(U4x4)*nX)/(nX*sizeof(U1x4));
		if( m.w >= nLIM )
			return NULL;

		return ((U1x4*)(p_alloc + sizeof(U4x4)*nX))+m.w*nX;
	}
	I8 nU4( I8 x=1 ){ return x ? nLD(x*sizeof(U4)) : nLD(sizeof(U4)); }
	U4* pU4n( int i = 0, int n = 1 ) { return (U4*)Ux( (i<0 ? nLD(sizeof(U4)): i) , sizeof(U4)*n, true, sizeof(U4)); }
	U4x2* pU4x2n( int i = 0, int n = 1 ) { return (U4x2*)Ux((i<0 ? nLD(sizeof(U4x2)): i),sizeof(U4x2)*n, true, sizeof(U4x2)); }
	U4x4* pU4x4n( int i = 0, int n = 1 ) { return (U4x4*)Ux((i<0 ? nLD(sizeof(U4x4)): i),sizeof(U4x4)*n, true, sizeof(U4x4)); }

	char* pCHAR( int i = 0 ) { return (char*)pU1n(i,sizeof(char)); }
	I1* pI1( int i = 0 ) { return (I1*)pU1n(i,sizeof(I1)); }

	U4* pU4( int i = 0 ) { return (U4*)pU1n(i,sizeof(U4)); }
	U4x2* pU4x2( int i = 0 ) { return (U4x2*)pU1n(i,sizeof(U4x2)); }
	U4x4* pU4x4( int i = 0 ) { return (U4x4*)pU1n(i,sizeof(U4x4)); }

	I4* pI4( int i = 0 ) { return (I4*)pU1n(i,sizeof(I4)); }
	I4x2* pI4x2( int i = 0 ) { return (I4x2*)pU1n(i,sizeof(I4x2)); }
	I4x4* pI4x4( int i = 0 ) { return (I4x4*)pU1n(i,sizeof(I4x4)); }

	I8x2* pI8x2( int i = 0 ) { return (I8x2*)pU1n(i,sizeof(I8x2)); }
	I8x4* pI8x4( int i = 0 ) { return (I8x4*)pU1n(i,sizeof(I8x4)); }
	U8x4* pU8x4( int i = 0 ) { return (U8x4*)pU1n(i,sizeof(U8x4)); }
	float* pF( int i = 0 ) { return (float*)pU1n(i,sizeof(float)); }
	F2* pF2( int i = 0 ) { return (F2*)pU1n(i,sizeof(F2)); }
	F4* pF4( int i = 0 ) { return (F4*)pU1n(i,sizeof(F4)); }
	F4x4* pF4x4( int i = 0 ) { return (F4x4*)pU1n(i,sizeof(F4x4)); }
	D4* pD4( int i = 0 ) { return (D4*)pU1n(i,sizeof(D4)); }
	void** ppVOID( int i = 0 ) { return (void**)pU1n(i,sizeof(U1*)); }


};


class gpcLZYdct {
	U4x4*		pIX; /// BEST!!!
public:
	gpcLZY		str,
				ix;
	U8			ver;

	gpcLZYdct(){};
	gpcLZYdct(U8 v)
	{
		gpmCLR;
	}
	void rst()
	{
		if(!this)
			return;
		pIX = NULL;
		str.lzyRST();
		ix.lzyRST();
	}
	U4 dctFND( const void* p, U8 nS, U4& nIX ) {
		if( !this )
		{
			nIX = 0;
			return 0;
		}
		U1* pS = (U1*)p;
		U8 aSTRT[2]; // = -1;
		if( !str.p_alloc )
			ver = 0;
		// bemásolja a str szótárba
		/// ebböl kéne akor csinálni egy olyat
		/// ami ki szedi a '_#' jeleket
		str.lzyADD( pS, nS+1, aSTRT[0] = -1 );
		U1* pS0 = str.p_alloc;
		pS = pS0+aSTRT[0];
		if( pS[nS] )
			pS[nS] = 0;

		ix.lzyADD( NULL, sizeof(U4x4), aSTRT[1] = -1 );
		nIX = (aSTRT[1]/sizeof(U4x4));
		U4x4 *p_ix0 = ((U4x4*)ix.p_alloc);

		pIX = p_ix0 + nIX;
		pIX->null();
		pIX->x = aSTRT[0];
		pIX->y = nS;

		U4 iIX = p_ix0->dctFND( pS0, *pIX );

		// UNDOza a bejegyzést
		str.n_load = aSTRT[0];
		ix.n_load = aSTRT[1];
		pIX = p_ix0+iIX;
		return iIX;
	}
	gpcLZYdct* dctADD( const void* pV, U8 nS ) {
		if( !this )
		{
			gpcLZYdct* p_this = new gpcLZYdct(0);

			return p_this->dctADD( pV, nS );
		}
		U1* pS = (U1*)pV;
		U8 aSTRT[2]; // = -1;
		if( !str.p_alloc )
			ver = 0;
		str.lzyADD( pS, nS+1, aSTRT[0] = -1 );
		U1* pS0 = str.p_alloc;
		pS = pS0+aSTRT[0];
		if( pS[nS] )
			pS[nS] = 0;

		ix.lzyADD( NULL, sizeof(U4x4), aSTRT[1] = -1 );
		U4 nIX = (aSTRT[1]/sizeof(U4x4));
		U4x4	*p_ix0 = ((U4x4*)ix.p_alloc);

		pIX = p_ix0 + nIX;
		pIX->null();
		pIX->x = aSTRT[0];
		pIX->y = nS;
		U4 iIX, nADD = p_ix0->dctADD( pS0, iIX, *pIX );

		if( nIX == nADD )
		{
			// UNDOza a bejegyzést
			str.n_load = aSTRT[0];
			ix.n_load = aSTRT[1];
			pIX = p_ix0+iIX;
			return this;
		}
		ver++;
		pIX = p_ix0+nIX;
		return this;
	}

	U4 dctMILLfnd( const void* pS, U8 nS, U4& nIX ) {
		if( !this )
		{
			nIX = 0;
			return 0;
		}
		U8 aSTRT[2]; // = -1;
		if( !str.p_alloc )
			ver = 0;
		// bemásolja a str szótárba
		/// ebböl kéne akor csinálni egy olyat
		/// ami ki szedi a '_#' jeleket
		U1* pH = (U1*)pS, *pSS = pH;
		U8 nH = nS;
		str.lzyADD( pH, nH+1, aSTRT[0] = -1 );
		U1* pS0 = str.p_alloc;
		pSS = pS0+aSTRT[0];
		nS = 0;
		for( U8 h = 0; h < nH; h++ ) {
			switch( pH[h] )
			{
				case '_':
				case '#':
					continue;
				default:
					break;
			}
			pSS[nS]=pH[h];
			nS++;
		}
		if( pSS[nS] )
			pSS[nS] = 0;
		str.n_load = aSTRT[0]+nS+1;

		ix.lzyADD( NULL, sizeof(U4x4), aSTRT[1] = -1 );
		nIX = (aSTRT[1]/sizeof(U4x4));
		U4x4 *p_ix0 = ((U4x4*)ix.p_alloc);

		pIX = p_ix0 + nIX;
		pIX->null();
		pIX->x = aSTRT[0];
		pIX->y = nS;

		U4 iIX = p_ix0->dctFND( pS0, *pIX );

		// UNDOza a bejegyzést
		str.n_load = aSTRT[0];
		ix.n_load = aSTRT[1];
		pIX = p_ix0+iIX;
		return iIX;
	}

	gpcLZYdct* dctMILLadd( U1* pS, U8 nS ) {
		if( !this )
		{
			gpcLZYdct* p_this = new gpcLZYdct(0);

			return p_this->dctADD( pS, nS );
		}
		U8 aSTRT[2]; // = -1;
		if( !str.p_alloc )
			ver = 0;
		// bemásolja a str szótárba
		/// ebböl kéne akor csinálni egy olyat
		/// ami ki szedi a '_#' jeleket
		U1* pH = pS;
		U8 nH = nS;
		str.lzyADD( pH, nH+1, aSTRT[0] = -1 );
		U1* pS0 = str.p_alloc;
		pS = pS0+aSTRT[0];
		nS = 0;
		for( U8 h = 0; h < nH; h++ ) {
			switch( pH[h] )
			{
				case '_':
				case '#':
					continue;
				default:
					break;
			}
			pS[nS]=pH[h];
			nS++;
		}
		if( pS[nS] )
			pS[nS] = 0;
		str.n_load = aSTRT[0]+nS+1;


		ix.lzyADD( NULL, sizeof(U4x4), aSTRT[1] = -1 );
		U4 nIX = (aSTRT[1]/sizeof(U4x4));
		U4x4	*p_ix0 = ((U4x4*)ix.p_alloc);

		pIX = p_ix0 + nIX;
		pIX->null();
		pIX->x = aSTRT[0];
		pIX->y = nS;
		U4 iIX, nADD = p_ix0->dctADD( pS0, iIX, *pIX );

		if( nIX == nADD )
		{
			// UNDOza a bejegyzést
			str.n_load = aSTRT[0];
			ix.n_load = aSTRT[1];
			pIX = p_ix0+iIX;
			return this;
		}
		ver++;
		pIX = p_ix0+nIX;
		return this;
	}

	U4 dctMILLfndnS( U1* pS, U8& nS, U4& nIX ) {
		if( !this )
		{
			nIX = 0;
			return 0;
		}
		U8 aSTRT[2]; // = -1;
		if( !str.p_alloc )
			ver = 0;
		// bemásolja a str szótárba
		/// ebböl kéne akor csinálni egy olyat
		/// ami ki szedi a '_#' jeleket
		U1* pH = pS;
		U8 nH = nS;
		str.lzyADD( pH, nH+1, aSTRT[0] = -1 );
		U1* pS0 = str.p_alloc;
		pS = pS0+aSTRT[0];
		nS = 0;
		for( U8 h = 0; h < nH; h++ ) {
			switch( pH[h] )
			{
				case '_':
				case '#':
					continue;
				default:
					break;
			}
			pS[nS]=pH[h];
			nS++;
		}
		if( pS[nS] )
			pS[nS] = 0;
		str.n_load = aSTRT[0]+nS+1;

		ix.lzyADD( NULL, sizeof(U4x4), aSTRT[1] = -1 );
		nIX = (aSTRT[1]/sizeof(U4x4));
		U4x4 *p_ix0 = ((U4x4*)ix.p_alloc);

		pIX = p_ix0 + nIX;
		pIX->null();
		pIX->x = aSTRT[0];
		pIX->y = nS;

		U4 iIX	= p_ix0->dctFND( pS0, *pIX );
		// UNDOza a bejegyzést
		str.n_load = aSTRT[0];
		ix.n_load = aSTRT[1];
		pIX = p_ix0+iIX;
		return iIX;
	}
	gpcLZYdct* dctMILLaddS( U1* pS, U8 nS ) {
		if( !this ) {
			gpcLZYdct* p_this = new gpcLZYdct(0);
			return p_this->dctADD( pS, nS );
		}
		U8 aSTRT[2]; // = -1;
		if( !str.p_alloc )
			ver = 0;
		// bemásolja a str szótárba
		/// ebböl kéne akor csinálni egy olyat
		/// ami ki szedi a '_#' jeleket
		U1* pH = pS;
		U8 nH = nS;
		str.lzyADD( pH, nH+1, aSTRT[0] = -1 );
		U1* pS0 = str.p_alloc;
		pS = pS0+aSTRT[0];
		nS = 0;
		for( U8 h = 0; h < nH; h++ ) {
			switch( pH[h] )
			{
				case '_':
				case '#':
					continue;
				default:
					break;
			}
			pS[nS]=pH[h];
			nS++;
		}
		if( pS[nS] )
			pS[nS] = 0;
		str.n_load = aSTRT[0]+nS+1;


		ix.lzyADD( NULL, sizeof(U4x4), aSTRT[1] = -1 );
		U4 nIX = (aSTRT[1]/sizeof(U4x4));
		U4x4	*p_ix0 = ((U4x4*)ix.p_alloc);

		pIX = p_ix0 + nIX;
		pIX->null();
		pIX->x = aSTRT[0];
		pIX->y = nS;
		U4 iIX, nADD = p_ix0->dctADD( pS0, iIX, *pIX );

		if( nIX == nADD )
		{
			// UNDOza a bejegyzést
			str.n_load = aSTRT[0];
			ix.n_load = aSTRT[1];
			pIX = p_ix0+iIX;
			return this;
		}
		ver++;
		pIX = p_ix0+nIX;
		return this;
	}
	U4 x( void ) {
		if( this ? !ix.n_load : true )
			return 0;

		return pIX-(U4x4*)ix.p_alloc;
	}
	U4 i_str(void) { return pIX ? pIX->x : str.n_load; }
	char* p_str( void ) { return (char*)str.p_alloc+pIX->x; }
	U8 nIX(void) { return this ? (ix.n_load / sizeof(U4x4)) : 0; }
	const char* sSTRix( U8 iX, const char* pER ) {
		if( !this )
			return pER;
		U4x4	*p_ix0 = ((U4x4*)ix.p_alloc);
		return (char*)(str.p_alloc + p_ix0[iX].x);
	}
	U4 nSTRix( U8 x ) {
		if( this ? x >= nIX() : true )
			return 0;

		return ((U4x4*)ix.p_alloc)[x].y;
	}
};
#define UX U4
class gpcLZYdctBIN {
	U4x4*		pIX; /// BEST!!!
public:
	gpcLZY		bin,
				ix;
	U8			ver;

	gpcLZYdctBIN(){};
	gpcLZYdctBIN(U4 n) { gpmCLR; }
	void rst() {
		if(!this)
			return;
		pIX = NULL;
		bin.lzyRST();
		ix.lzyRST();
	}
	U4 dctFND( U1* pB, U4 nB, U4& nIX ) {
		if( !this )
		{
			nIX = 0;
			return 0;
		}
		U8 aSTRT[2]; // = -1;
		if( !bin.p_alloc )
			ver = 0;
		// bemásolja a bin-t szótárba
		bin.lzyADD( pB, nB, aSTRT[0] = -1 );

		U1	*pB0 = bin.p_alloc,
			*pB1 = pB0+aSTRT[0];

		ix.lzyADD( NULL, sizeof(U4x4), aSTRT[1] = -1 );
		nIX = (aSTRT[1]/sizeof(U4x4));
		U4x4 *p_ix0 = ((U4x4*)ix.p_alloc);

		pIX = p_ix0 + nIX;
		pIX->null();
		pIX->x = aSTRT[0];
		pIX->y = nB;

		U4 iIX = p_ix0->dctFNDn( pB0, *pIX );

		// UNDOza a bejegyzést
		bin.n_load = aSTRT[0];
		ix.n_load = aSTRT[1];
		pIX = p_ix0+iIX;
		return iIX;
	}
	gpcLZYdctBIN* dctADD( U1* pB, U8 nB ) {
		if( !this ) {
			gpcLZYdctBIN* p_this = new gpcLZYdctBIN(0);
			return p_this->dctADD( pB, nB );
		}
		U8 aSTRT[2]; // = -1;
		if( !bin.p_alloc )
			ver = 0;
		// bemásolja a bin-t szótárba
		bin.lzyADD( pB, nB, aSTRT[0] = -1 );

		U1	*pB0 = bin.p_alloc,
			*pB1 = pB0+aSTRT[0];

		ix.lzyADD( NULL, sizeof(U4x4), aSTRT[1] = -1 );
		U4 nIX = (aSTRT[1]/sizeof(U4x4));
		U4x4	*p_ix0 = ((U4x4*)ix.p_alloc);

		pIX = p_ix0 + nIX;
		pIX->null();
		pIX->x = aSTRT[0];
		pIX->y = nB;
		U4 best, nADD = p_ix0->dctADDn( pB0, best, *pIX );

		if( nIX == nADD )
		{
			// UNDOza a bejegyzést
			bin.n_load = aSTRT[0];
			ix.n_load = aSTRT[1];
			pIX = p_ix0+best;
			return this;
		}
		ver++;
		pIX = p_ix0+nIX;
		return this;
	}

	U4 x( void ) {
		if( this ? !ix.n_load : true )
			return 0;

		return pIX-(U4x4*)ix.p_alloc;
	}
	U4 iBIN(void) {
		if( !pIX )
			return bin.n_load;

		return pIX->x;
	}
	void* pBINix( void ) {
		return bin.p_alloc+pIX->x;
	}
	U8 nIX(void) {
		return this ? (ix.n_load / sizeof(U4x4)) : 0;
	}
	const void* pBINi( U8 i, const void* pER ) {
		if( !this )
			return pER;
		U4x4	*p_ix0 = ((U4x4*)ix.p_alloc);
		return bin.p_alloc + p_ix0[i].x;
	}
	U4 nBINix( U8 iX ) {
		if( this ? iX >= nIX() : true )
			return 0;

		return ((U4x4*)ix.p_alloc)[iX].y;
	}
};


class gpcLZYall {
	gpcLZY	lzyID,
			**ppLZY, *pLZY;
	I8x2	iAN;
	U4		nLZYall, iLZYfr, nLZYld, iLZY;

	I8x2 rdAN( U4 i )
	{
		if( i >= nLZYld )
			return I8x2(0,0);
		return ((I8x2*)lzyID.p_alloc)[i];
	}
	I8x2& wrAN( U4 i )
	{
		U8 s = i*sizeof(iAN);
		lzyID.lzy_exp_rand( s, sizeof(iAN), sizeof(iAN) );
		return ((I8x2*)lzyID.p_alloc)[i];
	}
public:
	U1 aPUB[0x1000];


	//U4 alfFND( U1* pS );
	gpcLZY*	LZYi( U4 i ) {
		if( pLZY ? iLZY == i : false )
			return pLZY;

		if( i < nLZYall )
		{
			if( ppLZY[i] )
			{
				iLZY = i;
				iAN = rdAN(iLZY);
				return pLZY = ppLZY[iLZY];
			}
		}
		return NULL;
	}

	gpcLZY*	LZY( I8x2 an ) {
		if( pLZY ? (iAN==an) : false )
			return pLZY;

		pLZY = NULL;
		if( iLZYfr > nLZYld )
			iLZYfr = nLZYld;
		else if( !ppLZY )
			iLZYfr = nLZYld = nLZYall = 0;
		else if( ppLZY[iLZYfr] )
			iLZYfr = nLZYld;

		for( U4 p = 0; p < nLZYld; p++ )
		{
			if( !ppLZY[p] )
			{
				if( iLZYfr > p )
					iLZYfr = p;
				continue;
			}

			if( rdAN(p) != an )
				continue;
			iAN = an;
			pLZY = ppLZY[p];
			return pLZY;
		}
		if( iLZYfr >= nLZYld )
		{
			nLZYld++;
			if( nLZYld >= nLZYall )
			{
				nLZYall += 0x10;
				gpcLZY	**ppKILL = ppLZY;
				ppLZY = new gpcLZY*[nLZYall];
				gpmMcpyOF( ppLZY, ppKILL, iLZYfr ); // mert nGTld == iGTfr+1
				gpmDELary(ppKILL);
			}
		}
		wrAN(iLZYfr) = iAN = an;
		return ppLZY[iLZYfr] = pLZY = new gpcLZY();
	}
};

#endif // piMASS_H_INCLUDED
