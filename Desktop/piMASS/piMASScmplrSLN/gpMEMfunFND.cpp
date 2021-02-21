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

	gpITM(){};
	~gpITM(){
	};
	gpITM* dir( gpITMlst *pIDlst );
	gpITM& null(){
		/// ha lesz valami törölni való
		gpmCLR;
		return *this;
	}
};
class gpITMlst {
public:
	char	sPATH[0x400], *pF;
	gpcLZY	itmLST;
	gpITM*	pITM;
	gpITMlst(){};
	gpITMlst( char* pU1, U2 nU1 ) {
		gpmCLR;
		pF = sPATH+nU1;
		gpmMcpy( sPATH, pU1, nU1 );
	}
	~gpITMlst(){
		pITM = (gpITM*)itmLST.p_alloc;
		if( !pITM )
			return;

		for( I4 i = 0, n = itmLST.nLD(sizeof(gpITM)); i < n; i++ )
			pITM[i].null();
	}
};
gpITM* gpITM::dir( gpITMlst *pIDlst ) {
	sprintf( pIDlst->pF, "0x%0.16llx_dir/", ID );
	gpcLZY dr; U8 s;
	dr.lzyDIR( pIDlst->sPATH, s = 0 );

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
		i = nI4/gpaCsz[gpeCsz_l], j;
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
	gpITM* pI0 = (gpITM*)pIl->itmLST.p_alloc;
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
	I4x4 xyzw = 0;
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
