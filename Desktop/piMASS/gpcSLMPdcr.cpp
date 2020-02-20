#include "gpcgt.h"
#include "gpcwin.h"
extern U1 gpaALFadd[];
extern char gpsTAB[], *gppTAB;

gpcDrc& gpcDrc::judo( gpcZS& inp ) {
	*this = inp;
	switch( JD.y )
	{
		case 6:
			JD.y = 0;
			return *this;
		case 5:
			if(bHS2i()) {
				// még nem vette a jelet PULLING
				oHS2o();
				return *this;
			}
			JD.y = 6;
			// vette az adást mi is elvesszük
			xHS2o();
			return *this;
		case 4: /// 4.HS2 jelre várunk
			if(bHS2i()) {
				// jelzünk hogy olvastuk
				JD.y = 5;
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
			case 0: // ALAPH kell
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
