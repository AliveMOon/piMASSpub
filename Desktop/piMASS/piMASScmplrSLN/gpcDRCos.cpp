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
	/// jOHN TOOL
	{ mmX(685), mmX(-469), mmX(366),  mmX(300) },
	/// bILL bILL column
	{ 0, 0, mmX(-300), mmX(300) }, { 0, 0, 0, mmX(300) }, { 0, 0, mmX(300), mmX(300) }, { 0, 0, mmX(600), mmX(300) },
	/// jOHN column
	{ mmX(1500), 0, mmX(-300), mmX(300) }, { mmX(1500), 0, 0, mmX(300) },
	{ mmX(1500), 0, mmX(300), mmX(300) }, { mmX(1500), 0, mmX(600), mmX(300) },
};
//------------------------
// BOX
//------------------------
I4x4 gpaCAGEbillBOX[] = {
	{ mmX(750), mmX(225-900), mmX(16-900), mmX(900) },		// TABLE_bill
	{ mmX(750), mmX(1750), mmX(-210), mmX(750) },			// magazin__bill
	{ mmX(-(1300+500)), mmX(-150), mmX(-210), mmX(1300) },  // WALLx__bill
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
	/// bILL TOOL
	{ mmX(685), mmX(-469), mmX(366),  mmX(300) },
	/// jOHN jOHN column
	{ 0, 0, mmX(-300), mmX(300) }, { 0, 0, 0, mmX(300) }, { 0, 0, mmX(300), mmX(300) }, { 0, 0, mmX(600), mmX(300) },
	/// bILL column
	{ mmX(1500), 0, mmX(-300), mmX(300) }, { mmX(1500), 0, 0, mmX(300) },
	{ mmX(1500), 0, mmX(300), mmX(300) }, { mmX(1500), 0, mmX(600), mmX(300) },
};
//------------------------
// BOX
//------------------------
I4x4 gpaCAGEjohnBOX[] = {
	{ mmX(750), mmX(900-225), mmX(16-900), mmX(900) }, 		// TABLE__john
	{ mmX(750), mmX(-1750), mmX(-210), mmX(750) }, 			// magazin__john
	{ mmX(-(1300+500)), mmX(150), mmX(-210), mmX(1300) }, 	// WALLx__john
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
	return gpapBALL[i];
}

/// HD ------------------------------
I4x4 gpcDrc::cageBALLts( I4x4 T, I4x4 S, I4x4* pCAGE, U4 n, int rR ) {
	I4x4 a, b;
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
	D8 *= sqrt(dd) - mmX(1);
	D8 /= sqrt(dd0);
	return S+D8;
}
I4x4 gpcDrc::cageBOXts( I4x4 T, I4x4 S, I4x4* pCAGE, U4 n, int rR ) {
	I4x4 a, b;
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
	D8 *= sqrt(dd) - mmX(1);
	D8 /= sqrt(dd0);
	return S+D8;
}
I4x4 gpcDrc::cageXYZhd0( I4x4* pHD, I4 lim, U4 id, int rR ) {
	if(id>gpmN(gpanBALL)) {
		if( pHD[0].qlen_xyz() < mmX(250) )
		if( pHD[1].qlen_xyz() > mmX(250) )
			pHD[0].xyz_( okXYZ );
		return iXYZ;
	}
	return cageXYZhd( pHD, lim, gpapBOX[id], gpanBOX[id], gpapBALL[id], gpanBALL[id], rR );
}
I4x4 gpcDrc::cageXYZhd( I4x4* pHD, I4 lim, I4x4* pBOX, U4 nBOX, I4x4* pBALL, U4 nBALL, int rR ) {
	if( !this )
		return I4x2(mmX(500)).xxxy();

	if( pHD[0].qlen_xyz() < mmX(250) )
	if( pHD[1].qlen_xyz() > mmX(250) )
		pHD[0].xyz_( okXYZ );


	I4x4 tmp = cageBALLts( pHD[1].xyz0(), pHD[0].xyz0(), pBALL, nBALL, rR );
	tmp = cageBOXts( tmp, pHD[0].xyz0(), pBOX, nBOX, rR );
	if( lim )
		return pHD[0].lim_mx(tmp,lim);

	return tmp;
}
/// .... ...  ..   .


