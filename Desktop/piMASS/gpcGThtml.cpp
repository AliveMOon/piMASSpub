#include "gpcgt.h"
#include "gpcwin.h"
extern U1 gpaALFsub[];
extern char gpsTAB[], *gppTAB;

gpcLZY*	gpcGT::GThtmlOS( gpcLZY* pOUT, gpcGT& mom, gpcWIN* pWIN, char* pGET, char* pHOST ){
	if(!this)
		return pOUT;



	return pOUT->lzyFRMT( s = -1, gpdFRMThtml_S, pHOST  );
}
