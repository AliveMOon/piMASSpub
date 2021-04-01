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
class gpITMlst;

class gpITM {
public:
	I8 mID, ID;
	gpcLZY	atLST,
			voidLST;
	gpITM(){};
	~gpITM(){
	};
	gpITM* read( gpITMlst *pIDlst );
	gpITM* store( gpITMlst *pIDlst, I8x2* pAT, void* pVAR );
	gpITM& null(){
		/// ha lesz valami törölni való
		gpmCLR;
		return *this;
	}
	size_t sOF( gpeALF b ) {
		switch( b ) {
			case gpeALF_XYR: return sizeof(I4x4);
			case gpeALF_ID: return sizeof(I8);
			case gpeALF_LWO:
				return sizeof(I4);
			default: break;
		}
		return 0;
	}

	U1* fndAA( I8x2& aa ) {
		I8x4* pA0 = (I8x4*)atLST.Ux( 0, sizeof(*pA0) );
		I4 iA = 0;
		for( I4 nA = atLST.nLD(sizeof(*pA0)); iA < nA; iA++ ) {
			if( pA0[iA].a8x2[0] != aa )
				continue;
			return voidLST.Ux( pA0[iA].a8x2[1].x, pA0[iA].a8x2[1].y, true, 1 );
		}
		return NULL;
	}
	U1* pAA( I8x2& aa, size_t& n ) {
		U1* pU = fndAA( aa );
		if( pU )
			return pU;

		n = sOF(aa.b);

		I8x4* pA0 = NULL;
		I4 nA = atLST.nLD(sizeof(*pA0));
		pA0 = (I8x4*)atLST.Ux( nA, sizeof(*pA0) );
		if( !pA0 )
			return NULL;

		pA0->a8x2[0] = aa;
		pA0->a8x2[1].x = voidLST.nLD();
		pA0->a8x2[1].y = n;
		return voidLST.Ux( pA0->a8x2[1].x, pA0->a8x2[1].y, true, 1 );
	}
	I4x4* fndXYR( gpeALF alf ) {
		if( !this )
			return NULL;
		I8x2 aa( alf, gpeALF_XYR );
		return (I4x4*)fndAA( aa );
	}
};
class gpITMlst {
public:
	char	sPATH[0x400], *pF;
	I8		newID;
	I4		aixITMsel[2], iSW;
	gpITM	itmSEL;
	gpcLZY	itmLST;
	gpMEM*	pMEM;
	gpITMlst(){};
	gpITMlst( gpMEM* pM, char* pU1, U2 nU1 ) {
		gpmCLR;
		pMEM = pM;
		pF = sPATH+nU1;
		gpmMcpy( sPATH, pU1, nU1 );
	}
	~gpITMlst(){
		gpITM* pITM = (gpITM*)itmLST.p_alloc;
		if( !pITM )
			return;

		for( I4 i = 0, n = itmLST.nLD(sizeof(gpITM)); i < n; i++ )
			pITM[i].null();
	}
	gpITM* pITM( I4 ix ) {
		U4 n = itmLST.nLD(sizeof(gpITM));
		gpITM* pI = this ? (gpITM*)itmLST.Ux( ix, sizeof(*pI) ) : NULL;
		if( !pI )
			return NULL;
		if( ix < n )
			return pI;
		pI->mID = pI->ID = newID;
		++newID;
		pI->read( this );
		return pI;
	}

};

