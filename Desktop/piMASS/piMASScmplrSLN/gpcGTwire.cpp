#include "gpcgt.h"
#include "gpcwin.h"
#include <wiringPi.h>
extern U1 gpaALFsub[];
extern char gpsTAB[], *gppTAB;
class gpcDRV8825{
public:
    char sNAME[0x100];
    U1 iDIR, iSTP, iSLP, iRST, aiM[3], iEN;
    gpcDRV8825(){ gpmCLR; };
};
U1 aIObcm[] = { 5,6,12,13,16,17,18,19,20,21,22,23,24,25,26,27, };
U1 aIOfree[] = { 21,22,26,23,27,0,1,24,28,29,3,4,5,6,25,2, };
class gpcWIRE {
public:
    U4 aIO[0x28], cnt;
    int aIOm[0x28];
    gpcWIRE(){ gpmCLR; };
};

gpcWIRE* gpcGT::GTwire( gpcWIN* pWIN ) {

    gpcLZY *pLZYinp = pWIN->piMASS->GTlzyALL.LZY(gpdGTlzyIDinp(TnID));
	if( !pLZYinp )
		return NULL;
    U8 s;
    gpcWIRE* pWR = gpmLZYvali( gpcWIRE, pLZYinp );
	if( !pWR ) {
        pLZYinp->lzyINS( NULL, sizeof(gpcWIRE), s=0, sizeof(gpcWIRE) );
        pWR = gpmLZYvali( gpcWIRE, pLZYinp );
        if( !pWR )
            return NULL;
        gpmZ( *pWR );
        wiringPiSetup();		// Setup the library
        pWR->aIOm[0] = -1;
        gpmMsetOF( pWR->aIOm+1, gpmN(pWR->aIOm)-1, pWR->aIOm );

        //pinMode(0, OUTPUT);		// Configure GPIO0 as an output
        //pinMode(1, INPUT);		// Configure GPIO1 as an input
	}
    pWR->cnt++;
    for( U4 i = 0, n = gpmN(aIOfree); i < n; i++ ) {
        if( pWR->aIOm[aIOfree[i]] != OUTPUT ) {
           pinMode(aIOfree[i], OUTPUT);
           pWR->aIOm[aIOfree[i]] = OUTPUT;
        }
        digitalWrite(aIOfree[i], ((pWIN->mSEC.x>>i)&1) );
    }




    return pWR;
}
gpcLZY* gpcGT::GTwireOS( gpcLZY* pANS, U1* pSTR, gpcMASS* pMASS, SOCKET sockUSR, U4 mSEC ) {
	U8 s = -1, nLEN;
	U4 n = gpmSTRLEN( pSTR );
	if( this ? !n : true )
		return pANS->lzyFRMT( s = -1, "nonsens" );

    gpcLZY *pLZYinp = pMASS->GTlzyALL.LZY(gpdGTlzyIDinp(TnID));
	if( !pLZYinp )
		return NULL;

    gpcWIRE* pWR = gpmLZYvali( gpcWIRE, pLZYinp );
	if( !pWR )
		return pANS->lzyFRMT( s = -1, "nonsens" );



	return pANS->lzyFRMT( s = -1, "nonsens" );
}
I4 gpMEM::instDOitWIRE( gpcGT* pGT ) {
	if( this ? !pGT : true )
		return -1;

	I4 cnt = pGT->iCNT;
	gpcWIRE	*pWIRE = pGT->GTwire( pWIN );
	if( cnt == pGT->iCNT )
		return cnt;


    return pGT->iCNT;
}
