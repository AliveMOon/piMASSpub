#include "gpcSRC.h"
#include "gpcSRCinst.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];

gpBLOCK* gpcSRC::srcINSTdwn( char* pS, gpBLOCK *pBLKm, gpBLOCK* pBLK, gpBLOCK* pBLKup ) {
	if( !pBLKm )
		pBLKm = lzyBLOCK.pSTPdwn( pBLK->bIDm );
	if( pBLKup )
		pBLK = pBLKup;
	I4 aiPC[2], sOF = 0, cID = -1;
	U1* pU1 = NULL;

	//gpROW *pR0 = pBLK->pROW();
	//aiPC[0] = iPCrow( pR0, sOF, true );
	_move._L.EAl(pBLK->iPC).A0;

	gpROW *pRm = pBLKm->pROW(pBLK->bIDmR);
	aiPC[1] = pRm ? iPCrow( *pRm, sOF, false ) : gpeCsz_L;
	//pU1 = srcMEMiPC( iPC, sOF );
	_move._L.EAl(aiPC[1]).A1;
	_move._L.IA0I.IA1I;
	_nop;
	return pBLKm;
}



bool gpcSRC::srcINSTrun( gpcMASS* pMASS, gpcWIN* pWIN ) {
	if( pMEM ? !pMEM->nCD : true )
		return false;
	I4 nPC = pMEM->nCD;
	if( pMEM->pc >= nPC )
		return false;

	std::cout << stdRUN " RUN" stdRESET; // << std::endl;

	gpINST*pI;
	while( pMEM->pc < nPC )
	{
		pI = pMEM->instALU();
	}


	return true;
}

gpINST* gpMEM::instALU()
{
	std::cout << stdALU "+";
	gpINST& ins = pINST[pc];
	pc++;

	U4 sOF = gpaCsz[ins.cID];
	U1* p_src, *p_dst, *pALL = lzyMEM.p_alloc, *pPC = ((U1*)&ins);
	switch( ins.mS )
	{
		case gpeEA_Dn: p_src = (U1*)&(pD[ins.iS]); break;
		case gpeEA_An: p_src = (U1*)&(pA[ins.iS]); break;
		case gpeEA_IAnI: p_src = pALL + pA[ins.iS]; break;
		case gpeEA_IAnIp:
			p_src = pALL + pA[ins.iS];
			pA[ins.iS]+=sOF;
			break;
		case gpeEA_sIAnI:
			pA[ins.iS]-=sOF;
			p_src = pALL + pA[ins.iS];
			break;
		case gpeEA_d16IAnI:
			p_src = pALL + pA[ins.iS] + ins.a8x2.x;
			break;
		case gpeEA_d16IAnDnI:
			p_src = pALL + pA[ins.iS] + pD[ins.xS] + ins.a8x2.x;
			break;
		case gpeEA_d16IPcI:
			p_src = pPC + ins.a8x2.x;
			break;
		case gpeEA_d16IPcDnI:
			p_src = pPC + pD[ins.iS] + ins.a8x2.x;
			break;
		case gpeEA_W:
		case gpeEA_L:
			p_src = (U1*)&ins.a8x2.x;
			break;

		case gpeEA_OFF:
		default:
			p_src = NULL;
			break;
	}

	switch( ins.mD )
	{
		case gpeEA_Dn: p_dst = (U1*)&(pD[ins.iD]); break;
		case gpeEA_An: p_dst = (U1*)&(pA[ins.iD]); break;
		case gpeEA_IAnI: p_dst = pALL + pA[ins.iD]; break;
		case gpeEA_IAnIp:
			p_dst = pALL + pA[ins.iD];
			pA[ins.iS]+=sOF;
			break;
		case gpeEA_sIAnI:
			pA[ins.iS]-=sOF;
			p_dst = pALL + pA[ins.iD];
			break;
		case gpeEA_d16IAnI:
			p_dst = pALL + pA[ins.iD] + ins.a8x2.y;
			break;
		case gpeEA_d16IAnDnI:
			p_dst = pALL + pA[ins.iD] + pD[ins.xD] + ins.a8x2.y;
			break;
		case gpeEA_d16IPcI:
			p_dst = pPC + ins.a8x2.y;
			break;
		case gpeEA_d16IPcDnI:
			p_dst = pPC + pD[ins.iD] + ins.a8x2.y;
			break;
		case gpeEA_W:
		case gpeEA_L:
			p_dst = (U1*)&ins.a8x2.y;
			break;

		case gpeEA_OFF:
		default:
			p_dst = NULL;
			break;
	}
	switch( ins.op ) {
		case gpeOPid_EXTB:
			if( p_src ) {
				*(I8*)p_src = *(I1*)p_src;
			} break;
		case gpeOPid_EXT:
			if( p_src ) {
				*(I8*)p_src = *(I2*)p_src;
			} break;
		case gpeOPid_EXTL:
			if( p_src ) {
				*(I8*)p_src = *(I4*)p_src;
			} break;
		default: {
			switch(gpaOPgrp[ins.op]) {
					case gpeOPid_entry:{
							switch(ins.op)
							{
								case gpeOPid_dot:
									// find OBJ
									break;
								default: break;
							}
						} break;
					case gpeOPid_mov:{
							if( !p_dst || !p_src )
								break;
							switch(ins.op)
							{
								case gpeOPid_mov:
								default:
									gpmMcpy( p_dst, p_src, sOF );
									break;
							}
						} break;
					case gpeOPid_mul:{
							if( !p_dst || !p_src )
								break;
							U8 mx = gpaCszMX[ins.cID];
							switch( ins.cID )
							{
								case gpeCsz_Q: gpmMUL( I8, ins.op, mx ); break;
								case gpeCsz_q: gpmMUL( U8, ins.op, mx ); break;
								case gpeCsz_L: gpmMUL( I4, ins.op, mx ); break;
								case gpeCsz_l: gpmMUL( U4, ins.op, mx ); break;
								case gpeCsz_W: gpmMUL( I2, ins.op, mx ); break;
								case gpeCsz_w: gpmMUL( U2, ins.op, mx ); break;
								case gpeCsz_B: gpmMUL( I1, ins.op, mx ); break;
								case gpeCsz_b: gpmMUL( U1, ins.op, mx ); break;
								case gpeCsz_f: gpmMUL( F, ins.op, mx ); break;
								case gpeCsz_d: gpmMUL( D, ins.op, mx ); break;
								case gpeCsz_4: gpmMUL( U1x4, ins.op, mx ); break;
								default: break;
							}
						} break;
					case gpeOPid_add:{
							if( !p_dst || !p_src )
								break;

							switch( ins.cID )
							{
								case gpeCsz_Q: gpmADD( I8, ins.op ); break;
								case gpeCsz_q: gpmADD( U8, ins.op ); break;
								case gpeCsz_L: gpmADD( I4, ins.op ); break;
								case gpeCsz_l: gpmADD( U4, ins.op ); break;
								case gpeCsz_W: gpmADD( I2, ins.op ); break;
								case gpeCsz_w: gpmADD( U2, ins.op ); break;
								case gpeCsz_B: gpmADD( I1, ins.op ); break;
								case gpeCsz_b: gpmADD( U1, ins.op ); break;
								case gpeCsz_f: gpmADD( F, ins.op ); break;
								case gpeCsz_d: gpmADD( D, ins.op ); break;
								case gpeCsz_4: gpmADD( U1x4, ins.op ); break;
								default: break;
							}
						} break;
					case gpeOPid_sub:{
							if( !p_dst || !p_src )
								break;
							switch(ins.op)
							{
								default:
									pD[9] = (*p_dst == *p_src);
									break;
							}
						} break;
				}
		} break; // deafault:
	}


	return &ins;
}

