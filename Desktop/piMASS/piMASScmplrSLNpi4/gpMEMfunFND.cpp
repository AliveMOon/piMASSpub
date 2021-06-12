#include "gpcSRC.h"
#include "gpcSRCinst.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];
/// iDB			data base ID

/// cntID		main ID cnt
/// mainT

/// iIX = fnd( ID )
/// name.str

/// mIX = fnd( mID )
/// keyPOS.xyz,abc,t
/// drop A.xyzc B.xyz OVER.t
/// turn N.xyzr .... C.xyzr B.xyzr A.xyzr


/// pxSCR.xyr

gpDBitm* gpDBitm::read( gpDBlst *pIDlst ) {
	char *pFILE = pIDlst->pF+sprintf( pIDlst->pF, "0x%0.16llx_dir/", ID );
	gpcLZY dir, rd; U8 s, n;
	dir.lzyDIR( pIDlst->sPATH, s = 0 );
	if( !dir.p_alloc )
		return this;
	char	*pS = (char*)dir.p_alloc,
			*pSi = pS, *pSn = pSi + dir.nLD(),
			*pSt, *pSe, *pF, *pNM = pSi + gpmNINCS( pSi, "\r\n\t "), *pMNe;
	I4x4* pXYZW;
	I8x4 ABbb;
	while( pSi < pSn ) {
		pSt = (pMNe = pSi + gpmVAN(pSi,   "\t", s ))+1;
		pSe = pSt + gpmVAN(pSt, "\r\n", s );
		ABbb.a8x2[0].num = pSt-pSi;
		ABbb.a8x2[0] = pSi;
		ABbb.a8x2[1].num = pSt-pSi - ABbb.a8x2[0].num;
		ABbb.a8x2[1] = pSi + ABbb.a8x2[0].num + 1;
		ABbb.a8x2[0].b = ABbb.a8x2[1].a;
		U1 *pV = pAB( ABbb.a8x2[0], n=0 );
		if( pV ) {
			switch( ABbb.a8x2[0].b ) {
				case gpeALF_LWS:
				case gpeALF_LWO:
					gpmMcpy( pFILE, pNM, pMNe-pNM )[pMNe-pNM] = 0;
					break;
				default:
					pF = pFILE + ABbb.a8x2[0].ab2str(pFILE,".");
					break;
			}
			rd.lzyRD( pIDlst->sPATH, s = 0 );
			switch( ABbb.a8x2[0].b ) {
				case gpeALF_LWO: {
						/// keressünk egy ilyen nevű objectet
						I4* p3Did = (I4*)pV;
						*p3Did = 	pIDlst->pMEM
									? pIDlst->pMEM->pWgl->iLWO( ABbb.a8x2[0].a, pIDlst->sPATH, rd )
									: -1;

					} break;
				case gpeALF_LWS: {
						I4* p3Did = (I4*)pV;
						(*p3Did) = 	pIDlst->pMEM
									? pIDlst->pMEM->pWgl->iLWS( ABbb.a8x2[0].a, pIDlst->sPATH, rd )
									: -1;
					} break;
				default:
					if( rd.nLD() )
						gpmMcpy(pV,rd.p_alloc,gpmMIN(n,rd.nLD()) );
					if( rd.nLD() < n )
						gpmZn( pV+rd.nLD(), n-rd.nLD() );
					break;
			}


		}
		pSi = pSe + gpmNINCS( pSe, "\r\n\t ");
		pNM = pSi;
		continue;


		switch( ABbb.a8x2[0].b ) {
			case gpeALF_XYR: {
					pXYZW = (I4x4*)pAB( ABbb.a8x2[0], n=0 ); //sizeof(*pXYZW) );
					if( !pXYZW )
						break;
					pF = pFILE;
					pF += gpfALF2STR( pF, ABbb.a8x2[0].a );
					*pF = '.'; ++pF;
					gpfALF2STR( pF, ABbb.a8x2[0].b );
					rd.lzyRD( pIDlst->sPATH, s = 0 );

					(*pXYZW) = rd.nLD() ? (*(I4x4*)rd.p_alloc) : I4x4(0);
				} break;
			default:break;
		}
		pSi = pSe + gpmNINCS( pSe, "\r\n\t ");
	}

	return this;
}
gpDBitm* gpDBitm::store( gpDBlst *pIDlst, I8x2* pAT, void* pVAR ) {

	char* pF = pIDlst ? pIDlst->pF + sprintf( pIDlst->pF, "0x%0.16llx_dir/", ID ) : NULL;
	gpcLZY wr;
	U8 s;
	I8x2 ab( pAT[0].alf,pAT[1].alf);
	size_t n = sOF(ab.b);
	if( !n )
		return this;
	U1 *pV = pAB( ab, n );
	if( gpmMcmp(pV,pVAR,n) == n )
		return this;
	gpmMcpy(pV,pVAR,n);
	switch( ab.b ) {
		case gpeALF_LWO: return this; //pF = NULL; break;
		default: break;
	}
	if( !pF )
		return this;

	wr.lzyADD( pV, n, s=0 );
	pF += ab.ab2str(pF,".");
	wr.lzyWR( pIDlst->sPATH );
	return this;

}
size_t gpDBitm::sOF( gpeALF b ) {
	switch( b ) {
		case gpeALF_XYR: return sizeof(I4x4);
		case gpeALF_ID: return sizeof(I8);
		case gpeALF_TRK: return sizeof(gpc3Dtrk);
		case gpeALF_LWS:
		case gpeALF_LWO:
			return sizeof(I4);
		default: break;
	}
	return 0;
}
gpDBlst* gpcMASS::iDB( gpMEM* pMEM, gpPTR *pPi, char* sPATH, char* pFILE ) {
	I8 ixDB = pPi->i8(pMEM);
	if(ixDB == '\"') {
		ixDB = 0;
		if(pPi->bUTF8())
			return iDBu( pMEM, (char*)pPi->pU1(pMEM), sPATH, pFILE );
	}

	return NULL;
}
gpDBlst* gpcMASS::iDBu( gpMEM* pMEM, char *pU1, char* sPATH, char* pFILE ) {
	if(!this)
		return NULL;
	I8 ixDB = 0;
	U8 nLEN;
	U4 nDB = dctDB.nIX(), nU1 = 0, nU0;
	if(!nDB) { dctDB.dctADD( " ", 1 ); nDB = dctDB.nIX(); }

	pU1 += gpmNINCS(pU1,"\" \t\r\n,");
	nU0 = pU1 ? gpmVAN( pU1, "\" \t\r\n,", nLEN ) : 0;
	nU1 = pU1 ? gpmVAN( pU1, "\"", nLEN ) : 0;
	if( nU0 )
		ixDB = dctDB.dctFND( pU1, nU0, nDB );

	if( !ixDB )
		return NULL;

	I4 ixITM;
	gpDBitm* pITM;
	gpDBlst *pIl = NULL;
	gpcLZY dir;

	if( ixDB >= nDB ) {
		dctDB.dctADD( pU1, nU1 );
		ixDB = nDB;
		nDB = dctDB.nIX();
		gpDBlst **ppITMlst = (gpDBlst**)lstDB.Ux( ixDB, sizeof(gpDBlst*) );


		gpmMcpy( pFILE, pU1, nU1 )[nU1] = 0;
		pFILE += nU1;
		dir.lzyDIR( sPATH, nLEN = 0 );
		pIl = *ppITMlst;
		if( !pIl )
			*ppITMlst = (pIl=new gpDBlst( pMEM, sPATH,pFILE-sPATH));

		char	*pS = (char*)dir.p_alloc,
				*pSi = pS, *pSn = pSi + dir.nLD(),
				*pSt, *pSe;
		while( pSi < pSn ) {
			pSt = pSi + gpmVAN(pSi,   "\t", nLEN )+1;
			pSe = pSt + gpmVAN(pSt, "\r\n", nLEN );
			int oo = gpmMcmp(pSi,"0x",2);
			if( oo == 2 )
			if( ((*pSt)=='d') || ((*pSt)=='D') ) {
				ixITM = pIl->itmLST.nLD(sizeof(*pITM));
				pITM = (gpDBitm*)pIl->itmLST.Ux( ixITM, sizeof(*pITM) );
				pITM->mID = pITM->ID = gpfSTR2I8( pSi );
				pITM->read( pIl );
				if( pIl->newID <= pITM->ID )
					pIl->newID = pITM->ID+1;
			}
			pSi = pSe + gpmNINCS( pSe, "\r\n\t ");
		}

	}
	if( pIl )
		return pIl;

	return *(gpDBlst**)lstDB.Ux( ixDB, sizeof(gpDBlst*) );;
}

