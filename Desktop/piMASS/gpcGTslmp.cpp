#include "gpcgt.h"
#include "gpcwin.h"
extern U1 gpaALFadd[];
extern char gpsTAB[], *gppTAB;
//extern char gp_sSLMP_read[];
/// ---------- SLMP -------------
// SNo.NnSrUn..MsLen.Mtm.
// +-->+>+>+-->+>+-->1--4   8  12  16  20  24	len 24 0x18
// |234|2|2|234|2|234|234|234|234|2|23456|234
// 500000FF03FF000018000004010000D*0000120001
// +-->+>+>+-->+>+-->+-->5    11        21 24
// D00000FF03FF0000440000
// 0032 000A DEAD 0000
// 0000 0000 0000 0000
// 0000 0000 0000 0000
// 0000 0000 0000 0000

gpcLZY* gpcGT::GTslmpOS( gpcLZY* pANS, U1* pSTR, gpcMASS& mass, SOCKET sockUSR )
{
	U8 s = -1, nLEN;
	U4 n = gpmSTRLEN( pSTR );
	if( this ? !n : true )
		return pANS->lzyFRMT( s, "nonsens" );

	gpcLZY	*pLZYinp = mass.GTlzyALL.LZY( gpdGTlzyIDinp(TnID) ),
			*pLZYout = NULL,
			*pLZYusr = mass.GTlzyALL.LZY( gpdGTlzyIDusr(TnID) );
	///-----------------------------
	/// UJ felhasználó?
	U4 iSOCK = 0, nSOCK = 0;
	if( SOCKET* pSOCK = gpmLZYvali( SOCKET, pLZYusr) )
	{
        nSOCK = gpmLZYn(pLZYusr,sockUSR);
        for( iSOCK = 0; iSOCK < nSOCK; iSOCK++ )
        {
			if( pSOCK[iSOCK] != sockUSR )
				continue;
			// nem új bent van a listában
			break;
        }
	}
	if( iSOCK >= nSOCK )
	{
		// új felhasználó!
		pLZYusr->lzyADD( &sockUSR, sizeof(sockUSR), s = -1 );
		iSOCK = nSOCK;
		nSOCK = gpmLZYn(pLZYusr,sockUSR);
	}
	///-----------------------------
	U2 nU2 = gpmLZYn( pLZYinp, U2 ), *pU2i = NULL, *pU2o = NULL;
	I2* pI2;
	if( !nU2 )
		return pANS->lzyFRMT( s, "nonsens" );

	U1 	sCOM[] = "ABCD",
		*pCOM, *pEND = pSTR+n, *pNUM;
	U4& comA = *(U4*)sCOM, iCin, iCou, iEmpty = 0, iNUM = -1, u4TMP;
	// na nézzük mit akar
	I8x2 an;
	double d8;

	for( pSTR += gpmNINCS( pSTR, " \t\a\r\n;," ); *pSTR; pSTR += gpmNINCS( pSTR, " \t\a\r\n;," ) )
	{
		an.num = pEND-(pCOM = pSTR);
		an = pCOM;
		pSTR += an.num;
		if( an.alf )
		{
			iNUM = 0;
			switch( an.alf )
			{
				case gpeALF_FORMAT:
					pU2o = pU2o ? pU2o : (U2*)(pLZYout=mass.GTlzyALL.LZY(gpdGTlzyIDref(TnID)))->u2VALID(pLZYinp);
					if( !pU2o )
						return pANS->lzyFRMT( s, "nonsens" );
					gpmZn( pU2o, nU2 );
					continue;

				case gpeALF_HELO:
				case gpeALF_HELLO:
					return pANS->lzyFRMT( s = -1, "Hello! %d", iCou );
				case gpeALF_HELP:
					return pANS->lzyFRMT( s = -1, "ReadMe.txt %d", iCou );
				case gpeALF_LINE:
					return pANS->lzyFRMT( s = -1, "Line.txt %d", iCou );
				case gpeALF_JOIN:
					return pANS->lzyFRMT( s = -1, "Join.txt %d", iCou );

				case gpeALF_POS:
					iNUM = 0;
					break;
				case gpeALF_DIR:
					iNUM = 3;
					break;
				case gpeALF_SPEED:
					iNUM = 6;
					break;
				case gpeALF_TOOL:
					iNUM = 7;
					break;
				default:
					if( an.num >= 4 ) {
						comA = *(U4*)pCOM;
						iEmpty = nU2;
						if( pU2i = (U2*)pLZYinp->p_alloc )
						for( iCin = 17, nU2 = iEmpty; iCin < nU2; iCin+=32 )
						{
							if( *(U4*)(pU2i+iCin) == comA )
								break;	// iCin menjen végig
							if( *(U4*)(pU2i+iCin) )
								continue;
							if( iEmpty < iCin )
								continue;

							iEmpty = iCin;
						}

						if( iCin >= nU2 )
						if( iEmpty < nU2 )
						{
							pU2o = pU2o ? pU2o : (U2*)(pLZYout=mass.GTlzyALL.LZY(gpdGTlzyIDref(TnID)))->u2VALID(pLZYinp);
							if( !pU2o )
								return pANS->lzyFRMT( s, "nonsens" );

							*(U4*)(pU2o+iEmpty) = comA;
							gpmSTRnCPY( pU2o+iEmpty-6, "PxPyPzDaDbDc", 12 );
						}

					}
					continue;
			}
		} else if( iNUM > 7 )
			return pANS->lzyFRMT( s, "nonsens" );
		else
			iNUM++;


		pNUM = pSTR;
		an.num = gpfSTR2I8( pNUM, &pSTR );
		if( *pSTR != '.' )
			d8 = 0.0;
		else {
			d8 = an.num;
			if( an.num < 0 )
				d8 -= strtod( (char*)pSTR, (char**)&pSTR );
			else
				d8 += strtod( (char*)pSTR, (char**)&pSTR );
		}

		pU2o = pU2o ? pU2o : (U2*)(pLZYout=mass.GTlzyALL.LZY(gpdGTlzyIDref(TnID)))->u2VALID(pLZYinp);
		if( !pU2o )
			return pANS->lzyFRMT( s, "nonsens" );
		pI2 = (I2*)pU2o;
		iCou = iCin+2;
        switch(iNUM)
        {
				// POS
			case 0:
			case 1:
			case 2:
				// DIR
			case 3:
			case 4:
			case 5:
				if( d8 != 0.0 )
					pI2[iCou+iNUM] = d8*16.0;
				else
					pI2[iCou+iNUM] = an.num*16;
				break;
			case 6:
				// SPEED
				u4TMP = (*(U4*)(pU2o+iCou+iNUM))>>24;
				for( U1 i = 0; i < 6; i++ )
				{
					u4TMP <<= 4;
					u4TMP |= an.num%10;
					an.num /= 10;
				}
				*(U4*)(pU2o+iCou+iNUM) = u4TMP;
				break;
			case 7:
				// TOOL
				((char*)(pU2o+iCou+iNUM))[1] = an.num;
				break;
        }

	}
	if( !pU2i )
		return pANS->lzyFRMT( s, "nonsens" );

	if( iCin < nU2 )
	{
		if( !pI2 )
			pI2 = (I2*)pU2i;
		comA = *(U4*)(pU2i+iCin);
		iCou = iCin+2;
		pANS = pANS->lzyFRMT(
								s = -1, "%s x:%.2fmm y:%.2fmm z:%.2fmm A:%.2f° B:%.2f° C:%.2f°", sCOM,
								double(pI2[iCou])/16.0, double(pI2[iCou+1])/16.0, double(pI2[iCou+2])/16.0,
								double(pI2[iCou+3])/16.0, double(pI2[iCou+4])/16.0, double(pI2[iCou+5])/16.0
							);
		return pANS;
	}
    return pANS->lzyFRMT( s = -1, "ok" );
}

