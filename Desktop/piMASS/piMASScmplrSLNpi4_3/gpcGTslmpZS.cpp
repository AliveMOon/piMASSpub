#include "gpcgt.h"
#include "gpcwin.h"
extern U1 gpaALFsub[];
extern char gpsTAB[], *gppTAB;
static char gpsJDpubZS[0x100];
U4x4 gpaZSwr[] = {
	//	write,	nW, 		read,		nR
	{ 	0xA,  	gpdZSnWu2,	10000, gpdZSnRu2	},
	{ 	0x40A,  gpdZSnWu2,	20000, gpdZSnRu2	},
};
gpcDrc::gpcDrc( const gpcZS& zs, U4 nm )
{
	if( nm != NMnDIF.au4x2[0].x )
		format( nm );
	*this = zs;
}
gpcDrc::gpcDrc( char* pbuff, I4x4 a, I4x4 b, I4x4 c ) {
	/// DEBUG célból készült nem igazán használható másra
	gpmCLR;
	iXYZ.xyz_(a);
	tXYZ.xyz_(b);

	oXYZ.xyz_( (tXYZ+iXYZ)/2 );

	txyz.xyz_(c);

	I4x4	iR = (iXYZ-txyz).xyz0(),
			tR = (tXYZ-txyz).xyz0(),
			oR = (oXYZ-txyz).xyz0();

	I4	iRr = sqrt(iR.qlen_xyz())/0x100,
		oRr = sqrt(oR.qlen_xyz())/0x100,
		tRr = sqrt(tR.qlen_xyz())/0x100;

	oXYZ.xyz_( (oR*tRr)/oRr + txyz );

	std::cout << "iR   " << iR.pSTR( pbuff ) << "iRr " << iRr << std::endl;
	std::cout << "tR   " << tR.pSTR( pbuff ) << "tRr " << tRr << std::endl;
	std::cout << "oR   " << oR.pSTR( pbuff ) << "oRr " << oRr << std::endl;
	std::cout << "a    " << a.pSTR( pbuff ) << std::endl;
	std::cout << "oXYZ " << oXYZ.pSTR( pbuff ) << std::endl;
	std::cout << "tXYZ " << tXYZ.pSTR( pbuff ) << std::endl;
	std::cout << std::endl;


	iABC.ABC_( F4(  0, 30, 45 )*degX(1) );
	tABC.ABC_( F4( 90, 90, 90 )*degX(1) );

	oABC.ABC_( iABC.mmABC( tABC, degX(180.0/PI), degX(180.0/PI) )/2 + iABC );

	F4x4	iMX, tMX, oMX;
	float ab = 0.5;
	iMX.mxABC(iABC, degX(180.0/PI) );
	oMX.mxABC(oABC, degX(180.0/PI) );
	tMX.mxABC(tABC, degX(180.0/PI) );
	//oMX = iMX.lerp_zyx( tMX, ab );

	std::cout << "iABC   " << (F4(iABC)/degX(1)).pSTR( pbuff ) << std::endl;
	std::cout << "oABC   " << (F4(oABC)/degX(1)).pSTR( pbuff ) << std::endl;
	std::cout << "tABC   " << (F4(tABC)/degX(1)).pSTR( pbuff ) << std::endl;
	std::cout << std::endl;

	std::cout << "iX   " << iMX.x.pSTR( pbuff ) << std::endl;
	std::cout << "iY   " << iMX.y.pSTR( pbuff ) << std::endl;
	std::cout << "iZ   " << iMX.z.pSTR( pbuff ) << std::endl << std::endl;

	std::cout << "oX   " << oMX.x.pSTR( pbuff ) << std::endl;
	std::cout << "oY   " << oMX.y.pSTR( pbuff ) << std::endl;
	std::cout << "oZ   " << oMX.z.pSTR( pbuff ) << std::endl << std::endl;

	std::cout << "tX   " << tMX.x.pSTR( pbuff ) << std::endl;
	std::cout << "tY   " << tMX.y.pSTR( pbuff ) << std::endl;
	std::cout << "tZ   " << tMX.z.pSTR( pbuff ) << std::endl << std::endl;

    iABC.ABC_( iMX.eulABC()*degX(180.0/PI) );
    tABC.ABC_( oMX.eulABC()*degX(180.0/PI) );
	//tABC.ABC_( tMX.eABC()*degX(180.0/PI) );

	std::cout << "iABC   " << (F4(iABC)/degX(1)).pSTR( pbuff ) << std::endl;
	std::cout << "oABC   " << (F4(oABC)/degX(1)).pSTR( pbuff ) << std::endl;
	std::cout << "tABC   " << (F4(tABC)/degX(1)).pSTR( pbuff ) << std::endl << std::endl;

	tMX.mxABC(tABC, degX(180.0/PI) );
	std::cout << "oX   " << oMX.x.pSTR( pbuff ) << std::endl;
	std::cout << "oY   " << oMX.y.pSTR( pbuff ) << std::endl;
	std::cout << "oZ   " << oMX.z.pSTR( pbuff ) << std::endl << std::endl;

	std::cout << "tX   " << tMX.x.pSTR( pbuff ) << std::endl;
	std::cout << "tY   " << tMX.y.pSTR( pbuff ) << std::endl;
	std::cout << "tZ   " << tMX.z.pSTR( pbuff ) << std::endl;
	std::cout << std::endl;
}
gpcLZY* gpcLZY::lzyZSnDstat( U8& iSTRT, gpcZSnD& zs, U1 i ) {
	if( !&zs ) {
		iSTRT = nLD();
		return this;
	}
	gpcLZY* pANS = this;
	U1 n = gpmN(zs.aDrc);
	if( i < n )
	{
		pANS = zs.aDrc[i].answSTAT( pANS, i );
		return this;
	}

	for( U1 i = 0, n = gpmN(zs.aDrc); i < n; i++ )
		pANS = zs.aDrc[i].answSTAT( pANS, i );
	return this;
}
U1 gpcZSnD::iDrc( bool bPULL ) {
	if( bPULL && !(ioSW.y&1) )
		ioSW.y |= 1;
	return (ioSW.y>>1)&1;
}
gpcLZY* gpcZSnD::pulling( gpcLZY* pOUT, U4x4* pZSrw ) {
	U8 s = -1;
	U4	i = this ? iDrc( true ):0,
		n = pZSrw[i].w;
	i = pZSrw[i].z;
	if( this )
		pc.x = 0;
	if( !bWAIT() )
		ioSW.w += 4;
	return pOUT->lzyFRMT( s, gpdSLMP_recv_LN4SL6N4, 24, i, n );
}
gpcDrc& gpcDrc::operator = ( const gpcZS& zs ) {
	gpmMcpyOF( &iXYZ.x, &zs.aPOS, 3 );
	if( iXYZ.qlen_xyz() < 32*32 )
	{
		gpmMcpyOF( &iXYZ.x, &zs.oMxyzEspd, 3 );
		if( iXYZ.qlen_xyz() < 32*32 )
			iXYZ.x = iXYZ.y = iXYZ.z = mmX(450);
	}
	gpmMcpyOF( &iABC.A, &zs.aABC, 3 );

	ixyz.xyz_(iXYZ.ABC2xyz( txyz, iABC ) );

	gpmMcpyOF( &iabc.x, &zs.aabc, 3 );
	gpmMcpyOF( &aiAX1to6[0].x, zs.aJ16, 3 );
	gpmMcpyOF( &aiAX1to6[1].x, zs.aJ16+3, 3 );
	gpmMcpyOF( &aiax1to6[0].x, zs.aj16, 3 );
	gpmMcpyOF( &aiax1to6[1].x, zs.aj16+3, 3 );

	gpmMcpyOF( &iCTRL.y, &zs.io128.y, 3 );

	gpmMcpyOF( &msSMR2.x, &zs.apos, 3 );
	U4 msX = msSRT3.x;
	msSRT3 = msSMR2.xyz0().srt3();
	if( !sMS )
		sMS = msSRT3.x;
	Ems = msSRT3.x-msX;
	if( msX < msSRT3.x )
	{
		nMS++;
		AVGms = (msSRT3.x-sMS)/nMS;

		okXYZ.xyz_(iXYZ);
		okABC.ABC_(iABC);
	}
	return *this;
}
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
						gpmMcpyOF( &aABC,	&D.oABC.A, 3 );
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
		U1	*pD0 = (U1*)gpmSTRiSTR( (char*)pSTR, "d000" );
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
gpcDrc& gpcDrc::judoZS( gpcZS& iZS, U4 mSEC ) {
	*this = iZS;	/// XYZABCxyz 1. ixyz = iXYZ.ABC2xyz( txyz, iABC );
	switch( JD.y ) {
		case 7: {
				JD.y = 0;
				if( JD.z )
				{
					JD.x = 0;
					return *this;
				}

				switch(JD.x)
				{
					case 0:
						JD.x = 1;
						break;
					default:
						JD.x = 1;
						break;
				}
			}
			return *this;
		case 6:	/// 5.HS2 0-ra várunk
			if(bHS2i()) {
				// még nem vette a jelet PULLING
				oHS2o();
				return *this;
			}
			xHS2o();
			JD.y = 7;
			// vette az adást mi is elvesszuk
			//if( JD.z )
			return *this;
			//break;
		case 5: /// 4.HS2 jelre várunk
			if(bHS2i()) {
				// megkaptuk
				// jelzünk hogy olvastuk
				JD.y = 6;
				// kirakjuk a jelet
				oHS2o();

				switch(JD.z = iCTRL.z)
				{
					case 0:
						/// XYZABCxyz 2. OK ------------- JD.z = 0
						okXYZ.xyz_(oXYZ);
						okABC.ABC_(oABC);
						okxyz.xyz_(txyz);
						break;
					case 9115:	// a J5 túl erös akart lenni
						JD.z = 0;
					default:
						// ZS hibat jelzett
						// trg-be eltároljuk hol van most
						// ne is akarjon tovább menni
						/// XYZABCxyz 3. NOK ------------- JD.z = ERR
						if( okXYZ.qlen_xyz() ){
							tXYZ.xyz_( oXYZ.xyz_(okXYZ) );
							tABC.ABC_( oABC.ABC_(okABC) );
							std::cout << "9115tXYZ: " << tXYZ.pSTR( gpsJDpubZS ) <<std::endl;
						} else {
							tXYZ.xyz_( oXYZ.xyz_(iXYZ) );
							tABC.ABC_( oABC.ABC_(iABC) );
							std::cout << "Err tXYZ: " << tXYZ.pSTR( gpsJDpubZS ) <<std::endl;
						}
						break;
				}
				return *this;
			}
			// még nemjött
			/// PULLING
			xHS1o();
			return *this;
		case 4:
			if(bHS1i()) {
				/// PULLING HS1 : még nem vette el a jelet
				xHS1o();
				return *this;
			}
			JD.y = 5;
			return *this;
		case 9: /// 3.HS1 ZS jelére várunk
			if(bHS1i()) {
				// 3.HS1 megvan ZS jelzet olvasta
				JD.y = 4;
				// elveszük a jelet
				xHS1o();
				xHS2o();
				return *this;
			}
			oHS1o();
			if( bHS2i() )
				oHS2o();
			else
				xHS2o();

			JD.y = 3;
			return *this;
		case 3: /// 3.HS1 ZS jelére várunk
			if(bHS1i()) {
				// 3.HS1 megvan ZS jelzet olvasta
				JD.y = 4;
				// elveszük a jelet
				xHS1o();
				xHS2o();
				return *this;
			}
		case 2:
			/// PULLING HS1 : kint marat nem volt INIT
			oHS1o();
			if( bHS2i() )
				oHS2o();
			else
				xHS2o();

			JD.y = JD.y == 3 ? 9 : 3;
			return *this;
		case 1: /// 1.HS1 apa kezdődik
			//xHS3o();
			oHS1o();
			if( bHS2i() )
				oHS2o();
			JD.y = bHS1i() ? 2 : 3;
			return *this;
		default:
			break;
	}


	if( !NMnDIF.x )
		return *this;

	I8x4 mmABCD( sqrt(iXYZ.qlen_xyz()) );
	if( mmABCD.x < mmX(250) )
		return *this;		// nem fut a R2D task


	if( !JD.x )
	{
		oCTRL.w = 0;
		oCTRL.z = 1;
		JD.y = 0;
		tXYZ.xyz_( oXYZ.xyz_(iXYZ) );
		tABC.ABC_( oABC.ABC_(iABC) );
		txyz.xyz_( 0 ); //oxyz.xyz_(ixyz) );
		JD.x = 1;
		return *this;
	}
	/// CTRL.z = 0;
	oCTRL.z = 0;
	if( sqrt(okXYZ.qlen_xyz()) > mmX(200) ){
		// ezt kaptuk a robitol
		// ha netán akkor sem mozdul
		oXYZ.xyz_(iXYZ);
		oABC.ABC_(iABC);
	}

	F4x4 tMX = 1.0, iMX;
	iMX.mxABC(iABC, degX(180.0/PI) );
	// itt kell le ellenőrizni mondjuk az ütközésre
	//
	float ab = 1.0, k;
	static const float K = (100.0*PI2);

	I4 lim = 0;
	I4x4 	itD = iABC.mmABC( tABC, degX(180.0/PI), degX(180.0/PI) ),
			tmp,
			dXYZ = tXYZ - iXYZ, lXYZ,
			dGRP = tGRP - iGRP;

	//nD.z
	mmABCD.w = sqrt(dGRP.qlen_xyz());

	mmABCD.x = dXYZ.abs0().mx().x;
	if( mmABCD.x ) {

		U4 i = 3;
		switch( NMnDIF.x )
		{
			case gpeZS_BILL:
				i = 0;
				break;
			case gpeZS_JOHN:
				i = 1;
				break;
			default:
				i = 3;
				break;
		}

		// egyenlőre felfele kerekítjük,
		// azaz ha nem tudja legalább 2-re felosztani a távot
		// megprobálja egyszerre
		switch( jdALF )
		{
			case gpeALF_DROP:
				lim = mmABCD.x;
				break;
			default:
				lim = ((mmABCD.x/mmX(gpdROBlim))<2) ? mmABCD.x : mmX(gpdROBlim);
				break;
		}

		oXYZ.xyz_( cageXYZ( lim, i ) );

		lXYZ = oXYZ - iXYZ;
		mmABCD.y = lXYZ.abs0().mx().x;
		if( mmABCD.x < 50 )
		{
			mmABCD.y = mmABCD.x = 0;
		}
		else if( mmABCD.y ) {
			//mmABCD.y = sqrt(lXYZ.qlen_xyz());
			if( lim <= sqrt(lXYZ.qlen_xyz())+mmX(3) )
			{
				// elérte a lim-et azaz nem érte el a ketrecet
				lim = mmABCD.y;
				if( false )
				if( lim < mmABCD.x )
				{
					if( txyz.qlen_xyz() )
					{
						// de nem érte el az tXYZ-t
						// azaz felosztotta a pályát
						I4x4 	iR = (iXYZ-txyz).xyz0(),
								oR = (oXYZ-txyz).xyz0(),
								tR = (tXYZ-txyz).xyz0();
						I8	iRr = sqrt(iR.qlen_xyz()),
							oRr = sqrt(oR.qlen_xyz()),
							tRr = sqrt(tR.qlen_xyz()),
							lRr = iRr + (((tRr-iRr)*(I8)mmABCD.y)/(I8)mmABCD.x);


						if( lRr )
						{
							I4x4	trg = (oR*lRr)/oRr + txyz,
									tmp = cageXYZ( trg, 0, i );
							if( trg.xyz0() == tmp.xyz0() )
							{
								oXYZ.xyz_( tmp );
								dXYZ = oXYZ - iXYZ;
								//lim = mmABCD.y = sqrt(dXYZ.qlen_xyz());
							}
						}
					}
				}
			}
			else if( mmABCD.y > mmX(10) )
			{
				// kisebb let mint amit elvártunk a ketrec nem engedte
				lXYZ *= (I4)(mmABCD.y/mmX(1))-10;
				lXYZ /= (I4)(mmABCD.y/mmX(1));
				oXYZ.xyz_(lXYZ+iXYZ);
				//mmABCD.y = sqrt(lXYZ.qlen_xyz());
				/// na majd pattanjon le
			} else {
				// nagyon kicsit engedne
				mmABCD.y = 0;
			}
		}

		if( mmABCD.y ) {
			oCTRL.z |= 1;
			ab = float(mmABCD.y)/float(mmABCD.x);
		} else {
			oXYZ.xyz_(iXYZ);
			dXYZ.null();
			ab = 0.0;
		}
	}

	if( txyz.qlen_xyz() ) {
		I4x4 dxyz = txyz - okxyz;

		mmABCD.z = dxyz.abs0().mx().x;
		if( mmABCD.y|(mmABCD.z>10) )
		{
			tMX = 1.0;
			tMX.z.xyz_( txyz-tXYZ );
			tMX.z /= sqrt(tMX.z.qlen_xyz());	// normalizál
			tMX.x = iMX.y.X3(tMX.z);
			tMX.y = tMX.z.X3(iMX.x);
			float	xl = tMX.x.qlen_xyz(),
					yl = tMX.y.qlen_xyz();
			if( xl > yl )
			{
				tMX.x /= sqrt(xl);
				tMX.y = tMX.z.X3(tMX.x);
			} else {
				tMX.y /= sqrt(yl);
				tMX.x = tMX.y.X3(tMX.z);
			}
			tABC.ABC_( tMX.eulABC()*degX(180.0/PI) );
			itD = iABC.mmABC( tABC, degX(180.0/PI), degX(180.0/PI) );
		}
	}

	if( itD.w < (degX(1)/2) ) {	// kb: 50
		tABC.ABC_(oABC.ABC_(iABC));
	} else {

		k = ((K*float(itD.w))/degX(180.0));
		if( k > 0.0 ) {
			float lpk = gpdROBlim/k;
			if( ab == 0.0 )
			{
				ab = lpk;
				lim = mmABCD.y;
			}
			else if( ab > lpk )
			{
				ab = lpk;
				I8 l = (mmABCD.y*gpdROBlim)/k;
				oXYZ.xyz_( iXYZ.lim_xyz(oXYZ,l) );
				if( lim == mmABCD.y )
					lim = mmABCD.y = l; // nem a CAGE fogta meg hanem csak nem tekerhet gyorsabban
			}


			if( ab < 1.0 ) {
				if( mmABCD.x >= degX(2) ) {

					mmABCD.x /= degX(1);
					if( lim <= mmABCD.y )
					{
						lim = (mmABCD.y/=degX(1));
					} else {
						mmABCD.y /= degX(1);
						lim = mmABCD.y+3;
					}
				}

				if( mmABCD.y )
					oABC.ABC_( ((itD*mmABCD.y)/mmABCD.x)+iABC );
				else if( ab > 0.0 )
					oABC.ABC_( (F4(itD)*ab)+iABC );


				if( (oABC.A/degX(180)) > 1 ) {
					I4x2 rq = oABC.A;
					rq.XdivRQ( degX(180) );
					if( rq.y&1 )
					{
						oABC.A = rq.x-degX(180);
					}
				}
				else if( (oABC.A/degX(180)) < -1 ) {
					I4x2 rq = -oABC.A;
					rq.XdivRQ( degX(180) );
					if( rq.y&1 )
					{
						oABC.A = (rq.x-degX(180))*-1;
					}
				}

				if( (oABC.B/degX(180)) > 1 ) {
					I4x2 rq = oABC.B;
					rq.XdivRQ( degX(180) );
					if( rq.y&1 )
					{
						oABC.B = rq.x-degX(180);
					}
				}
				else if( (oABC.B/degX(180)) < -1 ) {
					I4x2 rq = -oABC.B;
					rq.XdivRQ( degX(180) );
					if( rq.y&1 )
					{
						oABC.B = (rq.x-degX(180))*-1;
					}
				}

				if( (oABC.C/degX(180)) > 1 ) {
					I4x2 rq = oABC.C;
					rq.XdivRQ( degX(180) );
					if( rq.y&1 )
					{
						oABC.C = rq.x-degX(180);
					}
				}
				else if( (oABC.C/degX(180)) < -1 ) {
					I4x2 rq = -oABC.C;
					rq.XdivRQ( degX(180) );
					if( rq.y&1 )
					{
						oABC.C = (rq.x-degX(180))*-1;
					}
				}
			} else {
				oABC.ABC_( tABC );
			}
		}
		oCTRL.z |= 2;
	}

	if( jdPRGstp( mSEC ) )
		return *this;


	if( lim > mmABCD.y )
	{
		// ketrec gátolta
        // o-kat berakjuk a t-be
        tXYZ.xyz_( oXYZ );
		tABC.ABC_( oABC );
		std::cout << "CAGE tXYZ: " << tXYZ.pSTR( gpsJDpubZS ) <<std::endl;
	} else
		std::cout << "oXYZ: " << oXYZ.pSTR( gpsJDpubZS ) << "\r\ntXYZ: " << tXYZ.pSTR( gpsJDpubZS ) <<std::endl;

	JD.y = 0;
	switch( JD.x )
	{
		case 0: // ALAPHelyzet kell
			oCTRL.w = 0;
			oCTRL.z = 1;
			JD.y = 0;
			tXYZ.xyz_( oXYZ.xyz_(iXYZ) );
			tABC.ABC_( oABC.ABC_(iABC) );
			JD.x = 1;
			break;
		case 1: // ALAPH ready!
			oCTRL.w = 0;
			oCTRL.z = 11; // linearis XYZ ABC
			JD.y = 1;
			break;
	}
	/// 1.HS1 elöbb ki X-eljük a HS1-et, nehogy ZS megszaladjon
	if( JD.y )
	{
		xHS1o();
		if( bHS2i() )
			oHS2o();
		else
			xHS2o();
	}
	return *this;

}
gpcLZY* gpcGT::GTdrcOSzs( gpcLZY* pANS, U1* pSTR, gpcMASS& mass, SOCKET sockUSR, U4 mSEC )
{
	U8 s = -1, nLEN;
	U4 n = gpmSTRLEN( pSTR );
	if( this ? !n : true )
		return pANS->lzyFRMT( s = -1, "nonsens" );

	gpcZSnD	*pZSnD = gpmLZYvali( gpcZSnD, mass.GTlzyALL.LZY( gpdGTlzyIDinp(TnID) ) );
	gpcLZY	*pLZYout = NULL,
			*pLZYusr = mass.GTlzyALL.LZY( gpdGTlzyIDusr(TnID) );
	///-----------------------------
	/// UJ felhasználó?
	///-----------------------------
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



	//pLZYout = mass.GTlzyALL.LZY(gpdGTlzyIDref(TnID));
	//if( !pLZYout )
	//	return pANS->lzyFRMT( s = -1, "nonsens" );


	if( !pZSnD )
		return pANS->lzyFRMT( s = -1, "nonsens" );

	gpcZSnD& ZSnD =	*pZSnD;
	U4	nD = 2,
		iD = nD, oD = 2;
	gpcDrc* pD = NULL;
	/*gpcDrc	*pDo = (gpcDrc*)gpmLZYvaliPD( U1, pLZYout, gpdZSio128 ),
			*pDi = (gpcDrc*)gpmLZYvaliPD( U1, pLZYinp, gpdZSio128 );*/
	U1 	sCOM[] = "ABCD",
		*pCOM, *pEND = pSTR+n, *pNUM;
	U4& comA = *(U4*)sCOM, iNUM, nNUM, iE = 2;
	I8x2 an;
	double d8;
	gpeALF alf = gpeALF_null;
	for(
			 pSTR += gpmNINCS( pSTR, " \t\a\r\n;," );
			*pSTR;
			 pSTR += gpmNINCS( pSTR, " \t\a\r\n;," )
		)
	{
		an.num = pEND-(pCOM = pSTR);
		an = pCOM;
		pSTR += an.num;
		if( an.alf ) {
			comA = *(U4*)pCOM;
			iNUM = gpeDRCos_NONS;
			switch( an.alf )
			{
				case gpeALF_BILL:
						iD = 0;

						if( ZSnD.aDrc[iD].NMnDIF.au4x2[0].x == gpeZS_BILL )
							break;
						ZSnD.aDrc[iD].format( gpeZS_BILL );
						break;
				case gpeALF_JOHN:
						iD = 1;
						if( ZSnD.aDrc[iD].NMnDIF.au4x2[0].x == gpeZS_JOHN )
							break;
						ZSnD.aDrc[iD].format( gpeZS_JOHN );
					 break;
				case gpeALF_OK:{
						if(iD >= nD )
						{
							iNUM = gpeDRCos_NONS;
							break;
						}
						ZSnD.aDrc[iD].okXYZ.xyz_( ZSnD.aDrc[iD].tXYZ );
						ZSnD.aDrc[iD].okABC.ABC_( ZSnD.aDrc[iD].tABC );
						ZSnD.aDrc[iD].okxyz.xyz_( ZSnD.aDrc[iD].txyz );
					} break;
				case gpeALF_POS:
				case gpeALF_XYZ:{
						if(iD >= nD )
						{
							iNUM = gpeDRCos_NONS;
							break;
						}

						switch( comA&0xffffff )
						{
							case gpeZS_XYZ0:
							case gpeZS_POS0:
								iNUM = gpeDRCos_POSx;
								if( ZSnD.aDrc[iD].okXYZ.qlen_xyz() )
									ZSnD.aDrc[iD].tXYZ.xyz_( ZSnD.aDrc[iD].okXYZ );
								else
									ZSnD.aDrc[iD].tXYZ.xyz_( ZSnD.aDrc[iD].iXYZ );
								break;
							default:
								iNUM = gpeDRCos_posx;
								ZSnD.aDrc[iD].txyz.xyz_( ZSnD.aDrc[iD].ixyz );
								break;
						}
						nNUM = 3;
					} break;
				case gpeALF_DIR:
				case gpeALF_ABC:{
						if(iD >= nD )
						{
							iNUM = gpeDRCos_NONS;
							break;
						}

						switch( comA&0xffffff )
						{
							case gpeZS_ABC0:
							case gpeZS_DIR0:
								iNUM = gpeDRCos_ABCa;
								if( ZSnD.aDrc[iD].okXYZ.qlen_xyz() )
									ZSnD.aDrc[iD].tABC.ABC_( ZSnD.aDrc[iD].okABC );
								else
									ZSnD.aDrc[iD].tabc.xyz_( ZSnD.aDrc[iD].iABC );
								break;
							default:
								iNUM = gpeDRCos_abcA;
								break;
						}
						nNUM = 3;
					} break;

				case gpeALF_GRIP:{

						if(iD >= nD )
						{
							iNUM = gpeDRCos_NONS;
							break;
						}
						iNUM = gpeDRCos_GRPx;
						nNUM = 3;
					} break;

				case gpeALF_DROP: {
						if(iD >= nD )
						{
							iNUM = gpeDRCos_NONS;
							break;
						}
						alf = an.alf;
						iNUM = gpeDRCos_drpX;
						nNUM = 4;
					} break;

				case gpeALF_SHLD:
				case gpeALF_SNAIL: {
						if(iD >= nD )
						{
							iNUM = gpeDRCos_NONS;
							break;
						}
						alf = an.alf;
						iNUM = gpeDRCos_prgA;
						nNUM = 4;
					} break;


				case gpeALF_LINK:
				case gpeALF_AXIS:{

						if(iD >= nD )
						{
							iNUM = gpeDRCos_NONS;
							break;
						}

						switch( comA )
						{
							case gpeZS_AXIS:
							case gpeZS_LINK:
								iNUM = gpeDRCos_L1;
								break;
							default:
								iNUM = gpeDRCos_l1;
								break;
						}
						nNUM = 6;
					} break;
				case gpeALF_STAT:
					if(iD >= nD )
					{
						iNUM = gpeDRCos_NONS;
						break;
					}

					pANS = ZSnD.aDrc[iD].answSTAT( pANS, iD );
					continue;
				case gpeALF_STOP:
				default:
					break;
			}

			if( oD != iD )
			if( pD )
			{
				//pD->cageXYZ( mmX(gpdROBlim), iD );
				pANS = pD->answSTAT( pANS, iD );
			}
			pD = NULL;
			oD = iD;
			continue;
		} else if( iNUM > gpeDRCos_NONS )
			return pANS->lzyFRMT( s = -1, "nonsens" );

		if( iD >= nD )
		{
			pANS = pANS->lzyFRMT( s = -1, "Who?" );
			for( iD = 0; iD < 2; iD++ )
			{
				if( !ZSnD.aDrc[iD].NMnDIF.au4x2[0].x )
					continue;
				comA = ZSnD.aDrc[iD].NMnDIF.au4x2[0].x;
				pANS = pANS->lzyFRMT( s = -1, "\r\n%s", sCOM );
			}
			return pANS;
		}



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

		pD = ZSnD.aDrc+iD;
		switch(iNUM)
        {

				// POS
			case gpeDRCos_POSx:
			case gpeDRCos_POSy:
			case gpeDRCos_POSz:
				pD->tXYZ.aXYZW[(iNUM-gpeDRCos_POSx)%nNUM] = (d8 == 0.0) ? (I4)an.num*mmX(1) : (I4)(d8*mmX(1));
				break;
				// pos
			case gpeDRCos_posx:
			case gpeDRCos_posy:
			case gpeDRCos_posz:
				pD->txyz.aXYZW[(iNUM-gpeDRCos_posx)%nNUM] = (d8 == 0.0) ? (I4)an.num*mmX(1) : (I4)(d8*mmX(1));
				break;


				// DIR
			case gpeDRCos_ABCa:
			case gpeDRCos_ABCb:
			case gpeDRCos_ABCc:
				pD->okxyz.xyz_(pD->txyz.xyz_(0));
				pD->tABC.aXYZW[(iNUM-gpeDRCos_ABCa)%nNUM] = (d8 == 0.0) ? (I4)an.num*degX(1) : (I4)(d8*degX(1));
				break;

				// GRIP
			case gpeDRCos_GRPx:
			case gpeDRCos_GRPy:
			case gpeDRCos_GRPz:
				pD->tGRP.aXYZW[(iNUM-gpeDRCos_GRPx)%nNUM] = (d8 == 0.0) ? (I4)an.num*mmX(1) : (I4)(d8*mmX(1));
				break;
				// PRG
			case gpeDRCos_prgA:
			case gpeDRCos_prgB:
			case gpeDRCos_prgC:
			case gpeDRCos_prgD:
				pD->jd0PRG.aXYZW[(iNUM-gpeDRCos_prgA)%nNUM] = (d8 == 0.0) ? (I4)an.num : (I4)d8;
				if(iNUM!=gpeDRCos_prgD)
					break;
				pD->jdALF = alf;
				pD->jdPRG = I4x4( 1, 0, 1 );
				pD->jdPRGstp( mSEC );
				alf = gpeALF_null;
				break;
			case gpeDRCos_drpX:
			case gpeDRCos_drpY:
			case gpeDRCos_drpZ:
			case gpeDRCos_drpUP:
			case gpeDRCos_drpT:
			case gpeDRCos_drpW:
				if(iNUM<gpeDRCos_drpT)
				{
					pD->jd1XYZ.aXYZW[(iNUM-gpeDRCos_prgA)%nNUM] = (d8 == 0.0) ? (I4)an.num*mmX(1) : (I4)(d8*mmX(1));
					break;
				} else {
					pD->jd0PRG.aXYZW[(iNUM-gpeDRCos_prgA)%nNUM] = (d8 == 0.0) ? (I4)an.num*ms2sec : (I4)(d8*ms2sec);
					if(iNUM<gpeDRCos_drpW)
						break;
				}
				pD->jdALF = alf;
				pD->jdPRG = I4x4( 1, 0, 1 );
				pD->jdPRGstp( mSEC );
				alf = gpeALF_null;
				break;
			/// -----------------------------------------------


			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
				// AXIS
				pD->aoAX1to6[((iNUM-6)%6)/3].aXYZW[(iNUM-6)%nNUM] = (d8 == 0.0) ? (I4)an.num*degX(1) : (I4)(d8*degX(1));
				break;

				// OFFSET - eltolás

				// dir
			case gpeDRCos_abcA:
			case gpeDRCos_abcB:
			case gpeDRCos_abcC:
				pD->oabc.aXYZW[(iNUM-gpeDRCos_abcA)%nNUM] = (d8 == 0.0) ? (I4)an.num*degX(1) : (I4)(d8*degX(1));
				break;
			case 18:
			case 19:
			case 20:
			case 21:
			case 22:
			case 23:
				// AXIS
				pD->aoax1to6[((iNUM-18)%6)/3].aXYZW[(iNUM-18)%nNUM] = (d8 == 0.0) ? (I4)an.num*degX(1) : (I4)(d8*degX(1));
				break;


			default:
				break;
        }
		iNUM++;
	}
	if( pD )
		return pD->answSTAT( pANS, iD );

	return pANS->lzyFRMT( s = -1, "nonsens" );
}

