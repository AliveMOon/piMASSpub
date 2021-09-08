#include "gpcgt.h"
#include "gpcwin.h"
extern U1 gpaALFsub[];
extern char gpsTAB[], *gppTAB;

gpcLZY*	gpcGT::GThtmlOS( gpcLZY* pOUT, gpcGT& mom, gpcWIN* pWIN, void* pG, void* pH ){
	if(!this)
		return pOUT;
	char	*pGET = (char*)pG,
			*pHOST = (char*)pH;
	/// sudo iptables -t nat -A PREROUTING -p tcp --dport 80 -j REDIRECT --to-port 6000
	U8 s;


	return pOUT->lzyFRMT( s = -1, gpdFRMThtml_S, pHOST  );
}
