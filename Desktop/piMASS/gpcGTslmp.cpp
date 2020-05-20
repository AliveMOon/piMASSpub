#include "gpcgt.h"
#include "gpcwin.h"
extern U1 gpaALFadd[];
extern char gpsTAB[], *gppTAB;

gpcLZY* gpcGT::gpcGTslmpSTAT( gpcLZY* pANS, U2* pU2 )
{
	U1 	sCOM[] = "ABCD";
	U4 &comA = *(U4*)sCOM;

	comA = *(U4*)pU2;
	U4 spd = (*(U4*)(pU2+32));
	U8 s;
	pANS = pANS->lzyFRMT(
							s = -1, "\r\n%s x:%.2fmm y:%.2fmm z:%.2fmm A:%.2f. B:%.2f. C:%.2f. spd:%0.6x tool:%d",
							sCOM,
							double(*(U4*)(pU2+2+16+0))/100.0,
							double(*(U4*)(pU2+2+16+2))/100.0,
							double(*(U4*)(pU2+2+16+4))/100.0,

							double(*(U4*)(pU2+2+48+0))/100.0,
							double(*(U4*)(pU2+2+48+2))/100.0,
							double(*(U4*)(pU2+2+48+4))/100.0,

							spd&0xffffff,
							spd>>24

						);
	return pANS;
}

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


U4 gpaiPAD[] = {
	2, 0, 2, 0,
};
U4 gpaiDEV[] = {
	gpdZSbad, 10000, gpdZSbad+1024, 20000,
};
U4 gpanDEV[] = {
	(sizeof(gpcZS)/sizeof(U2))-2,
	(sizeof(gpcZS)/sizeof(U2))-2,
	(sizeof(gpcZS)/sizeof(U2))-2,
	(sizeof(gpcZS)/sizeof(U2))-2,
};


U4x4 gpaZSrobi[] = {
	// nameID,
	{ gpeZS_BILL, 0, 0, 0 },
	{ gpeZS_JOHN, 0, 0, 0 },
};
U4x4 gpaZSwr[] = {
	//	write,	nW, 		read,		nR
	{ 	0xA,  	gpdZSnWu2,	10000, gpdZSnRu2	},
	{ 	0x40A,  gpdZSnWu2,	20000, gpdZSnRu2	},
};

