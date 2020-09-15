#include "gpcgt.h"
#include "gpcwin.h"
extern U1 gpaALFsub[];
extern char gpsTAB[], *gppTAB;
extern I4x4 gpaCAGEbillBALL[],
			gpaCAGEbillBOX[];
extern U4	gpnCAGEbillBALL,
			gpnCAGEbillBOX;
extern I4x4 gpaCAGEjohnBALL[],
			gpaCAGEjohnBOX[];
extern U4	gpnCAGEjohnBALL,
			gpnCAGEjohnBOX;

static char gpsJDpub[0x100];

gpcLZY* gpcLZY::lzyROBnDstat( U8& iSTRT, gpcROBnD& RnD, U1 i ) {
	if( !&RnD ) {
		iSTRT = nLD();
		return this;
	}
	gpcLZY* pANS = this;
	U1 n = gpmN(RnD.aDrc);
	if( i < n )
	{
		pANS = RnD.aDrc[i].answSTAT( pANS, i );
		return this;
	}

	for( U1 i = 0, n = gpmN(RnD.aDrc); i < n; i++ )
		pANS = RnD.aDrc[i].answSTAT( pANS, i );
	return this;
}
U1 gpcROBnD::iDrc( bool bPULL ) {
	if( bPULL && !(ioSW.y&1) )
		ioSW.y |= 1;
	return (ioSW.y>>1)&1;
}
gpcLZY* gpcROBnD::pull( gpcLZY* pOUT, U4x4* pROBrw ) {
	U8	s = -1;
	U4	i = this ? iDrc( true ):0,
		n = pROBrw[i].w;
	i = pROBrw[i].z;
	if( this )
		pc.x = 0;
	if( !bWAIT() )
		ioSW.w += 4;
	return pOUT->lzyFRMT( s, gpdSLMP_recv_LN4SL6N4, 24, i, n );
}
gpcDrc& gpcDrc::operator = ( const gpcROB& rob ) {
	if( rob.name != NMnDIF.x )
	{
		std::cout << "\033[1;31mIlegal W rob!\033[0m" << std::endl;
		return *this;
	}

	gpmMcpyOF( &iXYZ.x, &rob.aXYZ, 3 );
	if( iXYZ.qlen_xyz() < mmX(300) )
		iXYZ.x = iXYZ.y = iXYZ.z = mmX(600);

	gpmMcpyOF( &iABC.x, &rob.aABC, 3 );
	ixyz.xyz_(iXYZ.ABC2xyz( txyz, iABC ) );

	iCTRL.y = rob.HS;
	iCTRL.z = rob.COM;
	iCTRL.w = rob.PIX;

	gpmMcpyOF( &msSMR2.x, &rob.msSLV, 3 );

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
		okABC.xyz_(iABC);
	}
	return *this;
}

