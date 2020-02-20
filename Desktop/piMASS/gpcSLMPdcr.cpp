#include "gpcgt.h"
#include "gpcwin.h"
extern U1 gpaALFadd[];
extern char gpsTAB[], *gppTAB;

I4x4 gpaCAGEbill[] = {
	{ 0, 0, mm100(320), mm100(420) }, { 0, 0, mm100(-300), mm100(550) },
	{ mm100(1500), 0, mm100(320), mm100(800) },
	{ mm100(685), mm100(-469), mm100(366),  mm100(300) },
};
I4x4 gpaCAGEjohn[] = {
	{ 0, 0, mm100(320), mm100(420) }, { 0, 0, mm100(-300), mm100(550) },
	{ mm100(1500), 0, mm100(320), mm100(800) },
	{ mm100(685), mm100(-469), mm100(366),  mm100(300) },
};
I4x4 gpcDrc::cage( I4x4* pCAGE, U4 n ) {
	I4x4 T = trgXYZ.xyz_(), S = iXYZ.xyz_(), a, b;
	I8 dd = (T-S).qlen_xyz(), d = sqrt(dd), abba, ab;
	for( U4 i = 0; i < n; i++ )
	{
		a = (S-pCAGE[i]).xyz_();

		// +mm100(100)-a magának TOOL nak is adunk vele egy sugarat
        b = a.TSr( T-pCAGE[i], pCAGE[i].w+mm100(100) );
        abba = (b-a).qlen_xyz();
        ab = sqrt(abba);
        if( dd > abba )
        {
			T = b+pCAGE[i].xyz_();
			dd = abba;
        }
		i++;
	}
	return T;
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
				oHS2o();
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
						break;
					default:
						// ZS hibat jelzett
						// trg-be eltároljuk hol van most
						// ne is akarcon tovább menni
						trgXYZ.xyz_( oXYZ.xyz_(iXYZ) );
						trgABC.xyz_( oABC.xyz_(iABC) );
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

	I8 dif = iXYZ.qlen_xyz(), mm = sqrt(dif)/100;
	if( mm < 400 )
		return *this;

	I4x4 dir = trgXYZ - iXYZ;
	dif = dir.qlen_xyz();
	if( dif )
	{
		switch( NMnDIF.x )
		{
			case gpeZS_BILL:
				oXYZ.xyz_( iXYZ.lim_xyz( cage(gpaCAGEbill,gpmN(gpaCAGEbill)), mm100(50)) );
				break;
			case gpeZS_JOHN:
				oXYZ.xyz_( iXYZ.lim_xyz( cage(gpaCAGEjohn,gpmN(gpaCAGEjohn)), mm100(50)) );
				//oXYZ.xyz_( cage(gpaCAGEjohn,gpmN(gpaCAGEjohn)).lim_xyz(mm100(50)) );
				break;
			default:
				oXYZ.xyz_( iXYZ );
				break;
		}
		oCTRL.z |= 1;
	}

	dir = (trgABC - iABC).lim_xyz(mm100(10));
	dif = dir.qlen_xyz();
	if( dif )
		oCTRL.z |= 2;

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
				JD.y = 1;
				break;
			case 1: // ALAPH ready!
				oCTRL.w = 0;
				oCTRL.z = 10; // linearis XYZ ABC
				JD.y = 1;
				break;


		}
		/// 1.HS1 elöbb ki X-eljük a HS1-et, nehogy ZS megszaladjon
		xHS1o();
		return *this;
	}

	/// join adatok most egyenlőre nem
	return *this;
}
