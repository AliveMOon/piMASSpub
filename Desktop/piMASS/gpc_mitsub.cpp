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

U1 gpsSLMPabc[] = {
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
			{
				gpmMcpyOF( &io128.y, &D.oCTRL.y, 3 );
				switch( D.oCTRL.z )
				{
					case 10:
						gpmMcpyOF( &aPOS,	&D.oXYZ, 3 );
						gpmMcpyOF( &aABC,	&D.oABC, 3 );
						break;
					default:

						break;
				}
			} break;
		case 4:
		case 5:
		case 6:
			io128.y = D.oCTRL.y;
			//gpmMcpyOF( &io128.y, &D.oCTRL.y, 3 );
			break;
		default:
			break;
	}
	return *this;
}

gpcZS::gpcZS( const gpcDrc& D )
{
	gpmCLR;
	*this = D;
}


gpcDrc& gpcDrc::operator = ( gpcZS& zs ) {
	gpmMcpyOF( &iXYZ.x, &zs.aPOS, 3 );
	if( iXYZ.qlen_xyz() < 32*32 )
	{
		gpmMcpyOF( &iXYZ.x, &zs.oMxyzEspd, 3 );
		if( iXYZ.qlen_xyz() < 32*32 )
			iXYZ.x = iXYZ.y = iXYZ.z = mm100(450);
	}
	gpmMcpyOF( &iABC.x, &zs.aABC, 3 );
	gpmMcpyOF( &ixyz.x, &zs.apos, 3 );

	if( !ixyz.qlen_xyz() )
	{
		F4x4 mx;
		mx.ABC(iABC, mm100(180)/PI );

		if( txyz.qlen_xyz() )
		{
			ixyz.xyz_( iXYZ + (mx.z*sqrt((txyz-tXYZ).qlen_xyz())));
		} else {
			ixyz.xyz_( iXYZ + (mx.z*mm100(100)) );
		}
	}

	gpmMcpyOF( &iabc.x, &zs.aabc, 3 );
	gpmMcpyOF( &aiAX1to6[0].x, zs.aJ16, 3 );
	gpmMcpyOF( &aiAX1to6[1].x, zs.aJ16+3, 3 );
	gpmMcpyOF( &aiax1to6[0].x, zs.aj16, 3 );
	gpmMcpyOF( &aiax1to6[1].x, zs.aj16+3, 3 );
	gpmMcpyOF( &iCTRL.y, &zs.io128.y, 3 );
	return *this;
}