gpcZS& gpcZS::operator &= ( const gpcDrc& D )
{
	gpmCLR;
	io128.y = -1;
	return *this;
}
gpcZS& gpcZS::operator = ( const gpcDrc& D )
{
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

						gpmMcpyOF( &aPOS,	&D.oXYZ, 3 );
						gpmMcpyOF( &aABC,	&D.oABC, 3 );
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

void gpcGT::GTslmpDrc( gpcGT& mom, gpcWIN* pWIN, gpcGTall* pALL )
{
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
				pZSnD->aDrc[iDRC].ms13R2.w = pWIN->mSEC.x;
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
						= pZSnD->aDrc[iD0].judo( pZSnD->aZSio[iD0*2+1], pWIN->mSEC.x )
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



gpcLZY* gpcGT::GTslmpOS( gpcLZY* pANS, U1* pSTR, gpcMASS& mass, SOCKET sockUSR ) {
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
        nSOCK = gpmLZYload(pLZYusr,sockUSR);
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
		nSOCK = gpmLZYload(pLZYusr,sockUSR);
	}
	///-----------------------------
	U2 *pU2i = NULL, *pU2o = NULL;
	U4 nU2 = gpmLZYload( pLZYinp, *pU2i );
	//I2* pI2;
	if( !nU2 )
		return pANS->lzyFRMT( s, "nonsens" );

	U1 	sCOM[] = "ABCD",
		*pCOM, *pEND = pSTR+n, *pNUM;
	U4& comA = *(U4*)sCOM, iCin = 0, iCin2 = 0, iEmpty = 0, iNUM = -1, spd;
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
					pU2o = pU2o ? pU2o : (U2*)(pLZYout=mass.GTlzyALL.LZY(gpdGTlzyIDref(TnID)))->pVALID(pLZYinp);
					if( !pU2o )
						return pANS->lzyFRMT( s, "nonsens" );
					gpmZnOF( pU2o, nU2 );
					continue;

				case gpeALF_HELO:
				case gpeALF_HELLO:
					return pANS->lzyFRMT( s = -1, "Hello! %d", iCin );
				case gpeALF_HELP:
					return pANS->lzyFRMT( s = -1, "ReadMe.txt %d", iCin );
				case gpeALF_LINE:
					return pANS->lzyFRMT( s = -1, "Line.txt %d", iCin );
				case gpeALF_JOIN:
					return pANS->lzyFRMT( s = -1, "Join.txt %d", iCin );

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
						for( iCin = 129, nU2 = iEmpty; iCin < nU2; iCin+=64 )
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
							iCin = iEmpty;	// ha kiszeded nem olvas init adatokat

							pU2o = pU2o ? pU2o : (U2*)(pLZYout=mass.GTlzyALL.LZY(gpdGTlzyIDref(TnID)))->pVALID(pLZYinp);
							if( !pU2o )
								return pANS->lzyFRMT( s, "nonsens" );

							*(U4*)(pU2o+iEmpty) = comA;
							gpmSTRnCPY( pU2o+iEmpty+2		, "posXposYposZ", 12 );
							gpmSTRnCPY( pU2o+iEmpty+8		, "ACT:", 4 );
							gpmSTRnCPY( pU2o+iEmpty+16		, "TRG:", 4 );
							gpmSTRnCPY( pU2o+iEmpty+24		, "DIF:", 4 );
							gpmSTRnCPY( pU2o+iEmpty+32+2	, "degXdegYdegZ", 12 );
							gpmSTRnCPY( pU2o+iEmpty+32+8	, "ACT:", 4 );
							gpmSTRnCPY( pU2o+iEmpty+32+16	, "TRG:", 4 );
							gpmSTRnCPY( pU2o+iEmpty+32+24	, "DIF:", 4 );

						}

						if( iCin2 >= 129 )
						if( iCin2 != iCin )
							pANS = gpcGTslmpSTAT( pANS, pU2i+iCin2 );

						iCin2 = iCin;
					}
			}
			continue;
		} else if( iNUM > 7 )
			return pANS->lzyFRMT( s, "nonsens" );

		if( iCin < 129 )
			return pANS->lzyFRMT( s, "Who?" );


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

		pU2o = pU2o ? pU2o : (U2*)(pLZYout=mass.GTlzyALL.LZY(gpdGTlzyIDref(TnID)))->pVALID(pLZYinp);
		if( !pU2o )
			return pANS->lzyFRMT( s, "nonsens" );
		//pI4 = (I4*)pU2o;
		//iCou = iCin+2;
        switch(iNUM)
        {
				// POS
			case 0:
			case 1:
			case 2:
				((I4*)(pU2o+iCin+2+16))[iNUM%3] = (d8 == 0.0) ? (I4)an.num*100 : (I4)(d8*100.0);
				break;
				// DIR
			case 3:
			case 4:
			case 5:
				((I4*)(pU2o+iCin+2+48))[iNUM%3] = (d8 == 0.0) ? (I4)an.num*100 : (I4)(d8*100.0);
				break;
			case 6:
				// SPEED
				spd = (*(U4*)(pU2o+iCin+32))>>24;
				for( U1 i = 0; i < 6; i++ )
				{
					spd <<= 4;
					spd |= (an.num/100000)%10;
					an.num *= 10;
				}
				*(U4*)(pU2o+iCin+32) = spd;
				break;
			case 7:
				// TOOL
				((char*)(pU2o+iCin+32+1))[1] = an.num;
				break;
        }

		iNUM++;
	}
	if( !pU2i )
		return pANS->lzyFRMT( s, "nonsens" );

	if( iCin < nU2 )
		return gpcGTslmpSTAT( pANS, pU2i+iCin );

	return pANS->lzyFRMT( s = -1, "ok" );
}

