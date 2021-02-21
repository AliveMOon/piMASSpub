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
	gpITM* dir( gpITMlst *pIDlst );
	gpITM* store( gpITMlst *pIDlst, I8x2* pAT, void* pVAR );
	gpITM& null(){
		/// ha lesz valami törölni való
		gpmCLR;
		return *this;
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
	U1* pAA( I8x2& aa, size_t n ) {
		U1* pU = fndAA( aa );
		if( pU )
			return pU;
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
	I4		aixITMsel[2];
	gpcLZY	itmLST;
	gpITMlst(){};
	gpITMlst( char* pU1, U2 nU1 ) {
		gpmCLR;
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
		gpITM* pI = this ? (gpITM*)itmLST.Ux( ix, sizeof(*pI) ) : NULL;
		if( !pI )
			return NULL;
		pI->mID = pI->ID = newID++;
		pI->dir( this );
		return pI;
	}

};
gpITM* gpITM::dir( gpITMlst *pIDlst ) {
	sprintf( pIDlst->pF, "0x%0.16llx_dir/", ID );
	gpcLZY dir, rd; U8 s;
	dir.lzyDIR( pIDlst->sPATH, s = 0 );
	char	*pS = (char*)dir.p_alloc,
			*pSi = pS, *pSn = pSi + dir.nLD(),
			*pSt, *pSe, *pF;
	I4x4* pXYZW;
	while( pSi < pSn ) {
		pSt = pSi + gpmVAN(pSi,   "\t", s )+1;
		pSe = pSt + gpmVAN(pSt, "\r\n", s );
		I8x4 aaaa;
		aaaa.a8x2[0].num = pSt-pSi;
		aaaa.a8x2[0] = pSi;
		aaaa.a8x2[1].num = pSt-pSi - aaaa.a8x2[0].num;
		aaaa.a8x2[1] = pSi + aaaa.a8x2[0].num + 1;
		aaaa.a8x2[0].y = aaaa.a8x2[1].x;
		switch( aaaa.a8x2[1].alf ) {
			case gpeALF_XYR: {
					pXYZW = pIDlst->pAA( aaaa.a8x2[0], sizeof(*pXYZW) );
					if( !pXYZW )
						break;
					pF = pIDlst->pF;
					pF += gpfALF2STR( pF, aaaa.a8x2[0].x );
					*pF = '.'; ++pF;
					gpfALF2STR( pF, aaaa.a8x2[0].y );
					rd.lzyRD( pIDlst->sPATH, s = 0, -1 );

					(*pXYZW) = rd.nLD() ? (*(I4x4*)rd.p_alloc) : I4x4(0);
				} break;
			default:break;
		}
		pSi = pSe + gpmNINCS( pSe, "\r\n\t ");
	}

	return this;
}
gpITM* gpITM::store( gpITMlst *pIDlst, I8x2* pAT, void* pVAR ) {
	char* pF = pIDlst->pF + sprintf( pIDlst->pF, "0x%0.16llx_dir/", ID );
	switch( pAT[1].alf ) {
		case gpeALF_XYR: {
			I8x2 aa( pAT[0].alf,pAT[1].alf);
			pF += gpfALF2STR( pF, aa.x );
			*pF = '.'; ++pF;
			gpfALF2STR( pF, aa.y );
			I4x4* pXYR = (I4x4*)pAA( aa, sizeof(I4x4) );
			if( !pXYR )
				break;
			(*pXYR) = pVAR ? (*(I4x4*)pVAR) : I4x4(0);

		} break;
		default: break;
	}
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
			*ppITMlst = (pIl=new gpITMlst(sPATH,nU1));

		char	*pS = (char*)dir.p_alloc,
				*pSi = pS, *pSn = pSi + dir.nLD(),
				*pSt, *pSe;
		while( pSi < pSn ) {
			pSt = pSi + gpmVAN(pSi,   "\t", nLEN )+1;
			pSe = pSt + gpmVAN(pSt, "\r\n", nLEN );

			if( !strcmp(pSi,"0x") )
			if( *pSt=='d' || *pSt=='D' ) {
				ixITM = pIl->itmLST.nLD(sizeof(*pITM));
				pITM = (gpITM*)pIl->itmLST.Ux( ixITM, sizeof(*pITM) );
				pITM->mID = pITM->ID = gpfSTR2I8( pSi );
				pITM->dir( pIl );
				if( pIl->newID < pITM->ID )
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
	/// i 2: VAR id/sNAME

	pPi = (gpPTR*)pUn(pI4[i],sizeof(gpPTR));
	I8 ixTYP = pPi->i8(this);
	I4 nAT;
	U8 nLEN;
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

	pAT->alfCON( sPUB, nAT );
	I4x4 xyzw = 0, *pXYZW;
	I4x2 *pON, *pIA;
	gpcLZY onLZY, iaLZY;
	I4 onCNT = nITM, iaCNT = 0;
	for( I4 iA = 0, n; iA < nAT; iA+= 2 ) {
		n = alfLEN(pAT[iA].alf);
		pSi = pS +pAT[iA].num-n;
		pSe = pSi+pAT[iA+1].num;
		switch( pAT[iA+1].alf ) {
			case gpeALF_XYR:
				j = 0;
				xyzw = 0;
				while( i>-1 ) {
					pPi = (gpPTR*)pUn(pI4[i],sizeof(gpPTR));
					xyzw.aXYZW[j++] = pPi->i8(this);
					i--;
				}
				pIA = (I4x2*)onLZY.Ux( iaCNT, sizeof(*pIA) );
				pIA->x = onCNT;
				for( ixITM = 0; ixITM < nITM; ixITM++ ) {
					pXYZW = pI0[ixITM].fndXYR( pAT[iA].alf );
					if( !pXYZW )
						continue;
					pON = (I4x2*)onLZY.Ux( onCNT, sizeof(*pON) );
					pON->x = ixITM;
					pON->y = sqrt((xyzw.a4x2[0]-pXYZW->a4x2[0]).qlen());
					if( pON->y > (xyzw.z+pXYZW->z) )
						continue;
					onCNT++;
				}
				pIA->y = onCNT-pIA->x;
				if( pIA->y ) {
					pON = (I4x2*)onLZY.Ux( pIA->x, sizeof(*pON) );

					pON[0].median( pIA->y, (I4x2*)onLZY.p_alloc, true );
					if( pXYZW->w )
						pIA->y = gpmMIN( pXYZW->w, pIA->y );
					gpmMcpyOF( pON, onLZY.p_alloc, pIA->y );
					onCNT = pIA->sum();
					pIl->aixITMsel[0] = pON[0].x;
				}
				if( iSW ? iSW!=1 : true )
					break;
				// select
				if( pON ) {
					pIl->aixITMsel[1] = pIl->aixITMsel[0];
					break;
				}
				// ITM + alf.XYR
				pITM = pIl->pITM( nITM );
				if( !pITM )
					break;
				pIl->aixITMsel[1] = pIl->aixITMsel[0] = nITM;
				pI0 = pITM-pIl->aixITMsel[0];
				pITM->store( pIl, pAT+iA, &xyzw );
				break;
			case gpeALF_MOM:
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
