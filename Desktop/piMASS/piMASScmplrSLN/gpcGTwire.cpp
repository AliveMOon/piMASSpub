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
U1  aIOonPCB[] =    { 29,31,32,33,36,11,12,35,38,40,15,16,18,22,37,13, },
    aIObcm[] =      {  5, 6,12,13,16,17,18,19,20,21,22,23,24,25,26,27, },
    aIOfree[] =     { 21,22,26,23,27, 0, 1,24,28,29, 3, 4, 5, 6,25, 2, },

    aIOm2w[] = {
                    // //       //  3v3  + + 5v0
                    // //       //  SDA1 + + 5v0
                    // //       //  SCL1 + + GND
                    // //       //  1-Wir+ + TxD
                    // //       //  GND  + + RxD
                    00,01,      //  io00 + + io01
                    02,//       //  io02 + + GND
                    03,04,      //  io03 + + io04
                       05,      //  3V3  + + io05
                    // //       //  MOSI + + GND
                       06,      //  MISO + + io06
                    // //       //  Sclk + + CE0
                    // //       //  GND  + + CE1
                                //  SDA0 + + SCL0
                    21,//       //  io21 + + GND
                    22,26,      //  io22 + + io26
                    23,//       //  io21 + + GND
                    24,27,      //  io24 + + io27
                    25,28,      //  io25 + + io28
                       29,      //  GND  + + io29
            },
    aIOm2p[] = {    11,12,
                    13,//
                    15,16,
                       18,
                    // //
                       22,
                    // //
                    // //
                    // //
                    29,//
                    31,32,
                    33,//
                    35,36,
                    37,38,
                       39, };

#define gpdWRuSLP (1250*4)
#define gpdWRmSLP(n) ((gpdWRuSLP*n)/1000)
bool gpcPRGarm::bRUN( U4 mSEC, gpcGT* pGT, U4 nV ) {

    if( bCTRL() ) // mozog?
        return false; // igen mozog

    // ha megált  megnézi
	// program végrehajtásával foglalkozik?
    if( !jdPRG.x )
		return true;	// nem akor pihi

	// igen
    /// --------------------------
    /// 		END
    /// --------------------------
	if( jdPRG.y >= jdPRG.z ) {
        // de pont befejezte
        switch( jdALF ) {
			case gpeALF_PAINT:
				if( pGT ? !robROAD.nI4x4() : true ) {
                    if( nVER != nV ) // pl. pSRF ha új készül elvileg más lesz
                        return false;
					jdALF = gpeALF_null;
					break;
				}
                // paint addig megy mig van utánpotlás
                jdPRG.y = 0;    // azaz írány a start
				break;
            default: {
                    jdALF = gpeALF_null;
                } break;
        }

        if( !jdALF ) {
			jdALF = gpeALF_null;
			jdPRG.null();
			return true;
		}
    }

	// na nézzük a programot
	if( !jdPRG.y ) {
		/// ----------------------------
		///   		START
		/// ----------------------------
		nVER = nV;
		switch( jdALF ) {
            case gpeALF_PAINT:
                if( pGT ? robROAD.nI4x4() : false ){
                    lzyROAD.lzyRST();
                    lzyROAD = robROAD;
                    robROAD.lzyRST();
                }
                jdPRG.z = lzyROAD.nI4x4();
                jdPRG.y = 0;
                break;
            default:
				jdPRG.null();
				return true;
				break;
		}
		gpmMcpyOF( &stXYZ, &gdXYZ, 3 );
    }
    /// ----------------------------
	///   		STEP
	/// ----------------------------
	switch( jdALF ) {
		case gpeALF_PAINT: {
            /// paint21sep22 - 3./ STEP bRUN{ sw( jdALF )
            I4x4* pDOT = lzyROAD.pI4x4( jdPRG.y );
            if( !pDOT ){
                jdPRG.y = jdPRG.z;
                break;
            }
            tgXYZ = *pDOT;
        } break;
		default:
            jdPRG.null();
			return true;
			break;
    }
    jdPRG.y++;
	return true;
}
class gpcWIRE {
public:
    U4 aIO[0x28],aIOd[0x28], cnt;
    int aIOm[0x28],
        aPOSin[0x8],
        aPOSout[0x8];
    F2  aPOT[0x28];

