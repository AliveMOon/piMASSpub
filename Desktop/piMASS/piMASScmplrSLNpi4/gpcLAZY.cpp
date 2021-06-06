#include "piMASS.h"
#include <dirent.h>
extern U1 gpaALFsub[];
extern U1 gpaALFsub2[];
extern char gpaALF_H_sub[];
char	gps_lzy_pub1[1024*0x100];
gpcLZY* gpcLZY::lzyADD( const void* p_void, size_t n_byte, U8& iSTRT, U1 n ) {
	if( !n_byte )
		return this;

	if( !this ) {
		iSTRT = 0;
		gpcLZY* p_lazy = new gpcLZY( n );
		p_lazy->n_alloc = gpmPAD( n_byte*p_lazy->aSET[gpeLZYxN], 0x10 );
		p_lazy->p_alloc = gpmALLOC( p_lazy->n_alloc+0x10 ); //, 0x10 );
		if( !p_void )
		{
			p_lazy->p_alloc[p_lazy->n_load = n_byte] = 0;
			return p_lazy;
		}

		((U1*)memcpy( p_lazy->p_alloc, p_void, n_byte ))[n_byte] = 0;
		p_lazy->n_load = n_byte;
		return p_lazy;
	}
	if( n > 0x80 )
		n = 4;
	if( !n )
	{
		if( !aSET[gpeLZYxN] )
			aSET[gpeLZYxN] = 4;

		n = aSET[gpeLZYxN];
	}
	else if( !aSET[gpeLZYxN] )
			aSET[gpeLZYxN] = n;


	if( iSTRT > n_load )
		iSTRT = n_load;
	else
		n_load = iSTRT;

	size_t n_kill = n_alloc;
	if( p_alloc ? ((n_load+n_byte) > n_kill) : true ) {
		U1* p_kill = p_alloc;
		n_alloc = gpmPAD( (n_load+n_byte*n), 0x10 );
		p_alloc = gpmALLOC( n_alloc+0x10 );
		if( p_kill ) {
			gpmMcpy( p_alloc, p_kill, n_load );
			gpmFREE( p_kill );
		} else
			n_load = 0;
	}

	if( p_void )
		gpmMcpy( p_alloc+n_load, p_void, n_byte );

	n_load += n_byte;
	p_alloc[n_load] = 0;
	return this;
}

U1*  gpcLZY::Ux( I8 iPC, U4 n, bool bZ, U4 stp ) {
		if( !this )
			return NULL;

		if( iPC < 0 )
			iPC *= -1;
		if( !stp )
			stp = n;
		U8	e = iPC*stp + n;
		if( e <= nLD() )
			 return iPC ? p_alloc + iPC*stp : p_alloc;

		U8 s = -1, ee = e+n*3;

		lzyADD( NULL, ee-nLD(), s );
		if( bZ )
			gpmZn( p_alloc+s, nLD()-s );
		if( nLD() > e )
			n_load = e;
		if( !iPC )
			return p_alloc;

        return p_alloc + iPC*stp;
}
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
gpcLZY* gpcLZY::lzyDIR( const char* p_file, U8& iSTRT ) {
	if( !p_file )
		return this;

	struct dirent *pE;
	struct stat buf;
	DIR* pD = opendir( p_file );
	if( !pD )
		return lzySUB( iSTRT, -1 );

	gpcLZY* pDIR = this;
	if( iSTRT>pDIR->nLD() )
		iSTRT = pDIR->nLD();

	U8 s = iSTRT;
	char s___[] = "---";
	while( (pE=readdir(pD)) ) {

		if( !(strcmp(".", pE->d_name)))
			continue;
		if( !(strcmp("..", pE->d_name)))
			continue;

		pDIR = pDIR->lzyFRMT( s, "%s/%s", p_file, pE->d_name );
		stat( (char*)pDIR->p_alloc+s, &buf );
		if (pE->d_type == DT_DIR)
			s___[0]='d';
		else if (pE->d_type == DT_REG)
			s___[0]='f';
		s___[1]=(buf.st_mode & S_IRUSR) ? 'r' : '-';
		s___[2]=(buf.st_mode & S_IWOTH) ? 'w' : '-';

		pDIR = pDIR->lzyFRMT( s, "%s\t%s\t%zd\r\n", pE->d_name, s___, buf.st_size );
		s = pDIR->nLD();
	}
	closedir(pD);
	return pDIR;
}
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

	n = nLD(sizeof(U4x4));
	if( !n )
		return n;
	U4x4* p_u44 = (U4x4*)p_alloc;
	U4 fnd = p_u44->tree_fnd( id, n );

	if( p_u44[fnd].x == id )
		return fnd;

	return n;
}
gpcLZY* gpcLZY::tree_add( U4 id, U4& n ) {
	U8 s = -1;
	if( !this ) {
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

	n = nLD(sizeof(U8x4));
	if( !n )
		return n;
	U8x4* p_u84 = (U8x4*)p_alloc;
	U8 fnd = p_u84->tree_fnd( id, n );

	if( p_u84[fnd].x == id )
		return fnd;

	return n;
}
gpcLZY* gpcLZY::tree_add( U8 id, U8& n ) {
	U8 s = -1;
	if( !this ) {
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

	n = nLD(sizeof(I8x4));
	if( !n )
		return n;
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
int gpcLZY::nAT( const char* pS, int nS, const char* pFILT ) {
	if( this ? !pS : true )
		return 0;
	if( !nS ) {
		nS = gpmSTRLEN(pS);
		if( !nS )
			return 0;
	}
	U8 nUTF8;
	int nAT = 0;
	I8x2* pAn;
	char* pSi = (char*)pS, *pSe = pSi+nS; //sN[]=" ";
	for( 	pSi += gpmNINCS(pSi,pFILT);
			pSi < pSe;
			pSi += gpmNINCS(pSi,pFILT), nAT++ ) {

		if( !*pSi )
			break;

		pAn = (I8x2*)Ux(nAT,sizeof(*pAn));
		pAn->y = pSe-pSi;
		*pAn = pSi;
		if( !pAn->alf ) {

			pSi += gpfABCvan( (U1*)pSi, (U1*)pSe, nUTF8, gpaALFsub2 );
			switch( *pSi ){
				case 0:
					pSi = pSe;
					break;
				case '\r':
				case '\n':
					pSi += gpmNINCS(pSi, "\r\n" );
					pAn->alf = gpeALF_CRLF;
					pAn->y = pSi-pS;
					continue;
				case '+':
					pSi++;
					pAn->alf = gpeALF_PLUS;
					pAn->y = pSi-pS;
					continue;
				case '\"':
					pSi++;
					pAn->alf = gpeALF_MRK;
					pAn->y = pSi-pS;
					continue;
				case ',':
					pSi++;
					pAn->alf = gpeALF_CM;
					pAn->y = pSi-pS;
					continue;
				default: break;
			}
			nAT--;
			continue;
		}
		pSi+=pAn->y;
		pAn->y = pSi-pS;
	}
	pAn = (I8x2*)Ux(nAT,sizeof(*pAn));
	pAn->y = pSe-pS;
	return nAT;
}
