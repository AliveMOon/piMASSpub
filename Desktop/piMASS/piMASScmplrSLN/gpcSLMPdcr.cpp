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

gpcLZY* gpcLZY::lzyROBnDstat( U8& iSTRT, gpcROBnD& RnD, U1 i, int rR, char* pPP ) {
	if( !&RnD ) {
		iSTRT = nLD();
		return this;
	}
	gpcLZY* pANS = this;
	U1 n = gpmN(RnD.aDrc);
	if( i < n )
	{
		pANS = RnD.aDrc[i].answSTAT( pANS, i, rR, pPP );
		return this;
	}

	for( U1 i = 0, n = gpmN(RnD.aDrc); i < n; i++ )
		pANS = RnD.aDrc[i].answSTAT( pANS, i, rR, pPP );
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
		if(bSTDcout){gpdCOUT << "\033[1;31mIlegal W rob!\033[0m" << gpdENDL;}
		return *this;
	}

	gpmMcpyOF( &iXYZ.x, &rob.aXYZ, 3 );
	if( iXYZ.qlen_xyz() < mmX(300) )
		iXYZ.x = iXYZ.y = iXYZ.z = mmX(600);

	gpmMcpyOF( &iABC.A, &rob.aABC, 3 );
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
		okABC.ABC_(iABC);
	}
	return *this;
}