    gpeALF  aA[0x28];
    I8      nZ[0x28];
    I8x4    aCNT[0x28];

    U1  aMAP[0x28],
        sIP[0x100],
        aDIR[0x4], nD,
        aSTP[0x4],
        aMx[0x10];

    U4      aC[0x4], msJOIN;
    float   aR[0x4], aW[0x4];

    I4x2 nLEN;
    /// paint21sep22 - class gpcWIRE
    gpcPRGarm   prg;

    std::thread trd;

    gpcWIRE(){ gpmCLR; };
    U4 setCTRL( U1 iD ){ return this ? prg.setCTRL(iD) : 0; }
    U4 rstCTRL() { return this ? prg.rstCTRL() : 0; }
    bool bCTRL() { return this ? prg.bCTRL() : false; }
};
void gpfWIREtrd( gpcWIRE* pWR ) {

    int cnt = pWR->cnt, d, bB;
    U4 ms, iD, w, nD = pWR->nD, nCNT = 1, two;
    while( nCNT ) {
        ms = SDL_GetTicks()&(~1);
        for( two = 0; two < 2; two++ ){
            for( iD = 0, nCNT = 0; iD < nD; iD++ ) {
                if( pWR->aCNT[iD].y == pWR->aCNT[iD].x )
                    continue;
                nCNT++,
                d = pWR->aCNT[iD].y - pWR->aCNT[iD].x; /// TRG-NOW
                /// DIR ------------------------------------------
                w = pWR->aDIR[iD];
                if( pWR->aIOm[w] != OUTPUT ) {
                   pinMode(aIOm2w[w], OUTPUT);
                   pWR->aIOm[w] = OUTPUT;
                }
                bB = (d>-1);
                if( (pWR->aIO[w]&1) != bB ) {
                   digitalWrite(aIOm2w[w], bB );
                   pWR->aIO[w] = bB|ms;
                }

                /// STEP ------------------------------------------
                w = pWR->aSTP[iD];
                if( pWR->aIOm[w] != OUTPUT ) {
                    pinMode(aIOm2w[w], OUTPUT);
                    pWR->aIOm[w] = OUTPUT;
                }

                if( pWR->aCNT[iD].z < pWR->nZ[iD] ){
                    pWR->aCNT[iD].x = ((pWR->aCNT[iD].y-pWR->aCNT[iD].w)*pWR->aCNT[iD].z)/pWR->nZ[iD];
                    pWR->aCNT[iD].x += pWR->aCNT[iD].w;
                    pWR->aCNT[iD].z++;
                }
                else {
                    pWR->aCNT[iD].z = pWR->nZ[iD];
                    pWR->aCNT[iD].x = pWR->aCNT[iD].y;
                }
                bB = !(pWR->aCNT[iD].x&1);
                if( (pWR->aIO[w]&1) != bB ) {
                   digitalWrite(aIOm2w[w], bB );
                   pWR->aIO[w] = bB|ms;
                }
            }
            usleep(gpdWRuSLP);
        }
        pWR->cnt++;
        if( pWR->msJOIN <= ms )
            break;
    }
}
gpcWIRE* gpcGT::GTwire( gpcWIN* pWIN, int msRUN ) {

    gpcLZY  *pLZYinp = pWIN->piMASS->GTlzyALL.LZY(gpdGTlzyIDinp(TnID)),
            *pLZYusr = NULL;

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

        for( U4 i = 0, n = gpmN(aIOm2w); i < n; i++ ) {
            if( pWR->aIOm[aIOm2w[i]] != OUTPUT ) {
               pinMode(aIOm2w[i], OUTPUT);
               pWR->aIOm[aIOm2w[i]] = OUTPUT;
            }
            digitalWrite(aIOm2w[i], 0 );
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
                case 'W':
                case 'w':
                    i++;
                    pWR->aW[d] = strtod((char*)pWR->sIP+i, NULL);
                    continue;

                default:
                    continue;
            }
            pWR->aMAP[m] = c;
        }
    }
    if( pWR->msJOIN ) {
        pWR->trd.join();
        iCNT++;
        if( gpcGTall* pALL = pWIN->piMASS ? &pWIN->piMASS->GTacpt : NULL ) {
            char sANSW[0x100], *pANSW = sANSW;
            if( pWR->bCTRL() )
            //if( pWR->prg.jdPRG.x )
            if( pWR->aPOT[1] != pWR->aPOT[0] )
                pANSW += sprintf( pANSW, "%0.1f,%0.1f < %0.1f,%0.1f", pWR->aPOT[1].x, pWR->aPOT[1].y, pWR->aPOT[0].x, pWR->aPOT[0].y );

            pWR->rstCTRL();
            for( U4 iD = 0, w; iD < pWR->nD; iD++ ) {
                if( !pWR->aA[iD] )
                    continue;
                if( pWR->aCNT[iD].y == pWR->aCNT[iD].x )
                    continue;
                pWR->setCTRL(iD);
                //pWR->iCTRL.z |= 1<<iD;
                pANSW += sprintf( pANSW, "%lld<%lld %0.1f<%0.1f",pWR->aCNT[iD].y, pWR->aCNT[iD].x );
            }
            if( pANSW > sANSW ) {
                gpcGT *pGTusr = NULL;
                U4 nSOCK = 0;
                SOCKET	*pSOCK = gpmLZYvali( SOCKET, pLZYusr );
                if( !pSOCK ) {
                    pLZYusr = pWIN->piMASS->GTlzyALL.LZY( gpdGTlzyIDusr(TnID) );
                    pSOCK = gpmLZYvali( SOCKET, pLZYusr );
                }
                nSOCK = gpmLZYload( pLZYusr, SOCKET );

                for( U4 iS = 0; iS < nSOCK; iS++ ) {
                    pGTusr = pALL->GT( pSOCK[iS] );
                    if( pGTusr->bGTdie() )
                        continue;
                    pGTusr->pOUT = pGTusr->pOUT->lzyFRMT( s = -1, "\r\n %d.%s", pGTusr->iCNT, sANSW );
                    pGTusr->GTback();
                }
            }
        }
    }
    int add = msRUN ? pWIN->mSEC.x-msRUN : gpdWRmSLP(100);
    pWR->msJOIN = SDL_GetTicks() // pWIN->mSEC.x
                    + add;
    pWR->trd = std::thread( gpfWIREtrd, pWR );

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
	U4  &comA = *(U4*)sCOM, // nagy/kisbetut lehet megvizslatni
        iNUM = 0, nNUM, iE = 2;

	///-----------------------------
	/// UJ felhasználó?
	///-----------------------------
	gpcLZY		//*pLZYout = NULL,
				*pLZYusr = pMASS->GTlzyALL.LZY( gpdGTlzyIDusr(TnID) );
    U4 iSOCK = 0, nSOCK = 0;
    if( SOCKET* pSOCK = gpmLZYvali( SOCKET, pLZYusr) ) {
        nSOCK = gpmLZYload(pLZYusr,sockUSR);
        for( iSOCK = 0; iSOCK < nSOCK; iSOCK++ ) {
            if( pSOCK[iSOCK] != sockUSR )
                continue;
            // nem új bent van a listában
            break;
        }
    }
    if( iSOCK >= nSOCK ) {
        // új felhasználó!
        pLZYusr->lzyADD( &sockUSR, sizeof(sockUSR), s = -1 );
        iSOCK = nSOCK;
        nSOCK = gpmLZYload(pLZYusr,sockUSR);
    }
	///-----------------------------

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
        pNUM = pSTR;
        anC.num = gpfSTR2I8( pNUM, &pSTR );
		if( an.alf ) {
			comA = *(U4*)pCOM;
			iNUM = gpeDRCos_NONS;
			switch( an.alf ) {
				case gpeALF_STAT:
					bSTAT = true;
					break;
                case gpeALF_STOP:
					pWR->prg.jdPRG.x = 0;
					break;
				case gpeALF_POS:
					iNUM = 0;
					anC.alf = an.alf;
					bSTAT = true;
					break;
                case gpeALF_POT:
					iNUM = 0;
					anC.alf = an.alf;
					bSTAT = true;
					break;
                case gpeALF_PAINT:
                    iNUM = 0x10;
                    bSTAT = true;
                    /// pain 01. OS  // pWR->prg.jdPRG.x = 1; jdALF = an.alf;
                    pWR->prg.jdPRG.x = 1;
                    pWR->prg.jdALF = an.alf;
                    break;
				default:
                    break;

			}
		}

        if( iNUM >= 0x10 ) {
            /// pain 02. OS  // pWR->prg.jdPRGtool
            pWR->prg.jdPRGtool.aXYZW[iNUM&0x3] = anC.num;
        }
		else switch( anC.alf ) {
            case gpeALF_null:
                continue;
            case gpeALF_POT: {
                    pWR->aA[iNUM] = anC.alf;
                    pWR->aCNT[iNUM].z = 0;
                    pWR->aCNT[iNUM].w = pWR->aCNT[iNUM].x;
                    if(iNUM&1) {
                        pWR->aPOT[1].y = float(anC.num)/10.0;
                        pWR->aPOT[1].pot2cnt(   pWR->aCNT[iNUM-1].y, pWR->aCNT[iNUM].y,
                                                pWR->aW[0], pWR->aR[0], pWR->aC[0]); //, PIp2*(22.0/20.0) );
                        pWR->nZ[iNUM-1] =  abs(pWR->aCNT[iNUM-1].y-pWR->aCNT[iNUM-1].w);
                        pWR->nZ[iNUM  ] =  abs(pWR->aCNT[iNUM  ].y-pWR->aCNT[iNUM  ].w);
                        if( pWR->nZ[iNUM-1] < pWR->nZ[iNUM  ] )
                            pWR->nZ[iNUM-1] = pWR->nZ[iNUM  ];
                        else
                            pWR->nZ[iNUM  ] = pWR->nZ[iNUM-1];
                        break;
                    }
                    pWR->aPOT[0].cnt2pot(   pWR->aCNT[0].x, pWR->aCNT[1].x,
                                            pWR->aW[0], pWR->aR[0], pWR->aC[0] );
                    pWR->aPOT[1].x = float(anC.num)/10.0;
                } break;
            case gpeALF_PAINT:{
                    /// paint21sep22 - 1./ GTwireOS sw( anC.alf ) PAINT:

                } break;
            default:
                pWR->aA[iNUM] = anC.alf;
                pWR->aCNT[iNUM].z = 0;
                pWR->aCNT[iNUM].w = pWR->aCNT[iNUM].x;

                pWR->aCNT[iNUM].y = anC.num;
                pWR->nZ[iNUM] = abs(pWR->aCNT[iNUM].y-pWR->aCNT[iNUM].w);
                break;
		}


        iNUM++;
		if( iNUM >= gpmN(pWR->aA) )
            break;
		//break;
	}

	if(!bSTAT)
        return pANS->lzyFRMT( s = -1, "nonsens" );

    char sAN[0x80];
    for( U4 iD = 0, w; iD < pWR->nD; iD++ ) {
        if( !pWR->aA[iD] )
            continue;
        gpfALF2STR( sAN, pWR->aA[iD] );
        pANS = pANS->lzyFRMT( s = -1,   "%s:%lld ",
                                        sAN,
                                        pWR->aCNT[iD].x );
    }
    return pANS->lzyFRMT( s = -1, "nonsens" );
}

