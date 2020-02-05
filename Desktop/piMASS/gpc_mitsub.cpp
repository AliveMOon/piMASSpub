#include "gpcgt.h"
#include "gpcwin.h"
extern U1 gpaALFadd[];
extern char gpsTAB[], *gppTAB;

void gpcGT::GTrealMITSUB( gpcGT& mom, gpcWIN* pWIN, gpcGTall* pALL ) {
	U8 nOUT = GTout( pWIN ), s;
	if( nOUT )
		return;

	if( sGTent[2] == 's' && !sGTent[0] )
		return GTslmpBINref( mom, pWIN, pALL );

	U4 nLEN = 0, nU2;
	gpcMASS& mass = *(pWIN->piMASS);
	gpcGT* pGTusr = NULL;
	gpcLZY	*pLZYinp = mass.GTlzyALL.LZY( gpdGTlzyIDinp(TnID) ),
			*pLZYout = mass.GTlzyALL.LZY( gpdGTlzyIDref(TnID) ),
			*pLZYusr = NULL,
			*pLZYdead = NULL;
	U2		*pU2inp = gpmLZYvaliPD( U2, pLZYinp, 0 ),
			*pU2out = gpmLZYvaliPD( U2, pLZYout, 0 ),
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

	U4 	iU2 = gpmMEMCMP( pU2inp, pU2out, nLEN*sizeof(*pU2inp) )/sizeof(*pU2inp),
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

U1 gpsSLMPabc[] =
				"    "
				"    "
				"    "
				"    "
				"    "
				"    "
				"    "
				"    "

				"  ! " // !
				"  ! "
				"  ! "
				"  ! "
				"  ! "
				"    "
				"  ! "
				"    "

				"! ! " // "
				"! ! "
				"    "
				"    "
				"    "
				"    "
				"    "
				"    "

				"! ! " // #
				"!!!!"
				"! ! "
				"! ! "
				"! ! "
				"!!!!"
				"! ! "
				"    "

				"  ! " // $
				" !!!"
				"! ! "
				" !! "
				"  !!"
				"!!! "
				"  ! "
				"    "

				"    " // %
				"   !"
				"! ! "
				"  ! "
				" !  "
				" ! !"
				"!   "
				"    "

				"    " // &
				" !  "
				"! ! "
				" !  "
				"!  !"
				"! ! "
				" ! !"
				"    "

				"  ! " // (
				" !  "
				" !  "
				" !  "
				" !  "
				" !  "
				"  ! "
				"    "

				" !  " // )
				"  ! "
				"  ! "
				"  ! "
				"  ! "
				"  ! "
				" !  "
				"    "

				"    " // *
				" ! !"
				"  ! "
				" !!!"
				"  ! "
				" ! !"
				"    "
				"    "

				"    " // +
				"  ! "
				"  ! "
				" !!!"
				"  ! "
				"  ! "
				"    "
				"    "

				"    " // ,
				"    "
				"    "
				"    "
				"    "
				"    "
				"  ! "
				" !  "

				"    " // -
				"    "
				"    "
				" !!!"
				"    "
				"    "
				"    "
				"    "

				"    " // .
				"    "
				"    "
				"    "
				"    "
				"    "
				"  ! "
				"    "

				"    " // /
				"   !"
				"  ! "
				"  ! "
				" !  "
				" !  "
				"!   "
				"    "

				" !! " // 0
				"!  !"
				"! !!"
				"! !!"
				"!! !"
				"!! !"
				" !! "
				"    "

				"!!  " // 1
				" !  "
				" !  "
				" !  "
				" !  "
				" !  "
				"!!! "
				"    "

				" !! " // 2
				"!  !"
				"   !"
				"  ! "
				" !  "
				"!   "
				"!!!!"
				"    "

				" !! " // 3
				"!  !"
				"   !"
				" !! "
				"   !"
				"!  !"
				" !! "
				"    "

				" !  " // 4
				" !  "
				"!   "
				"! ! "
				"!!!!"
				"  ! "
				"  ! "
				"    "

				"!!!!" // 5
				"!   "
				"!!! "
				"   !"
				"   !"
				"!  !"
				" !! "
				"    "

				" !! " // 6
				"!  !"
				"!   "
				"!!! "
				"!  !"
				"!  !"
				" !! "
				"    "

				"!!!! " // 7
				"   !"
				"   !"
				"  ! "
				" !  "
				"!   "
				"!   "
				"    "

				" !! " // 8
				"!  !"
				"!  !"
				" !! "
				"!  !"
				"!  !"
				" !! "
				"    "

				" !! " // 9
				"!  !"
				"!  !"
				" !!!"
				"   !"
				"   !"
				" !! "
				"    "

				"    " // :
				"    "
				"    "
				"  ! "
				"    "
				"    "
				"  ! "
				"    "

				"    " // ;
				"    "
				"    "
				"  ! "
				"    "
				"    "
				"  ! "
				" !  "

				"    " // <
				"  ! "
				" !  "
				"!   "
				" !  "
				"  ! "
				"    "
				"    "

				"    " // =
				"    "
				" !! "
				"    "
				" !! "
				"    "
				"    "
				"    "

				"    " // >
				"!   "
				" !  "
				"  ! "
				" !  "
				"!   "
				"    "
				"    "

				" !! " // ?
				"!  !"
				"   !"
				"  ! "
				" !  "
				"    "
				" !  "
				"    "

				" !! " // @
				"!  !"
				"! !!"
				"! !!"
				"!   "
				"!   "
				" !! "
				"    "

				" !  " // A
				"! ! "
				"! ! "
				"!!! "
				"! ! "
				"! ! "
				"! ! "
				"    "

				"!!  " // B
				"! ! "
				"! ! "
				"!!  "
				"! ! "
				"! ! "
				"!!  "
				"    "

				" !  " // C
				"! ! "
				"!   "
				"!   "
				"!   "
				"! ! "
				" !  "
				"    "

				"!!  " // D
				"! ! "
				"! ! "
				"! ! "
				"! ! "
				"! ! "
				"!!  "
				"    "

				"!!! " // E
				"!   "
				"!   "
				"!!  "
				"!   "
				"!   "
				"!!! "
				"    "

				"!!! " // F
				"!   "
				"!   "
				"!!  "
				"!   "
				"!   "
				"!   "
				"    "

				" !  " // G
				"! ! "
				"!   "
				"! ! "
				"! ! "
				"! ! "
				" !! "
				"    "

				"! ! " // H
				"! ! "
				"! ! "
				"!!! "
				"! ! "
				"! ! "
				"! ! "
				"    "

				" !  " // I
				"    "
				"!!  "
				" !  "
				" !  "
				" !  "
				"!!! "
				"    "

				"  ! " // J
				"    "
				"!!! "
				"  ! "
				"  ! "
				"  ! "
				"  ! "
				"!!  "

				"!  !" // K
				"!  !"
				"! ! "
				"!!  "
				"! ! "
				"!  !"
				"!  !"
				"    "

				"!   " // L
				"!   "
				"!   "
				"!   "
				"!   "
				"!   "
				"!!!!"
				"    "

				"!!!!" // M
				"!!! "
				" !  "
				"    "
				"! ! "
				"!!! "
				"!!! "
				"!!! "

				"!  !" // N
				"!! !"
				"!! !"
				"! !!"
				"! !!"
				"!  !"
				"!  !"
				"    "

				" !  " // O
				"! ! "
				"! ! "
				"! ! "
				"! ! "
				"! ! "
				" !  "
				"    "

				"!!  " // P
				"! ! "
				"! ! "
				"! ! "
				"!!  "
				"!   "
				"!   "
				"    "

				" !  " // Q
				"! ! "
				"! ! "
				"! ! "
				"! ! "
				" !! "
				"  ! "
				"    "

				"!!  " // R
				"! ! "
				"! ! "
				"! ! "
				"!!  "
				"! ! "
				"!  !"
				"    "

				" !! " // S
				"!  !"
				"!   "
				" !! "
				"   !"
				"   !"
				"!!! "
				"    "

				"!!! " // T
				" !  "
				" !  "
				" !  "
				" !  "
				" !  "
				" !  "
				"    "

				"! ! " // U
				"! ! "
				"! ! "
				"! ! "
				"! ! "
				"! ! "
				" !! "
				"    "

				"!  !" // V
				"!  !"
				"!  !"
				"!  !"
				"! ! "
				"!!  "
				"!   "
				"    "

				"!!! " // W
				"!!! "
				"!!! "
				"! ! "
				"    "
				" !  "
				"!!! "
				"!!!!"

				"!  !" // X
				"!  !"
				" !! "
				" !  "
				" !! "
				"!  !"
				"!  !"
				"    "

				"!  !" // Y
				"!  !"
				"!  !"
				" ! !"
				"  ! "
				" !  "
				"!   "
				"    "

				"!!!!" // 7
				"   !"
				"  ! "
				" !  "
				"!   "
				"!   "
				"!!!!"
				"    "

				;
gpcZSnDrc& gpcZSnDrc::operator &= ( gpcZSnDrc& out )
{
	if( *this == out )
	{
		if( CTRL.w )
		if( bHS1i() )
		{
			// kint van a jel
			if( !bHS1o() )
			{
				// még olvasse||végrehajt
				// várni kell rá pullingolni
				*this = out;
				out.CTRL.x++;
				CTRL.w = 0;		// pullingolni mintha az robin
								// nem lenne meg a parancs
								// potolni akarja majd
				return *this;
			}

			// megértette||megérkezett
			out.CTRL.w = 0;
			out.rsetHS1i();
			out.rsetHS1o();
		}
		return *this;
	}

	// most jött, vissza ne küldjük
	out.iXYZ = iXYZ;
	out.iABC = iABC;
	out.ixyz = ixyz;
	out.iabc = iabc;
	gpmMEMCPY( out.aiAX1to6, aiAX1to6, sizeof(aiAX1to6) );
	gpmMEMCPY( out.aiax1to6, aiax1to6, sizeof(aiax1to6) );

	// THIS ez a IN most jött ropogos
	// az out amit akarok vagy korábbi állapot
	if( out.bHS2i() )
	{
		// akart valamit és jeleztük, hogy olvastuk
		if( bHS2o() )
		{
			// még olvassa
			// ne küldjük mégegyszer
			out.CTRL.z |= CTRL.z; //setHS2o(); // átveszük, hogy még olvassa
			*this = out; // ha ugyanaz nem küldheti
		} else {
			// már tud róla hogy olvastuk törölhetjük
			out.CTRL.w = 0;	// töröljük a kedvéért
			out.rsetHS2i(); // a miénket
			out.rsetHS2o();	// az övét is
		}
		return *this;
	}
	else if( bHS2o() )
	{
		// na valamit akart a HAVER
		out.CTRL.w = CTRL.w; // elrakjuk
		out.setHS2i(); // jelzem megvan

		rsetHS2i(); // force találat legyen

		// na mit csináljunk az infó szerint
		switch( out.CTRL.w )
		{
			default:
				// ha nem ismerjük semmit
				// majd más foglalkozik vele
				break;
		}
		return *this;
	}


	if( out.CTRL.w )
	if( out.bHS1i() )
	{
		// kint van a jel
		if( !bHS1o() )
		{
			// még olvasse||végrehajt
			// várni kell rá pullingolni
			*this = out;
			CTRL.w = 0;		// pullingolni
			return *this;
		}

		// megértette
		out.CTRL.w = 0;
		out.rsetHS1i();
		out.rsetHS1o();
		return *this;
	}


	I4x4 dir;
	U8 dif;

	oXYZ._yzw(iXYZ);
	oABC._yzw(iABC);
	oxyz._yzw(ixyz);
	oabc._yzw(iabc);
	gpmMEMCPY( out.aiAX1to6, aiAX1to6, sizeof(aiAX1to6) );
	gpmMEMCPY( out.aiax1to6, aiax1to6, sizeof(aiax1to6) );

	dir = (out.oXYZ - oXYZ)._yzw();
	dif = dir.qlen();
    if( dif )
	{
		out.CTRL.w |= 1;

	}
	dir = (out.oABC - oABC)._yzw();
	dif = dir.qlen();
	if( dif )
	{
		out.CTRL.w |= 2;

	}

	dir = (out.oxyz - oxyz)._yzw();
	dif = dir.qlen();
    if( dif )
	{
		out.CTRL.w |= 4;

	}

	dir = (out.oabc - oabc)._yzw();
	dif = dir.qlen();
	if( dif )
	{
		out.CTRL.w |= 8;

	}

	if( out.CTRL.w )
	{
		out.setHS1i();
		return *this;
	}

	return *this;
}
gpcLZY* gpcGT::GTzsndOS( gpcLZY* pANS, U1* pSTR, gpcMASS& mass, SOCKET sockUSR )
{
	U8 s = -1, nLEN;
	U4 n = gpmSTRLEN( pSTR );
	if( this ? !n : true )
		return pANS->lzyFRMT( s = -1, "nonsens" );

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

	U4 nZS = gpmLZYloadPD( pLZYinp, gpcZSnDrc, 0x20 ), iZS = nZS;
	if( !nZS )
		return pANS->lzyFRMT( s = -1, "nonsens" );

	pLZYout = mass.GTlzyALL.LZY(gpdGTlzyIDref(TnID));
	if( !pLZYout )
		return pANS->lzyFRMT( s = -1, "nonsens" );

	gpcZSnDrc	*pZSnDo = (gpcZSnDrc*)gpmLZYvaliPD( U1, pLZYout, 0x20 ),
				*pZSnDi = (gpcZSnDrc*)gpmLZYvaliPD( U1, pLZYinp, 0x20 );
	U1 	sCOM[] = "ABCD",
		*pCOM, *pEND = pSTR+n, *pNUM;
	U4& comA = *(U4*)sCOM, iNUM, nNUM, iE = nZS;
	I8x2 an;
	double d8;

	for( pSTR += gpmNINCS( pSTR, " \t\a\r\n;," ); *pSTR; pSTR += gpmNINCS( pSTR, " \t\a\r\n;," ) )
	{
		an.num = pEND-(pCOM = pSTR);
		an = pCOM;
		pSTR += an.num;
		if( an.alf )
		{
			comA = *(U4*)pCOM;
			iNUM = 0;
			switch( an.alf )
			{
				case gpeALF_FORMAT:
					*(U4*)pLZYout->p_alloc = 0xdeadfeed;
					gpfMEMSET( pLZYout->p_alloc+sizeof(U4), 0x10-sizeof(U4), pLZYout->p_alloc, sizeof(U4) );
					gpmZn( pLZYout->p_alloc+0x10, pLZYout->n_load+0x10 );
					continue;

				case gpeALF_HELO:
				case gpeALF_HELLO:
					return pANS->lzyFRMT( s = -1, "Hello! %d", iZS );
				case gpeALF_HELP:
					return pANS->lzyFRMT( s = -1, "ReadMe.txt %d", iZS );
				case gpeALF_LINE:
					return pANS->lzyFRMT( s = -1, "Line.txt %d", iZS );
				case gpeALF_JOIN:
					return pANS->lzyFRMT( s = -1, "Join.txt %d", iZS );

				case gpeALF_KOSZON:{
						U4	nR = sizeof(gpsSLMPabc),
							nC = nR/(4*8);
						pANS = pANS->lzyFRMT( s = -1, "CSOKOLOM %s BACSI!", pSTR );

					} return pANS;
				case gpeALF_POS:
				case gpeALF_XYZ:{
						switch( comA&0xffffff )
						{
							case gpeZS_XYZ0:
							case gpeZS_POS0:
								iNUM = 0;
								break;
							default:
								iNUM = 12;
								break;
						}
						nNUM = 3;
					} break;
				case gpeALF_DIR:
				case gpeALF_ABC:{
						switch( comA&0xffffff )
						{
							case gpeZS_ABC0:
							case gpeZS_DIR0:
								iNUM = 3;
								break;
							default:
								iNUM = 15;
								break;
						}
						nNUM = 3;
					} break;
				case gpeALF_LINK:
				case gpeALF_AXIS:{
						switch( comA )
						{
							case gpeZS_AXIS:
							case gpeZS_LINK:
								iNUM = 6;
								break;
							default:
								iNUM = 18;
								break;
						}
						nNUM = 6;
					} break;
				default:
					if( an.num >= 4 ) {
						for( iE = nZS, iZS = 1; iZS < nZS; iZS++ )
						{
							if( pZSnDi[iZS].NMnDIF.au4x2[0].x == comA )
								break;
							if( pZSnDi[iZS].NMnDIF.au4x2[0].x )
								continue;
							if( iE < iZS )
								continue;
							iE = iZS;
						}
						if( iZS >= nZS )
						if( iE < nZS )
						{
							// új robot
							iZS = iE;
							pZSnDo[iZS].DnZSfrm( comA );
						}
					} break;
			}
			continue;
		} else if( iNUM > 23 )
			return pANS->lzyFRMT( s = -1, "nonsens" );

		if( iZS >= nZS )
		{
			pANS = pANS->lzyFRMT( s = -1, "Who?" );
			for( iZS = strtZS; iZS < nZS; iZS++ )
			{
				if( !pZSnDi[iZS].NMnDIF.au4x2[0].x )
					continue;
				comA = pZSnDi[iZS].NMnDIF.au4x2[0].x;
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


		switch(iNUM)
        {
				// POS
			case 0:
			case 1:
			case 2:
				pZSnDo[iZS].oXYZ.aXYZW[(iNUM%nNUM)+1] = (d8 == 0.0) ? (I4)an.num*100 : (I4)(d8*100.0);
				break;
				// DIR
			case 3:
			case 4:
			case 5:
				pZSnDo[iZS].oABC.aXYZW[(iNUM%nNUM)+1] = (d8 == 0.0) ? (I4)an.num*100 : (I4)(d8*100.0);
				break;
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
				// AXIS
				pZSnDo[iZS].aoAX1to6[(iNUM%6)/3].aXYZW[(iNUM%nNUM)+1] = (d8 == 0.0) ? (I4)an.num*100 : (I4)(d8*100.0);
				break;

			// OFFSET - eltolás
				// POS
			case 12:
			case 13:
			case 14:
				pZSnDo[iZS].oxyz.aXYZW[(iNUM%nNUM)+1] = (d8 == 0.0) ? (I4)an.num*100 : (I4)(d8*100.0);
				break;
				// DIR
			case 15:
			case 16:
			case 17:
				pZSnDo[iZS].oabc.aXYZW[(iNUM%nNUM)+1] = (d8 == 0.0) ? (I4)an.num*100 : (I4)(d8*100.0);
				break;
			case 18:
			case 19:
			case 20:
			case 21:
			case 22:
			case 23:
				// AXIS
				pZSnDo[iZS].aoax1to6[(iNUM%6)/3].aXYZW[(iNUM%nNUM)+1] = (d8 == 0.0) ? (I4)an.num*100 : (I4)(d8*100.0);
				break;
			default:
				break;
        }

		iNUM++;
	}

	if( iZS < nZS )
		return pZSnDi[iZS].ANSstat( pANS );

	return pANS->lzyFRMT( s = -1, "ok" );
}