I4x4 gpcDrc::cageXYZ( I4x4 trg, I4 lim, I4x4* pBOX, U4 nBOX, I4x4* pBALL, U4 nBALL, int rR ) {
	if( !this )
		return I4x2(mmX(500)).xxxy();

	if( iXYZ.qlen_xyz() < mmX(250) )
	if( okXYZ.qlen_xyz() > mmX(250) ) {
		iXYZ.xyz_( okXYZ );
	}

	I4x4 tmp = cageBALL( trg.xyz0(), pBALL, nBALL, rR );
	tmp = cageBOX( tmp, pBOX, nBOX, rR );
	if( lim )
		return iXYZ.lim_mx(tmp,lim);

	return tmp;
}
I4x4 gpcDrc::cageXYZ( I4 lim, U4 id, int rR  ) {
	if( id > gpmN(gpanBALL) ) {
		if( iXYZ.qlen_xyz() < mmX(250) )
		if( okXYZ.qlen_xyz() > mmX(250) )
		{
			iXYZ.xyz_( okXYZ );
		}
		return iXYZ;
	}

	return cageXYZ( tXYZ, lim, gpapBOX[id], gpanBOX[id], gpapBALL[id], gpanBALL[id], rR );
}
I4x4 gpcDrc::cageXYZ( I4x4 trg, I4 lim, U4 id, int rR ) {
	if( id > gpmN(gpanBALL) )
	{
		if( iXYZ.qlen_xyz() < mmX(250) )
		if( okXYZ.qlen_xyz() > mmX(250) )
		{
			iXYZ.xyz_( okXYZ );
		}
		return iXYZ;
	}

	return cageXYZ( trg, lim, gpapBOX[id], gpanBOX[id], gpapBALL[id], gpanBALL[id], rR );
}