int gpcDrc::answINFOX( char* pANS, U4 id, I4 x ) {
	if( !this )
		return 0;

	//I4x4 cXYZ = cageXYZ( mmX(gpdROBlim), id );
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
							(iABC.A*x)/degX(1), (iABC.B*x)/degX(1), (iABC.C*x)/degX(1),
							(ixyz.x*x)/mmX(1), (ixyz.y*x)/mmX(1), (ixyz.z*x)/mmX(1),

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

int gpcDrc::answINFO( char* pANS, U4 id, int rR ) {
	if( !this )
		return 0;

	I4x4 cXYZ = cageXYZ( mmX(gpdROBlim), id, rR );
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
							double(iABC.A)/degX(1),	double(iABC.B)/degX(1),	double(iABC.C)/degX(1),
							double(txyz.x)/mmX(1),	double(txyz.y)/mmX(1),	double(txyz.z)/mmX(1),

							hs123()
					);

	//return nS;
}
gpcLZY* gpcDrc::answINFO( gpcLZY* pANS, U4 id, int rR ) {
	if( !this )
		return pANS;
	char sBUFF[0x100];
	if( !answINFO( sBUFF, id, rR ) )
		return pANS;
	U8 s = -1;
	return pANS->lzyFRMT( s, "%s", sBUFF );
}
gpcLZY* gpcDrc::answSTAT( gpcLZY* pANS, U4 id, int rR, char* pPP ) {
		if( !this )
			return pANS;
        if( !pPP )
            pPP = "//";
		I4x4 cXYZ = cageXYZ( mmX(gpdROBlim), id, rR );

		U1 	sCOM[] = "ABCD";
		U4 &comA = *(U4*)sCOM;

		comA = NMnDIF.x;
		U8 s;
		pANS = pANS->lzyFRMT( s = -1,	"\r\n%s %s HS123:%0.6X E:%dms AVG:%dms", pPP, *sCOM ? (char*)sCOM : "?", hs123(), Ems, AVGms );
		pANS = pANS->lzyFRMT( s = -1,	"\r\n%s POS:  n:%d"
										"\r\n%s\tiXYZ %7.2fmm, %7.2fmm, %7.2fmm "
										"\r\n%s\tcXYZ %7.2fmm, %7.2fmm, %7.2fmm "
										"\r\n%s\toXYZ %7.2fmm, %7.2fmm, %7.2fmm "
										"\r\n%s\ttXYZ %7.2fmm, %7.2fmm, %7.2fmm "
										"\r\n%s\tSMR2 %9.2fs,  %9.2fs, %9.2fs, %9.2fs;"
										"\r\n%s\tDIFF %9.2fs,  %9.2fs, %9.2fs, %9.2fs;",
										pPP, (U4)(sqrt((tXYZ-iXYZ).qlen_xyz())/mmX(1)),

										pPP,
										double(iXYZ.x)/mmX(1),
										double(iXYZ.y)/mmX(1),
										double(iXYZ.z)/mmX(1),

										pPP,
										double(cXYZ.x)/mmX(1),
										double(cXYZ.y)/mmX(1),
										double(cXYZ.z)/mmX(1),

										pPP,
										double(oXYZ.x)/mmX(1),
										double(oXYZ.y)/mmX(1),
										double(oXYZ.z)/mmX(1),

										pPP,
										double(tXYZ.x)/mmX(1),
										double(tXYZ.y)/mmX(1),
										double(tXYZ.z)/mmX(1),

										pPP,
										double( msSMR2.x )/ms2sec,
										double( msSMR2.y )/ms2sec,
										double( msSMR2.z )/ms2sec,
										double( msSMR2.w )/ms2sec,

										pPP,
										double( msSRT3.x-msSMR2.x )/ms2sec,
										double( msSRT3.x-msSMR2.y )/ms2sec,
										double( msSRT3.x-msSMR2.z )/ms2sec,
										double( msSRT3.x-msSMR2.w )/ms2sec

							);
		pANS = pANS->lzyFRMT( s = -1,	"\r\n%s DIR:  n:%d"
										"\r\n%s\tiABC %7.2fdg, %7.2fdg, %7.2fdg "
										"\r\n%s\toABC %7.2fdg, %7.2fdg, %7.2fdg "
										"\r\n%s\ttABC %7.2fdg, %7.2fdg, %7.2fdg;",
										pPP, (U4)(sqrt((tABC-iABC).qlen_xyz())/degX(1)),
										pPP,
										double(iABC.A)/degX(1),
										double(iABC.B)/degX(1),
										double(iABC.C)/degX(1),

										pPP,
										double(oABC.A)/degX(1),
										double(oABC.B)/degX(1),
										double(oABC.C)/degX(1),

										pPP,
										double(tABC.A)/degX(1),
										double(tABC.B)/degX(1),
										double(tABC.C)/degX(1)
							);
		pANS = pANS->lzyFRMT( s = -1,	"\r\n%s pos:"
										"\r\n%s\tixyz %7.2fmm, %7.2fmm, %7.2fmm "
										"\r\n%s\toxyz %7.2fmm, %7.2fmm, %7.2fmm "
										"\r\n%s\ttxyz %7.2fmm, %7.2fmm, %7.2fmm;",
										pPP,

										pPP,
										double(ixyz.x)/mmX(1),
										double(ixyz.y)/mmX(1),
										double(ixyz.z)/mmX(1),

										pPP,
										double(oxyz.x)/mmX(1),
										double(oxyz.y)/mmX(1),
										double(oxyz.z)/mmX(1),

										pPP,
										double(txyz.x)/mmX(1),
										double(txyz.y)/mmX(1),
										double(txyz.z)/mmX(1)
							);
		pANS = pANS->lzyFRMT( s = -1,	"\r\n%s ok:"
										"\r\n%s\tokXYZ %7.2fmm, %7.2fmm, %7.2fmm "
										"\r\n%s\tokabc %7.2fdg, %7.2fdg, %7.2fdg "
										"\r\n%s\tokxyz %7.2fmm, %7.2fmm, %7.2fmm;",
										pPP,

										pPP,
										double(okXYZ.x)/mmX(1),
										double(okXYZ.y)/mmX(1),
										double(okXYZ.z)/mmX(1),

										pPP,
										double(okABC.A)/degX(1),
										double(okABC.B)/degX(1),
										double(okABC.C)/degX(1),

										pPP,
										double(okxyz.x)/mmX(1),
										double(okxyz.y)/mmX(1),
										double(okxyz.z)/mmX(1)
							);

		return pANS;
	}

