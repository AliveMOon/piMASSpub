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

#define gpdWRuSLP 512 //(1250*2) /// DVR8825 250kHz
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
        aPOSout[0x8]; //, aADD[0x28];
    F2  aPOT[0x28], aALT[0x14], aHLF[0x14];
    float aqALT[0x14];

    gpeALF  aA[0x28];
    I8x4    aCNT[0x28];
    I4x2    nLH[0x28], nTO[0x28], anDIV[0x28];
	gpcLZY	aLZYstp[0x28];

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
    I4 to( U1 c0, I8x2* pAN = NULL ) {
		aCNT[c0].z = 0;
		aCNT[c0].w=aCNT[c0].x;
		if( !pAN )
			return aCNT[c0].w;

		aA[c0] = pAN->alf;
		aCNT[c0].y = pAN->num;
		nTO[c0].x =
		nTO[c0].y =	aCNT[c0].y-(aCNT[c0].w=aCNT[c0].x);
		nLH[c0].x = ( anDIV[c0] = I4x2(nLZYstp(c0)) ).x;
		nLH[c0].y = nLH[c0].x*nLH[c0].x;
		return aCNT[c0].w;
	}
    F2* pP2in( U1 p = 1, U1 c0 = 0, U1 c1 = 1 ) {
        aPOT[p].cnt2pot(	aCNT[c0].x, aCNT[c1].x,
							aW[c0], aR[c0], aC[c0] );
        prg.inXYZ.a4x2[0] = aPOT[p] * wrX(1.0);
        return aPOT+p;
    }
    F2& out2to( U1 p = 1, U1 c0 = 0, U1 c1 = 1 ) {
		aPOT[1] = prg.ouXYZ.a4x2[0];
		aPOT[1] /= wrX(1.0);
		return pot2to( p, pP2in(p-1,c0,c1), c0, c1 );
    }
    int nLZYstp( U1 c0 ) {
		// lépés lista össze állítása
		U4x2 a8[8];
		gpmZ(a8);
		I4x2 stp(0, abs(nTO[c0].x));
		I4 n = 0, nN = 0, nM = 0;
		for( I4 x = 0x20; x > 1; x>>=1, n++ ) {
			a8[n].y = stp.YdivQR(x).x;
			if( !a8[n].y )
				continue;
			a8[n].x = x;
			nN += a8[n].y;
			nM = n+1;
		}
		if( stp.y ) {
			a8[n].y = stp.y;
			a8[n].x = 1;
			nN += a8[n].y;
			nM = n+1;
		}
		U1* pU1;
		I4 i,j;
		for( j = 0; j < nM; j++ ) {
			for( i = 0, j; i < nN; i++ ) {
				if( !a8[j].x )
					continue;
				pU1 = aLZYstp[c0].pU1n(-1);
				(*pU1) = a8[j].x; // pU1i(-1) inkrementál
				a8[j].y--;
				if( a8[j].y )
					continue;
				a8[j].x = 0;
			}
		}
		return aLZYstp[c0].nLD();
    }
    F2& pot2to( U1 p = 1, F2* pP0 = NULL, U1 c0 = 0, U1 c1 = 1 ) {

        aA[c0] = aA[c1] = gpeALF_POT;
        aCNT[c0].z = aCNT[c1].z = 0;
        // here save c0c1.x -> c0c1.w
        aCNT[c0].w = aCNT[c0].x;
        aCNT[c1].w = aCNT[c1].x;
        // to c0c1.y
        aPOT[p].pot2cnt(   aCNT[c0].y,aCNT[c1].y,
                           aW[c0], aR[c0], aC[c0]);
		// half ?
		nTO[c0].x =	aCNT[c0].y-aCNT[c0].w;
		nTO[c0].y =	aCNT[c1].y-aCNT[c1].w;
		nTO[c1] = nTO[c0].yx();
		nLH[c0].x = ( anDIV[c1] = I4x2(nLZYstp(c0)) ).x;
		nLH[c0].y = ( anDIV[c0] = I4x2(nLZYstp(c1)) ).x;

		nLH[c1] = nLH[c0].LH();
		nLH[c1].y *= nLH[c1].x; // kisebbel a nagyobbat y-ba
		nLH[c0] = nLH[c1]; // megoszt

		if( !pP0 )
			return aPOT[p];
		U1 p2 = p/2;
		aALT[p2].cnt2pot(	(nTO[c0].x/2)+aCNT[c0].w, (nTO[c1].x/2)+aCNT[c1].w,
							aW[c0], aR[c0], aC[c0] );

		aHLF[p2] = (aPOT[p]+(*pP0))/2.0;
		aqALT[p2] = (aALT[p2]-aHLF[p2]).qlen();

		return aPOT[p];
    }
	U4 STEP( U1 c0, U4 ms ) {
		if( !anDIV[c0].x )
			return 0;

		if(aCNT[c0].x==aCNT[c0].y) {
			aLZYstp[c0].lzyRST();
			return 0;
		}

		ms &= ~1;
		U1* pU1 = aLZYstp[c0].pU1();
		if( aIO[aSTP[c0]]&1 ) {
			// kint kéne tartani valameddig
			if( aLZYstp[c0].nLD() ) {
				aLZYstp[c0].n_load--;
				aCNT[c0].x += pU1[aLZYstp[c0].nLD()] * ((aIO[aDIR[c0]]&1) ? 1 : -1);
				digitalWrite(aIOm2w[aSTP[c0]],0);
				aIO[aSTP[c0]] = ms;
				return 0;
			}

			aCNT[c0].x = aCNT[c0].y;
			digitalWrite(aIOm2w[aSTP[c0]],0);
			aIO[aSTP[c0]] = ms;
			return 0;
		}

		U4 c41 = c0*4 + 1, d, w = aDIR[c0], w2, nSTP = aLZYstp[c0].nLD(), bB, mul, add; /// TRG-NOW
		if( !nSTP ) {
			// egy kis cCFG // megnézi OUTPUTok rendben
			w = aDIR[c0];
			if( aIOm[w] != OUTPUT ) {
			   pinMode(aIOm2w[w], OUTPUT);
			   aIOm[w] = OUTPUT;
			}
			w = aSTP[c0];
			if( aIOm[w] != OUTPUT ) {
				pinMode(aIOm2w[w], OUTPUT);
				aIOm[w] = OUTPUT;
				w2 = aMx[c41];
				if( aIOm[w2] != OUTPUT ) {
					pinMode(aIOm2w[w2], OUTPUT);
					aIOm[w2] = OUTPUT;
				}
				w2 = aMx[c41+1];
				if( aIOm[w2] != OUTPUT ) {
					pinMode(aIOm2w[w2], OUTPUT);
					aIOm[w2] = OUTPUT;
				}
				w2 = aMx[c41+2];
				if( aIOm[w2] != OUTPUT ) {
					pinMode(aIOm2w[w2], OUTPUT);
					aIOm[w2] = OUTPUT;
				}
			}
		}
		// NEW?
		aCNT[c0].z += nLH[c0].x;
		if( anDIV[c0].y == (aCNT[c0].z/anDIV[c0].x) )
			return 1;

		anDIV[c0].y = aCNT[c0].z/anDIV[c0].x;

		pU1 = aLZYstp[c0].pU1();
		/// DIR ------------------------------------------
		w = aDIR[c0];
		d = (nTO[c0].x>-1);
		if( (aIO[w]&1) != d ) {
		   digitalWrite(aIOm2w[w], d );
		   aIO[w] = d|ms;
		}

		/// CALC
		//aADD[c0] = 1;
		add = pU1 ? pU1[nSTP-1] : 1;
		switch( add ) {
			case 0x2:
				mul = 4;
				break;
			case 0x4:
				mul = 3;
				break;
			case 0x8:
				mul = 2;
				break;
			case 0x10:
				mul = 1;
				break;
			case 0x20:
				mul = 0;
				break;
			default:
				mul = 0x5;
				break;
		}

		/// MULL ------------------------------------------
		bB = mul&1;
		w2 = aMx[c41];			// 0
		if( (aIO[w2]&1) != bB ) {
			digitalWrite(aIOm2w[w2], bB );
			aIO[w2] = bB|ms;
		}

		bB = (mul>>1)&1;
		w2 = aMx[c41+1];		// 1
		if( (aIO[w2]&1) != bB ) {
			digitalWrite(aIOm2w[w2], bB );
			aIO[w2] = bB|ms;
		}
		bB = (mul>>2)&1;
		w2 = aMx[c41+2];		// 2
		if( (aIO[w2]&1) != bB ) {
			digitalWrite(aIOm2w[w2], bB );
			aIO[w2] = bB|ms;
		}

		/// STEP ---------------------------------------------
		w = aSTP[c0];
		digitalWrite(aIOm2w[w], 1 );
		aIO[w] = 1|ms;

		return 1;
	}

};
static const U1 gpaMUL[] = {
													7, //  0                               // végtelen hiba /32 belassul
													0, //  1                               / 1
													1, //  2                                   / 2
	2,												2, //  3  4                                    / 4
	3, 3, 3,										3, //  5  6  7  8                                  / 8
	4, 4, 4, 4, 4, 4, 4,							4, //  9 10 11 12 13 14 15 16                          /16
	5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6,	6, // 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32      /32
};
static const U4 gpaSTP[] = {
	32, // 0
	16, // 1
	8,  // 2
	4,  // 3
	2,  // 4
	1,  // 5
	1,  // 6
	1,  // 7
};
void gpfWIREtrd_o( gpcWIRE* pWR ) {

    int cnt = pWR->cnt, d, bD, bB2, iQ;
    I4x2 hlQR;
    U4 ms, iD, iD4, w, w2, nD = pWR->nD, nCNT = 1, two, aMULL[0x28], aADD[0x28];
    bool bQ;
    while( nCNT ) {
        ms = SDL_GetTicks()&(~1);
        bQ = true;
        for( two = 0; two < 2; two++ ) {
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
                bD = (d>-1);
                if( (pWR->aIO[w]&1) != bD ) {
                   digitalWrite(aIOm2w[w], bD );
                   pWR->aIO[w] = bD|ms;
                }

                /// STEP set OUT------------------------------------------
                w = pWR->aSTP[iD];
                if( pWR->aIOm[w] != OUTPUT ) {
                    pinMode(aIOm2w[w], OUTPUT);
                    pWR->aIOm[w] = OUTPUT;
                    iD4 = 1+iD*4;
                    w2 = pWR->aMx[iD4];
					if( pWR->aIOm[w2] != OUTPUT ) {
						pinMode(aIOm2w[w], OUTPUT);
						pWR->aIOm[w2] = OUTPUT;
					}
					w2 = pWR->aMx[iD4+1];
					if( pWR->aIOm[w2] != OUTPUT ) {
						pinMode(aIOm2w[w], OUTPUT);
						pWR->aIOm[w2] = OUTPUT;
					}
					w2 = pWR->aMx[iD4+2];
					if( pWR->aIOm[w2] != OUTPUT ) {
						pinMode(aIOm2w[w], OUTPUT);
						pWR->aIOm[w2] = OUTPUT;
					}
                }

                if( bQ ) {
                    aMULL[iD] = 0x7;    // M2M1M0
                    aADD[iD] = 1;
                }

                if( pWR->aCNT[iD].z >= pWR->nLH[iD].y ) {
					pWR->aCNT[iD].z = pWR->nLH[iD].y;
					pWR->aCNT[iD].x = pWR->aCNT[iD].y;
					continue;
                }
                else {
					if( pWR->aCNT[iD].z < pWR->nLH[iD].y ) {
						pWR->aCNT[iD].x = ((pWR->aCNT[iD].y-pWR->aCNT[iD].w)*pWR->aCNT[iD].z)/pWR->nLH[iD].y;
						pWR->aCNT[iD].x += pWR->aCNT[iD].w;
						pWR->aCNT[iD].z++;
					}
					else {
						pWR->aCNT[iD].z = pWR->nLH[iD].y;
						pWR->aCNT[iD].x = pWR->aCNT[iD].y;
					}
                }
				iQ = pWR->aCNT[iD].x&0x1;
				if( (pWR->aIO[w]&1) != iQ ) {
                    /// MULL ------------------------------------------
                    iD4 = 1+iD*4;

                    w2 = pWR->aMx[iD4];
                    bB2 = aMULL[iD]&1;
                    if( (pWR->aIO[w2]&1) != bB2 ) {
                        digitalWrite(aIOm2w[w2], bB2 );
                        pWR->aIO[w2] = bB2|ms;
                    }

                    w2 = pWR->aMx[iD4+1];
                    bB2 = (aMULL[iD]>>1)&1;
                    if( (pWR->aIO[w2]&1) != bB2 ) {
                        digitalWrite(aIOm2w[w2], bB2 );
                        pWR->aIO[w2] = bB2|ms;
                    }

                    w2 = pWR->aMx[iD4+2];
                    bB2 = (aMULL[iD]>>2)&1;
                    if( (pWR->aIO[w2]&1) != bB2 ) {
                        digitalWrite(aIOm2w[w2], bB2 );
                        pWR->aIO[w2] = bB2|ms;
                    }

                    /// STEP ---------------------------------------------
                    digitalWrite(aIOm2w[w], iQ );
                    pWR->aIO[w] = iQ|ms;
                }
            }
            usleep(gpdWRuSLP*4);
            bQ = false;
        }
        pWR->cnt++;
        if( pWR->msJOIN <= ms )
            break;
    }
}
void gpfWIREtrd( gpcWIRE* pWR ) {

    int cnt = pWR->cnt, d, bD, bB2, iQ;
    I4x2 hlQR;
    U4 ms, iD, iD4, w, w2, nD = pWR->nD, nCNT = 1, two, aMULL[0x28], aADD[0x28];
    bool bQ;
    while( nCNT ) {
        ms = SDL_GetTicks()&(~1);
        //bQ = true;
        nCNT = 0;
        //for( two = 0; two < 2; two++ ) {
		for( iD = 0, nCNT = 0; iD < nD; iD++ ) {
			nCNT += pWR->STEP(iD, ms );
		}
		//bQ = false;
        //}
        pWR->cnt++;
        if( pWR->msJOIN <= ms )
            break;
		usleep(gpdWRuSLP*64);
    }
}