int gpcDrc::answINFOX( char* pANS, U4 id, I4 x ) {
	if( !this )
		return 0;

	I4x4 cXYZ = cageXYZ( mmX(gpdROBlim), id );
	U1 	sCOM[] = "ABCD";
	U4 &comA = *(U4*)sCOM;
	comA = NMnDIF.au4x2[0].x;
	return sprintf( pANS,

							"\r\n "
							"NM %s ERR 0x%0.4x ms 0x%0.8x xX %d "
							"POSX %d %d %d "
							"ABCX %d %d %d "
							"posx %d %d %d "
							"HS 0x%0.4x ",
							(char*)sCOM, JD.z, msSRT3.x, x,

							(iXYZ.x*x)/mmX(1), (iXYZ.y*x)/mmX(1), (iXYZ.z*x)/mmX(1),
							(iABC.x*x)/degX(1), (iABC.y*x)/degX(1), (iABC.z*x)/degX(1),
							(txyz.x*x)/mmX(1), (txyz.y*x)/mmX(1), (txyz.z*x)/mmX(1),

							hs123()
					);

	//return nS;
}
gpcLZY* gpcDrc::answINFOX( gpcLZY* pANS, U4 id, I4 x ) {
	if( !this )
		return pANS;
	char sBUFF[0x100];
	if( !answINFOX( sBUFF, id, x ) )
		return pANS;
	U8 s = -1;
	return pANS->lzyFRMT( s, "%s", sBUFF );
}
int gpcDrc::answINFO( char* pANS, U4 id ) {
	if( !this )
		return 0;

	I4x4 cXYZ = cageXYZ( mmX(gpdROBlim), id );
	U1 	sCOM[] = "ABCD";
	U4 &comA = *(U4*)sCOM;
	comA = NMnDIF.au4x2[0].x;
	return sprintf( pANS,

							"\r\n "
							"NM %s ERR 0x%0.4x ms 0x%0.8 xX %7.2f "
							"POSX %7.2f %7.2f %7.2f "
							"ABCX %7.2f %7.2f %7.2f "
							"posx %7.2f %7.2f %7.2f "
							"HS 0x%0.4x ",
							sCOM, JD.z, msSRT3.x, 1.0,

							double(iXYZ.x)/mmX(1),	double(iXYZ.y)/mmX(1),	double(iXYZ.z)/mmX(1),
							double(iABC.x)/degX(1),	double(iABC.y)/degX(1),	double(iABC.z)/degX(1),
							double(txyz.x)/mmX(1),	double(txyz.y)/mmX(1),	double(txyz.z)/mmX(1),

							hs123()
					);

	//return nS;
}
gpcLZY* gpcDrc::answINFO( gpcLZY* pANS, U4 id ) {
	if( !this )
		return pANS;
	char sBUFF[0x100];
	if( !answINFO( sBUFF, id ) )
		return pANS;
	U8 s = -1;
	return pANS->lzyFRMT( s, "%s", sBUFF );
}

