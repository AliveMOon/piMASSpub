#include "gpcSRC.h"
extern U1 gpaALFadd[];

/*I4x2::I4x2( U8x2 b )
{
	x = b.x > 0x7fffFFFF ? 0x7fffFFFF : b.x;
	y = b.y > 0x7fffFFFF ? 0x7fffFFFF : b.y;
}*/
I4x2::I4x2( I8x2 b )
{
	x = b.x;
	y = b.y;
}

/*I4x2& I4x2::operator = ( const U8x2& b )
{
	x = b.x > 0x7fffFFFF ? 0x7fffFFFF : b.x;
	y = b.y > 0x7fffFFFF ? 0x7fffFFFF : b.y;
	return *this;
}*/
I4x2& I4x2::operator = ( const I8x2& b )
{
	x = b.x;
	y = b.y;
	return *this;
}

