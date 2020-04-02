#include "gpcgt.h"
#include "gpcwin.h"
extern U1 gpaALFadd[];
extern char gpsTAB[], *gppTAB;

I4x4 gpaCAGEbillBALL[] = {
	{ mm100(1500), 0, mm100(320), mm100(800) },
	{ 0, 0, mm100(320), mm100(420) }, { 0, 0, mm100(-300), mm100(550) },
	{ mm100(685), mm100(-469), mm100(366),  mm100(300) },
};
I4x4 gpaCAGEbillBOX[] = {
	{ mm100(1500/2), mm100(4900), mm100(0), mm100(4000) }, // MIMI2bill
	{ mm100(-1000-2000), mm100(0), mm100(0), mm100(2000) }, // fal_bill
	{ mm100(600), mm100(600), mm100(-200), mm100(330) }, // asztal_bill
};
U4	gpnCAGEbillBALL = gpmN(gpaCAGEbillBALL),
	gpnCAGEbillBOX = gpmN(gpaCAGEbillBOX);

I4x4 gpaCAGEjohnBALL[] = {
	{ 0, 0, mm100(320), mm100(420) }, { 0, 0, mm100(-300), mm100(550) },
	{ mm100(1500), 0, mm100(320), mm100(800) },
	{ mm100(685), mm100(-469), mm100(366),  mm100(300) },
};
I4x4 gpaCAGEjohnBOX[] = {
	{ mm100(1500-600), mm100(-600), mm100(-200), mm100(330) }, // asztal_john
	{ mm100(-500-2000), mm100(0), mm100(0), mm100(2000) }, // MIMI_john
	{ mm100(1500/2), mm100(-4900), mm100(0), mm100(4000) }, // MIMI2john
};
U4	gpnCAGEjohnBALL = gpmN(gpaCAGEjohnBALL),
	gpnCAGEjohnBOX = gpmN(gpaCAGEjohnBOX);

gpcLZY* gpcDrc::answSTAT( gpcLZY* pANS ) {
		U1 	sCOM[] = "ABCD";
		U4 &comA = *(U4*)sCOM;

		comA = NMnDIF.x;
		U8 s;
		pANS = pANS->lzyFRMT( s = -1,	"\r\n// %s HS12:%0.4X", *sCOM ? (char*)sCOM : "?", hs12() );
		pANS = pANS->lzyFRMT( s = -1,	"\r\n// POS:  n:%d"
										"\r\n//\tiXYZ %7.2fmm, %7.2fmm, %7.2fmm "
										"\r\n//\toXYZ %7.2fmm, %7.2fmm, %7.2fmm "
										"\r\n//\ttXYZ %7.2fmm, %7.2fmm, %7.2fmm;"
										,
										(U4)(sqrt((tXYZ-iXYZ).xyz0().qlen())/mm100(100)),
										double(iXYZ.x)/100.0,
										double(iXYZ.y)/100.0,
										double(iXYZ.z)/100.0,

										double(oXYZ.x)/100.0,
										double(oXYZ.y)/100.0,
										double(oXYZ.z)/100.0,

										double(tXYZ.x)/100.0,
										double(tXYZ.y)/100.0,
										double(tXYZ.z)/100.0
							);
		pANS = pANS->lzyFRMT( s = -1,	"\r\n// DIR:  n:%d"
										"\r\n//\tiABC %7.2fdg, %7.2fdg, %7.2fdg "
										"\r\n//\toABC %7.2fdg, %7.2fdg, %7.2fdg "
										"\r\n//\ttABC %7.2fdg, %7.2fdg, %7.2fdg;"
										,
										(U4)(sqrt((tABC-iABC).xyz0().qlen())/mm100(100)),
										double(iABC.x)/100.0,
										double(iABC.y)/100.0,
										double(iABC.z)/100.0,

										double(oABC.x)/100.0,
										double(oABC.y)/100.0,
										double(oABC.z)/100.0,

										double(tABC.x)/100.0,
										double(tABC.y)/100.0,
										double(tABC.z)/100.0
							);
		pANS = pANS->lzyFRMT( s = -1,	"\r\n// pos:"
										"\r\n//\tixyz %7.2fmm, %7.2fmm, %7.2fmm "
										"\r\n//\toxyz %7.2fmm, %7.2fmm, %7.2fmm "
										"\r\n//\ttxyz %7.2fmm, %7.2fmm, %7.2fmm;"
										,
										double(ixyz.x)/100.0,
										double(ixyz.y)/100.0,
										double(ixyz.z)/100.0,

										double(oxyz.x)/100.0,
										double(oxyz.y)/100.0,
										double(oxyz.z)/100.0,

										double(txyz.x)/100.0,
										double(txyz.y)/100.0,
										double(txyz.z)/100.0
							);
		pANS = pANS->lzyFRMT( s = -1,	"\r\n// dir:"
										"\r\n//\tiabc %7.2fdg, %7.2fdg, %7.2fdg "
										"\r\n//\toabc %7.2fdg, %7.2fdg, %7.2fdg "
										"\r\n//\ttabc %7.2fdg, %7.2fdg, %7.2fdg;"
										,
										double(iabc.x)/100.0,
										double(iabc.y)/100.0,
										double(iabc.z)/100.0,

										double(oabc.x)/100.0,
										double(oabc.y)/100.0,
										double(oabc.z)/100.0,

										double(tabc.x)/100.0,
										double(tabc.y)/100.0,
										double(tabc.z)/100.0
							);
		return pANS;
	}

