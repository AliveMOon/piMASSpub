#include "gpcSRC.h"
extern U1 gpaALFsub[];

I4x2::I4x2( I8x2 b )
{
	x = b.x;
	y = b.y;
}

I4x2& I4x2::operator = ( const I8x2& b )
{
	x = b.x;
	y = b.y;
	return *this;
}
I8x2 I4x2::operator * ( int b ) const {
	if( !b )
		return 0;
	if( b == 1 )
		return (I8x2)*this;
	if( b == -1 )
		return I8x2( -x, -y );

	return I8x2( (I8)x*(I8)b, (I8)y*(I8)b );
}
I8x2 I4x2::operator * ( I8 b ) const {
	if( !b )
		return 0;
	if( b == 1 )
		return (I8x2)*this;
	if( b == -1 )
		return I8x2( -x, -y );

	return I8x2( (I8)x*b, (I8)y*b );
}
I8x2 I4x2::operator & (const I4x2& b) const
{
	return I8x2( x*b.x,  y*b.y );
}
I4x2 I4x2::operator / (const I8x2& b) const
{
	return I4x2( b.x ? x/b.x : 0x7fffffff ,  b.y ? y/b.y : 0x7fffffff );
}
I8x2 I4x2::operator + (const I8x2& b) const
{
	return I8x2( (I8)x+b.x,  (I8)y+b.y );
}
I8x2 I4x2::operator - (const I8x2& b) const
{
	return I8x2( (I8)x-b.x,  (I8)y-b.y );
}

I4x4 I4x2::xxxy() const { return I4x4(x,x,x,y); }


