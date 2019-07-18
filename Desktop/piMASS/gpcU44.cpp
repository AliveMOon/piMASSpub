#include "gpcSRC.h"
U4x4& U4x4::str2date( U1* p_str, U1* p_end, U1** pp_str )
{
	gpmCLR;
	U1 i = 0;
	U8 nLEN;
	while( p_str < p_end )
	{
		p_str += gpfVAN( p_str, (U1*)"0123456789", nLEN );
		aXYZW[i%4] = gpfSRC2I8( p_str, &p_str );
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
