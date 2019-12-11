#include "piMASS.h"
char	gps_lzy_pub1[1024*0x100];
gpcLZY* gpcLZY::lzyFRMT( U8& n_start, const char* p_format, ... )
{
	va_list vl;
	va_start(vl, p_format);
	gps_lzy_pub1[0] = 0;
	U8 n = vsprintf( gps_lzy_pub1, p_format, vl );
	if( n < 1 )
		return this;
	U8 s = -1;
	return lzyADD( (U1*)gps_lzy_pub1, n, n_start );
}

U4 gpcLZY::tree_fnd( U4 id, U4& n )
{
	if( !this )
		return n = 0;

	n = n_load/sizeof(U4x4);
	U4x4* p_u44 = (U4x4*)p_alloc;
	U4 fnd = p_u44->tree_fnd( id, n );

	if( p_u44[fnd].x == id )
		return fnd;

	return n;
}
gpcLZY* gpcLZY::tree_add( U4 id, U4& n )
{
	U8 s = -1;
	if( !this )
	{
		gpcLZY* p_this = lzyADD( NULL, sizeof(U4x4), s, 8 );
		if( !p_this )
			return NULL;

		n = 1;
		((U4x4*)p_this->p_alloc)->null().x = id;
		return p_this;
	}
	lzyADD( NULL, sizeof(U4x4), s );
	U4x4* p_u44 = (U4x4*)p_alloc;
	n = s/sizeof(*p_u44);
	s = p_u44->tree_add( id, n );
	if( s == n )
		return this;
	n = s;
	n_load = s*sizeof(*p_u44);
	return this;
}

U8 gpcLZY::tree_fnd( U8 id, U8& n )
{
	if( !this )
		return n = 0;

	n = n_load/sizeof(U8x4);
	U8x4* p_u84 = (U8x4*)p_alloc;
	U8 fnd = p_u84->tree_fnd( id, n );

	if( p_u84[fnd].x == id )
		return fnd;

	return n;
}
gpcLZY* gpcLZY::tree_add( U8 id, U8& n )
{
	U8 s = -1;
	if( !this )
	{
		gpcLZY* p_this = lzyADD( NULL, sizeof(U8x4), s, 8 );
		if( !p_this )
			return NULL;

		n = 1;
		((U8x4*)p_this->p_alloc)->null().x = id;
		return p_this;
	}
	lzyADD( NULL, sizeof(U8x4), s );
	U8x4* p_u84 = (U8x4*)p_alloc;
	n = s/sizeof(*p_u84);
	s = p_u84->tree_add( id, n );
	if( s == n )
		return this;
	n = s;
	n_load = s*sizeof(*p_u84);
	return this;
}

I8 gpcLZY::tree_fnd( I8 id, I8& n )
{
	if( !this )
		return n = 0;

	n = n_load/sizeof(U8x4);
	I8x4* p_i84 = (I8x4*)p_alloc;
	I8 fnd = p_i84->tree_fnd( id, n );

	if( p_i84[fnd].x == id )
		return fnd;

	return n;
}
gpcLZY* gpcLZY::tree_add( I8 id, I8& n )
{
	U8 s = -1;
	if( !this )
	{
		gpcLZY* p_this = lzyADD( NULL, sizeof(I8x4), s = -1, 8 );
		if( !p_this )
			return NULL;

		n = 1;
		((I8x4*)p_this->p_alloc)->null().x = id;
		return p_this;
	}
	lzyADD( NULL, sizeof(I8x4), s );
	I8x4* p_i84 = (I8x4*)p_alloc;
	n = s/sizeof(*p_i84);
	s = p_i84->tree_add( id, n );
	if( s == n )
		return this;
	n = s;
	n_load = s*sizeof(*p_i84);
	return this;
}

