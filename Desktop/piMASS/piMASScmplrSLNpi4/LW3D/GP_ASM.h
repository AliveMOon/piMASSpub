#ifndef GP_ASM
#define GP_ASM

#include "GP_NEW.h"
class float2;
class float3;
class float4;
class float4x4;
class double4;
class U24;
class U44;
class U84;
class I44;
class I84;
class GPC_CELL;
class GPC3Dvxc;

void* gpf_memXinc( void* p_d, void* p_s, U8 ynz, U8 w16, U8 Ws1X16 ); // ez csak disassembli hez példa hova kerül a 5. paraméter "A VEREMBE"

extern "C" U8		gp_strlen( const void* p_d );
extern "C" char*	gp_strcpy( void* p_d, const void* p_s, void* p_memcpy = (void*)&memcpy );
extern "C" void*	gp_PATx( void* pPAT, U8 nBYTE );
extern "C" void*	gp_PATy(void* pPAT, U8 nBYTE, U8 next );
extern "C" void*	gp_PATz(void* pPAT, U8 nBYTE, U8 next );

extern "C" bool		gp_qz( void* p_d, U8 n );
extern "C" void*	gp_zero( void* p_d, U8 n, void* p_memset = (void*)&memset );
extern "C" void*	gp_memset_u1( void* p_d, U1 u1, U8 n_byte );
extern "C" void*	gp_memset_u2( void* p_d, U2 u2, U8 n_byte );
extern "C" void*	gp_memset_u4( void* p_d, U4 u4, U8 n_byte );
extern "C" void*	gp_memset_q( void* p_d, U8 &u8, U8 n_byte );
extern "C" void*	gp_memset_0x10( void* p_d,  const void* p_q, U8 n );
extern "C" void*	gp_memset_0x20( void* p_d,  const void* p_q, U8 n );
extern "C" void*	gp_memset_0x30( void* p_d,  const void* p_q, U8 n );
extern "C" void*	gp_memset_0x40( void* p_d,  const void* p_q, U8 n );
extern "C" void*	gp_memset_0x50( void* p_d,  const void* p_q, U8 n );
extern "C" void*	gp_memset_0x50( void* p_d,  const void* p_q, U8 n );
extern "C" void*	gp_memset_0x60( void* p_d,  const void* p_q, U8 n );
extern "C" void*	gp_memset_0x70( void* p_d,  const void* p_q, U8 n );
extern "C" void*	gp_memset_0x80( void* p_d,  const void* p_q, U8 n );
extern "C" void*	gp_memset_0x90( void* p_d,  const void* p_q, U8 n );
extern "C" void*	gp_memset_0xa0( void* p_d,  const void* p_q, U8 n );
extern "C" void*	gp_memset_0xb0( void* p_d,  const void* p_q, U8 n );
extern "C" void*	gp_memset_0xc0( void* p_d,  const void* p_q, U8 n );
extern "C" void*	gp_memset_0xd0( void* p_d,  const void* p_q, U8 n );
extern "C" void*	gp_memset_0xe0( void* p_d,  const void* p_q, U8 n );
extern "C" void*	gp_memset_0xf0( void* p_d,  const void* p_q, U8 n );
extern "C" void*	gp_memset_0x100( void* p_d, const void* p_q, U8 n );
extern "C" U1*		gp_memcmp( void* p_a, const void* p_b, U8 n );
extern "C" void*	gp_memcpy( void* p_d, const void* p_s, U8 n, void* p_memcpy = (void*)&memcpy );


extern "C" void*	gp_memXinc( void* p_d, void* p_s, U8 ynz, U8 w16, U8 Ws1X16 );
extern "C" void*	gp_ddd_lut( void* p_d, void* p_s, U8 n, U4 &mask );
extern "C" void*	gp_ddd_z( void* p_d, U8 z, U4 &shr ); // , U4 &u4 );
extern "C" void*	gp_ddd_x( void* p_d, U8 n, U4 &shr );
extern "C" void*	gp_ddd_y( void* p_d, U4 &shr );
extern "C" void*	gp_ddd_weld5_x( void* p_d, U4 &shr, U4 &mask );
extern "C" void*	gp_ddd_weld5_y( void* p_d, U4 &shr, U4 &mask );
extern "C" void*	gp_ddd_weld5_z( void* p_d, U4 &shr, U4 &mask );
extern "C" void*	gp_ddd_mask( void* p_d,   void* p_mask, U4 z );
extern "C" bool		gp_ddd_C4( void* p_stf, void* p_c4, bool b_serv, U8 light );
extern "C" U1*		gp_ddd_C42P( void* p_pat, void* p_c4 );
//extern "C" void*	gp_mul_v4_i8( void* p_this, I8 i8 );
//extern "C" void*	gp_mul_v4_v4( void* p_this, void* p_v4 );

extern "C" I8	gp_muls_q( I8& b, I8 d );

extern "C" void*	gp_i4_add( void* p_d, void* p_s );
extern "C" void*	gp_i4_sub( void* p_d, void* p_s );


class float4;
class I84;

extern "C" float4* gp_f44_len( float4* p_s );
extern "C" float4* gp_f44_qlen( float4* p_s );
extern "C" float4* gp_f44_dot( float4* p_d, const float4* p_s );
extern "C" float4* gp_f44_div( float4* p_d, const float4* p_s );
extern "C" float4* gp_f44_mul( float4* p_d, const float4* p_s );
extern "C" float4* gp_f44_add( float4* p_d, const float4* p_s );
extern "C" float4* gp_f44_sub( float4* p_d, const float4* p_s );

extern "C" double* gp_d4_mul( double* p_d, const double* p_s );
extern "C" double* gp_d4_div( double* p_d, const double* p_s );
extern "C" double* gp_d4_sub( double* p_d, const double* p_s );
extern "C" double* gp_d4_add( double* p_d, const double* p_s );
extern "C" double gp_d4_qlen( double* p_d, const double* p_s );

extern "C" I84*	gp_i84_add( I84* p_d, const I84* p_s );
extern "C" I84*	gp_i84_sub( I84* p_d, const I84* p_s );



#define _memcpy( p_dst, p_src, n )( gp_memcpy(p_dst,p_src,n ) )
#define gp_strncpy	(char*)gp_memcpy
#define GPM_PAD( n, p ) ( (n) + (((n)%(p)) ? ((p)-((n)%(p))) : 0) )
#define GPM_CLR	gp_zero( this, sizeof(*this) ) //memset( this, 0, sizeof(*this) )
#define _memcmp( p_a, p_b, n )( (gp_memcmp(p_a,p_b,n )-(U1*)p_a) != n )

#define	GPM_MEMCPY( d, s, n ) _memcpy( (d), (s), (n)*sizeof(*(d)) )
#define	GPM_MEMSET( d, s, n ) gp_memset_u1( (U1*)(d), (U1*)(s), (n)*sizeof(*(d)) )
#define	GPM_MEMSETn( d, s, n ) GPF_MEM_set( &(d), (n), &(s), sizeof((d)) )





#define GPM_CLOSE( h ){ if( h ){ CloseHandle( h ); h = 0; } }
#define GPM_RELEASE( p ){ if( p ){ p->Release(); p = NULL; } }
#define GPM_DELARRAY( p ){ if( p ){ delete[] p; p = NULL; } }

#define GPM_FF_CLOSE( h ){ if( h ){ _findclose( h ); h = -1L; } }
#define GPM_FD_CLOSE( h ){ if( h ){ fclose( h ); h = NULL; } }
#define GPM_SOC_CLOSE( h ){ if( h != INVALID_SOCKET ){ closesocket( h ); h = INVALID_SOCKET; } }

#define GPM_STR( _p ) (_p ? GPM_STRCPY( new char[GPD_STRLEN(_p)+1], _p ) : NULL) 
#define GPM_DEL( p ){ if( p ){ delete p; p = NULL; } }
#define GPMZ( p ) gp_zero( &p, sizeof(p) ) //{ if( &p ){ gp_zero( &p, sizeof(p) ); } }
#define	GPMZn( d, n ) gp_zero( d, (n)*sizeof(*(d)) )




void*	cpy_up(void* p_dst, void* p_src, U8 n);
void*	cpy_down(void* p_dst, void* p_src, U8 n);
U4		GPF_SWAP_U4( char* p_s );
float	GPF_SWAP_F4( char* p_s );
char*		GPF_LW3D_Pad(char* p_data);
U2		GPF_SWAP_U2( char* p_s );
U2		GPF_SWAP_U2( U2 u2 );
U1*		GPF_MEM_skip( U1* p_dst, U8 nbyte );
U1*		GPF_MEM_or( U1* p_dst, U1 u1, U8 nbyte );
U1*		GPF_MEM_and( U1* p_dst, U1 u1, U8 nbyte );
U1*		GPF_MEM_max( U1* p_dst, U1* p_1, U1* p_2, U8 n_byte );
void*	GPF_MEM_set( void* p_dst, I8 n, const void* p_src, I8 n_byte );
//I8	GPF_ALFA_n( char* p_begin, char* p_end, GPE_ALFA alfa );
//I8	GPF_ALFA_find( char* p_begin, char* p_end, GPE_ALFA alfa );
U1*		GPF_a_alfa_init( void );
U8		GPF_ALFA2STR(char* p_out, I8 d0);

I8		GPF_STR2I8( char* p_str, char** pp_str );
U8		GPF_STR2U8( char* p_str, char** pp_str );
char*	GPF_STR( char* p_kill, const char* p_str );

char*	GPF_STR_icmp( char* p_kill, char* p_str );
char*	GPF_STRnSTRn( char* p_dst, U8 n_dst,  char* p_src, U8 n_src );
U1*		GPF_U1_replace( U1* p_dst, U1* p_u1, U1* p_u1e, U1* p_r0, I8 n_r0 );
char*	GPF_STR_replace(char* p_str, U8 n_str, char o, char c);
char*	GPF_STR_replace(char* p_str, char* p_end, char* p_o, char* p_n );
I8		GPF_STR_VAN( char* p_str, char* p_end, char* p_van);
I8		GPF_STR_VISZ(char* p_str, char* p_start, const char* p_van);
//I8			GPF_STR_VISZ_NINCS(char* p_str, char* p_start, char* p_nincs );
I8 GPF_STR_VISZ_NINCS(char* p_str, char* p_start, const char* p_nincs );
//{
//	if( !p_str )
//		return 0;
//	if( !p_start )
//		p_start = p_str+GPD_STRLEN(p_str);
//	if( p_str >= p_start )
//		return p_start - p_str;
//
//	U1 ab_nincs[0x100];
//	gp_memset_u1( ab_nincs, true, sizeof(ab_nincs));
//	//memset(ab_nincs, true, sizeof(ab_nincs));
//	for( U1* p_n = (U1*)p_nincs, *p_e = p_n + GPD_STRLEN(p_nincs); p_n < p_e; p_n++ )
//	{
//		ab_nincs[*p_n] = false;
//	}
//
//	while( p_start > p_str )
//	{
//		p_start--;
//		if( ab_nincs[*p_start] )
//			return (p_start - p_str);
//	}
//
//	return 0;
//}

int	GPF_STR_len(char* p_str, char* p_end, int& x, int& y, int& t );
I8	GPF_STR_lenI8(char* p_str, char* p_end, I8& x, I8& y, I8& t );

bool	GPF_STR_VAN_ABC( char* p_str );
char*	GPF_STR_PLUS_MINUS( char* p_buff, I8 n );
I8	GPF_VAN_NINCS( char* p_str, char* p_vn );
I8	GPF_STR_BLOCK( char* p_str, char* p_end, U1 end, char* p_comment );
int inline GPF_ACE( const char* p_file, int mode )
{
	//	00	Existence only
	//	02	Write-only
	//	04	Read-only
	//	06	Read and write
	if( !p_file )
		return -1; // ha negatív nem elérhetõ

	int io = _access( p_file, mode );
	return io;
}
bool inline GPF_MAKEDIR( char* p_buff, const char* p_new )
{
	if( p_new ? !*p_new : true )
		return false;
	int mi;
	char	*p_mkdir = p_buff+sprintf( p_buff, p_new ),
			*p_end = p_mkdir, *p_last = p_mkdir, 
			*p_first = NULL;
	while( p_mkdir = strrchr( p_buff, '\\') )
	{
		*(p_last = p_mkdir) = 0;
		if( !p_first )
			p_first = p_last;

		if( GPF_ACE( p_buff, 0 ) > -1 )
			break;

		mi = _mkdir( p_buff );
		if( mi > -1 )
			break;

	}
	while( p_last < p_first )
	{
		*p_last = '\\';
		p_last += GPD_STRLEN( p_last );
		if( GPF_ACE( p_buff, 0 ) > -1 )
			break;
		mi = _mkdir( p_buff );

	}
	return p_last == p_first;
}
U1 GPF_FILE_find(	U1 deep, char** pp_src, intptr_t *p_hFile, struct _finddatai64_t* p_cFile,
					//char* p_joly,
					U8* p_n, U8& n_byte, U8& n_file, U8& n_dir );
//{
//	if( p_hFile[deep] == -1L )
//		return 0;
//	
//	if( p_n[deep] )
//	{
//		while( _findnexti64( p_hFile[deep],&p_cFile[deep]) )
//		{
//			GPM_FF_CLOSE( p_hFile[deep] );
//			deep--;
//			if( !deep )
//				return 0;
//		}
//	} else {
//		//if( !p_joly )
//			GPM_STRCPY( pp_src[deep], "\\*" );
//		//else
//		//	sprintf( pp_src[deep], "\\%s", p_joly );
//		
//		p_hFile[deep] = _findfirsti64( pp_src[0], &p_cFile[deep] );
//		if( p_hFile[deep] == -1L )
//		{
//			GPM_FF_CLOSE(p_hFile[deep]);
//			if( !deep )
//				return 0;
//
//			deep--;
//			return deep;
//		}
//	}
//
//	p_n[deep]++;
//	if( p_cFile[deep].name[0] == '.' )
//	{
//		sprintf( pp_src[deep], "\\%s", p_cFile[deep].name );
//	} else {
//		bool b_dir = p_cFile[deep].attrib&0x10;
//		U1 nADD = 0;
//		if( pp_src[0] < pp_src[deep] )
//		{
//			if( pp_src[deep][-1] == '\\' )
//				nADD = 1;
//		} 
//
//		if( b_dir )
//		{
//			n_dir++;
//			pp_src[deep+1] = pp_src[deep] + sprintf( pp_src[deep], "\\%s"+nADD, p_cFile[deep].name );
//			deep++;
//			p_n[deep] = 0;
//			p_hFile[deep] = 0;
//			return deep;
//		} else {
//			sprintf( pp_src[deep], "\\%s"+nADD, p_cFile[deep].name );
//			n_byte += p_cFile[deep].size;
//			n_file++;
//		}
//	}
//	
//	return deep;
//}
class GPC_IDN
{
public:
	GPE_NET8	id;
	I8	n;
	GPC_IDN( U8* p_ull )
	{
		id = (GPE_NET8)p_ull[0];
		n = p_ull[1];
	}
	GPC_IDN& operator = ( const U8& idn )
	{
		U8* p_ull = (U8*)&idn;
		id = (GPE_NET8)p_ull[0];
		n = p_ull[1];
		return *this;
	}
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

I8 GPF_CON_n( void );
bool GPF_CON_on( void );
void GPF_CON_Write( const char *p_str );
void GPF_CON_pos( U2 x, U2 y );
void GPF_CON_cls( void );
void GPF_CON_FORMAT_Write( const char *p_format, ...);
class GPC_LAZY;


class GPC_ARR
{
public:
	U1* p_alloc;
	U8 n_alloc;
	~GPC_ARR()
	{
		_aligned_free( p_alloc );
	}
	/*{
		GPC_ARR* p_arr
		for( U8 n_done = 0, n_dzr = 0; n_done < n_src; n_done += n_dzr )
		{

		}
	}*/
	GPC_ARR* arr_exp( U8& n_start, U8 n_end, U8 n_exp )
	{
		if( !this )
		{
			if( !n_exp )
				return NULL;

			GPC_ARR* p_t = new GPC_ARR;
			if (!p_t)
				return NULL;

			p_t->p_alloc = NULL;
			p_t->n_alloc = 0;

			return p_t->arr_exp( n_start, n_end, n_exp );
		}

		U8 n_kill = n_alloc;
		U1* p_kill = p_alloc;

		if( !p_alloc )
		{
			n_end = n_start = 0;
		} else {
			if( n_start > n_kill )
				n_start = n_kill;

			if( n_end < n_start )
				n_end = n_start;
			else if( n_end > n_alloc )
				n_end = n_alloc;
		}

		n_alloc -= n_end;
		n_alloc += n_start + n_exp;

		if( n_alloc > 0)
		{
			if( n_kill == n_alloc )
				return this;
			U8 a16 = GPM_PAD( n_alloc, 0x10 );
			(p_alloc = (U1*)_mm_malloc( a16+0x10, 0x10 ))[n_alloc] = 0;

			if(n_start)
				gp_memcpy( p_alloc, p_kill, n_start );

			if (n_end < n_kill)
				gp_memcpy(p_alloc + n_start + n_exp, p_kill + n_end, n_kill - n_end);
		} else {
			p_alloc = NULL;
		}
		GPM_DELARRAY(p_kill);
		return this;
	}
	
	GPC_ARR* arr_ins( U1* p_str, U8 n_load, U8& n_start, U8 n_end)
	{
		GPC_ARR* p_arr = arr_exp( n_start, n_end, n_load);
		if (!p_arr)
			return NULL;
		if( !n_load )
			return p_arr;
		if( !p_str )
			return p_arr;
		U1* p_dst = p_arr->p_alloc + n_start;

		gp_memcpy(p_dst, p_str, n_load );
		return p_arr;
	}
	GPC_ARR* arr_read( char* p_file, U8& n_start, U8 n_end )
	{
		if (!p_file)
			return this;

		if( GPF_ACE(p_file, 4) < 0 )
			return this;

		FILE* p_f = fopen( p_file, "rb" );
		if( !p_f )
			return this;

		//GPF_PRI_LO();

		fpos_t n_load = 0;
		fseek(p_f, 0, SEEK_END );
		fgetpos(p_f, &n_load);
		fseek(p_f, 0, SEEK_SET);
		GPC_ARR* p_arr = this;

		if( n_load > 0 )
		{
			p_arr = p_arr->arr_exp( n_start, n_end, n_load );
			if( !p_arr )
				goto szasz;

			U1* p_mem = p_arr->p_alloc + n_start;
			U8 n = fread(p_mem, 1, n_load, p_f);
		} 

	szasz:
		fclose(p_f);

		return p_arr;
	}

	GPC_ARR* operator += ( const GPC_ARR& plus )
	{
		U8 s = -1;
		return arr_ins( plus.p_alloc, plus.n_alloc, s, -1 );
	}
	GPC_ARR* operator += ( const GPC_LAZY& plus );
	
};

inline U8 GPF_BUFF_read( char* p_buff, U8 n_byte, FILE* p_file )
{
	U8 n_r = 0;
	n_r = fread( p_buff, 1, n_byte, p_file );
	if( n_r == n_byte )
		return n_byte;
	if( !n_r )
		return 0;
	U4 n_err = 0, R0 = 0;
	while( n_r < n_byte )
	{
		if( n_err > 3 )
			return n_r;

		R0 = fread( p_buff+n_r, 1, n_byte-n_r, p_file );
		if( !R0 )
		{
			n_err++;
			Sleep(0);
			continue;
		}
		n_err = 0;
		n_r += R0;
	}
	return n_r;
}

inline U8 GPF_BUFF_write( char* p_buff, U8 n_byte, FILE* p_file )
{
	U8 n_w = 0;
	n_w = fwrite( p_buff, 1, n_byte, p_file );
	if( n_w == n_byte )
		return n_byte;
	if( !n_w )
		return 0;
	U4 n_err = 0, W0 = 0;
	while( n_w < n_byte )
	{
		if( n_err > 3 )
			return n_w;

		W0 = fwrite( p_buff+n_w, 1, n_byte-n_w, p_file );
		if( !W0 )
		{
			n_err++;
			Sleep(0);
			continue;
		}
		n_err = 0;
		n_w += W0;
	}
	return n_w;
}

class GPC_LAZY
{
public:
	union
	{
		struct{
			U1* p_alloc, a_setup[8];
			U8 n_load, n_alloc;
		};
		struct{
			U8* p_a8;
			U1	a_setup[8];
			U8	n_load, n_alloc;
		};
		struct{
			GPC3Dvxc* pVXC;
			U1	a_setup[8];
			U8	n_load, n_alloc;
		};
	};
	GPC_LAZY( void )
	{
		GPM_CLR;
	}
	GPC_LAZY( U1 n )
	{
		GPM_CLR;

		if( n < 1 )
			n = 4;

		a_setup[7] = n;
	}
	~GPC_LAZY()
	{
		_mm_free( p_alloc );
	}
	GPC_LAZY* lazy_dzr( U8& n_start, U8 n_end, const U1* p_src, U8 n_src, U1* pWIP, U2 n_wip, U24** ppW );
	GPC_LAZY* lazy_undzr(  U8& n_start, U8 n_end, U1* p_dzr, U8 n_dzr, U1* pWIP, U1* pDBG, U24* pWd );
	
	GPC_LAZY* lazy_strict( void )
	{
		if( this ? !n_load : true )
			return this;
			
		if( n_load < n_alloc )
		{
			U8 new_alloc = GPM_PAD( n_load, 0x10 );
			if( !new_alloc )
			{
				(*p_alloc) = n_load = 0;
				return this;
			}
			else if( n_alloc > new_alloc )
			{
				U1* p_kill = p_alloc;
				p_alloc = (U1*)_mm_malloc( n_alloc+0x10, 0x10 );
				gp_memcpy( p_alloc, p_kill, n_load );
				_mm_free( p_kill );
				n_alloc = new_alloc;
			}
		}
		p_alloc[ n_load ] = 0;
		return this;
	}


	GPC_LAZY* lazy_reset( void )
	{
		if( this ? !n_load : true )
			return this;
			
		if( n_load*2 < n_alloc )
		{
			U8 new_alloc = GPM_PAD( ((n_alloc*3)/4), 0x10 );
			if( !new_alloc )
			{
				(*p_alloc) = n_load = 0;
				return this;
			}
			else if( n_alloc > new_alloc )
			{
				_mm_free( p_alloc );
				n_alloc = new_alloc;
				p_alloc = (U1*)_mm_malloc( n_alloc+0x10, 0x10 );
			}
		}
		(*p_alloc) = n_load = 0;
		return this;
	}
	
	GPC_LAZY* lazy_add( const void* p_void, U8 n_byte, U8& n_start, U1 n = 0 )
	{
		if( !n_byte )
			return this;

		if( !this )
		{
			n_start = 0;
			GPC_LAZY* p_lazy = new GPC_LAZY( n );
			p_lazy->n_alloc = GPM_PAD( n_byte*p_lazy->a_setup[7], 0x10 );
			p_lazy->p_alloc = (U1*)_mm_malloc( p_lazy->n_alloc+0x10, 0x10 );
			if( !p_void )
			{
				p_lazy->p_alloc[p_lazy->n_load = n_byte] = 0;
				return p_lazy;
			}

			((U1*)gp_memcpy( p_lazy->p_alloc, p_void, n_byte ))[n_byte] = 0;
			p_lazy->n_load = n_byte;
			return p_lazy;
		}

		if( !n )
		{
			if( !a_setup[7] )
				a_setup[7] = 4;

			n = a_setup[7];
		} 
		else if( !a_setup[7] )
				a_setup[7] = n;


		if( n_start > n_load )
			n_start = n_load;
		else
			n_load = n_start;

		if( p_alloc ? (n_load+n_byte > n_alloc) : true )
		{
			U1* p_kill = p_alloc;
			n_alloc = GPM_PAD( (n_load+n_byte*n), 0x10 );

			p_alloc = (U1*)_mm_malloc( n_alloc+0x10, 0x10 );
			if( p_kill )
			{
				gp_memcpy( p_alloc, p_kill, n_load );
				_mm_free( p_kill );
			} else 
				n_load = 0;
		}
		if( !p_void )
		{
			n_load += n_byte;
			p_alloc[n_load] = 0;
			return this;
		}

		gp_memcpy( p_alloc+n_load, p_void, n_byte );
		n_load += n_byte;
		p_alloc[n_load] = 0;
		return this;
	}
	GPC_LAZY* lazy_plus(  const GPC_LAZY* p_b, U8& n_start )
	{
		return lazy_add( p_b->p_alloc, p_b->n_load, n_start, ( (p_b->n_load<=0x40) ? 0xf : 0x3 ) );
	}
	GPC_LAZY* lazy_sub( U8& n_start, U8 n_sub )
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
			gp_memcpy( p_alloc+n_start, p_alloc+n_start+n_sub, n_hi );
		}