gpcLZY* gpcLZY::lzyZSnD( U8& iSTRT, gpcZSnD& zs, U1 i )
{
	if( !&zs ) {
		iSTRT = nLD();
		return this;
	}
	gpcLZY* pANS = this;
	U1 n = gpmN(zs.aDrc);
	if( i < n )
	{
		pANS = zs.aDrc[i].answSTAT( pANS );
		return this;
	}

	for( U1 i = 0, n = gpmN(zs.aDrc); i < n; i++ )
		pANS = zs.aDrc[i].answSTAT( pANS );
	return this;
}

I4x4 gpcDrc::cageBALL( I4x4 T, I4x4* pCAGE, U4 n ) {
	I4x4 S = iXYZ.xyz0(), a, b;
	I8 dd0 = (T-S).qlen_xyz(), dd = dd0, d = sqrt(dd), abba, ab;
	for( U4 i = 0; i < n; i++ )
	{
		a = (S-pCAGE[i]).xyz0();
		// +mm100(100)-a magának TOOL nak is adunk vele egy sugarat
        b = a.TSrBALL( T-pCAGE[i], pCAGE[i].w+mm100(100) );
        abba = (b-a).qlen_xyz();
        if( dd <= abba )
			continue;
		dd = abba;
	}
	if( dd >= dd0 )
		return T;

	I8x4 D8 = T-S;
	D8 *= sqrt(dd)-mm100(1);
	D8 /= sqrt(dd0);
	return S+D8;
}
I4x4 gpcDrc::cageBOX( I4x4 T, I4x4* pCAGE, U4 n ) {
	I4x4 S = iXYZ.xyz0(), a, b;
	I8 dd0 = (T-S).qlen_xyz(), dd = dd0, d = sqrt(dd), abba, ab;
	for( U4 i = 0; i < n; i++ )
	{
		a = (S-pCAGE[i]).xyz0();
		// +mm100(100)-a magának TOOL nak is adunk vele egy sugarat
        b = a.TSrBOX( T-pCAGE[i], pCAGE[i].w+mm100(100) );
        abba = (b-a).qlen_xyz();
        if( dd <= abba )
			continue;
		dd = abba;
	}
	if( dd >= dd0 )
		return T;

	I8x4 D8 = T-S;
	D8 *= sqrt(dd)-mm100(1);
	D8 /= sqrt(dd0);
	return S+D8;
}
U1 gpcZSnD::iDrc( bool bPULL )
{
	if( bPULL && !(ioSW.y&1) )
		ioSW.y |= 1;
	return (ioSW.y>>1)&1;
}
gpcLZY* gpcZSnD::pulling( gpcLZY* pOUT, U4x4* pZSrw )
{
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

gpcDrc& gpcDrc::judo( gpcZS& inp ) {
	*this = inp;
	switch( JD.y )
	{
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
			} return *this;
		case 6:	/// 5.HS2 0-ra várunk
			if(bHS2i()) {
				// még nem vette a jelet PULLING
				xHS2o();
				return *this;
			}
			JD.y = 7;
			// vette az adást mi is elvesszük
			xHS2o();
			return *this;
		case 5: /// 4.HS2 jelre várunk
			if(bHS2i()) {
				// jelzünk hogy olvastuk
				JD.y = 6;
				// kirakjuk a jelet
				oHS2o();

				switch(JD.z = iCTRL.z)
				{
					case 0:
						// megérkeztünk
						/*iXYZ.xyz_(oXYZ);
						iABC.xyz_(oABC);*/
						okXYZ.xyz_(oXYZ);
						okABC.xyz_(oABC);
						break;
					default:
						// ZS hibat jelzett
						// trg-be eltároljuk hol van most
						// ne is akarcon tovább menni
						tXYZ.xyz_( oXYZ.xyz_(iXYZ) );
						tABC.xyz_( oABC.xyz_(iABC) );
						break;
				}
				return *this;
			}
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
		case 3: /// 3.HS1 ZS jelére várunk
			if(bHS1i()) {
				// 3.HS1 megvan ZS jelzet olvasta
				JD.y = 4;
				// elveszük a jelet
				xHS1o();
				return *this;
			}
		case 2:
			/// PULLING HS1 : kint marat nem volt INIT
			JD.y = 3;
			oHS1o();
			return *this;
		case 1: /// 1.HS1 apa kezdődik
			oHS1o();
			JD.y = bHS1i() ? 2 : 3;
			return *this;
		default:
			break;
	}


	if( !NMnDIF.x )
		return *this;

	I8 dif = iXYZ.qlen_xyz(), mm = sqrt(dif)/100, A, B;
	if( mm < 400 )
		return *this;

	oCTRL.z = 0;
	if( okXYZ.qlen_xyz() > mm100(400) )
	{
		iXYZ.xyz_( okXYZ );
	}
	I4x4 dir = tXYZ - iXYZ, tmp;
	dif = dir.qlen_xyz();
	if( dif )
	{
		switch( NMnDIF.x )
		{
			case gpeZS_BILL:
				tmp = cageBALL( tXYZ.xyz0(), gpaCAGEbillBALL,gpmN(gpaCAGEbillBALL) );
				tmp = cageBOX( tmp, gpaCAGEbillBOX,gpmN(gpaCAGEbillBOX) );
				oXYZ.xyz_( iXYZ.lim_xyz(tmp,mm100(100)) );
				break;
			case gpeZS_JOHN:
				tmp = cageBALL( tXYZ.xyz0(), gpaCAGEjohnBALL,gpmN(gpaCAGEjohnBALL) );
				tmp = cageBOX( tmp, gpaCAGEjohnBOX,gpmN(gpaCAGEjohnBOX) );
				oXYZ.xyz_( iXYZ.lim_xyz(tmp,mm100(100)) );
				//oXYZ.xyz_( iXYZ.lim_xyz( cageBALL(gpaCAGEjohnBALL,gpmN(gpaCAGEjohnBALL)), mm100(100)) );
				break;
			default:
				oXYZ.xyz_( iXYZ );
				break;
		}
		oCTRL.z |= 1;
	}

	dir = tABC.chkABC( iABC, mm100(1) );
	if( dir.w ) {

		oCTRL.z |= 2;
	}

	if( oCTRL.z )
	{
		// itt kell le ellenőrizni mondjuk az ütközésre
		//
		JD.y = 0;
		switch( JD.x )
		{
			case 0: // ALAPHelyzet kell
				oCTRL.w = 0;
				oCTRL.z = 1;
                JD.y = 0;
				tXYZ.xyz_( oXYZ.xyz_(iXYZ) );
				tABC.xyz_( oABC.xyz_(iABC) );
				JD.x = 1;
				break;
			case 1: // ALAPH ready!
				oCTRL.w = 0;
				oCTRL.z = 10; // linearis XYZ ABC
				JD.y = 1;
				break;


		}
		/// 1.HS1 elöbb ki X-eljük a HS1-et, nehogy ZS megszaladjon
		if( JD.y )
			xHS1o();
		return *this;
	}

	/// join adatok most egyenlőre nem
	return *this;
}

