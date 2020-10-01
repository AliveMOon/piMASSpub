#include "gpcSRC.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];

gpCORE* gpcSRC::srcRUN( gpcMASS* pMASS, gpcWIN* pWIN, gpCORE* pMOM ) {
	if( !this )
		return NULL;

	srcDBG( pMASS->OPER, 0 );
	if( !pDBG->nLD() )
		return NULL;

	static const U1 iSCP=0;
	static const bool b_dbg = true;
	//#ifndef newCMPI
		return NULL;
	//#endif // newCMPI

	I8x4 *pM0 = (I8x4*)gpmSCP.pMN();
	I4		nSTR, a, n = gpmSCP.nMN();
	char	*pALL = (char*)gpmSCP.p_str,
			*pSTR,
			sBF[0x100],
			*pDIS = (char*)pDBG->p_alloc, *pBF = NULL;
	pBF = sBF+1;
	if( pBF ) *sBF = ' ';

	gpCORE& core = *pCORE;

	I4x4	*pPC;
	I8		*pA, *pO, *pC,
			*pD,
			sNUM, dNUM;
	U1		*p_src, *p_dst;
	U1x4 op;
	gpeOPid iOP;
	U1 iS, iD, xS, xD;
	gpeEA mS, mD;
	gpeCsz iC;	// class id
	U4 szOF;
	U8 mx;
	gpcREG wVAR;

	for(
			/// loop INIT --------------
			core.ini(
						gpmLZYvali( I4x4, &gpmSCP.lzyASM ),  gpmSCP.nASM(),
						&pA, &pD, &pO, &pC
					);
			/// loop CMP --------------
			core.pc < core.nPC;
			/// loop INCRMNT --------------
			core.pc++
		)
	{
		if( b_dbg )
		{
			U8 nLEN;
			pDIS += gpmVAN( pDIS, "\r\n", nLEN );
			pDIS += gpmNINCS( pDIS, "\r\n" );
		}

		pPC = core.iPC( op, iOP, iS, xS, mS, iD, xD, mD, iC );
		if( !pPC )
			continue; /// - NOP! --------------

		if( iOP == gpeOPid_dot ) {
			pA[0] = core.entryOBJ2A0( pM0, pALL, &scpCNST, pWIN, bSW ); //, pD[1] );
			pA[7] = pD[0];
			continue;
		}
		szOF = gpaCsz[iC];
		mx = gpaCszMX[iC];
		switch( mS ) {
			case gpeEA_OFF:
				p_src = NULL;
				continue;
			case gpeEA_Dn:
				p_src = (U1*)(pD+iS); // pD[iS];
				break;
			case gpeEA_An:
				p_src = (U1*)(pA+iS);
				break;
			case gpeEA_IAnI:
				p_src = core.ea( pA[iS], pO+iS, pC+iS );
				break;
			case gpeEA_IAnIp:
				p_src = core.ea( pA[iS], pO+iS, pC+iS );
				pA[iS] += szOF;
				break;
			case gpeEA_sIAnI:
				pA[iS] -= szOF;
				p_src = core.ea( pA[iS], pO+iS, pC+iS );
				break;
			case gpeEA_d16IAnI:
				p_src = core.ea( pA[iS], pO+iS, pC+iS );
				if( !p_src )
					break;
				p_src += pPC->aOB[0];
				break;
			case gpeEA_d16IAnDnI:
				p_src = core.ea( pA[iS], pO+iS, pC+iS );
				if( !p_src )
					break;
				p_src += pPC->aOB[0] + pD[xS];
				break;
			case gpeEA_d16IPcI:
				p_src = (U1*)(pPC->aOB[0] + (I8)pPC );
				break;

			case gpeEA_d16IPcDnI:
				p_src = (U1*)(pPC->aOB[0] + pD[xS] + (I8)pPC );
				break;

			case gpeEA_num:
			default:{
				p_src = (U1*)&(sNUM=pPC->aOB[0]);
			} break;
		}

		if(iOP>=gpeOPid_jsr)
		{
			// jsr;
			// elvileg nem kell foglalkozni a céllal
			continue;
		}

		switch( mD ) {
			case gpeEA_OFF:
				p_dst = NULL;
				continue;
			case gpeEA_Dn:
				p_dst = (U1*)(pD+iD); // pD[iS];
				break;
			case gpeEA_An:
				p_dst = (U1*)(pA+iD);
				break;
			case gpeEA_IAnI:
				p_dst = core.ea( pA[iD], pO+iD, pC+iD );
				break;
			case gpeEA_IAnIp:
				p_dst = core.ea( pA[iD], pO+iD, pC+iD );
				pA[iD] += szOF;
				break;
			case gpeEA_sIAnI:
				pA[iD] -= szOF;
				p_dst = core.ea( pA[iD], pO+iD, pC+iD );
				break;
			case gpeEA_d16IAnI:
				p_dst = core.ea( pA[iD], pO+iD, pC+iD );
				if( !p_dst )
					break;
				p_dst += pPC->aOB[1];
				break;
			case gpeEA_d16IAnDnI:
				p_dst = core.ea( pA[iD], pO+iD, pC+iD );
				if( !p_dst )
					break;
				p_dst += pPC->aOB[1] + pD[xD];
				break;
			case gpeEA_d16IPcI:
				p_dst = (U1*)(pPC->aOB[1] + (I8)pPC );
				break;

			case gpeEA_d16IPcDnI:
				p_dst = (U1*)(pPC->aOB[1] + pD[xD] + (I8)pPC );
				break;

			case gpeEA_num:
			default:{
				p_dst = (U1*)&(dNUM=pPC->aOB[1]);
			} break;
		}
		if( !p_dst )
			continue;

		switch(gpaOPgrp[iOP]) {
			case gpeOPid_entry:{
					switch(iOP)
					{
						case gpeOPid_dot:
							// find OBJ
							continue;
						default:
							continue;
					}
				} continue;
			case gpeOPid_mov:{
					if( !p_dst || !p_src )
						continue;
					switch(iOP)
					{
						case gpeOPid_mov:
						default:
							gpmMcpy( p_dst, p_src, szOF );
							continue;
					}
				} continue;
			case gpeOPid_mul:{
					if( !p_dst || !p_src )
						continue;
					switch( iC )
					{
						case gpeCsz_Q: gpmMUL( I8, iOP, mx ); continue;
						case gpeCsz_q: gpmMUL( U8, iOP, mx ); continue;
						case gpeCsz_L: gpmMUL( I4, iOP, mx ); continue;
						case gpeCsz_l: gpmMUL( U4, iOP, mx ); continue;
						case gpeCsz_W: gpmMUL( I2, iOP, mx ); continue;
						case gpeCsz_w: gpmMUL( U2, iOP, mx ); continue;
						case gpeCsz_B: gpmMUL( I1, iOP, mx ); continue;
						case gpeCsz_b: gpmMUL( U1, iOP, mx ); continue;
						case gpeCsz_f: gpmMUL( F, iOP, mx ); continue;
						case gpeCsz_d: gpmMUL( D, iOP, mx ); continue;
						case gpeCsz_4: gpmMUL( U1x4, iOP, mx ); continue;
					}
				} continue;
			case gpeOPid_add:{
					if( !p_dst || !p_src )
						continue;

					switch( iC )
					{
						case gpeCsz_Q: gpmADD( I8, iOP ); continue;
						case gpeCsz_q: gpmADD( U8, iOP ); continue;
						case gpeCsz_L: gpmADD( I4, iOP ); continue;
						case gpeCsz_l: gpmADD( U4, iOP ); continue;
						case gpeCsz_W: gpmADD( I2, iOP ); continue;
						case gpeCsz_w: gpmADD( U2, iOP ); continue;
						case gpeCsz_B: gpmADD( I1, iOP ); continue;
						case gpeCsz_b: gpmADD( U1, iOP ); continue;
						case gpeCsz_f: gpmADD( F, iOP ); continue;
						case gpeCsz_d: gpmADD( D, iOP ); continue;
						case gpeCsz_4: gpmADD( U1x4, iOP ); continue;
					}
				} continue;
			case gpeOPid_sub:{
					if( !p_dst || !p_src )
						continue;
					switch(iOP)
					{

						default:
							pD[9] = (*p_dst == *p_src);
							continue;
					}
				} continue;
		}


	}


	return pCORE;

}