		n_load -= n_sub;
		p_alloc[n_load] = 0;
		return this;
	}
	GPC_LAZY* lazy_exp( U8& n_start, U8 n_sub, U8 n_add, U1 n = 0 )
	{
		if( !this )
		{
			return lazy_add( NULL, n_add, n_start, n );
		}
		
		if( !n )
		{
			if( !a_setup[7] )
				a_setup[7] = 4;

			n = a_setup[7];
		} 
		else if( !a_setup[7] )
				a_setup[7] = n;

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
			n_alloc += GPM_PAD( (n_add*n), 0x10 );
			p_kill = p_alloc;
			p_alloc = (U1*)_mm_malloc( n_alloc+0x10, 0x10 );
			gp_memcpy( p_alloc, p_kill, n_start );
			gp_memcpy( p_alloc+dst_hi, p_kill+src_hi, n_hi );
			n_load = new_load;
			p_alloc[n_load] = 0;
			_mm_free( p_kill );
			return this;
		}

		if( n_hi )
		if( n_load <= dst_hi )
		{
			// nincsen átfedésben mehet sima kopival
			gp_memcpy( p_alloc+dst_hi, p_kill+src_hi, n_hi );
		} 
		else if( dst_hi < src_hi )
		{
			gp_memcpy( p_alloc + dst_hi, p_alloc + src_hi, n_hi );
		}
		else if( dst_hi > src_hi )
		{
			// dst_hi == src_hi akkor a memcpy úgy sem másolna
			__declspec(align(16)) U8 a_buff[32];
			U8 n_stp = sizeof(a_buff), i = n_hi;
			U1	*p_s = p_alloc+src_hi,
				*p_d = p_alloc+dst_hi;
			if( n_stp > n_hi )
				n_stp = n_hi;
			while( i >= n_stp )
			{
				i -= n_stp;
				gp_memcpy( a_buff, p_s+i, n_stp );
				gp_memcpy( p_d+i, a_buff, n_stp );
			}

			if( i )
			{
				gp_memcpy( a_buff, p_s, i );
				gp_memcpy( p_d, a_buff, i );
			}
		}

		//gp_memcpy( p_alloc+start, p_u1, n_u1 );
		
		n_load = new_load;
		p_alloc[n_load] = 0;
		return this;
	}

	GPC_LAZY* lazy_ins( const U1* p_u1, U8 n_u1, U8& n_start, U8 n_sub, U1 n = 0 )
	{
		if( !this )
		{
			//start = n_u1;
			return lazy_add( p_u1, n_u1, n_start, n );
		}

		lazy_exp( n_start, n_sub,  n_u1, n );
		gp_memcpy( p_alloc+n_start, p_u1, n_u1 );
		return this;
	}
	GPC_LAZY& operator = ( const GPC_LAZY& plus )
	{
		U8 s = 0;
		lazy_ins( plus.p_alloc, plus.n_load, s, -1 );

		return *this;
	}

	bool operator != ( const GPC_LAZY& b )
	{
		if( b.n_load != n_load )
			return true;
		U8 s = gp_memcmp( p_alloc, b.p_alloc, n_load )-p_alloc;
		return s < n_load;
	}

	bool operator == ( const GPC_LAZY& b )
	{
		return !(*this != b);
	}

	bool operator != ( const U1* p_b )
	{
		if( !this )
			return p_b; 

		if( !p_b )
			return true;

		if( p_b == p_alloc )
			return false; // tök ugyan az a memoria terület azaz biztos azonos, azaz nem nem egyenlõ
				
		return (gp_memcmp( p_alloc, p_b, n_load )-p_alloc) < n_load;
	}

	bool operator == ( const U1* p_b )
	{
		return !( *this != p_b );
	}

	GPC_LAZY* operator += ( const GPC_LAZY& plus )
	{
		U8 s = -1;
		return lazy_add( plus.p_alloc, plus.n_load, s );
	}
	GPC_LAZY* operator += ( const GPC_ARR& plus )
	{
		U8 s = -1;
		return lazy_add( plus.p_alloc, plus.n_alloc, s );
	}
	GPC_LAZY* lazy_read( char* p_file, U8& n_start, U1 n = 0 )
	{
		if( !p_file )
			return this;
		
		if( !this )
			n_start = 0;

		if( GPF_ACE(p_file, 4) < 0 )
			return this;

		FILE* p_f = fopen( p_file, "rb" );
		if( !p_f )
			return this;

		//GPF_PRI_LO();

		fpos_t n_byte = 0;
		fseek(p_f, 0, SEEK_END );
		fgetpos(p_f, &n_byte);
		fseek(p_f, 0, SEEK_SET);
		GPC_LAZY* p_lazy = this;

		if( n_byte > 0 )
		{
			//U8 n_pos = p_lazy->n_load;
			p_lazy = p_lazy->lazy_add( NULL, n_byte, n_start, n );
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
		fclose(p_f);

		return p_lazy;
	}
	GPC_LAZY* lazy_write( const char* p_file, bool b_over = true )
	{
		if( this ? !n_load : true )
			return this;
		
		if( !b_over )
		if( GPF_ACE(p_file, 4) < 0 )
			return this;

		char s_buff[GPD_MAX_PATH];
		FILE* p_f = fopen( p_file, "wb" );
		U8 n_w = 0, n_err = 0, W0 = 0;

		if( !p_f )
		{
			if( !GPF_MAKEDIR( s_buff, p_file ) )
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
				Sleep(0);
				continue;
			}
			n_err = 0;
			n_w += W0;
		}
		
close:
		fclose(p_f);
szasz:
		return this;
	}
	GPC_LAZY* lazy_format( U8& n_start, const char* p_format, ... );
	GPC_LAZY* lasy_req_close( void )
	{
		if( !this )
			return this;

		a_setup[0] = 1;
		return this;
	}
	bool q_close( void )
	{
		if( !this )
			return false;
		return a_setup[0];
	}
	U4 tree_fnd( U4 id, U4& n );
	GPC_LAZY* tree_add( U4 id, U4& n );
	U8 tree_fnd8( U8 id, U8& n );
	GPC_LAZY* tree_add8( U8 id, U8& n );
};

class GPC_IDX
{
public:
	U8 ixPATH, dzPOS, nSIZE, dzCK, 
		dzEND;
	char	*pNAME,
			*p_name;
	U2 posN, nN, nP, nF;
	U4 nUNDO;

	GPC_IDX()
	{
		GPM_CLR;
	}
	~GPC_IDX()
	{
		GPM_DELARRAY(pNAME);
	}
	char* idx_scan( U8 pos, char* p_s );
	char* idx_ck( U8 pos, char* p_s );
	GPC_IDX* build( char* p_pub, FILE* p_fd, U8 n_size, U8& n_load, U8& n_alloc );
};
class GPC_IDX_TREE
{
public:
	U8 idx, m, l, r, up;
	U2	nL;
	GPC_IDX_TREE(){};
	GPC_IDX_TREE* build( GPC_IDX* p_idx, U8 n_idx );
	GPC_IDX_TREE* null()
	{
		GPM_CLR;
		return this;
	}
	void left( GPC_IDX_TREE* p_root, GPC_IDX_TREE* p_it )
	{
		if( !p_it->m )
		{
			p_it->m = this-p_root;
			l = p_it-p_root;
			return;
		}

		if( p_root[p_it->m].l == p_it-p_root )
			p_root[p_it->m].l = this-p_root;
		else
			p_root[p_it->m].r = this-p_root;
		m = p_it->m;
		p_it->m = this-p_root;
		l = p_it-p_root;
	}
	void right( GPC_IDX_TREE* p_root, GPC_IDX_TREE* p_it )
	{
		if( !p_it->m )
		{
			p_it->m = this-p_root;
			r = p_it-p_root;
			return;
		}
		if( p_root[p_it->m].up == p_it-p_root )
			p_root[p_it->m].up = this-p_root;
		else if( p_root[p_it->m].l == p_it-p_root )
			p_root[p_it->m].l = this-p_root;
		else 
			p_root[p_it->m].r = this-p_root;

		m = p_it->m;
		p_it->m = this-p_root;
		r = p_it-p_root;
	}
};

class U12
{
public:
	union{
		struct
		{
			U1 x, y;
		};
		struct
		{
			U1 aXY[2];
		};
		struct
		{
			gpeSTF	stf;
			U1		lev;
		};
	};
	U12(void){};
	U12( U1 _x, U1 _y )
	{
		x = _x;
		y = _y;
	};
};
// uint 1 -------------------------------------------------------

class U14
{
public:
	union{
		struct
		{
			U1 B, G, R, A;
		};
		struct
		{
			char x, y, z, w;
		};
		struct
		{
			U1 aBGRA[4];
		};
		struct
		{
			U4 u4;
		};
		struct
		{
			gpeSTF	stf;
			U1		lev,
					tim;	// or light
			char	ent;
		};
		struct
		{
			gpeMAP	pln;
			U1		lev,
					tim;	// or light
			char	ent;
		};
	};
	U14(void){};

	U14( U1 b, U1 g, U1 r = 0, U1 a = 0 )
	{
		B = b;
		G = g;
		R = r;
		A = a;
	}
	U14( U4 u )
	{
		u4 = u;
	};
	U14( const U44& b );
	U14( const I44& b );
	U14& operator = ( U1 a )
	{
		B = G = R = A = a;
		return *this;
	}
	U14 operator & ( U1 m ) const 
	{
		return U14(B&m, G&m, R&m, A&m);
	}
	
	U14 operator / (U1 d) const
	{
		return U14(B/d, G/d, R/d, A/d);
	}

	U84	operator * (U4 b) const;
	U4	operator * (const U44& b) const;
	
	U14& swap_BA(void)
	{
		U1 t = A;
		A = B;
		B = t;
		return *this;
	}
	U14& swap_GA(void)
	{
		U1 t = A;
		A = G;
		G = t;
		return *this;
	}
	U14& swap_RA(void)
	{
		U1 t = A;
		A = R;
		R = t;
		return *this;
	}
	U14& swap_BR(void)
	{
		U1 t = R;
		R = B;
		B = t;
		return *this;
	}
	U14& swap_GR(void)
	{
		U1 t = R;
		R = G;
		G = t;
		return *this;
	}
	U14& swap_BG(void)
	{
		U1 t = G;
		G = B;
		B = t;
		return *this;
	}
	U4 gr_xZYs2fnd5( void )
	{
		U4 fnd = A%6;
		fnd<<=5;
		fnd |= G&0x1f;
		fnd<<=5;
		fnd |= R&0x1f;
		fnd<<=5;
		fnd |= B&0x1f;
		return fnd;
	}
	
	U14& fnd2gr_xZYs5( U4 fnd )
	{
		B = fnd&0x1f;
		fnd >>= 5;
		R = fnd&0x1f;
		fnd >>= 5;
		G = fnd&0x1f;
		fnd >>= 5;
		A = fnd%6;
		return *this;
	}
	
	U14& fnd2gr_xYZs5(U4 fnd)
	{
		B = fnd & 0x1f;
		fnd >>= 5;
		G = fnd & 0x1f;
		fnd >>= 5;
		R = fnd & 0x1f;
		fnd >>= 5;
		A = fnd % 6;
		return *this;
	}
	U14 stp_fnd( const I44& stp ) const;

	U4 gr_xYZs2fnd5( void )
	{
		U4 fnd = A%6;
		fnd<<=5;
		fnd |= R&0x1f;
		fnd<<=5;
		fnd |= G&0x1f;
		fnd<<=5;
		fnd |= B&0x1f;
		return fnd;
	}
	

	/*U4 gr_xZYs2fnd8(void)
	{
		U4 fnd = A % 6;
		fnd <<= 8;
		fnd |= G & 0xff;
		fnd <<= 8;
		fnd |= R & 0xff;
		fnd <<= 8;
		fnd |= B & 0xff;
		return fnd;
	}
	U14& fnd2gr_xYZs8(U4 fnd)
	{
		B = fnd & 0xff;
		fnd >>= 8;
		G = fnd & 0xff;
		fnd >>= 8;
		R = fnd & 0xff;
		fnd >>= 8;
		A = fnd % 6;
		return *this;
	}
	U14& fnd2gr_xZYs8(U4 fnd)
	{
		B = fnd & 0xff;
		fnd >>= 8;
		R = fnd & 0xff;
		fnd >>= 8;
		G = fnd & 0xff;
		fnd >>= 8;
		A = fnd % 6;
		return *this;
	}*/
	ID3D11Buffer* upl_n(  ID3D11Device* p_dev, ID3D11DeviceContext* p_ctx, ID3D11Buffer* pX11cb, U4 n );
	/*U14& swap_BR( void )
	{
		U1 t = B;
		B = R;
		R = t;
		return *this;
	}
	U14& swap_GA( void )
	{
		U1 t = G;
		G = A;
		A = t;
		return *this;
	}
	U14& swap_BG( void )
	{
		U1 t = B;
		B = G;
		G = t;
		return *this;
	}
	U14& swap_RA( void )
	{
		U1 t = R;
		R = A;
		A = t;
		return *this;
	}
	U14& swap_GR( void )
	{
		U1 t = G;
		G = R;
		R = t;
		return *this;
	}
	U14& swap_BA( void )
	{
		U1 t = B;
		B = A;
		A = t;
		return *this;
	}*/
};

class U24
{
public:
	U2 x,y,z,w;
	
