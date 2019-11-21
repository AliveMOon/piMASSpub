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
#include "mysys.h"
#ifdef _WIN64

	#pragma once
	#define _CRT_SECURE_NO_WARNINGS
	#pragma warning( push )
	#pragma warning( disable : 4995 )

	#include <stdlib.h>
	#include <io.h>
	#include <direct.h>
	#include <malloc.h>
	#include <memory.h>
	#include <WinSock2.h>
	#include <ws2tcpip.h>
	#define bzero( a, n ) ( (void*)memset( a, 0, n ) )
	#define gpdMAX_PATH _MAX_PATH
	//#define close( h ){ if( h != INVALID_SOCKET ){ closesocket( h ); h = INVALID_SOCKET; } }
	#define ace _access
	#define mkd( a, b ) ( _mkdir( a ) )
	#define usleep Sleep
	#define sleep Sleep
	#define gpmALLOC( n ) ((U1*)_mm_malloc( n, 0x10 ))
	#define gpmFREE( p ) _aligned_free( p )
	#define gpmFD_CLOSE( h ){ if( h ){ CloseHandle( h ); h = 0; } }
	#define gpmFF_CLOSE( h ){ if( h ){ _findclose( h ); h = -1L; } }

	#pragma comment (lib, "iphlpapi.lib")
	#pragma comment (lib, "ws2_32.lib")


#else

	//#include <opencv2/core/core.hpp>
	//#include <opencv2/highgui/highgui.hpp>

	#ifdef gpdSYSpi
		#include <raspicam/raspicam.h>
		//#include <raspicam/raspicam_cv.h>
		#include <wiringPi.h>
	#endif // gpdSYSpi

	#include <unistd.h> // for usleep()

	#include <sys/socket.h>
	#include <sys/ioctl.h>
	#include <sys/select.h>
	#include <sys/stat.h>

	#include <netdb.h>
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
	#include <GL/glew.h>

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
	#define gpmSDL_FreeSRF( h ){ if( h ){ SDL_FreeSurface( h ); h = NULL; } }

	#define gpdMAX_PATH PATH_MAX

	#define gpdRPI_WIDTH	1280	//320 // 640		//  Allowable widths: 320, 640, 1280
	#define gpdRPI_HEIGHT	960	//240 // 480		//   Allowable heights: 240, 480, 960
	#define gpdSDL_tOUT		5
	#define gpdRPI_tOUT		5
#endif



#define gpdGT_LIST_tOUT 5
#define gpdPICbg "/mnt/ram/bg.png"

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

#include "jpeglib.h"
#include <setjmp.h>

#include <SDL.h>			//-lSDL2
#include <SDL_image.h>
#include <math.h>
// Include GLFW
//#include <glfw3.h>


/*#include "gpcSCHL.h"
#include "gpcOPCD.h"*/

using namespace std;

#define NULL nullptr

typedef unsigned char	U1;
typedef volatile U1		vU1;
typedef unsigned short	U2;
typedef uint32_t		U4;	//unsigned int	U4;
typedef uint64_t		U8;

//typedef float		float;
typedef double		F8;

typedef signed char		I1;
typedef signed short	I2;
typedef int				I4;
typedef int64_t			I8;
typedef I8				LL;




class float2;
class float3;
class float4;		// ez a F4 négyes singleP vektor
class float4x4;		// ja ez itt a matrix lenne
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

//#define gpm_bool( b, p ) if( b ){ (p) }

#define gpdGT_NoDALEY 1
#define INVALID_SOCKET  (SOCKET)(~0)
#define SOCKET_ERROR            (-1)


#define frac( a ) ( (a)-floor((a)) )


//#define gpdMAX_PATH			0x110
#define max( a, b ) ( a > b ? a : b )
#define min( a, b ) ( a < b ? a : b )
#define PI acos(-1.0)
#define COS5 0.99619469809174553229501040247389
#define SIN5 0.08715574274765817355806427083747
#define RAD5 (5.0*PIp180)
#define COSSIN45 0.70710678118654752440084436210485
#define gpdSQRT2 1.4142135623730950488016887242097
#define gpdSQRT3 1.7320508075688772935274463415059f
#define gpmNINCS( s, v )	strspn( (char*)(s), (char*)(v) )
#define gpmSTR2D( p )	strtod( (char*)(p), (char**)&(p) )
#define gpmSTR2U8( p, r )	strtol( (char*)(p), (char**)&(p), (r) )
#define gpdVAN	strcspn
#define gpmPAD( n, p ) ( (n) + (((n)%(p)) ? ((p)-((n)%(p))) : 0) )
#define gpmCLR	if( this ) bzero( this, sizeof(*this) )
#define gpmN( p ) ( sizeof(p)/sizeof(*p) )
#define gpmZ( p ) bzero( &p, sizeof(p) )
#define gpmZn( p, n ) bzero( (p), (n)*sizeof(*(p)) )

#define gpmOFF( st, m ) ( (size_t)(&((st*)NULL)->m) )
#define gpmOFFOFF(st, m,n) (GPM_OFF(st, m)-GPM_OFF(st, n))

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
#define gpmMEMCPY( d, s, n ) \
			(														\
				( (n)&&(d)&&(s)&&(((char*)(d))!=((char*)(s))) ) 	\
				? ( (char*)memcpy( (d), (s), ((n)*sizeof(*(d))) ) )		\
				: ( (char*)d )  									\
			)
#define gpmSTRnCPY( d, s, n ) \
			(														\
				( (n)&&(d)&&(s)&&(((char*)(d))!=((char*)(s))) ) 	\
				? ( (U1*)strncpy( (char*)(d), (char*)(s), (n) ) )  		\
				: ( (U1*)d )  									\
			)

#define gpmSTRLEN( s )			((s)? strlen((char*)(s)) : 0)

#define gpmDEL( p ){ if( (p) ){ delete (p); (p) = NULL; } }
#define gpmDELary( p ){ if( (p) ){ delete[] (p); (p) = NULL; } }
#define gpmMEMCMP (U1*)gp_memcmp
#define gpmUnB( u )		( abs(u)>0x7fFF ? (abs(u)>0x7fffFFFF ? 8 : 4) : (abs(u)>0x7f ? 2 : 1)  )
#define gpmFnB( f )		( abs(f)>0xffFFFF ? 8 : 4)		// float 23bit felbontású
#define gpmSHnB( b )	( b>2 ? (b>4 ? 3 : 2) : (b>1 ? 1 : 0)  )
//#define gpmbABC( c ) (c < 0x80 ? gpaALFadd[c] : true)
SOCKET inline gpfSOC_CLOSE( SOCKET& h )
{
	if( h == INVALID_SOCKET )
		return INVALID_SOCKET;

	close( h );
	h = INVALID_SOCKET;
	return h;
}

