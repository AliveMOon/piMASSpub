#include "gpcSRC.h"

U1x4& U1x4::str2time( U1* p_str, U1* p_end, U1** pp_str )
{
	gpmCLR;
	U1 i = 0;
	U8 nLEN;
	while( p_str < p_end )
	{
		p_str += gpfVAN( p_str, (U1*)"0123456789",nLEN );
		aXYZW[i%4] = gpfSTR2I8( p_str, &p_str );
		i++;
	}

	return *this;
}
U1x4* U1x4::pos( I4x2 pxy, const I4x4& whp )
{
	pxy.mx( I4x2(0,0) ).mn(whp.a4x2[1]-I4x2(1,1));
	return this + pxy.x + pxy.y*whp.z;
}
U1x4* U1x4::frm( I4x2 cr, gpeCLR clr, U1 flg, I4x4 whp  )
{

	if( flg&5 )
    for( I4 e = min( cr.x,whp.x), u = 0, d = min( cr.y-1, whp.y)*whp.z; u < e; u++, d++ )
	{
		if( flg&1 )
		{
			this[u].z = clr;
			this[u].w |= 0x1;
		}
		if( flg&4 )
		{
			this[d].z = clr;
			this[d].w |= 0x4;
		}
	}

	if(!whp.z)
		return this;

	if( flg&0xa )
    for( I4 l = 0, r = min( cr.x-1, whp.x), e = whp.z*min( cr.y,whp.y); l < e; l += whp.z, r += whp.z )
	{
		if( flg&8 )
		{
			this[l].z = clr;
			this[l].w |= 0x8;
		}
		if( flg&2 )
		{
			this[r].z = clr;
			this[r].w |= 0x2;
		}
	}


    return this;
}