	U24( void ){};
	U24( const I44& i44 );
	U24( const U84& i44 );
	U24( U2 u )
	{
		x = 
		y = 
		z = 
		w = u;
	}
	U24& i2xyz( U8 i, U2 r )
	{
		x = i%r;
		y = i/r;
		z = y/r;
		y %= r;
		return *this;
	}
	U24& operator = ( int i )
	{
		if( !i )
			return null();
		x =
		y = 
		z = 
		w = i;
		return *this;
	}
	U24& operator = ( const U14& b )
	{
		x = b.x;
		y = b.y;
		z = b.z;
		w = b.w;
		return *this;
	}
	U24& operator = ( const U84& b );
	U24( U2 _x, U2 _y, U2 _z = 0, U2 _w = 0 )
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}
	U24( char* p_str,  char** pp_str )
	{
		str2u24( p_str, pp_str );
	}
	U24& str2u24( char* p_str,  char** pp_str )
	{
		p_str += GPD_NINCS( p_str, " \t\r\n,." );
		x = strtol( p_str, &p_str, 10 );
		p_str += GPD_NINCS( p_str, " \t\r\n,." );
		y = strtol( p_str, &p_str, 10 );
		p_str += GPD_NINCS( p_str, " \t\r\n,." );
		z = strtol( p_str, &p_str, 10 );
		p_str += GPD_NINCS( p_str, " \t\r\n,." );
		w = strtol( p_str, &p_str, 10 );
		if( pp_str )
			*pp_str = p_str;
		return *this;
	}

	U24& operator &= ( const U14& b )
	{
		x *= b.B;
		y *= b.G;
		z *= b.R;
		w *= b.A;
		return *this;
	}
	U24& operator &= ( const U24& b )
	{
		x *= b.x;
		y *= b.y;
		z *= b.z;
		w *= b.w;
		return *this;
	}
	U24& operator /= ( U2 b )
	{
		if( !b )
		{
			x = y = z = w = -1;
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
	U24& operator /= ( const U14& b )
	{
		x = b.B ? x/b.B : -1;
		y = b.G ? y/b.G : -1;
		z = b.R ? z/b.R : -1;
		w = b.A ? w/b.A : -1;
		return *this;
	}
	U24& operator /= ( const U24& b )
	{
		x = b.x ? x/b.x : -1;
		y = b.y ? y/b.y : -1;
		z = b.z ? z/b.z : -1;
		w = b.w ? w/b.w : -1;
		return *this;
	}

	U24& operator += ( const U24& b )
	{
		x += b.x;
		y += b.y;
		z += b.z;
		w += b.w;
		return *this;
	}
	U24& operator -= ( const U24& b )
	{
		x -= b.x;
		y -= b.y;
		z -= b.z;
		w -= b.w;
		return *this;
	}
	U24 operator & ( const U14& b ) const
	{
		U24 out = *this;
		out &= b;
		return out;
	}
	U24 operator / ( const U14& b ) const
	{
		U24 out = *this;
		out /= b;
		return out;
	}
	U24 operator + ( const U24& b ) const
	{
		U24 out = *this;
		out += b;
		return out;
	}
	U24 operator - ( const U24& b ) const
	{
		U24 out = *this;
		out -= b;
		return out;
	}

	U84 operator * ( U2 b ) const;
	U84 operator & ( const U24& b ) const;
	U84 operator / ( const U24& b ) const;
	U8 operator * ( const U24& b ) const;
	
	bool operator != ( const U24& b ) const
	{
		if( x != b.x )
			return true;
		if( y != b.y )
			return true;
		if( z != b.z )
			return true;
		if( w != b.w )
			return true;
		return false;
	}
	bool operator == ( const U24& b ) const
	{
		return !(*this != b);
	}

	U24& null( void )
	{
		GPM_CLR;
		return *this;
	}

	U24& xy( U24 xy )
	{
		gp_memcpy( &x, &xy, 2*sizeof(x) );
		return *this;
	}
	U24& zw( U24 xy )
	{
		gp_memcpy( &z, &xy, 2*sizeof(x) );
		return *this;
	}

	U24 xy( void ) const
	{
		return U24( x, y );
	}
	U24& xy( I44 xy );
	
	U24 yzw( U2 u4 = 0 ) const
	{
		return U24( y,z,w, u4 );
	}
	U24 xzy( void ) const
	{
		return U24( x, z, y );
	}
	U24 zw( void ) const
	{
		return U24( z, w );
	}
	U24& zw( I44 xy );
	U24 xyz(void)
	{
		return U24(x,y,z);
	}
	double qlen( void ) const
	{
		__declspec(align(16)) double aD[4];
		aD[0] = x;
		aD[1] = y;
		aD[2] = z;
		aD[3] = w;

		return gp_d4_qlen( aD, aD );
	}
	double qlen_xyz( void ) const
	{
		__declspec(align(16)) double aD[4];
		aD[0] = x;
		aD[1] = y;
		aD[2] = z;
		aD[3] = 0.0;
		//gp_d4_qlen( aD, aD );

		return gp_d4_qlen( aD, aD );
	}
	double qlen_xy( void ) const
	{
		__declspec(align(16)) double aD[4];
		aD[0] = x;
		aD[1] = y;
		aD[2] = 0;
		aD[3] = 0.0;
		//gp_d4_qlen( aD, aD );

		return gp_d4_qlen( aD, aD );
	}
	U8 sum( void ) const
	{
		return x+y+z+w;
	}

	U8 sum_xyz( void ) const
	{
		return x+y+z;
	}


	U8 sum_xy( void ) const
	{
		return x+y;
	}
	U8 sum_yz( void ) const
	{
		return y+z;
	}
	U8 sum_zw( void ) const
	{
		return z+w;
	}


	U8 sum_xz( void ) const
	{
		return x+z;
	}
	U8 sum_yw( void ) const
	{
		return y+w;
	}

	
	
	U8 area_xy( void )
	{
		U8 arr = x;
		return arr*y;
	}
	U8 vol_xyz( void )
	{
		U8 vol = x;
		return vol*y*z;
	}

	
	
	U2 tree_add( U2 u4, U2 n_t )
	{
		// x érték, y mama, z kicsiegyenlõ, w nagyobb
		this[n_t].null().x = u4;
		if( !n_t )
			return 1;

		U2 i = 0;
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
	U2 tree_fnd( U2 u4, U2 n_t )
	{
		// figyelem/WARNING
		// visza térési érték, az, ameddig eljutott a fában
		// kell még egy ellenörzés, hogy tényleg azonos e a két elem
		U2 i = 0;
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



	U2 dict_add( U1* p_src, U2& m, U24& w )
	{

		m = 0;

		U24* p_w = this;
		U2	n_w = &w-p_w;
		if( !n_w )
			return 1;

		U2	cd = 0, e, i, 
			mx = 0,
			o = 0;

		U1	*p_s = p_src+w.x,
			*p_d;

		
		while( cd < n_w )
		{
			p_d = p_src + p_w[cd].x;
			e = p_w[cd].y;
			if( e > w.y )
				e = w.y;
			
			if( e < 2 )
			{
				i = (*p_d == *p_s);
			} else 
				i = (U1*)gp_memcmp( p_d, p_s, e ) - p_d;
			/*for( i = 0; i < e; i++ )
			{
				if( p_s[i] == p_d[i] )
					continue;
				break;
			}*/
			if( e == i )
			if( w.y == p_w[cd].y )
				return n_w; // azaz már szerepel a fában, készen vagyunk

			if( i >= p_w[cd].y )
			{
				// a w hosszabb
				// ha egyenõ lett volna akkor ott feljebb kilép
				// mert megtalálta
				if( !p_w[cd].z )
				{
					// .z a bal oldal
					m = cd;
					p_w[cd].z = n_w;
					return n_w+1;
				}

				m = cd;
				cd = p_w[cd].z;
				continue;
			}
			else if( i == w.y )
			{
				// ez azt jelenti, hogy a w teljes hosszában azonos 
				// a p_w[cd] elejével
				
				// azt is jelenti w rövidebb, azaz a p_w[cd] alá kell beszúrni a w-t

				// w baloldalára emeljük
				w.z = cd;
				
				// majd megnézzük a maminak melyik oldalán volt
				if( p_w[m].z == cd )
					p_w[m].z = n_w;
				else 
					p_w[m].w = n_w;

				return n_w+1; 
			}

			if( p_d[i] < p_s[i] )
			{
				if( !p_w[cd].w )
				{
					m = cd;
					p_w[cd].w = n_w;
					return n_w+1;
				}

				m = cd;
				cd = p_w[cd].w;
				continue;
			} 

			if( p_d[i] > p_s[i] )
			if( !p_w[cd].z )
			{
				m = cd;
				p_w[cd].z = n_w;
				return n_w+1;
			}

			m = cd;
			cd = p_w[cd].z;
		}

		return n_w;
	}
	U2 dict_find( U1* p_src, U24& w )
	{
		U24* p_w = this;
		U2	n_w = &w-p_w;
		if( !n_w )
			return 0;

		U2	cd = 0, e, i, m = 0, mx = 0, o = n_w;
		U1	*p_s = p_src+w.x,
			*p_d;

		while( cd < n_w )
		{
			p_d = p_src+p_w[cd].x;
			e = p_w[cd].y;
			if( e > w.y )
				e = w.y;

			if( e < 2 )
			{
				i = (*p_d == *p_s);
			} else 
				i = (U1*)gp_memcmp( p_d, p_s, e ) - p_d;
			/*for( i = 0; i < e; i++ )
			{
				if( p_s[i] == p_d[i] )
					continue;
				break;
			}*/
			if( i == e )
			if( !p_w[cd].z || p_w[cd].y == w.y )
				return cd;

			/*{
				if( p_w[cd].y == w.y )
					return cd;
				if( w.y == i ? p_w[cd].y > w.y : false )
					return o;
				if( !p_w[cd].z )
					return cd;
			}*/

			if( i >= p_w[cd].y )
			{
				// a w hosszabb
				// ha egyenõ lett volna akkor ott feljebb kilép
				// mert megtalálta
				if( i == p_w[cd].y )
				if( mx < i )
				{
					o = cd;
					mx = i;
				}
				// ez hosszabb
				if( !p_w[cd].z )
					return o;

				cd = p_w[cd].z;
				continue;
			}
			else if( i == w.y )
			{
				// ez azt jelenti, hogy a w teljes hosszában azonos 
				// a p_w[cd] elejével
				
				// azt is jelenti w rövidebb, azaz a p_w[cd]-nál
				// a rövidebbet választjuk
				return o; 
			}

			if( p_d[i] < p_s[i] )
			{
				if( !p_w[cd].w )
					return o;

				cd = p_w[cd].w;
				continue;
			} 

			if( p_d[i] > p_s[i] )
			if( !p_w[cd].z )
				return o;

			cd = p_w[cd].z;
		}
		return o;
	}
	

	U2 code_add( char* pCOD, U2& m, U24& w )
	{
		m = 0;
		U24* p_w = this;
		U2	n_w = &w-p_w;
		if( !n_w )
			return 1;

		U2	cd = 0, e, i, 
			mx = 0,
			o = 0;

		char	*p_s = pCOD+w.x,
				*p_d;

		
		while( cd < n_w )
		{
			p_d = pCOD + p_w[cd].x;
			e = p_w[cd].y;
			if( e > w.y )
				e = w.y;

			for( i = 0; i < e; i++ )
			{
				if( p_s[i] == p_d[i] )
					continue;
				break;
			}
			if( e == i )
			if( w.y == p_w[cd].y )
				return n_w;

			if( i >= p_w[cd].y )
			{
				// ez nagyobb
				if( !p_w[cd].z )
				{
					m = cd;
					p_w[cd].z = n_w;
					return n_w+1;
				}

				cd = p_w[cd].z;
				continue;
			}
			else if( i == w.y )
			{
				// feltolja ezt a szót mert w.n az elején van
				//w.m = p_w[code].m;
				w.z = cd;
				//p_w[code].m = n_w;

				if( p_w[m].z == cd )
					p_w[m].z = n_w;
				else 
					p_w[m].w = n_w;

				return n_w+1; 
			}

			if( p_d[i] < p_s[i] )
			{
				if( !p_w[cd].w )
				{
					m = cd;
					p_w[cd].w = n_w;
					return n_w+1;
				}

				m = cd;
				cd = p_w[cd].w;
				continue;
			} 

			if( p_d[i] > p_s[i] )
			if( !p_w[cd].z )
			{
				m = cd;
				p_w[cd].z = n_w;
				return n_w+1;
			}

			m = cd;
			cd = p_w[cd].z;
		}

		return n_w;
	}
	U2 code_find( char* pCOD, U24& w, bool b_mx = false )
	{
		U24* p_w = this;
		U2	n_w = &w-p_w;
		if( !n_w )
			return 0;

		U2	cd = 0, e, i, m = 0, mx = 0, out = n_w;
		char	*p_s = pCOD+w.x,
				*p_d;

		while( cd < n_w )
		{
			p_d = pCOD+p_w[cd].x;
			e = p_w[cd].y;
			if( e > w.y )
				e = w.y;
			for( i = 0; i < e; i++ )
			{
				if( p_s[i] == p_d[i] )
					continue;
				break;
			}
			if( i == e )
			{
				if( p_w[cd].y == w.y )
					return cd;
				if( w.y == i ? p_w[cd].y > w.y : false )
					return out;
				if( !p_w[cd].z )
					return cd;
			}

			if( i >= p_w[cd].y )
			{
				if( i == p_w[cd].y )
				if( mx < i )
				{
					if( b_mx )
						out = cd;
					mx = i;
				}
				// ez nagyobb
				if( !p_w[cd].z )
					return out;

				cd = p_w[cd].z;
				continue;
			}

			if( p_d[i] < p_s[i] )
			{
				if( !p_w[cd].w )
					return out;

				cd = p_w[cd].w;
				continue;
			} 

			if( p_d[i] > p_s[i] )
			if( !p_w[cd].z )
				return out;

			cd = p_w[cd].z;
		}
		return out;
	}
};

enum gpeOP : U1
{
	gpeOP_NOP,

	gpeOP_HD,
	gpeOP_RLE,
	gpeOP_LEFT,
	gpeOP_LEFT2,
	gpeOP_DICK,
	gpeOP_DICK_DBG,
	gpeOP_BIT,
	gpeOP_BIT2,
	gpeOP_DICK2,
	gpeOP_DICK3,
	gpeOP_DICK4,
	gpeOP_DICK5,
	gpeOP_DICK6,
	

	gpeOP_UNDZR,

	gpeOP_RLE2,

	gpeOP_LAST,
	gpeOP_END,
};

class GPCHD
{
public:
	U2		nw, nc;		//4
	gpeOP	pr, is;			//2		//6
	U1		sof, sw;		//2		//8
	U2		n_src, n_out;	//4		//12
	U4		chk;			//4		//16
};

class GPC_HD : public GPCHD
{
public:
	//gpeOP	pr, is;			//2
	//U1		sof, sw;		//2		//4
	//U2		n_src, n_out;	//4		//8
	//U4		chk, ver;		//4		//16

	U1* reset( gpeOP p, gpeOP i, U2 n ) //, U1 s = 0 )
	{
		GPM_CLR;
		sof = sizeof(*this);
		pr = p; is = i;
		n_src = n;
		/*n_out = 0;
		chk = 0;*/
		return ((U1*)this)+sof;
	}

	U1* p_buff( void )
	{
		return (U1*)this;
	}
		
	U4 do_chk( void )
	{
		U4 c = 0;
		U1* p_b = p_buff();
		for( U2 i = sof; i < n_out; i++ )
			c += p_b[i];
		c += n_out-sof;
		return c;
	}
	GPC_HD* enc( U1 *pWIP, const U1 *pIN, U2 nIN, U24** ppW = NULL, gpeOP pre = gpeOP_NOP );
	U2		dec( U1 *pOUT, U1 *pWIP, U1* pDBG, U24* pWd );
};


// unsigned int 4 -------------------------------------------------------
class U44
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
			U14	gr, ss;
			U4	fac, nPX;
		};
	};
	U44( void ){};
	U44( const I44& i44 );
	U44( const float4& f4 );
	U44( const U84& i44 );
	U44( U4 u )
	{
		x = 
		y = 
		z = 
		w = u;
	}
	U44& i2xyz( U8 i, U2 r )
	{
		x = i%r;
		y = i/r;
		z = y/r;
		y %= r;
		return *this;
	}
	U44& operator = ( int i )
	{
		if( !i )
			return null();
		x =
		y = 
		z = 
		w = i;
		return *this;
	}
	U44& operator = ( const U14& b )
	{
		x = b.x;
		y = b.y;
		z = b.z;
		w = b.w;
		return *this;
	}
	U44& operator = ( const U84& b );
	U44( U4 _x, U4 _y, U4 _z = 0, U4 _w = 0 )
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}
	U44( char* p_str,  char** pp_str )
	{
		str2u44( p_str, pp_str );
	}
	U44& str2u44( char* p_str,  char** pp_str )
	{
		p_str += GPD_NINCS( p_str, " \t\r\n,." );
		x = strtol( p_str, &p_str, 10 );
		p_str += GPD_NINCS( p_str, " \t\r\n,." );
		y = strtol( p_str, &p_str, 10 );
		p_str += GPD_NINCS( p_str, " \t\r\n,." );
		z = strtol( p_str, &p_str, 10 );
		p_str += GPD_NINCS( p_str, " \t\r\n,." );
		w = strtol( p_str, &p_str, 10 );
		if( pp_str )
			*pp_str = p_str;
		return *this;
	}

	U44& operator &= ( const U14& b )
	{
		x *= b.B;
		y *= b.G;
		z *= b.R;
		w *= b.A;
		return *this;
	}
	U44& operator &= ( const U44& b )
	{
		x *= b.x;
		y *= b.y;
		z *= b.z;
		w *= b.w;
		return *this;
	}
	U44& operator /= ( U4 b )
	{
		if( !b )
		{
			x = y = z = w = -1;
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
	U44& operator /= ( const U14& b )
	{
		x = b.B ? x/b.B : -1;
		y = b.G ? y/b.G : -1;
		z = b.R ? z/b.R : -1;
		w = b.A ? w/b.A : -1;
		return *this;
	}
	U44& operator /= ( const U44& b )
	{
		x = b.x ? x/b.x : -1;
		y = b.y ? y/b.y : -1;
		z = b.z ? z/b.z : -1;
		w = b.w ? w/b.w : -1;
		return *this;
	}

	U44& operator += ( const U44& b )
	{
		x += b.x;
		y += b.y;
		z += b.z;
		w += b.w;
		return *this;
	}
	U44& operator -= ( const U44& b )
	{
		x -= b.x;
		y -= b.y;
		z -= b.z;
		w -= b.w;
		return *this;
	}
	U44 operator & ( const U14& b ) const
	{
		U44 out = *this;
		out &= b;
		return out;
	}
	U44 operator / ( const U14& b ) const
	{
		U44 out = *this;
		out /= b;
		return out;
	}
	U44 operator + ( const U44& b ) const
	{
		U44 out = *this;
		out += b;
		return out;
	}
	U44 operator - ( const U44& b ) const
	{
		U44 out = *this;
		out -= b;
		return out;
	}

	U84 operator * ( U4 b ) const;
	U84 operator / (U4 b) const;
	U84 operator & ( const U44& b ) const;
	U84 operator / ( const U44& b ) const;
	U8 operator * ( const U44& b ) const;
	
	U84 operator & ( const I44& b ) const;
	U84 operator / ( const I44& b ) const;
	U8 operator * ( const I44& b ) const;
	
	bool operator != ( const U44& b ) const
	{
		if( x != b.x )
			return true;
		if( y != b.y )
			return true;
		if( z != b.z )
			return true;
		if( w != b.w )
			return true;
		return false;
	}
	bool operator == ( const U44& b ) const
	{
		return !(*this != b);
	}

	U44& null( void )
	{
		GPM_CLR;
		return *this;
	}

	U44& xy( U44 xy )
	{
		gp_memcpy( &x, &xy, 2*sizeof(x) );
		return *this;
	}
	U44& zw( U44 xy )
	{
		gp_memcpy( &z, &xy, 2*sizeof(x) );
		return *this;
	}

	U44 xy( void ) const
	{
		return U44( x, y );
	}
	U44& xy( I44 xy );
	
	U44 yzw( U4 u4 = 0 ) const
	{
		return U44( y,z,w, u4 );
	}
	U44 xzy( void ) const
	{
		return U44( x, z, y );
	}
	U44 zw( void ) const
	{
		return U44( z, w );
	}
	U44& zw( I44 xy );
	U44 xyz(void)
	{
		return U44(x,y,z);
	}
	double qlen( void ) const
	{
		__declspec(align(16)) double aD[4];
		aD[0] = x;
		aD[1] = y;
		aD[2] = z;
		aD[3] = w;

		return gp_d4_qlen( aD, aD );
	}
	double qlen_xyz( void ) const
	{
		__declspec(align(16)) double aD[4];
		aD[0] = x;
		aD[1] = y;
		aD[2] = z;
		aD[3] = 0.0;
		//gp_d4_qlen( aD, aD );

		return gp_d4_qlen( aD, aD );
	}
	double qlen_xy( void ) const
	{
		__declspec(align(16)) double aD[4];
		aD[0] = x;
		aD[1] = y;
		aD[2] = 0;
		aD[3] = 0.0;
		//gp_d4_qlen( aD, aD );

		return gp_d4_qlen( aD, aD );
	}
	U8 sum( void ) const
	{
		return x+y+z+w;
	}

	U8 sum_xyz( void ) const
	{
		return x+y+z;
	}


	U8 sum_xy( void ) const
	{
		return x+y;
	}
	U8 sum_yz( void ) const
	{
		return y+z;
	}
	U8 sum_zw( void ) const
	{
		return z+w;
	}


	U8 sum_xz( void ) const
	{
		return x+z;
	}
	U8 sum_yw( void ) const
	{
		return y+w;
	}

	
	
	U8 area_xy( void )
	{
		U8 arr = x;
		return arr*y;
	}
	U8 vol_xyz( void )
	{
		U8 vol = x;
		return vol*y*z;
	}

	U4 gr2q( void )
	{
		if(w >= 6)
			w %= 6;
		register U4 q = w;
		q <<= gpeQbyz;
		q |= z%gpeQyz;
		q <<= gpeQbyz;
		q |= y%gpeQyz;
		q <<= gpeQbyz;
		q |= x%gpeQx;
		return q;
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
	U4 dict_add( char* p_src, U4& m, U44& w )
	{
		m = 0;
		U44* p_w = this;
		U2	n_w = &w-p_w;
		if( !n_w )
			return 1;

		U4	cd = 0, e, i, 
			mx = 0,
			o = 0;

		char	*p_s = p_src+w.x,
				*p_d;

		
		while( cd < n_w )
		{
			p_d = p_src + p_w[cd].x;
			e = p_w[cd].y;
			if( e > w.y )
				e = w.y;

			for( i = 0; i < e; i++ )
			{
				if( p_s[i] == p_d[i] )
					continue;
				break;
			}
			if( e == i )
			if( w.y == p_w[cd].y )
				return n_w;

			if( i >= p_w[cd].y )
			{
				// ez nagyobb
				if( !p_w[cd].z )
				{
					m = cd;
					p_w[cd].z = n_w;
					return n_w+1;
				}

				cd = p_w[cd].z;
				continue;
			}
			else if( i == w.y )
			{
				// feltolja ezt a szót mert w.n az elején van
				//w.m = p_w[code].m;
				w.z = cd;
				//p_w[code].m = n_w;

				if( p_w[m].z == cd )
					p_w[m].z = n_w;
				else 
					p_w[m].w = n_w;

				return n_w+1; 
			}

			if( p_d[i] < p_s[i] )
			{
				if( !p_w[cd].w )
				{
					m = cd;
					p_w[cd].w = n_w;
					return n_w+1;
				}

				m = cd;
				cd = p_w[cd].w;
				continue;
			} 

			if( p_d[i] > p_s[i] )
			if( !p_w[cd].z )
			{
				m = cd;
				p_w[cd].z = n_w;
				return n_w+1;
			}

			m = cd;
			cd = p_w[cd].z;
		}

		return n_w;
	}
	U4 dict_find( char* p_src, U44& w )
	{
		U44* p_w = this;
		U4	n_w = &w-p_w;
		if( !n_w )
			return 0;

		U4	cd = 0, e, i, m = 0, mx = 0, o = n_w;
		char	*p_s = p_src+w.x,
				*p_d;

		while( cd < n_w )
		{
			p_d = p_src+p_w[cd].x;
			e = p_w[cd].y;
			if( e > w.y )
				e = w.y;
			for( i = 0; i < e; i++ )
			{
				if( p_s[i] == p_d[i] )
					continue;
				break;
			}
			if( i == e )
			{
				if( p_w[cd].y == w.y )
					return cd;
				if( w.y == i ? p_w[cd].y > w.y : false )
					return o;
				if( !p_w[cd].z )
					return cd;
			}

			if( i >= p_w[cd].y )
			{
				if( i == p_w[cd].y )
				if( mx < i )
				{
					o = cd;
					mx = i;
				}
				// ez nagyobb
				if( !p_w[cd].z )
					return o;

				cd = p_w[cd].z;
				continue;
			}

			if( p_d[i] < p_s[i] )
			{
				if( !p_w[cd].w )
					return o;

				cd = p_w[cd].w;
				continue;
			} 

			if( p_d[i] > p_s[i] )
			if( !p_w[cd].z )
				return o;

			cd = p_w[cd].z;
		}
		return o;
	}
	ID3D11Buffer* upl_n( ID3D11Device* p_dev, ID3D11DeviceContext* p_ctx, ID3D11Buffer* p_ps, U4 n );
	

	U4 code_add( char* pCOD, U4& m, U44& w )
	{
		m = 0;
		U44* p_w = this;
		U2	n_w = &w-p_w;
		if( !n_w )
			return 1;

		U4	cd = 0, e, i, 
			mx = 0,
			o = 0;

		char	*p_s = pCOD+w.x,
				*p_d;

		
		while( cd < n_w )
		{
			p_d = pCOD + p_w[cd].x;
			e = p_w[cd].y;
			if( e > w.y )
				e = w.y;

			for( i = 0; i < e; i++ )
			{
				if( p_s[i] == p_d[i] )
					continue;
				break;
			}
			if( e == i )
			if( w.y == p_w[cd].y )
				return n_w;

			if( i >= p_w[cd].y )
			{
				// ez nagyobb
				if( !p_w[cd].z )
				{
					m = cd;
					p_w[cd].z = n_w;
					return n_w+1;
				}

				cd = p_w[cd].z;
				continue;
			}
			else if( i == w.y )
			{
				// feltolja ezt a szót mert w.n az elején van
				//w.m = p_w[code].m;
				w.z = cd;
				//p_w[code].m = n_w;

				if( p_w[m].z == cd )
					p_w[m].z = n_w;
				else 
					p_w[m].w = n_w;

				return n_w+1; 
			}

			if( p_d[i] < p_s[i] )
			{
				if( !p_w[cd].w )
				{
					m = cd;
					p_w[cd].w = n_w;
					return n_w+1;
				}

				m = cd;
				cd = p_w[cd].w;
				continue;
			} 

			if( p_d[i] > p_s[i] )
			if( !p_w[cd].z )
			{
				m = cd;
				p_w[cd].z = n_w;
				return n_w+1;
			}

			m = cd;
			cd = p_w[cd].z;
		}

		return n_w;
	}
	U4 code_find( char* pCOD, U44& w, bool b_mx = false )
	{
		U44* p_w = this;
		U4	n_w = &w-p_w;
		if( !n_w )
			return 0;

		U4	cd = 0, e, i, m = 0, mx = 0, out = n_w;
		char	*p_s = pCOD+w.x,
				*p_d;

		while( cd < n_w )
		{
			p_d = pCOD+p_w[cd].x;
			e = p_w[cd].y;
			if( e > w.y )
				e = w.y;
			for( i = 0; i < e; i++ )
			{
				if( p_s[i] == p_d[i] )
					continue;
				break;
			}
			if( i == e )
			{
				if( p_w[cd].y == w.y )
					return cd;
				if( w.y == i ? p_w[cd].y > w.y : false )
					return out;
				if( !p_w[cd].z )
					return cd;
			}

			if( i >= p_w[cd].y )
			{
				if( i == p_w[cd].y )
				if( mx < i )
				{
					if( b_mx )
						out = cd;
					mx = i;
				}
				// ez nagyobb
				if( !p_w[cd].z )
					return out;

				cd = p_w[cd].z;
				continue;
			}

			if( p_d[i] < p_s[i] )
			{
				if( !p_w[cd].w )
					return out;

				cd = p_w[cd].w;
				continue;
			} 

			if( p_d[i] > p_s[i] )
			if( !p_w[cd].z )
				return out;

			cd = p_w[cd].z;
		}
		return out;
	}
};

// int 4 -------------------------------------------------------

class I44
{
public:
	I4 x,y,z,w;
	I44( void ){};
	I44( I4 i )
	{
		x =
		y = 
		z = 
		w = i;
	}
	I44( const I84& i );
	I44( const double4& d4 );
	
	I44& i2xyz( U8 i, U2 r )
	{
		x = i%r;
		y = i/r;
		z = y/r;
		y %= r;
		return *this;
	}
	I8 xyz2i( const I44& b ) const
	{
		return x + y*b.x + z*b.area_xy();
	}
	I44& operator = ( int i )
	{
		if( !i )
			return null();
		x =
		y = 
		z = 
		w = i;
		return *this;
	}
	I44& operator = ( const U44& u44 )
	{
		gp_memcpy( this, &u44, sizeof(*this) );
		return *this;
	}

	I44 operator / ( int i ) const
	{
		if( i == 1 )
			return *this;
		if( !i )
		{
			return 0x7fffffff;
		}
		
		return I44( x/i, y/i, z/i, w/i );
	}

	I44( I4 _x, I4 _y, I4 _z = 0, I4 _w = 0 )
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}
	I44( char* p_str,  char** pp_str )
	{
		str2i44( p_str, pp_str );
	}
	I44& str2i44( char* p_str,  char** pp_str )
	{
		p_str += GPD_NINCS( p_str, " \t\r\n,." );
		x = strtol( p_str, &p_str, 10 );
		p_str += GPD_NINCS( p_str, " \t\r\n,." );
		y = strtol( p_str, &p_str, 10 );
		p_str += GPD_NINCS( p_str, " \t\r\n,." );
		z = strtol( p_str, &p_str, 10 );
		p_str += GPD_NINCS( p_str, " \t\r\n,." );
		w = strtol( p_str, &p_str, 10 );
		if( pp_str )
			*pp_str = p_str;
		return *this;
	}
	char* i2str(char* p_buff, const char* p_ascii )
	{
		sprintf(p_buff, "%c%d%c %c%d%c %c%d%c %c%d%c",	p_ascii[1], x, p_ascii[0],
														p_ascii[2], y, p_ascii[0],
														p_ascii[3], z, p_ascii[0],
														p_ascii[1], w, p_ascii[0] );
		return p_buff;
	}
	I44& operator *= ( I4 b)
	{
		x *= b;
		y *= b;
		z *= b;
		w *= b;
		return *this;
	}

	I44& operator <<= ( const U14& b )
	{
		x <<= b.B;
		y <<= b.G;
		z <<= b.R;
		w <<= b.A;
		return *this;
	}
	I44& operator <<= ( U1 b )
	{
		x <<= b;
		y <<= b;
		z <<= b;
		w <<= b;
		return *this;
	}

	I44& operator &= ( const U14& b )
	{
		x *= b.B;
		y *= b.G;
		z *= b.R;
		w *= b.A;
		return *this;
	}
	I44& operator &= ( const I44& b )
	{
		x *= b.x;
		y *= b.y;
		z *= b.z;
		w *= b.w;
		return *this;
	}
	I44& operator /= (const I44& b);
	/*{
		x /= b.x;
		y /= b.y;
		z /= b.z;
		w /= b.w;
		return *this;
	}*/
	I44& operator += ( const I44& b )
	{
		x += b.x;
		y += b.y;
		z += b.z;
		w += b.w;
		return *this;
	}
	I44& operator -= ( const U44& b )
	{
		x -= b.x;
		y -= b.y;
		z -= b.z;
		w -= b.w;
		return *this;
	}
	I44 operator & ( const U14& b ) const
	{
		I44 out = *this;
		out &= b;
		return out;
	}
	I44 operator / (const I44& b) const;
	/*{
		double4 a = *this, bb = b;
		a /= b;
		I44 out = *this;
		out /= b;
		return out;
	}*/
	I44 operator + ( const I44& b ) const
	{
		I44 out = *this;
		out += b;
		return out;
	}
	I44 operator - ( const I44& b ) const
	{
		I44 out = *this;
		out -= b;
		return out;
	}

	I44 operator + (float3 b) const;
	

	I84 operator * ( I4 b ) const;
	I84 operator & ( const I44& b ) const;
	I8	operator * ( const I44& b ) const;
	I44 operator + ( const U44& b ) const;
	I44	operator - ( const U44& b ) const;

	bool operator != ( const I44& b ) const
	{
		if( x != b.x )
			return true;
		if( y != b.y )
			return true;
		if( z != b.z )
			return true;
		if( w != b.w )
			return true;
		return false;
	}
	bool operator == ( const I44& b ) const
	{
		return !(*this != b);
	}
	I44 abs( void ) const
	{
		return I44( x>0?x:-x, y>0?y:-y, z>0?z:-z, w>0?w:-w );
	}
	I44& null( void )
	{
		GPM_CLR;
		return *this;
	}
	I44 zero2inf( void ) const;

	I44 repl(I8 a, I8 b) const
	{
		if(a < 0)
			a *= -1;
		I44 out = abs(),
			n = norm();
		if(out.x == a)
			out.x = b;
		if(out.y == a)
			out.y = b;
		if(out.z == a)
			out.z = b;
		if(out.w == a)
			out.w = b;

		return out &= n;
	}

	/*{
		double4 d4( x ? s:)
		return ( x == 0.0 ? ) 
		if( x == 0.0 )
			x = 1.0/double(x);
		if( y == 0.0 )
			y = 1.0/double(y);
		if( z == 0.0 )
			z = 1.0/double(z);
		if( w == 0.0 )
			w = 1.0/double(w);
		return *this;
	}*/
	I44 xy( void ) const
	{
		return I44( x, y );
	}
	I44 __xy(void) const
	{
		return I44( 0, 0, x, y);
	}
	I44& xyz( const I44 &b )
	{
		x = b.x;
		y = b.y;
		z = b.z;
		return *this;
	}
	I44 xyz(void) const
	{
		return I44(x, y, z);
	}
	I44 xzy( void ) const
	{
		return I44( x, z, y );
	}
	I44& xy( I44 xy )
	{
		gp_memcpy( &x, &xy, 2*sizeof(x) );
		return *this;
	}
	I44 zw( void ) const
	{
		return I44( z, w );
	}
	I44& xyzw( void ) // const
	{
		return *this;
	}
	I44& zw( I44 xy )
	{
		gp_memcpy( &z, &xy, 2*sizeof(x) );
		return *this;
	}
	
	double qlen( void ) const
	{
		__declspec(align(16)) double aD[4];
		aD[0] = x;
		aD[1] = y;
		aD[2] = z;
		aD[3] = w;

		return gp_d4_qlen( aD, aD );
	}
	double qlen_xyz( void ) const
	{
		__declspec(align(16)) double aD[4];
		aD[0] = x;
		aD[1] = y;
		aD[2] = z;
		aD[3] = 0.0;
		//gp_d4_qlen( aD, aD );

		return gp_d4_qlen( aD, aD );
	}
	double qlen_xy( void ) const
	{
		__declspec(align(16)) double aD[4];
		aD[0] = x;
		aD[1] = y;
		aD[2] = 0.0;
		aD[3] = 0.0;
		//gp_d4_qlen( aD, aD );

		return gp_d4_qlen( aD, aD );
	}
	float3 f3(void) const;
	
	I8 sum( void ) const
	{
		return x+y+z+w;
	}

	I8 sum_xyz( void ) const
	{
		return x+y+z;
	}


	I8 sum_xy( void ) const
	{
		return x+y;
	}
	I8 sum_yz( void ) const
	{
		return y+z;
	}
	I8 sum_zw( void ) const
	{
		return z+w;
	}


	I8 sum_xz( void ) const
	{
		return x+z;
	}
	I8 sum_yw( void ) const
	{
		return y+w;
	}

	
	

	I8 area_xy( void ) const
	{
		I8 arr = x;
		return arr*y;
	}
	I8 vol_xyz( void ) const
	{
		I8 vol = x;
		return vol*y*z;
	}

	U4 add( char* p_src, U2& m, I44& w )
	{
		I44* p_w = this;
		U2	n_w = &w-p_w;
		if( !n_w )
			return 1;

		U4	cd = 0, e, i, 
			mx = 0,
			o = 0;

		char	*p_s = p_src+w.x,
				*p_d;

		m = 0;
		while( cd < n_w )
		{
			p_d = p_src + p_w[cd].x;
			e = p_w[cd].y;
			if( e > w.y )
				e = w.y;

			for( i = 0; i < e; i++ )
			{
				if( p_s[i] == p_d[i] )
					continue;
				break;
			}
			if( e == i )
			if( w.y == p_w[cd].y )
				return n_w;

			if( i >= p_w[cd].y )
			{
				// ez nagyobb
				if( !p_w[cd].z )
				{
					m = cd;
					p_w[cd].z = n_w;
					return n_w+1;
				}

				cd = p_w[cd].z;
				continue;
			}
			else if( i == w.y )
			{
				// feltolja ezt a szót mert w.n az elején van
				//w.m = p_w[code].m;
				w.z = cd;
				//p_w[code].m = n_w;

				if( p_w[m].z == cd )
					p_w[m].z = n_w;
				else 
					p_w[m].w = n_w;

				return n_w+1; 
			}

			if( p_d[i] < p_s[i] )
			{
				if( !p_w[cd].w )
				{
					m = cd;
					p_w[cd].w = n_w;
					return n_w+1;
				}

				m = cd;
				cd = p_w[cd].w;
				continue;
			} 

			if( p_d[i] > p_s[i] )
			if( !p_w[cd].z )
			{
				m = cd;
				p_w[cd].z = n_w;
				return n_w+1;
			}

			m = cd;
			cd = p_w[cd].z;
		}

		return n_w;
	}
	U4 find( char* p_src, I44& w )
	{
		I44* p_w = this;
		U4	n_w = &w-p_w;
		if( !n_w )
			return 0;

		U4	cd = 0, e, i, m = 0, mx = 0, o = n_w;
		char	*p_s = p_src+w.x,
				*p_d;

		while( cd < n_w )
		{
			p_d = p_src+p_w[cd].x;
			e = p_w[cd].y;
			if( e > w.y )
				e = w.y;
			for( i = 0; i < e; i++ )
			{
				if( p_s[i] == p_d[i] )
					continue;
				break;
			}
			if( i == e )
			{
				if( p_w[cd].y == w.y )
					return cd;
				if( w.y == i ? p_w[cd].y > w.y : false )
					return o;
				if( !p_w[cd].z )
					return cd;
			}

			if( i >= p_w[cd].y )
			{
				if( i == p_w[cd].y )
				if( mx < i )
				{
					o = cd;
					mx = i;
				}
				// ez nagyobb
				if( !p_w[cd].z )
					return o;

				cd = p_w[cd].z;
				continue;
			}

			if( p_d[i] < p_s[i] )
			{
				if( !p_w[cd].w )
					return o;

				cd = p_w[cd].w;
				continue;
			} 

			if( p_d[i] > p_s[i] )
			if( !p_w[cd].z )
				return o;

			cd = p_w[cd].z;
		}
		return o;
	}
	I44& swap_xz( void )
	{
		I4 t = x;
		x = z;
		z = t;
		return *this;
	}
	I44& swap_yw( void )
	{
		I4 t = y;
		y = w;
		w = t;
		return *this;
	}
	I44& swap_xy( void )
	{
		I4 t = x;
		x = y;
		y = t;
		return *this;
	}
	I44& swap_zw( void )
	{
		I4 t = z;
		z = w;
		w = t;
		return *this;
	}
	I44& swap_yz( void )
	{
		I4 t = y;
		y = z;
		z = t;
		return *this;
	}
	I44& swap_xw( void )
	{
		I4 t = x;
		x = w;
		w = t;
		return *this;
	}
	I44& swap_xyzw( U1 swap )
	{
		// BR,GA,BG,RA,GR,BA

		if( swap&1 )
			swap_xz();
		if( swap&2 )
			swap_yw();
		if( swap&4 )
			swap_xy();
		if( swap&8 )
			swap_zw();
		if( swap&0x10 )
			swap_yz();
		if( swap&0x20 )
			swap_xw();
		return *this;
	}
	U1 lohi_swap( void )
	{
		U1 o = 0;
		U14 idx = (U4)0x03020100; //( 2,1,0,3 );
		
		U1	*p_4 = (U1*)this;
		// BR,GA,BG,RA,GR,BA
		if( p_4[idx.B] > p_4[idx.R] )
		{
			o |= 1;
			idx.swap_BR();
		}
		if( p_4[idx.G] > p_4[idx.A] )
		{
			o |= 2;
			idx.swap_GA();
		}

		if( p_4[idx.B] > p_4[idx.G] )
		{
			o |= 4;
			idx.swap_BG();
		}
		if( p_4[idx.R] > p_4[idx.A] )
		{
			o |= 8;
			idx.swap_RA();
		}

		if( p_4[idx.G] > p_4[idx.R] )
		{
			o |= 16;
			idx.swap_GR();
		}
		if( p_4[idx.B] > p_4[idx.A] )
		{
			o |= 32;
			idx.swap_BA();
		}
		return o;
	}
	U1 hilo( void )
	{
		U1 o = 0;
		U14	idx = (U4)0x03020100; //( 2,1,0,3 );
		
		U1	*p_4 = (U1*)this;
		// BR,GA,BG,RA,GR,BA
		if( p_4[idx.B] < p_4[idx.R] )
		{
			o |= 1;
			idx.swap_BR();
		}
		if( p_4[idx.G] < p_4[idx.A] )
		{
			o |= 2;
			idx.swap_GA();
		}

		if( p_4[idx.B] < p_4[idx.G] )
		{
			o |= 4;
			idx.swap_BG();
		}
		if( p_4[idx.R] < p_4[idx.A] )
		{
			o |= 8;
			idx.swap_RA();
		}

		if( p_4[idx.G] < p_4[idx.R] )
		{
			o |= 16;
			idx.swap_GR();
		}
		if( p_4[idx.B] < p_4[idx.A] )
		{
			o |= 32;
			idx.swap_BA();
		}
		return o;
	}
	/*I44 abs( void ) const
	{
		return I44( abs(x), abs(y), abs(z), abs(w) );
	}*/
	I44 mn( I4 n ) const
	{
		return I44( min(n,x), min(n,y), min(n,z), min(n,w) );
	}
	I44 mx( I4 n ) const
	{
		return I44( max(n,x), max(n,y), max(n,z), max(n,w) );
	}
	I4 mn( void ) const
	{
		return min(x, min( y, min(z,w)));
	}
	I4 mx( void ) const
	{
		return max(x, max( y, max(z,w)));
	}
	I4 mn_xyz( void ) const
	{
		return min(x, min( y,z));
	}
	I4 mn_xy( void ) const
	{
		return min(x, y);
	}
	I4 mx_xyz( void ) const
	{
		return max(x, max( y, z));
	}
	I44 sign(void) const
	{
		return I44(x >= 0 ? 1 : -1, y >= 0 ? 1 : -1, z >= 0 ? 1 : -1, w >= 0 ? 1 : -1);
	}
	I44 norm( void ) const;
	I44 right_xy(void) const
	{
		return I44( y,-x,z,w );
	}
	I44 right_xz(void) const
	{
		return I44( z, y, -x, w );
	}
	I44 step( const I44& iD, const I44& iP, I4& i, U4 n ) const;
	gpeMAP qSPACE( U14* pBGRA, U1* pMSK, const I44& mul, U1 msk = 0x40 );
	gpeMAP qSPACEstp( I44 iGD, U14* pBGRA, U1* pMSK, const I44& mul, U1 msk = 0x40);
};
// unsigned int 8 -------------------------------------------------------
class U22
{
public:
	U2 x,y;
	
	U22(){};
	U22( U2 _x, U2 _y )
	{
		x = _x;
		y = _y;
	}
	U22( char* p_str,  char** pp_str )
	{
		str2u82( p_str, pp_str );
	}
	U22& str2u82( char* p_str,  char** pp_str )
	{
		p_str += GPD_NINCS( p_str, " \t\r\n,." );
		x = strtoull( p_str, &p_str, 10 );
		p_str += GPD_NINCS( p_str, " \t\r\n,." );
		y = strtoull( p_str, &p_str, 10 );
		if( pp_str )
			*pp_str = p_str;
		return *this;
	}

	bool operator != ( const U22& b ) const
	{
		if( x != b.x )
			return true;
		if( y != b.y )
			return true;
		return false;
	}
	bool operator == ( const U22& b ) const
	{
		return !(*this != b);
	}
	
	U22& null( void )
	{
		GPM_CLR;
		return *this;
	}

	U4 sum( void ) const
	{
		return x+y;
	}

	U4 area_xy( void )
	{
		return x*y;
	}
	
	U22* index( U2 n_i )
	{
		if( !this )
		{
			U22* p_this = new U22[n_i+1];
			if( !p_this )
				return NULL;

			return p_this->index( n_i );
		}

		for( U2 i = 0; i < n_i; i++ )
		{
			this[i].x = i;
			this[i].y = 0;
		}
		return this;
	}
	U2 histi( U1* p_src, U2 n_src )
	{
		for( U2 i = 0; i < n_src; i++ )
			this[p_src[i]].y++;

		U2 mx = 0x101;
		for( U2 i = 0; i < 0x100; i++ )
		{
			if( !this[i].y )
				continue;
			mx = i;
		}
		return mx+1;
	}
	void histi16( U1* p_src, U2 n_src )
	{
		for( U2 i = 0; i < n_src; i++ )
		{
			this[p_src[i]&0xf].y++;
			this[p_src[i]>>4].y++;
		}
	}

	U2 median( U2 n, bool b_inc = false )
	{
		// b_inc == true - incrementált növekvõ sorban leszenk
		// b_inc == false - dekrementáslt csökkenõ sorban leszenk (nem definiálod akkor ez, azaz csökenõ )
		if( !this || n < 1 )
			return 0;
	
		if( n < 2 )
			return this->y;
		
		U2	i, j, l, r;
		U22	*p_tree = new U22[n+1],
			x;
	
		r = n;
		while( r >= 1 )
		{
			// az öszes elem számát "r" elosztom 2-tõvel kerekítés nélkül
			// ezzel tudom, hogy a soron következõ szint meddig csökkenhet "l" lesz a küszöb
			l = ldiv( r, 2 ).quot;
			while( r > 0 )
			{
				// a következõ elemet berakom az x-be
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
							// és következõ ciklusban az ágról akarom folytatni
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

				// r-et csökkentem jöhet a következõ elem
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
			for( i = 0; i < n; i++ )
			{
				this[i] = p_tree[n-i];
			}
			GPM_DELARRAY( p_tree );
			if( n < 3 )
				return avarage( n );

			return this[n/2].y;
		}

		U22* p_src = p_tree+1;
		for( i = 0; i < n; i++ )
		{
			this[i] = p_src[i];
		}
		GPM_DELARRAY( p_tree );
		if( n < 3 )
			return avarage( n );

		return this[n/2].y;
	}
	U2 avarage( U2 n )
	{
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

		U4 avgr = 0;
		for( U2 j = 0; j < n; j++ ) 
			avgr += this[j].y;

		return avgr / n;
	}

};

class U42
{
public:
	U4 x,y;
	
	U42(){};
	U42( U4 _x, U4 _y )
	{
		x = _x;
		y = _y;
	}
	U42( char* p_str,  char** pp_str )
	{
		str2u82( p_str, pp_str );
	}
	U42& str2u82( char* p_str,  char** pp_str )
	{
		p_str += GPD_NINCS( p_str, " \t\r\n,." );
		x = strtoull( p_str, &p_str, 10 );
		p_str += GPD_NINCS( p_str, " \t\r\n,." );
		y = strtoull( p_str, &p_str, 10 );
		if( pp_str )
			*pp_str = p_str;
		return *this;
	}

	bool operator != ( const U42& b ) const
	{
		if( x != b.x )
			return true;
		if( y != b.y )
			return true;
		return false;
	}
	bool operator == ( const U42& b ) const
	{
		return !(*this != b);
	}
	
	U42& null( void )
	{
		GPM_CLR;
		return *this;
	}

	U8 sum( void ) const
	{
		return x+y;
	}

	U8 area_xy( void )
	{
		return x*y;
	}
	
	U42* index( U4 n_i )
	{
		if( !this )
		{
			U42* p_this = new U42[n_i+1];
			if( !p_this )
				return NULL;

			return p_this->index( n_i );
		}

		for( U4 i = 0; i < n_i; i++ )
		{
			this[i].x = i;
			this[i].y = 0;
		}
		return this;
	}
	U4 histi( U1* p_src, U4 n_src )
	{
		for( U4 i = 0; i < n_src; i++ )
			this[p_src[i]].y++;

		U2 mx = 0x101;
		for( U2 i = 0; i < 0x100; i++ )
		{
			if( !this[i].y )
				continue;
			mx = i;
		}
		return mx+1;
	}
	

	U4 median( U4 n, bool b_inc = false )
	{
		// b_inc == true - incrementált növekvõ sorban leszenk
		// b_inc == false - dekrementáslt csökkenõ sorban leszenk (nem definiálod akkor ez, azaz csökenõ )
		if( !this || n < 1 )
			return 0;
	
		if( n < 2 )
			return this->y;
		
		U4	i, j, l, r;
		U42	*p_tree = new U42[n+1],
			x;
	
		r = n;
		while( r >= 1 )
		{
			// az öszes elem számát "r" elosztom 2-tõvel kerekítés nélkül
			// ezzel tudom, hogy a soron következõ szint meddig csökkenhet "l" lesz a küszöb
			l = ldiv( r, 2 ).quot;
			while( r > 0 )
			{
				// a következõ elemet berakom az x-be
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
							// és következõ ciklusban az ágról akarom folytatni
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

				// r-et csökkentem jöhet a következõ elem
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
			GPM_DELARRAY( p_tree );
			if( n < 3 )
				return avarage( n );

			return this[n/2].y;
		}

		U42* p_src = p_tree+1;
		for( U4 i = 0; i < n; i++ )
		{
			this[i] = p_src[i];
		}
		GPM_DELARRAY( p_tree );
		if( n < 3 )
			return avarage( n );

		return this[n/2].y;
	}
	U4 avarage( U4 n )
	{
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

		U8 avgr = 0;
		for( U4 j = 0; j < n; j++ ) 
			avgr += this[j].y;

		return avgr / n;
	}

};

class U82
{
public:
	U8 x,y;
	
	U82(){};
	U82( U8 _x, U8 _y )
	{
		x = _x;
		y = _y;
	}
	U82( char* p_str,  char** pp_str )
	{
		str2u82( p_str, pp_str );
	}
	U82& str2u82( char* p_str,  char** pp_str )
	{
		p_str += GPD_NINCS( p_str, " \t\r\n,." );
		x = strtoull( p_str, &p_str, 10 );
		p_str += GPD_NINCS( p_str, " \t\r\n,." );
		y = strtoull( p_str, &p_str, 10 );
		if( pp_str )
			*pp_str = p_str;
		return *this;
	}

	bool operator != ( const U82& b ) const
	{
		if( x != b.x )
			return true;
		if( y != b.y )
			return true;
		return false;
	}
	bool operator == ( const U82& b ) const
	{
		return !(*this != b);
	}
	
	U82& null( void )
	{
		GPM_CLR;
		return *this;
	}

	U8 sum( void ) const
	{
		return x+y;
	}

	U8 area_xy( void )
	{
		return x*y;
	}
	
	U82* index( U8 n_i )
	{
		if( !this )
		{
			U82* p_this = new U82[n_i+1];
			if( !p_this )
				return NULL;

			return p_this->index( n_i );
		}

		for( U8 i = 0; i < n_i; i++ )
		{
			this[i].x = i;
			this[i].y = 0;
		}
		return this;
	}
	U8 histi( U1* p_src, U8 n_src )
	{
		for( U2 i = 0; i < n_src; i++ )
			this[p_src[i]].y++;

		U2 mx = 0x101;
		for( U2 i = 0; i < 0x100; i++ )
		{
			if( !this[i].y )
				continue;
			mx = i;
		}
		return mx+1;
	}
	void histi16( U1* p_src, U8 n_src )
	{
		for( U2 i = 0; i < n_src; i++ )
		{
			this[p_src[i]&0xf].y++;
			this[p_src[i]>>4].y++;
		}
	}

	U8 median( U8 n, bool b_inc = false )
	{
		// b_inc == true - incrementált növekvõ sorban leszenk
		// b_inc == false - dekrementáslt csökkenõ sorban leszenk (nem definiálod akkor ez, azaz csökenõ )
		if( !this || n < 1 )
			return 0;
	
		if( n < 2 )
			return this->y;
		
		U8	i, j, l, r;
		U82	*p_tree = new U82[n+1],
			x;
	
		r = n;
		while( r >= 1 )
		{
			// az öszes elem számát "r" elosztom 2-tõvel kerekítés nélkül
			// ezzel tudom, hogy a soron következõ szint meddig csökkenhet "l" lesz a küszöb
			l = ldiv( r, 2 ).quot;
			while( r > 0 )
			{
				// a következõ elemet berakom az x-be
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
							// és következõ ciklusban az ágról akarom folytatni
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

				// r-et csökkentem jöhet a következõ elem
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
			for( U2 i = 0; i < n; i++ )
			{
				this[i] = p_tree[n-i];
			}
			GPM_DELARRAY( p_tree );
			if( n < 3 )
				return avarage( n );

			return this[n/2].y;
		}

		U82* p_src = p_tree+1;
		for( I8 i = 0; i < n; i++ )
		{
			this[i] = p_src[i];
		}
		GPM_DELARRAY( p_tree );
		if( n < 3 )
			return avarage( n );

		return this[n/2].y;
	}
	U2 avarage( U2 n )
	{
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

		U4 avgr = 0;
		for( U2 j = 0; j < n; j++ ) 
			avgr += this[j].y;

		return avgr / n;
	}

};

class U84
{
public:
	U8 x,y,z,w;

	U84(){};
	U84( U8 _x, U8 _y, U8 _z = 0, U8 _w = 0 )
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}
	U14 sort(void)
	{
		U14 srt(0, 1, 2, 3);
		U8 *pU = (U8*)this;
		if(pU[srt.B] > pU[srt.R])
			srt.swap_BR();
		if(pU[srt.G] > pU[srt.A])
			srt.swap_GA();

		if(pU[srt.B] > pU[srt.G])
			srt.swap_BG();
		if(pU[srt.R] > pU[srt.A])
			srt.swap_RA();
		
		if(pU[srt.B] > pU[srt.A])
			srt.swap_BA();
		if(pU[srt.G] > pU[srt.R])
			srt.swap_GR();

		return srt;
	}
	U84( const double4& d4 );

	U84( char* p_str,  char** pp_str )
	{
		str2u84( p_str, pp_str );
	}
	U84& str2u84( char* p_str,  char** pp_str )
	{
		p_str += GPD_NINCS( p_str, " \t\r\n,." );
		x = strtoull( p_str, &p_str, 10 );
		p_str += GPD_NINCS( p_str, " \t\r\n,." );
		y = strtoull( p_str, &p_str, 10 );
		p_str += GPD_NINCS( p_str, " \t\r\n,." );
		z = strtoull( p_str, &p_str, 10 );
		p_str += GPD_NINCS( p_str, " \t\r\n,." );
		w = strtoull( p_str, &p_str, 10 );
		if( pp_str )
			*pp_str = p_str;
		return *this;
	}

	U82 xy( void )
	{
		return U82(x,y);
	}
	U82* p_xy( void )
	{
		return (U82*)this;
	}
	U84& operator = (const U8& u )
	{
		x = y = z = w = u;
		return *this;
	}
	U84& operator = ( const U82& xy )
	{
		x = xy.x;
		y = xy.y;
		return *this;
	}
	U84& operator = (const float4& b);
	/*{
		x = b.x;
		y = b.y;
		z = b.z;
		w = b.w;
		return *this;
	}*/
	bool operator != ( const U84& b ) const
	{
		if( x != b.x )
			return true;
		if( y != b.y )
			return true;
		if( z != b.z )
			return true;
		if( w != b.w )
			return true;
		return false;
	}
	bool operator == ( const U84& b ) const
	{
		return !(*this != b);
	}
	
	U84& null( void )
	{
		GPM_CLR;
		return *this;
	}

	U8 sum( void ) const
	{
		return x+y+z+w;
	}

	U8 sum_xyz( void ) const
	{
		return x+y+z;
	}


	U8 sum_xy( void ) const
	{
		return x+y;
	}
	U8 sum_yz( void ) const
	{
		return y+z;
	}
	U8 sum_zw( void ) const
	{
		return z+w;
	}


	U8 sum_xz( void ) const
	{
		return x+z;
	}
	U8 sum_yw( void ) const
	{
		return y+w;
	}

	
	

	U8 area_xy( void )
	{
		U8 arr = x;
		return arr*y;
	}
	U8 vol_xyz( void )
	{
		U8 vol = x;
		return vol*y*z;
	}

	U84* find( U8 iX, U8 n )
	{
		U8 i = 0;
		if( !n )
			return NULL;

		while( i < n )
		{
			if( this[i].x == iX )
				return this+i;

			if( this[i].x > iX )
			{
				if( this[i].z )
				{
					i = this[i].z;
					continue;
				}
				
				return NULL;
			}

			if( this[i].w )
			{
				i = this[i].w;
				continue;
			}

			return NULL;
		}

		return NULL;
	}
	U8 add( U84* p_ix, U8& i, U8 iY )
	{
		i = 0;
		
		U8	n = p_ix - this;
		if( !n )
		{
			y = iY;
			z = w = 0;
			return n+1;
		}

		while( i < n )
		{
			if( this[i].x == p_ix->x )
				return n;

			if( this[i].x > p_ix->x )
			{
				if( this[i].z )
				{
					i = this[i].z;
					continue;
				}
				
				this[i].z = n;
				p_ix->y = iY;
				i = n;
				return n+1;
			}

			if( this[i].w )
			{
				i = this[i].w;
				continue;
			}

			this[i].w = n;
			p_ix->y = iY;
			i = n;
			return n+1;
		}

		return n;
	}

	U8 tree_add( U8 u4, U8 n_t )
	{
		// x érték, y mama, z kicsiegyenlõ, w nagyobb
		this[n_t].null().x = u4;
		if( !n_t )
			return 1;

		U8 i = 0;
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
	U8 tree_fnd( U8 u4, U8 n_t )
	{
		if( !this )
			return n_t;
		
		U8 i = 0;
		while( i < n_t )
		{
			if( this[i].x == u4 )
				return i;

			if( this[i].x < u4 )
			{
				if( !this[i].z )
					return n_t;
				
				i = this[i].z;
				continue;
			}
			if( !this[i].w )
				return n_t;
			
			i = this[i].w;
		}
		return n_t;
	}
	
	U84 operator * ( U8 b ) const;
	U84 operator & ( const U84& b ) const;
	U84 operator / ( const U84& b ) const;
	U8 operator * (const U44& b) const;
	U8 operator * ( const U84& b ) const;
	U84 operator + ( const U84& b ) const;
	U84 operator - ( const U84& b ) const;
};
// int 8 -------------------------------------------------------

__declspec(align(16)) class I84
{
public:
	union
	{
		struct {
			I8 x,y,z,w;
		};
		struct
		{
			U4 Qx, GRy, GRz, GRsid, a_Q4[4];
		};
		struct
		{
			I8	itm_id, mom_id;
			U4	POSq, TRGq, 
				snd_cnt;
			gpeACT	act;
			U1	mxi, drh, drv;
		};
		struct // BUSSINES
		{
			// I8 1,2, 
			I8		src_id, dst_id;
			// I8 3,
			U4		DRPq, MINq,
					gx_s4;	// 4 másodperc			
			// I8 4
			gpeACT	act;
			U1		ixIL, ixIR, n_op; // OPcode 1-one 2-half 3-all
		};
	};

	I84 zero2inf(void) const;;
	char* i2str(char* p_buff, const char* p_ascii)
	{
		sprintf(p_buff, "%c%lld%c %c%lld%c %c%lld%c %c%lld%c", p_ascii[1], x, p_ascii[0],
			p_ascii[2], y, p_ascii[0],
			p_ascii[3], z, p_ascii[0],
			p_ascii[1], w, p_ascii[0]);
		return p_buff;
	}

	I84( void ){};
	I84( I8 _x )
	{
		if( !_x )
		{
			GPM_CLR;
			return;
		}

		x = 
		y = 
		z = 
		w = _x;
	}
	I84( I8 _x, I8 _y, I8 _z = 0, I8 _w = 0 )
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}
	I84( const U44& u )
	{
		x = u.x;
		y = u.y;
		z = u.z;
		w = u.w;
	}
	I84( const U84& u )
	{
		x = u.x;
		y = u.y;
		z = u.z;
		w = u.w;
	}
	I84( const I44& i44 )
	{
		x = i44.x;
		y = i44.y;
		z = i44.z;
		w = i44.w;
	}

	I84( const float3 f3 );
	I84( const float4 f3);
	I84( const double4 d4 );

	I84( const float* pF )
	{
		x = pF[0];
		y = pF[1];
		z = pF[2];
		w = pF[3];
	}
	I84( const double* pD )
	{
		x = pD[0];
		y = pD[1];
		z = pD[2];
		w = pD[3];
	}
	
	I84( char* p_str,  char** pp_str )
	{
		str2i84( p_str, pp_str );
	}
	I84& str2i84( char* p_str,  char** pp_str )
	{
		p_str += GPD_NINCS( p_str, " \t\r\n,." );
		x = strtoll( p_str, &p_str, 10 );
		p_str += GPD_NINCS( p_str, " \t\r\n,." );
		y = strtoll( p_str, &p_str, 10 );
		p_str += GPD_NINCS( p_str, " \t\r\n,." );
		z = strtoll( p_str, &p_str, 10 );
		p_str += GPD_NINCS( p_str, " \t\r\n,." );
		w = strtoll( p_str, &p_str, 10 );
		if( pp_str )
			*pp_str = p_str;
		return *this;
	}
	
	I84 abs( void ) const
	{
		return I84( x>0?x:-x, y>0?y:-y, z>0?z:-z, w>0?w:-w );
	}

	I84 mn(I8 n) const
	{
		return I84(min(n, x), min(n, y), min(n, z), min(n, w));
	}
	I84 mx(I8 n) const
	{
		return I84(max(n, x), max(n, y), max(n, z), max(n, w));
	}
	I84 mn(void) const
	{
		return min(x, min(y, min(z, w)));
	}
	I84 mx(void) const
	{
		return max(x, max(y, max(z, w)));
	}
	I84 mn_xyz(void) const
	{
		return min(x, min(y, z));
	}
	I84 mn_xy(void) const
	{
		return min(x, y);
	}
	I84 mx_xyz(void) const
	{
		return max(x, max(y, z));
	}
	I84 sign(void) const
	{
		return I84(x >= 0 ? 1 : -1, y >= 0 ? 1 : -1, z >= 0 ? 1 : -1, w >= 0 ? 1 : -1);
	}
	I84 norm(void) const
	{
		I84 out = *this;
		return out /= abs().mx(1);
	}
	I84 repl(I8 a, I8 b) const
	{
		if( a < 0 )
			a *= -1;
		I84 out = abs(),
			n = sign();
		if(out.x == a)
			out.x = b;
		if(out.y == a)
			out.y = b;
		if(out.z == a)
			out.z = b;
		if(out.w == a)
			out.w = b;

		return out &= n;
	}
	I84 xyz( void ) const
	{
		return I84(x,y,z);
	}
	I84& xyz(  const I84& b  )
	{
		x = b.x;
		y = b.y;
		z = b.z;
		return *this;
	}
	float3 f3( void ) const;
	double4 d4xyz(void) const;
	I84& null( void )
	{
		GPM_CLR;
		return *this;
	}
	I84& null_xyz( void )
	{
		GPMZn( &x, 3 );
		return *this;
	}
	U4 gr2q( void )
	{
		register U4 q = w%6;
		q <<= gpeQbyz;
		q |= z%gpeQyz;
		q <<= gpeQbyz;
		q |= y%gpeQyz;
		q <<= gpeQbyz;
		q |= x%gpeQx;
		return q;
	}
	I84& q2gr( U4 q )
	{
		x = q&gpeQxMSK;
		q >>= gpeQbx;
		y = q&gpeQyzMSK;
		q >>= gpeQbyz;
		z = q&gpeQyzMSK;
		q >>= gpeQbyz;
		w = q;
		return *this;
	}
	I84( U4 q )
	{
		x = q&gpeQxMSK;
		q >>= gpeQbx;
		y = q&gpeQyzMSK;
		q >>= gpeQbyz;
		z = q&gpeQyzMSK;
		q >>= gpeQbyz;
		w = q;
	}
	I84 gr2cr_1m( void ) const
	{
		double	rz = (((double)(z>>4)) + 0.25)*PI,
				ry = (((double)(y>>4)) + 0.25)*PI,
				ctgZ = cos(rz)/sin(rz),
				ctgY = cos(ry)/sin(ry),
				ctgZY2 = ctgZ*ctgZ + ctgY*ctgY,
				rx = (double)gpeQx / sqrt(1.0+ctgZY2), dif; 
		I84 cr;

		ctgZ *= -rx;
		ctgY *= -rx;
		ctgZY2 = ctgZ*ctgZ + ctgY*ctgY;
		dif = rx;
		rx = sqrt( (double)gpeQcr1m2 - ctgZY2 );
		dif -= rx;

		switch(w)
		{
			case 0:
				cr.y = -rx;
				cr.x =  ctgY;
				cr.z =  ctgZ;
				break;
			case 1:
				cr.x =  rx;
				cr.y =  ctgY;
				cr.z =  ctgZ;
				break;
			case 2:
				cr.z =  rx;
				cr.y =  ctgY;
				cr.x = -ctgZ;
				break;
			case 3:
				cr.y =  rx;
				cr.z = -ctgY;
				cr.x = -ctgZ;
				break;
			case 4:
				cr.x = -rx;
				cr.z = -ctgY;
				cr.y = -ctgZ;
				break;
			case 5:
				cr.z = -rx;
				cr.x = ctgY;
				cr.y = -ctgZ;
				break;
			default:
				// ERROR
				cr.null_xyz();
		}
		cr.w = sqrt(cr.qlen_xyz());
		return cr;
	}
	I84 gr2ar( I8 mul, I8 div ) const
	{
		// w -ben a ar-vektor hossza lesz
		if( !div )
			div = 1;

		I84 ar;
		if( mul == 1 )
		{
			ar = gr2cr_1m();
		} else {
			I84 gr = *this;
			gr &= I84( mul, mul, mul, 1 );
			ar = gr.gr2cr_1m();
		}
		if( !x )
		{
			ar /= div;
			return ar;
		}
		
		//ar /= div;
		I8 c = ar.w;
		ar *= c + x*mul;
		ar /= c*div;

		#ifdef _DEBUG_GR2AR
			GPCI4 gr( ar, div, mul, true ),
				er = gr-*this;
			while( er.sum_abs() )
			{
				gr.ar2gr( ar, div, mul, true ).gr_valid();
				er = gr-*this;
			}
		#endif
		return ar;
	}
	I84& operator = (const I8 b)
	{
		x = y = z = w = b;
		return *this;
	}
	bool operator != ( const I84& b ) const
	{
		if( x != b.x )
			return true;
		if( y != b.y )
			return true;
		if( z != b.z )
			return true;
		if( w != b.w )
			return true;
		return false;
	}
	bool operator == ( const I84& b ) const
	{
		return !(*this != b);
	}
	


	I8 sum( void ) const
	{
		return x+y+z+w;
	}

	I8 sum_xyz( void ) const
	{
		return x+y+z;
	}
	

	I8 sum_xy( void ) const
	{
		return x+y;
	}
	I8 sum_yz( void ) const
	{
		return y+z;
	}
	I8 sum_zw( void ) const
	{
		return z+w;
	}


	I8 sum_xz( void ) const
	{
		return x+z;
	}
	I8 sum_yw( void ) const
	{
		return y+w;
	}

	
	

	I8 area_xy( void )
	{
		I8 arr = x;
		return arr*y;
	}
	I8 vol_xyz( void )
	{
		I8 vol = x;
		return vol*y*z;
	}
	double qlen( void ) const
	{
		__declspec(align(16)) double aD[4];
		aD[0] = x;
		aD[1] = y;
		aD[2] = z;
		aD[3] = w;

		return gp_d4_qlen( aD, aD );
	}
	double qlen_xyz( void ) const
	{
		__declspec(align(16)) double aD[4];
		aD[0] = x;
		aD[1] = y;
		aD[2] = z;
		aD[3] = 0.0;
		//gp_d4_qlen( aD, aD );

		return gp_d4_qlen( aD, aD );
	}
	I84& operator %= ( const I84& b )
	{
		if( b.x )
			x %= b.x;
		if( b.y )
			y %= b.y;
		if( b.z )
			z %= b.z;
		if( b.w )
			w %= b.w;
		return *this;
	}
	I84& operator *= ( double b );
	//{
	//	double4 a = *this;
	//	//__declspec(align(16)) double aA[4];
	//	__declspec(align(16)) double aB[4];
	//	/*aA[0] = x;
	//	aA[1] = y;
	//	aA[2] = z;
	//	aA[3] = w;*/

	//	aB[0] = aB[1] = aB[2] = aB[3] = b;
	//	gp_d4_mul( &a, aB );

	//	*this = a;
	//	x = aA[0];
	//	y = aA[1];
	//	z = aA[2];
	//	w = aA[3];
	//	return *this;
	//}
	I84& operator &= ( const I84& b )
	{
		__declspec(align(16)) double aA[4];
		__declspec(align(16)) double aB[4];
		aA[0] = x;
		aA[1] = y;
		aA[2] = z;
		aA[3] = w;

		aB[0] = b.x;
		aB[1] = b.y;
		aB[2] = b.z;
		aB[3] = b.w;

		gp_d4_mul( aA, aB );

		x = aA[0];
		y = aA[1];
		z = aA[2];
		w = aA[3];
		return *this;
	}
	I84& operator /= ( double b )
	{
		__declspec(align(16)) double aA[4];
		__declspec(align(16)) double aB[4];
		aA[0] = x;
		aA[1] = y;
		aA[2] = z;
		aA[3] = w;

		aB[0] = aB[1] = aB[2] = aB[3] = b;
		gp_d4_div( aA, aB );

		x = aA[0];
		y = aA[1];
		z = aA[2];
		w = aA[3];
		return *this;
	}
	I84& operator /= ( const I84& b )
	{
		__declspec(align(16)) double aA[4];
		__declspec(align(16)) double aB[4];
		aA[0] = x;
		aA[1] = y;
		aA[2] = z;
		aA[3] = w;

		aB[0] = b.x;
		aB[1] = b.y;
		aB[2] = b.z;
		aB[3] = b.w;

		gp_d4_div( aA, aB );

		x = aA[0];
		y = aA[1];
		z = aA[2];
		w = aA[3];
		return *this;
	}
	I84 operator & ( const I84& b ) const
	{
		__declspec(align(16)) double aA[4];
		__declspec(align(16)) double aB[4];
		aA[0] = x;
		aA[1] = y;
		aA[2] = z;
		aA[3] = w;

		aB[0] = b.x;
		aB[1] = b.y;
		aB[2] = b.z;
		aB[3] = b.w;

		gp_d4_mul( aA, aB );
		
		return I84( aA );
	}
	I84 operator * ( double b ) const
	{
		__declspec(align(16)) double aA[4];
		__declspec(align(16)) double aB[4];
		aA[0] = x;
		aA[1] = y;
		aA[2] = z;
		aA[3] = w;

		aB[0] = aB[1] = aB[2] = aB[3] = b;
		gp_d4_mul( aA, aB );

		return I84( aA[0], aA[1], aA[2], aA[3] );

	}
	I84 operator / ( double b ) const
	{
		__declspec(align(16)) double aA[4];
		__declspec(align(16)) double aB[4];
		aA[0] = x;
		aA[1] = y;
		aA[2] = z;
		aA[3] = w;

		aB[0] = aB[1] = aB[2] = aB[3] = b;
		gp_d4_div( aA, aB );

		return I84( aA[0], aA[1], aA[2], aA[3] );

	}
	
	I84& operator += ( const I84& b )
	{
		gp_i84_add( this, &b );
		return *this;
	}

	I84 operator + ( const I84& b ) const
	{
		I84 o = *this;
		gp_i84_add( &o, &b );
		return o;
	}
	I84 operator - ( const I84& b ) const
	{
		I84 o = *this;
		gp_i84_sub( &o, &b );
		return o;
	}

	float4 operator / ( const I84& b ) const;
	/*{
		__declspec(align(16)) double aA[4];
		__declspec(align(16)) double aB[4];
		aA[0] = x;
		aA[1] = y;
		aA[2] = z;
		aA[3] = w;

		aB[0] = b.x;
		aB[1] = b.y;
		aB[2] = b.z;
		aB[3] = b.w;

		gp_d4_div( aA, aB );
		
		return float4( aA );
	}*/
};

class GPCBGRA_CACHE
{
public:
	GPC_LAZY	*pBGRA,
		*pTREE;
	U8			nTREE;
	~GPCBGRA_CACHE()
	{
		GPM_DEL(pBGRA);
		GPM_DEL(pTREE);
	}
	GPCBGRA_CACHE(void)
	{
		GPM_CLR;
	}
	GPCBGRA_CACHE* add(U14* p_u14, U4 n_u14, U8 cd, U8& is, U8& n)
	{
		if(!n_u14)
			return this;

		if(!this)
		{
			GPCBGRA_CACHE* p_this = new GPCBGRA_CACHE;
			if(!p_this)
				return p_this;

			return p_this->add(p_u14, n_u14, cd, is, n);
		}

		is = nTREE;
		pTREE = pTREE->tree_add8(cd, nTREE);
		n = nTREE;
		if(is >= nTREE)
			return this;

		U84* pT = ((U84*)pTREE->p_alloc);
		U8 dbg = pT[is].y;
		pBGRA = pBGRA->lazy_add(p_u14, n_u14 * sizeof(*p_u14), pT[is].y = -1, 8);
		if(pT[is].y ? (pT[is].y != dbg) : true)
			return this;
		pBGRA = pBGRA->lazy_add(p_u14, n_u14 * sizeof(*p_u14), pT[is].y = -1, 8);
		return this;
	}

	U8 fnd(U8 cd, U8& nG)
	{
		nG = 0;
		if(!this)
			return 0;
		nG = nTREE;
		return ((U84*)pTREE->p_alloc)->tree_fnd(cd, nTREE);
	}
	U14* get_pBGRA(U8 i)
	{
		if(this ? (nTREE <= i) : true)
			return NULL;

		return (U14*)(pBGRA->p_alloc + (((U84*)pTREE->p_alloc)[i].y));
	}
	U8 get_nBGRA(U8 i)
	{
		if(this ? (nTREE <= i) : true)
			return NULL;
		U8 j = (i + 1 >= nTREE) ? pBGRA->n_load : ((U84*)pTREE->p_alloc)[i + 1].y;

		return (j - (((U84*)pTREE->p_alloc)[i].y)) / sizeof(U14);
	}
};
__declspec(align(16)) class GPC_PROTO
{
public:
	gpeMAP	aPROTO[32 * 32 * 32 * 6];
	U1		aPAT[32 * 32 * 32 * 6],
			aDTR[16 * 16 * 16],
			aMSK[0x100];
	U14		aBGRA[0x100];
	char	sROOT[GPD_MAX_PATH], *p_file;

	GPC_CELL		*p_cell;
	GPCBGRA_CACHE	*pGENc;
	//ID3D11Texture2D *p_2D_stage;
	GPC_PROTO()
	{
		GPM_CLR;
	};
	/*~GPC_PROTO()
	{
		GPM_RELEASE( p_2D_stage );
	};*/
};

// FLOAT 4 -------------------------------------------------------
class float2
{
public:
	float	x,y;
	float2( void ){};
	float2( float _x, float _y )
	{
		x = _x;
		y = _y;
	}

	bool operator != ( const float2& b ) const
	{
		if( x != b.x )
			return true;
		if( y != b.y )
			return true;
		return false;
	}
	bool operator == ( const float2& b ) const
	{
		return !(*this != b);
	}
	float	area_xy( void )
	{
		return x*y;
	}
	float2& operator += ( const float2& b )
	{
		x += b.x;
		y += b.y;
		return *this;
	}
	float2 operator + ( const float2& b ) const
	{
		return float2( x+b.x, y+b.y );
	}
	float2& operator -= ( const float2& b )
	{
		x -= b.x;
		y -= b.y;
		return *this;
	}
	float2 operator - ( const float2& b ) const
	{
		return float2( x-b.x, y-b.y );
	}
	float2& operator &= ( const float2& b )
	{
		x *= b.x;
		y *= b.y;
		return *this;
	}
	float2 operator & ( const float2& b ) const
	{
		return float2( x*b.x, y*b.y );
	}
	float2& operator /= ( const float2& b )
	{
		x /= b.x;
		y /= b.y;
		return *this;
	}
	float2 operator / ( const float2& b ) const
	{
		return float2( x/b.x, y/b.y );
	}

	float operator * ( const float2& b ) const
	{
		return x*b.x+y*b.y;
	}

	float3 cell_x2p( void );
	//{
	//	float3 f3;// = 0;
	//
	//	f3.y = x / GPE_3D_W16; 
	//	f3.x = GPE_3D_W16 * ( f3.y-floor(f3.y) );
	//	f3.y = floor( f3.y ) / GPE_3D_W16;
	//	f3.z = GPE_3D_W16 * ( f3.y-floor(f3.y) );
	//	f3.y = floor( f3.y );
	//
	//	return f3;	//3Dmap-nál .xzy;
	//}
	
	ID3D11Buffer* upl( ID3D11Device* p_dev, U4 n )
	{
		if( !n )
			return NULL;

		D3D11_BUFFER_DESC bDSC;
		D3D11_SUBRESOURCE_DATA iniDT;
		GPMZ( bDSC );
		bDSC.Usage = D3D11_USAGE_DEFAULT;				// D3D11_USAGE_STAGING;// D3D11_USAGE_IMMUTABLE; //D3D11_USAGE_DEFAULT;//    D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
		bDSC.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// use as a vertex buffer
		iniDT.SysMemPitch = 0;
		iniDT.SysMemSlicePitch = 0;			
		D3D11_MAPPED_SUBRESOURCE ms;

		bDSC.ByteWidth = n * sizeof(*this);       // size is the VERTEX struct * 3
		iniDT.pSysMem = this;
		ID3D11Buffer* p_out;
		HRESULT hr = p_dev->CreateBuffer(&bDSC, &iniDT, &p_out );
		return p_out;
	}
};

#define F2 float2



//extern "C" float3* gp_f43_add( float3* p_d, const float3* p_s );

class float3
{
public:
	union
	{
		struct
		{
			float	x,y,z;
		};
		struct
		{
			float	ry, rp, rr;
		};
	};

	float3( void ){};
	float3( float _x, float _y = 0.0, float _z = 0.0 )
	{
		x = _x;
		y = _y;
		z = _z;
	}
	float3( const I44& b )
	{
		x = b.x;
		y = b.y;
		z = b.z;
	}
	float3( char* p_str,  char** pp_str )
	{
		str2f3( p_str, pp_str );
	}
	float3& str2f3( char* p_str,  char** pp_str )
	{
		p_str += GPD_NINCS( p_str, " \t\r\n,." );
		x = strtod( p_str, &p_str );
		p_str += GPD_NINCS( p_str, " \t\r\n,." );
		y = strtod( p_str, &p_str );
		p_str += GPD_NINCS( p_str, " \t\r\n,." );
		z = strtod( p_str, &p_str );
		if( pp_str )
			*pp_str = p_str;
		return *this;
	}
	char* str(char* p_buff)
	{
		sprintf( p_buff, "%f, %f, %f,", x, y, z );
		return p_buff;
	}
	char* str3(char* p_buff)
	{
		sprintf(p_buff, "%0.3f, %0.3f, %0.3f,", x, y, z);
		return p_buff;
	}
	char* str4(char* p_buff)
	{
		sprintf(p_buff, "%0.4f, %0.4f, %0.4f,", x, y, z);
		return p_buff;
	}
	char* f2str2(char* p_buff, const char* p_ascii)
	{
		sprintf(p_buff, "%c%0.2f%c %c%0.2f%c %c%0.2f%c",	p_ascii[1], x, p_ascii[0],
															p_ascii[2], y, p_ascii[0],
															p_ascii[3], z, p_ascii[0] );
		return p_buff;
	}
	char* f2str(char* p_buff, const char* p_ascii)
	{
		sprintf(p_buff, "%c%f%c %c%f%c %c%f%c", p_ascii[1], x, p_ascii[0],
			p_ascii[2], y, p_ascii[0],
			p_ascii[3], z, p_ascii[0]);
		return p_buff;
	}
	/*float3& operator = ( const float3& b )
	{
		gp_memcpy( this, &b, sizeof(*this) );
		return *this;
	}*/
	float3& operator = ( float f )
	{
		x = y = z = f;
		return *this;
	}
	float3& null( void )
	{
		GPM_CLR;
		return *this;
	}
	
	bool operator != ( const float3& b ) const
	{
		if( x != b.x )
			return true;
		if( y != b.y )
			return true;
		if( z != b.z )
			return true;
		return false;
	}
	bool operator == ( const float3& b ) const
	{
		return !(*this != b);
	}
	float area_xy( void )
	{
		return x*y;
	}
	float vol_xyz( void )
	{
		return x*y*z;
	}
	

	float3 operator + ( const float3& b ) const;
	float3 operator - ( const float3& b ) const;
	
	float3 operator * ( float b ) const;
	float3 operator / ( float b ) const;

	float3 operator ^ ( const float3& b) const
	{
		return float3(
						y * b.z - z * b.y,
						z * b.x - x * b.z,
						x * b.y - y * b.x
				);
	}
	float3 norm( void ) const;
	//{
	//	float4 v4 = *this;
	//	float c = sqrt(v4.qlen());// sqrt(x*x+y*y+z*z);
	//	if( c == 1.0f )
	//		return *this;

	//	return *this / c;
	//}
	float operator * ( const float3& b) const
	{
		return x*b.x + y*b.y + z*b.z;
	}
	float3 operator / (const float3& b) const
	{
		return float3( x/b.x, y/b.y, z/b.z );
	}
	float3 operator + () const
	{
		return *this;
	}
	float3 operator - () const
	{
		return float3( -x, -y, -z );
	}

	float3& operator *= ( float b );
	float3& operator /= ( float b );

	float3& operator += ( const float3& b );
	float3& operator -= ( const float3& b );
	float3& operator &= ( const float3& b );
	float3& operator /= ( const float3& b );
	
	float3 operator & ( const float3& b ) const;

	float2 operator & ( const float2& b ) const
	{
		return float2( x*b.x, y*b.y );
	}
	
	float2 operator / ( const float2& b ) const
	{
		return float2( x/b.x, y/b.y );
	}

	float operator * ( const float2& b ) const
	{
		return x*b.x+y*b.y;
	}
	float3 turn( const float3& v2, float n )
	{
		if( n == 0.0 )
			return *this;

		if( n == 1.0 )
			return v2;

		float c = *this * v2;
		if( c >= COS5 )
			return v2;
		float rad = c > -COS5 ? acos( c ) : PI;
		rad *= n;
		if(rad < PIp180)
			return *this;

		float3	rig = c < -COS5 ? (*this ^ (v2-float3(v2.z*1.0, v2.x*3.0, v2.z*3.0) ).norm() ) : (*this ^ v2 ).norm(),
				frn = (rig^(*this)).norm();
		return frn*sin(n) + *this*cos(n);
	}

	float3 right_xy(void) const
	{
		return float3( y, -x, z );
	}
	float3 right_yz(void) const
	{
		return float3(x, z, -y);
	}
	float3 right_xz(void) const
	{
		return float3( z, y, -x );
	}
	float3 abs(void) const
	{
		return float3( ::abs(x), ::abs(y), ::abs(z) );
	}
	float3 floor(void) const
	{
		return float3( (I8)x, (I8)y, (I8)z );
	}
	float sum(void) const
	{
		return x+y+z;
	}
	float qlen( void ) const
	{
		return x*x + y*y + z*z;
	}
};



__declspec(align(16)) class float4
{
public:
	float	x,y,z,w;

	char* str( char* p_buff )
	{
		sprintf(p_buff, "%f, %f, %f, %f,", x, y, z, w);
		return p_buff;
	}
	char* str4(char* p_buff)
	{
		sprintf(p_buff, "%0.4f, %0.4f, %0.4f, %0.4f,", x, y, z, w);
		return p_buff;
	}

	float4( void ){};
	float4( const float3& b )
	{
		gp_memcpy( this, &b, sizeof(float3) );
		w = 0.0;
	}
	float4& null( void )
	{
		GPM_CLR;
		return *this;
	}
	float4( float f )
	{
		x = 
		y = 
		z = 
		w = f;
	}
	float4( float _x, float _y, float _z, float _w )
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}
	float4( const double* pD )
	{
		x = pD[0];
		y = pD[1];
		z = pD[2];
		w = pD[3];
	}
	float4( const I8* p_i84x )
	{
		x = p_i84x[0];
		y = p_i84x[1];
		z = p_i84x[2];
		w = p_i84x[3];
	}
	/*float4( const I84& b )
	{
		x = b.x;
		y = b.y;
		z = b.z;
		w = b.w;
	}*/
	float4& operator = ( float f )
	{
		x = y = z = w = f;
		return *this;
	}
	float4& float3w1( const float3& b )
	{
		gp_memcpy( this, &b, sizeof(b) );
		w = 1.0;
		return *this;
	}
	float4& operator = ( const U14& b )
	{
		x = b.B;
		y = b.G;
		z = b.R;
		w = b.A;
		return *this;
	}
	float4& operator = ( const U44& b )
	{
		x = b.x;
		y = b.y;
		z = b.z;
		w = b.w;
		return *this;
	}
	float4& operator = (const I84& b)
	{
		x = b.x;
		y = b.y;
		z = b.z;
		w = b.w;
		return *this;
	}
	float4& operator = ( const float3& b )
	{
		gp_memcpy( this, &b, sizeof(b) );
		w = 0.0;
		return *this;
	}
	bool operator != ( const float4& b ) const
	{
		if( x != b.x )
			return true;
		if( y != b.y )
			return true;
		if( z != b.z )
			return true;
		if( w != b.w )
			return true;
		return false;
	}
	bool operator == ( const float4& b ) const
	{
		return !(*this != b);
	}
	float	area_xy( void )
	{
		float arr = x;
		return arr*y;
	}
	float vol_xyz( void )
	{
		float vol = x;
		return vol*y*z;
	}
	
	float4& operator /= ( const float4& b )
	{
		gp_f44_div( this, &b );
		return *this;
	}
	float4 operator / (const float4& b) const
	{
		float4 o = *this;
		gp_f44_div( &o, &b);
		return o;
	}
	float4& operator &= ( const float4& b )
	{
		gp_f44_mul( this, &b );
		return *this;
	}
	float4& operator *= ( float b )
	{
		if( b == 1.0 )
			return *this;

		float4 bb = b;
		gp_f44_mul( this, &bb );
		return *this;
	}
	float4& operator += ( const float4& b )
	{
		gp_f44_add( this, &b );
		return *this;
	}
	float4& operator -= ( const float4& b )
	{
		gp_f44_sub( this, &b );
		return *this;
	}
	float4 operator + ( const float4& b ) const
	{
		float4 o = *this;
		gp_f44_add( &o, &b );
		return o;
	}
	float4 operator - ( const float4& b ) const
	{
		float4 o = *this;
		gp_f44_sub( &o, &b );
		return o;
	}
	float4& col4x4( const float* p_f )
	{
		x = p_f[0];
		y = p_f[4];
		z = p_f[8];
		w = p_f[12];
		return *this;
	}
	float operator * ( const float4& b ) const
	{
		float4 o = *this;
		gp_f44_dot( &o, &b );
		return o.x;
	}
	float4 operator & ( const float4& b ) const
	{
		float4 o = *this;
		gp_f44_mul( &o, &b );
		return o;
	}
	float4 operator * ( float b ) const
	{
		if( b == 1.0 )
			return *this;
		float4 o = b;
		gp_f44_mul( &o, this );
		return o;
	}

	float qlen( void ) const
	{
		float4 o = *this;
		gp_f44_qlen( &o );
		return o.x;
	}
	float qlen_xyz(void) const
	{
		float4 o = *this;
		gp_f44_qlen(&o);
		return o.x-w*w;
	}
	float len( void ) const
	{
		float4 o = *this;
		gp_f44_len( &o );
		return o.x;
	}
	float4 norm( void ) 
	{
		float4 n = *this, o = *this;
		gp_f44_len( &n );
		gp_f44_div( &o, &n );
		return o;
	}
	float4& mx_xyz(const float4& b)
	{
		if(x < b.x)
			x = b.x;
		if(y < b.y)
			y = b.y;
		if(z < b.z)
			z = b.z;
		return *this;
	}
	float4& mn_xyz(const float4& b)
	{
		if(x > b.x)
			x = b.x;
		if(y > b.y)
			y = b.y;
		if(z > b.z)
			z = b.z;
		return *this;
	}
	float mn_xyz( void )
	{
		float mn = x;
		
		if(mn > y)
			mn = y;
		if(mn > z)
			mn = z;
		return mn;
	}
	float4 abs(void) const
	{
		return float4(x > 0 ? x : -x, y > 0 ? y : -y, z > 0 ? z : -z, w > 0 ? w : -w);
	}
	float3 xyz(void)
	{
		return float3(x,y,z);
	}
	ID3D11Buffer* upl_n(  ID3D11Device* p_dev, ID3D11DeviceContext* p_ctx, ID3D11Buffer* pX11cb, U4 n )
	{
		if( pX11cb )
			return pX11cb;

		D3D11_BUFFER_DESC bDSC;
		GPMZ( bDSC );
		bDSC.ByteWidth = n*sizeof(float4);
		//bDSC.Usage = D3D11_USAGE_DEFAULT;				// D3D11_USAGE_STAGING;// D3D11_USAGE_IMMUTABLE; //D3D11_USAGE_DEFAULT;//    D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
		//bDSC.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// use as a vertex buffer
	
		bDSC.Usage = D3D11_USAGE_DYNAMIC;				// D3D11_USAGE_STAGING;// D3D11_USAGE_IMMUTABLE; //D3D11_USAGE_DEFAULT;//    D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
		bDSC.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// use as a vertex buffer
		bDSC.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		//D3D11_SUBRESOURCE_DATA iniDT;
		//GPMZ( iniDT );
		//iniDT.pSysMem = &mx_w;
		HRESULT hr = p_dev->CreateBuffer( &bDSC, NULL, &pX11cb );
		D3D11_MAPPED_SUBRESOURCE ms;
	
		hr = p_ctx->Map( pX11cb, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		if( !FAILED(hr) )
		{
			gp_memcpy( ms.pData, this, bDSC.ByteWidth );
			/*float4 *p_f4 = (float4*)ms.pData;
			for( U4 i = 0; i < n; i++ )
			{
				p_f4[i] = this[i];
			}*/
			p_ctx->Unmap( pX11cb, NULL );
		}

		return pX11cb;
	}
};
__declspec(align(16)) class double4
{	
public:
	double x,y,z,w;
	double4(){};
	double4( U4 u )
	{
		x = 
		y = 
		z = 
		(w = u);
	}
	
	double4( double X, double Y, double Z = 0, double W = 0 )
	{
		x = X;
		y = Y;
		z = Z;
		w = W;
	}
	double4( const double4& d4 )
	{
		gp_memcpy( this, &d4, sizeof(d4) );
	}
	double4( const float3& f3 )
	{
		x = f3.x;
		y = f3.y;
		z = f3.z;
		w = 0.0;
	}

	double4& operator = ( const float3& f3 )
	{
		x = f3.x;
		y = f3.y;
		z = f3.z;
		w = 0.0;
		return *this;
	}
	double4& operator = ( const float4& f4 )
	{
		x = f4.x;
		y = f4.y;
		z = f4.z;
		w = f4.w;
		return *this;
	}
	double4& operator = ( const U14& u )
	{
		x = u.B;
		y = u.G;
		z = u.R;
		w = u.A;
		return *this;
	}
	double4& operator = ( U4 u )
	{
		x = 
		y = 
		z = 
		(w = u);
		return *this;
	}
	double4& operator = ( const U44& u )
	{
		x = u.x;
		y = u.y;
		z = u.z;
		w = u.w;
		return *this;
	}
	double4& operator = ( const U84& u )
	{
		x = u.x;
		y = u.y;
		z = u.z;
		w = u.w;
		return *this;
	}
	double4& operator = ( const I44& u )
	{
		x = u.x;
		y = u.y;
		z = u.z;
		w = u.w;
		return *this;
	}
	double4& operator = ( const I84& u )
	{
		x = u.x;
		y = u.y;
		z = u.z;
		w = u.w;
		return *this;
	}

	double4( const float4& f4 )
	{
		*this = f4;
	}
	double4( const U14& u )
	{
		*this = u;
	}
	double4( const U44& u )
	{
		*this = u;
	}
	double4( const U84& u )
	{
		*this = u;
	}
	double4( const I44& u )
	{
		*this = u;
	}
	double4( const I84& u )
	{
		*this = u;
	}

	double4& operator *= ( double b )
	{
		__declspec(align(16)) double aB[4];

		aB[0] = aB[1] = aB[2] = aB[3] = b;
		gp_d4_mul( &x, aB );
		
		return *this;
	}

	double4& operator &= ( const double4& b )
	{
		gp_d4_mul( &x, &b.x );
		return *this;
	}
	
	double4& operator &= ( const I84& b )
	{
		double4 d4b = b;
		/*__declspec(align(16)) double aB[4];
		
		aB[0] = b.x;
		aB[1] = b.y;
		aB[2] = b.z;
		aB[3] = b.w;*/

		gp_d4_mul( &x, &d4b.x );
		return *this;
	}

	double4& operator /= ( const double4& b )
	{
		gp_d4_div( &x, &b.x );
		return *this;
	}
	double4& operator /= ( double b )
	{
		__declspec(align(16)) double aB[4];

		aB[0] = aB[1] = aB[2] = aB[3] = b;
		gp_d4_div( &x, aB );
		return *this;
	}
	double4& operator /= ( const I84& b )
	{
		__declspec(align(16)) double aB[4];
		aB[0] = b.x;
		aB[1] = b.y;
		aB[2] = b.z;
		aB[3] = b.w;

		gp_d4_div( &x, aB );
		return *this;
	}
	double4 operator & ( const I84& b ) const
	{
		double4 a = *this, d4b = b;
		/*__declspec(align(16)) double aB[4];

		aB[0] = b.x;
		aB[1] = b.y;
		aB[2] = b.z;
		aB[3] = b.w;*/

		gp_d4_mul( &a.x, &d4b.x );
		
		return a;
	}

	double4 operator * ( double b ) const
	{
		double4 a = *this;
		__declspec(align(16)) double aB[4];
		
		aB[0] = aB[1] = aB[2] = aB[3] = b;
		gp_d4_mul( &a.x, aB );

		return a;

	}

	double4 operator / ( double b ) const
	{
		double4 a = *this;
		__declspec(align(16)) double aB[4];
		
		aB[0] = aB[1] = aB[2] = aB[3] = b;
		gp_d4_div( &a.x, aB );

		return a;

	}
	
	double4& operator += ( const double4& b )
	{
		gp_d4_add( &x, &b.x );
		return *this;
	}

	double4 operator + ( const double4& b ) const
	{
		double4 a = *this;
		gp_d4_add( &a.x, &b.x );
		return a;
	}

	double4& operator -= ( const double4& b )
	{
		gp_d4_sub( &x, &b.x );
		return *this;
	}
	double4 operator - ( const double4& b ) const
	{
		double4 a = *this;
		gp_d4_sub( &a.x, &b.x );
		return a;
	}

	double4 operator / ( const I84& b ) const
	{
		double4 a = *this, db = b;
		__declspec(align(16)) double aB[4];
	
		aB[0] = b.x;
		aB[1] = b.y;
		aB[2] = b.z;
		aB[3] = b.w;

		gp_d4_div( &a.x, aB );
		
		return a;
	}
	I84 i84( void )
	{
		return I84( &x );
	}
	double sum( void ) const
	{
		return x+y+z+w;
	}
	double qlen(void) const
	{
		return x*x + y*y + z*z + w*w;
	}
	double qlen_xyz(void) const
	{
		return x * x + y * y + z * z;
	}
	float3 xyz(void)
	{
		return float3(x, y, z);
	}
};
//................|B11,B12,B13,B14|
//................|B21,B22,B23,B24|
//................|B31,B32,B33,B34|
//................|B41,B42,B43,B44|
//----------------+----------------
//|A11,A12,A13,A14|C11,C12,C13,C14|
//|A21,A22,A23,A24|C21,C22,C23,C24|
//|A31,A32,A33,A34|C31,C32,C33,C34|
//|A41,A42,A43,A44|C41,C42,C43,C44|
//C11 = A11*B11+A12*B21+A13*B31+A14*B41;
__declspec(align(16)) class float4x4
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
			float4		x4, y4, z4, t4;
		};
		struct
		{
			float3		x3; float xw;
			float3		y3; float yw;
			float3		z3; float zw;
			float3		t3; float tw;
		};
	};

	float4x4(){};
	float4x4& id( float f = 1.0f )
	{
		GPM_CLR;
		_11 = _22 = _33 = f;
		_44 = 1.0f;
		return *this;
	}
	float4x4( float f )
	{
		id( f );
	}

	float4x4(	float e11, float e12, float e13, float e14,
			float e21, float e22, float e23, float e24,
			float e31, float e32, float e33, float e34,
			float e41, float e42, float e43, float e44	)
	{
		_11 = e11;	_12 = e12;	_13 = e13;	_14 = e14;
		_21 = e21;	_22 = e22;	_23 = e23;	_24 = e24;
		_31 = e31;	_32 = e32;	_33 = e33;	_34 = e34;
		_41 = e41;	_42 = e42;	_43 = e43;	_44 = e44;
	}
	float4x4( float *p_f )
	{
		gp_memcpy( this, p_f, sizeof(*this) );
	}
	float4x4( const float4x4& b )
	{
		gp_memcpy( this, &b, sizeof(*this) );
	}
	float4x4( D3DXMATRIX& dxmx )
	{
		gp_memcpy( this, (void*)&dxmx, sizeof(*this) );
	}
	float4x4& zoom( float z )
	{
		x4 *= z;
		y4 *= z;
		z4 *= z;
		return *this;
	}
	float4x4& CW_yaw( float rad )
	{
		if(rad*rad > 0.0)
		{
			GPM_CLR;
			z3.z = x3.x = cos(rad);
			x3.z = -(z3.x = sin(rad));
			_44 = y3.y = 1.0;
			return *this;
		}

		return id();
	}
	float4x4& CW_pitch( float rad )
	{
		if(rad*rad > 0.0)
		{
			GPM_CLR;
			y3.y = z3.z = cos(rad);
			z3.y = -(y3.z = sin(rad));
			_44 = x3.x = 1.0;
			return *this;
		}

		return id();
	}
	float4x4& CW_roll( float rad )
	{
		if(rad*rad > 0.0)
		{
			GPM_CLR;
			y3.y = x3.x = cos(rad);
			x3.y = -(y3.x = sin(rad));
			_44 = z3.z = 1.0;
			return *this;
		}

		return id();
	}
	float4x4( U1 sw, float rad )
	{
		switch(sw%3)
		{
		case 0:
			CW_yaw(rad);
			return;
		case 1:
			CW_pitch(rad);
			return;
		}
		CW_roll(rad);
	}
	float4x4( U14 srt, const float3& ypr )
	{
		id();

		*this *= float4x4(srt.B, ((float*)&ypr)[srt.B]);
		
		//if( ((float*)&ypr)[srt.G] == 0.0 )
		//	return;
		*this *= float4x4(srt.G, ((float*)&ypr)[srt.G]);

		//if( ((float*)&ypr)[srt.R] == 0.0)
		//	return;

		*this *= float4x4(srt.R, ((float*)&ypr)[srt.R]);
	}
	float4x4& CW_ypr( const float3& ypr )
	{
		float l = ypr.qlen();
		if(l > 0.0)
		{
			U14 srt( 0, 1, 2 );
			float3 aypr = ypr.abs();
			float* pYPR = &aypr.x;
			if( pYPR[srt.B] > pYPR[srt.R] )
				srt.swap_BR();
			if(pYPR[srt.G] > pYPR[srt.R])
				srt.swap_GR();
			else if(pYPR[srt.B] > pYPR[srt.G])
				srt.swap_BG();
			
			*this = float4x4( srt, ypr );
			return *this;
		}

		return id();
	}

	float4x4& CW_ypr( const float3& ypr, const float3& speed )
	{
		float l = ypr.qlen();
		if(l > 0.0)
		{
			U14 srt(0, 1, 2);
			float3 aSPD = speed.abs();
			float* pSPD = &aSPD.x;
			if(pSPD[srt.B] < pSPD[srt.R])
				srt.swap_BR();
			if(pSPD[srt.G] < pSPD[srt.R])
				srt.swap_GR();
			else if(pSPD[srt.B] < pSPD[srt.G])
				srt.swap_BG();

			*this = float4x4(srt, ypr);
			return *this;
		}

		return id();
	}
	

	float4x4& ypr( const float3& ypr )
	{
		float	cosy = cosf(ypr.x), siny = sinf(ypr.x),
				cosp = cosf(ypr.y), sinp = sinf(ypr.y),
				cosr = cosf(ypr.z), sinr = sinf(ypr.z);
	
		_11 = cosr*cosy + sinr*sinp*siny;	_12 = sinr*cosp;	_13 = -cosr*siny + sinr*sinp*cosy;	_14 = 0.0f;
		_21 = -sinr*cosy + cosr*sinp*siny;	_22 = cosr*cosp;	_23 = sinr*siny + cosr*sinp*cosy;	_24 = 0.0f;
		_31 = cosp*siny;					_32 = -sinp;		_33 = cosp*cosy;					_34 = 0.0f;
		
		_41 = _42 = _43 = 0.0f;				_44 = 1.0f;

		return *this;
	}
	float4x4& ypr( const float4& ypr )
	{
		float	cosy = cosf(ypr.x), siny = sinf(ypr.x),
				cosp = cosf(ypr.y), sinp = sinf(ypr.y),
				cosr = cosf(ypr.z), sinr = sinf(ypr.z);
	
		_11 = cosr*cosy + sinr*sinp*siny;	_12 = sinr*cosp;	_13 = -cosr*siny + sinr*sinp*cosy;	_14 = 0.0f;
		_21 = -sinr*cosy + cosr*sinp*siny;	_22 = cosr*cosp;	_23 = sinr*siny + cosr*sinp*cosy;	_24 = 0.0f;
		_31 = cosp*siny;					_32 = -sinp;		_33 = cosp*cosy;					_34 = 0.0f;
		
		_41 = _42 = _43 = 0.0f;				_44 = 1.0f;

		return *this;
	}
	float4x4& AXr( const float3& vec, float rad )
	{
		double l = float4( vec ).qlen();
		float	cosr = cosf(rad), sinr = sinf(rad), t = 1-cosr,
				x = vec.x/l, y = vec.y/l, z = vec.z/l,
				x2 = x*x, y2 = y*y, z2 = z*z;
		_11 = t*x2+cosr;	_12 = t*x*y+sinr*z; _13 = t*x*z-sinr*y; _14 = 0.0f;
		_21 = t*x*y-sinr*z;	_22 = t*y2+cosr;	_23 = t*y*z+sinr*x; _24 = 0.0f;
		_31 = t*x*z+sinr*y; _32 = t*y*z-sinr*x;	_33 = t*z2+cosr;	_34 = 0.0f;
		
		_41 = _42 = _43 = 0.0f;			_44 = 1.0f;
	
		//D3DXMatrixRotationAxis((D3DXMATRIX*)this, (D3DXVECTOR3*)&vec, rad);
	
		return *this;
	}
	float3& eula( void )
	{
		float3 ypr;
		
		if( y4.x > 0.998f )
		{
			ypr.ry = atan2( x4.z, z4.z);
			ypr.rp = PIp2;
			ypr.rr = 0;
			return ypr;
		}
		else if( y4.x < -0.998f )
		{
			ypr.ry = atan2( x4.z, z4.z );
			ypr.rp = PIp2;
			ypr.rr = 0;
			return ypr;
		}

		ypr.ry = atan2(-z4.x, x4.x );
		ypr.rr = atan2(-y4.z, y4.y );
		ypr.rp = asin( y4.x );
		return ypr;
	}
	float4x4& lookat( float3 &eye, float3 &center, float3 &up )
	{
		float3	F = (center-eye).norm(),
				S = (up^F).norm(),
				U = F^S;

		_11=S.x; _12=U.x; _13=F.x; _14=0.0f; 
		_21=S.y; _22=U.y; _23=F.y; _24=0.0f; 
		_31=S.z; _32=U.z; _33=F.z; _34=0.0f; 
		_41=-eye*S; _42=-eye*U; _43=-eye*F; _44=1.0f; 

		return *this;
	}
	float4x4& proj( float2& ccd, float mount, float focus, float dist, float obj )
	{
		// a távolságot kapom meg, ahol a height belefér a t = mount-focus ba;
		GPM_CLR;
		if( mount < 12.5f )
			mount = 12.5f;

		_34 = 1.0f;
		float n = mount-focus,h,w;
		if( focus < 2 )
		{
			focus = 2.0f;
			dist = focus*(obj/ccd.y + 1.0f);
			n = mount-focus;
		}
		float r = ccd.x/ccd.y;

		if( n < focus )
		{
			// telecenter // ORTO opic?
			n = focus*2.0f;
			if( focus > 100.0f )
			{
				_34 = 0.0f;
				h = ccd.y*(1+(n/200.0f));
				w = h*r;
				_11 = 2.0f/w; 
				_22 = 2.0f/h;
				_33 = 4.0f/( dist*(1+(n/200.0f)) );
				_43 = -_33;
				return *this;
			}
		}
		// FL = (WD*IM)/(OB+IM)
		
		h = (n/focus-1)*ccd.y;
		w = h*r; 

		_11 = (2.0f/w); 
		_22 = (2.0f/h);
		_33 = (dist/(dist-n));		
		_43 = (-n*dist/(dist-n));

		return *this;
	}
	float4x4 inv( void )
	{
		float4x4 mxi; //dst;
		float tmp[12], src[16], det;
		float* p_src16 = (float*) this;
		float* p_dst16 = (float*) &mxi;
	
		 /* transpose matrix */
		/*   sor -> oszlop  */
		src[0 +  0]		= p_src16[0 * 4 + 0];
		src[0 +  4]		= p_src16[0 * 4 + 1];
		src[0 +  8]		= p_src16[0 * 4 + 2];
		src[0 + 12]		= p_src16[0 * 4 + 3];

		src[1 +  0]		= p_src16[1 * 4 + 0];
		src[1 +  4]		= p_src16[1 * 4 + 1];
		src[1 +  8]		= p_src16[1 * 4 + 2];
		src[1 + 12]		= p_src16[1 * 4 + 3];

		src[2 +  0]		= p_src16[2 * 4 + 0];
		src[2 +  4]		= p_src16[2 * 4 + 1];
		src[2 +  8]		= p_src16[2 * 4 + 2];
		src[2 + 12]		= p_src16[2 * 4 + 3];

		src[3 +  0]		= p_src16[3 * 4 + 0];
		src[3 +  4]		= p_src16[3 * 4 + 1];
		src[3 +  8]		= p_src16[3 * 4 + 2];
		src[3 + 12]		= p_src16[3 * 4 + 3];

		/* calculate pairs for first 8 elements (cofactors) */
			
		tmp[ 6] = src[ 8] * src[15];
		tmp[ 8] = src[ 8] * src[14];
		tmp[10] = src[ 8] * src[13];

		tmp[ 2] = src[ 9] * src[15];
		tmp[ 4] = src[ 9] * src[14];
		tmp[11] = src[ 9] * src[12];

		tmp[ 0] = src[10] * src[15];
		tmp[ 5] = src[10] * src[13];
		tmp[ 9] = src[10] * src[12];

		tmp[ 1] = src[11] * src[14];			
		tmp[ 3] = src[11] * src[13];
		tmp[ 7] = src[11] * src[12];

			/* calculate first 8 elements (cofactors) */
			p_dst16[0]  = tmp[0]*src[5] + tmp[3]*src[6] + tmp[4]*src[7];
			p_dst16[0] -= tmp[1]*src[5] + tmp[2]*src[6] + tmp[5]*src[7];

			p_dst16[1]  = tmp[1]*src[4] + tmp[6]*src[6] + tmp[9]*src[7];
			p_dst16[1] -= tmp[0]*src[4] + tmp[7]*src[6] + tmp[8]*src[7];
			
			p_dst16[2]  = tmp[2]*src[4] + tmp[7]*src[5] + tmp[10]*src[7];
			p_dst16[2] -= tmp[3]*src[4] + tmp[6]*src[5] + tmp[11]*src[7];
			
			p_dst16[3]  = tmp[5]*src[4] + tmp[8]*src[5] + tmp[11]*src[6];
			p_dst16[3] -= tmp[4]*src[4] + tmp[9]*src[5] + tmp[10]*src[6];
			


			p_dst16[4]  = tmp[1]*src[1] + tmp[2]*src[2] + tmp[5]*src[3];
			p_dst16[4] -= tmp[0]*src[1] + tmp[3]*src[2] + tmp[4]*src[3];
			
			p_dst16[5]  = tmp[0]*src[0] + tmp[7]*src[2] + tmp[8]*src[3];
			p_dst16[5] -= tmp[1]*src[0] + tmp[6]*src[2] + tmp[9]*src[3];
			
			p_dst16[6]  = tmp[3]*src[0] + tmp[6]*src[1] + tmp[11]*src[3];
			p_dst16[6] -= tmp[2]*src[0] + tmp[7]*src[1] + tmp[10]*src[3];
			
			p_dst16[7]  = tmp[4]*src[0] + tmp[9]*src[1] + tmp[10]*src[2];
			p_dst16[7] -= tmp[5]*src[0] + tmp[8]*src[1] + tmp[11]*src[2];
		
		/* calculate pairs for second 8 elements (cofactors) */
		tmp[6]	= src[0]*src[7];
		tmp[8]	= src[0]*src[6];
		tmp[10]	= src[0]*src[5];

		tmp[2]	= src[1]*src[7];
		tmp[4]	= src[1]*src[6];
		tmp[11]	= src[1]*src[4];
			
		tmp[0]	= src[2]*src[7];
		tmp[5]	= src[2]*src[5];
		tmp[9]	= src[2]*src[4];
			
		tmp[1]	= src[3]*src[6];
		tmp[3]	= src[3]*src[5];
		tmp[7]	= src[3]*src[4];

			/* calculate second 8 elements (cofactors) */
			p_dst16[ 8]	 = tmp[ 0]*src[13] + tmp[ 3]*src[14] + tmp[ 4]*src[15];
			p_dst16[ 8]	-= tmp[ 1]*src[13] + tmp[ 2]*src[14] + tmp[ 5]*src[15];

			p_dst16[ 9]	 = tmp[ 1]*src[12] + tmp[ 6]*src[14] + tmp[ 9]*src[15];
			p_dst16[ 9]	-= tmp[ 0]*src[12] + tmp[ 7]*src[14] + tmp[ 8]*src[15];
			
			p_dst16[10]	 = tmp[ 2]*src[12] + tmp[ 7]*src[13] + tmp[10]*src[15];
			p_dst16[10]	-= tmp[ 3]*src[12] + tmp[ 6]*src[13] + tmp[11]*src[15];
			
			p_dst16[11]	 = tmp[ 5]*src[12] + tmp[ 8]*src[13] + tmp[11]*src[14];
			p_dst16[11]	-= tmp[ 4]*src[12] + tmp[ 9]*src[13] + tmp[10]*src[14];
			


			p_dst16[12]	 = tmp[ 2]*src[10] + tmp[ 5]*src[11] + tmp[ 1]*src[ 9];
			p_dst16[12]	-= tmp[ 4]*src[11] + tmp[ 0]*src[ 9] + tmp[ 3]*src[10];
			
			p_dst16[13]	 = tmp[ 8]*src[11] + tmp[ 0]*src[ 8] + tmp[ 7]*src[10];
			p_dst16[13]	-= tmp[ 6]*src[10] + tmp[ 9]*src[11] + tmp[ 1]*src[ 8];
			
			p_dst16[14]	 = tmp[ 6]*src[ 9] + tmp[11]*src[11] + tmp[ 3]*src[ 8];
			p_dst16[14]	-= tmp[10]*src[11] + tmp[ 2]*src[ 8] + tmp[ 7]*src[ 9];
			
			p_dst16[15]	 = tmp[10]*src[10] + tmp[ 4]*src[ 8] + tmp[ 9]*src[ 9];
			p_dst16[15]	-= tmp[ 8]*src[ 9] + tmp[11]*src[10] + tmp[ 5]*src[ 8];
		
		/* calculate determinant */
		det = src[0]*p_dst16[0] + src[1]*p_dst16[1] + src[2]*p_dst16[2] + src[3]*p_dst16[3];

		/* calculate matrix inverse */
		//if(det >= -0.0000001 && det <= 0.0000001) return dst;
		det = 1.0f / det;

		p_dst16[ 0] *= det;
		p_dst16[ 1] *= det;
		p_dst16[ 2] *= det;
		p_dst16[ 3] *= det;
		p_dst16[ 4] *= det;
		p_dst16[ 5] *= det;
		p_dst16[ 6] *= det;
		p_dst16[ 7] *= det;
		p_dst16[ 8] *= det;
		p_dst16[ 9] *= det;
		p_dst16[10] *= det;
		p_dst16[11] *= det;
		p_dst16[12] *= det;
		p_dst16[13] *= det;
		p_dst16[14] *= det;
		p_dst16[15] *= det;

		return mxi;
	}
	float4x4& operator = ( const float4x4& b ) 
	{
		gp_memcpy( this, &b, sizeof(*this) );
		return *this;
	}
	float4x4& test( void )
	{
		float* p_f = &_11;
		for( float f = 0.0f; f < 16.0f; f++ )
		{
			*p_f = f;
			p_f++;
		}
		return *this;
	}
	float4x4& transp( const float4x4& b )
	{
		x4.col4x4( &b._11 ); // x = float4( b._11, b._21, b._31, b._41 );
		y4.col4x4( &b._12 ); // y = float4( b._12, b._22, b._32, b._42 );
		z4.col4x4( &b._13 ); // z = float4( b._13, b._23, b._33, b._43 );
		t4.col4x4( &b._14 ); // w = float4( b._14, b._24, b._34, b._44 );
		return *this;
	}
	float4x4 operator ^ (const float4x4& b)
	{
		float4x4 o(1);
		o.x4 = b.x4*x4.x + b.y4*x4.y + b.z4*x4.z;
		o.y4 = b.x4*y4.x + b.y4*y4.y + b.z4*y4.z;
		o.z4 = b.x4*z4.x + b.y4*z4.y + b.z4*z4.z;
		o.t4 = b.x4*t4.x + b.y4*t4.y + b.z4*t4.z + b.t4;
		return o;
	}
	float4x4& operator *= ( const float4x4& b )
	{
		/*float4x4 a = *this;
		_11 = a._11*b._11 + a._12*b._21 + a._13*b._31 + a._14*b._41; 		
		_12 = a._11*b._12 + a._12*b._22 + a._13*b._32 + a._14*b._42; 		
		_13 = a._11*b._13 + a._12*b._23 + a._13*b._33 + a._14*b._43; 		
		_14 = a._11*b._14 + a._12*b._24 + a._13*b._34 + a._14*b._44;
																	
		_21 = a._21*b._11 + a._22*b._21 + a._23*b._31 + a._24*b._41; 		
		_22 = a._21*b._12 + a._22*b._22 + a._23*b._32 + a._24*b._42; 		
		_23 = a._21*b._13 + a._22*b._23 + a._23*b._33 + a._24*b._43; 		
		_24 = a._21*b._14 + a._22*b._24 + a._23*b._34 + a._24*b._44;
																	
		_31 = a._31*b._11 + a._32*b._21 + a._33*b._31 + a._34*b._41; 		
		_32 = a._31*b._12 + a._32*b._22 + a._33*b._32 + a._34*b._42; 		
		_33 = a._31*b._13 + a._32*b._23 + a._33*b._33 + a._34*b._43; 		
		_34 = a._31*b._14 + a._32*b._24 + a._33*b._34 + a._34*b._44;
																	
		_41 = a._41*b._11 + a._42*b._21 + a._43*b._31 + a._44*b._41; 		
		_42 = a._41*b._12 + a._42*b._22 + a._43*b._32 + a._44*b._42; 		
		_43 = a._41*b._13 + a._42*b._23 + a._43*b._33 + a._44*b._43; 		
		_44 = a._41*b._14 + a._42*b._24 + a._43*b._34 + a._44*b._44;
		return *this;*/

		
		float4 v;
		float4x4 c = *this;
		
		v.col4x4( &b._11 );
		_11 = c.x4*v;
		_21 = c.y4*v;
		_31 = c.z4*v;
		_41 = c.t4*v;

		v.col4x4( &b._12 );
		_12 = c.x4*v;
		_22 = c.y4*v;
		_32 = c.z4*v;
		_42 = c.t4*v;

		v.col4x4( &b._13 );
		_13 = c.x4*v;
		_23 = c.y4*v;
		_33 = c.z4*v;
		_43 = c.t4*v;

		v.col4x4( &b._14 );
		_14 = c.x4*v;
		_24 = c.y4*v;
		_34 = c.z4*v;
		_44 = c.t4*v;

		return *this;
	}
	float4x4 operator * ( const float4x4& b ) const
	{
		float4 v; 
		float4x4 c;
		
		v.col4x4( &b._11 );
		c._11 = x4*v;
		c._21 = y4*v;
		c._31 = z4*v;
		c._41 = t4*v;

		v.col4x4( &b._12 );
		c._12 = x4*v;
		c._22 = y4*v;
		c._32 = z4*v;
		c._42 = t4*v;

		v.col4x4( &b._13 );
		c._13 = x4*v;
		c._23 = y4*v;
		c._33 = z4*v;
		c._43 = t4*v;

		v.col4x4( &b._14 );
		c._14 = x4*v;
		c._24 = y4*v;
		c._34 = z4*v;
		c._44 = t4*v;

		return c;

		/*float4x4 a = *this, c;
		c._11 = a._11*b._11 + a._12*b._21 + a._13*b._31 + a._14*b._41; 		
		c._12 = a._11*b._12 + a._12*b._22 + a._13*b._32 + a._14*b._42; 		
		c._13 = a._11*b._13 + a._12*b._23 + a._13*b._33 + a._14*b._43; 		
		c._14 = a._11*b._14 + a._12*b._24 + a._13*b._34 + a._14*b._44;
																		
		c._21 = a._21*b._11 + a._22*b._21 + a._23*b._31 + a._24*b._41; 		
		c._22 = a._21*b._12 + a._22*b._22 + a._23*b._32 + a._24*b._42; 		
		c._23 = a._21*b._13 + a._22*b._23 + a._23*b._33 + a._24*b._43; 		
		c._24 = a._21*b._14 + a._22*b._24 + a._23*b._34 + a._24*b._44;
																		
		c._31 = a._31*b._11 + a._32*b._21 + a._33*b._31 + a._34*b._41; 		
		c._32 = a._31*b._12 + a._32*b._22 + a._33*b._32 + a._34*b._42; 		
		c._33 = a._31*b._13 + a._32*b._23 + a._33*b._33 + a._34*b._43; 		
		c._34 = a._31*b._14 + a._32*b._24 + a._33*b._34 + a._34*b._44;
																		
		c._41 = a._41*b._11 + a._42*b._21 + a._43*b._31 + a._44*b._41; 		
		c._42 = a._41*b._12 + a._42*b._22 + a._43*b._32 + a._44*b._42; 		
		c._43 = a._41*b._13 + a._42*b._23 + a._43*b._33 + a._44*b._43; 		
		c._44 = a._41*b._14 + a._42*b._24 + a._43*b._34 + a._44*b._44;
		return c;*/
	}
	float4 operator * ( const float4& va )
	{
		return x4*va.x + y4*va.y + z4*va.z + t4*va.w; 
	}
	float3 operator * ( const float3& va )
	{
		float4 vc = x4*va.x + y4*va.y + z4*va.z + t4; 
		return vc.xyz();
	}

	I84 operator / (const I84& va)
	{
		float4 vc, d4 = &va.x;
		vc.x = d4 * x4;
		vc.y = d4 * y4;
		vc.z = d4 * z4;
		vc.w = d4.w;
		vc -= t4;
		return I84( vc );
	}

	float4 operator / (const float4& va)
	{
		float4 vc;
		vc.x = va * x4;
		vc.y = va * y4;
		vc.z = va * z4;
		vc -= t4;
		return vc.xyz();
	}

	float3 operator / (const float3& va )
	{
		float4 vc, d4 = va;
		vc.x = d4 * x4;
		vc.y = d4 * y4;
		vc.z = d4 * z4;
		vc -= t4;
		return vc.xyz();
	}
	float4x4& ortoZ(void)
	{
		float	d = z3.qlen();
		if( d != 1.0 )
		{
			z3 /= sqrt(d);
		}
		float3	rig = y3^z3,
				up = z3^x3;
		float	r = rig.qlen(),
				u = up.qlen();
		if( u > r )
		{
			up /= sqrt(u);
			x3 = (up ^ z3).norm();
			y3 = (z3 ^ x3).norm();
			return *this;
		}
		rig /= sqrt(r);
		y3 = (z3 ^ rig).norm();
		x3 = (y3 ^ z3).norm();
		return *this;
	}
	float4x4& orto( void )
	{
		y4.w = 0.0;
		float q = y4.qlen();
		if( q > 0.0 )
		{
			y4 /= sqrt(q);
		} else {
			q = float4(t3).qlen();
			if( q > 0.0 )
				y4 = t3;
			else {
				y3 = z3^x3;
				q = y4.qlen();
			}

			if( q > 0.0 )
			{
				y4 /= sqrt(q);
			} else {
				y4.y = 1.0;
			}
		}

		z4.w = x4.w = 0.0;

		z4 -= y4 * (y4*z4);
		q = z4.qlen();
		if( q > 0.0 )
		{
			z4 /= sqrt(q);
			x3 = y3^z3;
		} else {
			x4 -= y4*(y4*x4);
			q = x4.qlen();
			if( q > 0.0 )
			{
				x4 /= sqrt(q);
				z3 = x3^y3;
			}
		}
		return *this;
	}
	I84& operator * ( const I84& w ) const
	{
		I84 fly = w;
		if( !this )
			return fly;

		if( !w.qlen_xyz() )
			return fly;

		fly = (double4(x4) * (double)w.x + double4(y4) * (double)w.y + double4(z4) * (double)w.z + double4(t4)*w.w ).i84();
		return fly;
	}
};
class GPC_DICT_REC
{
public:
	char		*p_str, *p_rec;
	I8			n_id, //n_level,
				n_lo, n_hi,
				n_rec;

