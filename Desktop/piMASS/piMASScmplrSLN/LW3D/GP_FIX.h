#pragma once
#ifndef GP_FIX
#define GP_FIX

#include "GP_NEW.h"
#include "GP_ASM.h"

#define GPD_BAGxMAP		0x100
#define GPD_BAGxMAPx10	(GPD_BAGxMAP+0x10)

#define GPD_BAG			0x400
#define GPD_BAG2		(GPD_BAG*2)
#define GPD_BAG3		(GPD_BAG*3)
#define GPD_BAGx10		(GPD_BAG+0x10)
#define GPD_BAG_SAFE	GPD_BAG2 
#define GPD_BAG_CUT		4
#define GPD_BAG_BUFF	(GPD_BAG*0x80)
#define GPD_BAG4		(GPD_BAG*4)
#define GPD_BAG8		(GPD_BAG*8)
#define GPD_BAG16		(GPD_BAG*16)
#define GPD_BAG_BUFF2	(GPD_BAG_BUFF + GPD_BAG4*2)


class GPC_FIX;
class GPC_RES;
class GPC_TAB;
class GPC_CRS;
class GPC_WBx64;
class GPS_AIMOM_TRD;
class GPC_AIMOM;

typedef enum GPE_BAG:U1
{
	GPE_BAG_CPY,
	GPE_BAG_RLE,
	GPE_BAG_HISTI,
	GPE_BAG_ONE,

	GPE_BAG_DICK = 0x10,

	GPE_BAG_BIT = 0x20,
	//GPE_BAG_BIT2,

	GPE_BAG_DICK_BIT = GPE_BAG_BIT|GPE_BAG_DICK,

	GPE_BAG_BAG = 0x40,
};


class GPC_CODEx64
{
	public:
		union
		{
			struct
			{
				ULL	u8;
			};
			struct
			{
				U4	u40, u41;
			};
			struct
			{
				U2	u20, u21, u22, u23;
			};
			struct
			{
				U1	u10, u11, u12, u13, u14, u15, u16, u17;
			};
		};
	U1* decoder( U1* p_oe, U1* p_map, U1* p_s, U1* p_se, ULL n_prev )
	{
		U1	add, shift = 0, *p_out = p_oe; //, *p_4bit = p_out+GPD_BAG, *p_4be = p_4bit, add;
		U4	mask, n_bit = 0, n_in_bit = 16, n_out_bit = 0, n_2;
		gp_zero(  p_out, p_se-p_s );
		u11 = *p_s;
		p_s++;
		u10 = *p_s;
		p_s++;
		
		while( p_s <= p_se || n_out_bit < n_in_bit )
		{
			if( u11&0x80 )
			{
				shift = 2;
				mask = 0x10000;
				add = 0;
			}
			else if( u11&0x40 )
			{
				shift = 4;
				mask = 0x30000;
				add = 2;
			}
			else if( u11&0x20 )
			{
				shift = 6;
				mask = 0x70000;
				add = 2+4;
			}
			else if( u11&0x10 )
			{
				shift = 8;
				mask = 0xf0000;
				add = 2+4+8;
			}
			else if( u11&0x8 )
			{
				shift = 10;
				mask = 0x1f0000;
				add = 2+4+8+16;
			}
			else if( u11&0x4 )
			{
				shift = 12;
				mask = 0x3f0000;
				add = 2+4+8+16+32;
			}
			else if( u11&0x2 )
			{
				shift = 14;
				mask = 0x7f0000;
				add = 2+4+8+16+32+64;
			}
			else if( u11&0x1 )
			{
				shift = 16;
				mask = 0xff0000;
				add = 2+4+8+16+32+64+128;
			} else {
				break;
			}
			u40 <<= shift;
			n_bit += shift;
			n_out_bit += shift;
			while( n_bit >= 8 )
			{
				n_bit -= 8;
				if( p_s < p_se )
				{
					u41 = *p_s;
					n_in_bit += 8;
				} else {
					u41 = 0;
				}
				u41 <<= n_bit;
				u40 |= u41;
				p_s++;
			}
			*p_oe = ((u40&mask)>>16) + add;
			p_oe++;
		}
	
		if( !p_map )
			return p_oe;
		U2 e = p_oe-p_out;
		if( e == GPD_BAG )
		{
			for( U2 i = 0, j; i < GPD_BAG; i++ )
			{
				j = (i/(GPD_BAG/4)) + (i%(GPD_BAG/4))*4;
				p_oe[j] = p_map[p_out[i]];
			}
			gp_memcpy( p_out, p_oe, GPD_BAG );
		}
		else for( U2 i = 0; i < e; i++ )
		{
			p_out[i] = p_map[p_out[i]];
		}
		return p_oe;
	}
	GPC_CODEx64& operator = ( U2 u2 )
	{
		if( u2 < 2 )
		{
			u10 = u2;		// data
			u11 = 2;		// SHIFT
			u21 = 0x2 | u10;		// HEAD
			return *this;
		}
		if( u2 < 2+4 )
		{
			u10 = u2 - 2;		// data
			u11 = 4;		// SHIFT
			u21 = 0x4 | u10;		// HEAD
			return *this;
		}
		if( u2 < 2+4+8 )
		{
			u10 = u2 - (2+4);	// data
			u11 = 6;		// SHIFT
			u21 = 0x8 | u10;		// HEAD
			return *this;
		}
		if( u2 < 2+4+8+16 )
		{
			u10 = u2 - (2+4+8);	// data
			u11 = 8;		// SHIFT
			u21 = 0x10 | u10;	// HEAD
			return *this;
		}
		if( u2 < 2+4+8+16+32 )
		{
			u10 = u2 - (2+4+8+16);	// data
			u11 = 10;		// SHIFT
			u21 = 0x20 | u10;	// HEAD
			return *this;
		}
		if( u2 < 2+4+8+16+32+64 )
		{
			u10 = u2 - (2+4+8+16+32);	// data
			u11 = 12;		// SHIFT
			u21 = 0x40 | u10;	// HEAD
			return *this;
		}
		if( u2 < 2+4+8+16+32+64+128 )
		{
			u10 = u2 - (2+4+8+16+32+64);	// data
			u11 = 14;			// SHIFT
			u21 = 0x80 | u10;		// HEAD
			return *this;
		}

		u11 = 16;			// SHIFT
		u21 = 0x100;		// HEAD
		u12 = u10 = u2 - (2+4+8+16+32+64+128);	// data
		return *this;
	}
};


class GPC_IIx64
{
public:
	union
	{
		struct
		{
			I8	i,
				vi;
		};
		struct
		{
			I8	i;
			U4	u40, u41;
		};
		struct
		{
			I8	i;
			U2	u20, u21, u22, u23;
		};
		struct
		{
			I8	i;
			U1	u10, u11, u12, u13, u14, u15, u16, u17;
		};
		struct
		{
			I8		i;
			GPC_CODEx64	code;
		};
	};
	GPC_IIx64(){};

	GPC_IIx64* index( I8 n_i )
	{
		if( !this )
		{
			GPC_IIx64* p_this = new GPC_IIx64[n_i+1];
			if( !p_this )
				return NULL;

			return p_this->index( n_i );
		}

		for( I8 j = 0; j <= n_i; j++ )
		{
			this[j].i = j;
			this[j].vi = 0;
		}
		return this;
	}

	U2 code_list( U1* p_src, U2 n_src )
	{
		for( U2 i = 0; i < n_src; i++ )
		{
			this[ p_src[i] ].vi++;
		}
		median( 0x100 );
		
		U2 mx_i = 0;
		for( U2 i = 0; i < 0x100; i++ )
		{
			if( !this[i].vi )
			if( !mx_i )
				mx_i = i+1;
				
			
			this[i].code = i;
		}
		if( !mx_i )
			mx_i = 0x100;
		return mx_i;
	}

	U2 bit_encoder4( U1* p_out, U1* p_map, U1* p_src, U2 n_src )
	{
		U1 n_shift = 0, *p_oe = p_out, u1, *p_s = p_src;
		U2 n_bit = 0;
		U4 u4 = 0, cpy4;

		if( p_map )
		{
			p_src = p_out + GPD_BAG;
			if( n_src == GPD_BAG )
			{
				for( U2 i=0, j; i < GPD_BAG; i++ )
				{
					j = (i/4) + ((i%4)*(GPD_BAG/4)); 
					p_src[j] = p_map[p_s[i]]; 
				}
				
			}
			else for( U2 i=0; i < n_src; i++ )
			{
				p_src[i] = p_map[p_s[i]]; 
			}
		}
		
		p_oe = p_out;
		for( U2 i = 0; i < n_src; i++ )
		{
			u1 = p_src[i];
			
			n_shift += this[u1].u11;
			u4 <<= this[u1].u11;
			u4 |= this[u1].u21;
			//if( this[idx].u10 )
			//	u4 |= this[idx].u10;
			
			while( n_shift >= 8 )
			{
				n_bit += 8;
				n_shift -= 8;
				cpy4 = u4>>n_shift;
				*p_oe = cpy4;
				p_oe++;
			}
		}
	
		if( n_shift )
		{
			n_bit += n_shift;
			cpy4 = u4<<(8-n_shift);
			*p_oe = cpy4;
			p_oe++;
		}
		return p_oe-p_out;
	}
	
	I8	avarage( ULL n );
	I8	median( ULL n, bool b_inc = false );
	I8	deviation( ULL n, float& devi ); // szórás
};


class GPC_WORD
{
public:
	U1* p_w;
	U2 his, left, right, n_w;
	I2 n_win, n_lose, out, i;
	GPC_WORD& operator = ( GPC_WORD& b )
	{
		gp_memcpy( this, &b, sizeof(*this) );
		return *this;
	}
	GPC_WORD* n_reset( U4 n_reset )
	{
		return (GPC_WORD*)gp_zero( this, n_reset*sizeof(*this) );
	}
	I2 win( U2 n_src )
	{
		if( !his || !n_w )
			return 0;
		if( his*n_w > n_src )
			his = n_src/n_w;

		int	lose = his+n_w,
			win = his*n_w;
		
		return win-lose;
	}
	char lo_eq_hi( GPC_WORD& b )
	{
		if( !p_w )
			n_w = 0;

		if( !b.p_w )
			b.n_w = 0;

		for( U1	*p_a = p_w, *p_ae = p_a+n_w,
			*p_b = b.p_w, *p_be = p_b+b.n_w, a, b;

			(p_a < p_ae)
			|| (p_b < p_be);

			p_a++, p_b++ )
		{
			a = (p_a < p_ae) ? *p_a : 0;
			b = (p_b < p_be) ? *p_b : 0;
			
			if( a == b )
				continue;
			
			if( a > b )
				return 1;
			
			return -1;
		}
		if( n_w == b.n_w )
			return 0;
		if( n_w > b.n_w )
			return 1;

		return -1;
	}
	bool operator > ( GPC_WORD& b )
	{
		if( lo_eq_hi( b ) == 1 )
			return true;

		return false;
	}
	
	bool operator >= ( GPC_WORD& b )
	{
		switch( lo_eq_hi( b ) )
		{
			case 0:
				if( n_w < b.n_w )
					return false;
			case 1:
				return true;
		}
		return false;
	}
	bool operator < ( GPC_WORD& b )
	{
		return !(*this >= b);
	}
	bool operator <= ( GPC_WORD& b )
	{
		switch( lo_eq_hi( b ) )
		{
			case 0:
				if( n_w > b.n_w )
					return false;
			case -1:
				return true;
		}
		return false;
	}
	bool operator == ( GPC_WORD& b )
	{
		if( !lo_eq_hi( b ) )
		if( n_w == b.n_w )
				return true;

		return false;
	}
	bool operator != ( GPC_WORD& b )
	{
		return !(*this == b);
	}
	GPC_WORD* word_mom( U2 n_dick )
	{
		if( !this )
			return NULL;

		if( !n_dick )
			return this;

		if( !this[n_dick].n_w )
			return this;
		
		U2 mom = 0, next = 0; 
		while( this[n_dick] != this[next] )
		{
			mom = next;
			switch( this[n_dick].lo_eq_hi( this[mom]) )
			{
				case 0:
					if( this[n_dick].n_w == this[next].n_w )
						return this+mom;
				case -1:
					// a keresett elem kisebb, vagy egyenlõ
					if( !this[mom].left )
						return this+mom; // ha nincsen kiseb ága akkor ez az anyja
				
					// van kisebb ága megvizsgáljuk mehetünk-e arra?
					next = this[mom].left; // balra megy
					continue;
				default:
					// A keresett elem nagyobb
					if( !this[mom].right )
						return this+mom; // nincsen nagyobb tehetjük ide

					// van nagyobb ága megvizsgáljuk mehetünk-e arra?
					next = this[mom].right; // jobbra megy
			}
		}
		// az ágban hosszabb szó van feljebb kell tolni
		return this+next;
	}
	U2 word_find( GPC_WORD* p_root, U2 n_dick, U1*p_w, U1 n_w )
	{
		GPC_WORD* p_word = this+n_dick;
		p_word->his = 0;
		p_word->p_w = p_w;
		p_word->n_w = n_w;
		GPC_WORD* p_mom = word_mom( n_dick );

		if( p_mom )
		if( *p_mom == *p_word )
			return p_mom-this;

		return p_word-this;
	}
	GPC_WORD* word_find( U2 n_dick, U1*p_w, U1 n_w )
	{
		GPC_WORD* p_word = this+n_dick;
		p_word->his = 0;
		p_word->p_w = p_w;
		p_word->n_w = n_w;
		GPC_WORD* p_mom = word_mom( n_dick );

		if( p_mom )
		if( *p_mom == *p_word )
			return p_mom;
		
		return this+n_dick;
	}
	U2 word_add( GPC_WORD* p_mom, U2 n_dick )
	{
		if( p_mom )
		{
			if( !n_dick )
				return 1;

			if( *p_mom == this[n_dick] )
				return n_dick;
		} else 
			return n_dick;
		
		// uj elem azonos hosszú vagy hosszabb
		if( this[n_dick] <= *p_mom )
		{
			// értékre is kisebb vagy azonos
			if( p_mom->left )
			{
				// már van kisebb vagy azonos ág
				if( this[p_mom->left] <= this[n_dick] )
				{
					// kisebb vagy azonos ágat az új baloldalára kötjük
					this[n_dick].left = p_mom->left;
				} else {
					// ha nagyobb akkor jobbra
					this[n_dick].right = p_mom->left;
				}
			}
			p_mom->left = n_dick;
		} else {
			if( p_mom->right )
			{
				if( this[p_mom->right] <= this[n_dick] )
				{
					this[n_dick].left = p_mom->right;
				} else {
					this[n_dick].right = p_mom->right;
				}		
			}
			p_mom->right = n_dick;
		}
		
		return n_dick+1;
	}
};
struct GPS_DICK
{
	U2 idx, n_w;
};



class GPS_FILE_ASYNC
{
public:
	FILE	*p_f;
	U1	*p_alloc, *p_mem;
	ULL	n_alloc, n;
	HANDLE	h;

	void write( void )
	{
		if( !this )
			return;

		U1 *p_end = (p_mem=p_alloc)+n;
		while( p_mem < p_end )
		{
			p_mem += fwrite( p_mem, 1, p_end-p_mem, p_f );
		}
		fclose( p_f );
	}
	bool b_wait( void )
	{
		return ( p_mem-p_alloc < n );
	}
	GPS_FILE_ASYNC* cpy( char* p_cpy, ULL n_cpy )
	{
		if( n_alloc < n_cpy )
		{
			GPM_DELARRAY( p_alloc );
			p_alloc = new U1[n_cpy];
			p_mem = p_alloc;
		}
		p_mem = (U1*)gp_memcpy( p_alloc, p_cpy, n = n_cpy );
		return this;
	}
	U4 exit( void )
	{
		if( !h )
			return 0;
		WaitForSingleObject( h, INFINITE );
		DWORD exit;
		GetExitCodeThread( h, &exit );
		CloseHandle( h );
		h = 0;
		return exit;
	}
	~GPS_FILE_ASYNC()
	{
		GPM_DELARRAY( p_alloc );
	}
};

class GPC_NODE;
class GPCV3;
class GPCI2;
class GPCI4;
class GPCV4;
class GPCMX;
class GPC_COLOR4;
class GPCQ;
class GPC_CNL;
class GPC_GIO_VX32C;
class GPC3DHI;
class GPC_GX_LOD;
class GPC3D64_LOD;
class GPC_GIOx64_LAY_LOD;
class GPC_GIOx64_LAY;
class GPC_GALAXY;

#define C4 GPC_COLOR4

class U2V2
{
public:
	union
	{
		struct
		{
			U2 x, y;
		};
		struct
		{
			U2 i2, u2;
		};
		struct
		{
			U2 a_xy[2];
		};
	};
	U2V2(){};
	U2V2( int b )
	{
		*this = b;
	}
	U2V2( U2 b )
	{
		*this = b;
	}
	U2V2( U2 X, U2 Y )
	{
		x = X;
		y = Y;
	}
	U2V2( U2* p_u2 )
	{
		x = p_u2[0];
		y = p_u2[1];
	}
	

	U2V2& operator = ( U2 b )
	{
		x = y = b;
		return *this;
	}
	U2V2& operator *= ( U2 b )
	{
		if( b == 1 )
			return *this;
		x *= b;
		y *= b;
		return *this;
	}
	U2V2& operator *= ( U2V2& b )
	{
		x *= b.x;
		y *= b.y;
		return *this;
	}
	U2V2& operator /= ( U2V2& b )
	{
		x /= b.x;
		y /= b.y;
		return *this;
	}
	U2V2& operator += ( U2 b )
	{
		if( !b )
			return *this;
		x += b;
		y += b;
		return *this;
	}
	U2V2& operator -= ( U2 b )
	{
		if( !b )
			return *this;
		x -= b;
		y -= b;
		return *this;
	}
	U2V2& operator /= ( U2 b )
	{
		if( !b )
		{
			x = y = -1;
			return *this;
		} else if( b == 1 )
			return *this;
		x /= b;
		y /= b;
		return *this;
	}

	U2V2* index( U2 n_i )
	{
		if( !this )
		{
			U2V2* p_this = new U2V2[n_i+1];
			if( !p_this )
				return NULL;

			return p_this->index( n_i );
		}

		for( U2 i = 0; i < n_i; i++ )
		{
			this[i].i2 = i;
			this[i].u2 = 0;
		}
		return this;
	}
	U2V2* histi_u2( U2* p_code, U2 n_code, U2 n_w )
	{
		if( !this )
		{
			U2V2* p_this = index( n_w );
			if( !p_this )
				return NULL;

			return p_this->histi_u2( p_code, n_code, n_w );
		}

		for( U2 i = 0; i < n_code; i++ )
		{
			this[p_code[i]].u2++;
		}
		return this;
	}
	U2V2* histi_u2( U2* p_code, U2 n_code, U2 n_w, U2 low )
	{
		if( !this )
		{
			U2V2* p_this = index( n_w );
			if( !p_this )
				return NULL;

			return p_this->histi_u2( p_code, n_code, n_w, low );
		}

		for( U2 i = 0; i < n_code; i++ )
		{
			//if( p_code[i] < low )
			//	continue;

			this[p_code[i] - (p_code[i]%low)].u2++;
		}
		return this;
	}

	U2 histi( U1* p_src, U2 n_src )
	{
		for( U2 i = 0; i < n_src; i++ )
			this[p_src[i]].u2++;

		U2 mx = 0x0;
		for( U2 i = 0; i < 0x100; i++ )
		{
			if( !this[i].u2 )
				continue;
			mx = i;
		}
		return mx+1;
	}
	void histi16( U1* p_src, U2 n_src )
	{
		for( U2 i = 0; i < n_src; i++ )
		{
			this[p_src[i]&0xf].u2++;
			this[p_src[i]>>4].u2++;
		}
	}

	U2 median( U2 n, bool b_inc = false )
	{
		// b_inc == true - incrementált növekvõ sorban leszenk
		// b_inc == false - dekrementáslt csökkenõ sorban leszenk (nem definiálod akkor ez, azaz csökenõ )
		if( !this || n < 1 )
			return 0;

		if( n < 2 )
			return this->u2;

		U2	i, j, l, r;
		U2V2	*p_tree = new U2V2[n+1],
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
						if ( j+1 <= n )
							if ( p_tree[j+1].u2 < p_tree[j].u2 )
								j++; // azt jelenti, hogy két ága is volt, és a magasabb indexün kissebb volt az érték

						if( x.u2 > p_tree[j].u2 )
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
						if ( p_tree[i+1].u2 < p_tree[i].u2 )
							i++;

					if ( x.u2 > p_tree[i].u2 )
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

			return this[n/2].u2;
		}

		U2V2* p_src = p_tree+1;
		for( U2 i = 0; i < n; i++ )
		{
			this[i] = p_src[i];
		}
		GPM_DELARRAY( p_tree );
		if( n < 3 )
			return avarage( n );

		return this[n/2].u2;
	}
	U4 area( void )
	{
		return x*y;
	};
	U2 avarage( U2 n )
	{
		// vigyázz ez sorrendezi az értékeket
		if( !this || n < 1 )
			return 0;

		if( n < 2 )
		{
			return this[0].u2;
		}
		else if( n < 3 )
		{
			return (this[1].u2+this[0].u2)/2;
		}

		U4 avgr = 0;
		for( U2 j = 0; j < n; j++ ) 
			avgr += this[j].u2;

		return avgr / n;
	}
	U2 find_y( U2 f, U2 n )
	{
		U2 stp = n, i = 0, gd = n, dif = -1;
		U2V2* pV2 = this;
		while( pV2[i].y != f )
		{
			if( stp > 1 )
				stp /= 2;
			if( pV2[i].y > f )
			{
				if( dif > pV2[i].y-f )
				{
					dif = pV2[i].y-f;
					gd = i;
				}
				i -= stp;
				continue;
			}
			if( dif > f-pV2[i].y )
			{
				dif = f-pV2[i].y;
				gd = i;
			}
			i += stp;
		}
		return ( pV2[i].y == f ) ? i : gd;
	}
};



class GPCV2
{
public:
	float x, y;
	GPCV2() {};
	GPCV2(const float *);
	GPCV2(float _x, float _y = 0.0f);
	//GPCV2(const GPC_COMPLEX &C);
	//GPCV2(const std::complex<double> &C);
	GPCV2(char* p_str, char** pp_stop);
	GPCV2(U4* p_ptr, U4* p_null, U4 pitch, float f_pix, float aspect_h);
	//GPCV2(GPC_COLOR4* p_ptr, GPC_COLOR4* p_null, U4 pitch, GPCV2 pix_dim);
	GPCV2(GPCV3& v3);
	//GPCV2(GPCV3A& v3);
	//GPCV2(GPCV3A& r, GPCV3A& u, GPCV3A& v);
	//GPCV2(GPCV3A& r, GPCV3A& u, GPCV3A& v, GPCV3A& n);
	operator float* ();
	operator const float* () const;

	// assignment operators
	GPCV2& operator = (float f)
	{
		x = y = f;
		return *this;
	}
	GPCV2& str2v2(char* p_str, char* p_nincs, char** pp_str );
	GPCV2& operator = (char* p_str);
	GPCV2& operator += (const GPCV2&);
	GPCV2& operator -= (const GPCV2&);
	GPCV2& operator *= (float);
	GPCV2& operator *= (const GPCV2& v2);
	GPCV2& operator /= (float);
	GPCV2& operator /= (const GPCV2& v2);

	// unary operators
	GPCV2 operator + () const;
	GPCV2 operator - () const;

	// binary operators
	GPCV2 operator + (const GPCV2&) const;
	GPCV2 operator - (const GPCV2&) const;
	GPCV2 operator * (float) const;
	GPCV2 operator / (float) const;

	friend GPCV2 operator * (float, const GPCV2&);
	friend float operator * (const GPCV2& v1, const GPCV2& v2);
	friend GPCV2 operator & (const GPCV2& v1, const GPCV2& v2);
	friend GPCV2 operator / (const GPCV2& v1, const GPCV2& v2);

	double qlen(void) const;
	double len(void) const;
	
	bool operator == (const GPCV2&) const;
	bool operator != (const GPCV2&) const;
	bool operator < (const GPCV2&) const;
	bool operator > (const GPCV2&) const;
	bool operator <= (const GPCV2&) const;
	bool operator >= (const GPCV2&) const;
	GPCV2& null(void);
	float area(void);
	float aspect(void);
	GPCV2 vmax(const GPCV2&);
	GPCV2 vmin(const GPCV2&);
	GPCV2 len_max(const GPCV2&);
	GPCV2& xy( float x, float y );
	GPCV2& XY( char* p_c, char** pp_c );
	float xy_max(void);
	float xy_abs_max(void);
	float abs_xy_abs_max(void);
	float xy_min(void);
	GPCV2 len_min(const GPCV2&);
	GPCV2 lerp(GPCV2& v2, float f);
	GPCV2 abs(void);
	float sum(void);
	GPCV2 normalize(void);

	GPCV2	tgr(float tg) const;

	GPCV2	left(void);
	GPCV2	right(void);
	float	stretch(GPCV2& u, GPCV2& v);
	GPCV2&	stretch(GPCV3& r, GPCV3& u, GPCV3& v);
	//GPCV2&	stretch(GPCV3A& r, GPCV3A& u, GPCV3A& v);
	//GPCV2&	stretch(GPCV3A& r, GPCV3A& u, GPCV3A& v, GPCV3A& n);
	//GPCV2&	strip(GPCV3A& A, GPCV3A& C, GPCV3A& D);
	float side(const GPCV2& vnew);
	GPCV2 rotate(const float rad);
	GPCV2 clamp(const GPCV2& _min, const GPCV2& _max);
	//GPCV2 conv(const GPCI3& i1, const GPCI3& i2, const GPCI3& i3);
	//GPCV2 shpere_vs_line(GPCV4 &line, float r);
	GPCV2 shpere_vs_line(GPCV2 &p0, GPCV2 &p1, float r);
};
GPCV2* GPCF_GPCV2_MAX_GPCV2(GPCV2* &p_cut1, GPCV2* &p_cut2, float min_diff);
GPCV2* GPCF_GPCV2_MAX_GPCV2_2(GPCV2* &p_cut1, GPCV2* &p_cut2, float min_diff);

class GPCV3
{
public:
	union
	{
		struct
		{
			float x, y, z;
		};
		struct
		{
			float b, g, r;
		};
		struct
		{
			float ry, rp, rr;
		};
	};
	GPCV3(){};
	GPCV3( const float *);
	//GPCV3(const GPCV3A& v3);
	GPCV3( const D3DVECTOR&);
	//GPCV3( const GPCV4& v4 );
	GPCV3( float _x, float _y, float _z = 0.0f );
	GPCV3( const GPCV2& v2, float _z = 0.0f);
	GPCV3( char* p_str, char** pp_stop);
	GPCV3( GPC_COLOR4* p_ptr, GPC_COLOR4* p_null, U4 pitch, GPCV2& pix_dim );

	// casting
	operator float* ();
	operator const float* () const;
	operator D3DXVECTOR3* ();

	// assignment operators
	GPCV3& operator = (float f)
	{
		x = y = z = f;
		return *this;
	}
	GPCV3&	operator = ( D3DVECTOR& d3dv);
	GPCV3&	operator = ( D3DXVECTOR3& d3dxv);
	GPCV3&	operator = ( GPCQ q );

	//GPCV3&	operator = (GPCV3A& d3dxv);
	GPCV3&	operator += (const GPCV3&);
	GPCV3&	operator -= (const GPCV3&);
	GPCV3&	operator *= (float);
	GPCV3&	operator /= (float);

	GPCV3&	operator += (float);
	GPCV3&	operator += (I8);
	//GPCV3&	operator -= (float);
	//GPCV3&	operator -= (I8);
	
	GPCV3&	operator /= (const GPCV3& v3);
	GPCV3&	operator &= (const GPCV3& v3);
	//GPCV3&	operator *= ( const GPCV3& v3 );
	// unary operators
	GPCV3 operator + () const;
	GPCV3 operator - () const;