void gpcGT::GTslmp( gpcGT& mom, gpcWIN* pWIN, gpcGTall* pALL )
{
	U8 nOUT = GTout( pWIN ), s;
	if( nOUT )
		return;

	if( aGTcrs[1] == 's' && !aGTcrs[0] )
		return GTslmpBINref( mom, pWIN, pALL );

	U4 nLEN = 0, nU2;
	gpcMASS& mass = *(pWIN->piMASS);
	gpcGT* pGTusr = NULL;
	gpcLZY	*pLZYinp = mass.GTlzyALL.LZY( gpdGTlzyIDinp(TnID) ),
			*pLZYout = mass.GTlzyALL.LZY( gpdGTlzyIDref(TnID) ),
			*pLZYusr = NULL,
			*pLZYdead = NULL;
	U2		*pU2inp = gpmLZYvali( U2, pLZYinp ),
			*pU2out = gpmLZYvali( U2, pLZYout ),
			*pU2dead = NULL;
	SOCKET	*pSOCK;
	U4		nSOCK, iLEN = 0;
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
		if( pINP->n_load < 18+nSUB )
		{
			// nincsen elég adat még
			// az nLEN megállapításához sem
			pINP = pINP->lzySUB( s = 0, nSUB );
			return;
		}

		//pSTR = pD000;

		// SNo.NnSnUn..MsLen.End.
		//    4   8  12  1618
		// 1234567890123456780000
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

		pD000 += 12;	// saláta
		char 	sLEN[] = "0x000000",	*pL = sLEN+2,
				sWORD[] = "0x0000",		*pW = sWORD+2;

		nLEN = gpfSTR2U8( gpmMEMCPY( pL, pD000, 6 )-2, NULL );
		pD000 += 6;
		if( pINP->n_load < 18+nLEN+nSUB )
			return; // még nem jött le az egész

		iCNT++;
		pU2inp = (U2*)(pLZYinp->lzyINS( NULL, nLEN/2, s = 0, nLEN/2 )->p_alloc);
		*pU2inp = gpfSTR2U8( gpmMEMCPY( pW, pD000, 4 )-2, NULL );
		if( !*pU2inp )
		{
			/// Good!
			for( iLEN = 4; iLEN < nLEN; iLEN+=4 )
				pU2inp[iLEN>>2] = gpfSTR2U8( gpmMEMCPY( pW, pD000+iLEN, 4 )-2, NULL );
		} else {
			/// error
			if( pALL )
			{
				/// üzenet a felhasználóknak
				pSOCK = gpmLZYvali( SOCKET, pLZYusr );
				if( !pSOCK )
				{
					pLZYusr = mass.GTlzyALL.LZY( gpdGTlzyIDusr(TnID) );
					pSOCK = gpmLZYvali( SOCKET, pLZYusr );
					nSOCK = gpmLZYn( pLZYusr, SOCKET );
				}

				for( U4 iS = 0; iS < nSOCK; iS++ )
				{
					pGTusr = pALL->GT( pSOCK[iS] );
					if( pGTusr->bGTdie() )
						continue;
					pGTusr->pOUT = pGTusr->pOUT->lzyFRMT( s = -1, "\r\n%s\r\n", pD000-18, pGTusr->iCNT );
					pGTusr->GTback();
				}
			}

			switch( *pU2inp )
			{
				case 0xc050:
					// ASCII-ból binárisba lett kapcsolva


					/*aGTcrs[1] = 's';
					aGTcrs[0] = 0; // NEM ASCII
					pOUT = pOUT->lzyADD( gpaSLMPbin[1].aU1, sizeof(gpaSLMPbin[1]), s = -1 );
					gpmDEL(pINP);
					return;*/
					break;
				default:
					break;
			}

		}

		nSUB += 18+nLEN;
		iCNT++;
		pINP->lzySUB( s = 0, nSUB );
		nSUB = 0;

		if( nSYNsum < nSYNdo ) {
			nSYNsum = nSYNdo;
			if( *pU2inp )
				return;

			pOUT = pOUT->lzyFRMT( s = -1, gpdSLMP_recv_LN4SL6N4, 0x18, 0, gpdSLMPnDEV );
			return;
		}
		if( iLEN <= 4 )
			return;

		if( pINP->n_load )
			return;

		//gpmDEL(pINP);
		pU2inp = gpmLZYvali( U2, pLZYinp);
	}


	nLEN = pU2inp ? pLZYinp->n_load : 0;
	if( !nLEN )
		return;


	if( !pLZYout )
		return;	// valamiért nem tudott csinálni

	if( !pLZYout->n_load )
	{
		// nem volt benne semmi azaz használjuk azt amit a robot adott
		pLZYout->lzyADD( pU2inp, nLEN, s = 0 );
		iCNT++;
		return;
	}

	if( !pU2out )
		pU2out = gpmLZYvali( U2, pLZYout );
	if( nLEN > pLZYout->n_load )
		nLEN = pLZYout->n_load;

	nLEN /= sizeof(*pU2inp);
	if( nLEN < 2 )
		return;

	nLEN--;
	pU2inp++;
	pU2out++;

	U4 	iU2 = gpmMEMCMP( pU2inp, pU2out, nLEN*sizeof(*pU2inp) )/sizeof(*pU2inp),
		i_cpy, n_cpy,
		nOld = pOUT ? pOUT->n_load : 0;

	while( iU2 < nLEN )
	{
		i_cpy = iU2;
		while( iU2 < nLEN )
		{
			if( pU2inp[iU2] == pU2out[iU2] )
				break;

			iU2++;
		}

		if( n_cpy = iU2-i_cpy )
		{
			//iU2 += n_cpy;
			if( iU2 > nLEN )
				iU2 = nLEN;
			n_cpy = iU2-i_cpy;

			// \n send dadogjon
			if( nOld )
			{
				aGTcrs[1] = 's';
				pOUT->p_alloc[pOUT->n_load] = aGTcrs[0] = '\n';
				pOUT->n_load++;
				pOUT->p_alloc[pOUT->n_load] = 0;
			}
			//		               +------- Len: 0x28 = 32+8 = 40 --------+
			//   SNo.NnSnUn..MsLen.Mtm.Com.Sub.D.Slot..Nw..Data............
			//   +-->+>+>+-->+>+-->.1--.4  .8  .12 .16 .20 .24 .28 .32 .36 .40
			//   +-->+>+>+-->+>+-->.x00.x04.x08.x0C.x10.x14.x18.x1c.x20.x24.0x28
			//   500000FF03FF000028000014010000D*0000100004.   .   .   .   .
			// \n500000FF03FF000020000014010000D*000000000265686f6c
			pOUT = pOUT->lzyFRMT( s = -1, gpdSLMP_send_LN4SL6N4, 0x18 + 4*n_cpy, i_cpy, n_cpy );
			pOUT->lzyINS( NULL, 4*n_cpy, s = -1, -1 );

			for(  ; i_cpy < iU2; i_cpy++ )
				s += sprintf( (char*)pOUT->p_alloc+s, "%0.4X", pU2out[i_cpy] );

			if( iU2 >= nLEN )
				break;
		}

		iU2 += gpmMEMCMP( pU2inp+iU2, pU2out+iU2, (nLEN-iU2)*sizeof(*pU2inp) )/sizeof(*pU2inp);
	}

	if( nOld < (pOUT ? pOUT->n_load : 0) )
	{
		nSYNsum = nSYNdo;
		nSYNdo++; // küldtem valamit várom a választ

		if( pALL )
		{
			pSOCK = gpmLZYvali( SOCKET, pLZYusr );
			if( !pSOCK )
			{
				//pLZYusr = mass.GTlzyALL.LZY( gpdGTlzyIDusr(TnID) );
				pSOCK = gpmLZYvali( SOCKET, pLZYusr=mass.GTlzyALL.LZY(gpdGTlzyIDusr(TnID)) ); //pLZYusr );
				nSOCK = gpmLZYn( pLZYusr, SOCKET );
			}

			for( U4 iS = 0; iS < nSOCK; iS++ )
			{
				pGTusr = pALL->GT( pSOCK[iS] );
				if( !pGTusr )
					continue;
				pGTusr->pOUT = pGTusr->pOUT->lzyFRMT( s = -1, "\r%s\r\n", pOUT->p_alloc+nOld );
				pGTusr->GTback();
			}
		}
	}

	iCNT++;
}


