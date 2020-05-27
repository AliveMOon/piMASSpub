#include "gpcgt.h"
#include "gpcwin.h"
extern U1 gpaALFadd[];
extern char gpsTAB[], *gppTAB;
U4x4 gpaZSwr[] = {
	//	write,	nW, 		read,		nR
	{ 	0xA,  	gpdZSnWu2,	10000, gpdZSnRu2	},
	{ 	0x40A,  gpdZSnWu2,	20000, gpdZSnRu2	},
};

gpcZS& gpcZS::operator &= ( const gpcDrc& D ) {
	gpmCLR;
	io128.y = -1;
	return *this;
}
gpcZS& gpcZS::operator = ( const gpcDrc& D ) {
	gpmCLR;
	switch( D.JD.y )
	{
		case 1:
		case 2:
		case 3:
		case 9:
			{
				switch( D.oCTRL.z )
				{
					case 10:
					case 11:
						if( D.JD.y > 1 )
							gpmMcpyOF( &io128.y, &D.oCTRL.y, 3 );

						gpmMcpyOF( &aPOS,	&D.oXYZ.x, 3 );
						gpmMcpyOF( &aABC,	&D.oABC.x, 3 );
						io128.w = D.MPosS;
						break;
					default:

						break;
				}
			} break;
		case 4:
		case 5:
		case 6:
			io128.y = D.oCTRL.y;
			break;
		default:
			break;
	}
	return *this;
}
gpcZS::gpcZS( const gpcDrc& D ) {
	gpmCLR;
	*this = D;
}
void gpcGT::GTslmpDrcZS( gpcGT& mom, gpcWIN* pWIN, gpcGTall* pALL ) {
	U8 nOUT = GTout( pWIN ), s;
	if( nOUT )
		return;

	if( sGTent[2] == 's' && sGTent[0] == 'b' )
		return GTslmpBINref( mom, pWIN, pALL );

	//, nPAD = gpdZSpad, nPADu2 = nPAD/sizeof(U2);
	gpcMASS& mass = *(pWIN->piMASS);
	gpcGT* pGTusr = NULL;
	gpcLZY	*pLZYinp = mass.GTlzyALL.LZY( gpdGTlzyIDinp(TnID) ),
			//*pLZYout = mass.GTlzyALL.LZY( gpdGTlzyIDref(TnID) ),
			*pLZYusr = NULL,
			*pLZYdead = NULL;
	gpcZSnD	*pZSnD = gpmLZYvali( gpcZSnD, pLZYinp );
			//&ZSnD = *pZSnD;

	U2		*pU2io	= NULL, *pU2,
			*pU2inp = pZSnD->pU2inp();

	SOCKET	*pSOCK;
	U4 nSOCK, nD0 = 0, iD0 = 0, eD0, iDRC;
	U1* pSTR = pINP ? ( pINP->n_load ? pINP->p_alloc : NULL ) : NULL;
	if( pSTR )
	{
		U1	*pD0 = (U1*)strcasestr( (char*)pSTR, "d000" );
		if( pD0 < pSTR )
		{
			// nincs D000
			// valami zöldségnek kell lenie
			gpmDEL(pINP);
			return;
		}
		I4 nSUB = pD0-pSTR;
		if( pINP->n_load < 18+nSUB )
		{
			// nincsen elég adat még
			// az nLEN megállapításához sem
			pINP = pINP->lzySUB( s = 0, nSUB );
			return;
		}

		{//SNo.NnSnUn..MsLen.End.
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
		}

		pD0 += 12;	// saláta
		char 	sLEN[] = "0x000000",	*pL = sLEN+2,
				sWORD[] = "0x0000",		*pW = sWORD+2;

		nD0 = gpfSTR2U8( gpmMcpy(pL,pD0,6)-2, NULL );
		pD0 += 6;
		if( pINP->n_load < 18+nD0+nSUB )
			return; // még nem jött le az egész

		iCNT++;

		if( !pU2inp )
		{
			pLZYinp->lzyINS( NULL, sizeof(gpcZSnD), s=0, sizeof(gpcZSnD) );
			pZSnD = gpmLZYvali( gpcZSnD, pLZYinp );
			pZSnD->reset( 0 );

			pU2inp = pZSnD->pU2inp();
		}

		/// ERROR?
		pZSnD->ioSW.aU2[0] = gpfSTR2U8( gpmMcpy(pW,pD0,4)-2, NULL );
		if( pZSnD->iWR() && !pZSnD->ioSW.aU2[0] )
		{
			// olvastunk && nincsen hiba
			/// Good!
			pU2 = pU2io = pZSnD->pZSioU2();
			eD0 = gpaZSwr[pZSnD->iDrc()].a4x2[pZSnD->iWR()].y*4; // 0000 u2ASCii
			if( eD0 > nD0 )
				eD0 = nD0;

			for( 	iD0 = 4; //, pU2 += 10;
					iD0 < eD0; iD0 +=4, pU2++ )
				*pU2 = gpfSTR2U8( gpmMcpy(pW,pD0+iD0,4)-2, NULL );

			if( iD0 > 4 )
			{
				iDRC = pZSnD->iDrc();
				// nem fecsegés, hanem adat
				pZSnD->aDrc[iDRC] = pZSnD->ioZS();
				pZSnD->aDrc[iDRC].msSMR2.w = pWIN->mSEC.x;
				pZSnD->stpPULL();
				if( pZSnD->bPULL() ) // másikat is lehuzzuk
					pOUT = pZSnD->pulling( pOUT, gpaZSwr );
			}

		} else {
			/// error
			if( pALL ) {
				/// üzenet a felhasználóknak
				pSOCK = gpmLZYvali( SOCKET, pLZYusr );
				if( !pSOCK )
				{
					pLZYusr = mass.GTlzyALL.LZY( gpdGTlzyIDusr(TnID) );
					pSOCK = gpmLZYvali( SOCKET, pLZYusr );
					nSOCK = gpmLZYload( pLZYusr, SOCKET );
				}

				for( U4 iS = 0; iS < nSOCK; iS++ )
				{
					pGTusr = pALL->GT( pSOCK[iS] );
					if( pGTusr->bGTdie() )
						continue;
					pGTusr->pOUT = pGTusr->pOUT->lzyFRMT( s = -1, "\r\n %d.%s\r\n", pGTusr->iCNT, pD0-18 );
					pGTusr->GTback();
				}
			}

			switch( pZSnD->ioSW.aU2[0] )
			{
				case 0x4031:
					// nyugta?
					pOUT = pZSnD->pulling( pOUT, gpaZSwr );
					break;
				case 0x0000: // good!
					break;
				case 0xc050:
					break;
				case 0xc051:
					pOUT = pZSnD->pulling( pOUT, gpaZSwr );
					break;
				default:
					break;
			}

		}

		nSUB += 18+nD0;
		pINP->lzySUB( s = 0, nSUB );
		nSUB = 0;

		if( nOUT < pOUT->nLD() )
			return; // kérdeztem valamit amig nincs válasz visza térünk
		if( iD0 <= 4 )
			return;
		if( pINP->n_load )
			return;
		pU2inp = pZSnD->pU2inp();
	}

	if( pZSnD->bWAIT() )
	{
		if( pZSnD ? pZSnD->pc.x > 10 : false )
			pOUT = pZSnD->pulling( pOUT, gpaZSwr );
		return;
	}

	iCNT++;
	//gpcZSnD& ZSnD = *pZSnD;
	if( pZSnD->bPULL() ) // lekérdeztünk mindenkit?
	{
		nSYNsum = nSYNdo;
		pOUT = pZSnD->pulling( pOUT, gpaZSwr );
		return;
	}


	/// ---------------------------------
	/// Drc judo filter
	/// ---------------------------------
	iD0 = pZSnD->stpPUSH( true );

	U4x4& JD = pZSnD->aDrc[iD0].JD;
	U4 JDy = JD.y;
	U2	*pB = (U2*)&(
						pZSnD->aZSio[4+iD0]
						= pZSnD->aDrc[iD0]
					), // pB-ben a JUDO elötti
		*pA = (U2*)&(
						pZSnD->aZSio[iD0*2]
						= pZSnD->aDrc[iD0].judoZS( pZSnD->aZSio[iD0*2+1], pWIN->mSEC.x )
					); // pA-ban azaz új out lesz
	if( JDy == JD.y )
	{
		pOUT = pZSnD->pulling( pOUT, gpaZSwr );
		return;
	}
	char sANSW[0x100], *pANSW = sANSW, sCOM[] = "ABCD";
	U4& comA = *(U4*)sCOM, msg = JDy+JD.y*0x10;
	comA = pZSnD->aDrc[iD0].NMnDIF.au4x2[0].x;
	I4x4 iXYZ = pZSnD->aDrc[iD0].iXYZ;
	switch( msg )
	{
		case 0x10:
			pANSW += sprintf( pANSW, "%s Turn start X:%d", sCOM, JD.x  );
			break;
		case 0x21:

			pANSW += sprintf( pANSW, "%s not init zs160 X:%d", sCOM, JD.x );
			break;
		case 0x31:
			pANSW += sprintf( pANSW, "%s d160 turn on X:%d", sCOM, JD.x );
			break;
		case 0x32:
			//pANSW += sprintf( pANSW, "%s ? X:%d", sCOM, JD.x );
			break;
		case 0x43:
			pANSW += sprintf( pANSW, "%s Wait zs160 X:%d POS: %0.2fmm %0.2fmm %0.2fmm", sCOM, JD.x,
										double(iXYZ.x)/mmX(1),
										double(iXYZ.y)/mmX(1),
										double(iXYZ.z)/mmX(1) );
			break;
		case 0x54:
			//pANSW += sprintf( pANSW, "%s zs160 ok! Wait zs161 X:%d", sCOM, JD.x );
			break;
		case 0x65:
			//pANSW += sprintf( pANSW, "%s zs161 on! d161 on! Err:%d X:%d", sCOM, JD.x );
			break;
		case 0x76:
			pANSW += sprintf( pANSW, "%s Wait zs161 X:%d POS: %0.2fmm %0.2fmm %0.2fmm", sCOM, JD.x,
										double(iXYZ.x)/mmX(1),
										double(iXYZ.y)/mmX(1),
										double(iXYZ.z)/mmX(1) );
			break;
		case 0x07:
			pANSW += sprintf( pANSW, "%s Turn end! Err:%d X:%d", sCOM, JD.z, JD.x );
			break;
		default:
			pANSW = sANSW;
			break;
	}
	if( sANSW < pANSW )
	{
		pSOCK = gpmLZYvali( SOCKET, pLZYusr );
		if( !pSOCK )
		{
			pLZYusr = mass.GTlzyALL.LZY( gpdGTlzyIDusr(TnID) );
			pSOCK = gpmLZYvali( SOCKET, pLZYusr );
			nSOCK = gpmLZYload( pLZYusr, SOCKET );
		}

		for( U4 iS = 0; iS < nSOCK; iS++ )
		{
			pGTusr = pALL->GT( pSOCK[iS] );
			if( pGTusr->bGTdie() )
				continue;
			pGTusr->pOUT = pGTusr->pOUT->lzyFRMT( s = -1, "\r\n %d.%s", pGTusr->iCNT, sANSW );
			pGTusr->GTback();
		}
	}

    // itt kell majd variálni, hogy elösször maradjanak a control bitek
    // és a következő körben írjuk be
	pZSnD->aZSio[4+iD0] &= pZSnD->aDrc[iD0]; // ez a pB

	U4	iU2 = gpaZSwr[0].x, //iD0].x, mert pB és pA már el van tolva?
		nU2 = gpaZSwr[0].y,
		offU2= gpaZSwr[iD0].x-gpaZSwr[0].x; //iD0].y;
	iU2 += gpmMcmpOF( pB+iU2, pA+iU2, nU2 );
	if( iU2 >= nU2 )
	{
		// nem volt különbség
		pOUT = pZSnD->pulling( pOUT, gpaZSwr );
		return;
	}



	// talált különbséget
	// beállítjuk a pZSnD->ioSW,
	// hogy ezt kérdezze legközelebb
	U4 i_cpy = iU2, n_cpy, nGD;
	if( i_cpy ) {
		if( n_cpy = nU2-i_cpy )
		{
			// \n send dadogjon

			//		               +------- Len: 0x28 = 32+8 = 40 --------+
			//   SNo.NnSnUn..MsLen.Mtm.Com.Sub.D.Slot..Nw..Data............
			//   +-->+>+>+-->+>+-->.1--.4  .8  .12 .16 .20 .24 .28 .32 .36 .40
			//   +-->+>+>+-->+>+-->.x00.x04.x08.x0C.x10.x14.x18.x1c.x20.x24.0x28
			//   500000FF03FF000028000014010000D*0000100004.   .   .   .   .
			// \n500000FF03FF000020000014010000D*000000000265686f6c
			pOUT = pOUT->lzyFRMT( s = -1, "%s" gpdSLMP_send_LN4SL6N4, pOUT->nLD() ? "\n": "", 0x18 + 4*n_cpy, offU2+i_cpy, n_cpy );
			pOUT->lzyINS( NULL, 4*n_cpy, s = -1, -1 );
			for(  ; i_cpy < nU2; i_cpy++ )
				s += sprintf( (char*)pOUT->p_alloc+s, "%0.4X", pA[i_cpy] );
		}
		iU2 = nU2;
	}
	else while( iU2 < nU2 ) {
		i_cpy = iU2&(~0x7);
		iU2 = i_cpy+8;
		if( iU2 > nU2 )
			iU2 = nU2;
		else {
			while( iU2 < nU2 )
			{
				nGD = gpmMcmpOF( pB+iU2, pA+iU2, nU2-iU2 );
				if( nGD > 8 )
					break;
				iU2 += 8;
			}
			if( iU2 > nU2 )
				iU2 = nU2;
		}

		if( n_cpy = iU2-i_cpy )
		{
			//iU2 += n_cpy;
			if( iU2 > nU2 )
				iU2 = nU2;
			n_cpy = iU2-i_cpy;

			// \n send dadogjon

			//		               +------- Len: 0x28 = 32+8 = 40 --------+
			//   SNo.NnSnUn..MsLen.Mtm.Com.Sub.D.Slot..Nw..Data............
			//   +-->+>+>+-->+>+-->.1--.4  .8  .12 .16 .20 .24 .28 .32 .36 .40
			//   +-->+>+>+-->+>+-->.x00.x04.x08.x0C.x10.x14.x18.x1c.x20.x24.0x28
			//   500000FF03FF000028000014010000D*0000100004.   .   .   .   .
			// \n500000FF03FF000020000014010000D*000000000265686f6c
			pOUT = pOUT->lzyFRMT( s = -1, "%s" gpdSLMP_send_LN4SL6N4, pOUT->nLD() ? "\n": "", 0x18 + 4*n_cpy, i_cpy, n_cpy );
			pOUT->lzyINS( NULL, 4*n_cpy, s = -1, -1 );
			for(  ; i_cpy < iU2; i_cpy++ )
				s += sprintf( (char*)pOUT->p_alloc+s, "%0.4X", pA[i_cpy] );
			//break;
			if( iU2 >= nU2 )
				break;
		}

		iU2 += gpmMcmpOF( pB+iU2, pA+iU2, nU2-iU2);
	}

	if( !pOUT->nLD() )
	{
		pOUT = pZSnD->pulling( pOUT, gpaZSwr );
		return;
	}

	nSYNsum = nSYNdo;
	nSYNdo++;
	if( pZSnD->ioSW.y&1  )
		return;

	pZSnD->ioSW.y |= 1;
	//sGTent[2] = 's';
	//sGTent[0] = '\n';
	return;
}