	// binary operators
	GPCV3 operator + (const GPCV3&) const;
	GPCV3 operator - (const GPCV3&) const;
	GPCV3 operator * (float) const;
	GPCV3 operator / (float) const;

	friend GPCV3 operator * (float, const GPCV3&);
	friend float operator * (const GPCV3& v1, const GPCV3& v2);
	friend GPCV3 operator & (const GPCV3& v1, const GPCV3& v2);
	friend GPCV3 operator / (const GPCV3& v1, const GPCV3& v2);
	friend GPCV3 operator ^ (const GPCV3& v1, const GPCV3& v2);

	bool operator == (const GPCV3&) const;
	bool operator != (const GPCV3&) const;
	bool operator < (const GPCV3&) const;
	bool operator > (const GPCV3&) const;
	bool operator <= (const GPCV3&) const;
	bool operator >= (const GPCV3&) const;

	float volume( void )
	{
		return x*y*z;
	}
	float	qlen(void) const;
	float	len(void) const;
	GPCV3	lerp(GPCV3& v2, float f);
	float	volume_elips(void);
	GPCV3	smoot(float elapse, float loop, float tresh);
	GPCV3	normalize(void) const
	{
		double inv = x*x + y*y + z*z;
		if( inv == 1.0f )
			return *this;
		inv = sqrt(inv);
		GPCV3 out(x / inv, y / inv, z / inv);
		return out;
	}
	GPCV3	normalize(float& out_len) const
	{
		// use: GPC_OBJ_VS_ENERGY
		// len -be megadja a vaktor hosszát
		out_len = sqrtf(x*x + y*y + z*z);
		GPCV3 out(x / out_len, y / out_len, z / out_len);
		return out;
	}
	GPCI4&	round( void );
	GPCV3&	null(void)
	{
		x = y = z = 0.0f;
		return *this;
	}
	GPCV3	abs(void);
	GPCV3	side(void);
	bool	b_have( float f )
	{
		return ( x == f || y == f || z == f );
	}
	U1	n_have( float f )
	{
		U1 n = (x == f);
		if( y == f)
			n++;
		if( z == f)
			n++;
		return n;
	}
	U1	n_equ( GPCV3& v3 )
	{
		U1 n = (x == v3.x);
		if( y == v3.y)
			n++;
		if( z == v3.z )
			n++;
		return n;
	}
	
	GPCV2 xy(void)
	{
		return GPCV2(x, y);
	}
	GPCI4 xy_i4(void);
	GPCV2 yz(void)
	{
		return GPCV2(y, z);
	}
	GPCV2 xz(void)
	{
		return GPCV2(x, z);
	}
	GPCV2 zx(void)
	{
		return GPCV2(z, x);
	}
	GPCV2 xy(float _1)
	{
		x = y = _1;
		return xy();
	}
	GPCV2 yz(float _1)
	{
		y = z = _1;
		return yz();
	}
	GPCV2 xz(float _1)
	{
		x = z = _1;
		return xz();
	}
	GPCV3&	xy( GPCV2 v2 )
	{
		x = v2.x;
		y = v2.y;
		z = 0.0f;
		return *this;
	}
	GPCV3 xyz( float _1, float _2, float _3 )
	{
		x = _1;
		y = _2;
		z = _3;
		return *this;
	}
	/*GPCV3 zxy( void )
	{
		return GPCV3( x,y,z );
	}*/
	GPCV3 zxy( void )
	{
		return GPCV3( z,x,y );
	}
	GPCV3 yzx( void )
	{
		return GPCV3( y,z,x );
	}
	GPCV3 zyx(void)
	{
		return GPCV3(z, y, x);
	}
	GPCV3 XYZ( char *p_c, char **pp_c, float mul = 1000.0f )
	{
		x = GPF_SWAP_F4( p_c )*mul;
		p_c += 4;
		
		y = GPF_SWAP_F4( p_c )*mul;
		p_c += 4;
		
		z = GPF_SWAP_F4( p_c )*mul;
		p_c += 4;

		if( pp_c )
			*pp_c = p_c;

		return *this;
	}

	GPCV3&	xyz(float _1)
	{
		x = y = z = _1;
		return *this;
	}
	GPCV2	xy(float _1, float _2)
	{
		x = _1;
		y = _2;
		return xy();
	}
	GPCV2	yz(float _1, float _2)
	{
		y = _1;
		z = _2;
		return yz(y, z);
	}
	GPCV2	xz(float _1, float _2)
	{
		x = _1;
		z = _2;
		return xz();
	}
	GPCV3& norm_rad( void )
	{
		x = acos(cos(x));
		y = acos(cos(y));
		z = acos(cos(z));
		return *this;
	}
	
	GPCV3& mad( GPCV3& mul, GPCV3& add )
	{
		x = x*mul.x+add.x;
		y = y*mul.y+add.y;
		z = z*mul.z+add.z;
		return *this;
	}
	float sum(void)
	{
		return x+y+z;
	}
	GPCV3* gr_qube( GPCI4 gr, I8 mul, I8 div, I8 l );
	GPC_GIO_VX32C* VX324_cut( GPC_GIO_VX32C* p_buff, GPC_GIO_VX32C* p_vx, GPC_GIO_VX32C* p_vxe,  GPCV3& norm );
	// xy--
	// x-z-
	// x--w
	// -yz-
	// -y-w
	// --zw
	GPCV3& swap_xy( void )
	{
		I8 s = x;
		x = y;
		y = s;
		return *this;
	}
	GPCV3& swap_xz( void )
	{
		I8 s = x;
		x = z;
		z = s;
		return *this;
	}
	GPCV3& swap_yz( void )
	{
		I8 s = y;
		y = z;
		z = s;
		return *this;
	}

	GPCI4 hilo_xyz_ix( void );
	GPCI4 hilo_xyz_abs_ix( void );
	float mini(void)
	{
		float r = x;
		if( r > y )
			r = y;
		if( r > z )
			r = z;
		return r;
	};
	float maxi(void)
	{
		float r = x;
		if( r < y )
			r = y;
		if( r < z )
			r = z;
		return r;
	};
	float min3( const GPCV3& v3 )
	{
		if( x > v3.x )
			x = v3.x;
		if( y > v3.y )
			y = v3.y;
		if( z > v3.z )
			z = v3.z;
		return mini();
	}
	float max3( const GPCV3& v3 )
	{
		if( x < v3.x )
			x = v3.x;
		if( y < v3.y )
			y = v3.y;
		if( z < v3.z )
			z = v3.z;
		return maxi();
	}
};

__declspec(align(16))  
class GPCV4
{
public:
	union
	{
		struct
		{
			float x, y, z, w;
		};
		struct
		{
			float b, g, r, a;
		};
		struct
		{
			float ry, rp, rr, rt;
		};
		struct
		{
			GPCV3 v3;
			float div3d;
		};
	};
	GPCV4(){};
	GPCV4( float _x, float _y = 0.0f, float _z = 0.0f, float _w = 0.0f )
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}
	GPCV4( float* p_f )
	{
		x = p_f[0];
		y = p_f[1];
		z = p_f[2];
		w = p_f[3];
	}
	GPCV4& operator = ( GPC_COLOR4& c4 );
	GPCV4& operator = ( float f )
	{
		x = 
		y = 
		z = 
		w = f;
		return *this;
	}

	GPCV4( GPC_COLOR4& c4 );
	GPCV4( GPCI4& i4 );
	float qlen( void )
	{
		return x*x+y*y+z*z+w*w;
	}
	float len( void )
	{
		return sqrt(x*x+y*y+z*z+w*w);
	}
	float qlen_xyz( void )
	{
		return x*x+y*y+z*z;
	}
	float len_xyz( void )
	{
		return sqrt(x*x+y*y+z*z);
	}
	GPCV4 operator * ( float f ) const
	{
		if( f == 0.0f )
			return GPCV4(0,0,0,0);
		if( f == 1.0f )
			return *this;
		GPCV4 o( f, f, f, f );
		gp_f44_mul( &o, this );
		return o;
	}
	GPCV4 operator / ( float f ) const
	{
		if( f == 1.0f )
			return *this;
		f = 1.0/f;
		GPCV4 o( f, f, f, f );
		gp_f44_mul( &o, this );
		return o;
	}
	GPCV4& operator /= ( float f )
	{
		if( f == 1.0f )
			return *this;
		x /= f;
		y /= f;
		z /= f;
		w /= f;
		return *this;
	}

	GPCV4& operator -= ( float f )
	{
		if( f == 0.0f )
			return *this;
		x -= f;
		y -= f;
		z -= f;
		w -= f;
		return *this;
	}
	GPCV4& operator -= ( I8 i8 )
	{
		if( !i8 )
			return *this;
		x -= i8;
		y -= i8;
		z -= i8;
		w -= i8;
		return *this;
	}
	GPCV4& operator += ( float f )
	{
		if( f == 0.0f )
			return *this;
		x += f;
		y += f;
		z += f;
		w += f;
		return *this;
	}
	GPCV4& operator += ( I8 i8 )
	{
		if( !i8 )
			return *this;
		x += i8;
		y += i8;
		z += i8;
		w += i8;
		return *this;
	}
	
	GPCV4& null( void )
	{
		GPM_CLR;
		return *this;
	}

	

	GPCV4& operator *= ( GPCV2& v2 )
	{
		x *= v2.x;
		y *= v2.y;
		z *= v2.x;
		w *= v2.y;
		return *this;
	}
	GPCV4& operator /= ( GPCV2& v2 )
	{
		x /= v2.x;
		y /= v2.y;
		z /= v2.x;
		w /= v2.y;
		return *this;
	}

	GPCV4& operator += ( GPCV2& v2 )
	{
		x += v2.x;
		y += v2.y;
		z += v2.x;
		w += v2.y;
		return *this;
	}
	GPCV4& operator -= ( GPCV2& v2 )
	{
		x -= v2.x;
		y -= v2.y;
		z -= v2.x;
		w -= v2.y;
		return *this;
	}
	GPCV4& operator *= ( GPCV4& v4 )
	{
		x *= v4.x;
		y *= v4.y;
		z *= v4.z;
		w *= v4.w;
		return *this;
	}
	GPCV4& operator /= (GPCV4& v4)
	{
		x /= v4.x;
		y /= v4.y;
		z /= v4.z;
		w /= v4.w;
		return *this;
	}
	GPCV4& operator *= ( float f )
	{
		if( f == 1.0f )
			return *this;
		else if( f == 0.0f )
			return null();
		x *= f;
		y *= f;
		z *= f;
		w *= f;
		return *this;
	}

	GPCV4 operator & ( GPCV2& v2 )
	{
		return GPCV4( x*v2.x, y*v2.y, z*v2.x,  w*v2.y );
	}
	GPCV4 operator / ( GPCV2& v2 )
	{
		return GPCV4( x/v2.x, y/v2.y, z/v2.x,  w/v2.y );
	}
	GPCV4 operator + ( GPCV2& v2 )
	{
		return GPCV4( x+v2.x, y+v2.y, z+v2.x,  w+v2.y );
	}
	GPCV4 operator - ( GPCV2& v2 )
	{
		return GPCV4( x-v2.x, y-v2.y, z-v2.x,  w-v2.y );
	}

	GPCV4& col4x4( const float* p_f )
	{
		x = p_f[0];
		y = p_f[4];
		z = p_f[8];
		w = p_f[12];
		return *this;
	}
	float operator * ( const GPCV4& b ) const
	{
		GPCV4 o = *this;
		gp_f44_dot( &o, &b );
		return o.x;
	}

	/*float operator * ( GPCV4& v2 )
	{
		return x*v2.x+y*v2.y+z*v2.z+w*v2.w;
	}*/
	GPCV4 operator + ( const GPCV4& b ) const
	{
		GPCV4 o = *this;
		gp_f44_add( &o, &b );
		return o;
	}
	GPCV4 operator - ( const GPCV4& b ) const
	{
		GPCV4 o = *this;
		gp_f44_sub( &o, &b );
		return o;
	}
	GPCV2 xy(void) const
	{
		return GPCV2( x,y );
	}
	
	
	GPCV4& xy( GPCV2& v2 )
	{
		x = v2.x;
		y = v2.y;
		return *this;
	}
	GPCV2 zw(void) const
	{
		return GPCV2( z,w );
	}
	GPCV4& zw( GPCV2& v2 )
	{
		z = v2.x;
		w = v2.y;
		return *this;
	}
	GPCV4 xyxy(void) const
	{
		return GPCV4( x, y, x, y );
	}
	GPCV4 zyxw(void) const
	{
		return GPCV4(z, y, x, w);
	}
	GPCV4 zwzw(void) const
	{
		return GPCV4( z, w, z, w );
	}
	GPCV3 xyz(void) const
	{
		return GPCV3( &x );
	}
	GPCV3 xyz( float* p_f )
	{
		gp_memcpy( &x, p_f, sizeof(float)*3 );
		return xyz();
	}
	GPCV4& xyz( GPCV3& v3 )
	{
		gp_memcpy( this, &v3, sizeof(v3) );
		w = 1.0f;
		return *this;
	}

	GPCV4& xy(GPCI4& v4);
	GPCV4& zw(GPCI4& v4);
	GPCV4& xyz( GPCI4& i4 );
	GPCV4& xyzw( GPCI4& i4 );
	GPCV4& xyzw( float f );

	GPCV4& operator += ( const GPCV4& b )
	{
		gp_f44_add( this, &b );
		/*x += b.x;
		y += b.y;
		z += b.z;
		w += b.w;*/
		return *this;
	}
	GPCV4& operator -= ( const GPCV4& b )
	{
		gp_f44_sub( this, &b );
		/*x -= b.x;
		y -= b.y;
		z -= b.z;
		w -= b.w;*/
		return *this;
	}
	GPCV4 rgba2smpa( void )
	{
		GPCV3	vr( 0.0f, GPD_SMP_ALT, GPD_SMP_BASE ),
			vg( GPD_SMP_GX, GPD_SMP_ALT, GPD_SMP_GZ ),
			vb( GPD_SMP_BX, GPD_SMP_ALT, GPD_SMP_BZ ),
			vc = vr*x+vg*y+vb*z;
		
		GPCV3	vm = vc;
		
		vm.y = 0.0f;

		float	m = vm.len(),
			s = acos( m > 0.0f ? vm.z/m : 0.0f );
		
		if( vm.x < 0 )
			s = PI2-s;
		
		return GPCV4( s/PI2p3,m/GPD_SMP_BASE,vc.y/GPD_SQRT3,w );
	}
	GPCV4 bgra2smpa( void )
	{
		GPCV3	vr( 0.0f, GPD_SMP_ALT, GPD_SMP_BASE ),
			vg( GPD_SMP_GX, GPD_SMP_ALT, GPD_SMP_GZ ),
			vb( GPD_SMP_BX, GPD_SMP_ALT, GPD_SMP_BZ ),
			vc = vr*r+vg*g+vb*b;
		
		GPCV3	vm = vc;
		
		vm.y = 0.0f;

		float	m = vm.len(),
			s = acos( m > 0.0f ? vm.z/m : 0.0f );
		
		if( vm.x < 0 )
			s = PI2-s;
		
		return GPCV4( s/PI2p3,m/GPD_SMP_BASE,vc.y/GPD_SQRT3,w );
	}
	GPCV4 smpa2rgba( void )
	{
		float	spi = PI2p3*x;
		GPCV3	vs( sin(spi), 0, cos(spi) ),
				vr( 0.0f, GPD_SMP_ALT, GPD_SMP_BASE ),
				vg( GPD_SMP_GX, GPD_SMP_ALT, GPD_SMP_GZ ),
				vb( GPD_SMP_BX, GPD_SMP_ALT, GPD_SMP_BZ );
			
		vs *= y*GPD_SMP_BASE;
		vs.y += z*GPD_SQRT3;
		return GPCV4( vs*vr, vs*vg, vs*vb, w );
	}
	GPCV4& s2rgba( void )
	{
		GPCV4 out;
		out.x = (
			max( 0, 1.0f-abs(x))
			+ max( 0,(1.0f-abs(x-3.0f))))*y+z;
		out.y = max( 0, 1.0f-abs(x-1.0f) )*y+z;
		out.z = max( 0, 1.0f-abs(x-2.0f) )*y+z;

		out.w = w;
		return out;
	}
	float sum(void)
	{
		return x+y+z+w;
	}

	GPCI4 i4( void );
	GPCI4 i_xyz( void );
};

class GPCF4
{
public:
	float b,g,r,a;
	GPCF4(){};
	GPCF4( float f )
	{
		b = g = r = a = f;
	}
	GPCF4( float _b, float _g, float _r, float _a  )
	{
		b = _b;
		g = _g;
		r = _r;
		a = _a;
	}
	GPCF4& operator = ( const GPCV4 &c )
	{
		gp_memcpy( this, &c, sizeof(*this) );
		return *this;
	}
	GPCF4& operator += ( const GPCV4 &c )
	{
		gp_uf44_add( this, &c );
		return *this;
	}
	GPCF4& operator += ( const GPCF4 &c )
	{
		gp_uf44_add( this, &c );
		return *this;
	}
	GPCF4& operator -= ( const GPCV4 &c )
	{
		gp_uf44_sub( this, &c );
		return *this;
	}
	GPCF4& operator -= ( const GPCF4 &c )
	{
		gp_uf44_sub( this, &c );
		return *this;
	}
	GPCF4& operator *= ( const GPCV4 &c )
	{
		gp_uf44_mul( this, &c );
		return *this;
	}
	GPCF4& operator *= ( const GPCF4 &c )
	{
		gp_uf44_mul( this, &c );
		return *this;
	}
	
	GPCF4& operator /= ( const GPCV4 &c )
	{
		gp_uf44_div( this, &c );
		return *this;
	}
	GPCF4& operator /= ( const GPCF4 &c )
	{
		gp_uf44_div( this, &c );
		return *this;
	}
	GPCF4& null( void )
	{
		GPM_CLR;
		return *this;
	}

	GPCF4& operator *= ( float f )
	{
		if( f == 1.0 )
			return *this;
		else if( f == 0.0 )
			return null();
		GPCF4 c( f );
		gp_uf44_mul( this, &c );
		return *this;
	}

	GPCF4& operator /= ( float f )
	{
		if( f == 1.0 )
			return *this;

		GPCF4 c( 1.0f/f );
		gp_uf44_mul( this, &c );
		return *this;
	}
};

class GPC_COLOR4
{
public:
	union{
		struct
		{
			U1 B, G, R, A;
		};
		struct
		{
			U1 aBGRA[4];
		};
		struct
		{
			GPESTF	stf;
			U1		lev,
					tim;
			char	ent;
		};
		struct
		{
			U4 u4;
		};
	};

	GPC_COLOR4& swap_BA(void)
	{
		U1 t = A;
		A = B;
		B = t;
		return *this;
	}
	GPC_COLOR4& swap_GA(void)
	{
		U1 t = A;
		A = G;
		G = t;
		return *this;
	}
	GPC_COLOR4& swap_RA(void)
	{
		U1 t = A;
		A = R;
		R = t;
		return *this;
	}
	GPC_COLOR4& swap_BR(void)
	{
		U1 t = R;
		R = B;
		B = t;
		return *this;
	}
	GPC_COLOR4& swap_GR(void)
	{
		U1 t = R;
		R = G;
		G = t;
		return *this;
	}
	GPC_COLOR4& swap_BG(void)
	{
		U1 t = G;
		G = B;
		B = t;
		return *this;
	}
	GPC_COLOR4 hilo_bgra_ix( void )
	{
		GPC_COLOR4	out = (U4)0x03020100; //( 2,1,0,3 );
		U1	*p_4 = (U1*)this;
		if( p_4[out.B] < p_4[out.A] )
			out.swap_BA();
		if( p_4[out.G] < p_4[out.A] )
			out.swap_GA();
		if( p_4[out.R] < p_4[out.A] )
			out.swap_RA();

		if( p_4[out.B] < p_4[out.R] )
			out.swap_BR();
		if( p_4[out.G] < p_4[out.R] )
			out.swap_GR();
		if( p_4[out.B] < p_4[out.G] )
			out.swap_BG();
		return out;
	}


	GPC_COLOR4(void)
	{
		*(U4*)&B = 0;
	};
	GPC_COLOR4( const U1 r, const U1 g, const U1 b, const U1 a )
	{
		A = a;
		R = r;
		G = g;
		B = b;
	};
	GPC_COLOR4( float r, float g, float b, float a )
	{
		A = 255.0f*a;
		R = 255.0f*r;
		B = 255.0f*b;
		G = 255.0f*g;
	};
	GPC_COLOR4( U4 u4color )
	{
		*(U4*)&B = u4color;
	}
	GPC_COLOR4( GPESTF s )
	{
		*(U4*)&B = 0;
		stf = s;
	}
	GPC_COLOR4( GPCV4& color )
	{
		// szaturál
		R = color.x > 1.0f ? 0xff : 255.0f * color.x;
		G = color.y > 1.0f ? 0xff : 255.0f * color.y;
		B = color.z > 1.0f ? 0xff : 255.0f * color.z;
		A = color.w > 1.0f ? 0xff : 255.0f * color.w;
	};
	float a( void )
	{
		return (float)A/255.0f;
	};
	float r( void )
	{
		return (float)R/255.0f;
	};
	float g( void )
	{
		return (float)G/255.0f;
	};
	float b( void )
	{
		return (float)B/255.0f;
	};

	float r( float r )
	{
		R = 255.0*r;
		return R;
	};
	float g( float g )
	{
		G = 255.0*g;
		return G;
	};
	float b( float b )
	{
		B = 255.0*b;
		return B;
	};

	U2 intens(void)
	{
		U2 r = (((B + G + R)/3)*A)/256;
		return r;
	};
	U2 maxi(void)
	{
		U2 r = B;
		if( r < G )
			r = G;
		if( r < R )
			r = R;
		r*=A;
		return r/255;
	};
	U2 mini(void)
	{
		U2 r = B;
		if( r > G )
			r = G;
		if( r > R )
			r = R;
		r*=A;
		return r/255;
	};
	U2 sum(void)
	{
		U2 r = B+G+R;
		return r;
	};
	
	U2 lut16(void)
	{
		U2	r = R >> 6,
			g = G >> 6,
			b = B >> 6,
			lut = (r+g+b+2)/3;
			
		if( r > g ) // nagyobb az r
		{
			lut |= ( r > b ) ? 12 : 4;
		}
		else if( b > g )
		{
			lut |= 4;
		}
		else if( g > r || g > b )
		{
			lut |= 8;
		}
		return lut;
	};
	

	GPCV4 v4( void )
	{
		GPCV4 color;
		color.x = r();
		color.y = g();
		color.z = b();
		color.w = a();
		return color;
	};
	GPCV3 bgr( void )
	{
		GPCV3 color;
		color.r = r();
		color.g = g();
		color.b = b();
		return color;
	};

	GPCV3 bgr2smp( void ) // spec mul plus
	{
		GPCV3	color = bgr(), out, red( 0,0,1 ), blue( 1,0,0 ), normal(GPD_1pSQRT3,GPD_1pSQRT3,GPD_1pSQRT3),
			sub = GPCV3( 0.5, 0.5, 0.5 );//normal&normal;
		
		blue -= sub;
		red -= sub;

		GPCV3	Z = red.normalize(),
			X = (blue - Z*(Z*blue)).normalize();
		float dx = -0.4f, dz = 0.8, dl = sqrt(dx*dx+dz*dz), ex = 0.6, ez = -0.3, dp = (dx/dl)*ex+(dz/dl)*ez, dpz = dz*dp, su = ez-dpz; 
		out.g = color.len()/GPD_SQRT3; // igaziból dotproduct csak leegyszerûsitve  
		color -= GPCV3(out.g,out.g,out.g);
		out.r = color.len();
		color /= out.r;
		out.b = Z*color;
		out.b = acos(out.b);
		if( X*color < 0.0f )
			out.b = PI2-out.b;
		out.b /= PI2/3.0f;
		return out;
	};

	GPC_COLOR4 operator = ( GPESTF s )
	{
		stf = s;
		return *this;
	}

	GPC_COLOR4 operator = ( U1 i )
	{
		u4 = 0x1010101*i;
		return *this;
	}
	GPC_COLOR4 operator <<= ( U1 i )
	{
		B <<= i;
		G <<= i;
		R <<= i;
		A <<= i;
		return *this;
	}
	GPC_COLOR4 operator >>= (U1 i)
	{
		B >>= i;
		G >>= i;
		R >>= i;
		A >>= i;
		return *this;
	}
	GPC_COLOR4 operator * (U1 i)
	{
		B *= i;
		G *= i;
		R *= i;
		A *= i;
		return *this;
	}
	//GPC_COLOR4& u4( U4 _u4 )
	//{
	//	u4 = _u4;
	//	return *this;
	//}
	GPC_COLOR4 operator *= ( GPC_COLOR4 b )
	{
		B = ((I8)B*(I8)A*(I8)b.B)/255;		
		G = ((I8)G*(I8)A*(I8)b.G)/255;		
		R = ((I8)R*(I8)A*(I8)b.R)/255;		
		A = ((I8)A*(I8)A*(I8)b.A)/255;		
		return *this;
	}
	
	GPC_COLOR4 operator *= ( I8 i8 )
	{
		I8 ib = ((I8)B*(I8)A*i8)/0xff;//ff; 
		I8 ig = ((I8)G*(I8)A*i8)/0xff;//ff;
		I8 ir = ((I8)R*(I8)A*i8)/0xff;//ff;
		I8 ia = (I8)A*i8;
		
		if( ib > 0xff )
			ib = 0xff;
		if( ig > 0xff )
			ig = 0xff;
		if( ir > 0xff )
			ir = 0xff;
		if( ia > 0xff )
			ia = 0xff;
		
		B = ib; //255;		
		G = ig; ///255;		
		R = ir; ///255;		
		A = ia; ///255;		
		return *this;
	}
	GPC_COLOR4 operator &= ( I8 i8 )
	{
		// be zsugorítja 0 és i8 közé
		I8 ib = (I8)B*i8/0xff; 
		I8 ig = (I8)G*i8/0xff;
		I8 ir = (I8)R*i8/0xff;
		I8 ia = (I8)A*i8/0xff;
		
		if( ib > 0xff )
			ib = 0xff;
		if( ig > 0xff )
			ig = 0xff;
		if( ir > 0xff )
			ir = 0xff;
		if( ia > 0xff )
			ia = 0xff;
		
		B = ib; //255;		
		G = ig; ///255;		
		R = ir; ///255;		
		A = ia; ///255;		
		return *this;
	}
	GPC_COLOR4 operator /= ( I8 i8 )
	{
		if( i8 == 0 )
			return GPC_COLOR4( (U1)0xff, 0xff, 0xff, 0xff );
		B = (I8)B/i8;		
		G = (I8)G/i8;		
		R = (I8)R/i8;		
		A = (I8)A/i8;		
		return *this;
	}
	GPC_COLOR4& operator += (const GPC_COLOR4& i4)
	{
		U4 sum = B;
		sum += i4.B;
		if (sum > 255)
			B = 255;
		else
			B = sum;

		sum = G;
		sum += i4.G;
		if (sum > 255)
			G = 255;
		else
			G = sum;

		sum = R;
		sum += i4.R;
		if (sum > 255)
			R = 255;
		else
			R = sum;

		sum = A;
		sum += i4.A;
		if (sum > 255)
			A = 255;
		else
			A = sum;

		return *this;
	}

