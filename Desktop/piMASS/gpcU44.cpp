#include "gpcSRC.h"
U4x4& U4x4::src2date( U1* p_str, U1* p_end, U1** pp_str )
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
