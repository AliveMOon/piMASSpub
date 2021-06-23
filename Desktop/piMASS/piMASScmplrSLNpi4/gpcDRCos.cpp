#include "gpcgt.h"
#include "gpcwin.h"
extern U1 gpaALFsub[];
extern char gpsTAB[], *gppTAB;
///------------------------
///
/// CAGE BILL
///
///------------------------
//------------------------
// BALL
//------------------------
I4x4 gpaCAGEbillBALL[] = {
	{ mmX(1500), 0, mmX(320), mmX(350) },						// jOHN
	{ 0, 0, mmX(320), mmX(420) }, { 0, 0, mmX(-300), mmX(550) },
	{ mmX(685), mmX(-469), mmX(366),  mmX(300) },
};
//------------------------
// BOX
//------------------------
I4x4 gpaCAGEbillBOX[] = {
	{ mmX(600), mmX(500), mmX(-350), mmX(330) }, // asztal_bill
	{ mmX(1500/2), mmX(5100), mmX(0), mmX(4000) }, // MIMI2bill
	{ mmX(-1000-2000), mmX(0), mmX(0), mmX(2000) }, // fal_bill
};
U4	gpnCAGEbillBALL = gpmN(gpaCAGEbillBALL),
	gpnCAGEbillBOX = gpmN(gpaCAGEbillBOX);

///------------------------
///
/// CAGE JOHN
///
///------------------------
//------------------------
// BALL
//------------------------
I4x4 gpaCAGEjohnBALL[] = {
	{ mmX(1500), 0, mmX(320), mmX(350) },						// bILL
	{ 0, 0, mmX(320), mmX(300) }, { 0, 0, mmX(-300), mmX(350) },
	{ mmX(685), mmX(-469), mmX(366),  mmX(300) },
};
//------------------------
// BOX
//------------------------
I4x4 gpaCAGEjohnBOX[] = {
	{ mmX(1500-600), mmX(-500), mmX(-250), mmX(330) }, // asztal_john
	{ mmX(-500-2000), mmX(0), mmX(0), mmX(2000) }, // MIMI_john
	{ mmX(1500/2), mmX(-5100), mmX(0), mmX(4000) }, // MIMI2john
};

U4	gpnCAGEjohnBALL = gpmN(gpaCAGEjohnBALL),
	gpnCAGEjohnBOX = gpmN(gpaCAGEjohnBOX);

I4x4* gpapBOX[] = {
	gpaCAGEbillBOX,
	gpaCAGEjohnBOX,
};
I4x4* gpapBALL[] = {
	gpaCAGEbillBALL,
	gpaCAGEjohnBALL,
};
U4 gpanBOX[] = {
	gpnCAGEbillBOX,
	gpnCAGEjohnBOX
};
U4 gpanBALL[] = {
	gpnCAGEbillBALL,
	gpnCAGEjohnBALL
};
I4x4* gpcDrc::pBALLtool( U4 i ) {
	return gpapBALL[i]+3;
}
I4x4 gpcDrc::cageXYZ( I4x4 trg, I4 lim, I4x4* pBOX, U4 nBOX, I4x4* pBALL, U4 nBALL )
{
	if( !this )
		return I4x2(mmX(500)).xxxy();

	if( iXYZ.qlen_xyz() < mmX(250) )
	if( okXYZ.qlen_xyz() > mmX(250) )
	{
		iXYZ.xyz_( okXYZ );
	}

	I4x4 tmp = cageBALL( trg.xyz0(), pBALL, nBALL );
	tmp = cageBOX( tmp, pBOX, nBOX );
	if( lim )
		return iXYZ.lim_mx(tmp,lim);

	return tmp;
}
I4x4 gpcDrc::cageXYZ( I4 lim, U4 id )
{
	if( id > gpmN(gpanBALL) )
	{
		if( iXYZ.qlen_xyz() < mmX(250) )
		if( okXYZ.qlen_xyz() > mmX(250) )
		{
			iXYZ.xyz_( okXYZ );
		}
		return iXYZ;
	}

	return cageXYZ( tXYZ, lim, gpapBOX[id], gpanBOX[id], gpapBALL[id], gpanBALL[id] );
}
I4x4 gpcDrc::cageXYZ( I4x4 trg, I4 lim, U4 id )
{
	if( id > gpmN(gpanBALL) )
	{
		if( iXYZ.qlen_xyz() < mmX(250) )
		if( okXYZ.qlen_xyz() > mmX(250) )
		{
			iXYZ.xyz_( okXYZ );
		}
		return iXYZ;
	}

	return cageXYZ( trg, lim, gpapBOX[id], gpanBOX[id], gpapBALL[id], gpanBALL[id] );
}

