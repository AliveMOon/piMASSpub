#include "gpcgt.h"
#include "gpcwin.h"
extern U1 gpaALFsub[];
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
							double(*(U4*)(pU2+2+16+0))/mmX(1),
							double(*(U4*)(pU2+2+16+2))/mmX(1),
							double(*(U4*)(pU2+2+16+4))/mmX(1),

							double(*(U4*)(pU2+2+48+0))/degX(1),
							double(*(U4*)(pU2+2+48+2))/degX(1),
							double(*(U4*)(pU2+2+48+4))/degX(1),

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

static gpcROB refROB;
U4x4 gpaROBwr[] = {
	//	write,	nW, 			read,		nR
	{ 	0xA,  	refROB.nWu2,	10000, refROB.nRu2	},
	{ 	0x40A,  refROB.nWu2,	20000, refROB.nRu2	},
};


gpcROB& gpcROB::operator &= ( const gpcDrc& D )
{
	null();
	HS = COM = -1;
	return *this;
}
gpcROB& gpcROB::operator = ( const gpcDrc& D )
{
	null();
	gpmMcpyOF( aXYZ,	&D.oXYZ.x, 3 );
	gpmMcpyOF( aABC,	&D.oABC.A, 3 );
	HS = D.oCTRL.y;
	if( D.bHS1o() )
		COM = D.oCTRL.z;

	msS = D.MPosS;
	return *this;
}