void gpfWIREtrd_oo( gpcWIRE* pWR ) {

    int cnt = pWR->cnt, d, bD, bB2, iQ;
    I4x2 hlQR;
    U4 ms, iD, iD4, w, w2, nD = pWR->nD, nCNT = 1, two, aMULL[0x28], aADD[0x28];
    bool bQ;
    while( nCNT ) {
        ms = SDL_GetTicks()&(~1);
        bQ = true;
        for( two = 0; two < 2; two++ ) {
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
                bD = (d>-1);
                if( (pWR->aIO[w]&1) != bD ) {
                   digitalWrite(aIOm2w[w], bD );
                   pWR->aIO[w] = bD|ms;
                }

                /// STEP set OUT------------------------------------------
                w = pWR->aSTP[iD];
                if( pWR->aIOm[w] != OUTPUT ) {
                    pinMode(aIOm2w[w], OUTPUT);
                    pWR->aIOm[w] = OUTPUT;
                    iD4 = 1+iD*4;
                    w2 = pWR->aMx[iD4];
					if( pWR->aIOm[w2] != OUTPUT ) {
						pinMode(aIOm2w[w], OUTPUT);
						pWR->aIOm[w2] = OUTPUT;
					}
					w2 = pWR->aMx[iD4+1];
					if( pWR->aIOm[w2] != OUTPUT ) {
						pinMode(aIOm2w[w], OUTPUT);
						pWR->aIOm[w2] = OUTPUT;
					}
					w2 = pWR->aMx[iD4+2];
					if( pWR->aIOm[w2] != OUTPUT ) {
						pinMode(aIOm2w[w], OUTPUT);
						pWR->aIOm[w2] = OUTPUT;
					}
                }

                if( bQ ) {
                    aMULL[iD] = 0x7;    // M2M1M0
                    aADD[iD] = 1;
                }

                //if( pWR->aCNT[iD].z < pWR->nLH[iD].y ) {
				if( abs(pWR->aCNT[iD].y-pWR->aCNT[iD].w) < abs(pWR->aCNT[iD].x-pWR->aCNT[iD].w) ) {
					pWR->aCNT[iD].z = pWR->nLH[iD].y;
					pWR->aCNT[iD].x = pWR->aCNT[iD].y;
					continue;
				} else {
					if( bQ ) {
						I8	H = abs(d),
							hh = H/8;
						if( hh >= 1 ) {
							if( hh >= 2 ) {
								if( hh >= 4 ) {
									// 32
									aADD[iD] = 32;
									aMULL[iD] = 0;
								} else {
									// 16
									aADD[iD] = 16;
									aMULL[iD] = 1;
								}
							} else {
								// 8
								aADD[iD] = 8;
								aMULL[iD] = 2;
							}
						} else {
							hh = H/2;
							if( hh >= 1 ) {
								if( hh >= 2 ) {
									// 4
									aADD[iD] = 4;
									aMULL[iD] = 3;
								} else {
									// 2
									aADD[iD] = 2;
									aMULL[iD] = 4;
								}
							} else {
								// 1
								aADD[iD] = 1;
								aMULL[iD] = 6;
							}
						}

					}
					else if( bD )
						pWR->aCNT[iD].x += aADD[iD];
					else
						pWR->aCNT[iD].x -= aADD[iD];

                }

                iQ = bQ; //!(pWR->aCNT[iD].x&1);
                if( (pWR->aIO[w]&1) != iQ ) {
                    /// MULL ------------------------------------------
                    iD4 = 1+iD*4;

                    w2 = pWR->aMx[iD4];
                    bB2 = aMULL[iD]&1;
                    if( (pWR->aIO[w2]&1) != bB2 ) {
                        digitalWrite(aIOm2w[w2], bB2 );
                        pWR->aIO[w2] = bB2|ms;
                    }

                    w2 = pWR->aMx[iD4+1];
                    bB2 = (aMULL[iD]>>1)&1;
                    if( (pWR->aIO[w2]&1) != bB2 ) {
                        digitalWrite(aIOm2w[w2], bB2 );
                        pWR->aIO[w2] = bB2|ms;
                    }

                    w2 = pWR->aMx[iD4+2];
                    bB2 = (aMULL[iD]>>2)&1;
                    if( (pWR->aIO[w2]&1) != bB2 ) {
                        digitalWrite(aIOm2w[w2], bB2 );
                        pWR->aIO[w2] = bB2|ms;
                    }

                    /// STEP ---------------------------------------------
                    digitalWrite(aIOm2w[w], iQ );
                    pWR->aIO[w] = iQ|ms;
                }
            }
            usleep(gpdWRuSLP);
            bQ = false;
        }
        pWR->cnt++;
        if( pWR->msJOIN <= ms )
            break;
    }
}
static char gpsANSW[0x1000];
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
            char *pANSW = gpsANSW;
            if( pWR->bCTRL() )
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
            if( pANSW > gpsANSW ) {
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
                    pGTusr->pOUT = pGTusr->pOUT->lzyFRMT( s = -1, "\r\n %d.%s", pGTusr->iCNT, gpsANSW );
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
					pWR->to( iNUM, NULL );
                    /*pWR->aA[iNUM] = anC.alf;
                    pWR->aCNT[iNUM].z = 0;
                    pWR->aCNT[iNUM].w = pWR->aCNT[iNUM].x;*/
                    if(iNUM&1) {
						pWR->aPOT[iNUM].y = float(anC.num)/wrX(1.0);
						pWR->pot2to( iNUM, pWR->aPOT+iNUM-1, iNUM-1, iNUM );
                        /*
                        pWR->aPOT[1].pot2cnt(   pWR->aCNT[iNUM-1].y, pWR->aCNT[iNUM].y,
                                                pWR->aW[0], pWR->aR[0], pWR->aC[0]);
						pWR->nZ01[iNUM/2].x =  abs(pWR->aCNT[iNUM-1].y-pWR->aCNT[iNUM-1].w);
                        pWR->nZ01[iNUM/2].y =  abs(pWR->aCNT[iNUM  ].y-pWR->aCNT[iNUM  ].w);
                        pWR->nLH[iNUM  ] =
                        pWR->nLH[iNUM-1] = pWR->nZ01[iNUM/2].LH();*/
                        break;
                    }
                    pWR->pP2in(iNUM,iNUM,iNUM+1);
                    pWR->aPOT[1].x = float(anC.num)/wrX(1.0);
                } break;
            case gpeALF_PAINT:{
                    /// paint21sep22 - 1./ GTwireOS sw( anC.alf ) PAINT:

                } break;
            default:
				pWR->to(iNUM, &anC );
				/*pWR->aA[iNUM] = anC.alf;
				pWR->aCNT[iNUM].z = 0;
				pWR->aCNT[iNUM].w = pWR->aCNT[iNUM].x;

				pWR->nLH[iNUM].x =
				pWR->nLH[iNUM].y = abs(pWR->aCNT[iNUM].y-pWR->aCNT[iNUM].w);
				pWR->aCNT[iNUM].y = anC.num;*/
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
            pWR->out2to(); // oXY
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
                    I4x4* pRD, lurd = pWR->prg.jdPRGtool, wh;
                    if( !lurd.x )
                        lurd.x = gpdPAINTlfX;
                    if( !lurd.y )
                        lurd.y = gpdPAINTtpX;
                    if( !lurd.z )
                        lurd.z = gpdPAINTrgX;
                    if( !lurd.w )
                        lurd.w = gpdPAINTbtX;
                    wh.x = lurd.z-lurd.x;
                    wh.y = lurd.y-lurd.w;
                    //if( false )
                    {
                        bool bFRT = true;
                        for( I4 r = -nR, ir; r < 0; r++ ) {
                            xy = pRi[r];
                            xy.YdivRQ(w);
                            xy.x = ((xy.x*wh.x)/w) + lurd.x;
                            xy.y = lurd.y - ((xy.y*wh.y)/h);
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
                                xyA.x = ((xyA.x*wh.x)/w) + lurd.x;
                                xyA.y = lurd.y - ((xyA.y*wh.y)/h);
                                //xyA.x = ((xyA.x*gpdPAINTwX)/w) + gpdPAINTlfX;
                                //xyA.y = gpdPAINTtpX - ((xyA.y*gpdPAINThX)/h);
                                if( !base ) {
                                    x0B = xyA-xy;
                                    if( x0B.abs().mx() < wr2(3) )
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
                                if( alt < wr2(2) )
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
                            xy.x = ((xy.x*wh.x)/w) + lurd.x;
                            xy.y = lurd.y - ((xy.y*wh.y)/h);
                            //xy.x = ((xy.x*gpdPAINTwX)/w) + gpdPAINTlfX;
                            //xy.y = gpdPAINTtpX - ((xy.y*gpdPAINThX)/h);

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