gpcLZY* gpcDrc::answSTAT( gpcLZY* pANS, U4 id ) {
		if( !this )
			return pANS;
		I4x4 cXYZ = cageXYZ( mmX(gpdROBlim), id );

		U1 	sCOM[] = "ABCD";
		U4 &comA = *(U4*)sCOM;

		comA = NMnDIF.x;
		U8 s;
		pANS = pANS->lzyFRMT( s = -1,	"\r\n// %s HS123:%0.6X E:%dms AVG:%dms", *sCOM ? (char*)sCOM : "?", hs123(), Ems, AVGms );
		pANS = pANS->lzyFRMT( s = -1,	"\r\n// POS:  n:%d"
										"\r\n//\tiXYZ %7.2fmm, %7.2fmm, %7.2fmm "
										"\r\n//\tcXYZ %7.2fmm, %7.2fmm, %7.2fmm "
										"\r\n//\toXYZ %7.2fmm, %7.2fmm, %7.2fmm "
										"\r\n//\ttXYZ %7.2fmm, %7.2fmm, %7.2fmm "
										"\r\n//\tSMR2 %9.2fs,  %9.2fs, %9.2fs, %9.2fs;"
										"\r\n//\tDIFF %9.2fs,  %9.2fs, %9.2fs, %9.2fs;"
										,
										(U4)(sqrt((tXYZ-iXYZ).qlen_xyz())/mmX(1)),
										double(iXYZ.x)/mmX(1),
										double(iXYZ.y)/mmX(1),
										double(iXYZ.z)/mmX(1),

										double(cXYZ.x)/mmX(1),
										double(cXYZ.y)/mmX(1),
										double(cXYZ.z)/mmX(1),

										double(oXYZ.x)/mmX(1),
										double(oXYZ.y)/mmX(1),
										double(oXYZ.z)/mmX(1),

										double(tXYZ.x)/mmX(1),
										double(tXYZ.y)/mmX(1),
										double(tXYZ.z)/mmX(1),

										double( msSMR2.x )/ms2sec,
										double( msSMR2.y )/ms2sec,
										double( msSMR2.z )/ms2sec,
										double( msSMR2.w )/ms2sec,

										double( msSRT3.x-msSMR2.x )/ms2sec,
										double( msSRT3.x-msSMR2.y )/ms2sec,
										double( msSRT3.x-msSMR2.z )/ms2sec,
										double( msSRT3.x-msSMR2.w )/ms2sec

							);
		pANS = pANS->lzyFRMT( s = -1,	"\r\n// DIR:  n:%d"
										"\r\n//\tiABC %7.2fdg, %7.2fdg, %7.2fdg "
										"\r\n//\toABC %7.2fdg, %7.2fdg, %7.2fdg "
										"\r\n//\ttABC %7.2fdg, %7.2fdg, %7.2fdg;"
										,
										(U4)(sqrt((tABC-iABC).qlen_xyz())/degX(1)),
										double(iABC.x)/degX(1),
										double(iABC.y)/degX(1),
										double(iABC.z)/degX(1),

										double(oABC.x)/degX(1),
										double(oABC.y)/degX(1),
										double(oABC.z)/degX(1),

										double(tABC.x)/degX(1),
										double(tABC.y)/degX(1),
										double(tABC.z)/degX(1)
							);
		pANS = pANS->lzyFRMT( s = -1,	"\r\n// pos:"
										"\r\n//\tixyz %7.2fmm, %7.2fmm, %7.2fmm "
										"\r\n//\toxyz %7.2fmm, %7.2fmm, %7.2fmm "
										"\r\n//\ttxyz %7.2fmm, %7.2fmm, %7.2fmm;"
										,
										double(ixyz.x)/mmX(1),
										double(ixyz.y)/mmX(1),
										double(ixyz.z)/mmX(1),

										double(oxyz.x)/mmX(1),
										double(oxyz.y)/mmX(1),
										double(oxyz.z)/mmX(1),

										double(txyz.x)/mmX(1),
										double(txyz.y)/mmX(1),
										double(txyz.z)/mmX(1)
							);
		pANS = pANS->lzyFRMT( s = -1,	"\r\n// ok:"
										"\r\n//\tokXYZ %7.2fmm, %7.2fmm, %7.2fmm "
										"\r\n//\tokabc %7.2fdg, %7.2fdg, %7.2fdg "
										"\r\n//\tokxyz %7.2fmm, %7.2fmm, %7.2fmm;"
										,
										double(okXYZ.x)/mmX(1),
										double(okXYZ.y)/mmX(1),
										double(okXYZ.z)/mmX(1),

										double(okABC.x)/degX(1),
										double(okABC.y)/degX(1),
										double(okABC.z)/degX(1),

										double(okxyz.x)/mmX(1),
										double(okxyz.y)/mmX(1),
										double(okxyz.z)/mmX(1)
							);
		/*pANS = pANS->lzyFRMT( s = -1,	"\r\n// dir:"
										"\r\n//\tiabc %7.2fdg, %7.2fdg, %7.2fdg "
										"\r\n//\toabc %7.2fdg, %7.2fdg, %7.2fdg "
										"\r\n//\ttabc %7.2fdg, %7.2fdg, %7.2fdg;"
										,
										double(iabc.x)/degX(1),
										double(iabc.y)/degX(1),
										double(iabc.z)/degX(1),

										double(oabc.x)/degX(1),
										double(oabc.y)/degX(1),
										double(oabc.z)/degX(1),

										double(tabc.x)/degX(1),
										double(tabc.y)/degX(1),
										double(tabc.z)/degX(1)
							);*/
		return pANS;
	}

I4x4 gpcDrc::cageBALL( I4x4 T, I4x4* pCAGE, U4 n ) {
	I4x4 S = iXYZ.xyz0(), a, b;
	I8 dd0 = (T-S).qlen_xyz(), dd = dd0, d = sqrt(dd), abba, ab;
	for( U4 i = 0; i < n; i++ )
	{
		a = (S-pCAGE[i]).xyz0();
		// +mmX(100)-a magának TOOL nak is adunk vele egy sugarat
        b = a.TSrBALL( T-pCAGE[i], pCAGE[i].w+mmX(100) );
        abba = (b-a).qlen_xyz();
        if( dd <= abba )
			continue;
		dd = abba;
	}
	if( dd >= dd0 )
		return T;

	I8x4 D8 = T-S;
	D8 *= sqrt(dd)-mmX(1);
	D8 /= sqrt(dd0);
	return S+D8;
}
I4x4 gpcDrc::cageBOX( I4x4 T, I4x4* pCAGE, U4 n ) {
	I4x4 S = iXYZ.xyz0(), a, b;
	I8 dd0 = (T-S).qlen_xyz(), dd = dd0, d = sqrt(dd), abba, ab;
	for( U4 i = 0; i < n; i++ )
	{
		a = (S-pCAGE[i]).xyz0();
		// +mmX(100)-a magának TOOL nak is adunk vele egy sugarat
        b = a.TSrBOX( T-pCAGE[i], pCAGE[i].w+mmX(100) );
        abba = (b-a).qlen_xyz();
        if( abba < 1 )
			return S;
        if( dd <= abba )
			continue;
		dd = abba;
	}
	if( dd >= dd0 )
		return T;

	I8x4 D8 = T-S;
	D8 *= sqrt(dd)-mmX(1);
	D8 /= sqrt(dd0);
	return S+D8;
}

