#include "gpcgt.h"
#include "gpcwin.h"
#include <wiringPi.h>
extern U1 gpaALFsub[];
extern char gpsTAB[], *gppTAB;
class gpcDRV8825{
public:
    char sNAME[0x100];
    U1  iDIR, iSTP,     iSLP, iRST,
        aiM[3],         iEN;
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
    U1  aMAP[0x28],
        sIP[0x100],
        aDIR[0x4], nD,
        aSTP[0x4],
        aMx[0x10];

    U4      aC[0x4], msJOIN;
    float   aR[0x4];

    I4x2 nLEN;
    std::thread trd;

    gpcWIRE(){ gpmCLR; };
};
void gpfWIREtrd( gpcWIRE* pWR ) {

    int cnt = pWR->cnt, d, bB;
    U4 ms;
    while( (ms=SDL_GetTicks()) < pWR->msJOIN ) {
        ms &= ~1;
        for( U4 iD = 0, w; iD < pWR->nD; iD++ ) {
            if( pWR->aAN[iD].num == pWR->aCNT[iD] )
                continue;
            d = pWR->aAN[iD].num - pWR->aCNT[iD]; /// TRG-NOW
            /// DIR ------------------------------------------
            w = pWR->aDIR[iD];
            if( pWR->aIOm[w] != OUTPUT ) {
               pinMode(aIOfree[w], OUTPUT);
               pWR->aIOm[w] = OUTPUT;
            }
            bB = (d>-1);
            if( (pWR->aIO[w]&1) != bB ) {
               digitalWrite(aIOfree[w], bB );
               pWR->aIO[w] = bB|ms;
            }
            /// STEP ------------------------------------------
            w = pWR->aSTP[iD];
            if( pWR->aIOm[w] != OUTPUT ) {
                pinMode(aIOfree[w], OUTPUT);
                pWR->aIOm[w] = OUTPUT;
            }
            if( bB ) {
                pWR->aCNT[iD]++;
            } else {
                pWR->aCNT[iD]--;
            }
            bB = pWR->aCNT[iD]&1;
            if( (pWR->aIO[w]&1) != bB ) {
               digitalWrite(aIOfree[w], bB );
               pWR->aIO[w] = bB|ms;
            }

        }
        usleep(1250);//00);
        pWR->cnt++;

    }
}
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
	// wire = "D0S1M2M3M4C200R150";
	pWR->nLEN.y = gpmSTRLEN(s_ip);

	if( pWR->nLEN.x == pWR->nLEN.y )
        pWR->nLEN.x = gpmMcmp( pWR->sIP, s_ip, pWR->nLEN.x );
    if( pWR->nLEN.x != pWR->nLEN.y ){
        pWR->nLEN.x = pWR->nLEN.y;
        gpmMcpy( pWR->sIP, s_ip, pWR->nLEN.x )[pWR->nLEN.x] = 0;
        pWR->nD = 0;
        gpmZn(pWR->aMAP,sizeof(pWR->aMAP));
        for( U1 i = 0, d = 0, m, c; i < pWR->nLEN.x; i++ ){
            switch( c = pWR->sIP[i] ){
                case 'D':
                case 'd':
                    i++;
                    m = gpfSTR2I8(pWR->sIP+i);
                    d = pWR->nD++;

                    pWR->aDIR[d] = m;
                    pWR->aMx[d*4]=0;
                    break;
                case 'S':
                case 's':
                    i++;
                    m = gpfSTR2I8(pWR->sIP+i);
                    pWR->aSTP[d] = m;
                    break;
                case 'M':
                case 'm':
                    i++;
                    if( pWR->aMx[d*4]>=3 )
                        continue;
                    m = gpfSTR2I8(pWR->sIP+i);

                    pWR->aMx[d*4]++;
                    pWR->aMx[d*4+pWR->aMx[d*4]] = m;
                    break;
                case 'C':
                case 'c':
                    i++;
                    m = gpfSTR2I8(pWR->sIP+i);
                    pWR->aC[d] = m;
                    continue;
                case 'R':
                case 'r':
                    i++;
                    pWR->aR[d] = strtod((char*)pWR->sIP+i, NULL);
                    continue;

                default:
                    continue;
            }
            pWR->aMAP[m] = c;
        }
    }
    if( pWR->msJOIN )
        pWR->trd.join();
    pWR->msJOIN = pWIN->mSEC.x+2000;
    pWR->trd = std::thread( gpfWIREtrd, pWR );
    return pWR;

    U4 reg = pWR->cnt, bK, ms = pWIN->mSEC.x&0xfffffffe;
    int d, bB;
    for( U4 iD = 0, w; iD < pWR->nD; iD++ ) {
        if( pWR->aAN[iD].num == pWR->aCNT[iD] )
            continue;
        d = pWR->aAN[iD].num - pWR->aCNT[iD]; /// TRG-NOW
        /// DIR ------------------------------------------
        w = pWR->aDIR[iD];
        if( pWR->aIOm[w] != OUTPUT ) {
           pinMode(aIOfree[w], OUTPUT);
           pWR->aIOm[w] = OUTPUT;
        }
        bB = (d>-1);
        if( (pWR->aIO[w]&1) != bB ) {
           digitalWrite(aIOfree[w], bB );
           pWR->aIO[w] = bB|ms;
        }
        /// STEP ------------------------------------------
        w = pWR->aSTP[iD];
        if( pWR->aIOm[w] != OUTPUT ) {
            pinMode(aIOfree[w], OUTPUT);
            pWR->aIOm[w] = OUTPUT;
        }
        if( bB ) {
            pWR->aCNT[iD]++;
        } else {
            pWR->aCNT[iD]--;
        }
        bB = pWR->aCNT[iD]&1;
        if( (pWR->aIO[w]&1) != bB ) {
           digitalWrite(aIOfree[w], bB );
           pWR->aIO[w] = bB|ms;
        }

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
