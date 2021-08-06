#include "gpcSRC.h"
#include "gpcSRCinst.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];
extern char gpsPUB[0x1000];


I4 gpMEM::instDOitSLMP( gpcGT* pGT ) {
	if( this ? !pGT : true )
		return -1;

	I4 cnt = pGT->iCNT;
	pGT->GTcnct( pWIN );
	if( cnt == pGT->iCNT )
		return cnt;

#ifdef stdON
	if(bSTDcout){gpdCOUT << stdALU "SLMP" << pGT->iCNT;}
#endif
	gpOBJ	*pOi = pOBJ(gpeALF_RINP),
			*pOo = pOBJ(gpeALF_ROUT), *pOpic = pOBJ(gpeALF_RPIC),
			*apO[2];
    apO[0] = pOBJ(gpeALF_BILL),
    apO[1] = pOBJ(gpeALF_JOHN);

	I8x2 an;
	U1* pU1;
	gpcSRC* pS2;
	U4 xfnd;
	U8 s;
	gpcLZY *pLZYinp = pMASS->GTlzyALL.LZY(gpdGTlzyIDinp(pGT->TnID));
	if( !pLZYinp )
		return pGT->iCNT;

	gpcROBnD *pROBnD = gpmLZYvali( gpcROBnD, pLZYinp );
	if( !pROBnD )
		return pGT->iCNT;
	int iASY = -1;
	/// ----------------------------------------
	///					rINP
	/// ----------------------------------------
	if( pOi ) {
	#ifdef stdON
		if(bSTDcout){gpdCOUT << stdALU "rINP" << gpdENDL;}
    #endif
		pU1 = pOi->pU1(); // pSRC->srcMEMiPC( pOi->iPC, gpeCsz_l );
		if( pU1 ) {
			an.x = *(U4*)pU1;
			an.y = an.x&0xff;
			an.x >>= 0x10;

			for( U1 i = 0; i < 2; i++ ) {
				xfnd = pMASS->getXFNDan( an+I8x2( i, 0 ) );
				pS2 = xfnd ? pMASS->srcFND( xfnd ) : NULL;
				if( !pS2 )
                    continue;
                pS2->pMINI = pS2->pMINI->lzyFRMT( s=0, "\r\n" ); //\r\n" );
                pS2->pMINI = pS2->pMINI->lzyROBnDstat( s=0, *pROBnD, i, gpdHEADmmX, "" );

                if( pROBnD->aDrc[i].asyncSYS( gpsPUB, apO[i]->pU1() ) )
					iASY = i;
			}
		}
	}

	if( pOpic )
	/// ----------------------------------------
	///					rPIC
	/// ----------------------------------------
	if( !gpdROBrd.nLD() ) {
		pU1 = pOpic->pU1();
		if( pU1 ) {
			gpdID[0] = pOpic->bUTF8()
							? pMASS->PIC.alfFND( (pU1+=gpmNINCS(pU1," \t\"")) )
							: *(I4*)pU1;
			gpcPIC* pPIC = pMASS->PIC.PIC( gpdID[0] );
			SDL_Surface* pSRF = pPIC->pPICrtxSRF();
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
		}
		#ifdef stdON
			if(bSTDcout){gpdCOUT << stdALU "rPIC " << gpdROBrd.nLD() <<gpdENDL;}
		#endif
	}

	if( !pOo )
		return pGT->iCNT;
	/// ----------------------------------------
	///					rOUT
	/// ----------------------------------------

#ifdef stdON
	if(bSTDcout){gpdCOUT << stdALU "rOUT" << gpdENDL;}
#endif
	pU1 = pOo->pU1(); // pSRC->srcMEMiPC( pOo->iPC, gpeCsz_l );
	if( !pU1 )
		return pGT->iCNT;
	an.x = *(U4*)pU1;
	an.y = an.x&0xff;
	an.x >>= 0x10;

	xfnd = pMASS->getXFNDan( an );
	pS2 = xfnd ? pMASS->srcFND( xfnd ) : NULL;
	if( !pS2 )
		return pGT->iCNT;
	gpcLZY *pLZYhex = pWIN->piMASS->GTlzyALL.LZY(gpdGTlzyIDinp(pGT->TnID));
	if( !pLZYhex->nLD() )
		return pGT->iCNT;
	pS2->pMINI = pS2->pMINI->lzyFRMT( s=0, "\r\n" ); //\r\n" );
	pS2->pMINI = pS2->pMINI->lzyHEXl( s = 0, pLZYhex->p_alloc, pLZYhex->n_load, false );

	return pGT->iCNT;
}
#define gpdCTRL (pCTRL?pCTRL:(pCTRL = new gpCTRL))