	~GPC_DICT_REC()
	{
		if( p_str == p_rec )
			GPM_DELARRAY( p_str );
	}
};

class GPC_DICT
{
public:
	GPC_DICT_REC	*dick_p_alloc,
					*dick_p_load,
					*p_find,
					*p_back;
	
	I8				dick_n_alloc;
	~GPC_DICT()
	{
		GPM_DELARRAY( dick_p_alloc );
	}
	GPC_DICT* dick_add( char* p_src, const char* p_token = " \t\r\n" )
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
		
		GPC_DICT* p_this = this;

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

	GPC_DICT* dick_find( char* p_word, I8 n_word, char* p_begin = NULL )
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

			GPC_DICT* p_this = this ? this : (GPC_DICT*)memset( new GPC_DICT, 0, sizeof(*p_this) );
			if( p_this )
			{
				p_this->dick_n_alloc = 0x20;
				p_this->dick_p_alloc = p_this->dick_p_load = new GPC_DICT_REC[p_this->dick_n_alloc];
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
						GPC_DICT_REC* dick_p_kill = dick_p_alloc;
						dick_p_alloc = new GPC_DICT_REC[dick_n_alloc+0x10];
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
					GPC_DICT_REC* dick_p_kill = dick_p_alloc;
					dick_p_alloc = new GPC_DICT_REC[dick_n_alloc+0x10];
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
typedef enum gpeVX : U4
{
	gpeVXu142,
	gpeVXcl,
	gpeVXclsk,
	gpeVXc,
};
typedef enum gpeSURF : U4
{
	gpeSURF_mini,

	gpeSURF_clnosk,
	gpeSURF_clnoskline,
	gpeSURF_clglbmp,
	gpeSURF_clglbdzr,
	gpeSURF_clglbu142,
	gpeSURF_clsk,
	
	gpeSURF_stereo,

	gpeSURF_vxcline,

	gpeSURF_vxcsprite,

	gpeSURF_end,
};

typedef enum gpeTRG : U4
{
	gpeTRG_back,
	gpeTRG_ocluLS,
	gpeTRG_ocluSS,
	gpeTRG_shdw,
	gpeTRG_trace,
	gpeTRG_txt ,

	gpeTRG_stereo,

	gpeTRG_end ,
};
typedef enum gpeBLEND : U4
{
	gpeBLEND_off,
	gpeBLEND_on,
	
	gpeBLEND_addRGB,
	
	gpeBLEND_end ,
};
typedef enum gpeDP : U4
{
	gpeDP_off,
	gpeDP_on,
	gpeDP_END,
};
#define GPD_SHDR( t, s ) ( gpeSURF_end*t + s)

typedef enum gpeALFA : I8
{
	gpeALFA_zero = 0,
	gpeALFA_A,
	gpeALFA_B,
	gpeALFA_C,

	gpeALFA_D,
	gpeALFA_E,
	gpeALFA_F,
	gpeALFA_G,

	gpeALFA_H,
	gpeALFA_I,
	gpeALFA_J,
	gpeALFA_K,

	gpeALFA_L,
	gpeALFA_M,
	gpeALFA_N,
	gpeALFA_O,

	gpeALFA_P,
	gpeALFA_Q,
	gpeALFA_R,
	gpeALFA_S,

	gpeALFA_T,
	gpeALFA_U,
	gpeALFA_V,
	gpeALFA_W,

	gpeALFA_X,
	gpeALFA_Y,
	gpeALFA_Z,
	//- 2 -----------------
	gpeALFA_AA = GPD_AB('A', 'A'),
	gpeALFA_AN = GPD_AB('A', 'N'),
	gpeALFA_AT = GPD_AB('A', 'T'),
	gpeALFA_CH = GPD_AB('C', 'H'),
	gpeALFA_CL = GPD_AB('C', 'L'),
	gpeALFA_CU = GPD_AB('C', 'U'),
	gpeALFA_CW = GPD_AB('C', 'W'),
	gpeALFA_DS = GPD_AB('D', 'S'),
	gpeALFA_FI = GPD_AB('F', 'I'),
	gpeALFA_GX = GPD_AB('G', 'X'),
	gpeALFA_ID = GPD_AB('I', 'D'),
	gpeALFA_IN = GPD_AB('I', 'N'),

	// key ( A, B, ..., Z )
	gpeALFA_KA = GPD_AB('K', 'A'),
	gpeALFA_KO = GPD_AB('K', 'O'),
	gpeALFA_KZ = GPD_AB('K', 'Z'),

	gpeALFA_MA = GPD_AB('M', 'A'),
	gpeALFA_MN = GPD_AB('M', 'N'),
	gpeALFA_MS = GPD_AB('M', 'S'),
	gpeALFA_MX = GPD_AB('M', 'X'),
	gpeALFA_MY = GPD_AB('M', 'Y'),
	gpeALFA_NO = GPD_AB('N', 'O'),
	gpeALFA_ON = GPD_AB('O', 'N'),
	gpeALFA_OR = GPD_AB('O', 'R'),
	gpeALFA_PI = GPD_AB('P', 'I'),
	gpeALFA_RE = GPD_AB('R', 'E'),
	gpeALFA_SR = GPD_AB('S', 'R'),
	gpeALFA_UP = GPD_AB('U', 'P'),
	gpeALFA_WB = GPD_AB('W', 'B'),
	gpeALFA_WH = GPD_AB('W', 'H'),
	gpeALFA_WX = GPD_AB('W', 'X'),
	gpeALFA_WY = GPD_AB('W', 'Y'),
	gpeALFA_WW = GPD_AB('W', 'W'),
	gpeALFA_ZM = GPD_AB('Z', 'M'),
	gpeALFA_2 = GPD_AB('Z', 'Z'),
	//- 3 -----------------
	gpeALFA_ABS = GPD_ABC('A', 'B', 'S'),
	gpeALFA_ACE = GPD_ABC('A', 'C', 'E'), // access
	gpeALFA_ACT = GPD_ABC('A', 'C', 'T'),
	gpeALFA_ADD = GPD_ABC('A', 'D', 'D'),
	gpeALFA_ADR = GPD_ABC('A', 'D', 'R'),
	gpeALFA_AIN = GPD_ABC('A', 'I', 'N'),
	gpeALFA_ARJ = GPD_ABC('A', 'R', 'J'),
	gpeALFA_ASM = GPD_ABC('A', 'S', 'M'),
	gpeALFA_BIN = GPD_ABC('B', 'I', 'N'),
	gpeALFA_BOB = GPD_ABC('B', 'O', 'B'),
	gpeALFA_BOX = GPD_ABC('B', 'O', 'X'),
	gpeALFA_CAM = GPD_ABC('C', 'A', 'M'),
	gpeALFA_CLR = GPD_ABC('C', 'L', 'R'),
	gpeALFA_CMX = GPD_ABC('C', 'M', 'X'),
	gpeALFA_CMY = GPD_ABC('C', 'M', 'Y'),
	gpeALFA_CNL = GPD_ABC('C', 'N', 'L'),
	gpeALFA_COS = GPD_ABC('C', 'O', 'S'),
	gpeALFA_CPP = GPD_ABC('C', 'P', 'P'),
	gpeALFA_CPU = GPD_ABC('C', 'P', 'U'),
	gpeALFA_CPY = GPD_ABC('C', 'P', 'Y'),
	gpeALFA_CUT = GPD_ABC('C', 'U', 'T'),
	gpeALFA_DEC = GPD_ABC('D', 'E', 'C'),
	gpeALFA_DEF = GPD_ABC('D', 'E', 'F'),
	gpeALFA_DIR = GPD_ABC('D', 'I', 'R'),
	gpeALFA_DST = GPD_ABC('D', 'S', 'T'),
	gpeALFA_DZR = GPD_ABC('D', 'Z', 'R'),
	gpeALFA_END = GPD_ABC('E', 'N', 'D'),
	gpeALFA_ERU = GPD_ABC('E', 'R', 'U'),
	gpeALFA_EYE = GPD_ABC('E', 'Y', 'E'),

	// function key ( f1, f2, ..., f12)
	gpeALFA_FKA = GPD_ABC('F', 'K', 'A'),
	gpeALFA_FKL = GPD_ABC('F', 'K', 'L'),

	gpeALFA_FPS = GPD_ABC('F', 'P', 'S'),
	gpeALFA_FMX = GPD_ABC('F', 'M', 'X'),
	gpeALFA_FMY = GPD_ABC('F', 'M', 'Y'),
	gpeALFA_GET = GPD_ABC('G', 'E', 'T'),
	gpeALFA_GIM = GPD_ABC('G', 'I', 'M'),
	gpeALFA_GIO = GPD_ABC('G', 'I', 'O'),
	gpeALFA_GIS = GPD_ABC('G', 'I', 'S'),
	gpeALFA_GIT = GPD_ABC('G', 'I', 'T'),
	gpeALFA_GIV = GPD_ABC('G', 'I', 'V'),
	gpeALFA_GPU = GPD_ABC('G', 'P', 'U'),
	gpeALFA_GXT = GPD_ABC('G', 'X', 'T'),
	gpeALFA_HUD = GPD_ABC('H', 'U', 'D'),
	gpeALFA_INC = GPD_ABC('I', 'N', 'C'),
	gpeALFA_INT = GPD_ABC('I', 'N', 'T'),
	gpeALFA_JPG = GPD_ABC('J', 'P', 'G'),
	gpeALFA_JOY = GPD_ABC('J', 'O', 'Y'),
	gpeALFA_KEY = GPD_ABC('K', 'E', 'Y'),
	gpeALFA_LOG = GPD_ABC('L', 'O', 'G'),
	gpeALFA_LWO = GPD_ABC('L', 'W', 'O'),
	gpeALFA_LWS = GPD_ABC('L', 'W', 'S'),
	gpeALFA_MAN = GPD_ABC('M', 'A', 'N'),
	gpeALFA_MAX = GPD_ABC('M', 'A', 'X'),
	gpeALFA_MIN = GPD_ABC('M', 'I', 'N'),
	gpeALFA_MLB = GPD_ABC('M', 'L', 'B'),
	gpeALFA_MMX = GPD_ABC('M', 'M', 'X'),
	gpeALFA_MMY = GPD_ABC('M', 'M', 'Y'),
	gpeALFA_MOM = GPD_ABC('M', 'O', 'M'),
	gpeALFA_MPG = GPD_ABC('M', 'R', 'B'),
	gpeALFA_MRB = GPD_ABC('M', 'R', 'B'),
	gpeALFA_OFF = GPD_ABC('O', 'F', 'F'),
	gpeALFA_OUT = GPD_ABC('O', 'U', 'T'),
	gpeALFA_PIC = GPD_ABC('P', 'I', 'C'),
	gpeALFA_PIX = GPD_ABC('P', 'I', 'X'),
	gpeALFA_PMX = GPD_ABC('P', 'M', 'X'),
	gpeALFA_PMY = GPD_ABC('P', 'M', 'Y'),
	gpeALFA_PNG = GPD_ABC('P', 'N', 'G'),
	gpeALFA_REF = GPD_ABC('R', 'E', 'F'),
	gpeALFA_REN = GPD_ABC('R', 'E', 'N'),
	gpeALFA_RUN = GPD_ABC('R', 'U', 'N'),
	gpeALFA_SIN = GPD_ABC('S', 'I', 'N'),
	gpeALFA_SIT = GPD_ABC('S', 'I', 'T'),
	gpeALFA_SRC = GPD_ABC('S', 'R', 'C'),
	gpeALFA_STR = GPD_ABC('S', 'T', 'R'),
	gpeALFA_SYS = GPD_ABC('S', 'Y', 'S'),
	gpeALFA_SUM = GPD_ABC('S', 'U', 'M'),
	gpeALFA_VEC = GPD_ABC('V', 'E', 'C'),
	gpeALFA_VOX = GPD_ABC('V', 'O', 'X'),
	gpeALFA_WIN = GPD_ABC('W', 'I', 'N'),
	gpeALFA_WMX = GPD_ABC('W', 'M', 'X'),
	gpeALFA_WMY = GPD_ABC('W', 'M', 'Y'),
	gpeALFA_YPR = GPD_ABC('Y', 'P', 'R'),
	gpeALFA_ZIP = GPD_ABC('Z', 'I', 'P'),
	gpeALFA_3 = GPD_ABC('Z', 'Z', 'Z'),
	//- 4 -----------------
	gpeALFA_ABMS = GPD_ABCD('A', 'B', 'M', 'S'),
	gpeALFA_ACOS = GPD_ABCD('A', 'C', 'O', 'S'),
	gpeALFA_ASIN = GPD_ABCD('A', 'S', 'I', 'N'),
	gpeALFA_BONE = GPD_ABCD('B', 'O', 'N', 'E'),
	gpeALFA_BUMM = GPD_ABCD('B', 'U', 'M', 'M'),
	gpeALFA_BUMP = GPD_ABCD('B', 'U', 'M', 'P'),
	gpeALFA_CNLC = GPD_ABCD('C', 'N', 'L', 'C'),
	gpeALFA_COPY = GPD_ABCD('C', 'O', 'P', 'Y'),
	gpeALFA_CRSA = GPD_ABCD('C', 'R', 'S', 'A'),
	gpeALFA_CRSN = GPD_ABCD('C', 'R', 'S', 'N'),
	gpeALFA_CRSX = GPD_ABCD('C', 'R', 'S', 'X'),
	gpeALFA_CRSY = GPD_ABCD('C', 'R', 'S', 'Y'),
	gpeALFA_DEEP = GPD_ABCD('D', 'E', 'E', 'P'),
	gpeALFA_EDGE = GPD_ABCD('E', 'D', 'G', 'E'),
	gpeALFA_ENDD = GPD_ABCD('E', 'N', 'D', 'D'),
	gpeALFA_EXIT = GPD_ABCD('E', 'X', 'I', 'T'),
	gpeALFA_EXPL = GPD_ABCD('E', 'X', 'P', 'L'),
	gpeALFA_FERI = GPD_ABCD('F', 'E', 'R', 'I'),
	gpeALFA_FILE = GPD_ABCD('F', 'I', 'L', 'E'),
	gpeALFA_FIND = GPD_ABCD('F', 'I', 'N', 'D'),
	gpeALFA_FSEC = GPD_ABCD('F', 'S', 'E', 'C'),
	gpeALFA_GOLD = GPD_ABCD('G', 'O', 'L', 'D'),


	gpeALFA_GPUC = GPD_ABCD('G', 'P', 'U', 'C'),	// gpeALFA_GPU + VOXEL VERTEX

	gpeALFA_HAIR = GPD_ABCD('H', 'A', 'I', 'R'),
	gpeALFA_HAND = GPD_ABCD('H', 'A', 'N', 'D'),
	gpeALFA_HEAD = GPD_ABCD('H', 'E', 'A', 'D'),
	gpeALFA_HELP = GPD_ABCD('H', 'E', 'L', 'P'),
	gpeALFA_HOST = GPD_ABCD('H', 'O', 'S', 'T'),
	gpeALFA_HTML = GPD_ABCD('H', 'T', 'M', 'L'),
	gpeALFA_HTTP = GPD_ABCD('H', 'T', 'T', 'P'),

	gpeALFA_IDLE = GPD_ABCD('I', 'D', 'L', 'E'),

	gpeALFA_ITEM = GPD_ABCD('I', 'T', 'E', 'M'),
	gpeALFA_JUMP = GPD_ABCD('J', 'U', 'M', 'P'),

	gpeALFA_KALT = GPD_ABCD('K', 'A', 'L', 'T'),
	gpeALFA_KICK = GPD_ABCD('K', 'I', 'C', 'K'),

	gpeALFA_LINE = GPD_ABCD('L', 'I', 'N', 'E'),
	gpeALFA_LEFT = GPD_ABCD('L', 'E', 'F', 'T'),
	gpeALFA_LOOP = GPD_ABCD('L', 'O', 'O', 'P'),
	gpeALFA_MINI = GPD_ABCD('M', 'I', 'N', 'I'),

	gpeALFA_MONO = GPD_ABCD('M', 'O', 'N', 'O'),
	gpeALFA_MOON = GPD_ABCD('M', 'O', 'O', 'N'),

	gpeALFA_MLBX = GPD_ABCD('M', 'L', 'B', 'X'),		// egér X poziciója mielött le lett nyomva a BAL gomb
	gpeALFA_MLBY = GPD_ABCD('M', 'L', 'B', 'Y'),		// egér Y poziciója mielött le lett nyomva a BAL gomb

	gpeALFA_MRBX = GPD_ABCD('M', 'R', 'B', 'X'),		// egér X poziciója mielött le lett nyomva a JOBB gomb
	gpeALFA_MRBY = GPD_ABCD('M', 'R', 'B', 'Y'),		// egér Y poziciója mielött le lett nyomva a JOBB gomb
	
	gpeALFA_NAME = GPD_ABCD('N', 'A', 'M', 'E'),
	gpeALFA_MSEC = GPD_ABCD('M', 'S', 'E', 'C'),
	gpeALFA_NEAR = GPD_ABCD('N', 'E', 'A', 'R'),
	gpeALFA_NUSE = GPD_ABCD('N', 'U', 'S', 'E'),
	gpeALFA_OTOS = GPD_ABCD('O', 'T', 'O', 'S'),
	gpeALFA_OVER = GPD_ABCD('O', 'V', 'E', 'R'),

	gpeALFA_PACK = GPD_ABCD('P', 'A', 'C', 'K'),
	gpeALFA_PASS = GPD_ABCD('P', 'A', 'S', 'S'),

	gpeALFA_PIXN = GPD_ABCD('P', 'I', 'X', 'N'),
	gpeALFA_POLY = GPD_ABCD('P', 'O', 'L', 'Y'),
	gpeALFA_PORT = GPD_ABCD('P', 'O', 'R', 'T'),

	gpeALFA_QUAD = GPD_ABCD('Q', 'U', 'A', 'D'),

	//gpeALFA_RESA =	GPD_ABCD( 'R','E','S','A' ),
	gpeALFA_ROBI = GPD_ABCD('R', 'O', 'B', 'I'),
	gpeALFA_ROCK = GPD_ABCD('R', 'O', 'C', 'K'),
	gpeALFA_SAVE = GPD_ABCD('S', 'A', 'V', 'E'),
	gpeALFA_STAR = GPD_ABCD('S', 'T', 'A', 'R'),
	gpeALFA_STEP = GPD_ABCD('S', 'T', 'E', 'P'),
	gpeALFA_STON = GPD_ABCD('S', 'T', 'O', 'N'),
	gpeALFA_STOW = GPD_ABCD('S', 'T', 'O', 'W'),
	gpeALFA_SQRT = GPD_ABCD('S', 'Q', 'R', 'T'),
	gpeALFA_TURN = GPD_ABCD('T', 'U', 'R', 'N'),
	
	gpeALFA_USER = GPD_ABCD('U', 'S', 'E', 'R'),

	gpeALFA_WALK = GPD_ABCD('W', 'A', 'L', 'K'),
	gpeALFA_WHAM = GPD_ABCD('W', 'H', 'A', 'M'),

	gpeALFA_4 = GPD_ABCD('Z', 'Z', 'Z', 'Z'),
	//- 5 -----------------
	gpeALFA_BLOCK = GPD_ABCDE('B', 'L', 'O', 'C', 'K'),
	gpeALFA_BOBER = GPD_ABCDE('B', 'O', 'B', 'E', 'R'),
	gpeALFA_BUBLE = GPD_ABCDE('B', 'U', 'B', 'L', 'E'),
	gpeALFA_CACHE = GPD_ABCDE('C', 'A', 'C', 'H', 'E'),
	gpeALFA_COLOR = GPD_ABCDE('C', 'O', 'L', 'O', 'R'),
	gpeALFA_COUNT = GPD_ABCDE('C', 'O', 'U', 'N', 'T'),
	gpeALFA_CROSS = GPD_ABCDE('C', 'R', 'O', 'S', 'S'),
	gpeALFA_DEBUG = GPD_ABCDE('D', 'E', 'B', 'U', 'G'),
	gpeALFA_DEBUS = GPD_ABCDE('D', 'E', 'B', 'U', 'S'),
	gpeALFA_ENTER = GPD_ABCDE('E', 'N', 'T', 'E', 'R'),
	gpeALFA_ERECT = GPD_ABCDE('E', 'R', 'E', 'C', 'T'),
	gpeALFA_GETMX = GPD_ABCDE('G', 'E', 'T', 'M', 'X'),
	gpeALFA_GLOBA = GPD_ABCDE('G', 'L', 'O', 'B', 'A'),
	gpeALFA_HISTI = GPD_ABCDE('H', 'I', 'S', 'T', 'I'),
	gpeALFA_HUMAN = GPD_ABCDE('H', 'U', 'M', 'A', 'N'),
	gpeALFA_INDEX = GPD_ABCDE('I', 'N', 'D', 'E', 'X'),
	gpeALFA_KCTRL = GPD_ABCDE('K', 'C', 'T', 'R', 'L'),
	gpeALFA_LABEL = GPD_ABCDE('L', 'A', 'B', 'E', 'L'),
	gpeALFA_LATHE = GPD_ABCDE('L', 'A', 'T', 'H', 'E'),
	gpeALFA_LOCAL = GPD_ABCDE('L', 'O', 'C', 'A', 'L'),
	gpeALFA_NGATE = GPD_ABCDE('N', 'G', 'A', 'T', 'E'),
	gpeALFA_NGCON = GPD_ABCDE('N', 'G', 'C', 'O', 'N'),
	gpeALFA_NGDIE = GPD_ABCDE('N', 'G', 'D', 'I', 'E'),

	gpeALFA_ORBIT = GPD_ABCDE('O', 'R', 'B', 'I', 'T'),
	gpeALFA_PASTE = GPD_ABCDE('P', 'A', 'S', 'T', 'E'),
	gpeALFA_PICQC = GPD_ABCDE('P', 'I', 'C', 'Q', 'C'),
	gpeALFA_PRINT = GPD_ABCDE('P', 'R', 'I', 'N', 'T'),
	gpeALFA_REPIC = GPD_ABCDE('R', 'E', 'P', 'I', 'C'),

	gpeALFA_RESET = GPD_ABCDE('R', 'E', 'S', 'E', 'T'),

	gpeALFA_RIGHT = GPD_ABCDE('R', 'I', 'G', 'H', 'T'),
	gpeALFA_RULES = GPD_ABCDE('R', 'U', 'L', 'E', 'S'),
	
	gpeALFA_SETUP = GPD_ABCDE('S', 'E', 'T', 'U', 'P'),
	gpeALFA_SHARE = GPD_ABCDE('S', 'H', 'A', 'R', 'E'),
	gpeALFA_SHOES = GPD_ABCDE('S', 'H', 'O', 'E', 'S'),
	gpeALFA_SLEFT = GPD_ABCDE('S', 'L', 'E', 'F', 'T'),
	gpeALFA_STORE = GPD_ABCDE('S', 'T', 'O', 'R', 'E'),

	gpeALFA_STACK = GPD_ABCDE('S', 'T', 'A', 'C', 'K'),
	gpeALFA_STONE = GPD_ABCDE('S', 'T', 'O', 'N', 'E'),
	gpeALFA_TABWH = GPD_ABCDE('T', 'A', 'B', 'W', 'H'),
	gpeALFA_TABXY = GPD_ABCDE('T', 'A', 'B', 'X', 'Y'),
	gpeALFA_THISA = GPD_ABCDE('T', 'H', 'I', 'S', 'A'),
	gpeALFA_THISN = GPD_ABCDE('T', 'H', 'I', 'S', 'N'),
	gpeALFA_TOKEN = GPD_ABCDE('T', 'O', 'K', 'E', 'N'),
	gpeALFA_TOUCH = GPD_ABCDE('T', 'O', 'U', 'C', 'H'),
	gpeALFA_TRACK = GPD_ABCDE('T', 'R', 'A', 'C', 'K'),
	gpeALFA_VARIA = GPD_ABCDE('V', 'A', 'R', 'I', 'A'),

	gpeALFA_UNDEF = GPD_ABCDE('U', 'N', 'D', 'E', 'F'),

	gpeALFA_UNSEL = GPD_ABCDE('U', 'N', 'S', 'E', 'L'),

	gpeALFA_WMLBX = GPD_ABCDE('W', 'M', 'L', 'B', 'X'),		// egér X poziciója mielött le lett nyomva a BAL gomb
	gpeALFA_WMLBY = GPD_ABCDE('W', 'M', 'L', 'B', 'Y'),		// egér Y poziciója mielött le lett nyomva a BAL gomb

	gpeALFA_WMRBX = GPD_ABCDE('W', 'M', 'R', 'B', 'X'),		// egér X poziciója mielött le lett nyomva a JOBB gomb
	gpeALFA_WMRBY = GPD_ABCDE('W', 'M', 'R', 'B', 'Y'),		// egér Y poziciója mielött le lett nyomva a JOBB gomb
	gpeALFA_WRITE = GPD_ABCDE('W', 'R', 'I', 'T', 'E'),

	gpeALFA_5 = GPD_ABCDE('Z', 'Z', 'Z', 'Z', 'Z'),

	//- 6 -----------------
	gpeALFA_AAAAAA = GPD_ABCDEF('A', 'A', 'A', 'A', 'A', 'A'),
	gpeALFA_ACCEPT = GPD_ABCDEF('A', 'C', 'C', 'E', 'P', 'T'),
	gpeALFA_ABOARD = GPD_ABCDEF('A', 'B', 'O', 'A', 'R', 'D'),
	gpeALFA_BUBLER = GPD_ABCDEF('B', 'U', 'B', 'L', 'E', 'R'),
	gpeALFA_ELAPSE = GPD_ABCDEF('E', 'L', 'A', 'P', 'S', 'E'),
	gpeALFA_FERIKE = GPD_ABCDEF('F', 'E', 'R', 'I', 'K', 'E'),
	gpeALFA_FLINCH = GPD_ABCDEF('F', 'L', 'I', 'N', 'C', 'H'),
	gpeALFA_FORBID = GPD_ABCDEF('F', 'O', 'R', 'B', 'I', 'D'),
	gpeALFA_GALAXY = GPD_ABCDEF('G', 'A', 'L', 'A', 'X', 'Y'),
	gpeALFA_JACKET = GPD_ABCDEF('J', 'A', 'C', 'K', 'E', 'T'),
	gpeALFA_KSHIFT = GPD_ABCDEF('K', 'S', 'H', 'I', 'F', 'T'),
	gpeALFA_LISTEN = GPD_ABCDEF('L', 'I', 'S', 'T', 'E', 'N'),
	gpeALFA_MODBUS = GPD_ABCDEF('M', 'O', 'D', 'B', 'U', 'S'),
	gpeALFA_NBUILD = GPD_ABCDEF('N', 'B', 'U', 'I', 'L', 'D'),
	gpeALFA_PICCPY = GPD_ABCDEF('P', 'I', 'C', 'C', 'P', 'Y'),
	gpeALFA_PLANET = GPD_ABCDEF('P', 'L', 'A', 'N', 'E', 'T'),
	gpeALFA_POLYER = GPD_ABCDEF('P', 'O', 'L', 'Y', 'E', 'R'),
	gpeALFA_PRAGMA = GPD_ABCDEF('P', 'R', 'A', 'G', 'M', 'A'),
	gpeALFA_REGGIO = GPD_ABCDEF('R', 'E', 'G', 'G', 'I', 'O'),
	gpeALFA_RENAME = GPD_ABCDEF('R', 'E', 'N', 'A', 'M', 'E'),
	//gpeALFA_SHADER =	GPD_ABCDEF( 'S','H','A','D','E','R' ),
	
	gpeALFA_SHADOW = GPD_ABCDEF('S', 'H', 'A', 'D', 'O', 'W'),
	gpeALFA_SPRITE = GPD_ABCDEF('S', 'P', 'R', 'I', 'T', 'E'),
	gpeALFA_SRIGHT = GPD_ABCDEF('S', 'R', 'I', 'G', 'H', 'T'),
	gpeALFA_STEREO = GPD_ABCDEF('S', 'T', 'E', 'R', 'E', 'O'),
	gpeALFA_STOREE = GPD_ABCDEF('S', 'T', 'O', 'R', 'E', 'E'),
	gpeALFA_STOWUP = GPD_ABCDEF('S', 'T', 'O', 'W', 'U', 'P'),
	gpeALFA_STRROW = GPD_ABCDEF('S', 'T', 'R', 'R', 'O', 'W'),
	gpeALFA_TARGET = GPD_ABCDEF('T', 'A', 'R', 'G', 'E', 'T'),
	gpeALFA_TELNET = GPD_ABCDEF('T', 'E', 'L', 'N', 'E', 'T'),

	gpeALFA_6 = GPD_ABCDEF('Z', 'Z', 'Z', 'Z', 'Z', 'Z'),

	//- 7 -----------------

	gpeALFA_ACCOUNT = GPD_ABCDEFG('A', 'C', 'C', 'O', 'U', 'N', 'T'),
	gpeALFA_CAMLIST = GPD_ABCDEFG('C', 'A', 'M', 'L', 'I', 'S', 'T'),
	gpeALFA_CONNECT = GPD_ABCDEFG('C', 'O', 'N', 'N', 'E', 'C', 'T'),

	gpeALFA_FAVICON = GPD_ABCDEFG('F', 'A', 'V', 'I', 'C', 'O', 'N'),
	
	gpeALFA_GIOFILE = GPD_ABCDEFG('G', 'I', 'O', 'F', 'I', 'L', 'E'),
	gpeALFA_GRIPPER = GPD_ABCDEFG('G', 'R', 'I', 'P', 'P', 'E', 'R'),
	gpeALFA_NOBUILD = GPD_ABCDEFG('N', 'O', 'B', 'U', 'I', 'L', 'D'),
	gpeALFA_NRENDER = GPD_ABCDEFG('N', 'R', 'E', 'N', 'D', 'E', 'R'),
	gpeALFA_PICCPYE = GPD_ABCDEFG('P', 'I', 'C', 'C', 'P', 'Y', 'E'),
	gpeALFA_PICLIST = GPD_ABCDEFG('P', 'I', 'C', 'L', 'I', 'S', 'T'),
	gpeALFA_REFERER = GPD_ABCDEFG('R', 'E', 'F', 'E', 'R', 'E', 'R'),
	gpeALFA_RESTART = GPD_ABCDEFG('R', 'E', 'S', 'T', 'A', 'R', 'T'),
	//gpeALFA_INCLUDE =	GPD_ABCDEFG( 'I','N','C','L','U','D','E' ),
	gpeALFA_STEREOO = GPD_ABCDEFG('S', 'T', 'E', 'R', 'E', 'O', 'O'),
	gpeALFA_TROUSET = GPD_ABCDEFG('T', 'R', 'O', 'U', 'S', 'E', 'T'),
	gpeALFA_UPGRADE = GPD_ABCDEFG('U', 'P', 'G', 'R', 'A', 'D', 'E'),
	gpeALFA_VEHICLE = GPD_ABCDEFG('V', 'E', 'H', 'I', 'C', 'L', 'E'),
	gpeALFA_WINMOVE = GPD_ABCDEFG('W', 'I', 'N', 'M', 'O', 'V', 'E'),
	gpeALFA_7 = GPD_ABCDEFG('Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z'),

	//- 8 -----------------
	gpeALFA_BONELIST = GPD_ABCDEFGH('B', 'O', 'N', 'E', 'L', 'I', 'S', 'T'),
	gpeALFA_GATELIST = GPD_ABCDEFGH('G', 'A', 'T', 'E', 'L', 'I', 'S', 'T'),
	gpeALFA_GIOLAYER = GPD_ABCDEFGH('G', 'I', 'O', 'L', 'A', 'Y', 'E', 'R'),

	gpeALFA_GPICBGRA = GPD_ABCDEFGH('G', 'P', 'I', 'C', 'B', 'G', 'R', 'A'),


	gpeALFA_GXBULLET = GPD_ABCDEFGH('G', 'X', 'B', 'U', 'L', 'L', 'E', 'T'),

	gpeALFA_GXBUYFTO = GPD_ABCDEFGH('G', 'X', 'B', 'U', 'Y', 'F', 'T', 'O'),
	gpeALFA_GXBUYGPX = GPD_ABCDEFGH('G', 'X', 'B', 'U', 'Y', 'G', 'P', 'X'),
	gpeALFA_GXBUYGRN = GPD_ABCDEFGH('G', 'X', 'B', 'U', 'Y', 'G', 'R', 'N'),
	gpeALFA_GXBUYPNG = GPD_ABCDEFGH('G', 'X', 'B', 'U', 'Y', 'P', 'N', 'G'),
	gpeALFA_GXBUYPNR = GPD_ABCDEFGH('G', 'X', 'B', 'U', 'Y', 'P', 'N', 'R'),
	gpeALFA_GXBUYTIM = GPD_ABCDEFGH('G', 'X', 'B', 'U', 'Y', 'T', 'I', 'M'),
	gpeALFA_GXBUYVOX = GPD_ABCDEFGH('G', 'X', 'B', 'U', 'Y', 'V', 'O', 'X'),

	gpeALFA_GXDDDDST = GPD_ABCDEFGH('G', 'X', 'D', 'D', 'D', 'D', 'S', 'T'),
	gpeALFA_GXDDDGET = GPD_ABCDEFGH('G', 'X', 'D', 'D', 'D', 'G', 'E', 'T'),
	gpeALFA_GXDDDGPX = GPD_ABCDEFGH('G', 'X', 'D', 'D', 'D', 'G', 'P', 'X'),

	gpeALFA_GXDDDLST = GPD_ABCDEFGH('G', 'X', 'D', 'D', 'D', 'L', 'S', 'T'),
	gpeALFA_GXDDDRST = GPD_ABCDEFGH('G', 'X', 'D', 'D', 'D', 'R', 'S', 'T'),

	gpeALFA_GXDDDPNG = GPD_ABCDEFGH('G', 'X', 'D', 'D', 'D', 'P', 'N', 'G'),	// gx_id, size_3d, div_3d, sizeof(a_dddpng), (GPCI4)near_xyz
	gpeALFA_GXDDDRDY = GPD_ABCDEFGH('G', 'X', 'D', 'D', 'D', 'R', 'D', 'Y'),

	// GIV
	gpeALFA_GXGIVDST = GPD_ABCDEFGH('G', 'X', 'G', 'I', 'V', 'D', 'S', 'T'),
	gpeALFA_GXGIVGET = GPD_ABCDEFGH('G', 'X', 'G', 'I', 'V', 'G', 'E', 'T'),
	gpeALFA_GXGIVGPX = GPD_ABCDEFGH('G', 'X', 'G', 'I', 'V', 'G', 'P', 'X'),
	gpeALFA_GXGIVLST = GPD_ABCDEFGH('G', 'X', 'G', 'I', 'V', 'L', 'S', 'T'),
	gpeALFA_GXGIVPUB = GPD_ABCDEFGH('G', 'X', 'G', 'I', 'V', 'P', 'U', 'B'),
	gpeALFA_GXGIVRDY = GPD_ABCDEFGH('G', 'X', 'G', 'I', 'V', 'R', 'D', 'Y'),
	gpeALFA_GXGIVRST = GPD_ABCDEFGH('G', 'X', 'G', 'I', 'V', 'R', 'S', 'T'),
			 
			 
	gpeALFA_GXIABOVR = GPD_ABCDEFGH('G', 'X', 'I', 'A', 'B', 'O', 'V', 'R'),
	gpeALFA_GXIABTIM = GPD_ABCDEFGH('G', 'X', 'I', 'A', 'B', 'T', 'I', 'M'),
	gpeALFA_GXIABTUR = GPD_ABCDEFGH('G', 'X', 'I', 'A', 'B', 'T', 'U', 'R'),
	gpeALFA_GXIACTID = GPD_ABCDEFGH('G', 'X', 'I', 'A', 'C', 'T', 'I', 'D'), // akció azonosító
	gpeALFA_GXIACTIM = GPD_ABCDEFGH('G', 'X', 'I', 'A', 'C', 'T', 'I', 'M'), // action (animáció) idõ kezdete
	gpeALFA_GXIMOMID = GPD_ABCDEFGH('G', 'X', 'I', 'M', 'O', 'M', 'I', 'D'),
	gpeALFA_GXISIZER = GPD_ABCDEFGH('G', 'X', 'I', 'S', 'I', 'Z', 'E', 'R'),
	gpeALFA_GXITAVEC = GPD_ABCDEFGH('G', 'X', 'I', 'T', 'A', 'V', 'E', 'C'),
	gpeALFA_GXITBORN = GPD_ABCDEFGH('G', 'X', 'I', 'T', 'B', 'O', 'R', 'N'),
	gpeALFA_GXITBVEC = GPD_ABCDEFGH('G', 'X', 'I', 'T', 'B', 'V', 'E', 'C'),
	gpeALFA_GXITEMID = GPD_ABCDEFGH('G', 'X', 'I', 'T', 'E', 'M', 'I', 'D'), // elem egyedi azonosító
	gpeALFA_GXITMPOS = GPD_ABCDEFGH('G', 'X', 'I', 'T', 'M', 'P', 'O', 'S'),
	gpeALFA_GXITMYPR = GPD_ABCDEFGH('G', 'X', 'I', 'T', 'M', 'Y', 'P', 'R'), // xy YAW PITCH ROLL float
	gpeALFA_GXITNAME = GPD_ABCDEFGH('G', 'X', 'I', 'T', 'N', 'A', 'M', 'E'),
	gpeALFA_GXITPACK = GPD_ABCDEFGH('G', 'X', 'I', 'T', 'P', 'A', 'C', 'K'), // elem block

	gpeALFA_GXITPLAY = GPD_ABCDEFGH('G', 'X', 'I', 'T', 'P', 'L', 'A', 'Y'),

	gpeALFA_GXITSTAR = GPD_ABCDEFGH('G', 'X', 'I', 'T', 'S', 'T', 'A', 'R'),
	gpeALFA_GXITTYPE = GPD_ABCDEFGH('G', 'X', 'I', 'T', 'T', 'Y', 'P', 'E'),
	gpeALFA_GXITYVEC = GPD_ABCDEFGH('G', 'X', 'I', 'T', 'Y', 'V', 'E', 'C'), // YZ Y VEC folat
	gpeALFA_GXITZVEC = GPD_ABCDEFGH('G', 'X', 'I', 'T', 'Z', 'V', 'E', 'C'), // YZ Z VEC float
	gpeALFA_GXITYZTM = GPD_ABCDEFGH('G', 'X', 'I', 'T', 'Y', 'Z', 'T', 'M'), // YZ tengelyek bázis ideje
	gpeALFA_GXMAINTM = GPD_ABCDEFGH('G', 'X', 'M', 'A', 'I', 'N', 'T', 'M'), // server idõ
	gpeALFA_IDXCOUNT = GPD_ABCDEFGH('I', 'D', 'X', 'C', 'O', 'U', 'N', 'T'),
	gpeALFA_IDXSTART = GPD_ABCDEFGH('I', 'D', 'X', 'S', 'T', 'A', 'R', 'T'),
	gpeALFA_INTERNET = GPD_ABCDEFGH('I', 'N', 'T', 'E', 'R', 'N', 'E', 'T'),
	gpeALFA_NLAYALOC = GPD_ABCDEFGH('N', 'L', 'A', 'Y', 'A', 'L', 'O', 'C'),
	gpeALFA_NSURFPIC = GPD_ABCDEFGH('N', 'S', 'U', 'R', 'F', 'P', 'I', 'C'),
	gpeALFA_PARTSURF = GPD_ABCDEFGH('P', 'A', 'R', 'T', 'S', 'U', 'R', 'F'),
	gpeALFA_PICSFILE = GPD_ABCDEFGH('P', 'I', 'C', 'S', 'F', 'I', 'L', 'E'),
	gpeALFA_PLAYERID = GPD_ABCDEFGH('P', 'L', 'A', 'Y', 'E', 'R', 'I', 'D'),
	gpeALFA_PLAYNAME = GPD_ABCDEFGH('P', 'L', 'A', 'Y', 'N', 'A', 'M', 'E'),
	gpeALFA_PLAYPASW = GPD_ABCDEFGH('P', 'L', 'A', 'Y', 'P', 'A', 'S', 'W'),
	gpeALFA_ROWORBIT = GPD_ABCDEFGH('R', 'O', 'W', 'O', 'R', 'B', 'I', 'T'),
	gpeALFA_RULEPACK = GPD_ABCDEFGH('R', 'U', 'L', 'E', 'P', 'A', 'C', 'K'),
	gpeALFA_SHUTDOWN = GPD_ABCDEFGH('S', 'H', 'U', 'T', 'D', 'O', 'W', 'N'),
	gpeALFA_STEREOOO = GPD_ABCDEFGH('S', 'T', 'E', 'R', 'E', 'O', 'O', 'O'),
	gpeALFA_TRIANGLE = GPD_ABCDEFGH('T', 'R', 'I', 'A', 'N', 'G', 'L', 'E'),
	gpeALFA_VRTXBASE = GPD_ABCDEFGH('V', 'R', 'T', 'X', 'B', 'A', 'S', 'E'),
	gpeALFA_VXNCOUNT = GPD_ABCDEFGH('V', 'X', 'N', 'C', 'O', 'U', 'N', 'T'),
	gpeALFA_VXPTCUFB = GPD_ABCDEFGH('V', 'X', 'P', 'T', 'C', 'U', 'F', 'B'),
	 
	gpeALFA_8 = GPD_ABCDEFGH('Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z'),

	//- 8 -----------------

	gpeALFA_STEREOOOO = GPD_ABCDEFGHI('S', 'T', 'E', 'R', 'E', 'O', 'O', 'O', 'O'),
	gpeALFA_9 = GPD_ABCDEFGHI('Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z'),
	
	//- 9 -----------------

	gpeALFA_CONNECTION = GPD_ABCDEFGHIJ('C','O','N','N','E','C','T','I','O','N'),

	gpeALFA_CRPXNLSKVLJFHG = 0x7fffffffffffffff,
} GPT_ALFA;

U1*		GPF_A_ALFA_init(void);
gpeALFA	GPF_STR2ALFA( char* p_str, char* p_end, char** pp_str = NULL );
U8		GPF_ALFA2STR( char* p_out, I8 d0, bool b_hex );

class GPCAN
{
public:
	gpeALFA	a;
	I8	n;
	GPCAN(void)
	{
		GPM_CLR; //memset( this, 0, sizeof(*this) );
	}
	GPCAN( gpeALFA _a, I8 _n )
	{
		a = _a;
		n = _n;
	}
	GPCAN( I8 _a, I8 _n )
	{
		a = (gpeALFA)_a;
		n = _n;
	}
	GPCAN( char* p_str )
	{
		GPM_CLR;
		if( !p_str )
			return;
		p_str += GPD_NINCS( p_str, " \t\r\n" );
		a = GPF_STR2ALFA( p_str, NULL, &p_str );
		n = strtoll( p_str, &p_str, 10 );
	}
	//GPCAN& an( char* p_str, int radix = 10 )
	//{
	//	GPM_CLR;
	//	if( !p_str )
	//		return *this;
	//	p_str += GPD_NINCS( p_str, " \t\r\n" );
	//	a = GPF_STR2ALFA( p_str, NULL, &p_str );
	//	n = strtoll( p_str, &p_str, radix );
	//	return *this;
	//}
	GPCAN& an( char* p_str, char** pp_str = NULL, int radix = 10 )
	{
		GPM_CLR;
		if( !p_str )
			return *this;
		p_str += GPD_NINCS( p_str, "_/ \t\r\n" );
		a = GPF_STR2ALFA( p_str, NULL, &p_str );
		n = strtoll( p_str, &p_str, radix );
		if( pp_str )
			*pp_str = p_str;
		return *this;
	}
	GPCAN& an( gpeALFA _a, I8 _n )
	{
		a = _a;
		n = _n;
		return *this;
	}
	bool operator == (const GPCAN& an ) const
	{
		return ( a == an.a && n == an.n );
	}
	bool operator != (const GPCAN& an ) const
	{
		return !(*this == an);
	}
	GPCAN plus( I8 ia, I8 in )
	{
		return GPCAN( (gpeALFA)(a+ia), n+in );
	}
	bool q_valid(void)
	{
		if( a > gpeALFA_zero || n > -1 )
			return true;

		return false;
	}
	U8 str( char* p_buff, const char* p_post = NULL, bool b_hex = false )
	{
		if( !p_buff )
			return 0;
		if( !this )
		{
			*p_buff = 0;
			return 0;
		}
		char* p_begin = p_buff;
		p_buff += GPF_ALFA2STR( p_buff, a, b_hex );
		//_strupr( p_begin );
		p_buff += sprintf( p_buff, b_hex ? "%llx": "%lld", max(n,0) );
		if( p_post )
			p_buff += sprintf( p_buff, "%s", p_post );
		return p_buff-p_begin;
	}
};


class GPC_LAZY_DICT
{
	U44*		pIX;
public:
	GPC_LAZY	str,
				ix;
	U8			ver;

	GPC_LAZY_DICT(){};
	GPC_LAZY_DICT(U8 i)
	{
		GPM_CLR;
	}
	GPC_LAZY_DICT* dict_add( char* p_str, U8 n )
	{
		if( !this )
		{
			GPC_LAZY_DICT* p_this = new GPC_LAZY_DICT(0);

			return p_this->dict_add( p_str, n );
		}
		U8 aSTRT[2]; // = -1;
		if( !str.p_alloc )
			ver = 0;
		str.lazy_add( p_str, n+1, aSTRT[0] = -1 );
		char* p_str0 = ((char*)str.p_alloc);
		p_str = p_str0+aSTRT[0];
		if( p_str[n] )
			p_str[n] = 0;

		ix.lazy_add( NULL, sizeof(U44), aSTRT[1] = -1 );
		U4 nIX = (aSTRT[1]/sizeof(U44));
		U44	*p_ix0 = ((U44*)ix.p_alloc);

		pIX = p_ix0 + nIX;
		pIX->null();
		pIX->x = aSTRT[0];
		pIX->y = n;
		U4 iIX, nADD = p_ix0->dict_add( p_str0, iIX, *pIX );

		if( nIX == nADD )
		{
			str.n_load = aSTRT[0];
			ix.n_load = aSTRT[1];
			pIX = p_ix0+iIX;
			return this;
		}
		ver++;
		pIX = p_ix0+nIX;
		return this;
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
		return ix.n_load / sizeof(U44);
	}
	char* sSTRix( U8 iX )
	{
		U44	*p_ix0 = ((U44*)ix.p_alloc);
		return (char*)(str.p_alloc + p_ix0[iX].x);
	}
};

class GPCMINI : public I44
{
public:
	U14* p_alloc = NULL, a_color[4];
	char *pBUFF;
	U4 n_alloc = 0, iBUFF = 0;
	//I44 mini_ss_size = 0;
	U44	mini_y = 0, mini_dim = 0, mini_n = 0, mini_u4, aPS[4];
	I44	crs_pos, crs_frm;
	U8	nLAST, nVER;
	
	~GPCMINI( void )
	{
		GPM_DELARRAY( p_alloc );
	}

	GPCMINI( void )
	{
		GPM_CLR;
		a_color[0] = -1;
	}
	
	GPCMINI* resize( const I44& resiz )
	{
		if( !this )
		{
			GPCMINI* p_this = new GPCMINI;
			if( !p_this )
				return NULL;

			return p_this->resize( resiz );
		}
		if( zw() == resiz.zw() )
			return this;
		
		nLAST = nVER;
		zw( resiz.zw() );
		crs_pos.xy( 0 );
		crs_pos.zw( xy(zw()/8) );
		crs_frm = crs_pos; 
		U4 n_area = area_xy();
		if( mini_n.x != n_area )
		{
			GPM_DELARRAY( p_alloc );
			mini_n = n_area;
								
			mini_dim.xy( xy() );
			mini_y.x = 0;					// txt
			mini_dim.zw( mini_dim.xy() );	// txt

			mini_y.y = mini_dim.w;			// frame
			mini_dim.w += mini_dim.y;		// frame

			mini_y.z = mini_dim.w;			// big
			mini_dim.w += mini_dim.y/2;		// big
				
			mini_y.w = mini_dim.w;			// icon
			mini_dim.w += mini_dim.y/4;		// icon

			mini_u4 = mini_y * mini_dim.z; 
			
			aPS[0] = mini_dim;
			aPS[1] = mini_y.yzw( 0 );
			 
			n_alloc = GPM_PAD( (iBUFF = mini_dim.w*mini_dim.x)+0x1000, 0x10 );
			mini_n = mini_u4.yzw( iBUFF ) - mini_u4;

			p_alloc = new U14[n_alloc];
			pBUFF = (char*)(p_alloc + iBUFF);
			GPMZn( p_alloc, n_alloc );
			nVER++;
			
		}

		return this;
	}
	I44 clr( void )
	{
		if( !this )
			return 0;

		nVER++;
		GPMZn( p_alloc, n_alloc );
		crs_pos = crs_frm;
		return crs_pos;
	}
	
	I44 print_txt( const char *p_format, ... )
	{
		if( !this )
			return 0;

		I8 o = 0;
		va_list vl;
		va_start(vl, p_format);
	
		if( (o = vsprintf( (char*)pBUFF, p_format, vl )) > 0 )
		{
			//GPF_CON_Write( gx_s_con_buff );
		
			char	*p_str = pBUFF,
					*p_se, s_ch[] = " ";
			U14		*pPLN = p_alloc, *p_c, *p_ce; 
			U4		n, n_str;
			while( *p_str )
			{
				n_str = GPD_VAN( p_str, " \t\r\n" );
				if( n_str )
				{
					p_se = p_str+n_str;
					if( crs_pos.y >= crs_frm.y && crs_pos.y < crs_frm.w )
					if( crs_pos.x >= crs_frm.x && crs_pos.x < crs_frm.z )
					{
						if( nLAST == nVER )
							nVER++;
						if( crs_pos.x < crs_frm.x )
						{
							p_str += crs_frm.x-crs_pos.x;
							crs_pos.x = crs_frm.x;
						}
				
						p_c = pPLN + crs_pos.y * mini_dim.z + crs_pos.x;
						if( n_str > crs_frm.z-crs_pos.x )
							p_ce = p_c + crs_frm.z-crs_pos.x;
						else 
							p_ce = p_c + n_str;

						GPF_MEM_set( p_c, p_ce-p_c, a_color, sizeof(*p_alloc) );
						while( p_c < p_ce )
						{
							p_c->A = *p_str++;
							p_c++;
						}
					}

					crs_pos.x += n_str;
					p_str = p_se;
				}
			
				*s_ch = *p_str;
				n = GPD_NINCS( p_str, s_ch );
				p_str += n;
				switch( *s_ch )
				{
					case '\n':
						crs_pos.y += n;
					case '\r':
						crs_pos.x = crs_frm.x;
						continue;
					case '\t':
						crs_pos.x = ( (crs_pos.x-crs_frm.x)/8 + n )*8 + crs_frm.x;
						continue;
					case ' ':
						crs_pos.x += n;
				}
			}

		} else
			GPF_CON_Write("ERROR-FW");
		va_end(vl);
		return crs_pos;
	}

	I44 print_big( I4* pSKIP, const char *p_format, ... );
	
};
#endif