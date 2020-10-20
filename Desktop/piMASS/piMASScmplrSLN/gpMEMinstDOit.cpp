#include "gpcSRC.h"
#include "gpcSRCinst.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];

I4 gpMEM::instDOit( gpOBJ& obj, U1* pU1 )
{
	bool bCID = this ? !pU1 : true;
	I4 cID = -1;
	switch( obj.AN.alf )
	{
		case gpeALF_TELNET: {
				cID = gpeCsz_L;
				if( bCID ) break;
				I4 port = *(U4*)pU1;
				if( port < 0 )
					break;

			} break;
		case gpeALF_SLMP: {
				cID = gpeCsz_L;
				if( bCID ) break;
				I4 port = *(U4*)pU1;
				if( port < 0 )
					break;

			} break;
		case gpeALF_AGAIN:	cID = gpeCsz_l;
			if( bCID ) break;

			msRUN = *(U4*)pU1;
			if( !msRUN )
				break;
			msRUN += pWIN->mSEC.x;
			break;
		default:
			break;
	}
	return cID;
}