///-------------------------------------
///              - REF -
///-------------------------------------

gpcLZY* gpcGT::GTslmpOSref( gpcLZY* pANS, U1* pSTR, gpcMASS& mass, SOCKET sockUSR ) {
	U8 s = -1, nLEN;
	U4 n = gpmSTRLEN( pSTR );
	if( this ? !n : true )
		return pANS->lzyFRMT( s, "nonsens" );
	U1* pEND = pSTR+n, *pCOM;
	I8x2 an;
	U1 sCOM[] = "ABCD";
	U4& com = *(U4*)sCOM, iCin, iCou;
	U2 nU2, *pU2i = NULL, *pU2o = NULL;
	gpcLZY	*pLZYinp = NULL,
			*pLZYout = NULL,
			*pLZYusr = mass.GTlzyALL.LZY( gpdGTlzyIDusr(TnID) );
	U4 iSOCK = 0, nSOCK = 0;
	if( SOCKET* pSOCK = gpmLZYvali( SOCKET, pLZYusr) )
	{
        nSOCK = gpmLZYn(pLZYusr,sockUSR);
        for( iSOCK = 0; iSOCK < nSOCK; iSOCK++ )
        {
			if( pSOCK[iSOCK] != sockUSR )
				continue;

			break;
        }
	}
	if( iSOCK >= nSOCK )
	{
		pLZYusr->lzyADD( &sockUSR, sizeof(sockUSR), s = -1 );
		iSOCK = nSOCK;
		nSOCK = gpmLZYn(pLZYusr,sockUSR);
	}

	for( pSTR += gpmNINCS( pSTR, " \t\a\r\n;" ); *pSTR; pSTR += gpmNINCS( pSTR, " \t\a\r\n;" ) )
	{
		pCOM = pSTR;
		an.num = pEND-pSTR;
		an = pCOM;
		if( an.num )
		{
			com = *(U4*)pCOM;
			pSTR += an.num;
			if( !pLZYinp )
				pLZYinp=mass.GTlzyALL.LZY( gpdGTlzyIDinp(TnID) );

			if( pU2i = (U2*)pLZYinp->p_alloc )
			for( iCin = 17, nU2 = pLZYinp->n_load/sizeof(U2); iCin < nU2; iCin+=8 )
			{
				if( *(U4*)(pU2i+iCin) == com )
					break;	// iCin menjen végig
			}

			if( iCin >= nU2 )
			{
				iCou = iCin = nU2;
				// nem vol egyáltalán
				pU2o = (U2*)(pLZYout->u2VALID(pLZYinp,pU2o));
				if( !pU2o )
					pU2o = (U2*)((pLZYout=mass.GTlzyALL.LZY(gpdGTlzyIDref(TnID)))->u2VALID(pLZYinp));

				if( pU2o )
				for( iCou = 17, nU2 = pLZYout->n_load/sizeof(U2); iCou < nU2; iCou+=8 )
				{
					if( pU2o[iCou] ? *(U4*)(pU2o+iCou) == com : true )
						break;
				}
			}

			if( iCou > iCin )
				iCou = iCin;

			if( !pU2o )
				pU2o = (U2*)((pLZYout=mass.GTlzyALL.LZY(gpdGTlzyIDref(TnID)))->u2VALID(pLZYinp));

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
				return pANS->lzyFRMT( s = -1, " %d %s", iCou, sCOM );
				*(U4*)(pU2o+iCou) = 0;
				break;
		}
	}
    return pANS->lzyFRMT( s, "ok" );
}
void gpcGT::GTslmpREF( gpcGT& mom, gpcWIN* pWIN, gpcGTall* pALL ) {
	U8 nOUT = GTout( pWIN ), s;
	if( nOUT )
		return;

	if( aGTcrs[1] == 's' && !aGTcrs[0] )
		return GTslmpBINref( mom, pWIN, pALL );

	U4 nLEN = 0, nU2;
	gpcMASS& mass = *(pWIN->piMASS);
	gpcGT* pGTusr = NULL;
	gpcLZY	*pLZYinp = mass.GTlzyALL.LZY( gpdGTlzyIDinp(TnID) ),
			*pLZYout = mass.GTlzyALL.LZY( gpdGTlzyIDref(TnID) ),
			*pLZYusr = NULL,
			*pLZYdead = NULL;
	U2		*pU2inp = gpmLZYvali( U2, pLZYinp ),
			*pU2out = gpmLZYvali( U2, pLZYout ),
			*pU2dead = NULL;
	SOCKET	*pSOCK;
	U4		nSOCK, iLEN = 0;
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
		if( pINP->n_load < 18+nSUB )
		{
			// nincsen elég adat még
			// az nLEN megállapításához sem
			pINP = pINP->lzySUB( s = 0, nSUB );
			return;
		}

		//pSTR = pD000;

		// SNo.NnSnUn..MsLen.End.
		//    4   8  12  1618
		// 1234567890123456780000
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

		pD000 += 12;	// saláta
		char 	sLEN[] = "0x000000",	*pL = sLEN+2,
				sWORD[] = "0x0000",		*pW = sWORD+2;

		nLEN = gpfSTR2U8( gpmMEMCPY( pL, pD000, 6 )-2, NULL );
		pD000 += 6;
		if( pINP->n_load < 18+nLEN+nSUB )
			return; // még nem jött le az egész

		iCNT++;
		pU2inp = (U2*)(pLZYinp->lzyINS( NULL, nLEN/2, s = 0, nLEN/2 )->p_alloc);
		*pU2inp = gpfSTR2U8( gpmMEMCPY( pW, pD000, 4 )-2, NULL );
		if( !*pU2inp )
		{
			/// Good!
			for( iLEN = 4; iLEN < nLEN; iLEN+=4 )
				pU2inp[iLEN>>2] = gpfSTR2U8( gpmMEMCPY( pW, pD000+iLEN, 4 )-2, NULL );
		} else {
			/// error
			if( pALL )
			{
				/// üzenet a felhasználóknak
				pSOCK = gpmLZYvali( SOCKET, pLZYusr );
				if( !pSOCK )
				{
					pLZYusr = mass.GTlzyALL.LZY( gpdGTlzyIDusr(TnID) );
					pSOCK = gpmLZYvali( SOCKET, pLZYusr );
					nSOCK = gpmLZYn( pLZYusr, SOCKET );
				}

				for( U4 iS = 0; iS < nSOCK; iS++ )
				{
					pGTusr = pALL->GT( pSOCK[iS] );
					if( pGTusr->bGTdie() )
						continue;
					pGTusr->pOUT = pGTusr->pOUT->lzyFRMT( s = -1, "\r\n%s\r\n", pD000-18, pGTusr->iCNT );
					pGTusr->GTback();
				}
			}

			switch( *pU2inp )
			{
				case 0xc050:
					// ASCII-ból binárisba lett kapcsolva


					/*aGTcrs[1] = 's';
					aGTcrs[0] = 0; // NEM ASCII
					pOUT = pOUT->lzyADD( gpaSLMPbin[1].aU1, sizeof(gpaSLMPbin[1]), s = -1 );
					gpmDEL(pINP);
					return;*/
					break;
				default:
					break;
			}

		}

		nSUB += 18+nLEN;
		iCNT++;
		pINP->lzySUB( s = 0, nSUB );
		nSUB = 0;

		if( nSYNsum < nSYNdo ) {
			nSYNsum = nSYNdo;
			if( *pU2inp )
				return;

			pOUT = pOUT->lzyFRMT( s = -1, gpdSLMP_recv_LN4SL6N4, 0x18, 0, gpdSLMPnDEV );
			return;
		}
		if( iLEN <= 4 )
			return;

		if( pINP->n_load )
			return;

		//gpmDEL(pINP);
		pU2inp = gpmLZYvali( U2, pLZYinp);
	}


	nLEN = pU2inp ? pLZYinp->n_load : 0;
	if( !nLEN )
		return;


	if( !pLZYout )
		return;	// valamiért nem tudott csinálni

	if( !pLZYout->n_load )
	{
		// nem volt benne semmi azaz használjuk azt amit a robot adott
		pLZYout->lzyADD( pU2inp, nLEN, s = 0 );
		iCNT++;
		return;
	}

	if( !pU2out )
		pU2out = gpmLZYvali( U2, pLZYout );
	if( nLEN > pLZYout->n_load )
		nLEN = pLZYout->n_load;

	nLEN /= sizeof(*pU2inp);
	if( nLEN < 2 )
		return;

	nLEN--;
	pU2inp++;
	pU2out++;

	U4 	iU2 = gpmMEMCMP( pU2inp, pU2out, nLEN*sizeof(*pU2inp) )/sizeof(*pU2inp),
		i_cpy, n_cpy,
		nOld = pOUT ? pOUT->n_load : 0;

	while( iU2 < nLEN )
	{
		i_cpy = iU2;
		while( iU2 < nLEN )
		{
			if( pU2inp[iU2] == pU2out[iU2] )
				break;

			iU2++;
		}

		if( n_cpy = iU2-i_cpy )
		{
			//iU2 += n_cpy;
			if( iU2 > nLEN )
				iU2 = nLEN;
			n_cpy = iU2-i_cpy;

			// \n send dadogjon
			if( nOld )
			{
				aGTcrs[1] = 's';
				pOUT->p_alloc[pOUT->n_load] = aGTcrs[0] = '\n';
				pOUT->n_load++;
				pOUT->p_alloc[pOUT->n_load] = 0;
			}
			//		               +------- Len: 0x28 = 32+8 = 40 --------+
			//   SNo.NnSnUn..MsLen.Mtm.Com.Sub.D.Slot..Nw..Data............
			//   +-->+>+>+-->+>+-->.1--.4  .8  .12 .16 .20 .24 .28 .32 .36 .40
			//   +-->+>+>+-->+>+-->.x00.x04.x08.x0C.x10.x14.x18.x1c.x20.x24.0x28
			//   500000FF03FF000028000014010000D*0000100004.   .   .   .   .
			// \n500000FF03FF000020000014010000D*000000000265686f6c
			pOUT = pOUT->lzyFRMT( s = -1, gpdSLMP_send_LN4SL6N4, 0x18 + 4*n_cpy, i_cpy, n_cpy );
			pOUT->lzyINS( NULL, 4*n_cpy, s = -1, -1 );

			for(  ; i_cpy < iU2; i_cpy++ )
				s += sprintf( (char*)pOUT->p_alloc+s, "%0.4X", pU2out[i_cpy] );

			if( iU2 >= nLEN )
				break;
		}

		iU2 += gpmMEMCMP( pU2inp+iU2, pU2out+iU2, (nLEN-iU2)*sizeof(*pU2inp) )/sizeof(*pU2inp);
	}

	if( nOld < (pOUT ? pOUT->n_load : 0) )
	{
		nSYNsum = nSYNdo;
		nSYNdo++; // küldtem valamit várom a választ

		if( pALL )
		{
			pSOCK = gpmLZYvali( SOCKET, pLZYusr );
			if( !pSOCK )
			{
				//pLZYusr = mass.GTlzyALL.LZY( gpdGTlzyIDusr(TnID) );
				pSOCK = gpmLZYvali( SOCKET, pLZYusr=mass.GTlzyALL.LZY(gpdGTlzyIDusr(TnID)) ); //pLZYusr );
				nSOCK = gpmLZYn( pLZYusr, SOCKET );
			}

			for( U4 iS = 0; iS < nSOCK; iS++ )
			{
				pGTusr = pALL->GT( pSOCK[iS] );
				if( !pGTusr )
					continue;
				pGTusr->pOUT = pGTusr->pOUT->lzyFRMT( s = -1, "\r%s\r\n", pOUT->p_alloc+nOld );
				pGTusr->GTback();
			}
		}
	}

	iCNT++;
}

