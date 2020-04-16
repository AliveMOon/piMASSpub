#include "gpcgt.h"
#include "gpcwin.h"
extern U1 gpaALFadd[];
extern char gpsTAB[], *gppTAB;
extern I4x4 gpaCAGEbillBALL[],
			gpaCAGEbillBOX[];
extern U4	gpnCAGEbillBALL,
			gpnCAGEbillBOX;
extern I4x4 gpaCAGEjohnBALL[],
			gpaCAGEjohnBOX[];
extern U4	gpnCAGEjohnBALL,
			gpnCAGEjohnBOX;


gpcLZY* gpcDrc::answSTAT( gpcLZY* pANS, U1 id ) {
		if( !this )
			return pANS;
		I4x4 cXYZ = chkXYZ( mmX(gpdROBlim), id );

		U1 	sCOM[] = "ABCD";
		U4 &comA = *(U4*)sCOM;

		comA = NMnDIF.x;
		U8 s;
		pANS = pANS->lzyFRMT( s = -1,	"\r\n// %s HS12:%0.4X", *sCOM ? (char*)sCOM : "?", hs12() );
		pANS = pANS->lzyFRMT( s = -1,	"\r\n// POS:  n:%d"
										"\r\n//\tiXYZ %7.2fmm, %7.2fmm, %7.2fmm "
										"\r\n//\tcXYZ %7.2fmm, %7.2fmm, %7.2fmm "
										"\r\n//\toXYZ %7.2fmm, %7.2fmm, %7.2fmm "
										"\r\n//\ttXYZ %7.2fmm, %7.2fmm, %7.2fmm;"
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
										double(tXYZ.z)/mmX(1)
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
		pANS = pANS->lzyFRMT( s = -1,	"\r\n// dir:"
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
							);
		return pANS;
	}

gpcLZY* gpcLZY::lzyZSnDstat( U8& iSTRT, gpcZSnD& zs, U1 i ) {
	if( !&zs ) {
		iSTRT = nLD();
		return this;
	}
	gpcLZY* pANS = this;
	U1 n = gpmN(zs.aDrc);
	if( i < n )
	{
		pANS = zs.aDrc[i].answSTAT( pANS, i );
		return this;
	}

	for( U1 i = 0, n = gpmN(zs.aDrc); i < n; i++ )
		pANS = zs.aDrc[i].answSTAT( pANS, i );
	return this;
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
U1 gpcZSnD::iDrc( bool bPULL ) {
	if( bPULL && !(ioSW.y&1) )
		ioSW.y |= 1;
	return (ioSW.y>>1)&1;
}
gpcLZY* gpcZSnD::pulling( gpcLZY* pOUT, U4x4* pZSrw ) {
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

gpcDrc& gpcDrc::operator = ( gpcZS& zs ) {
	gpmMcpyOF( &iXYZ.x, &zs.aPOS, 3 );
	if( iXYZ.qlen_xyz() < 32*32 )
	{
		gpmMcpyOF( &iXYZ.x, &zs.oMxyzEspd, 3 );
		if( iXYZ.qlen_xyz() < 32*32 )
			iXYZ.x = iXYZ.y = iXYZ.z = mmX(450);
	}
	gpmMcpyOF( &iABC.x, &zs.aABC, 3 );

	ixyz = iXYZ.ABC2xyz( txyz, iABC );

	//gpmMcpyOF( &ixyz.x, &zs.apos, 3 );
	//if( !ixyz.qlen_xyz() )
	//{
	/*F4x4 iMX;
	iMX.ABC(iABC, degX(180.0/PI) );
	I4 Zxyz = txyz.qlen_xyz();
	if( Zxyz ) {
		Zxyz = sqrt((txyz-tXYZ).qlen_xyz());
		ixyz.xyz_( iXYZ + (iMX.z*Zxyz) );
	} else {
		if( (iMX.z.z <= -COSSIN45) && (iXYZ.z > 0) )
			ixyz.xyz_( iXYZ + (iMX.z * (float(-iXYZ.z)/iMX.z.z)) );	// lerakja z=0-ra
		else
			ixyz.xyz_( iXYZ + (iMX.z*mmX(200)) );
	}*/
	//}

	gpmMcpyOF( &iabc.x, &zs.aabc, 3 );
	gpmMcpyOF( &aiAX1to6[0].x, zs.aJ16, 3 );
	gpmMcpyOF( &aiAX1to6[1].x, zs.aJ16+3, 3 );
	gpmMcpyOF( &aiax1to6[0].x, zs.aj16, 3 );
	gpmMcpyOF( &aiax1to6[1].x, zs.aj16+3, 3 );

	gpmMcpyOF( &iCTRL.y, &zs.io128.y, 3 );
	return *this;
}

gpcDrc::gpcDrc( char* pbuff, I4x4 a, I4x4 b, I4x4 c ) {
	gpmCLR;
	iXYZ.xyz_(a);
	tXYZ.xyz_(b);

	oXYZ.xyz_( (tXYZ+iXYZ)/2 );

	txyz.xyz_(c);

	I4x4	iR = (iXYZ-txyz).xyz0(),
			tR = (tXYZ-txyz).xyz0(),
			oR = (oXYZ-txyz).xyz0();

	U4	iRr = sqrt(iR.qlen_xyz())/0x100,
		oRr = sqrt(oR.qlen_xyz())/0x100,
		tRr = sqrt(tR.qlen_xyz())/0x100;

	oXYZ.xyz_( (oR*tRr)/oRr + txyz );

	std::cout << "iR   " << iR.pSTR( pbuff ) << "iRr " << iRr << std::endl;
	std::cout << "tR   " << tR.pSTR( pbuff ) << "tRr " << tRr << std::endl;
	std::cout << "oR   " << oR.pSTR( pbuff ) << "oRr " << oRr << std::endl;
	std::cout << "a    " << a.pSTR( pbuff ) << std::endl;
	std::cout << "oXYZ " << oXYZ.pSTR( pbuff ) << std::endl;
	std::cout << "tXYZ " << tXYZ.pSTR( pbuff ) << std::endl;
	std::cout << std::endl;
}

gpcDrc& gpcDrc::judo( gpcZS& iZS ) {
	*this = iZS;	/// XYZABCxyz 1. ixyz = iXYZ.ABC2xyz( txyz, iABC );
	switch( JD.y ) {
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
				// megkaptuk
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
						/// XYZABCxyz 2. OK ------------- JD.z = 0
						okXYZ.xyz_(oXYZ);
						okABC.xyz_(oABC);
						okxyz.xyz_(txyz);
						/*if( txyz.qlen_xyz() )
							okxyz.xyz_(okXYZ.ABC2xyz(txyz,okABC));
						else
							okxyz.xyz_( 0 );*/
						break;
					default:
						// ZS hibat jelzett
						// trg-be eltároljuk hol van most
						// ne is akarjon tovább menni
						/// XYZABCxyz 3. NOK ------------- JD.z = ERR
						tXYZ.xyz_( oXYZ.xyz_(iXYZ) );
						tABC.xyz_( oABC.xyz_(iABC) );
						break;
				}
				return *this;
			}
			// még nemjött
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

	I8x4 mmABCD( sqrt(iXYZ.qlen_xyz()) );
	if( mmABCD.x < mmX(400) )
		return *this;		// nem fut a R2D task


	if( !JD.x )
	{
		oCTRL.w = 0;
		oCTRL.z = 1;
		JD.y = 0;
		tXYZ.xyz_( oXYZ.xyz_(iXYZ) );
		tABC.xyz_( oABC.xyz_(iABC) );
		txyz.xyz_( 0 ); //oxyz.xyz_(ixyz) );
		JD.x = 1;
		return *this;
	}
	/// CTRL.z = 0;
	oCTRL.z = 0;
	if( sqrt(okXYZ.qlen_xyz()) > mmX(200) ){
		// ezt kaptuk a robitol
		// ha netán akkor sem mozdul
		oXYZ.xyz_(iXYZ);
		oABC.xyz_(iABC);
		//oxyz.xyz_(ixyz);
		// ha van ok felül írjuk amit a robi adott
		// mindig abbol számoljunk

		/*iXYZ.xyz_(okXYZ);
		iABC.xyz_(okABC);
		ixyz.xyz_(okxyz);*/
	}

	F4x4 tMX = 1.0, iMX;
	iMX.ABC(iABC, degX(180.0/PI) );
	// itt kell le ellenőrizni mondjuk az ütközésre
	//
	float ab = 1.0, k;
	static const float K = (100.0*PI*2.0);

	I4 itD = iABC.chkABC( tABC, mmX(1) ).w, lim = 0;
	I4x4 	tmp,
			dXYZ = tXYZ - iXYZ,
			dGRP = tGRP - iGRP;
	/*if( !txyz.qlen_xyz() )
		dxyz.null();*/

	//nD.z
	mmABCD.w = sqrt(dGRP.qlen_xyz());

	mmABCD.x = dXYZ.abs0().mx().x; //qlen_xyz();
	if( mmABCD.x ) {

		mmABCD.x = sqrt(dXYZ.qlen_xyz());
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
		lim = ((mmABCD.x/mmX(gpdROBlim))<2) ? mmABCD.x : mmX(gpdROBlim);

		oXYZ.xyz_( chkXYZ( lim, i ) );

		dXYZ = oXYZ - iXYZ;
		mmABCD.y = dXYZ.abs0().mx().x;
		if( mmABCD.y ) {
			mmABCD.y = sqrt(dXYZ.qlen_xyz());
			if( lim <= mmABCD.y + mmX(3) )
			{
				// elérte a lim-et azaz nem érte el a ketrecet
				lim = mmABCD.y;
				if( mmABCD.y < mmABCD.x )
				if( txyz.qlen_xyz() )
				{
					// de nem érte el az tXYZ-t
                    // azaz felosztotta a pályát
					I4x4 oR = (oXYZ-txyz).xyz0(),
						 tR = (tXYZ-txyz).xyz0();
					I4	oRr = sqrt(oR.qlen_xyz()),
						tRr = sqrt(tR.qlen_xyz());


					if( oRr )
					if( oRr < tRr )
					{
						I4x4	trg = (oR*tRr)/oRr + txyz,
								tmp = chkXYZ( trg, 0, i );
						if( trg.xyz0() == tmp.xyz0() )
						{
							oXYZ.xyz_( tmp );
							dXYZ = oXYZ - iXYZ;
							lim = mmABCD.y = sqrt(dXYZ.qlen_xyz());
						}
						/*else {

						}


						I4x4	SVoXYZ = oXYZ,
								SVtXYZ = tXYZ;
						// levágná az utat
						tXYZ.xyz_( (oR*tRr)/oRr + txyz );
                        // sért e ketrecet

						chkXYZ( 0, i );
						if( oXYZ.xyz0() == tXYZ.xyz0() )
						{
							dXYZ = oXYZ - iXYZ;
							lim = mmABCD.y = sqrt(dXYZ.qlen_xyz());
						} else {
							oXYZ.xyz_( SVoXYZ );
						}
						tXYZ.xyz_( SVtXYZ );*/
					}
				}
			}
			else if( mmABCD.y > mmX(10) )
			{
				// kisebb let mint amit elvártunk a ketrec nem engedte
				dXYZ *= (I4)(mmABCD.y/mmX(1))-10;
				dXYZ /= (I4)(mmABCD.y/mmX(1));
				oXYZ.xyz_(dXYZ+iXYZ);
				mmABCD.y = sqrt(dXYZ.qlen_xyz());
			} else {
				// nagyon kicsit engedne
				mmABCD.y = 0;
			}
		}

		if( mmABCD.y )
		{
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

			//mmABCD.z = sqrt(dxyz.qlen_xyz());
			/*if( abs(ab) < 0.0001f ) {
				tMX.z = iMX.z;
			}
			else if( ab < 1.0 )
			{
				F4 Yd = tMX.z-iMX.z, Yn;
                float	yl = sqrt(Yd.qlen_xyz()), alf;
                if( yl > 0.0 ) {
					alf = acos(tMX.z.norm_xyz()*iMX.z)*ab; 	// tZ és iZ szöge alf

					Yn = Yd/yl;		// normalizálom a Zb-t
					tMX.x = Yn.cross_xyz(iMX.z).norm_xyz(); // egy x merölegest csinálunk a Yn x iZ síkra

					Yd = iMX.z.cross_xyz(tMX.x.norm_xyz()).norm_xyz(); // Yd-t derékszögüre koorigáljuk x segitségével

					tMX.z = Yd*sin(alf) + iMX.z*cos(alf);
                }
			}*/

			tMX.z /= sqrt(tMX.z.qlen_xyz());	// normalizál
			tMX.x = iMX.y.cross_xyz(tMX.z);
			tMX.y = tMX.z.cross_xyz(iMX.x);
			float	xl = tMX.x.qlen_xyz(),
					yl = tMX.y.qlen_xyz();
			if( xl > yl )
			{
				tMX.x /= sqrt(xl);
				tMX.y = tMX.z.cross_xyz(tMX.x);
			} else {
				tMX.y /= sqrt(yl);
				tMX.x = tMX.y.cross_xyz(tMX.z);
			}
			tABC.xyz_( tMX.eABC()*degX(180.0/PI) );
			itD = iABC.chkABC( tABC, degX(1) ).w;
		}
	}

	if( itD < 11 ) {	// kb: ~800
		tABC.xyz_(oABC.xyz_(iABC));
	} else {
		//, dMX;

		// mátrixot csinálunk belőle
		iMX.ABC(iABC, degX(180.0/PI) );
		tMX.ABC(tABC, degX(180.0/PI) );

		F4 rnd( K,K,K );
		rnd &= (iMX.dot(tMX).acos()/(2.0*PI));	// n karika * kerület

		k = rnd.xyz0().abs().mx();
		if( k > 0.0 )
		{
			float lpk = gpdROBlim/k;
			if( ab == 0.0 )
			{
				ab = lpk;
				lim = mmABCD.y;
			}
			else if( ab > lpk )
			{
				ab = lpk;
				mmABCD.y *= gpdROBlim;
				mmABCD.y /= k;
				oXYZ.xyz_( iXYZ.lim_xyz(oXYZ,mmABCD.y) );

				lim = mmABCD.y;
			}


			if( ab < 1.0 )
			{
				// fel lett osztva a mozgás
				F4x4 dMX = (tMX-iMX)*ab + iMX;
				dMX.z /= sqrt(dMX.z.qlen_xyz());	// normalizál

				dMX.x = iMX.y.cross_xyz(dMX.z);
				dMX.y = dMX.z.cross_xyz(iMX.x);
				float	xl = sqrt(dMX.x.qlen_xyz()),
						yl = sqrt(dMX.y.qlen_xyz());
				if( xl > yl )
				{
					dMX.x /= xl;
					dMX.y = dMX.z.cross_xyz(dMX.x);
				} else {
					dMX.y /= yl;
					dMX.x = dMX.y.cross_xyz(dMX.z);
				}

				oABC.xyz_( dMX.eABC()*degX(180.0/PI) );
			} else {
				oABC.xyz_( tABC );
			}
		}
		oCTRL.z |= 2;
	}

	if( jdPRGstp() )
		return *this;

	if( lim > mmABCD.y )
	{
		// ketrec gátolta
        // o-kat berakjuk a t-be
        /*if( abs( oABC.y ) > degX(100) )
        {
			oCTRL.z = 0;
			return *this;
        }*/
		tXYZ.xyz_( oXYZ );
		tABC.xyz_( oABC );
		/// akkor sem írjuk felül ez marad mint cél
		//if( txyz.qlen_xyz() )
		//	txyz.xyz_( oxyz );
	}

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


