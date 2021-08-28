#include "gpcSRC.h"
const char* gppsTYP[] = {
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
	while( p_str < p_end ) {
 		p_str += gpfVAN( p_str, (U1*)"0123456789", NULL ); //,nLEN );
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
	gpfMset( this, cr.x, &u, sizeof(u) );

	if( cr.mn() < 1 )
		return this;

	for(	U1x4	*pS = this, *pD = this+(whp.z*cr.y); pD > pS; pD -= whp.z )
		gpmMcpyOF( pD, pS, cr.x );


	return this;
}
U1x4* U1x4::frmBRDR( I4x2 cr, gpeCLR clr, U1 flg, I4x4 whp  )
{
	I4 r = whp.a4x2[0].mn();
	if( r < 1)
		return this;

	cr.mn(whp.a4x2[0]) -= 1;
	if( flg&5 )
    for( I4 e = cr.x, u = 0, d = cr.y*whp.z; u <= e; u++, d++ ) {
		if( flg&1 ) {
			this[u].z = clr;
			this[u].w |= 0x1;
		}
		if( flg&4 ) {
			this[d].z = clr;
			this[d].w |= 0x4;
		}
	}

	if(!whp.z)
		return this;

	if( flg&0xa )
    for( I4 l = 0, r = cr.x, e = whp.z*cr.y; l <= e; l += whp.z, r += whp.z ) {
		if( flg&8 ) {
			this[l].z = clr;
			this[l].w |= 0x8;
		}
		if( flg&2 ) {
			this[r].z = clr;
			this[r].w |= 0x2;
		}
	}


    return this;
}
int U1x4::hstXrgb( I4x4* pH, int p, int n, int w, int h ) {
	if( n ? n>0x100 : true )
		return 0;

	if(!h)
		h = w;
	w*=3;
	p*=3;

	int c = 0x100/n;
	if( (0x100/c) > n )
		c++;

	gpmZnOF(pH,n);
	for( U1 *pS = (U1*)this, *pSe = pS+p*h, *pSi, *pSie; pS < pSe; pS += p )
	for( pSi = pS, pSie = pSi+w; pSi<pSie; pSi+=3 ){
		pH[pSi[0]/c].x++;
		pH[pSi[1]/c].y++;
		pH[pSi[2]/c].z++;
	}
	return n;
}
int U1x4::hstX( I4x4* pH, int p, int n, int w, int h ) {
	if( n ? n>0x100 : true )
		return 0;
	if(!h)
		h = w;
	int c = 0x100/n;
	if( (0x100/c) > n )
		c++;

	gpmZnOF(pH,n);
	for( U1x4 *pS = this, *pSe = pS+p*h, *pSi, *pSie; pS < pSe; pS += p )
	for( pSi = pS, pSie = pSi+w; pSi<pSie; pSi++ ){
		pH[pSi->x/c].x++;
		pH[pSi->y/c].y++;
		pH[pSi->z/c].z++;
	}
	return n;
}
U4 U1x4::bugU1( I4x2* pR, U4* pM, I4 mom,
				I4 b, I4* pD, U4 n,
				I4 rg, U4 nX ) {
	if(pM[b]!=mom)
		return 0;
	pM[b] = b;
	U1	*pU1 = (U1*)this,
		in = pU1[b];
	if(!in)
		return 0;

	I4x2	*pRr = pR+1,
			*pRi = pRr;

	U4	nR, n_stp = 0;
	I4	w = pD[2],
		s = b, bx, by,
		aR[] = {-w,0,w,0};
	U1 rule;
	I1 dir = 1;

	pM[s] = s;
	*pRi = 0;

	b++;
	by = s-s%w;

	bool b_pre = true, b_in;
	if( rg < 1 )
		rg = w;
	else if( rg > w )
		rg = w;

	while( s == b ? dir != 2 : true ) //s != b || (pRi->x&1) )
	{
		b_in = false;
		bx=b-by;

		if(bx>=0)
		if(bx<rg) 			// balra van
		if(b>=0)			// lent van
		if(b>=by)			// jobra van
		if(b<n)				// fent van
		if( (pM[b]==mom) || (pM[b]==s) )
		if( b_in = (pU1[b]==in) )
				pM[b]=s;


		if( dir&1 )
		if( b_in != b_pre )
		{
			pRi->y = b;
			pRi++;
			pRi->x = pRi-pRr;
		}

		if( b_in )
		{
			if( b_in != b_pre )
				n_stp++;

			dir--;
			if(dir<0)
				dir = 3;
		} else {
			dir++;
			dir %= 4;
		}

		b_pre=b_in;
		b	+= pD[dir];
		by	+= aR[dir];
	}

	if( nX ? nX < pRi->x : false )
		return 0;


	return pR->x = pRi->x;
}
U4 U1x4::bugW( I4x2* pR, U4* pMSK, I4 mom, I4 b, I4* pD, U4 n, U4 nX )
{
	if(pMSK[b]!=mom)
		return 0;
	pMSK[b] = b;
	U1 in = this[b].w;
	if(!in)
		return 0;

	I4x2	*pRr = pR+1,
			*pRi = pRr;

	U4	nR, n_stp = 0;
	I4	w = pD[2],
		s = b, bx, by,
		aR[] = {-w,0,w,0};
	U1 rule;
	I1 dir = 1;

	pMSK[s] = s;
	*pRi = 0;

	b++;
	by = s-s%w;

	bool b_pre = true, b_in;

	while( s == b ? dir != 2 : true ) //s != b || (pRi->x&1) )
	{
		bx = b-by;

		rule =	   (b  >= 0	)		// lent van
				| ((b  >= by)<<1)	// jobra van
				| ((bx <  w	)<<2)	// balra van
				| ((b  <  n )<<3);	// fent van

		b_in = false;
		if( rule == 0xf )
		{
			if( pMSK[b] == mom ? false : pMSK[b] == s )
				b_in = true;
			else if( b_in = this[b].w == in )
				pMSK[b] = s;
		}


		if( b_in != b_pre )
		if( dir&1 )
		{
			pRi->y = b;
			pRi++;
			pRi->x = pRi-pRr;
		}

		if( b_in )
		{
			if( b_in != b_pre )
				n_stp++;

			dir--;
			if(dir<0)
				dir = 3;
		} else {
			dir++;
			dir %= 4;
		}

		b_pre=b_in;
		b	+= pD[dir];
		by	+= aR[dir];
	}

	if( nX ? nX < pRi->x : false )
		return 0;


	return pR->x = pRi->x;
}