gpcSLMP gpaSLMPbin[] = {
// 	Response
//	{ SNo.  , nNET, sRNG, nUio,   MlDrp, nLEN,	EndCD,	?											},
	{ 0x00d0, 0x00, 0xff, 0x03ff, 0x00, 0x0000, 0x0010, 0x0000, 0x0000, 0x000000, 0xa8, 0x0001 },
//	READ data
//	{ SNo.  , nNET, sRNG, nUio,   MlDrp, nLEN,	tMON,	com,	subC,	iDev,	  d0,	nDev  },
	{ 0x0050, 0x00, 0xff, 0x03ff, 0x00, 0x000c, 0x0010, 0x0401, 0x0000, 0x000000, 0xa8, gpdSLMPnDEV  },
//	WRITE data
//	{ SNo.  , nNET, sRNG, nUio,   MlDrp, nLEN,	tMON,	com,	subC,	iDev,	  d0,	nDev  },
	{ 0x0050, 0x00, 0xff, 0x03ff, 0x00, 0x000e, 0x0010, 0x1401, 0x0000, 0x000000, 0xa8, 0x0001  },
};
void gpcGT::GTslmpBINref( gpcGT& mom, gpcWIN* pWIN, gpcGTall* pALL  ) {
	U8 nOUT = GTout( pWIN ), s;
	if( nOUT )
		return;

	U4 nLEN = 0;
	gpcLZY	*pLZYinp = pWIN->piMASS->GTlzyALL.LZY( gpdGTlzyIDinp(TnID) );
	U2		*pU2inp = pLZYinp ? (U2*)pLZYinp->p_alloc : NULL;

	U1* pSTR = pINP ? ( pINP->n_load ? pINP->p_alloc : NULL ) : NULL;
	if( pSTR )
	{
		/*if( nSYNsum < nSYNdo )
		{
			nSYNsum = nSYNdo;
			pOUT = pOUT->lzyADD( gpaSLMPbin[1].aU1, sizeof(gpaSLMPbin[1]), s = -1 );
			//pOUT = pOUT->lzyFRMT( s = -1, gpdSLMP_recv_LN4SL6N4, 0x18, 0, gpdSLMPnDEV );
		}*/
		if( (U1*)strcasestr( (char*)pSTR, "d000" ) == pSTR )
		{
			aGTcrs[0] = '/n';
			return GTslmp( mom, pWIN, pALL );
		}
		gpcSLMP* pD000 = (gpcSLMP*)(pSTR + gpfMEMMEM( pSTR, pINP->n_load, (U1*)&gpaSLMPbin[0].SER, sizeof(gpaSLMPbin[0].SER) ));
		if( (U1*)pD000 < pSTR )
		{
			// nincs D000
			// valami zöldségnek kell lenie
			gpmDEL(pINP);
			return;
		}
		I4 nSUB = (U1*)pD000-pSTR;
		//pSTR = pD000;

		// SNo.NnSnUn..MsLen.End.
		//    4   8  12  1618
		// 1234567890123456780000
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
		if( pINP->n_load >= pD000->nEND()+nSUB )
		{
			nLEN = pD000->nLEN;
			pU2inp = (U2*)pLZYinp->lzyINS( NULL, nLEN/2, s = 0, nLEN/2 )->p_alloc;
			gpmMEMCPY( pU2inp, pD000->pCPY(), nLEN );
			/*for( U4 i = 0; i < nLEN; i+=4 )
			{
				gpmMEMCPYoff( pW, pD000+i, 4 );
				pU2inp[i>>2] = gpfSTR2U8( sWORD, NULL );
				if( i ? true : !pU2inp[i>>2] )
					continue;
				switch( pU2inp[i>>2] )
				{
					case 0xc050:
						// ASCII-ból binárisba lett kapcsolva
						aGTcrs[1] = 's';
						aGTcrs[0] = '0'; // NEM ASCII

						pOUT = pOUT->lzyADD( gpaSLMPbin[1], sizeof(gpaSLMPbin[1], s = -1 );
						gpmDEL(pINP);
						return;
				}
				break;
			}*/

			nSUB += pD000->iEndCD()+nLEN;
			iCNT++;
			nSYNsum++;
		}
		pINP->lzyINS( NULL, 0, s = 0, nSUB );
		if( pINP->n_load )
			return;
		gpmDEL(pINP);
		pU2inp = pLZYinp ? (U2*)pLZYinp->p_alloc : NULL;
	}


	nLEN = pU2inp ? pLZYinp->n_load : 0;
	if( !nLEN )
		return;

	gpcLZY	*pLZYout = pWIN->piMASS->GTlzyALL.LZY( gpdGTlzyIDref(TnID) );
	if( !pLZYout )
		return;	// valamiért nem tudott csinálni

	if( !pLZYout->n_load )
	{
		// nem volt benne semmi azaz használjuk azt amit a robot adott
		pLZYout->lzyADD( pU2inp, nLEN, s = 0 );
		iCNT++;
		return;
	}

	U2* pU2out = (U2*)pLZYout->p_alloc;
	if( nLEN > pLZYout->n_load )
		nLEN = pLZYout->n_load;

	nLEN /= sizeof(*pU2inp);
	if( nLEN < 2 )
		return;

	nLEN--;
	pU2inp++;
	pU2out++;

	U4 iU2 = gpmMEMCMP( pU2inp, pU2out, nLEN*sizeof(*pU2inp) )/sizeof(*pU2inp),
	i_cpy, n_cpy, nGD, nOld = pOUT ? pOUT->n_load : 0;
	while( iU2 < nLEN )
	{
		i_cpy = iU2;
		while( iU2 < nLEN )
		{
			if( pU2inp[iU2] == pU2out[iU2] )
				break;

			iU2++;
		}

		if( n_cpy = iU2-i_cpy )
		{
			iU2 += n_cpy;
			if( iU2 > nLEN )
				iU2 = nLEN;
			n_cpy = iU2-i_cpy;

			pOUT = pOUT->lzyINS( NULL, sizeof(*gpaSLMPbin)+n_cpy*sizeof(*pU2out), s = -1, -1 );
			gpcSLMP* pSLMP = (gpcSLMP*)(pOUT->p_alloc+s);
			*pSLMP = gpaSLMPbin[2];
			pSLMP->setIDev( i_cpy );
			pSLMP->nDev += n_cpy-1;
			pSLMP->nLEN += (n_cpy-1)*sizeof(*pU2out);
			gpmMEMCPY( pSLMP+1, pU2out+i_cpy, n_cpy*sizeof(*pU2out) );
			aGTcrs[1] = 's';
			aGTcrs[0] = 0;

			if( iU2 >= nLEN )
				break;

		}

		iU2 += gpmMEMCMP( pU2inp+iU2, pU2out+iU2, (nLEN-iU2)*sizeof(*pU2inp) )/sizeof(*pU2inp);
	}
	if( nOld >= (pOUT ? pOUT->n_load : 0) )
		return;

	nSYNsum = nSYNdo;
	nSYNdo++;

	aGTcrs[1] = 's';
	aGTcrs[0] = 0; // NEM ASCII

	pOUT = pOUT->lzyADD( gpaSLMPbin[1].aU1, sizeof(gpaSLMPbin[1]), s = -1 );
	iCNT++;
}
