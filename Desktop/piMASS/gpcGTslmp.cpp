#include "gpcgt.h"
#include "gpcwin.h"
extern U1 gpaALFadd[];
extern char gpsTAB[], *gppTAB;
/// ---------- SLMP -------------
// SNo.NnSnUn..MsLen.Mtm.
// +-->+>+>+-->+>+-->1--4   8  12  16  20  24	len 24 0x18
// |234|2|2|234|2|234|234|234|234|2|23456|234
// 500000FF03FF000018000004010000D*0000120001
// +-->+>+>+-->+>+-->+-->5    11        21 24
// D00000FF03FF0000440000
// 0032 000A DEAD 0000
// 0000 0000 0000 0000
// 0000 0000 0000 0000
// 0000 0000 0000 0000

void gpcGT::GTslmp( gpcGT& mom, gpcWIN* pWIN, gpcGTall* pALL  )
{
	U8 nOUT = GTout( pWIN ), s;
	if( nOUT )
		return;

	U1* pSTR = pINP ? pINP->p_alloc : NULL;
	if( !pSTR )
		return;

	U1* pANS = (U1*)strcasestr( (char*)pSTR, "d000" );
	if( !pANS )
		return;

	if( pANS-pSTR )
		pINP->lzyINS( NULL, 0, s = 0, pANS-pSTR );
	// SNo.NnSnUn..MsLen.Mtm.
	//    4   8  12  1618
	// 123456789012345678
	// D00000FF03FF000044
	//               xxxx
	//             14+-->
	if( pINP->n_load < 18 )
		return;

	pSTR = pINP ? pINP->p_alloc : NULL;
	char sWORD[] = "0x0000", *pS;
	gpmMEMCPYoff( sWORD+2, pSTR+14, 4 );
	U4 nLEN = gpfSTR2U8( sWORD, NULL );
	if( pINP->n_load < 18+nLEN )
		return;

	gpcLZY	*pLZYin = pWIN->piMASS->GTlzy.LZY( TnID&I8x2(1,2) ),
			*pLZYout = pWIN->piMASS->GTlzy.LZY( TnID&I8x2(1,2)+I8x2(0,1) );

	nLEN-=4; // kivonjuk a timert vagy mi a bubánatot
	U2		*pU2in = (U2*)pLZYin->lzyINS( NULL, nLEN/2, s = 0, nLEN/2 )->p_alloc,
			*pU2out = NULL;
	pU2in++;

	for( U4 i = 0; i < nLEN; i+=4 )
	{
		gpmMEMCPYoff( sWORD+2, pSTR+18+4 + i, 4 ); // +4 a timer
		pU2in[i/4] = gpfSTR2U8( sWORD, NULL );

	}
	iCNT++;
	pINP->lzyINS( NULL, 0, s = 0, 18+4+nLEN );

	if( pLZYout ? !pLZYout->n_load : true )
		return;

	pU2out = (U2*)pLZYout->p_alloc;
	U4	nU2 = min( pLZYout->n_load, pLZYin->n_load )/sizeof(*pU2in),
		iU2 = gpmMEMCMP( pU2in, pU2out, nU2*sizeof(*pU2in) )/sizeof(*pU2in),
		i_cpy, n_cpy;
	while( iU2 < nU2 )
	{
		i_cpy = iU2;
		while( iU2 < nU2 )
		{
			if( pU2in[iU2] == pU2out[iU2] )
				break;
			iU2++;
		}

		if( n_cpy = iU2-i_cpy )
		{
										//		             +------- Len: 0x28 = 32+8 = 40 --------+
		                                // SNo.NnSnUn..MsLen.Mtm.Com.Sub.D.Slot..Nw..Data............
		                                // +-->+>+>+-->+>+-->1--4   8  12  16  20  24  28  32  36  40
		                                // 500000FF03FF000028000014010000D*0000100004
			pOUT = pOUT->lzyFRMT( s = -1, "500000FF03FF00%0.4x000014010000D*%0.6x%0.4x", 0x18 + 4*n_cpy, i_cpy, n_cpy );
			pOUT->lzyINS( NULL, 4*n_cpy, s = -1, -1 );
			for( ; i_cpy < iU2; i_cpy++ );
            	s += sprintf( (char*)pOUT->p_alloc+s, "%0.4x", pU2out[i_cpy] );

			if( iU2 >= nU2 )
				break;
		}

        iU2 += gpmMEMCMP( pU2in+iU2, pU2out+iU2, nU2-iU2 );
	}
}
