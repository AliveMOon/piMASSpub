#include <gpcSRC.h>


extern U1 gpaALFsub[];
int I8x2::alfFND( gpeALF af, int n ) {
	I8x2* pAT = this;
	for( int iAT = 0; iAT < n; iAT++ ) {
		if( pAT[iAT].alf != af )
			continue;
		return iAT;
	}
	return n;
}
int I8x2::alfN( gpeALF af, int n_a ) {
	I8x2* pAT = this;
	int n = 0;
	for( int iAT = 0; iAT < n_a; iAT++ ) {
		if( pAT[iAT].alf != af )
			continue;
		n++;
	}
	return n;
}

int I8x2::aALFfnd( const gpeALF* aALF, int n, int nA ) {
	I8x2* pAT = this;
	for( int iAT = 0, j; iAT < n; iAT++ ) {
		if( pAT[iAT].alf == gpeALF_MRK ) {
			iAT++;
			iAT += pAT[iAT].alfFND( gpeALF_MRK, n-iAT );
			continue;
		}

		for(j = 0; j < nA; j++){
			if( pAT[iAT].alf == aALF[j] )
				return iAT;
		}
	}

	return n;
}
I8x2& I8x2::operator = ( const U1* pS )
{
	if( num ? ( pS ? !*pS : true ) : true )
		return null();
	if( num > 14 )
		num = 14;
	U1* pE = (U1*)pS + num;
	alf = gpfSTR2ALF( pS, pE, &pE );
	num = pE-pS;
	return *this;
}
I8x2& I8x2::operator = ( const char* pS ) { return *this = (U1*)pS; }
gpeTYP I8x2::cdrMILLnum( const char* pS, U4 nS ) {
	if( !this )
		gpeTYP_null;
	char* pSe = (char*)pS;
	uy = gpfSTR2U8( pSe, &pSe );
	/// x[7s,6f,5r,4str : 3-0 nBYTE = 1<<(x&0xf) ]
	/// yz[ dimXY ] 	, w nBYTE //= 1<<(x&0xf)
	/// see you examp. -> enum gpeTYP:U4
	if( *pSe != '.' )
	if( uy > 0xFFff )
		return uy > 0xFFFFffff ? gpeTYP_U8 : gpeTYP_U4;
	else
		return uy > 0xFf ? gpeTYP_U2 : gpeTYP_U1;

	dy = (double)uy + gpmSTR2D( pSe );
	double ady = dy < 0.0 ? -dy : dy;
	if( (ady>(1024.0*1024.0)) || (ady<(1.0/1024.0)) )  // 2^23
		return gpeTYP_D;

	return gpeTYP_F;
}
gpeTYP I8x2::cdrMILLalf( const char* pS, U4 nS )
{
	if( !this )
		gpeTYP_null;

	num = nS;
	*this = pS;
	char *pSi = (char*)pS+num, *pSe;
	gpeTYP typ = alf ? gpeTYP_sA8:gpeTYP_null;
	if( typ ? (num >= nS) : true )
		return typ;

	num = gpfSTR2I8( pSi, &pSe );
	return (pSe > pSi) ? gpeTYP_sA8N : typ;
}

gpeCsz I8x2::gpCszNUM( const char* pS, U4 nS )
{
	if( !this )
		return gpeCsz_OFF;
	char* pSe = (char*)pS;
	ux = gpfSTR2U8( pSe, &pSe );
	if( *pSe != '.' )
	if( ux > 0xFFff )
		return ux > 0xFFFFffff ? gpeCsz_q : gpeCsz_l;
	else
		return ux > 0xFf ? gpeCsz_w : gpeCsz_b;

	dx = (double)ux + gpmSTR2D( pSe );
	double adx = dx < 0.0 ? -dx : dx;
	if( (adx>(1024.0*1024.0)) || (adx<(1.0/1024.0)) )  // 2^23
		return gpeCsz_d;

	return gpeCsz_f;
}
gpeCsz I8x2::gpCszALF( const char* pS, U4 nS )
{
	if( !this )
		return gpeCsz_OFF;

	num = nS;
	*this = pS;
	char *pSi = (char*)pS+num, *pSe;
	gpeCsz typ = alf ? gpeCsz_a:gpeCsz_OFF;
	if( typ ? (num >= nS) : true )
		return typ;

	num = gpfSTR2I8( pSi, &pSe );
	return (pSe > pSi) ? gpeCsz_c : typ;
}
