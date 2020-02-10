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
gpcZS& gpcZS::operator = ( const gpcDrc& D )
{
	gpmMcpyOF( &aPOS,	&D.oXYZ, 3 );
	gpmMcpyOF( &aABC,	&D.oABC, 3 );
	gpmMcpyOF( &apos,	&D.oxyz, 3 );
	gpmMcpyOF( &aabc,	&D.oabc, 3 );
	gpmMcpyOF( aJ16,	&D.aoAX1to6[0], 3 );
	gpmMcpyOF( aJ16+3,	&D.aoAX1to6[1], 3 );
	gpmMcpyOF( aj16,	&D.aoax1to6[0], 3 );
	gpmMcpyOF( aj16+3,	&D.aoax1to6[1], 3 );
	return *this;
}
gpcZS::gpcZS( const gpcDrc& D )
{
	gpmCLR;
	*this = D;
}
gpcDrc& gpcDrc::judo( gpcZS& inp )
{
	*this = inp;

	if( bHS1o() )
	{
		// kint van a HS1 jelünk
		if( bHS1i() )
		{
			// megvan ZS jelzet olvasta
            oCTRL.z = oCTRL.w = 0; // törlünk ZS kedvéért
            rstHS1();
			return *this;
        }
        /// PULLING HS1
		setHS1();
		return *this;
	}

	if( bHS2o() )
	{
		// kint van a HS2 jelünk, hogy olvastuk
		if( bHS2i() )
		{
			// megvan ZS jelzet megértette, hogy olvastuk
            oCTRL.z = oCTRL.w = 0; // törlünk ZS kedvéért
            rstHS2();
			return *this;
        }
        /// PULLING HS2
		setHS2();
		return *this;
	}

	if( bHS2i() )
	{
		// ZS hibat jelzett
		switch( iCTRL.z ) // na mi a hiba
		{
			default:
				oCTRL.z = iCTRL.z;
				oCTRL.w = 0;
				break;
		}
		rstHS2();
		return *this;
	}
	if( !NMnDIF.x )
		return *this;

	I4x4 dir = (oXYZ - iXYZ).xyz_();
	I8 dif = dir.qlen(), mm = sqrt(dif/100);
	if( mm < 400 )
	{
		if( mm > 250 )
		{
			iXYZ &= I4x4( 450,450,450, 1 );
			iXYZ /= I4x4( mm,mm,mm, 1 );
		} else {
			// safe space
			iXYZ.x = 350;
			iXYZ.y = (NMnDIF.x == gpeZS_BILL) ? -450 : 450;
			iXYZ.z = 734;
		}
		iXYZ &= I4x4( 100,100,100, 1 );
		dir = (oXYZ - iXYZ).xyz_();
		dif = dir.qlen();
		mm = sqrt(dif/100);
		oxyz &= I4x4( 0,0,0, 1 );
		ixyz.xyz_(oxyz);
	}
    if( mm > 50 )
    {
		dir &= I4x4( 50, 50, 50, 0 );
		dir /= I4x4( mm,mm,mm, 1 );
		dir &= I4x4( 100,100,100, 0 );
		oXYZ.xyz_( dir+iXYZ );
		dir = (oXYZ - iXYZ).xyz_();
		dif = dir.qlen();
		mm = sqrt(dif/100);
		oxyz &= I4x4( 0,0,0, 1 );
		ixyz.xyz_(oxyz);
    }
	if( dif )
	{
		oCTRL.w |= 1;
	}

	dir = (oABC - iABC).xyz_();
	dif = dir.qlen();
	if( dif )
	{
		oCTRL.w |= 2;
	}

	dir = (oxyz - ixyz).xyz_();
	dif = dir.qlen();
	if( dif )
	{
		oCTRL.w |= 4;
	}

	dir = (oabc - iabc).xyz_();
	dif = dir.qlen();
	if( dif )
	{
		oCTRL.w |= 8;
	}

	if( oCTRL.w )
	{
		// itt kell le ellenőrizni mondjuk az ütközésre
		//
		setHS1();
		return *this;
	}

	/// join adatok most egyenlőre nem

	return *this;
}
/*gpcDrc& gpcDrc::out( gpcDrc& D, gpcDrc& ZS )
{
	// az OUT-ban vagyunk
	// addig küld az SLMP amig a D-ben az nem lesz mint
	// az OUT-ban tehát this-ben
	if( bHS1() )
	{
		// OUT-ba el lett küldve
        if( ZS.bHS1() )
        {
			// megvan ZS jelzet olvasta
			CTRL.z = CTRL.w = 0; // törlünk ZS kedvéért


			rstHS1(D);
		} else {
			// még ZS nem olvasta PULLing
			// nem szabad *this = ZSo; tartjuk a D-hez magunkat
			D = *this;
			/// PULLING 1
			setHS1(D);
			return *this;
		}

	}

	// eltároljuk a friss adatokat
	i(D.i(ZS));

	if( bHS2() )
	{
		if( ZS.bHS2() )
		{
			// megértette hogy értettük
			CTRL.z = CTRL.w = 0; // törlünk ZS kedvéért

			rstHS2(D);
			return *this;
		}
		// olvassa olvasse könnyeit hulajtja
		/// PULLING 2
		setHS2(D);
		return *this;
	}
	if( ZS.bHS2() )
	{
		// ZS akar valamit
		D.CTRL.z = CTRL.z = ZS.CTRL.z; // hiba szám

		setHS2(D); // OLVASTUK
		return *this;
	}
	// minden kötelező vége jöhet a free style

	I4x4 dir = (oXYZ - D.oXYZ).xyz_();
	U8 dif = dir.qlen();
	if( dif )
	{
		CTRL.w |= 1;
	}

	dir = (oABC - D.oABC).xyz_();
	dif = dir.qlen();
	if( dif )
	{
		CTRL.w |= 2;
	}

	dir = (oxyz - D.oxyz).xyz_();
	dif = dir.qlen();
	if( dif )
	{
		CTRL.w |= 4;
	}

	dir = (oabc - D.oabc).xyz_();
	dif = dir.qlen();
	if( dif )
	{
		CTRL.w |= 8;
	}

	if( CTRL.w )
	{
		// itt kell leellenőrizni mondjuk az ütközést
		setHS1(D);
		return *this;
	}

	return *this;
}*/
gpcLZY* gpcGT::GTdrcOS( gpcLZY* pANS, U1* pSTR, gpcMASS& mass, SOCKET sockUSR )
{
	U8 s = -1, nLEN;
	U4 n = gpmSTRLEN( pSTR );
	if( this ? !n : true )
		return pANS->lzyFRMT( s = -1, "nonsens" );

	gpcZSnD	*pZSnD = gpmLZYvali( gpcZSnD, mass.GTlzyALL.LZY( gpdGTlzyIDinp(TnID) ) );
	gpcLZY	//*pLZYinp = mass.GTlzyALL.LZY( gpdGTlzyIDinp(TnID) ),
			*pLZYout = NULL,
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
					gpmZn( pZSnD, sizeof(*pZSnD) );
					continue;

				case gpeALF_HELO:
				case gpeALF_HELLO:
					return pANS->lzyFRMT( s = -1, "Hello! %d", iD );
				case gpeALF_HELP:
					return pANS->lzyFRMT( s = -1, "ReadMe.txt %d", iD );
				case gpeALF_LINE:
					return pANS->lzyFRMT( s = -1, "Line.txt %d", iD );
				case gpeALF_JOIN:
					return pANS->lzyFRMT( s = -1, "Join.txt %d", iD );

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
				case gpeALF_BILL:
						iD = 0;
						if( ZSnD.aDrc[iD].NMnDIF.au4x2[0].x == comA )
							break;
						ZSnD.aDrc[iD].DnZSfrm( comA );
						break;
				case gpeALF_JOHN:
						iD = 1;
						if( ZSnD.aDrc[iD].NMnDIF.au4x2[0].x == comA )
							break;
						ZSnD.aDrc[iD].DnZSfrm( comA );
					 break;
				default:
					iNUM = 23;
					break;
			}
			continue;
		} else if( iNUM > 23 )
			return pANS->lzyFRMT( s = -1, "nonsens" );

		if( iD >= 2 )
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

		if( pD )
		if( oD != iD )
		{
			pANS = pD->ANSstat( pANS );
			oD = iD;
		}
		pD = ZSnD.aDrc+iD;


		switch(iNUM)
        {
				// POS
			case 0:
			case 1:
			case 2:
				pD->oXYZ.aXYZW[(iNUM%nNUM)+1] = (d8 == 0.0) ? (I4)an.num*100 : (I4)(d8*100.0);
				break;
				// DIR
			case 3:
			case 4:
			case 5:
				pD->oABC.aXYZW[(iNUM%nNUM)+1] = (d8 == 0.0) ? (I4)an.num*100 : (I4)(d8*100.0);
				break;
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
				// AXIS
				pD->aoAX1to6[(iNUM%6)/3].aXYZW[(iNUM%nNUM)+1] = (d8 == 0.0) ? (I4)an.num*100 : (I4)(d8*100.0);
				break;

			// OFFSET - eltolás
				// POS
			case 12:
			case 13:
			case 14:
				pD->oxyz.aXYZW[(iNUM%nNUM)+1] = (d8 == 0.0) ? (I4)an.num*100 : (I4)(d8*100.0);
				break;
				// DIR
			case 15:
			case 16:
			case 17:
				pD->oabc.aXYZW[(iNUM%nNUM)+1] = (d8 == 0.0) ? (I4)an.num*100 : (I4)(d8*100.0);
				break;
			case 18:
			case 19:
			case 20:
			case 21:
			case 22:
			case 23:
				// AXIS
				pD->aoax1to6[(iNUM%6)/3].aXYZW[(iNUM%nNUM)+1] = (d8 == 0.0) ? (I4)an.num*100 : (I4)(d8*100.0);
				break;
			default:
				break;
        }
		iNUM++;
	}

	if( pD )
		return pD->ANSstat( pANS );

	return pANS->lzyFRMT( s = -1, "nonsens" );
}