gpBLOCK* gpcSRC::srcINSTadd2( char* pS, gpBLOCK *pBLKm, gpBLOCK* pBLK ) {
	if( !pBLKm )
		pBLKm = lzyBLOCK.pSTPdwn( pBLK->bIDm );

	gpROW	*pR0 = pBLK->pROW();
	I4 iPC, sOF = 0, nR = pBLK->nROW();
	U1* pU1 = NULL;
	for( I4 i = 0; i < nR; i++ )
	{
		gpROW& Ri = pR0[i];
		iPC = iPCrow( Ri, sOF, true );
		pU1 = srcMEMiPC( iPC, sOF );
		_move._L.EAl(iPC).A0;
		if( !i ){
			_move.c((gpeCsz)Ri.cID).IA0I.D0;
			continue;
		}
		pMEM->inst( (gpeOPid)pR0[i-1].pstOP ).c((gpeCsz)Ri.cID).IA0I.D0;
	}

	pU1 = srcMEMiPC( pBLK->iPC, sOF );
	_move._L.EAl( pBLK->iPC ).A0;
	_move.c((gpeCsz)pBLK->cID).D0.IA0I;
	_nop;
	return pBLKm;
}
gpBLOCK* gpcSRC::srcINSTmul2( char* pS, gpBLOCK *pBLKm, gpBLOCK* pBLK  ) {
	if( !pBLKm )
		pBLKm = lzyBLOCK.pSTPdwn( pBLK->bIDm );

	gpROW	*pR0 = pBLK->pROW();
	I4 iPC = 0 , sOF = 0, nR = pBLK->nROW();
	U1* pU1 = NULL;
	for( I4 i = 0; i < nR; i++ )
	{
		gpROW& Ri = pR0[i];
		iPC = iPCrow( Ri, sOF, true );
		pU1 = srcMEMiPC( iPC, sOF );
		//_move.l #iPC.Aa;
		_move._L.EAl(iPC).A0;
		if( !i ){
			// move.l (A0),d0
			_move.c((gpeCsz)Ri.cID).IA0I.D0;
			continue;
		}
		// add.l (A0),d0
		pMEM->inst( (gpeOPid)pR0[i-1].pstOP ).c((gpeCsz)Ri.cID).IA0I.D0;
	}

	pU1 = srcMEMiPC( pBLK->iPC, sOF );
	_move._L.EAl( pBLK->iPC ).A0;
	_move.c((gpeCsz)pBLK->cID).D0.IA0I;
	_nop;
	return pBLKm;
}