#define gpdID pWR->prg.aROBpID
#define gpdRD pWR->prg.lzyRD
#define gpdRM pWR->prg.pRM
#define gpdROBrd pWR->prg.robROAD
#define gpdSRF pWR->prg.pROBsrf
#define gpdDIF pWR->prg.pROBdif
I4 gpMEM::instDOitWIRE( gpcGT* pGT ) {
	if( this ? !pGT : true )
		return -1;

	I4 cnt = pGT->iCNT;
	gpcWIRE	*pWR = pGT->GTwire( pWIN, msRUN );
	if( cnt == pGT->iCNT )
		return cnt;

    if( !pGT->pGTm )
        return cnt;
    /// ----------------------------------------
	///					rPIC
	/// ----------------------------------------
	/// paint21sep22 - 2./ instDOitWIRE rPIC
	/// pain 03. // instDOitWIRE pGT->pGTm?
	gpOBJ *pOpic = pOBJ(gpeALF_RPIC);
    U1* pU1 = pOpic->pU1();
    if( !pU1 )
        return pGT->iCNT;

    gpdID[0]    = pOpic->bUTF8()
                ? pMASS->PIC.alfFND( (pU1+=gpmNINCS(pU1," \t\"")) )
                : *(I4*)pU1;
    gpcPIC* pPIC = pMASS->PIC.PIC( gpdID[0] );
    SDL_Surface* pSRF = pPIC->pPICrtxSRF();

	if( pWR->prg.bRUN( pWIN->mSEC.x, pGT, pPIC ? pPIC->nCPY : 0 ) ) {
        if( pWR->prg.ouXYZ != pWR->prg.tgXYZ ) {
            pWR->prg.ouXYZ = pWR->prg.tgXYZ;
            for( U1 iNUM = 0; iNUM < 2; iNUM++ ) {
                pWR->aA[iNUM] = gpeALF_POT;
                pWR->aCNT[iNUM].z = 0;
                pWR->aCNT[iNUM].w = pWR->aCNT[iNUM].x;
                if(iNUM&1) {
                    pWR->aPOT[1].y = float(pWR->prg.ouXYZ.y)/100.0;
                    pWR->aPOT[1].pot2cnt(   pWR->aCNT[iNUM-1].y, pWR->aCNT[iNUM].y,
                                            pWR->aW[0], pWR->aR[0], pWR->aC[0]); //, PIp2*(22.0/20.0) );
                    pWR->nZ[iNUM-1] =  abs(pWR->aCNT[iNUM-1].y-pWR->aCNT[iNUM-1].w);
                    pWR->nZ[iNUM  ] =  abs(pWR->aCNT[iNUM  ].y-pWR->aCNT[iNUM  ].w);
                    if( pWR->nZ[iNUM-1] < pWR->nZ[iNUM  ] )
                        pWR->nZ[iNUM-1] = pWR->nZ[iNUM  ];
                    else
                        pWR->nZ[iNUM  ] = pWR->nZ[iNUM-1];
                    break;
                }

                pWR->aPOT[0].cnt2pot(   pWR->aCNT[0].x, pWR->aCNT[1].x,
                                        pWR->aW[0], pWR->aR[0], pWR->aC[0] );
                pWR->prg.inXYZ.a4x2[0] = pWR->aPOT[0]*100.0;
                pWR->aPOT[1].x = float(pWR->prg.ouXYZ.x)/100.0;
            }
        }
        return cnt;
	}

    if( !pSRF )
        gpdID[0] = -1;
    else {
        if( gpdSRF ? (gpdSRF->w != pSRF->w  || gpdSRF->h != pSRF->h ) : false ) {
            gpmSDL_FreeSRF(gpdSRF);
            gpmSDL_FreeSRF(gpdDIF);
            gpmDELary( gpdRM );
        }

        if( !gpdSRF ) {
            gpdSRF = SDL_CreateRGBSurface( 0, pSRF->w, pSRF->h, 32, 0,0,0,0 );
            gpdDIF = SDL_CreateRGBSurface( 0, pSRF->w, pSRF->h, 32, 0,0,0,0 );
        }
        if( gpdSRF ) {
            U4	*apU4[3] = { (U4*)pSRF->pixels, (U4*)gpdSRF->pixels, (U4*)gpdDIF->pixels },
                n = pSRF->w*pSRF->h, i, nD = 0;

            for( U4 i = 0; i < n; i++ ) {
                if( apU4[1][i] == apU4[0][i] ) {
                    apU4[2][i] = 0 ;
                    continue;
                }
                apU4[2][i] = (apU4[1][i] = apU4[0][i]);
                nD++;
            }
            if( nD ) {
                if( !gpdRM )
                    gpdRM = new U4[n];
                gpdRM;
                gpmZnOF( gpdRM, n );
                I1		dir = 1;
                I4 		w = pSRF->w, h = pSRF->h,
                        aR[] = {-w, 0,w, 0},
                        aD[] = {-w,+1,w,-1},
                        bx, by, //rg = aD[2]/2,
                        b,s;

                I4x2	*pRi, xy, xyB, xyA, xAB, x0B, x0A;
                U4		in = 0xffffffff, n_stp = 0,
                        i = w+1, nn = n-i;
                bool b_pre = true, bIN;


                for( ; i < nn; i++ ) {
                    if( gpdRM[i] || (apU4[2][i]!=in) ) continue;

                    for( ; i < nn; i++ )
                    if( apU4[2][i]==in ) continue;
                    else break;

                    if( i >= nn )
                        break;

                    gpdRD.lzyRST();
                    dir = 1;
                    gpdRM[s] = s = b = i;
                    b++;
                    by = s-s%w;
                    b_pre = true;
                    while( s != b ) { //s == b ? dir != 2 : true ) {
                        bIN = false;
                        bx=b-by;

                        if(bx>=0)
                        if(bx<w) 			// balra van
                        if(b>=0)			// lent van
                        if(b>=by)			// jobra van
                        if(b<n)				// fent van
                        if( (!gpdRM[b]) || (gpdRM[b]==s) )
                        if( bIN = (apU4[2][b]==in) )
                                gpdRM[b]=s;


                        if( dir&1 )
                        if( bIN != b_pre ) {
                            pRi = gpdRD.pI4x2n(-1);
                            *pRi = I4x2( gpdRD.nI4x2()-1, b );
                        }

                        if( bIN ) {
                            if( bIN != b_pre )
                                n_stp++;
                            dir--;
                            if(dir<0)
                                dir = 3;
                        } else {
                            dir++;
                            dir %= 4;
                        }

                        b_pre=bIN;
                        b	+= aD[dir];
                        by	+= aR[dir];
                    }
                    /// ROAD
                    I4	nR = gpdRD.nI4x2(), nRD = 0, base, alt;
                    pRi = gpdRD.pI4x2n( nR, nR+1);
                    I4x4* pRD;
                    //if( false )
                    {
                        bool bFRT = true;
                        for( I4 r = -nR, ir; r < 0; r++ ) {
                            xy = pRi[r];
                            xy.YdivRQ(w);
                            xy.x = ((xy.x*gpdPAINTwX)/w) + gpdPAINTlfX;
                            xy.y = gpdPAINTtpX - ((xy.y*gpdPAINThX)/h);
                            if( bFRT ) {
                                bFRT = false;
                                (pRD = gpdROBrd.pI4x4n(-1))->a4x2[0] = xy; pRD->z = gpdPAINTupX; pRD->w = 0;
                                nRD++;
                            }
                            (pRD = gpdROBrd.pI4x4n(-1))->a4x2[0] = xy; pRD->z = gpdPAINTdwX; pRD->w = 0;
                            nRD++;
                            xyB = xy;
                            base = alt = 0;
                            for( ir = r+1; ir < 0; ir++ ) {
                                xyA = pRi[ir];
                                xyA.YdivRQ(w);
                                xyA.x = ((xyA.x*gpdPAINTwX)/w) + gpdPAINTlfX;
                                xyA.y = gpdPAINTtpX - ((xyA.y*gpdPAINThX)/h);
                                if( !base ) {
                                    x0B = xyA-xy;
                                    if( x0B.abs().mx() < mmX(3) )
                                        continue;
                                    r = ir;
                                    base = x0B.abs().mx();
                                    continue;
                                }
                                x0A = xyA-xy;
                                xAB = (x0B*(x0A*x0B));
                                xAB /= (base*base);
                                x0A -= xAB;
                                alt = x0A.abs().mx();
                                if( alt < mmX(2) )
                                    continue;


                                (pRD = gpdROBrd.pI4x4n(-1))->a4x2[0] = xyA; pRD->z = gpdPAINTdwX; pRD->w = 0;
                                nRD++;
                                r = ir;
                                break;
                            }
                        }
                        if( nRD ) {
                            xy = pRi[-nR];
                            xy.YdivRQ(w);
                            xy.x = ((xy.x*gpdPAINTwX)/w) + gpdPAINTlfX;
                            xy.y = gpdPAINTtpX - ((xy.y*gpdPAINThX)/h);

                            (pRD = gpdROBrd.pI4x4n(-1))->a4x2[0] = xy;
                            pRD->z = gpdPAINTdwX; pRD->w = 0;
                            nRD++;
                            (pRD = gpdROBrd.pI4x4n(-1))->a4x2[0] = xy;
                            pRD->z = gpdPAINTupX; pRD->w = 0;
                            nRD++;
                        }
                    }
                    /// FILL
                    pRi[-nR].median(nR, pRi, true );
                    for( I4 r = -nR; r < 0; r += 2 ) {
                        gpfMset( gpdRM+pRi[r].y, pRi[r+1].y-pRi[r].y, &s, sizeof(s) );
                    }
                }
            }
        }
    }


    return pGT->iCNT;
}