gpcLZY* gpcGT::GTdrcOS( gpcLZY* pANS, U1* pSTR, gpcMASS& mass, SOCKET sockUSR )
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
					ZSnD.format();
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

				/*case gpeALF_KOSZON:{
						U4	nR = sizeof(gpsSLMPabc),
							nC = nR/(4*8);
						pANS = pANS->lzyFRMT( s = -1, "CSOKOLOM %s BACSI!", pSTR );

					} return pANS;*/
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
				default:
					iNUM = 23;
					break;
			}
			if( pD )
			if( oD != iD )
			{
				pANS = pD->answSTAT( pANS );
				oD = iD;
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


		pD = ZSnD.aDrc+iD;


		switch(iNUM)
        {
				// POS
			case 0:
			case 1:
			case 2:
				pD->tXYZ.aXYZW[iNUM%nNUM] = (d8 == 0.0) ? (I4)an.num*100 : (I4)(d8*100.0);
				break;
				// DIR
			case 3:
			case 4:
			case 5:
				pD->tABC.aXYZW[iNUM%nNUM] = (d8 == 0.0) ? (I4)an.num*100 : (I4)(d8*100.0);
				break;
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
				// AXIS
				pD->aoAX1to6[(iNUM%6)/3].aXYZW[iNUM%nNUM] = (d8 == 0.0) ? (I4)an.num*100 : (I4)(d8*100.0);
				break;

			// OFFSET - eltolás
				// POS
			case 12:
			case 13:
			case 14:
				pD->oxyz.aXYZW[iNUM%nNUM] = (d8 == 0.0) ? (I4)an.num*100 : (I4)(d8*100.0);
				break;
				// DIR
			case 15:
			case 16:
			case 17:
				pD->oabc.aXYZW[iNUM%nNUM] = (d8 == 0.0) ? (I4)an.num*100 : (I4)(d8*100.0);
				break;
			case 18:
			case 19:
			case 20:
			case 21:
			case 22:
			case 23:
				// AXIS
				pD->aoax1to6[(iNUM%6)/3].aXYZW[iNUM%nNUM] = (d8 == 0.0) ? (I4)an.num*100 : (I4)(d8*100.0);
				break;
			default:
				break;
        }
		iNUM++;
	}

	if( pD )
		return pD->answSTAT( pANS );

	return pANS->lzyFRMT( s = -1, "nonsens" );
}