#define gpdGL (pMgl?pMgl:(pMgl = new gpGL))
//#define gpdGLpVTX	gpdGL->pVTX
//#define gpdGLpPIX	gpdGL->pPIX
#define gpdGLmskPIC	gpdGL->mskPIC
#define gpdGLapPIC	gpdGL->apPIC
#define gpdGLpTRG	gpdGL->pTRG
#define gpdGLpTRG	gpdGL->pTRG
#define gpdGLpTRGxy	gpdGL->trgWH.a4x2[0]
#define gpdGLpTRGwh	gpdGL->trgWH.a4x2[1]
#define gpmGLnCNL 	gpdGL->lzyCNL.nLD(sizeof(F4))
#define gpmGLaCNL( a ) ((float*)gpdGL->lzyCNL.Ux( (a), sizeof(F4) ))
#define gpmGLaCNL4( a ) ((F4*)gpdGL->lzyCNL.Ux( (a), sizeof(F4) ))
#define gpdGLaPICid gpdGL->aPICid
#define gpdGLpCAM (gpdGL->pCAM?gpdGL->pCAM:(gpdGL->pCAM = new gpcPICAM))


I4 gpMEM::instDOit( gpOBJ& obj, U1* pU1 ) {
	bool bCID = this ? !pU1 : true;
	gpcGT* pGT;
	if( obj.cAN != gpeCsz_a )
        return gpeCsz_OFF;


	SDL_Texture* apTX[0x10];
	U1 sSHF[0x8];
	U8 nLEN;
	U8& shuffle = *(U8*)sSHF;
	I4 cID = gpeCsz_OFF;
	/// GLSL CHANNALs -------------------------------------------------------
	if((obj.AN.alf == gpeALF_CNL)||(obj.AN.alf >= gpeALF_CNLA && obj.AN.alf <= gpeALF_CNLZ)) {
		U1 iCNL = (obj.AN.alf==gpeALF_CNL ) ? 0 : obj.AN.alf-gpeALF_CNLA+1;
		cID = gpeCsz_L; if( bCID )
							return cID;
		// ix id dist 'xyr' x, y, r
		// 1 0x0003 29 xyr: 131, 471, 30
		I4 ix, dist, nCNL = 0, i; I8 id;
		if( obj.bUTF8() ) {
			U1* pS = pU1+1, *pSe = pS+gpmVAN( pS, "\"", nLEN ), *pSx;
			for( pS += gpmNINCS(pS," \t\r\n"); pS < pSe; pS += gpmNINCS(pS," \t\r\n") ) {
				ix = gpfSTR2I8(pS,&pS);
				id = gpfSTR2I8(pS,&pS);
				pS += gpmNINCS(pS," \t");
				if( *pS == '+' )
					pS += gpmNINCS(pS,"+");
				else
					dist = gpfSTR2I8(pS,&pS);

				pS += gpmNINCS(pS," \t");
				pSx = pS;
				pS = pSx+gpmVAN(pS,": \t\r\n", nLEN);

				shuffle = 0;
				gpmMcpy(sSHF,pSx, gpmMIN(4,pS-pSx) );
				pSx = pS;
				pS += gpmVAN(pS,"\r\n", nLEN);
				float* pF = gpmGLaCNL(nCNL);
				for( pSx += gpmNINCS(pSx,": \t\r\n"), i=0; pSx < pS; pSx += gpmNINCS(pSx,", \t\r\n") ) {
					pF[i] = gpfSTR2I8(pSx, &pSx);
					if(*pSx == '.')
					if( pF[i] < 0.0 )
						pF[i] -= gpmSTR2D(pSx);
					else
						pF[i] += gpmSTR2D(pSx);
					i++;
				}

				nCNL++;
				if( pMgl ) pMgl->nCNL = nCNL;
			}
		}
		return cID;
	}
	/// GLSL Target COPY -------------------------------------------------------
	if((obj.AN.alf == gpeALF_TRGCP)||(obj.AN.alf >= gpeALF_TRGCPA && obj.AN.alf <= gpeALF_TRGCPZ)) {
		cID = gpeCsz_L; if( bCID )
							return cID;
		U4 iP = (obj.AN.alf==gpeALF_TRGCP ) ? 0 : obj.AN.alf-gpeALF_TRGCPA+1;
		int trig = *(I4*)pU1;
		if( trig != 1 )
			return cID;

		if( !gpdGLpTRG )
			return cID;
		SDL_Surface	//*pTRG = gpdGLpTRG->pPICrtxSRF(),
					*pSRF = gpdGLapPIC[iP]->pPICrtxSRF(); //pSRF;
		if( !pSRF )
			return cID;

		glReadPixels(	0, 0,
				pSRF->w, pSRF->h,
				GL_BGRA,
				GL_UNSIGNED_BYTE,
				pSRF->pixels );
		//gpdGLpTRG->pREF = NULL;
		gpdGLapPIC[iP]->pREF = NULL;
		return cID;
	}
	/// GLSL SET TETURE -------------------------------------------------------
	if((obj.AN.alf == gpeALF_PIC)||(obj.AN.alf >= gpeALF_PICA && obj.AN.alf <= gpeALF_PICZ)) {
		cID = gpeCsz_L; if( bCID )
							return cID;

		U4 iP = (obj.AN.alf==gpeALF_PIC ) ? 0 : obj.AN.alf-gpeALF_PICA+1;
		U1* pS = pU1;
		I4 picID 	= obj.bUTF8()
					? pMASS->PIC.alfFND( (pS+=gpmNINCS(pS," \t\"")) )
					: *(I4*)pU1;
		gpcPIC* pPIC = pMASS->PIC.PIC( picID );
		if( pPIC ) {
			gpdGLapPIC[iP] = pMASS->PIC.PIC( picID );
			if( !gpdGLapPIC[iP] )
				return cID;

			gpdGLmskPIC |= 1<<iP;
			gpdGLaPICid[iP] = picID+1;
			return cID;
		}
		if( !*pS )
			return cID;

		I8x2 alfN(0,14);

		alfN = pS;
		alfN.num = gpfSTR2U8( pS+alfN.num, &pS );
		gpdGLapPIC[iP] = pMASS->PIC.PIC( alfN );

		return cID;
	}

	switch( obj.AN.alf )
	{
		/// --------------------------------------------------------------------------
		/// LISTENER
		case gpeALF_TELNET: {								cID = gpeCsz_L; if( bCID ) break;
			I4 port = *(U4*)pU1;
			if( port < 1 ) break;
			pGT = pMASS->GTacpt.GT( obj.AN.alf, port );
			if( !pGT ) break;
			#ifdef stdON
			if(bSTDcout){gpdCOUT << stdALU " TNET" << gpdENDL;}
			#endif
			pGT->GTlst( pWIN, pMASS->GTcnct );
		} break;
		case gpeALF_GSM: {									cID = gpeCsz_b; if( bCID ) break;
            pGT = pMASS->GTcnct.GT( obj.AN.alf, pU1, 0 );
            instDOitGSM( pGT );
        } break;
		/// --------------------------------------------------------------------------
		/// CONNECT

		case gpeALF_SLMP: {									cID = gpeCsz_b; if( bCID ) break;
            pGT = pMASS->GTcnct.GT( obj.AN.alf, pU1, 0 );
            instDOitSLMP( pGT );
        } break;
		case gpeALF_SYNC: {									cID = gpeCsz_b; if( bCID ) break;
			pGT = pMASS->GTcnct.GT( obj.AN.alf, pU1, 0 );
			if( !pGT ) break;
			pGT->GTcnct( pWIN );
		} break;
		/// --------------------------------------------------------------------------
		/// SRC CTRL
		case gpeALF_UNSEL: {								cID = gpeCsz_L; if( bCID ) break;
			if( (*pU1)&1 )
				pSRC->bSW |= gpeMASSunselMSK;
			else
				pSRC->bSW &= ~gpeMASSunselMSK;
		} break;
		case gpeALF_AGAIN: {								cID = gpeCsz_l; if( bCID ) break;
			msRUN = *(U4*)pU1;
			if( !msRUN )
				break;
			msRUN += pWIN->mSEC.x;
		} break;
		case gpeALF_NAME: {									cID = gpeCsz_b; if( bCID ) break;
			gpmSTRCPY( gpdCTRL->sNAME, pU1 );
		} break;
		/// --------------------------------------------------------------------------
		/// periFERI
		case gpeALF_CAM: {									cID = gpeCsz_l; if( bCID ) break;
			U4 iCAM = *(U4*)pU1;
			if( !iCAM )
				break;
			if(gpdGLapPIC[0] = pMASS->PIC.PIC(I8x2(obj.AN.alf,iCAM))) {
				//gpcPICAM* pCAM = gpdGLpCAM;
				//if(bSTDcout){gpdCOUT << "gpdGLapPIC[0] = " << (U8)gpdGLapPIC[0]  << gpdENDL;}
				U1* pRGB = gpdGLapPIC[0]->getPIX( gpdGLpCAM, pWIN->mSEC.y ); // pCAM, pWIN->mSEC.y );
				if( pRGB )
				{
					if( gpdGLpCAM->bGD() )
					{
						gpmDEL( gpdGLapPIC[0]->pPACK );
						pWIN->pSYNwin =
						pWIN->pSYNwin->syncADD(
												gpcSYNC(
															gpeNET4_0PIC,
															gpdGLapPIC[0]->id+1,
															pWIN->mSEC.y,
															INVALID_SOCKET, 0
														),
												pWIN->msSYN
											);
					}

					if( !pWIN->pPICbg )
					{
						pWIN->pPICbg = gpdGLapPIC[0];
					}
				}
				else if( gpdGLapPIC[0]->pSHR )
						pWIN->pPICbg = gpdGLapPIC[0];
			}
		} break;
		///----------------------------------------------------
		///
		///				OpenGL render
		///
		///----------------------------------------------------
		case gpeALF_PIC: {									cID = gpeCsz_b; if( bCID ) break;
				U1* pS = pU1;
				I4 picID 	= obj.bUTF8()
							? pMASS->PIC.alfFND( (pS+=gpmNINCS(pS," \t\"")) )
							: *(I4*)pU1;
				gpcPIC* pPIC = pMASS->PIC.PIC( picID );
				if( pPIC )
				{
					gpdGLapPIC[0] = pMASS->PIC.PIC( picID );
					gpdGLaPICid[0] = picID+1;
					break;
				}

				if( !*pS )
					break;

				I8x2 alfN(0,14);

				alfN = pS;
				alfN.num = gpfSTR2U8( pS+alfN.num, &pS );
				gpdGLapPIC[0] = pMASS->PIC.PIC( alfN );
			} break;
		case gpeALF_TRGCP: {								cID = gpeCsz_L; if( bCID ) break;
				int trig = *(I4*)pU1;
				if( trig != 1 )
					break;
				if( !gpdGLpTRG )
					break;
				SDL_Surface* pSRF = gpdGLapPIC[0]->pPICrtxSRF(); //pSRF;
				if( !pSRF )
					break;
				glReadPixels(	0, 0,
						pSRF->w, pSRF->h,
						GL_BGRA,
						GL_UNSIGNED_BYTE,
						pSRF->pixels );
				gpdGLapPIC[0]->pREF = NULL;
			} break;
		case gpeALF_MSKAB: {							cID = gpeCsz_L; if( bCID ) break;

				U1* pS = pU1;
				I4 mskID 	= obj.bUTF8()
							? pMASS->PIC.alfFND( (pS+=gpmNINCS(pS," \t\"")) )	// "mask"
							: *(I4*)pU1;
				if(mskID<1)
					break;
				if( gpdGLpTRG ? !gpdGLpTRG->glRNDR.x : true )
					break;

				U4	iPdrw = 1,													// A. draw
					iPsrc = 2;													// B. cam1
				if( gpdGLapPIC[iPsrc] ? !gpdGLapPIC[iPsrc]->pSRF : true )
					break;
				//if( gpdGLapPIC[iPdrw] ? !gpdGLapPIC[iPdrw]->pSRF : true )
				//	break;

				gpcPIC* pPmsk = pMASS->PIC.PIC( mskID );						// "mask"
				if( !pPmsk )
					break;
				pPmsk->TOOLmaskAB(  this,
									gpdGLpTRG, //gpdGLapPIC[iPdrw],
									gpdGLapPIC[iPsrc],
									pWIN->gpsMASSname,
									pWIN->gpsMASSpath ,pWIN->gppMASSfile );

				pPmsk->pREF = NULL;
			} break;

		/// Target PICTURE
		case gpeALF_TRGH: 									cID = gpeCsz_l; if( bCID ) break;
				gpdGLpTRGwh.y =  *(U4*)pU1; break;
		case gpeALF_TRGW: 									cID = gpeCsz_l; if( bCID ) break;
				gpdGLpTRGwh.x =  *(U4*)pU1; break;
		case gpeALF_TRG:{									cID = gpeCsz_b; if( bCID ) break;
				//U1* pS = pU1;
				I4 trg_id 	= obj.bUTF8()
							? pMASS->PIC.alfFND(pU1)
							: *(I4*)pU1;
				gpdGLpTRG = pMASS->PIC.PIC( trg_id );
			} break;
		/// Vertex SHADERT
		case gpeALF_VTX: {									cID = gpeCsz_b; if( bCID ) break;
				//if( !pMgl ) break;
				if(obj.bUTF8())
					gpdGL->VTX( (char*)pU1 );
			} break;
		/// Pixel SHADERT Compile?
		case gpeALF_PIX: {									cID = gpeCsz_b; if( bCID ) break;
				if( obj.bUTF8() )
					gpdGL->PIX( (char*)pU1 );

				gpOBJ* pOnm = pOBJ(gpeALF_NAME);
				if( !pOnm->bUTF8() ) break;
				gpPTR* pP = pOnm->pPTRu1();
				pU1 = pP->pU1(this);
				if( !pU1 )
					break;

				I8x2 aNM(0,14);
				aNM = (char*)pU1+ ((*pU1 == '\"') ? 1 : 0);
				pWgl->GLSLset( aNM, pMgl->pPIX, pMgl->pVTX );
			} break;

		/// DRAW with GPU
		case gpeALF_GPU: if( pWgl ? gpdGLpTRG : NULL  ) {	cID = gpeCsz_b; if( bCID ) break;
				for( I4 i = 0, msk = gpdGLmskPIC; 	msk; 	i++, msk>>=1 ) {
					//pMASS
					apTX[i] = (msk&1)
							? gpdGLapPIC[i]->surDRWtx(pWgl->pRNDR)
							: NULL;
				}
				I8x2 an(0,14);
				if( obj.bUTF8() )
					an = (char*)pU1+ ((*pU1 == '\"') ? 1 : 0);
				else
					an = I8x2( *(I4*)pU1, 0 );
				U4 tCD = pWIN ? pWIN->nJDOIT.w : 0;
				pWgl->glSETtrg3D( gpdGLpTRG, gpdGLpTRGwh, tCD, tCD )
					->GLSLset( an )
					->glSETbox( gpdGLpTRGxy, gpdGLpTRGwh )
					->glSETcnl( 0, gpmGLaCNL4(0), gpmGLnCNL )
					->glSETtx( gpdGLmskPIC, gpdGLapPIC )
					->glDRW( gpdGLpTRGxy, gpdGLpTRGwh )
					->glDONE();
			} break;
		case gpeALF_SCENE: if( pWgl ? gpdGLpTRG : NULL  ) {	cID = gpeCsz_b; if( bCID ) break;
				if( obj.bUTF8() ) {
					U4 tCD = pWIN ? pWIN->nJDOIT.w : 0;
					pWgl->glSETtrg3D( gpdGLpTRG, gpdGLpTRGwh, tCD, tCD, true )
						->glSCENE( this, (char*)pU1+ ((*pU1 == '\"') ? 1 : 0) );
					pWgl->glDONE();
				}
			} break;
		default:
			break;
	}
	return cID;
}