static char gpsJDprgPUB[0x100];
bool gpcDrc::jdPRGstp( U4 mSEC ) {
	// ha létre jött mozgá hagyja végre hajtani
	if( oCTRL.z )
		return false;

	// ha megált  megnézi
	// program végrehajtásával foglalkozik?
	if( jdPRG.x ) {
		// igen
		if( jdPRG.y >= jdPRG.z ) {
			/// END --------------------------
			// de pont befejezte
			if( jd0XYZ.qlen_xyz() )
			switch( jdALF ) {
				case gpeALF_null:
				case gpeALF_DROP:
				case gpeALF_CALIB:
					break;
				default:
					tXYZ.xyz_(jd0XYZ);
					tABC.ABC_(jd0ABC);
					break;
			}
			jdALF = gpeALF_null;
			jdPRG.null();
			MPosS = 0;
			return true;
		}
	} else
		return true;	// nem akor pihi

	// na nézzük a programot
	if( !jdPRG.y ) {
		/// START ----------------------------
		if( !txyz.qlen_xyz() ) {
			// ha nincsen target kinyirjuk a programot
			jdPRG.null();
			jdPRGtool.null();
			return true;
		}
		// jd0PRG -> ban vannak a felhasználó számai
		switch( jdALF ) {
			case gpeALF_SHLD:
					jdPRG.w = jd0PRG.x;
					jdPRG.z = jd0PRG.a4x2[0].area(); // (jdPRG.w=jd0PRG.x)*jd0PRG.y;
					break;
			case gpeALF_BRIDGE:						/// slmp0x16 john snail 32 4
			case gpeALF_SNAIL:						/// slmp0x16 john snail 32 4
					jdPRG.w = jd0PRG.x;
					jdPRG.z = jd0PRG.y*jd0PRG.y; // jd0PRG.a4x2[1].area(); // (jdPRG.w=jd0PRG.x)*jd0PRG.y;
					jdPRGtool.a4x2[0] = jd0PRG.a4x2[1];
					break;
			case gpeALF_DROP: if( jd1XYZ.qlen_xyz()*gpmMAX(0,jd0PRG.y-jd0PRG.x) ) {
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

		if( jdPRG.z ? !okXYZ.abs0().mx().x : true ) {
			jdPRG.null();
			return true;
		}
		jd0XYZ.xyz_(okXYZ);
		jd0ABC.ABC_(okABC);
		// itt a txyz azért van a füg.ben txyz-jd0XYZ hossza a sugarat adja meg
		jd0xyz.xyz_(jd0XYZ.ABC2xyz( txyz, jd0ABC ));	// az okxyz nem jó mert ha nem történt mozgás nincsen benne semmi
		jd0mx.mxABC(jd0ABC,degX(180.0/PI));
		jd0mxTL = jd0mx;

		jd1ABC.ABC_(tABC);
		jd1up.xyz_(tXYZ-txyz);
	}
	I4 zl = sqrt((jd0XYZ-jd0xyz).qlen_xyz());
	I4x2 xy = jdPRG.y;
	F4x4 mxTMP = jd0mxTL;
	F4 cr; float d;
	I4x4 aVEC[2];
	switch( jdALF ) {
		/*case gpeALF_SHLD: {
				// jdPRG.y=0 x=0 y=0
				(xy = jdPRG.y).XdivRQ(jdPRG.w) += jdPRG.w;
				xy -= jd0PRG.a4x2[1];
				xy %= jdPRG.w;
				cr.gr2core( xy, jdPRG.w );
				d = cr.w/zl;
				aVEC[0] = ((jd0mx.x*(cr.x/d)) + (jd0mx.y*(cr.y/d)) + (jd0mx.z*(cr.z/d)));
				tXYZ.xyz_( jd0xyz - aVEC[0] );
			} break;*/
		case gpeALF_SHLD:
		case gpeALF_SNAIL: { 							/// jdPRG.x ACT, //.y iCNT //.z nEND //.w width
				if( jdALF == gpeALF_SNAIL )
					xy.snail( jdPRG.y );
				else
					xy.zigzag( jdPRG.y, sqrt(jdPRG.z) );
				xy += jdPRG.w/2;
				xy %= jdPRG.w;
				cr.gr2core( xy, jdPRG.w );

				d = cr.w/zl;
				if( jdPRGtool.a4x2[0].abs().sum() > 0 ) {
					if( jdPRGtool.x ) {
						mxTMP.z = (jd0mx.z.N3());
						float a = PI * float(jdPRGtool.x)/degX(180.0);
						mxTMP.x = jd0mx.x.N3()*cos(a) + jd0mx.y.N3()*sin(a);
						mxTMP.y = mxTMP.z.X3(mxTMP.x).N3();
					}
					aVEC[0] = ((mxTMP.x*(cr.x/d)) + (mxTMP.y*(cr.y/d)) + (mxTMP.z*(cr.z/d)));
					if( jdPRGtool.y )
						aVEC[0] += mxTMP.x*float(jdPRGtool.y);
				} else
					aVEC[0] = ((mxTMP.x*(cr.x/d)) + (mxTMP.y*(cr.y/d)) + (mxTMP.z*(cr.z/d)));
				tXYZ.xyz_( jd0xyz - aVEC[0] );
			} break;
		case gpeALF_BRIDGE: { 							/// jdPRG.x ACT, //.y iCNT //.z nEND //.w width
				xy.zigzag( jdPRG.y, sqrt(jdPRG.z) );
				xy += jdPRG.w/2;
				xy %= jdPRG.w;
				cr.gr2cyli( xy, jdPRG.w );

				d = cr.w/zl;
				if( jdPRGtool.a4x2[0].abs().sum() > 0 ) {
					if( jdPRGtool.x ) {
						mxTMP.z = jd0mx.z.N3();
						float a = PI * float(jdPRGtool.x)/degX(180.0);
						mxTMP.x = jd0mx.x.N3()*cos(a) + jd0mx.y.N3()*sin(a);
						mxTMP.y = mxTMP.z.X3(mxTMP.x).N3();
					}
					aVEC[1] = aVEC[0] = (mxTMP.y*(cr.y/d));
					if( jdPRGtool.y )
						aVEC[1] += mxTMP.x*float(jdPRGtool.y);
					aVEC[1] += ((mxTMP.x*(cr.x/d)) + (mxTMP.z*(cr.z/d)));
				} else {
					aVEC[1] = aVEC[0] = (mxTMP.y*(cr.y/d));
					aVEC[1] += ((mxTMP.x*(cr.x/d)) + (mxTMP.z*(cr.z/d)));
				}
				tXYZ.xyz_( jd0xyz - aVEC[1] );
				txyz.xyz_( jd0xyz - aVEC[0] );
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
		if( SOCKET* pSOCK = gpmLZYvali( SOCKET, pLZYusr) ) {
			nSOCK = gpmLZYload(pLZYusr,sockUSR);
			for( iSOCK = 0; iSOCK < nSOCK; iSOCK++ ) {
				if( pSOCK[iSOCK] != sockUSR )
					continue;
				// nem új bent van a listában
				break;
			}
		}
		if( iSOCK >= nSOCK ) {
			// új felhasználó!
			pLZYusr->lzyADD( &sockUSR, sizeof(sockUSR), s = -1 );
			iSOCK = nSOCK;
			nSOCK = gpmLZYload(pLZYusr,sockUSR);
		}
	///-----------------------------

	if( !pROBnD )
		return pANS->lzyFRMT( s = -1, "nonsens" );

	gpcROBnD& RnD =	*pROBnD;
	U4	nD = 2,
		iD = nD, oD = 2;
	gpcDrc* pD = NULL;

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
		) {
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
						if(iD >= nD ) {
							iNUM = gpeDRCos_NONS;
							break;
						}
						RnD.aDrc[iD].okXYZ.xyz_( RnD.aDrc[iD].tXYZ );
						RnD.aDrc[iD].okABC.ABC_( RnD.aDrc[iD].tABC );
						RnD.aDrc[iD].okxyz.xyz_( RnD.aDrc[iD].txyz );
					} break;
				case gpeALF_POS:
				case gpeALF_XYZ:{
						if(iD >= nD ) {
							iNUM = gpeDRCos_NONS;
							break;
						}

						switch( comA&0xffffff ) {
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
						if(iD >= nD ) {
							iNUM = gpeDRCos_NONS;
							break;
						}

						switch( comA&0xffffff ) {
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
						if(iD >= nD ) {
							iNUM = gpeDRCos_NONS;
							break;
						}
						iNUM = gpeDRCos_GRPx;
						nNUM = 3;
					} break;

				case gpeALF_DROP: {
						if(iD >= nD ) {
							iNUM = gpeDRCos_NONS;
							break;
						}
						alf = an.alf;
						iNUM = gpeDRCos_drpX;
						nNUM = 4;
					} break;

				case gpeALF_SHLD:
				case gpeALF_SNAIL:
				case gpeALF_BRIDGE: {
						if(iD >= nD ) {
							iNUM = gpeDRCos_NONS;
							break;
						}
						alf = an.alf;
						iNUM = gpeDRCos_prgA;
						nNUM = 4;
					} break;


				case gpeALF_LINK:
				case gpeALF_AXIS:{
						if(iD >= nD ) {
							iNUM = gpeDRCos_NONS; break;
						}
						switch( comA ) {
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
					if(iD >= nD ) {
						iNUM = gpeDRCos_NONS;
						break;
					}

					pANS = RnD.aDrc[iD].answSTAT( pANS, iD, gpdHEADmmX );
					continue;
				case gpeALF_INFO:
					if(iD >= nD ) {
						iNUM = gpeDRCos_NONS;
						break;
					}

					pANS = RnD.aDrc[iD].answINFO( pANS, iD, gpdHEADmmX );
					continue;
				case gpeALF_INFOX:
					if(iD >= nD ) {
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
			if( pD ) {
				pANS = pD->answSTAT( pANS, iD, gpdHEADmmX );
			}
			pD = NULL;
			oD = iD;
			continue;
		} else if( iNUM > gpeDRCos_NONS )
			return pANS->lzyFRMT( s = -1, "nonsens" );

		if( iD >= nD ) {
			pANS = pANS->lzyFRMT( s = -1, "Who?" );
			for( iD = 0; iD < 2; iD++ ) {
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
		switch(iNUM) {

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
				switch( alf ) {
					case gpeALF_BRIDGE:
					case gpeALF_SNAIL:
						if( iNUM >= gpeDRCos_prgC ) {
							pD->jd0PRG.aXYZW[(iNUM-gpeDRCos_prgA)%nNUM] = (d8 == 0.0) ? (I4)an.num*mmX(1) : (I4)(d8*mmX(1));
							if( pD->jd0PRG.aXYZW[(iNUM-gpeDRCos_prgA)%nNUM] > mmX(200) )
								pD->jd0PRG.aXYZW[(iNUM-gpeDRCos_prgA)%nNUM] = mmX(200);
							else if( pD->jd0PRG.aXYZW[(iNUM-gpeDRCos_prgA)%nNUM] < mmX(-200) )
								pD->jd0PRG.aXYZW[(iNUM-gpeDRCos_prgA)%nNUM] = mmX(-200);
							break;
						}
					default:
						pD->jd0PRG.aXYZW[(iNUM-gpeDRCos_prgA)%nNUM] = (d8 == 0.0) ? (I4)an.num : (I4)d8;
						break;
				}
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
				if(iNUM<gpeDRCos_drpT) {
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
		return pD->answSTAT( pANS, iD, gpdHEADmmX );

	return pANS->lzyFRMT( s = -1, "nonsens" );
}