	bool operator == ( const GPC_COLOR4 &c ) const
	{
		return *(U4*)this == *(U4*)&c;
		//return ( A == c.A && R == c.R && G == c.G && B == c.B );
	}
	bool operator != ( const GPC_COLOR4 &c ) const
	{
		return !(*this == c);
	}
	GPC_COLOR4 operator & ( GPC_COLOR4 b ) const
	{
		return GPC_COLOR4( (U1)(R&b.R), G&b.G, B&b.B, A&b.A );
	}
	GPC_COLOR4 operator > ( GPC_COLOR4 b ) const
	{
		return GPC_COLOR4( (U1)((R>b.R)*255), (U1)((G>b.G)*255), (U1)((B>b.B)*255), (U1)((A>b.A)*255) );
	}
	GPC_COLOR4 operator >> ( GPC_COLOR4 b ) const
	{
		return GPC_COLOR4( (U1)(R>>b.R), (U1)(G>>b.G), (U1)(B>>b.B), (U1)(A>>b.A) );
	}
	GPC_COLOR4& operator -= ( GPC_COLOR4 b )
	{
		int ib = abs((int(B&A) - int(b.B&b.A))); //+128;
		int ig = abs((int(G&A) - int(b.G&b.A))); //+128;
		int ir = abs((int(R&A) - int(b.R&b.A))); //+128;
		int ia = A-b.A;
		
		if( ib < 0 )
			ib = 0;
		if( ig < 0 )
			ig = 0;
		if( ir < 0 )
			ir = 0;
		if( ia < 0 )
			ia = 0;
		B = ib;
		G = ig;
		R = ir;
		A = ia;
		return *this;
	}
	GPC_COLOR4 operator - ( GPC_COLOR4 b ) const
	{
		int ib = abs((int(B&A) - int(b.B&b.A))); //+128;
		int ig = abs((int(G&A) - int(b.G&b.A))); //+128;
		int ir = abs((int(R&A) - int(b.R&b.A))); //+128;
		int ia = A|b.A;
		
		if( ib < 0 )
			ib = 0;
		if( ig < 0 )
			ig = 0;
		if( ir < 0 )
			ir = 0;
		return GPC_COLOR4( (U1)ir, (U1)ig, (U1)ib, (U1)ia );
	}
	GPC_COLOR4 operator + ( GPC_COLOR4 b ) const
	{
		int ib = (int(B&A) + int(b.B&b.A)); //+128;
		int ig = (int(G&A) + int(b.G&b.A)); //+128;
		int ir = (int(R&A) + int(b.R&b.A)); //+128;
		int ia = A|b.A;
		if( ib > 255 )
			ib = 255;
		if( ig > 255 )
			ig = 255;
		if( ir > 255 )
			ir = 255;
		return GPC_COLOR4( (U1)ir, (U1)ig, (U1)ib, (U1)ia );
	}
	GPC_COLOR4 operator / ( I8 b ) const
	{
		if( b == 0 )
			return GPC_COLOR4( (U1)0xff, 0xff, 0xff, 0xff );

		int ib = (int(B&A) / b); //+128;
		int ig = (int(G&A) / b); //+128;
		int ir = (int(R&A) / b); //+128;
		int ia = A/b;
		
		if( ib < 0 )
			ib = 0;
		if( ig < 0 )
			ig = 0;
		if( ir < 0 )
			ir = 0;
		return GPC_COLOR4( (U1)ir, (U1)ig, (U1)ib, (U1)ia );
	}
	GPC_COLOR4 operator += ( U1 b )
	{
		if( !b )
			return *this;
		int ib = (int(B&A) + b); //+128;
		int ig = (int(G&A) + b); //+128;
		int ir = (int(R&A) + b); //+128;
		int ia = A+b;
		if( ib > 255 )
			ib = 255;
		if( ig > 255 )
			ig = 255;
		if( ir > 255 )
			ir = 255;
		if( ia > 255 )
			ia = 255;
		B = ib;
		G = ig;
		R = ir;
		A = ia;
		return *this; //GPC_COLOR4( (U1)ir, (U1)ig, (U1)ib, (U1)ia );
	}
	GPC_COLOR4 operator -= ( U1 b )
	{
		if( !b )
			return *this;

		int ib = (int(B&A) - b); //+128;
		int ig = (int(G&A) - b); //+128;
		int ir = (int(R&A) - b); //+128;
		int ia = A|b;
		if( ib < 0 )
			ib = 0;
		if( ig < 0 )
			ig = 0;
		if( ir < 0 )
			ir = 0;
		if( ia < 0 )
			ia = 0;
		B = ib;
		G = ig;
		R = ir;
		A = ia;
		return *this; //GPC_COLOR4( (U1)ir, (U1)ig, (U1)ib, (U1)ia );
	}
	D3DXCOLOR dxc( void )
	{
		return D3DXCOLOR( R, G, B, A )*(1.0f/256.0f);
	}
	//U4 u4( void )
	//{
	//	return *((U4*)this);//*((U4*)&B);
	//}
	U4* bgr_histi( U4* p_histi, U4 size, U2 pic_w, bool b_center = true)
	{
		U4	n_h1 = 0x100,
			n_h4 = n_h1<<2;
		
		if( !p_histi )
		{
			p_histi = new U4[n_h4];
			if( !p_histi )
				return NULL;
		}
		gp_zero( p_histi, n_h4*sizeof(*p_histi) );
		U4	*p_hb = p_histi,
			*p_hg = p_hb+0x100,
			*p_hr = p_hg+0x100,
			*p_hbgr = p_hr+0x100,
			n_mod = b_center ? pic_w/3*2 : 0,
			n_add = pic_w-n_mod,
			n_center = size-(size/3*2),
			n_begin = n_center + (n_mod/2),
			n_cnt = 0,			
			n_cent = n_add;

		n_center -= (n_mod/2);

		for( GPC_COLOR4* p_pix = this+n_begin, *p_e = p_pix+n_center; p_pix < p_e; p_pix++ )
		{
			p_hb[p_pix->B] += p_pix->A;
			p_hg[p_pix->G] += p_pix->A; 
			p_hr[p_pix->R] += p_pix->A;
			n_cnt++;

			if( !n_mod )
				continue;

			if( n_cnt == n_cent )
			{
				n_cent += n_add;
				p_pix += n_mod-1;
			}
		}

		
		for( U4 i = 0; i < n_h1; i++ )
		{
			p_hb[i] >>= 8;
			p_hg[i] >>= 8;
			p_hr[i] >>= 8;

			p_hbgr[i] = (p_hb[i]+p_hg[i]+p_hr[i])/3;
		}

		if( !n_cnt )
			return p_histi;

		p_hb[0] += size-n_cnt;
		p_hg[0] += size-n_cnt;
		p_hr[0] += size-n_cnt;
		p_hbgr[0] += size-n_cnt;

		return p_histi;
	}
	U4* bgr_histi_xywh( U4* p_histi, U4 size, U2 pic_w, GPCI4 xywh );

	bool q_empty( ULL* p_end )
	{
		if( this ? this >= (GPC_COLOR4*)p_end : true )
			return true;
		
		for( ULL* p_ll = (ULL*)this; p_ll < p_end; p_ll++ )
		{
			if( *p_ll )
				return false;
		}
		return true;
	}

	GPC_COLOR4& c4_heal( I8 qc, bool b_free )
	{
		if( A > 0xfe )
			return *this;

		int pwr = A;
		GPE_STF stf = (GPE_STF)((B == R) ? B : R );
		switch( stf )
		{
			case GPE_STF_SOIL2:
				if( qc%6 )
					return *this;

				pwr++;
				if( b_free )
				if( pwr > 0xe0 )
				{
					stf = GPE_STF_SOIL;
					pwr = 0x80;
				}
				break;
			case GPE_STF_SOIL:
				if( qc%6 )
					break;
				pwr++; 
				if( pwr < 0x78 )
				{
					stf = GPE_STF_SOIL2;
					pwr = 0x80;
				}
				break;
			case GPE_STF_ROCK:
				if( qc%12 )
					break;
				pwr++; 
				break;
			case GPE_STF_TREE1:
			case GPE_STF_TREE2:
				if( qc%GPD_STF_HEAL_QC_TREE )
					return *this;
				pwr++; 
				break;
			case GPE_STF_WATTER:
				pwr++; 
				break;
			default:
				if( qc%4 )
					break;
				pwr++; 
				
		}
		 
		if( pwr > 0xff )
			pwr = 0xff;

		B = R = stf;
		A = pwr;
		return *this;
	}
	GPC_COLOR4 abs_dif( GPC_COLOR4& b )
	{
		GPC_COLOR4 out;
		out.B = B >= b.B ? B-b.B : b.B-B;  
		out.G = G >= b.G ? G-b.G : b.G-G;  
		out.R = R >= b.R ? R-b.R : b.R-R;
		out.A = A >= b.A ? A-b.A : b.A-A;
		return out;
	}
	I8 c4_cut_line( GPCV4* p_buff, I8 n_buff, GPCV2 pic_dim, GPCV4 p1p2, GPCI4& i_xywh, GPCV4& smpa );
	I8 c4_cut_box_y( GPCV4* p_buff, I8 n_buff, GPCV2 pic_dim, GPCV4 p1p2, GPCI4& i_xywh, GPCV4& smpa );
	I8 c4_cut_bug( GPCV4* p_buff, I8 n_buff, GPCV2 pic_dim, GPCV4 p1p2, GPCI4& i_xywh, GPCV4& smpa );
	I8 c4_cut_circle_bugO(GPCV4* p_buff, I8 n_buff, GPCV2 pic_dim, GPCV4 p1p2, GPCI4& i_xywh, GPCV4& smpa, GPC_COLOR4* p_trg);
	
	I8 c4_sample( GPCV4& res, GPCV4 pr, GPCV2 srcDIM, GPCI4& i_xywh, I8& nPIX, GPC_COLOR4* p_trg, GPCV4 dstDIM );
	
	I8 c4_tool_line_bug(GPCV4* p_buff, float szDONE, GPCV2 pic_dim, GPCV4 p1p2, GPCI4& i_xywh, GPCV4& rgba, I8 deg_low, I8 deg_hi, GPC_COLOR4* p_trg);
	I8 c4_tool_turn_bug(GPCV4* p_buff, float szDONE, GPCV2 pic_dim, GPCV4 p1p2, GPCI4& i_xywh, GPCV4& rgba, I8 deg_low, I8 deg_hi, GPC_COLOR4* p_trg);
	I8 c4_cut_turn_degree_bug_o(GPCV4* p_buff, float szDONE, GPCV2 pic_dim, GPCV4 p1p2, GPCI4& i_xywh, GPCV4& rgba, I8 deg_low, I8 deg_hi, GPC_COLOR4* p_trg);

	I8 c4_cut_circle_bug( GPCV4* p_buff, I8 n_buff, GPCV2 pic_dim, GPCV4 p1p2, GPCI4& i_xywh, GPCV4& smpa, GPC_COLOR4* p_trg );
	
	GPC_COLOR4* C4_set( U4 set, I8 n_d, I8 n_z )
	{
		if( n_d == n_z )
		{
			gp_memset_u4( this, set, n_z*n_z*n_z*sizeof(set) );
			return this;
		}
		U4	n_y = n_z*n_z,
			n_dc4 = n_d*sizeof(set);

		if( !set )
		{
			for( GPC_COLOR4* p_y = this, *p_ye = p_y + n_y*n_d;  p_y < p_ye; p_y += n_y  )
			for( GPC_COLOR4* p_z = p_y,  *p_ze = p_z + n_z*n_d;  p_z < p_ze; p_z += n_z  )
			{
				gp_zero( p_z, n_dc4 );
			}
			return this;
		}
		for( GPC_COLOR4* p_y = this, *p_ye = p_y + n_y*n_d;  p_y < p_ye; p_y += n_y  )
		for( GPC_COLOR4* p_z = p_y,  *p_ze = p_z + n_z*n_d;  p_z < p_ze; p_z += n_z  )
		{
			gp_memset_u4( p_z, set, n_dc4 );
		}
		return this;
	}
	GPC_COLOR4* C4_3D_set( U4 set, I8 n_d, I8 n_z )
	{
		if( n_d == n_z )
		{
			gp_memset_u4( this, set, n_z*n_z*n_z*sizeof(set) );
			return this;
		}
		U4	n_y = n_z*n_z,
			n_dc4 = n_d*sizeof(set);

		if( !set )
		{
			for( GPC_COLOR4* p_y = this, *p_ye = p_y + n_y*n_d;  p_y < p_ye; p_y += n_y  )
			for( GPC_COLOR4* p_z = p_y,  *p_ze = p_z + n_z*n_d;  p_z < p_ze; p_z += n_z  )
				gp_zero( p_z, n_dc4 );

			return this;
		}

		for( GPC_COLOR4* p_y = this, *p_ye = p_y + n_y*n_d;  p_y < p_ye; p_y += n_y  )
		for( GPC_COLOR4* p_z = p_y,  *p_ze = p_z + n_z*n_d;  p_z < p_ze; p_z += n_z  )
			gp_memset_u4( p_z, set, n_dc4 );
		
		return this;
	}
	GPC_COLOR4* C4_3D_half00( U1* p_out_msk, GPC_COLOR4* p_c4, char* p_mask, const I8 n_d, const I8 n_z );
	I8 c4t( void )
	{
		I8 out;
		U1 *p_u1 = (U1*)&out;
		p_u1[0] = this[0x000].tim;
		p_u1[1] = this[0x001].tim;
		p_u1[2] = this[0x010].tim;
		p_u1[3] = this[0x011].tim;
		p_u1[4] = this[0x100].tim;
		p_u1[5] = this[0x101].tim;
		p_u1[6] = this[0x110].tim;
		p_u1[7] = this[0x111].tim;
		return out;
	}
	I8 c4t( I8 in )
	{
		if( !this[0].ent )
		{
			I8
			e  = abs(this[0x001].ent);
			e += abs(this[0x010].ent);
			e += abs(this[0x011].ent);
			e += abs(this[0x100].ent);
			e += abs(this[0x101].ent);
			e += abs(this[0x110].ent);
			e += abs(this[0x111].ent);
			if( !e )
				in = 0;
		}
		U1 *p_u1 = (U1*)&in;
		I8 dif = c4t()-in;
		if( dif < min2ms*5 )
		{
			if( in ? dif > 0 : false ) 
				return in;
		}
		//if( in ? (dif > 0 ? dif < min2ms*5 : false ) : false )
		//	return in;

		this[0x000].tim = p_u1[0];
		this[0x001].tim = p_u1[1];
		this[0x010].tim = p_u1[2];
		this[0x011].tim = p_u1[3];
		this[0x100].tim = p_u1[4];
		this[0x101].tim = p_u1[5];
		this[0x110].tim = p_u1[6];
		this[0x111].tim = p_u1[7];
		return in;
	}
	I8* c2tim( I8* p_tim, I8* p_e, I8 ms );
	GPC_COLOR4& stf_cpy( GPC_COLOR4& _stf )
	{
		if( this == &_stf )
			return *this;
		U1 t = tim;
		*this = _stf;
		tim = t;
		return *this;
	}
	GPC_COLOR4& stf_mov( GPC_COLOR4& _stf, U1 dir )
	{
		if( this == &_stf )
			return *this;
		// újba átrakjuk vigyázunk az idõre // tim -et nebátsd
		stf = _stf.stf;
		lev = _stf.lev&0xf0;
		lev += 0x10;
		if( lev < 0x20 )
			lev = 0x20;
		lev |= dir&0x7;
		ent = _stf.ent;


		_stf.stf = gp_stf_air;
		_stf.lev = _stf.ent = 0;
		return *this;
	}
	GPC_COLOR4& stf_fall( GPC_COLOR4& _stf )
	{
		if( this == &_stf )
			return *this;
		// újba átrakjuk vigyázunk az idõre // tim -et nebátsd
		stf = _stf.stf;
		lev = 0x20 | (_stf.lev&0x7);
		ent = _stf.ent;
		
		if( (_stf.lev>>4) == 1 )
			return *this; // mondjuk fárol potyan nem tüntetjük el

		_stf.stf = gp_stf_air;
		_stf.lev = _stf.ent = 0;
		return *this;
	}
	GPC_COLOR4& stf_dilet( GPC_COLOR4& _stf )
	{
		if( this == &_stf )
			return *this;
		if( _stf.stf < 0x10 )
			return *this;
		// újba átrakjuk vigyázunk az idõre // tim -et nebátsd
		stf = _stf.stf;
		lev = _stf.lev&0xf0;
		lev -= 0x10;
		ent = 0x18;
		return *this; // mondjuk fárol potyan nem tüntetjük el
	}
	GPC_COLOR4& stf_grow( GPC_COLOR4& _stf )
	{
		if( this == &_stf )
			return *this;
		if( !(_stf.lev&0xf) )
			return *this;
		// újba átrakjuk vigyázunk az idõre // tim -et nebátsd
		stf = _stf.stf;
		lev = _stf.lev;
		if( lev&0xf )
			lev--;
		ent = 0x18;
		return *this; // mondjuk fárol potyan nem tüntetjük el
	}
	GPC_COLOR4* cx8hl( U4 i, GPS_AIMOM_TRD& trd, bool b_free );
	GPC_COLOR4* tim2c( I8* p_tim );
	I8 c4_heal( I8 e8_ms, I8* p_up, I8* p_dwn )
	{
		if( !e8_ms )
			return ent;
		if( !ent )
			return ent;
		I8 dir = ent > 0 ? p_dwn[stf] : p_up[stf], mov;
		if( dir <= e8_ms )
			return 0;

		I8	e2 = 0,
			e80ms = 0x80*e8_ms;
		
		mov = e80ms/dir;

		if( ent > 0 )
		{
			e2 = ent-mov;
			return e2 > 0 ? e2 : 0;
		}

		e2 = ent+mov;
		return e2 < 0 ? e2 : 0;
	}



	I8 c4x8_abs( void )
	{
		I8 dif = 0;

		dif += abs( this[0x000].ent );
		dif += abs( this[0x001].ent );
		dif += abs( this[0x010].ent );
		dif += abs( this[0x011].ent );
		dif += abs( this[0x100].ent );
		dif += abs( this[0x101].ent );
		dif += abs( this[0x110].ent );
		dif += abs( this[0x111].ent );

		return dif;
	}
	void c4x8_heal(I8 e8_ms, I8* p_up, I8* p_dw )
	{
		this[0x000].ent = this[0x000].c4_heal( e8_ms, p_up, p_dw );
		this[0x001].ent = this[0x001].c4_heal( e8_ms, p_up, p_dw );
		this[0x010].ent = this[0x010].c4_heal( e8_ms, p_up, p_dw );
		this[0x011].ent = this[0x011].c4_heal( e8_ms, p_up, p_dw );
		this[0x100].ent = this[0x100].c4_heal( e8_ms, p_up, p_dw );
		this[0x101].ent = this[0x101].c4_heal( e8_ms, p_up, p_dw );
		this[0x110].ent = this[0x110].c4_heal( e8_ms, p_up, p_dw );
		this[0x111].ent = this[0x111].c4_heal( e8_ms, p_up, p_dw );
	}
	char c4_dmge( I8& ent8, I8 dam, I8* p_shield )
	{
		I8	atk = (dam * p_shield[stf])/sec2ms,
			re = dam-atk;
		ent8 = atk+ent;

		if( ent8 > 0x7f )
			ent8 = 0x7f;
		else if( ent8 < -0x7f )
			ent8 = -0x7f;
		
		return re;
	}

	GPESTF c4_chg(	
			char ent_new,
			char* p_ent_dwn, GPESTF* p_stf_dwn,
			char* p_ent_up, GPESTF* p_stf_up
		)
	{
		if( ent_new == ent )
			return stf;

		if( ent_new-ent < 0 )
		{
			if( stf == p_stf_dwn[stf] )
				return stf;

			// lefele tart
			if( ent_new > p_ent_dwn[stf] )
				return stf;	// nem kisebb nem lépett át semmit
			
			// most lépte át a lefele határt,
			// átalakul
			return p_stf_dwn[stf];
			

		}

		if( stf == p_stf_up[stf] )
			return stf;
		
		if( ent_new < p_ent_up[stf] )
			return stf;	// kisebb nem lépett át semmit
		
		return p_stf_up[stf];
		
	}
	U2 c4_free( GPC_COLOR4* p_map, U2* p_free, char* p_lig );
	//U4 c4_heal(	
	//		U4* p_load, U2* p_free,
	//		char* p_lig, I8* p_tim, I8* p_e, I8 ms,
	//		I8* p_hl_up, I8* p_hl_up_free, char* p_hl_chg_ent_up, GPESTF* p_hl_stf_up, 
	//		I8* p_hl_dw, I8* p_hl_dw_free, char* p_hl_chg_ent_dw, GPESTF* p_hl_stf_dw
	//	);
	GPC_COLOR4* sid( U4 map_y, U4 map_wz, U1 sid )
	{
		switch( sid%6 )
		{
			case 1:
					return this + map_y;
			case 2:
					return this + map_y + map_wz;
					break;
			case 3:
					return this + map_y*2 + map_wz;
					break;
			case 4:
					return this + map_y*2 + map_wz*2;
					break;
			case 5:
					return this + map_y*3 + map_wz*2;
					break;
		}
		return this;
	}

	

};