void gpcGT::GTslmp( gpcGT& mom, gpcWIN* pWIN, gpcGTall* pALL ) {
	U8 nOUT = GTout( pWIN ), s;
	if( nOUT )
		return;

	if( sGTent[2] == 's' && sGTent[0] == 'b' )
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

		nLEN = gpfSTR2U8( gpmMcpy( pL, pD000, 6 )-2, NULL );
		pD000 += 6;
		if( pINP->n_load < 18+nLEN+nSUB )
			return; // még nem jött le az egész

		iCNT++;
		pU2inp = (U2*)(pLZYinp->lzyINS( NULL, nLEN/2, s = 0, nLEN/2 )->p_alloc);
		*pU2inp = gpfSTR2U8( gpmMcpy( pW, pD000, 4 )-2, NULL );
		if( !*pU2inp )
		{
			/// Good!
			for( iLEN = 4; iLEN < nLEN; iLEN+=4 )
				pU2inp[iLEN>>2] = gpfSTR2U8( gpmMcpy( pW, pD000+iLEN, 4 )-2, NULL );
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
					nSOCK = gpmLZYload( pLZYusr, SOCKET );
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

	U4 	iU2 = gpmMcmp( pU2inp, pU2out, nLEN*sizeof(*pU2inp) )/sizeof(*pU2inp),
		i_cpy, n_cpy,
		nOld = pOUT ? pOUT->n_load : 0;

	while( iU2 < nLEN )
	{
		i_cpy = iU2&(~0x7);
		iU2 = i_cpy+8;
		if( iU2 > nLEN )
			iU2 = nLEN;
		else while( iU2 < nLEN )
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
				sGTent[2] = 's';
				pOUT->p_alloc[pOUT->n_load] = sGTent[0] = '\n';
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

		iU2 += gpmMcmp( pU2inp+iU2, pU2out+iU2, (nLEN-iU2)*sizeof(*pU2inp) )/sizeof(*pU2inp);
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
				nSOCK = gpmLZYload( pLZYusr, SOCKET );
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
        nSOCK = gpmLZYload(pLZYusr,sockUSR);
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
		nSOCK = gpmLZYload(pLZYusr,sockUSR);
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
				pU2o = (U2*)(pLZYout->pVALID(pLZYinp,pU2o));
				if( !pU2o )
					pU2o = (U2*)((pLZYout=mass.GTlzyALL.LZY(gpdGTlzyIDref(TnID)))->pVALID(pLZYinp));

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
				pU2o = (U2*)((pLZYout=mass.GTlzyALL.LZY(gpdGTlzyIDref(TnID)))->pVALID(pLZYinp));

			if( pU2o )
				*(U4*)(pU2o+iCou) = com;

		} else
			pSTR += gpmVAN( pSTR, " \t\a\r\n;", nLEN );

		switch( an.alf )
		{
			case gpeALF_FORMAT:
				gpmZnOF( pU2o, nU2 );
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

	if( sGTent[2] == 's' && sGTent[0] == 'b' )
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

		nLEN = gpfSTR2U8( gpmMcpy( pL, pD000, 6 )-2, NULL );
		pD000 += 6;
		if( pINP->n_load < 18+nLEN+nSUB )
			return; // még nem jött le az egész

		iCNT++;
		pU2inp = (U2*)(pLZYinp->lzyINS( NULL, nLEN/2, s = 0, nLEN/2 )->p_alloc);
		*pU2inp = gpfSTR2U8( gpmMcpy( pW, pD000, 4 )-2, NULL );
		if( !*pU2inp )
		{
			/// Good!
			for( iLEN = 4; iLEN < nLEN; iLEN+=4 )
				pU2inp[iLEN>>2] = gpfSTR2U8( gpmMcpy( pW, pD000+iLEN, 4 )-2, NULL );
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
					nSOCK = gpmLZYload( pLZYusr, SOCKET );
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

	U4 	iU2 = gpmMcmp( pU2inp, pU2out, nLEN*sizeof(*pU2inp) )/sizeof(*pU2inp),
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
				sGTent[2] = 's';
				pOUT->p_alloc[pOUT->n_load] = sGTent[0] = '\n';
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

		iU2 += gpmMcmp( pU2inp+iU2, pU2out+iU2, (nLEN-iU2)*sizeof(*pU2inp) )/sizeof(*pU2inp);
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
				nSOCK = gpmLZYload( pLZYusr, SOCKET );
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
			sGTent[0] = '\n';	// visza kapcsol ASCII-ba
			return GTslmpDrc( mom, pWIN, pALL );
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
			gpmMcpy( pU2inp, pD000->pCPY(), nLEN );
			/*for( U4 i = 0; i < nLEN; i+=4 )
			{
				gpmMcpyOF( pW, pD000+i, 4 );
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

	U4 iU2 = gpmMcmp( pU2inp, pU2out, nLEN*sizeof(*pU2inp) )/sizeof(*pU2inp),
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
			gpmMcpy( pSLMP+1, pU2out+i_cpy, n_cpy*sizeof(*pU2out) );
			sGTent[2] = 's';
			sGTent[0] = 'b'; // bin

			if( iU2 >= nLEN )
				break;

		}

		iU2 += gpmMcmp( pU2inp+iU2, pU2out+iU2, (nLEN-iU2)*sizeof(*pU2inp) )/sizeof(*pU2inp);
	}
	if( nOld >= (pOUT ? pOUT->n_load : 0) )
		return;

	nSYNsum = nSYNdo;
	nSYNdo++;

	sGTent[2] = 's';
	sGTent[0] = 'b'; // bin

	pOUT = pOUT->lzyADD( gpaSLMPbin[1].aU1, sizeof(gpaSLMPbin[1]), s = -1 );
	iCNT++;
}
