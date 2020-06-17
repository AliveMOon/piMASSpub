#include <gpcSRC.h>


extern U1 gpaALFsub[];

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
gpeTYP I8x2::cdrMILLnum( const char* pS, U4 nS )
{
	if( !this )
		gpeTYP_null;
	char* pSe = (char*)pS;
	uy = gpfSTR2U8( pSe, &pSe );
	/// x[7s,6f,5r,4str : 3-0 nBYTE = 1<<(x&0xf) ]
	/// yz[ dimXY ] 	, w nBYTE //= 1<<(x&0xf)
	/// see you examp. -> enum gpeTYP:U4
	if( pS >= pSe )
		return gpeTYP_U8;

	dy = (double)uy + gpmSTR2D( pSe );
	return gpeTYP_D;
}
gpeTYP I8x2::cdrMILLalf( const char* pS, U4 nS )
{
	if( !this )
		gpeTYP_null;

	num = nS;
	*this = pS;
	char *pSi = (char*)pS+num, *pSe;
	gpeTYP typ = alf ? gpeTYP_A:gpeTYP_null;
	if( typ ? (num >= nS) : true )
		return typ;

	num = gpfSTR2I8( pSi, &pSe );
	return (pSe > pSi) ? typ : gpeTYP_AN;
}
