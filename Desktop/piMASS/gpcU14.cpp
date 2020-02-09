#include "gpcSRC.h"
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
U1x4* U1x4::frmFILL( I4x2 cr, U1x4 u, I4x4 whp  )
{
	cr.mn(whp.a4x2[0]) -= 1;
	gpfMEMSET( this, cr.x, &u, sizeof(u) );

	if( cr.mn() < 1 )
		return this;

	for(	U1x4	*pS = this, *pD = this+(whp.z*cr.y); pD > pS; pD -= whp.z )
		gpmMCPYof( pD, pS, cr.x );


	return this;
}
U1x4* U1x4::frmBRDR( I4x2 cr, gpeCLR clr, U1 flg, I4x4 whp  )
{
	I4 r = whp.a4x2[0].mn();
	if( r < 1)
		return this;

	cr.mn(whp.a4x2[0]) -= 1;
	if( flg&5 )
    for( I4 e = cr.x, u = 0, d = cr.y*whp.z; u <= e; u++, d++ )
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
    for( I4 l = 0, r = cr.x, e = whp.z*cr.y; l <= e; l += whp.z, r += whp.z )
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