static char gpsJDprgPUB[0x100];
bool gpcDrc::jdPRGstp( U4 mSEC ) {
	// ha létre jött mozgá hagyja végre hajtani
	if( oCTRL.z )
		return false;

	// ha megált  megnézi
	// program végrehajtásával foglalkozik?
	if( jdPRG.x )
	{
		// igen
		if( jdPRG.y >= jdPRG.z )
		{
			/// END --------------------------
			// de pont befejezte
			jdPRG.null();
			MPosS = 0;
			if( jd0XYZ.qlen_xyz() )
			switch( jdALF )
			{
				case gpeALF_null:
				case gpeALF_DROP:
				case gpeALF_CALIB:
					break;
				default:
					tXYZ.xyz_(jd0XYZ);
					tABC.ABC_(jd0ABC);
					break;
			}

			return true;
		}
	} else
		return true;	// nem akor pihi

	// na nézzük a programot
	if( !jdPRG.y )
	{
		/// START ----------------------------
		if( !txyz.qlen_xyz() )
		{
			// ha nincsen target kinyirjuk a programot
			jdPRG.null();
			return true;
		}
		// jd0PRG -> ban vannak a felhasználó számai
		switch( jdALF )
		{
			case gpeALF_SHLD:
			case gpeALF_SNAIL:
					jdPRG.z = (jdPRG.w = jd0PRG.x) * jd0PRG.y;
					break;

			case gpeALF_DROP:if( jd1XYZ.qlen_xyz() * gpmMAX( 0, jd0PRG.y-jd0PRG.x) ) {
					//I4 lag = mSEC < msSMR2.w ? 0 : mSEC-msSMR2.w;
					//if(bSTDcout){gpdCOUT << "lag: " << lag << gpdENDL;}
					jdPRG.y = jdPRG.w = msSRT3.x;				// w-ben örizzük az indulási időt y aktuális idő
					jdPRG.z = jdPRG.w+jd0PRG.a4x2[0].sum();		// z-ben pedig a kívánt megérkezési időt
					break;
				}
			case gpeALF_CALIB:
			default:
				jdPRG.null();
				return true;
				break;
		}

		if( jdPRG.z ? !okXYZ.abs0().mx().x : true )
		{
			jdPRG.null();
			return true;
		}
		jd0XYZ.xyz_(okXYZ);
		jd0ABC.ABC_(okABC);
		// itt a txyz azért van a füg.ben txyz-jd0XYZ hossza a sugarat adja meg
		jd0xyz.xyz_(jd0XYZ.ABC2xyz( txyz, jd0ABC ));	// az okxyz nem jó mert ha nem történt mozgás nincsen benne semmi
		jd0mx.mxABC(jd0ABC,degX(180.0/PI));


		jd1ABC.ABC_(tABC);
		jd1up.xyz_(tXYZ-txyz);
	}
	I4 zl = sqrt((jd0XYZ-jd0xyz).qlen_xyz());
	I4x2 xy = jdPRG.y;
	F4 cr; float d;
	I4x4 vec;
	switch( jdALF )
	{
		case gpeALF_SHLD: {
				// jdPRG.y=0 x=0 y=0
				(xy = jdPRG.y).XdivRQ(jdPRG.w) += jdPRG.w;
				xy -= jd0PRG.a4x2[1];
				xy %= jdPRG.w;
				cr.gr2cr( xy, jdPRG.w );
				d = cr.w/zl;
				vec = ((jd0mx.x*(cr.x/d)) + (jd0mx.y*(cr.y/d)) + (jd0mx.z*(cr.z/d)));
				tXYZ.xyz_( jd0xyz - vec );
			} break;

		case gpeALF_SNAIL: {
				vec.z = (jdPRG.w-1);
				vec.a4x2[0].snail(vec.z*vec.z);
				xy.snail( jdPRG.y ) -= vec.a4x2[0];
				xy += jdPRG.w;
				xy -= jd0PRG.a4x2[1];
				xy %= jdPRG.w; 							// %10 0->9
				cr.gr2cr( xy, jdPRG.w );
				d = cr.w/zl;
				vec = ((jd0mx.x*(cr.x/d)) + (jd0mx.y*(cr.y/d)) + (jd0mx.z*(cr.z/d)));
				tXYZ.xyz_( jd0xyz - vec );
			} break;
		case gpeALF_CALIB:
		case gpeALF_DROP: {
				// I4 lag = mSEC < msSMR2.w ? 0 : mSEC-msSMR2.w;
				// if(bSTDcout){gpdCOUT << "lag: " << lag << gpdENDL;}
				/// msSRT3.x robot ms // AVGms átlagos válasz idő
				/// jd0PRG.x késleltetés
				I8	tn = jdPRG.z-jdPRG.w,
					preY = jdPRG.y,
					preti = preY-jdPRG.w;

				jdPRG.y = msSRT3.x + 5*ms2sec - jd0PRG.x;
				I8 ti = jdPRG.y-jdPRG.w;

				I4x4	up0 = (jd0XYZ-jd0xyz).xyz0(),
						dti = jd0XYZ.drop( jd1XYZ, up0, jd1XYZ.w, ti, tn );
				if( preti<0 )
					preti=0;
				if( ti>tn )
					ti=tn;

				if( (preti>=0) && (ti<=tn) )
				{
					double	radA = jd0XYZ.dropRAD( jd1XYZ, up0, jd1XYZ.w, 0, tn ),
							radB = jd0XYZ.dropRAD( jd1XYZ, up0, jd1XYZ.w, tn, tn ),
							piAB = radB-radA,
							piA = radA > 0.0 ? PIp2-radA : PIp2,
							th = double(tn)*piA/piAB;
					if( ti > th ) {
						if( preti < th ) {
							ti = th;
							jdPRG.y = ti+jdPRG.w;
							dti = jd0XYZ.drop( jd1XYZ, up0, jd1XYZ.w, ti, tn );
							if(bSTDcout){gpdCOUT << "HI ";}
						}
						//I4x4 up1 = (jd1XYZ-jd1xyz).xyz0();
						up0 += ((jd1up-up0)*(I8)(ti-th))/(I8)(tn-th);
					}
				}
				MPosS = jdPRG.y-preY;
				HS1ms = msSRT3.x + (MPosS/2);
				if( HS1ms-msSRT3.x > ms2sec )
					HS1ms -= ms2sec;
				tXYZ.xyz_( dti );
				txyz.xyz_( tXYZ-up0 );

				if(bSTDcout){gpdCOUT << ti << "/" << tn << "\t" << tXYZ.pSTR( gpsJDprgPUB ) <<gpdENDL;}
			} break;
		default:
			jdPRG.null();
			return true;
			break;
	}
	jdPRG.y++;
	return true;
}
gpcLZY* gpcGT::GTdrcOSrob( gpcLZY* pANS, U1* pSTR, gpcMASS& mass, SOCKET sockUSR, U4 mSEC ) {
	U8 s = -1, nLEN;
	U4 n = gpmSTRLEN( pSTR );
	if( this ? !n : true )
		return pANS->lzyFRMT( s = -1, "nonsens" );

	gpcROBnD	*pROBnD = gpmLZYvali( gpcROBnD, mass.GTlzyALL.LZY( gpdGTlzyIDinp(TnID) ) );
	gpcLZY		*pLZYout = NULL,
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


	if( !pROBnD )
		return pANS->lzyFRMT( s = -1, "nonsens" );

	gpcROBnD& RnD =	*pROBnD;
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
			switch( an.alf ) {
				case gpeALF_BILL:
						iD = 0;
						if( RnD.aDrc[iD].NMnDIF.au4x2[0].x == gpeZS_BILL )
							break;
						RnD.aDrc[iD].format( gpeZS_BILL );
						break;
				case gpeALF_JOHN:
						iD = 1;
						if( RnD.aDrc[iD].NMnDIF.au4x2[0].x == gpeZS_JOHN )
							break;
						RnD.aDrc[iD].format( gpeZS_JOHN );
					 break;
				case gpeALF_OK:{
						if(iD >= nD )
						{
							iNUM = gpeDRCos_NONS;
							break;
						}
						RnD.aDrc[iD].okXYZ.xyz_( RnD.aDrc[iD].tXYZ );
						RnD.aDrc[iD].okABC.ABC_( RnD.aDrc[iD].tABC );
						RnD.aDrc[iD].okxyz.xyz_( RnD.aDrc[iD].txyz );
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
								if( RnD.aDrc[iD].okXYZ.qlen_xyz() )
									RnD.aDrc[iD].tXYZ.xyz_( RnD.aDrc[iD].okXYZ );
								else
									RnD.aDrc[iD].tXYZ.xyz_( RnD.aDrc[iD].iXYZ );
								break;
							default:
								iNUM = gpeDRCos_posx;
								RnD.aDrc[iD].txyz.xyz_( RnD.aDrc[iD].ixyz );
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
								if( RnD.aDrc[iD].okXYZ.qlen_xyz() )
									RnD.aDrc[iD].tABC.ABC_( RnD.aDrc[iD].okABC );
								else
									RnD.aDrc[iD].tabc.xyz_( RnD.aDrc[iD].iABC );
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

					pANS = RnD.aDrc[iD].answSTAT( pANS, iD );
					continue;
				case gpeALF_INFO:
					if(iD >= nD )
					{
						iNUM = gpeDRCos_NONS;
						break;
					}

					pANS = RnD.aDrc[iD].answINFO( pANS, iD );
					continue;
				case gpeALF_INFOX:
					if(iD >= nD )
					{
						iNUM = gpeDRCos_NONS;
						break;
					}

					pANS = RnD.aDrc[iD].answINFOX( pANS, iD, 100 );
					continue;
				case gpeALF_STOP:
				default:
					break;
			}

			if( oD != iD )
			if( pD )
			{
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
				if( !RnD.aDrc[iD].NMnDIF.au4x2[0].x )
					continue;
				comA = RnD.aDrc[iD].NMnDIF.au4x2[0].x;
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

		pD = RnD.aDrc+iD;
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
				if( pD->jdPRG.x )
				{
					U4 i = (iNUM-gpeDRCos_posx)%nNUM;
					pD->jd1up.aXYZW[i] = pD->jd1XYZ.aXYZW[i] - ((d8 == 0.0) ? (I4)an.num*mmX(1) : (I4)(d8*mmX(1)));
					break;
				}
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
			case gpeDRCos_drpUP:
				pD->jd1up.xyz_( pD->tXYZ-pD->txyz );
			case gpeDRCos_drpX:
			case gpeDRCos_drpY:
			case gpeDRCos_drpZ:
			///case gpeDRCos_drpUP: valójában a 4. para
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

