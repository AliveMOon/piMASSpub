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


gpcLZY* gpcDrc::answSTAT( gpcLZY* pANS ) {
		if( !this )
			return pANS;

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

gpcLZY* gpcLZY::lzyZSnD( U8& iSTRT, gpcZSnD& zs, U1 i ) {
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
        if( abba < 1 )
			return S;
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

	I8x4 mmABCD( sqrt(iXYZ.qlen_xyz()) );
	//I8	mmA = sqrt(iXYZ.qlen_xyz()), mmB = 0, mmC = 0, mmD = 0;
	if( mmABCD.x < mm100(400) )
		return *this;

	oCTRL.z = 0;
	if( sqrt(okXYZ.qlen_xyz()) > mm100(400) )
		iXYZ.xyz_( okXYZ );

	F4x4 tMX, iMX;

	iMX.ABC(iABC, mm100(180)/PI );

	if( !JD.x )
	{
		oCTRL.w = 0;
		oCTRL.z = 1;
		JD.y = 0;
		tXYZ.xyz_( oXYZ.xyz_(iXYZ) );
		tABC.xyz_( oABC.xyz_(iABC) );
		txyz.xyz_( oxyz.xyz_(ixyz) );
		JD.x = 1;
		return *this;
	}

	// itt kell le ellenőrizni mondjuk az ütközésre
	//
	float ab = 1.0, k;
	static const float K = (100.0*PI*2.0);
	I4 itD = iABC.chkABC( tABC, mm100(1) ).w, lim = 0;
	I4x4 	tmp,
			//nD = iABC.chkABC( tABC, mm100(1) ),
			dXYZ = tXYZ - iXYZ,
			dxyz = txyz - ixyz,
			dGRP = tGRP - iGRP;

	//nD.z
	mmABCD.w = sqrt(dGRP.qlen_xyz());

	mmABCD.x = dXYZ.qlen_xyz();
	if( mmABCD.x )
	{
		mmABCD.x = sqrt(mmABCD.x);
		lim = ((mmABCD.x/mm100(gpdROBlim))<2) ? mmABCD.x : mm100(gpdROBlim);

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

		chk( lim, i );
		dXYZ = oXYZ - iXYZ;
		mmABCD.y = sqrt(dXYZ.qlen_xyz());
		ab = float(mmABCD.y)/float(mmABCD.x);
		if( mmABCD.y )
		{
			if( lim <= mmABCD.y+10 )
			{
				// elérte a lim-et azaz nem érte el a ketrecet
				lim = mmABCD.y;
			}
			else if( mmABCD.y > mm100(10) )
			{
				// kisebb let mint amit elvártunk a ketrec nem engedte
				dXYZ *= (I4)(mmABCD.y/100)-10;
				dXYZ /= (I4)(mmABCD.y/100);
				oXYZ.xyz_(dXYZ+iXYZ);
				mmABCD.y -= mm100(10);
				ab = float(mmABCD.y)/float(mmABCD.x);
			}
			oCTRL.z |= 1;
		}
	}

	if( txyz.qlen_xyz() )
	{
		mmABCD.z = dxyz.abs_xyz0().mx().x;
		if( mmABCD.y|(mmABCD.z>10) )
		{
			mmABCD.z = sqrt(dxyz.qlen_xyz());
			tMX = 1.0;
			tMX.z = (txyz-tXYZ).xyz0();
			if( abs(ab) < 0.0001f )
			{
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
			}

			tMX.z /= sqrt(tMX.z.qlen_xyz());
			tMX.x = iMX.y.cross_xyz(tMX.z);
			tMX.y = tMX.z.cross_xyz(iMX.x);
			float	xl = sqrt(tMX.x.qlen_xyz()),
					yl = sqrt(tMX.y.qlen_xyz());
			if( xl > yl )
			{
				tMX.x /= xl;
				tMX.y = tMX.z.cross_xyz(tMX.x);
			} else {
				tMX.y /= yl;
				tMX.x = iMX.y.cross_xyz(tMX.z);
			}


			tABC.xyz_( tMX.eABC()*(180.0/PI)*mm100(1) );
			itD = iABC.chkABC( tABC, mm100(1) ).w;
		}
	}



	if( itD < 55 )
	{
		tABC.xyz_(oABC.xyz_(iABC));
	} else {

		 //, dMX;

		// mátrixot csinálunk belőle
		iMX.ABC(iABC, mm100(180)/PI );
		tMX.ABC(tABC, mm100(180)/PI );

		F4 rnd( K,K,K );
		rnd &= (iMX.dot(tMX).acos()/(2.0*PI));	// n karika * kerület

		k = rnd.xyz0().abs().mx();
		if( k > 0.0 )
		{
			float lpk = gpdROBlim/k;

			if( ab > lpk )
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
				oABC.xyz_( tMX.eABC()*(180.0/PI)*mm100(1) );
			} else {
				oABC.xyz_( tABC );
			}
		}
		oCTRL.z |= 2;
	}



	if( !oCTRL.z )
		return *this;

	if( lim > mmABCD.y )
	{
		// ketrec gátolta
        // o-kat berakjuk a t-be
        if( abs( oABC.y ) > mm100(100) )
        {
			oCTRL.z = 0;
			return *this;
        }
		tXYZ.xyz_( oXYZ );
		tABC.xyz_( oABC );
		if( txyz.qlen_xyz() )
			txyz.xyz_( oxyz );
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