void drc_trd( char* pBUFF ) {
	if( pBUFF ? !pBUFF : true )
		return;
	char	sBUFF[0x100];
	strcpy( sBUFF, pBUFF );
	int o = system( sBUFF );
	std::cout << o << ":" << sBUFF <<std::endl;;
}
bool gpcDrc::async( char* pBUFF, gpcALU& alu, gpcRES* pRES ) {
	if( this ? !pBUFF : true )
		return false;

	U1 sNM[] = "ABCD";
	U4	//hs2 = hs123(),
		&nm = *(U4*)sNM;
	if( JD.w == JD.y )
		return false;

	JD.w = JD.y;
	if( okXYZ.xyz0() != tXYZ.xyz0() )
		return false;

	switch( JD.w )
	{
		case 4:{
			// 5->6 jelzünk hogy olvastuk a HS2i-t
			nm = NMnDIF.x;
			gpcADR A0 = gpfSTR2ALF( sNM, sNM+4 );	/// gpcADR
			A0 = pRES;
			if( !A0.pRM )
				return false;

			gpcALU aB = A0.pRM->ALU( A0.iA );
			if(!aB.bSTR())
				return false;
			if(!alu.pDAT)
				return false;

			char	//*p_pat = (char*)alu.pDAT,
					*pP = pBUFF;

			pP += okXYZ.str( pP, "_" );
			pP += okABC.str( pP, "_" );
			pP += sprintf( pP, ".png" );
			pP++;
			sprintf( pP, (char*)alu.pDAT, pBUFF );
			if( n_join < n_trd )
			{
				trd.join();
				n_join = n_trd;
			}
			trd = std::thread( drc_trd, pP );
			n_trd++;

			//int o = system( pP+1 );
			//std::cout << o << ":" << pP+1 <<std::endl;;
		} return true;
	}
	return false;
}

