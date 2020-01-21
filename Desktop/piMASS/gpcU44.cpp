#include "gpcSRC.h"
extern U1 gpaALFadd[];
char* gppsTYP[] = {
	"U",
	"*U",
	"rU",
	"*rU",
	"F",
	"*F",
	"rF",
	"*rF",
	"I",
	"*I",
	"rI",
	"*rI",
	"F",
	"*F",
	"rF",
	"*rF",

};
U1* U1x4::typ2str( U1* pBUFF )
{
	/// x[7s,6f,5r,4str : 3-0 nBYTE = 1<<(x&0xf) ]
	// yz dimxy
	if(  y*z > 1 )
		sprintf( (char*)pBUFF, "%s%dx%dx%d", gppsTYP[x>>0x4], 1<<(x&0xf), y, z );
	else
		sprintf( (char*)pBUFF, "%s%d", gppsTYP[x>>0x4], 1<<(x&0xf) );

	return pBUFF;
}
U4x2& U4x2::operator = ( const I8x2& an )
{
	if( an.alf )
	{
		a4 = (gpeALFu4)an.alf;
		n4 = an.num;
	} else
		gpmCLR;

	return *this;
}
U4x2::U4x2( const I8x2& an )
{
	*this = an.alf;
}
U4x2::U4x2( const I4x2& i42 )
{
	x = i42.x < 0 ? -i42.x : i42.x;
	y = i42.y < 0 ? -i42.y : i42.y;
}

I8x2& I8x2::operator = ( const U1* pS )
{
	if( num ? ( pS ? !*pS : true ) : true )
		return null();
	if( num > 14 )
		num = 14;
	U1* pE = (U1*)pS + num;
	alf = gpfSTR2ALF( pS, pE, &pE );
	num = pE-pS;
	return *this;
}



U4x2& U4x2::mx( I4x2 b )
{
	if( b.x < 0 )
		b.x = 0;
	if( b.y < 0 )
		b.y = 0;

	if( x < b.x )
		x = b.x;
	if( y < b.y )
		y = b.y;
	return *this;
}

U4x2& U4x2::mn( I4x2 b )
{
	if( b.x < 0 )
		b.x = 0;
	if( b.y < 0 )
		b.y = 0;

	if( x > b.x )
		x = b.x;
	if( y > b.y )
		y = b.y;
	return *this;
}


U4x4::U4x4( const I4x2 _xy, I4x2* p_zw )
{
	a4x2[0] = _xy;
	a4x2[1] = p_zw ? *p_zw : _xy;
}
U4x2& U4x2::operator = ( const I4x2& b )
{
	x = b.x > 0 ? b.x : 0;
	y = b.y > 0 ? b.y : 0;
	return *this;
}
U4x4& U4x4::operator = ( const I4x2& b )
{
	a4x2[1] = a4x2[0] = b;
	return *this;
}
U4x4& U4x4::str2date( U1* p_str, U1* p_end, U1** pp_str )
{
	gpmCLR;
	U1 i = 0;
	U8 nLEN;
	while( p_str < p_end )
	{
		p_str += gpfVAN( p_str, (U1*)"0123456789", nLEN );
		aXYZW[i%4] = gpfSTR2I8( p_str, &p_str );
		i++;

	}

	return *this;
}

U4  U4x4::dict_add( U1* p_src, U4& m, U4x4& w )
{
	m = 0;
	U4x4* p_w = this;
	U2	n_w = &w-p_w;
	if( !n_w )
		return 1;

	U4	cd = 0, e, i,
		mx = 0,
		o = 0;

	U1	*pS = p_src+w.x,
			*p_d;


	while( cd < n_w )
	{
		p_d = p_src + p_w[cd].x;
		e = p_w[cd].y;
		if( e > w.y )
			e = w.y;

		for( i = 0; i < e; i++ )
		{
			if( pS[i] == p_d[i] )
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

			m = cd;				/// ez nem volt
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

		if( p_d[i] < pS[i] )
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

		if( p_d[i] > pS[i] )
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
U4  U4x4::dict_find( U1* p_src, U4x4& w )
{
	U4x4* p_w = this;
	U4	n_w = &w-p_w;
	if( !n_w )
		return 0;

	U4	cd = 0, e, i, m = 0, mx = 0, o = n_w;
	U1	*pS = p_src+w.x,
		*p_d;

	while( cd < n_w )
	{
		p_d = p_src+p_w[cd].x;
		e = p_w[cd].y;
		if( e > w.y )
			e = w.y;
		for( i = 0; i < e; i++ )
		{
			if( pS[i] == p_d[i] )
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

		if( p_d[i] < pS[i] )
		{
			if( !p_w[cd].w )
				return o;

			cd = p_w[cd].w;
			continue;
		}

		if( p_d[i] > pS[i] )
		if( !p_w[cd].z )
			return o;

		cd = p_w[cd].z;
	}
	return o;
}
