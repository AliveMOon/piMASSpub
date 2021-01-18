#include "piMASS.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];
char	gps_lzy_pub1[1024*0x100];
gpcLZY* gpcLZY::lzyFRMT( U8& iSTRT, const char* p_format, ... )
{
	va_list vl;
	va_start(vl, p_format);
	gps_lzy_pub1[0] = 0;
	U8 n = vsprintf( gps_lzy_pub1, p_format, vl );
	if( n < 1 )
		return this;
	U8 s = -1;
	return lzyADD( (U1*)gps_lzy_pub1, n, iSTRT );
}
static const char* gpasADDR[] = {
	"0x%0.2x|",
	"0x%0.4x|",
	"0x%0.8x|",
	"0x%0.16llx|",
	"//%0.2x|",
	"//%0.4x|",
	"//%0.8x|",
	"//%0.16llx|",
	"%S0x%0.2x|",
	"%S0x%0.4x|",
	"%S0x%0.8x|",
	"%S0x%0.16llx|",
};
gpcLZY* gpcLZY::lzyHEXb( U8& iSTRT, U1* pBIN, U4 nBIN ) {
	if( nBIN ? !pBIN : true )
		return this;

	if( !this )
	{
		gpcLZY* pTHIS = new gpcLZY();
		if( !pTHIS )
			return NULL;

		return pTHIS->lzyHEXb( iSTRT, pBIN, nBIN );
	}

	iSTRT = n_load;
	U8 s;

	U1 nLOG = log2( nBIN );
	char sLINE[0x100], *pLINE = sLINE, *pADDR;
	//pLINE += sprintf( pLINE, "\"\r\n");
	for( U4 i = 0, j, je; i < nBIN; i += 16 )
	{
		pLINE += sprintf( pLINE, gpasADDR[nLOG/8], i );
		for( j = i, je = j+16; j < je; j++ )
		{
			if( j >= nBIN )
			{
				pLINE += sprintf( pLINE, "   " );
				continue;
			}
			pLINE += sprintf( pLINE, "%0.2x ", pBIN[j] );
		}
		pLINE += sprintf( pLINE, "|" );
		for( j = i, je = j+16; j < je; j++ )
		{
			if( j >= nBIN )
				break;
			*pLINE = ((pBIN[j] >= 0x20) && (pBIN[j] < 0x80)) ? pBIN[j] : '.';
			pLINE++;
		}
		*pLINE = 0;
		lzyFRMT( s = -1, "\r\n%s", sLINE );
		pLINE = sLINE;
	}
	pLINE += sprintf( pLINE, gpasADDR[nLOG/8], nBIN );
	lzyFRMT( s = -1, "\r\n%s", sLINE );
	return this;
}
gpcLZY* gpcLZY::lzyHEXw( U8& iSTRT, U1* pBIN, U4 nBIN ) {
	if( nBIN ? !pBIN : true )
		return this;

	if( !this )
	{
		gpcLZY* pTHIS = new gpcLZY();
		if( !pTHIS )
			return NULL;

		return pTHIS->lzyHEXw( iSTRT, pBIN, nBIN );
	}

	iSTRT = n_load;
	U8 s;

	U1 nLOG = log2( nBIN )/8+4;
	char sLINE[0x400], *pLINE = sLINE, *pADDR;
	//pLINE += sprintf( pLINE, "\"\r\n");
	for( U4 i = 0, j, je; i < nBIN; i += 16 )
	{
		pLINE += sprintf( pLINE, gpasADDR[nLOG], i );
		for( j = i, je = j+16; j < je; j+=2 )
		{
			if( j >= nBIN )
			{
				pLINE += sprintf( pLINE, "     " );
				continue;
			}
			pLINE += sprintf( pLINE, "%0.4x ", *(U2*)(pBIN+j) );
		}
		pLINE += sprintf( pLINE, "|" );
		for( j = i, je = j+16; j < je; j++ )
		{
			if( j >= nBIN )
				break;
			*pLINE = ((pBIN[j] >= 0x20) && (pBIN[j] < 0x80)) ? pBIN[j] : '.';
			pLINE++;
		}
		*pLINE = 0;
		lzyFRMT( s = -1, "\r\n%s", sLINE );
		pLINE = sLINE;
	}
	pLINE += sprintf( pLINE, gpasADDR[nLOG], nBIN );
	lzyFRMT( s = -1, "\r\n%s", sLINE );
	return this;
}
gpcLZY* gpcLZY::lzyHEXl( U8& iSTRT, U1* pBIN, U4 nBIN, bool bCOM ) {
	if( nBIN ? !pBIN : true )
		return this;

	if( !this )
	{
		gpcLZY* pTHIS = new gpcLZY();
		if( !pTHIS )
			return NULL;

		return pTHIS->lzyHEXl( iSTRT, pBIN, nBIN, bCOM );
	}

	iSTRT = n_load;
	U8 s;

	U1 nLOG = log2( nBIN )/8+(bCOM?4:0);

	char sLINE[0x400], *pLINE = sLINE, *pADDR;
	//pLINE += sprintf( pLINE, "\"\r\n");
	for( U4 i = 0, j, je; i < nBIN; i += 16 )
	{
		pLINE += sprintf( pLINE, gpasADDR[nLOG], i );
		for( j = i, je = j+16; j < je; j += sizeof(U4) )
		{
			if( j >= nBIN )
			{
				pLINE += sprintf( pLINE, "         " );
				continue;
			}
			pLINE += sprintf( pLINE, "0x%0.8x ", *(U4*)(pBIN+j) );
		}
		pLINE += sprintf( pLINE, "//" );
		for( j = i, je = j+16; j < je; j++ )
		{
			if( j >= nBIN )
				break;
			*pLINE = ((pBIN[j] >= 0x20) && (pBIN[j] < 0x80)) ? pBIN[j] : '.';
			pLINE++;
		}
		*pLINE = 0;
		lzyFRMT( s = -1, "\r\n%s", sLINE );
		pLINE = sLINE;
	}
	pLINE += sprintf( pLINE, gpasADDR[nLOG], nBIN );
	lzyFRMT( s = -1, "\r\n%s", sLINE );
	return this;
}
U4 gpcLZY::tree_fnd( U4 id, U4& n ) {
	if( !this )
		return n = 0;

	n = n_load/sizeof(U4x4);
	U4x4* p_u44 = (U4x4*)p_alloc;
	U4 fnd = p_u44->tree_fnd( id, n );

	if( p_u44[fnd].x == id )
		return fnd;

	return n;
}
gpcLZY* gpcLZY::tree_add( U4 id, U4& n ) {
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

U8 gpcLZY::tree_fnd( U8 id, U8& n ) {
	if( !this )
		return n = 0;

	n = n_load/sizeof(U8x4);
	U8x4* p_u84 = (U8x4*)p_alloc;
	U8 fnd = p_u84->tree_fnd( id, n );

	if( p_u84[fnd].x == id )
		return fnd;

	return n;
}
gpcLZY* gpcLZY::tree_add( U8 id, U8& n ) {
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

I8 gpcLZY::tree_fnd( I8 id, I8& n ) {
	if( !this )
		return n = 0;

	n = n_load/sizeof(U8x4);
	I8x4* p_i84 = (I8x4*)p_alloc;
	I8 fnd = p_i84->tree_fnd( id, n );

	if( p_i84[fnd].x == id )
		return fnd;

	return n;
}
gpcLZY* gpcLZY::tree_add( I8 id, I8& n ) {
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
int gpcLZY::nAT( char* pSat, int nSat, const char* pFILT ) {
	if( this ? !pSat : true )
		return 0;
	if( !nSat ) {
		nSat = gpmSTRLEN(pSat);
		if( !nSat )
			return 0;
	}

	U8 nUTF8;
	int nAT = 0;
	I8x2* pAn;
	char* pSatI = pSat, *pSatE = pSat+nSat, aN[]=" ";
	for( 	pSatI += gpmNINCS(pSatI,pFILT);
			pSatI < pSatE;
			pSatI += gpmNINCS(pSatI,pFILT), nAT++ ) {

		if( !*pSatI )
			break;

		pAn = (I8x2*)Ux(nAT,sizeof(*pAn));
		pAn->y = pSatE-pSatI;
		*pAn = pSatI;
		if( !pAn->alf ) {
			nAT--;
			pSatI += gpfABCvan( (U1*)pSatI, (U1*)pSatE, nUTF8, gpaALFsub );
			continue;
		}
		pSatI+=pAn->y;
		pAn->y = pSatI-pSat;
	}
	pAn = (I8x2*)Ux(nAT,sizeof(*pAn));
	pAn->y = pSatE-pSat;
	return nAT;
}
