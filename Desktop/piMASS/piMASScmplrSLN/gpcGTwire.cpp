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
U1  aIOonPCB[] = { 29,31,32,33,36,11,12,35,38,40,15,16,18,22,37,13, },
    aIObcm[] = { 5,6,12,13,16,17,18,19,20,21,22,23,24,25,26,27, },
    aIOfree[] = { 21,22,26,23,27,0,1,24,28,29,3,4,5,6,25,2, };
class gpcWIRE {
public:
    U4 aIO[0x28],aIOd[0x28], cnt;
    int aIOm[0x28],
        aPOSin[0x8],
        aPOSout[0x8];
    I8x2 aAN[0x28];
    I8  aCNT[0x28];
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
        if( socket == INVALID_SOCKET ) {
           socket = ::socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
        }
        wiringPiSetup();		// Setup the library
        pWR->aIOm[0] = -1;
        gpmMsetOF( pWR->aIOm+1, gpmN(pWR->aIOm)-1, pWR->aIOm );
        for( U4 i = 0, n = gpmN(aIOfree); i < n; i++ ) {
            if( pWR->aIOm[aIOfree[i]] != OUTPUT ) {
               pinMode(aIOfree[i], OUTPUT);
               pWR->aIOm[aIOfree[i]] = OUTPUT;
            }
            digitalWrite(aIOfree[i], 0 );
        }
        //pinMode(0, OUTPUT);		// Configure GPIO0 as an output
        //pinMode(1, INPUT);		// Configure GPIO1 as an input
	}
    U4 reg = pWR->cnt, bI, ms = pWIN->mSEC.x&0xfffffffe;



    for( U4 i = 0, n = gpmN(aIOfree); i < n; i++ ) {
        if( !pWR->aAN[i].alf )
            continue;
        if( pWR->aCNT[i] == pWR->aAN[i].num )
            continue;
        int dif = pWR->aAN[i].num - pWR->aCNT[i];
        if( dif < 0 )
            pWR->aCNT[i]--;
        else
            pWR->aCNT[i]++;

        if( pWR->aIOm[i] != OUTPUT ) {
           pinMode(aIOfree[i], OUTPUT);
           pWR->aIOm[i] = OUTPUT;
        }
        bI = pWR->aCNT[i]&1;
        if( pWR->aIO[i]&1 == bI )
            continue;
        pWR->aIOd[i] = ms-pWR->aIO[i];
        pWR->aIO[i] = ms|bI;
        digitalWrite(aIOfree[i], bI );
    }


    pWR->cnt++;

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
   U1 	sCOM[] = "ABCD",
		*pCOM, *pEND = pSTR+n, *pNUM;
	U4& comA = *(U4*)sCOM, iNUM = 0, nNUM, iE = 2;
	I8x2 an, anC;
	double d8;
	gpeALF alf = gpeALF_null;
	bool bSTAT = false;
	for(
			 pSTR += gpmNINCS( pSTR, " \t\a\r\n;," );
			*pSTR;
			 pSTR += gpmNINCS( pSTR, " \t\a\r\n;," )
		) {
		an.num = pEND-(pCOM = pSTR);
		an = pCOM;
		pSTR += an.num;
		//pNUM = NULL;
		if( an.alf ) {
			comA = *(U4*)pCOM;
			iNUM = gpeDRCos_NONS;
			switch( an.alf ) {
				case gpeALF_STAT:
					bSTAT = true;
					break;
				case gpeALF_POS:
					pNUM = pSTR;
					iNUM = 0;
					anC = an;
					bSTAT = true;
					break;
				/*case gpeALF_HUP:
					pNUM = NULL;
					pWR->aNUM[1] = 0;
					iNUM = 0;
					bSTAT = true;
					break;*/
				default:
					break;

			}
		}
		if( !pNUM )
			continue;

        pNUM = pSTR;
        pWR->aAN[iNUM] = anC;
        pWR->aAN[iNUM].num = gpfSTR2I8( pNUM, &pSTR );
		iNUM++;
		if( iNUM >= gpmN(pWR->aAN) )
            break;
		//break;
	}
	//if(bSTAT)
	//	return pGSM->answSTAT( pANS );
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
