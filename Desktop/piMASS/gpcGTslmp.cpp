#include "gpcgt.h"
#include "gpcwin.h"
extern U1 gpaALFadd[];
extern char gpsTAB[], *gppTAB;
//extern char gp_sSLMP_read[];
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
gpcLZY* gpcGT::GTslmpOS( gpcLZY* pANS, U1* pSTR, gpcMASS& mass )
{
	U8 s = -1, nLEN;
	U4 n = gpmSTRLEN( pSTR );
	if( this ? !n : true )
		return pANS->lzyFRMT( s, "nonsens" );
	U1* pEND = pSTR+n, *pCOM;
	I8x2 an;
	U1 sCOM[] = "ABCD";
	U4& com = *(U4*)sCOM, iCin, iCou;
	U2 nU2, *pU2i = NULL, *pU2o = NULL;
	gpcLZY	*pLZYin = NULL,
			*pLZYout = NULL;
	for( pSTR += gpmNINCS( pSTR, " \t\a\r\n;" ); *pSTR; pSTR += gpmNINCS( pSTR, " \t\a\r\n;" ) )
	{
		pCOM = pSTR;
		an.num = pEND-pSTR;
		an = pCOM;
		if( an.num )
		{
			com = *(U4*)pCOM;
			pSTR += an.num;
			if( !pLZYin )
				pLZYin=mass.GTlzy.LZY( TnID&I8x2(1,2) );

			if( pU2i = (U2*)pLZYin->p_alloc )
			for( iCin = 1, nU2 = pLZYin->n_load/sizeof(U2); iCin < nU2; iCin+=8 )
			{
				if( *(U4*)(pU2i+iCin) == com )
					break;	// iCin menjen végig
			}

			if( iCin >= nU2 )
			{
				iCou = iCin = nU2;
				// nem vol egyáltalán
				pU2o = pLZYout->u2VALID(pLZYin,pU2o);
				if( !pU2o )
					pU2o = (pLZYout=mass.GTlzy.LZY((TnID&I8x2(1,2))+I8x2(0,1)))->u2VALID(pLZYin);

				if( pU2o )
				for( iCou = 1, nU2 = pLZYout->n_load/sizeof(U2); iCou < nU2; iCou+=8 )
				{
					if( pU2o[iCou] ? *(U4*)(pU2o+iCou) == com : true )
						break;
				}
			}

			if( iCou > iCin )
				iCou = iCin;

			if( !pU2o )
				pU2o = (pLZYout=mass.GTlzy.LZY((TnID&I8x2(1,2))+I8x2(0,1)))->u2VALID(pLZYin);

			if( pU2o )
				*(U4*)(pU2o+iCou) = com;

		} else
			pSTR += gpmVAN( pSTR, " \t\a\r\n;", nLEN );

		switch( an.alf )
		{
			case gpeALF_FORMAT:
				gpmZn( pU2o, nU2 );
				break;

			case gpeALF_HELO:
			case gpeALF_HELLO:
				return pANS->lzyFRMT( s = -1, "Hello! %d", iCou );
			case gpeALF_HELP:
				return pANS->lzyFRMT( s = -1, "ReadMe.txt %d", iCou );
			case gpeALF_LINE:
				return pANS->lzyFRMT( s = -1, "Line.txt %d", iCou );
			case gpeALF_JOIN:
				return pANS->lzyFRMT( s = -1, "Join.txt %d", iCou );
			default:
				*(U4*)(pU2o+iCou) = 0;
				break;
		}
	}
    return pANS->lzyFRMT( s, "ok" );
}
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
		//     4   8  12 16
		// D00000FF03FF000804
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
			nSYNsum++;
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

	gpcLZY	*pLZYout = pWIN->piMASS->GTlzy.LZY( (TnID&I8x2(1,2))+I8x2(0,1) );
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
	i_cpy, n_cpy, nOld = pOUT ? pOUT->n_load : 0;
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
			iU2 += n_cpy;
			if( iU2 > nLEN )
				iU2 = nLEN;
			n_cpy = iU2-i_cpy;

			nSYNsum++;
			nSYNdo++;

										//		             +------- Len: 0x28 = 32+8 = 40 --------+
										// SNo.NnSnUn..MsLen.Mtm.Com.Sub.D.Slot..Nw..Data............
										// +-->+>+>+-->+>+-->1--4   8  12  16  20  24  28  32  36  40
										// 500000FF03FF000028000014010000D*0000100004 123 123
										// 500000FF03FF000020000014010000D*000000000265686f6c
			pOUT = pOUT->lzyFRMT( s = -1, "500000FF03FF00%0.4x000014010000D*%0.6x%0.4x", 0x18 + 4*n_cpy, i_cpy, n_cpy );
			pOUT->lzyINS( NULL, 4*n_cpy + 4, s = -1, -1 );
			for( ; i_cpy < iU2; i_cpy++ )
				s += sprintf( (char*)pOUT->p_alloc+s, "%0.4x", pU2out[i_cpy] );

			s += sprintf( (char*)pOUT->p_alloc+s, "0000" );
			//if( iU2 >= nLEN )
			break;
		}

		iU2 += gpmMEMCMP( pU2in+iU2, pU2out+iU2, (nLEN-iU2)*sizeof(*pU2in) )/sizeof(*pU2in);
	}
	if( nOld < (pOUT ? pOUT->n_load : 0) )
	{
		pOUT = pOUT->lzyFRMT( s = -1, gpdSLMP_read, 0x18, 0, gpdSLMPnDEV );
	}

	//nOUT = GTout( pWIN );
	iCNT++;
}