gpcDrc& gpcDrc::judo( gpcROB& iROB, U4 mSEC ) {
	*this = iROB;	/// XYZABCxyz 1. ixyz = iXYZ.ABC2xyz( txyz, iABC );
	switch( JD.y ) {
		case 4: { /// 4.HS2i? // HS2o elvesz VÉGE
				if( bHS2i() )
				{
					/// PULLING még nem vette az adást
					oHS2o();
					JD.y = 4;
					return *this;
				}
				xHS2o();
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
			} return *this; // hagyjuk élni a másik robotot is
		case 3: { /// 3.HS2i? // HS2o kitesz
				if( bHS2i() ) {
					/// na végre megérkezett a robot hurrá
					switch(JD.z = iCTRL.z)
					{
						case 0:
							/// XYZABCxyz 2. OK ------------- JD.z = 0
							okXYZ.xyz_(oXYZ);
							okABC.xyz_(oABC);
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
								tABC.xyz_( oABC.xyz_(okABC) );
								std::cout << "9115tXYZ: " << tXYZ.pSTR( gpsJDpub ) << std::endl;
							} else {
								tXYZ.xyz_( oXYZ.xyz_(iXYZ) );
								tABC.xyz_( oABC.xyz_(iABC) );
								std::cout << "Err tXYZ: " << tXYZ.pSTR( gpsJDpub ) << std::endl;
							}
							break;
					}

					oHS2o();
					JD.y = 4;
					return *this;
				} else if( bHS1i() ? false : HS1ms ) {
					/// megnézzük holt tart a robot
					U4	lag = (mSEC-msSMR2.w),
						lagX = msSRT3.x+lag;
					std::cout << "HS1ms-msSRT3=" << HS1ms << "-" << lagX << "=" << (I8)HS1ms-(I8)lagX << std::endl;
					if( HS1ms < lagX )
					{
						HS1ms = 0;
						//oCTRL.z = 0;
						//JD.y = 0;
						std::cout << "\033[1;32mEXTRA GO! lag:" << lag << "ms\033[0m" << std::endl;
						//break;
					}
				}
				/// PULLING most arra várunk megérkezzen a robot
				xHS1o();
				JD.y = 3;
				// nem fogunk várni számolni fogunk új poziciót
			} return *this;
		case 2: { /// 2.HS1o elvesz
				JD.y = bHS1i() ? 2 : 3;
				xHS1o();
			} return *this;
		case 1: { /// 1.HS1o kintvan // HS1i?
				if( bHS1i() )
				{
					/// na végre
					xHS1o();
					JD.y = 2;
					return *this;
				}
				/// PULLING még nem vette az adást tartjuk
				oHS1o();

				if( bHS2i() )
				{
					//! bent maradt a HS2
					oHS2o();
					JD.y = 4;
					return *this;
				} else {
					xHS2o();
				}
				JD.y = 1;
			} return *this;
		default:
			if( bHS2i() )
			{
				//! bent maradt a HS2
				oHS2o();
				JD.y = 4;
				return *this;
			}
			break;
	}


	if( !NMnDIF.x )
		return *this;

	I8x4 mmABCD( sqrt(iXYZ.qlen_xyz()) );
	if( mmABCD.x < mmX(250) )
		return *this;		// nem fut a R2D task


	if( !JD.x ) {
		oCTRL.w = 0;
		oCTRL.z = 1;
		JD.y = 0;
		tXYZ.xyz_( oXYZ.xyz_(iXYZ) );
		tABC.xyz_( oABC.xyz_(iABC) );
		if( txyz.qlen_xyz() )
			txyz.xyz_( oxyz.xyz_(ixyz) );
		JD.x = 1;
		return *this;
	}

	/// --------------------
	/// CTRL.z = 0;
	/// --------------------
	oCTRL.z = 0;
	if( sqrt(okXYZ.qlen_xyz()) > mmX(200) ){
		// ezt kaptuk a robitol
		// ha netán akkor sem mozdul
		oXYZ.xyz_(iXYZ);
		oABC.xyz_(iABC);
	}

	F4x4 tMX = 1.0, iMX;
	iMX.ABC(iABC, degX(180.0/PI) );
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
		if( mmABCD.x < (mmX(1)/0x10) )
		{
			mmABCD.y = mmABCD.x = 0;
		} else if( mmABCD.y ) {
			if( lim <= mmABCD.y+mmX(1) )
			{
				// elérte a lim-et azaz nem érte el a ketrecet
				lim = mmABCD.y;
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
			tABC.xyz_( tMX.eulABC()*degX(180.0/PI) );
			itD = iABC.mmABC( tABC, degX(180.0/PI), degX(180.0/PI) );
		}
	}

	if( itD.w < (degX(1)/2) ) {	// kb: 50
		tABC.xyz_(oABC.xyz_(iABC));
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
					oABC.xyz_( ((itD*mmABCD.y)/mmABCD.x)+iABC );
				else if( ab > 0.0 )
					oABC.xyz_( (F4(itD)*ab)+iABC );


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
				oABC.xyz_( tABC );
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
		tABC.xyz_( oABC );
		std::cout << "CAGE tXYZ: " << tXYZ.pSTR( gpsJDpub ) <<std::endl;
	} else
		std::cout << "oXYZ: " << oXYZ.pSTR( gpsJDpub ) << "\r\ntXYZ: " << tXYZ.pSTR( gpsJDpub ) <<std::endl;

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
			oCTRL.z = 11; // linearis XYZ ABC S
			JD.y = 1;
			break;
	}
	/// 1.HS1 elöbb ki X-eljük a HS1-et, nehogy ZS megszaladjon
	if( JD.y )
	{
		oHS1o();
		xHS2o();
	}
	return *this;

}