gpITM* gpITM::read( gpITMlst *pIDlst ) {
	char *pFILE = pIDlst->pF+sprintf( pIDlst->pF, "0x%0.16llx_dir/", ID );
	gpcLZY dir, rd; U8 s, n;
	dir.lzyDIR( pIDlst->sPATH, s = 0 );
	char	*pS = (char*)dir.p_alloc,
			*pSi = pS, *pSn = pSi + dir.nLD(),
			*pSt, *pSe, *pF, *pNM = pSi + gpmNINCS( pSi, "\r\n\t "), *pMNe;
	I4x4* pXYZW;
	I8x4 aaaa;
	while( pSi < pSn ) {
		pSt = (pMNe = pSi + gpmVAN(pSi,   "\t", s ))+1;
		pSe = pSt + gpmVAN(pSt, "\r\n", s );
		aaaa.a8x2[0].num = pSt-pSi;
		aaaa.a8x2[0] = pSi;
		aaaa.a8x2[1].num = pSt-pSi - aaaa.a8x2[0].num;
		aaaa.a8x2[1] = pSi + aaaa.a8x2[0].num + 1;
		aaaa.a8x2[0].b = aaaa.a8x2[1].a;
		U1 *pV = pAA( aaaa.a8x2[0], n=0 );
		if( pV ) {
			switch( aaaa.a8x2[0].b ) {
				case gpeALF_LWO:
					gpmMcpy( pFILE, pNM, pMNe-pNM )[pMNe-pNM] = 0;
					break;
				default:
					pF = pFILE + aaaa.a8x2[0].ab2str(pFILE,".");
					break;
			}
			rd.lzyRD( pIDlst->sPATH, s = 0 );
			switch( aaaa.a8x2[0].b ) {
				case gpeALF_LWO: {
						/// keressünk egy iloyen nevű objectet
						I4* p3Did = (I4*)pV;
						*p3Did = 	pIDlst->pMEM
									? pIDlst->pMEM->pWgl->iLWO( aaaa.a8x2[0].a, pIDlst->sPATH, rd )
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


		switch( aaaa.a8x2[0].b ) {
			case gpeALF_XYR: {
					pXYZW = (I4x4*)pAA( aaaa.a8x2[0], n=0 ); //sizeof(*pXYZW) );
					if( !pXYZW )
						break;
					pF = pFILE;
					pF += gpfALF2STR( pF, aaaa.a8x2[0].a );
					*pF = '.'; ++pF;
					gpfALF2STR( pF, aaaa.a8x2[0].b );
					rd.lzyRD( pIDlst->sPATH, s = 0 );

					(*pXYZW) = rd.nLD() ? (*(I4x4*)rd.p_alloc) : I4x4(0);
				} break;
			default:break;
		}
		pSi = pSe + gpmNINCS( pSe, "\r\n\t ");
	}

	return this;
}
gpITM* gpITM::store( gpITMlst *pIDlst, I8x2* pAT, void* pVAR ) {

	char* pF = pIDlst ? pIDlst->pF + sprintf( pIDlst->pF, "0x%0.16llx_dir/", ID ) : NULL;

	gpcLZY wr;
	U8 s;
	I8x2 aa( pAT[0].alf,pAT[1].alf);
	size_t n = sOF(aa.b);
	if( !n )
		return this;
	U1 *pV = pAA( aa, n );
	if( gpmMcmp(pV,pVAR,n) == n )
		return this;
	gpmMcpy(pV,pVAR,n);
	switch( aa.b ) {
		case gpeALF_LWO: {
				pF = NULL;


			} break;
		default:
			break;
	}
	if( !pF )
		return this;
	wr.lzyADD( pV, n, s=0, -1);
	pF += aa.ab2str(pF,".");
	wr.lzyWR( pIDlst->sPATH );
	return this;

	switch( aa.b ) {
		case gpeALF_XYR: {
			I4x4* pXYR = (I4x4*)pAA( aa, n = 0 ); //sizeof(I4x4) );
			if( !pXYR )
				break;
			if( (*pXYR) == (pVAR?(*(I4x4*)pVAR):I4x4(0)) )
				break;
			(*pXYR) = pVAR ? (*(I4x4*)pVAR) : I4x4(0);
			if( !pF ) break;
			wr.lzyADD( pXYR, sizeof(*pXYR), s=0, -1);
		} break;
		case gpeALF_ID: {
			I8* pI8 = (I8*)pAA( aa, n = 0 ); //sizeof(*pI8) );
			if( !pI8 )
				break;
			if( (*pI8) == (pVAR?(*(I8*)pVAR):0) )
				break;
			(*pI8) = (pVAR?(*(I8*)pVAR):0);
			if( !pF ) break;
			wr.lzyADD( pI8, sizeof(*pI8), s=0, -1);
		} break;
		default: break;
	}

	if( pF ? !wr.nLD() : true )
		return this;
	pF += aa.ab2str(pF,".");
	wr.lzyWR( pIDlst->sPATH );
	return this;
}

gpITMlst* gpcMASS::iDB( gpMEM* pMEM, gpPTR *pPi, char* sPATH, char* pFILE ) {
	I8 ixDB = pPi->i8(pMEM);
	U8 nLEN;
	U1* pU1 = NULL;

	U4 nDB = dctDB.nIX(), nU1 = 0;
	if(!nDB) {
		dctDB.dctADD( " ", 1 );
		nDB = dctDB.nIX();
	}

	if(ixDB == '\"') {
		ixDB = 0;
		if(pPi->bUTF8()) {
			pU1 = pPi->pU1(pMEM)+1;
			nU1 = pU1 ? gpmVAN( pU1, "\"", nLEN ) : 0;
			if( nU1 )
				ixDB = dctDB.dctFND( pU1, nU1, nDB );
		}
	}
	if( !ixDB )
		return NULL;

	I4 ixITM;
	gpITM* pITM;
	gpITMlst *pIl = NULL;
	gpcLZY dir;

	if( ixDB >= nDB ) {
		dctDB.dctADD( pU1, nU1 );
		ixDB = nDB;
		nDB = dctDB.nIX();
		gpITMlst **ppITMlst = (gpITMlst**)lstDB.Ux( ixDB, sizeof(gpITMlst*) );


		gpmMcpy( pFILE, pU1, nU1 )[nU1] = 0;
		pFILE += nU1;
		dir.lzyDIR( sPATH, nLEN = 0 );
		pIl = *ppITMlst;
		if( !pIl )
			*ppITMlst = (pIl=new gpITMlst( pMEM, sPATH,pFILE-sPATH));

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
				pITM = (gpITM*)pIl->itmLST.Ux( ixITM, sizeof(*pITM) );
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

	return *(gpITMlst**)lstDB.Ux( ixDB, sizeof(gpITMlst*) );;
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
	gpITMlst *pIl  = pMASS->iDB( this, pPi, sPUB, pP );
	if( !pIl )
		return;
	gpITM	*pI0 = (gpITM*)pIl->itmLST.p_alloc,
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
		switch( pAT[iA+1].alf ) {
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
						I4x4* pXYR = (I4x4*)pI0[pON[i].x].pAA( aa, sOF );
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
						I4x4* pXYR = (I4x4*)pIl->itmSEL.pAA( aa, sOF );
						if( !pXYR )
							break;
						U4 nPUB = sprintf( sPUB, "\r\n %4d 0x%0.4llx ++ xyr: %d, %d, %d", pON[0].x, pI0[pON[i].x].ID, pXYR->x, pXYR->y, pXYR->z );
						pPRNT = pPRNT->lzyINS( (U1*)sPUB, nPUB, (s=0), 0 );
						break;
					}
					if( (iSW==1) && (pON[0].y<xyzw.z) ) {
						/// igen benne van, most lett lenyomva
						/// SELECT
						pIl->aixITMsel[0] = pON[0].x;
						pIl->iSW=1;
						I4x4* pXYR = (I4x4*)pI0[pON[0].x].pAA( aa, sOF );
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
	{std::cout	<< stdCYAN << "funFND:"
				<< stdALU << (pDST?(char*)pDST:"?") << std::endl;}

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