class U2V4
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
			U2 B,G,R,A;
		};
		struct
		{
			U2 a_xyzw[4];
		};
	};
	U2V4(){};
	U2V4( char b )
	{
		*this = b;
	}
	U2V4( U2 b )
	{
		*this = b;
	}
	U2V4( int b )
	{
		*this = b;
	}
	U2V4( U2 X, U2 Y, U2 Z, U2 W )
	{
		x = X;
		y = Y;
		z = Z;
		w = W;
	}
	U2V4( U2* p_u2 )
	{
		x = p_u2[0];
		y = p_u2[1];
		z = p_u2[2];
		w = p_u2[3];
	}
	//U2V4& AQ3( U4& sum, U2* p_code, U2 n_code );

	//U2V4( I4V4& i4 );
	U2V4& operator = ( C4& b )
	{
		x = b.B;
		y = b.G;
		z = b.R;
		w = b.A;
		return *this;
	}
	/*U2V4& operator = ( U2* p_b )
	{
	x = p_b[0];
	y = p_b[1];
	z = p_b[2];
	w = p_b[3];
	return *this;
	}*/
	//U2V4& operator = ( I4V4& i4 );

	U2V4& operator = ( U2 b )
	{
		x = y = z = w = b;
		return *this;
	}
	U2V4 operator * ( U2 b )
	{
		if( !b )
			return U2V4(0);
		if( b == 1 )
			return *this;
		return U2V4(
			x * b,
			y * b,
			z * b,
			w * b
		);
	}
	U2V4 operator * ( U2V4 b )
	{
		return U2V4(
			x * b.x,
			y * b.y,
			z * b.z,
			w * b.w
		);
	}
	U2V4 operator << ( U2 b )
	{
		if( !b )
			return *this;
		return U2V4(
			x << b,
			y << b,
			z << b,
			w << b
		);
	}
	U2V4 operator << ( U2V4 b )
	{
		return U2V4(
			x << b.x,
			y << b.y,
			z << b.z,
			w << b.w
		);
	}
	U2V4 operator >> ( U2 b )
	{
		if( !b )
			return *this;
		return U2V4(
			x >> b,
			y >> b,
			z >> b,
			w >> b
		);
	}
	U2V4 operator >> ( U2V4 b )
	{
		return U2V4(
			x >> b.x,
			y >> b.y,
			z >> b.z,
			w >> b.w
		);
	}
	U2V4 operator / ( U2 b )
	{

		if( !b )
			return U2V4(-1);
		if( b == 1 )
			return *this;
		return U2V4(
			x / b,
			y / b,
			z / b,
			w / b
		);
	}
	U2V4 operator / ( U2V4 b )
	{
		return U2V4(
			b.x ? x / b.x : -1,
			b.y ? y / b.y : -1,
			b.z ? z / b.z : -1,
			b.w ? w / b.w : -1
		);
	}
	U2V4& operator *= ( U2 b )
	{
		if( b == 1 )
			return *this;
		x *= b;
		y *= b;
		z *= b;
		w *= b;
		return *this;
	}
	U2V4& operator *= ( U2V4& b )
	{
		x *= b.x;
		y *= b.y;
		z *= b.z;
		w *= b.w;
		return *this;
	}
	U2V4& operator /= ( U2V4& b )
	{
		x /= b.x;
		y /= b.y;
		z /= b.z;
		w /= b.w;
		return *this;
	}
	U2V4& operator += ( U2 b )
	{
		if( !b )
			return *this;
		x += b;
		y += b;
		z += b;
		w += b;
		return *this;
	}
	U2V4& operator -= ( U2 b )
	{
		if( !b )
			return *this;
		x -= b;
		y -= b;
		z -= b;
		w -= b;
		return *this;
	}
	U2V4& operator += ( U2V4& b )
	{
		x += b.x;
		y += b.y;
		z += b.z;
		w += b.w;
		return *this;
	}
	U2V4& operator -= ( U2V4& b )
	{
		x -= b.x;
		y -= b.y;
		z -= b.z;
		w -= b.w;
		return *this;
	}
	U2V4& operator /= ( U2 b )
	{
		if( !b )
		{
			x = y = z = w = -1;
			return *this;
		} else if( b == 1 )
			return *this;
		x /= b;
		y /= b;
		z /= b;
		w /= b;
		return *this;
	}
	U2V4 hofi( U2 c );
	U2V4 hofi( U2 c, U1 n );
	U2 t_rst( U2V2 *p_t, U2 n_t )
	{
		U2V4* p_t4 = this;
		p_t4[0] = -1;
		p_t4[0].y = 1;
		p_t4[0].x = p_t[0].i2;
		U2 n_t4 = 1, j, i;
		for( i = 1; i < n_t; i++ )
		{
			p_t4[n_t4].x = p_t[i].i2;
			j = 0;
			while( j < n_t4 )
			{
				if( p_t4[n_t4].x == p_t4[j].x )
				{
					// már benne van a fában
					p_t4[j].y++;
					break;
				}
				if( p_t4[j].x < p_t4[n_t4].x )
				{
					if( p_t4[j].w < n_t4 )
					{
						j = p_t4[j].w;
						continue;
					}
					// jobbra
					// új cucc
					p_t4[j].w = n_t4;
					p_t4[n_t4].y = 1;
					n_t4++;
					p_t4[n_t4] = -1;
					break;
				}
				if( p_t4[j].z < n_t4 )
				{
					j = p_t4[j].z;
					continue;
				}
				// bal
				// új cucc
				p_t4[j].z = n_t4;
				p_t4[n_t4].y = 1;
				n_t4++;
				p_t4[n_t4] = -1;
				break;
			}
		}
		return n_t4;
	}
	//U2 t_rst( U1 *p_t, U2 n_t )
	//{
	//	U2V4* p_t4 = this;
	//	p_t4[0] = -1;
	//	p_t4[0].y = 1;
	//	p_t4[0].x = p_t[0];
	//	U2 n_t4 = 1, j, i;
	//	p_t4[n_t4] = -1;
	//	for( i = 1; i < n_t; i++ )
	//	{
	//		p_t4[n_t4].x = p_t[i];
	//		j = 0;
	//		while( j < n_t4 )
	//		{
	//			if( p_t4[n_t4].x == p_t4[j].x )
	//			{
	//				// már benne van a fában
	//				p_t4[j].y++;
	//				break;
	//			}
	//			if( p_t4[j].x < p_t4[n_t4].x )
	//			{
	//				if( p_t4[j].w < n_t4 )
	//				{
	//					j = p_t4[j].w;
	//					continue;
	//				}
	//				// jobbra
	//				// új cucc
	//				p_t4[j].w = n_t4;
	//				p_t4[n_t4].y = 1;
	//				n_t4++;
	//				p_t4[n_t4] = -1;
	//				break;
	//			}
	//			if( p_t4[j].z < n_t4 )
	//			{
	//				j = p_t4[j].z;
	//				continue;
	//			}
	//			// bal
	//			// új cucc
	//			p_t4[j].z = n_t4;
	//			p_t4[n_t4].y = 1;
	//			n_t4++;
	//			p_t4[n_t4] = -1;
	//			break;
	//		}
	//	}
	//	return n_t4;
	//}
	U2 t_rst( U1 *p_t, U2 n_t, U2 n_t42 )
	{
		U2V4* p_t4 = this;
		p_t4[0] = -1;
		p_t4[0].y = 1;
		p_t4[0].x = p_t[0];
		U2 n_t4 = 1, j, i, ie = n_t42 ? n_t+1: n_t;
		p_t4[n_t4] = -1;
		for( i = 1; i < ie; i++ )
		{
			if( i == n_t )
				p_t4[n_t4].x = p_t4[n_t42].x;
			else
				p_t4[n_t4].x = p_t[i];
			j = 0;
			while( j < n_t4 )
			{
				if( p_t4[n_t4].x == p_t4[j].x )
				{
					// már benne van a fában
					p_t4[j].y++;
					break;
				}
				if( p_t4[j].x < p_t4[n_t4].x )
				{
					if( p_t4[j].w < n_t4 )
					{
						j = p_t4[j].w;
						continue;
					}
					// jobbra
					// új cucc
					p_t4[j].w = n_t4;
					p_t4[n_t4].y = 1;
					n_t4++;
					p_t4[n_t4] = -1;
					break;
				}
				if( p_t4[j].z < n_t4 )
				{
					j = p_t4[j].z;
					continue;
				}
				// bal
				// új cucc
				p_t4[j].z = n_t4;
				p_t4[n_t4].y = 1;
				n_t4++;
				p_t4[n_t4] = -1;
				break;
			}
		}
		return n_t4;
	}
	U2 t_rst( U2 *p_t, U2 n_t, U2 n_t42 )
	{
		U2V4* p_t4 = this;
		p_t4[0] = -1;
		p_t4[0].y = 1;
		p_t4[0].x = p_t[0];
		U2 n_t4 = 1, j, i, ie = n_t42 ? n_t+1: n_t;
		p_t4[n_t4] = -1;
		for( i = 1; i < ie; i++ )
		{
			if( i == n_t )
				p_t4[n_t4].x = p_t4[n_t42].x;
			else
				p_t4[n_t4].x = p_t[i];
			j = 0;
			while( j < n_t4 )
			{
				if( p_t4[n_t4].x == p_t4[j].x )
				{
					// már benne van a fában
					p_t4[j].y++;
					break;
				}
				if( p_t4[j].x < p_t4[n_t4].x )
				{
					if( p_t4[j].w < n_t4 )
					{
						j = p_t4[j].w;
						continue;
					}
					// jobbra
					// új cucc
					p_t4[j].w = n_t4;
					p_t4[n_t4].y = 1;
					n_t4++;
					p_t4[n_t4] = -1;
					break;
				}
				if( p_t4[j].z < n_t4 )
				{
					j = p_t4[j].z;
					continue;
				}
				// bal
				// új cucc
				p_t4[j].z = n_t4;
				p_t4[n_t4].y = 1;
				n_t4++;
				p_t4[n_t4] = -1;
				break;
			}
		}
		return n_t4;
	}
	U2V4& swap_BA(void)
	{
		U2 t = A;
		A = B;
		B = t;
		return *this;
	}
	U2V4& swap_GA(void)
	{
		U2 t = A;
		A = G;
		G = t;
		return *this;
	}
	U2V4& swap_RA(void)
	{
		U2 t = A;
		A = R;
		R = t;
		return *this;
	}
	U2V4& swap_BR(void)
	{
		U2 t = R;
		R = B;
		B = t;
		return *this;
	}
	U2V4& swap_GR(void)
	{
		U2 t = R;
		R = G;
		G = t;
		return *this;
	}
	U2V4& swap_BG(void)
	{
		U2 t = G;
		G = B;
		B = t;
		return *this;
	}
	C4 srt_BGRA_ix( void )
	{
		C4	out = (U4)0x03020100; //( 2,1,0,3 );
								  // BR,GA,BG,RA,GR,BA
		if( a_xyzw[out.B] < a_xyzw[out.R] )
			out.swap_BR();
		if( a_xyzw[out.G] < a_xyzw[out.A] )
			out.swap_GA();

		if( a_xyzw[out.B] < a_xyzw[out.G] )
			out.swap_BG();
		if( a_xyzw[out.R] < a_xyzw[out.A] )
			out.swap_RA();

		if( a_xyzw[out.G] < a_xyzw[out.R] )
			out.swap_GR();
		if( a_xyzw[out.B] < a_xyzw[out.A] )
			out.swap_BA();
		return out;
	}
	U1 srt_swap( void )
	{
		U1 swap = 0;
		C4	ix = (U4)0x03020100; //( 2,1,0,3 );
								 // BR,GA,BG,RA,GR,BA
		if( a_xyzw[ix.B] > a_xyzw[ix.R] )
		{
			swap = 1;
			ix.swap_BR();
		}
		if( a_xyzw[ix.G] > a_xyzw[ix.A] )
		{
			swap |= 2;
			ix.swap_GA();
		}
		if( a_xyzw[ix.B] > a_xyzw[ix.G] )
		{
			swap |= 4;
			ix.swap_BG();
		}
		if( a_xyzw[ix.R] > a_xyzw[ix.A] )
		{
			swap |= 8;
			ix.swap_RA();
		}
		if( a_xyzw[ix.G] > a_xyzw[ix.R] )
		{
			swap |= 0x10;
			ix.swap_GR();
		}
		if( a_xyzw[ix.B] > a_xyzw[ix.A] )
		{
			swap |= 0x20;
			ix.swap_BA();
		}

		return swap;
	}
	U1 srt_swap( C4& ix )
	{
		U1 swap = 0;
		ix = (U4)0x03020100;	//( 2,1,0,3 );
								// BR,GA,BG,RA,GR,BA
		if( a_xyzw[ix.B] > a_xyzw[ix.R] )
		{
			swap = 1;
			ix.swap_BR();
		}
		if( a_xyzw[ix.G] > a_xyzw[ix.A] )
		{
			swap |= 2;
			ix.swap_GA();
		}
		if( a_xyzw[ix.B] > a_xyzw[ix.G] )
		{
			swap |= 4;
			ix.swap_BG();
		}
		if( a_xyzw[ix.R] > a_xyzw[ix.A] )
		{
			swap |= 8;
			ix.swap_RA();
		}
		if( a_xyzw[ix.G] > a_xyzw[ix.R] )
		{
			swap |= 0x10;
			ix.swap_GR();
		}
		if( a_xyzw[ix.B] > a_xyzw[ix.A] )
		{
			swap |= 0x20;
			ix.swap_BA();
		}

		return swap;
	}
	U2 add( U2* p_src, U2& m, U2V4& w )
	{
		U2V4* p_w = this;
		U2	n_w = &w-p_w;
		if( !n_w )
			return 1;

		U2	cd = 0, e, i,
			*p_s = p_src+w.x,
			*p_d, 
			mx = 0,
			o = 0;

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
	U2 find( U2* p_src, U2V4& w )
	{
		U2V4* p_w = this;
		U2	n_w = &w-p_w;
		if( !n_w )
			return 0;

		U2	cd = 0, e, i,
			*p_s = p_src+w.x,
			*p_d, m = 0, mx = 0, o = n_w;


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

	U2 find2( U1&lg, U2& go, U2* p_src, U2V4& w )
	{
		lg = 0;
		go = 1;

		U2V4* p_w = this;
		U2	n_w = &w-p_w;
		if( !n_w )
			return 0;

		U2	cd = 0, e, i,
			*p_s = p_src+w.x,
			*p_d, m = 0, mx = 0, o = n_w;

		while( cd < n_w )
		{
			lg++;
			go <<= 1;
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
				go |= 1;
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
	U2 addFLOP( U2* p_src, U2& m, U2V4& w )
	{
		U2V4* p_w = this;
		U2	n_w = &w-p_w;
		if( !n_w )
			return 1;

		U2	cd = 0, e, i,
			*p_s = p_src+w.x,
			*p_d, 
			mx = 0,
			o = 0;

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
	U2 addFLIP( U2* p_src, U2& m, U2V4& w )
	{
		U2V4* p_w = this;
		U2	n_w = &w-p_w;
		if( n_w < 2 )
			return 2;

		U2	cd = 1, e, i, j, ji,
			*p_s = p_src+w.x,
			*p_d, 
			mx = 0,
			o = 0;

		m = 0;
		while( cd < n_w )
		{
			p_d = p_src + p_w[cd].x;
			e = p_w[cd].y;
			j = e-1;
			if( e > w.y )
				e = w.y;
			for( i = 0; i < e; i++ )
			{
				ji = j-i;
				if( p_s[i] == p_d[ji] )
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

			if( p_d[ji] < p_s[i] )
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

			if( p_d[ji] > p_s[i] )
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
	U2 addFF( U2* p_src, U2& m, U2V4& w )
	{
		U2 aW[2], aM[2];
		aW[1] = &w-this;
		U2V4* p_w = this+aW[1]+1;
		*p_w = w;

		aW[0] = addFLOP( p_src, aM[0], w );
		if( aW[0] == aW[1] )
			return aW[0];

		aW[1] = addFLIP( p_src, aM[1], *p_w );
		return aW[0]+1;
	}
	U2 findFLOP( U2* p_src, U2V4& w )
	{
		U2V4* p_w = this;
		U2	n_w = &w-p_w;
		if( !n_w )
			return 0;

		U2	cd = 0, e, i,
			*p_s = p_src+w.x,
			*p_d, m = 0, mx = 0, o = n_w;


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
	U2 findFLIP( U2* p_src, U2V4& w )
	{
		U2V4* p_w = this;
		U2	n_w = &w-p_w;
		if( !n_w )
			return 0;

		U2	cd = 1, e, i, j, ji,
			*p_s = p_src+w.x,
			*p_d, m = 0, mx = 0, o = n_w;


		while( cd < n_w )
		{
			p_d = p_src+p_w[cd].x;
			e = p_w[cd].y;
			j = e-1;
			if( e > w.y )
				e = w.y;
			for( i = 0; i < e; i++ )
			{
				ji = j-i;
				if( p_s[i] == p_d[ji] )
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

			if( p_d[ji] < p_s[i] )
			{
				if( !p_w[cd].w )
					return o;

				cd = p_w[cd].w;
				continue;
			} 

			if( p_d[ji] > p_s[i] )
				if( !p_w[cd].z )
					return o;

			cd = p_w[cd].z;
		}
		return o;
	}
	U2 findFF( U2* p_src, U2V4& w )
	{
		U2 aO[2], n_w = &w-this, an[2] = {0};
		aO[0] = findFLOP( p_src, w );
		aO[1] = findFLIP( p_src, w );
		if( aO[0] < n_w )
			an[0] = this[aO[0]].y;
		if( aO[1] < n_w )
			an[1] = this[aO[1]].y;
		return an[0] >= an[1] ? aO[0] : aO[1];
	}
	U2V4 find( U2V4& fnd )
	{
		U2V4 out;
		for( U2 s = 0, f; s < 4; s++ )
		{
			f = fnd.a_xyzw[s];
			for( U2 d = 0; d < 4; d++ )
			{
				if( f != a_xyzw[d] )
					continue;
				out.a_xyzw[s] = d;
				break;
			}
		}
		return out;
	}
	U2V4 traf_dst( C4 &ix )
	{
		U2V4 out; 
		for( U1 i = 0; i < 4; i++ )
			out.a_xyzw[ix.aBGRA[i]] = a_xyzw[i];
		return out;
	}
	U2V4 traf_src( C4 &ix )
	{
		U2V4 out; 
		for( U1 i = 0; i < 4; i++ )
			out.a_xyzw[i] = a_xyzw[ix.aBGRA[i]];
		return out;
	}
	U2V4& swap_BGRA( U1 swap )
	{
		// BR,GA,BG,RA,GR,BA
		if( swap&1 )
			swap_BR();
		if( swap&2 )
			swap_GA();
		if( swap&4 )
			swap_BG();
		if( swap&8 )
			swap_RA();
		if( swap&0x10 )
			swap_GR();
		if( swap&0x20 )
			swap_BA();
		return *this;
	}
	U2V4 lb( void )
	{
		return U2V4( (U2)log2(x),  log2(y),  log2(z),  log2(w) );
	}
	U2V4 n( void )
	{
		return U2V4(
			x = x?1:0,
			y = y?1:0,
			z = z?1:0,
			w = w?1:0
		);
	}
	U2V4 lb1( void )
	{
		return U2V4( log2(x)+1,  log2(y)+1,  log2(z)+1,  log2(w)+1 );
	}
	U4 sum( void )
	{
		return ((U4)x+y)+((U4)z+w);
	}
	U4 sum_xyz( void )
	{
		return ((U4)x+y)+(U4)z;
	}
	U4 sum_xy( void )
	{
		return ((U4)x+y);
	}
	U4 sum_yz( void )
	{
		return ((U4)z+y);
	}
	U4 sum_zw( void )
	{
		return ((U4)z+w);
	}
	U2 mx( void )
	{
		return max(x,max(y,max(z,w)));
	}
	U2 mn( void )
	{
		return min(x,min(y,min(z,w)));
	}
	U2V4& judo( void )
	{
		w -= z;
		z -= y;
		y -= x;
		return *this;
	}

	U2 find( U1* p_src, U2V4& w )
	{
		U2V4* p_w = this;
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
	U2 add( U1* p_src, U2& m, U2V4& w )
	{
		U2V4* p_w = this;
		U2	n_w = &w-p_w;
		if( !n_w )
			return 1;

		U2	cd = 0, e, i, 
			mx = 0,
			o = 0;
		U1	*p_s = p_src+w.x,
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
};


#define GPD_3D_LINE	gpe_giv_LINE		// gpe_giv_LINE->"GP_NEW.h"

#define GPD_3D_STEP_X	1
#define GPD_3D_STEP_Y	(GPD_3D_LINE*GPD_3D_LINE)
#define GPD_3D_STEP_Z	GPD_3D_LINE

#define GPD_3D_END_X		(GPD_3D_STEP_X*GPD_3D_LINE)
#define GPD_3D_END_Y		(GPD_3D_STEP_Y*GPD_3D_LINE)
#define GPD_3D_END_Z		(GPD_3D_STEP_Z*GPD_3D_LINE)
#define GPD_3D_END_YC3	(GPD_3D_END_Y*3)
#define GPD_3D_END_YC4	(GPD_3D_END_Y*4)
#define GPD_STUFF_N		(GPE_3D_END-1)
//#define GPD_3D_NCPU 2
#define GPD_x3	2
#define GPD_x8	8
#define GPD_x2228 (2*2*2*8)
#define GPD_x3338 (3*3*3*8)

//#define GPD_3D_RECV_N_PNG	16

#define GPD_3D_defDIM	48
#define GPD_3D_defDIMf	GPD_3D_defDIM.0f
#define GPD_3D_NBREAK	8

#define GPD_3D_CORE		192
#define GPD_3D_WIDTH	5

#define GPD_DIR_D 0x1
#define GPD_DIR_B 0x2
#define GPD_DIR_R 0x4
#define GPD_DIR_L 0x8
#define GPD_DIR_F 0x10
#define GPD_DIR_U 0x20

typedef enum GPE_DIR:U1
{
	GPE_DIR_DOWN,
	GPE_DIR_BACK,
	GPE_DIR_LEFT,
	GPE_DIR_RIGHT,
	GPE_DIR_FRONT,
	GPE_DIR_TOP,
	GPE_DIR_ERROR,
};

#define GPD_3D_UP	(GPD_3D_END_Y-GPD_3D_STEP_Y)
#define GPD_3D_FRONT	(GPD_3D_END_Z-GPD_3D_STEP_Z)
#define GPD_3D_RIGHT	(GPD_3D_END_X-GPD_3D_STEP_X)
#define GPD_3D_LEFT	0
#define GPD_3D_BACK	0
#define GPD_3D_DOWN	0

typedef enum GPE_3D_PIPE : I8
{
	GPE_3D_PIPE_OFF,
	GPE_3D_PIPE_INIT,
	GPE_3D_PIPE_FETCH,

	GPE_3D_PIPE_CHECK,
	GPE_3D_PIPE_EXEC,
	GPE_3D_PIPE_VOX,
	GPE_3D_PIPE_UPLOAD,
	GPE_3D_PIPE_DRAW,
	GPE_3D_PIPE_END,
};

typedef enum GPE_3D : I8
{
	GPE_3D_STEP_X	= GPD_3D_STEP_X,
	GPE_3D_Ws1		= GPD_3D_WIDTH-1,
	GPE_3D_WIDTH	= GPD_3D_WIDTH,
	GPE_3D_ULL		= (GPD_3D_LINE/2),
	GPE_3D_LINE		= GPD_3D_LINE,
	GPE_xCENTER		= (GPD_x3*GPD_x3*GPD_x3)/2,			//+(GPD_x3*GPD_x3)/2+(GPD_x3/2),
	GPE_xHIRES		= (GPD_x3*GPD_x3*GPD_x3),
	GPE_3D_WIDTHx	= GPE_3D_WIDTH*GPE_3D_WIDTH,
	GPE_3D_WC		= GPE_3D_LINE*(GPE_3D_Ws1/2),
	GPE_Ws1x16		= GPE_3D_LINE*GPE_3D_Ws1,
	GPE_Ws1x16p1	= GPE_Ws1x16+1,
	GPE_W16			= GPE_3D_LINE*GPE_3D_WIDTH,
	GPE_3D_WIDTHq	= GPE_3D_WIDTH*GPE_3D_WIDTH*GPE_3D_WIDTH,
	GPE_3D_xz		= GPE_3D_LINE*GPE_3D_LINE,
	GPE_3D_xzy		= GPE_3D_xz*GPE_3D_LINE,
	GPE_W16_LINE	= GPE_3D_LINE*GPE_W16,
	GPE_W168		= GPE_W16*8,
	GPE_W1688		= GPE_W168*8,
	GPE_W1688p1		= GPE_W1688+GPE_W16,
	GPE_Wx			= GPE_W16*GPE_W16,
	GPE_Wx8			= GPE_Wx*8,
	GPE_WxY			= GPE_Wx*GPE_3D_LINE,
	GPE_WxWs1		= GPE_Wx*GPE_Ws1x16,
	GPE_Wq			= GPE_Wx*GPE_W16,
	GPE_Wq_FETCH	= GPE_Wq*3,
	//GPE_3D_ENV		= 1<<24,
};

//=============================================================================
//
//			CELL_CELL
//
//=============================================================================
//		( +6400.0f to_unsigned)
//
//		Y[ d->T: +64 ]
//		Z[ b->F: +32 ]
//		X[ l->R: + 0 ]
//
//=============================================================================
#define GPD_LIG_t (( GPE_Wx			+GPE_Wx)*256+64)
#define GPD_LIG_f (( GPE_W16		+GPE_Wx)*256+32)
#define GPD_LIG_r (( GPE_3D_STEP_X	+GPE_Wx)*256)
#define GPD_LIG_l ((-GPE_3D_STEP_X	+GPE_Wx)*256)
#define GPD_LIG_b ((-GPE_W16		+GPE_Wx)*256+32)
#define GPD_LIG_d ((-GPE_Wx			+GPE_Wx)*256+64)

enum GPE_LIG:I8
{

	GPE_LIG_d = GPD_LIG_d,
	GPE_LIG_d4 = GPE_LIG_d+4,

	GPE_LIG_b = GPD_LIG_b,
	GPE_LIG_b10 = GPE_LIG_b+10,

	GPE_LIG_l = GPD_LIG_l,
	GPE_LIG_l12 = GPE_LIG_l+12,

	GPE_LIG_r = GPD_LIG_r,
	GPE_LIG_r14 = GPE_LIG_r+14,
	
	GPE_LIG_f = GPD_LIG_f,
	GPE_LIG_f16 = GPE_LIG_f+16,
	
	GPE_LIG_t = GPD_LIG_t,
	GPE_LIG_t22 = GPE_LIG_t+22,

};

#define	veof	0,0
#define	v01r14	0, GPE_LIG_r14
#define	v01t22	0, GPE_LIG_t22
#define	v01f16	0, GPE_LIG_f16

#define	v02l12	1, GPE_LIG_l12
#define	v02r14	1, GPE_LIG_r14
#define	v02f16	1, GPE_LIG_f16
#define	v02t22	1, GPE_LIG_t22

#define	v04	GPE_W16
#define	v04b10	v04, GPE_LIG_b10
#define	v04r14	v04, GPE_LIG_r14
#define	v04t22	v04, GPE_LIG_t22

#define	v08	(GPE_W16+1)
#define	v08b10	v08, GPE_LIG_b10
#define	v08l12	v08, GPE_LIG_l12
#define	v08t22	v08, GPE_LIG_t22

#define	v10	GPE_Wx
#define	v10d4	v10, GPE_LIG_d4
#define	v10r14	v10, GPE_LIG_r14
#define	v10f16	v10, GPE_LIG_f16

#define	v20	GPE_Wx+1
#define	v20d4	v20, GPE_LIG_d4
#define	v20l12	v20, GPE_LIG_l12
#define	v20f16	v20, GPE_LIG_f16

#define	v40	GPE_Wx+GPE_W16
#define	v40d4	v40, GPE_LIG_d4
#define	v40b10	v40, GPE_LIG_b10
#define	v40r14	v40, GPE_LIG_r14

#define	v80	GPE_Wx+GPE_W16+1
#define	v80d4	v80, GPE_LIG_d4
#define	v80b10	v80, GPE_LIG_b10
#define	v80l12	v80, GPE_LIG_l12

typedef enum GPE_BASE : I8
{
	gpe_base_x		= 0x100,
	GPE_BASE_HALF	= gpe_base_x*0x08,
	GPE_BASE_FULL	= gpe_base_x*0x10,
	GPE_BASE_XZ	= gpe_base_x*gpe_base_x,
	GPE_BASE_sub	= GPE_BASE_XZ+gpe_base_x+1,
	GPE_BASE_XYZ	= GPE_BASE_XZ*gpe_base_x,
};

typedef enum GPE_Px7 : I8
{
	gpe_px7_x		= 0x80,
	GPE_Px7_HALF	= gpe_px7_x*0x08,
	GPE_Px7_FULL	= gpe_px7_x*0x10,
	gpe_px7_xz		= gpe_px7_x*gpe_px7_x,
	gpe_px7_xzy	= gpe_px7_xz*gpe_px7_x,
	
	GPE_Px7_XZ		= GPE_Px7_FULL*GPE_Px7_FULL,
	GPE_Px7_XZY	= GPE_Px7_XZ*GPE_Px7_FULL,
};

enum GPE_ACTION : U1
{
	GPE_ACTION_DEF,
	GPE_ACTION_IDLE,	// <BEGIN>  1.0 <END> 31.0 <START>  1.0 <MIX>  0.0  <FADE> 0.25 <GIM> 1.0 <LOOP> 1.0 <ENDTRACK>
	GPE_ACTION_WALK,	// <BEGIN> 32.5 <END> 34.5 <START> 32.5 <MIX> 33.5  <FADE> 0.5  <GIM> 1.0 <LOOP> 33.0 <ENDTRACK>
	GPE_ACTION_RUN,	// <BEGIN> 36.5 <END> 38.5 <START> 36.5 <MIX> 38.5  <FADE> 0.5  <GIM> 0.3 <LOOP> 37.0 <ENDTRACK>
	GPE_ACTION_JUMP,	// <BEGIN> 40.0 <END> 42.0 <START> 40.0 <MIX> 41.5  <FADE> 0.5  <GIM> 0.3 <STOP> <ENDTRACK>
	GPE_ACTION_SLEFT,	// <BEGIN> 43.0 <END> 45.0 <START> 43.0 <MIX> 44.5  <FADE> 0.5  <GIM> 1.0 <LOOP> 43.0 <ENDTRACK>
	GPE_ACTION_SRIGHT,	// <BEGIN> 46.0 <END> 48.0 <START> 46.0 <MIX> 47.5  <FADE> 0.5  <GIM> 1.0 <LOOP> 46.0 <ENDTRACK>
	GPE_ACTION_FLINCH,	// <BEGIN> 49.0 <END> 52.0 <START> 49.5 <MIX> 51.5  <FADE> 0.5  <GIM> 0.5 <LOOP> 50.0 <ENDTRACK>
	GPE_ACTION_BOX,	// <BEGIN> 53.0 <END> 54.0 <START> 53.0 <MIX> 53.75 <FADE> 0.25 <GIM> 0.1 <LOOP> 53.0 <ENDTRACK>
	GPE_ACTION_KICK,	// <BEGIN> 55.0 <END> 56.0 <START> 55.0 <MIX> 55.5  <FADE> 0.5  <GIM> 0.1 <LOOP> 55.0 <ENDTRACK>
	GPE_ACTION_ABOARD,	// <BEGIN> 57.0 <END> 58.0 <START> 57.0 <MIX> 57.75 <FADE> 0.25 <GIM> 0.2 <STOP> <ENDTRACK>
	GPE_ACTION_DEBUS,	// <BEGIN> 58.0 <END> 60.0 <START> 58.0 <MIX> 60.0  <FADE> 0.25 <GIM> 0.2 <STOP> <ENDTRACK>
	GPE_ACTION_STOW,	// <BEGIN> 61.0 <END> 61.5 <START> 61.0 <MIX> 61.5  <FADE> 0.25 <GIM> 0.1 <STOP> <ENDTRACK>
	GPE_ACTION_STOWUP,	// <BEGIN> 61.5 <END> 62.0 <START> 61.5 <MIX> 62.0  <FADE> 0.25 <GIM> 0.1 <STOP> <ENDTRACK>
	GPE_ACTION_WHAM,	// <BEGIN> 63.0 <END> 64.0 <START> 63.0 <MIX> 63.75 <FADE> 0.25 <GIM> 0.1 <LOOP> 63.0 <ENDTRACK>
	GPE_ACTION_KO,	// <BEGIN> 65.0 <END> 69.0 <START> 65.0 <MIX> 68.9  <FADE> 0.01 <GIM> 0.1 <STOP> <ENDTRACK>
	GPE_ACTION_ERECT,	// <BEGIN> 69.0 <END> 74.0 <START> 69.0 <MIX> 73.9  <FADE> 0.5  <GIM> 0.1 <STOP> <ENDTRACK>
	GPE_ACTION_END,
};

class GPCI2
{
public:
	I8 x, y;
	GPCI2& null()
	{
		GPM_CLR;
		return *this;
	}
	GPCI2& operator = (I8 i)
	{
		x = y = i;
		return *this;
	}
	GPCI2* set( ULL n )
	{
		GPF_MEM_set(this + 1, n-1, this, sizeof(*this) );
		return this;
	}
	GPCI2* add( I8 id, I8 ms, I8& ld, I8& n )
	{
		GPCI2	TMP, *p_kill, *p_this;
		I8 i = 0, n_kill;
		while( i < ld )
		{
			if( this[i].x == id )
				break;
			i++;
		}
		if( i >= ld )
		{
			if( i >= n )
			{
				p_kill = this, n_kill = min(ld,n);
				n = ld + 0x10;
				p_this = new GPCI2[n];
				GPM_MEMCPY( p_this, p_kill, n_kill );
				GPM_DELARRAY( p_kill );
			}
			i = ld;
			p_this[i].x = id;
			ld++;
		} else
			p_this = this;

		if( i > 0 )
		{
			TMP = *p_this;
			*p_this = p_this[i];
			p_this[i] = TMP;
		}
		p_this->y = ms;
		return p_this;
	}

	GPCI2* add( GPC_NODE& node, I8& ld, I8& n );
	GPC_FIX* list( GPC_FIX* p_fix, I8 ld );
	
};
//__declspec(align(16))
class GPCI4
{
public:
	union
	{
		struct
		{
			I8 x, y, z, w;
		};
		struct
		{
			I8 x, y, z, r;
		};
		struct
		{
			I8 n_cnt, n_req, n_recv, n_end;
		};
		struct
		{
			I8 n_cnt, e0, n_load, n_alloc;
		};
		struct
		{
			I8 lev0, lev1, lev2, lev3;
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
			GPE_ACTION	act;
			U1	mxi, drh, drv;
		};
		struct // BUSSINES
		{
			// I8 1,2, 
			I8 src_id, dst_id;
			// I8 3,
			U4	DRPq, MINq,
				gx_s4;	// 4 másodperc			
			// I8 4
			GPE_ACTION	act;
			U1			ixIL, ixIR, n_op; // OPcode 1-one 2-half 3-all
		};
	};


	bool overlap_angle( GPCI4& b )
	{
		if( b.z >= z )
		{
			if( ((b.z - z) % 0x10000) < x )
				return true;
			return false;
		}

		if( ((z - b.z) % 0x10000) < b.x )
			return true;

		return false;
	}


	GPCI4(){};
	GPCI4( I8 xyzw )
	{
		x = 
		y = 
		z = 
		w = xyzw;
	}
	GPCI4( I8 _x, I8 _y, I8 _z = 0, I8 _w = 0 )
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}
	GPCI4( GPCV3& v3 )
	{
		x = v3.x;
		y = v3.y;
		z = v3.z;
		w = 0;
	}
	
	GPCI4( GPCV4& v4 )
	{
		x = v4.x;
		y = v4.y;
		z = v4.z;
		w = v4.w;
	}
	GPCI4( const GPCI4& air, I8 mul, I8 div, bool b_dbg )
	{
		ar2gr( air, mul, div, b_dbg ).gr_valid();
	}

	
	GPCI4& max_one( void )
	{
		//*this %= 3;
		I8 mx = 0, *p_v4 = &x;
		if( abs(x) >= abs(y) )
		{	
			y = 0;
			mx = 0; // x
		} else {
			x = 0;
			mx = 1; // y
		}

		if( abs(z) >= abs(p_v4[mx]) )
		{
			mx = 2;
			p_v4[mx] = 0; // z
		}
		if( abs(w) >= abs(p_v4[mx]) )
		{
			mx = 3;
			p_v4[mx] = 0; // w
		}
		if( p_v4[mx] >= 0 )
			p_v4[mx] = 1;
		else
			p_v4[mx] = -1;
		return *this;
	}
	GPCI4& step_one( GPCI4 trg )
	{
		trg -= *this;
		I8 s = trg.sum_abs();
		if( !s )
			return *this;
		*this += trg.max_one(); 
		return *this;
	}
	GPC_AN an( void )
	{
		return GPC_AN( z, w );
	}
	GPC_AN an_lu( void )
	{
		return GPC_AN( min(x,z), min(y,w) );
	}
	GPC_AN an_rd( void )
	{
		return GPC_AN( max(x,z), max(y,w) );
	}
	GPC_AN& operator = ( const GPC_AN& _an )
	{
		z = _an.a;
		w = _an.n;
		if( w < 0 )
		{
			z = w = 0;
		}
		return an();
	}
	GPCI4 lb( void ) // log2
	{
		return GPCI4( x ? (I8)log2(x) : 0, y ? (I8)log2(y) : 0, z ? (I8)log2(z) : 0, w ? (I8)log2(w): 0 );
	}
	GPCI4& operator = ( I8 i8 )
	{
		 x = y = z = w = i8;
		 return *this;
	}
	
	GPCI4& operator = (const GPC_COLOR4& i4 )
	{
		 x = i4.B;
		 y = i4.G;
		 z = i4.R;
		 w = i4.A;
		 return *this;
	}
	
	GPCI4& operator += (const GPCV3& v3 )
	{
		 x += v3.x;
		 y += v3.y;
		 z += v3.z;
		 //w += i4.w;
		 return *this;
	}
	//GPCI4& operator ++( void )
	//{
	//	 x++;
	//	 y++;
	//	 z++;
	//	 w++;
	//	 return *this;
	//}
	//GPCI4& operator --( void )
	//{
	//	 x--;
	//	 y--;
	//	 z--;
	//	 w--;
	//	 return *this;
	//}
	GPCI4& operator += (const GPCI4& i4 )
	{
		gp_i4_add( this, (void*)&i4 );
		//I8 *p_d = (I8*)this, *p_s = (I8*)&i4;
		//#pragma loop count(4)
		//for( int i = 0; i < 4; ++i )
		//	p_d[i] += p_s[i];
		//x += i4.x;
		//y += i4.y;
		//z += i4.z;
		//w += i4.w;
		 return *this;
	}
	GPCI4& operator -= (const GPCI4& i4 )
	{
		 gp_i4_sub( this, (void*)&i4 );
		 //x -= i4.x;
		 //y -= i4.y;
		 //z -= i4.z;
		 //w -= i4.w;
		 return *this;
	}
GPCI4& operator *= ( I8 i8 )
	{
		if( !i8 )
			return null();
		if( i8 == 1 )
			return *this;
		//x *= i8;
		//y *= i8;
		//z *= i8;
		//w *= i8;
		gp_muls_q( x, i8 );
		gp_muls_q( y, i8 );
		gp_muls_q( z, i8 );
		gp_muls_q( w, i8 );
	
		return *this;
	}
	GPCI4& operator *= ( const GPCI4& i42 ) 
	{
		//return *(GPCI4*)gp_mul_v4_v4( (void*)this, (void*)&i42 );
		//x = gp_mul64( x, i42.x);
		//y = gp_mul64( y, i42.y);
		//z = gp_mul64( z, i42.z);
		//w = gp_mul64( w, i42.w);

		gp_muls_q( x, i42.x);
		gp_muls_q( y, i42.y);
		gp_muls_q( z, i42.z);
		gp_muls_q( w, i42.w);

		return *this;
	}

	GPCI4 operator * ( GPCMX& mx );
	GPCI4 operator * ( const GPCI4& i42 ) const
	{
		GPCI4 out = *this;
		return out *= i42;
	}
	GPCI4 operator * ( I8 i8 ) const
	{
		GPCI4 out = *this;
		return out *= i8;
	}
	GPCI4& operator &= ( I8 i8 )
	{
		if( !i8 )
			return null();
		x &= i8;
		y &= i8;
		z &= i8;
		w &= i8;
		return *this;
	}
	GPCI4& operator /= ( I8 i8 )
	{
		if( i8 == 1 )
			return *this;
		else if( !i8 )
		{
			x = x > 0 ? 0x7FFFffffFFFFffff : -0x7FFFffffFFFFffff;
			y = y > 0 ? 0x7FFFffffFFFFffff : -0x7FFFffffFFFFffff;
			z = z > 0 ? 0x7FFFffffFFFFffff : -0x7FFFffffFFFFffff;
			w = w > 0 ? 0x7FFFffffFFFFffff : -0x7FFFffffFFFFffff;
			return *this;
		}
		
		x /= i8;
		y /= i8;
		z /= i8;
		w /= i8;
		return *this;
	}
	GPCI4& operator %= ( I8 i8 )
	{
		 x %= i8;
		 y %= i8;
		 z %= i8;
		 w %= i8;
		 return *this;
	}
	GPCI4& operator /= (const GPCI4& i4 )
	{
		if( i4.x != 1 )
			x = i4.x ? x/i4.x : ( x = x > 0 ? 0x7FFFffffFFFFffff : -0x7FFFffffFFFFffff );
		if( i4.y != 1 )
			y = i4.y ? y/i4.y : ( y = y > 0 ? 0x7FFFffffFFFFffff : -0x7FFFffffFFFFffff );
		if( i4.z != 1 )
			z = i4.z ? z/i4.z : ( z = z > 0 ? 0x7FFFffffFFFFffff : -0x7FFFffffFFFFffff );
		if( i4.w != 1 )
			w = i4.w ? w/i4.w : ( w = w > 0 ? 0x7FFFffffFFFFffff : -0x7FFFffffFFFFffff );
		// y /= i4.y;
		// z /= i4.z;
		// w /= i4.w;
		 return *this;
	}
	GPCI4& operator %= (const GPCI4& i4 )
	{
		 x %= i4.x;
		 y %= i4.y;
		 z %= i4.z;
		 w %= i4.w;
		 return *this;
	}
	bool operator == (const GPCI4& i4 ) const
	{
		return x == i4.x && y == i4.y && z == i4.z && w == i4.w;
	}
	bool operator != (const GPCI4& i4 ) const
	{
		return x != i4.x || y != i4.y || z != i4.z || w != i4.w;
	}
	GPCI4 operator + (const GPCV3& i32 ) const
	{
		return GPCI4( x+i32.x, y+i32.y, z+i32.z, w );
	}
	GPCI4 operator + (const GPCI4& i42 ) const
	{
		return GPCI4( x+i42.x, y+i42.y, z+i42.z, w+i42.w );
	}
	GPCI4& operator += ( I8 i8 )
	{
		if( !i8 )
			return *this;
		x += i8;
		y += i8;
		z += i8;
		w += i8;
		return *this;
	}
	GPCI4& operator -= ( I8 i8 )
	{
		if( !i8 )
			return *this;
		x -= i8;
		y -= i8;
		z -= i8;
		w -= i8;
		return *this;
	}
	GPCI4& operator <<= ( I8 i8 )
	{
		if( !i8 )
			return *this;
		x <<= i8;
		y <<= i8;
		z <<= i8;
		w <<= i8;
		return *this;
	}
	GPCI4& operator >>= ( I8 i8 )
	{
		if( !i8 )
			return *this;
		x >>= i8;
		y >>= i8;
		z >>= i8;
		w >>= i8;
		return *this;
	}
	GPCI4& operator &= (const GPCI4& i4 )
	{
		 x &= i4.x;
		 y &= i4.y;
		 z &= i4.z;
		 w &= i4.w;
		 return *this;
	}
	GPCI4& operator += ( GPC_COLOR4 c4 )
	{
		x += c4.B;
		y += c4.G;
		z += c4.R;
		w += c4.A;
		return *this;
	}
	GPCI4	operator + ( I8 i8 ) const;
	GPCI4	operator - ( I8 i8 ) const;
	GPCI4	operator - (const GPCI4&) const;
	GPCI4	operator << (const I8 i8 ) const;
	//GPCI4	operator * (const GPCI4&) const;
	//I8	operator * (const GPCI4&) const;
	GPCI4	operator & (const I8 i8 ) const;
	GPCI4	operator & (const GPCI4&) const;
	GPCI4	operator >> (const I8 i8 ) const;
	GPCI4	operator / (const GPCI4&) const;
	GPCI4	operator / ( I8 i8 )
	{
		if( i8 )
		if( i8 == 1 )
			return *this;
		else 
			return GPCI4( x/i8, y/i8, z/i8, w/i8 );

		return GPCI4( 
						x > 0 ? 0x7FFFffffFFFFffff : -0x7FFFffffFFFFffff,
						y > 0 ? 0x7FFFffffFFFFffff : -0x7FFFffffFFFFffff,
						z > 0 ? 0x7FFFffffFFFFffff : -0x7FFFffffFFFFffff,
						w > 0 ? 0x7FFFffffFFFFffff : -0x7FFFffffFFFFffff
					);
	}
	GPCI4& cross_xyz_l( GPCI4& v1, GPCI4& v2 )
	{
		x = v1.y * v2.z - v1.z * v2.y;
		y = v1.z * v2.x - v1.x * v2.z;
		z = v1.x * v2.y - v1.y * v2.x;
		w = maxi_absabs_xyz();
		return *this;
	}
	GPCI4 cross_xyz( GPCI4& v2 )
	{
		GPCI4 out;
		return out.cross_xyz_l( *this, v2 );
	}
	GPCI4& threshold( I8 i )
	{
		if( i < 0 )
			i *= -1;

		if( abs( x ) < i )
			x = 0;

		if( abs( y ) < i )
			y = 0;

		if( abs( z ) < i )
			z = 0;

		if( abs( w ) < i )
			w = 0;

		return *this;
	}
	GPCI4& abs_xyzw( void )
	{
		if( x < 0 )
			x *= -1;
		if( y < 0 )
			y *= -1;
		if( z < 0 )
			z *= -1;
		if( w < 0 )
			w *= -1;
	}
	GPCI4& threshold( GPCI4 v4 )
	{
		v4.abs_xyzw();
		if( abs( x ) < v4.x )
			x = 0;

		if( abs( y ) < v4.y )
			y = 0;

		if( abs( z ) < v4.z )
			z = 0;

		if( abs( w ) < v4.w )
			w = 0;

		return *this;
	}
	GPCI4 abs_i4( void )
	{
		return GPCI4( (x < 0 ? -x : x), (y < 0 ? -y : y), (z < 0 ? -z : z), (w < 0 ? -w : w) );
	}
	GPCI4 abs_xyz( void )
	{
		return GPCI4( (x < 0 ? -x : x), (y < 0 ? -y : y), (z < 0 ? -z : z), 0 );
	}
	GPCI4& null( void )
	{
		GPM_CLR;
		return *this;
	}
	GPCI4& null_xyz( void )
	{
		x = y = z = 0;
		return *this;
	}
	GPCI4& null_xy( void )
	{
		x = y = 0;
		return *this;
	}
	I8 line_dist_xyz( GPCI4& p )
	{
		GPCI4	v1 = this[1]-this[0],
				v2 = p-this[0];
		I8 l = v1.len_xyz();
		if( !l )
			return v2.len_xyz();
		I8 l2 = v2.len_xyz();
		if( !l2 )
			return 0;

		I8 d = (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z)/l,
			o = sqrt(l2*l2 - d*d);
		return o;
	}

	GPCV3 tq2z( GPCI4& DIRax, GPCI4& TRGgr, U4 DSTq )
	{
		TRGgr.q2gr( DSTq );
		GPCI4	TRGar = TRGgr.gr2ar( gpe_giv_1m, 1 );
		
		//DIRax = TRGar - *this;
		//GPCI4 Zax = DIRax.yz();
		//I8 Z = Zax.maxi_absabs_yz();
		//if( !Z )
		//{
		//	Zax = GPCI4( 0, 6, 0, 0 );
		//	DIRax = Zax;
		//} else {
		//	Zax *= 6;
		//	Zax /= Z;
		//}
		//GPCI4	Zgr = TRGar - *this,
		//		Zar = Zgr.gr2ar( gpe_giv_1m, 1 );
	
		return (*this-TRGar).xyz_norm();
	}
	GPCV3 q2grNarNz( GPCI4& Zax, GPCI4& DIRax, GPCI4& TRGgr, GPCI4& POSgr, U4 DSTq, U4 POSq )
	{
		// POSar
		*this = POSgr.q2gr( POSq ).gr2ar( gpe_giv_1m, 1 );
		TRGgr.q2gr( DSTq );//.gr2ar( gpe_giv_1m, 1 );
		if( TRGgr.w != POSgr.w )
		{
			GPCI4 TRGar = TRGgr.gr2ar( gpe_giv_1m, 1 );
			TRGgr.ar2gr_sid( TRGar, 1, gpe_giv_1m, POSgr.w );
		}
		DIRax = TRGgr - POSgr;
		Zax = DIRax.yz();
		I8 Z = Zax.maxi_absabs_yz();
		if( !Z )
		{
			Zax = GPCI4( 0, 6, 0, 0 );
			DIRax = Zax;
		} else {
			Zax *= 6;
			Zax /= Z;
		}
		GPCI4	Zgr = POSgr+Zax,
				Zar = Zgr.gr2ar( gpe_giv_1m, 1 );

		return (*this-Zar).xyz_norm();
	}
	GPCV3 gr2arNz( GPCI4& Zax, GPCI4& DIRax, GPCI4& TRGgr, GPCI4& POSgr )
	{
		*this = POSgr.gr2ar( gpe_giv_1m, 1 );
		DIRax = TRGgr - POSgr;
		Zax = DIRax.yz();
		I8	Z = Zax.len_yz(),
			D = DIRax.len_xyz();
		if( !Z )
		{
			Zax = GPCI4( 0, 6, 0, 0 );
			DIRax = Zax;
		} else {
			Zax *= 6;
			Zax /= Z;
			DIRax *= 6;
			DIRax /= D;
		}
		GPCI4	Zgr = POSgr+Zax,
			Zar = Zgr.gr2ar( gpe_giv_1m, 1 );

		return (*this-Zar).xyz_norm();
	}
	

	GPCI4& q2gr( U4 q0 )
	{
		x = q0&gpe_giv_qh_msk;
		q0 >>= gpe_giv_hl;
		z = q0&gpe_giv_qw_msk;
		q0 >>= gpe_giv_wl;
		y = q0&gpe_giv_qw_msk;
		q0 >>= gpe_giv_wl;
		w = q0&0xf;
		return *this;
	}
	GPCI4& Q2G( U4 Q0 )
	{
		x = Q0&0xf;
		Q0 >>= gpe_giv_h;
		z = Q0&gpe_giv_Qw_msk;
		Q0 >>= gpe_giv_w;
		y = Q0&gpe_giv_Qw_msk;
		Q0 >>= gpe_giv_w;
		w = Q0&0x7;
		return *this;
	}
	GPCI4& Q2gr( U4 Q0 )
	{
		Q2G( Q0 );
		x *= gpe_giv_X;
		y *= gpe_giv_X;
		z *= gpe_giv_X;
		return *this;
	}
	U4 gr2q( void )
	{
		register U4 q = w%6;
		q <<= gpe_giv_wl;
		q |= y%gpe_giv_qw;
		q <<= gpe_giv_wl;
		q |= z%gpe_giv_qw;
		q <<= gpe_giv_hl;
		q |= x%gpe_giv_qh;

		//U4 q  =  (x&0x0ff);
		//q |=  (z&0x3ff)<<gpe_giv_hl;
		//q |=  (y&0x3ff)<<( gpe_giv_hl + gpe_giv_wl   );
		//q |=  (w&0x007)<<( gpe_giv_hl + gpe_giv_wl*2 );
		return q;
	}
	U4 gr2Q( void )
	{
		register U4 q = w%6;
		q <<= gpe_giv_w;
		q |= (y%gpe_giv_qw)/gpe_giv_LINE;
		q <<= gpe_giv_w;
		q |= (z%gpe_giv_qw)/gpe_giv_LINE;
		q <<= gpe_giv_h;
		q |= (x%gpe_giv_qh)/gpe_giv_LINE;
		//U4 
		//q  =  x/gpe_giv_LINE;
		//q |= (z/gpe_giv_LINE)<<gpe_giv_h;
		//q |= (y/gpe_giv_LINE)<<(gpe_giv_h+gpe_giv_w);
		//q |=  w<<(gpe_giv_h+gpe_giv_w+gpe_giv_w);
		return q;
	}
	U4 GR2Q( void )
	{
		U4 q = x;
		if( z < 0 || z >= gpe_giv_Z )
			return gpe_giv_XZY3;
		if( y < 0 || y >= gpe_giv_Z )
			return gpe_giv_XZY3;
		q |= z<<gpe_giv_h;
		q |= y<<(gpe_giv_h+gpe_giv_w);
		q |= w<<(gpe_giv_h+gpe_giv_w+gpe_giv_w);
		return q;
	}
	GPCI4& GRdwn( I8 div )
	{
		GPCI4 out = *this / div;
			
		if( out.x&1 )
			out.x -= 2;
		else
			out.x--;

		if( out.y&1 )
			out.y -= 2;
		else
			out.y--;
		
		if( out.z&1 )
			out.z -= 2;
		else
			out.z--;
		
		out *= div;
		out.w = w;
		return out;
	}
	GPCI4& GRdwn_fly( I8 div )
	{
		GPCI4 out = *this;
		
		out.down_xyz( div );
		//if( out.x&1 )
		//	out.x -= 2;
		//else
		//	out.x--;
		//
		//if( out.y&1 )
		//	out.y -= 2;
		//else
		//	out.y--;
		//
		//if( out.z&1 )
		//	out.z -= 2;
		//else
		//	out.z--;
		//
		//out *= div;
		//out.w = w;
		return out;
	}
	GPCI4& Qdwn27( I8 n27 );
	GPCI4& QsubZ( void )
	{
		if( z >= 0 )
			return *this;
		I8 n, tmp;
		
		z %= (gpe_giv_Z*4);
		n = z/gpe_giv_Z;
		z %= gpe_giv_Z;
		switch( w )
		{
			case 0:
				switch( n )
				{
					case 0:
						w = 5;
						//B.y = y;
						z += gpe_giv_Z;
						break;
					case 1:
						w = 3;
						tmp = gpe_giv_Z-1-y;
						y = gpe_giv_Z+z;
						z = tmp;
						break;
					case 2:
						w = 2;
						tmp = gpe_giv_Z-1-y;
						y = gpe_giv_Z+z;
						z = tmp;
						break;
					case 3:
						w = 0;
						//B.y = y;
						z += gpe_giv_Z;
						break;
				}
				break;
			case 1:
				switch( n )
				{
					case 0:
						w = 5;
						tmp = gpe_giv_Z-1-y;
						y = gpe_giv_Z+z;
						z = tmp;
						break;
					case 1:
						w = 4;
						tmp = gpe_giv_Z-1-y;
						y = gpe_giv_Z+z;
						z = tmp;
						break;
					case 2:
						w = 2;
						// y = y;
						z += gpe_giv_Z;
						break;
					case 3:
						w = 1;
						// y = y;
						z += gpe_giv_Z;
						break;
				}
				break;
			case 2:
				switch( n )
				{
					case 0:
						w = 1;
						// y = y;
						z += gpe_giv_Z;
						break;
					case 1:	
						w = 5;
						tmp = gpe_giv_Z-1-y;
						y = gpe_giv_Z+z;
						z = tmp;
						break;
					case 2:
						w = 4;
						tmp = gpe_giv_Z-1-y;
						y = gpe_giv_Z+z;
						z = tmp;
						break;
					case 3:
						w = 2;
						// y = y;
						z += gpe_giv_Z;
						break;
				}
				break;
			case 3:
				switch( n )
				{
					case 0:
						w = 1;
						tmp = gpe_giv_Z-1-y;
						y = gpe_giv_Z+z;
						z = tmp;
						break;
					case 1:
						w = 0;
						tmp = gpe_giv_Z-1-y;
						y = gpe_giv_Z+z;
						z = tmp;
						break;
					case 2:
						w = 4;
						// y = y;
						z += gpe_giv_Z;
						break;
					case 3:
						w = 3;
						// y = y;
						z += gpe_giv_Z;
						break;
				}
				break;
			case 4:
				switch( n )
				{
					case 0:
						w = 3;
						// y = y;
						z += gpe_giv_Z;
						break;
					case 1:
						w = 1;
						tmp = gpe_giv_Z-1-y;
						y = gpe_giv_Z+z;
						z = tmp;
						break;
					case 2:
						w = 0;
						tmp = gpe_giv_Z-1-y;
						y = gpe_giv_Z+z;
						z = tmp;
						break;
					case 3:
						w = 4;
						// y = y;
						z += gpe_giv_Z;
						break;
				}
				break;
			case 5:
				switch( n )
				{
					case 0:
						w = 3;
						tmp = gpe_giv_Z-1-y;
						y = gpe_giv_Z+z;
						z = tmp;
						break;
					case 1:
						w = 2;
						tmp = gpe_giv_Z-1-y;
						y = gpe_giv_Z+z;
						z = tmp;
						break;
					case 2:
						w = 0;
						// y = y;
						z += gpe_giv_Z;
						break;
					case 3:
						w = 5;
						// y = y;
						z += gpe_giv_Z;
						break;
				}
				break;
		}
		return *this;
	}
	GPCI4& QsubY( void )
	{
		if( y >= 0 )
			return *this;

		I8 n, tmp;
		y %= (gpe_giv_Z*4);
		n = y/gpe_giv_Z;
		y %= gpe_giv_Z;
		switch( w )
		{
			case 0:
				switch( n )
				{
					case 0:
						w = 4;
						tmp = gpe_giv_Z-1-z;
						z = gpe_giv_Z+y;
						y = tmp;
						break;
					case 1:
						w = 3;
						tmp = gpe_giv_Z-1-z;
						z = gpe_giv_Z+y;
						y = tmp;
						break;
					case 2:
						w = 1;
						y = gpe_giv_Z+y;
						break;
					case 3:
						w = 0;
						y = gpe_giv_Z+y;
						break;
				}
				break;
			case 1:
				switch( n )
				{
					case 0:
						w = 0;
						y = gpe_giv_Z+y;
						break;
					case 1:
						w = 4;
						z = gpe_giv_Z+y;
						y = tmp;
						break;
					case 2:
						w = 3;
						z = gpe_giv_Z+y;
						y = tmp;
						break;
					case 3:
						w = 1;
						y = gpe_giv_Z+y;
						break;
				}
				break;
			case 2:
				switch( n )
				{
					case 0:
						w = 0;
						z = gpe_giv_Z+y;
						y = tmp;
						break;
					case 1:
						w = 5;
						z = gpe_giv_Z+y;
						y = tmp;
						break;
					case 2:
						w = 3;
						y = gpe_giv_Z+y;
						break;
					case 3:
						w = 2;
						y = gpe_giv_Z+y;
						break;
				}
				break;
			case 3:
				switch( n )
				{
					case 0:
						w = 2;
						y = gpe_giv_Z+y;
						break;
					case 1:
						w = 0;
						z = gpe_giv_Z+y;
						y = tmp;
						break;
					case 2:
						w = 5;
						z = gpe_giv_Z+y;
						y = tmp;
						break;
					case 3:
						w = 3;
						y = gpe_giv_Z+y;
						break;
				}
				break;
			case 4:
				switch( n )
				{
					case 0:
						w = 2;
						z = gpe_giv_Z+y;
						y = tmp;
						break;
					case 1:
						w = 1;
						z = gpe_giv_Z+y;
						y = tmp;
						break;
					case 2:
						w = 5;
						y = gpe_giv_Z+y;
						break;
					case 3:
						w = 4;
						y = gpe_giv_Z+y;
						break;
				}
				break;
			case 5:
				switch( n )
				{
					case 0:
						w = 4;
						y = gpe_giv_Z+y;
						break;
					case 1:
						w = 2;
						z = gpe_giv_Z+y;
						y = tmp;
						break;
					case 2:
						w = 1;
						z = gpe_giv_Z+y;
						y = tmp;
						break;
					case 3:
						w = 5;
						y = gpe_giv_Z+y;
						break;
				}
				break;
		}


		return *this;
	}
	GPCI4& GR_valid( void );
	GPCI4& gr_valid( void );
	GPCI4& gr_frame( GPCI4 in )
	{
		if( w != in.w )
			return *this;
		I8 dz = 0, dy = 0;
		if( z < 0 )
			dz = -z;
		else if( z > gpe_giv_z1s )
			dz = z-gpe_giv_z1s;
		if( y < 0 )
			dz = -y;
		else if( y > gpe_giv_z1s )
			dy = y-gpe_giv_z1s;

		if( !(dz+dy) )
			return *this;
		
		GPCI4 v = *this - in;
		if( abs(v.z) > abs(v.y) )
			return gr_frame_z( in, v );

		return gr_frame_y( in, v );
	}
	GPCI4& gr_frame_z( GPCI4 in, GPCI4 v )
	{
		I8 b = v.z, a=b;
		if( b < 0 )
		{
			// balra megy
			if( z != 0 )
			{
				a = in.z;
				b *= -1;
			}
		}
		else if( b > 0 )
		{
			// jobbra tart
			if( z != gpe_giv_z1s )
				a = gpe_giv_z1s-in.z;
		}

		if( b != a )
		{
			*this = (v*a)/b + in;
			v = *this - in;
		}

		b = v.y, a=b;

		if( b < 0 )
		{
			// balra megy
			if( y < 0 )
			{
				a = in.y;
				b *= -1;
			}
		}
		else if( b > 0 )
		{
			// jobbra tart
			if( y > gpe_giv_z1s )
				a = gpe_giv_z1s-in.y;
		}
		
		if( v.y != a )
			*this = (v*a)/b + in;

		return *this;
	}
	GPCI4& gr_frame_y( GPCI4 in, GPCI4 v )
	{
		I8 b = v.y, a=b;

		if( b < 0 )
		{
			// balra megy
			if( y != 0 )
			{	
				a = in.y;	
				b *= -1;
			}
		}
		else if( b > 0 )
		{
			// jobbra tart
			if( y != gpe_giv_z1s )
				a = gpe_giv_z1s-in.y;
		}
		
		if( v.y != a )
		{
			*this = (v*a)/b + in;
			v = *this - in;
		}
		
		
		
		b = v.z, a=b;
		if( b < 0 )
		{
			// balra megy
			if( z < 0 )
			{
				a = in.z;	
				b *= -1;
			}
		}
		else if( b > 0 )
		{
			// jobbra tart
			if( z > gpe_giv_z1s )
				a = gpe_giv_z1s-in.z;
		}

		if( b != a )
			*this = (v*a)/b + in;

		return *this;
	}
	I8 gr2cr1m( bool b_sub )
	{
		x *= gpe_giv_1m;
		y *= gpe_giv_1m;
		z *= gpe_giv_1m;
		//x += gpe_giv_1m/8;
		//y += gpe_giv_1m/8;
		//z += gpe_giv_1m/8;
		switch( w )
		{
			case 0:
				x = y;
				// z jó
				y = 0;
				break;
			case 1:
				x = gpe_giv_z1m;
				// y jó
				// z jó
				break;
			case 2:
				x = gpe_giv_z1m-z;
				// y jó
				z = gpe_giv_z1m;
				break;
			case 3:
				x = gpe_giv_z1m-z;
				z = gpe_giv_z1m-y;
				y = gpe_giv_z1m;
				break;
			case 4:
				x = y;
				y = gpe_giv_z1m-z;
				z = gpe_giv_z1m-x;
				x = 0;
				break;
			default:
				y = gpe_giv_z1m-z;
				x = gpe_giv_z1m-y;
				z = 0;
		}
		if( b_sub )
			*this -= gpe_giv_lo1m; 
		w = 0;
		return b_sub;
	}
	
	GPCI4 gr2cr_1m( void ) const;
	//GPCI4 cr2gr_1m_o( GPCI4 air );
	GPCI4 cr2gr_1m_o( GPCI4 air );
	GPCI4 cr2gr_1m( GPCI4 air, bool b_dbg );
	GPCI4 cr2gr_sid_1m( GPCI4 air, char sid, bool b_dbg );
	GPCI4 cr2gr_sid_1m6( GPCI4 air, char sid, GPCV3& axyz );
	
	GPCV3 qube_ax_o( void )
	{
		I8 xx = x*x, yy = y*y, zz = z*z;
		double	xy = sqrt(xx+yy),
			yz = sqrt(yy+zz),
			xz = sqrt(xx+zz),
			pi = acos( -1.0 );
		GPCV3	ax(
					( acos( -(double)x/xy ) / pi )*gpe_giv_y1m - gpe_giv_z1m,
					( acos( -(double)y/yz ) / pi )*gpe_giv_y1m - gpe_giv_z1m,
					( acos( -(double)z/yz ) / pi )*gpe_giv_y1m - gpe_giv_z1m
				);
		return ax;
	}
	GPCV3 qube_ax( void )
	{
		// X
		double	qx = -x, 
			qy = -y, 
			qz = -z, 
			xx = qx*qx, yy = qy*qy, zz = qz*qz,

			rxy = sqrt(xx+yy),
			ryz = sqrt(yy+zz),
			rxz = sqrt(xx+zz),
			
			pi = acos(-1.0);

		GPCV3	ax(0,0,0 );


		if( rxy > rxz )
			ax.x = ( acos( qx/rxy ) / pi )*gpe_giv_y1m - gpe_giv_z1m;
		else
			ax.x = ( acos( qx/rxz ) / pi )*gpe_giv_y1m - gpe_giv_z1m;

		if( rxy > ryz )
			ax.y = ( acos( qy/rxy ) / pi )*gpe_giv_y1m - gpe_giv_z1m;
		else
			ax.y = ( acos( qy/ryz ) / pi )*gpe_giv_y1m - gpe_giv_z1m;

		if( ryz > rxz )
			ax.z = ( acos( qz/ryz ) / pi )*gpe_giv_y1m - gpe_giv_z1m;
		else
			ax.z = ( acos( qz/rxz ) / pi )*gpe_giv_y1m - gpe_giv_z1m;
		
		return ax;
	}

	

	GPCI4 gr2ar( I8 mul, I8 div ) const
	{
		// w -ben a ar-vektor hossza lesz
		if( !div )
			div = 1;

		GPCI4 ar;
		if( mul == 1 )
		{
			ar = gr2cr_1m();
		} else {
			GPCI4 gr = *this;
			gr *= GPCI4( mul, mul, mul, 1 );
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
	U1 n_RADgr( const GPCI4& p1, const GPCI4& p0, I8 div );
	GPCI4& ar2gr( const GPCI4& air, I8 mul, I8 div, bool b_dbg = false )
	{
		GPCI4 a = air * mul;
		cr2gr_1m( a, b_dbg );
		if( w > 5 )
			cr2gr_1m( a, true );

		if( div < 2 )
			return *this;

		GPCI4 err = *this % div;
		if( err.maxi_absabs_xyz() )
			round_xyz( div );
		
		*this /= GPCI4( div, div, div, 1 );
	}
	GPCI4& ar2gr_sid( GPCI4 air, I8 mul, I8 div, char sid, bool b_dbg = false )
	{
		air *= mul;
		cr2gr_sid_1m( air, sid, b_dbg );

		x = air.len_xyz()-gpe_giv_cr1m;
		
		GPCI4 err = *this % gpe_giv_1m;
		if( err.maxi_absabs_xyz() )
		{
			round_xyz( gpe_giv_1m );
		}
		
		*this /= GPCI4( div, div, div, 1 );
		return *this;	
	}
	GPCI4 sid_gr( I8 sid )
	{
		if( w == sid )
			return *this;
		GPCI4 gr;
		return gr.ar2gr_sid( gr2ar( gpe_giv_1m, 1 ), 1, gpe_giv_1m, sid );
	}
	GPCI4& nr_si_fr_dt( I8 mx, I8 ws1, I8 line );
	GPCI4& down( I8 i8 );
	GPCI4& up( I8 i8 );
	GPCI4& down_xyz( I8 i8 );
	GPCI4& down_xyz0( I8 i8 );
	GPCI4& round_xyz( I8 i8 );
	GPCI4& round_xyz0( I8 i8 );
	GPCI4& up_xyz( I8 i8 );
	GPCI4& up_xyz0( I8 i8 );
	GPCI4  mine_dir2vox( GPCI4& dir, I8 dl, I8 n, I8 dim );
	

	GPCI4	operator % (const I8 i8 ) const;
	//GPCI4	operator * (const I8 i8 ) const;
	long double	qlen(void) const;
	I8		qlen_xyz(void) const;
	I8		qlen_msk( GPCI4 msk ) const;
	//I8		qlen_xyzi8(void);
	I8		len(void) const;
	I8		len_xyz(void) const;
	I8		len_xy(void) const;
	I8		qlen_xy(void) const;
	I8		len_yz(void) const;
	GPCI4	len_air( I8 r ) const;
	I8		qlen_yz(void) const;
	I8 GPCI4::volume( I8 null = 1 )
	{
		I8 v = x ? x : null;
		v *= y ? y : null;
		v *= z ? z : null;
		v *= w ? w : null;
		return v;
	}
	I8 vox(void)
	{
		return x+(y<<4)+(z<<8);
	}
	I8 ddd( I8 d, I8 s  )
	{
		return x/s + (y/s)*d*d + (z/s)*d;
	}
	I8 ddd( I8 d )
	{
		return x + y*d*d + z*d;
	}
	I8 dddx10( void )
	{
		return x + y*0x100 + z*0x10;
	}
	GPCI4& a0_xyz0( I8 a0 )
	{
		x = a0%0x10;
		y = a0/0x100;
		z = (a0%0x100)/0x10;
		w = 0;
		return *this;
	}
	GPCI4& a0_xyz32( I8 a0 )
	{
		x = a0%0x20;
		y = a0/0x400;
		z = (a0%0x400)/0x20;
		w = 0;
		return *this;
	}
	GPCI4& a0_xyz48( I8 a0 )
	{
		x = a0%0x30;
		y = a0/0x900;
		z = (a0%0x900)/0x30;
		w = 0;
		return *this;
	}
	GPCI4& a0_xyz64( I8 a0 )
	{
		x = a0%0x40;
		y = a0/0x1000;
		z = (a0%0x1000)/0x40;
		w = 0;
		return *this;
	}

	
	GPCI4& a0_xyzWIDTH( I8 a0 )
	{
		x =  a0%GPE_W16;
		y =  a0/GPE_Wx;
		z = (a0%GPE_Wx)/GPE_W16;
		w = 0;
		return *this;
	}
	GPCI4& a0_xyz0( I8 a0, I8 _w )
	{
		w = _w*_w;
		x = a0%_w;
		y = a0/w;
		z = (a0%w)/_w;
		w = 0;
		return *this;
	}
	GPCI4& a0_xyz_s( I8 a0, I8 _w, I8 s )
	{
		w = _w*_w;
		x = (a0%_w) - s;
		y = (a0/w) - s;
		z = ((a0%w)/_w) - s;
		w = 0;
		return *this;
	}
	GPCI4 d2xyz( I8 d )
	{
		return GPCI4( d%x, (d%y)/x, d/y, 0);
	}
	GPCV3 d2v3( I8 d );
	GPCI4& max_w( void )
	{
		if( w < 0 )
			w *= -1;
		
		if( w & 0xf )
			w += 0x10-(w&0xf);

		if( x > w )
			x = w;
		else if( x < -w )
			x = -w;
		
		if( y > w )
			y = w;
		else if( y < -w )
			y = -w;
		
		if( z > w )
			z = w;
		else if( z < -w )
			z = -w;
		
		return *this;
	}
	
	GPCI4 xy( void )
	{
		return GPCI4( x, y, 0, 0 );
	}
	GPCI4 zw__( void )
	{
		return GPCI4( z, w, 0, 0 );
	}
	GPCI4 zwzw(void)
	{
		return GPCI4(z, w, z, w);
	}
	GPCI4 xyxy(void)
	{
		return GPCI4(x,y,x,y);
	}
	GPCI4 xyz( void )
	{
		return GPCI4( x, y, z, 0 );
	}
	GPCI4 yzwx( void )
	{
		return GPCI4( y, z, w, x );
	}
	
	GPCI4& xz( I8 i )
	{
		x = z = i;
		return *this;
	}
	GPCI4& yw( I8 i )
	{
		y = w = i;
		return *this;
	}

	GPCI4& xy( I8 _x, I8 _y )
	{
		x = _x;
		y = _y;
		return *this;
	}

	
	GPCI4& zw( I8 _x, I8 _y )
	{
		z = _x;
		w = _y;
		return *this;
	}
	GPCI4& zw( GPCI4& xy )
	{
		z = xy.x;
		w = xy.y;
		return *this;
	}
	
	GPCI4 zyx( void )
	{
		return GPCI4( z,y,x,w );
	}
	GPCI4& xyz( GPCV3 xyz )
	{
		x = xyz.x;
		y = xyz.y;
		z = xyz.z;
		w = 0;
		return *this;
	}
	GPCI4& pow_ab( I8 a, I8 b = 1 )
	{
		GPCI4 out = *this;
		double apb = (double)a/(double)b;
		out.x = pow( (double)x, apb );
		out.y = pow( (double)y, apb );
		out.z = pow( (double)z, apb );
		out.w = pow( (double)w, apb );
		
		return out;
	}
	
	// xy--
	// x-z-
	// x--w
	// -yz-
	// -y-w
	// --zw
	GPCI4& swap_xy( void )
	{
		I8 s = x;
		x = y;
		y = s;
		return *this;
	}
	GPCI4& swap_xz( void )
	{
		I8 s = x;
		x = z;
		z = s;
		return *this;
	}
	GPCI4& swap_xw( void )
	{
		I8 s = x;
		x = w;
		w = s;
		return *this;
	}
	GPCI4& swap_yz( void )
	{
		I8 s = y;
		y = z;
		z = s;
		return *this;
	}
	GPCI4 right_yz( void )
	{
		GPCI4 out;
		out.x = x;
		out.y = -z;
		out.z = y;
		out.w = w;
		return out;
	}
	GPCI4 left_yz( void )
	{
		GPCI4 out;
		out.x = x;
		out.y = z;
		out.z = -y;
		out.w = w;
		return out;
	}
	GPCI4 left_yz_norm( void )
	{
		GPCI4 out;
		out.x = x;
		out.y = z;
		out.z = -y;
		out.w = w;
		return out.norm_0yz0();
	}
	GPCI4& swap_yw( void )
	{
		I8 s = y;
		y = w;
		w = s;
		return *this;
	}
	GPCI4& swap_zw( void )
	{
		I8 s = z;
		z = w;
		w = s;
		return *this;
	}

	GPCI4 hilo_xyzw_ix( void )
	{
		GPCI4	out( 0,1,2,3 );
		I8	*p_4 = (I8*)this;
		if( p_4[out.x] < p_4[out.w] )
			out.swap_xw();
		if( p_4[out.y] < p_4[out.w] )
			out.swap_yw();
		if( p_4[out.z] < p_4[out.w] )
			out.swap_zw();
		if( p_4[out.x] < p_4[out.z] )
			out.swap_xz();
		if( p_4[out.y] < p_4[out.z] )
			out.swap_yz();
		if( p_4[out.x] < p_4[out.y] )
			out.swap_xy();
		return out;
	}
	GPCI4& swap_xyzw( GPCI4 idx )
	{
		I8 *p_4 = (I8*)this;
		GPCI4 out( p_4[idx.x],  p_4[idx.y],  p_4[idx.z],  p_4[idx.w] );
		*this = out;
		return *this;
	}
	GPCI4& swap_xyz( GPCI4 idx )
	{
		I8 *p_4 = (I8*)this;
		GPCI4 out( p_4[idx.x],  p_4[idx.y],  p_4[idx.z],  w );
		*this = out;
		return *this;
	}
	GPCI4 hilo_xyz_ix( void )
	{
		GPCI4	out( 0,1,2,3 );
		I8	*p_4 = (I8*)this;
		if( p_4[out.x] < p_4[out.z] )
			out.swap_xz();
		if( p_4[out.y] < p_4[out.z] )
			out.swap_yz();
		if( p_4[out.x] < p_4[out.y] )
			out.swap_xy();
		return out;
	}
	GPCI4 hilo_xyz_abs_ix( void )
	{
		GPCI4	out( 0,1,2,3 );
		I8	*p_4 = (I8*)this;
		if( abs(p_4[out.x]) < abs(p_4[out.z]) )
			out.swap_xz();
		if( abs(p_4[out.y]) < abs(p_4[out.z]) )
			out.swap_yz();
		if( abs(p_4[out.x]) < abs(p_4[out.y]) )
			out.swap_xy();
		return out;
	}
	GPCI4 hilo_abs_xyz_ix( void )
	{
		GPCI4	out( 0,1,2,3 );
		I8	*p_4 = (I8*)this;
		if( abs(p_4[out.x]) < abs(p_4[out.z]) )
			out.swap_xz();
		if( abs(p_4[out.y]) < abs(p_4[out.z]) )
			out.swap_yz();
		if( abs(p_4[out.x]) < abs(p_4[out.y]) )
			out.swap_xy();
		return out;
	}
	char sid2_1m( void )
	{
		I8 dz = 0, dy = 0;
		if( z > gpe_giv_z1m1s )
			dz = z-gpe_giv_z1m1s;
		else if( z < 0 )
			dz = -z;

		if( y > gpe_giv_z1m1s )
			dy = y-gpe_giv_z1m1s;
		else if( y < 0 )
			dy = -y;

		if( !(dz+dy) )
			return w;

		if( dz > dy )
		{
			if( z >= gpe_giv_z1m )
			switch( w )
			{
				case 0:
				case 1: return 2;
				case 2:
				case 3: return 4;
				case 4:
				case 5: return 0;
			}

			if( z < 0 )
			switch( w )
			{
				case 0:
				case 1: return 5;
				case 2:
				case 3: return 1;
				case 4:
				case 5: return 3;
			}
		}

		if( y >= gpe_giv_z1m )
		switch( w )
		{
			case 5: 
			case 0: return 1;
			case 1: 
			case 2: return 3;
			case 3: 
			case 4: return 5;
		}
		
		if( y < 0 )
		switch( w )
		{
			case 5:
			case 0: return 4;
			case 1:
			case 2: return 0;
			case 3:
			case 4: return 2;
		}

		return w;
	}

	GPCI4& lmh_xyz( void )
	{
		GPCI4 out = *this;

		if( out.x > y )
			 out.x = y;
		if( out.x > z )
			 out.x = z;

		if( out.z < x )
			 out.z = x;
		if( out.z < y )
			 out.z = y;

		out.y = (out.z+out.x)/2;
		return out;
	}
	GPCI4& lmh_xyz( GPCI4& in )
	{
		I8	h = in.maxi_xyz(),
			l = in.mini_xyz();

		if( x > l )
			 x = l;
		if( z < h )
			 z = h;

		y = (z+x)/2;
		return *this;
	}
	GPCI4& lo( I8 i8 )
	{
		if( x > i8 )
			x = i8;
		if( y > i8 )
			y = i8;
		if( z > i8 )
			z = i8;
		if( w > i8 )
			w = i8;
		return *this;
	}
	GPCI4& lo_xyz( I8 i8 )
	{
		if( x > i8 )
			x = i8;
		if( y > i8 )
			y = i8;
		if( z > i8 )
			z = i8;
		w = 0;
		return *this;
	}
	GPCI4& lo( const GPCI4&v4 )
	{
		if( x > v4.x )
			x = v4.x;
		if( y > v4.y )
			y = v4.y;
		if( z > v4.z )
			z = v4.z;
		if( w > v4.w )
			w = v4.w;
		return *this;
	}
	GPCI4& hi( I8 i8 )
	{
		if( x < i8 )
			x = i8;
		if( y < i8 )
			y = i8;
		if( z < i8 )
			z = i8;
		if( w < i8 )
			w = i8;
		return *this;
	}
	GPCI4& border_xyz( I8 i8 )
	{
		if( !i8 )
			return null();

		I8 j8 = maxi_absabs_xyz();
		if( j8 < i8 )
			return *this;

		*this *= i8;
		*this /= j8;
		w = 0;
		return *this;
	}
	GPCI4& border_yz( I8 i8 )
	{
		if( !i8 )
			return null();

		I8 j8 = maxi_absabs_yz();
		if( j8 <= i8 )
			return *this;

		*this *= i8;
		*this /= j8;
		w = 0;
		return *this;
	}

	
	GPCI4& hi_xyz( I8 i8 )
	{
		if( x < i8 )
			x = i8;
		if( y < i8 )
			y = i8;
		if( z < i8 )
			z = i8;
		w = 0;
		return *this;
	}
	GPCI4& hi( const GPCI4&v4 )
	{
		if( x < v4.x )
			x = v4.x;
		if( y < v4.y )
			y = v4.y;
		if( z < v4.z )
			z = v4.z;
		if( w < v4.w )
			w = v4.w;
		return *this;
	}

	GPCV2 xy_v2( void )
	{
		return GPCV2(x,y);
	}
	GPCV2 xy_v2( GPCV2 v2 )
	{
		x = v2.x;
		y = v2.y;
		return GPCV2(z,w);
	}
	ULL area_xy( void )
	{
		ULL o = x > 0 ? x : -x;
		if( y > 0 )
			o *= y;
		else
			o *= -y;
		return o;
	}
	ULL area_zw( void )
	{
		ULL o = z > 0 ? z : -z;
		if( w > 0 )
			o *= w;
		else
			o *= -w;
		return o;
	}
	GPCV2 zw_v2(void)
	{
		return GPCV2(z,w);
	}
	GPCV2 zw_v2( GPCV2 v2 )
	{
		z = v2.x;
		w = v2.y;
		return GPCV2(z,w);
	}

	GPCV3 xyz_v3( void )
	{
		return GPCV3(x,y,z);
	}
	GPCV4 xyz_v4( void )
	{
		return GPCV4(x,y,z,0);
	}
	GPCV3 xyz_norm( void )
	{
		long double l = len_xyz();

		long double xx = (long double)x/l;
		long double yy = (long double)y/l;
		long double zz = (long double)z/l;
		
		return GPCV3(xx,yy,zz);
	}
	GPCI4 norm_i4( void )
	{
		return GPCI4( !x ? 0 : x/abs(x), !y ? 0 : y/abs(y), !z ? 0 : z/abs(z), !w ? 0 : w/abs(w)  );
	}
	GPCI4 norm_0yz0_12( void )
	{
		I8 n = maxi_absabs_yz();
		if( !n )
			return 0;
		if( n > 1 )
		{
			n /= 2;
		}
		return GPCI4(	0,
					y/n ? y/abs(y) : 0,
					z/n ? z/abs(z) : 0,
					0 );
	}
	GPCI4 norm_0yz0_34( void )
	{
		I8 n = maxi_absabs_yz();
		if( !n )
			return 0;
		if( n > 1 )
		{
			n *= 3;
			n /= 4;
		}
		return GPCI4(	0,
					y/n ? y/abs(y) : 0,
					z/n ? z/abs(z) : 0,
					0 );
	}

	GPCI4 one_xy00( void )
	{
		I8 n = maxi_absabs_xy();
		if( !n )
			return 0;
		return GPCI4( x/n, y/n, 0, 0 );
	}
	GPCI4& turn_xyR(void)
	{
		return GPCI4(-1 * y, x, 0, 0);
	}
	GPCI4 right_xy00( void )
	{
		//  |
		// -+-> +x
		//  |\
		//  V
		//  +y
		// y le x jobbra
		if( x < 0 )
			return GPCI4( 0, -1, 0, 0 );
		else if( x > 0 )
			return GPCI4( 0, 1, 0, 0 );

		if( y < 0 )
			return GPCI4( 1, 0, 0, 0 );

		return GPCI4( -1, 0, 0, 0 );
	}
	GPCI4 left_xy00( void )
	{
		//  |
		// -+-> +x
		//  |\
		//  V
		//  +y
		// y le x jobbra
		if( x < 0 )
			return GPCI4( 0, 1, 0, 0 );
		else if( x > 0 )
			return GPCI4( 0, -1, 0, 0 );

		if( y < 0 )
			return GPCI4( -1, 0, 0, 0 );

		return GPCI4( 1, 0, 0, 0 );
	}

	GPCI4 norm_0yz0( void )
	{
		return GPCI4(	0,
					!y ? 0 : y/abs(y),
					!z ? 0 : z/abs(z),
					0 );
	}
	GPCI4 norm_xyz0( void )
	{
		return GPCI4(	
					!x ? 0 : x/abs(x),
					!y ? 0 : y/abs(y),
					!z ? 0 : z/abs(z),
					0 
				);
	}
	GPCI4 norm_xyz0_12( void )
	{
		I8 n = maxi_absabs_xyz();
		if( !n )
			return 0;
		if( n > 1 )
		{
			n /= 2;
		}
		return GPCI4( 
					x/n ? x/abs(x) : 0,
					y/n ? y/abs(y) : 0,
					z/n ? z/abs(z) : 0,
					0 
				);
	}
	GPCI4 norm_xyz0_34( void )
	{
		I8 n = maxi_absabs_xyz();
		if( !n )
			return 0;
		if( n > 1 )
		{
			n *= 3;
			n /= 4;
		}
		return GPCI4( 
					x/n ? x/abs(x) : 0,
					y/n ? y/abs(y) : 0,
					z/n ? z/abs(z) : 0,
					0 
				);
	}
	//GPCI4 norm_xyz34( void )
	//{
	//	I8 mx = maxi_absabs_xyz();
	//	if( mx < 2 )
	//		return GPCI4( !x ? 0 : x/abs(x), !y ? 0 : y/abs(y), !z ? 0 : z/abs(z), 0 );
	//
	//	
	//	GPCI4 out = GPCI4( x, y, z, 0 )*4 / mx;
	//	return GPCI4(	
	//			abs(out.x) < 3 ? 0 : out.x/abs(out.x), 
	//			abs(out.y) < 3 ? 0 : out.y/abs(out.y),
	//			abs(out.z) < 3 ? 0 : out.z/abs(out.z),	 0
	//		);
	//}
	GPC_AN xAN( void )
	{
		return GPC_AN(x,y);
	}
	GPC_AN zAN( void )
	{
		return GPC_AN(z,w);
	}
	GPCI4& xy( GPCI4& v )
	{
		x = v.x;
		y = v.y;
		return *this;
	}
	
	GPCI4& xy( I8 i )
	{
		x = y = i;
		return *this;
	}
	//GPCI4 xy(void)
	//{
	//	return GPCI4( x, y, 0, 0);
	//}
	GPCI4& yz( GPCI4& v )
	{
		y = v.y;
		z = v.z;
		return *this;
	}
	GPCI4& yz( I8 i )
	{
		y = z = i;
		return *this;
	}
	
	GPCI4 yz( void )
	{
		return GPCI4( 0, y, z, 0);
	}
	/*GPCI4 long_yz( I8 l )
	{
		return (GPCI4( 0, y, z, 0)*l)/maxi_absabs_yz();
	}
	GPCI4 long_xyz( I8 l )
	{
		return (GPCI4( x, y, z, 0)*l)/maxi_absabs_xyz();
	}
	GPCI4 long( I8 l )
	{
		return (GPCI4( 0, y, z, 0)*l)/maxi_absabs();
	}*/
	/*GPCI4& xz( I8 i )
	{
		x = z = i;
		return *this;
	}*/
	GPCI4 xz(void)
	{
		return GPCI4( x, 0, z, 0);
	}
	GPCI4& xyz( GPCI4& ixyz )
	{
		x = ixyz.x;
		y = ixyz.y;
		z = ixyz.z;
		return *this;
	}
	GPCI4& xyzw( GPCV4& vxyzw )
	{
		x = vxyzw.x;
		y = vxyzw.y;
		z = vxyzw.z;
		w = vxyzw.w;
		return *this;
	}
	GPCI4& xyz( I8 i )
	{
		x = y = z = i;
		return *this;
	}
	GPCI4& xyzw( I8 i )
	{
		x = y = z = w = i;
		return *this;
	}
	bool q_in_xyz( const GPCI4& _min, const GPCI4& _max ) const  
	{
		return (	   x >= _min.x && x <= _max.x
			&& y >= _min.y && y <= _max.y
			&& z >= _min.z && z <= _max.z );
	}

	

	I8 sum( void )
	{
		return x+y+z+w;
	}
	I8 sum_abs( void )
	{
		return abs(x)+abs(y)+abs(z)+abs(w);
	}
	I8 sum_xy( void )
	{
		return x+y;
	}
	I8 sum_xw(void)
	{
		return x + w;
	}
	I8 sum_yz( void )
	{
		return y+z;
	}
	I8 sum_xyz( void )
	{
		return x+y+z;
	}
	I8 sum_abs_xyz( void )
	{
		return abs(x)+abs(y)+abs(z);
	}
	I8 sum_abs_xy( void )
	{
		return abs(x)+abs(y);
	}
	I8 sum_abs_yz( void )
	{
		return abs(y)+abs(z);
	}
	I8 sum_abs_zw( void )
	{
		return abs(z)+abs(w);
	}
	I8 dot( const GPCI4& i4 ) const
	{
		return x*i4.x+y*i4.y+z*i4.z+w*i4.w;
	}
	I8 dot_xyz( const GPCI4& i4 ) const
	{
		return x*i4.x+y*i4.y+z*i4.z;
	}
	I8 cos255_xyz( const GPCI4& i4 ) const
	{
		I8	lT = len_xyz();
		if( !lT )
			return 0;  /// nagyon eltér míg nincs jobb ötlet egyenlõre legyen 90°
		I8	lI = i4.len_xyz();
		//i4 *= 255;
		//i4 /= lI;
		return dot_xyz((i4*255)/lI)/lT;
	}



	I8 dot_xy(const GPCI4& i4) const
	{
		return x * i4.x + y * i4.y;
	}
	I8 cosB16_xy(const GPCI4& i4, I8 lB = 0, I8 lI = 0) const
	{
		if(!lB)
		{
			// len BASE
			lB = len_xy();
			if(!lB)
				return 0;
		}

		if(!lI)
		{
			// len INPUT
			lI = i4.len_xy();
			if(!lI) 
				return 0;
		}
		//i4 *= 255;
		//i4 /= lI;
		I8 c = (dot_xy(i4) * 0x10000);
		return c / (lI*lB);
	}
	I8 AcosB16_xy(const GPCI4& u4, const GPCI4& r4, I8 pl = 0x10000) const
	{
		I8	r = r4.len_xy();
		if(!r)
			return 0;

		I8	c = cosB16_xy( u4 ),
			s = r4.cosB16_xy(u4, r);

		if(abs(c) >= 0xc000)
		{
			double	d = (acos((double)s / (double)65536) - PIp2) / PI * -(double)0x8000;  //asin(d);
			if(c > 0)
			{
				// fent
				if(d < 0)
					return pl + d;

				return d;
			}
			// lent

			if(d < 0)
				return pl - (d + 0x8000);

			return 0x8000 - d;
		}

		//if( c >= 0xc000)
		//{
		//	double	d = (double)s / (double)65536,
		//			c = (acos(d)-PIp2) / PI * -(double)0x8000;  //asin(d);
		//	//I8 a = c*float(0x8000);
		//	if( c < 0 )
		//		return pl + c;

		//	return c;
		//} else if(c <= -0xc000)
		//{
		//	double	d = (double)s / (double)65536,
		//			c = (acos(d) - PIp2 ) / PI * -(double)0x8000;  //asin(d);
		////I8 a = c*float(0x8000);
		//	if(c < 0)
		//		return pl + c;

		//	return c;

		//	return 0x8000;
		//}
		I8 a = (acos(float(c) / float(0x10000))*float(0x8000)) / PI;

		if(s < 0)
			a = pl - a;
		return a;
	}



	I8 cosB16_xyz( const GPCI4& i4 ) const
	{
		I8	lT = len_xyz();
		if( !lT )
			return 0;  /// nagyon eltér míg nincs jobb ötlet egyenlõre legyen 90°
		I8	lI = i4.len_xyz();
		//i4 *= 255;
		//i4 /= lI;
		I8 c = (dot_xyz(i4)* 0x10000 );
		return c/(lI*lT);
	}
	I8 AcosB16_xyz( const GPCI4& u4, const GPCI4& r4 , I8 pl = 0x10000 ) const
	{
		I8	c = cosB16_xyz( u4 ),
			s = r4.cosB16_xyz( u4 );
		if( c >= 0x10000)
		{
			double	d = (double)s/(double)65536,
					c = asin(d);
			I8 a = (c/PI)*float(0x8000);
			return a;
		}
		else if( c <= -0x10000)
		{
			return 0x8000;
		}	
		I8 a = (acos( float(c)/float(0x10000) )*float(0x8000))/PI;

		if( s < 0 )
			a = pl-a;
		return a;
	}
	
	GPCI4 base( const GPCI4& i4 ) const
	{
		GPCI4 o4 = *this;
		o4 *= (o4*i4).sum();
		o4 /= qlen_xyz();
		return o4;
	}
	GPCI4 base_xyz0( const GPCI4& i4 ) const
	{
		GPCI4 o4 = *this;
		o4 *= (o4*i4).sum_xyz();
		o4 /= qlen_xyz();
		o4.w = 0;
		return o4;
	}
	I8 b8( const GPCI4& i4 ) const
	{
		return (*this * i4).sum_xyz() / len_xyz();
	}
	I8 qalt_xyz( I8& bb, GPCI4 i4, I8 ql )
	{
		I8	dot = (*this * i4).sum_xyz(),
			aa = i4.qlen_xyz();
		GPCI4	B = (*this*dot)/ql,
				A = i4-B;
		bb = B.qlen_xyz();
		//bb = dot / ql;
		//bb *= bb;
		aa -= bb;
		if( dot < 0 )
			bb *= -1;
		return aa; //A.qlen_xyz();
	}
	GPCI4& sqrt_xyzw( void )
	{
		x = sqrt((double)x);
		y = sqrt((double)y);
		z = sqrt((double)z);
		w = sqrt((double)w);
		return *this;
	}
	GPCI4& sqrt_xyz( void )
	{
		x = sqrt((double)x);
		y = sqrt((double)y);
		z = sqrt((double)z);
		w = sqrt((double)w);
		return *this;
	}
	I8 maxi_abs(void)
	{
		I8 o = x;

		if( abs(o) < abs(y) )
			o = y;
		if( abs(o) < abs(z) )
			o = z;
		if( abs(o) < abs(w) )
			o = w;
		return o;
	}

	I8 maxi_absabs(void)
	{
		I8 o = abs(x);

		if( o < abs(y) )
			o = abs(y);
		if( o < abs(z) )
			o = abs(z);
		if( o < abs(w) )
			o = abs(w);
		return o;
	}
	I8 null_w( void )
	{
		I8 o = w;
		w = 0;
		return o;
	}
	GPCI4& null_zw( void )
	{
		z = w = 0;
		return *this;
	}
	I8 maxi_absabs_xyz(void)
	{
		I8 o = abs(x);

		if( o < abs(y) )
			o = abs(y);
		if( o < abs(z) )
			o = abs(z);
		
		return o;
	}
	
	I8 maxi_abs_xyz(void)
	{
		I8 o = x;

		if( abs(o) < abs(y) )
			o = y;

		if( abs(o) < abs(z) )
			return z;
		
		return o;
	}
	I8 maxi_abs_xy(void)
	{
		// ez megtartja az elöjelet
		if( abs(x) > abs(y) )
			return x;
		return y;
	}
	I8 maxi_absabs_xy(void)
	{
		//ez az értéket is absolut adja vissza
		I8 o = abs(x);
		if( o < abs(y) )
			return abs(y);
		return o;
	}
	I8 maxi_abs_yz(void)
	{
		if( abs(y) > abs(z) )
			return y;
		return z;
	}
	I8 maxi_absabs_yz(void)
	{
		I8 o = abs(y);
		if( o < abs(z) )
			o = abs(z);
		return o;
	}
	I8 maxi_absabs_xz(void)
	{
		I8 o = abs(x);
		if( o < abs(z) )
			return abs(z);
		return o;
	}
	I8 maxi_absabs_zw(void)
	{
		//ez az értéket is absolut adja vissza
		I8 o = abs(z);
		if (o < abs(w))
			return abs(w);
		return o;
	}

	GPCI4 long_yz0( I8 l )
	{
		I8 o = maxi_absabs_yz();
		if( o == l )
			return GPCI4( 0, y, z, 0);

		return (GPCI4( 0, y, z, 0)*l)/o;
	}
	GPCI4 long_xyz0( I8 l )
	{
		I8 o = maxi_absabs_xyz();
		if( o == l )
			return GPCI4( x, y, z, 0);

		return (GPCI4( x, y, z, 0)*l)/o;
	}
	GPCI4 long_i4( I8 l )
	{
		I8 o = maxi_absabs();
		if( o == l )
			return *this;

		return (GPCI4( x, y, z, w)*l)/o;
	}

	GPCI4 axVALID( I8 v )
	{
		I8	lT = maxi_absabs_yz(),
			lF = maxi_absabs_yz();
		GPCI4 out = *this;
		if( lT ? ((lF*v/lT) < 1) : true )
		{
			// a longban után legyen belölle legalább 1
			if( !lF)
			{
				if( !lT )
					lT = v;
				out.y = out.z = sqrt((lT*lT)/2);
			} else {
				out.y *= lT;
				out.y /= lF;
				out.z *= lT;
				out.z /= lF;
			}
		}
		return out.long_xyz0( v );
	}


	GPCI4& cut_box( GPCI4& p1, GPCI4& p0, GPCI4 l, GPCI4 h )
	{
		*this = p1-p0;
		GPCI4	lo( min(l.x,h.x), min(l.y,h.y), min(l.z,h.z), 0 ),
				hi( max(l.x,h.x), max(l.y,h.y), max(l.z,h.z), 0 );
		I8 a;
		if( x )
		{
			if( x < 0 )
			{
				if( hi.x > p1.x )
				{
					if( hi.x < p0.x )
					{
						a = hi.x - p0.x; // ez most negatív
						p0 += (*this*a)/x;
						*this = p1-p0;
					}

					if( lo.x > p1.x )
					{
						p1 = (*this*(lo.x-hi.x))/x;
						p1 += p0;
						*this = p1-p0;
					}
				}
			}
			else if( lo.x > p1.x )
			{
				if( lo.x > p0.x )
				{
					a = lo.x - p0.x; // ez most pozitív
					p0 += (*this*a)/x;
					*this = p1-p0;
				}

				if( hi.x < p1.x )
				{
					p1 = (*this*(hi.x-lo.x))/x;
					p1 += p0;
					*this = p1-p0;
				}
			}

		}

		if( y )
		{
			if( y < 0 )
			{
				if( hi.y > p1.y )
				{
					if( hi.y < p0.y )
					{
						a = hi.y - p0.y; // ez most negatív
						p0 += (*this*a)/y;
						*this = p1-p0;
					}

					if( lo.y > p1.y )
					{
						p1 = (*this*(lo.y-hi.y))/y;
						p1 += p0;
						*this = p1-p0;
					}
				}
			}
			else if( lo.y > p1.y )
			{
				if( lo.y > p0.y )
				{
					a = lo.y - p0.y; // ez most pozitív
					p0 += (*this*a)/y;
					*this = p1-p0;
				}

				if( hi.y < p1.y )
				{
					p1 = (*this*(hi.y-lo.y))/y;
					p1 += p0;
					*this = p1-p0;
				}
			}

		}

		if( z )
		{
			if( z < 0 )
			{
				if( hi.z > p1.z )
				{
					if( hi.z < p0.z )
					{
						a = hi.z - p0.z; // ez most negatív
						p0 += (*this*a)/z;
						*this = p1-p0;
					}

					if( lo.z > p1.z )
					{
						p1 = (*this*(lo.z-hi.z))/z;
						p1 += p0;
						*this = p1-p0;
					}
				}
			}
			else if( lo.z > p1.z )
			{
				if( lo.z > p0.z )
				{
					a = lo.z - p0.z; // ez most pozitív
					p0 += (*this*a)/z;
					*this = p1-p0;
				}

				if( hi.z < p1.z )
				{
					p1 = (*this*(hi.z-lo.z))/z;
					p1 += p0;
					*this = p1-p0;
				}
			}

		}

		return *this;
	}

	GPCI4& cut_box( GPCI4& p1, GPCI4& p0, I8 lo, I8 hi )
	{
		*this = p1-p0;
		I8 a;
		if( lo > hi )
		{
			a = hi;
			hi = lo;
			lo = a;
		}
		
		if( x )
		{
			if( x < 0 )
			{
				if( hi > p1.x )
				{
					if( lo > p1.x )
					{
						p1 = (*this*(lo-hi))/x;
						p1 += p0;
						*this = p1-p0;
					}

					if( hi < p0.x )
					{
						a = hi - p0.x; // ez most negatív
						p0 += (*this*a)/x;
						*this = p1-p0;
					}
				}
			}
			else if( lo > p1.x )
			{
				if( hi < p1.x )
				{
					p1 = (*this*(hi-lo))/x;
					p1 += p0;
					*this = p1-p0;
				}

				if( lo > p0.x )
				{
					a = lo - p0.x; // ez most pozitív
					p0 += (*this*a)/x;
					*this = p1-p0;
				}
			}
			
		}

		if( y )
		{
			if( y < 0 )
			{
				if( hi > p1.y )
				{
					if( lo > p1.y )
					{
						p1 = (*this*(lo-hi))/y;
						p1 += p0;
						*this = p1-p0;
					}

					if( hi < p0.y )
					{
						a = hi - p0.y; // ez most negatív
						p0 += (*this*a)/y;
						*this = p1-p0;
					}
				}
			}
			else if( lo > p1.y )
			{
				if( hi < p1.y )
				{
					p1 = (*this*(hi-lo))/y;
					p1 += p0;
					*this = p1-p0;
				}

				if( lo > p0.y )
				{
					a = lo - p0.y; // ez most pozitív
					p0 += (*this*a)/y;
					*this = p1-p0;
				}
			}

		}


		if( !z )
			return *this;

		
		if( z < 0 )
		{
			if( hi > p1.z )
			{
				if( lo > p1.z )
				{
					p1 = (*this*(lo-hi))/z;
					p1 += p0;
					*this = p1-p0;
				}

				if( hi < p0.z )
				{
					a = hi - p0.z; // ez most negatív
					p0 += (*this*a)/z;
					*this = p1-p0;
				}
			}
		}
		else if( lo > p1.z )
		{
			if( hi < p1.z )
			{
				p1 = (*this*(hi-lo))/z;
				p1 += p0;
				*this = p1-p0;
			}

			if( lo > p0.z )
			{
				a = lo - p0.z; // ez most pozitív
				p0 += (*this*a)/z;
				*this = p1-p0;
			}
		}

		return *this;
	}


	GPCI4 ins_x( GPCI4 d, I8 h )
	{
		if( x == h )
			return *this;

		if( !d.x )
		{
			I8 mx = d.maxi_absabs_yz();
			if( !mx )
			{
				d.yz( 0x7FFFffffFFFFffff );
				d.x = h;
				return d;
			}
			mx = 0x7FFFffffFFFFffff/mx;
			d *= mx;
			d.x = h;
			return d;	
		}

		I8 a = (h-x);
		
		d.y *= a;
		d.y /= d.x;
		d.y += y;

		d.z *= a;
		d.z /= d.x;
		d.z += z;

		d.x = h;
		return d;
	}
	GPCI4 ins_y( GPCI4 d, I8 h )
	{
		if( y == h )
			return *this;

		if( !d.y )
		{
			I8 mx = d.maxi_absabs_xz();
			if( !mx )
			{
				d.xz( 0x7FFFffffFFFFffff );
				d.y = h;
				return d;
			}
			mx = 0x7FFFffffFFFFffff/mx;
			d *= mx;
			d.y = h;
			return d;	
		}

		I8 a = (h-y);
		d.x *= a;
		d.x /= d.y;
		d.x += x;

		d.z *= a;
		d.z /= d.y;
		d.z += z;
		
		d.y = h;

		return d;
	}
	GPCI4 ins_z( GPCI4 d, I8 h )
	{
		if( z == h )
			return *this;

		if( !d.z )
		{
			I8 mx = d.maxi_absabs_xy();
			if( !mx )
			{
				d.xy( 0x7FFFffffFFFFffff );
				d.z = h;
				return d;
			}
			mx = 0x7FFFffffFFFFffff/mx;
			d *= mx;
			d.z = h;
			return d;	
		}

		I8 a = (h-z);
		d.x *= a;
		d.x /= d.z;
		d.x += x;

		d.y *= a;
		d.y /= d.z;
		d.y += y;
		
		d.z = h;
		return d;
	}

	GPCI4 insec_gr( GPCI4 p,  GPCI4 d, I8 half )
	{
		
		GPCI4 i = ins_x( d, p.x + ( (p.x>x) ? -half : half ) );
		i.w = (i-p).maxi_absabs_xyz();
		if( i.w <= half )
			return i;

		GPCI4 ii = ins_y( d, p.y + ( (p.y>y) ? -half : half ) );
		ii.w = (ii-p).maxi_absabs_xyz();
		if( ii.w <= half )
			return ii;
		
		if( i.w < ii.w )
			i = ii;

		ii = ins_z( d, p.z + ( (p.z>z) ? -half : half ) );
		ii.w = (ii-p).maxi_absabs_xyz();
		if( ii.w <= half )
			return ii;

		if( i.w < ii.w )
			return ii;
		return i;
	}
	GPCI4 ins_45dgr_xy( GPCI4 trg )
	{
		I8	x0 = x,
			y0 = y,
			x1 = trg.x,
			y1 = trg.y;

		if( x0 < 0 )
		{
			x0 *= -1;
			x1 *= -1;
		}
		if( y0 < 0 )
		{
			y0 *= -1;
			y1 *= -1;
		}

		I8	y2 = y0-y1,
			x2 = x1-x0;

		if( !(x2+y2) )
			return GPCI4( 0, 0, 0, 0 );
		y1 += y2-x0;
		return GPCI4( x2+y2, y1, y1, 0 );		
	}
	GPCI4 ins_45dgr_yz( GPCI4 trg )
	{
		I8	z0 = z,
			y0 = y,
			z1 = trg.z,
			y1 = trg.y;

		if( z0 < 0 )
		{
			z0 *= -1;
			z1 *= -1;
		}
		if( y0 < 0 )
		{
			y0 *= -1;
			y1 *= -1;
		}

		I8	y2 = y0-y1,
			z2 = z1-z0;

		if( !(z2+y2) )
			return GPCI4( 0, 0, 0, 0 );
		z1 += z2-y0;	
		return GPCI4( y2+z2, z1, z1, 0 );	
	}
	GPCI4 ins_45dgr_xz( GPCI4 trg )
	{
		I8	z0 = z,
			x0 = x,
			z1 = trg.z,
			x1 = trg.x;

		if( z0 < 0 )
		{
			z0 *= -1;
			z1 *= -1;
		}
		if( x0 < 0 )
		{
			x0 *= -1;
			x1 *= -1;
		}

		I8	x2 = x0-x1,
			z2 = z1-z0;

		if( !(z2+x2) )
			return GPCI4( 0, 0, 0, 0 );
		z1 += z2-x0;	
		return GPCI4( x2+z2, z1, z1, 0 );	
	}
	GPCI4	ins_45dgr( GPCI4& trg_gr, GPCI4& aft_gr, GPCI4& bef_gr, GPCI4& pos_gr, GPCI4 trg_ar, I8 dim, bool b_dbg )  const;
	
	GPCI4&	ai_QyzSID( I8 n );
	GPCI4&	ai_QxQ4( void );
	U4	gr2cnt( I8 div );
	GPCI4 qub( GPCI4 p, I8 half )
	{
		// this a dir
		GPCI4 o; // = *this;
		U1 n_in = 0;
		I8 mul, div;
		o.w = 0;
		if( x )
		{
			if( x > 0 )
			{
				mul = p.x-half;
				if( mul < 0 )
				{
					// belül van,
					// mert negatív lett
					mul = 0;
				}
			}
			else if( x < 0 )
			{
				mul = p.x+half;
				if( mul > 0 )
				{
					// belül van 
					// mert pozítív lett
					mul = 0;
				}
			}
			
			o.x = (x*mul)/x;
			o.y = (y*mul)/x;
			o.z = (z*mul)/x;
			o.w = (o-p).maxi_absabs_xyz();
			if( o.w <= half )
				return o;
		}

		if( y )
		{
			if( y > 0 )
			{
				mul = p.y-half;
				if( mul < 0 )
				{
					// belül van,
					// mert negatív lett
					mul = 0;
				}
			}
			else if( y < 0 )
			{
				mul = p.y+half;
				if( mul > 0 )
				{
					// belül van 
					// mert pozítív lett
					mul = 0;
				}
			}
		
			o.x = (x*mul)/y;
			o.y = (y*mul)/y;
			o.z = (z*mul)/y;
			o.w = (o-p).maxi_absabs_xyz();
			if( o.w <= half )
				return o;
		}
		
		if( z )
		{
			if( z > 0 )
			{
				mul = p.z-half;
				if( mul < 0 )
				{
					// belül van,
					// mert negatív lett
					mul = 0;
				}
			}
			else if( y < 0 )
			{
				mul = p.z+half;
				if( mul > 0 )
				{
					// belül van 
					// mert pozítív lett
					mul = 0;
				}
			}
		
			o.x = (x*mul)/z;
			o.y = (y*mul)/z;
			o.z = (z*mul)/z;
			o.w = (o-p).maxi_absabs_xyz();
			if( o.w <= half )
				return o;
		}

		return o;
	}

	GPCI4& best_xyz( void )
	{
		if( abs(x) < abs(y)  )
		{
			x = 0;
			if( abs(z) < abs(y)  )
				z = 0;
			else 
				y = 0;
		} else {
			y = 0;
			if( abs(z) <= abs(x)  )
				z = 0;
			else 
				x = 0;
		}
		return *this;
	}
	I8 maxi(void)
	{
		I8 o = x;
		if( o < y )
			o = y;
		if( o < z )
			o = z;
		if( o < w )
			o = w;
		return o;
	}
	I8 maxi_xyz(void)
	{
		I8 o = x;
		if( o < y )
			o = y;
		if( o < z )
			o = z;
		return o;
	}
	I8 mini(void)
	{
		I8 o = x;
		if( o > y )
			o = y;
		if( o > z )
			o = z;
		if( o > w )
			o = w;
		return o;
	}
	I8 mini_xyz(void)
	{
		I8 o = x;
		if( o > y )
			o = y;
		if( o > z )
			o = z;
		return o;
	}
	I8 maxii( I8 i )
	{
		I8 o = maxi();
		if( x > i )
			x = i;
		if( y > i )
			y = i;
		if( z > i )
			z = i;
		if( w > i )
			w = i;
		return o;
	}
	I8 minii( I8 i )
	{
		I8 o = maxi();
		if( x < i )
			x = i;
		if( y < i )
			y = i;
		if( z < i )
			z = i;
		if( w < i )
			w = i;
		return o;
	}

	I8 mini4( const GPCI4& i4 )
	{
		// a megadott i4 vektorol az alacsonyabb értékeket eltárolja ide
		if( x > i4.x )
			x = i4.x;
		if( y > i4.y )
			y = i4.y;
		if( z > i4.z )
			z = i4.z;
		if( w > i4.w )
			w = i4.w;
		return maxi();
	}
	I8 maxi4( const GPCI4& i4 )
	{
		// a megadott i4 vektorol az magasabb értékeket eltárolja ide
		if( x < i4.x )
			x = i4.x;
		if( y < i4.y )
			y = i4.y;
		if( z < i4.z )
			z = i4.z;
		if( w < i4.w )
			w = i4.w;
		return mini();
	}

	GPC_COLOR4& bgra_fast( void )
	{
		return GPC_COLOR4( (U1)z, y, x, w );
	}
	GPC_COLOR4 bgra( void )
	{
		U1 b = min( max( 0, x ), 255 );
		U1 g = min( max( 0, y ), 255 );
		U1 r = min( max( 0, z ), 255 );
		U1 a = min( max( 0, w ), 255 );
		return GPC_COLOR4( r,g,b,a );
	}
	GPCI4& ddd_444( I8 n_id, I8 n_cnt, I8 n_size );
	GPCI4& ddd_8444( I8 n_cnt, I8 n_size );

	I8 lurd_w(void)
	{
		return z-x;
	}
	I8 lurd_h(void)
	{
		return w-y;
	}

	GPCI4& lurd_cut( GPCI4 xy1xy2 )
	{
		I8 tmp;
		if( x > z )
		{
			tmp = z;
			z = x;
			x = tmp;
		}
		if( y > w )
		{
			tmp = w;
			w = y;
			y = tmp;
		}

		bool	b_flip_x = xy1xy2.z < xy1xy2.x,
				b_flip_y = xy1xy2.w < xy1xy2.y;

		// FLIP --------------------------------------
		if( b_flip_x )
		{
			tmp = xy1xy2.z;
			xy1xy2.z = xy1xy2.x;
			xy1xy2.x = tmp;
		}
		if( xy1xy2.z < x )
			goto error;
		if( xy1xy2.x >= z )
			goto error;
	
		if( b_flip_y )
		{
			tmp = xy1xy2.w;
			xy1xy2.w = xy1xy2.y;
			xy1xy2.y = tmp;
		}
		if( xy1xy2.w < y )
			goto error;
		if( xy1xy2.y >= w )
			goto error;

		tmp = xy1xy2.z-xy1xy2.x;
		if( !tmp )
		{
			if( xy1xy2.x < x || xy1xy2.x >= z )
				goto error;
		}

		//dir_wh.y = pix_xy1xy2.w-pix_xy1xy2.y;
		if( !xy1xy2.lurd_h() )
		{
			if( !tmp )
				goto error;

			if( xy1xy2.y < y || xy1xy2.y >= w )
				goto error;
		}

		GPCI4 dir_wh( xy1xy2.lurd_w(), xy1xy2.lurd_h(), 0, 0 ) ;
		// X --------------------------------------
		if( xy1xy2.x < x )
		{
			if( xy1xy2.z <= x )
				goto error;

			tmp = x-xy1xy2.x;
			dir_wh.y *= tmp;
			dir_wh.y /= dir_wh.x;

			dir_wh.x -= tmp;
			xy1xy2.y = xy1xy2.w-dir_wh.y;
			xy1xy2.x = x;
		}

		if( xy1xy2.z > z )
		{
			tmp = z-xy1xy2.x;
			dir_wh.y *= tmp;
			dir_wh.y /= dir_wh.x;
			dir_wh.x = tmp;
						
			xy1xy2.w = xy1xy2.y+dir_wh.y;
			xy1xy2.z = z;
		}

		// X --------------------------------------
		if( xy1xy2.y < y )
		{
			if( xy1xy2.w <= y )
				goto error;

			tmp = y-xy1xy2.y;
			dir_wh.x *= tmp;
			dir_wh.x /= dir_wh.y;

			dir_wh.y -= tmp;
			xy1xy2.x = xy1xy2.z-dir_wh.x;
			xy1xy2.y = y;
		}

		if( xy1xy2.w > w )
		{
			tmp = w-xy1xy2.y;
			dir_wh.x *= tmp;
			dir_wh.x /= dir_wh.y;
			dir_wh.y = tmp;
						
			xy1xy2.z = xy1xy2.x+dir_wh.x;
			xy1xy2.w = w;
		}
		
		
		// UN FLIP --------------------------------------
		if( b_flip_x )
		{
			tmp = xy1xy2.z;
			xy1xy2.z = xy1xy2.x;
			xy1xy2.x = tmp;
		} 
		if( b_flip_y )
		{
			tmp = xy1xy2.w;
			xy1xy2.w = xy1xy2.y;
			xy1xy2.y = tmp;
		}

		return xy1xy2;
	error:
		xy1xy2.null();
		return xy1xy2;
	}
	
	I8 find_IX( I8 id, I8 n )
	{
		if( !this )
			return n;

		GPCI4* p_root = this;
		I8 i = 0;
		while( i < n )
		{
			if( id == p_root[i].x )
				return i;
			if( id < p_root[i].x )
			{
				if( p_root[i].z < 0 )
					return i; // fa vége ez lesz a MOM

				// megy balra
				i = p_root[i].z;
				continue;
			}

			if( p_root[i].w < 0 )
				return i;  // fa vége ez lesz a MOM

			// megy jobbra
			i = p_root[i].w;
		}
		return n;
	}
	I8 add_IX( U8 id, I8 m, I8& n )
	{
		GPCI4	*p_root = this,
				*p_ix = p_root+n;

		p_ix->x = id;
		if( !n )
		{
			p_ix->z = p_ix->w = -1;
			return 1;
		}
		
		if( m > n )
			m = 0;
		else if( n < 0 )
			m = 0;

		while( m < n )
		{
			if( id == p_root[m].x )
				return n;

			if( id < p_root[m].x )
			{
				if( p_root[m].z < 0 )
				{
					p_root[m].z = n;
					p_ix->y = m;		// mom
					p_ix->z = p_ix->w = -1;
					n++;
					break; 
				}
				// megy balra
				m = p_root[m].z;
				continue;
			}

			if( p_root[m].w < 0 )
			{
				p_root[m].w = n;
				p_ix->y = m;			// mom
				p_ix->z = p_ix->w = -1;
				n++;
				break; 
			}
			// megy jobbra
			m = p_root[m].w;
		}
		return n;
	}
};



class GPC_CAT
{
public:
	GPCI4	origo,	 rU,
			rR,		 degree,
			left,	 leftOUT, //hi,
			rightIN, right;
	GPC_CAT(){};

	GPC_CAT& init( GPCI4& UP, GPCI4 o, GPCI4& bug )
	{
		GPM_CLR;
		origo = o;
		left = bug;
		degree = 0;
		rU = left - origo;
		rR = rU.turn_xyR();
		GPCI4 RIG = UP.turn_xyR();
		rU.z = (UP.AcosB16_xy(rU, RIG, 0) * 18000) / 0x8000;
		return *this;
	}

	I8 deg(GPCI4& bug)
	{
		GPCI4 r = bug - origo;
		I8 deg16 = (rU.AcosB16_xy(r, rR, 0) * 18000) / 0x8000;
		if(degree.x > deg16)
			degree.x = deg16;
		if(degree.w < deg16)
			degree.w = deg16;
		return deg16;
	}
	GPC_CAT& null( U4 n = 0 )
	{
		if( n > 1 )
		{
			GPM_ZEROn(this, n);
			return *this;
		}
		GPM_CLR;
		return *this;
	}
	GPC_CAT& operator += ( GPC_CAT& b )
	{
		origo	+= b.origo;
		rU		+= b.rU;
		rR		+= b.rR;
		degree	+= b.degree;
		left	+= b.left;
		leftOUT += b.leftOUT;
		rightIN += b.rightIN;
		right	+= b.right;
		return *this;
	}
	GPC_CAT& operator /= ( I8 b )
	{
		if( !b || b == 1 )
			return *this;

		origo	/= b;
		rU		/= b;
		rR		/= b;
		degree	/= b;
		left	/= b;
		leftOUT /= b;
		rightIN /= b;
		right	/= b;
		return *this;
	}
};

class GPC_CAT2
{
public:
	GPCI4	origo, rU,
			rR, degree,
			enter, enterOUT, //hi,
			offIN, off;
	GPC_CAT2( void )
	{
		GPM_CLR;
	};

	GPC_CAT2& init(GPCI4& UP, GPCI4 o, GPCI4& bug)
	{
		GPM_CLR;
		origo = o;
		enter = bug;
		degree = 0;
		rU = enter - origo;
		rR = rU.turn_xyR();
		GPCI4 RIG = UP.turn_xyR();
		rU.z = (UP.AcosB16_xy(rU, RIG, 0) * 18000) / 0x8000;
		return *this;
	}

	I8 deg(GPCI4& bug)
	{
		GPCI4 r = bug - origo;
		I8 deg16 = (rU.AcosB16_xy(r, rR, 0) * 18000) / 0x8000;
		if(degree.x > deg16)
			degree.x = deg16;
		if(degree.w < deg16)
			degree.w = deg16;
		return deg16;
	}

	GPC_CAT2& init_base(GPCI4& DIR, GPCI4 o, GPCI4& bug)
	{
		GPM_CLR;
		origo = o;
		enter = bug;
		degree = 0;
		rU = DIR;
		rR = rU.turn_xyR();
		GPCI4 RIG = DIR.turn_xyR();
		rU.z = 0;
		return *this;
	}

	GPCI4 alt(GPCI4& bug)
	{
		GPCI4	r = bug - enter,
				base = rU.base( r ),
				alt = r-base;
		I8 b = base.len_xy() * (rU.cosB16_xy(base) < 0 ? -1 : 1), a;
		
		if(degree.x > b)
			degree.x = b;
		if(degree.w < b)
			degree.w = b;
		a = alt.len_xy() * (rR.cosB16_xy(alt) < 0 ? -1 : 1);
		return GPCI4( b, a, 0, 0 );
	}

	GPC_CAT2& null(U4 n = 0)
	{
		if(n > 1)
		{
			GPM_ZEROn(this, n);
			return *this;
		}
		GPM_CLR;
		return *this;
	}
	GPC_CAT2& operator += (GPC_CAT2& b)
	{
		origo += b.origo;
		rU += b.rU;
		rR += b.rR;
		degree += b.degree;
		enter += b.enter;
		enterOUT += b.enterOUT;
		offIN += b.offIN;
		off += b.off;
		return *this;
	}
	GPC_CAT2& operator /= (I8 b)
	{
		if(!b || b == 1)
			return *this;

		origo /= b;
		rU /= b;
		rR /= b;
		degree /= b;
		enter /= b;
		enterOUT /= b;
		offIN /= b;
		off /= b;
		return *this;
	}
};
//#define GPD_PACK_DEBUG
//#define GPD_PACK_RLE

#define GPD_N_PACK	0x4000
#define GPD_N_HISTI	0x100
#define GPD_N_POS	GPD_N_HISTI
#define GPD_N_GOTO	GPD_N_HISTI
#define GPD_N_REAL	GPD_N_PACK
#define GPD_N_CODE	GPD_N_PACK
#define GPD_N_GPCW	GPD_N_PACK
#define GPD_N_DICK	GPD_N_PACK
#define GPD_N_RLE	GPD_N_PACK
#define GPD_U2_OFF	0xffff



GPM_PCK( 1,
	struct GPS_NN
	{
		U1	n;
		U2	nn;
	};
);

U1*	memmem( U1* p_d, ULL n_d, U1* p_s, ULL n_s );
U2	gpf_mem2rle( U1* p_buff, U1* p_src, U2 n_src, bool b_filter = true );
U2	gpf_rle2mem( U1* p_buff, U1* p_rle, U2 n_r );
U2	gpf_mem2left( U1* p_buff, U1* p_mem, U2 n_byte );
U2	gpf_left2mem( U1* p_buff, U1* p_left, U2 n_byte );
ULL	gpf_U2toULL( U2* p_h );
U1*	gpf_ULLtoABC( U1* p_abc, ULL ull, U1 add );

class GPCW
{
	public:
		U2	p, n, hi,
			m, l, r;
	
	GPCW* reset( void )
	{
		GPM_CLR;
		//p = hi = m = 0; 
		n = 1;
		//l = r = 0x4000;
		return this;
	}

	U2	add( U1* p_base, GPCW& w );
	U2	find( U1* p_src, GPCW& w );
	
	U2 init( U2* p_goto, U2* p_pos, U2* p_histi )
	{
		U2 n_w = 0, i;
		GPCW* p_w = this;

		for( i = 0; i < GPD_N_HISTI; i++ )
		{
			if( !p_histi[i] )
				continue;
					
			p_goto[i] = n_w;
			p_w[n_w].reset();
			p_w[n_w].m = 0;
			p_w[n_w].p = p_pos[i];
			n_w++;
			i++;
			break;
		}
		for( ; i < GPD_N_HISTI; i++ )
		{
			if( !p_histi[i] )
				continue;
					
			p_goto[i] = n_w;
			p_w[n_w].reset();
			p_w[p_w[n_w].m = n_w-1].r = n_w;
			p_w[n_w].p = p_pos[i];
			n_w++;
		}
		return n_w;
	}
	#ifdef GPD_PACK_DEBUG
	U1*	dec( U1* p_work, U2 n_w, U2* p_goto, U1* p_code, U2 n_code, U1* p_chk );
	#else
	U1*	dec( U1* p_work, U2 n_w, U2* p_goto, U1* p_code, U2 n_code );
	#endif
	U2* enc( U2* p_code, U2* p_goto, U1* p_s, U2 n_s, U2& n_w )
	{
		GPCW* p_w = this;
		U2 code, n_add = n_w;
		U2 i = 0;
		while( i < n_s ) // i++ )
		{
				p_w[n_w].p = i;
				p_w[n_w].n = n_s-p_w[n_w].p;
				p_w[n_w].l = p_w[n_w].r = 0;
				p_w[n_w].m = p_goto[p_s[i]];

				code = p_w->find( p_s, p_w[n_w] );
				if( n_w > n_add ) // most lett hozzáadva 
				if( code == n_add )
				{
					code = p_goto[p_s[i]];
				}
				if( p_w[code].p + p_w[code].n < n_s )
					p_w[n_w].n = p_w[code].n+1;
				p_w[n_w].m = code;
				n_add = n_w;
				n_w = p_w->add( p_s, p_w[n_w] );
				
				i += p_w[code].n;
				if( !p_code )
					continue;

				p_w[code].hi++;
				*p_code = code;
				p_code++;
		}

		return p_code;
	}

	U2* build( U2* p_code, U2* p_goto, U1* p_s, U2 n_s, U2& n_w, U2 n_break )
	{
		GPCW* p_w = this;
		U2 code;
		for( U2 i = 0; i < n_s; i++ )
		{
				p_w[n_w].p = i;
				p_w[n_w].n = n_s-p_w[n_w].p;
				p_w[n_w].l = p_w[n_w].r = 0;
				p_w[n_w].m = p_goto[p_s[i]];

				code = p_w->find( p_s, p_w[n_w] );
				if( code >= n_w )
				{
					p_w[n_w].p = i;
					p_w[n_w].n = n_s-p_w[n_w].p;
					p_w[n_w].l = p_w[n_w].r = 0;
					p_w[n_w].m = p_goto[p_s[i]];

					code = p_w->find( p_s, p_w[n_w] );
				}
				if( n_break )
				if( p_w[code].n > n_break )
						return p_code;

				if( p_w[code].p + p_w[code].n < n_s )
					p_w[n_w].n = p_w[code].n+1;
				p_w[n_w].m = code;
				n_w = p_w->add( p_s, p_w[n_w] );
				
				i += p_w[code].n-1;
				if( !p_code )
					continue;

				p_w[code].hi++;
				*p_code = code;
				p_code++;
		}

		return p_code;
	}
};
enum GPE_PACK
{
	GPE_PACK_INIT	= GPD_N_HISTI/8,
	GPE_PACK_HISTI	= GPD_N_HISTI*sizeof(U2),
	GPE_PACK_POS	= GPE_PACK_HISTI,
	GPE_PACK_GOTO	= GPE_PACK_HISTI,
	GPE_PACK_PACK	= GPD_N_PACK,
	GPE_PACK_RLE	= GPE_PACK_PACK,
	GPE_PACK_CODE	= GPE_PACK_PACK*sizeof(U2),
	GPE_PACK_GPW	= GPE_PACK_PACK*sizeof(GPCW),
	
	GPE_PACK_DEC_BUFF = 
							GPE_PACK_PACK
						#ifdef GPD_PACK_RLE
						+	GPE_PACK_RLE
						#endif
						+	GPE_PACK_HISTI+GPE_PACK_POS+GPE_PACK_GOTO
						+	GPE_PACK_GPW,
	GPE_PACK_ENC_BUFF = 
						 GPE_PACK_INIT
						+GPE_PACK_CODE
						+GPE_PACK_DEC_BUFF,
	GPE_BAG_BUFF =	max( GPD_BAG_BUFF, GPE_PACK_ENC_BUFF ),
};


class GPC_FIX
{
public:
	char*	fix_p_alloc;
	ULL		fix_n_alloc,
			fix_sw64,
			fix_ver;

	U2*	fix_histi( U2* p_pos, U2* p_histi, U1* p_s, U2 n, bool b_rst )
	{
		if( !p_histi )
			p_histi = new U2[GPD_N_HISTI];

		if( b_rst )
		{
			gp_zero( p_histi, GPD_N_HISTI*sizeof(*p_histi) );
			if( p_pos )
				gp_zero( p_pos, GPD_N_HISTI*sizeof(*p_pos) );
		}
		if( !p_pos )
		{
			for( U2 i = 0; i < n; i++ )
			{
				p_histi[p_s[i]]++;
			}
			return p_histi;
		}

		for( U2 i = 0; i < n; i++ )
		{
			if( !p_histi[p_s[i]] )
				p_pos[p_s[i]] = i;

			p_histi[p_s[i]]++;
		}
		return p_histi;
	}

	GPC_FIX*	fix_16k_dec( U1* p_buff, void* p_byte, ULL n_byte, ULL& s, ULL e, U1* p_chk );
	GPC_FIX*	fix_16k_enc( U1* p_buff, void* p_byte, ULL n_byte, ULL& s, ULL e );
	
	GPC_FIX*	fix_format( char* p_format, ... );
	GPC_FIX*	fix_format( ULL& start, ULL end, char* p_format, ... );
	GPC_FIX*	fix_expand( ULL& start, ULL end, ULL n_exp);
	U2	fix_nop( U1* p_buff, U1* p_src, U2 n_src, U1 op = GPE_BAG_CPY );
	U2	fix_rle( U1* p_buff, U1* p_src, U2 n_src, U1 op = GPE_BAG_CPY );
	
	U2 fix_bit( U1* p_out, GPC_IIx64* p_code_list, U1* p_idx_map, U1* p_src, U2 n_src, U1 op, U1 op2 = GPE_BAG_BIT )
	{
		// HEAD
		p_out[0] = op;					// 1
		p_out[1] = op2;					// 2
		U2	*p_size = (U2*)(p_out+2);	// 3,4

		*p_size = p_code_list->bit_encoder4( (U1*)(p_size+1), p_idx_map, p_src, n_src );
		
		return *p_size+4;
	}
	

	
	
	GPC_FIX*	fix_bag_encode( U1* p_pub_buff, ULL& n_start, ULL e, void* p_byte, ULL n_byte, ULL ver, bool b_dbg );
	GPC_FIX*	fix_bag_encode_v8( U1* p_pub_buff, ULL& n_start, ULL e, void* p_byte, ULL n_byte, ULL ver, bool b_dbg );
	GPC_FIX*	fix_bgra2pack_v2( U1* p_pub_buff, ULL& n_start, void* p_byte, ULL n_byte, bool b_dbg );

	GPC_FIX*	fix_bag_decode( U1* p_buff, ULL& n_start, ULL e, void* p_byte, ULL n_byte, GPE_ALFA alfa = GPE_ALFA_zero );
	

	GPC_FIX*	fix_insert( char* p_str, ULL n_load, ULL& n_start, ULL n_end );
	GPC_FIX*	fix_rn( ULL& s, char r, char n )
	{
		if( !this || !r || !n )
			return NULL;

		if( s > fix_n_alloc )
		{
			s = fix_n_alloc;
			return this;
		}
		//char s_r[] = " ";
		//*s_r = r;
		for( ULL ss = s; ss < fix_n_alloc; ss++ )
		{
			if( fix_p_alloc[ss] != n )
				continue;
			if( ss )
			if( fix_p_alloc[ss-1] == r )
				continue;
						
			fix_insert( &r, 1, ss, ss );
			ss++;
		}
		return this;
	}
	GPC_FIX*	fix_read( char* p_file, ULL& n_start, ULL n_end);
	I8			fix_write( char* p_file, I8 n_undo, ULL s = -1 );
	HANDLE		fix_write_async( GPS_FILE_ASYNC& file, char* p_file, I8 n_undo, ULL s = 0, ULL n = -1 );

	GPC_FIX*	fix_plus( GPC_FIX* p_add )
	{
		if( !p_add )
			return this;
		if( p_add->fix_p_alloc ? !p_add->fix_n_alloc : true )
			return this;
		ULL s = -1;
		return fix_insert( p_add->fix_p_alloc, p_add->fix_n_alloc, s, 0 );
	}

	GPC_FIX( void )
	{
		GPM_CLR;
		//memset( this, 0, sizeof(this) );
	}
	GPC_FIX( void* p_void, ULL n )
	{
		GPM_CLR;
		fix_p_alloc = (char*)p_void;
		fix_n_alloc = n;
		//memset( this, 0, sizeof(this) );
	}
	GPC_FIX( ULL _ver )
	{
		GPM_CLR;
		//memset( this, 0, sizeof(this) );
		fix_ver = _ver;
	}
	GPC_FIX( bool b_res )
	{
		if( b_res )
			return;

		GPM_CLR;
		//memset( this, 0, sizeof(this) );
	}
	~GPC_FIX()
	{
		GPM_DELARRAY( fix_p_alloc );
	}
	// 152.5 x 152.5 x 152.5 mm 
	// 230mm x 150mm x 140mm
	U4*			fix_store_png( GPC_WBx64* p_wb, GPE_ALFA type_id, GPC_CRS* gx_p_crs, I8 id );
	GPC_FIX*	fix_store_png_load( GPC_WBx64* p_wbx64, char* p_data2, I8 n_byte );
	GPC_FIX*	fix_store( GPC_WBx64* p_wb, U2* p_histi, I8 n_draw );
	GPC_FIX*	fix_store_sub( GPC_WBx64* p_wb, GPE_STF* p_stuff, U1 n_sub, I8 n_draw );
	GPC_FIX*	fix_res2str( GPC_RES* p_res, bool b_vari );
	GPC_FIX*	fix_share( GPC_CRS* p_crs, GPC_TAB* p_in );
	
	//GPC_FIX*	fix_compi_label( char* p_label, char* p_label_end );

	GPC_FIX*	pack_str( char* p_head, char* p_str )
	{
		char	s_head[] = "________87654321";
		I8 n = GPD_STRLEN( p_head );
		if( n > 8 )
			n = 8;

		gp_strncpy( s_head, p_head, n );

		GPC_FIX* p_out = this;

		n = p_out ? p_out->fix_n_alloc : 0;
		p_out = p_out->fix_format( s_head );
		if( !p_out )
			return NULL;

		p_out = p_out->fix_format( p_str );
		if( !p_out )
			return NULL;

		ULL nn = 0x20-(p_out->fix_n_alloc%0x10), s = -1;
		p_out = p_out->fix_expand( s, 0, nn );
		if( !p_out )
			return NULL;

		memset( p_out->fix_p_alloc+s, 0, nn );

		*(ULL*)&(p_out->fix_p_alloc[n+8]) = p_out->fix_n_alloc-n;
		return p_out;
	}
	
	GPC_FIX* pack_i8( GPE_NET8 hd, I8 i8 )
	{
		ULL a_hd[2];
		a_hd[0] = hd;
		a_hd[1] = 0;
	//GPC_FIX* pack_i8( char* p_head, I8 i8 )
	//{
	//	char s_head[] = "________87654321";
	//	I8 n = GPD_STRLEN( p_head );
	//	if( n > 8 )
	//		n = 8;
	//
	//	gp_strncpy( s_head, p_head, n );

		ULL n_begin = 0, s = -1;
		if( !this )
		{
			GPC_FIX* p_out = ((GPC_FIX*)NULL)->fix_expand( s, 0, sizeof(a_hd)+sizeof(i8) );
			gp_memcpy( p_out->fix_p_alloc+s, a_hd, sizeof(a_hd) );
			*(I8*)( p_out->fix_p_alloc+s+sizeof(a_hd)) = i8;
			//GPC_FIX* p_out = ((GPC_FIX*)NULL)->fix_format( s_head ); //"GXITNAME00000000" );
			//p_out = p_out->fix_expand( s, 0, sizeof(i8) );
			//*(I8*)(p_out->fix_p_alloc+s) = i8;
			*(ULL*)&(p_out->fix_p_alloc[n_begin+8]) = p_out->fix_n_alloc-n_begin;
			return p_out;
		}
		n_begin = fix_n_alloc;
		fix_expand( s, 0, sizeof(a_hd)+sizeof(i8) );
		gp_memcpy( fix_p_alloc+s, a_hd, sizeof(a_hd) );
		*(I8*)(fix_p_alloc+s+sizeof(a_hd)) = i8;
			
		//fix_format( s_head ); //"GXITNAME00000000" );
		//fix_expand( s, 0, sizeof(i8) );
		//*(I8*)(fix_p_alloc+s) = i8;
		*(ULL*)&(fix_p_alloc[n_begin+8]) = fix_n_alloc-n_begin;
		return this;
	}

	GPC_FIX* pack_v3( char* p_head, GPCV3& v3 )
	{
		char s_head[] = "________87654321";
		I8 n = GPD_STRLEN( p_head );
		if( n > 8 )
			n = 8;

		gp_strncpy( s_head, p_head, n );

		ULL n_begin = 0, s = -1;
		if( !this )
		{
			GPC_FIX* p_out = ((GPC_FIX*)NULL)->fix_format( s_head ); //"GXITNAME00000000" );
			p_out = p_out->fix_expand( s, 0, sizeof(v3) );
			*(GPCV3*)(p_out->fix_p_alloc+s) = v3;
			*(ULL*)&(p_out->fix_p_alloc[n_begin+8]) = p_out->fix_n_alloc-n_begin;
			return p_out;
		}
		n_begin = fix_n_alloc;
		fix_format( s_head ); //"GXITNAME00000000" );
		fix_expand( s, 0, sizeof(v3) );
		*(GPCV3*)(fix_p_alloc+s) = v3;
		*(ULL*)&(fix_p_alloc[n_begin+8]) = fix_n_alloc-n_begin;
		return this;
	}

	GPC_FIX* pack_iv4( char* p_head, GPCI4& iv4 )
	{
		char s_head[] = "________87654321";
		I8 n = GPD_STRLEN( p_head );
		if( n > 8 )
			n = 8;

		gp_strncpy( s_head, p_head, n );

		ULL n_begin = 0, s = -1;
		if( !this )
		{
			GPC_FIX* p_out = ((GPC_FIX*)NULL)->fix_format( s_head ); //"GXITNAME00000000" );
			p_out = p_out->fix_expand( s, 0, sizeof(iv4) );
			*(GPCI4*)(p_out->fix_p_alloc+s) = iv4;
			*(ULL*)&(p_out->fix_p_alloc[n_begin+8]) = p_out->fix_n_alloc-n_begin;
			return p_out;
		}
		n_begin = fix_n_alloc;
		fix_format( s_head ); //"GXITNAME00000000" );
		fix_expand( s, 0, sizeof(iv4) );
		*(GPCI4*)(fix_p_alloc+s) = iv4;
		*(ULL*)&(fix_p_alloc[n_begin+8]) = fix_n_alloc-n_begin;
		return this;
	}
	GPC_FIX* pack_p_exp( char* p_head, ULL& s, I8 n_byte )
	{
		char s_head[] = "________87654321";
		I8 n_head = GPD_STRLEN( p_head );
		if( n_head > 8 )
			n_head = 8;

		gp_strncpy( s_head, p_head, n_head );

		ULL n_begin = 0;
		s = -1;
		if( !this )
		{
			GPC_FIX* p_out = ((GPC_FIX*)NULL)->fix_format( s_head ); //"GXITNAME00000000" );
			p_out = p_out->fix_expand( s, 0, n_byte );
			//gp_memcpy( (p_out->fix_p_alloc+s), p_void, n_void );
			*(ULL*)&(p_out->fix_p_alloc[n_begin+8]) = p_out->fix_n_alloc-n_begin;
			return p_out;
		}
		n_begin = fix_n_alloc;
		fix_format( s_head ); //"GXITNAME00000000" );
		fix_expand( s, 0, n_byte );
		//gp_memcpy( (fix_p_alloc+s), p_void, n_void );
		*(ULL*)&(fix_p_alloc[n_begin+8]) = fix_n_alloc-n_begin;
		return this;
	}
	GPC_FIX* pack_p_void( GPE_NET8 hd, void* p_void, I8 n_void )
	{
		ULL a_hd[2];
		a_hd[0] = hd;
		a_hd[1] = 0;
		//char s_head[] = "________87654321";
		//I8 n_head = GPD_STRLEN( p_head );
		//if( n_head > 8 )
		//	n_head = 8;
		//
		//gp_strncpy( s_head, p_head, n_head );
		
		ULL n_begin = 0, s = -1;
		if( !this )
		{
			GPC_FIX* p_out = ((GPC_FIX*)NULL)->fix_expand( s, 0, sizeof(a_hd)+n_void );
			gp_memcpy( p_out->fix_p_alloc+s, a_hd, sizeof(a_hd) );
			gp_memcpy( p_out->fix_p_alloc+s+sizeof(a_hd), p_void, n_void );

			//GPC_FIX* p_out = ((GPC_FIX*)NULL)->fix_format( s_head ); //"GXITNAME00000000" );
			//p_out = p_out->fix_expand( s, 0, n_void );
			//gp_memcpy( (p_out->fix_p_alloc+s), p_void, n_void );
			*(ULL*)&(p_out->fix_p_alloc[n_begin+8]) = p_out->fix_n_alloc-n_begin;
			return p_out;
		}
		n_begin = fix_n_alloc;
		fix_expand( s, 0, sizeof(a_hd)+n_void );
		gp_memcpy( fix_p_alloc+s, a_hd, sizeof(a_hd) );
		gp_memcpy( fix_p_alloc+s+sizeof(a_hd), p_void, n_void );
		
		//fix_format( s_head ); //"GXITNAME00000000" );
		//fix_expand( s, 0, n_void );
		//gp_memcpy( (fix_p_alloc+s), p_void, n_void );
		*(ULL*)&(fix_p_alloc[n_begin+8]) = fix_n_alloc-n_begin;
		return this;
	}

	GPC_FIX* pack_fix( GPE_NET8 hd, GPC_FIX* p_fix )
	{
		return pack_p_void( hd, p_fix->fix_p_alloc, p_fix->fix_n_alloc );
		//ULL a_hd[2];
		//a_hd[0] = hd;
		//a_hd[1] = 0;
		////char s_head[] = "________87654321";
		////I8 n = GPD_STRLEN( p_head );
		////if( n > 8 )
		////	n = 8;
		////
		////gp_strncpy( s_head, p_head, n );
		//
		//ULL n_begin = 0, s = -1;
		//if( !this )
		//{
		//	GPC_FIX* p_out = ((GPC_FIX*)NULL)->fix_format( s_head ); //"GXITNAME00000000" );
		//	p_out = p_out->fix_expand( s, 0, p_fix->fix_n_alloc );
		//	gp_memcpy( (p_out->fix_p_alloc+s), p_fix->fix_p_alloc, p_fix->fix_n_alloc );
		//	*(ULL*)&(p_out->fix_p_alloc[n_begin+8]) = p_out->fix_n_alloc-n_begin;
		//	return p_out;
		//}
		//n_begin = fix_n_alloc;
		//fix_format( s_head ); //"GXITNAME00000000" );
		//fix_expand( s, 0, p_fix->fix_n_alloc );
		//gp_memcpy( (fix_p_alloc+s), p_fix->fix_p_alloc, p_fix->fix_n_alloc );
		//*(ULL*)&(fix_p_alloc[n_begin+8]) = fix_n_alloc-n_begin;
		//return this;
	}
	
	GPC_FIX* fix_dir( char* p_path, char* p_pe, char *p_atrib, bool b_ace = true );

	void fix_clr( void )
	{
		GPM_DELARRAY( fix_p_alloc );
		GPM_CLR;
	}

};

class GPC_BAG_TRD
{
public:
	GPC_IIx64		*p_ii_bit;
	U1		*p_0,
			*p_map,
			*p_join,
			a_A[GPD_BAG3],
			a_B[GPD_BAG3],
			a_C[GPD_BAG3];
			//a_ize[GPD_BAG2];

	#ifdef _GP_STD_THREAD
			std::thread *pt_DEC;
	#else
			HANDLE *p_run;
	#endif

	GPC_FIX		*p_fix;
	GPC_CODEx64	coder;

	U2	n_0, n_A, n_B, n_C, n_D, n_E, n_run, n_trd;
	GPE_BAG	bag;
	//U1	a_pad[0xf];

	U2		GPC_BAG_encode_v1( void );
	U2		GPC_BAG_decode_v1( void );

	GPC_BAG_TRD*	GPC_BAG_TRD_engine_encdV8( GPC_FIX* p_fix, U1* p_src, U2 n_src, U1* p_m, GPC_IIx64* p_ii  );
	GPC_BAG_TRD*	GPC_BAG_TRD_encdV8_RDY( GPC_FIX* p_fix, U1* p_src, U2 n_src, U1* p_m, GPC_IIx64* p_ii  );
	GPC_BAG_TRD(){};

	/*~GPC_BAG_TRD()
	{
		if( !p_run )
			return;

		delete[] p_run;
	}*/
};



#endif