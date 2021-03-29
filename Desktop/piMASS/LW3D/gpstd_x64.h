// "$(SolutionDir)MrClockSpring\screen.sht"
// "$(SolutionDir)\$(Configuration)\$(Configuration)2.sht"
// "$(SolutionDir)MrPiiQ26\mr_piiq26.sht"
// "$(SolutionDir)MrPiiQ26\mr_piiq26.sht"
// $(SolutionDir)MrClockSpring\
// "$(SolutionDir)$(Configuration)\$(Configuration).sht"
#pragma once
#ifndef GPSTD_H
#define GPSTD_H



// KAPCSOLÓK
#define _DEBUG_MUTI
#define _DEBUG_NEW
#define _MEMMAP_OFF

#define GPD_CHM_N 2

#define GPD_ATI	true
#define GPD_INTEL	true //false //true
#define GPD_CBUFF	false 

#define _DEBUG_GT_MUTI
#define _GPC3D_EXEC_MUTI
#define _GP_STD_THREAD
#define _DTCT_9

#define _DTCT_LOCAL
#define _DTCT_NEWCAM
#define _DTCT_DROP 5

#define GPD_GIV_MAP "map"
#define GPD_GIV_PNG ".png"
#define GPD_GIV_JPG ".jpg"
#define GPD_GIV_MAP_PNG	GPD_GIV_MAP\
					GPD_GIV_PNG	//"map.png"
#define GPD_GIV_MAP_JPG	GPD_GIV_MAP\
					GPD_GIV_JPG	//"map.jpg"
//#define GPD_3d_INST
//#define _TRD_PRESENT
#define GPD_3D_RECV_N_GPX	8

//#define GPD_VOXVOX
#define GPD_GT_NO_DALEY 1 //0

//#pragma once
#define _CRT_SECURE_NO_WARNINGS
#pragma warning( push )
#pragma warning( disable : 4995 )
#include <string>
#pragma warning( pop )

#pragma inline_recursion(on)
#pragma inline_depth(8)

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>

#include <stdio.h>
#include <direct.h>
#include <assert.h>
//#include <string.h>



#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <xnamath.h>

#include <vector>


#include "streams.h"
#include <Dbt.h>
#include "Wmcodecdsp.h"
#include <initguid.h> 

#include <io.h>
#include <mmsystem.h>
#ifndef _MUTEX_
	#include <thread>
	#include <mutex>
#endif

#include "ipmib.h"
#include <iphlpapi.h>
// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")
#pragma comment (lib, "iphlpapi.lib")
#pragma comment (lib, "ws2_32.lib")

#include "GP_FIX.h"

ULL* gpf_ull_cpy( ULL* p_dst, ULL* p_src, ULL n_src, ULL d_stp, ULL d_up, ULL w, ULL z );

bool GPF_CON_on( void );
//void GPF_CON_null( void );
void GPF_CON_pos( U2 x = 0, U2 y = 0 );
void GPF_CON_cls( void );
void GPF_CON_Write( char *p_str );
void GPF_CON_LST_write( GPE_CLST n, GPE_GT_CnS cNs, char* p_str, U4 data );
void GPF_CON_FORMAT_Write( char *p_format, ... );

__declspec(align(16)) typedef LONG	A16L;


U4 GPF_Bx7_LEN_xyz( U4 e0, U4 e1 );
I8 GPF_CON_n( void );






class U16
{
public:
	U8 al, ah;
	U16( U8 a )
	{
		al = a;
		ah = 0;
	}

	U16& operator -= ( U8 b )
	{
		b = ~b; 
		// polinommá alakítjuk és öszegezzük tologatjuk
		// (A0+B)+(C0+D) = A0+C0 + B + D 
		U8	A0 = al>>32,		//A0
			B = al&0xffffffff,	//B	
			C0 = b>>32,		//C0	
			D = b&0xffffffff;		//D
		
		ah = A0+C0;
		al = (ah&0xffffffff)<<32;
		al += B+D;
		ah >>= 32;
		return *this;
	}

	U16& operator += ( U8 b )
	{
		
		// polinommá alakítjuk és öszegezzük tologatjuk
		// (A0+B)+(C0+D) = (A0+C0) + (B+D) 
		U8	A0 = al>>32,		//A0
			B = al&0xffffffff,	//B	
			C0 = b>>32,		//C0	
			D = b&0xffffffff;		//D
		
		ah = A0+C0;
		al += B+D;
		al += (ah&0xffffffff)<<32;
		ah >>=32;
		return *this;
	}
	U16& operator *= ( U8 b )
	{
		// polinommá alakítjuk és szorzunk 
		// (A0+B)*(C0+D) =  A0(C0+D) + B(C0+D) = A0C0+A0D+BC0+BD = A0C0+A0D+BC0+BD 

		U8	A0 = al>>32,		//A0
			B = al&0xffffffff,	//B	
			C0 = b>>32,		//C0	
			D = b&0xffffffff;		//D
		

		U8	A0C0 = A0*C0, // ah
			A0D = A0*D,
			BC0 = B*C0,
			BD = B*D;
		
		ah = A0C0 + (A0D>>32);
		al = (A0D<<32)+(BC0<<32)+BD;
		return *this;
	}
};


void	GPF_MEM_Reset(void);
I8	GPF_MEM_NUSE(void);



U4 GPF_q2Q( U4 q );
U4 GPF_q2a0( U4 q );


char*	GPF_MEM_GetMap( char*& p_lo, char*& p_hi );
void*	GPF_MEM_GetTex( U4* p_heapmap );

void	GPF_GET_n_res_reset( void );
I8	GPF_GET_n_res_alloc( void );
I8	GPF_GET_n_res_free( void );

char* GPF_LW3D_Pad( char* p_data );

//void operator delete(void *p);
//void operator delete[](void *p);


struct GPS_ALLOC
{
	void*	p_alloc;
	ULL	size;
};

#define GPD_CRS_NUNDO 0x1000
#define GPD_Sleep0 Sleep(0);
#define GPD_Sleep10 Sleep(20);


//#define GPM_LW3D_PAD( p ) { if( p&1 ){ p++ } } 

//#define GPM_DELARRAY( p ){ { delete[] p; p = NULL; } }
//#define GPM_DEL( p ){ { delete p; p = NULL; } }



//											dec



#define GPD_ROBI_EXEC		1<<15
#define GPD_ROBI_CLOSE	1<<9

#define GPD_GX_ITEM_XX_STR	"ssTEMMAIVIXXTTTvvTv rATGGGGGGGG" 
#define GPD_GX_ITEM_UNPACK_STR	"ssTEMMAIVIVVTTTvvTv RATGGGGGGGG"	
#define GPD_GX_FAR 1 //(d/300000 + 1)


#define GPD_48q	48*48*48
#define GPD_24q	24*24*24
#define GPD_12q	12*12*12
#define GPD_06q	6*6*6
#define GPD_03q	3*3*3
#define GPD_48q_all	GPD_48q*2+GPD_24q+GPD_12q+GPD_06q+GPD_03q

#define GPD_2x	(2*2)
#define GPD_4x	(4*4)
#define GPD_8x	(8*8)
#define GPD_16x	(16*16)

#define GPD_32x	(32*32)
#define GPD_32q	32*32*32
#define GPD_32to5x6	6*(GPD_32x+GPD_16x+GPD_8x+GPD_4x+GPD_2x)



#define GPD_80x	80*80
#define GPD_80q	80*80*80
#define GPD_80to5x6	6*(80*80+40*40+20*20+10*10+5*5)








typedef enum GPE3D_LIGHT : I8
{
	GPE_3D_LIGHT_RES,
	GPE_3D_LIGHT_nTHIS	= 2,
	GPE_3D_LIGHT_nX	= 4,
	GPE_3D_LIGHT_nY	= 8,
	GPE_3D_LIGHT_nZ	= 16,
	GPE_3D_LIGHT_THIS	= GPD_3D_END_Y,
	GPE_3D_LIGHT_END	= GPE_3D_LIGHT_THIS+GPD_3D_END_Y,
};

#define GPD_MEMCPY (1<<(5*4))
#define GPD_MEMALLOC (1<<12)

// ; rcx p_d; rdx p_s; r8 yz; r9 w16; r10 wx16

#include "GP_ASM.h"

__forceinline I8 gp_mul64( I8 b, I8 d )
{
	if( !b || !d )
		return 0;
	if( b == 1 )
		return d;
	else if( d == 1 )
		return b;

	I8 a,c;
	U1 n = 0;

	if( b < 0 )
	{
		b = -b;
		n++;
	}
	if( d < 0 )
	{
		d = -d;
		n++;
	}

	if( b >= d )
	{
		a = b>>32;
	} else {
		a = d;
		d = b;
		b = a;
		a >>= 32;
	}
	
	c = d>>32;

	if( !(a*c) )
	{
		b &= 0xFFFFffff;
		d &= 0xFFFFffff;
		a = ((b*c+a*d)<<32) + b*d; 
	} else {
		a = 0x7FFFffffFFFFffff;
	}
			
	return ( n&1 ) ? -a : a;
}

//#define _memcpy( p_dst, p_src, n ){if(n&&p_dst&&p_src)if(n<GPD_MEMCPY*2){ gp_memcpy(p_dst,p_src,n); }else{GPF_MEM_cpy(p_dst,p_src,n);}}								
//#define _memcpy( p_dst, p_src, n ){ gp_memcpy(p_dst,p_src,n); } // if(n&&p_dst&&p_src) gp_memcpy(p_dst,p_src,n); }								

//memmove
//void GPF_PRI_LO( void );
//void GPF_PRI_HI( void );
void GPF_wXh256_INIT( void );

typedef enum GPE_WIP : volatile LONG 
{
	GPE_WIP_TRD_INIT,
	GPE_WIP_BOSS_SYNC,
	GPE_WIP_BOSS_SETGO,
	GPE_WIP_TRD_START,
	

	GPE_WIP_TRD_STEP20 ,
	GPE_WIP_BOSS_STEP30,
	GPE_WIP_BOSS_STEP80,
	GPE_WIP_BOSS_STEP90,

	GPE_WIP_TRD_DONE,
	GPE_WIP_BOSS_USE,
	GPE_WIP_BOSS_KILL,
	GPE_WIP_TRD_DIE,
} GPT_WIP;
typedef enum GPE_CPU:I8
{
	GPE_CPU_IDLE = -1,
	GPE_CPU_BALANCE,
	GPE_CPU_DEZS,
	GPE_CPU_FILL_SIZE,
	GPE_CPU_ALT,
	GPE_CPU_GIV_BUILD,
	GPE_CPU_BUG,
	GPE_CPU_BUG2,
	GPE_CPU_GIV_RENDER,

} GPT_CPU;


typedef enum GPE_LWS_COM:I8
{
		GPE_LWS_COM_LoadObjectLayer	,
		GPE_LWS_COM_AddBone		,
		GPE_LWS_COM_AddNullObject	,
		GPE_LWS_COM_AddLight		,
		GPE_LWS_COM_AddCamera		,
		GPE_LWS_COM_NumChannels	,
		GPE_LWS_COM_Channel		,
		GPE_LWS_COM_C_open		,
		GPE_LWS_COM_Envelope		,
		GPE_LWS_COM_Key		,
		GPE_LWS_COM_C_close		,
		GPE_LWS_COM_ParentItem	,
		GPE_LWS_COM_BoneName		,
		GPE_LWS_COM_BoneRestPosition	,
		GPE_LWS_COM_BoneRestDirection	,
		GPE_LWS_COM_BoneRestLength	,
} GPT_LWS_COM;





I8 inline clamp(I8 x, I8 a, I8 b)
{
	I8 ab = a-b, xb = x-b;
	if( ab < 0 )
	{
		ab*=-1;
		xb*=-1;
	}
	if( xb > ab )
		return a;
	if( xb < 0 )
		return b;
	
	return x;
}
I8 inline midle(I8 x, I8 a, I8 b )
{
	I8 y = clamp( x, a, b );
	if( y == x )
		return x;
	return (a+b)/2;
}
U4* GPF_HISTI( U4* p_histi, U1* p_pat, I8 n );

float GPF_NOP( float f1, float f2, float f3 );
float GPF_MEDIAN( float f1, float f2, float f3 );
float GPF_AVGR( float f1, float f2, float f3 );
float GPF_MIN_MAX_MIDLE( float f1, float f2, float f3 );
I8 inline GPF_LOG_AB( I8 a, I8 b )
{
	return log10(b)/log10(a);
}
float inline GPF_LOG_AB( float a, float b )
{
	return log10(b)/log10(a);
}

class GPC_FIx64
{
public:
	I8	i;
	double	v;
	
	void	ipn_mul( I8 n, double mul )
	{
		if( !this || n < 1 )
			return;
		for( I8 i = 0; i < n; i++ )
		{
			this[i].v = mul*i/n;
		}
	}
	double	avarage( ULL n );
	double	median( ULL n, bool b_inc = false );
	double	deviation( ULL n, double& devi ); // szórás
	//void	shrt( U4 n, bool b_inc = false );
};


class GPC_U1
{
public:
	U1	i, vi;
	
	GPC_U1(){};
	U1  avarage( U1 n )
	{
		// vigyázz ez sorrendezi az értékeket
		if( !this || n < 1 )
			return 0.0f;

		if( n < 2 )
		{
			return this[0].vi;
		}
		else if( n < 3 )
		{
			return ((this[1].vi-this[0].vi)/2.0f + this[0].vi);
		}

		double avgr = 0.0f;
		for( ULL j = 0; j < n; j++ ) 
			avgr += this[j].vi;

		return (I8)(avgr / (double)n);
	}
	U1 median( U1 n, bool b_inc )
	{
		// b_inc == true - incrementált növekvõ sorban leszenk
		// b_inc == false - dekrementáslt csökkenõ sorban leszenk (nem definiálod akkor ez, azaz csökenõ )
		if( !this || n < 1 )
			return 0;
	
		if( n < 2 )
			return this->vi;
		
		U2 i, j, l, r;
		GPC_U1* p_tree = new GPC_U1[n+1],
			x;
	
		r = n;
		while( r >= 1 )
		{
			// az öszes elem számát "r" elosztom 2-tõvel kerekítés nélkül
			// ezzel tudom, hogy a soron következõ szint meddig csökkenhet "l" lesz a küszöb
			l = r>>1;
			while( r > 0 )
			{
				// a következõ elemet berakom az x-be
				x = this[r-1];

				i = r;
				if ( i*2 <= n )
				{
					// i mutatja majd azt a helyet ahonva az x et be akarnám rakni
					while ( i*2 <= n )
					{
						j = i*2;
						// azt jelenti, hogy az i nek van ága
						if ( j+1 <= n )
							if ( p_tree[j+1].vi < p_tree[j].vi )
								j++; // azt jelenti, hogy két ága is volt, és a magasabb indexün kissebb volt az érték
					
						if( x.vi > p_tree[j].vi )
						{
							// azt jelenti hogy az x nagyobb volt mint az ág ezért lejebb rakom a tartalmát
							p_tree[i] = p_tree[j];
							// és következõ ciklusban az ágról akarom folytatni
							i = j;
						} else {
							// azt jelenti, nincs ennél magasabb szám az ágakon
							//p_tree[i] = x;
							break;
						}
					}//while
					p_tree[i] = x;
				} else {
					p_tree[r] = x;
				}

				// r-et csökkentem jöhet a következõ elem
				r--;
			}
		}
		l = r>>1;
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
						if ( p_tree[i+1].vi < p_tree[i].vi )
							i++;
				
					if ( x.vi > p_tree[i].vi )
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
			for( U2 i = 0; i < n; i++ )
			{
				this[i] = p_tree[n-i];
			}
			GPM_DELARRAY( p_tree );
			if( n < 3 )
				return avarage( n );
			return this[n/2].vi;
		}

		GPC_U1* p_src = p_tree+1;
		for( U2 i = 0; i < n; i++ )
		{
			this[i] = p_src[i];
		}
		GPM_DELARRAY( p_tree );
		if( n < 3 )
			return avarage( n );

		return this[n/2].vi;
	}
};




long double GPF_Ramanujan( long double a, long double b );

//class GPCV3;
//class GPCI4;
//class GPCV4;
//class GPC_COLOR4;
//class GPCQ;
//class GPC_CNL;
//class GPC_GIO_VX32C;
//class GPC3DHI;
//class GPC_GX_LOD;
//class GPC3D64_LOD;
//class GPC_GIOx64_LAY_LOD;
//class GPC_GIOx64_LAY;





class GPC_TXT_C4
{
public:
	GPC_COLOR4* p_mini_c4, * p_frame_c4;
	I8	col, row, n_cr;
	
};

__forceinline I8 GPF_DOWN( I8 i8, I8 dwn )
{
	if( !i8 )
		return 0;
	
	if( dwn < 0 )
		dwn *= -1;

	if( dwn < 2 )
		return i8;

	if( !(i8%dwn) )
		return i8;

	if( i8 < 0 )
		i8 -= dwn;

	return i8 - (i8%dwn);
}
__forceinline I8 GPF_UP( I8 i8, I8 up )
{
	if( !i8 )
		return up;
	if( !(i8%up) )
		return i8;
	i8 += up;
	return i8 - i8%up;
}


class GPCQ
{
public:
	float s, i, j, k;
	GPCQ(){};
	GPCQ( float _s, float _x, float _y, float _z)
	{
		s = _s;
		i = _x;
		j = _y;
		k = _z;
	}
};



__declspec(align(16)) class GPCMX
{
public:
	union
	{
		struct
		{
			float	_11,_12,_13,_14,
					_21,_22,_23,_24,
					_31,_32,_33,_34,
					_41,_42,_43,_44;
		};
		struct
		{
			float	xx,xy,xz,xw,
				yx,yy,yz,yw,
				zx,zy,zz,zw,
				wx,wy,wz,ww;
		};
		struct
		{
			GPCV4	x4,
				y4,
				z4,
				t4;
		};
		struct
		{
			GPCV3	x3; float pitch;
			GPCV3	y3; float yaw;
			GPCV3	z3; float roll;
			GPCV3	t3; float one;
		};
	};

	GPCMX(){};
	GPCMX( float f )
	{
		GPM_CLR;
		_11 = _22 = _33 = f;
		_44 = 1.0f;
	};
	GPCMX(	float e11, float e12, float e13, float e14,
			float e21, float e22, float e23, float e24,
			float e31, float e32, float e33, float e34,
			float e41, float e42, float e43, float e44	);
	GPCMX( float *p_f )
	{
		gp_memcpy( this, p_f, sizeof(*this) );
	}
	GPCMX( GPCMX& dxmx )
	{
		gp_memcpy( this, &dxmx, sizeof(*this) );
	}
	GPCMX( D3DXMATRIX& dxmx )
	{
		gp_memcpy( this, (void*)&dxmx, sizeof(*this) );
	}
	GPCMX( char* p_str, char** pp_stop );
	GPCMX( GPC_CNL& cnl );
	GPCMX( GPCV3& scale );

	operator D3DXMATRIX* ();

	GPCMX&	operator = ( GPCQ &q );
	GPCMX&	operator = ( D3DXMATRIX& dxmx );
	//GPCMX&	operator = ( GPCMX& dxmx );
	GPCMX&	operator *= ( GPCMX& bmx );
	//bool	operator == ( const GPCMX& mx ) const;
	//bool	operator != ( const GPCMX& mx ) const;
	GPCMX	operator * ( float f );
	GPCV3	operator * ( const GPCV3& v ) const;
	GPCMX	operator * ( const GPCMX& mx2 ) const;
	GPCMX	operator + ( GPCMX& mx2 );
	GPCMX	operator - ( GPCMX& mx2 );
	//GPCMX&	mul( const GPCMX& amx, const GPCMX& bmx );			// ezt akkor lehet használni ha a mûveletben szereplõ minden mátrix különbözõ
	GPCV2	operator * ( GPCV2& va );
	GPCV3	operator * ( GPCV3& va );
	//GPCV4	operator * ( GPCV3& va );
	GPCV4	operator * ( GPCV4& va );
	//friend GPCV4 operator * ( const GPCV4A& va, const GPCMXA& bmx ) ;
	//friend GPC_PLANE	operator * (GPC_PLANE &P, const GPCMXA &MX);
	
	GPCMX&	identity(void);
	GPCMX&	move( GPCV3& v3 );
	GPCMX&	scaling( GPCV3& vi );
	GPCMX&	translation( float x, float y, float z );
	GPCMX&	translation( const GPCV3 &vxt );
	GPCMX&	rotateZ( float rad );
	GPCMX&	rotateX( float rad );
	GPCMX&	rotateY( float rad );
	GPCMX&	proj( GPCV2& ccd, float mount, float focus, float dist, float obj );
	
	GPCMX&	ypr( GPCV3 ypr );
	GPCV3&	eula( void );
	
	GPCMX&	rotateAXIS( GPCV3 vec, float rad );
	GPCMX	transpose( void );
	GPCMX	adjungate( void );
	GPCMX	invert( void );
	GPCMX&	trans( void )
	{
		GPCMX yprt(1.0f), m(1.0f);
		yprt.xx = xx;
		yprt.yx = xy;
		yprt.zx = xz;
		
		yprt.xy = yx;
		yprt.yy = yy;
		yprt.zy = yz;
		
		yprt.xz = zx;
		yprt.yz = zy;
		yprt.zz = zz;
		
		m.t3 = -t3;
		m *= yprt;
		return m;
	}
	float	det( void );
	GPCMX&	lookat( GPCV3 &eye, GPCV3 &center, GPCV3 &up );
	GPCMX&	ortho( float left, float right, float top, float bottom, float zNear, float zFar );
	GPCMX&	interpolate( GPCMX& mx_1, float f );
	GPCMX& norm( void )
	{
		GPCMX m;
		m.identity();
		// y vagy up rendbetesz
		float qlen;
		if( (qlen = y3.qlen()) > 0.01f )
		{
			//egész jó normalizáljuk
			if( qlen != 1.0f )
				m.y3 = y3/sqrt(qlen);
		}
		else if( (qlen = t3.qlen()) > 0.01f )
		{
			m.y3 = t3/sqrt(qlen);
		} else {
			return m;
		}
		
		// x vagy right rendbetesz
		if( (qlen = x3.qlen()) > 0.01f )
		{
			//egész jó normalizáljuk
			m.x3 = x3/sqrt(qlen);
		} else {
			m.x3 = GPCV3( m.y3.y, m.y3.x, m.y3.z );
			if( abs(m.x3.y) < abs( m.y3.z ) )
			{
				qlen = m.x3.y;
				m.x3.y = m.x3.z;
				m.x3.z = qlen;
			}
			m.x3.y *= -1.0f;
		}
		
		GPCV3 temp_z3 = m.y3^m.x3;
		if( (qlen = temp_z3.qlen()) > 0.01f )
		{
			m.z3 = temp_z3/sqrt(qlen);
		} else {
			if( (qlen = z3.qlen()) > 0.01f )
			{
				//egész jó normalizáljuk
				m.z3 = z3/sqrt(qlen);
			} else {
				m.z3 = GPCV3( m.y3.z, m.y3.x, m.y3.y );
				if( abs(m.z3.x) < abs( m.z3.y ) )
				{
					qlen = m.z3.x;
					m.z3.x = m.z3.y;
					m.z3.y = qlen;
				}
				m.x3.x *= -1.0f;
			}
		}
		
		m.x3 = m.z3^m.y3;
		if( (qlen = m.x3.qlen()) != 1.0f )
		{
			m.x3 /= sqrt(qlen);
			m.z3 = m.y3^m.x3;
		}
		m.t3 = t3;
		return m;
	}
	GPCV4& trans( GPCV3 v3 )
	{
		GPCV4 out;

		out.x = _11*v3.x + _21*v3.y + _31*v3.z + _41;
		out.x = _12*v3.x + _22*v3.y + _32*v3.z + _42;
		out.x = _13*v3.x + _23*v3.y + _33*v3.z + _43;
		out.x = _14*v3.x + _24*v3.y + _34*v3.z + _44;

		return out;
	}
};




class GPC_CNL
{
public:
	GPCV3		xyz;
	GPCV3		ypr;	// forgás íránya a combinációs térben
	float		t;
	GPC_COLOR4	rgba;
	U4			n_update;
	U4			n_use;
	char*		p_str;
	char		s_name[0x20];
	
	GPC_CNL(void)
	{
		GPM_CLR;
	};
	
	GPC_CNL& operator = ( GPCMX mx )
	{
		char n = 0;
		if( xyz != mx.t4.xyz() )
		{
			xyz = mx.t4.xyz();
			n++;
		}
		GPCV3 nypr = mx.eula();
		if( nypr != ypr )
		{
			ypr = nypr;
			n++;
		}
		
		if( n )
			n_update = n_use;
		
		return *this;
	}
};







class GPC_GIO_VX4SSLL		
{
public:
	GPCV4	pos,		// 0 pozició.light		
			stf0,		// 16 
			stf8,		// 32
			lig0,		// 48
			lig8;		// 64
					// 80
};
class GPC_GIO_VX32CU		
{
public:
	GPCV3	pos;		// pozició		// 0	
	GPCV2	uv;			// UV map		// 12
	GPCF4	rgba;		// color		// 20
	
	GPCV3	up;			// normál fel	// 36
						// sizeof()	// 48
	GPC_GIO_VX32CU(void){};
	GPC_GIO_VX32CU( GPCV3 xyz, GPCV2 _uv, GPCV4 _rgba, GPCV3 origo )
	{
		pos = xyz;
		uv = _uv;
		rgba = _rgba;
		up = origo;
	}
};
// 0
//16
//32
//48
class GPC_GIO_VX32CUFBI
{
public:
	GPCV3	pos,		// pozició			// 0	// 12		
			up;			// normál fel		// 12	// 12	// 24
	GPCV2	uv;			// UV map			// 24	// 8	// 32
	GPCV4	rgba;		// color			// 32	// 16	// 48

	GPCV3	front;		// normál elörre	// 48	// 12	// 60
	U4		indices;	// indexe			// 60	// 4	// 64
	GPCV4	blend;		// súly				// 64	// 16	// 80
	GPC_GIO_VX32CUFBI(void) {};

};
/*class GPC_GIO_VX32CUFBI		
{
public:
	GPCV3	pos;		// pozició			// 0	// 12		
	GPCV2	uv;			// UV map			// 12	// 8
	GPCV4	rgba;		// color			// 20	// 16
	
	GPCV3	up;			// normál fel		// 36	// 12
	GPCV3	front;		// normál elörre	// 48	// 12
	GPCV4	blend;		// súly				// 60	// 16
	U4		indices;	// indexe			// 76	// 4
						// sizeof()			// 80
	GPC_GIO_VX32CUFBI(void){};
};*/

class GPC_GIO_VX32C
{
public:
	GPCV3	pos;	// pozició	// 0
	GPCV2	uv;		// UV map	// 12
	GPCF4	rgba;	// color	// 20
					// sizeof()	// 36
	GPC_GIO_VX32C(void){};
	GPC_GIO_VX32C( GPCV3 xyz, GPCV2 _uv, GPCV4 _rgba )
	{
		pos = xyz;
		uv = _uv;
		rgba = _rgba;
	}
	GPC_GIO_VX32C& operator += ( GPC_GIO_VX32C& v2)
	{
		pos += v2.pos;
		uv += v2.uv;
		rgba += v2.rgba;
		return *this;
	}
	GPC_GIO_VX32C& operator -= ( GPC_GIO_VX32C& v2)
	{
		pos -= v2.pos;
		uv -= v2.uv;
		rgba -= v2.rgba;
		return *this;
	}

	GPC_GIO_VX32C& operator *= ( float f )
	{
		if( f == 1.0f )
			return *this;
		if( f == 0.0f )
		{
			GPM_CLR;
			return *this;
		}
		pos *= f;
		uv *= f;
		rgba *= f;
		return *this;
	}

	GPC_GIO_VX32C operator + ( GPC_GIO_VX32C& v2)
	{
		GPC_GIO_VX32C out = *this;
		out += v2;
		return out;
	}
	GPC_GIO_VX32C operator - ( GPC_GIO_VX32C& v2)
	{
		GPC_GIO_VX32C out = *this;
		out -= v2;
		return out;
	}
	GPC_GIO_VX32C operator * ( float f )
	{
		GPC_GIO_VX32C out = *this;
		out *= f;
		return out;
	}

	GPC_GIO_VX32C& null( void )
	{
		GPM_CLR;
		return *this;
	}

	GPC_GIO_VX32C* vx324_gr( GPCV3* p_v3n9, GPCI4& gr, I8 mul, I8 div, GPCV4& hi, GPCV4& lo, I8 l );
};
//class GPC_GIO_VX32CUFBI		
//{
//public:
//	GPCV3	pos;		// pozició		// 0	
//	GPCV2	uv;		// UV map		// 12
//	GPCV4	rgba;		// color		// 20
//	GPCV3	up;		// normál fel	// 36
//	GPCV3	front;		// normál elörre	// 48
//	GPCV4	blend;		// súly		// 60
//	U4	indices;		// indexe		// 76
//				// sizeof()	// 80
//	GPC_GIO_VX32CUFBI(void){};
//};

class GPC_GPUx64_CONST
{
public:
	GPCV4	a_cnl[0x10];
	U1		b_gpu_c_upd, b_intel, pad1,pad2;	// 4
	U4		cnl_size, n_cnl, pad3;				// 8,12,16

	void cnl_reset( void )
	{
		b_intel = GPD_CBUFF;
		b_gpu_c_upd = true;
		cnl_size = GPM_OFF( GPC_GPUx64_CONST, b_gpu_c_upd );
		//cnl_size = (char*)(&b_gpu_c_upd) - (char*)this;
		n_cnl = 0;
	}
	GPC_GPUx64_CONST* cpy( void )
	{
		if( !this )
			return NULL;

		return (GPC_GPUx64_CONST*)GPM_MEMCPY( new GPC_GPUx64_CONST, this, 1 );
	}
	
	void init( void)
	{
		b_intel = GPD_CBUFF;
		cnl_size = GPM_OFF( GPC_GPUx64_CONST, b_gpu_c_upd );
		//cnl_size = (char*)(&b_gpu_c_upd) - (char*)this;
	}
};

class GPC_GIO_SURF
{
public:
	U4		s,n,is,in;					// 4x4 16

	I8		*p_pic, n_pic,
			gpu_id, gpu_ver,
			target_id;					// 5x8		// 40
	U1		b_idx, b_clr, aPAD[2+4];	// 1x2+6	// 8	//48+16 // 64
	char	s_part[0x20],
			s_surf[0x20];
	GPC_AN	surf_ps;					

	GPC_GPUx64_CONST	*p_cVS, *p_pad;

	GPC_GIO_SURF()
	{
		surf_clear();
	}
	/*
	GPC_GIO_SURF( GPC_GIO_SURF& b )
	{
		GPM_CLR;
		*this = b; 
	}
	GPC_GIO_SURF& operator = ( GPC_GIO_SURF& b )
	{
		s = b.s;
		n = b.n;
		is = b.is;
		in = b.in;
		GPM_STRCPY( s_part, b.s_part );
		GPM_STRCPY( s_surf, b.s_surf );
		if( n_pic != b.n_pic )
		{
			n_pic = b.n_pic;
			GPM_DELARRAY( p_pic );
			if( n_pic)
				p_pic = new I8[n_pic];
		}
		if( n_pic )
			gp_memcpy( p_pic, b.p_pic, n_pic*sizeof(*p_pic) );
		gpu_id = b.gpu_id;
		target_id = b.target_id;
		return *this;
	}*/
	~GPC_GIO_SURF()
	{
		GPM_DELARRAY( p_pic );
		GPM_DEL( p_cVS );
	}
	void surf_clear( void )
	{
		GPM_CLR;
		target_id = -1;
		gpu_id = -1;
	}
	GPC_GIO_SURF* surf_reset( GPC_GIO_SURF* p_back = NULL )
	{
		if( !this )
			return NULL;

		GPM_DELARRAY( p_pic );
		GPM_DEL( p_cVS );
		surf_clear();
		if( !p_back )
			return this;

		s = p_back->n;
		is =  p_back->in;
		return this;
	}

	I8*		surf_read( char* p_mem, char* p_end );
	
	
	
};


class GPCELL_BUFF
{
public:
	GPCV2		*p_alloc, *p_vox, *p_voxo;
	ULL		n_alloc_vx;
	GPC_GIO_SURF	*p_surf;
	ULL		n_alloc_surf;
	
	U4		*p_alloc_idx, *p_ix, *p_ixo;
	ULL		n_alloc_idx;

	GPCELL_BUFF()
	{
		GPM_CLR;
	}

	GPCELL_BUFF( I8 n_vx_plus, I8 n_idx_plus )
	{
		GPM_CLR;
		cell_expand( n_vx_plus );
		cell_idx_expand( n_idx_plus );
	}

	GPCV2* cell_add( I8 n_add )
	{
		if( n_add < 0 )
		{
			return p_voxo = p_vox = p_alloc+n_alloc_vx;
		}
		p_voxo += n_add;
		if( p_vox < p_voxo )
			p_vox = p_voxo;
		
		if( p_vox > p_alloc+n_alloc_vx )
			p_voxo = p_vox = p_alloc+n_alloc_vx;

		return p_voxo;
	}

	U4* cell_idx_add( I8 n_add )
	{
		if( n_add < 0 )
		{
			return p_ixo = p_ix = p_alloc_idx+n_alloc_idx;
		}
		p_ixo += n_add;
		if( p_ix < p_ixo )
			p_ix = p_ixo;

		if( p_ix > p_alloc_idx+n_alloc_idx )
			p_ixo = p_ix = p_alloc_idx+n_alloc_idx;
		
		return p_ixo;
	}

	GPCV2* cell_expand( ULL n_add, bool b_surf = true )
	{
		if (!this)
			return NULL;
		if( !n_add )
			return p_voxo;
		//if( !p_alloc )
		//{
		//	n_alloc = 0;
		//	GPM_DELARRAY( p_surf );
		//}

		if( !p_surf )				// megsemmisült a p_surface 
			n_alloc_surf = 0;			// akor nem lehet értéke

		if( n_alloc_surf < 1 )
		{
			n_alloc_surf = 0;
			GPM_DELARRAY( p_surf );		// niincs értéke nem lehet p_surface, ha valamiért van megsemmisíteni
		} 

		if( p_vox < p_alloc )
		{
			// valami gebasz talán mégg NULL vagy rossz helyen van a p_vox 
			p_voxo = p_vox = p_alloc;
			p_ixo = p_ix = p_alloc_idx;
		}
		else if( p_vox > p_alloc+n_alloc_vx )
		{
			p_vox = p_alloc+n_alloc_vx;
		}

		if( p_ix < p_alloc_idx )
		{
			p_ixo = p_ix = p_alloc_idx;
		}
		else if( p_ix > p_alloc_idx+n_alloc_idx )
		{
			p_ix = p_alloc_idx+n_alloc_idx;
		}

		if( !b_surf )
		{
			p_voxo = p_vox;
			p_ixo = p_ix;
		}
		else if( p_surf )
		{
			GPC_GIO_SURF* p_s = p_surf+n_alloc_surf-1;
			
			//------------------------------
			//
			//        -- VERTEX --
			//
			// p_alloc	...p_voxo	...p_vox 
			// p_s	...s	+n
			//
			//-----------------------------------
			if( p_s->s < 0 )
				p_s->s = 0;
			else if( p_s->s > p_vox-p_alloc )
			{
				p_s->s = p_vox-p_alloc;
				p_s->n = 0;
			}
									
			p_voxo = p_alloc + p_s->s+p_s->n;
			if( p_voxo > p_vox )
			{
				p_voxo = p_vox;
				p_s->n = p_voxo-p_alloc;
				p_s->n -= p_s->s;
			}
						
			//------------------------------
			//
			//        -- INDEX --
			//
			// p_alloc_index	...p_ixo	...p_ix 
			// p_s		...is	+in
			//
			//-----------------------------------
			if( p_s->is < 0 )
				p_s->is = 0;
			else if( p_s->is > p_ix-p_alloc_idx )
			{
				p_s->is = p_ix-p_alloc_idx;
				p_s->in = 0;
			}

			p_ixo = p_alloc_idx + p_s->is+p_s->in;
			if( p_ixo > p_ix )
			{
				p_ixo = p_ix;
				p_s->in = p_ixo-p_alloc_idx;
				p_s->in -= p_s->is;
			}
						
		} else {
			p_voxo = p_vox;
			p_ixo = p_ix;
		}
		

		ULL n_new = p_voxo - p_alloc;
		n_new += n_add;
		if( n_new&0xf )
			n_new += 0x10-(n_new&0xf);

		if( n_new <= n_alloc_vx )
			return p_voxo;
		
		//if( n_new & 0xf )
		//{
		//	n_new &= 0xfffffffffffffff0;
		//	n_new += 0x10;
		//}
		
		GPCV2* p_kill = p_alloc;
		n_alloc_vx = n_new;
		p_alloc = new GPCV2[n_alloc_vx];
		if( !p_kill )
		{
			return p_voxo = p_vox = p_alloc;
		}
		ULL	n_vx = p_vox > p_kill ? p_vox - p_kill : 0,
			n_vxo = p_voxo > p_kill ? p_voxo - p_kill : 0;

		if( n_vx > n_alloc_vx )
			n_vx = n_alloc_vx;
		if( n_vx )
			gp_memcpy( p_alloc, p_kill, n_vx*sizeof(*p_kill) );
		
		delete[] p_kill; //_aligned_free(p_kill);

		p_voxo = p_alloc+n_vxo;
		p_vox = p_alloc+n_vx;

		return p_voxo;
	}

	U4* cell_idx_expand( ULL n_add )
	{
		if (!this)
			return NULL;
		
		if( !n_add )
			return p_ixo;
		
		if( !p_alloc_idx )
		{
			p_ix = p_ixo = NULL;
		}
		
		ULL n_new = (p_ixo - p_alloc_idx) + n_add;
		if( n_new&0xf )
			n_new += 0x10-(n_new&0xf);

		if( n_new <= n_alloc_idx )
			return p_ixo;
		
		//if( n_new & 0xf )
		//{
		//	n_new &= 0xfffffffffffffff0;
		//	n_new += 0x10;
		//}
		
		U4* p_kill = p_alloc_idx;
		n_alloc_idx = n_new;
		//p_alloc = (GPC_GIO_VX32C*)_aligned_malloc(n_alloc*sizeof(GPC_GIO_VX32C), 0x10);
		p_alloc_idx = new U4[n_alloc_idx];
		if( !p_kill )
			return p_ix = p_ixo = p_alloc_idx;
		ULL	n_ix = p_ix - p_kill,
			n_ixo = p_ixo - p_kill;

		gp_memcpy( p_alloc_idx, p_kill, n_ix*sizeof(*p_kill) );
		//gp_memcpy( p_alloc_idx, p_kill, n_ix*sizeof(*p_kill) );
		delete[] p_kill; //_aligned_free(p_kill);
		
		p_ix = p_alloc_idx+n_ix;
		p_ixo = p_alloc_idx+n_ixo;

		return p_ixo;
	}

	void cell_clr( void )
	{
		if (!this)
			return;
		
		delete[] p_alloc;
		delete[] p_alloc_idx;
		delete[] p_surf;

		GPM_CLR;

	}

	GPCV2* cell_null( void )
	{
		if (!this)
			return NULL;
		
		p_ixo = p_ix = p_alloc_idx;
		GPM_DELARRAY(p_surf);
		n_alloc_surf = 0;

		return p_voxo = p_vox = p_alloc;
	}

	GPCV2* cell_reset( void )
	{
		if (!this)
			return NULL;
		// VERTEX
		ULL n_old = p_vox - p_alloc;
		if( n_old & 0xf )
		{
			n_old += 0x10;
			n_old &= 0xfffffffffffffff0;
		}
		if( n_old == n_alloc_vx )
			p_vox = p_alloc;
		else {
			delete[] p_alloc; //_aligned_free(p_alloc);
		
			n_alloc_vx = n_old;
			p_vox = p_alloc = new GPCV2[n_alloc_vx];
		}

		// INDEX
		n_old = p_ix - p_alloc_idx;
		if( n_old & 0xf )
		{
			n_old += 0x10;
			n_old &= 0xfffffffffffffff0;
		}

		if( n_old == n_alloc_idx )
			p_ix = p_alloc_idx;
		else {
			delete[] p_alloc_idx; //_aligned_free(p_alloc);
		
			n_alloc_idx = n_old;
			p_ix = p_alloc_idx = new U4[n_alloc_idx];
		}
		
		GPM_DELARRAY(p_surf);
		n_alloc_surf = 0;

		p_ixo = p_ix;
		return p_voxo = p_vox = p_alloc;
		
	}
	~GPCELL_BUFF()
	{
		delete[] p_alloc;
		delete[] p_alloc_idx;
		delete[] p_surf;
	}
	GPC_GIO_SURF* cell_surf_pre( I8 target_id, I8 gpu_id, I8 *p_pic, I8 n_pic )
	{
		if( !p_surf )
		{
			if( p_vox == p_alloc )
			{
				// ha most kezdödik csak egyet nyit
				n_alloc_surf = 1;
				p_surf = new GPC_GIO_SURF[n_alloc_surf];
				p_surf->gpu_id = gpu_id;
				if( p_pic && n_pic > 0 )
				{
					p_surf->p_pic = (I8*)gp_memcpy( new I8[n_pic], p_pic, n_pic*sizeof(*p_pic) );
					p_surf->n_pic = n_pic;
				}
				p_surf->target_id = target_id;
				p_surf->n = 0;
				return p_surf;
			}
			// ha már volt lezárja és nyit egy újat
			p_surf = new GPC_GIO_SURF[n_alloc_surf = 2];

			p_surf[1].s = p_surf[0].n = p_vox-p_alloc;
			p_surf[1].is = p_surf[0].in = p_ix-p_alloc_idx;
			p_surf[1].gpu_id = gpu_id;
			p_surf[1].target_id = target_id;
			if( p_pic && n_pic > 0 )
			{
				p_surf[1].p_pic = (I8*)gp_memcpy( new I8[n_pic], p_pic, n_pic*sizeof(*p_pic) );
				p_surf[1].n_pic = n_pic;
			}
			return p_surf+n_alloc_surf-1;
		}

		
		GPC_GIO_SURF *p_s = p_surf+n_alloc_surf-1;
		
		if( p_s->s > p_voxo-p_alloc )
			p_s->s = p_voxo-p_alloc;

		if( p_s->is > p_ixo-p_alloc_idx )
			p_s->is = p_ixo-p_alloc_idx;

		if( !p_s->n )
		{
			GPM_DELARRAY( p_s->p_pic );
			p_s->p_pic = (I8*)gp_memcpy( new I8[n_pic], p_pic, n_pic*sizeof(*p_pic) );
			p_s->n_pic = n_pic;
			p_s->gpu_id = gpu_id;

			p_s->in = 0;
			return p_s;
		}
		else if( p_alloc + p_s->s+p_s->n != p_voxo )
		{
			p_s->n = p_voxo-p_alloc-p_s->s;
		}

		if( p_alloc_idx + p_s->is + p_s->in != p_ixo )
		{
			p_s->in = p_ixo - p_alloc_idx-p_s->is;
		}

		if( target_id < 0 || target_id == p_s->target_id )
		if( gpu_id == p_s->gpu_id )
		if( !p_pic && !p_s->p_pic )
		{
			return p_s;
		} 
		else if( p_pic && p_s->p_pic )
		if( n_pic <= p_s->n_pic )
		{
			I8 *p_in = p_pic, *p_ine = p_in+n_pic, *p_cmp = p_s->p_pic; 
			for( ; p_in < p_ine; p_in++,p_cmp++ )
			{
				if( *p_in != *p_cmp )
					break;
			}

			if( p_in == p_ine)
			{
				// nem kell új
				return p_s;
			}
		}

		
		I8 n_surf_over = n_alloc_surf;
		n_alloc_surf++;
		GPC_GIO_SURF* p_kill = p_surf;
		p_surf = new GPC_GIO_SURF[n_alloc_surf];
		if( p_kill )
		{
			gp_memcpy( p_surf, p_kill, n_surf_over*sizeof(*p_surf) );
			memset( p_kill, 0, n_surf_over*sizeof(*p_surf) );
		}
		GPM_DELARRAY( p_kill );
		p_s = p_surf+n_surf_over;
		

		p_s = p_surf+n_surf_over;
		p_s->gpu_id = gpu_id;
		if( p_pic && n_pic > 0 )
		{
			p_s->p_pic = (I8*)gp_memcpy( new I8[n_pic], p_pic, n_pic*sizeof(*p_pic) );
			p_s->n_pic = n_pic;
		}
		
		p_s->target_id = ( target_id > 0 ) ? target_id : p_s[-1].target_id;
		p_s->s = p_voxo-p_alloc;
		p_s->is = p_ixo-p_alloc_idx;
		p_s->in = p_s->n = 0;

		return p_s;
	}

	GPC_GIO_SURF* cell_surf_add( I8 target_id, I8 gpu_id, I8 *p_pic, I8 n_pic )
	{
		if( p_vox == p_alloc )
			return NULL;

		if( !p_surf )
		{
			//n_surf_over = 0;
			p_surf = new GPC_GIO_SURF[ n_alloc_surf = 1 ];
			
			p_surf->s = 0;
			p_surf->n = p_voxo-p_alloc;
			p_surf->gpu_id = gpu_id;
			p_surf->target_id = target_id;
			if( p_pic && n_pic > 0 )
			{
				p_surf->p_pic = (I8*)gp_memcpy( new I8[n_pic], p_pic, n_pic*sizeof(*p_pic) );
				p_surf->n_pic = n_pic;
			}
			p_surf->in = p_ixo-p_alloc_idx;
			p_surf->in -= p_surf->is;
			return p_surf;
		}

		//n_surf_over = n_alloc_surf-1;
		p_voxo = p_vox;
		p_ixo = p_ix;
		GPC_GIO_SURF *p_s = p_surf+n_alloc_surf-1;

		if( p_s->s > p_voxo-p_alloc )
			p_s->s = p_voxo-p_alloc;

		if( p_s->is > p_ixo-p_alloc_idx )
			p_s->is = p_ixo-p_alloc_idx;
		

		if( target_id < 0 || target_id == p_s->target_id )
		if( gpu_id == p_s->gpu_id )
		if( !p_pic && !p_s->p_pic )
		{
			//p_surf->in = p_idx-p_alloc_idx;
			return p_s;
		} 
		else if( p_pic && p_s->p_pic )
		if( n_pic <= p_s->n_pic )
		{
			I8 *p_in = p_pic, *p_ine = p_in+n_pic, *p_cmp = p_s->p_pic; 
			for( ; p_in < p_ine; p_in++,p_cmp++ )
			{
				if( *p_in != *p_cmp )
					break;
			}

			if( p_in == p_ine)
			{
				// nem kell új
				return p_s;
			}
		}

		I8 n_surf_over = n_alloc_surf;
		n_alloc_surf++;
		GPC_GIO_SURF* p_kill = p_surf;
		p_surf = new GPC_GIO_SURF[n_alloc_surf];
		if( p_kill )
		{
			gp_memcpy( p_surf, p_kill, n_surf_over*sizeof(*p_surf) );
			memset( p_kill, 0, n_surf_over*sizeof(*p_surf) );
		}
		GPM_DELARRAY( p_kill );
		p_s = p_surf+n_surf_over;
		
		
		p_s->gpu_id = gpu_id;
		if( p_pic && n_pic > 0 )
		{
			p_s->p_pic = (I8*)gp_memcpy( new I8[n_pic], p_pic, n_pic*sizeof(*p_pic) );
			p_s->n_pic = n_pic;
		}
		

		p_s->target_id = ( target_id > 0 ) ? target_id : p_s[-1].target_id;
		p_s->s = 0; 
		p_s->n = p_voxo-p_alloc;

		p_s->is = p_s[-1].in+p_s[-1].is;
		p_s->in = (p_ixo-p_alloc_idx) - p_s->is;
		return p_s;
	}
	
	
	//GPCELL_BUFF& operator += ( GPCELL_BUFF& b );
	//GPCELL_BUFF& operator |= ( GPCELL_BUFF& b );

};

class GPCVOX
{
public:
	GPCV4	p,
			t,
			n,
			c;
};

class GPCVOX_BUFF
{
public:
	GPCVOX		*p_alloc, *p_vox, *p_voxo;
	ULL		n_alloc_vx;
	GPC_GIO_SURF	*p_surf;
	ULL		n_alloc_surf;
	
	U4		*p_alloc_idx, *p_ix, *p_ixo;
	ULL		n_alloc_idx;

	GPCVOX_BUFF()
	{
		GPM_CLR;
	}

	GPCVOX_BUFF( I8 n_vx_plus, I8 n_idx_plus )
	{
		GPM_CLR;
		vox_expand( n_vx_plus );
		vox_idx_expand( n_idx_plus );
	}

	GPCVOX* vox_add( I8 n_add )
	{
		if( n_add < 0 )
		{
			return p_voxo = p_vox = p_alloc+n_alloc_vx;
		}
		p_voxo += n_add;
		if( p_vox < p_voxo )
			p_vox = p_voxo;
		
		if( p_vox > p_alloc+n_alloc_vx )
			p_voxo = p_vox = p_alloc+n_alloc_vx;

		return p_voxo;
	}

	U4* vox_idx_add( I8 n_add )
	{
		if( n_add < 0 )
		{
			return p_ixo = p_ix = p_alloc_idx+n_alloc_idx;
		}
		p_ixo += n_add;
		if( p_ix < p_ixo )
			p_ix = p_ixo;

		if( p_ix > p_alloc_idx+n_alloc_idx )
			p_ixo = p_ix = p_alloc_idx+n_alloc_idx;
		
		return p_ixo;
	}

	GPCVOX* vox_expand( ULL n_add, bool b_surf = true )
	{
		if (!this)
			return NULL;
		if( !n_add )
			return p_voxo;
		//if( !p_alloc )
		//{
		//	n_alloc = 0;
		//	GPM_DELARRAY( p_surf );
		//}

		if( !p_surf )				// megsemmisült a p_surface 
			n_alloc_surf = 0;			// akor nem lehet értéke

		if( n_alloc_surf < 1 )
		{
			n_alloc_surf = 0;
			GPM_DELARRAY( p_surf );		// niincs értéke nem lehet p_surface, ha valamiért van megsemmisíteni
		} 

		if( p_vox < p_alloc )
		{
			// valami gebasz talán mégg NULL vagy rossz helyen van a p_vox 
			p_voxo = p_vox = p_alloc;
			p_ixo = p_ix = p_alloc_idx;
		}
		else if( p_vox > p_alloc+n_alloc_vx )
		{
			p_vox = p_alloc+n_alloc_vx;
		}

		if( p_ix < p_alloc_idx )
		{
			p_ixo = p_ix = p_alloc_idx;
		}
		else if( p_ix > p_alloc_idx+n_alloc_idx )
		{
			p_ix = p_alloc_idx+n_alloc_idx;
		}

		if( !b_surf )
		{
			p_voxo = p_vox;
			p_ixo = p_ix;
		}
		else if( p_surf )
		{
			GPC_GIO_SURF* p_s = p_surf+n_alloc_surf-1;
			
			//------------------------------
			//
			//        -- VERTEX --
			//
			// p_alloc	...p_voxo	...p_vox 
			// p_s	...s	+n
			//
			//-----------------------------------
			if( p_s->s < 0 )
				p_s->s = 0;
			else if( p_s->s > p_vox-p_alloc )
			{
				p_s->s = p_vox-p_alloc;
				p_s->n = 0;
			}
									
			p_voxo = p_alloc + p_s->s+p_s->n;
			if( p_voxo > p_vox )
			{
				p_voxo = p_vox;
				p_s->n = p_voxo-p_alloc;
				p_s->n -= p_s->s;
			}
						
			//------------------------------
			//
			//        -- INDEX --
			//
			// p_alloc_index	...p_ixo	...p_ix 
			// p_s		...is	+in
			//
			//-----------------------------------
			if( p_s->is < 0 )
				p_s->is = 0;
			else if( p_s->is > p_ix-p_alloc_idx )
			{
				p_s->is = p_ix-p_alloc_idx;
				p_s->in = 0;
			}

			p_ixo = p_alloc_idx + p_s->is+p_s->in;
			if( p_ixo > p_ix )
			{
				p_ixo = p_ix;
				p_s->in = p_ixo-p_alloc_idx;
				p_s->in -= p_s->is;
			}
						
		} else {
			p_voxo = p_vox;
			p_ixo = p_ix;
		}
		

		ULL n_new = p_voxo - p_alloc;
		n_new += n_add;
		if( n_new&0xf )
			n_new += 0x10-(n_new&0xf);

		if( n_new <= n_alloc_vx )
			return p_voxo;
		
		//if( n_new & 0xf )
		//{
		//	n_new &= 0xfffffffffffffff0;
		//	n_new += 0x10;
		//}
		
		GPCVOX* p_kill = p_alloc;
		n_alloc_vx = n_new;
		p_alloc = new GPCVOX[n_alloc_vx];
		if( !p_kill )
		{
			return p_voxo = p_vox = p_alloc;
		}
		ULL	n_vx = p_vox > p_kill ? p_vox - p_kill : 0,
			n_vxo = p_voxo > p_kill ? p_voxo - p_kill : 0;

		if( n_vx > n_alloc_vx )
			n_vx = n_alloc_vx;
		if( n_vx )
			gp_memcpy( p_alloc, p_kill, n_vx*sizeof(*p_kill) );
		
		delete[] p_kill; //_aligned_free(p_kill);

		p_voxo = p_alloc+n_vxo;
		p_vox = p_alloc+n_vx;

		return p_voxo;
	}

	U4* vox_idx_expand( ULL n_add )
	{
		if (!this)
			return NULL;
		
		if( !n_add )
			return p_ixo;
		
		if( !p_alloc_idx )
		{
			p_ix = p_ixo = NULL;
		}
		
		ULL n_new = (p_ixo - p_alloc_idx) + n_add;
		if( n_new&0xf )
			n_new += 0x10-(n_new&0xf);

		if( n_new <= n_alloc_idx )
			return p_ixo;
		
		//if( n_new & 0xf )
		//{
		//	n_new &= 0xfffffffffffffff0;
		//	n_new += 0x10;
		//}
		
		U4* p_kill = p_alloc_idx;
		n_alloc_idx = n_new;
		//p_alloc = (GPC_GIO_VX32C*)_aligned_malloc(n_alloc*sizeof(GPC_GIO_VX32C), 0x10);
		p_alloc_idx = new U4[n_alloc_idx];
		if( !p_kill )
			return p_ix = p_ixo = p_alloc_idx;
		ULL	n_ix = p_ix - p_kill,
			n_ixo = p_ixo - p_kill;

		gp_memcpy( p_alloc_idx, p_kill, n_ix*sizeof(*p_kill) );
		//gp_memcpy( p_alloc_idx, p_kill, n_ix*sizeof(*p_kill) );
		delete[] p_kill; //_aligned_free(p_kill);
		
		p_ix = p_alloc_idx+n_ix;
		p_ixo = p_alloc_idx+n_ixo;

		return p_ixo;
	}

	void vox_clr( void )
	{
		if (!this)
			return;
		
		delete[] p_alloc;
		delete[] p_alloc_idx;
		delete[] p_surf;

		GPM_CLR;

	}

	GPCVOX* vox_null( void )
	{
		if (!this)
			return NULL;
		
		p_ixo = p_ix = p_alloc_idx;
		GPM_DELARRAY(p_surf);
		n_alloc_surf = 0;

		return p_voxo = p_vox = p_alloc;
	}

	GPCVOX* vox_reset( void )
	{
		if (!this)
			return NULL;
		// VERTEX
		ULL n_old = p_vox - p_alloc;
		if( n_old & 0xf )
		{
			n_old += 0x10;
			n_old &= 0xfffffffffffffff0;
		}
		if( n_old == n_alloc_vx )
			p_vox = p_alloc;
		else {
			delete[] p_alloc; //_aligned_free(p_alloc);
		
			n_alloc_vx = n_old;
			p_vox = p_alloc = new GPCVOX[n_alloc_vx];
		}

		// INDEX
		n_old = p_ix - p_alloc_idx;
		if( n_old & 0xf )
		{
			n_old += 0x10;
			n_old &= 0xfffffffffffffff0;
		}

		if( n_old == n_alloc_idx )
			p_ix = p_alloc_idx;
		else {
			delete[] p_alloc_idx; //_aligned_free(p_alloc);
		
			n_alloc_idx = n_old;
			p_ix = p_alloc_idx = new U4[n_alloc_idx];
		}
		
		GPM_DELARRAY(p_surf);
		n_alloc_surf = 0;

		p_ixo = p_ix;
		return p_voxo = p_vox = p_alloc;
		
	}
	~GPCVOX_BUFF()
	{
		delete[] p_alloc;
		delete[] p_alloc_idx;
		delete[] p_surf;
	}
	GPC_GIO_SURF* vox_surf_pre( I8 target_id, I8 gpu_id, I8 *p_pic, I8 n_pic )
	{
		if( !p_surf )
		{
			if( p_vox == p_alloc )
			{
				// ha most kezdödik csak egyet nyit
				n_alloc_surf = 1;
				p_surf = new GPC_GIO_SURF[n_alloc_surf];
				p_surf->gpu_id = gpu_id;
				if( p_pic && n_pic > 0 )
				{
					p_surf->p_pic = (I8*)gp_memcpy( new I8[n_pic], p_pic, n_pic*sizeof(*p_pic) );
					p_surf->n_pic = n_pic;
				}
				p_surf->target_id = target_id;
				p_surf->n = 0;
				return p_surf;
			}
			// ha már volt lezárja és nyit egy újat
			p_surf = new GPC_GIO_SURF[n_alloc_surf = 2];

			p_surf[1].s = p_surf[0].n = p_vox-p_alloc;
			p_surf[1].is = p_surf[0].in = p_ix-p_alloc_idx;
			p_surf[1].gpu_id = gpu_id;
			p_surf[1].target_id = target_id;
			if( p_pic && n_pic > 0 )
			{
				p_surf[1].p_pic = (I8*)gp_memcpy( new I8[n_pic], p_pic, n_pic*sizeof(*p_pic) );
				p_surf[1].n_pic = n_pic;
			}
			return p_surf+n_alloc_surf-1;
		}

		
		GPC_GIO_SURF *p_s = p_surf+n_alloc_surf-1;
		
		if( p_s->s > p_voxo-p_alloc )
			p_s->s = p_voxo-p_alloc;

		if( p_s->is > p_ixo-p_alloc_idx )
			p_s->is = p_ixo-p_alloc_idx;

		if( !p_s->n )
		{
			GPM_DELARRAY( p_s->p_pic );
			p_s->p_pic = (I8*)gp_memcpy( new I8[n_pic], p_pic, n_pic*sizeof(*p_pic) );
			p_s->n_pic = n_pic;
			p_s->gpu_id = gpu_id;

			p_s->in = 0;
			return p_s;
		}
		else if( p_alloc + p_s->s+p_s->n != p_voxo )
		{
			p_s->n = p_voxo-p_alloc-p_s->s;
		}

		if( p_alloc_idx + p_s->is + p_s->in != p_ixo )
		{
			p_s->in = p_ixo - p_alloc_idx-p_s->is;
		}

		if( target_id < 0 || target_id == p_s->target_id )
		if( gpu_id == p_s->gpu_id )
		if( !p_pic && !p_s->p_pic )
		{
			return p_s;
		} 
		else if( p_pic && p_s->p_pic )
		if( n_pic <= p_s->n_pic )
		{
			I8 *p_in = p_pic, *p_ine = p_in+n_pic, *p_cmp = p_s->p_pic; 
			for( ; p_in < p_ine; p_in++,p_cmp++ )
			{
				if( *p_in != *p_cmp )
					break;
			}

			if( p_in == p_ine)
			{
				// nem kell új
				return p_s;
			}
		}

		
		I8 n_surf_over = n_alloc_surf;
		n_alloc_surf++;
		GPC_GIO_SURF* p_kill = p_surf;
		p_surf = new GPC_GIO_SURF[n_alloc_surf];
		if( p_kill )
		{
			gp_memcpy( p_surf, p_kill, n_surf_over*sizeof(*p_surf) );
			memset( p_kill, 0, n_surf_over*sizeof(*p_surf) );
		}
		GPM_DELARRAY( p_kill );
		p_s = p_surf+n_surf_over;
		

		p_s = p_surf+n_surf_over;
		p_s->gpu_id = gpu_id;
		if( p_pic && n_pic > 0 )
		{
			p_s->p_pic = (I8*)gp_memcpy( new I8[n_pic], p_pic, n_pic*sizeof(*p_pic) );
			p_s->n_pic = n_pic;
		}
		
		p_s->target_id = ( target_id > 0 ) ? target_id : p_s[-1].target_id;
		p_s->s = p_voxo-p_alloc;
		p_s->is = p_ixo-p_alloc_idx;
		p_s->in = p_s->n = 0;

		return p_s;
	}

	GPC_GIO_SURF* vox_surf_add( I8 target_id, I8 gpu_id, I8 *p_pic, I8 n_pic )
	{
		if( p_vox == p_alloc )
			return NULL;

		if( !p_surf )
		{
			//n_surf_over = 0;
			p_surf = new GPC_GIO_SURF[ n_alloc_surf = 1 ];
			
			p_surf->s = 0;
			p_surf->n = p_voxo-p_alloc;
			p_surf->gpu_id = gpu_id;
			p_surf->target_id = target_id;
			if( p_pic && n_pic > 0 )
			{
				p_surf->p_pic = (I8*)gp_memcpy( new I8[n_pic], p_pic, n_pic*sizeof(*p_pic) );
				p_surf->n_pic = n_pic;
			}
			p_surf->in = p_ixo-p_alloc_idx;
			p_surf->in -= p_surf->is;
			return p_surf;
		}

		//n_surf_over = n_alloc_surf-1;
		p_voxo = p_vox;
		p_ixo = p_ix;
		GPC_GIO_SURF *p_s = p_surf+n_alloc_surf-1;

		if( p_s->s > p_voxo-p_alloc )
			p_s->s = p_voxo-p_alloc;

		if( p_s->is > p_ixo-p_alloc_idx )
			p_s->is = p_ixo-p_alloc_idx;
		
		//if( p_alloc + p_s->s+p_s->n != p_voxo )
		//{
		//	// ha nagy a p_s->n
		//	p_s->n = p_voxo-p_alloc-p_s->s;
		//}
		//
		//if( p_alloc_idx + p_s->is+p_s->in != p_ixo )
		//{
		//	// ha nagy a p_s->in
		//	p_s->in = p_ixo - p_alloc_idx-p_s->is;
		//}

		if( target_id < 0 || target_id == p_s->target_id )
		if( gpu_id == p_s->gpu_id )
		if( !p_pic && !p_s->p_pic )
		{
			//p_surf->in = p_idx-p_alloc_idx;
			return p_s;
		} 
		else if( p_pic && p_s->p_pic )
		if( n_pic <= p_s->n_pic )
		{
			I8 *p_in = p_pic, *p_ine = p_in+n_pic, *p_cmp = p_s->p_pic; 
			for( ; p_in < p_ine; p_in++,p_cmp++ )
			{
				if( *p_in != *p_cmp )
					break;
			}

			if( p_in == p_ine)
			{
				// nem kell új
				return p_s;
			}
		}

		I8 n_surf_over = n_alloc_surf;
		n_alloc_surf++;
		GPC_GIO_SURF* p_kill = p_surf;
		p_surf = new GPC_GIO_SURF[n_alloc_surf];
		if( p_kill )
		{
			gp_memcpy( p_surf, p_kill, n_surf_over*sizeof(*p_surf) );
			memset( p_kill, 0, n_surf_over*sizeof(*p_surf) );
		}
		GPM_DELARRAY( p_kill );
		p_s = p_surf+n_surf_over;
		
		
		p_s->gpu_id = gpu_id;
		if( p_pic && n_pic > 0 )
		{
			p_s->p_pic = (I8*)gp_memcpy( new I8[n_pic], p_pic, n_pic*sizeof(*p_pic) );
			p_s->n_pic = n_pic;
		}
		

		p_s->target_id = ( target_id > 0 ) ? target_id : p_s[-1].target_id;
		p_s->s = 0; 
		p_s->n = p_voxo-p_alloc;

		p_s->is = p_s[-1].in+p_s[-1].is;
		p_s->in = (p_ixo-p_alloc_idx) - p_s->is;
		return p_s;
	}
	
	
	

};



class GPC_GIO_BUFF_VX324
{
public:
	GPC_GIO_VX32C	*p_alloc, *p_vx;
	ULL				n_alloc;
	GPC_GIO_SURF	*p_surf;
	ULL				n_surf_load;
	
	U4				*p_alloc_idx, *p_idx;
	ULL				n_alloc_idx;
	
	//I8		max_dif;
	
	GPC_GIO_BUFF_VX324()
	{
		GPM_CLR;
	}
	void VX324_clear(void)
	{
		GPM_DELARRAY( p_alloc );
		GPM_DELARRAY( p_surf );
		GPM_DELARRAY( p_alloc_idx );
		GPM_CLR;
	}
	~GPC_GIO_BUFF_VX324()
	{
		VX324_clear();
	}
	GPC_GIO_VX32C* VX324_add(ULL n_add)
	{
		p_vx += n_add;
		if( p_vx > p_alloc+n_alloc )
			p_vx = p_alloc+n_alloc;
		return p_vx;
	}
	

	GPC_GIO_VX32C* VX324_expand( ULL n_add )
	{
		if( !this )
			return NULL;

		ULL n_new = p_vx - p_alloc + n_add;
		if( n_new < n_alloc )
			return p_vx;
		
		if( n_new & 0xf )
		{
			n_new += 0x10;
			n_new &= 0xfffffffffffffff0;
		}
		
		GPC_GIO_VX32C* p_kill = p_alloc;
		n_alloc = n_new;
		//p_alloc = (GPC_GIO_VX32C*)_aligned_malloc(n_alloc*sizeof(GPC_GIO_VX32C), 0x10);
		p_alloc = new GPC_GIO_VX32C[n_alloc];
		if( !p_kill )
			return p_vx = p_alloc;
		ULL n = p_vx - p_kill;
		gp_memcpy( p_alloc, p_kill, n*sizeof(*p_kill) );
		delete[] p_kill; //_aligned_free(p_kill);

		return p_vx = (p_alloc + n);
	}

	//U4* VX324_idx_add( ULL n_add )
	//{
	//	p_idx += n_add;
	//	if( p_idx > p_alloc_idx+n_alloc_idx )
	//		p_idx = p_alloc_idx+n_alloc_idx;
	//	return p_idx;
	//}
	U4* VX324_idx_add(ULL n_add)
	{
		p_idx += n_add;
		if( p_idx > p_alloc_idx+n_alloc_idx )
			p_idx = p_alloc_idx+n_alloc_idx;
		return p_idx;
	}

	U4* VX324_IDX_expand( ULL n_add )
	{
		if (!this)
			return NULL;

		ULL n_new = p_idx - p_alloc_idx + n_add;
		if( n_new < n_alloc_idx )
			return p_idx;
		
		if( n_new & 0xf )
		{
			n_new += 0x10;
			n_new &= 0xfffffffffffffff0;
		}
		
		U4* p_kill = p_alloc_idx;
		n_alloc_idx = n_new;
		//p_alloc = (GPC_GIO_VX32C*)_aligned_malloc(n_alloc*sizeof(GPC_GIO_VX32C), 0x10);
		p_alloc_idx = new U4[n_alloc_idx];
		if( !p_kill )
			return p_idx = p_alloc_idx;
		ULL n = p_idx - p_kill;
		gp_memcpy( p_alloc_idx, p_kill, n*sizeof(*p_kill) );
		delete[] p_kill; //_aligned_free(p_kill);

		return p_idx = (p_alloc_idx + n);
	}

	GPC_GIO_VX32C* VX324_reset( void )
	{
		if (!this)
			return NULL;
		// VERTEX
		ULL n_old = p_vx - p_alloc;
		if( n_old & 0xf )
		{
			n_old += 0x10;
			n_old &= 0xfffffffffffffff0;
		}
		ULL n_dif = n_alloc - n_old;
		if( !n_dif )
			p_vx = p_alloc;
		else if( n_dif < n_old )
		{
			p_vx = p_alloc;
		} else {

			delete[] p_alloc; //_aligned_free(p_alloc);
			
			n_alloc += n_old;
			n_alloc /= 2;
			if( n_alloc < 128 )
				n_alloc = 128;
			
			p_vx = p_alloc = new GPC_GIO_VX32C[n_alloc];
		}

		// INDEX
		n_old = p_idx - p_alloc_idx;
		if( n_old & 0xf )
		{
			n_old += 0x10;
			n_old &= 0xfffffffffffffff0;
		}

		n_dif = n_alloc_idx-n_old;
		if( !n_dif )
			p_idx = p_alloc_idx;
		else if( n_dif < n_old )
		{
			p_idx = p_alloc_idx;
		} else {
			delete[] p_alloc_idx; //_aligned_free(p_alloc);
		
			n_alloc_idx += n_old;
			n_alloc_idx /= 2;
			if( n_alloc_idx < 128 )
				n_alloc_idx = 128;

			p_idx = p_alloc_idx = new U4[n_alloc_idx];
		}
		
		GPM_DELARRAY(p_surf);
		n_surf_load = 0;

		return p_vx;
		
	}
	
	void VX324_surf( I8 target_id, I8 gpu_id, I8 *p_pic, I8 n_pic, GPC_GPUx64_CONST* p_const )
	{
		if( !p_surf )
		{
			if( p_vx == p_alloc )
			{
				p_surf = new GPC_GIO_SURF[n_surf_load = 1];
				p_surf->gpu_id = gpu_id;
				if( p_pic && n_pic > 0 )
				{
					p_surf->p_pic = (I8*)gp_memcpy( new I8[n_pic], p_pic, n_pic*sizeof(*p_pic) );
					p_surf->n_pic = n_pic;
				}
				p_surf->target_id = target_id;
				p_surf->n = 0;
				p_surf->p_cVS = p_const->cpy();
				return;
			}

			p_surf = new GPC_GIO_SURF[n_surf_load = 2];
			p_surf[1].s = p_surf[0].n = p_vx-p_alloc;
			p_surf[1].is = p_surf[0].in = p_idx-p_alloc_idx;
			p_surf[1].gpu_id = gpu_id;
			p_surf[1].target_id = target_id;
			p_surf[1].p_cVS = p_const->cpy();
			if( p_pic && n_pic > 0 )
			{
				p_surf[1].p_pic = (I8*)gp_memcpy( new I8[n_pic], p_pic, n_pic*sizeof(*p_pic) );
				p_surf[1].n_pic = n_pic;
			}
			return;
		}

		GPC_GIO_SURF *p_s = p_surf+(n_surf_load-1);
		if( !p_s->n )
		{
			GPM_DELARRAY( p_s->p_pic );
			p_s->p_pic = (I8*)gp_memcpy( new I8[n_pic], p_pic, n_pic*sizeof(*p_pic) );
			p_s->n_pic = n_pic;
			p_s->gpu_id = gpu_id;
			if( target_id > 0 )
			if( p_s->target_id != target_id )
				p_s->target_id = target_id;

			return;
		}
		
		if( target_id < -1 )
		{
			target_id = -2;
		}
		else if( target_id < 0 || target_id == p_s->target_id )
		if( gpu_id == p_s->gpu_id )
		if( !p_pic && !p_s->p_pic )
		{
			return;
		} 
		else if( p_pic && p_s->p_pic )
		if( n_pic <= p_s->n_pic )
		{
			I8 *p_in = p_pic, *p_ine = p_in+n_pic, *p_cmp = p_s->p_pic; 
			for( ; p_in < p_ine; p_in++,p_cmp++ )
			{
				if( *p_in != *p_cmp )
					break;
			}
			if( p_in == p_ine)
			{
				// új kell
				return;
			}
		}

		GPC_GIO_SURF* p_kill = p_surf;
		p_surf = (GPC_GIO_SURF*)gp_memcpy( new GPC_GIO_SURF[n_surf_load+1], p_kill, n_surf_load*sizeof(*p_surf) );
		memset( p_kill, 0, n_surf_load*sizeof(*p_kill) );
		GPM_DELARRAY( p_kill );
		n_surf_load++;
		p_s = p_surf+(n_surf_load-1);
		p_s->gpu_id = gpu_id;
		if( p_pic && n_pic > 0 )
		{
			p_s->p_pic = (I8*)gp_memcpy( new I8[n_pic], p_pic, n_pic*sizeof(*p_pic) );
			p_s->n_pic = n_pic;
		}
		
		p_s->target_id = ( target_id != -1 ) ? target_id : p_s[-1].target_id;
		p_s->s = p_vx-p_alloc;
		p_s->is = p_idx-p_alloc_idx;
		p_s->in = p_s->n = 0;
		p_s->p_cVS = p_const->cpy();
	}
	GPC_GIO_VX32C* VX324_mini( GPCV3& pos, GPCV3& vx, GPCV3& vy, GPCV4& smpa, char* p_str, char* p_end, bool b_secret = false )
	{
		if( vx.qlen() < 0.00001f || vy.qlen() < 0.00001f )
			return p_vx;
		
		GPCV4 rgba = smpa.smpa2rgba();

		GPCV3	move(0,0,0), vxy = vx+vy; 
		GPCV2	uv_lu, uv_rd;
	
		char	*p_row = p_str,
			*p_setup = "+-- --  -";

		

		p_str += GPD_NINCS(p_str, " ");
		ULL len = p_str - p_row;
		if( len )
		{
			move.x += len;
		}
		I8 t;
		U1 c;
		p_row = p_str;
		GPC_GIO_VX32C* p_vxe = NULL;

		if( p_str > p_end )
		{
			p_end = p_str + GPD_STRLEN(p_str);
		}

		while( p_str < p_end )
		{
			p_str += GPD_VAN(p_str, " \t\r\n\a");
			if( len = p_str - p_row )
			{
				p_vxe = VX324_expand( len*6 );
				for ( ; p_row < p_str; p_row++)
				{
					c = (b_secret) ? '*' : *p_row;
					c -= ' ';
					// 0 
					// 1 3
					//   4
					// 2 5
					
					p_vxe[0].pos = pos + vx*move.x + vy*move.y;				//GPCV3(-1.0f + move.x*pos_wh.x, 1.0f+move.y*pos_wh.y, 0.5f);
					p_vxe[0].uv = GPCV2(float(c % 8) / 8.0f, (float(c / 8) / 32.0f));

					p_vxe[4].pos = p_vxe[0].pos + vxy;
					p_vxe[4].uv = p_vxe[0].uv + GPCV2( 1.0f/8.0f, 1.0f/32.0f);

					p_vxe[1].pos = p_vxe[0].pos + vx;				// GPCV3(p_vxe[4].pos.x, p_vxe[0].pos.y, 0.5f);
					p_vxe[1].uv = GPCV2(p_vxe[4].uv.x, p_vxe[0].uv.y);
					p_vxe[2].pos = p_vxe[0].pos + vy;
					p_vxe[2].uv = GPCV2(p_vxe[0].uv.x, p_vxe[4].uv.y);
				
					p_vxe[3] = p_vxe[1];
					p_vxe[5] = p_vxe[2];
				
					for (GPC_GIO_VX32C* p_vxi = p_vxe, *p_e = p_vxi+6; p_vxi < p_e; p_vxi++)
					{
						p_vxi->rgba = rgba;
					}

					p_vxe = VX324_add(6);
					move.x += 1.0f;
				}
			}
			switch (*p_str)
			{
				case ' ':
					move.x += 1.0f;
					break;
				case '\t':
					move.x = ((4+(I8)move.x) & 0xfffffffffffffffc);
					break;
				case '\r':
				case '\n':
				case '\a':
					move.x = 0.0;// pos.x;
					move.y += 1.0f;
					if (*p_str != '\a')
						break;

					p_row = p_setup;
					len = GPD_STRLEN(p_setup);
					p_vxe = VX324_expand(len * 6);
					for (; p_row < p_setup+len; p_row++)
					{
						c = (b_secret) ? '*' : *p_row;
						c -= ' ';
						// 0 
						// 1 3
						//   4
						// 2 5
						p_vxe[0].pos = pos + vx*move.x + vy*move.y;				//GPCV3(-1.0f + move.x*pos_wh.x, 1.0f+move.y*pos_wh.y, 0.5f);
						p_vxe[0].uv = GPCV2(float(c % 8) / 8.0f, (float(c / 8) / 32.0f));

						p_vxe[4].pos = p_vxe[0].pos + vxy;
						p_vxe[4].uv = p_vxe[0].uv + GPCV2( 1.0f/8.0f, 1.0f/32.0f);

						p_vxe[1].pos = p_vxe[0].pos + vx;				// GPCV3(p_vxe[4].pos.x, p_vxe[0].pos.y, 0.5f);
						p_vxe[1].uv = GPCV2(p_vxe[4].uv.x, p_vxe[0].uv.y);
						p_vxe[2].pos = p_vxe[0].pos + vy;
						p_vxe[2].uv = GPCV2(p_vxe[0].uv.x, p_vxe[4].uv.y);
				
						p_vxe[3] = p_vxe[1];
						p_vxe[5] = p_vxe[2];

						for (GPC_GIO_VX32C* p_vxi = p_vxe, *p_e = p_vxi + 6; p_vxi < p_e; p_vxi++)
						{
							p_vxi->rgba = rgba;
						}

						p_vxe = VX324_add(6);
						move.x += 1.0f;
					}
					move.x = 0.0; // pos.x;
					move.y += 1.0f;
					break;
				default:
					// egész biztos ez a 0;
					p_str = p_end;
					break;
			}
			p_str++;
			p_row = p_str;

		}

		return p_vxe;
	}
	I8 VX324_line( GPCV3& p0, GPCV4& rgba0, GPCV3& p1, GPCV4& rgba1, float y  )
	{
		VX324_expand( 2 );
		p_vx->pos = p0;
		p_vx->rgba = rgba0;
		p_vx->uv.x = 0.0f;
		p_vx->uv.y = y;
		p_vx++;
		
		p_vx->pos = p1;
		p_vx->rgba = rgba1;
		p_vx->uv.x = 1.0f;
		p_vx->uv.y = y;
		p_vx++;
		return 2;
	}

	I8 VX324_line2( GPCV3& p0, GPCV4& rgba0, GPCV3& p1, GPCV4& rgba1, float y  )
	{
		VX324_expand( 4 );
		p_vx->pos = p0;
		p_vx->rgba = rgba0;
		p_vx->uv.x = 0.0f;
		p_vx->uv.y = y;
		p_vx++;
		
		p_vx->pos = (p0+p1)/2.0;
		p_vx->rgba = (rgba0+rgba1)/2.0;
		p_vx->uv.x = 0.5f;
		p_vx->uv.y = y;
		p_vx++;

		p_vx->pos = p_vx[-1].pos;
		p_vx->rgba = p_vx[-1].rgba;
		p_vx->uv.x = 0.5f;
		p_vx->uv.y = y;
		p_vx++;

		p_vx->pos = p1;
		p_vx->rgba = rgba1;
		p_vx->uv.x = 1.0f;
		p_vx->uv.y = y;
		p_vx++;
		return 2;
	}

	I8 VX324_line3( GPCV3& p0, GPCV4& rgba0, GPCV3& p1, GPCV4& rgba1, float y  )
	{
		VX324_expand( 6 );
		p_vx->pos = p0;
		p_vx->rgba = rgba0;
		p_vx->uv.x = 0.0f;
		p_vx->uv.y = y;
		p_vx++;
		
		p_vx->pos = (p1-p0)/3.0 + p0;
		p_vx->rgba = (rgba0+rgba1)/2.0;
		p_vx->uv.x = 0.5f;
		p_vx->uv.y = y;
		p_vx++;

		p_vx->pos = p_vx[-1].pos;
		p_vx->rgba = p_vx[-1].rgba;
		p_vx->uv.x = 0.5f;
		p_vx->uv.y = y;
		p_vx++;

		p_vx->pos = (p1-p0)*2.0/3.0 + p0;
		p_vx->rgba = (rgba0+rgba1)/2.0;
		p_vx->uv.x = 0.5f;
		p_vx->uv.y = y;
		p_vx++;

		p_vx->pos = p_vx[-1].pos;
		p_vx->rgba = p_vx[-1].rgba;
		p_vx->uv.x = 0.5f;
		p_vx->uv.y = y;
		p_vx++;

		p_vx->pos = p1;
		p_vx->rgba = rgba1;
		p_vx->uv.x = 1.0f;
		p_vx->uv.y = y;
		p_vx++;
		return 2;
	}
	I8 VX324_circle( GPCV3& vp, GPCV3& vx, GPCV3& vy, float y, U4 n, GPCV4& rgba )
	{
		GPC_GIO_VX32C	*p_vx = VX324_expand( n*2 ), *p_vxe = p_vx;
		p_vxe->pos = vx+vp;
		p_vxe->rgba = rgba;
		p_vxe->uv.x = 0.0f;
		p_vxe->uv.y = y;
		p_vxe++;

		for( float s = PI2/n, t = s, e = PI2; t < e; t += s )
		{
			p_vxe->pos = vx*cos(t) + vy*sin(t) + vp;
			p_vxe->rgba = rgba;
			p_vxe->uv.x = t/PI2;
			p_vxe->uv.y = y;
			p_vxe++;
			p_vxe[0] = p_vxe[-1];
			p_vxe++;
		}
		p_vxe[0] = *p_vx;
		p_vxe++;
		VX324_add( p_vxe-p_vx );
		return p_vxe-p_vx;
	}
	GPC_GIO_VX32C* VX324_buble_line( GPCV3& pos, GPCV3& vx, GPCV3& vy, float r, GPCV2& uvp, GPCV2& uvx, GPCV2& uvy, GPCV4& color, GPCV4* p_cut = NULL )
	{
		r = 0.0f;
		if (vx.qlen() < 0.00001f || vy.qlen() < 0.00001f)
			return p_vx;

		//float c = vx.normalize()*vy.normalize();
		//if( fabs(c) > 0.01f )
		//{
		//	GPF_CON_Write( "Buble vx*vy nem derékszög?" );
		//}
		GPC_GIO_VX32C* p_out, tmp, a_vx[8];
		float a, b;
		GPCV3 diff;
		I8 n_add = 8;
		if( r == 0.0f )
		{
			//  a_vx[8]
			//  0---1
			// 7     2
			// |     | 
			// 6     3
			//  5---4
			a_vx[5].pos = a_vx[0].pos = pos-vx;
			a_vx[4].pos = a_vx[1].pos = pos+vx;
			a_vx[0].pos += vy;
			a_vx[1].pos += vy;
			a_vx[5].pos -= vy;
			a_vx[4].pos -= vy;

			a_vx[5].uv = a_vx[0].uv = uvp-uvx;
			a_vx[4].uv = a_vx[1].uv = uvp+uvx;
			a_vx[0].uv += uvy;
			a_vx[1].uv += uvy;
			a_vx[5].uv -= uvy;
			a_vx[4].uv -= uvy;

			a_vx[7] = a_vx[0];
			a_vx[2] = a_vx[1];
			a_vx[3] = a_vx[4];
			a_vx[6] = a_vx[5];

			p_out = p_vx = VX324_expand(8);
			// sima négyzet
			if( !p_cut )
			{
				for( GPC_GIO_VX32C* p_v = p_vx, *p_e = p_v+8; p_v < p_e; p_v++ )
				{
					*p_v = a_vx[p_v-p_vx];
					p_v[0].uv.y = 1.0f-p_v[0].uv.y;
					p_v[0].rgba = color;
				}
			} else {
				n_add = 0;
				for( GPC_GIO_VX32C* p_v = a_vx, *p_e = p_v+8; p_v < p_e; p_v += 2 )
				{
					p_v[0].uv.y = 1.0f-p_v[0].uv.y;
					p_v[0].rgba = color;
					p_v[1].uv.y = 1.0f-p_v[1].uv.y;
					p_v[1].rgba = color;
					if( p_v[0].pos.x > p_v[1].pos.x )
					{
						tmp = p_v[0];
						p_v[0] = p_v[1];
						p_v[1] = tmp;
					}

					if( p_v[0].pos.x < p_cut->x )
					{
						if( p_v[1].pos.x <= p_cut->x )
						{
							p_v[0] = p_v[1];
							continue;
						}
					

						a = p_v[0].pos.x-p_v[1].pos.x;
						b = p_cut->x-p_v[1].pos.x;
						diff = p_v[0].pos-p_v[1].pos;

						p_v[0].pos = diff*(b/a)+p_v[1].pos;
					}
				
					if( p_v[1].pos.x > p_cut->z )
					{
						if( p_v[0].pos.x >= p_cut->z )
						{
							p_v[0] = p_v[1];
							continue;
						}

						a = p_v[1].pos.x-p_v[0].pos.x;
						b = p_cut->z-p_v[0].pos.x;
						diff = p_v[1].pos-p_v[0].pos;

						p_v[1].pos = diff*(b/a)+p_v[0].pos;
					}

					if( p_v[0].pos.y > p_v[1].pos.y )
					{
						tmp = p_v[0];
						p_v[0] = p_v[1];
						p_v[1] = tmp;
					}

					if( p_v[0].pos.y < p_cut->w )
					{
						if( p_v[1].pos.y <= p_cut->w )
						{
							p_v[0] = p_v[1];
							continue;
						}
					

						a = p_v[0].pos.y-p_v[1].pos.y;
						b = p_cut->w-p_v[1].pos.y;
						diff = p_v[0].pos-p_v[1].pos;

						p_v[0].pos = diff*(b/a)+p_v[1].pos;
					}

					if( p_v[1].pos.y > p_cut->y )
					{
						if( p_v[0].pos.y >= p_cut->y )
						{
							p_v[0] = p_v[1];
							continue;
						}

						a = p_v[1].pos.y-p_v[0].pos.y;
						b = p_cut->y-p_v[0].pos.y;
						diff = p_v[1].pos-p_v[0].pos;

						p_v[1].pos = diff*(b/a)+p_v[0].pos;
					}
					p_vx[n_add] = p_v[0];
					p_vx[n_add+1] = p_v[1];
					n_add += 2;
				}
			}
			if( p_surf )
			{
				p_surf[n_surf_load-1].n += n_add;
			}
			VX324_add( n_add );
			return p_out;
		}
		// a lekerekített majd itt számoljuk
		return p_vx;
	}
	
	GPC_GIO_VX32C* VX324_buble( GPCV3& pos, GPCV3& vx, GPCV3& vy, float r, GPCV2& uvp, GPCV2& uvx, GPCV2& uvy, GPCV4 smpa = GPCV4( 0.0f, 0.0f, 1.0f, 1.0f ) , GPCV4* p_cut = NULL )
	{
		r = 0.0f;
		GPCV4 limit( -1,1,1,-1 );
		if( !p_cut )
			p_cut = &limit;
		GPCV4 rgba = smpa.smpa2rgba();
		GPC_GIO_VX32C* p_out, a_vx[0x20], *pa_vx = a_vx, *pa_vxe = pa_vx+4, *pb_vx = pa_vx+0x10, *pb_vxe ;
		//  a_vx[8]
		// +0--1--2--3+
		// 0          3
		// |          |
		// 11         4
		// |          | 
		// |          |
		// 10         5
		// |          | 
		// 9          6
		// +9--8--7--6+
		a_vx[0].pos = pos-vx+vy;
		a_vx[0].uv = uvp-uvx+uvy;
		a_vx[0].rgba = rgba;

		a_vx[1].pos = pos+vx+vy;
		a_vx[1].uv = uvp+uvx+uvy;
		a_vx[1].rgba = rgba;

		a_vx[2].pos = pos+vx-vy;
		a_vx[2].uv = uvp+uvx-uvy;
		a_vx[2].rgba = rgba;

		a_vx[3].pos = pos-vx-vy;
		a_vx[3].uv = uvp-uvx-uvy;
		a_vx[3].rgba = rgba;
		
		I8 n_vx;

		// bal
		GPCV3 p0( p_cut->x, 0, 0 ), norm( 1.0f, 0, 0 );
		pb_vxe = p0.VX324_cut( pb_vx, pa_vx, pa_vxe, norm );
		n_vx = pb_vxe-pb_vx;
		if( n_vx < 3 )
			return p_vx;

		// fent
		p0.xyz( 0, p_cut->y, 0 );
		norm.xyz( 0, -1, 0 );

		pa_vxe = p0.VX324_cut( pa_vx, pb_vx, pb_vxe, norm );
		n_vx = pa_vxe-pa_vx;
		if( n_vx < 3 )
			return p_vx;

		// jobb
		p0.xyz( p_cut->z, 0, 0 );
		norm.xyz( -1, 0, 0 );

		pb_vxe = p0.VX324_cut( pb_vx, pa_vx, pa_vxe, norm );
		n_vx = pb_vxe-pb_vx;
		if( n_vx < 3 )
			return p_vx;

		// lent
		p0.xyz( 0, p_cut->w, 0 );
		norm.xyz( 0, 1, 0 );

		pa_vxe = p0.VX324_cut( pa_vx, pb_vx, pb_vxe, norm );
		n_vx = pa_vxe-pa_vx;
		if( n_vx < 3 )
			return p_vx;

		I8	na_vx = pa_vxe-pa_vx,
			na_t = na_vx-2;
		p_out = p_vx = VX324_expand(na_t*3);

		GPC_GIO_VX32C first = *pa_vx;
		first.uv /= 2.0f;
		first.uv += GPCV2( 0.5f,0.5f );
		first.uv.y = 1.0f-first.uv.y;
		for( U4 i = 0; i < na_t; i++ )
		{
			*p_vx = first;
			p_vx++;

			*p_vx = pa_vx[i+1];
			p_vx->uv /= 2.0f;
			p_vx->uv += GPCV2( 0.5f,0.5f );
			p_vx->uv.y = 1.0f-p_vx->uv.y;
			p_vx++;

			*p_vx = pa_vx[i+2];
			p_vx->uv /= 2.0f;
			p_vx->uv += GPCV2( 0.5f,0.5f );
			p_vx->uv.y = 1.0f-p_vx->uv.y;
			p_vx++;
		}

		// a lekerekített majd itt számoljuk
		if( p_surf )
		{
			//if( !p_surf[n_surf_load-1].n )
			//	p_surf[n_surf_load-1].n = p_surf[n_surf_load-1].s;
			p_surf[n_surf_load-1].n += p_vx-p_out;
		}
		return p_out;
	}

	GPC_GIO_BUFF_VX324& operator += ( GPC_GIO_BUFF_VX324& b );
	
	ULL VX324_updt_vx( ID3D11Buffer* &p_buff, ID3D11Device* p_dev, ID3D11DeviceContext* p_ctx, ULL n_vx )
	{
		ULL n_load = p_vx - p_alloc;

		if( n_vx < n_load )
		{	
			GPM_RELEASE( p_buff );
			if( !n_load )
				return 0;

			HRESULT hr;
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DYNAMIC;// D3D11_USAGE_STAGING;// D3D11_USAGE_IMMUTABLE; //D3D11_USAGE_DEFAULT;//    D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
			bd.ByteWidth = n_load * sizeof(*p_alloc);             // size is the VERTEX struct * 3
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

			hr = p_dev->CreateBuffer( &bd, NULL, &p_buff );       // create the buffer
			if( hr != S_OK )
			{
				GPM_RELEASE( p_buff );
				return 0;
			}
			n_vx = n_load;
		}
		
		if( !n_load )
			return 0;

		D3D11_MAPPED_SUBRESOURCE ms;
		p_ctx->Map( p_buff, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);	// map the buffer
		gp_memcpy(ms.pData, p_alloc, n_load*sizeof(*p_alloc));		// copy the data
		p_ctx->Unmap( p_buff, NULL);

		return n_vx;
	}
	ULL VX324_updt_ix( ID3D11Buffer* &p_buff, ID3D11Device* p_dev, ID3D11DeviceContext* p_ctx, ULL n_ix )
	{
		ULL n_load = p_idx - p_alloc_idx;
		if( n_ix < n_alloc_idx )
		{
			GPM_RELEASE( p_buff );
			if( !n_load )
				return 0;

			HRESULT hr;
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DYNAMIC;// D3D11_USAGE_STAGING;// D3D11_USAGE_IMMUTABLE; //D3D11_USAGE_DEFAULT;//    D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
			bd.ByteWidth = n_alloc_idx * sizeof(*p_alloc_idx);             // size is the VERTEX struct * 3
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;       // use as a vertex buffer
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

			hr = p_dev->CreateBuffer( &bd, NULL, &p_buff );       // create the buffer
			if( hr != S_OK )
			{
				GPM_RELEASE( p_buff );
				return 0;
			}
			n_ix = n_alloc_idx;
		}

		if( !n_load )
			return 0;

		D3D11_MAPPED_SUBRESOURCE ms;
		p_ctx->Map( p_buff, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);	// map the buffer
		gp_memcpy(ms.pData, p_alloc_idx, n_load*sizeof(*p_alloc_idx));		// copy the data
		p_ctx->Unmap( p_buff, NULL);
		return n_ix;
	}

	//U2*				VX324_ball_ix( U2 map_w, U2 map_h, U2 div );
	GPC_GIO_VX32C*	VX324_ball_vxNix( GPC_GIO_VX32C* p_out, U4 map_w, U4 map_h, GPC_COLOR4* p_map, U4 core, U4 div );
	U4 vx324_vx_n_load( void )
	{
		return p_vx-p_alloc;
	}
	U4 vx324_ix_n_load( void )
	{
		return p_idx-p_alloc_idx;
	}
	
};

class GPC_GIO_BUFF_VX32CUFBI
{
public:
	GPC_GIO_VX32CUFBI	*p_alloc, *p_vx, *p_vxo;
	ULL					n_alloc;
	GPC_GIO_SURF		*p_surf;
	ULL					n_alloc_surf;
	
	U4		*p_alloc_idx, *p_ix, *p_ixo;
	ULL		n_alloc_idx;
	
	//GPC_GIO_VX32CUFBI	*p_ufbi_alloc, *p_ufbi_vx;

	GPC_GIO_BUFF_VX32CUFBI()
	{
		GPM_CLR;
	}

	GPC_GIO_BUFF_VX32CUFBI( I8 n_vx_plus, I8 n_idx_plus )
	{
		GPM_CLR;
		VX324ufbi_expand( n_vx_plus );
		VX324ufbi_idx_expand( n_idx_plus );
	}

	GPC_GIO_VX32CUFBI* VX324ufbi_add( ULL n_add )
	{
		p_vxo += n_add;
		if( p_vx < p_vxo )
			p_vx = p_vxo;
		
		if( p_vx > p_alloc+n_alloc )
			p_vxo = p_vx = p_alloc+n_alloc;

		return p_vxo;
	}

	U4* VX324ufbi_idx_add( ULL n_add )
	{
		p_ixo += n_add;
		if( p_ix < p_ixo )
			p_ix = p_ixo;

		if( p_ix > p_alloc_idx+n_alloc_idx )
			p_ixo = p_ix = p_alloc_idx+n_alloc_idx;
		
		return p_ixo;
	}

	GPC_GIO_VX32CUFBI* VX324ufbi_expand( ULL n_add )
	{
		if (!this)
			return NULL;
		if( !n_add )
			return p_vxo;
		//if( !p_alloc )
		//{
		//	n_alloc = 0;
		//	GPM_DELARRAY( p_surf );
		//}

		if( !p_surf )				// megsemmisült a p_surface 
			n_alloc_surf = 0;			// akor nem lehet értéke

		if( n_alloc_surf < 1 )
		{
			n_alloc_surf = 0;
			GPM_DELARRAY( p_surf );		// niincs értéke nem lehet p_surface, ha valamiért van megsemmisíteni
		} 

		if( p_vx < p_alloc )
		{
			// valami gebasz talán mégg NULL vagy rossz helyen van a p_vx 
			p_vxo = p_vx = p_alloc;
			p_ixo = p_ix = p_alloc_idx;
		}
		else if( p_vx > p_alloc+n_alloc )
		{
			p_vx = p_alloc+n_alloc;
		}

		if( p_ix < p_alloc_idx )
		{
			p_ixo = p_ix = p_alloc_idx;
		}
		else if( p_ix > p_alloc_idx+n_alloc_idx )
		{
			p_ix = p_alloc_idx+n_alloc_idx;
		}

		if( p_surf )
		{
			GPC_GIO_SURF* p_s = p_surf+n_alloc_surf-1;
			
			//------------------------------
			//
			//        -- VERTEX --
			//
			// p_alloc	...p_vxo	...p_vx 
			// p_s	...s	+n
			//
			//-----------------------------------
			if( p_s->s < 0 )
				p_s->s = 0;
			else if( p_s->s > p_vx-p_alloc )
			{
				p_s->s = p_vx-p_alloc;
				p_s->n = 0;
			}
									
			p_vxo = p_alloc + p_s->s+p_s->n;
			if( p_vxo > p_vx )
			{
				p_vxo = p_vx;
				p_s->n = p_vxo-p_alloc;
				p_s->n -= p_s->s;
			}
						
			//------------------------------
			//
			//        -- INDEX --
			//
			// p_alloc_index	...p_ixo	...p_ix 
			// p_s		...is	+in
			//
			//-----------------------------------
			if( p_s->is < 0 )
				p_s->is = 0;
			else if( p_s->is > p_ix-p_alloc_idx )
			{
				p_s->is = p_ix-p_alloc_idx;
				p_s->in = 0;
			}

			p_ixo = p_alloc_idx + p_s->is+p_s->in;
			if( p_ixo > p_ix )
			{
				p_ixo = p_ix;
				p_s->in = p_ixo-p_alloc_idx;
				p_s->in -= p_s->is;
			}
						
		} else {
			p_vxo = p_vx;
			p_ixo = p_ix;
		}
		

		ULL n_new = p_vxo - p_alloc + n_add;
		if( n_new <= n_alloc )
			return p_vxo;
		
		if( n_new & 0xf )
		{
			n_new += 0x10;
			n_new &= 0xfffffffffffffff0;
		}
		
		GPC_GIO_VX32CUFBI* p_kill = p_alloc;
		n_alloc = n_new;
		p_alloc = new GPC_GIO_VX32CUFBI[n_alloc];
		if( !p_kill )
		{
			return p_vxo = p_vx = p_alloc;
		}
		ULL	n_vx = p_vx > p_kill ? p_vx - p_kill : 0,
			n_vxo = p_vxo > p_kill ? p_vxo - p_kill : 0;

		if( n_vx > n_alloc )
			n_vx = n_alloc;
		if( n_vx )
			gp_memcpy( p_alloc, p_kill, n_vx*sizeof(*p_kill) );
		delete[] p_kill; //_aligned_free(p_kill);

		p_vxo = p_alloc+n_vxo;
		p_vx = p_alloc+n_vx;

		return p_vxo;
	}

	U4* VX324ufbi_idx_expand( ULL n_add )
	{
		if (!this)
			return NULL;
		
		if( !n_add )
			return p_ixo;
		
		if( !p_alloc_idx )
		{
			p_ix = p_ixo = NULL;
		}
		
		ULL n_new = (p_ixo - p_alloc_idx) + n_add;
		if( n_new <= n_alloc_idx )
			return p_ixo;
		
		if( n_new & 0xf )
		{
			n_new += 0x10;
			n_new &= 0xfffffffffffffff0;
		}
		
		U4* p_kill = p_alloc_idx;
		n_alloc_idx = n_new;
		//p_alloc = (GPC_GIO_VX32C*)_aligned_malloc(n_alloc*sizeof(GPC_GIO_VX32C), 0x10);
		p_alloc_idx = new U4[n_alloc_idx];
		if( !p_kill )
			return p_ix = p_ixo = p_alloc_idx;
		ULL	n_ix = p_ix - p_kill,
			n_ixo = p_ixo - p_kill;
		gp_memcpy( p_alloc_idx, p_kill, n_ix*sizeof(*p_kill) );
		delete[] p_kill; //_aligned_free(p_kill);
		
		p_ix = p_alloc_idx+n_ix;
		p_ixo = p_alloc_idx+n_ixo;

		return p_ixo;
	}

	void VX324ufbi_clr( void )
	{
		if (!this)
			return;
		
		delete[] p_alloc;
		delete[] p_alloc_idx;
		delete[] p_surf;

		GPM_CLR;

	}

	GPC_GIO_VX32CUFBI* VX324ufbi_null( void )
	{
		if (!this)
			return NULL;
		
		p_ixo = p_ix = p_alloc_idx;
		GPM_DELARRAY(p_surf);
		n_alloc_surf = 0;

		return p_vxo = p_vx = p_alloc;
	}

	GPC_GIO_VX32CUFBI* VX324ufbi_reset( void )
	{
		if (!this)
			return NULL;
		// VERTEX
		ULL n_old = p_vx - p_alloc;
		if( n_old & 0xf )
		{
			n_old += 0x10;
			n_old &= 0xfffffffffffffff0;
		}
		if( n_old == n_alloc )
			p_vx = p_alloc;
		else {
			delete[] p_alloc; //_aligned_free(p_alloc);
		
			n_alloc = n_old;
			p_vx = p_alloc = new GPC_GIO_VX32CUFBI[n_alloc];
		}

		// INDEX
		n_old = p_ix - p_alloc_idx;
		if( n_old & 0xf )
		{
			n_old += 0x10;
			n_old &= 0xfffffffffffffff0;
		}

		if( n_old == n_alloc_idx )
			p_ix = p_alloc_idx;
		else {
			delete[] p_alloc_idx; //_aligned_free(p_alloc);
		
			n_alloc_idx = n_old;
			p_ix = p_alloc_idx = new U4[n_alloc_idx];
		}
		
		GPM_DELARRAY(p_surf);
		n_alloc_surf = 0;

		p_ixo = p_ix;
		return p_vxo = p_vx = p_alloc;
		
	}
	~GPC_GIO_BUFF_VX32CUFBI()
	{
		delete[] p_alloc;
		delete[] p_alloc_idx;
		delete[] p_surf;
	}
	GPC_GIO_SURF* VX324ufbi_surf_pre( I8 target_id, I8 gpu_id, I8 *p_pic, I8 n_pic )
	{
		if( !p_surf )
		{
			if( p_vx == p_alloc )
			{
				// ha most kezdödik csak egyet nyit
				n_alloc_surf = 1;
				p_surf = new GPC_GIO_SURF[n_alloc_surf];
				p_surf->gpu_id = gpu_id;
				if( p_pic && n_pic > 0 )
				{
					p_surf->p_pic = (I8*)gp_memcpy( new I8[n_pic], p_pic, n_pic*sizeof(*p_pic) );
					p_surf->n_pic = n_pic;
				}
				p_surf->target_id = target_id;
				p_surf->n = 0;
				return p_surf;
			}
			// ha már volt lezárja és nyit egy újat
			p_surf = new GPC_GIO_SURF[n_alloc_surf = 2];

			p_surf[1].s = p_surf[0].n = p_vx-p_alloc;
			p_surf[1].is = p_surf[0].in = p_ix-p_alloc_idx;
			p_surf[1].gpu_id = gpu_id;
			p_surf[1].target_id = target_id;
			if( p_pic && n_pic > 0 )
			{
				p_surf[1].p_pic = (I8*)gp_memcpy( new I8[n_pic], p_pic, n_pic*sizeof(*p_pic) );
				p_surf[1].n_pic = n_pic;
			}
			return p_surf+n_alloc_surf-1;
		}

		
		GPC_GIO_SURF *p_s = p_surf+n_alloc_surf-1;
		
		if( p_s->s > p_vxo-p_alloc )
			p_s->s = p_vxo-p_alloc;

		if( p_s->is > p_ixo-p_alloc_idx )
			p_s->is = p_ixo-p_alloc_idx;

		if( !p_s->n )
		{
			GPM_DELARRAY( p_s->p_pic );
			p_s->p_pic = (I8*)gp_memcpy( new I8[n_pic], p_pic, n_pic*sizeof(*p_pic) );
			p_s->n_pic = n_pic;
			p_s->gpu_id = gpu_id;

			p_s->in = 0;
			return p_s;
		}
		else if( p_alloc + p_s->s+p_s->n != p_vxo )
		{
			p_s->n = p_vxo-p_alloc-p_s->s;
		}

		if( p_alloc_idx + p_s->is + p_s->in != p_ixo )
		{
			p_s->in = p_ixo - p_alloc_idx-p_s->is;
		}

		if( target_id < 0 || target_id == p_s->target_id )
		if( gpu_id == p_s->gpu_id )
		if( !p_pic && !p_s->p_pic )
		{
			return p_s;
		} 
		else if( p_pic && p_s->p_pic )
		if( n_pic <= p_s->n_pic )
		{
			I8 *p_in = p_pic, *p_ine = p_in+n_pic, *p_cmp = p_s->p_pic; 
			for( ; p_in < p_ine; p_in++,p_cmp++ )
			{
				if( *p_in != *p_cmp )
					break;
			}

			if( p_in == p_ine)
			{
				// nem kell új
				return p_s;
			}
		}

		
		I8 n_surf_over = n_alloc_surf;
		n_alloc_surf++;
		GPC_GIO_SURF* p_kill = p_surf;
		p_surf = new GPC_GIO_SURF[n_alloc_surf];
		if( p_kill )
		{
			gp_memcpy( p_surf, p_kill, n_surf_over*sizeof(*p_surf) );
			memset( p_kill, 0, n_surf_over*sizeof(*p_surf) );
		}
		GPM_DELARRAY( p_kill );
		p_s = p_surf+n_surf_over;
		

		p_s = p_surf+n_surf_over;
		p_s->gpu_id = gpu_id;
		if( p_pic && n_pic > 0 )
		{
			p_s->p_pic = (I8*)gp_memcpy( new I8[n_pic], p_pic, n_pic*sizeof(*p_pic) );
			p_s->n_pic = n_pic;
		}
		
		p_s->target_id = ( target_id > 0 ) ? target_id : p_s[-1].target_id;
		p_s->s = p_vxo-p_alloc;
		p_s->is = p_ixo-p_alloc_idx;
		p_s->in = p_s->n = 0;

		return p_s;
	}

	GPC_GIO_SURF* VX324ufbi_surf_add( I8 target_id, I8 gpu_id, I8 *p_pic, I8 n_pic )
	{
		if( p_vx == p_alloc )
			return NULL;

		if( !p_surf )
		{
			//n_surf_over = 0;
			p_surf = new GPC_GIO_SURF[ n_alloc_surf = 1 ];
			
			p_surf->s = 0;
			p_surf->n = p_vxo-p_alloc;
			p_surf->gpu_id = gpu_id;
			p_surf->target_id = target_id;
			if( p_pic && n_pic > 0 )
			{
				p_surf->p_pic = (I8*)gp_memcpy( new I8[n_pic], p_pic, n_pic*sizeof(*p_pic) );
				p_surf->n_pic = n_pic;
			}
			p_surf->in = p_ixo-p_alloc_idx;
			p_surf->in -= p_surf->is;
			return p_surf;
		}

		//n_surf_over = n_alloc_surf-1;
		p_vxo = p_vx;
		p_ixo = p_ix;
		GPC_GIO_SURF *p_s = p_surf+n_alloc_surf-1;

		if( p_s->s > p_vxo-p_alloc )
			p_s->s = p_vxo-p_alloc;

		if( p_s->is > p_ixo-p_alloc_idx )
			p_s->is = p_ixo-p_alloc_idx;
		
		//if( p_alloc + p_s->s+p_s->n != p_vxo )
		//{
		//	// ha nagy a p_s->n
		//	p_s->n = p_vxo-p_alloc-p_s->s;
		//}
		//
		//if( p_alloc_idx + p_s->is+p_s->in != p_ixo )
		//{
		//	// ha nagy a p_s->in
		//	p_s->in = p_ixo - p_alloc_idx-p_s->is;
		//}

		if( target_id < 0 || target_id == p_s->target_id )
		if( gpu_id == p_s->gpu_id )
		if( !p_pic && !p_s->p_pic )
		{
			//p_surf->in = p_idx-p_alloc_idx;
			return p_s;
		} 
		else if( p_pic && p_s->p_pic )
		if( n_pic <= p_s->n_pic )
		{
			I8 *p_in = p_pic, *p_ine = p_in+n_pic, *p_cmp = p_s->p_pic; 
			for( ; p_in < p_ine; p_in++,p_cmp++ )
			{
				if( *p_in != *p_cmp )
					break;
			}

			if( p_in == p_ine)
			{
				// nem kell új
				return p_s;
			}
		}

		I8 n_surf_over = n_alloc_surf;
		n_alloc_surf++;
		GPC_GIO_SURF* p_kill = p_surf;
		p_surf = new GPC_GIO_SURF[n_alloc_surf];
		if( p_kill )
		{
			gp_memcpy( p_surf, p_kill, n_surf_over*sizeof(*p_surf) );
			memset( p_kill, 0, n_surf_over*sizeof(*p_surf) );
		}
		GPM_DELARRAY( p_kill );
		p_s = p_surf+n_surf_over;
		
		
		p_s->gpu_id = gpu_id;
		if( p_pic && n_pic > 0 )
		{
			p_s->p_pic = (I8*)gp_memcpy( new I8[n_pic], p_pic, n_pic*sizeof(*p_pic) );
			p_s->n_pic = n_pic;
		}
		

		p_s->target_id = ( target_id > 0 ) ? target_id : p_s[-1].target_id;
		p_s->s = 0; 
		p_s->n = p_vxo-p_alloc;

		p_s->is = p_s[-1].in+p_s[-1].is;
		p_s->in = (p_ixo-p_alloc_idx) - p_s->is;
		return p_s;
	}

	GPC_GIO_VX32CUFBI* VX324ufbi_buble_line( GPCV3& pos, GPCV3& vx, GPCV3& vy, float r, GPCV2& uvp, GPCV2& uvx, GPCV2& uvy )
	{
		r = 0.0f;

		GPC_GIO_VX32CUFBI* p_out;
		if( r == 0.0f )
		{
			//  0---1
			// 7     2
			// |     | 
			// 6     3
			//  5---4
			
			p_out = p_vx = VX324ufbi_expand(8);
			
			p_vx[5].pos = p_vx[0].pos = pos-vx;
			p_vx[4].pos = p_vx[1].pos = pos+vx;
			p_vx[0].pos += vy;
			p_vx[1].pos += vy;
			p_vx[5].pos -= vy;
			p_vx[4].pos -= vy;

			p_vx[5].uv = p_vx[0].uv = uvp-uvx;
			p_vx[4].uv = p_vx[1].uv = uvp+uvx;
			p_vx[0].uv += uvy;
			p_vx[1].uv += uvy;
			p_vx[5].uv -= uvy;
			p_vx[4].uv -= uvy;

			p_vx[7] = p_vx[0];
			p_vx[2] = p_vx[1];
			p_vx[3] = p_vx[4];
			p_vx[6] = p_vx[5];
			// sima négyzet
			for( GPC_GIO_VX32CUFBI* p_v = p_vx, *p_e = p_v+8; p_v < p_e; p_v++ )
			{
				p_v->uv.y = 1.0f-p_v->uv.y;
				p_v->rgba = GPCV4( 1.0f,1.0f,1.0f,1.0f );
			}
			if( p_surf )
			{
				p_surf[n_alloc_surf-1].n += 8;
			}
			VX324ufbi_add(8);
			return p_out;
		}
		// a lekerekített majd itt számoljuk
		return p_vx;
	}
	GPC_GIO_VX32CUFBI* VX324ufbi_buble( GPCV3& pos, GPCV3& vx, GPCV3& vy, float r, GPCV2& uvp, GPCV2& uvx, GPCV2& uvy )
	{
		r = 0.0f;

		GPC_GIO_VX32CUFBI* p_out;
		if( r == 0.0f )
		{
			p_out = p_vx = VX324ufbi_expand(6);
			
			p_vx[2].pos = p_vx[0].pos = pos-vx;
			p_vx[1].pos = p_vx[4].pos = pos+vx;
			p_vx[0].pos += vy;
			p_vx[1].pos += vy;
			p_vx[2].pos -= vy;
			p_vx[4].pos -= vy;

			p_vx[2].uv = p_vx[0].uv = uvp-uvx;
			p_vx[1].uv = p_vx[4].uv = uvp+uvx;
			p_vx[0].uv += uvy;
			p_vx[1].uv += uvy;
			p_vx[2].uv -= uvy;
			p_vx[4].uv -= uvy;

			p_vx[3] = p_vx[1];
			p_vx[5] = p_vx[2];
			// sima négyzet
			for( GPC_GIO_VX32CUFBI* p_v = p_vx, *p_e = p_v+6; p_v < p_e; p_v++ )
			{
				p_v->uv /= 2.0f;
				p_v->uv += GPCV2( 0.5f,0.5f );
				p_v->uv.y = 1.0f-p_v->uv.y;
				p_v->rgba = GPCV4( 1.0f,1.0f,1.0f,1.0f );
			}
			if( p_surf )
			{
				p_surf[n_alloc_surf-1].n += 6;
			}
			VX324ufbi_add(6);
			return p_out;
		}
		// a lekerekített majd itt számoljuk
		return p_vx;
	}
	
	GPC_GIO_BUFF_VX32CUFBI& operator += ( GPC_GIO_BUFF_VX32CUFBI& b );
	GPC_GIO_BUFF_VX32CUFBI& operator |= ( GPC_GIO_BUFF_VX32CUFBI& b );

};


class GPC_GIO_BUFF_VX32CU
{
public:
	GPC_GIO_VX32CU	*p_alloc, *p_vx, *p_vxo;
	ULL		n_alloc_vx;
	GPC_GIO_SURF	*p_surf;
	ULL		n_alloc_surf;
	
	U4		*p_alloc_idx, *p_ix, *p_ixo;
	ULL		n_alloc_idx;
	
	//GPC_GIO_VX32CU	*p_ufbi_alloc, *p_ufbi_vx;

	GPC_GIO_BUFF_VX32CU()
	{
		GPM_CLR;
	}

	GPC_GIO_BUFF_VX32CU( I8 n_vx_plus, I8 n_idx_plus )
	{
		GPM_CLR;
		VX32cu_expand( n_vx_plus );
		VX32cu_idx_expand( n_idx_plus );
	}

	GPC_GIO_VX32CU* VX32cu_add( I8 n_add )
	{
		if( n_add < 0 )
		{
			return p_vxo = p_vx = p_alloc+n_alloc_vx;
		}
		p_vxo += n_add;
		if( p_vx < p_vxo )
			p_vx = p_vxo;
		
		if( p_vx > p_alloc+n_alloc_vx )
			p_vxo = p_vx = p_alloc+n_alloc_vx;

		return p_vxo;
	}

	U4* VX32cu_idx_add( I8 n_add )
	{
		if( n_add < 0 )
		{
			return p_ixo = p_ix = p_alloc_idx+n_alloc_idx;
		}
		p_ixo += n_add;
		if( p_ix < p_ixo )
			p_ix = p_ixo;

		if( p_ix > p_alloc_idx+n_alloc_idx )
			p_ixo = p_ix = p_alloc_idx+n_alloc_idx;
		
		return p_ixo;
	}

	GPC_GIO_VX32CU* VX32cu_expand( ULL n_add, bool b_surf = true )
	{
		if (!this)
			return NULL;
		if( !n_add )
			return p_vxo;
		//if( !p_alloc )
		//{
		//	n_alloc = 0;
		//	GPM_DELARRAY( p_surf );
		//}

		if( !p_surf )				// megsemmisült a p_surface 
			n_alloc_surf = 0;			// akor nem lehet értéke

		if( n_alloc_surf < 1 )
		{
			n_alloc_surf = 0;
			GPM_DELARRAY( p_surf );		// niincs értéke nem lehet p_surface, ha valamiért van megsemmisíteni
		} 

		if( p_vx < p_alloc )
		{
			// valami gebasz talán mégg NULL vagy rossz helyen van a p_vx 
			p_vxo = p_vx = p_alloc;
			p_ixo = p_ix = p_alloc_idx;
		}
		else if( p_vx > p_alloc+n_alloc_vx )
		{
			p_vx = p_alloc+n_alloc_vx;
		}

		if( p_ix < p_alloc_idx )
		{
			p_ixo = p_ix = p_alloc_idx;
		}
		else if( p_ix > p_alloc_idx+n_alloc_idx )
		{
			p_ix = p_alloc_idx+n_alloc_idx;
		}

		if( !b_surf )
		{
			p_vxo = p_vx;
			p_ixo = p_ix;
		}
		else if( p_surf  )
		{
			GPC_GIO_SURF* p_s = p_surf+n_alloc_surf-1;
			
			//------------------------------
			//
			//        -- VERTEX --
			//
			// p_alloc	...p_vxo	...p_vx 
			// p_s	...s	+n
			//
			//-----------------------------------
			if( p_s->s < 0 )
				p_s->s = 0;
			else if( p_s->s > p_vx-p_alloc )
			{
				p_s->s = p_vx-p_alloc;
				p_s->n = 0;
			}
									
			p_vxo = p_alloc + p_s->s+p_s->n;
			if( p_vxo > p_vx )
			{
				p_vxo = p_vx;
				p_s->n = p_vxo-p_alloc;
				p_s->n -= p_s->s;
			}
						
			//------------------------------
			//
			//        -- INDEX --
			//
			// p_alloc_index	...p_ixo	...p_ix 
			// p_s		...is	+in
			//
			//-----------------------------------
			if( p_s->is < 0 )
				p_s->is = 0;
			else if( p_s->is > p_ix-p_alloc_idx )
			{
				p_s->is = p_ix-p_alloc_idx;
				p_s->in = 0;
			}

			p_ixo = p_alloc_idx + p_s->is+p_s->in;
			if( p_ixo > p_ix )
			{
				p_ixo = p_ix;
				p_s->in = p_ixo-p_alloc_idx;
				p_s->in -= p_s->is;
			}
						
		} else {
			p_vxo = p_vx;
			p_ixo = p_ix;
		}
		

		ULL n_new = p_vxo - p_alloc;
		n_new += n_add;
		if( n_new&0xf )
			n_new += 0x10-(n_new&0xf);

		if( n_new <= n_alloc_vx )
			return p_vxo;
		
		//if( n_new & 0xf )
		//{
		//	n_new &= 0xfffffffffffffff0;
		//	n_new += 0x10;
		//}
		
		GPC_GIO_VX32CU* p_kill = p_alloc;
		n_alloc_vx = n_new;
		p_alloc = new GPC_GIO_VX32CU[n_alloc_vx];
		if( !p_kill )
		{
			return p_vxo = p_vx = p_alloc;
		}
		ULL	n_vx = p_vx > p_kill ? p_vx - p_kill : 0,
			n_vxo = p_vxo > p_kill ? p_vxo - p_kill : 0;

		if( n_vx > n_alloc_vx )
			n_vx = n_alloc_vx;
		if( n_vx )
			gp_memcpy( p_alloc, p_kill, n_vx*sizeof(*p_kill) );
		delete[] p_kill; //_aligned_free(p_kill);

		p_vxo = p_alloc+n_vxo;
		p_vx = p_alloc+n_vx;

		return p_vxo;
	}

	U4* VX32cu_idx_expand( ULL n_add )
	{
		if (!this)
			return NULL;
		
		if( !n_add )
			return p_ixo;
		
		if( !p_alloc_idx )
		{
			p_ix = p_ixo = NULL;
		}
		
		ULL n_new = (p_ixo - p_alloc_idx) + n_add;
		if( n_new&0xf )
			n_new += 0x10-(n_new&0xf);

		if( n_new <= n_alloc_idx )
			return p_ixo;
		
		//if( n_new & 0xf )
		//{
		//	n_new &= 0xfffffffffffffff0;
		//	n_new += 0x10;
		//}
		
		U4* p_kill = p_alloc_idx;
		n_alloc_idx = n_new;
		//p_alloc = (GPC_GIO_VX32C*)_aligned_malloc(n_alloc*sizeof(GPC_GIO_VX32C), 0x10);
		p_alloc_idx = new U4[n_alloc_idx];
		if( !p_kill )
			return p_ix = p_ixo = p_alloc_idx;
		ULL	n_ix = p_ix - p_kill,
			n_ixo = p_ixo - p_kill;
		gp_memcpy( p_alloc_idx, p_kill, n_ix*sizeof(*p_kill) );
		delete[] p_kill; //_aligned_free(p_kill);
		
		p_ix = p_alloc_idx+n_ix;
		p_ixo = p_alloc_idx+n_ixo;

		return p_ixo;
	}

	void VX32cu_clr( void )
	{
		if (!this)
			return;
		
		delete[] p_alloc;
		delete[] p_alloc_idx;
		delete[] p_surf;

		GPM_CLR;

	}

	GPC_GIO_VX32CU* VX32cu_null( void )
	{
		if (!this)
			return NULL;
		
		p_ixo = p_ix = p_alloc_idx;
		GPM_DELARRAY(p_surf);
		n_alloc_surf = 0;

		return p_vxo = p_vx = p_alloc;
	}

	GPC_GIO_VX32CU* VX32cu_reset( void )
	{
		if (!this)
			return NULL;
		// VERTEX
		ULL n_old = p_vx - p_alloc;
		if( n_old & 0xf )
		{
			n_old += 0x10;
			n_old &= 0xfffffffffffffff0;
		}
		if( n_old == n_alloc_vx )
			p_vx = p_alloc;
		else {
			delete[] p_alloc; //_aligned_free(p_alloc);
		
			n_alloc_vx = n_old;
			p_vx = p_alloc = new GPC_GIO_VX32CU[n_alloc_vx];
		}

		// INDEX
		n_old = p_ix - p_alloc_idx;
		if( n_old & 0xf )
		{
			n_old += 0x10;
			n_old &= 0xfffffffffffffff0;
		}

		if( n_old == n_alloc_idx )
			p_ix = p_alloc_idx;
		else {
			delete[] p_alloc_idx; //_aligned_free(p_alloc);
		
			n_alloc_idx = n_old;
			p_ix = p_alloc_idx = new U4[n_alloc_idx];
		}
		
		GPM_DELARRAY(p_surf);
		n_alloc_surf = 0;

		p_ixo = p_ix;
		return p_vxo = p_vx = p_alloc;
		
	}
	~GPC_GIO_BUFF_VX32CU()
	{
		delete[] p_alloc;
		delete[] p_alloc_idx;
		delete[] p_surf;
	}
	GPC_GIO_SURF* VX32cu_surf_pre( I8 target_id, I8 gpu_id, I8 *p_pic, I8 n_pic )
	{
		if( !p_surf )
		{
			if( p_vx == p_alloc )
			{
				// ha most kezdödik csak egyet nyit
				n_alloc_surf = 1;
				p_surf = new GPC_GIO_SURF[n_alloc_surf];
				p_surf->gpu_id = gpu_id;
				if( p_pic && n_pic > 0 )
				{
					p_surf->p_pic = (I8*)gp_memcpy( new I8[n_pic], p_pic, n_pic*sizeof(*p_pic) );
					p_surf->n_pic = n_pic;
				}
				p_surf->target_id = target_id;
				p_surf->n = 0;
				return p_surf;
			}
			// ha már volt lezárja és nyit egy újat
			p_surf = new GPC_GIO_SURF[n_alloc_surf = 2];

			p_surf[1].s = p_surf[0].n = p_vx-p_alloc;
			p_surf[1].is = p_surf[0].in = p_ix-p_alloc_idx;
			p_surf[1].gpu_id = gpu_id;
			p_surf[1].target_id = target_id;
			if( p_pic && n_pic > 0 )
			{
				p_surf[1].p_pic = (I8*)gp_memcpy( new I8[n_pic], p_pic, n_pic*sizeof(*p_pic) );
				p_surf[1].n_pic = n_pic;
			}
			return p_surf+n_alloc_surf-1;
		}

		
		GPC_GIO_SURF *p_s = p_surf+n_alloc_surf-1;
		
		if( p_s->s > p_vxo-p_alloc )
			p_s->s = p_vxo-p_alloc;

		if( p_s->is > p_ixo-p_alloc_idx )
			p_s->is = p_ixo-p_alloc_idx;

		if( !p_s->n )
		{
			GPM_DELARRAY( p_s->p_pic );
			p_s->p_pic = (I8*)gp_memcpy( new I8[n_pic], p_pic, n_pic*sizeof(*p_pic) );
			p_s->n_pic = n_pic;
			p_s->gpu_id = gpu_id;

			p_s->in = 0;
			return p_s;
		}
		else if( p_alloc + p_s->s+p_s->n != p_vxo )
		{
			p_s->n = p_vxo-p_alloc-p_s->s;
		}

		if( p_alloc_idx + p_s->is + p_s->in != p_ixo )
		{
			p_s->in = p_ixo - p_alloc_idx-p_s->is;
		}

		if( target_id < 0 || target_id == p_s->target_id )
		if( gpu_id == p_s->gpu_id )
		if( !p_pic && !p_s->p_pic )
		{
			return p_s;
		} 
		else if( p_pic && p_s->p_pic )
		if( n_pic <= p_s->n_pic )
		{
			I8 *p_in = p_pic, *p_ine = p_in+n_pic, *p_cmp = p_s->p_pic; 
			for( ; p_in < p_ine; p_in++,p_cmp++ )
			{
				if( *p_in != *p_cmp )
					break;
			}

			if( p_in == p_ine)
			{
				// nem kell új
				return p_s;
			}
		}

		
		I8 n_surf_over = n_alloc_surf;
		n_alloc_surf++;
		GPC_GIO_SURF* p_kill = p_surf;
		p_surf = new GPC_GIO_SURF[n_alloc_surf];
		if( p_kill )
		{
			gp_memcpy( p_surf, p_kill, n_surf_over*sizeof(*p_surf) );
			memset( p_kill, 0, n_surf_over*sizeof(*p_surf) );
		}
		GPM_DELARRAY( p_kill );
		p_s = p_surf+n_surf_over;
		

		p_s = p_surf+n_surf_over;
		p_s->gpu_id = gpu_id;
		if( p_pic && n_pic > 0 )
		{
			p_s->p_pic = (I8*)gp_memcpy( new I8[n_pic], p_pic, n_pic*sizeof(*p_pic) );
			p_s->n_pic = n_pic;
		}
		
		p_s->target_id = ( target_id > 0 ) ? target_id : p_s[-1].target_id;
		p_s->s = p_vxo-p_alloc;
		p_s->is = p_ixo-p_alloc_idx;
		p_s->in = p_s->n = 0;

		return p_s;
	}

	GPC_GIO_SURF* VX32cu_surf_add( I8 target_id, I8 gpu_id, I8 *p_pic, I8 n_pic )
	{
		if( p_vx == p_alloc )
			return NULL;

		if( !p_surf )
		{
			//n_surf_over = 0;
			p_surf = new GPC_GIO_SURF[ n_alloc_surf = 1 ];
			
			p_surf->s = 0;
			p_surf->n = p_vxo-p_alloc;
			p_surf->gpu_id = gpu_id;
			p_surf->target_id = target_id;
			if( p_pic && n_pic > 0 )
			{
				p_surf->p_pic = (I8*)gp_memcpy( new I8[n_pic], p_pic, n_pic*sizeof(*p_pic) );
				p_surf->n_pic = n_pic;
			}
			p_surf->in = p_ixo-p_alloc_idx;
			p_surf->in -= p_surf->is;
			return p_surf;
		}

		//n_surf_over = n_alloc_surf-1;
		p_vxo = p_vx;
		p_ixo = p_ix;
		GPC_GIO_SURF *p_s = p_surf+n_alloc_surf-1;

		if( p_s->s > p_vxo-p_alloc )
			p_s->s = p_vxo-p_alloc;

		if( p_s->is > p_ixo-p_alloc_idx )
			p_s->is = p_ixo-p_alloc_idx;
		
		//if( p_alloc + p_s->s+p_s->n != p_vxo )
		//{
		//	// ha nagy a p_s->n
		//	p_s->n = p_vxo-p_alloc-p_s->s;
		//}
		//
		//if( p_alloc_idx + p_s->is+p_s->in != p_ixo )
		//{
		//	// ha nagy a p_s->in
		//	p_s->in = p_ixo - p_alloc_idx-p_s->is;
		//}

		if( target_id < 0 || target_id == p_s->target_id )
		if( gpu_id == p_s->gpu_id )
		if( !p_pic && !p_s->p_pic )
		{
			//p_surf->in = p_idx-p_alloc_idx;
			return p_s;
		} 
		else if( p_pic && p_s->p_pic )
		if( n_pic <= p_s->n_pic )
		{
			I8 *p_in = p_pic, *p_ine = p_in+n_pic, *p_cmp = p_s->p_pic; 
			for( ; p_in < p_ine; p_in++,p_cmp++ )
			{
				if( *p_in != *p_cmp )
					break;
			}

			if( p_in == p_ine)
			{
				// nem kell új
				return p_s;
			}
		}

		I8 n_surf_over = n_alloc_surf;
		n_alloc_surf++;
		GPC_GIO_SURF* p_kill = p_surf;
		p_surf = new GPC_GIO_SURF[n_alloc_surf];
		if( p_kill )
		{
			gp_memcpy( p_surf, p_kill, n_surf_over*sizeof(*p_surf) );
			memset( p_kill, 0, n_surf_over*sizeof(*p_surf) );
		}
		GPM_DELARRAY( p_kill );
		p_s = p_surf+n_surf_over;
		
		
		p_s->gpu_id = gpu_id;
		if( p_pic && n_pic > 0 )
		{
			p_s->p_pic = (I8*)gp_memcpy( new I8[n_pic], p_pic, n_pic*sizeof(*p_pic) );
			p_s->n_pic = n_pic;
		}
		

		p_s->target_id = ( target_id > 0 ) ? target_id : p_s[-1].target_id;
		p_s->s = 0; 
		p_s->n = p_vxo-p_alloc;

		p_s->is = p_s[-1].in+p_s[-1].is;
		p_s->in = (p_ixo-p_alloc_idx) - p_s->is;
		return p_s;
	}

	GPC_GIO_VX32CU* VX32cu_buble_line( GPCV3& pos, GPCV3& vx, GPCV3& vy, float r, GPCV2& uvp, GPCV2& uvx, GPCV2& uvy )
	{
		r = 0.0f;

		GPC_GIO_VX32CU* p_out;
		if( r == 0.0f )
		{
			//  0---1
			// 7     2
			// |     | 
			// 6     3
			//  5---4
			
			p_out = p_vx = VX32cu_expand(8);
			
			p_vx[5].pos = p_vx[0].pos = pos-vx;
			p_vx[4].pos = p_vx[1].pos = pos+vx;
			p_vx[0].pos += vy;
			p_vx[1].pos += vy;
			p_vx[5].pos -= vy;
			p_vx[4].pos -= vy;

			p_vx[5].uv = p_vx[0].uv = uvp-uvx;
			p_vx[4].uv = p_vx[1].uv = uvp+uvx;
			p_vx[0].uv += uvy;
			p_vx[1].uv += uvy;
			p_vx[5].uv -= uvy;
			p_vx[4].uv -= uvy;

			p_vx[7] = p_vx[0];
			p_vx[2] = p_vx[1];
			p_vx[3] = p_vx[4];
			p_vx[6] = p_vx[5];
			// sima négyzet
			for( GPC_GIO_VX32CU* p_v = p_vx, *p_e = p_v+8; p_v < p_e; p_v++ )
			{
				p_v->uv.y = 1.0f-p_v->uv.y;
				p_v->rgba = GPCV4( 1.0f,1.0f,1.0f,1.0f );
			}
			if( p_surf )
			{
				p_surf[n_alloc_surf-1].n += 8;
			}
			VX32cu_add(8);
			return p_out;
		}
		// a lekerekített majd itt számoljuk
		return p_vx;
	}
	GPC_GIO_VX32CU* VX32cu_buble( GPCV3& pos, GPCV3& vx, GPCV3& vy, float r, GPCV2& uvp, GPCV2& uvx, GPCV2& uvy )
	{
		r = 0.0f;

		GPC_GIO_VX32CU* p_out;
		if( r == 0.0f )
		{
			p_out = p_vx = VX32cu_expand(6);
			
			p_vx[2].pos = p_vx[0].pos = pos-vx;
			p_vx[1].pos = p_vx[4].pos = pos+vx;
			p_vx[0].pos += vy;
			p_vx[1].pos += vy;
			p_vx[2].pos -= vy;
			p_vx[4].pos -= vy;

			p_vx[2].uv = p_vx[0].uv = uvp-uvx;
			p_vx[1].uv = p_vx[4].uv = uvp+uvx;
			p_vx[0].uv += uvy;
			p_vx[1].uv += uvy;
			p_vx[2].uv -= uvy;
			p_vx[4].uv -= uvy;

			p_vx[3] = p_vx[1];
			p_vx[5] = p_vx[2];
			// sima négyzet
			for( GPC_GIO_VX32CU* p_v = p_vx, *p_e = p_v+6; p_v < p_e; p_v++ )
			{
				p_v->uv /= 2.0f;
				p_v->uv += GPCV2( 0.5f,0.5f );
				p_v->uv.y = 1.0f-p_v->uv.y;
				p_v->rgba = GPCV4( 1.0f,1.0f,1.0f,1.0f );
			}
			if( p_surf )
			{
				p_surf[n_alloc_surf-1].n += 6;
			}
			VX32cu_add(6);
			return p_out;
		}
		// a lekerekített majd itt számoljuk
		return p_vx;
	}
	
	GPC_GIO_BUFF_VX32CU& operator += ( GPC_GIO_BUFF_VX32CU& b );
	GPC_GIO_BUFF_VX32CU& operator += ( GPC_GIO_BUFF_VX32CUFBI& b );
	GPC_GIO_BUFF_VX32CU& operator |= ( GPC_GIO_BUFF_VX32CU& b );

};

enum GPE_VX_S: U1
{
	gpe_vx_GPCV2 = sizeof( GPCV2 ),
	gpe_vx_VX32C = sizeof( GPC_GIO_VX32C ),
	gpe_vx_VX32CU = sizeof( GPC_GIO_VX32CU ),
	gpe_vx_VOX = sizeof( GPCVOX ),
	gpe_vx_VX32CUFBI = sizeof(GPC_GIO_VX32CUFBI),
	gpe_vx_VX32CUFBI_inst = sizeof(GPC_GIO_VX32CUFBI)+sizeof( GPCV2 ),
};

class GPC_RES;
class GPC_TAB;
class GPC_CRS;
class GPC_WBx64;
class GPC_GIOx64_LAY;
class GPC_GIOx64;
class GPC_GISx64;
class GPC_GATEx64;
//class GPC3D_Bx7;
class GPC_GX_ITM;
class GPC3D_EXEC;
//class GPC3D_BASE;
class GPC_GALAXY;
class GPC_BBUFF;
class GPC_3D_X11;

class GPC3D;
class GPC_GIVLx64;
class GPC_GIVx64;





//#define GPD_NET_i8	MAKE_ID( '_','_','i','8' )
//#define GPD_NET_u4	MAKE_ID( '_','_','u','4' )
//#define GPD_NET_u8	MAKE_ID( '_','_','u','8' )
//#define GPD_NET_f4	MAKE_ID( '_','_','f','4' )
//#define GPD_NET_vari	MAKE_ID( 'v','a','r','i' )
//#define GPD_NET_adr	MAKE_ID( '_','a','d','r' )
//#define GPD_NET_str	MAKE_ID( '_','s','t','r' )
//#define GPD_NET_fix	MAKE_ID( '_','f','i','x' )
#define GPD_VT100_BLUE MAKE_ID( 0x1b,'B',0,0 )

#define GPD_VT100_BLINK MAKE_ID( 0x1b,'[','5','m' )
#define GPD_VT100_HOME MAKE_ID( 0x1b,'[','H',0 )
#define GPD_VT100_JUPUP MAKE_ID( 0x1b,'[','n','A' )
#define GPD_VT100_GRAPH MAKE_ID( 0x1b,'(','0',0 )
typedef enum GPE_NET:U4
{
	GPE_NET_f4		= MAKE_ID( '_','_','f','4' ),
	GPE_NET_i8		= MAKE_ID( '_','_','i','8' ),
	GPE_NET_u4		= MAKE_ID( '_','_','u','4' ),
	GPE_NET_u8		= MAKE_ID( '_','_','u','8' ),
	GPE_NET_adr	= MAKE_ID( '_','a','d','r' ),
	GPE_NET_box	= MAKE_ID( '_','b','o','x' ),
	GPE_NET_res	= MAKE_ID( '_','r','e','s' ),
	GPE_NET_RES	= MAKE_ID( '_','R','E','S' ),
	GPE_NET_str	= MAKE_ID( '_','s','t','r' ),
	GPE_NET_sht	= MAKE_ID( '_','s','h','t' ),
	GPE_NET_PNG	= MAKE_ID( 0x89,'P','N','G' ),
	GPE_NET_fire	= MAKE_ID( 'f','i','r','e' ),
	GPE_NET_step	= MAKE_ID( 's','t','e','p' ),
	GPE_NET_vari	= MAKE_ID( 'v','a','r','i' ),

	GPE_NET_XP0	= MAKE_ID( 'X','P','0',';' ),
	GPE_NET_XP1	= MAKE_ID( 'X','P','1',';' ),
	
};

typedef enum LWO_ID:U4
{
	LWO_ID_FORM	= MAKE_ID('F','O','R','M'),
	LWO_ID_LWO2	= MAKE_ID('L','W','O','2'),

	/**  PRIMARY CHUNK ID  **/
	LWO_ID_LAYR	= MAKE_ID('L','A','Y','R'),
	LWO_ID_PNTS	= MAKE_ID('P','N','T','S'),
	LWO_ID_VMAP	= MAKE_ID('V','M','A','P'),
			  
	LWO_ID_VMAD	= MAKE_ID('V','M','A','D'),
			  
	LWO_ID_POLS	= MAKE_ID('P','O','L','S'),
	LWO_ID_TAGS	= MAKE_ID('T','A','G','S'),
	LWO_ID_PTAG	= MAKE_ID('P','T','A','G'),
	LWO_ID_ENVL	= MAKE_ID('E','N','V','L'),
	LWO_ID_CLIP	= MAKE_ID('C','L','I','P'),
	LWO_ID_SURF	= MAKE_ID('S','U','R','F'),
	LWO_ID_BBOX	= MAKE_ID('B','B','O','X'),
	LWO_ID_DESC	= MAKE_ID('D','E','S','C'),
	LWO_ID_TEXT	= MAKE_ID('T','E','X','T'),
	LWO_ID_ICON	= MAKE_ID('I','C','O','N'),
		
	/**  POLS TYPE  **/		
	LWO_ID_FACE	= MAKE_ID('F','A','C','E'),
	LWO_ID_CRVS	= MAKE_ID('C','U','R','V'),
	LWO_ID_PCHS	= MAKE_ID('P','T','C','H'),
	LWO_ID_MBAL	= MAKE_ID('M','B','A','L'),
	LWO_ID_BONE	= MAKE_ID('B','O','N','E'),
		
	/**  PTAG TYPE  **/		
//	LWO_ID_SURF	= MAKE_ID('S','U','R','F'),
	LWO_ID_BNID	= MAKE_ID('B','N','I','D'),
	LWO_ID_SGMP	= MAKE_ID('S','G','M','P'),
	LWO_ID_PART	= MAKE_ID('P','A','R','T'),
		
	/**  IMAGE SUB-CHUNK ID  */	
	LWO_ID_STIL	= MAKE_ID('S','T','I','L'),
	LWO_ID_ISEQ	= MAKE_ID('I','S','E','Q'),
	LWO_ID_ANIM	= MAKE_ID('A','N','I','M'),
	LWO_ID_XREF	= MAKE_ID('X','R','E','F'),
	LWO_ID_STCC	= MAKE_ID('S','T','C','C'),
	LWO_ID_CONT	= MAKE_ID('C','O','N','T'),
	LWO_ID_BRIT	= MAKE_ID('B','R','I','T'),
	LWO_ID_SATR	= MAKE_ID('S','A','T','R'),
	LWO_ID_HUE		= MAKE_ID('H','U','E',' '),
	LWO_ID_GAMM	= MAKE_ID('G','A','M','M'),
	LWO_ID_NEGA	= MAKE_ID('N','E','G','A'),
	LWO_ID_CROP	= MAKE_ID('C','R','O','P'),
	LWO_ID_ALPH	= MAKE_ID('A','L','P','H'),
	LWO_ID_COMP	= MAKE_ID('C','O','M','P'),
	LWO_ID_IFLT	= MAKE_ID('I','F','L','T'),
	LWO_ID_PFLT	= MAKE_ID('P','F','L','T'),
		
	/**  ENVELOPE SUB-CHUNK  **/	
	LWO_ID_PRE		= MAKE_ID('P','R','E',' '),
	LWO_ID_POST	= MAKE_ID('P','O','S','T'),
	LWO_ID_KEY		= MAKE_ID('K','E','Y',' '),
	LWO_ID_SPAN	= MAKE_ID('S','P','A','N'),
	LWO_ID_CHAN	= MAKE_ID('C','H','A','N'),
		
	/**  SURFACE SUB-CHUNK ID  */	
	LWO_ID_COLR	= MAKE_ID('C','O','L','R'),
	LWO_ID_DIFF	= MAKE_ID('D','I','F','F'),
	LWO_ID_LUMI	= MAKE_ID('L','U','M','I'),
	LWO_ID_SPEC	= MAKE_ID('S','P','E','C'),
	LWO_ID_REFL	= MAKE_ID('R','E','F','L'),
	LWO_ID_TRAN	= MAKE_ID('T','R','A','N'),
	LWO_ID_TRNL	= MAKE_ID('T','R','N','L'),
	LWO_ID_GLOS	= MAKE_ID('G','L','O','S'),
	LWO_ID_SHRP	= MAKE_ID('S','H','R','P'),
	LWO_ID_BUMP	= MAKE_ID('B','U','M','P'),
	LWO_ID_SIDE	= MAKE_ID('S','I','D','E'),
	LWO_ID_SMAN	= MAKE_ID('S','M','A','N'),
	LWO_ID_RFOP	= MAKE_ID('R','F','O','P'),
	LWO_ID_RIMG	= MAKE_ID('R','I','M','G'),
	LWO_ID_RSAN	= MAKE_ID('R','S','A','N'),
	LWO_ID_RIND	= MAKE_ID('R','I','N','D'),
	LWO_ID_CLRH	= MAKE_ID('C','L','R','H'),
	LWO_ID_TROP	= MAKE_ID('T','R','O','P'),
	LWO_ID_TIMG	= MAKE_ID('T','I','M','G'),
	LWO_ID_CLRF	= MAKE_ID('C','L','R','F'),
	LWO_ID_ADTR	= MAKE_ID('A','D','T','R'),
	LWO_ID_GLOW	= MAKE_ID('G','L','O','W'),
	LWO_ID_LINE	= MAKE_ID('L','I','N','E'),
	//LWO_ID_ALPH	= MAKE_ID('A','L','P','H'),
	LWO_ID_AVAL	= MAKE_ID('A','V','A','L'),
	LWO_ID_GVAL	= MAKE_ID('G','V','A','L'),
	LWO_ID_BLOK	= MAKE_ID('B','L','O','K'),
	LWO_ID_LCOL	= MAKE_ID('L','C','O','L'),
	LWO_ID_LSIZ	= MAKE_ID('L','S','I','Z'),
	LWO_ID_CMNT	= MAKE_ID('C','M','N','T'),
		
	/**  TEXTURE LAYER  **/	
	//LWO_ID_CHAN	= MAKE_ID('C','H','A','N'),
	LWO_ID_TYPE	= MAKE_ID('T','Y','P','E'),
	LWO_ID_NAME	= MAKE_ID('N','A','M','E'),
	LWO_ID_ENAB	= MAKE_ID('E','N','A','B'),
	LWO_ID_OPAC	= MAKE_ID('O','P','A','C'),
	LWO_ID_FLAG	= MAKE_ID('F','L','A','G'),
	LWO_ID_PROJ	= MAKE_ID('P','R','O','J'),
	LWO_ID_STCK	= MAKE_ID('S','T','C','K'),
	LWO_ID_TAMP	= MAKE_ID('T','A','M','P'),
		
	/**  TEXTURE MAPPING  **/	
	LWO_ID_TMAP	= MAKE_ID('T','M','A','P'),
	LWO_ID_AXIS	= MAKE_ID('A','X','I','S'),
	LWO_ID_CNTR	= MAKE_ID('C','N','T','R'),
	LWO_ID_SIZE	= MAKE_ID('S','I','Z','E'),
	LWO_ID_ROTA	= MAKE_ID('R','O','T','A'),
	LWO_ID_OREF	= MAKE_ID('O','R','E','F'),
	LWO_ID_FALL	= MAKE_ID('F','A','L','L'),
	LWO_ID_CSYS	= MAKE_ID('C','S','Y','S'),
		
	/**  IMAGE MAP  **/		
	LWO_ID_IMAP	= MAKE_ID('I','M','A','P'),
	LWO_ID_IMAG	= MAKE_ID('I','M','A','G'),
	LWO_ID_WRAP	= MAKE_ID('W','R','A','P'),
	LWO_ID_WRPW	= MAKE_ID('W','R','P','W'),
	LWO_ID_WRPH	= MAKE_ID('W','R','P','H'),
//	LWO_ID_VMAP	= MAKE_ID('V','M','A','P'),
	LWO_ID_AAST	= MAKE_ID('A','A','S','T'),
	LWO_ID_PIXB	= MAKE_ID('P','I','X','B'),
		
	/**  PROCUDUAL TEXTURE  **/	
	LWO_ID_PROC	= MAKE_ID('P','R','O','C'),
//	LWO_ID_COLR	= MAKE_ID('C','O','L','R'),
	LWO_ID_VALU	= MAKE_ID('V','A','L','U'),
	LWO_ID_FUNC	= MAKE_ID('F','U','N','C'),
	LWO_ID_FTPS	= MAKE_ID('F','T','P','S'),
	LWO_ID_ITPS	= MAKE_ID('I','T','P','S'),
	LWO_ID_ETPS	= MAKE_ID('E','T','P','S'),
		
	/**  GRADIENT **/		
	LWO_ID_GRAD	= MAKE_ID('G','R','A','D'),
	LWO_ID_GRST	= MAKE_ID('G','R','S','T'),
	LWO_ID_GREN	= MAKE_ID('G','R','E','N'),
		
	/**  SHADER PLUGIN  */	
	LWO_ID_SHDR	= MAKE_ID('S','H','D','R'),
	LWO_ID_DATA	= MAKE_ID('D','A','T','A'),
		
		
	/**  VMAP TYPE  **/
	LWO_ID_MNVW	= MAKE_ID('M','N','V','W'),
	LWO_ID_MORF	= MAKE_ID('M','O','R','F'),
	LWO_ID_TXUV	= MAKE_ID('T','X','U','V'),
	LWO_ID_WGHT	= MAKE_ID('W','G','H','T'),
	/**  saját id-k **/
	LWO_ID_LIGHT	= MAKE_ID('l','i','g','h'),
	LWO_ID_COLOR	= MAKE_ID('c','o','l','o'),
};



class GPC_UNDO
{
public:
	GPC_AN	an;
	char*	p_str;
	I8	n_str, pos;
	~GPC_UNDO()
	{
		GPM_DELARRAY( p_str );
	}
};
class GPC_RES : public GPC_FIX
{
public:
	GPE_ALFA	res_id;	// 0
	GPE_NET	res_type;	// 8
	U4	res,	// 12
		res_a,	// 16
		res_n;	// 20
	U8	res_an;	// 24
			// - 32 -
	GPC_RES(void);
	GPC_RES( bool b_res );
	I8 res_clear( void );
	~GPC_RES();
	GPE_ALFA	res_alfa(void);

	GPC_RES&	operator = ( I8 b );
	GPC_RES&	operator = ( float b );
	GPC_RES&	operator = ( GPC_RES& b );
	GPC_RES&	operator += ( I8 b );
	GPC_RES&	operator += ( float b );
	GPC_RES&	operator += ( GPC_RES& b );
	GPC_RES&	operator -= ( I8 b );
	GPC_RES&	operator <	 ( I8 b );
	GPC_RES&	operator <= ( I8 b );
	GPC_RES&	operator >= ( I8 b );
	GPC_RES&	operator >  ( I8 b );
	GPC_RES&	operator == ( I8 b );
	GPC_RES&	operator != ( I8 b );
	GPC_RES&	operator -= ( float b );
	GPC_RES&	operator <	 ( float b );
	GPC_RES&	operator <= ( float b );
	GPC_RES&	operator >= ( float b );
	GPC_RES&	operator >  ( float b );
	GPC_RES&	operator == ( float b );
	GPC_RES&	operator != ( float b );
	GPC_RES&	operator -= ( GPC_RES& b );
	GPC_RES&	operator <	 ( GPC_RES& b );
	GPC_RES&	operator <= ( GPC_RES& b );
	GPC_RES&	operator >= ( GPC_RES& b );
	GPC_RES&	operator >  ( GPC_RES& b );
	GPC_RES&	operator == ( GPC_RES& b );
	GPC_RES&	operator != ( GPC_RES& b );
	GPC_RES&	operator *= ( I8 b );
	GPC_RES&	operator *= ( float b );
	GPC_RES&	operator *= ( GPC_RES& b );
	GPC_RES&	operator /= ( I8 b );
	GPC_RES&	operator /= ( float b );
	GPC_RES&	operator /= ( GPC_RES& b );

	GPC_RES&	operator %= ( I8 b );
	GPC_RES&	operator %= ( float b );
	GPC_RES&	operator %= ( GPC_RES& b );

	GPC_RES&	operator ^= ( I8 b );
	GPC_RES&	operator ^= ( float b );
	GPC_RES&	operator ^= ( GPC_RES& b );

	GPC_RES*	res_cpy(void)
	{
		if( !this )
			return NULL;

		if( !fix_p_alloc )
			return NULL;

		if( !fix_n_alloc )
			return NULL;

		ULL s = 0;
		GPC_RES* p_new = new GPC_RES;

		(*p_new) = *this;

		return p_new;
	}
	GPCI4&	res_axn(void);
	GPCV2	res_v2( I8 n = 0 );
	GPCV3	res_v3( I8 n = 0 );
	//GPC_RES&	res2f4(void);
	GPC_RES& GPC_RES::res2f4(void)
	{
		switch( res_type )
		{
		case GPE_NET_f4:
			return *this;
		case GPE_NET_res:
			return res_res2f4();
		case GPE_NET_i8:
			res_type = GPE_NET_f4;
			if( res_an == 1 )
			{
				float* new_p_alloc = new float[1];
				*new_p_alloc = float(*(I8*)fix_p_alloc);
				delete[] fix_p_alloc;
				fix_p_alloc = (char*)new_p_alloc;
				fix_n_alloc = sizeof(float);
			} else {
				ULL new_n_alloc = res_an*sizeof(float);
				char* new_p_alloc = (char*)gp_zero( new char[new_n_alloc], new_n_alloc);

				I8	*p_i8 = (I8*)fix_p_alloc,
					*p_i8e = (I8*)fix_p_alloc;

				float	*p_f4 = (float*)(new_p_alloc),
					*p_f4e = (float*)(new_p_alloc+new_n_alloc);

				while( p_f4 < p_f4e )
				{
					*p_f4 = *p_i8;
					p_i8++;
					p_f4++;
				}

				delete[] fix_p_alloc;
				fix_p_alloc = new_p_alloc;
				fix_n_alloc = new_n_alloc;
			}
			return *this;
		case GPE_NET_str:
			return *this;
		}
		return *res_equ( 0.0f );
	}

	GPC_RES&	res_res2f4(void);

	GPC_RES&	res2i8( bool b_int = false )
	{
		switch( res_type )
		{
		case GPE_NET_i8:
			return *this;
		case GPE_NET_res:
			return res_res2i8();
		case GPE_NET_f4:
		{
			ULL new_n_alloc = res_an*sizeof(I8);
			char* new_p_alloc = (char*)gp_zero( new char[new_n_alloc+1], new_n_alloc);

			float	*p_f4 = (float*)fix_p_alloc,
				*p_f4e = (float*)(fix_p_alloc+fix_n_alloc);

			I8	*p_i8 = (I8*)new_p_alloc,
				*p_i8e = (I8*)(new_p_alloc+new_n_alloc);

			while( p_i8 < p_i8e )
			{
				*p_i8 = *p_f4;
				p_i8++;
				p_f4++;
			}
			res_type = GPE_NET_i8;
			delete[] fix_p_alloc;
			fix_p_alloc = new_p_alloc;
			fix_n_alloc = new_n_alloc;
		}
		return *this;
		case GPE_NET_str:
			if( !b_int )
				return *this;
			return res_str2i8();


		}

		return *res_equ( (I8)0 );
	}
	GPC_RES&	res_str2i8(void);
	GPC_RES&	res_res2i8(void);
	
	I8	i8(void);
	I8	i8_an( U4 a, U4 n);
	float	f4(void);
	float	f4_an( U4 a, U4 n);
	GPC_RES*	res_expand( ULL&s, ULL e, ULL size )
	{	
		if( !this )
		{
			GPC_RES* p_this = new GPC_RES;
			if( !p_this )
				return NULL;
			p_this->res_a = p_this->res_n = p_this->res_an = 1;
			return p_this->res_expand( s, e, size );
		}
		((GPC_FIX*)&fix_p_alloc)->fix_expand( s, e, size );
		return this;
	}
	GPC_RES* res_insert( char* p_str, ULL n_load, ULL& n_start, ULL n_end )
	{	
		if( !this )
		{
			GPC_RES* p_this = new GPC_RES;
			if( !p_this )
				return NULL;
			p_this->res_a = p_this->res_n = p_this->res_an = 1;
			return p_this->res_insert( p_str, n_load, n_start, n_end );
		}
		((GPC_FIX*)&fix_p_alloc)->fix_expand( n_start, n_end, n_load );

		if (n_load)
			gp_memcpy(fix_p_alloc + n_start, p_str, n_load);

		return this;
	}
	GPC_RES* res_format(char* p_format, ...)
	{
		va_list vl;
		va_start(vl, p_format);
		char s_buffer[10240];
		ULL n = vsprintf(s_buffer, p_format, vl);
		if( n < 1 )
			return this;
	
		ULL s = -1;

		return res_insert( s_buffer, n, s, 0 );
	}
	GPC_RES*	fun_cam( GPC_WBx64* p_wb );
	GPC_RES*	fun_lwo( GPC_WBx64* p_wb );
	GPC_RES*	fun_lws( GPC_WBx64* p_wb );
	//GPC_RES*	fun_giv_vox( GPC_WBx64* p_wb );
	//GPC_RES*	fun_giv( GPC_WBx64* p_wb );
	GPC_RES*	fun_max(void);
	GPC_RES*	fun_min(void);
	GPC_RES*	fun_x( GPE_ALFA fun );
	GPC_RES*	fun_xy(void);
	GPC_RES*	fun_zw(void);
	GPC_RES*	fun_abs(void);
	GPC_RES*	fun_int(void);
	
	// file system
	GPC_RES*	fun_ace( GPC_CRS& crs );
	GPC_RES*	fun_dir( GPC_CRS& crs, GPC_AN& tab_an );

	GPC_RES*	fun_rename(GPC_CRS& crs);
	GPC_RES*	fun_log( GPC_CRS& crs, GPC_TAB* p_tab );
	GPC_RES*	fun_cos(void);
	GPC_RES*	fun_sin(void);
	GPC_RES*	fun_acos(void);
	GPC_RES*	fun_asin(void);
	GPC_RES*	fun_sum(void);
	GPC_RES*	fun_sqrt(void);
	GPC_RES*	fun_vec(void);
	GPC_RES*	an(void);
	GPC_RES*	fun_gio( GPC_WBx64* p_wb );
	GPC_RES*	fun_gis( GPC_WBx64* p_wb );
	GPC_RES*	fun_gpu( GPC_WBx64* p_wb );
	GPC_RES*	fun_save( GPC_CRS& crs, GPC_AN tab_an );
	GPC_RES*	fun_win( GPC_WBx64* p_wb );
	GPC_RES*	fun_pic( GPC_WBx64* p_wb );
	GPC_RES*	fun_pix( GPC_WBx64* p_wb );
	GPC_RES*	fun_picqc( GPC_CRS& crs );
	GPC_RES*	fun_repic( GPC_CRS& crs );
	GPC_RES*	fun_npix( GPC_WBx64* p_wb );
	GPC_RES*	fun_print( void );
	GPC_RES*	fun_system( GPC_CRS& crs );
	GPC_RES*	fun_strrow( void );
	GPC_RES*	fun_strstr( void );
	GPC_RES*	fun_strlen( void );
	GPC_RES*	fun_token( GPC_CRS& crs );
	GPC_RES*	fun_histi( GPC_CRS& crs );
	GPC_RES*	fun_edge( GPC_CRS& crs );
	GPC_RES*	fun_storee( GPC_CRS& crs );
	GPC_RES*	fun_stored(GPC_CRS& crs);
	GPC_RES*	fun_count( GPC_CRS& crs );
	GPC_RES*	fun_gripper( GPC_CRS& crs, GPC_TAB* p_tab );
	GPC_RES*	fun_gripper2( GPC_CRS& crs, GPC_TAB* p_tab );
	//GPC_RES*	fun_gripper3( GPC_CRS& crs, GPC_TAB* p_tab );
	//GPC_RES*	fun_orbit( GPC_CRS& crs, GPC_TAB* p_tab );
	GPC_RES*	fun_track( GPC_CRS& crs, GPC_TAB* p_tab );

	GPC_RES*	fun_lathe( GPC_CRS& crs, GPC_TAB* p_tab );

	GPC_RES*	fun_reggio( GPC_CRS& crs );

	GPC_RES*	fun_orbit( GPC_CRS& crs );
	GPC_RES*	fun_turn( GPC_CRS& crs );
	GPC_RES*	fun_cross( GPC_CRS& crs );
	GPC_RES*	fun_getmx( GPC_CRS& crs, GPC_TAB* p_tab );
	GPC_RES*	fun_joy( GPC_CRS& crs, GPC_TAB* p_tab );
	
	GPC_RES*	fun_over( GPC_CRS& crs );
	GPC_RES*	fun_step( GPC_CRS& crs );
	GPC_RES*	fun_inc( GPC_CRS& crs );
	GPC_RES*	fun_cpy(GPC_CRS& crs);
	GPC_RES*	fun_dec( GPC_CRS& crs );

	GPC_RES*	fun_store( GPC_CRS& crs );

	GPC_RES*	fun_tabwh( GPC_CRS& crs );
	GPC_RES*	fun_tabxy( GPC_CRS& crs );

	GPC_RES*	fun_otos( GPC_CRS& crs );

	GPC_RES*	fun_file( GPC_CRS& crs );

	GPC_RES*	fun_near( GPC_CRS& crs );

	GPC_RES*	fun_expl( GPC_CRS& crs, GPC_TAB* p_tab );

	GPC_RES*	fun_break(void);
	GPC_RES*	fun_left(void);
	GPC_RES*	fun_right(void);
	// GPE_ALFA_AIMOM
	GPC_RES*	fun_ai( GPC_CRS& crs, GPC_AN tab_an );
	GPC_RES*	fun_aiy(GPC_CRS& crs); // , GPC_AN tab_an );
	GPC_RES*	fun_aiget( GPC_CRS& crs, GPC_AN tab_an );
	GPC_RES*	fun_aimom( GPC_CRS& crs, GPC_AN tab_an );
	GPC_RES*	fun_aifind( GPC_CRS& crs, GPC_AN tab_an );
	GPC_RES*	fun_listener( GPC_AN tab_an, GPC_WBx64* p_wb );
	GPC_RES*	fun_connect( GPC_AN tab_an, GPC_WBx64* p_wb );

	GPC_RES*	fun_winmove(  GPC_CRS& crs );

	GPC_RES*	res_join_i8( I8 n );
	GPC_RES*	res_vari( GPE_ALFA d0 );
	GPC_RES*	res_sht( GPC_CRS* p_mom, char* p_sht_name, GPC_AN& mom_an );
	GPC_RES*	res_equ( I8 d0 );
	GPC_RES*	res_equ( float d0 );
	GPC_RES*	res_equ( GPCV2 v2 );
	GPC_RES*	res_equ( U2V2 v2 );
	GPC_RES*	res_equ( GPCI4 v4 );
	GPC_RES*	res_equ(char* p_str, char* p_end = NULL, char mark = 0 );
	GPC_RES*	res_stack( GPC_RES* p_res_in );
	GPC_RES*	res2str( GPC_RES* p_fix );

	GPC_RES* res_alfa( GPE_ALFA alfa )
	{
		if( !fix_p_alloc )
			return NULL;

		if( alfa < GPE_ALFA_A )
			return (res_type == GPE_NET_res) ? ((GPC_RES**)(fix_p_alloc + fix_n_alloc))[-1] : this;
		
		if( res_id == alfa )
		{
			return this;
		}
		else if( res_type == GPE_NET_res )
		{
			GPC_RES	**pp_a = (GPC_RES**)fix_p_alloc,
				**pp_e = (GPC_RES**)(fix_p_alloc+fix_n_alloc);
			while( pp_a < pp_e )
			{
				if( pp_a )
				if( (*pp_a)->res_id == alfa )
					return (*pp_a);
				pp_a++;
			}
		}
		return NULL;
	}
	GPC_TAB*	res_adr( GPC_CRS* p_crs, GPC_CRS** pp_crs );

	char* res_p_str(  char* p_nincs = NULL,  char* p_buff = NULL )
	{
		if( !this )
			return NULL;
		
		if( res_type == GPE_NET_str )
		if( !p_nincs )
			return fix_p_alloc;
		else {
			return fix_p_alloc+GPD_NINCS( fix_p_alloc, p_nincs );
		}

		if( !fix_p_alloc || res_type != GPE_NET_RES )
		{
			if( !p_buff )
				return NULL;

			switch( res_type )
			{
				case GPE_NET_i8:
					sprintf( p_buff, "%lld", i8() );
					return p_buff;
				case GPE_NET_f4:
					{
						float f = f4();
						if( f == i8() )
							sprintf( p_buff, "%lld", i8() );
						else
							sprintf( p_buff, "%f", f );
					}
					return p_buff;
			}
			return NULL;
		}

		return (*(GPC_RES**)fix_p_alloc)->res_p_str( p_buff );
	}

	GPCI4& res_gpci4( void )
	{
		GPCI4 v( 0,0,0,0 );
		if( !this )
			return v;
		
		v.x = i8_an( 0, 0 );
		v.y = i8_an( 1, 0 );
		v.z = i8_an( 2, 0 );
		v.w = i8_an( 3, 0 );

		return v;
	}
};

class GPC_FILE : public GPC_RES
{
public:
	char	s_file[_MAX_PATH];
	GPC_WBx64* p_wb;

	GPC_FILE( GPC_WBx64* _p_wb, char* p_file ):GPC_RES( true )
	{
		GPM_CLR;
		p_wb = _p_wb;
		GPM_STRCPY( s_file, p_file );
	}
};
class GPC_CALC
{
public:
	I1	calc_add,
		calc_mul,
		calc_neg,
		calc_sw;		// 4

	GPE_ALFA	calc_id,
				calc_fun;		// 16
	GPC_RES*	calc_p_res;
	char*		calc_p_src;	// 32

	int	n_jump, calc_level;	// 40


	GPC_CALC()
	{
		memset(this, 0, sizeof(*this));
	}
	~GPC_CALC()
	{
		if (calc_fun == GPE_ALFA_END)
			return;

		GPM_DEL( calc_p_res );
	}

	I8	calc_n(GPC_FIX* p_fix);
	//GPC_CALC&	calc_swap(void);
	GPC_CALC&	calc_alfa(char* p_str, char* p_end, char** pp_str);
	GPC_CALC&	calc_num(char* p_str, char* p_end, char** pp_str);
	GPC_FIX*	calc_decod(GPC_FIX* p_fix);
	GPC_FIX*	calc_debug(GPC_FIX* p_fix);
	GPC_RES*	calc_adr(GPC_CRS& crs, GPC_TAB* ref = NULL );
	GPC_RES*	calc_vari( GPC_TAB* p_tab, GPC_CRS& crs);
	GPC_RES*	calc_alu( GPC_TAB* p_tab, GPC_CRS& crs, GPC_CALC* p_rts);

	//GPC_FIX*	calc_label( GPC_FIX* p_fix, char* p_label, char* p_label_end );

};
// - switch64 ----------------------
typedef enum GPE_SW64:ULL	// nshift	fun	mask
{
	GPE_SW64_CLR = 0,
	GPE_SW64_loop = 1,			// 0	ENTER	1
	GPE_SW64_degug = 2,			// 1	debug	2
	GPE_SW64_GPU = 4,			// 2	GPU	4
	GPE_SW64_ERROR = 8,			// 3	ERROR	8
	GPE_SW64_COLOLR = 16,		// 4	COLOR	16
	GPE_SW64_UNSEL = 32,		// 5	UNSEL	32
	GPE_SW64_BIN = 64,			// 6	BIN	64
	GPE_SW64_OFF = 128,			// 7	OFF	128	// kikapcsolja a keretet és a felíratot // egyedül a háttérkép marad ( ha van )
	GPE_SW64_JUMP = 256,		// 8	JUMP	256	// nem lehet cursorral bele mozogni
	GPE_SW64_FORBID = 512,		// 9	FORBID	512
	GPE_SW64_NO = 1024,			// 10	NO	1024	// no_mini
	GPE_SW64_REF = 2048,		// 11	REF	2048	// no_mini
	GPE_SW64_IN = 4096,			// 12	INCLUDE	4096
	GPE_SW64_GPU_done = 8192,	// 13
	GPE_SW64_GPUC = 8192*2,		// 14	16k
	GPE_SW64_NOO = 8192*4,		// 15	NOO	32k	// no_mini
	GPE_SW64_RED_ALARM = (ULL)1<<62,
	GPE_SW64_BUSY = (ULL)1<<63,
};
 
class GPC_TAB
{
public:
	GPC_FIX	*tab_p_src,
			*tab_p_exe,
			*tab_p_mini,
			*tab_p_compi;

	GPC_RES	*tab_p_in,
			*tab_p_res;
		
	GPC_TAB	*tab_p_up,
			*tab_p_down;
	//bool	b_left,b_right;
	ULL		tab_txt,
			tab_ver;

	I8		tab_id;
	GPC_AN	tab_an;

	int	tab_l, tab_u, tab_x, tab_y;
	
	ULL	switch64; //, n_ale,n_free;
	char	tab_s_name[0x100-2], b_left,b_right;

	void		tab_crsxy( GPC_CRS& crs );
	I8			tab_xy2pos( I8 _x, I8 _y, GPC_CRS& crs );
	void		tab_mini( GPC_CRS& crs, GPCV2& pos_wh, bool b_crs, bool b_secret = false, GPC_GIO_BUFF_VX324* p_mini = NULL );
	void		tab_mini_c4( GPC_TXT_C4* p_c4, GPC_CRS& crs, bool b_crs, bool b_secret = false );
	void		tab_line( GPC_CRS& crs, GPCV2& pos_wh, GPC_RES* p_res );
	void		tab_frame_c4( GPC_TXT_C4* p_c4,GPC_CRS& crs, GPC_RES* p_res );
	void		tab_solid( GPC_CRS& crs, GPCV2& pos_wh, GPC_RES* p_RES );
	
	void		tab_res_angle( GPC_CRS& crs, GPC_RES& res );
	void		tab_res_rules( GPC_CRS& crs, GPC_RES& res );

	void		tab_res( GPC_CRS& crs, GPC_RES* p_res );
	void		tab_res_plus( GPC_CRS& crs, GPC_RES* p_res );
	void		tab_size( GPC_CRS& crs );
	GPC_FIX*	tab_mini( GPC_CRS* p_crs );
	GPC_FIX*	tab_src( GPC_CRS* p_crs );
	GPC_TAB*	tab_set( GPC_CRS* p_crs,  GPE_ALFA a );
	char*		tab_str( GPC_CRS* p_crs, ULL pos, char** p_end );
	bool		tab_tab( GPC_CRS& crs );
	void		tab_enter( GPC_CRS& crs );
	void		tab_clear( void );
	GPC_TAB*	tab_insert( GPC_CRS& crs, char* p_src );
	GPC_FIX*	tab_compi_sw( char* &p_src, char* p_end );
	//GPC_FIX*	tab_compi2( void );
	GPC_FIX*	tab_compi( void );

	I8	tab_n_enter( GPC_CRS* p_crs );
	
	GPC_FIX*	tab_reN_kill_enter( GPC_CRS* p_crs, GPC_AN an, GPE_ALFA upleft );
	GPC_FIX*	tab_reN_enter( GPC_CRS* p_crs, GPC_AN an );
	GPC_FIX*	tab_reA_left( GPC_CRS* p_crs, GPC_AN an );
	GPC_FIX*	tab_reA_right( GPC_CRS* p_crs, GPC_AN an );
	GPC_RES*	tab_run( GPC_CRS& crs, GPE_ALFA alfa = GPE_ALFA_zero );
	void	tab_irq( GPC_CRS* p_crs );
	void	tab_irq_res( GPC_CRS& crs );
	
	bool	tab_q_in_lu( GPCI4& xy )
	{
		return	(xy.x >= tab_l)
			& (xy.y >= tab_u)
			& (xy.x <  tab_x+tab_l)
			& (xy.y <  tab_y+tab_u); 
	}
	void	tab_row_read( char* p_p, I8 n, char* p_mask );
	void	tab_row_write( GPC_CRS& crs, char* p_p, I8 n, char* p_mask );
};
class GPC_TIME
{
public:
	I8	year,
		mounth,
		day,
		hour,
		minute,
		sec,
		msec,
		reset_ms,
		on_ms,
		touch_ms;
		
	double	dsec;
	GPC_TIME( void )
	{
		GPM_CLR;
	}
	I8 get_sys( bool b_touch = false );
};
//class GPC_TAB_INS
//{
//public:
//	GPC_AN	an;
//	char*	p_set;
//	char*	p_txt;
//
//};
class GPC_CRS
{
public:
	GPC_TAB*	crs_p_root;
	ULL	crs_n_load,
		crs_n_alloc;

	GPC_GIO_VX32C *p_crs_vx,*p_crs_vxe;
	GPC_TAB	*crs_p_ref,
		*crs_p_tab_on,
		*crs_p_tab_sel;

	GPE_ALFA	crs_a1;
	I8	crs_n1,
		crs_p1;

	//GPC_TAB*	crs_p_tab2;
	//I8	crs_a2,
	//	crs_n2,
	//	crs_p2;

	GPC_TAB	*crs_p_edit,
		*crs_p_end;
	GPCI4	win_lurd_lb, win_lurd_rb;
	I8	win_l,
		win_u,
		win_r,
		win_d,

		crs_e,		

		crs_x, crs_y,

		crs_ver,
		crs_n_compi,
		crs_n_loop,
		crs_n_ready,
		crs_n_build,
		crs_n_render, crs_n_present,
		crs_n_undo,
		crs_n_ref;
	double	crs_elapse_t,
		crs_fsec_pre,
		crs_fps;
	GPC_TIME	crs_time;
	
	U2	plb,prb,
		//lb,rb,
		pctrl,pshift, palt; //,
		//ctrl,shift, alt;
	U2V2 crs_brd;
	GPCI4	crs_win,
		crs_win_trg,

		crs_m_pysic,

		crs_m_paqu,
		crs_m_aqu,

		//crs_mb_lr,
		crs_mxy_wzw,
		crs_prev_mxy_wzw,
		
		crs_mxy_wzw_lb,
		crs_mxy_wzw_lbdir,
		
		crs_mxy_wzw_rb,
		crs_mxy_wzw_rbdir,

		crs_m_on_luan,
		crs_m_on_inxy;

	I8	crs_mwheel;

	GPCI4	crs_build_xywh;
	GPCV2	crs_build_cwh, tab01;
	GPC_TAB	*crs_p_AN,
		**crs_pp_row;
	ULL	crs_n_row;

	GPC_WBx64* crs_p_wb;
	GPC_CRS*	mom_p_crs;
	GPC_AN		mom_tab_an;
	//GPC_TAB_INS**	pp_row_ins;
	//I8		n_row_ins;
	GPCI4	a_stack[0x100],
			*p_stack;

	GPC_UNDO	a_undo[GPD_CRS_NUNDO],
				*p_undo, *p_redo;

	GPC_GIO_BUFF_VX324	*res_p_mini_vx, crs_mini_vx,
						*res_p_line_vx, crs_line_vx,
						*res_p_solid_vx, crs_solid_vx;
	U1	crs_aBOOL[0x8]; // [0]crs_b_solid
	
	GPC_GPUx64_CONST	gpu_const;
	GPC_GISx64			*res_p_gis;

	I8	res_target_id,
		res_shadow_id,
		res_deep_id,
		res_hud_id,
		res_cam_id,
		*res_p_pic,
		res_n_pic,
		res_gpu_id,
		res_gate_id,
		res_gis_id,
		crs_n_debug;

	U1	res_b_clr,

		crs_b_push,
		crs_b_touch,
		crs_b_no_mini,
			
		crs_b_win_show,
		crs_b_win_move;

	char	*crs_p_file,		// ez maga az SHT file-> ide menti el magát
			crs_s_file[_MAX_PATH],	
			*crs_p_path,
			crs_s_path[_MAX_PATH*2],	// ez pedig ahonnan dolgozik // ezt szabad buferálni
			crs_s_tab_sel[_MAX_PATH],
			crs_s_tab_on[_MAX_PATH],
			
			crs_s_empty[0x20],
			*crs_p_cam, *crs_p_cam_e,
			*crs_p_pic, *crs_p_pic_e,
			crs_s_pub[0x10000],
			crs_s_sys[0x10000], *crs_p_sys;
	
	GPC_TIME	sht_time;
	GPC_TXT_C4 mc4;
	

	GPC_CRS( GPC_WBx64* p_wb, char* p_file, char* p_file_e, GPC_CRS* p_mom = NULL );
	~GPC_CRS();

	char* crs_path_set( I8 n_undo, char* p_path )
	{
		if( !this )
			return NULL;

		char* p_p = strrchr( p_path, '\\' );
		if( !p_p )
			return NULL;

		p_p++;
		
		crs_p_file = GPM_STRCPY( crs_s_file, p_path ) + (p_p-p_path);
		crs_p_path = GPM_STRCPY( crs_s_path, crs_s_file ) + (p_p-p_path);
		
		if( n_undo != crs_n_undo )
			crs_n_undo = n_undo;

		return crs_p_path;
	}
	void	crs_undo_record( void );
	void	crs_redo_record( void );
	void	crs_init( GPC_TAB* p_tab_mom );
	bool	crs_save( char* p_file = NULL );
	bool	crs_save_html( char* p_path, char* p_file );
	GPC_TAB*	crs_tab_a_n( GPE_ALFA a, I8 n );
	GPC_TAB*	crs_tab_an( GPC_AN& an, GPC_TAB* p_this = NULL, bool b_touch = false );
	ULL	crs_ascii_ins(U1* p_ascii, ULL n_ascii);
	GPC_CRS*	crs_touch( bool b_lb, bool b_mb, bool b_rb, bool b_shift, bool b_ctrl, bool b_alt );
	void	crs_ready( GPC_TAB* p_tab_mom );
	I8	crs_run( bool b_no_mini );
	GPC_CRS*	crs_build( GPC_TXT_C4* p_c4, GPC_TAB* p_tab_mom, GPCV2* p_tab_pos, bool b_gui, U2 n_deep );
	void	crs_build_plus(void);
	void crs_print( GPC_AN& an, char* p_format, ... )
	{
		if( !this )
			return;

		GPC_TAB*	p_tab = crs_tab_an( an );
		if( !p_tab )
			return;
		
		va_list vl;
		va_start(vl, p_format);
		char s_buffer[10240];
		ULL n = vsprintf(s_buffer, p_format, vl);
		if( n < 1 )
			return;

		p_tab->tab_p_src = p_tab->tab_src( this );

		ULL s = p_tab->tab_txt;
		p_tab->tab_p_src = p_tab->tab_p_src->fix_insert( s_buffer, n, s, -1 ); 
	}
	//GPC_TAB_INS* crs_ins( GPC_AN& an );
};



class GPC_TERRAx64
{
public:
	U2	x,y,w,yh,lut;
	GPC_COLOR4	color;

	GPC_TERRAx64& operator *= ( const U4 m )
	{
		x *= m;
		y *= m;
		w *= m;
		yh *= m;
		return *this;
	}
};

class GPC_TBOXx64
{
public:
	I8		id;
	U2		xl,yl,xh,yh;
	GPC_TERRAx64	*p_tra;
	U4		*p_idx,
			n_load, n_idx, n_alloc;
	bool	tbox_colled( I8 in_id, U2 in_xl, U2 in_xh, U2 in_yl, U2 in_yh, U4* pp_plus );
	~GPC_TBOXx64()
	{
		GPM_DELARRAY( p_idx );
		GPM_DELARRAY( p_tra );
	}
};

class GPC_ALTx64
{
public:
	I8	s,a,b;
	GPCV3	v_base,v_alt;
	float	alt, base;
	GPCV3&	alt_alt( GPCV3* p_coord, I8 n_coord, bool b_plus = false, bool b_div = true );
};

struct GPS_ROADx64
{
public:
	I8	n_idx,
		n_step,
		n_area;
	GPCI4	lurd;
	U2	in;
	U4	*p_row, *p_rowe, *p_rowleft;

	I8 bug( U4 *p_bug, U4 *p_t, U4 *p_te, U4* p_idx, I8 bob_w, I8 bob_h );
	I8 bug( char *p_bug, char *p_t, char *p_te, U4* p_idx, int bob_w, int bob_h );
};

class GPC_LALA
{
public:
	U4	level;
	float	alt;
};

class GPC_BOBx64
{
public:
	GPCV3	vc,vu,vv, v2c, v3d, v3dn, v3dn2, v3du,v3dv, bob_lc, bob_rc, v_eye,
		*p_coord, *p_3d;
	I8	n_coord;
	
	GPC_ALTx64		*p_alt; 
	I8		n_alt;
	GPCI4		alt_reset_enws, fun_enws;
				
	GPCV4		smpa;
	GPC_COLOR4		*p_bgra;
	GPC_TERRAx64	*p_terra;
	I8		n_terra;

	GPC_BOBx64		**pp_bob;
	I8		n_bob,
			n_area,
			bob_id,boss_id,
			left_id, new_id;

	GPC_LALA	*p_lala;
	U4	n_lala_level, n_lala, n_conc_level;

	U4	*p_histi, *p_hr,*p_hg,*p_hb,*p_hrgb,
		n_h4, n_h1,
		n_level, n_cut;
	GPCV2	pix_dim;
	float	weight;

	int	bob_x,bob_y,
		bob_w,bob_h,
		pic_w,pic_h,
		n_bgra;
	

	GPC_BOBx64( void )
	{
		GPM_CLR;
		left_id = -1;
		n_cut = 1;
	}
	GPC_BOBx64* bob_reset( GPC_COLOR4* p_in_bgra, int bob__x, int bob__y, int bob__w, int bob__h, int pic__w, int pic__h, GPCV2& pix__dim )
	{
		
		if( bob__x >= pic__w || bob__y >= pic__h )
			return this;
		if( bob__x+bob__w < 1 || bob__y+bob__h < 1 )
			return this;

		if( bob__x < 0 )
		{
			bob__w += bob__x;
			bob__x = 0;
		}
		if( bob__y < 0 )
		{
			bob__h += bob__y;
			bob__y = 0;
		}

		if( bob__x+bob__w > pic__w )
			bob__w -= (bob__x+bob__w)-pic__w;
		if( bob__y+bob__h > pic__h )
			bob__h -= (bob__y+bob__h)-pic__h;

		if( bob__w*bob__h < 16 )
			return this;

		if( !this )
		{
			
			GPC_BOBx64* p_this = new GPC_BOBx64;
			//if( !p_this )
			//	return NULL;
			return GPD_IF_THIS( p_this, bob_reset( p_in_bgra, bob__x, bob__y, bob__w, bob__h, pic__w, pic__h, pix__dim ), NULL );
			//return p_this->bob_reset( p_in_bgra, bob__x, bob__y, bob__w, bob__h, pic__w, pic__h, pix__dim );
		}

		for( GPC_BOBx64 **pp_b = pp_bob, **pp_be = pp_b+n_bob; pp_b < pp_be; pp_b++ )
			GPM_DEL( *pp_b );

		GPM_DELARRAY( pp_bob );
		n_bob = 0;
		GPM_DELARRAY( p_coord );
		GPM_DELARRAY( p_3d );
		n_coord = 0;
		GPM_DELARRAY( p_alt );
		n_alt = 0;
		GPM_DELARRAY( p_terra );
		n_terra = 0;

		if( p_bgra )
		if( n_bgra != bob__w*bob__h )
			GPM_DELARRAY( p_bgra );

		bob_x = bob__x;
		bob_y = bob__y;
		bob_w = bob__w;
		bob_h = bob__h;
		pic_w = pic__w;
		pic_h = pic__h;
		n_bgra = bob_w*bob_h;
		n_area = 0;

		if( !p_bgra )
			p_bgra = new GPC_COLOR4[n_bgra];
		
		if( bob_x == 0 && bob_y == 0 && bob_w == pic__w && bob_h == pic__h )
			gp_memcpy( p_bgra, p_in_bgra, n_bgra*sizeof(*p_bgra) );
		else {
			for( int h = 0; h < bob_h; h++ )
			{
				gp_memcpy( p_bgra+h*bob_w, p_in_bgra+bob_x+((bob_y+h)*pic__w), bob_w*sizeof(*p_bgra) );
			}
		}

		for( GPC_COLOR4* p_pix = p_bgra, *p_e = p_pix+n_bgra; p_pix < p_e; p_pix++ )
		{
			n_area += p_pix->A;
		}
		n_area >>= 8;

		if( n_area < 16 )
		{
			delete this;
			return NULL;
		}
		pix_dim = pix__dim;
		vu.x = pix_dim.x*float(bob_w/2);
		vv.y = pix_dim.y*float(bob_h/2);
		vc.x = vu.x + (bob_x-(pic_w/2))*pix_dim.x;
		vc.y = ((pic_h/2)-bob_y)*pix_dim.y - vv.y;
		return this;
	}
	~GPC_BOBx64()
	{
		delete[] p_bgra; 
		for( GPC_BOBx64 **pp_b = pp_bob, **pp_be = pp_b+n_bob; pp_b < pp_be; pp_b++ )
			GPM_DEL( *pp_b );

		GPM_DELARRAY( pp_bob );
		GPM_DELARRAY( p_coord );
		GPM_DELARRAY( p_alt );
		GPM_DELARRAY( p_histi );
		GPM_DELARRAY( p_terra );
		GPM_DELARRAY( p_lala );
	}
	U4	bob_dezso( void );
	U4	bob_histi( U4 n_min, float balance );
	I8	bob_balance( float balance );
	I8	bob_dezs( I8 n_div = 3 );
	I8	bob_bugi3( I8 n_div );
	I8	bob_bugi2( I8 n_div );
	I8	bob_bugi( I8 n_div );
	
	void	alt_shift( I8 n_left );
	//GPCV3*	alt_box( GPCI4& nesw );
	void	alt_reset( void );
	void	alt_reset_3d( void );
	void	alt_vc_uv( void );
	void	alt_v3d( float rad, float yaw );

	void	bob_alt( void );
	void	bob_erosing( I8 r );
	void	bob_dilating( I8 r );
	void	bob_size( void );
	//void	bob_ROAD2ALFA_r( GPCI4& xywh, GPC_BOBx64* p_boss, U4* p_idx_map, GPS_ROADx64* p_r, I8 r );
	void	bob_ROAD2ALFA2( GPCI4& xywh, GPC_BOBx64* p_boss, U4* p_idx_map, GPS_ROADx64* p_r );
	void	bob_ROAD2ALFA( GPCI4& xywh, GPC_BOBx64* p_boss, U4* p_idx_map, GPS_ROADx64* p_r );
	void	bob_ALFAx2( GPCV2& wh, GPC_BOBx64* p_boss );
	
};

DWORD WINAPI GPF_BOBx64_EXEC_TRD( LPVOID p_param );

class GPC_BOBx64_EXEC
{
public:
	GPE_WIP		wip100;
	HANDLE		exec_whd;
	GPC_BOBx64	*p_exec_bob;
	I8			id,n;
	GPE_CPU		cpu_id;

	GPC_COLOR4	color;
	float		balance;
	I8			n_div, n_fat;

	GPC_BOBx64_EXEC( void )
	{
		GPM_CLR;
	}
	GPC_BOBx64_EXEC*	GPC_BOBx64_EXEC_ncpu( I8 n_cpu );
	bool		q_done( void );
	bool		set_go( void );
	bool		set_wait( void );
	bool		set_exit( void );
	void		set_done( void );
	
};
class GPC_BOB_MEM
{
public:
	I8 bob_id, equ_id;
	GPCV3 v3d, v3dn, v3du, v3dv;
	float dif;
	bool b_die;
};
class GPC_PICx64INF
{
public:
	U4* p_histi, n_histi;
	I8	n_div,
		n_fat;

	GPC_PICx64INF(void)
	{
		GPM_CLR;
	}
	~GPC_PICx64INF()
	{
		GPM_DELARRAY( p_histi );
	}
};

//#define GPD_buff2D_BACK

class GPC_PICx64
{
public:
	I8	pic_id,
		wip_i8,
		n_qc, // PIC_upload n_new++;			p_target->n_new = n_qc;
		n_cpu; // if( n_cpu >= n_new )	return false;	p_target->n_cpu = n_qc-1;

	GPC_PICx64INF* p_pic_inf;

	int	pic_w,pic_h, pic_d;
	int	deep_w,deep_h;
	int	new_w,new_h;
	GPCV3	pic_dim, new_dim;
	GPCV2	pix_dim;

	char			s_name_[_MAX_PATH],
					*p_name_,
					s_file[_MAX_PATH];
	D3D11_VIEWPORT		pic_viewport;
	ID3D11Texture2D
#ifdef GPD_buff2D_BACK
				*p_buff2D_BACK,
#endif
				*p_buff2D_bgra,
				*p_buff2D_depth,
				*p_buff2D_stage;

	ID3D11Texture3D		*p_buff3D_bgra;		

	ID3D11ShaderResourceView	*p_samp_bgra;
	
	ID3D11RenderTargetView	*p_target_bgra;
	ID3D11DepthStencilView	*p_target_depth;
	HRESULT			hr;

	U4	*p_bgra, n_bgra,
		*p_res_bgra, n_res_bgra;
	GPCV4	*p_bgra_hdr;

	GPC_AN	res_an;
	
	GPC_BOBx64	**pp_bob;
	I8			n_bob, mx_N;
	GPC_BOBx64	**pp_wip_bob;
	I8			 wip_s, wip_i, n_wip_bob;		

	GPC_BOB_MEM			*p_bob_mem;
	I8					n_bob_mem;

	GPC_BOBx64_EXEC*	p_exec;
	U4					n_exec;
	GPC_WBx64*			p_wb;

	bool	b_hdr, b_hdr_new, b_back_buff;

	GPC_PICx64( char* p_file, U2 w, U2 h, GPCV3& dim, I8 id, bool b_hdr = false );
	~GPC_PICx64();

	GPC_PICx64&	operator = ( GPC_PICx64 &cpy );

	bool	bob_3d_drop_equ2( void );
	bool	bob_3d_drop_equ( void );
	bool	bob_3d_sort_size( bool b_reuv = true );
	bool	bob_3d_sort_z( void );
	bool	bob_3d_sort_pos( I8 n_max, GPCV3 pos );
	bool	bob_3d_sort_y( I8 n_max );
	void	bob2bgra( GPC_WBx64* p_wb, I8 s, I8 e );
	void	wip2bgra( GPC_WBx64* p_wb, I8 s, I8 e );
	void	bob_ready( GPC_WBx64* p_wb );
	bool	pic_bob_alt( GPC_WBx64* p_wb );
	bool	bob_fill_size( GPC_WBx64* p_wb );
	void	bgra_alt( void );
	void	bgra_histi( void );
	void	bgra_minimax( void );
	void	bgra_minimax_histi( void );
	void	bgra_smooth( I8 r );
	void	bgra_scanline( void );
	void	bgra_scanline2( void );
	void	bgra_scanline3( I8 r );
	void	bgra_eyebug2( float n_eb, I8 n_dil );
	void	bgra_eyebug( I8 n_eb, I8 n_dil );
	void	bgra_stereo_histi2( void );
	void	bgra_stereo_histi( I8	n_cut );

	void	bgra_ndiv( I8 n_div, I8 n_fat );
	void	bgra_rubik( I8 n_div, bool b_upload = false );
	void	bgra_hx2( bool b_upload = false );
	void	bgra_erosing( I8 n, bool b_upload = false );
	void	bgra_dilating( I8 n, bool b_upload = false );
	void	bgra_more( I8 r, bool b_upload = false );
	bool	bgra_explode( GPE_CPU cpu_id );

	void	bgra_furi( I8 f, bool b_upload = false );
	void	bgra_feri( I8 w1, I8 w2, bool b_upload = false );
	void	bgra_ferike( int trash, int div, bool b_upload = false );
	void	bgra_glue( I8 r );
	void	bgra_cartoon( void );
	void	bgra_bin( I8 r );
	GPC_RES*	bgra_npix( float perc );
	bool	pic_nbob( void ); //GPC_WBx64* p_WB );
	bool	pic_ndezs( void );
	I8	pic_sort_size( I8* p_i8a, I8* p_i8ae );
	I8	pic_to_center( I8* p_i8a, I8* p_i8ae, GPCV3 to_c );
	I8	pic_to_eye( I8* p_i8a, I8* p_i8ae, GPCV2 to_c, GPCV3 up, GPCV3 pos, GPCV3 deep );
	I8	pic_filter_equ( I8* p_i8a, I8* p_i8ae, GPCV3 to_c, float dif );
	bool	pic_mono( GPC_WBx64* p_wb, char* p_gio_name, float focus_mm, float cam_mm, float desk_mm, float desk_rad, float desk_yaw, float ccd_mn, float ccd_mx, float bump_mm );
	bool	pic_stereo( GPC_WBx64* p_wb, char* p_gio_name, float f, float e, float desk, float desk_rad, float desk_yaw, float to_center = 0.75 );
	
	bool	pic_stereo4( GPC_WBx64* p_wb, char* p_gio_name, float eye_mm, float focus_mm, float cam_mm, float desk_mm, float desk_rad, float desk_yaw, float mn_mm, float mx_mm, float bump_mm );

	bool	pic_stereo3( GPC_WBx64* p_wb, char* p_gio_name, float f, float e, float desk, float desk_rad, float desk_yaw, float ccd_mn, float ccd_mx, float to_center = 0.75 );
	bool	pic_stereo2( GPC_WBx64* p_wb, char* p_gio_name, float f, float e, float desk, float desk_rad, float desk_yaw, float ccd_mn, float ccd_mx, float to_center = 0.75 );
	
	bool	pic_save( GPC_WBx64* p_wb, char* p_file, GPC_FIX* p_fix = NULL, bool b_bgra = false, ULL e0 = 0 );
	U4*	pic_p_bgra( void )
	{
		//if( !this )
		//	return NULL;

		if( !n_bgra )
			return NULL;
		
		if( p_bgra )
			return p_bgra;

		if( !p_bgra_hdr )
			return NULL;
		
		p_bgra = new U4[n_bgra];
		GPC_COLOR4* p_c4 = (GPC_COLOR4*)p_bgra;
		for( I8 i = 0; i < n_bgra; i++ )
		{
			 p_c4[i] = p_bgra_hdr[i];
		}
		
		return p_bgra;
	}

	GPC_COLOR4* pic_p_swap_c4( void )
	{
		//if( !this )
		//	return NULL;
		

		U4	*p_d = pic_p_bgra(),
			*p_s = p_bgra, *p_ss = p_s,
			*p_de = p_d+n_bgra;

		register  ULL* p_ld = (ULL*)p_d, *p_lde = p_ld+(n_bgra/2),
				bgrabgra,	rgbargba; 
		for( ; p_ld < p_lde; p_ld++, p_s += 2 )
		{
			bgrabgra = *(ULL*)p_s;
			rgbargba =	  (bgrabgra&0x00000000ff000000) | ((bgrabgra&0x00000000ff0000)>>16) | (bgrabgra&0x00000000ff00) | ((bgrabgra&0x00000000ff)<<16) 
						| (bgrabgra&0xff00000000000000) | ((bgrabgra&0xff000000000000)>>16) | (bgrabgra&0xff0000000000) | ((bgrabgra&0xff00000000)<<16);
			*p_ld = rgbargba;
		}
		bgrabgra = p_s-p_ss;
		if( bgrabgra < n_bgra )
		{
			U4 bgra, rgba;
			p_d += bgrabgra;
			for( ; p_d < p_de; p_d++, p_s++ )
			{
				bgra = *p_s;
				rgba = (bgra&0xff000000) | ((bgra&0xff0000)>>16) | (bgra&0xff00) | ((bgra&0xff)<<16) ;

				*p_d = rgba;
			}
		}

		return (GPC_COLOR4*)p_bgra;
	}

	GPC_COLOR4* pic_p_bgra_c4( void )
	{
		//if( !this )
		//	return NULL;
		return (GPC_COLOR4*)pic_p_bgra();
	}

	GPCV4*	pic_p_bgra_hdr( void )
	{
		if( !this )
			return NULL;

		if( !n_bgra )
			return NULL;
		
		if( p_bgra_hdr )
			return p_bgra_hdr;

		if( !p_bgra )
			return NULL;
		
		p_bgra_hdr = new GPCV4[n_bgra];
		GPC_COLOR4* p_c4 = (GPC_COLOR4*)p_bgra;
		for( I8 i = 0; i < n_bgra; i++ )
		{
			p_bgra_hdr[i] = p_c4[i];
		}

		return p_bgra_hdr;
	}
};
class GPC_LWO_FACE
{
public:
	U4 a_idx[0x10];
	U2 n_p, part, surf, colr, n_part, n_surf;
};
class GPC_LWO_TAG
{
public:
	char s_name[0x20];
	U4 a_pic[0x10];
	U2 n_p;
	GPC_LWO_TAG( void )
	{
		GPM_CLR;
	}
};
//class GPC_GIO_VX_TREE
//{
//	public:
//		GPC_GIO_VX_TREE	*p_low,*p_high,*p_back;
//		float*		p_float;
//		U4		n_float;
//		U4		id, n_id;
//		//GPC_GIO_VX_TREE(){};
//		GPC_GIO_VX_TREE( GPC_GIO_VX_TREE* _p_back, GPC_GIO_VX_TREE* _p_low, U4 now_id, float* p_now, U4 n_now )
//		{
//			p_low = _p_low;
//			if( p_low )
//			{
//				p_back = p_low->p_back;
//				p_low->p_back = p_low;
//			} else {
//				p_back = _p_back;
//			}
//			p_high = NULL;
//			n_float = n_now;
//			p_float = (float*)gp_memcpy( new float[n_float], p_now, n_float*sizeof( *p_float ) );
//			id = now_id;
//			n_id = now_id+1;
//		}
//		~GPC_GIO_VX_TREE()
//		{
//			GPM_DEL( p_low );
//			GPM_DEL( p_high );
//			GPM_DELARRAY( p_float );
//		}
//		GPC_GIO_VX_TREE* add( float* p_now, U4 n_now, GPC_GIO_VX_TREE** pp_find )
//		{
//			if( !p_now )
//			{
//				if( pp_find )
//					*pp_find = this;
//				return this;
//			}
//
//			if( !this )
//			{
//				GPC_GIO_VX_TREE* p_find = new GPC_GIO_VX_TREE( NULL, NULL, 0 ,p_now, n_now );
//				if( pp_find )
//					*pp_find = p_find;
//				return p_find;
//			}
//
//			if( pp_find )
//				*pp_find = this;
//			
//			float	*p_lf, *p_le,
//				*p_nf, *p_ne = p_now + n_now, l, n ,d;
//			
//			GPC_GIO_VX_TREE* p_loop = this;
//			while( p_loop )
//			{
//				p_lf = p_loop->p_float,
//				p_le = p_lf + p_loop->n_float,
//				p_nf = p_now;
//				
//				while( (p_lf < p_le) && (p_nf < p_ne) )
//				{
//					if( *p_lf != *p_nf )
//						break;
//					p_lf++;
//					p_nf++;
//				}
//
//				l = (p_lf < p_le) ? *p_lf : 0.0f;
//				n = (p_nf < p_ne) ? *p_nf : 0.0f;
//				d = n-l;
//
//				if( d == 0.0f )
//					break;
//				
//				if( d > 0.0f )
//				{
//					if( p_loop->p_high )
//					{
//						p_loop = p_loop->p_high;
//						continue;
//					}
//
//					p_loop->p_high = new GPC_GIO_VX_TREE( p_loop, NULL, n_id, p_now, n_now );
//					n_id = p_loop->p_high->n_id;
//					if( pp_find )
//						*pp_find = p_loop->p_high;
//					return this;
//				} else {
//					if( p_loop->p_low )
//					{
//						p_loop = p_loop->p_low;
//						continue;
//					}
//
//					p_loop->p_low = new GPC_GIO_VX_TREE( p_loop, NULL, n_id, p_now, n_now );
//					n_id = p_loop->p_low->n_id;
//					if( pp_find )
//						*pp_find = p_loop->p_low;
//					return this;
//				}
//			}
//
//			if( pp_find )
//				*pp_find = p_loop;
//			return this;
//		}
//
//};
class GPC_GIOx64_PART
{
public:
	GPC_AN pns;
	I8 *p_s;

	I8 part_add( GPE_ALFA alfa, I8 s )
	{
		// ez úgy müködik, hogy feltételezi, van egy elég nagy tömb és annak az elsõ eleméröl indul
		GPC_GIOx64_PART* p_loop = this;
		while( p_loop->pns.a > GPE_ALFA_zero )
		{
			if( p_loop->pns.a == alfa )
			{
				break;	
			}
			p_loop++;
		}
		
		if( s > -1 )
		{
			I8 *p_kill = p_loop->p_s;
			p_loop->p_s = new I8[p_loop->pns.n+1];
			if( p_kill )
				gp_memcpy( p_loop->p_s, p_kill, p_loop->pns.n*sizeof(*p_s) );
			p_loop->p_s[p_loop->pns.n] = s;
			p_loop->pns.n++;
			GPM_DELARRAY( p_kill );
		}
		if( p_loop->pns.a < GPE_ALFA_A )
			p_loop->pns.a = alfa;
		return p_loop-this;
	}
};

class GPC3D_BUFF
{
public:
	//GPC_GIO_BUFF_VX32CU	buf_vx,
	GPCVOX_BUFF	buf_vox;
	GPC_GIO_BUFF_VX32CU	pat_vx;
	I8		gio_id,
			n_buff,
			n_load,
			n_alloc,
			pat_vn,
			pat_in,
			n_alloc_idx; 
	HRESULT		hr;

	//ID3D11Buffer	*gp_p_dx_lod_ix;
	//ULL		gp_n_dx_lod_ix;
	bool		b_solid, b_skip;


	GPC3D_BUFF(void){};

	GPC3D_BUFF( I8 gid )
	{
		GPM_CLR;
		gio_id = gid;
	}
};

class GPC_3D64_DX
{
public:
	GPC_GPUx64_CONST	cbuff;
	ID3D11Buffer	*gp_p_dx_const;

	U4		n_stride,
			offset;
	HRESULT		lod_hr;

	//char		s_gpu[0x100];
	//I8		gpu_id;
	bool		b_update;

	bool	c2dx( GPC_WBx64* p_wb );
	void	free( void )
	{
		GPM_RELEASE( gp_p_dx_const );
	}
	~GPC_3D64_DX()
	{
		free();
	}
};

class GPC_GIOx64_LAY_LOD
{
public:
	ID3D11Buffer	*gp_p_dx_lod_vx;
	ULL		gp_n_dx_lod_vx;

	GPC_GPUx64_CONST	cbuff;
	ID3D11Buffer	*gp_p_lod_const;
	//I8		n_save_vx; // ha át lett pakolva nem szabad releas-elni a gp_p_dx_lod_vx-t, mert másikba lett rakva !!!		

	//ID3D11Buffer	*gp_p_dx_lod_ix;
	//ULL		gp_n_dx_lod_ix;
	GPC_GIO_SURF	*p_surf;
	ULL		n_surf;

	U4		stride,
			offset;
	HRESULT		lod_hr;

	GPC3D_BUFF*	p_idx_buff;


	bool		b_upload, b_solid, b_off;
	GPC3DHI*		p_3d_hd;

	GPC_GIOx64_LAY_LOD( void )
	{
		GPM_CLR;
		lod_hr = S_FALSE;
	}

	~GPC_GIOx64_LAY_LOD( void )
	{
		lod_free();
	}
	bool q_free( I8 n )
	{
		for( GPC_GIOx64_LAY_LOD *p_lod = this, *p_e = p_lod+n; p_lod<p_e; p_lod++ )
		{
			if( p_lod->gp_p_dx_lod_vx )
				return false;
		}
		return true;
	}
	GPC_GIOx64_LAY_LOD* lod_free( void )
	{
		GPM_RELEASE( gp_p_dx_lod_vx );
		GPM_RELEASE( gp_p_lod_const );
		GPM_DELARRAY( p_surf );
		n_surf = 0;
		lod_hr = S_FALSE;
		b_upload = false;
		b_off = true;
		p_3d_hd = NULL;
		return this;
	}
	void n_free( I8 n = 1 )
	{
		for( GPC_GIOx64_LAY_LOD *p_lod = this, *p_e = p_lod+n; p_lod < p_e; p_lod++ )
		{
			p_lod->lod_free();
		}
	}
	//void lod_upl( GPC_GIOx64_LAY* p_lay, GPC_GIO_BUFF_VX32CU& solid_vx );
	void lod_upl( GPC_WBx64* p_wb, GPC_GIOx64_LAY* p_lay, GPC3D_BUFF* p_buff );
	//void lod_render( GPC_GIOx64_LAY* p_lay, I8 l, I8 map );
	bool lod_const( GPC_WBx64* p_wb );
	
};

class GPC_GIOx64_LAY
{
public:
	//GPC_GIOx64_LAY_LOD*		p_lod;
	//I8			n_lo_lod, n_lod, n_lod_upl, n_lod_render, n_lod_thread;


	ULL				n_solid_update, n_solid_upload;
	ID3D11Buffer	*gp_p_dx_solid_vx;
	ULL				gp_n_dx_solid_vx,
					gp_n_dx_solid_load;

	ID3D11Buffer	*gp_p_dx_solid_idx;
	ULL				gp_n_dx_solid_idx,
					gp_n_dx_solid_idx_load;
	I8				strip_id, strip_ver, a_pic_id[2];
	bool			b_strip;

	ID3D11Buffer	*gp_p_dx_lod_ix;
	ULL				gp_n_dx_lod_ix;

	GPC_GIO_BUFF_VX32CUFBI	solid_vx;

	char			s_name[0x100];

	GPCV3			pivot, bbox[2];
	float			r;
	U4				lay_id,
					parent_id,
					stride,
					offset;
	
	GPC_GIOx64_PART	*p_part;
	I8				n_part;

	// MX
	I8			n_mx;
	GPCMX		mx_base,
				mx_cam,
				*p_mx_reset,
				*p_mx_set;

	char		*p_mx_name;
	GPC_GIOx64	*p_lay_gio;			
	I8			n_lay_upl, n_lay_upd;
	HRESULT		hr, hr_idx;

	float		weight;
	I8			gx_ms,
				map_skip;
	
	//GPC3D_BASE		*p_3d_base;
	GPC_GIVx64	*p_lay_giv;

	// ----------------- instance -------------------
	ID3D11Buffer	*gp_p_dx_c_lay;
	void			*p_vx;
	U4				n_vx_load, n_vx_alloc, n_bit;
	I8				ins_gpu_id, ins_gpu_ver, n_qc; //, n_cpu;


	GPC_GIOx64_LAY(void)
	{
		GPM_CLR;
	}
	void lay_clear( GPC_GIOx64* p_g = NULL )
	{
		GPM_CLR;
		p_lay_gio = p_g;
		strip_id = -1;
	}

	void lay_free( void )
	{
		GPM_RELEASE( gp_p_dx_solid_vx );
		GPM_RELEASE( gp_p_dx_solid_idx );
	}

	~GPC_GIOx64_LAY(void)
	{
		lay_free();
	}

	GPC_GIOx64_LAY* lay_ready( GPC_LWO_FACE* p_start, GPC_LWO_FACE* p_end, GPC_LWO_TAG* p_tags, char n_bone_min, char n_bone_max, char** pp_pic  );
	GPC_FIX*	lay_info( GPC_FIX* p_info );
	char*	lay_read( char* p_mem, char* p_end );
	GPC_FIX*	lay_write( GPC_FIX* p_buff );
	void lay_upload( void );
	void lay_render( I8 map );
	void lay_rnd_q4( I8 map, I8 q4 );
	void lay_vox_cell( GPC_WBx64* p_wbx64, I8 i, U1 p );
};

class GPC_GIOx64_CONST
{
public:
	GPCV4	sec, LIGq, a_pad1[2];	// 64
	GPCMX	mxi_wrd,			// 64		128
			a_mx_wrd[0x18],	// 64*24	1536
			a_mxi_rst[0x18];	// 64*24	1536	3072	3200

	//float	pad[44];			// 0xd00

	U2		b_upd, b_intl;		// 0xd04
	U2		size, n_mx;		// 0xd08
	I8		item_id, ver, 		// 0xd18
			LIGid, pad;		// 0xd28

	HRESULT		hr;			// 
	D3D11_USAGE	Usage;		//

	GPC_GIOx64_CONST(void)
	{
		GPM_CLR;
		size = GPM_OFF( GPC_GIOx64_CONST, b_upd );
		b_intl = GPD_CBUFF;
		LIGid = -1;
	}
	void q_irq( void )
	{
		if(!this)
			return;
		b_upd = true;
		b_intl = GPD_CBUFF;
	}
	ID3D11Buffer* upld( ID3D11Device* p_dev, ID3D11DeviceContext* p_contx, ID3D11Buffer* p_dx_c_gio )
	{
		ID3D11Buffer* p_dx_c_gis_kill = p_dx_c_gio;
		D3D11_BUFFER_DESC bDSC;
		gp_zero( &bDSC, sizeof(bDSC) );
		//if( n_mx > 2 )
			bDSC.ByteWidth = size;
		//else {
		//	bDSC.ByteWidth = sizeof(sec) + sizeof(GPCMX)*(n_mx+1);
		//}
		//if( bDSC.ByteWidth > size )
		//	bDSC.ByteWidth = size;
		
		if( b_intl || !p_dx_c_gio )
		{
			D3D11_SUBRESOURCE_DATA iniDT;
			iniDT.pSysMem = this;
			iniDT.SysMemPitch = 0;
			iniDT.SysMemSlicePitch = 0;
			
			
			bDSC.Usage = D3D11_USAGE_DEFAULT;			// D3D11_USAGE_STAGING;// D3D11_USAGE_IMMUTABLE; //D3D11_USAGE_DEFAULT;//    D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
			bDSC.BindFlags = D3D11_BIND_CONSTANT_BUFFER;		// use as a vertex buffer
			hr = p_dev->CreateBuffer( &bDSC, &iniDT, &p_dx_c_gio );

			GPM_RELEASE( p_dx_c_gis_kill );

		} else {
			bDSC.Usage = D3D11_USAGE_DYNAMIC;
			bDSC.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bDSC.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			if( Usage != bDSC.Usage )
				GPM_RELEASE( p_dx_c_gio );
			if( !p_dx_c_gio )
				hr = p_dev->CreateBuffer( &bDSC, NULL, &p_dx_c_gio );
			
			D3D11_MAPPED_SUBRESOURCE ms;
			hr = p_contx->Map( p_dx_c_gio, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
			if( !FAILED(hr) )
			{
				if( bDSC.ByteWidth < ms.DepthPitch )
					gp_memcpy(ms.pData, this, bDSC.ByteWidth );
				else 
					gp_memcpy(ms.pData, this, ms.DepthPitch );
				p_contx->Unmap( p_dx_c_gio, NULL );
			}
		}
		Usage = bDSC.Usage;
		b_upd = false;
		return p_dx_c_gio;
	}
};


class GPC_CELL
{
	public:
		char	v10, v11,v12,v13,
			v00, v01,v02,v03;
		U1 cell( void )
		{
			if( !this )
				return 0;

			U1 c = 0;
			c = (bool)v13;
			c <<= 1;
			c |= (bool)v12;
			c <<= 1;
			c |= (bool)v11;
			c <<= 1;
			c |= (bool)v10;
			c <<= 1;
			c |= (bool)v03;
			c <<= 1;
			c |= (bool)v02;
			c <<= 1;
			c |= (bool)v01;
			c <<= 1;
			c |= (bool)v00;
			return c; 
		}
};
struct GPS_CAKE
{
	GPCV3	pos,
		norm;
	//GPCV2	uv;
};



class GPC_GIOx64
{
public:
	char s_name[0x100];
	I8 gio_id;

	ID3D11Buffer	*gp_p_dx_c_gio,
					*gp_p_dx_c_bullet;
	// LINE
	ULL				n_gio_line_upd, n_gio_line_upl;
	ID3D11Buffer	*gp_p_dx_line_vx;
	ULL				gp_n_dx_line_vx,
					gp_n_dx_line_load;

	ID3D11Buffer	*gp_p_dx_line_idx;
	ULL				gp_n_dx_line_idx,
					gp_n_dx_line_idx_load;

	GPC_GIO_BUFF_VX324	line_vx;

	// SOLID
	GPC_GIOx64_LAY	*p_gio_lay;
	I8				gio_n_lay, n_inst, n_sky, n_gnd;
	bool			b_inst;

	U4		stride,
			offset;
	HRESULT	hr;
	GPC_WBx64	*p_wb;
	float		gio_dim; // lwo vagy valamelyik betöltö ezzel szorozza fel a vertex coordinátákat

	GPC_FIX*	p_info;
	I8			n_save;
	
	GPC_GIOx64( GPC_WBx64* p__wb, char* p_name );
	~GPC_GIOx64();

	void		gio_reset( void );
	void		gio_upload( void );
	void		gio_render( I8 map );
	void		gio_render_q4( I8 map, I8 q4 );
	GPC_FIX*		gio_info( GPC_FIX* p_info );
	GPC_FIX*		gio_write( GPC_FIX* p_info );
	GPC_GIOx64_LAY*	gio_p_lay( I8 i );
	char*		gio_read_lay( char* p_mem, char* p_end );
	GPC_GIOx64*	gio_read( GPC_WBx64* p__wb, char* p_name, GPC_FIX* p_info );

	GPC_GIO_VX32C*	fake_box( float size, GPCV3 cam );

	
};

class GPC_DICK_REC
{
public:
	char		*p_str, *p_rec;
	I8			n_id, //n_level,
				n_lo, n_hi,
				n_rec;

	~GPC_DICK_REC()
	{
		if( p_str == p_rec )
			GPM_DELARRAY( p_str );
	}
};

class GPC_DICK
{
public:
	GPC_DICK_REC	*dick_p_alloc,
					*dick_p_load,
					*p_find,
					*p_back;
	
	I8				dick_n_alloc;
	~GPC_DICK()
	{
		GPM_DELARRAY( dick_p_alloc );
	}
	GPC_DICK* dick_add( char* p_src, char* p_token = " \t\r\n" )
	{
		if( !p_src )
			return NULL;
		
		p_src += GPD_NINCS( p_src, p_token );
		I8 n = GPF_STR_VISZ_NINCS( p_src,  NULL, p_token );
		if( p_src[n] )
		{
			n++;	
		}
		if( n < 1 )
			return NULL;
		
		char	*p_begin = gp_strncpy( (new char[n+1]), p_src, n+1),
			*p_rec = p_begin,
			*p_end = p_begin + n,
			*p_next = p_begin;
		*p_end = 0;
		if( !*p_next )
			return NULL;
		
		GPC_DICK* p_this = this;

		while( p_rec < p_end )
		{
			p_rec += GPD_NINCS( p_rec, p_token );
			p_next = p_rec+GPD_VAN( p_rec, p_token );
			if( p_next == p_rec ) // igazábol == csak hátha valami elrontja a pointert és p_loop mondjuk valami p_rec-humbulumbu
				return p_this; // akkor kész

			p_this = p_this->dick_find( p_rec, p_next - p_rec, p_begin );
						 
			p_next++;
			p_rec = p_next;
		}

		return p_this;
	}

	GPC_DICK* dick_find( char* p_word, I8 n_word, char* p_begin = NULL )
	{
		// ha van p_str beleír a szó végére egy 0-át hogy zárt szringet csináljon a szóból 

		if( n_word < 1 )
			return this;
		if( p_word ? p_word < p_begin : true )
			return this;
		
		if( !this || !dick_p_alloc )
		{
			if( !p_begin )
				return this;

			GPC_DICK* p_this = this ? this : (GPC_DICK*)memset( new GPC_DICK, 0, sizeof(*p_this) );
			if( p_this )
			{
				p_this->dick_n_alloc = 0x20;
				p_this->dick_p_alloc = p_this->dick_p_load = new GPC_DICK_REC[p_this->dick_n_alloc];
				p_this->p_back = NULL;
				p_this->p_find = p_this->dick_p_alloc;
				p_this->p_find->p_str = p_begin;
				
				if( p_word > p_begin )
				{
					char *p_dest = p_begin;
					while( *p_word )
					{
						*p_dest = *p_word;
						p_dest++;
						p_word++;
					}
					*p_dest = 0;
					p_word = p_begin;
				}

				(p_this->p_find->p_rec = p_word)[p_this->p_find->n_rec = n_word] = 0;
				p_this->p_find->n_lo = p_this->p_find->n_hi = p_this->p_find->n_id = 0; //n_level,
				p_this->dick_p_load++; 
			}

			return p_this;
		}

		p_find = NULL;
		if( !dick_p_alloc )
			return this;
		
		I8 n_char;
		char	*p_r,
			*p_re,
			*p_w;

		p_back = dick_p_alloc;
		while( p_back < dick_p_load )
		{
			n_char = n_word;
			if( n_char > p_back->n_rec )
				n_char = p_back->n_rec;

			for(	p_r = p_back->p_rec,	p_re = p_r + n_char,	p_w = p_word;
				p_r < p_re;
				p_r++, p_w++	)
			{
				if( *p_r != *p_w )
					break;
			}

			
			if( !*p_r ) // eljutot a rec végéig?
			if( p_w-p_word == n_word ) // eljutott a feresett szó végéig?
			{
				p_find = p_back;
				return this; // megtalálta
			}

			if( (*p_w - *p_r) > 0 )
			{
				if( !p_back->n_hi )
				{
					if( !p_begin )
						return this;

					if( dick_p_load-dick_p_alloc == dick_n_alloc )
					{
						GPC_DICK_REC* dick_p_kill = dick_p_alloc;
						dick_p_alloc = new GPC_DICK_REC[dick_n_alloc+0x10];
						if( !dick_p_alloc )
						{
							dick_p_alloc = dick_p_kill;
							return this;
						}
						gp_memcpy( dick_p_alloc, dick_p_kill, dick_n_alloc*sizeof(*dick_p_alloc) );
						dick_p_load = dick_p_alloc+dick_n_alloc;
						dick_n_alloc += 0x20;
						I8 b = p_back-dick_p_kill;
						p_back = dick_p_alloc+b;
						GPM_DELARRAY(dick_p_kill);
					}
					
					p_back->n_hi = dick_p_load-dick_p_alloc;
					p_find = dick_p_load;
					goto rec;
				}

				p_back = dick_p_alloc + p_back->n_hi;
				continue;
			} 

			if( !p_back->n_lo )
			{
				if( !p_begin )
					return this;

				if( dick_p_load-dick_p_alloc == dick_n_alloc )
				{
					GPC_DICK_REC* dick_p_kill = dick_p_alloc;
					dick_p_alloc = new GPC_DICK_REC[dick_n_alloc+0x10];
					if( !dick_p_alloc )
					{
						dick_p_alloc = dick_p_kill;
						return this;
					}
					gp_memcpy( dick_p_alloc, dick_p_kill, dick_n_alloc*sizeof(*dick_p_alloc) );
					dick_p_load = dick_p_alloc+dick_n_alloc;
					dick_n_alloc += 0x20;
					I8 b = p_back-dick_p_kill;
					p_back = dick_p_alloc+b;
					GPM_DELARRAY(dick_p_kill);
				}

				p_back->n_lo = dick_p_load-dick_p_alloc;
				p_find = dick_p_load;
				goto rec;
			}
			// aszondja van balra
			p_back = dick_p_alloc + p_back->n_lo;
		}
		return this;
	rec:
		if( !p_find )
			return this;

		//p_find = dick_p_load;
		p_find->p_str = p_begin;
		p_find->p_rec = p_word;
		p_find->p_rec[p_find->n_rec = n_word] = 0;
		p_find->n_lo = p_find->n_hi = 0;
		p_find->n_id = dick_p_load-dick_p_alloc;
		dick_p_load++;
		return this;
	}
};



//struct InstanceData
//{
//	XMFLOAT3 pos;
//};

class GPC_GPUx64
{
public:
	I8	gpu_id, n_bgra,
		sdw_id, bgra_id,
		sdw_vr, bgra_vr;
	char	s_name[0x100], a_nn[0x10],
		*p_error, a_map[8];

	GPE_ALFA	alf_name;

	ID3D11VertexShader	*p_vs;		// the vertex shader


	ID3D11GeometryShader	*p_gs;
	ID3D11ShaderResourceView* p_gs_rsv;
	//std::vector<InstanceData> treeBillboardInstanceData(numTrees);
	int num2draw;
	ID3D11Buffer* p_ibuff;
	ID3D11Buffer* p_vbuff;


	ID3D11PixelShader	*p_ps;		// the pixel shader
	ID3D11InputLayout	//*p_lay_old,
			*p_lay_cell,
			*p_lay_VOX,
			*p_lay_VX32C,
			*p_lay_VX32CU,
			*p_lay_VX32CU_inst,
			*p_lay_VX32CUFBI,
			*p_lay_VX32CUFBI_inst;
	char		*p_src;

	GPC_GPUx64( I8 gpu_id, char* p_name );
	~GPC_GPUx64();
	ID3D11InputLayout*	get_lay( U4 strd );
};

class GPC_GISx64_ITEM
{
public:
	I8	bone_id, gio_id, n_item_ver,
		*p_child,
		n_child, n_in_mom, n_child_empty;
	
	float	item_sec;
	GPCV3	ypr,
			pos,
			scale,
			reset_ypr,
			reset_pos;

	GPCV4	*p_ypr;
	I8		n_ypr;
	GPCV4	*p_pos;
	I8		n_pos;
	GPCV4	*p_scale;
	I8		n_scale;
	
	GPCMX	mx_reset,		// csontozásnál a reset mx localis
			mxi_reset,
			mx_local,		// localis mx 
			mx_world,		// world = local * mom
			mxi_world,
			mx_draw,
			mx_gx;		// draw = post*world
		
	GPC_GIOx64_CONST	*p_cbuff;


	I8	mx_id,
		gis_mom_id;

	char	s_name[0x20];
	//GPE_ALFA	alf_name;
	GPC_GISx64	*p_gisx64;
	GPC_BBUFF		*p_bbuff;

	HRESULT	hr;

	GPC_GISx64_ITEM(){};
	GPC_GISx64_ITEM( char* _p_name )
	{
		GPM_CLR;
		//p_name = GPM_STR( _p_name );
		U4 n = GPD_STRLEN(_p_name);
		if( n > 0x1f )
			n = 0x1f;
		gp_memcpy( s_name, _p_name, n );
		//s_name[0x1f] = 0;
		gio_id = gis_mom_id = -1;
	}
	GPC_GISx64_ITEM* gis_item_init( GPC_GISx64* p_gis, char* _p_name )
	{
		if( !this )
			return NULL;
		//GPM_CLR;
		//GPM_DELARRAY( p_name );
		//p_name = GPM_STR( _p_name );

		U4 n = GPD_STRLEN(_p_name);
		if( n > 0x1f )
			n = 0x1f;
		gp_memcpy( s_name, _p_name, n );
		s_name[0x1f] = 0;

		gio_id = gis_mom_id = -1;
		p_gisx64 = p_gis;
		return this;
	}
	~GPC_GISx64_ITEM( void )
	{
		//GPM_DELARRAY( p_name );
		GPM_DELARRAY( p_child );
	}

	GPC_GISx64_ITEM* item_join( GPC_GISx64* _p_gis, I8 target_id, bool b_con = false );
	//GPC_GISx64_ITEM* item_add( I8 _id );
	void	item_refresh( GPC_GIOx64_CONST* p_cb );
	void	item_sub( void );
};

class GPC_GISx64_CONST
{
public:
	GPCMX	mx_light,
			mx_world,
			mx_view,
			mx_proj,
			mx_ss_wvp,
			mx_ls_wvp;
	GPCV4	msec_off,
			sun_xyz;
	
	U2		b_upd, b_intl;
	U4		size;
	I8		lightmap_id;
	HRESULT		hr;
	D3D11_USAGE	Usage;

	void reset( void )
	{
		b_upd = true;
		size = GPM_OFF( GPC_GISx64_CONST, b_upd ); // (char*)(&b_upd) - (char*)this;
		b_intl = GPD_CBUFF;
	}
	void q_irq( void )
	{
		if(!this)
			return;
		b_upd = true;
		b_intl = GPD_CBUFF;
	}
	ID3D11Buffer* upld( ID3D11Device* p_dev, ID3D11DeviceContext* p_contx, ID3D11Buffer* p_dx_c_gis )
	{
		ID3D11Buffer* p_dx_c_gis_kill = p_dx_c_gis;
		D3D11_BUFFER_DESC bDSC;
		gp_zero( &bDSC, sizeof(bDSC) );
		bDSC.ByteWidth = size;
		

		if( b_intl || !p_dx_c_gis )
		{
			D3D11_SUBRESOURCE_DATA iniDT;
			iniDT.pSysMem = this;
			iniDT.SysMemPitch = 0;
			iniDT.SysMemSlicePitch = 0;

			GPM_RELEASE( p_dx_c_gis );

			bDSC.Usage = D3D11_USAGE_DEFAULT;			// D3D11_USAGE_STAGING;// D3D11_USAGE_IMMUTABLE; //D3D11_USAGE_DEFAULT;//    D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
			bDSC.BindFlags = D3D11_BIND_CONSTANT_BUFFER;		// use as a vertex buffer
			hr = p_dev->CreateBuffer( &bDSC, &iniDT, &p_dx_c_gis );
		} else {
			bDSC.Usage = D3D11_USAGE_DYNAMIC;
			bDSC.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bDSC.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			if( Usage != bDSC.Usage )
				GPM_RELEASE( p_dx_c_gis );
			if( !p_dx_c_gis )
				hr = p_dev->CreateBuffer( &bDSC, NULL, &p_dx_c_gis );
			
			D3D11_MAPPED_SUBRESOURCE ms;
			hr = p_contx->Map( p_dx_c_gis, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
			if( !FAILED(hr) )
			{
				if( size <= ms.DepthPitch )
					gp_zero( (char*)gp_memcpy(ms.pData, this, size )+size, ms.DepthPitch-size ); //gp_memcpy(ms.pData, this, size );
				else 
					gp_memcpy(ms.pData, this, ms.DepthPitch );
				p_contx->Unmap( p_dx_c_gis, NULL );
			}
		}
		Usage = bDSC.Usage;
		b_upd = false;
		return p_dx_c_gis;
	}
};
class GPC_CNLx64
{
	public:
		GPCV2	*p_key, *p_load;
		I8	n_key;
	~GPC_CNLx64()
	{
		GPM_DELARRAY( p_key );
	}

	float GPC_CNLx64_find( float sec )
	{
		if( !this )
			return 0.0f;
		
		if( !p_key )
			return 0.0f;
		
		if( p_key >= p_load )
			return 0.0f;

		if( sec >= p_load[-1].y )
			return p_load[-1].x;

		GPCV2 *p_hi = p_key;
		while( p_hi < p_load )
		{
			if( p_hi->y < sec )
			{
				p_hi++;
				continue;
			} 

			if( p_hi->y > sec )
				break;

			// ilyenkor pontosan megyegyzik a key idõ a seckel
			return p_hi->x;
		}

		if( p_hi == p_key )
			return p_key->x;

		GPCV2	prev =  p_hi[-1],
			next = *p_hi; 
		
		next -= prev;
		sec -= prev.y;
		return next.x*sec/next.y+prev.x;
	}
};



enum GPE_WHAM
{
	GPE_WHAM_EMPTY,
	GPE_WHAM_MINE,
	GPE_WHAM_BULETT,
};
class GPC_ACTIONx64
{
public:
	float	begin, end, start, mix, fade, gim, loop, long_s;
	I8	begin_ms, end_ms, start_ms, loop_ms, long_ms;
	GPC_ACTIONx64(	float _begin, float _end,
			float _start, float _mix,
			float _fade, float _gim, float _loop )
	{
		begin	= _begin;
		end	= _end;
		start	= _start;
		mix	= _mix;
		fade	= _fade;
		gim	= _gim;
		loop	= _loop;

		long_ms = (I8)((long_s = end-begin)*sec2ms_f);
		begin_ms = (I8)(begin*sec2ms_f);
		end_ms = (I8)(end*sec2ms_f);
		loop_ms = (I8)(loop*sec2ms_f);
		start_ms = (I8)(start*sec2ms_f);
	}
};
class GPC_TRACKx64
{
public:
	char	s_name[0x100];
	
	I8	mom_id, item_id, gio_id, n_level;
	GPC_CNLx64 a_cnl[9];
	GPCV3	a_rst[3];
	void clear( void )
	{
		for( I8 c = 0; c < 9; c++ )
		{
			GPM_DELARRAY( a_cnl[c].p_key );
			a_cnl[c].n_key = 0;
		}
		GPM_CLR;
	}
};
class GPC_GIMx64
{
public:
	char	s_file[GPD_MAX_PATH],
		*p_name;
	GPC_WBx64	*p_wb;

	GPC_TRACKx64	*track_p_alloc,
			*track_p_load;
	I8		track_n_alloc,
			gim_id,
			n_load, n_update;
	
	float		dim, sec1, sec2, lerp;

	GPCMX	*p_mx,
		*p_mxi;

	GPCV3	pos, ypr, scl,
		pos1, ypr1, scl1,
		pos2, ypr2, scl2;

	GPC_GIMx64( GPC_WBx64	*_p_wb, char* p_file )
	{
		GPM_CLR;
		p_wb = _p_wb;

		GPM_STRCPY( s_file, p_file );
		p_name = s_file + GPF_STR_VISZ( s_file, s_file+GPD_STRLEN(s_file), ":\\" );
		if( *p_name == '\\' || *p_name == ':' )
			p_name++;
		dim = 1000.0f;
		sec1 = sec2 = lerp = 0.0f;
	}
	~GPC_GIMx64()
	{
		GPM_DELARRAY( track_p_alloc );
		GPM_DELARRAY( p_mx );
	}
	GPCMX* gim_set( float s1, float s2, float l )
	{
		U4 n = track_p_load-track_p_alloc;
		if( !p_mx )
		{
			p_mx = new GPCMX[n*2];
			gp_zero( p_mx, n*2 );
			p_mxi = p_mx+n;
			for( GPC_TRACKx64 *p_t = track_p_alloc, *p_te = track_p_load; p_t < p_te; p_t++ )
			{
				for( U4 n_para = 12; n_para < 18; n_para++ )
				{
					switch( n_para )
					{
						// ------------ RESET ------------
						// reset_pos
						case 12:
							pos.x = pos2.x = pos1.x = ((float*)p_t->a_rst)[n_para-12];
							break;
						case 13:
							pos.y = pos2.y = pos1.y = ((float*)p_t->a_rst)[n_para-12];
							break;
						case 14:
							pos.z = pos2.z = pos1.z = ((float*)p_t->a_rst)[n_para-12];
							break;
						// reset_ypr
						case 15:
							ypr.x = ypr2.x = ypr1.x = ((float*)p_t->a_rst)[n_para-12];
							break;
						case 16:
							ypr.y = ypr2.y = ypr1.y = ((float*)p_t->a_rst)[n_para-12];
							break;
						case 17:
							ypr.z = ypr2.z = ypr1.z = ((float*)p_t->a_rst)[n_para-12];
							break;
					}
				}
				n = p_t-track_p_alloc;
				p_mx[n].identity();
				p_mx[n].ypr( ypr1 );
				p_mx[n].t3 = pos1;
				if( n != p_t->mom_id )
					p_mx[n] *= p_mx[p_t->mom_id];
				p_mxi[n] = p_mx[n].invert();
			}
			scl = scl2 = scl1.xyz( 1.0 );
		}
		if( s1 == sec1 && s2 == sec2 && l == lerp )
			return p_mx;

		
		for( GPC_TRACKx64 *p_t = track_p_alloc, *p_te = track_p_load; p_t < p_te; p_t++ )
		{
			//I8 mom_id = p_t->mom_id;
			for( U4 n_para = 3; n_para < 18; n_para++ )
			{
				switch( n_para )
				{
					case 3:
						if( l == 0.0f )
						{
							pos.x = p_t->a_cnl[n_para-3].GPC_CNLx64_find( s1 );
							break;
						}
						else if( l == 1.0f )
						{
							pos.x = p_t->a_cnl[n_para-3].GPC_CNLx64_find( s2 );
							break;
						}
						if( s1 != sec1 )
							pos1.x = p_t->a_cnl[n_para-3].GPC_CNLx64_find( s1 );
						if( s2 != sec2 )
							pos2.x = p_t->a_cnl[n_para-3].GPC_CNLx64_find( s2 );
						
						pos.x = ((pos2.x-pos1.x)*l) + pos1.x;
						break;
					case 4:
						if( l == 0.0f )
						{
							pos.y = p_t->a_cnl[n_para-3].GPC_CNLx64_find( s1 );
							break;
						}
						else if( l == 1.0f )
						{
							pos.y = p_t->a_cnl[n_para-3].GPC_CNLx64_find( s2 );
							break;
						}
						if( s1 != sec1 )
							pos1.y = p_t->a_cnl[n_para-3].GPC_CNLx64_find( s1 );
						if( s2 != sec2 )
							pos2.y = p_t->a_cnl[n_para-3].GPC_CNLx64_find( s2 );
						
						pos.y = ((pos2.y-pos1.y)*l) + pos1.y;
						break;
					case 5:
						if( l == 0.0f )
						{
							pos.z = p_t->a_cnl[n_para-3].GPC_CNLx64_find( s1 );
							break;
						}
						else if( l == 1.0f )
						{
							pos.z = p_t->a_cnl[n_para-3].GPC_CNLx64_find( s2 );
							break;
						}
						if( s1 != sec1 )
							pos1.z = p_t->a_cnl[n_para-3].GPC_CNLx64_find( s1 );
						if( s2 != sec2 )
							pos2.z = p_t->a_cnl[n_para-3].GPC_CNLx64_find( s2 );
						
						pos.z = ((pos2.z-pos1.z)*l) + pos1.z;
						break;
					// ypr
					case 6:
						if( l == 0.0f )
						{
							ypr.x = p_t->a_cnl[n_para-3].GPC_CNLx64_find( s1 );
							break;
						}
						else if( l == 1.0f )
						{
							ypr.x = p_t->a_cnl[n_para-3].GPC_CNLx64_find( s2 );
							break;
						}
						if( s1 != sec1 )
							ypr1.x = p_t->a_cnl[n_para-3].GPC_CNLx64_find( s1 );
						if( s2 != sec2 )
							ypr2.x = p_t->a_cnl[n_para-3].GPC_CNLx64_find( s2 );
						
						ypr.x = ((ypr2.x-ypr1.x)*l) + ypr1.x;
						break;
					case 7:
						if( l == 0.0f )
						{
							ypr.y = p_t->a_cnl[n_para-3].GPC_CNLx64_find( s1 );
							break;
						}
						else if( l == 1.0f )
						{
							ypr.y = p_t->a_cnl[n_para-3].GPC_CNLx64_find( s2 );
							break;
						}
						if( s1 != sec1 )
							ypr1.y = p_t->a_cnl[n_para-3].GPC_CNLx64_find( s1 );
						if( s2 != sec2 )
							ypr2.y = p_t->a_cnl[n_para-3].GPC_CNLx64_find( s2 );
						
						ypr.y = ((ypr2.y-ypr1.y)*l) + ypr1.y;
						break;
					case 8:
						if( l == 0.0f )
						{
							ypr.z = p_t->a_cnl[n_para-3].GPC_CNLx64_find( s1 );
							break;
						}
						else if( l == 1.0f )
						{
							ypr.z = p_t->a_cnl[n_para-3].GPC_CNLx64_find( s2 );
							break;
						}
						if( s1 != sec1 )
							ypr1.z = p_t->a_cnl[n_para-3].GPC_CNLx64_find( s1 );
						if( s2 != sec2 )
							ypr2.z = p_t->a_cnl[n_para-3].GPC_CNLx64_find( s2 );
						
						ypr.z = ((ypr2.z-ypr1.z)*l) + ypr1.z;
						break;
					// scale
					case 9:
						if( l == 0.0f )
						{
							scl.x = p_t->a_cnl[n_para-3].GPC_CNLx64_find( s1 );
							break;
						}
						else if( l == 1.0f )
						{
							scl.x = p_t->a_cnl[n_para-3].GPC_CNLx64_find( s2 );
							break;
						}
						if( s1 != sec1 )
							scl1.x = p_t->a_cnl[n_para-3].GPC_CNLx64_find( s1 );
						if( s2 != sec2 )
							scl2.x = p_t->a_cnl[n_para-3].GPC_CNLx64_find( s2 );
						
						scl.x = ((scl2.x-scl1.x)*l) + scl1.x;
						break;
					case 10:
						if( l == 0.0f )
						{
							scl.y = p_t->a_cnl[n_para-3].GPC_CNLx64_find( s1 );
							break;
						}
						else if( l == 1.0f )
						{
							scl.y = p_t->a_cnl[n_para-3].GPC_CNLx64_find( s2 );
							break;
						}
						if( s1 != sec1 )
							scl1.y = p_t->a_cnl[n_para-3].GPC_CNLx64_find( s1 );
						if( s2 != sec2 )
							scl2.y = p_t->a_cnl[n_para-3].GPC_CNLx64_find( s2 );
						
						scl.y = ((scl2.y-scl1.y)*l) + scl1.y;
						break;
					case 11:
						if( l == 0.0f )
						{
							scl.z = p_t->a_cnl[n_para-3].GPC_CNLx64_find( s1 );
							break;
						}
						else if( l == 1.0f )
						{
							scl.z = p_t->a_cnl[n_para-3].GPC_CNLx64_find( s2 );
							break;
						}
						if( s1 != sec1 )
							scl1.z = p_t->a_cnl[n_para-3].GPC_CNLx64_find( s1 );
						if( s2 != sec2 )
							scl2.z = p_t->a_cnl[n_para-3].GPC_CNLx64_find( s2 );
						
						scl.z = ((scl2.z-scl1.z)*l) + scl1.z;
						break;
				}
			}

			if( l == 0.0f || l == 1.0 )
			{
				pos1 = pos2 = pos;
				ypr1 = ypr2 = ypr;
				scl1 = scl2 = scl;
			}
		}

		sec1 = s1; 
		sec2 = s2;
		lerp = l;
	}


};

class GPC_BULLET
{
public:
	I8		start_ms, end_ms,
			killer_id,
			prey_id;

	GPE_NET	bul_id;
	
	GPCI4	pos1, pos2,
			up, sun_xyz, prey_gr;
	I8		prey_idx;

	// send ------------------------
	I8			gio_id;
	GPCMX		mxw;
	GPCV4		v4killer,
				v4time;
	GPC_GX_ITM	*p_grav;
	GPC_BULLET( void )
	{
		GPM_CLR;
	}
	I8 bul_end_ms( I8 gx_ms, float speed = 12.0f )
	{
		gx_ms += sec2ms;
		I8 bullet_road = (v4killer = (pos1-pos2).xyz_v4()).len_xyz();
		float road_sec = (float)bullet_road/(speed*GPD_GX_HUMAN_1m);
		if( road_sec > 20.0f )
		{
			v4killer *= 20.0f;
			v4killer /= road_sec;
			v4killer.w = 0;
			pos2 = pos1-v4killer.i_xyz();

			v4killer = (pos1-pos2).xyz_v4();
			bullet_road = v4killer.len_xyz();
			road_sec = (float)bullet_road/(speed*GPD_GX_HUMAN_1m);
		}
		float roadNbumm_sec = road_sec+GPD_BUM_S;
		end_ms = (start_ms = gx_ms) + roadNbumm_sec*sec2ms;
		return end_ms;
	}
};

class GPC_BBUFF
{
public:
	GPC_BULLET* p_buff;
	I8	n, e, i, n_fly, ms;
	GPC_BBUFF( void )
	{
		GPM_CLR;
	}
	GPC_BULLET* bulet_add( I8 s_ms, I8 e_ms )
	{
		if( !s_ms )
			return NULL;
		if( s_ms >= e_ms )
			return NULL;

		for( e; e < n; e++ )
		{
			if( !p_buff[e].start_ms )
				break;
		}

		if( e >= n )
		{
			I8 n_kill = n;
			n += 0x10;
			GPC_BULLET* p_kill = p_buff;
			p_buff = new GPC_BULLET[n];
			if( p_kill )
			{
				gp_memcpy( p_buff, p_kill, n_kill*sizeof(*p_kill) );
			}
			memset( p_buff+e, 0, (n-e)*sizeof(*p_buff) );
			GPM_DELARRAY(p_kill);
		}
		GPC_BULLET* p_out = p_buff+e;
		p_out->start_ms = s_ms;
		p_out->end_ms = e_ms;
		p_out->gio_id = -1;
		return p_out;
	}

	GPC_BULLET* bulet_run( I8 gx_ms )
	{
		if( i >= n )
		{
			i = 0;
			return NULL;
		}

		if( !i )
			n_fly = 0;

		for( ; i < n; i++ )
		{
			if( p_buff[i].start_ms < sec2ms )
				continue;

			if( gx_ms < p_buff[i].end_ms )
			{
				n_fly++;
				GPC_BULLET* p_out = p_buff+i;
				i++;
				return p_out;
			}

			if( e > i )
				e = i;
			
			p_buff[i].start_ms = 0;
		}
		return NULL;
	}

};
class GPC_GX_TRG
{
	public:
		union
		{
			struct
			{
				// I8 1,2, 
				I8 src_id, dst_id;
				// I8 3,
				U4	DRPq, MINq,
					gx_s4;	// 4 másodperc			
							// I8 4
				GPE_ACTION	act;
				U1			ixIL, ixIR, n_op; // OPcode 1-one 2-half 3-all

				GPCI4	SRCld,
						DRPld,
						MINld,
						TRGld;
			};
			struct
			{
				GPCI4 a_TRG[5];
			};
		};
		GPC_GX_TRG(){};
};
class GPC_GX_LOD
{
public:
	GPCI4	*p_alloc;
	I8		n_load,
			n_alloc,
			n_RADar;

	GPCI4	EYEld, EYEdr, eye_sun, eye_gravi, eye_light, eye_play, ALTld,
			lod,
			player_idx,
			PLYld,
			RIGdr,
			ONEld,
			star_idx,
			star_xyz,
			a_RADgr[4],
			a_RADar[4];

	I8	play_id, store_qc, sit_qc,
		sel_qc, drp_qc, fire_qc,
		sel_ix, drp_ix, 
		sel_ms, drp_ms, fir_ms,
		gravi_id,
		gravi_r,
		lightmap_id;
	GPC_BULLET	bullet, puff;
	char	s_lihtmap[0x100];
	GPCI4	src_pos, dst_pos;

	GPE_ALFA	mod_id;
	GPCV3	up;

	GPC_IIx64	*p_ii, *p_iie;
	I8			n_ii;

	GPC_COLOR4*	a_store[0x30];	// 0 left // 0x10 right // 0x20 wb
	I8			store_ver;

	GPCI4		store_left, store_right, lod_go;

	GPC_GX_TRG	TRG;
	GPC_FIX		*p_info;

	bool		b_fly;

	GPC_GX_LOD( void )
	{
		GPM_CLR;
		store_left.x = 2;
	}
	~GPC_GX_LOD()
	{
		GPM_DELARRAY(p_alloc);
	}
	I8 lod_reset(void)
	{
		n_load = 0;
		lod.y = GPD_GX_PLANET_1E;
		lod.z = GPD_GX_SOLARSYS_R;
		lod.w = GPD_GX_SOLARSYS_D<<1;
		player_idx.x = -1;
		//store_right = store_left.null();
		//store_left.x = 2;
		return n_alloc;
	}
	void lod_add( GPCI4& i4 )
	{
		if( n_load >= n_alloc )
		{
			GPCI4* p_kill = p_alloc;
			I8 n_kill = n_alloc;
			n_alloc = n_load+0x10;
			p_alloc = new GPCI4[n_alloc];
			if( p_kill )
				gp_memcpy( p_alloc, p_kill, n_load*sizeof(*p_alloc));
			if( n_kill > n_load )
				memset(p_alloc+n_kill, 0, (n_load-n_kill)*sizeof(*p_alloc));
			GPM_DELARRAY( p_kill );
		}
		p_alloc[n_load] = i4;
		n_load++;
	}
	void lod_median( I8 level )
	{
		if( n_load < 1 )
			return;

		if( n_alloc >= n_ii )
		{
			GPM_DELARRAY( p_ii );
			n_ii = n_alloc+0x10;
			p_ii = new  GPC_IIx64[n_ii];
		}
		if( !p_ii )
			return;
		
		p_iie = p_ii;
		for( I8 i = 0; i < n_load; i++ )
		{
			if( p_alloc[i].w < level )
				continue;

			p_iie->i = p_alloc[i].x;
			p_iie->vi = p_alloc[i].z;
			p_iie++;
		}

		p_ii->median( p_iie-p_ii, true );
	}
};

class GPC_GX_MINE
{
	public:
		I8	gx_id, gx_idx, n_pos;
		GPCI4	pos_0x0,
				pos_0x1,
				a_pos[0x200];

		GPE_STF	a_histi[0x100];
		char	n_a27;
	GPC_GX_MINE( void )
	{
		GPM_CLR;
		n_a27 = -1;
	}
};

class GPC_GISx64
{
public:
	char	s_file[_MAX_PATH],
		*p_name,
		s_screen[0x100],
		s_deep[0x100],
		s_shadow[0x100];

	I8	id,
		screen_id,
		deep_id,
		shadow_id,
		hud_id,
		n_ver,
		n_rndr,
		n_q4; //, n_q44;

	GPCV3	light_pos2, light_pos,
			light_trg2, light_trg,
			light_up,
			cam_pos,
			cam_trg,
			cam_up;

	GPC_GX_LOD* p_lod;

	GPC_GISx64_CONST mx_cbuff;
	
	GPC_GISx64_ITEM	*p_alloc, *p_gis_itm;
	I8				n_alloc, boss_id;

	GPC_WBx64	*p_wb;
	bool	b_use, b_clr, b_gx;
	HRESULT	hr;

	bool				gis_b_updt;
	GPC_GIO_BUFF_VX324	gis_mini_vx,
					gis_line_vx,
					gis_solid_vx,
					gis_hud_line_vx,
					gis_hud_mini_vx;

	// LINE
	ID3D11Buffer	*gp_p_dx_line_vx;
	ULL			gp_n_dx_line_vx,
				gp_n_dx_line_load;

	ID3D11Buffer	*gp_p_dx_line_idx;
	ULL			gp_n_dx_line_idx,
				gp_n_dx_line_idx_load;

	// MINI
	ID3D11Buffer	*gp_p_dx_mini_vx;
	ULL			gp_n_dx_mini_vx,
				gp_n_dx_mini_load;

	// HUD
	ID3D11Buffer	*gp_p_dx_hud_line_vx;
	ULL			gp_n_dx_hud_line_vx,
				gp_n_dx_hud_line_load;

	GPC_GISx64(){};
	GPC_GISx64( GPC_WBx64	*_p_wb, char* p_file, GPCV3 c_pos, GPCV3 c_trg, GPCV3 c_up )
	{
		GPM_CLR;
		p_wb = _p_wb;
		mx_cbuff.reset();

		GPM_STRCPY( s_file, p_file );
		p_name = s_file + GPF_STR_VISZ( s_file, s_file+GPD_STRLEN(s_file), ":\\" );
		if( *p_name == '\\' || *p_name == ':' )
			p_name++;
		cam_pos = c_pos;
		light_trg = cam_trg = c_trg;
		light_pos = cam_up = c_up;
		
	}
	void clr_vx( void )
	{
		if( !this )
			return;

		gis_mini_vx.VX324_reset();
		gis_line_vx.VX324_reset();
		gis_solid_vx.VX324_reset();
		gis_b_updt = true;
	}
	void clr( void )
	{
		GPM_CLR;
	}
	GPC_GISx64_ITEM* item_find_name( I8 mom_id, char* _p_name )
	{
		if( !_p_name )
			return NULL;
		if( !*_p_name )
			return NULL;
		for( I8 i = 0; i < n_alloc; i++ )
		{
			if( p_alloc[i].gis_mom_id == mom_id )
			//if( p_alloc[i].p_name )
			if( !strstr( p_alloc[i].s_name, _p_name ) )
				return p_alloc+i;
		}
		return NULL;
	}
	GPC_GISx64_ITEM* item_find( I8 find_id )
	{
		if( find_id < 0 )
			return NULL;

		if( !p_alloc )
			return NULL;

		if( n_alloc <= find_id )
			return NULL;

		return p_alloc+find_id;
	}
	
	GPC_GISx64_ITEM* item_add( I8 _id );
	void gis_item( GPC_RES* p_res );
	void gis_render( I8 n_rnd );

	GPC_GISx64* gis_gx_cam( GPC_GX_LOD& lod, I8 off = 0, bool b_clear = true )
	{
		if( !this )
			return NULL;
		
		p_lod = &lod;

		cam_pos.null();
		cam_trg = lod.EYEdr.xyz_v3();	// EYEdr = lod_pos + word.z - w_cam
		cam_up = lod.up;
		mx_cbuff.q_irq();
		b_gx = b_use = true;

		if( b_clear )
			clr_vx();

		GPCV2 ccd( 4.8,3.6 );
		//mx_cbuff.mx_proj.proj( ccd, 17.5, 8, (cam_trg-cam_pos).len()*6, 16000 );
		mx_cbuff.mx_proj.proj( ccd, 17.5, 8, cam_trg.len()*6, 16000 );
		mx_cbuff.mx_view.lookat( cam_pos, cam_trg, cam_up );
		mx_cbuff.mx_world.identity();
		// itt mocorgatjuk a kamerát
		mx_cbuff.msec_off.w = n_q4 = off;
		//mx_cbuff.mx_view.t3 += (
		//			  mx_cbuff.mx_view.x3*((float(off%2)-0.5f)/640.0)
		//			+ mx_cbuff.mx_view.y3*((float(off/2)-0.5f)/256.0)
		//		);
		mx_cbuff.mx_ss_wvp = mx_cbuff.mx_view*mx_cbuff.mx_proj;

		mx_cbuff.sun_xyz.xyz( lod.eye_sun );	// eye_sun	  = lod_sun  - EYEld 
		light_pos	= lod.eye_light.xyz_v3();	// eye_light = eye_sun  - eye_gravi    
		light_trg	= lod.eye_gravi.xyz_v3();	// eye_gravi = lod_grav - EYEld
		light_pos2	= lod.eye_play.xyz_v3();	// eye_play = (POSld - EYEld) - eye_gravi;

		GPCV3	light_z = light_pos-light_trg;
		float	lz = light_z.len(),
				vz = GPE_BASE_FULL*GPD_3D_defDIM;
		
		light_z /= lz;
		light_pos = light_trg+(light_z*vz);
		
		GPCV3	t2p = (lod.eye_play-lod.eye_gravi).xyz_v3(),		// eye_play = (POSld - EYEld) - eye_gravi;
				g2t2 = t2p - (light_z*(light_z*t2p)),
				lp = g2t2+light_pos,
				lt = g2t2+light_trg,
		light_up = GPCV3( 0.0, 1.0, 0.0 );
		GPCMX mx;
		mx_cbuff.mx_light = mx.lookat( lp, lt, light_up );
		mx_cbuff.lightmap_id = lod.lightmap_id;
		
		return this;
	}

	I8		gis_lws_load( char* p_file, float dim );
	GPC_GX_ITM*	gs_lod_gx_add( GPC_GX_MINE& gx_mine, I8& id, GPC_GX_ITM& gx_itm, I8 gx_ms, GPC_GX_ITM* p_alloc, GPC_IIx64* p_ii, GPC_IIx64* p_iie, GPC_GX_LOD* p_send );
	void		gx_lod_post( GPC_GX_MINE& gx_mine, GPC_GX_ITM& gx_itm, I8 gx_ms, GPC_GX_ITM* p_alloc, GPC_IIx64* p_ii, GPC_IIx64* p_iie );
	void		gx_lod_HUD( 
					GPC_GX_ITM* p_gravy,
					GPC_GX_MINE& gx_mine, GPC_GX_ITM& gx_veh,
					GPC_GX_ITM& gx_player, GPC_GX_ITM* p_alloc,
					I8 gx_ms, GPC_IIx64* p_ii, GPC_IIx64* p_iie,
					bool b_walk 
				);
	void		gx_lod_HUD_flat( 
					GPC_GIO_BUFF_VX324* p_line_vx,
					GPC_GIO_BUFF_VX324* p_mini_vx,
					GPC_GX_ITM* p_gravy,
					GPC_GX_MINE& gx_mine, GPC_GX_ITM& gx_veh,
					GPC_GX_ITM& gx_player, GPC_GX_ITM* p_alloc,
					I8 gx_ms, GPC_IIx64* p_ii, GPC_IIx64* p_iie,
					bool b_walk 
				);
	void		gx_lod_HUD_new( 
					GPC_GIO_BUFF_VX324& line_vx, GPC_GIO_BUFF_VX324& mini_vx, 
					GPC_GX_ITM* p_gravy,
					GPC_GX_MINE& gx_mine, GPC_GX_ITM& gx_veh,
					GPC_GX_ITM& gx_player, GPC_GX_ITM* p_alloc,
					I8 gx_ms, GPC_IIx64* p_ii, GPC_IIx64* p_iie,
					bool b_walk 
				);
	void		gx_lod_HUD_roll_lr(
					GPC_GIO_BUFF_VX324& mini_vx, 
					int cx, int cy, int cw, int ch, char* p_str, char* p_end, GPCV3 pos, GPCV3 right, GPCV3 down 
					);
};

//----------------------------------------
//
//		GALAXY
//
//----------------------------------------
class GPC_GX_I8
{
public:
	I8	n_idx_add,
		n_idx_sub,
		*p_idx_alloc,
		n_idx_empty,
		n_idx_alloc,
		pad0;
	GPC_GX_I8( void )
	{
		GPM_CLR;
	}
	~GPC_GX_I8()
	{
		GPM_DELARRAY(p_idx_alloc);
		GPM_CLR;
	}
	GPC_GX_I8& operator = ( GPC_GX_I8& item )
	{
		I8 diff = n_idx_alloc-item.n_idx_alloc;
		if( diff < 0 || diff > 0x10 )
		{
			GPM_DELARRAY( p_idx_alloc );
			p_idx_alloc = new I8[item.n_idx_alloc];
			if( diff )
				memset( p_idx_alloc+item.n_idx_alloc, -1, diff );
		}
		if( p_idx_alloc )
		{
			n_idx_alloc = item.n_idx_alloc;
			n_idx_empty = item.n_idx_empty;
			gp_memcpy( p_idx_alloc, item.p_idx_alloc, n_idx_alloc*sizeof(*p_idx_alloc) );
		} else {
			GPM_CLR;
		}
		return *this;
	}
	bool i8_add( I8 idx )
	{
		if( idx < 0 || !this )
			return false;

		for( I8 i = n_idx_empty; i < n_idx_alloc; i++ )
		{
			if( p_idx_alloc[i] == idx )
				return true;

			// korábban talált üres helyet
			if( n_idx_empty >= n_idx_alloc )
				n_idx_empty = i;
		}

		if( n_idx_empty >= n_idx_alloc )
		{
			n_idx_empty = n_idx_alloc;

			I8* p_kill = p_idx_alloc;
			
			p_idx_alloc = new I8[n_idx_alloc+0x10];
			if( p_kill )
				gp_memcpy( p_idx_alloc, p_kill, n_idx_alloc*sizeof(*p_idx_alloc) );
			
			memset( p_idx_alloc+n_idx_alloc, -1, 0x10*sizeof(*p_idx_alloc));
			GPM_DELARRAY( p_kill );
			n_idx_alloc += 0x10;
		}
		n_idx_add++;
		p_idx_alloc[n_idx_empty] = idx;
		n_idx_empty++;
		return true;
	}
	bool i8_sub( I8 idx )
	{
		if( !this )
			return false;

		for( I8 i = 0; i < n_idx_alloc; i++ )
		if( p_idx_alloc[i] == idx )
		{
			p_idx_alloc[i] = -1;
			n_idx_empty = i;
			n_idx_sub++;
			return true;
		}

		return true;
	}
};
class GPC_GX_BUY
{
public:
	GPCI4	posNr, drp_gr,
			form_nXstuff_to,
			time4;
	GPC_GX_BUY(void)
	{
		GPM_CLR;
	}
	GPC_GX_BUY& gx_buy_clr( void )
	{
		GPM_CLR;
		return *this;
	}
	GPC_FIX* gx_buy_join( GPC_FIX* p_out )
	{
		if( !this )
			return p_out;

		ULL n_begin = p_out ? p_out->fix_n_alloc : 0;
		p_out = p_out->fix_format( "GXITPACK00000000" );
		
		p_out->pack_iv4( "GXBUYPNR", posNr );
		p_out->pack_iv4( "GXBUYGRN", drp_gr );
		p_out->pack_iv4( "GXBUYFTO", form_nXstuff_to );
		p_out->pack_iv4( "GXBUYTIM", time4 );

		*(ULL*)&(p_out->fix_p_alloc[n_begin+8]) = p_out->fix_n_alloc-n_begin;
		return p_out;
	}
};


class GPC_TXTx64
{
public:
	char		s_name[0x100],
			s_txt_buff[0x4000];
	GPC_WBx64		*p_wbx64;
	I8		txt_id;
	GPC_COLOR4		*p_txt_buff,
			*p_txt_ref,
			txt_color;
	int		txt_w, txt_h, txt_size;
	GPCI4		txt_xywh,
			STFxyBIGzw;
	
	GPC_TXTx64( GPC_WBx64* p_wb, char* p_name, int w, int h )
	{
		GPM_CLR;
		p_wbx64 = p_wb;
		txt_w = w;
		txt_h = (h/2)*3;
		txt_size = txt_w*txt_h;
		txt_id = -1;

		STFxyBIGzw = GPCI4( 0, (txt_h*2)/3, txt_w/4, (txt_h*2)/3 );
		GPM_STRCPY( s_name, p_name );
	}
	GPCI4 gpc_txt_clr( bool b_null )
	{
		if( this ? !gpc_txt_chk() : true )
			return GPCI4(0,0,0,0);
		
		txt_color = GPC_COLOR4( 0xffffffff );
		gp_zero( p_txt_buff, txt_size*sizeof(*p_txt_buff) );
		if( b_null )
			txt_xywh.null();
		return txt_xywh;
	}
	bool gpc_txt_chk( void );
	bool gpc_txt_ready( void );
	GPCI4 gpc_printf( char *p_format, ... );
	GPC_COLOR4* gpc_txt_txt( I8 x, I8 y )
	{
		txt_xywh.null();
		return p_txt_add( x, y ); 
	}
	GPC_COLOR4* gpc_txt_stf( I8 x, I8 y )
	{
		txt_xywh = STFxyBIGzw.xy();
		return p_txt_add( x, y ); 
	}
	GPC_COLOR4* gpc_txt_big( I8 x, I8 y )
	{
		txt_xywh = STFxyBIGzw.zw__();
		return p_txt_add( x, y ); 
	}
	GPC_COLOR4* p_txt_add( I8 x, I8 y )
	{
		if( !p_txt_buff )
			return NULL;
		txt_xywh.x += x;
		txt_xywh.y += y;
		return p_txt_buff+txt_xywh.x+txt_xywh.y*txt_w;
	}
};

GPE_ALFA GPF_ACT2ALFA( GPE_ACTION i );
I8 GPF_ACT_long_ms( U1 act );

class GPC_GX_STP
{
public:
	GPCI4	A_prev,
			a_step[2];
	GPCV3	z_prev, z;
	I8		ms, long_ms, n_break;
	GPC_AN	act_an;
	U4		snd_cnt;
	//GPCI4	dzy2( U1 drh );
	GPC_GX_STP()
	{
		GPM_CLR;
	}
};
class GPC_GX_ITM
{
public:
	char	s_gx_name[0x1f],nn,
		s_pasw[0x1f],np;
	
	// gp_memcpy 0
	I8	born_t,
		fuel,

		exp,pay;

	GPE_ALFA	type_id;
	I8	id;
	GPCI4	star_xyz;
	I8	mom_id;
	// gp_memcpy 0

	// gp_memcpy 1
	GPCI4	AA,B;
	I8	AB_base_ms, AB_turn_s, AB_over_ms;
	// gp_memcpy 1

	// gp_memcpy 2
	GPCV3	y,z;			//if( AB_turn_s ) ? globalis : lokális; 
	I8	yz_base_ms;
	GPCV3	gx_ypr_s;
	// gp_memcpy 2
	
	// gp_memcpy 3
	I8	size_r;
	GPE_ALFA	act_alfa;
	I8	act_base_ms,
		a_gio[0x8]; // hair,head(race),hand,jacket,trouset,shoes,?,stuff
	// gp_memcpy 3
	

	// ---------------------------------------------------------- ---  --   -
	// ------ BEG... TEMPORARY --------------------------------
	// --------------------------
	// tab_row_pack

	I8	n_add_idx;
	I8	n_mom_idx,
		n_glue_idx;

	GPC_GALAXY* p_gx;
	GPC_GX_I8	child;
	GPC_FIX*	p_star_idx;

	GPCI4	gx_lod, gx_send,
			A_prev,	//local_prev,
			in_prev;
		
	GPC_GX_STP	*p_step;
	
	
	I8	b_updt,
		gx_n_build;

	GPC_CRS*	gx_p_crs;
	char	*p_user_path;

	GPCMX	mx_world,
		mxi_w;

	GPE_ALFA	alf_name;
	
	int	LIGid, LIGq;
	
	I8	mx_ms, mxi_ms,
		ab_turn_zero_ms,
		push_ms,  release_ms,
		lod_t, send_t,
		gim_id, gim_aid, gen_id, gx_main_id,
		gx_stuff_id, gx_stuff_gis_id,
		play_t, lod_sit,
		gio_id, lod_store_ver,
		store_src_z, store_dst_z, go_yaw;


	double	press, cos_tng, perimeter;
	GPCI4	store_src_dst;
	GPC_FIX	*p_store,
			*p_xprss;

	GPC_GIVLx64	*p_givl;
	GPC_GIVx64	*p_giv;	
	GPC_BBUFF		*p_bullet_buff;
	//GPC_GIOx64_CONST	*p_gx_giocb;

	// --------------------------
	// ------ END... TEMPORARY --------------------------------
	// ---------------------------------------------------------- ---  --   -
	
	// out --------------------------------
	char	lod_s_name[0x1f],lod_nn,
		lod_s_pasw[0x1f],lod_np;
	
	// gp_memcpy 0
	I8	lod_born_t,
		lod_fuel,

		lod_take, lod_drop;

	GPE_ALFA	lod_type_id;
	I8	lod_id;
	GPCI4	lod_cam_vec;				// lod_A-lod.cam // star_xyz
	I8	lod_mom_id;
	// gp_memcpy 0

	// gp_memcpy 1
	GPCI4	POSld, prvPOSld;			// A B
	I8		lod_AB_base_ms, lod_AB_turn_s, lod_AB_over;
	// gp_memcpy 1

	// gp_memcpy 2
	GPCV3	world_y,world_z;
	I8	lod_yz_base_t;
	GPCV3	lod_ypr;
	// gp_memcpy 2
	
	// gp_memcpy 3
	I8	lod_size_r,
		lod_act_id,
		lod_act_base_t,
		lod_a_gio[0x8]; // hair,head(race),hand,jacket,trouset,shoes,?,?
	// gp_memcpy 3
	GPCMX& mxi_world( void )
	{
		if( mxi_ms < mx_ms )
		{
			mxi_ms = mx_ms;
			mxi_w = mx_world.invert();
		}
		return mxi_w;
	}
	
	GPC_FIX* gx_itm_idx_N_sys_write( GPC_FIX* p_out, GPC_GX_ITM* p_alloc, U8& s, GPC_CRS* p_pub, GPC_AN& an );
	GPC_FIX* gx_itm_join( GPC_FIX* p_out, GPC_GX_ITM* p_alloc, bool b_clies, GPC_WBx64* p_wb, bool b_down, bool b_3d );
	
	long double gx_itm_rad( I8 gx_ms, long double mul = PI2 )
	{
		if( this ? !AB_turn_s : true )
			return 0.0;

		I8	AB_turn_ms = AB_turn_s*sec2ms,
			ab_tms = (gx_ms - AB_over_ms - AB_base_ms )%AB_turn_ms;

		//long double ab_tld = ((long double)lod_AB_turn_s)/AB_turn_ms;
		long double ab_tld = ((long double)ab_tms)/AB_turn_ms;
		return ab_tld * mul;
	}

	GPC_GX_ITM&	gx_itm_AB_new( GPC_GX_ITM& out, I8 gx_ms, double press, double mul, GPC_GX_ITM* p_mom, bool b_walk = false, double press2 = 0.0  );
	bool		gx_itm_stp_turn( GPC_GALAXY* p_gx, U4 tq, I8 ms );
	bool		gx_itm_stp( GPC_GALAXY* p_gx, GPCI4* p_step, GPE_GT_CnS b_CnS );
	
	GPCI4&		gx_itm_pos( I8 gx_ms, GPC_GX_ITM* p_mom );
	GPCMX&		gx_itm_ypr( I8 gx_ms, GPC_GX_ITM* p_mom );
	void		gx_itm_init( void );
	GPC_GX_ITM*	gx_itm_DOWN( GPC_GX_ITM* p_alloc, GPE_ALFA type );
	GPC_GX_ITM*	gx_itm_GRAVI( GPC_GX_ITM* p_alloc );
	void  __vectorcall	gx_itm_BLD( GPC_GX_LOD* p_lod, GPC_GX_LOD* p_send, GPC_GX_ITM* p_alloc, I8 gx_ms, GPC_GX_ITM* p_mom );
	
	GPC_COLOR4*	gx_itm_STORE( GPC_WBx64* p_wb, GPC_GX_LOD& lod );

	bool __vectorcall	gx_itm_PLAY( GPCI4 go, bool b_jump, I8 gx_ms, I8 gx_et_ms, GPC_GX_ITM* p_mom, GPC_GX_ITM* p_alloc, bool b_walk = false );
	void		gx_itm_PLAY2( GPCI4 go, bool b_jump, I8 gx_ms, GPC_GX_ITM* p_mom, GPC_GX_ITM* p_alloc );
	GPCI4&	gx_itm_in_xyz( GPC_GX_ITM* p_boss ); //, I8 front_n = 0);
	void		gx_itm_BUSINESS(  GPC_GALAXY* p_gx, GPC_GX_ITM* p_player, GPCI4 fnto, I8 gx_ms );
	void		gx_itm_ADD( GPC_GALAXY* p_gx, GPCI4 posNr, GPESTF stuff );
	void		gx_itm_BUMM(  GPC_GALAXY* p_gx, GPC_GX_ITM* p_player, I8 stf_ix, I8 gx_ms, U4 DRPq, U4 MINq ); 
	GPE_ALFA	gx_itm_action( GPE_ALFA act, I8 ms );

	GPE_ACTION gx_alfa2act_id( GPE_ALFA alfa = GPE_ALFA_zero )
	{
		if( alfa == GPE_ALFA_zero )
		{
			// ha zéró akkor az aktuális értékkel tér vissza
			alfa = act_alfa;
		}

		if( (GPE_ACTION)alfa < GPE_ACTION_END )
		{
			// ó ha kisebb az érték alfa-ban mint action end akkor
			// nem alfa hanem action
			return (GPE_ACTION)alfa;
		}

		switch( act_alfa )
		{
			case GPE_ALFA_IDLE:	// unatkozik
				return GPE_ACTION_IDLE;
			case GPE_ALFA_WALK:
				return GPE_ACTION_WALK;
			case GPE_ALFA_RUN:
				return GPE_ACTION_RUN;
			case GPE_ALFA_JUMP:
				return GPE_ACTION_JUMP;
			case GPE_ALFA_SLEFT:
				return GPE_ACTION_SLEFT;
			case GPE_ALFA_SRIGHT:
				return GPE_ACTION_SRIGHT;
			case GPE_ALFA_FLINCH:
				return GPE_ACTION_FLINCH;
			case GPE_ALFA_BOX:
				return GPE_ACTION_BOX;
			case GPE_ALFA_KICK:
				return GPE_ACTION_KICK;
			case GPE_ALFA_SIT:
			case GPE_ALFA_ABOARD:
				return GPE_ACTION_ABOARD;
			case GPE_ALFA_DEBUS:
				return GPE_ACTION_DEBUS;
			case GPE_ALFA_STOW:
				return GPE_ACTION_STOW;
			case GPE_ALFA_STOWUP:
				return GPE_ACTION_STOWUP;
			case GPE_ALFA_WHAM:
				return GPE_ACTION_WHAM;
			case GPE_ALFA_KO:
				return GPE_ACTION_KO;
			case GPE_ALFA_ERECT:
				return GPE_ACTION_ERECT;
		}
		return GPE_ACTION_IDLE; 
	}
	GPCI4& gx_itm_in( GPCI4 i4 )
	{
		if( !this )
			return i4;

		i4 -= POSld;
		I8 i4_alt = i4.len_xyz();
		GPCV3	i4_norm = i4.xyz_norm(),
				i4_v3 = (mxi_world() * i4_norm)*i4_alt;
		i4 = i4_v3;
		return i4;
	}
	GPCI4& gx_itm_in_dot( GPCI4 i4 )
	{
		if( !this )
			return i4;

		i4 -= POSld;
		GPCV3 v3 = i4.xyz_v3();
		i4.x = v3*mx_world.x3;
		i4.y = v3*mx_world.y3;
		i4.z = v3*mx_world.z3;
		i4.w = 0;
		return i4;
	}
	GPCI4& gx_itm_in2out( GPCI4 i4 )
	{
		if( !this )
			return i4;

		I8 i4_alt = i4.len_xyz();
		GPCV3	i4_norm = i4.xyz_norm(),
				i4_v3 = (mx_world * i4_norm)*i4_alt;
		
		i4 = i4_v3;
		i4 += POSld;
		return i4;
	}

	GPC_GX_ITM& operator = ( GPC_GX_ITM& src_item )
	{
		
		if( src_item.nn )
		{
			(gp_strncpy( s_gx_name, src_item.s_gx_name, src_item.nn ))[src_item.nn] = 0;
			nn = src_item.nn;
			//b_upd = true;
		}
		if( src_item.np )
		{
			(gp_strncpy( s_pasw, src_item.s_pasw, src_item.np ))[src_item.np] = 0;
			np = src_item.np;
			//b_upd = true;
		}

		if( play_t <= src_item.play_t )
		{
			// gp_memcpy 0
			if( _memcmp( &born_t, &src_item.born_t, GPM_OFFOFF( GPC_GX_ITM, AA, born_t) ) )
			{
				gp_memcpy( &born_t, &src_item.born_t, GPM_OFFOFF( GPC_GX_ITM, AA, born_t) );
				b_updt = true;
			}

			// gp_memcpy 1
			if( AB_base_ms <= src_item.AB_base_ms )
			{
				gp_memcpy( &AA, &src_item.AA, GPM_OFFOFF( GPC_GX_ITM, y, AA) );
				b_updt = true;
			}
			// gp_memcpy 2
			if( yz_base_ms <= src_item.yz_base_ms )
			{
				gp_memcpy( &y, &src_item.y,  GPM_OFFOFF( GPC_GX_ITM, size_r, y) );
				b_updt = true;
			}

			// gp_memcpy 3
			if( act_base_ms <= src_item.act_base_ms )
			{
				gp_memcpy( &size_r, &src_item.size_r,  GPM_OFFOFF( GPC_GX_ITM, n_add_idx, size_r) );
				b_updt = true;
			}
			play_t = src_item.play_t;
		} else {
			born_t = src_item.born_t; // sérülést/gyogyulás
			exp = src_item.exp;
			pay = src_item.pay;
		}

		return *this;
	}

	

	void gx_clear( void )
	{
		GPM_DEL( p_star_idx );
		GPM_DEL( gx_p_crs );
		GPM_DEL( p_store );
		GPM_DELARRAY( child.p_idx_alloc );
		GPM_CLR;
		GPM_DELARRAY( p_user_path );
		n_glue_idx = n_mom_idx =
		gio_id = gim_id = gen_id = gim_aid =
		gx_stuff_id = -1;
		
		lod_sit = 0;
		LIGid = -1;
	}

	GPC_GX_ITM( void )
	{
		GPM_CLR;
		n_glue_idx = n_mom_idx =
		gio_id = gim_id = gen_id = gim_aid =
		gx_stuff_id = -1;
		
		lod_sit = 0;
		store_src_dst.x = 2;
		LIGid = -1;
	}

	~GPC_GX_ITM()
	{
		gx_clear();
	}

	void	gx_itm_child( GPC_GALAXY* p_gx, GPC_GATEx64* p_gt_aimom, I8 on_ms );
	
	GPCV3	gx_itm_HUD_loc(  GPC_TXTx64* p_txt, char** pp_stf, GPC_GX_ITM* p_cmps, GPC_COLOR4& color, bool b_walk );
	I8 gx_itm_ent( I8 gx_ms )
	{
		I8	// deficit = exp-pay,
			// ent = 1000/(10+(x/3000)^2);
			profit = pay-exp,
			ent = born_t+profit - gx_ms;
			ent /= min2ms*5;
			ent *= ent;
			ent += 10;
			ent = 1000/ent;
		return ent;
	}
	I8 gx_itm_eat( I8 gx_ms, I8 mat )
	{
		I8 p = sqrt((min2ms*5)*(min2ms*5)*40); 
		p += exp+gx_ms-born_t;
		p -= pay;
		p /= mat;
		return p;
	}


};


//class GPC3D64_LOD
//{
//public:
//	GPC3D_BUFF		*p_idx_buff;
//	ID3D11Buffer	*p_dx_lod_vx;
//	ID3D11Buffer	*p_dx_lod_cbuff;
//	I8		n_dx_lod_vx;
//	GPC_GIO_SURF	*p_surf;
//	GPC_GPUx64_CONST	cbuff;
//	GPC3DHI		*p_xhd;
//	U4		stride, offset, pad1, pad2;
//
//	void null( void )
//	{
//		GPM_CLR;
//	}
//	void reset( void )
//	{
//		GPM_CLR;
//	}
//	I8 lod_store( GPC_GIOx64_LAY_LOD* p_lod );
//	void lod_reload( GPC_GIOx64_LAY_LOD* p_lod );
//};
//==========================-====--===---==---- ---  --   -
//
//	STF2PAT
//
//=====================-====--===---==---- ---  --   -
__forceinline U1* GPF_STF2PAT( U1* p_pat, GPE_STF* p_stf, U1* p_filter )
{
	U4 u4 = 0x80808080, shr, shl;
	gp_ddd_lut( gp_memcpy( p_pat, p_stf, 0x1000 ), p_filter, 0x1000, u4 );
	gp_ddd_z( p_pat, 0x10, shr = 2 ); //, u4 );
	gp_ddd_x( p_pat, 0x100, shl = 1 );
	gp_ddd_y( p_pat, shr = 4 );
	return p_pat;
}






__forceinline U4* GPF_STF2HISTx80( U4* p_histi, U1* p_pat )
{
	for( U1* p_p = p_pat, *p_pe = p_p+GPE_Wq; p_p < p_pe; p_p++ )
	{
		p_histi[*p_p]++;
	}
	
	return p_histi;
}
__forceinline U1* GPF_STF2PATx80( U1* p_pat, GPE_STF* p_stf )
{
	for( U1* p_p = p_pat, *p_v = (U1*)p_stf, *p_pe = p_p+GPE_Wq; p_p < p_pe; p_p++, p_v++ )
	{
		if( !*p_v || *p_v == 0xff )
		{
			*p_p = 0x0;
			continue;
		}
		*p_p = 0x80;
	}
	// Z
	for( ULL* p_s = (ULL*)p_pat, *p_se = p_s+(GPE_Wq/8); p_s < p_se; p_s += (GPE_Wx/8) )
	for( ULL *p_d = p_s+2*GPE_3D_WIDTH, *p_de = p_s+2*GPE_3D_WIDTH*GPE_W16; p_d < p_de; p_s += 2*GPE_3D_WIDTH, p_d += 2*GPE_3D_WIDTH )
	
	//for( ULL* p_s = (ULL*)p_pat, *p_se = p_s+(GPE_Wq/8); p_s < p_se; p_s += (GPE_Wx/8) )
	//for( ULL *p_d = p_s+2, *p_de = p_s+(GPE_Wx/8); p_d < p_de; p_s += (GPE_W16/8), p_d += (GPE_W16/8) )
	{
		p_d[0] |= (p_s[0]&0x8080808080808080)>>2;
		p_d[1] |= (p_s[1]&0x8080808080808080)>>2;

		p_d[2] |= (p_s[2]&0x8080808080808080)>>2;
		p_d[3] |= (p_s[3]&0x8080808080808080)>>2;

		p_d[4] |= (p_s[4]&0x8080808080808080)>>2;
		p_d[5] |= (p_s[5]&0x8080808080808080)>>2;

		p_d[6] |= (p_s[6]&0x8080808080808080)>>2;
		p_d[7] |= (p_s[7]&0x8080808080808080)>>2;

		p_d[8] |= (p_s[8]&0x8080808080808080)>>2;
		p_d[9] |= (p_s[9]&0x8080808080808080)>>2;
	}
	// X
	ULL a_pat[GPE_3D_WIDTH*2+1], pat; 
	for( ULL* p_s = (ULL*)p_pat, *p_se = p_s+(GPE_Wq/8); p_s < p_se; p_s += (GPE_W16/8)  )
	{
		pat = 0;
		for( I8 p = 0; p < GPE_3D_WIDTH*2; p++ )
		{	
			// 0xa
			//    +-----+
			// 48 | 0 1 |
			// 12 | 0 1 |
			//    +-----+
			p_s[p]	|= pat;
			pat = p_s[p]&0xa0a0a0a0a0a0a0a0;  
			p_s[p]	|= pat<<7;
			pat >>= 57;
		}
	}
	// Y
	for( ULL* p_s = (ULL*)p_pat, *p_d = p_s+(GPE_Wx/8), *p_de = p_s+(GPE_Wq/8); p_d < p_de; p_s++, p_d++ )
	{
		*p_d |= ((*p_s)&0xf0f0f0f0f0f0f0f0)>>4;
	}
	return p_pat;
}

__forceinline U1* GPF_STF2PATx80( U1* p_pat )
{
	ULL	*p_se = (ULL*)p_pat,
		*p_s = p_se + (GPE_Wq/8),
		p0 = 0, p1 = 0,
		*p_d;
	// X
	p_s--;
	p0 = *p_s;
	*p_s |= p0<<7;
	while( p_s > p_se )
	{
		p_s--;
		p0 = *p_s;
		p_s[1] |= p0>>57;
		*p_s |= p0<<7;
	}

	// Z
	p_s = p_se + (GPE_Wq/8) - (GPE_W16*2/8);
	while( p_s >= p_se )
	{
		p_d = p_s+(GPE_W16/8);

		p_d[0] |= p_s[0]>>2;
		p_d[1] |= p_s[1]>>2;
		p_d[2] |= p_s[2]>>2;
		p_d[3] |= p_s[3]>>2;
		p_d[4] |= p_s[4]>>2;
		p_d[5] |= p_s[5]>>2;
		p_d[6] |= p_s[6]>>2;
		p_d[7] |= p_s[7]>>2;
		p_d[8] |= p_s[8]>>2;
		p_d[9] |= p_s[9]>>2;

		p_s -= (GPE_W16/8);
	}
	// Y
	p_s = p_se + (GPE_Wq/8) - (GPE_Wx*2/8);
	while( p_s >= p_se )
	{
		p_d = p_s+(GPE_Wx/8);

		p_d[0] |= p_s[0]>>4;
		p_d[1] |= p_s[1]>>4;
		p_d[2] |= p_s[2]>>4;
		p_d[3] |= p_s[3]>>4;
		p_d[4] |= p_s[4]>>4;
		p_d[5] |= p_s[5]>>4;
		p_d[6] |= p_s[6]>>4;
		p_d[7] |= p_s[7]>>4;
		p_d[8] |= p_s[8]>>4;
		p_d[9] |= p_s[9]>>4;

		p_s -= (GPE_W16/8);
	}
	return p_pat;
}

__forceinline GPE_STF GPF_STF_2x2( GPE_STF* p_s )
{
	if(
		   !p_s[0]		|| !p_s[1]		|| !p_s[GPE_3D_LINE]		|| !p_s[GPE_3D_LINE+1]
		|| !p_s[GPE_3D_xz]	|| !p_s[GPE_3D_xz+1]	|| !p_s[GPE_3D_xz+GPE_3D_LINE]	|| !p_s[GPE_3D_xz+GPE_3D_LINE+1]
	)
		return GPE_STF_SPACE;

	return *p_s;	

}
__forceinline U1 GPF_U1_erode_2x2x2_smpl( U1* p_s, I8 x, I8 y, I8 z )
{
	if(
		     !*p_s    || !p_s[  x]  ||  !p_s[  z]  || !p_s[  z+x] 
		 ||  !p_s[y]  || !p_s[y+x]  ||  !p_s[y+z]  || !p_s[y+z+x]
	)
		return GPE_STF_SPACE;

	return *p_s;
}
__forceinline U1 GPF_U1_erode_2x2x2_hard( U1* p_s, I8 x, I8 y, I8 z )
{
	if( *p_s&0x3f != p_s[x]&0x3f )
		return GPE_STF_SPACE;
	
	if( p_s[z]&0x3f != p_s[z+x]&0x3f  )
		return GPE_STF_SPACE;
	
	if( p_s[y]&0x3f != p_s[y+x]&0x3f )
		return GPE_STF_SPACE;
	
	if( p_s[y+z]&0x3f != p_s[y+z+x]&0x3f )
		return GPE_STF_SPACE;
	
	// ---------------- 
	
	if( *p_s&0x3f != p_s[z]&0x3f )
		return GPE_STF_SPACE;
	
	if( p_s[y]&0x3f != p_s[y+z]&0x3f )
		return GPE_STF_SPACE;
	
	return *p_s;
}

__forceinline U1 GPF_U1_erode_2x2x2_soft( U1* p_s, I8 x, I8 y, I8 z )
{
	U1 n =       !*p_s   + !p_s[  x] + !p_s[  z] + !p_s[  z+x] 
		+ !p_s[y] + !p_s[y+x] + !p_s[y+z] + !p_s[y+z+x];
	
	if( !n )
		return *p_s;
	
	if( n > 3 )
		return GPE_STF_SPACE;
	
	if( *p_s )
		return *p_s;
	if( p_s[y+z+x] )
		return p_s[y+z+x];
	if( p_s[x] )
		return p_s[x];
	if( p_s[y+z] )
		return p_s[y+z];
	if( p_s[y] )
		return p_s[y];
	if( p_s[y+x] )
		return p_s[y+x];
	if( p_s[z] )
		return p_s[z];
	return p_s[z+x];	

}
__forceinline U1 GPF_U1_EQU_2x2x2( U1* p_s, I8 x, I8 y, I8 z, U1 stf )
{
	if(	   stf != p_s[  0] 
		|| stf != p_s[  x]
		|| stf != p_s[  z]
		|| stf != p_s[  z+x]

		|| stf != p_s[y]
		|| stf != p_s[y+x]
		|| stf != p_s[y+z]
		|| stf != p_s[y+z+x]
	)
		return GPE_STF_SPACE;

	p_s[0] = p_s[  x] = p_s[  z] = p_s[  z+x] = p_s[y] = p_s[y+x] = p_s[y+z] = p_s[y+z+x] = 0;

	return stf;	

}
__forceinline U1 GPF_U1_dilate_2x2x2( U1* p_s, I8 x, I8 y, I8 z )
{
	return	  p_s[0] | p_s[  x] | p_s[  z] | p_s[  z+x]  
		| p_s[y] | p_s[y+x] | p_s[y+z] | p_s[y+z+x];
}
U1* GPF_U1_set2x2x2( U1* p_d, I8 n_d, I8 n_x, U1 set );
U1* GPF_U1_erode_smpl( U1* p_d, I8 n_d, I8 n_x, U1* p_s );
U1* GPF_U1_erode_soft( U1* p_d, I8 n_d, I8 n_x, U1* p_s );
U1* GPF_U1_erode_hard( U1* p_d, I8 n_d, I8 n_x, U1* p_s );
U1* GPF_U1_dilate_2x2x2( U1* p_d, I8 n_d, I8 n_x, U1* p_s );


class GPC3DLO
{
public:
	GPE_STF	a_stf[GPD_3D_END_Y];
	U1	a_light[GPD_3D_END_Y],
		a_pat[GPD_3D_END_Y];
		
	I8	n_updt, n_done;
	U2	id,	sum,	pad0x04,	pad0x06, 
		pad0x08,	pad0x0a,	pad0x0c,	pad0x0e;

	GPC3DLO( U1 _id )
	{
		GPM_CLR;
		id = _id;
	}

	GPC3DLO* lo_fill( ULL a2, GPE_STF* p_stuff, U1* p_light, I8 n_z = (GPE_3D_LINE/2), I8 n_line = GPE_3D_LINE );
	
	U1 set_irq( void )
	{
		if( !this )
			return 0;

		n_updt = n_done+1;
		return id;
	}
	U1 q_done( void )
	{
		return  this ? (n_updt == n_done) : false;
	}
	GPC3DLO* lo_ready( U1* p_filter )
	{
		if( !this )
			return NULL;
		if( n_updt <= n_done )
			return this;

		GPF_STF2PAT( a_pat, a_stf, NULL );
		n_done = n_updt;
		return this;
	}
	U2 n_sum(U2* p_share_n, GPC_COLOR4* p_color)
	{
		if( !this )
			return 0;

		sum = 0;
		gp_zero( p_share_n, sizeof(*p_share_n) * 0x100 );
		for (U2 i = 0; i < GPD_3D_END_Y; i++)
		{
			p_share_n[a_stf[i]]++;
		}
		sum = GPD_3D_END_Y - *p_share_n;

		if (p_color)
		{
			if (!sum)
			{
				gp_zero( a_pat, GPD_3D_END_Y);
				return 0;
			}
			gp_ddd_C42P( a_pat, p_color );
			return sum;
		}

		if (!sum)
		{
			gp_zero( a_pat, GPD_3D_END_Y );
			return 0;
		}

		GPF_STF2PAT( a_pat, a_stf, NULL);
		return sum;
	}
	U1* light_3d( U1* p_dst, U1 start_light, GPE_STF stuff_light )
	{
		gp_zero( p_dst, GPD_3D_END_Y );
		if( !this )
			return NULL;

		U1* p_out = NULL;
		for( U2 i = 0; i < GPD_3D_END_Y; i++ )
		{
			if(
				(a_stf[i] & 0x3f) != stuff_light
				|| p_dst[i] >= start_light
			)
			{
				if( p_out ? true : !p_dst[i] )
					continue;

				p_out = p_dst;
				continue;
			}

			p_dst[i] = start_light;
			if (p_out)
				continue;
			p_out = p_dst;
		}
		return p_out;
	}
};


class GPC_Qx
{
public:
	GPC_COLOR4 a_C4[GPD_3D_END_Y];
	I8	n_updt, n_done;
	U2	id,	sum,	pad0x04,	pad0x06, 
		pad0x08,	pad0x0a,	pad0x0c,	pad0x0e;

	GPC_Qx( U1 _id )
	{
		GPM_CLR;
		id = _id;
	}

	GPC_Qx* qx_updt( GPC_COLOR4* p_C4_shr )
	{
		if( !p_C4_shr )
		{
			if( this )
			{
				GPM_ZERO( a_C4 );
				set_irq();
			}
			return this;
		}

		if( !this )
		{
			GPC_Qx* p_this = new GPC_Qx(1);
			if( !p_this)
				return NULL;

			return p_this->qx_updt( p_C4_shr );
		}
		U4 n_equ = gp_memcmp( a_C4, p_C4_shr, sizeof(a_C4)) - (U1*)a_C4;
		if( n_equ >= sizeof(a_C4) )
			return this;

		n_equ &= ~0xf;
		gp_memcpy( ((U1*)a_C4)+n_equ, ((U1*)p_C4_shr)+n_equ, sizeof(a_C4)-n_equ );
		set_irq();
		return this;
	}

	GPC_Qx*	lo_fill( U1* p_done_msk, ULL a2, GPC_COLOR4* p_C4, char* p_mask, I8 n_z = (GPE_3D_LINE/2), I8 n_line = GPE_3D_LINE );
	U2	n_sum( GPC3D* p_3d, U2* p_share_n, char* p_mask );

	U1 set_irq( void )
	{
		if( !this )
			return 0;

		n_updt = n_done+1;
		return id;
	}
	U1 q_done( void )
	{
		return  this ? (n_updt == n_done) : false;
	}
	GPC_Qx* lo_ready( U1* p_filter )
	{
		if( !this )
			return NULL;
		n_done = n_updt;
		return this;
	}
	
	char* qx_light( char* p_dst, char* p_mask, char* p_moon )
	{
		gp_zero( p_dst, GPD_3D_END_Y );
		if( !this )
			return NULL;

		char* p_first = NULL;
		
		for( U2 i = 0; i < GPD_3D_END_Y; i++ )
		{
			if( a_C4[i].ent < 0x68 )
			{
				if( p_moon[a_C4[i].stf] )
				{
					p_dst[i] = p_moon[a_C4[i].stf];
					continue;
				}
				p_dst[i] = p_mask[a_C4[i].stf];
				continue;
			}
			
			p_first = p_dst+i;
			for( ; i < GPD_3D_END_Y; i++ )
			{
				if( a_C4[i].ent < 0x68 )
				{
					p_dst[i] = p_mask[a_C4[i].stf];
					continue;
				}
				p_dst[i] = a_C4[i].ent-0x68;
				if( p_dst[i] > 0x10 )
					p_dst[i] = 0x10;
			}
			break;
		}
		return p_first;
	}
};
class GPC_STF
{
public:
	int	pos, gio_id;
	GPC_COLOR4 stf1, stf2;
};
class GPC3D
{
public:
	char		a_light[GPD_3D_END_Y],				// 0x1000		0x1000
				s_q0_str[0x18];						// 0x18
	GPC_Qx		*ap_Qx[GPD_3DnX11];					// 0x18			0x1030

	GPC_GIVx64	*p_giv;								// 8				
	U4			Q0, n_gpx_updt,						// 8	
				QD, QR,								// 8					
				QF, QU,								// 8	
				QL, QB,								// 8					
				_Q, gpx_ix;							// 8			0x1060

	U1			rd,rr,rf,ru,rl,rb, ab_light, pdr7;	// 8			0x1068
	U1 			*p_gpx_updt;						// 8			0x1070

	GPC_COLOR4	*p_color, *p_ref;					// 	16			0x1080

	GPC_FIX		*p_gpx, *p_pad;						//	16			0x1090	
	ULL			n_done, n_updt,						//	16				
				n_dtct, gpxver;						//	16			0x10A0		
	GPCI4		GR, nr_gr;							//	64			0x10E0

	GPC3D( U4 Q, GPC_GIVx64* _p_giv )
	{
		GPM_CLR;
		QD = QR = QF = QU = QL = QB = Q0 = Q;
		sprintf( s_q0_str, "%0.8x.gpx", Q0 );
		p_giv = _p_giv;
		//oiix = gpe_giv_XZY;
		GR.Q2G( Q0 );
		nr_gr = GR*GPCI4( gpe_giv_LINE, gpe_giv_LINE, gpe_giv_LINE, 1 );
		//-----------------------------
		//	X
		//-----------------
		if( GR.x && GR.x < 0xf )
		{
			QD = Q0-1;
			QU = Q0+1;	
		} 

		switch( GR.w )
		{
			case 0:
				//-----------------------------
				//	Z0
				//-----------------
				if( !GR.z )
				{
					//------------ 
					QL += gpe_giv_XZZ5 + gpe_giv_XZ - gpe_giv_X;
					QR += gpe_giv_X;
				} 
				else if( GR.z == (gpe_giv_Z-1) )
				{
					//------------ 
					QL -= gpe_giv_X;
					rr = 1;
					QR = GR.x + gpe_giv_XZY - gpe_giv_X;
					QR -= gpe_giv_X*GR.y;
				} else {
					QL -= gpe_giv_X;
					QR += gpe_giv_X;
				}

				//-----------------------------
				//	Y0
				//-----------------
				if( !GR.y )
				{
					//------------
					rb = 3;
					QB = GR.x + gpe_giv_XZZ5 - gpe_giv_X - gpe_giv_XZ*GR.z; 	
				} else {
					QB -= gpe_giv_XZ;
				}
				QF += gpe_giv_XZ;
				break;
			case 1:
				//-----------------------------
				//	Z1
				//-----------------
				if( !GR.z )
				{
					//------------ 
					rl = 1;
					QL = GR.x + gpe_giv_XZY3 - gpe_giv_X;
					QL -= gpe_giv_X*GR.y;
					QR += gpe_giv_X;
				} 
				else if( GR.z == (gpe_giv_Z-1) )
				{
					//------------ 
					QL -= gpe_giv_X;
					QR += gpe_giv_XZZ - gpe_giv_XZ + gpe_giv_X;
				} else {
					QL -= gpe_giv_X;
					QR += gpe_giv_X;
				}

				//-----------------------------
				//	Y1
				//-----------------
				QB -= gpe_giv_XZ;
				if( GR.y == (gpe_giv_Z-1) )
				{
					//------------
					rf = 3;
					QF = GR.x + gpe_giv_XZY2 - gpe_giv_XZ;
					QF -= GR.z*gpe_giv_XZ;
				} else {
					QF += gpe_giv_XZ;
				}
				break;
			case 2:
				//-----------------------------
				//	Z2
				//-----------------
				if( !GR.z )
				{
					//------------ 
					QL -= gpe_giv_XZZ;
					QL += gpe_giv_XZ - gpe_giv_X;
					QR += gpe_giv_X;
				} 
				else if( GR.z == (gpe_giv_Z-1) )
				{
					//------------ 
					QL -= gpe_giv_X;
					rr = 1;
					QR = GR.x + gpe_giv_XZY2 - gpe_giv_X;
					QR -= gpe_giv_X*GR.y;
				} else {
					QL -= gpe_giv_X;
					QR += gpe_giv_X;
				}

				//-----------------------------
				//	Y2
				//-----------------
				if( !GR.y )
				{
					//------------
					rb = 3;
					QB = GR.x + gpe_giv_XZZ - gpe_giv_X - gpe_giv_XZ*GR.z; 
				} else {
					QB -= gpe_giv_XZ;
				}
				QF += gpe_giv_XZ;
				break;
			case 3:
				//-----------------------------
				//	Z3
				//-----------------
				if( !GR.z )
				{
					//------------ 
					rl = 1;
					QL = GR.x + gpe_giv_XZY - gpe_giv_X;
					QL -= gpe_giv_X*GR.y;
					QR += gpe_giv_X;
				} 
				else if( GR.z == (gpe_giv_Z-1) )
				{
					//------------ 
					QL -= gpe_giv_X;
					QR += gpe_giv_XZZ - gpe_giv_XZ + gpe_giv_X;
				} else {
					QL -= gpe_giv_X;
					QR += gpe_giv_X;
				}

				//-----------------------------
				//	Y3
				//-----------------
				QB -= gpe_giv_XZ;
				if( GR.y == (gpe_giv_Z-1) )
				{
					//------------
					rf = 3;
					QF  = GR.x + gpe_giv_XZY3 - gpe_giv_XZ;
					QF -= GR.z*gpe_giv_XZ; 
				} else {
					QF += gpe_giv_XZ;
				}
				break;
			case 4:
				//-----------------------------
				//	Z4
				//-----------------
				if( !GR.z )
				{
					//------------ 
					QL -= gpe_giv_XZZ;
					QL += gpe_giv_XZ - gpe_giv_X; // (gpe_giv_X*((gpe_giv_Y-1)-GR.y)) ;
					QR += gpe_giv_X;
				} 
				else if( GR.z == (gpe_giv_Z-1) )
				{
					//------------ 
					QL -= gpe_giv_X;
					rr = 1;
					QR = GR.x + gpe_giv_XZ - gpe_giv_X;
					QR -= gpe_giv_X*GR.y;
				} else {
					QL -= gpe_giv_X;
					QR += gpe_giv_X;
				}

				//-----------------------------
				//	Y4
				//-----------------
				if( !GR.y )
				{
					//------------
					rb = 3;
					QB = GR.x + gpe_giv_XZZ3 - gpe_giv_X - gpe_giv_XZ*GR.z;	
				} else {
					QB -= gpe_giv_XZ;
				}
				QF += gpe_giv_XZ;
				break;
			case 5:
				//-----------------------------
				//	Z5
				//-----------------
				if( !GR.z )
				{
					//------------ 
					rl = 1;
					QL = GR.x + gpe_giv_XZY2 - gpe_giv_X;
					QL -= gpe_giv_X*GR.y;
					QR += gpe_giv_X;
				} 
				else if( GR.z == (gpe_giv_Z-1) )
				{
					//------------ 
					QL -= gpe_giv_X;
					QR -= gpe_giv_XZZ5 + gpe_giv_XZ - gpe_giv_X;
				} else {
					QL -= gpe_giv_X;
					QR += gpe_giv_X;
				}

				//-----------------------------
				//	Y5
				//-----------------
				QB -= gpe_giv_XZ;
				if( GR.y == (gpe_giv_Z-1) )
				{
					//------------
					rf = 3;
					QF = GR.x + gpe_giv_XZY - gpe_giv_XZ;
					QF -= GR.z*gpe_giv_XZ;
				} else {
					QF += gpe_giv_XZ;
				}
				break;
		}
		U4* p_q6 = &QD;
		for( U1 i = 0; i < 6; i++ )
		{
			if( p_q6[i] >= gpe_giv_XZY3 )
				p_q6[i] %= gpe_giv_XZY3;
		}
	}
	GPC_Qx* p_Q0_unp( U1* p_buff, char* p_path, char* p_file )
	{
		if( this ? ap_Qx[0] : true )
			return this ? ap_Qx[0] : NULL;
		
		if( !p_path )
			return ap_Qx[0];

		ULL s;
		if( !p_gpx )
		{
			if( p_file < p_path )
				p_file = p_path;

			if( !*s_q0_str )
			{
				sprintf( s_q0_str, "%0.8x.gpx", Q0 );
			}
			GPM_STRCPY( p_file, s_q0_str );
			if( GPF_ACE( p_path, 4 ) < 0 )
				return ap_Qx[0];

			p_gpx = ((GPC_FIX*)NULL)->fix_read( p_path, s = 0, 0 );
			*p_file = 0;
			if( !p_gpx )
				return ap_Qx[0];
		}
		GPC_FIX* p_unp = ((GPC_FIX*)NULL)->fix_bag_decode( p_buff, s = -1, -1, p_gpx->fix_p_alloc, p_gpx->fix_n_alloc );
		if( !p_unp )
		{
			GPM_DEL(p_gpx);
			return ap_Qx[0];
		}
		gpxver = p_unp->fix_ver;
		//GPF_CON_LST_write( GPE_CLST_GIV_AI_DEC, GPE_GT_SERV, s_q0_str, Q0 );
		
		ap_Qx[0] = ap_Qx[0]->qx_updt( (GPC_COLOR4*)p_unp->fix_p_alloc );
		GPM_DEL(p_unp);
		return ap_Qx[0];
	}
	GPC_COLOR4* q_A0_alloc( void )
	{
		if( !this )
			return NULL;
		if( !ap_Qx[0] )
			ap_Qx[0] = new GPC_Qx(1);
		return q_A0();
	}
	GPC_COLOR4* q_A0( void )
	{
		if( this ? !ap_Qx[0] : true )
			return NULL;

		p_color = ap_Qx[0]->a_C4;
		if( p_ref )
			return p_color;
		
		p_ref = new GPC_COLOR4[GPD_3D_END_Y];
		gp_memcpy( p_ref, p_color, sizeof(ap_Qx[0]->a_C4) );
		return p_color;
	}
	bool b_core( GPCI4 xyz, I8 r, char* p_mask )
	{
		if( !this )
			return false;
		if( !ap_Qx[0] || !p_mask )
			return false;
		GPCI4	xyz_c = xyz-nr_gr,
			xyz_q;
		U2 i = xyz_c.x+xyz_c.y*0x100+xyz_c.z*0x10;
		if( r < 2 )
			return ( p_mask[ap_Qx[0]->a_C4[i].stf] < 0 );

		//- Y ---------------
		xyz_q.y = xyz_c.y-(r-1);
		if( xyz_q.y > 0xf )
			return false;
		xyz_q.w = xyz_c.y+r;
		if( xyz_q.w <= xyz_q.y )
			return false;
		if( xyz_q.w > 0x10 )
			xyz_q.w = 0x10;


		//- Z ---------------
		xyz_q.z = xyz_c.z-(r-1);
		if( xyz_q.z > 0xf )
			return false;
		I8 d = xyz_c.z+r;
		if( d <= xyz_q.z )
			return false;
		if( d > 0x10 )
			d = 0x10;

		//- X ---------------
		xyz_q.x = xyz_c.x-r;
		if( xyz_q.x > 0xf )
			return false;
		GPC_COLOR4	*p_c0  = ap_Qx[0]->a_C4;
		for( i = 0; i < GPM_N(ap_Qx[0]->a_C4); i++ )
		{
			if( p_mask[p_c0[i].stf] > -1 )
				continue;
			xyz_q.a0_xyz0(i);
			d = (xyz_c-xyz_q).maxi_absabs_xyz();
			if( d < r )
				return true;
		}
		
		return false;
	}
	GPC_COLOR4* p_c4( GPCI4 gr0x10 )
	{
		if( this ? !ap_Qx[0] : true )
			return NULL;
		gr0x10 -= nr_gr;
		U4 d = gr0x10.dddx10();
		if( d >= GPM_N(ap_Qx[0]->a_C4) )
			return NULL;
		
		return  ap_Qx[0]->a_C4+d;
	}
	GPC_COLOR4* p_c4( U4 d )
	{
		if( this ? !ap_Qx[0] : true )
			return NULL;
		if( d >= GPM_N(ap_Qx[0]->a_C4) )
			return NULL;

		return  ap_Qx[0]->a_C4+d;
	}

	bool updt_gpx(void* p_byte, U2 n_byte)
	{
		if( this ? (!p_byte || !n_byte) : true)
			return false;
		if( n_byte >= GPD_3D_END_YC4)
			return false;
		I8 i = 0;
		while( p_gpx_updt ) 
		{
			Sleep( 0 );
			i++;
			if( i == 10 )
			{
				GPM_DELARRAY( p_gpx_updt );
				break;
			}
		}
		if( !p_gpx_updt)
			p_gpx_updt = new U1[GPD_3D_END_YC4];
		
		gp_memcpy( p_gpx_updt, p_byte, n_gpx_updt = n_byte);
		n_gpx_updt = n_byte;
		return true;
	}

	char* flood_X_cross( char* p_dst )
	{
		if( !p_dst )
			return NULL;

		GPCI4 xyz(0, 0, 0, 0);
		char light;
		I8 t0, t1, t2;
		for( xyz.z = 0; xyz.z < GPD_3D_END_Z; xyz.z += GPD_3D_STEP_Z )
		for( xyz.y = 0; xyz.y < GPD_3D_END_Y; xyz.y += GPD_3D_STEP_Y )
		{
			t0 = xyz.sum_xyz();
			
			light = p_dst[t0] < 1 ? 0 : p_dst[t0];
			for( t1 = GPD_3D_STEP_X; t1 < GPD_3D_END_X; t1 += GPD_3D_STEP_X )
			{
				if (light > 0)
					light--;

				t2 = t0 + t1;
				if( p_dst[t2] < 0 )
				{
					light = 0;
					continue;
				}

				if( light < p_dst[t2] )
					light = p_dst[t2];
				if( !light )
					continue;
				p_dst[t2] = light;
			}

			t1 = GPD_3D_END_X - GPD_3D_STEP_X;
			t2 = t0 + t1;
			light = p_dst[t2] < 1 ? 0 : p_dst[t2];
			for( t1 -= GPD_3D_STEP_X; t1 >= 0; t1 -= GPD_3D_STEP_X )
			{
				if (light > 0)
					light--;

				t2 = t0 + t1;
				if( p_dst[t2] < 0 )
				{
					light = 0;
					continue;
				}
				if( light < p_dst[t2] )
					light = p_dst[t2];

				if( !light)
					continue;
				p_dst[t2] = light;
			}
			
		}
	}
	char* flood_Z_cross( char* p_dst )
	{
		if( !p_dst )
			return NULL;

		GPCI4 xyz(0, 0, 0, 0);
		char light;
		I8 t0, t1, t2;
		for( xyz.x = 0; xyz.x < GPD_3D_END_X; xyz.x += GPD_3D_STEP_X )
		for( xyz.y = 0; xyz.y < GPD_3D_END_Y; xyz.y += GPD_3D_STEP_Y )
		{
			t0 = xyz.sum_xyz();
			
			light = p_dst[t0] < 1 ? 0 : p_dst[t0];
			for( t1 = GPD_3D_STEP_Z; t1 < GPD_3D_END_Z; t1 += GPD_3D_STEP_Z )
			{
				if( light > 0 )
					light--;

				t2 = t0 + t1;
				if( p_dst[t2] < 0 )
				{
					light = 0;
					continue;
				}
				if( light < p_dst[t2] )
					light = p_dst[t2];

				if (!light)
					continue;
				p_dst[t2] = light;
			}
			

			t1 = GPD_3D_END_Z - GPD_3D_STEP_Z;
			t2 = t0 + t1;
			
			light = p_dst[t2] < 1 ? 0 : p_dst[t2];
			for( t1 -= GPD_3D_STEP_Z; t1 >= 0; t1 -= GPD_3D_STEP_Z )
			{
				if (light > 0)
					light--;

				t2 = t0 + t1;
				if( p_dst[t2] < 0 )
				{
					light = 0;
					continue;
				}
				if( light < p_dst[t2] )
					light = p_dst[t2];

				if (!light)
					continue;
				p_dst[t2] = light;
			}
			
		}
	}
	char* flood_Y_cross( char* p_dst )
	{
		if (!p_dst)
			return NULL;

		GPCI4 xyz(0, 0, 0, 0);
		char light;
		I8 t0, t1, t2;
		for (xyz.x = 0; xyz.x < GPD_3D_END_X; xyz.x += GPD_3D_STEP_X)
		for (xyz.z = 0; xyz.z < GPD_3D_END_Z; xyz.z += GPD_3D_STEP_Z)
		{
			t0 = xyz.sum_xyz();
			light = p_dst[t0] < 1 ? 0 : p_dst[t0];
			for (t1 = GPD_3D_STEP_Y; t1 < GPD_3D_END_Y; t1 += GPD_3D_STEP_Y)
			{
				if (light > 0)
					light--;

				t2 = t0 + t1;
				if( p_dst[t2] < 0 )
				{
					light = 0;
					continue;
				}
				if (light < p_dst[t2])
					light = p_dst[t2];

				if (!light)
					continue;
				p_dst[t2] = light;
			}
			
			t1 = GPD_3D_END_Y - GPD_3D_STEP_Y;
			t2 = t0 + t1;
			light = p_dst[t2] < 1 ? 0 : p_dst[t2];
			for( t1 -= GPD_3D_STEP_Y; t1 >= 0; t1 -= GPD_3D_STEP_Y )
			{
				if (light > 0)
					light--;

				t2 = t0 + t1;
				if( p_dst[t2] < 0 )
				{
					light = 0;
					continue;
				}
				if( light < p_dst[t2] )
					light = p_dst[t2];

				if( !light )
					continue;
				p_dst[t2] = light;
			}
			
		}
	}
	
	char* light_X_cpy( char* p_dst, U2 src )
	{
		if( !this || !p_dst )
			return p_dst;
		GPCI4 xyz;
		xyz.x = 0;
		I8 t0, n_add = 0;
		char*	p_src = a_light + src;
		
		char light;
		for (xyz.z = 0; xyz.z < GPD_3D_END_Z; xyz.z += GPD_3D_STEP_Z)
		for (xyz.y = 0; xyz.y < GPD_3D_END_Y; xyz.y += GPD_3D_STEP_Y)
		{
			t0 = xyz.sum_xyz();
			if( p_dst[t0] < 0 )
				continue;

			light = p_dst[t0];
			light -= p_src[t0];
			if( light > 1 )
			{
				if( p_src[t0] < 0 )
					continue;
				// a célban erõsebb fény van
				// bemásoljuk a forrásba
				// és nem azért mert fal van
				p_src[t0] = p_dst[t0] - 1;
				n_add++;
				continue;
			}
			else if (light > -1)
				continue;
			n_add++;
			p_dst[t0] = p_src[t0] - 1;
		}
		if (n_add)
			ab_light |= GPE_3D_LIGHT_nX;
		return p_dst;
	}
	char* light_Y_cpy( char* p_dst, U2 src, U1 rot )
	{
		if (!this || !p_dst)
			return p_dst;
		GPCI4 xyz;
		xyz.y = 0;
		I8 t0, n_add = 0;
		char*	p_src = a_light + src;
		
		if( rot )
		{

		}
		char light;
		for(xyz.z = 0; xyz.z < GPD_3D_END_Z; xyz.z += GPD_3D_STEP_Z)
		for(xyz.x = 0; xyz.x < GPD_3D_END_X; xyz.x += GPD_3D_STEP_X)
		{
			t0 = xyz.sum_xyz();
			if( p_dst[t0] < 0 )
				continue;

			light = p_dst[t0];
			light -= p_src[t0];

			if( light > 1 )
			{
				if( p_src[t0] < 0 )
					continue;
				// a célban erõsebb fény van
				// bemásoljuk a forrásba
				// és nem azért mert fal van
				p_src[t0] = p_dst[t0] - 1;
				n_add++;
				continue;
			}
			else if (light > -1)
				continue;
			n_add++;
			p_dst[t0] = p_src[t0] - 1;
		} 
		if (n_add)
			ab_light |= GPE_3D_LIGHT_nY;
		return p_dst;
	}
	char* light_Z_cpy( char* p_dst, U2 src, U1 rot )
	{
		if( !this || !p_dst )
			return p_dst;
		GPCI4 xyz;
		xyz.z = 0;
		I8 t0, n_add = 0;
		char*	p_src = a_light + src;
		if( rot )
		{

		}
		int light;
		for( xyz.y = 0; xyz.y < GPD_3D_END_Y; xyz.y += GPD_3D_STEP_Y)
		for( xyz.x = 0; xyz.x < GPD_3D_END_X; xyz.x += GPD_3D_STEP_X)
		{
			t0 = xyz.sum_xyz();
			if( p_dst[t0] < 0 )
				continue;

			light = p_dst[t0];
			light -= p_src[t0];

			if (light > 2)
			{
				if( p_src[t0] < 0 )
					continue;
				// a célban erõsebb fény van
				// bemásoljuk a forrásba
				// és nem azért mert fal van
				p_src[t0] = p_dst[t0] - 1;
				n_add++;

				continue;
			}
			else if (light > -1)
				continue;
			p_dst[t0] = p_src[t0] - 1;
			n_add++;
		}
		if( n_add )
			ab_light |= GPE_3D_LIGHT_nZ;
		return p_dst;
	}

	void	giv_light_flood( GPC3D** pp_3d );
	GPC_FIX*	giv_gpx_store( bool b_bgra );
	
	
};

GPCV2*	GP_A_CELL_get_hi( void );
GPCV2*	GP_A_CELL_get_lo( void );
U2	GP_n_cell_v2( void );
U2*	GP_A_CELL_ix_lo( void );
U2	GP_A_CELL_n_ix_lo( void );
GPE_DIR	GPF_CELL_LIG2FC_hi( float l );




__declspec(align(16)) class GPC3D64
{
	public:
		U1	a_buff[GPE_PACK_DEC_BUFF];
		GPC_COLOR4	a_3d64_map[GPE_Wq]; // *2];
		U4			a_stf_histi[0x100];
		// lo -------------------------
		U4	a_lo_512[8*8*8],
			n_v2_lo_load, n_v2_lo_alloc;
		U2	a_lo_512nIX[8*8*8];
		// hi -------------------------
		U4	a_hi_512[6*8*8*8],
			an_v2_hi_load[6], an_v2_hi_alloc[6];
		U2	a_hi_512nIX[6*8*8*8];
		U1	a_512sw[8*8*8];
		// tak1 -------------------------
		U4	a_tk1_512[8*8*8],
			n_v2_tk1_load, n_v2_tk1_alloc;
		U2	a_tk1_512nIX[8*8*8];
		// tak2 -------------------------
		U4	a_tk2_512[8*8*8],
			n_v2_tk2_load, n_v2_tk2_alloc;
		U2	a_tk2_512nIX[8*8*8];
		// tak1 -------------------------
		U4	a_bs1_512[8*8*8],
			n_v2_bs1_load, n_v2_bs1_alloc;
		U2	a_bs1_512nIX[8*8*8];
		// tak2 -------------------------
		U4	a_bs2_512[8*8*8],
			n_v2_bs2_load, n_v2_bs2_alloc;
		U2	a_bs2_512nIX[8*8*8];

		// GPD_3d_INST -+- INSTANCE
		GPC_IIx64	a_ii[GPE_WxWs1+GPE_Wx], *p_ii;
		U4		a_inst[0x1000], a_in[0x1000], *p_in;


		GPE_STF	a_3d[GPE_Wq];
		U1		a_light[GPE_Wq],
				a_pat[GPE_Wq],
				a_wXh[GPE_Wq];
		
		GPC3D	*a_3d2_ftch[GPE_3D_WIDTHq+1];
		char	a_rot[32],
				s_cache[GPD_MAX_PATH*GPE_3D_WIDTHq]; // GPE_3D_WIDTHx
		U4		a_Q0[GPE_3D_WIDTHq+3];

		GPCI4	GR;

		I8		a_ms[GPE_3D_PIPE_END+1],				// 9
				size_3d, div_3d, n,						// 12
				n_build, dim_base, e0_nr_off,			// 15		
				*p_a0_buff, n_fatch, n_a0_buff, bld_id,	// 19
				n_027, n_x11, n3x3x3;					// 22
		
		GPCV2	*p_v2_lo,
				*ap_v2_hi[6],
				*ap_v2_tk[2],
				*ap_v2_bs[2];
		I8		s_ms, e_ms;


		GPE_STF	*p_64_stf;
		U1		*p_64_lig,
				*p_64_pat;
		U2		*p_64_wXh;
				
		// új
		U4			pad1, pad2, pad3, ddd_res;
		I8			map_id;
		GPE_3D_PIPE	n_pipe;				// 1
		

	GPC3D64( void ){};
	~GPC3D64( void ){};

	//void	ddd64_PAT( GPE_STF* p_64c, U1 *p_l64, U1 *p_pat );
	//void	ddd64_WELD( GPE_STF* p_64c, U1 *p_l64, U1 *p_pat, I8 off );

	void	ddd64_fill( U1* p_dst, U1* p_src );
	void	ddd64_mask( U1* p_dst, U1* p_src );

	void	ddd64_GROW(  U2* p_wXh, U1 *p_pat );
	U4	ddd64_BUILD_histi( U1* p_pat );
	/*I8	ddd64_BUILD_cell_x11(
					GPC_WBx64* p_wbx64,
					U1* p_pat,
					U4* p_histi,
					GPCV2* p_cell, U4* p_s, U4* p_sn,
					U1 s,
					U2 *p_reserve
				);*/
	GPCV2* 	__vectorcall ddd64_BUILD_cell_x11_512(
					GPCV2* p_v2,
					U4* pn_v2_load,
					U4* pn_v2_alloc,

					GPC_WBx64* p_wbx64,
					U1* p_pat,
					U4* p_histi,
					GPCV2* p_cell, U4* p_s, U4* p_sn,
					U1 s,
					U4* p_512
				);

	void	build_64to32_dilet( GPE_STF* p_64c );

	//void	build_64to32( GPE_STF* p_dst, GPE_STF* p_64c );
	//void	build_64to32_253( GPE_STF* p_dst, GPE_STF* p_64c );
	

	//void	histi_64x( U4* p_histi, GPC_COLOR4* p_deep, U1 s );
	void	histi_64q( U4* p_histi, U4* p_histi2x2, U1* p_64s, GPE_STF* p_64c );

	//void	histi_pat( U4* p_histi, U1* p_pat, I8 n );

	//void	ddd64_VOX( GPC3D_BASE* p_base, bool b_x11 = false );
	void	ddd64_VOX( GPC3D_EXEC* p_exec );
	//void	ddd_VOX_inst( GPC3D_BASE* p_base, GPC_GIOx64* p_gio, GPC_GIOx64_LAY* p_inst_lay );

	__forceinline ULL build_pat0x10( ULL p, GPE_STF* p_stf )
	{
		if( p )
		{
			p >>= 4;
			p &= (ULL)0x0f0f0f0f0f0f0f0f;
		}
		register ULL pat = p;
		register ULL row0 = *(ULL*)p_stf;
		register ULL row1 = *(ULL*)(p_stf+8);
		
		if( row0&(ULL)0xff )
			pat |= (ULL)0x10;
		if( row0&((ULL)0xff<<8) )
			pat |= (ULL)0x1020;
		if( row0&((ULL)0xff<<16) )
			pat |= (ULL)0x1020<<8;
		if( row0&((ULL)0xff<<24) )
			pat |= (ULL)0x1020<<16;
		if( row0&((ULL)0xff<<32) )
			pat |= (ULL)0x1020<<24;
		if( row0&((ULL)0xff<<40) )
			pat |= (ULL)0x1020<<32;
		if( row0&((ULL)0xff<<48) )
			pat |= (ULL)0x1020<<40;
		if( row0&((ULL)0xff<<56))
			pat |= (ULL)0x1020<<48;
		if( row1&(ULL)0xff)
			pat |= (ULL)0x20<<56;

		row0 = *(ULL*)(p_stf+GPE_W16);
		row1 = *(ULL*)(p_stf+GPE_W16+8);

		if( row0&(ULL)0xff )
			pat |= (ULL)0x40;
		if( row0&((ULL)0xff<<8) )
			pat |= (ULL)0x4080;
		if( row0&((ULL)0xff<<16) )
			pat |= (ULL)0x4080<<8;
		if( row0&((ULL)0xff<<24) )
			pat |= (ULL)0x4080<<16;
		if( row0&((ULL)0xff<<32) )
			pat |= (ULL)0x4080<<24;
		if( row0&((ULL)0xff<<40) )
			pat |= (ULL)0x4080<<32;
		if( row0&((ULL)0xff<<48) )
			pat |= (ULL)0x4080<<40;
		if( row0&((ULL)0xff<<56))
			pat |= (ULL)0x4080<<48;
		if( row1&(ULL)0xff)
			pat |= (ULL)0x80<<56;

		return pat;
	}
	
	/*bool q_chk( void )
	{
		if( !this )
			return false;
		if( n_pipe == GPE_3D_PIPE_OFF )
			return false;

		I8 n_dif = 0, n_up, n_empty = 0, n_3d = 0;
		GPCI4	xyz(0,0,0,0);
		GPC3D	*p_3d;
		for( xyz.y = 0; xyz.y < GPE_3D_WIDTH ; xyz.y++ )
		for( xyz.z = 0; xyz.z < GPE_3D_WIDTH ; xyz.z++ )
		for( xyz.x = 0; xyz.x < GPE_3D_WIDTH ; xyz.x++ )
		{
			n_3d++;
			if( !a_3d_fatch[xyz.ddd(GPE_3D_WIDTH)] )
			{
				n_empty++;
				continue;
			}
			n_dif++;
		}

		n_pipe = GPE_3D_PIPE_CHECK; 
		if( !n_dif )
			n_pipe = ((n_empty == n_3d ) ? GPE_3D_PIPE_OFF : GPE_3D_PIPE_UPLOAD);
		return true;
	}*/
	
};




void GPF_TRD_NAME( char* _p_name );
class GPC3D_EXEC
{
public:
	GPE_STF	a_64_stf[GPE_Wq*6];
	U1	a_64_lig[GPE_Wq*2],
		a_64_pat[GPE_Wq*2];
	U2	a_64_wXh[GPE_Wq];
	char	s_name[0x100];

	

	HANDLE	exec_whd;
	DWORD_PTR	p_affi_whd;

	GPCI4	eye,
		origo;

	GPC3D64	*p_3d64;
	GPC_3D_X11	*p_x11;
	GPC_WBx64	*p_wbx64;

	I8	n_build, buff_r, eye_r, n_res, e, cnt, map;
	float	water_r;
	
	// ÚJ ---------------------------------------
	bool		b_break;
	GPC_GIVx64		*p_giv, *p_pad;
	
	GPE_WIP		ex_wip;
	std::mutex		ex_mutex;
	

	GPC3D_EXEC( void )
	{
		#ifdef _DEBUG_GT_MUTI
			gp_zero( this, ((char*)&ex_wip)-(char*)this );
			ex_wip = GPE_WIP_TRD_DONE;
		#else
			GPM_CLR;
			wip100 = GPE_WIP_TRD_DONE;
		#endif

		water_r = 1.0f;
	}
	void	giv_loop( void );
	bool	ddd_q_done( void );
	bool	ddd_set_go( void );
	bool	ddd_set_wait( void );
	bool	ddd_set_exit( void );
	void	ddd_set_done( void );
};

class GPC3D_PNG
{
public:
	GPCI4	xyz_near;
	char*	p_data;
	I8	n_byte;
};

//class GPC3D_Px7
//{
//public:
//	char		s_file[_MAX_PATH];
//	GPC_FIX		*p_opng, *p_gpx;
//	GPC_COLOR4		*p_color, *p_ref;
//	//I8		save_ms, updt_ms;
//	GPCI4		near_xyz;
//	I8		e0, a_min[6], a_qc[6], n_file;
//
//	GPC3D_Px7( char* p_file, GPCI4 nr, I8 n_f, I8 a0 )
//	{
//		GPM_CLR;
//		if( !p_file )
//			return;
//		n_file = n_f;
//		GPM_STRCPY( s_file, p_file );
//		ULL s;
//		//e0 = nr.ddd_e0()
//		e0 = a0;
//		near_xyz.bx7_xyz(e0);
//		if( nr.xyz() != near_xyz.xyz() )
//		{
//			GPF_CON_Write( "Mivan?" );
//		}
//
//		p_opng = ((GPC_FIX*)NULL)->fix_read( s_file, s = 0, 0 );
//		near_xyz = nr;
//	}
//	GPC3D_Px7( char* p_file, I8 n_f, I8 a0 )
//	{
//		GPM_CLR;
//		if( !p_file )
//			return;
//		n_file = n_f;
//		GPM_STRCPY( s_file, p_file );
//		//ULL s;
//		e0 = a0;
//
//		//p_gpx = ((GPC_FIX*)NULL)->fix_read( s_file, s = 0, 0 );
//		
//		near_xyz.bx7_xyz(e0);
//	}
//	GPC_COLOR4	*px7_png_color( GPC_WBx64* p_wb );
//	GPC_COLOR4	*px7_gpx_color( void );
//	GPC_FIX	*px7_png_store( GPC_WBx64* p_wb, GPC3D_Bx7* p_bx7, bool b_bgra );
//	GPC_FIX	*px7_gpx_store( GPC3D_Bx7* p_bx7, bool b_bgra );
//
//	I8	px7_heal_f2b( GPC3D_Bx7* p_bx7, U1* p_deep, I8& n_free, I8 gx_min, U1 dir );
//	I8	px7_heal_l2r( GPC3D_Bx7* p_bx7, U1* p_deep, I8& n_free, I8 gx_min, U1 dir );
//	I8	px7_heal_u2d( GPC3D_Bx7* p_bx7, U1* p_deep, I8& n_free, I8 gx_min, U1 dir );
//	I8	px7_heal_b2f( GPC3D_Bx7* p_bx7, U1* p_deep, I8& n_free, I8 gx_min, U1 dir );
//	I8	px7_heal_r2l( GPC3D_Bx7* p_bx7, U1* p_deep, I8& n_free, I8 gx_min, U1 dir );
//	I8	px7_heal_d2u( GPC3D_Bx7* p_bx7, U1* p_deep, I8& n_free, I8 gx_min, U1 dir );
//	void	px7_tree_grow( GPC3D_Bx7* p_bx7, GPC_COLOR4* p_c, int n_free );
//	void	px7_watter( GPC3D_Bx7* p_bx7, GPC_COLOR4* p_c );
//};

//class GPC3D_Bx7
//{
//public:
//	GPE_STF	a_stf_share[GPD_3D_END_Y];
//	U1	a_pat_share[GPD_3D_END_Y],
//		a_deep[0x100];
//	GPC_GX_ITM* p_gx_itm;
//	GPC3D_Px7** pp_px7;
//	GPCI4	*p_alloc, *p_free; 
//	I8	near_i8, gx_base_ms, gx_id, gx_ms, start_ms, end_ms, list_ms,
//		n_gpx, n_bx7_save, n_bx7_remove, n_bx7_light,
//		n_gpx_size,
//		n_list_size,
//		bx7_dim;
//	char	s_x7_path[_MAX_PATH],
//		*p_x7_file,
//		s_x7_name[0x100];
//	GPC_FIX*	p_px7_list;
//	GPC_TIME	time;
//	I8	*p_updt,
//		n_updt_alloc,
//		n_updt_load;
//	U2	an_mine[0x100];
//	
//	// DOWNLOAD
//	GPCI4	dwl_cnt, dwl_pos,
//		*p_dwl_list,
//		a_dwl_buff[0x100];
//	GPC_IIx64	*p_ii, *p_iie;
//	I8	n_dwl_list, n_dwl_load, n_ii;
//	// BUILD
//	GPCI4	bld_pos;
//	GPC_WBx64	*p_wbx64;
//	GPC_COLOR4	*p_stf_color;
//	I8	stf_a0, stf_b0;
//
//	GPCI4	aimom_cnt;
//	I8	aimom_n,
//		aimom_ms,
//		loop_ms,
//		n_heal,
//		n_loop_heal;
//	
//	U2	n_deep;	
//	GPE_STF	stf_plant;
//
//	GPS_FILE_ASYNC	file;
//	
//
//	GPC3D_Bx7( GPC_GX_ITM* p_gxi, I8 dim, char* p_path, char* p_file, GPC_WBx64* p_wb );
//	U4 bx7_e0( GPCI4& near_pos )
//	{
//		return ((near_pos+GPE_Px7_HALF)>>4).ddd( gpe_px7_x );
//	}
//	
//	GPC3D_Px7* bx7_find( I8 a0 )
//	{
//		if( a0 < 0 || a0 > gpe_px7_xzy )
//			return NULL;
//
//		if( !pp_px7 )
//		{
//			pp_px7 = new GPC3D_Px7*[gpe_px7_xzy];
//			if( !pp_px7 )
//				return NULL;
//			gp_zero( pp_px7, gpe_px7_xzy*sizeof(*pp_px7) );
//		}
//		
//		return pp_px7[a0];
//	}
//
//
//	GPC_FIX*	bx7_list( void );
//	GPC_FIX*	bx7_list_join( GPC_FIX* p_fix );
//	GPC_FIX*	bx7_gpx_join( GPC_FIX* p_fix, U4 e0 );
//	void bx7_updt( I8 a0 )
//	{
//		if( !this )
//			return;
//		
//		if( a0 < 0 || a0 > gpe_px7_xzy )
//		if( pp_px7[a0] )
//		if( !pp_px7[a0]->p_gpx )
//			return;
//
//		for( I8 i = 0; i < n_updt_load; i++ )
//		{
//			if( p_updt[i] == a0 )
//			{
//				if( !pp_px7[a0]->p_gpx )
//					return;
//
//				GPM_DEL( pp_px7[a0]->p_gpx );
//			}
//		}
//		if( n_updt_load >= n_updt_alloc )
//		{
//			I8	*p_kill = p_updt,
//				n_kill = n_updt_load;
//
//			n_updt_alloc = n_updt_load + 0x10;
//			p_updt = new I8[n_updt_alloc];
//			gp_memcpy( p_updt, p_kill, n_kill*sizeof(*p_updt) );
//		}
//
//		if( pp_px7[a0] )
//			GPM_DEL( pp_px7[a0]->p_gpx );
//		
//		p_updt[n_updt_load] = a0;
//		n_updt_load++;
//	}
//	GPC3D_Px7*	bx7_px7_add( GPC_WBx64* p_wbx64, GPCI4 near_xyz );
//
//	I8	bx7_build( GPC_GX_ITM& gx_itm, char* p_name, I8 dim_lay, GPCI4 play_pos, GPCI4 eye_pos, GPCV3& eye_dir );
//	void	bx7_dwl( GPCI4 dwn_pos );
//	GPC_COLOR4 bx7_stf( GPCI4 pos );
//	GPC_COLOR4 bx7_stf_cut( GPCI4 pos );
//	GPC_COLOR4 bx7_stf_past( GPCI4 pos, GPC_COLOR4 pst, bool b_new = false );
//
//	I8	bx7_MINE( GPC_GX_ITM* p_pub, GPCI4 pos1, GPCI4 pos2, GPE_NET bulet_id, GPC_GX_ITM* p_killer, GPC_GX_ITM* p_prey  );
//	void	bx7_aimom( GPC_GALAXY* p_gx, GPC_GX_ITM& gx_itm );
//	
//	I8	bx7_aimom_f2b( GPC_GALAXY* p_gx, GPC_GX_ITM& gx_itm, I8 d0, U1 dir );
//	I8	bx7_aimom_l2r( GPC_GALAXY* p_gx, GPC_GX_ITM& gx_itm, I8 d0, U1 dir );
//	I8	bx7_aimom_u2d( GPC_GALAXY* p_gx, GPC_GX_ITM& gx_itm, I8 d0, U1 dir );
//	I8	bx7_aimom_b2f( GPC_GALAXY* p_gx, GPC_GX_ITM& gx_itm, I8 d0, U1 dir );
//	I8	bx7_aimom_r2l( GPC_GALAXY* p_gx, GPC_GX_ITM& gx_itm, I8 d0, U1 dir );
//	I8	bx7_aimom_d2u( GPC_GALAXY* p_gx, GPC_GX_ITM& gx_itm, I8 d0, U1 dir );
//	
//};



class GPC_3D_X11
{
public:
	GPC_GPUx64_CONST	cns_bf;
	
	U4		*p_inst, n_inst;
	
	ID3D11Buffer	*ap_dx_vx[14],
				*p_dx_const;
	I8			qe0, div3d,
				map_id;
	//GPCV4		GR;
	U4		an_dx_vx[12], an_dx_ix[12], n_stride, n_ix_max, x11_res,
			a_hi_512[6*8*8*8], a_lo_512[8*8*8],
			a_tk1_512[6*8*8*8], a_tk2_512[6*8*8*8],
			a_bs1_512[6*8*8*8], a_bs2_512[6*8*8*8];

	U2		a_hi_512nIX[6*8*8*8], a_lo_512nIX[8*8*8],
			a_tk1_512nIX[8*8*8], a_tk2_512nIX[8*8*8],
			a_bs1_512nIX[8*8*8], a_bs2_512nIX[8*8*8];
	U1		a_512sw[8*8*8];
	
	HRESULT		hr;


	~GPC_3D_X11()
	{
		for( U1 i = 0; i < GPM_N(an_dx_vx); i++ )
			GPM_RELEASE(ap_dx_vx[i]);
	}

	bool	c2dx( GPC_WBx64* p_wb, GPC3D64* p_3d64, I8 n = 0 );
	//void	x11_render( GPC_WBx64* p_wbx64, GPC3D_BASE*p_base, I8 m, U4* p_256, U2* p_256nIX, U1* p_512sw );
	void	x11_rndr_set( GPC_WBx64* p_wbx64, GPC_GIVx64* p_giv, I8 map );

	void	x11_rndr_lo( GPC_WBx64* p_wbx64, GPC_GIVx64* p_giv, I8 map, U1 slot );
	void	x11_rndr_hi( GPC_WBx64* p_wbx64, GPC_GIVx64* p_giv, I8 map );
	void	x11_rndr_vox( GPC_WBx64* p_wbx64, GPC_GIVx64* p_giv, I8 map, U4 off );
};
#ifdef _GP_STD_THREAD
	void GPF_GIV_TRD_DEC( GPC_GIVx64* p_giv );
#else
DWORD WINAPI GPF_GIV_TRD_DEC( LPVOID p_param );
#endif
struct  GPS_IDXnOP
{
	int bas, sub, add;
	float mul;
	GPS_IDXnOP( int b, int s, int a, float m )
	{
		bas = b;
		sub = s;
		add = a;
		mul = m;
	}
};

class GPC_NODE
{
public:
	I8			id, mom, node_ms, n_undo;
	ULL			aS[0x10], nS;
	GPE_ALFA	aFUN[0x10];
	GPC_FIX		PARA,
				sht;
	U1			sNAM[0x40],
				sCRD[0x10];
	GPC_NODE()
	{
		GPM_CLR;
	}
	
	ULL node_ins( ULL slot, GPE_ALFA fun, U1 typ, void* p_void, ULL sof )
	{
		if( !slot )
		{
			aS[0] = nS = 0;
		}
		aFUN[slot] = fun;
		sCRD[slot] = typ;
		nS = slot+1;
		aS[nS] = aS[slot]+sof;
		PARA.fix_insert( (char*)p_void, sof, aS[slot], aS[nS] );
		return nS;
	}
	ULL node_ins( ULL slot, GPE_ALFA fun, U1 typ, I8 i8 )
	{
		if( !slot )
		{
			aS[0] = nS = 0;
		}
		aFUN[slot] = fun;
		sCRD[slot] = typ;
		nS = slot+1;
		aS[nS] = aS[slot]+sizeof(i8);
		PARA.fix_insert( (char*)&i8, sizeof(i8), aS[slot], aS[nS] );
		return nS;
	}
	ULL node_ins( ULL slot, GPE_ALFA fun, U1 typ, GPCV2& v2 )
	{
		if( !slot )
		{
			aS[0] = nS = 0;
		}
		aFUN[slot] = fun;
		sCRD[slot] = typ;
		nS = slot+1;
		aS[nS] = aS[slot]+sizeof(v2);
		PARA.fix_insert( (char*)&v2, sizeof(v2), aS[slot], aS[nS] );
		return nS;
	}
	void sht_out( void )
	{
		if( sht.fix_n_alloc )
			return;
		sht.fix_format( "\a ;enter enter \a ID \a \a mom \a \a ms \a \a name " );
		char	a_col[] = "\a \a %c:%s ",
				a_enter[] = "\a\r\n ;enter enter \a 0x%0.16X \a \a 0x%0.16X  \a \a 0x%0.16X \a \a %s ",
				s_fun[0x20]; 
		for( ULL i = 0; i < nS; i++ )
		{
			GPF_ALFA2STR( s_fun, aFUN[i] );
			sht.fix_format( a_col, sCRD[i], s_fun );
		}
		sht.fix_format( a_enter, id, mom, node_ms, sNAM+1 );
		GPCV2* p_v2;
		for( ULL i = 0; i < nS; i++ )
		{
			switch( sCRD[i] )
			{
				case 'y':
					p_v2 = (GPCV2*)(PARA.fix_p_alloc+aS[i]);
					sht.fix_format( "\a \a %f,%f ", p_v2->x, p_v2->y );
					break;
				default:
					sht.fix_format( "\a \a         " );
			}
		}
	}

	void sht_in( void );


};
class GPC_AIMOM
{
public:
	I8			nIX, ldIX, main_ms, on_ms,
				mxID, fndID;
	GPCI4		*pIX;
	GPC_NODE	**ppNODE;

	GPCI2		*pUP, *pSV;
	I8			ldUP, nUP,			// UPDT
				ldSV, nSV, SVms;	// save
	
	//GPC_FIX		*pUP_list;

	GPC_WBx64	*ai_p_wb;
	char		*ai_p_path,
				ai_s_path[_MAX_PATH];
	GPC_AIMOM( GPC_WBx64* p_wb, GPC_CRS& crs, char* p_gt_name )
	{
		GPM_CLR;
		ai_p_wb = p_wb;
		on_ms = crs.crs_time.on_ms;
		
		ai_p_path = ai_s_path+(crs.crs_p_path-crs.crs_s_path);
		gp_memcpy( ai_s_path, crs.crs_s_path, ai_p_path-ai_s_path );
		strcpy( ai_p_path, "main.tim" );
		GPC_FIX fix, pub;
		ULL s = -1;
		fix.fix_read( ai_s_path, s, 0 );
		if( fix.fix_n_alloc )
		{
			main_ms = GPF_STR2I8( fix.fix_p_alloc, NULL );
		}

		I8 n = sprintf( ai_p_path, "%s\\", p_gt_name );
		fix.fix_clr();
		char *p_path = ai_p_path+n;
		fix.fix_dir( ai_s_path, ai_p_path+n, "0x*.sht");
		GPC_NODE node;
		char *p_str = fix.fix_p_alloc, *p_se = p_str+fix.fix_n_alloc, *p_beg;
		while( p_str < p_se )
		{
			p_str += GPD_NINCS( p_str, "\t\r\n" );
			p_beg = p_str;
			p_str += GPD_VAN( p_str, "\t\r\n" );
			if( p_str == p_beg )
				continue;
			((U1*)gp_memcpy( p_path, p_beg, p_str-p_beg ))[p_str-p_beg] = 0;
			node.sht.fix_clr();
			node.node_ms = 0; // ha nem sikerül betölteni az idõ biztos alacsony legyen?
			node.sht.fix_read( ai_s_path, s = 0, -1 );
			if( !node.sht.fix_n_alloc )
				continue;
			node.sht_in();
			node_rec( node, true );
		}
	}
	
	~GPC_AIMOM()
	{
		GPM_DELARRAY(pIX);
		if( !ppNODE )
			return;
		for( I8 i = 0; i < ldIX; i++ )
		{
			GPM_DEL( ppNODE[i] );
		}
		GPM_DELARRAY(ppNODE);
	}
	I8 ai_ms( I8 now_ms )
	{
		return now_ms - on_ms + main_ms;
	}
	I8 ai_ms_sv( I8 now_ms )
	{
		I8 out = now_ms - on_ms + main_ms;
		on_ms = now_ms;
		main_ms = out;
		return out;
	}
	GPC_NODE* node_get( I8 id )
	{
		if( id < 0 )
			return NULL;
		GPCI4* pMOM = NULL;
		I8 i = pIX->find_IX( id, ldIX );

		if( pIX )
		if( pIX[i].x == id )
		{
			pMOM = pIX+pIX[i].y;
		} else {
			pMOM = pIX+i;
			i = ldIX;
		}

		GPC_NODE* p_is = NULL;
		if( i < ldIX )
		{
			p_is = ppNODE ? ppNODE[i] : NULL;
			if( p_is )
				return p_is;

			return ppNODE[i] = new GPC_NODE;
		}
		
		if( ldIX >= nIX )
		{
			GPCI4* p_kill = pIX;
			GPC_NODE** pp_kill = ppNODE;

			nIX = ldIX+0x10;
			pIX = new GPCI4[nIX];  
			GPM_MEMCPY( pIX, p_kill, ldIX );
			GPM_DELARRAY(p_kill);
			ppNODE = new GPC_NODE*[nIX];
			//ppNODE[ldIX] = NULL;
			GPM_MEMCPY( ppNODE, pp_kill, ldIX );
			GPM_ZEROn( ppNODE+ldIX, nIX-ldIX );
			GPM_DELARRAY( pp_kill );
		}

		ldIX = pIX->add_IX( id, 0, ldIX );
		p_is = ppNODE ? ppNODE[i] : NULL;
		if( p_is )
			return p_is;
		if( ldIX <= i )
			ldIX = i+1;
		return ppNODE[i] = new GPC_NODE;
	}
	I8			node_rec( GPC_NODE& RC, bool b_init = false );
	GPC_NODE*	node_find( GPC_NODE& RC );
};
class GPS_AIMOM_TRD
{
public:
	I8	a_tim[0x200],
		a_e[0x200];
	U2	a_free[0x110];
	U4	a_load[gpe_giv_AIM_Q0 + gpe_giv_X*2];

	GPC_GIVx64	*p_ai_giv;		// 8   8
	GPC3D		*p_3d;			// 8  16
	GPC_Qx		*p_Qx;			// 8  24
	ULL			n_dbg;			// 8  32
	U4			a_dtct_q[4];	// 16 48	

	GPC_COLOR4	*p_c0,
				*p_dtct_c4;

	char	s_file[GPD_MAX_PATH],
			*p_file;
	U1		*p_buff;

	void load_rdy( U4 QX )
	{
		if( QX >= gpe_giv_XZY3 )
			return;

		U4* p_back = a_load+a_load[gpe_giv_AIM_n_load];
					
		while( p_back > a_load )
		{
			p_back--;
			if( *p_back == QX )
				return;
		}

		a_load[a_load[gpe_giv_AIM_n_load]] = QX;
		a_load[gpe_giv_AIM_n_load]++;
	}
	I8	ai_ins( GPCI4& gr, GPC_COLOR4& stf, I8 n );
	void reset( GPC_GIVx64* p_giv )
	{
		gp_zero( this, GPM_OFF(GPS_AIMOM_TRD, p_buff) );
		p_ai_giv = p_giv;
	}
	GPS_AIMOM_TRD( void )
	{
		GPM_CLR;
	}
	~GPS_AIMOM_TRD()
	{
		GPM_DELARRAY( p_buff );
	}
};
#define GPD_RADAR_W (40+2)
class GPC_GIOx64_CACHE
{
public:
	ID3D11Buffer		*p_dx_vx,
						*p_dx_ix;
	ID3D11InputLayout	*ap_dx_lay[3];
	ID3D11VertexShader	*ap_vs[3];
	ID3D11PixelShader	*ap_ps[3];
	GPC_GIOx64_LAY		*p_lay;

	U4		ix_n, ix_s, vx_s, vx_n,
			strd, off;
	I8		n_ver, pic_id, gpu_id;
	
	GPC_GIOx64_CACHE*	scan_lay( GPC_GIOx64_LAY* p_l );
	//void		rndr( GPC_WBx64* p_wb );
};

U4* gp_80264_init( void );

class GPC_GIVx64_DTCT
{
public:
	GPC_COLOR4	a_3d64_map[GPE_Wq];
	U4			a_histi[0x100],
				iPq0, iPq1,
				iZq0, iZq1;

	GPESTF	a_mine[64*64*64],
			a_move[64*64*64],
			a_bush[64*64*64];
	char		s_info[0x400];
	GPCI4	GR, stf_gr, safe_gr,
			i2q, i2qm,
			//aCAMin[8],
			POSar, POSgr, POSin,// DRin, DRGin,
			MINin, axDRP, DRPin,
			axR, axU, axT, axZ, axSF, axSR,
			axMV, axGD,
			Zgr, Zin;
		//	MINin;
	I8	lT, lU, lR, lZ, lD, ems, tms;
	GPC_GIVx64* p_dtct_giv;
	GPC_GALAXY* p_gx;
	ULL n_updt, n_filter;
	GPCI4	a_step[3],
		*p_step;
	I8	wait_ms, n_false;
	GPCV3	z;
	GPC_GIVx64_DTCT() {};
	~GPC_GIVx64_DTCT()
	{
		p_step = NULL;
	};
	
	GPC_GIVx64_DTCT*	dtct_fill( GPC3D64& src, GPC_GIVx64* p_g );
	GPESTF*			dtct_p_stf( void );
	void				dtct_turn( GPCI4& go, GPCV3& z, I8 e_ms );
	GPCI4*			dtct_step( GPCI4& go, GPC_GX_ITM& ply, I8 gx_ms, I8 on_ms );
	//GPCI4*			dtct_step3( GPCI4& go, GPC_GX_ITM& ply, I8 gx_ms );
	//GPCI4*			dtct_step2( GPCI4& go, GPC_GX_ITM& ply, I8 gx_ms );
	bool				dtct_cam( I8 s, GPE_ALFA act );
	//bool				dtct_cam2( I8 s );
	I8				dtct_ax7_and( GPESTF* p_dtct, GPCI4& stp, GPCI4& POSin );
	I8				dtct_777( GPESTF* p_dtct, GPCI4 dir, GPCI4& POSin );

	//U4 dtct_axT_RDY( void )
	//{
	//	axF = axT.yz();
	//	axR = axF.right_yz();
	//	axSR = axR.norm_0yz0_34();
	//	axSF = axSR.left_yz();
	//	axU.cross_xyz_l( axT, axR ).null_w();
	//	return (TRGin*i2q).sum_xyz();
	//}
	GPCI4 dtct_drp( GPESTF* p_dtct, GPCI4 SCin, GPCI4 ax, GPCI4 pos, I8 n_drp );
	GPCI4 dtct_trg( GPESTF* p_dtct, GPCI4 SCin, GPCI4& gd, GPCI4& ax, GPCI4 pos, I8 mx )
	{
		gd = pos;

		I8 nD = ax.maxi_absabs_yz();
		if( !nD )
		{
			ax = SCin-pos;
			nD = ax.maxi_absabs_yz();
		}

		if( !nD )
		{
			ax.y = 1;
			nD = abs(ax.x);
			if( nD > mx )
				ax.x = ax.x*mx/nD;
			SCin = ax+pos;
			return SCin; 
		}
		nD = ax.maxi_absabs_xyz();
		if( mx < 1 )
		{
			if( !mx )
				mx = 15;
			else
				mx *= -1;
		}
		int q0, m,n;
		for( I8 i = 1; i <= mx; i++ )
		{
			SCin = pos + ((ax*i)/nD);
			m = SCin.maxi_xyz(), n = SCin.mini_xyz();
			if( m > 63 || n < 0 )
			{
				SCin = gd;
				gd = pos + ((ax*(i-1))/nD);
				break;
			}
			q0 = (SCin * i2q).sum_xyz();
			if( p_dtct[q0] )
				break;
			gd = SCin;
		}

		

		return SCin;
	}
	GPCI4 dtct_mine( GPESTF* p_dtct, GPCI4 SCin, GPCI4& DIRax, GPCI4& GDin )
	{
		GPCI4			dif, hilo, add, mine = SCin,
				const	i2q( 1, 0x1000, 0x40, 0 ),
				const	a_i4ix[] = {
										{ 1, 0, 0, 0 },
										{ 0, 1, 0, 0 },
										{ 0, 0, 1, 0 },
										{ 0, 0, 0, 1 },
									};
		dif = SCin-GDin;
		I8 d = dif.sum_abs_xyz();
		if( d == 1 )
			return mine;
		if( !d )
		{
			d = DIRax.maxi_absabs_xyz();
			if( !d )
				d = 1;
			SCin = (SCin-DIRax)+((DIRax*d+1)/d);
			dif = SCin-GDin;
		}

		U4 q0;
		hilo = DIRax.abs_xyz().hilo_xyz_ix();
		I8 *p_hl = (I8*)&hilo; 
		for( I8 i = 0; i < 3; i++ )
		{
			add = dif*a_i4ix[p_hl[i]];
			SCin = add+GDin;
			q0 = (SCin * i2q).sum_xyz();
			if( !p_dtct[q0] )
				continue;

			return SCin;
		}

		return mine;
	}
};

class GPx16
{
public:
	GPC_COLOR4	a_map[gpe_giv_xxx];
	GPC_FIX*		p_gpx;
	GPx16*		ap_lo[4];	// 2,4,8,16
};
class GPC_GIVLx64
{
	public:
		GPx16		**pp_gpx,
					*p_c;
		GPC_COLOR4	*p_c4;
		GPCI4		GR2Q;
		
		
	GPC_GIVLx64()
	{
		GPM_CLR;
		GR2Q = GPCI4( 1, gpe_giv_LINE*gpe_giv_LINE, gpe_giv_LINE, 0 );
	}
	GPCI4 giv_mine( GPCI4& gd, GPCI4 gr1, GPCI4 gr0, char* p_msk )
	{
		GPCI4 Mgr = gr1, GR;
		gd = gr0;
		GPCI4 dir = gr1-gr0;
		if( this ? !pp_gpx : true )
		{
			dir.cut_box( gr1, gr0, 0, 0x80 );
			dir.w = dir.maxi_absabs_xyz();
			if( gr1.x == 0x80 && gr1.y == 0x80 && gr1.z == 0x80 )
			{
				Mgr = gr1.xyz();
				gd = dir*(dir.w-1);
				gd /= dir.w;
				gd += gr0;
			} else 
				gd = Mgr;
			return Mgr;
		}
		dir.cut_box( gr1, gr0, 0, 0xff );
		
		I8 d = dir.maxi_absabs_xyz();
		U4 Q0, q0, cQ;
		
		for( I8 i = 1; i < d; i++ )
		{
			Mgr = ((dir*i)/d)+gr0;
			GR = Mgr/gpe_giv_LINE;
			Q0 = (GR*GR2Q).sum_xyz();
			if( cQ != Q0 )
			{
				cQ = Q0;
				p_c = pp_gpx[Q0];
				p_c4 = p_c ? p_c->a_map : NULL;
			}
			if( !p_c4 )
			{
				gd = Mgr;
				continue;
			}
						
			q0 = ((Mgr-GR)*GR2Q).sum_xyz();
			if( !p_msk[p_c4[q0].stf] )
			{
				gd = Mgr;
				continue;
			}
			gd = Mgr;
		}

	}
};
class GPC_GIVx64
{
	bool		giv_detect( GPCI4 pos_gr, I8 r );

public:
	GPC3D64			a_3d64[GPD_3DnX11];
	
	GPC_3D_X11		*ap_e0_x11[GPD_3DnX11];
	ULL				a_cnt[GPD_3DnX11], a_0[GPD_3DnX11],
					a_spd[2];
	GPCI4			a_q0_x11[GPD_3DnX11];
	U2				an_mine[0x100];

	GPC_GIO_BUFF_VX324	giv_line_vx;
	ID3D11Buffer		*gp_p_dx_line_vx;
	ULL					gp_n_dx_line_vx,
						n_line_updt, n_line_upld;
	GPC_TIME			giv_t;

	char	s_giv_file[GPD_MAX_PATH], *p_giv_file,
			s_cache[GPD_MAX_PATH], *p_cache,
			s_name[0x110],
			s_gpu1[0x110],
			s_gpu2[0x110],
			s_gpu3[0x110],
			s_gpu4[0x110],
			s_radar[80*80],
			s_aimom[80*80],
			s_DEC[0x100];
	ULL		cache_mx_ver, cache_aq_ver, cache_n_ver,
			a_head[4];
	GPC_WBx64		*p_wbx64;
	GPC_GX_ITM		*p_gx_itm;

	GPC3D_EXEC		*p_exec, *p_ee;
	I8				n_exec_alloc, n_done;
	GPCI4			exec_count;

	GPC3D			**pp_3d,
					*p_3d_shr;
	//----------------------
	//
	//	ID
	//
	//----------------------
	I8		gx_id, gio_id, 
			a_gpu_id[6], a_gpu_ver[6];

	//----------------------
	//
	//	DWNL
	//
	//----------------------
	GPE_GIV_STT		stt_dwnl;
	GPCI4			dwnl_cnt, dwnlN;
	I8				a_q0_wait_ms;
	U4				*p_QV,
					*p_q0_clnt, n_q0_clnt_load, n_q0_clnt,
					a_q0[GPD_3D_RECV_N_GPX],
					dwnl_Q0, n_DWNL_ii, // n_CACHE_ii,
					n_QV, n_cache_load, n_fatch,
					n_q0_save, n_q0_rem, n_q0_size,
					decQ0;

	//----------------------
	//
	//	SHARE
	//
	//----------------------
	GPC_Qx			*p_Q1;
	U4				Q0_shr,
					*p_gpx_updt_q0, n_gpx_updt_load, n_gpx_updt_alloc, n_DEC;
	GPC_COLOR4		a_C4_shr[GPD_3D_END_Y];

	//----------------------
	//
	//	MAP
	//
	//----------------------
	GPC_COLOR4		*p_giv_map, *p_giv_map0x10; //[gpe_giv_map];
	GPC_FIX			*ap_giv_map_fix[0x11];								
	I8				giv_map_id, n_giv_map_save, giv_map0x10_id;
	U4				map_w, map_h;
	U1				a_map_op[8];
	GPC_GIO_VX32C	*p_map_vx;
	ID3D11Buffer	*gp_p_dx_ball_vx;
	ULL				gp_n_dx_ball_vx;
	ID3D11Buffer	*gp_p_dx_ball_ix;
	ULL				gp_n_dx_ball_ix;

	//----------------------
	//
	//	LIST
	//
	//----------------------
	GPC_IIx64	*p_DWNL_ii, *p_DWNL_iie;
				//*p_CACHE_ii, *p_CACHE_iie;
	GPC_FIX	*p_q0_LST;
	U4		*p_q0_pub;	
	//----------------------
	//
	//	N
	//
	//----------------------
	ULL	n_dec, n_err, n_giv_updt, n_recvNcnt,
		n_3d64_load, n_rndr, n_dwnl_rdy, n_bld,
		n_rad, n_mine, n_q0_pub_load, n_q0_pub_alloc, n_store,

		n_aimom_main, n_aimom_dbg, n_aimom_dbg_cnt,

		n_heal_aq, n_dtct, n_road, n_x11; //, n_dif_ent_aq;

	GPCI4	dtct_mxmn, a_road[0x16];
	U4		a_dtct_q[2];
	GPC_COLOR4		*p_dtct_c4;
	GPC_GIVx64_DTCT	*p_dtct_xc0; 

	//----------------------
	//
	//	ms
	//
	//----------------------
	I8	dwnl_ms, on_ms, bld_ms, strt_ms, end_ms, list_ms,
		giv_gx_ms, prv_ms,
		aimom_ms, aimom_aq_ms; //, POSms;
	
	//----------------------
	//
	//	POS
	//
	//----------------------
	GPCI4	ply_ar, ply_gr, bld_GR,
			eye_ar, eye_gr,
			lig_ar, lig_gr,
			radar_gr, cam_gr,
		
			mine_gr, drop_gr, gr_gr,
			
			trg_gr, aft_gr, bef_gr, pos_gr,
			rfPOSgr, rfMINgr,
			POSgr, Zgr, TRGgr,
			C0gr, C1gr,
			C0ar, C1ar,
			WRKgr;

	I8 giv_dim, giv_half, ply_alt, core_alt, sky_alt;
	U4	ply_Q, lig_Q, bld_Q, eye_Q_;
	GPC_COLOR4	mine_c4;	
	GPCV4	nr;
	HRESULT	hr; 
	//----------------------
	//
	//	POINTER
	//
	//----------------------
	GPC_GALAXY* p_giv_gx;
	//----------------------
	//
	//	FILE
	//
	//----------------------
	GPS_FILE_ASYNC	giv_file;
	//----------------------
	//
	//	AiMom
	//
	//----------------------
	

	char	a_msk_lig[0x100],
		a_msk_bush[0x100],
		a_msk_ent[0x100],
		a_ent_take[0x100],
		a_msk_lev[0x100],
		a_msk_solid[0x100],
		a_msk_moon[0x100],
	
	// határok
		// gyogyításkor
		a_ent_hl_chg_dw[0x100],
		a_ent_hl_chg_up[0x100],
		// romboláskor
		a_ent_dm_chg_dw[0x100],
		a_ent_dm_chg_up[0x100];
	// anyag
	GPESTF	// gyogyításkor
		a_stf_hl_chg_dw[0x100],
		a_stf_hl_chg_up[0x100],
		// romboláskor
		a_stf_dm_chg_dw[0x100],	
		a_stf_dm_chg_up[0x100];
	// IDÕ
	I8	a_tim_hl_up[0x100],		// gyogyulás idõ
		a_tim_hl_dw[0x100],		// gyogyulás idõ
		a_tim_hl_up_free[0x100],	// gyogyulás idõ
		a_tim_hl_dw_free[0x100],	// gyogyulás idõ
		a_ent_shield[0x100],		// védekezés
		an_stf_chg[0x100];
	int		a_stf_gio_id1[0x100],
			a_stf_gio_id2[0x100],
			a_stf_gio_id3[0x100];
	GPC_GIOx64_CACHE	a_stf_cache1[0x100],
						a_stf_cache2[0x100],
						a_stf_cache3[0x100];


	ULL b_q4_main, b_q4_dbg;
	GPS_AIMOM_TRD a_q4_main[4];
	GPS_AIMOM_TRD a_q4_dbg[4];


	HANDLE			a_trd[3];
	ID3D11Buffer	*gp_p_dx_c_stf;
	GPCV4			cv4_stf;
	//GPCI4	ai_GR, ai_GR_ref;
	GPC_IIx64		a_ii27[27];

	GPE_WIP		wip_DEC;
	std::thread	*pt_DEC;
	std::mutex	mtx_DEC;
	
	


	GPC_GIVx64( GPC_WBx64* p_wb, GPC_GX_ITM& gx_itm, I8 d = gpe_giv_1m, char* p_file = NULL, I8 n_file = 0 )
	{
		//GPM_CLR;
		gp_zero( this, GPM_OFF( GPC_GIVx64, mtx_DEC ) );
		//ai_GR.GRsid = 6;

		a_map_op[0] = 1;

		giv_half = d/2;
		giv_dim = giv_half*2;
		p_wbx64 = p_wb;
		p_gx_itm = &gx_itm;
		gx_id = gx_itm.id;
		gio_id = -1;
		pp_3d = new GPC3D*[gpe_giv_XZY3];
		gp_zero( pp_3d, sizeof(*pp_3d)*gpe_giv_XZY3 );
		core_alt = gpe_giv_cr*giv_dim;
		sky_alt = gpe_giv_sky*giv_dim;
		p_giv_file = s_giv_file;
		Q0_shr = gpe_giv_XZY;
		// TAKE +------------------------------
		gp_memset_u1( a_ent_take, 0x70, sizeof(a_ent_take) );

		// LIG +------------------------------
		gp_memset_u1( a_msk_lig, -1, sizeof(a_msk_lig) );
		a_msk_lig[gp_stf_spc] = a_msk_lig[gp_stf_free] = a_msk_lig[gp_stf_air] = a_msk_lig[gp_stf_watter] = a_msk_lig[gp_stf_grass] = 0;
		gp_memcpy( a_msk_solid, a_msk_lig, sizeof(a_msk_solid) );
		
		// MOON LIGHT +-------------------------------
		GPM_ZERO( a_msk_moon );
		a_msk_moon[gp_stf_grass] = 2;
		a_msk_moon[gp_stf_free] = 4;

		// BUSH +-------------------------------
		GPM_ZERO( a_msk_bush );
		a_msk_bush[gp_stf_grass] = 1;
		// STF_xx_CHG +------------------------------
		for( U2 i = 0; i < 0x100; i++ )
			a_stf_hl_chg_dw[i] = (GPESTF)i;
		gp_memcpy( a_stf_hl_chg_up, a_stf_hl_chg_dw, sizeof(a_stf_hl_chg_dw) );
		gp_memcpy( a_stf_dm_chg_dw, a_stf_hl_chg_dw, sizeof(a_stf_hl_chg_dw) );
		gp_memcpy( a_stf_dm_chg_up, a_stf_hl_chg_dw, sizeof(a_stf_hl_chg_dw) );

		// ENT_xx_CHG +------------------------------
		gp_memset_u1( a_ent_hl_chg_dw, -0x40, sizeof(a_ent_hl_chg_dw) );
		gp_memset_u1( a_ent_hl_chg_up, -0x30, sizeof(a_ent_hl_chg_up) );
		gp_memset_u1( a_ent_dm_chg_dw,  0x30, sizeof(a_ent_dm_chg_dw) );
		gp_memset_u1( a_ent_dm_chg_up,  0x40, sizeof(a_ent_dm_chg_up) );


			// DAMAGE: WEED - ROAD - BED
			a_ent_dm_chg_dw[gp_stf_weed] = -30;
			a_stf_dm_chg_dw[gp_stf_weed] = gp_stf_road;
			a_ent_dm_chg_dw[gp_stf_road] = -60;
			a_stf_dm_chg_dw[gp_stf_road] = gp_stf_bed;
			// HEAL: BED - ROAD - WEED
			a_ent_hl_chg_up[gp_stf_bed] = -40;
			a_stf_hl_chg_up[gp_stf_bed] = gp_stf_road;
			a_ent_hl_chg_up[gp_stf_road] = -10;
			a_stf_hl_chg_up[gp_stf_road] = gp_stf_weed;
		

		// LEV +------------------------------
		gp_zero( a_msk_lev, sizeof(a_msk_lev) );
		// HEAL TIME +------------------------------
		I8 i8 = 5*60*sec2ms;
		GPF_MEM_set( a_tim_hl_up_free, GPM_N(a_tim_hl_up_free), &i8, sizeof(i8) );
		gp_memcpy( a_tim_hl_dw_free, a_tim_hl_up_free, sizeof(a_tim_hl_dw_free) );
		for( U2 i = 0; i < 0x100; i++ )
		{
			a_tim_hl_up[i] = a_tim_hl_up_free[i]*4;
			a_tim_hl_dw[i] = a_tim_hl_dw_free[i]*4;
		}
		
		// SHIELD TIME +------------------------------
		i8 = sec2ms/2;
		GPF_MEM_set( a_ent_shield, GPM_N(a_ent_shield), &i8, sizeof(i8) );
		
		gp_memset_u1( a_gpu_id, -1, sizeof(a_gpu_id) );
		if( !*s_gpu1 )
			GPM_STRCPY( s_gpu1, "stone" );
		if( !*s_gpu2 )
			GPM_STRCPY( s_gpu2, "take" );
		if( !*s_gpu3 )
			GPM_STRCPY( s_gpu3, "bush" );

		// ENT +------------------------------
		gp_memset_u1( a_msk_ent, -0x40, sizeof(a_msk_ent) );
		
		Zgr.w = POSgr.w = -1;
		giv_stf_init();
		giv_jit_init( p_wbx64 );
		p_cache = s_cache;
		if( !p_file )
		{
			giv_UPL_ix(0xFFF0);

			#ifdef GPD_VOXVOX
				giv_upl_vox( false );
				giv_upl_msk();
			#endif

			giv_stf_gio_init();
			giv_gpx_cache_init();
			return;
		}
		GPM_STRCPY( s_giv_file, p_file );
		p_giv_file += n_file;
		p_giv_file += sprintf( p_giv_file,"sys_%lld_%lld_%lld\\%0.16I64X_giv\\", gx_itm.star_xyz.x, gx_itm.star_xyz.y, gx_itm.star_xyz.z, gx_itm.id );
		
		
	}
	bool		giv_QV_mine( void );
	void		giv_gpx_ii_refresh( void );
	U4*			giv_QV_init(void);
	ULL			giv_gpx_cache_load( char* p_buff, U4 Q0, ULL ver );
	void		giv_gpx_cache_init(void);
	void		giv_jit_init( GPC_WBx64* p_wb );
	void		giv_stf_gio_init( void );
	void		giv_stf_init( void );
	void giv_stf(	GPESTF stf, I8 shield,
					char ent_dm_chg_dw, GPESTF stf_dm_chg_dw,
					I8 tim_hl_up,	char ent_hl_chg_up, GPESTF stf_hl_chg_up,
					I8 tim_hl_dw,	char ent_hl_chg_dw, GPESTF stf_hl_chg_dw,
					char ent_dm_chg_up, GPESTF stf_dm_chg_up
	)
	{
		// SHIELD
		a_ent_shield[stf] = shield;

		//-----------------------------------
		//	NEGATIV
		//-----------------------------------
		// DAMAGE DOWN
		a_ent_dm_chg_dw[stf] = ent_dm_chg_dw;
		a_stf_dm_chg_dw[stf] = stf_dm_chg_dw;

		// HEAL UP
		a_tim_hl_up[stf] = tim_hl_up;
		a_ent_hl_chg_up[stf] = ent_hl_chg_up;
		a_stf_hl_chg_up[stf] = stf_hl_chg_up;

		//-----------------------------------
		//	POZITIVE
		//-----------------------------------
		// DAMAGE UP
		a_ent_dm_chg_up[stf] = ent_dm_chg_up;
		a_stf_dm_chg_up[stf] = stf_dm_chg_up;

		// HEAL DWN
		a_tim_hl_dw[stf] = tim_hl_dw;
		a_ent_hl_chg_dw[stf] = ent_hl_chg_dw;
		a_stf_hl_chg_dw[stf] = stf_hl_chg_dw;
	}
	GPC_FIX* giv_SND_LST( GPC_FIX* p_fix, U1 n )
	{
		GPC_FIX* p_list = giv_LIST( n );
		if( !p_list )
			return p_fix;
		ULL s;
		return p_fix->fix_insert( p_list->fix_p_alloc, p_list->fix_n_alloc, s = -1, 0 );
	}
	GPC_FIX*	giv_LIST( U1 n );
	U1			giv_MAP0x10( GPC_GALAXY* p_gx, char* p_byte, ULL n_byte, U1 n0x10 );
	bool		giv_MAP( char* p_byte, ULL n_byte );
	GPC3D*		giv_SHARE( U4 e0 );
	GPC3D*		giv_SHARE_ALLOC( GPC_COLOR4* p_C4_shr = NULL, I8 dim = gpe_giv_1m );
	
	GPC3D*		giv_ALLOC( U4 e0 );
	GPC3D*		giv_GPX_UPDT(I8& n_alloc, I8& n_free, void* p_byte, I8 n_byte, U4 q0 );
	GPC_GIOx64*	giv_gio( I8 dim_lay );
	GPC_FIX*		giv_gpx_join( GPC_FIX* p_fix, U4 q0 );

	GPC_FIX*		giv_pub_join( GPC_FIX* p_fix, U4* p_q0, U4 n_pub );

	GPC_FIX*		giv_GET( GPC_FIX* p_send );
	void			giv_DWNL( GPCI4 dwn_pos );

	//void		giv_aimom2( I8 n_ms );
	//I8	giv_ins( GPCI4 gr, GPC_COLOR4* p_stf, GPS_AIMOM_TRD* p_trd );
	U4			giv_ai_roll( U4 QX, GPC_COLOR4* p_A0, U4 src, GPS_AIMOM_TRD* p_trd );
	U4			giv_ai_tree2_bough( U4 QX, GPC_COLOR4* p_A0, U4 dst, GPS_AIMOM_TRD* p_trd );
	U4			giv_ai_tree2_bud( U4 QX, GPC_COLOR4* p_A0, U4 dst, GPS_AIMOM_TRD* p_trd );
	//U4		giv_ai_tree( GPC_COLOR4* p_A1, U4 QU, GPC_COLOR4* p_A0, U4 QX, U4 dst, GPS_AIMOM_TRD* p_trd );
	U4			giv_ai_fruit_core( GPC_COLOR4* p_A1, U4 QU, GPC_COLOR4* p_A0, U4 QX, U4 dst, GPS_AIMOM_TRD* p_trd );
	U4			giv_ai_col( U1* p_buff, U2* p_free, U4* p_load, I8 *p_tim, I8 *p_e, GPS_AIMOM_TRD* p_trd  );
	char		giv_ai_mom_map0x10( bool b_init );
	I8			giv_ai_mom( I8 n_ms, I8 n_aimom );
	I8			giv_ai_mom_QyzSID( I8 n_aimom, GPCI4 QyzSID, GPS_AIMOM_TRD* p_q4 );

	I8			giv_attack( U1* p_buff, GPCI4 prey_gr, GPCI4 dr, I8 dam );
	
	I8			giv_MINE( U1* p_buff, GPC_GX_ITM* p_pub, GPCI4 pos1, GPCI4 pos2, GPCI4 prey_gr, GPE_NET bulet_id, GPC_GX_ITM* p_killer, GPC_GX_ITM* p_prey  );
	I8 __vectorcall	giv_BUILD( GPCI4 sun_pos, GPCI4 play_pos, GPCI4 eye_pos, bool b_grav, bool b_fly ); //, GPCV3& eye_dir );
	
	GPC_COLOR4*	giv_GET_gr( GPCI4 pos_gr, I8 n );
	GPC_COLOR4*	giv_MINE_gr( U1* p_buff, GPCI4 pos_gr, I8 n, GPS_AIMOM_TRD* p_trd );
	GPC_COLOR4*	giv_GET_ar( GPCI4 posNr, I8 n );
	GPCI4 giv_ar2gr( GPCI4 air, bool b_dbg )
	{
		return GPCI4( air, 1, giv_dim, b_dbg );
	}
	GPCI4 giv_gr2ar( GPCI4 gr )
	{
		return gr.gr2ar( giv_dim, 1 );
	}
	GPCI4	giv_push_up( GPCI4 trg_ar );
	GPCI4	giv_detect2( GPCI4 trg_ar, GPCI4 pos_ar, bool b_dbg = false );
	GPCI4	giv_detect_GR( GPCI4 gr1, GPCI4 gr0, I8 r, I8 mnbb, bool b_dbg );
	GPCI4	giv_detect_ar( GPCI4 trg_ar, GPCI4 pos_ar, I8 r, bool b_dbg = false );
	char*	giv_radar( GPCI4 trg, GPCI4 posNr );
	
	void		giv_FTCH( GPC3D64* p_3d64_dst );
	void		giv_BLD_strtNrdy( I8 n_exec );
	bool		giv_BLD_strtNrdy_try( I8 n_exec );
	
	
	void		giv_UPL_ix( U4 n_ix );
	#ifdef GPD_VOXVOX
		U4		giv_upl_vox( bool b_sw );
		void		giv_upl_msk( void );
	#endif
	void		giv_UPL_vx_off( GPC3D64* p_3d64 );
	U4		giv_UPL_vx( GPC3D64* p_3d64 );
	void		giv_RNDR( GPC_GIOx64_LAY* p_lay, I8 map );
	void		giv_RNDR_q4( GPC_GIOx64_LAY* p_lay, I8 map, I8 q4 );
	
	GPC3D* giv_FIND( U4 Q0 )
	{
		if( this ? Q0 >= gpe_giv_XZY3 : true )
			return NULL;
		
		return pp_3d[Q0];
	}
	void giv_BLD_reset( I8 n_x11 = 0)
	{
		//build64_start.null();
		for( ; n_x11 < GPD_3DnX11; n_x11++)
		{
			a_q0_x11[n_x11].e0 = 0;
		}
		//decQ0 = gpe_giv_XZY3;
	}

	
	#ifdef _GP_STD_THREAD
		bool giv_gpx_DEC_wait(void)
		{
			if( !mtx_DEC.try_lock() )
				return false;
			else if( wip_DEC != GPE_WIP_TRD_DONE )
			{	
				bool b_init = wip_DEC == GPE_WIP_TRD_INIT || wip_DEC == GPE_WIP_BOSS_USE;
				mtx_DEC.unlock();
				return b_init;
			}

			GPCI4 a_gr[2];
			a_gr[0].Q2gr(ply_Q);
			a_gr[1].Q2gr(decQ0);
			I8 d = (a_gr[0].gr2ar(gpe_giv_1m,gpe_giv_1m)-a_gr[1].gr2ar(gpe_giv_1m,gpe_giv_1m)).len_xyz(); 

			sprintf( s_DEC+0x38, ":d%lld %lld=%lld/%lld", d, (n_dec*sec2ms)/on_ms, n_dec, on_ms/sec2ms ); // = 0;
			GPF_CON_pos( 0, GPE_CLST_DEC_ );
			GPF_CON_Write( s_DEC );

			
			//if( (a_ar[0]-a_ar[1]).len_xyz() < 16 )
			if( d < 64 )
				giv_BLD_reset( 0 );
			else
				giv_BLD_reset( 1 );
			decQ0 = gpe_giv_XZY3;
			n_gpx_updt_load = 0;
			wip_DEC = GPE_WIP_BOSS_USE;
			mtx_DEC.unlock();
			return true;
		}

		bool giv_DEC_strt(void)
		{
			n_DEC++;
			s_DEC[(n_DEC)%0x40] = ' ';
			s_DEC[(n_DEC+1)%0x40] = '-';
			s_DEC[(n_DEC+2)%0x40] = '+';
		
			if( p_gpx_updt_q0 ? !n_gpx_updt_load : true )
				return false;

			if( !pt_DEC )
			{
				wip_DEC = GPE_WIP_BOSS_USE;
				decQ0 = gpe_giv_XZY3;

				pt_DEC = new std::thread( GPF_GIV_TRD_DEC, this );
				if( !pt_DEC )
					return false;
				mtx_DEC.lock();
				//pt_DEC->join();
			}
			else if( !mtx_DEC.try_lock() )
				return false;
			else if( wip_DEC != GPE_WIP_BOSS_USE )
			{
				mtx_DEC.unlock();
				return false;
			}

			GPC3D* p_3d_alloc = NULL;
			for( U2 i = 0; i < n_gpx_updt_load; i++ )
			{
				p_3d_alloc = giv_ALLOC( p_gpx_updt_q0[i] );
			}

			s_DEC[n_DEC%0x40] = 'D';
			wip_DEC = GPE_WIP_BOSS_SETGO;
			mtx_DEC.unlock();
			return true;
			
		}
	#else
		bool giv_gpx_DEC_wait(void)
		{
			if (this ? !h_DEC : true)
				return false;
			GPM_STRCPY( s_DEC+0x40, ":dec" ); // = 0;
			GPF_CON_pos( 0, GPE_CLST_DEC_ );
			GPF_CON_Write( s_DEC );

			giv_BLD_reset();
			WaitForSingleObject( h_DEC, INFINITE);
			GPM_CLOSE( h_DEC );
			h_DEC = 0;
			n_gpx_updt_load = 0;
			return true;
		}
		bool giv_DEC_strt(void)
		{
			n_DEC++;
			s_DEC[(n_DEC)%0x40] = ' ';
			s_DEC[(n_DEC+1)%0x40] = '-';
			s_DEC[(n_DEC+2)%0x40] = '+';
		
			if( h_DEC )
				return false;

			if( p_gpx_updt_q0 ? !n_gpx_updt_load : true )
				return false;

			GPC3D* p_3d_alloc = NULL;
			for( U2 i = 0; i < n_gpx_updt_load; i++ )
			{
				p_3d_alloc = giv_ALLOC( p_gpx_updt_q0[i] );
			}

			s_DEC[n_DEC%0x40] = 'D';
			h_DEC = CreateThread( NULL, 0, GPF_GIV_TRD_DEC, this, 0, NULL);
			return true;
			
		}
	#endif
	

	I8 giv_n_FTCH(void)
	{
		exec_count.n_req = 0;
		for (I8 i = 0; i < exec_count.n_recv; i++)
		{
			if( a_3d64[i].n_pipe != GPE_3D_PIPE_FETCH )
				continue;

			a_3d64[i].n_pipe = GPE_3D_PIPE_EXEC;
			exec_count.n_req++;
		}
		return exec_count.n_req;
	}
	void giv_DONE( U1* p_done_msk, GPC3D* p_f1 )
	{
		if( !p_f1 )
			return;

		//----------------------------------------------
		//		F1
		//---------------------------------------
		if( p_f1->n_updt == p_f1->n_done )
			return;

		p_f1->n_done = p_f1->n_updt;
		//if( !p_3d_share->p_3d1 )
		//{
		//	p_3d_share->p_3d1 = new GPC3DLO(1);
		//}
		//gp_memcpy( p_3d_share->p_3d1->a_3d, p_3d_share->a_3d, sizeof(p_3d_share->a_3d) );
		//gp_memcpy( p_3d_share->p_3d1->a_light, p_3d_share->a_light+GPE_3D_LIGHT_THIS, sizeof(p_3d_share->a_3d) );
		//p_3d_share->p_3d1->set_irq();

		//----------------------------------------------
		//		F2
		//---------------------------------------
		// W 0x70000	 0x70000
		// Y 0x0FC00	 0x0F800
		// Z 0x003F0	 0x003E0
		// X 0x0000F	 0x0000E
		//		---------
		//		 0x7FBEE
		I8	q2 = p_f1->Q0&0x7FBEE,
			off2 = p_f1->Q0 - q2;
		GPCI4 xyz_lo;
		GPC3D* p_f2 = pp_3d[q2];
		if( !p_f2 )
		{
			p_f2 = pp_3d[q2] = new GPC3D( q2, this );
		}
		if( !p_f2->ap_Qx[1] )
		{
			p_f2->ap_Qx[1] = new GPC_Qx(2);
		}
		xyz_lo.Q2G( off2 );
		xyz_lo <<= 3;
		//xyz_lo &= 8;
		//off2 <<= 3;
		//GPCI4 xyz_lo( off2&8, (off2>>16)&8, (off2>>8)&8, 0 );
		I8 a2 = xyz_lo.dddx10();
		p_f2->ap_Qx[1]->lo_fill( p_done_msk, a2, p_f1->ap_Qx[0] ? p_f1->ap_Qx[0]->a_C4 : NULL, a_msk_lig )->set_irq();
		if( p_done_msk )
		{
			
		}
		//----------------------------------------------
		//		F4
		//---------------------------------------
		// W 0x70000	 0x70000
		// Y 0x0FC00	 0x0F000
		// Z 0x003F0	 0x003C0
		// X 0x0000F	 0x0000C
		//		---------
		//		 0x7F3CC
		U4	e4 = p_f1->Q0&0x7F3CC,
			off4 = p_f1->Q0-e4;

		GPC3D* p_f4 = pp_3d[e4];
		if( !p_f4 )
		{
			p_f4 = pp_3d[e4] = new GPC3D( e4, this );
		}
		if( !p_f4->ap_Qx[2] )
		{
			p_f4->ap_Qx[2] = new GPC_Qx(4);
		}
		
		xyz_lo.Q2G( off4 );
		xyz_lo <<= 2;
		xyz_lo &= 0xc;

		I8 a4 = xyz_lo.dddx10();
		p_f4->ap_Qx[2]->lo_fill(  p_done_msk, a4, p_f2->ap_Qx[1]->a_C4+a2, a_msk_lig, 4, GPE_3D_LINE )->set_irq();

		////----------------------------------------------
		////		F8
		////---------------------------------------
		//// W 0x70000	 0x70000
		//// Y 0x0FC00	 0x0E000
		//// Z 0x003F0	 0x00380
		//// X 0x0000F	 0x00008
		////		---------
		////		 0x7E388
		//
		//I8	e8 = p_f1->Q0&0x7E388,
		//	off8 = p_f1->Q0-e8;
		//
		//GPC3D* p_f8 = pp_3d[e8];
		//if( !p_f8 )
		//{
		//	p_f8 = pp_3d[e8] = new GPC3D( e8, this );
		//}
		//if( !p_f8->ap_Qx[3] )
		//{
		//	p_f8->ap_Qx[3] = new GPC_Qx(8);
		//}
		//
		//xyz_lo.Q2G( off8 );
		//xyz_lo <<= 1;
		//xyz_lo &= 0xe;
		//I8 a8 = xyz_lo.dddx10();
		//
		//p_f8->ap_Qx[3]->lo_fill(  p_done_msk, a8, p_f4->ap_Qx[2]->a_C4+a4, a_msk_lig, 2, GPE_3D_LINE )->set_irq();

	}
	void giv_PUB( U4 q0 );
	GPC_COLOR4* giv_paste_stf( U1* p_buff, U4 q0, GPESTF stuff );
	GPC_COLOR4 giv_switch_stf( GPCI4 src_gr, GPC_COLOR4* p_this );
};


class GPC_GX_TREE
{
public:
	GPC_GX_ITM	*p_find;
	GPCI4		*p_alloc;
	I8		n_load, n_alloc;
	~GPC_GX_TREE()
	{
		GPM_DELARRAY(p_alloc);
	}
	void gx_tree_reset( void )
	{
		GPM_DELARRAY(p_alloc);
		GPM_CLR;
	}
};

class GPC_GALAXY
{
public:
	char		gx_s_file[_MAX_PATH],
				*gx_p_file,
				gx_s_path[_MAX_PATH],
				*gx_p_path,
				gx_s_pub[0x10000];
	U1			gx_a_enc_buff[GPE_PACK_ENC_BUFF];

	GPC_GX_ITM	*gx_p_eye_item,
				*gx_p_star_pub;

	I8			gx_n_id, load_t,
				gx_ms, on_t, gx_ms_prev,
				gx_on_mx, gt_dwnl_ms;

	GPC_GX_ITM	*gx_p_alloc;	// szülõ szerint sorrendezve
	I8			gx_n_alloc;

	GPC_GX_TREE	gx_idx_player,
				gx_idx_V_star,
				gx_idx_id,
				gx_idx_id_updt_save,
				gx_idx_id_updt_send;
	
	I8			gx_save_t,
				*gx_p_empty,	// azok amik felletek szabadítva idõ közben
				gx_n_empty,
				gx_n_build,
				gx_n_pack,
				gx_n_updt_save,
				gx_n_updt;
	
			//gx_itm_sizeof,
			//gx_itm_id_off,
			//gx_itm_star_off;
	GPC_GX_LOD	lod,
				send_list;
	I8			user_id, gis_id;
	GPC_CRS		*gx_p_crs_pub;

	GPCI4		*p_buy;
	I8			n_buy_load, n_buy;
	GPC_WBx64*	p_wbx64;

	GPC_CRS		*p_sys_crs,
				*p_ply_crs,	
				*p_hot_crs,
				*p_wb_crs,
				*p_wb_res_crs,
				*p_wb_sel_crs,
				*p_wb_to_crs,
				*p_store_crs;

	GPC_TAB		*p_sys_tab,
				*p_ply_tab,
				*p_wb_tab,
				*p_wb_res_tab,
				*p_wb_sel_tab,
				*p_wb_to_tab,
				*p_store_tab;

	I8			sel_qc,
				wb_t;

	//U1*		gx_p_combi;
	
	GPC_GX_MINE	gx_mine;
	GPCI4		gx_puff;

	I8		dwn_id_3d_prev,
			dwn_n;

	GPC_TXTx64*	p_gx_txt;
	// TXT ---------------------
	//I8		txt_id;
	//GPC_COLOR4		*p_txt_buff,
	//		*p_txt_ref,
	//		txt_color;
	//int		txt_w, txt_h, txt_size;
	//GPCI4		txt_xywh,
	//		STFxyBIGzw;

	// SERV BULLETT
	U4			a_err[0x20],
				n_err;
	GPC_COLOR4	*ap_bul27[27];

	I8			ddd_dwnl_pic_id,
				vox_dwnl_pic_id;
	GPC3D64		*p_vox;

	GPCI4		aimom_cnt;
	GPC_GX_ITM*	p_aimom_pub;
	GPCSTF_CMBI	a_gx_cmbi[0x100];
	
	I8			n_stp_brk;

	// BALL ---------------------
	
	//GPC_GIO_VX32C	*p_ball, *p_balle;
	//U4				ball_w, ball_h,
	//				ball_y, ball_z,
	//				map_y, map_z,
	//				n_ball;
	GPC_GIO_BUFF_VX324* p_gx_ball;
	//U2				*p_idx, n_idx;				

	void gx_init_cmbi( void );
	void gx_reset( I8 on_ms, GPC_WBx64* p_wb );
	GPC_GALAXY( I8 on_ms, GPC_WBx64* p_wb )
	{
		gx_reset( on_ms, p_wb );
	}
	GPC_CRS* gx_crs_pub( void );
	GPC_GALAXY( char* p_path, I8 on_ms, GPC_WBx64* p_wb )
	{
		gx_reset( on_ms, p_wb );

		gx_p_file = GPM_STRCPY( gx_s_file, p_path );
		gx_p_file += GPD_STRLEN( gx_p_file );
		if( gx_p_file[-1] != '\\' )
		{
			gx_p_file += sprintf( gx_p_file ,"\\" );
		}

		if( GPF_ACE(gx_s_file, 4) < 0 )
		{
			_mkdir( gx_s_file );
		}

		on_t = on_ms;
		// load_t = 0; //de ilyenkor constructor-ban eleve 0;

		sprintf( gx_p_file, "%s", "root.txt" );
		ULL s = 0;
		GPC_FIX* p_fix = ((GPC_FIX*)NULL)->fix_read( gx_s_file, s, -1 );
		
		if( p_fix )
		if( char* p_str = p_fix->fix_p_alloc )
		{
			char* p_end = p_str + p_fix->fix_n_alloc;
			p_str += GPD_VAN( p_str, GPD_09 );
			I8* p_i8 = &gx_n_id, *p_i8b = p_i8;
			while( p_str < p_end )
			{
				*p_i8 = strtoll( p_str, &p_str, 10 );
				if( p_i8-p_i8b > 0 )
					break;
				p_i8++;
				p_str += GPD_VAN( p_str, GPD_09 );
			}
		}

		GPM_DEL( p_fix );


		gx_crs_pub();
	}

	~GPC_GALAXY()
	{
		for( I8  i = 0; i < gx_n_alloc; i++ )
		{
			gx_p_alloc[i].gx_clear();
		}
		GPM_DELARRAY(gx_p_alloc);
	}

	GPC_GX_ITM* gx_itm_get( I8 idx )
	{
		if( this ? !gx_p_alloc : true )
			return NULL;

		if( idx < 0 || idx > gx_n_alloc )
			return NULL;

		return  gx_p_alloc+idx;

	}

	GPC_GX_ITM*	gx_find_star( GPCI4 star_xyz );
	GPC_GX_ITM*	gx_find_player( char* p_str_player, char* s_str_pasw );
	GPC_GX_ITM*	gx_itm_idx_save( GPC_GX_ITM* p_star, GPC_GX_ITM* p_chk, GPC_AN& an );
	//U1*		gx_combi( void );
	U1		gx_combi_stuff( GPESTF* p_combi );
	GPC_GALAXY*	gx_unp( char* p_src, char* p_end, GPC_GATEx64* p_accept, I8 on_ms, bool b_debug  );
	GPC_GALAXY*	gx_save( void );
	//GPC_GX_ITM* gx_add( char* p_name, I8 type_id );
	GPC_GX_ITM*	gx_find_str( GPC_GX_TREE& idx, I8 offs, char* p_name, I8 W = -100, bool b_alloc = false );
	GPC_GX_ITM*	gx_find_equ( GPC_GX_TREE& idx, I8 offs,  GPCI4& i4, I8 W = -100, bool b_alloc = false );
	GPC_GX_ITM*	gx_find_i8( GPC_GX_TREE& idx, I8 offs, I8 i8, I8 W = -100, bool b_alloc = false );
	char GPC_GX_ITEM_migi( GPC_GX_ITM* p_item, I8 mom_id )
	{
		if( (p_item < gx_p_alloc) || ( p_item-gx_p_alloc >= gx_n_alloc ) )
			return 0;

		if( p_item->n_mom_idx > -1 )
		if( mom_id == p_item->mom_id )
			return 2; // minden rendben ez stimmel

		GPC_GX_ITM* p_o_mom = NULL;
		if( p_item->n_mom_idx > 0 && p_item->n_mom_idx < gx_n_alloc  )
		{
			p_o_mom = gx_p_alloc+p_item->n_mom_idx;
		} else {
			p_o_mom = gx_find_i8( gx_idx_id, GPM_OFF( GPC_GX_ITM, id ), p_item->mom_id );
		}

		if( p_o_mom ? ( p_o_mom->id == p_item->mom_id ) : false )
		{
			if( p_item->n_mom_idx < 0 )
			if( mom_id == p_item->mom_id )
			{
				p_o_mom->child.i8_add( p_item-gx_p_alloc );
				p_item->n_mom_idx = p_o_mom-gx_p_alloc;
				return 3;
			}
			p_o_mom->child.i8_sub( p_item-gx_p_alloc );
		}
		p_item->n_mom_idx = -1;

		GPC_GX_ITM* p_new_mom = gx_find_i8( gx_idx_id, GPM_OFF( GPC_GX_ITM, id ), mom_id );
		if( !p_new_mom )
			mom_id = -1;
		else if( !p_new_mom->child.i8_add( p_item-gx_p_alloc ) )
			return 4;
		
		if( (p_item->mom_id = mom_id) < 0 )
		{
			p_item->n_mom_idx = -1;
		} else 
			p_item->n_mom_idx = p_new_mom-gx_p_alloc;

		return 5;
	}

	GPC_GALAXY*	gx_play( GPC_GATEx64* p_gate, I8 on_ms );
	//GPC_GALAXY*	gx_ready( void );
	void		gx_aimom( GPC_GATEx64* p_gt_aimom, I8 on_ms );
	char		gx_s_con_buff[10240];
	
	//GPCI4		gx_printf( char *p_format, ... );
};


class GPC_XP
{
public:
	union
	{
		struct
		{
			GPC_COLOR4	rcnl;
			U4	rn, rpi, rpn;
			char	a_buf[0x590];
		};
		struct
		{
			U4	scpy, n_all, sb,  sc, 
				scnl, si, spi, spn;
			char	a_snd[0x580];
		};
	};
	GPC_FIX* xp_send( GPC_GATEx64* p_gt, GPC_FIX* p_fix, U4 cnl );
	bool xp_recv( GPC_GATEx64* p_gt );
	GPC_XP(){};
};
class GPC_GATEx64
{
public:
#ifdef _DEBUG_GT_MUTI
	std::mutex		gt_mutex;
	GPE_WIP		gt_wip;
#else
	GPE_WIP		wip100;
#endif
	GPC_AN			typid,
					tab_an,
					gt_recv,
					gt_send,
					gt_ip;
	int				port;
	SOCKET			socket,
					new_socket;

	bool			gt_b_die, gt_b_spy;
	I8				n_die, n_skip;

	HANDLE			whd_old,
					whd;

	ULL				n_INxp0, n_INxp1;

	GPC_FIX			*p_INxp0, *p_INwip0,
					*p_INxp1, *p_INwip1,
					*p_OUTxp0, *p_OUTwip0,
					*p_OUTxp1, *p_OUTwip1;

	GPC_AN			an;
	I8				pos,
					idle_sec;
	GPC_XP			snd0, snd1,
					rcv;
	U4				ixcn, ix0, ix3, ixof;

	char			s_buff[0x30000/12], //s_buff[0x30000],
					s_ip[0x400],
					s_telnet[80*25+4],
					gt_crs;
	
	GPC_CRS			*gt_p_crs; // ez kívülröl van ne töröld

	SOCKADDR		sockAddr;
	addrinfo		*p_ai;
	struct sockaddr_in	addr;

	GPC_GATEx64		**pp_accept;
	I8				n_accept,
					gt_n_empty,
					n_fds,n_fde,
					n_sel,
					n_share;

	fd_set			fdset_read,
					fdset_write,
					fdset_except;
	U2				a_plc_in[4];	// 0: 0-15 1: 16-32
	U2				a_plc_out[4];
	I8				n_plc,
					n_msec,
					a_plc_i8[16*4];
	bool			b_plc_reset;
	char			s_address[0x100],
					s_name[0x100];
	
	GPC_GALAXY		*p_gt_gx;
	GPC_GX_ITM		eye_itm;
	GPCI4			xyz_star,
					n_done;

	void gt_null( void )
	{
		#ifdef _DEBUG_GT_MUTI
			gp_zero( &gt_wip, sizeof(*this)-(((char*)&gt_wip)-(char*)this) );
		#else
			GPM_CLR;
		#endif
	}
	GPC_GATEx64( GPC_AN an, GPC_AN tid, GPC_CRS* p_crs, int _port )
	{
		gt_null();
		typid = tid;
		tab_an = an;
		gt_p_crs = p_crs;
		port = _port;
	}
	GPC_GATEx64( GPC_AN an, GPC_CRS* p_crs, int _port, SOCKET acpt_soc )
	{
		gt_null();
		typid = an;
		gt_p_crs = p_crs;
		port = _port;
		socket = acpt_soc;
	}
	~GPC_GATEx64()
	{
		#ifdef _DEBUG_GT_MUTI
			gt_wip = GPE_WIP_BOSS_KILL;
		#else
			wip100 = GPE_WIP_BOSS_KILL;
		#endif
		if( pp_accept )
		{
			for( I8 a = 0; a < n_accept; a++ )
			{
				GPM_DEL( pp_accept[a] );
			}
			GPM_DELARRAY( pp_accept );
		}
		while( !set_exit() )
		{
			GPD_Sleep10;
		}
		GPM_DEL( p_INxp0 );
		GPM_DEL( p_INxp1 );
		GPM_DEL( p_OUTxp0 );
		GPM_DEL( p_OUTxp1 );
		GPM_DEL( p_INwip0 );
		GPM_DEL( p_INwip1 );
		GPM_DEL( p_OUTwip0 );
		GPM_DEL( p_OUTwip1 );
		GPM_DELARRAY( p_ai );
		GPM_DEL( p_gt_gx );
	}
	
	int	socket_error( char* p_error, char** pp_error );
	bool	socket_op( char* p_error, char** pp_error, int no_daley );
	bool	gt_q_done(void);
	void	gt_wait( void )
	{
		while( !gt_q_done() )
		{
			GPD_Sleep10;
		}
	}
	bool	gt_set_go( void );
	bool	set_wait( void );
	bool	set_exit( void );
	void	set_done( void );
	void	trd_select_modbus( char *p_error );
	I8	trd_select_connect( void );
	I8	trd_select_listen( void );

	I8	gt_n_recv_null;
	bool	trd_recv( void );
	bool	trd_rcv_xp( void );
	bool	trd_send(  void );
	bool	trd_snd_xp( void );
	void	gate_crs( GPC_CRS& crs, GPC_GATEx64* p_server );

	bool	join_robi( GPCV3& pos, GPCV3& z, GPCV3& x, GPC_TAB* p_tab, GPC_CRS* p_crs );

	GPC_GALAXY* gt_gx_unp( GPC_GATEx64* p_dest, GPC_FIX* p_wip, I8& n_wip, bool b_debug = false );
};
//class GPC_SERVERx64 : public GPC_GATEx64
//{
//public:
//	GPC_SERVERx64( GPC_AN an, GPC_CRS* p_crs, int _port ):GPC_GATEx64( an, p_crs, _port )
//	{
//		GPM_CLR;
//		typid = an;
//		gate_p_crs = p_crs;
//		port = _port;
//	}
//	GPC_SERVERx64( GPC_AN an, GPC_CRS* p_crs, int _port, SOCKET acpt_soc ):GPC_SERVERx64( an, p_crs, _port, acpt_soc )
//	{
//		GPM_CLR;
//		typid = an;
//		gate_p_crs = p_crs;
//		port = _port;
//		socket = acpt_soc;
//	}
//};
typedef enum GPE_PIX
{
	GPE_PIX_INIT,
	GPE_PIX_RGB,
	GPE_PIX_RGB_FV,
	GPE_PIX_RGBA,
	GPE_PIX_BGR,
	GPE_PIX_BGRA,
	GPE_PIX_ABGR,
	GPE_PIX_UYVY,
	GPE_PIX_BY8,
	GPE_PIX_Y800,
	GPE_PIX_YUY2,
} GPT_PIX;

void GPF_RGB2RGBAx64(int w, int h, int p, U4* p_dst, U1* p_src, U1 a, int magic);
void GPF_RGB2RGBA(int w, int h, int p, U4* p_dst, U1* p_src, U1 a, int magic);
void GPF_RGB2RGBA_FlipV(int w, int h, int p, U4* p_dst, U1* p_src, U1 a);
void GPF_YUV2RGB(int w, int h, int p, U4* p_dst, U4* p_src);
struct __declspec(uuid("{71771540-2017-11cf-ae26-0020afd79767}")) CLSID_Sampler;


class GPC_CAP_SET
{
public:
	//<CNL>0
	U2V2 pix;
	//U2	pix_x,	//<CNL_X>pix_x
	//	pix_y;	//<CNL_Y>pix_y
	//<CNL>1
	float	exp,	//<CNL_W>sec
		//<CNL>2
		f_mirrhorr, //<CNL_X>
		f_mirrvert, //<CNL_X>
		//<CNL>3
		bright,
		contr,	//<CNL_X>contrast
		gain,	//<CNL_Y>gain
		gama,	//<CNL_Z>game
		sharp,	//<CNL_W>sharpness
		//<CNL>4	
		hue,	//<CNL_X>hue
		satu,	//<CNL_Y>saturation
		white;	//<CNL_Z>whitebalance

	//<CNL>5
	GPCV4	v4gain,

		//<CNL>6
		v4white;

	bool	b_pix,
		b_exp,

		b_bright,
		b_contr,
		b_gain,
		b_gama,
		b_shar,

		b_hue,
		b_satu,
		b_white,

		b_v4gain,
		b_v4white,
		b_net,
		b_mirrhorr,
		b_mirrvert;

	U4	cap_update;

	GPC_CAP_SET(void)
	{
		GPM_CLR;
		memset( &b_pix, 1, (char*)&cap_update-(char*)&b_pix );
	}

	bool cmp_exc( GPC_CAP_SET& set )
	{
		
		if( set.b_exp )
		if( exp != set.exp )
		{
			exp = set.exp;
			b_exp = true;
			cap_update++;
		}

		if( set.b_v4white )
		if( (v4white-set.v4white).qlen() > 0.0f )
		{
			v4white = set.v4white;
			b_v4white = true;
			cap_update++;
		}

		if( set.b_v4gain )
		if( (v4gain-set.v4gain).qlen() > 0.0f )
		{
			v4gain = set.v4gain;
			b_v4gain = true;
			cap_update++;
		}

		return cap_update > 0;
	}
};


class GPC_CAMx64 : public CBaseVideoRenderer
{
public:
	I8	cam_id,
		n_qc,
		n_done;
	GPC_AN	an;
	char	s_name[0x100],
		s_path[0x100];
	GPE_WIP	cam_wip100;

	U2	pix_w, pix_h;
	GPCV2	cam_dim;

	U4	*p_bgra;
	U8	n_rgba, n_alloc;
	U8	gpc_src_size, sleep_max, sleep_awg,
		n_drop, n_drop_awg;
	
	bool	b_cam_die, b_cam_die_ack;
	
private:
	HWND				cam_hwnd;
	MSG					msg;
	BITMAPINFOHEADER	bmih;
	GPE_PIX				gpc_pix_form;
	HRESULT				hr;
	HANDLE				trd_hd;
	DWORD				trd_id;
	FILTER_STATE		fs;
	//IBaseFilter	*p_wmv;
	//IPin		*p_wout;

	IBaseFilter	*p_cam;
	IBaseFilter	*p_prot_cam;

	IGraphBuilder		*p_graph;
	ICaptureGraphBuilder2	*p_build2;
	IMediaControl		*p_ctrl;
	IAMStreamConfig		*p_stream;
	
	IAMVideoControl		*p_vid;
	IAMCameraControl	*p_cfg;
	IAMVideoProcAmp		*p_amp;

	IPin	*p_pin_cam,
			*p_pin_smp;

	bool	b_cam_off;
	
	int		mhz, main, samp;
	//U4		*p_trd_samp;
	//GPE_PIX		trd_pix;
	//WCHAR	s_priv[GPD_MAX_PATH];

	I8		n_g;
	I8		n_s;
public:
	I8		trd_pix_size;
	U2		trd_pix_w, trd_pix_h;
	U4		trd_pitch;

	GPC_CAP_SET	cam_set;

	GPC_CAMx64( char* p_path, char* p_name, U2 w, U2 h, GPCV2& dim, I8 id );

	~GPC_CAMx64()
	{
		b_cam_die = true;
		b_cam_die_ack = false;
		GPC_CAMx64_clear(false);
		GPM_RELEASE(p_graph);
		GPM_RELEASE(p_build2);
		GPM_RELEASE(p_ctrl);
		GPM_RELEASE(p_stream);
		GPM_RELEASE(p_cfg);
		GPM_RELEASE(p_amp);
		GPM_RELEASE(p_vid);
		GPM_RELEASE(p_pin_cam);
		GPM_RELEASE(p_pin_smp);
		GPM_RELEASE(p_cam);
		GPM_RELEASE(p_prot_cam);
		
		GPM_DELARRAY(p_bgra);
		//GPM_DELARRAY(p_trd_samp);
	}
	bool	GPC_CAMx64_BOS_tDONE_bSYNC( GPC_WBx64* p_wb );
	bool	GPC_CAMx64_TRD_bSY_tST_t20( BITMAPINFOHEADER* p_bmi, BYTE* p_data, GPE_PIX gpc_pix_form, I8 size );
	bool	GPC_CAMx64_BOS_t20_b30_b80( GPC_WBx64* p_wb );
	bool	GPC_CAMx64_BOS_MonoScope(GPC_WBx64* p_wb);

	HRESULT	CheckMediaType(const CMediaType *p_media);
	HRESULT	DoRenderSample(IMediaSample *p_sample);
	HRESULT	ShouldDrawSampleNow(IMediaSample *sample, REFERENCE_TIME *start, REFERENCE_TIME *stop);
	void	GPC_CAMx64_disconect(void);
	int	GPC_CAMx64_format(void);

	bool		GPC_CAMx64_run( void );
	bool		GPC_CAMx64_off(void);
	bool		GPC_CAMx64_pause(void);
	GPC_CAMx64*	GPC_CAMx64_switch( void );
	GPC_CAMx64*	GPC_CAMx64_wait(void);
	GPC_CAMx64*	GPC_CAMx64_clear(bool b_cam_protect = false);
	GPC_CAMx64*	GPC_CAMx64_reset(HWND hwnd, bool b_cam_protect = false);
	
	IBaseFilter*	GPF_CAM_SOURCE_set(LPMONIKER lpMonik);
	IBaseFilter*	GPF_CAM_SOURCE_get(void)
	{
		return p_cam;
	}
	char	s_buff[3 * 0x10000];

};

class GPC_JOY
{
public:
	GPCI4 a_arm[2];
	I8 a_buton[4];
	JOYINFOEX joyinfo, o_joyinfo;
	JOYCAPS joycap;
	bool b_live;
	
	GPC_JOY(void)
	{
		GPM_CLR;
	}
};

LRESULT CALLBACK WinProc2(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class GPC_WIN
{
public:
	HWND			hWnd;
	WNDCLASSEX	wc;

	char			s_name[0x100],
				*p_error;
				
	I8			pic_id;
	bool			b_border, b_kill;
	GPCI4		lurd;
	GPC_WBx64		*gp_p_wb;

	IDXGISwapChain			*gp_p_dx11_swap;		// the pointer to the swap chain interface
	ID3D11Device			*gp_p_dx11_dev;		// the pointer to our Direct3D device interface
	ID3D11DeviceContext		*gp_p_dx11_contx;		// the pointer to our Direct3D device context
	D3D11_VIEWPORT			win_vp, chk_vp;
	SYSTEM_INFO			SysInfo;
	
	ID3D11Texture2D		*p_buff2D_bgra,
						*p_buff2D_depth;
	ID3D11RenderTargetView	*p_target_bgra;
	ID3D11DepthStencilView	*p_target_depth;

	HRESULT			hr;
	float			sec;

	GPC_GIO_BUFF_VX324	solid_vx;
	ID3D11Buffer		*gp_p_buff_solid_vx;
	ULL				gp_n_buff_solid_vx;
	I8				n_creat_solid;
	U4				stride,
					offset;

	ID3D11VertexShader	*p_vs;
	ID3D11PixelShader	*p_ps;
	ID3D11InputLayout	*p_lay;

	ID3D11RasterizerState	*gp_p_dx11_RstStt_front;
	ID3D11BlendState		*gp_p_dx11_BlendState;
	ID3D11DepthStencilState	*gp_p_dx11_DebthState_gui;
	ID3D11SamplerState		*gp_p_SamplerLinear;
	ID3D11SamplerState		*gp_p_SamplerPoint;
	
	ID3D11ShaderResourceView	*p_samp_bgra;
	I8			smp_id;

	GPC_COLOR4*		p_bgra;
	I8			pic_w, pic_h;

	GPC_WIN( void )
	{
		GPM_CLR;
		pic_id = -1;
		wc.cbSize = sizeof(wc);
	}
	~GPC_WIN( void )
	{
		if( hWnd )
			CloseWindow( hWnd );

	}
	GPC_WIN* win_lurd( GPCI4 _lurd, WINDOWINFO& wi, bool _b_border = false )
	{
		if( b_kill )
		{
			CloseWindow( hWnd );
			hWnd = NULL;
		}
		int	n_monitor = GetSystemMetrics(SM_CMONITORS),
			sx = GetSystemMetrics(SM_CXVIRTUALSCREEN),
			sy = GetSystemMetrics(SM_CYVIRTUALSCREEN);
		
		if( _lurd.x > sx )
			_lurd.x = sx;
		if( _lurd.y > sy )
			_lurd.y = sy;

		if( _lurd.z > sx )
			_lurd.z = sx;
		if( _lurd.w > sy )
			_lurd.w = sy;
		
		GPCI4	client( wi.rcClient.left, wi.rcClient.top, wi.rcClient.right, wi.rcClient.bottom ),
			window( wi.rcWindow.left, wi.rcWindow.top, wi.rcWindow.right, wi.rcWindow.bottom ),
			border( wi.cxWindowBorders, wi.cyWindowBorders,0,0 ),
			creat(0,0,0,0);

		DWORD style = WS_EX_APPWINDOW; //|WS_EX_WINDOWEDGE;
		if( _b_border )
		{
			style |= WS_EX_WINDOWEDGE;
			
			_lurd.x -= border.x;
			_lurd.y -= border.y+client.y;
			_lurd.z += border.x;
			_lurd.w += border.y+client.w;
		}

		if( lurd == _lurd && b_border == _b_border )
			return this;

		if( b_border != _b_border )
		{
			CloseWindow( hWnd );
			hWnd = NULL;
			b_border = _b_border;
		}
		lurd = _lurd;
		creat.x = lurd.x;
		creat.y = lurd.y;
		creat.z = lurd.z;
		creat.w = lurd.w;
		if( !hWnd )
		{
			wc.style = CS_HREDRAW | CS_VREDRAW;
			wc.lpfnWndProc = WinProc2;
			wc.hInstance = 0; //hInstance;
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);
			wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
			wc.lpszClassName = L"WindowClass1";

			// register the window class
			RegisterClassEx(&wc);
			hWnd = CreateWindowExA(	
						style,
						"WindowClass1",	// name of the window class
						s_name,	// title of the window
						WS_OVERLAPPED|WS_POPUP, //WS_OVERLAPPEDWINDOW,	// window style
						creat.x,		//(sx-w)/2, // x-position of the window
						creat.y,		// y-position of the window
						creat.z,	// width of the window
						creat.w,	// height of the window
						NULL,		// we have no parent window, NULL
						NULL,		// we aren't using menus, NULL
						0,		// application handle
						NULL
					);    // used with multiple windows, NULL
		} else {
			
			//MoveWindow( crs_p_wb->wb_hWnd, crs_win_trg.x, crs_win_trg.y, crs_win_trg.z, crs_win_trg.w, crs_b_win_show );
			GPCI4 border( 
					wi.rcWindow.top	- wi.rcClient.top,
					wi.rcWindow.right	- wi.rcClient.right,
					wi.rcWindow.bottom	- wi.rcClient.bottom,
					wi.rcWindow.left	- wi.rcClient.left );

			

			SetWindowPos(
					hWnd,
					HWND_TOP,
					lurd.x,
					lurd.y,
					lurd.z,
					lurd.w,
					0 );

			GetWindowInfo( hWnd, &wi );
			//crs_win_trg.x = wi.rcClient.left;
			//crs_win_trg.y = wi.rcClient.top;
			//crs_win_trg.z = wi.rcClient.right - wi.rcClient.left;
			//crs_win_trg.w = wi.rcClient.bottom - wi.rcClient.top;
		}
		ShowWindow(  hWnd, SW_SHOW );
		return this;
	}
	bool win_run( GPC_WBx64* p_wbx64 );
};

class GPC_WBx64
{
public:
	U1			a_c2p[GPD_3D_END_Y*2];
	GPC_WIN*	p_more_win;
	I8			n_more_win;

	GPC_FIX		*wb_p_fix_CLIP;
	GPC_FIX		*wb_p_fix_com;
	HANDLE		main_trdhd,
				show_trdhd,
				h_present;
	HWND		wb_hWnd,
				show_hwnd;
	DWORD		swow_id;

	GPC_PICx64		**pp_pic;
	GPC_RES*		ap_glob_res[GPD_ALFA];

	//ID3D11Query *pQuery;
	//ID3D11Asynchronous *p_async;
	I8		//n_begin,
			n_pic, n_pic_scan,
		// BEGIN memset( -1 )
			gpc_pic_out_id,
			a_pic_in_id[0x10],
			wb_out_id,	wb1_out_id,
			wb_mini_id, wb_frame_id,
			wb_minv_id,
			wb_memmap_id, cam_busy_id,
			wb_pub_png_id,
			wb_bx7_repnd_id,
			store_png_id;

	GPC_AN		cam_busy_an;
		// END memset( -1 )
	GPC_GIOx64_CONST	*p_gio_cbuff_rnd;
	GPC_GIOx64_LAY		*p_gio_lay_rnd;
	GPC_GIOx64			*p_gio_rnd;
	GPC_GISx64			*p_gis_rnd;

	GPC_GIOx64	**pp_gio;
	I8			n_gio;

	GPC_GIMx64	**pp_gim;
	I8			n_gim;
	GPC_DICK	*p_lws_com,
				*p_lws_bons;

	GPC_GISx64	**pp_gis;
	I8			n_gis;
	
	GPC_CAMx64	*p_cam_busy, *p_cam_on,
				**pp_cam;
	I8		n_cam, n_cam_cnt;
	GPE_WIP		show_wip100;
	I8		un_plug, plug_in, sleep_rec, sleep_awg, n_cam_use, n_qshowdone, n_joy,
			//n_mx_gpu,
			n_gpu_ver,
			//n_mx_bgra, n_mx_shdw,
			plug_msec, init_msec, n_usb_er;

	GPC_GPUx64		**pp_gpu;
	I8		n_gpu,
			gpc_gpu_id,
			gpc_gpu_map,
			gpc_gpu_stride,
			solid_id,
			line_id,
			gis_line_id,
			gis_mini_id,
			gio_solid_id,
			gio_line_id,
			gio_static_id;

	//====-===--==--- --  -
	//	NET
	char				s_hostname[_MAX_PATH];
	hostent				*p_host_ips;
	PMIB_IPADDRTABLE	p_IPTable;

	GPC_AIMOM	**pp_AiMOM;
	I8			n_AiMOM;
	GPC_GATEx64	**pp_gate;
	I8			n_gate,
				n_gcon,
				n_gdie;
	




	GPC_JOY*	p_joy_alloc;
	I8			n_joy_alloc;

	GPC_FILE**		pp_file;
	I8		n_file,
			wb_a_keymap[0x400],
			*wb_p_keymap_ext,
			*wb_p_mousemap,
			wb_key_qc,
			wb_touch_qc,
			wb_last_key;

	char	wb_s_work_title[_MAX_PATH],
			//wb_s_tab_crs[_MAX_PATH],
			
			wb_s_tab_on[_MAX_PATH],
			*wb_p_tab_on,

			wb_s_tab_sel[_MAX_PATH],
			*wb_p_tab_sel,
			
			wb_s_title[_MAX_PATH];
	
	
	U4*	wb_p_memmap_pix;
	bool	wb_b_memmap, wb_b_pred;
	// D3D11Create Device And Swap Chain
	IDXGISwapChain		*gp_p_dx11_swap;		// the pointer to the swap chain interface
	ID3D11Device		*gp_p_dx11_dev;		// the pointer to our Direct3D device interface
	ID3D11DeviceContext		*gp_p_dx11_contx;		// the pointer to our Direct3D device context
	
	// ReSize TARGET
	ID3D11RenderTargetView	*gp_p_dx11_backbuffer;	// the pointer to our back buffer
	D3D11_VIEWPORT		wb_viewport;
	ID3D11DepthStencilView	*gp_p_target_depth;
	// MINI_tex
	ID3D11ShaderResourceView	*gp_p_mini_tex;
	U4			char_x, char_y,
				mini_x, mini_y,
				mper2_x, mper2_y;
	ID3D11SamplerState		*gp_p_SamplerAnisotrop;
	ID3D11SamplerState		*gp_p_SamplerLinear;
	ID3D11SamplerState		*gp_p_SamplerPoint;
	ID3D11BlendState		*gp_p_dx11_BlendState;
	ID3D11BlendState		*gp_p_dx11_BlendState_sky;
	ID3D11BlendState		*gp_p_dx11_BlendState_off;
	ID3D11DepthStencilState	*gp_p_dx11_DebthState;
	ID3D11DepthStencilState	*gp_p_dx11_DebthState_sky;
	ID3D11DepthStencilState	*gp_p_dx11_DebthState_gui;
	ID3D11RasterizerState		*gp_p_dx11_RstStt_front,
				*gp_p_dx11_RstStt_all,
				*gp_p_dx11_RstStt_wire;
	ID3D11Buffer		*gp_p_dx_cgis;
	// BUFF_UpLoad
	// SOLID
	ID3D11Buffer		*gp_p_buff_solid_vx;
	ULL			gp_n_buff_solid_vx,
				gp_n_buff_solid_load;
	
	ID3D11Buffer		*gp_p_buff_solid_idx;
	ULL			gp_n_buff_solid_idx,
				gp_n_buff_solid_idx_load;

	// MINI
	ID3D11Buffer		*gp_p_buff_mini_vx;
	ULL			gp_n_buff_mini_vx,
				gp_n_buff_mini_load;

	// LINE
	ID3D11Buffer		*gp_p_buff_line_vx;
	ULL			gp_n_buff_line_vx,
				gp_n_buff_line_load;

	ID3D11Buffer		*gp_p_buff_line_idx;
	ULL			gp_n_buff_line_idx,
				gp_n_buff_line_idx_load;
	
	ID3D11Buffer	*gp_p_dx_vs_const,
					*gp_ap_dx_vs_c0[0x10],
					*gp_ap_dx_vs_c1[0x10],
					*gp_ap_dx_vs_c2[0x10];
	I8				n_c;

	ID3D11Buffer	*gp_p_dx_c_gio_lay;

	ID3D11Buffer	*gp_p_jit_vs_cbuff;
	// UnMAP
	
	float		sec;
	
	SYSTEM_INFO	gp_sysinfo;
	GPC_CRS		*wb_p_crs,
				*wb_p_crs_sel;

	HRESULT		hr;
	
	GPC_GIO_BUFF_VX32CUFBI *gp_p_cake_vx_buff;
	
	char		s_pub_buff[0x20000];

	GPC3D_BUFF**	pp_3d_pub;
	I8				n_3d_pub;
	I8				gx_id_busy;
	
	
	// ------- CELL_CELL ------------- ---  --   -
	GPCV2	*ap_3d_cell[6];
	U4		a_3d_cell_i[6][0x110],
			a_3d_cell_n[6][0x110];

	GPCV2	*p_3d_cell_lo;
	U4		a_3d_cell_lo_i[0x110],
			a_3d_cell_lo_n[0x110];

	GPCV2	**GPC_WBx64_cell_get_hi( void ),
			*GPC_WBx64_cell_get_lo( void );
			//*GPC_WBx64_cell_join_get( void );
	GPCELL_BUFF	cell1;

	ID3D11Buffer	*gp_p_dx_cell_ix;
	ULL				gp_n_dx_cell_ix;

	#ifdef GPD_VOXVOX
		ID3D11Buffer	*gp_p_dx_vox_vx;
		U4			gp_n_dx_vox_vx, vox_stride;
		ID3D11Buffer	*gp_p_dx_vox_psc;
		I8			vox_pat_id;
	#endif

	// -------- Új MINI ONE-------------------

	GPC_GPUx64_CONST	mini_one_cbuff;
	ID3D11Buffer		*gp_p_dx_mini_one_vx,
						*gp_p_dx_mini_one_const;
	I8					gp_n_dx_mini_one_vx,
						wb_mini1_pic_id,wb_frame1_pic_id,
						wb_mini1_gpu_id, wb_mini2_gpu_id,
						wb_stf_id;
	U4					wb_mini1_stride;
	GPC_COLOR4			*p_mini_c4, *p_frame_c4;
	I8					mini_col,mini_row, mini_cr;
	GPC_GIOx64_CACHE*	p_gio_cache;
	//HANDLE		h_present;

	GPC_WIN* GPC_WBx64_WIN_add( char* p_name )
	{
		
		for( I8 i = 0; i < n_more_win; i++ )
		{
			if( !strcmp( p_more_win[i].s_name, p_name ) )
				return p_more_win+i;
		}

		GPC_WIN* p_kill = p_more_win;
		I8 n_kill = n_more_win;

		n_more_win++;
		p_more_win = new GPC_WIN[n_more_win];
		if( p_kill )
		{
			gp_memcpy( p_more_win, p_kill, n_kill*sizeof(*p_kill));
		} else {
			n_kill = 0;
		}
		GPM_DELARRAY(p_kill);
		
		GPC_WIN* p_out = p_more_win+n_kill;
		gp_zero( p_out, (n_more_win-n_kill)*sizeof(*p_out));
		GPM_STRCPY( p_out->s_name, p_name );
		p_out->wc.cbSize = sizeof(p_out->wc);
		p_out->gp_p_wb = this;
		return p_out;
	}

	//GPC_GIO_VX32CU* GPC_WBx64_3D_cell( GPC3D_BASE* p_base );
	GPC3D_BUFF* GPC_WBx64_3D_PUB_get( I8 gio_id )
	{
		GPC3D_BUFF* p_out = NULL;

		I8 i = 0, n_empty = n_3d_pub;
		for( ; i < n_3d_pub; i++ )
		{
			if( !pp_3d_pub[i] )
			{
				n_empty = i;
				break;
			}
			if( pp_3d_pub[i]->gio_id == gio_id )
				return pp_3d_pub[i];
		}
		
		if( n_empty >= n_3d_pub )
		{
			GPC3D_BUFF** pp_kill = pp_3d_pub;
			n_3d_pub = n_empty+1;
			pp_3d_pub = new GPC3D_BUFF*[n_3d_pub];
			if( pp_kill )
			{
				gp_memcpy( pp_3d_pub, pp_kill, n_empty*sizeof(*pp_3d_pub) );
				if( n_3d_pub-n_empty > 1 )
					gp_zero( pp_3d_pub+n_empty, (n_3d_pub-n_empty)*sizeof(*pp_3d_pub) );
				GPM_DELARRAY( pp_kill );
			}
		}
		pp_3d_pub[n_empty] = new GPC3D_BUFF(gio_id);
		for( i = 0; i < n_3d_pub; i++ )
		{
			if(!pp_3d_pub[i])
				continue;
			pp_3d_pub[i]->n_buff = n_3d_pub-i;
		}
		return pp_3d_pub[i-1];
	}

	// this function initializes and prepares Direct3D for use
	GPC_WBx64(HWND hWnd, char* p_file, char* p_file_e );
	GPC_FIX*	GP_WB_COM( GPC_FIX* p_fix_answ = NULL, GPC_FIX* p_fix_com = NULL, bool b_alloc_answ = false, GPC_GATEx64* p_gt_mom = NULL, GPC_GATEx64* p_gt_ply = NULL );
	void	GP_DX11_GPU_init(void);
	void	GP_DX11_BUFF_upload(void);
	I8	GP_DX11_PRES_wait( void )
	{
		#ifdef _TRD_PRESENT
			if( h_present )
			{
				WaitForSingleObject( h_present, INFINITE );
				GPM_CLOSE( h_present );
				h_present = 0;
			}
		#else
			if( wb_p_crs ? !wb_p_crs->crs_n_render : true )
				return 0;
			if( wb_p_crs->crs_n_present == wb_p_crs->crs_n_render )
				return wb_p_crs->crs_n_render;
			wb_p_crs->crs_n_present = wb_p_crs->crs_n_render;
			/*if( n_begin )
			{
				gp_p_dx11_contx->Begin(p_async);
				gp_p_dx11_contx->End(p_async);
				n_begin = 0;
			}*/
			gp_p_dx11_swap->Present( 0, 0 );

			//gp_p_dx11_contx->Flush();
			/*DXGI_PRESENT_PARAMETERS PresentDesc;
			ZeroMemory(&PresentDesc, sizeof(PresentDesc));
			HRESULT hr =  gp_p_dx11_swap->Present(0, 0, &PresentDesc);*/
		#endif
			return wb_p_crs->crs_n_render;
	}
	void	GP_DX11_Render(void);

	static DWORD WINAPI	GPC_WBx64_SHOW_TRD(LPVOID p_param);
	bool		GPC_WBx64_SHOW_TRD_loop( MSG& msg );
	void		GPC_WBx64_SHOW_Init(void);
	
	GPC_CAMx64*	GP_WBx64_CAM_find(char* p_name, char* p_path );
	bool		q_show_done(void);
	bool		show_go(void);
	bool		show_wait(void);
	bool		show_exit(void);
	void		show_done(void);

	I8		GPC_GISx64_load( char* p_file );
	GPC_GISx64* GPC_GISx64_p_get( I8 gis_id )
	{
		if( gis_id > -1 && gis_id < n_gis )
			return pp_gis[gis_id];

		return NULL;
	}

	char*		GP_WBx64_PIC_list( void );
	GPC_PICx64*	GP_WBx64_PIC_get( I8 pic_id );
	
	GPC_PICx64*	GP_WBx64_PIC_load( I8 pic_id, char* p_file );
	I8		GP_WBx64_PIC_load( char* p_file, bool b_hdr = false );
	GPC_PICx64*	GP_WBx64_PIC_find( char* p_name );
	GPC_PICx64*	GP_WBx64_PIC_upload(I8 pic_id, I8 bob_id, U4 w, U4 h, U4 p, U4* p_pix, bool b_new_qc, U4 n_set = 0 );
	GPC_PICx64*	GP_WBx64_PIC_upload3d(I8 pic_id, I8 bob_id, U4 w, U4 d, U4* p_pix, bool b_new_qc, U4 n_set = 0 );
	GPC_PICx64*	GP_WBx64_VOX_upload3d(I8 pic_id, U1 n_set );
	GPC_PICx64*	GP_WBx64_PIC_download( I8 pic_id, I8 bob_id );
	
	bool	GP_WBx64_PIC_rePNG( GPC_PICx64* p_pic, char* p_png, I8 n_png );
	bool	GP_WBx64_PIC_repic( GPC_PICx64* p_pic );
	void GP_WBx64_SMP_desc(D3D11_TEXTURE2D_DESC& desc)
	{
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0; // D3D11_STANDARD_MULTISAMPLE_PATTERN;
	}
	bool	GP_WBx64_PIC_in( int n_id, I8 pic_id, I8 bob_id );
	bool	GP_WBx64_PIC_out( I8 pic_id, bool b_clr, GPC_PICx64* p_deep = NULL  );
	bool	GP_WBx64_GPU_Const( GPC_GPUx64_CONST* p_cbuff );
	ID3D11Buffer*	GP_WBx64_GPU_Const( ID3D11Buffer* p_dx, GPC_GPUx64_CONST* p_cbuff );
	bool	GP_DX11_ReSize( I8 pic_id );
	
	GPC_GIOx64*	GP_WBx64_GIO_find( I8 n_id );
	I8		GP_WBx64_GIO_ID( GPC_GIOx64* p_gio );
	GPC_GIOx64*	GP_WBx64_GIO_Creat( char* p_name );
	I8		GP_WBx64_GIO_lwo( char* p_file, float gio_dim );
	GPC_GIMx64*	GP_WBx64_GIM_Creat( char* p_name );
	void		GP_WBx64_GIM_Reset( void );

	GPC_FILE*	GP_WBx64_FILE_Creat( char* p_file );

	GPC_GPUx64* GP_WBx64_GPU_get( I8 g )
	{
		if( g < -1 || g >= n_gpu )
			return NULL;
		return pp_gpu[g];
	}
	I8		GP_WBx64_GPU_find( I8 g, char* p_name, I8& n_ver );
	GPC_GPUx64*	GP_WBx64_GPU_compi( char* p_src, char* p_name ); //, bool b_gio, bool b_lod );
	bool		GP_WBx64_GPU( I8 id, I8 map, I8 n_stride, bool b_inst = false );
	
	I8	GP_WBx64_GATE_listen( GPC_AN tab_an, int port, GPC_AN& tab_send, GPC_AN& tab_recv, char* p_name = NULL );
	void	GP_WBx64_GATE_crs( I8 gate_id, GPC_CRS& crs );
	void	GP_WBx64_GATE_share( I8 gate_id );
	void	GP_WBx64_GT_GX( I8 gate_id );

	void GP_WBx64_GATE_ho( void )
	{
		GPC_GATEx64	*p_gate = NULL,
				**pp_g = pp_gate,
				**pp_empty = NULL;
		for( GPC_GATEx64 **pp_ge = pp_g+n_gate; pp_g < pp_ge; pp_g++ )
		if( p_gate = *pp_g )
		{
			if( !p_gate->whd )
				continue;
			SetThreadPriority( p_gate->whd, THREAD_PRIORITY_IDLE );
			//SuspendThread( p_gate->whd );
		}
	}
	void GP_WBx64_GATE_gyia( void )
	{
		GPC_GATEx64	*p_gate = NULL,
				**pp_g = pp_gate,
				**pp_empty = NULL;
		for( GPC_GATEx64 **pp_ge = pp_g+n_gate; pp_g < pp_ge; pp_g++ )
		if( p_gate = *pp_g )
		{
			if( !p_gate->whd )
				continue;

			SetThreadPriority( p_gate->whd, THREAD_PRIORITY_NORMAL );
			//ResumeThread( p_gate->whd );
		}
	}
	I8	GP_WBx64_GATE_connect( GPC_AN tab_an, char* p_ip, int port, GPC_AN& tab_send, GPC_AN& tab_recv );
	GPC_FIX*	GP_WBx64_GATE_LIST( GPC_FIX *p_fix, char* p_enter = "\n" );
	void	GP_WBx64_GATE_robi( I8 gate_id );
	void	GP_WBx64_GATE_robi2( I8 gate_id );
	void	GP_WBx64_GATE_MODBUS( GPC_GATEx64* p_plc );
	void	GP_WBx64_GATE_modbus2( I8 gate_id );


	I8	GPC_WBx64_JOY_n( void );
	I8	GPC_WBx64_KEY_n(  HACCEL hAccelTable,  U1* gp_s_ascii, ULL &gp_n_ascii );
	void	GPC_WBx64_close( void );

	void GPC_GIOx64_CACHE_rndr_ix( GPC_GIOx64_CACHE* p_cache, U1 map, U4 n_inst, U4 s_inst, ID3D11Buffer** pp_buff, U4* p_stride, U4* p_offset  )
	{
		if( p_cache ? !p_cache->ap_dx_lay[map] : true )
			return;
		gp_p_dx11_contx->IASetIndexBuffer( p_cache->p_dx_ix, DXGI_FORMAT_R32_UINT, 0 );
		// VERTEX
		pp_buff[0] = p_cache->p_dx_vx;
		gp_p_dx11_contx->IASetVertexBuffers(0, 2, pp_buff, p_stride, p_offset );
		gp_p_dx11_contx->DrawIndexedInstanced( p_cache->ix_n, n_inst, 0, 0, s_inst );
							
	}
	
	~GPC_WBx64();
};
#endif

#include "GPCVx64.inl"
