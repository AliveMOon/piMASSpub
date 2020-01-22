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


	U4 nLEN = 0;
	gpcLZY	*pLZYin = pWIN->piMASS->GTlzy.LZY( TnID&I8x2(1,2) );
	U2		*pU2in = pLZYin ? (U2*)pLZYin->p_alloc : NULL;

	U1* pSTR = pINP ? ( pINP->n_load ? pINP->p_alloc : NULL ) : NULL;
	if( pSTR )
	{
		U1	*pD000 = (U1*)strcasestr( (char*)pSTR, "d000" );
		if( pD000 < pSTR )
		{
			// nincs D000
			// valami zöldségnek kell lenie
			gpmDEL(pINP);
			return;
		}
		I4 nSUB = pD000-pSTR;
		//pSTR = pD000;

		// SNo.NnSnUn..MsLen.Mtm.
		//    4   8  12  1618
		// 123456789012345678
		// D00000FF03FF000044
		//               xxxx
		//             14+-->
		//      4    8   12   16
		// D000 00FF 03FF 000804
		//18
		// 0000 // EndCode
		// 0000 0000 0000 0000
		// 0000 0000 0000 0000
		// 0000 0000 0000
		// A090 A090 A490 EA92
		// AE95 A895 A44A
		if( pINP->n_load >= 18+nLEN+nSUB )
		{
			pD000 += 12;	// saláta
			char 	sLEN[] = "0x000000", *pL = sLEN+2,
					sWORD[] = "0x0000", *pW = sWORD+2;

			gpmMEMCPYoff( pL, pD000, 6 );
			pD000 += 6;
			nLEN = gpfSTR2U8( sLEN, NULL );
			pU2in = (U2*)pLZYin->lzyINS( NULL, nLEN/2, s = 0, nLEN/2 )->p_alloc;

			for( U4 i = 0; i < nLEN; i+=4 )
			{
				gpmMEMCPYoff( pW, pD000+i, 4 );
				pU2in[i>>2] = gpfSTR2U8( sWORD, NULL );
			}

			nSUB += 18+nLEN;
			iCNT++;
		}
		pINP->lzyINS( NULL, 0, s = 0, nSUB );
		if( pINP->n_load )
			return;
		gpmDEL(pINP);
		pU2in = pLZYin ? (U2*)pLZYin->p_alloc : NULL;
	}


	nLEN = pU2in ? pLZYin->n_load : 0;
	if( !nLEN )
		return;

	gpcLZY	*pLZYout = pWIN->piMASS->GTlzy.LZY( TnID&I8x2(1,2)+I8x2(0,1) );
	if( !pLZYout )
		return;	// valamiért nem tudott csinálni

	if( !pLZYout->n_load )
	{
		// nem volt benne semmi azaz használjuk azt amit a robot adott
		pLZYout->lzyADD( pU2in, nLEN, s = 0 );
		iCNT++;
		return;
	}

	U2* pU2out = (U2*)pLZYout->p_alloc;
	if( nLEN > pLZYout->n_load )
		nLEN = pLZYout->n_load;

	nLEN /= sizeof(*pU2in);
	if( nLEN < 2 )
		return;

	nLEN--;
	pU2in++;
	pU2out++;

	U4 iU2 = gpmMEMCMP( pU2in, pU2out, nLEN*sizeof(*pU2in) )/sizeof(*pU2in),
	i_cpy, n_cpy;
	while( iU2 < nLEN )
	{
		i_cpy = iU2;
		while( iU2 < nLEN )
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

			if( iU2 >= nLEN )
				break;
		}

		iU2 += gpmMEMCMP( pU2in+iU2, pU2out+iU2, (nLEN-iU2)*sizeof(*pU2in) )/sizeof(*pU2in);
	}

	//nOUT = GTout( pWIN );
	iCNT++;


}