void gpMEM::funFND() {
	char	sPUB[0x400], *pP,
			*pS, *pSi, *pSit, *pSt, *pSe, *pSn;
	I4	nS, nI4 = pD[7]-pA[7],
		*pI4 = (I4*)pUn(pA[7], nI4 ), nCPY,
		i = nI4/gpaCsz[gpeCsz_l], j,
		ixITM;
	gpcLZY* pPRNT = NULL;
	gpPTR *pPi;
	--i;
	if( !i )
		return;
	/// i 0: DB switch 0 on 1 SEL ... n off
	pPi = (gpPTR*)pUn(pI4[i],sizeof(gpPTR));
	I8 iSW = pPi->i8(this);
	switch( iSW ) {
		case 0: // ON
		case 1: // SEL
			break;
		default:
			iSW = 0; break;
			return; break;
	}

	--i;
	if( !i )
		return;
	/// i 1: DB id/sNAME
	pPi = (gpPTR*)pUn(pI4[i],sizeof(gpPTR));
	pP = sPUB + sprintf( sPUB, "./" );
	gpDBlst *pIl  = pMASS->iDB( this, pPi, sPUB, pP );
	if( !pIl )
		return;
	gpDBitm	*pI0 = (gpDBitm*)pIl->itmLST.p_alloc,
			*pITM;
	I4 nITM = pI0 ? pIl->itmLST.nLD(sizeof(*pI0)) : 0;

	--i;
	if( !i )
		return;
	/// i 2: FORMAT

	pPi = (gpPTR*)pUn(pI4[i],sizeof(gpPTR));
	I8 ixTYP = pPi->i8(this);
	I4 nAT;
	U8 nLEN, s;
	gpcLZY	AT;
	I8x2* pAT = NULL;
	if(ixTYP == '\"') {
		ixTYP = 0;
		if(pPi->bUTF8()) {
			pS = (char*)pPi->pU1(this)+1;
			nS = (pS ? gpmVAN( pS, "\"", nLEN ) : 0);
			nAT = AT.nAT( pS, nS );
			pAT = (I8x2*)AT.p_alloc;
		}
	}
	if( !pAT )
		return;
	--i;
	if( !i )
		return;

	/// i 3: nOUT
	pPi = (gpPTR*)pUn(pI4[i],sizeof(gpPTR));
	I8 nOUT = pPi->i8(this);
	--i;
	if( !i )
		return;
	//pAT->alfCON( sPUB, nAT );
	I4x4 xyzw = 0, *pXYZW;
	I4x2 *pON = NULL, *pIA;
	gpcLZY onLZY, iaLZY;
	I4 onCNT = gpmPAD(nITM+1,0x10), iaCNT = 0;
	size_t sOF;
	for( I4 iA = 0, n; iA < nAT; iA+= 2 ) {
		n = alfLEN(pAT[iA].alf);
		pSi = pS +pAT[iA].num-n;
		pSe = pSi+pAT[iA+1].num;

		I8x2 aa( pAT[iA].alf,pAT[iA+1].alf);
		switch( aa.b ) { //pAT[iA+1].alf ) {
			case gpeALF_XYR:
				xyzw = 0;
				/// függvény paramétereit olvassa XYR
				for( j = 0; j < 3; j++ ) {
					pPi = (gpPTR*)pUn(pI4[i],sizeof(gpPTR));
					xyzw.aXYZW[j] = pPi->i8(this);
					if( !i )
						break;
					i--;
				}
				xyzw.w = nOUT;

				/// ITEM-ek távolságának kiszámítása a megadott xy poziciohoz képest
				pIA = (I4x2*)onLZY.Ux( iaCNT, sizeof(*pIA) );
				pIA->x = onCNT;
				for( ixITM = 0; ixITM < nITM; ixITM++ ) {
					pXYZW = pI0[ixITM].fndXYR( pAT[iA].alf );
					if( !pXYZW )
						continue;
					pON = (I4x2*)onLZY.Ux( onCNT, sizeof(*pON) );
					pON->x = ixITM;
					if( (pIl->aixITMsel[0]==ixITM) && (pIl->iSW==1) && (iSW==1) )
						pON->y = -1;
					else
						pON->y = sqrt((xyzw.a4x2[0]-pXYZW->a4x2[0]).qlen());
					onCNT++;
				}
				/// FND NEAR XY
				pIA = (I4x2*)onLZY.Ux( iaCNT, sizeof(*pIA) );
				pIA->y = onCNT-pIA->x;
				if( pIA->y ) {
					pON = (I4x2*)onLZY.Ux( pIA->x, sizeof(*pON) );
					/// sorting MEDIAN --------------------------------
					pON[0].median( pIA->y, (I4x2*)onLZY.p_alloc, true );
					if( nOUT )
						pIA->y = gpmMIN( nOUT, pIA->y );
					onCNT = pIA->sum();
					iaCNT++;

					for( i = 0; i < pIA->y; i++ ) {
						pPRNT = pPRNT->lzyFRMT( (s=-1), "\r\n %4d 0x%0.4llx %d", pON[i].x, pI0[pON[i].x].ID, pON[i].y );
						I4x4* pXYR = (I4x4*)pI0[pON[i].x].pAB( aa, sOF );
						if( pXYR )
							pPRNT = pPRNT->lzyFRMT( (s=-1), " xyr: %d, %d, %d", pXYR->x, pXYR->y, pXYR->z );
					}
				}

				pI0 = pIl->pITM( 0 );
				if(pON&&pI0){
					if( (pIl->aixITMsel[0]==pON[0].x) && (pIl->iSW==1) && (iSW==1) ) {
						/// folyamatosan le van lenyomva
						/// DRAG
						pI0[pON[0].x].store( pIl, pAT+iA, &xyzw );
						I4x4* pXYR = (I4x4*)pIl->itmSEL.pAB( aa, sOF );
						if( !pXYR )
							break;
						U4 nPUB = sprintf( sPUB, "\r\n x%4d 0x%0.4llx ++ xyr: %d, %d, %d", pON[0].x, pI0[pON[i].x].ID, pXYR->x, pXYR->y, pXYR->z );
						pPRNT = pPRNT->lzyINS( (U1*)sPUB, nPUB, (s=0), 0 );
						break;
					}
					if( (iSW==1) && (pON[0].y<xyzw.z) ) {
						/// igen benne van, most lett lenyomva
						/// SELECT
						pIl->aixITMsel[0] = pON[0].x;
						pIl->iSW=1;
						I4x4* pXYR = (I4x4*)pI0[pON[0].x].pAB( aa, sOF );
						pIl->itmSEL.store( NULL, pAT+iA, pXYR );
						break;
					}
				}

				if( iSW!=1 ) {
					pIl->iSW=0;
					pIl->aixITMsel[0]=nITM;
					if( pON ? (pON[0].y<xyzw.z) : false ) {
						/// igen benne van, DE nincs lenyomva
						/// ON
						U4 nPUB = sprintf( sPUB, "\r\n %4d 0x%0.4llx ++ xyr: %d, %d, %d", nITM, pIl->newID, xyzw.x, xyzw.y, xyzw.z );
						pPRNT = pPRNT->lzyINS( (U1*)sPUB, nPUB, (s=0), 0 );
						break;
					}
					/// nincs benne nincs lenyomva
					/// FREE search
					U4 nPUB = sprintf( sPUB, 	"\r\n %4d 0x%0.4llx ++ xyr: %d, %d, %d"
												"\r\n %4d 0x%0.4llx ++ xyr: %d, %d, %d",
												nITM, pIl->newID, xyzw.x, xyzw.y, xyzw.z,
												nITM, pIl->newID, xyzw.x, xyzw.y, xyzw.z );
					pPRNT = pPRNT->lzyINS( (U1*)sPUB, nPUB, (s=0), 0 );
					break;
				}

				pITM = pIl->pITM( nITM );
				if( !pITM )
					break;
				/// nincs benne, DE lelett nyomva
				/// ADD
				pIl->aixITMsel[0] = nITM;
				pIl->iSW=1;
				pITM->store( pIl, pAT+iA, &xyzw );
				pI0 = pITM - pIl->aixITMsel[0];
				break;

			case gpeALF_ID:

				break;
			default: break;
		}

	}
	/// iDB			data base ID

	/// cntID		main ID cnt
	/// mainT

	/// iIX = fnd( ID )
	/// name.str

	/// mIX = fnd( mID )
	/// keyPOS.xyz,abc,t
	/// drop A.xyzc B.xyz OVER.t
	/// turn N.xyzr .... C.xyzr B.xyzr A.xyzr


	/// pxSCR.xyr


	gpPTR *pPhr = pPTR(pA[0]);
	pS = pPRNT ? (char*)pPRNT->p_alloc : NULL;
	nCPY = gpmSTRLEN(pS)+2;
	I4 nA = nALL(pPhr->iPC);
	if( nA < nCPY ) {
		if( nA )
			iFREE( pPhr->iPC );
		pPhr->iPC = iALL( nCPY );
	}
	nA = nALL(pPhr->iPC);
	pPhr->cID(gpeCsz_b);
	I4x2 dm(nCPY,1);
	pPhr->d2D( dm );

	U1* pDST = pPhr->pU1( this );
	if( pDST )
		sprintf( (char*)pDST, "\"%s\"", pS );
		//gpmMcpy( pDST, pS, nCPY )[nCPY] = 0;

	if(bSTDcout_jsr)
	{gpdCOUT	<< stdCYAN << "funFND:"
				<< stdALU << (pDST?(char*)pDST:"?") << gpdENDL;}

	gpmDEL(pPRNT);
}

void gpMEM::funNEW() {
	char	sPUB[0x400], *pP,
			*pS, *pSi, *pSit, *pSt, *pSe, *pSn;
	I4	nI4 = pD[7]-pA[7],
		*pI4 = (I4*)pUn(pA[7], nI4 ), nCPY;
	nI4 /= gpaCsz[gpeCsz_l];

	/// cntID		main ID cnt
	/// mainT

	/// iIX = fnd( ID )
	/// name.str

	/// mIX = fnd( mID )
	/// keyPOS.xyz,abc,t
	/// drop A.xyzc B.xyz OVER.t
	/// turn N.xyzr .... C.xyzr B.xyzr A.xyzr


	/// pxSCR.xyr



}