I4x4 gpcDrc::cageBALL( I4x4 T, I4x4* pCAGE, U4 n, int rR ) {
	I4x4 S = iXYZ.xyz0(), a, b;
	I8 dd0 = (T-S).qlen_xyz(), dd = dd0, d = sqrt(dd), abba, ab;
	for( U4 i = 0; i < n; i++ )
	{
		a = (S-pCAGE[i]).xyz0();
		// +mmX(100)-a magának TOOL nak is adunk vele egy sugarat
        b = a.TSrBALL( T-pCAGE[i], pCAGE[i].w+rR );
        abba = (b-a).qlen_xyz();
        if( dd <= abba )
			continue;
		b = a.TSrBALL( T-pCAGE[i], pCAGE[i].w+rR );
		dd = abba;
	}
	if( dd >= dd0 )
		return T;

	I8x4 D8 = T-S;
	D8 *= sqrt(dd)-mmX(1);
	D8 /= sqrt(dd0);
	return S+D8;
}
I4x4 gpcDrc::cageBOX( I4x4 T, I4x4* pCAGE, U4 n, int rR ) {
	I4x4 S = iXYZ.xyz0(), a, b;
	I8 dd0 = (T-S).qlen_xyz(), dd = dd0, d = sqrt(dd), abba, ab;
	for( U4 i = 0; i < n; i++ )
	{
		a = (S-pCAGE[i]).xyz0();
		// +mmX(100)-a magának TOOL nak is adunk vele egy sugarat
        b = a.TSrBOX( T-pCAGE[i], pCAGE[i].w+rR );
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
	char	sBUFF[0x100], *pB = pBUFF;
	pB += gpmNINCS(pB, "\" \t");
	U8 nLEN, n;
	while( *pB ) {
		pB += gpmNINCS(pB, "\r\n");
		n = gpmVAN(pB, "\r\n", nLEN );
		gpmMcpy( sBUFF, pB, n )[n] = 0;
		pB += n;
		int o = system( sBUFF );
		if(bSTDcout_slmp){gpdCOUT << o << ":" << sBUFF <<gpdENDL;};
	}

}
/// http://192.168.1.160:5000/api/camera/take_calibration_image?camera=SECOND&x=600&y=600&z=600&a=180&b=0&c=135&first=false

/// wget -O /robi/first%s.txt http://192.168.1.160:5000/api/camera/take_calibration_image?camera=FIRST&x&y&z&a&b&c&f
///	wget -O /robi/first%s.png http://192.168.1.160:5000/api/camera/take_picture?camera=FIRST&width=1280&height=1024
///	wget -O /robi/second%s.txt http://192.168.1.160:5000/api/camera/take_and_save_picture_given_name?camera=SECOND&name=second%s
///	wget -O /robi/second%s.png http://192.168.1.160:5000/api/camera/take_picture?camera=SECOND&width=1280&height=1024";
bool gpcDrc::asyncSYS( char* pBUFF, U1* pCLI ) {
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
	U8 nLEN;
	switch( JD.w )
	{
		case 4:{
			if( !pCLI )
				return true;

			char	*pXS = pBUFF, *pC = (char*)pCLI, * pE = pC+gpmSTRLEN(pC), *pP, *pB;
			pXS += sprintf( pXS, "%0.4d_",jdPRG.y );
			pXS += okXYZ.str( pXS, "_" );
			pXS += okABC.str( pXS, "_" );
			size_t n = 0, nXS = pXS-pBUFF;
			pXS++; /// 0

			pP = pB = pXS;
			while( *pC ) {
				n = gpmVAN( pC, "%&!", nLEN );
				gpmMcpy( pB, pC, n );
				pC += n;
				pB += n;
				switch( pC[0] ) {
					case '!':
						switch( pC[1] ){
							case '!':
								pB += sprintf( pB, "\"" );
								break;
							default:
								gpmMcpy( pB, pC, 2 );
								pB += 2;
								break;
						}
						break;
					case '&':
					case '%':
						switch( pC[1] ){
							case 's':
								gpmMcpy( pB, pBUFF, nXS );
								pB += nXS;
								break;
							case 'x':
								pB += sprintf( pB, "&x=%0.4d", okXYZ.x );
								break;
							case 'y':
								pB += sprintf( pB, "&y=%0.4d", okXYZ.y );
								break;
							case 'z':
								pB += sprintf( pB, "&z=%0.4d", okXYZ.z );
								break;
							case 'a':
								pB += sprintf( pB, "&a=%0.4d", okABC.x );
								break;
							case 'b':
								pB += sprintf( pB, "&b=%0.4d", okABC.y );
								break;
							case 'c':
								pB += sprintf( pB, "&c=%0.4d", okABC.z );
								break;
							case 'f':
								pB += sprintf( pB, "&first=%s", ((jdPRG.y-1) ? "false" : "true" ) );
								break;
						}
						break;
					default:
						break;
				}
				pC += 2;
			}
			//U4 n = sprintf( pP, (char*)pCLI, pBUFF );

			if( n_join < n_trd )
			{
				trd.join();
				n_join = n_trd;
			}
			trd = std::thread( drc_trd, pP );
			n_trd++;
            if(bSTDcout_slmp){gpdCOUT << pP+1 <<gpdENDL;};
			//int o = system( pP+1 );
			//if(bSTDcout){gpdCOUT << o << ":" << pP+1 <<gpdENDL;};
		} return true;
	}
	return false;
}
#define gpdXeZxY true
#define gpdXeYxZ false
gpcDrc& gpcDrc::judo( gpcROB& iROB, U4 mSEC, U4 iD0 ) {
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
								if(bSTDcout){gpdCOUT << "9115tXYZ: " << tXYZ.pSTR( gpsJDpub ) << gpdENDL;}
							} else {
								tXYZ.xyz_( oXYZ.xyz_(iXYZ) );
								tABC.ABC_( oABC.ABC_(iABC) );
								if(bSTDcout){gpdCOUT << "Err tXYZ: " << tXYZ.pSTR( gpsJDpub ) << gpdENDL;}
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
					if(bSTDcout){gpdCOUT << "HS1ms-msSRT3=" << HS1ms << "-" << lagX << "=" << (I8)HS1ms-(I8)lagX << gpdENDL;}
					if( HS1ms < lagX )
					{
						HS1ms = 0;
						//oCTRL.z = 0;
						//JD.y = 0;
						if(bSTDcout){gpdCOUT << "\033[1;32mEXTRA GO! lag:" << lag << "ms\033[0m" << gpdENDL;}
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
		tABC.ABC_( oABC.ABC_(iABC) );
		if( txyz.qlen_xyz() )
			txyz.xyz_( oxyz.xyz_(ixyz) );
		JD.x = 1;
		return *this;
	}

	/// --------------------
	/// CTRL.z = 0;
	/// --------------------
	oCTRL.z = 0;
	if( sqrt(okXYZ.qlen_xyz()) > mmX(200) ) {
		// ezt kaptuk a robitol
		// ha netán akkor sem mozdul
		oXYZ.xyz_(iXYZ);
		oABC.ABC_(iABC);
	}

	F4x4 iMX, oMX, tMX = 1.0;
	iMX.mxABC(iABC, degX(180.0/PI) );
	// itt kell le ellenőrizni mondjuk az üTKöZéSre
	//
	float ab = 1.0, k;
	static const float K = (100.0*PI2);

	I4 lim = 0;
	I4x4 	itABCdif = iABC.mmABC( tABC, degX(180.0/PI), degX(180.0/PI) ),
			tmp, dxyz,
			dXYZ = tXYZ - iXYZ, lXYZ,
			dGRP = tGRP - iGRP, hdXYZ,
			aHDxyz[8];


	U4 nH = gpmN(gpaCAGEheadBALL);

	mmABCD.D = sqrt(dGRP.qlen_xyz());
	mmABCD.A = dXYZ.abs0().mx().x;
	bool bNEW = true;

	if( mmABCD.A ) {
		U4 i = 3; //iD0;
		switch( NMnDIF.x )
		{
			case gpeZS_BILL: {
					i = 0;
					gpcDrc* pJ = this+1;
					I4x4	j2b = (pJ->iXYZ+pJ->tXYZ)/2,
							*pTOOL = pBALLtool( i );
					*pTOOL = (j2b&I4x4(-1,-1,1,0)) + I4x4( mmX(1500), 0, 0, mmX(300) );
				} break;
			case gpeZS_JOHN: {
					i = 1;
					gpcDrc* pB = this-1;
					I4x4 	b2j = (pB->iXYZ+pB->tXYZ)/2,
							*pTOOL = pBALLtool( i );
					*pTOOL = (b2j&I4x4(-1,-1,1,0)) + I4x4( mmX(1500), 0, 0, mmX(300) );
				} break;
			default:
				i = 3;
				break;
		}


		/// megszüntettem a dadogást
		/// a nagybetűs JUDO-ban benne van ha kell


		if( bNEW ) {
			oXYZ.xyz_(tXYZ);
			oABC.ABC_(tABC);
			oxyz.xyz_(txyz);
			for( int iH = nH-1; iH>-1; iH-- ) {
				if( iH == nH-1) {
					oMX = 1.0;
					oMX.mxABC(oABC, degX(180.0/PI) );
					if( oxyz.qlen_xyz() ) {
						oMX.z.xyz_( oxyz-oXYZ );
						oMX.z /= sqrt(oMX.z.qlen_xyz());	// normalizál

						switch( jdALF ) {
							case gpeALF_BRIDGE:
							case gpeALF_SNAIL:
								oMX.x = jd0mxTL.y.X3(oMX.z);
								oMX.y = jd0mxTL.z.X3(oMX.x);
								break;
							default:
								oMX.x = oMX.y.X3(oMX.z);
								oMX.y = oMX.z.X3(oMX.x);
								break;
						}

						float	xl = oMX.x.qlen_xyz(),
								yl = oMX.y.qlen_xyz();
						if( xl > yl ) {
							oMX.x /= sqrt(xl);
							oMX.y = oMX.z.X3(oMX.x);
						} else {
							oMX.y /= sqrt(yl);
							oMX.x = oMX.y.X3(oMX.z);
						}
					}
					oABC.ABC_( oMX.eulABC()*degX(180.0/PI) );
				}
				hdXYZ = gpaCAGEheadBALL[iH];
				aHDxyz[0] = (iMX.x*double(hdXYZ.x))+(iMX.y*double(hdXYZ.y))+(iMX.z*double(hdXYZ.z))+iXYZ;
				aHDxyz[1] = (oMX.x*double(hdXYZ.x))+(oMX.y*double(hdXYZ.y))+(oMX.z*double(hdXYZ.z))+oXYZ;
				aHDxyz[4] = (aHDxyz[1]-aHDxyz[0]).xyz0();
				aHDxyz[3].A = sqrt(aHDxyz[4].qlen_xyz());
				if( aHDxyz[3].A < (mmX(1)/16) )
					continue; // nem mozdult

				aHDxyz[2] = cageXYZhd0( aHDxyz, aHDxyz[3].A, i, hdXYZ.w );
								//cageXYZhd( aHDxyz, aHDxyz[3].A, gpapBOX[i], gpanBOX[i], gpapBALL[i], gpanBALL[i], hdXYZ.w );
				aHDxyz[3].B = ((aHDxyz[2]-aHDxyz[0]).xyz0()*aHDxyz[4])/aHDxyz[3].A;
				if( aHDxyz[3].B < (mmX(1)/16) ) {
					// ellentétes írányú
					oXYZ.xyz_(iXYZ);
					oABC.ABC_(iABC);
					break;
				}
				if( (aHDxyz[3].A-aHDxyz[3].B) < (mmX(1)/8) )
					continue;

				iH = nH-1;
				oXYZ.xyz_( (oXYZ+iXYZ)/2 );
				oABC.ABC_( tABC );
				if( !oxyz.qlen_xyz() )
					continue;
				oxyz.xyz_( (oxyz+ixyz)/2 );
			}
		}
		 else  {
			lim = mmABCD.A;
			oXYZ.xyz_( cageXYZ(lim,i,gpaCAGEheadBALL[0].w) );
			lXYZ = oXYZ - iXYZ;
			mmABCD.B = lXYZ.abs0().mx().x;
			if( mmABCD.A < (mmX(1)/0x10) ) {
				mmABCD.B =
				mmABCD.x = 0;
			}
			else if( mmABCD.B ) {
				if( lim <= mmABCD.B+mmX(1) ) {
					// elérte a lim-et azaz nem érte el a ketrecet
					lim = mmABCD.B;
				}
				else if( mmABCD.B > mmX(10) ) {
					// kisebb let mint amit elvártunk a ketrec nem engedte
					I4 mm = mmABCD.B/mmX(1);
					lXYZ *= mm-10;
					lXYZ /= mm;
					oXYZ.xyz_(lXYZ+iXYZ);
					/// na majd pattanjon le
				}
				else {
					// nagyon kicsit engedne
					mmABCD.B = 0;
				}
			}

			if( mmABCD.B ) {
				oCTRL.z |= 1;
				ab = float(mmABCD.B)/float(mmABCD.A);
			}
			else {
				oXYZ.xyz_(iXYZ);
				dXYZ.null();
				ab = 0.0;
			}
		}
	}

	if( bNEW ) {

	}
	else if( txyz.qlen_xyz() ) {
		dxyz = txyz - okxyz;

		mmABCD.C = dxyz.abs0().mx().x;
		if( mmABCD.B | (mmABCD.C>10) ) {
			tMX = 1.0;
			tMX.z.xyz_( txyz-tXYZ );
			tMX.z /= sqrt(tMX.z.qlen_xyz());	// normalizál

			switch( jdALF ) {
				case gpeALF_BRIDGE:
				case gpeALF_SNAIL:
					tMX.x = jd0mxTL.y.X3(tMX.z);
					tMX.y = jd0mxTL.z.X3(iMX.x);
					break;
				default:
					tMX.x = iMX.y.X3(tMX.z);
					tMX.y = tMX.z.X3(iMX.x);
					break;
			}

			float	xl = tMX.x.qlen_xyz(),
					yl = tMX.y.qlen_xyz();
			if( xl > yl ) {
				tMX.x /= sqrt(xl);
				tMX.y = tMX.z.X3(tMX.x);
			} else {
				tMX.y /= sqrt(yl);
				tMX.x = tMX.y.X3(tMX.z);
			}

			tABC.ABC_( tMX.eulABC()*degX(180.0/PI) );
			itABCdif = iABC.mmABC( tABC, degX(180.0/PI), degX(180.0/PI) );
		}
	}

	if( bNEW ) {
		dXYZ = tXYZ - iXYZ;
		lim = dXYZ.abs0().mx().x;
		lXYZ = oXYZ - iXYZ;
		mmABCD.B = lXYZ.abs0().mx().x;
		if( mmABCD.B >= (mmX(1)/16) )
			oCTRL.z |= 1;
		else {
			oXYZ.xyz_(iXYZ);
			dXYZ.null();
			ab = 0.0;
		}
		itABCdif = iABC.mmABC( oABC, degX(180.0/PI), degX(180.0/PI) );
		if( itABCdif.w >= (degX(1)/16) )
			oCTRL.z |= 2;
		else {
			tABC.ABC_(oABC.ABC_(iABC));
		}
	}
	else if( itABCdif.w < (degX(1)/2) ) {	// kb: 50
		tABC.ABC_(oABC.ABC_(iABC));
	}
	else {
		if( ab == 0.0 )
			lim = mmABCD.B;

		if( ab < 1.0 ) {
			if( mmABCD.A >= degX(2) ) {
				mmABCD.A /= degX(1);
				if( lim <= mmABCD.B )
				{
					lim = (mmABCD.B/=degX(1));
				} else {
					mmABCD.B /= degX(1);
					lim = mmABCD.B+3;
				}
			}

			if( mmABCD.B )
				oABC.ABC_( ((itABCdif*mmABCD.B)/mmABCD.A)+iABC ); // volt mozgás
			else if( ab > 0.0 )
				oABC.ABC_( (F4(itABCdif)*ab)+iABC );
			else
				oABC.ABC_( itABCdif+iABC );


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
		}
		else {
			oABC.ABC_( tABC );
		}

		oCTRL.z |= 2;
	}

	if( jdPRGstp( mSEC ) )
		return *this;


	if( lim > mmABCD.B ) {
		// ketrec gátolta
        // o-kat berakjuk a t-be
        tXYZ.xyz_( oXYZ );
		tABC.ABC_( oABC );
		if(bSTDcout){gpdCOUT << "CAGE tXYZ: " << tXYZ.pSTR( gpsJDpub ) <<gpdENDL;}
	} else
		if(bSTDcout){gpdCOUT << "oXYZ: " << oXYZ.pSTR( gpsJDpub ) << "\r\ntXYZ: " << tXYZ.pSTR( gpsJDpub ) <<gpdENDL;}

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

gpcDrc& gpcDrc::judo_OHNEnew( gpcROB& iROB, U4 mSEC, U4 iD0 ) {
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
								if(bSTDcout){gpdCOUT << "9115tXYZ: " << tXYZ.pSTR( gpsJDpub ) << gpdENDL;}
							} else {
								tXYZ.xyz_( oXYZ.xyz_(iXYZ) );
								tABC.ABC_( oABC.ABC_(iABC) );
								if(bSTDcout){gpdCOUT << "Err tXYZ: " << tXYZ.pSTR( gpsJDpub ) << gpdENDL;}
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
					if(bSTDcout){gpdCOUT << "HS1ms-msSRT3=" << HS1ms << "-" << lagX << "=" << (I8)HS1ms-(I8)lagX << gpdENDL;}
					if( HS1ms < lagX )
					{
						HS1ms = 0;
						//oCTRL.z = 0;
						//JD.y = 0;
						if(bSTDcout){gpdCOUT << "\033[1;32mEXTRA GO! lag:" << lag << "ms\033[0m" << gpdENDL;}
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
		tABC.ABC_( oABC.ABC_(iABC) );
		if( txyz.qlen_xyz() )
			txyz.xyz_( oxyz.xyz_(ixyz) );
		JD.x = 1;
		return *this;
	}

	/// --------------------
	/// CTRL.z = 0;
	/// --------------------
	oCTRL.z = 0;
	if( sqrt(okXYZ.qlen_xyz()) > mmX(200) ) {
		// ezt kaptuk a robitol
		// ha netán akkor sem mozdul
		oXYZ.xyz_(iXYZ);
		oABC.ABC_(iABC);
	}

	F4x4 iMX, oMX, tMX = 1.0;
	iMX.mxABC(iABC, degX(180.0/PI) );
	// itt kell le ellenőrizni mondjuk az üTKöZéSre
	//
	float ab = 1.0, k;
	static const float K = (100.0*PI2);

	I4 lim = 0;
	I4x4 	itABCdif = iABC.mmABC( tABC, degX(180.0/PI), degX(180.0/PI) ),
			tmp, dxyz,
			dXYZ = tXYZ - iXYZ, lXYZ,
			dGRP = tGRP - iGRP, hdXYZ,
			aHDxyz[8];


	U4 nH = gpmN(gpaCAGEheadBALL);

	mmABCD.D = sqrt(dGRP.qlen_xyz());
	mmABCD.A = dXYZ.abs0().mx().x;

	if( mmABCD.A ) {
		U4 i = 3; //iD0;
		switch( NMnDIF.x )
		{
			case gpeZS_BILL: {
					i = 0;
					gpcDrc* pJ = this+1;
					I4x4	j2b = (pJ->iXYZ+pJ->tXYZ)/2,
							*pTOOL = pBALLtool( i );
					*pTOOL = (j2b&I4x4(-1,-1,1,0)) + I4x4( mmX(1500), 0, 0, mmX(300) );
				} break;
			case gpeZS_JOHN: {
					i = 1;
					gpcDrc* pB = this-1;
					I4x4 	b2j = (pB->iXYZ+pB->tXYZ)/2,
							*pTOOL = pBALLtool( i );
					*pTOOL = (b2j&I4x4(-1,-1,1,0)) + I4x4( mmX(1500), 0, 0, mmX(300) );
				} break;
			default:
				i = 3;
				break;
		}


		/// megszüntettem a dadogást
		/// a nagybetűs JUDO-ban benne van ha kell
		lim = mmABCD.A;
		oXYZ.xyz_( cageXYZ(lim,i,gpaCAGEheadBALL[0].w) );
		lXYZ = oXYZ - iXYZ;
		mmABCD.B = lXYZ.abs0().mx().x;
		if( mmABCD.A < (mmX(1)/0x10) ) {
			mmABCD.B =
			mmABCD.x = 0;
		}
		else if( mmABCD.B ) {
			if( lim <= mmABCD.B+mmX(1) ) {
				// elérte a lim-et azaz nem érte el a ketrecet
				lim = mmABCD.B;
			}
			else if( mmABCD.B > mmX(10) ) {
				// kisebb let mint amit elvártunk a ketrec nem engedte
				I4 mm = mmABCD.B/mmX(1);
				lXYZ *= mm-10;
				lXYZ /= mm;
				oXYZ.xyz_(lXYZ+iXYZ);
				/// na majd pattanjon le
			}
			else {
				// nagyon kicsit engedne
				mmABCD.B = 0;
			}
		}

		if( mmABCD.B ) {
			oCTRL.z |= 1;
			ab = float(mmABCD.B)/float(mmABCD.A);
		}
		else {
			oXYZ.xyz_(iXYZ);
			dXYZ.null();
			ab = 0.0;
		}

	}

	if( txyz.qlen_xyz() ) {
		dxyz = txyz - okxyz;

		mmABCD.C = dxyz.abs0().mx().x;
		if( mmABCD.B | (mmABCD.C>10) ) {
			tMX = 1.0;
			tMX.z.xyz_( txyz-tXYZ );
			tMX.z /= sqrt(tMX.z.qlen_xyz());	// normalizál

			switch( jdALF ) {
				case gpeALF_BRIDGE:
				case gpeALF_SNAIL:
					tMX.x = jd0mxTL.y.X3(tMX.z);
					tMX.y = jd0mxTL.z.X3(iMX.x);
					break;
				default:
					tMX.x = iMX.y.X3(tMX.z);
					tMX.y = tMX.z.X3(iMX.x);
					break;
			}

			float	xl = tMX.x.qlen_xyz(),
					yl = tMX.y.qlen_xyz();
			if( xl > yl ) {
				tMX.x /= sqrt(xl);
				tMX.y = tMX.z.X3(tMX.x);
			} else {
				tMX.y /= sqrt(yl);
				tMX.x = tMX.y.X3(tMX.z);
			}

			tABC.ABC_( tMX.eulABC()*degX(180.0/PI) );
			itABCdif = iABC.mmABC( tABC, degX(180.0/PI), degX(180.0/PI) );
		}
	}

	if( itABCdif.w < (degX(1)/2) ) {	// kb: 50
		tABC.ABC_(oABC.ABC_(iABC));
	}
	else {
		if( ab == 0.0 )
			lim = mmABCD.B;

		if( ab < 1.0 ) {
			if( mmABCD.A >= degX(2) ) {
				mmABCD.A /= degX(1);
				if( lim <= mmABCD.B )
				{
					lim = (mmABCD.B/=degX(1));
				} else {
					mmABCD.B /= degX(1);
					lim = mmABCD.B+3;
				}
			}

			if( mmABCD.B )
				oABC.ABC_( ((itABCdif*mmABCD.B)/mmABCD.A)+iABC ); // volt mozgás
			else if( ab > 0.0 )
				oABC.ABC_( (F4(itABCdif)*ab)+iABC );
			else
				oABC.ABC_( itABCdif+iABC );


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
		}
		else {
			oABC.ABC_( tABC );
		}

		oCTRL.z |= 2;
	}

	if( jdPRGstp( mSEC ) )
		return *this;


	if( lim > mmABCD.B ) {
		// ketrec gátolta
        // o-kat berakjuk a t-be
        tXYZ.xyz_( oXYZ );
		tABC.ABC_( oABC );
		if(bSTDcout){gpdCOUT << "CAGE tXYZ: " << tXYZ.pSTR( gpsJDpub ) <<gpdENDL;}
	} else
		if(bSTDcout){gpdCOUT << "oXYZ: " << oXYZ.pSTR( gpsJDpub ) << "\r\ntXYZ: " << tXYZ.pSTR( gpsJDpub ) <<gpdENDL;}

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
gpcDrc& gpcDrc::JUDO( gpcROB& iROB, U4 mSEC ) {
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
								if(bSTDcout){gpdCOUT << "9115tXYZ: " << tXYZ.pSTR( gpsJDpub ) << gpdENDL;}
							} else {
								tXYZ.xyz_( oXYZ.xyz_(iXYZ) );
								tABC.ABC_( oABC.ABC_(iABC) );
								if(bSTDcout){gpdCOUT << "Err tXYZ: " << tXYZ.pSTR( gpsJDpub ) << gpdENDL;}
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
					if(bSTDcout){gpdCOUT << "HS1ms-msSRT3=" << HS1ms << "-" << lagX << "=" << (I8)HS1ms-(I8)lagX << gpdENDL;}
					if( HS1ms < lagX )
					{
						HS1ms = 0;
						//oCTRL.z = 0;
						//JD.y = 0;
						if(bSTDcout){gpdCOUT << "\033[1;32mEXTRA GO! lag:" << lag << "ms\033[0m" << gpdENDL;}
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
		tABC.ABC_( oABC.ABC_(iABC) );
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
		oABC.ABC_(iABC);
	}

	F4x4 tMX = 1.0, iMX;
	iMX.mxABC( iABC, degX(180.0/PI) );
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

		oXYZ.xyz_( cageXYZ( lim, i, gpaCAGEheadBALL[0].w ) );

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
		if(bSTDcout){gpdCOUT << "CAGE tXYZ: " << tXYZ.pSTR( gpsJDpub ) <<gpdENDL;}
	} else
		if(bSTDcout){gpdCOUT << "oXYZ: " << oXYZ.pSTR( gpsJDpub ) << "\r\ntXYZ: " << tXYZ.pSTR( gpsJDpub ) <<gpdENDL;}

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


