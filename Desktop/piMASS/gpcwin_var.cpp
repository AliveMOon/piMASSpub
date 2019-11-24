#include "gpccrs.h"
extern U1 gp_s_key_map_sdl[];

bool gpcWIN::WINvar( gpcREG& out, gpeALF alf )
{
	out.off();
	if( !alf )
		return out.bGD();
	U8 iS = winPUB.n_load;
	gpcCRS& crs = *apCRS[onDIV.x];
	if( alf < gpeALF_AAAAAA ) {
		if( alf < gpeALF_AAAAA ) {
			if( alf < gpeALF_AAAA ) {
				if( alf < gpeALF_AAA ) {
					if( alf < gpeALF_AA ) {		// A - Z ------------------------------------------

					} else {					// AA - ZZ ----------------------------------------
						switch( alf )
						{
							case gpeALF_IA:
								out = crs.scnZN.x+1;
								break;
							case gpeALF_IN:
								out = crs.scnZN.y;
								break;
							case gpeALF_IX:
								out = crs.scnIN.x;
								break;
							case gpeALF_IY:
								out = crs.scnIN.y;
								break;
							case gpeALF_IW:
								out = crs.scnIN.z;
								break;
							case gpeALF_IH:
								out = crs.scnIN.w;
								break;
							default:
								break;
						}
					}
				} else {						// AAA - ZZZ --------------------------------------
						switch( alf )
						{
							case gpeALF_MLB:
								out = !!(nMB&1);
								break;
							case gpeALF_MMB:
								out = !!(nMB&2);
								break;
							case gpeALF_MRB:
								out = !!(nMB&4);
								break;
							case gpeALF_FPS:
								out = mSEC.w;
								break;
							default:
								break;
						}
				}
			} else {							// AAAA - ZZZZ ------------------------------------
						switch( alf )
						{
							case gpeALF_MSEC:
								out = mSEC.x&(~1);
								break;
							case gpeALF_HOST: if( bINIThu() ) {
									out = sHOST;
								} break;
							case gpeALF_USER: if( bINIThu() ) {
									out = sUSER;
								} break;
							default:
								break;
						}
			}
		} else {								// AAAAA - ZZZZZ ----------------------------------

		}
	} else {									// AAAAAA - ZZZZZZ --------------------------------


	}
	winPUB.n_load = iS;
	return out.bGD();
}