U8 inline gpfABCnincs( const U1* p_str, const U1* pE, U8& nUTF8, const U1* gpaALFadd )
{
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
		if( !gpaALFadd[*pS] )
			break;

		nUTF8++;
		pS++;
	}
	return pS-p_str;
}
#define gpdSTR_READ( x ) ( (x) ? (x) : "" )

#define MAKE_ID( a,b,c,d )				\
	(									\
		  (((U4)d)<<24)					\
		| (((U4)c)<<16)					\
		| (((U4)b)<< 8)					\
		| ((U4)a)						\
	)
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
	gpeLZY1,
	gpeLZY2,
	gpeLZY3,
	gpeLZY4,
	gpeLZYwipSTK,
	gpeLZYwip,
	gpeLZYxN,
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


enum gpeFD
{
	gpeFD_recv,
	gpeFD_send,
	gpeFD_excp,
	gpeFD_n,

};


enum gpeLX:U8
{
    gpeZERO,
    gpeMXPATH = gpmPAD( PATH_MAX, 0x10 ) , //0x10*0x11,
    gpeRECVn = (0x30000/12),
};

#include "gpeALF.h"

inline void* gpfMEMSET( void* pD, U8 n, void* pS, U8 nS )
{
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

	U8 a,b;
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
int inline gpfACE( const char* p_file, I4 mode )
{
	//	00	Existence only
	//	02	Write-only
	//	04	Read-only
	//	06	Read and write
	if( !p_file )
		return -1; // ha negatív nem elérhetõ

	int io = ace( p_file, mode );
	return io;
}
bool inline gpfMKDR( char* p_buff, const char* p_new )
{
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
inline char* gpfP2F( char* p2P, char* p2F, const char* pS, char c = '/' )
{
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
inline void* gp_memcmp( U1* pA, U1* pB, U8 n )
{
	if( !pA || !pB )
		return pA;

	if( pA == pB )
		return (void*)(pA+n);

	U1	*pAu1 = (U1*)pA,
		*pBu1 = (U1*)pB;
	for( U8 i = 0; i < n; i++ )
	{
		if( n-i > 8 )
		{
			if( *(U8*)(pAu1+i) == *(U8*)(pBu1+i) )
			{
				i += 7;
				continue;
			}
		}
		if( n-i > 4 )
		{
			if( *(U4*)(pAu1+i) == *(U4*)(pBu1+i) )
			{
				i += 3;
				continue;
			}
		}

		if( pAu1[i] == pBu1[i] )
			continue;

		return (void*)(pAu1+i);
	}
	return (void*)(pAu1+n);
}

class UTF8
{
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

	UTF8& operator += ( U8 n )
	{
		if( pBK() )
		{
			for( U8 i = 0; i < n; i++ )
			{
				if( !*pU )
					break;

				pU++;
				while( *pU & 0x80 )
				{
					if( (*pU & 0xc0) == 0xc0 )
						break;
					pU++;
				}
			}
		}
		return *this;
	}

	UTF8& operator -= ( U8 n )
	{
		if( pBK() )
		{
			for( U8 i = n; i; i++ )
			{
				if( !*pU )
					break;

				pU++;
				pBK();
			}
		}
		return *this;
	}

};
enum gpeISA : I1
{
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

class U1x4
{
public:
    union
    {
        struct
        {
            U1 x,y,z,w;
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
    U1x4& null()
    {
		u4 = 0;
		return *this;
    }
    U1x4(){};
    U1x4( U4 b )
    {
        u4 = b;
    }
    U1x4( U1 _x, U1 _y, U1 _z = 0, U1 _w = 0 )
    {
        x = _x; y = _y; z = _z; w = _w;
    }
    U1x4& str2time( U1* p_str, U1* p_end, U1** pp_str = NULL );
    U4 area_xy()
    {
		U4 a = x;
		a *= y;
		return a;
    }
    U4 area_yz()
    {
		U4 a = y;
		a *= z;
		return a;
    }
    U4 area_zw()
    {
		U4 a = z;
		a *= w;
		return a;
    }
    U4 area()
    {
		U4 a = x;
		a *= y;
		a *= z;
		a *= w;
		return a;
    }
    U4 area_xyz()
    {
		U4 a = x;
		a *= y;
		a *= z;
		return a;
    }

    U1x4& mx( U1x4 xyzw )
    {
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
    U1x4& mn( U1x4 xyzw )
    {
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
	U1x4& typ()
	{
		if( w )
			return *this;

		if( !y )
			y = 1;
		if( !z )
			z = 1;

		if( x&0x40 )
			x = ((x&0xf) < 2 ) ? 0xc2 : 0xc3;

		w = 1<<(x&0xf);
		return *this;
	}
	U1x4& typMX( U1x4 tp )
	{
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

		x = tp.x|min(3,sh1);
		w = 1<<(x&0xf);

		// legalább 1x1 es valamikből áll
		if( y < tp.y )
			y = tp.y;
		if( z < tp.z )
			z = tp.z;

		return *this;
	}
	U1* typ2str( U1* pBUFF );	// x[7s,6f,5r,4str : 3-0 nBYTE = 1<<(x&0xf) ] // yz dimxy

};

class I1x4
{
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


class U2x2
{
public:
	union
	{
		struct
		{
			U4 x,y;
		};
		struct
		{
			U1x4 aCLR[2];
		};
		struct
		{
			U4 u4;
		};
	};

    U2x2(){};
    U2x2( U2 _x, U2 _y = 0 )
    {
        x = _x; y = _y;
    }
	// cnt = fract * U4x2(1, w);
	/*U4x2& cnt2fract(U4 w, U8 cnt)
	{
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
	}*/

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


	U4 sum( void ) const
	{
		return (U4)x+y;
	}

	U4 area( void )
	{
		return x*y;
	}
	U4 are_sum( void )
	{
		return area()+sum();
	}

	U4 qlen(void ) const
	{
		return x*x+y*y;
	}

	U2 mn( void ) const
	{
		return x < y ? x:y;
	}

	U2 mx( void ) const
	{
		return x > y ? x:y;
	}

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

class U2x4
{
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
    char* str( char* pBUFF, const char* pSP = ", "  )
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

	/*U4	dict_add( U1* p_src, U4& m, U4x4& w );
	U4  dict_find( U1* p_src, U4x4& w );*/
};

class U4x2
{
public:
	union
	{
		struct
		{
			U4 x,y;
		};
		struct
		{
			gpeALFu4	a4;
			U4			n4;
		};
		struct
		{
			U1x4 aCLR[2];
		};
		struct
		{
			gpeALF var;
		};
		struct
		{
			U8 u8;
		};
		struct
		{
			U1* aSTR[2];
		};
		struct
		{
			double d8;
		};
	};

    U4x2(){};
    U4x2( U4 _x, U4 _y = 0 )
    {
        x = _x; y = _y;
    }
    U4x2( const I8x2& an );
    U4x2( const I4x2& i42 );

    U4x2& operator = ( const I8x2& an );
	// cnt = fract * U4x2(1, w);
	U1* strA4N( U1* pBUFF )
    {
		sprintf( (char*)(pBUFF+gpfALF2STR( pBUFF, a4 )), "%d", n4 );
		return pBUFF;
    }
    U1* strVAR( U1* pBUFF )
    {
		gpfALF2STR( pBUFF, var );
		return pBUFF;
    }
	U1* str( U1* pBUFF, const char* pSP = ", " )
    {
		sprintf( (char*)pBUFF, "%d%s%d%s", x,pSP,y,pSP );
		return pBUFF;
    }
	U4x2& cnt2fract(U4 w, U8 cnt)
	{
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
	U4x2& null( void )
	{
		gpmCLR;
		return *this;
	}
	U4x2& operator += ( const U4x2& b )
	{
		x += b.x;
		y += b.y;
		return *this;
	}
	U4x2& operator -= ( const U4x2& b )
	{
		x -= b.x;
		y -= b.y;
		return *this;
	}
	U4x2& operator &= ( const U4x2& u )
	{
		x *= u.x;
		y *= u.y;
		return *this;
	}
	U4x2& operator /= ( const U4x2& u )
	{
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
	U4x2& operator %= ( const U4x2& u )
	{
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
	U4x2& operator += ( U4 u )
	{
		x += u;
		y += u;
		return *this;
	}
	U4x2& operator += ( I4 i )
	{
		x += i;
		y += i;
		return *this;
	}
	U4x2& operator += ( float f )
	{
		x += f;
		y += f;
		return *this;
	}

	U4x2& operator -= ( U4 u )
	{
		x -= u;
		y -= u;
		return *this;
	}
	U4x2& operator -= ( I4 i )
	{
		x -= i;
		y -= i;
		return *this;
	}
	U4x2& operator -= ( float f )
	{
		x -= f;
		y -= f;
		return *this;
	}
	U4x2& operator = ( int i )
	{
		if( !i )
			return null();

		x = y = i;
		return *this;
	}

	U4x2& operator = ( const I4x2& b );


	U4x2 operator + (const U4 b) const
	{
		return U4x2( x+b, y+b );
	}
	U4x2 operator + (const U4x2 b) const
	{
		return U4x2( x+b.x, y+b.y );
	}

	U4x2 operator - (const U4 b) const
	{
		return U4x2( x-b, y-b );
	}
	U4x2 operator - (const U4x2 b) const
	{
		return U4x2( x-b.x, y-b.y );
	}

	bool operator != ( const U4x2& b ) const
	{
		if( x != b.x )
			return true;
		return y != b.y;
	}
	bool operator == ( const U4x2& b ) const
	{
		return !(*this!=b);
	}

	U8 operator * (const U4x2& b) const
	{
		return (U8)x*b.x + (U8)y*b.y;
	}
	U4x2 operator & (const U4x2& b) const
	{
		return U4x2( x*b.x, y*b.y );
	}
	U4x2& operator *= ( U4 i )
	{
		if( !i )
			return null();
		if( i == 1 )
			return *this;

		x*=i;
		y*=i;

		return *this;
	}
	U4x2& operator /= ( U4 i )
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
	U4x2& operator %= ( U4 i )
	{
		if( i == 1 )
			return null();

		if( !i )
			return *this;


		x%=i;
		y%=i;

		return *this;
	}


	U8 sum( void ) const
	{
		return (U8)x+y;
	}

	U8 area( void )
	{
		return x*y;
	}
	U8 are_sum( void )
	{
		return area()+sum();
	}

	U8 qlen(void ) const
	{
		return x*x+y*y;
	}

	U4 mn( void ) const
	{
		return x < y ? x:y;
	}

	U4 mx( void ) const
	{
		return x > y ? x:y;
	}

	U4x2 abs( void ) const
	{
		return U4x2( x<0?-x:x, y<0?-y:y );
	}
	U4x2& mx( U4x2 b )
	{
		if( x < b.x )
			x = b.x;
		if( y < b.y )
			y = b.y;
		return *this;
	}

	U4x2& mn( const U4x2& b )
	{
		if( x > b.x )
			x = b.x;
		if( y > b.y )
			y = b.y;
		return *this;
	}
	U4x2& mx( I4x2 b );
	U4x2& mn( I4x2 b );

};



class U4x4
{
public:
    union
    {
        struct
        {
            U4 x,y,z,w;
        };
        struct
        {
			U4x2 a4x2[2];
		};
        struct
        {
            U4 aXYZW[4];
        };

    };

    U4x4(){};
    U4x4& null( void )
	{
		gpmCLR;
		return *this;
	}
    U4x4( U4 _x, U4 _y = 0, U4 _z = 0, U4 _w = 0 )
    {
        x = _x; y = _y; z = _z; w = _w;
    }
	U4x4( const I4x2 _xy, I4x2* p_zw = NULL );
    U4x4( U4x2& _xy, U4x2* p_zw = NULL )
    {
        a4x2[0] = _xy;
        a4x2[1] = p_zw ? *p_zw : _xy;
    }
    U4x4( U4x2& _xy, U4x2& _zw )
    {
        a4x2[0] = _xy;
        a4x2[1] = _zw;
    }
    U4x4& operator = ( U4 b )
    {
        x = y = z = w = b;
    }
    U4x4& operator = ( I4 b )
    {
        x = y = z = w = abs(b);
    }
    U4x4& operator = ( const U4x2& b )
    {
		a4x2[1] = a4x2[0] = b;
		return *this;
    }
    U4x4& operator = ( const I4x2& b );


    U4x4& str2date( U1* p_str, U1* p_end, U1** pp_str = NULL );
    char* str( char* pBUFF, const char* pSP = ", "  )
    {
		sprintf( pBUFF, "%d%s%d%s%d%s%d%s", x,pSP,y,pSP,z,pSP,w,pSP );
		return pBUFF;
    }

    bool operator != ( const U4x4& b ) const
	{
		if( a4x2[0] != b.a4x2[0] )
			return true;
		return a4x2[1] != b.a4x2[1];
	}
	bool operator == ( const U4x4& b ) const
	{
		return !(*this!=b);
	}


	U4x4& operator += ( const U4x4& b )
	{
		a4x2[0] += b.a4x2[0];
		a4x2[1] += b.a4x2[1];
		return *this;
	}
	U4x4& operator -= ( const U4x4& b )
	{
		a4x2[0] -= b.a4x2[0];
		a4x2[1] -= b.a4x2[1];
		return *this;
	}

	U4x4& operator += ( const U4x2& b )
	{
		a4x2[0] += b;
		a4x2[1] += b;
		return *this;
	}
	U4x4& operator -= ( const U4x2& b )
	{
		a4x2[0] -= b;
		a4x2[1] -= b;
		return *this;
	}

	U4x4& operator += ( U4 u )
	{
		a4x2[0] += u;
		a4x2[1] += u;
		return *this;
	}
	U4x4& operator += ( I4 i )
	{
		a4x2[0] += i;
		a4x2[1] += i;
		return *this;
	}
	U4x4& operator += ( float f )
	{
		a4x2[0] += f;
		a4x2[1] += f;
		return *this;
	}

	U4x4& operator -= ( U4 u )
	{
		a4x2[0] -= u;
		a4x2[1] -= u;
		return *this;
	}
	U4x4& operator -= ( I4 i )
	{
		a4x2[0] -= i;
		a4x2[1] -= i;
		return *this;
	}
	U4x4& operator -= ( float f )
	{
		a4x2[0] -= f;
		a4x2[1] -= f;
		return *this;
	}

    U4x4 zwxy( void )  const
    {
		return U4x4( z,w, x,y );
    }

	U4 tree_add( U4 u4, U4 n_t )
	{
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
	U4 tree_fnd( U4 u4, U4 n_t )
	{
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

	U4	dict_add( U1* p_src, U4& m, U4x4& w );
	U4  dict_find( U1* p_src, U4x4& w );
};

class U8x4
{
public:
    union
    {
        struct
        {
            U8 x,y,z,w;
        };
        struct
        {
            U8 aXYZW[4];
        };
        struct
        {
            gpeALF labe;
            U8 mom, up, nx;
        };
    };
    U8x4(){};
    U8x4( U8 _x, U8 _y = 0, U8 _z = 0, U8 _w = 0 )
    {
        x = _x; y = _y; z = _z; w = _w;
    }
    U8x4& operator = ( U8 b )
    {
		gpmCLR;
        x = b;
        return *this;
    }
    U8x4& operator = ( I8 b )
    {
		gpmCLR;
        x = abs(b);
        return *this;
    }
    U8x4& null( void )
	{
		gpmCLR;
		return *this;
	}
    U8 tree_add( U8 u8, U8 n_t )
	{
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
	U8 tree_fnd( U8 u8, U8 n_t )
	{
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
	U8 tree_fndHARD( U8 u8, U8 n_t )
	{
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

class I4x2
{
public:
    I4 x,y;

    I4x2(){};
    I4x2( I4 _x, I4 _y = 0 )
    {
        x = _x; y = _y;
    }
	I4x2( U4x2 b )
    {
        x = b.x;
        y = b.y;
    }
	// cnt = fract * U42(1, w);
	I4x2& cnt2fract(U4 w, U8 cnt)
	{
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
	I8 operator * (const I4x2& b) const
	{
		return (I8)x*b.x + (I8)y * b.y;
	}
	I4x2 operator & (const I4x2& b) const
	{
		return I4x2( x*b.x,  y*b.y );
	}
	I4x2 operator & (const U4x2& b) const
	{
		return I4x2( x*b.x,  y*b.y );
	}

	I4x2 operator % (const I4x2& b) const
	{
		return I4x2( b.x ? x/b.x : 0,  b.y ? y/b.y : 0 );
	}
	I4x2 operator / (const I4x2& b) const
	{
		return I4x2( b.x ? x/b.x : 0x7fffffff ,  b.y ? y/b.y : 0x7fffffff );
	}
	I4x2 operator / (const U4x2& b) const
	{
		return I4x2( b.x ? x/b.x : 0x7fffffff,  b.y ? y/b.y : 0x7fffffff );
	}

	I4x2 operator + (const I4x2& b) const
	{
		return I4x2( x+b.x,  y+b.y );
	}
	I4x2 operator + (const U4x2& b) const
	{
		return I4x2( x+b.x,  y+b.y );
	}

	I4x2 operator - (const I4x2& b) const
	{
		return I4x2( x-b.x,  y-b.y );
	}
	I4x2 operator - (const U4x2& b) const
	{
		return I4x2( x-b.x,  y-b.y );
	}
	bool operator != ( const I4x2& b ) const
	{
		if( x != b.x )
			return true;
		return y != b.y;
	}
	bool operator == ( const I4x2& b ) const
	{
		return !(*this!=b);
	}

	I4x2& operator *= ( I4 i )
	{
		if( !i )
			return null();
		if( i == 1 )
			return *this;

		x*=i;
		y*=i;

		return *this;
	}
	I4x2& operator /= ( I4 i )
	{
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
	I4x2& operator %= ( I4 i )
	{
		if( i == 1 )
			return null();

		if( !i )
			return *this;


		x%=i;
		y%=i;

		return *this;
	}
	I4x2& null( void )
	{
		gpmCLR;
		return *this;
	}

	I4x2& operator += ( const U4x2& u )
	{
		I4x2 b = u;
		x += b.x;
		y += b.y;
		return *this;
	}
	I4x2& operator -= ( const U4x2& u )
	{
		I4x2 b = u;
		x -= b.x;
		y -= b.y;
		return *this;
	}


	I4x2& operator &= ( const I4x2& i )
	{
		x *= i.x;
		y *= i.y;
		return *this;
	}
	I4x2& operator /= ( const I4x2& i )
	{
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
	I4x2& operator %= ( const I4x2& i )
	{
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


	I4x2& operator += ( const I4x2& b )
	{
		x += b.x;
		y += b.y;
		return *this;
	}
	I4x2& operator -= ( const I4x2& b )
	{
		x -= b.x;
		y -= b.y;
		return *this;
	}
	I4x2& operator += ( U4 u )
	{
		if( u > 0x7fffFFFF )
			u = 0x7fffFFFF;
		x += u;
		y += u;
		return *this;
	}
	I4x2& operator += ( I4 i )
	{
		if( i < 0 )
			i *= -1;
		x += i;
		y += i;
		return *this;
	}
	I4x2& operator += ( float f )
	{
		x += f;
		y += f;
		return *this;
	}

	I4x2& operator -= ( U4 u )
	{
		if( u > 0x7fffFFFF )
			u = 0x7fffFFFF;
		x -= u;
		y -= u;
		return *this;
	}
	I4x2& operator -= ( I4 i )
	{
		if( i < 0 )
			i *= -1;
		x -= i;
		y -= i;
		return *this;
	}
	I4x2& operator -= ( float f )
	{
		x -= f;
		y -= f;
		return *this;
	}
	I4x2& operator = ( int i )
	{
		if( !i )
			return null();

		x = y = i;
		return *this;
	}

	U1* strA4N( U1* pBUFF )
    {
		sprintf( (char*)(pBUFF+gpfALF2STR( pBUFF, x )), "%d", y );
		return pBUFF;
    }

	I4x2& operator = ( const U4x2& b )
	{
		x = b.x > 0x7fffFFFF ? 0x7fffFFFF : b.x;
		y = b.y > 0x7fffFFFF ? 0x7fffFFFF : b.y;
		return *this;
	}

	I8 sum( void ) const
	{
		return (I8)x+y;
	}

	I8 area( void ) const
	{
		return x*y;
	}

	U8 are_sum( void )
	{
		return abs().area()+abs().sum();
	}

	U8 qlen (void ) const
	{
		return x*x + y*y;
	}

	I4 mn( void )
	{
		return x < y ? x:y;
	}

	I4 mx( void )
	{
		return x > y ? x:y;
	}

	I4x2 abs( void )
	{
		return I4x2( x<0?-x:x, y<0?-y:y );
	}
	I4x2& mx( I4x2 b )
	{
		if( x < b.x )
			x = b.x;
		if( y < b.y )
			y = b.y;
		return *this;
	}

	I4x2& mn( const I4x2& b )
	{
		if( x > b.x )
			x = b.x;
		if( y > b.y )
			y = b.y;
		return *this;
	}
};

class I4x4
{
public:
	union
    {
        struct
        {
			I4 x,y,z,w;
		};
		struct
        {
			I4x2 a4x2[2];
		};

		struct
        {
			SDL_Rect xyWH;
        };

        struct
        {
			U4x2 au4x2[2];
        };
	};
    I4x4(){};
    I4x4( I4 _x, I4 _y = 0, I4 _z = 0, I4 _w = 0 )
    {
        x = _x; y = _y; z = _z; w = _w;
    }
	I4x4( I4x2 _xy, I4x2 _zw )
    {
        a4x2[0] = _xy;
        a4x2[1] = _zw;
    }
    I4x4( U4x2 _xy, U4x2 _zw )
    {
        a4x2[0] = _xy;
        a4x2[1] = _zw;
    }
    I4x4& operator = ( I4 i )
    {
		x = y = z = w = i;
		return *this;
    }
    I4x4& operator = ( I4x2 b )
    {
		a4x2[1] = a4x2[0] = b;
		return *this;
    }
	char* str( char* pBUFF, const char* pSP = ", "  )
    {
		sprintf( pBUFF, "%d%s%d%s%d%s%d%s", x,pSP,y,pSP,z,pSP,w,pSP );
		return pBUFF;
    }

    bool operator != ( const I4x4& b ) const
	{
		if( a4x2[0] != b.a4x2[0] )
			return true;
		return a4x2[1] != b.a4x2[1];
	}
	bool operator == ( const I4x4& b ) const
	{
		return !(*this!=b);
	}

	I4x4& operator += ( const I4x4& b )
	{
		a4x2[0] += b.a4x2[0];
		a4x2[1] += b.a4x2[1];
		return *this;
	}
	I4x4& operator -= ( const I4x4& b )
	{
		a4x2[0] -= b.a4x2[0];
		a4x2[1] -= b.a4x2[1];
		return *this;
	}

	I4x4& operator += ( const I4x2& b )
	{
		a4x2[0] += b;
		a4x2[1] += b;
		return *this;
	}
	I4x4& operator -= ( const I4x2& b )
	{
		a4x2[0] -= b;
		a4x2[1] -= b;
		return *this;
	}

	I4x4& operator += ( U4 u )
	{
		a4x2[0] += u;
		a4x2[1] += u;
		return *this;
	}
	I4x4& operator += ( I4 i )
	{
		a4x2[0] += i;
		a4x2[1] += i;
		return *this;
	}
	I4x4& operator += ( float f )
	{
		a4x2[0] += f;
		a4x2[1] += f;
		return *this;
	}

	I4x4& operator -= ( U4 u )
	{
		a4x2[0] -= u;
		a4x2[1] -= u;
		return *this;
	}
	I4x4& operator -= ( I4 i )
	{
		a4x2[0] -= i;
		a4x2[1] -= i;
		return *this;
	}
	I4x4& operator -= ( float f )
	{
		a4x2[0] -= f;
		a4x2[1] -= f;
		return *this;
	}

	I4x4 operator + (const I4x4& b) const
	{
		return I4x4( x+b.x, y+b.y, z+b.z, w+b.w );
	}
	I4x4 operator - (const I4x4& b) const
	{
		return I4x4( x-b.x, y-b.y, z-b.z, w-b.w );
	}




	U8 operator * (const I4x2& b) const
	{
		return a4x2[0]*b + a4x2[1]*b;
	}

	I4x4 operator & (const I4x2& b) const
	{
		return I4x4( a4x2[0]&b, a4x2[1]&b );
	}
	I4x4 operator & (const U4x2& b) const
	{
		return I4x4( a4x2[0]&b, a4x2[1]&b );
	}
	I4x4 operator / (const I4x4& b) const
	{
		return I4x4( a4x2[0]/b.a4x2[0], a4x2[1]/b.a4x2[1] );
	}
	I4x4 operator % (const I4x4& b) const
	{
		return I4x4( a4x2[0]%b.a4x2[0], a4x2[1]%b.a4x2[1] );
	}
	I4x4 operator / (const U4x4& b) const
	{
		return I4x4( a4x2[0]/b.a4x2[0], a4x2[1]/b.a4x2[1] );
	}

	I4x4 lurd( void )
	{
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

	I4x4& null( void )
	{
		gpmCLR;
		return *this;
	}

	I8 sum( void ) const
	{
		return (I8)x + y + z + w;
	}

	I8 volume( void ) const
	{
		return x*y*z*w;
	}

	I4x4 abs( void ) const
	{
		return I4x4( x<0 ? -x: x, y<0 ? -y: y, z<0 ? -z: z, w<0 ? -w: w );
	}
	I4x4 zwxy( void ) const
	{
		return I4x4( z, w, x, y );
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
			this[i] = i;
			//this[i].y = 0;
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
};


class I8x2
{
public:
	union{
		struct {
			I8 x,y;
		};
		struct {
			gpeALF	alf;
			I8		num;
		};
		struct
        {
			I4x2 i4x2[2];
		};
		struct
        {
			U4x2 u4x2[2];
		};
	};

    I8x2(){};
    I8x2( I8 _x, I8 _y = 0 )
    {
        x = _x; y = _y;
    }
    I8x2( U4x2 b )
    {
        x = b.x;
        y = b.y;
    }
	I8x2( U8* pB )
    {
		gpmMEMCPY( this, pB, 1 );
    }
    I8x2& operator = ( const U1* pS );
	// cnt = fract * U42(1, w);
	I8x2& cnt2fract( U4 w, U8 cnt )
	{
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
	I8 operator * (const I8x2& b) const
	{
		return (I8)x*b.x + (I8)y * b.y;
	}
	I8x2 operator & (const I8x2& b) const
	{
		return I8x2( x*b.x,  y*b.y );
	}
	I8x2 operator & (const U8* pB ) const
	{
		return I8x2( x* pB[0],  y* pB[1] );
	}

	I8x2 operator / (const U4x2& b) const
	{
		return I8x2( b.x ? x/b.x : 0x7FFFffffFFFFffff,  b.y ? y/b.y : 0x7FFFffffFFFFffff );
	}
	I8x2 operator / (const I8x2& b) const
	{
		return I8x2( b.x ? x/b.x : 0x7FFFffffFFFFffff ,  b.y ? y/b.y : 0x7FFFffffFFFFffff );
	}

	I8x2 operator + (const I4x2& b) const
	{
		return I8x2( x+b.x,  y+b.y );
	}
	I8x2 operator + (const I8x2& b) const
	{
		return I8x2( x+b.x,  y+b.y );
	}

	I8x2 operator + (const U4x2& b) const
	{
		return I8x2( x+b.x,  y+b.y );
	}
	I8x2 operator + ( const U8* pB ) const
	{
		return I8x2( x+pB[0],  y+pB[1] );
	}

	I8x2 operator - (const I4x2& b) const
	{
		return I8x2( x-b.x,  y-b.y );
	}
	I8x2 operator - (const I8x2& b) const
	{
		return I8x2( x-b.x,  y-b.y );
	}

	I8x2 operator - (const U4x2& b) const
	{
		return I8x2( x-b.x,  y-b.y );
	}
	I8x2 operator - ( const U8* pB ) const
	{
		return I8x2( x-pB[0], y-pB[1] );
	}

	bool operator != ( const I4x2& b ) const
	{
		if( x != b.x )
			return true;
		return y != b.y;
	}
	bool operator != ( const I8x2& b ) const
	{
		if( x != b.x )
			return true;
		return y != b.y;
	}
	bool operator == ( const I4x2& b ) const
	{
		return !(*this!=b);
	}
	bool operator == ( const I8x2& b ) const
	{
		return !(*this!=b);
	}

	I8x2& operator *= ( I8 i )
	{
		if( !i )
			return null();
		if( i == 1 )
			return *this;

		x*=i;
		y*=i;

		return *this;
	}
	I8x2& operator /= ( I8 i )
	{
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
	I8x2& operator %= ( I8 i )
	{
		if( i == 1 )
			return null();

		if( !i )
			return *this;


		x%=i;
		y%=i;

		return *this;
	}
	I8x2& null( void )
	{
		gpmCLR;
		return *this;
	}

	I8 sum( void ) const
	{
		return x+y;
	}

	I8 area( void ) const
	{
		return x*y;
	}

	I8 are_sum( void ) const
	{
		return abs().area()+abs().sum();
	}

	I8 qlen (void ) const
	{
		return x*x + y*y;
	}

	I8 mn( void ) const
	{
		return x < y ? x:y;
	}

	I8 mx( void ) const
	{
		return x > y ? x:y;
	}

	I8x2 abs( void ) const
	{
		return I8x2( x<0?-x:x, y<0?-y:y );
	}
	I8x2& A( U1* pA, U1** ppA );

	U8 str( U1* p_buff, const U1* p_post = NULL, bool b_hex = false )
	{
		if( !p_buff )
			return 0;
		if( !this )
		{
			*p_buff = 0;
			return 0;
		}
		U1* p_begin = p_buff;
		p_buff += gpfALF2STR( p_buff, alf ); //, b_hex );
		//_strupr( p_begin );
		p_buff += sprintf( (char*)p_buff, b_hex ? "%llx": "%lld", max(num,0) );
		if( p_post )
			p_buff += sprintf( (char*)p_buff, "%s", p_post );
		return p_buff-p_begin;
	}

};


class I8x4
{
public:
    union
    {
        struct
        {
            I8 x,y,z,w;
        };
        struct
        {
            I8 aXYZW[4];
        };
        struct
        {
            I8x2 a8x2[2];
        };
        struct
        {
            gpeALF labe;
            I8 mom, up, nx;
        };
    };
    I8x4(){};
    I8x4( I8 _x, I8 _y = 0, I8 _z = 0, I8 _w = 0 )
    {
        x = _x; y = _y; z = _z; w = _w;
    }
    I8x4& operator = ( U8 b )
    {
		gpmCLR;
        x = min( b, 0x7FFFffffFFFFffff );
		return *this;
    }
    I8x4& operator = ( I8 b )
    {
        gpmCLR;
        x = b;
		return *this;
    }
    I8x4& null( void )
	{
		gpmCLR;
		return *this;
	}
	I8x4& AB( U1* pA, U1* pB, U1** ppA = NULL, U1** ppB = NULL );

    I8 tree_add( gpeALF alf, I8 n_t )
    {
		return tree_add( (I8)alf, n_t );
    }
    I8 tree_add( I8 i8, I8 n_t )
	{
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
	I8 tree_fnd( gpeALF alf, I8 n_t )
    {
		return tree_fnd( (I8)alf, n_t );
    }
	I8 tree_fnd( I8 i8, I8 n_t )
	{
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
};



class F4
{
public:
    float x,y,z,w;
    F4(){};
    F4( I4 _x, I4 _y, I4 _z, I4 _w )
    {
        x = _x; y = _y; z = _z; w = _w;
    }
    F4( float _x, float _y = 0.0, float _z = 0.0, float _w = 0.0 )
    {
        x = _x; y = _y; z = _z; w = _w;
    }
    F4( float* pF )
    {
        gpmMEMCPY( this, pF, 1 );
    }
    double sum( void ) const
    {
        return x+y+z+w;
    }
    double sum_xyz( void ) const
    {
        return x+y+z;
    }
    double qlen( void ) const
    {
        return x*x+y*y+z*z+w*w;
    }
    double qlen_xyz( void ) const
    {
        return x*x+y*y+z*z;
    }
    F4 norm_xyz( void ) const
	{
        double l = sqrt(qlen_xyz());
		return F4( x/l, y/l, z/l );
	}
	float dot_xyz( F4 b ) const
	{
        return x*b.x + y*b.y + z*b.z;
	}
    F4 cross_xyz( F4 b ) const
	{
		return F4(
						y * b.z - z * b.y,
						z * b.x - x * b.z,
						x * b.y - y * b.x
				);
	}
	F4 operator + ( const F4& b ) const
	{
        return F4( x+b.x, y+b.y, z+b.z, w+b.w );
	}
	F4 operator - ( const F4& b ) const
	{
        return F4( x-b.x, y-b.y, z-b.z, w-b.w );
	}
};

class D4
{
public:
    double x,y,z,w;
    D4(){};
    D4( I4 _x, I4 _y, I4 _z, I4 _w )
    {
        x = _x; y = _y; z = _z; w = _w;
    }
    D4( double _x, double _y = 0.0, double _z = 0.0, double _w = 0.0 )
    {
        x = _x; y = _y; z = _z; w = _w;
    }
    D4( double* pD )
    {
        gpmMEMCPY( this, pD, 1 );
    }
    double sum( void ) const
    {
        return x+y+z+w;
    }
    double sum_xyz( void ) const
    {
        return x+y+z;
    }
    double qlen( void ) const
    {
        return x*x+y*y+z*z+w*w;
    }
    double qlen_xyz( void ) const
    {
        return x*x+y*y+z*z;
    }
    D4 norm_xyz( void ) const
	{
        double l = sqrt(qlen_xyz());
		return D4( x/l, y/l, z/l );
	}
	double dot_xyz( D4 b ) const
	{
        return x*b.x + y*b.y + z*b.z;
	}
    D4 cross_xyz( D4 b ) const
	{
		return D4(
						y * b.z - z * b.y,
						z * b.x - x * b.z,
						x * b.y - y * b.x
				);
	}
	D4 operator + ( const D4& b ) const
	{
        return D4( x+b.x, y+b.y, z+b.z, w+b.w );
	}
	D4 operator - ( const D4& b ) const
	{
        return D4( x-b.x, y-b.y, z-b.z, w-b.w );
	}
};

typedef enum gpeNET2:U4
{
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

};
typedef enum gpeNET4:U4
{
	gpcNET4_NULL, // = 0,
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
	gpeNET4_PREV	= MAKE_ID( 'P', 'R', 'E', 'V' ),
};

class gpcCMPL;

class gpcLAZY
{
public:
	union
	{
		struct{
			U1* p_alloc, aSET[8];
			U8 n_load, n_alloc;
		};
		struct{
			U8		*p_a8;
			gpeWIP	aWIP[8];
			U8		n_l8, n_a8;
		};
	};

	U4 nPC( void );
	void wait( void )
	{
		if( !this )
			return;

		while( aWIP[gpeLZYwip] != gpeWIP_done )
			usleep(100);

	}
	void DO(void)
	{
		aWIP[gpeLZYwip] = aWIP[gpeLZYwipSTK];
	}
	gpcLAZY* qEVENT(void);
	gpcLAZY( void )
	{
		gpmCLR;
	}
	gpcLAZY( U1 n )
	{
		gpmCLR;

		if( n < 1 )
			n = 4;
		aWIP[gpeLZYwip] = gpeWIP_done,
		aSET[gpeLZYxN] = n;
	}
	~gpcLAZY()
	{
		gpmFREE( p_alloc );
	}

	gpcLAZY* lzy_strict( void )
	{
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


	gpcLAZY* lzy_reset( void )
	{
		if( this ? !n_load : true )
			return this;

		if( n_load*2 < n_alloc )
		{
			U8 new_alloc = gpmPAD( ((n_alloc*3)/4), 0x10 );
			if( !new_alloc )
			{
				(*p_alloc) = n_load = 0;
				return this;
			}
			else if( n_alloc > new_alloc )
			{
				free( p_alloc );
				n_alloc = new_alloc;
				p_alloc = gpmALLOC( n_alloc+0x10 );
			}
		}
		(*p_alloc) = n_load = 0;
		return this;
	}

	gpcLAZY* lzy_add( const void* p_void, U8 n_byte, U8& n_start, U1 n = 0 )
	{
		if( !n_byte )
			return this;

		if( !this )
		{
			n_start = 0;
			gpcLAZY* p_lazy = new gpcLAZY( n );
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

		if( !n )
		{
			if( !aSET[gpeLZYxN] )
				aSET[gpeLZYxN] = 4;

			n = aSET[gpeLZYxN];
		}
		else if( !aSET[gpeLZYxN] )
				aSET[gpeLZYxN] = n;


		if( n_start > n_load )
			n_start = n_load;
		else
			n_load = n_start;

		if( p_alloc ? (n_load+n_byte > n_alloc) : true )
		{
			U1* p_kill = p_alloc;
			n_alloc = gpmPAD( (n_load+n_byte*n), 0x10 );
            p_alloc = NULL;
			p_alloc = gpmALLOC( n_alloc+0x10 ); //, 0x10 );
			if( p_kill )
			{
				memcpy( p_alloc, p_kill, n_load );
				free( p_kill );
			} else
				n_load = 0;
		}
		if( !p_void )
		{
			n_load += n_byte;
			p_alloc[n_load] = 0;
			return this;
		}

		memcpy( p_alloc+n_load, p_void, n_byte );
		n_load += n_byte;
		p_alloc[n_load] = 0;
		return this;
	}
	gpcLAZY* lzy_plus(  const gpcLAZY* p_b, U8& n_start )
	{
		return lzy_add( p_b->p_alloc, p_b->n_load, n_start, ( (p_b->n_load<=0x40) ? 0xf : 0x3 ) );
	}
	gpcLAZY* lzy_sub( U8& n_start, U8 n_sub )
	{
		if( !this )
		{
			n_start = 0;
			return NULL;
		}
		if( n_start > n_load )
		{
			n_start = n_load;
			return this;
		}
		if( n_start+n_sub >= n_load )
		{
			n_sub = n_load-n_start;
		} else {
			U8 n_hi = n_load-(n_start+n_sub);
			memcpy( p_alloc+n_start, p_alloc+n_start+n_sub, n_hi );
		}

		n_load -= n_sub;
		p_alloc[n_load] = 0;
		return this;
	}
	gpcLAZY* lzy_exp( U8& n_start, U8 n_sub, U8 n_add, U1 n = 0 )
	{
		if( !this )
		{
			return lzy_add( NULL, n_add, n_start, n );
		}

		if( !n )
		{
			if( !aSET[gpeLZYxN] )
				aSET[gpeLZYxN] = 4;

			n = aSET[gpeLZYxN];
		}
		else if( !aSET[gpeLZYxN] )
				aSET[gpeLZYxN] = n;

		if( n_start > n_load )
		{
			n_start = n_load;
			n_sub = 0;
		}
		else if( n_start+n_sub >= n_load )
		{
			n_sub = n_load-n_start;
		}
		U8 src_hi = n_start+n_sub,
			dst_hi = n_start+n_add,
			n_hi = n_load-src_hi,
			new_load = n_start + n_add + n_hi;
		U1* p_kill = NULL;

		if( new_load > n_alloc )
		{
			n_alloc += gpmPAD( (n_add*n), 0x10 );
			p_kill = p_alloc;
			p_alloc = gpmALLOC( n_alloc+0x10 ); //, 0x10 );
			memcpy( p_alloc, p_kill, n_start );
			memcpy( p_alloc+dst_hi, p_kill+src_hi, n_hi );
			n_load = new_load;
			p_alloc[n_load] = 0;
			gpmFREE( p_kill );
			return this;
		}

		if( n_hi )
		if( n_load <= dst_hi )
		{
			// nincsen átfedésben mehet sima kopival
			memcpy( p_alloc+dst_hi, p_kill+src_hi, n_hi );
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

	gpcLAZY* lzy_ins( const U1* p_u1, U8 n_u1, U8& n_start, U8 n_sub, U1 n = 0 )
	{
		if( !this )
		{
			//start = n_u1;
			return lzy_add( p_u1, n_u1, n_start, n );
		}
		lzy_exp( n_start, n_sub,  n_u1, n );
		memcpy( p_alloc+n_start, p_u1, n_u1 );
		return this;
	}
	gpcLAZY& operator = ( const gpcLAZY& plus )
	{
		U8 s = 0;
		lzy_ins( plus.p_alloc, plus.n_load, s, -1 );

		return *this;
	}

	gpcLAZY* operator += ( const gpcLAZY& plus )
	{
		U8 s = -1;
		return lzy_add( plus.p_alloc, plus.n_load, s );
	}

	gpcLAZY* lzy_read( char* p_file, U8& n_start, U1 n = 0 )
	{
		if( !p_file )
			return this;

		if( !this )
			n_start = 0;

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
		gpcLAZY* p_lazy = this;

		if( n_byte > 0 )
		{
			p_lazy = p_lazy->lzy_add( NULL, n_byte, n_start, n );
			if( !p_lazy )
				goto szasz;
			U8 n;
			U1* p_l = p_lazy->p_alloc + n_start, *p_e = p_l+n_byte;
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
	gpcLAZY* lzy_write( const char* p_file, bool b_over = true )
	{
		if( this ? !n_load : true )
			return this;

		if( !b_over )
		if( gpfACE(p_file, 4) < 0 )
			return this;

		char s_buff[gpeMXPATH];
		FILE* p_f = fopen( p_file, "wb" );
		U8 n_w = 0, n_err = 0, W0 = 0;

		if( !p_f )
		{
			if( !gpfMKDR( s_buff, p_file ) )
				goto close;
			p_f = fopen(p_file, "wb");
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
	gpcLAZY* lzy_format( U8& n_start, const char* p_format, ... );
	gpcLAZY* lzy_reqCLOSE( void )
	{
		if( !this )
			return this;

		aSET[gpeLZYoff] = 1;
		return this;
	}
	bool qCLOSE( void )
	{
		if( !this )
			return false;
		return aSET[gpeLZYoff];
	}
	U4 tree_fnd( U4 id, U4& n );
	gpcLAZY* tree_add( U4 id, U4& n );
	U8 tree_fnd( U8 id, U8& n );
	gpcLAZY* tree_add( U8 id, U8& n );
	I8 tree_fnd( I8 id, I8& n );
	gpcLAZY* tree_add( I8 id, I8& n );

	//U8 gpcLAZY::tree_fnd( U8 id, U8& n )
	gpcCMPL* pPC( U4 pc, U1* pS = NULL );
	gpcCMPL* pSPARE( U4 pc, gpeALF sw = gpeALF_null , U1* pS = NULL );
};


class gpcLZYdct
{
	U4x4*		pIX;
public:
	gpcLAZY		str,
				ix;
	U8			ver;

	gpcLZYdct(){};
	gpcLZYdct(U8 i)
	{
		gpmCLR;
	}
	U4 dict_find( U1* pS, U8 nS, U4& nIX )
	{
		if( !this )
		{
			nIX = 0;
			return 0;
		}
		U8 aSTRT[2]; // = -1;
		if( !str.p_alloc )
			ver = 0;
		str.lzy_add( pS, nS+1, aSTRT[0] = -1 );
		U1* pS0 = str.p_alloc;
		pS = pS0+aSTRT[0];
		if( pS[nS] )
			pS[nS] = 0;

		ix.lzy_add( NULL, sizeof(U4x4), aSTRT[1] = -1 );
		nIX = (aSTRT[1]/sizeof(U4x4));
		U4x4	*p_ix0 = ((U4x4*)ix.p_alloc);

		pIX = p_ix0 + nIX;
		pIX->null();
		pIX->x = aSTRT[0];
		pIX->y = nS;

		U4 iIX = p_ix0->dict_find( pS0, *pIX );

		// UNDOza a bejegyzést
		str.n_load = aSTRT[0];
		ix.n_load = aSTRT[1];
		pIX = p_ix0+iIX;
		return iIX;
	}
	gpcLZYdct* dict_add( U1* pS, U8 nS )
	{
		if( !this )
		{
			gpcLZYdct* p_this = new gpcLZYdct(0);

			return p_this->dict_add( pS, nS );
		}
		U8 aSTRT[2]; // = -1;
		if( !str.p_alloc )
			ver = 0;
		str.lzy_add( pS, nS+1, aSTRT[0] = -1 );
		U1* pS0 = str.p_alloc;
		pS = pS0+aSTRT[0];
		if( pS[nS] )
			pS[nS] = 0;

		ix.lzy_add( NULL, sizeof(U4x4), aSTRT[1] = -1 );
		U4 nIX = (aSTRT[1]/sizeof(U4x4));
		U4x4	*p_ix0 = ((U4x4*)ix.p_alloc);

		pIX = p_ix0 + nIX;
		pIX->null();
		pIX->x = aSTRT[0];
		pIX->y = nS;
		U4 iIX, nADD = p_ix0->dict_add( pS0, iIX, *pIX );

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
	U4 x( void )
	{
		if( this ? !ix.n_load : true )
			return 0;

		return pIX-(U4x4*)ix.p_alloc;
	}
	U4 i_str(void)
	{
		if( !pIX )
			return str.n_load;

		return pIX->x;
	}
	char* p_str( void )
	{
		return (char*)str.p_alloc+pIX->x;
	}
	U8 nIX(void)
	{
		return this ? (ix.n_load / sizeof(U4x4)) : 0;
	}
	const char* sSTRix( U8 iX, const char* pER )
	{
		if( !this )
			return pER;
		U4x4	*p_ix0 = ((U4x4*)ix.p_alloc);
		return (char*)(str.p_alloc + p_ix0[iX].x);
	}
};

#endif // piMASS_H_INCLUDED