gpcROB::gpcROB( const gpcDrc& D ) {
	gpmCLR;
	*this = D;
}
void gpcGT::GTslmpDrcRob( gpcGT& mom, gpcWIN* pWIN, gpcGTall* pALL ) {
	U8 nOUT = GTout( pWIN ), s;
	if( nOUT )
		return;

	/*if( sGTent[2] == 's' && sGTent[0] == 'b' )
		return GTslmpBINref( mom, pWIN, pALL );*/

	//, nPAD = gpdZSpad, nPADu2 = nPAD/sizeof(U2);
	gpcMASS	*pMASS = pWIN->piMASS;
	gpcGT	*pGTusr = NULL;
	gpcLZY	*pLZYinp = pMASS->GTlzyALL.LZY( gpdGTlzyIDinp(TnID) ),
			*pLZYusr = NULL,
			*pLZYdead = NULL;
	gpcROBnD *pROBnD = gpmLZYvali( gpcROBnD, pLZYinp );

	U2		*pU2io	= NULL, *pU2,
			*pU2inp = pROBnD->pU2inp();

	SOCKET	*pSOCK;
	char sANSW[0x100], *pANSW = sANSW, sCOM[] = "ABCD";

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
			pLZYinp->lzyINS( NULL, sizeof(gpcROBnD), s=0, sizeof(gpcROBnD) );
			pROBnD = gpmLZYvali( gpcROBnD, pLZYinp );
			pROBnD->reset( 0 );

			pU2inp = pROBnD->pU2inp();
		}

		/// ERROR?
		pROBnD->ioSW.aU2[0] = gpfSTR2U8( gpmMcpy(pW,pD0,4)-2, NULL );
		if( pROBnD->iWR() && !pROBnD->ioSW.aU2[0] )
		{
			// olvastunk && nincsen hiba
			/// Good!
			pU2 = pU2io = pROBnD->pROBioU2();
			eD0 = gpaROBwr[pROBnD->iDrc()].a4x2[pROBnD->iWR()].y*4; // 0000 u2ASCii
			if( eD0 > nD0 )
				eD0 = nD0;

			for( 	iD0 = 4; //, pU2 += 10;
					iD0 < eD0; iD0 +=4, pU2++ )
				*pU2 = gpfSTR2U8( gpmMcpy(pW,pD0+iD0,4)-2, NULL );

			if( iD0 > 4 )
			{
				iDRC = pROBnD->iDrc();
				I4x4	befPOS = pROBnD->aDrc[iDRC].iXYZ,
						befABC = pROBnD->aDrc[iDRC].iABC;

				// nem fecsegés, hanem adat
				pROBnD->aDrc[iDRC] = pROBnD->ioROB();
				pROBnD->aDrc[iDRC].msSMR2.w = pWIN->mSEC.x;


				I4	dPOS = (pROBnD->aDrc[iDRC].iXYZ-befPOS).qlen_xyz(),
					dABC = befABC.mmABC( pROBnD->aDrc[iDRC].iABC, degX(180.0/PI), degX(180.0/PI) ).w;
				if( dPOS+(dABC>=(degX(1)/2)) ) {
					pANSW += pROBnD->aDrc[iDRC].answINFOX( pANSW, iDRC, 100 );
					if( sANSW < pANSW ) {
						pSOCK = gpmLZYvali( SOCKET, pLZYusr );
						if( !pSOCK ) {
							pLZYusr = pMASS->GTlzyALL.LZY( gpdGTlzyIDusr(TnID) );
							pSOCK = gpmLZYvali( SOCKET, pLZYusr );
							nSOCK = gpmLZYload( pLZYusr, SOCKET );
						}

						for( U4 iS = 0; iS < nSOCK; iS++ ) {
							pGTusr = pALL->GT( pSOCK[iS] );
							if( pGTusr->bGTdie() )
								continue;
							pGTusr->pOUT = pGTusr->pOUT->lzyFRMT( s = -1,	//"\r\n %d."
																			"%s",
																			//pGTusr->iCNT,
																			sANSW );
							pGTusr->GTback();
						}
					}
				}



				pROBnD->stpPULL();
				if( pROBnD->bPULL() ) // másikat is lehuzzuk
					pOUT = pROBnD->pull( pOUT, gpaROBwr );
			}

		}
		else {
			/// error
			if( pALL ) {
				/// üzenet a felhasználóknak
				pSOCK = gpmLZYvali( SOCKET, pLZYusr );
				if( !pSOCK )
				{
					pLZYusr = pMASS->GTlzyALL.LZY( gpdGTlzyIDusr(TnID) );
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

			switch( pROBnD->ioSW.aU2[0] )
			{
				case 0x4031:
					// nyugta?
					pOUT = pROBnD->pull( pOUT, gpaROBwr );
					break;
				case 0x0000: // good!
					break;
				case 0xc050:
					break;
				case 0xc051:
					pOUT = pROBnD->pull( pOUT, gpaROBwr );
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
		pU2inp = pROBnD->pU2inp();
	}

	if( pROBnD->bWAIT() )
	{
		if( pROBnD ? pROBnD->pc.x > 10 : false )
			pOUT = pROBnD->pull( pOUT, gpaROBwr );
		return;
	}

	iCNT++;
	if( pROBnD->bPULL() ) // lekérdeztünk mindenkit?
	{
		nSYNsum = nSYNdo;
		pOUT = pROBnD->pull( pOUT, gpaROBwr );
		return;
	}


	/// ---------------------------------
	/// Drc judo filter
	/// ---------------------------------
	iD0 = pROBnD->stpPUSH( true );
	I4x4	befPOS = pROBnD->aDrc[iD0].iXYZ,
			befABC = pROBnD->aDrc[iD0].iABC;


	U4x4& JD = pROBnD->aDrc[iD0].JD;
	U4 JDy = JD.y;
	U2	*pB = (U2*)&(
						pROBnD->aROBio[4+iD0]
						= pROBnD->aDrc[iD0]
					), // pB-ben a JUDO elötti
		*pA = (U2*)&(
						pROBnD->aROBio[iD0*2]
						= pROBnD->aDrc[iD0].judo( pROBnD->aROBio[iD0*2+1], pWIN->mSEC.x )
					); // pA-ban azaz új out lesz

	if( JDy == JD.y )
	{

		pOUT = pROBnD->pull( pOUT, gpaROBwr );
		return;
	}


	U4& comA = *(U4*)sCOM, msg = JDy+JD.y*0x10;
	comA = pROBnD->aDrc[iD0].NMnDIF.au4x2[0].x;
	switch( msg )
	{
		case 0x04:
		case 0x10:
		case 0x11:
		case 0x21:
		case 0x33:
		case 0x40:
		case 0x41:
		case 0x43:
		case 0x44:
			pANSW += pROBnD->aDrc[iD0].answINFOX( pANSW, iD0, 100 );
			break;
		default:

			break;
	}
	if( sANSW == pANSW ) {

		I4x4 iXYZ = pROBnD->aDrc[iD0].iXYZ;
		switch( msg )
		{
			case 0x04:
				pANSW += sprintf( pANSW, "%s %0.2x END HS2i off! HS2o off!", sCOM, msg  );
				break;
			case 0x10:
				pANSW += sprintf( pANSW, "%s %0.2x START HS1o on!", sCOM, msg  );
				break;
			case 0x11:
				pANSW += sprintf( pANSW, "%s %0.2x Wait! HS1i on??", sCOM, msg  );
				break;
			case 0x21:
				pANSW += sprintf( pANSW, "%s %0.2x HS1i on! HS1o off!", sCOM, msg  );
				break;
			case 0x33:
				pANSW += sprintf( pANSW, "%s %0.2x Wait! HS2i on?", sCOM, msg  );
				break;
			case 0x40:
			case 0x41:
				pANSW += sprintf( pANSW, "%s %0.2x ON maradt a HS2!", sCOM, msg  );
				break;
			case 0x43:
				pANSW += sprintf( pANSW, 	"%s %0.2x HS2i on!  HS2o on!"
											" Error:%d"
											" POS: %0.2fmm %0.2fmm %0.2fmm ",
											sCOM, msg, //JD.x,
											JD.z,
											double(iXYZ.x)/mmX(1),
											double(iXYZ.y)/mmX(1),
											double(iXYZ.z)/mmX(1)
											 );
				break;
			case 0x44:
				pANSW += sprintf( pANSW, "%s %0.2x Wait! HS2i off?", sCOM, msg  );
				break;

		}
	}

	if( sANSW < pANSW ) {
		pSOCK = gpmLZYvali( SOCKET, pLZYusr );
		if( !pSOCK ) {
			pLZYusr = pMASS->GTlzyALL.LZY( gpdGTlzyIDusr(TnID) );
			pSOCK = gpmLZYvali( SOCKET, pLZYusr );
			nSOCK = gpmLZYload( pLZYusr, SOCKET );
		}

		for( U4 iS = 0; iS < nSOCK; iS++ ) {
			pGTusr = pALL->GT( pSOCK[iS] );
			if( pGTusr->bGTdie() )
				continue;
			pGTusr->pOUT = pGTusr->pOUT->lzyFRMT( s = -1, "\r\n %d.%s", pGTusr->iCNT, sANSW );
			pGTusr->GTback();
		}
	}

    // itt kell majd variálni, hogy elösször maradjanak a control bitek
    // és a következő körben írjuk be
	pROBnD->aROBio[4+iD0] &= pROBnD->aDrc[iD0]; // ez a pB

	U4	iU2 = gpaROBwr[0].x, //iD0].x, mert pB és pA már el van tolva?
		nU2 = gpaROBwr[0].y,
		offU2= gpaROBwr[iD0].x-gpaROBwr[0].x; //iD0].y;
	iU2 += gpmMcmpOF( pB+iU2, pA+iU2, nU2 );
	if( iU2 >= nU2 )
	{
		// nem volt különbség
		pOUT = pROBnD->pull( pOUT, gpaROBwr );
		return;
	}



	// talált különbséget
	// beállítjuk a pROBnD->ioSW,
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
		pOUT = pROBnD->pull( pOUT, gpaROBwr );
		return;
	}

	nSYNsum = nSYNdo;
	nSYNdo++;
	if( pROBnD->ioSW.y&1  )
		return;

	pROBnD->ioSW.y |= 1;
	//sGTent[2] = 's';
	//sGTent[0] = '\n';
	return;
}




///-------------------------------------
///              - REF -
///-------------------------------------

/*gpcLZY* gpcGT::GTslmpOSref( gpcLZY* pANS, U1* pSTR, gpcMASS& mass, SOCKET sockUSR ) {
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
}*/
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
		U1	*pD000 = (U1*)gpmSTRiSTR( (char*)pSTR, "d000" );
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
		if( (U1*)gpmSTRiSTR( (char*)pSTR, "d000" ) == pSTR )
		{
			sGTent[0] = '\n';	// visza kapcsol ASCII-ba
			return GTslmpDrcRob( mom, pWIN, pALL );
		}
		U4 nFND = gpfMM( pSTR, pINP->n_load, (U1*)&gpaSLMPbin[0].SER, sizeof(gpaSLMPbin[0].SER) );
		if( nFND >= pINP->n_load )
		{
			// nincs D000
			// valami zöldségnek kell lenie
			gpmDEL(pINP);
			return;
		}
		gpcSLMP* pD000 = (gpcSLMP*)(pSTR + nFND );
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
