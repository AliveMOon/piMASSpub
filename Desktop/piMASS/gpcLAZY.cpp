#include "piMASS.h"

U4 gpcLAZY::tree_fnd( U4 id, U4& n )
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
gpcLAZY* gpcLAZY::tree_add( U4 id, U4& n )
{
	U8 s = -1;
	if( !this )
	{
		gpcLAZY* p_this = lazy_add( NULL, sizeof(U4x4), s, 8 );
		if( !p_this )
			return NULL;

		n = 1;
		((U4x4*)p_this->p_alloc)->null().x = id;
		return p_this;
	}
	lazy_add( NULL, sizeof(U4x4), s );
	U4x4* p_u44 = (U4x4*)p_alloc;
	n = s/sizeof(*p_u44);
	s = p_u44->tree_add( id, n );
	if( s == n )
		return this;
	n = s;
	n_load = s*sizeof(*p_u44);
	return this;
}

U8 gpcLAZY::tree_fnd( U8 id, U8& n )
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
gpcLAZY* gpcLAZY::tree_add( U8 id, U8& n )
{
	U8 s = -1;
	if( !this )
	{
		gpcLAZY* p_this = lazy_add( NULL, sizeof(U8x4), s, 8 );
		if( !p_this )
			return NULL;

		n = 1;
		((U8x4*)p_this->p_alloc)->null().x = id;
		return p_this;
	}
	lazy_add( NULL, sizeof(U8x4), s );
	U8x4* p_u84 = (U8x4*)p_alloc;
	n = s/sizeof(*p_u84);
	s = p_u84->tree_add( id, n );
	if( s == n )
		return this;
	n = s;
	n_load = s*sizeof(*p_u84);
	return this;
}
/*U8 gpcLAZY::tree_fnd8( U8 id, U8& n )
{
	if( !this )
		return n = 0;

	n = n_load/sizeof(U8x4);
	U8x4* p_u84 = (U8x4*)p_alloc;
	U4 fnd = p_u84->tree_fnd( id, n );

	if( p_u84[fnd].x == id )
		return fnd;

	return n;
}
gpcLAZY* gpcLAZY::tree_add8( U8 id, U8& n )
{
	U8 s = -1;
	if( !this )
	{
		gpcLAZY* p_this = lazy_add( NULL, sizeof(U8x4), s, 8 );
		if( !p_this )
			return NULL;

		n = 1;
		((U8x4*)p_this->p_alloc)->null().x = id;
		return p_this;
	}
	lazy_add( NULL, sizeof(U8x4), s );
	U8x4* p_u84 = (U8x4*)p_alloc;
	n = s/sizeof(*p_u84);
	s = p_u84->tree_add( id, n );
	if( s == n )
		return this;

	n = s;
	n_load = s*sizeof(*p_u84);
	return this;
}*/
