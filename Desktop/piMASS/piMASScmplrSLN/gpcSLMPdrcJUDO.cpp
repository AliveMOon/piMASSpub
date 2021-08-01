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

//extern static char gpsJDpub[];
gpcDrc& gpcDrc::judo( gpcROB& iROB, U4 mSEC, U4 iD0, gpcGT* pGT, gpcROBnD *pROBnD ) {
	*this = iROB;	/// XYZABCxyz 1. ixyz = iXYZ.ABC2xyz( txyz, iABC );
	I4x4* pSTP;
	/// --------------------
	///			JD I/O
	/// --------------------
	switch( JD.y ) {
		case 4: { /// 4.HS2i? // HS2o elvesz VÉGE
				if( bHS2i() ) {
					/// PULLING még nem vette az adást
					oHS2o();
					oADRin = 0;
					JD.y = 4;
					return *this;
				}
				xHS2o();
				JD.y = 0;
				if( JD.z ) {
					JD.x = 0;
					return *this;
				}
				switch(JD.x) {
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
					switch(JD.z = iCTRL.z) {
						case 0:
							/// XYZABCxyz 2. OK ------------- JD.z = 0
							okXYZ.xyz_(oXYZ);
							okABC.ABC_(oABC);
							okxyz.xyz_(txyz);
							oADRin = 0;
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
				}
				else if( bHS1i() ? false : HS1ms ) {
				/// megnézzük holt tart a robot
					U4	lag = (mSEC-msSMR2.w),
						lagX = msSRT3.x+lag;
					if(bSTDcout){gpdCOUT << "HS1ms-msSRT3=" << HS1ms << "-" << lagX << "=" << (I8)HS1ms-(I8)lagX << gpdENDL;}
					if( HS1ms < lagX ) {
						HS1ms = 0;
						if(bSTDcout){gpdCOUT << "\033[1;32mEXTRA GO! lag:" << lag << "ms\033[0m" << gpdENDL;}
					}
				}
				/// PULLING most arra várunk megérkezzen a robot
				xHS1o();
				JD.y = 3;
			} return *this;
		case 2: { /// 2.HS1o elvesz
				if( oADRin.i < oADRin.n ) {
					if( !bHS1i() ) {
						/// ROBi HS1dw
						/// STEP
						oADRin.i++;
						JD.y = 1;
						/// PI HS1up
						oHS1o();
					} else {
						/// HOLD
						JD.y = 2;
						xHS1o();
					}
					pSTP = lstSTP.pI4x4( oADRin.i*3 );
					if( pSTP ) {
						oXYZ.xyz_( pSTP[0] );
						oABC.xyz_( pSTP[1] );
					}
					return *this;
				}
				/// PI HS1dw
				xHS1o();
				JD.y = 3;
			} return *this;
		case 1: { /// 1.HS1o kintvan // HS1i?
				/// 0 már benne van a aXYZ-ben
				if( !oADRin.n ) {
					if( bHS1i() ) {
						/// ROBi HS1up
						/// END
						xHS1o();
						JD.y = 3;
						return *this;
					}
				}
				else if( oADRin.i < oADRin.n ) {
					/// csomagolt küldés
					if( bHS1i() ) {
						/// ROBi HS1up
						/// STEP
						oADRin.i++;
						JD.y = 2;
						/// PI HS1dw
						xHS1o();
					} else {
						/// HOLD
						JD.y = 1;
						oHS1o();
					}
					pSTP = lstSTP.pI4x4( oADRin.i*3 );
					if( pSTP ) {
						oXYZ.xyz_( pSTP[0] );
						oABC.xyz_( pSTP[1] );
					}
					return *this;
				} else {
					if( bHS1i() ) {
						/// PI HS1dw
						xHS1o();
						JD.y = 3;
					} else {
						/// HOLD
						JD.y = 1;
						oHS1o();
					}
					return *this;
				}

				if( bHS2i() ) {
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
			if( bHS2i() ) {
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
	static const float K = (100.0*PI2);

	I4x4 	itABCdif = iABC.mmABC( tABC, degX(180.0/PI), degX(180.0/PI) ),
			tmp, dxyz = txyz - ixyz,
			cN,
			iiXYZ = iXYZ, iiABC = iABC, iixyz = ixyz,
			cXYZ, cABC, cxyz,
			hdXYZ,
			dXYZ = tXYZ - iXYZ, lXYZ,
			dGRP = tGRP - iGRP;

	mmABCD.D = sqrt(dGRP.qlen_xyz());
	mmABCD.A = dXYZ.abs0().mx().x;
	if( itABCdif.w > (degX(1)/16) )
		mmABCD.A += itABCdif.w;
	if( txyz.abs0().mx().x > (mmX(1)/16) )
		mmABCD.A += dxyz.abs0().mx().x;

	U4 i = 3, j;
	I4x2 tr = 0;
	if( mmABCD.A ) {
		switch( NMnDIF.x ) {
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
		/// --------------------------
		///		lstSTP TRG1
		/// --------------------------
		pSTP = lstSTP.pI4x4n( 0 );
		if( !oADRin.n ) {
			pSTP[0] = tXYZ;
			pSTP[1] = tABC;
			pSTP[2] = txyz;
		}

		for( j = 0; j <= oADRin.n; j++ ) {
			/// --------------------------
			///		lstSTP START
			/// --------------------------
			pSTP = lstSTP.pI4x4( j*3 );
			if( !pSTP ) {
				oADRin.n = j;
				break;
			}
			cXYZ.xyz_(pSTP[0]);
			cABC.ABC_(pSTP[1]);
			if( txyz.abs0().mx().x )
				cxyz.xyz_(pSTP[2]);
			else
				cxyz.null();

			cN.null();
			mmABCD = judoCAGEin(  cXYZ,  cABC,  cxyz,
								 iiXYZ, iiABC, iixyz,
								 cN, i, iMX );
			if( mmABCD.C )
				oCTRL.z |= 2;
			else if( mmABCD.A < (mmX(1)/16) )
				continue;

			if( mmABCD.B < (mmX(1)/16) ) {
				tXYZ = pSTP[0] = iiXYZ;
				tABC = pSTP[1] = iiABC;
				txyz = pSTP[2] = iixyz;
				if(bSTDcout){gpdCOUT << "CAGE tXYZ: " << tXYZ.pSTR( gpsJDpub ) <<gpdENDL;}
				continue;
			}
			oCTRL.z |= 1;

			iiXYZ = pSTP[0] = cXYZ;
			iiABC = pSTP[1] = cABC;
			iixyz = pSTP[2] = cxyz;
			tr.x += mmABCD.B;
			tr.y += mmABCD.C;

			if( (mmABCD.A-mmABCD.B) <= (mmX(1)/16) )
				continue;
			/// --------------------------
			///		lstSTP mandiner		+1
			/// --------------------------
			oADRin.n++;
			j++;

			pSTP = lstSTP.pI4x4nINS( j*3, 1*3 );
			iiXYZ = pSTP[0] = cXYZ;
			iiABC = pSTP[1] = cABC;
			iixyz = pSTP[2] = cxyz;

			cN = (cN*mmX(50))/cN.abs0().mx().x;
			cXYZ += cN;
			mmABCD = judoCAGEin(  cXYZ,  cABC,  cxyz,
								 iiXYZ, iiABC, iixyz,
								 cN, i, iMX );
			if( mmABCD.C )
				oCTRL.z |= 2;

			if( mmABCD.B < (mmX(1)/16) ) {
				tXYZ = pSTP[0] = iiXYZ;
				tABC = pSTP[1] = iiABC;
				txyz = pSTP[2] = iixyz;
				if(bSTDcout){gpdCOUT << "CAGE tXYZ: " << tXYZ.pSTR( gpsJDpub ) <<gpdENDL;}
				continue;
			}
			oCTRL.z |= 1;
			tr.x += mmABCD.B;
			tr.y += mmABCD.C;

			iiXYZ = pSTP[0] = cXYZ;
			iiABC = pSTP[1] = cABC;
			iixyz = pSTP[2] = cxyz;

		}
	}

	/// --------------------------
	///		OUT <- pSTP[0]
	/// --------------------------
	pSTP = lstSTP.pI4x4( 0 );
	if( oCTRL.z&1 )
		oXYZ.xyz_(pSTP[0]);
	else {
		oXYZ.xyz_(iXYZ);
		//ab = 0.0;
	}

	if( oCTRL.z&2 ) {
		oABC.ABC_(pSTP[1]);
		if( !txyz.abs0().mx().x )
			oxyz.xyz_(pSTP[2]);
		else
			oxyz.null();
	}
	else {
		tABC.ABC_(oABC.ABC_(iABC));
	}

	/// --------------------------
	///			jdPRG (STEP)
	/// --------------------------
	if( jdPRGstp( mSEC, pGT, pROBnD ) )
		return *this;

	JD.y = 0;
	switch( JD.x ) {
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
	if( JD.y ) {
		oHS1o();
		xHS2o();
	}
	return *this;

}
gpcDrc& gpcDrc::judo2( gpcROB& iROB, U4 mSEC, U4 iD0, gpcGT* pGT, gpcROBnD *pROBnD ) {
	*this = iROB;	/// XYZABCxyz 1. ixyz = iXYZ.ABC2xyz( txyz, iABC );
	I4x4* pSTP;
	/// --------------------
	///			JD I/O
	/// --------------------
	switch( JD.y ) {
		case 4: { /// 4.HS2i? // HS2o elvesz VÉGE
				if( bHS2i() ) {
					/// PULLING még nem vette az adást
					oHS2o();
					oADRin = 0;
					JD.y = 4;
					return *this;
				}
				xHS2o();
				JD.y = 0;
				if( JD.z ) {
					JD.x = 0;
					return *this;
				}
				switch(JD.x) {
					case 0:
						JD.x = 1;
						break;
					default:
						JD.x = 1;
						break;
				}
			} return *this; // hagyjuk élni a másik robotot is
		case 3: { /// 3.HS2i? // HS2o kitesz
				if( oADRin.n )
				if( oADRin.i < oADRin.n ) {
					/// csomagolt küldés
					oADRin.i++; /// 0 már ellet küldve
					pSTP = lstSTP.pI4x4( oADRin.i*3 );
					if( pSTP ) {
						oXYZ.xyz_( pSTP[0] );
						oABC.xyz_( pSTP[1] );
					}
					oHS1o();
					JD.y = 1;
					return *this;
				}

				if( bHS2i() ) {
					/// na végre megérkezett a robot hurrá
					switch(JD.z = iCTRL.z) {
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
					oADRin = 0;
					JD.y = 4;
					return *this;
				}
				else if( bHS1i() ? false : HS1ms ) {
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
				if( bHS1i() ) {
					/// na végre
					xHS1o();
					JD.y = 2;
					return *this;
				}
				/// PULLING még nem vette az adást tartjuk
				oHS1o();

				if( bHS2i() ) {
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
			if( bHS2i() ) {
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
	//float ab = 1.0, k;
	static const float K = (100.0*PI2);

	//I4 lim = 0;
	I4x4 	itABCdif = iABC.mmABC( tABC, degX(180.0/PI), degX(180.0/PI) ),
			tmp, dxyz = txyz - ixyz,
			cN,
			iiXYZ = iXYZ, iiABC = iABC, iixyz = ixyz,
			cXYZ, cABC, cxyz,
			hdXYZ,
			dXYZ = tXYZ - iXYZ, lXYZ,
			dGRP = tGRP - iGRP;
			//aHDxyz[8];

	mmABCD.D = sqrt(dGRP.qlen_xyz());
	mmABCD.A = dXYZ.abs0().mx().x;
	if( itABCdif.w > (degX(1)/16) )
		mmABCD.A += itABCdif.w;
	if( txyz.abs0().mx().x > (mmX(1)/16) )
		mmABCD.A += dxyz.abs0().mx().x;

	U4 i = 3, j;
	I4x2 tr = 0;
	if( mmABCD.A ) {
		//aa = mmABCD.A;
		switch( NMnDIF.x ) {
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
		/// --------------------------
		///		lstSTP TRG1
		/// --------------------------
		pSTP = lstSTP.pI4x4n( 0 );
		if( !oADRin.n ) {
			pSTP[0] = tXYZ;
			pSTP[1] = tABC;
			pSTP[2] = txyz;
		}

		for( j = 0; j <= oADRin.n; j++ ) {
			/// --------------------------
			///		lstSTP START
			/// --------------------------
			pSTP = lstSTP.pI4x4( j*3 );
			if( !pSTP ) {
				oADRin.n = j;
				break;
			}
			cXYZ.xyz_(pSTP[0]);
			cABC.ABC_(pSTP[1]);
			if( txyz.abs0().mx().x )
				cxyz.xyz_(pSTP[2]);
			else
				cxyz.null();

			cN.null();
			mmABCD = judoCAGEin(  cXYZ,  cABC,  cxyz,
								 iiXYZ, iiABC, iixyz,
								 cN, i, iMX );
			if( mmABCD.C )
				oCTRL.z |= 2;
			else if( mmABCD.A < (mmX(1)/16) )
				continue;

			if( mmABCD.B < (mmX(1)/16) ) {
				tXYZ = pSTP[0] = iiXYZ;
				tABC = pSTP[1] = iiABC;
				txyz = pSTP[2] = iixyz;
				if(bSTDcout){gpdCOUT << "CAGE tXYZ: " << tXYZ.pSTR( gpsJDpub ) <<gpdENDL;}
				continue;
			}
			oCTRL.z |= 1;

			iiXYZ = pSTP[0] = cXYZ;
			iiABC = pSTP[1] = cABC;
			iixyz = pSTP[2] = cxyz;
			tr.x += mmABCD.B;
			tr.y += mmABCD.C;

			if( (mmABCD.A-mmABCD.B) <= (mmX(1)/16) )
				continue;
			/// --------------------------
			///		lstSTP mandiner		+1
			/// --------------------------
			oADRin.n++;
			j++;

			pSTP = lstSTP.pI4x4nINS( j*3, 1*3 );
			iiXYZ = pSTP[0] = cXYZ;
			iiABC = pSTP[1] = cABC;
			iixyz = pSTP[2] = cxyz;

			cN = (cN*mmX(50))/cN.abs0().mx().x;
			cXYZ += cN;
			mmABCD = judoCAGEin(  cXYZ,  cABC,  cxyz,
								 iiXYZ, iiABC, iixyz,
								 cN, i, iMX );
			if( mmABCD.C )
				oCTRL.z |= 2;

			if( mmABCD.B < (mmX(1)/16) ) {
				tXYZ = pSTP[0] = iiXYZ;
				tABC = pSTP[1] = iiABC;
				txyz = pSTP[2] = iixyz;
				if(bSTDcout){gpdCOUT << "CAGE tXYZ: " << tXYZ.pSTR( gpsJDpub ) <<gpdENDL;}
				continue;
			}
			oCTRL.z |= 1;
			tr.x += mmABCD.B;
			tr.y += mmABCD.C;

			iiXYZ = pSTP[0] = cXYZ;
			iiABC = pSTP[1] = cABC;
			iixyz = pSTP[2] = cxyz;

		}
	}

	/// --------------------------
	///		OUT <- pSTP[0]
	/// --------------------------
	pSTP = lstSTP.pI4x4( 0 );
	if( oCTRL.z&1 )
		oXYZ.xyz_(pSTP[0]);
	else {
		oXYZ.xyz_(iXYZ);
		//ab = 0.0;
	}

	if( oCTRL.z&2 ) {
		oABC.ABC_(pSTP[1]);
		if( !txyz.abs0().mx().x )
			oxyz.xyz_(pSTP[2]);
		else
			oxyz.null();
	}
	else {
		tABC.ABC_(oABC.ABC_(iABC));
	}

	/// --------------------------
	///			jdPRG
	/// --------------------------
	if( jdPRGstp( mSEC, pGT, pROBnD ) )
		return *this;

	/*if( mmABCD.A > mmABCD.B ) {
		// ketrec gátolta
        // o-kat berakjuk a t-be
		if( mmABCD.B < (mmX(1)/16) ) {
			tXYZ.xyz_( oXYZ );
			tABC.ABC_( oABC );
			if( txyz.qlen_xyz() )
				txyz.xyz_( oxyz );
		}

		dXYZ = oXYZ-iXYZ;
		lXYZ = I4x4(mmX(750), mmX(375), mmX(1125) )-oXYZ;
		mmABCD.C = lXYZ.abs0().mx().x;
		if( mmABCD.C )
			oXYZ.xyz_( oXYZ+((lXYZ*mmX(11))/mmABCD.C) );
		oXYZ.xyz_( (oXYZ+iXYZ)/2 );
		oxyz.xyz_( (oxyz+ixyz)/2 );
		if(bSTDcout){gpdCOUT << "CAGE tXYZ: " << tXYZ.pSTR( gpsJDpub ) <<gpdENDL;}
	} else
		if(bSTDcout){gpdCOUT << "oXYZ: " << oXYZ.pSTR( gpsJDpub ) << "\r\ntXYZ: " << tXYZ.pSTR( gpsJDpub ) <<gpdENDL;}
	*/
	JD.y = 0;
	switch( JD.x ) {
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
	if( JD.y ) {
		oHS1o();
		xHS2o();
	}
	return *this;

}
