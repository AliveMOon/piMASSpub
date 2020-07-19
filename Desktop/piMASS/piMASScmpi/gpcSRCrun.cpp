#include "gpcSRC.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];

gpcRES* gpcSRC::SRCmnMILLrun( gpcMASS* pMASS, gpcWIN* pWIN, gpcRES* pMOM ) {
	if( !this )
		return NULL;

	SRCmnMILLdbg( //pMASS->aOP,
					pMASS->OPER, 0 );
	if( !pDBG->nLD() )
		return NULL;

	static const U1 iMN=0;
	I8x4 *pM0 = (I8x4*)SCOOP.mini.p_alloc;
	char* pDIS = (char*)pDBG->p_alloc;
	gpCORE core;
	I4x4	*pPC;
	I8		*pA, *pO, *pC,
			*pD,
			sNUM, dNUM;
	U1		*p_src, *p_dst;
	U1x4 op;
	gpeOPid iOP;
	U1 iS, iD, xS, xD;
	gpeEA mS, mD;
	U4 szOF;

	for(	core.ini(
						(I4x4*)SCOOP.vASM.p_alloc,SCOOP.nASM(),
						&pA, &pD, &pC, &pO
					);
			core.pc < core.nPC;
			core.pc++ )
	{
		pDIS += gpmVAN( pDIS, "\r\n", nL );
		pDIS += gpmNINCS( pDIS, "\r\n" );
		pPC = core.iPC( op, iOP, iS, xS, mS, iD, xD, mD, szOF );
		if( !pPC )
			continue; // NOP?

		if( iOP == gpeOPid_dot ) {

			continue;
		}

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
				pA[iS] += szOF;
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
				pA[iD] += szOF;
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

		switch(gpaOPgrp[iOP])
		{
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
					switch( szOF )
					{
						case 0x10:
							switch(iOP)
							{
								case gpeOPid_and:
									*(F4*)p_dst &= *(F4*)p_src;
									continue;
								case gpeOPid_rem:
									*(F4*)p_dst %= *(F4*)p_src;
									continue;
								case gpeOPid_div:
									*(F4*)p_dst /= *(F4*)p_src;
									continue;
								case gpeOPid_mul:
									*(F4*)p_dst *= *(F4*)p_src;
									continue;
								default:
									*(F4*)p_dst *= *(F4*)p_src;
									continue;
							}
							continue;
						case 8:
							switch(iOP)
							{
								case gpeOPid_and:
									*(I8*)p_dst &= *(I8*)p_src;
									continue;
								case gpeOPid_rem:
									*(I8*)p_dst %= *(I8*)p_src;
									continue;
								case gpeOPid_div:
									*(I8*)p_dst /= *(I8*)p_src;
									continue;
								case gpeOPid_mul:
									*(I8*)p_dst *= *(I8*)p_src;
									continue;
								default:
									*(I8*)p_dst *= *(I8*)p_src;
									continue;
							}
							continue;
						case 4:
							switch(iOP)
							{
								case gpeOPid_and:
									*(I4*)p_dst &= *(I4*)p_src;
									continue;
								case gpeOPid_rem:
									*(I4*)p_dst %= *(I4*)p_src;
									continue;
								case gpeOPid_div:
									*(I4*)p_dst /= *(I4*)p_src;
									continue;
								case gpeOPid_mul:
									*(I4*)p_dst *= *(I4*)p_src;
									continue;
								default:
									*(I4*)p_dst *= *(I4*)p_src;
									continue;
							}
							continue;
						case 2:
							switch(iOP)
							{
								case gpeOPid_and:
									*(U2*)p_dst &= *(U2*)p_src;
									continue;
								case gpeOPid_rem:
									*(U2*)p_dst %= *(U2*)p_src;
									continue;
								case gpeOPid_div:
									*(U2*)p_dst /= *(U2*)p_src;
									continue;
								case gpeOPid_mul:
									*(U2*)p_dst *= *(U2*)p_src;
									continue;
								default:
									*(U2*)p_dst *= *(U2*)p_src;
									continue;
							}
							continue;
						case 1:
						default:
							switch(iOP)
							{
								case gpeOPid_and:
									*p_dst &= *p_src;
									continue;
								case gpeOPid_rem:
									*p_dst %= *p_src;
									continue;
								case gpeOPid_div:
									*p_dst /= *p_src;
									continue;
								case gpeOPid_mul:
									*p_dst *= *p_src;
									continue;
								default:
									*p_dst *= *p_src;
									continue;
							}
							continue;
					}
				} continue;
			case gpeOPid_add:{
					if( !p_dst || !p_src )
						continue;

					switch(iOP)
					{
						case gpeOPid_or:
							*p_dst |= *p_src;
							continue;
						case gpeOPid_sub:
							*p_dst -= *p_src;
							continue;
						case gpeOPid_add:
							*p_dst += *p_src;
							continue;
						default:
							*p_dst += *p_src;
							continue;
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


	return NULL;

}

gpcRES* gpcSRC::SRCmnMILLrunTRESH( gpcMASS* pMASS, gpcWIN* pWIN, gpcRES* pMOM ) {
	if( !this )
		return NULL;

	SRCmnMILLdbg( //pMASS->aOP,
					pMASS->OPER, 0 );
	if( !pDBG->nLD() )
		return NULL;

	/// -------------------------------------
	 /// return NULL;


	static const U1 iMN=0;
	I8x4 *pM0 = (I8x4*)SCOOP.mini.p_alloc;
	U1	//Xs,
		Ys, Xd, Yd, bOBs = false, bOBd = false, iS, iD, //sz,
		szOF;
	gpeEA mS, mD;
	char	*pALL = (char*)SCOOP.pALL,
			*pSTR;
	I4		nSTR, a;

	gpcLZY	aMEM[0x10];
	I8		*pSRC, *pDST, sNUM, dNUM,
			aA[8*4],
			*pD = aA+8,
			*pC = pD+8;
	double	*pF = (double*)(pC+8);
	gpmZ(aA);

	aA[6] = (I8)aMEM[6].Ux( 0x400, sizeof(I8) );
	aA[7] = (I8)aMEM[7].Ux( 0x400, sizeof(I8) );

	gpcREG wVAR;

	gpcPIK	PIK;
	gpcO	*pDOT = NULL, wip0; //, *pOUT = NULL, wipO;
	gpcSRC	*pANs;

	char* pDIS = (char*)pDBG->p_alloc;
	for( I4x4* pPCs = (I4x4*)SCOOP.vASM.p_alloc, *pPC = pPCs, *pPCe = pPC+SCOOP.nASM();
			pPC < pPCe; pPC++ )
	{
		pDIS += gpmVAN( pDIS, "\r\n", nL );
		pDIS += gpmNINCS( pDIS, "\r\n" );
		U1x4 op = pPC->op;
		gpeOPid oID = (gpeOPid)op.x;
		if( !oID ) // nop
			continue;
		iS = op.y&7;
		szOF = gpaEAsz[pPC->sz];

		if( oID == gpeOPid_dot ) {
			/// jsr entryOBJ2A0
			// keressük meg az OBJ-t
			pDOT = NULL;
			for( U4 ie = pD[0], ii=aA[7]; ii<ie; ii++ ) {
				I4 o = *(I8*)( aMEM[iS].Ux(ii,sizeof(I8)) );
				if( o >= 0 ) {
					gpcOBJlnk& O = ((gpcOBJlnk*)OBJ.Ux( o, sizeof(gpcOBJlnk)))[0];
					gpeTYP typ = O.typ;
					gpeALF alf = O.obj.alf;
					switch( O.typ )
					{
						case gpeTYP_sA8:
							if( pDOT )
							{
								pDOT = PIK.fnd( pDOT, wip0, o );
								if( pANs ? pANs->bSW&gpeMASSloopMSK : false )
									bSW |= gpeMASSloopMSK;
								continue;
							}

							if( pWIN->WINvar( wVAR, alf ) )
							{
								// valamit kapott
								bSW |= gpeMASSloopMSK;
								pSTR = (char*)wVAR.getSTR();
								continue;
							}
							/// ITT kell létrehozni ha nincsen
							pDOT = PIK.add( pDOT, wip0, o );
							if( !pDOT )
							{
								aA[0] = 0;
								ii = ie;
								continue;
							}
							aA[0] = (I8)pDOT;
							continue;
					}
				} else {
					o *= -1;
					pSTR = pALL+pM0[o].iMNi;
					nSTR = pM0[o].iMNn;
				}
			}
			// A7 vissza állít D0-ra
			// obj *pointerét betesz az A0-ba

			continue;
		}

		std::cout << pPC-pPCs << std::endl;
		mS = (gpeEA)(op.y>>3);
		switch( mS ) {
			case gpeEA_OFF:
				pSRC = NULL;
				continue;
			case gpeEA_Dn:
				pSRC = pD+iS;
				break;
			case gpeEA_An:
				pSRC = aA+iS;
				break;
			case gpeEA_IAnI:
				pSRC = (I8*)( aMEM[iS].Ux( aA[iS],sizeof(*aA)) );
				break;
			case gpeEA_IAnIp:
				pSRC = (I8*)( aMEM[iS].Ux( aA[iS],sizeof(*aA)) );
				++aA[iS];
				break;
			case gpeEA_sIAnI:
				--aA[iS];
				pSRC = (I8*)( aMEM[iS].Ux( aA[iS],sizeof(*aA)) );
				break;
			case gpeEA_d16IAnI:
				a = aA[iS] + pPC->aOB[0];
				pSRC = (I8*)( aMEM[iS].Ux( a,sizeof(*aA)) );
				break;
			case gpeEA_d16IAnDnI:
				a = aA[iS] + pPC->aOB[0] + pD[op.w&7];
				pSRC = (I8*)( aMEM[iS].Ux( a,sizeof(*aA)) );
				break;
			case gpeEA_d16IPcI:
				/*a = (pPC-pPCs)+pPC->aOB[0];
				pSRC = &(aRES[iS].ALU(a));
				break;*/
			case gpeEA_d16IPcDnI:
				pSRC = NULL;
				break;
				/*a = (pPC-pPCs)+pPC->aOB[0]+aD[op.w&7].i8();
				pSRC = &(aRES[iS].ALU(a));
				break;*/
			case gpeEA_num:
			default:{
				pSRC = &(sNUM=*pPC->aOB);
				/*if( ob >= 0 )
					NUM = (gpcOBJlnk*)OBJ.Ux( ob, sizeof(gpcOBJlnk));
				else {
					ob *= -1;
					if(pM0[ob].iMNn)
					{
						NUM = ? pALL+pM0[ob].iMNi
					} else
						NUM.zero();

				}*/

			} break;
		}

		mD = (gpeEA)(op.z>>3);
		iD = op.z&7;
		switch( mD ) {
			case gpeEA_OFF:
				pDST = NULL;
				continue;
			case gpeEA_Dn:
				pDST = pD+iD;
				break;
			case gpeEA_An:
				pDST = aA+iD;
				break;
			case gpeEA_IAnI:
				pDST = (I8*)( aMEM[iD].Ux( aA[iD],sizeof(*aA)) );
				break;
			case gpeEA_IAnIp:
				pDST = (I8*)( aMEM[iD].Ux( aA[iD],sizeof(*aA)) );
				++aA[iD];
				break;
			case gpeEA_sIAnI:
				--aA[iD];
				pDST = (I8*)( aMEM[iD].Ux( aA[iD],sizeof(*aA)) );
				break;
			case gpeEA_d16IAnI:
				a = aA[iD] + pPC->aOB[0];
				pDST = (I8*)( aMEM[iD].Ux( a,sizeof(*aA)) );
				break;
			case gpeEA_d16IAnDnI:
				a = aA[iD] + pPC->aOB[0] + pD[op.w>>4];
				pDST = (I8*)( aMEM[iD].Ux( a,sizeof(*aA)) );
				break;
			case gpeEA_d16IPcI:
			case gpeEA_d16IPcDnI:
				pDST = NULL;
				break;
			case gpeEA_num:
			default:{
				pDST = &(dNUM=*pPC->aOB);
			} break;
		}

		if(op.x>=gpeOPid_jsr)
		{
			// jsr;
			continue;
		}

		switch(gpaOPgrp[oID])
		{
			case gpeOPid_entry:{
					switch(oID)
					{
						case gpeOPid_dot:
							// find OBJ
							continue;
						default:
							continue;
					}
				} continue;
			case gpeOPid_mov:{
					if( !pDST || !pSRC )
						continue;
					switch(oID)
					{
						case gpeOPid_mov:
							*pDST = *pSRC;
							continue;
						default:
							*pDST = *pSRC;
							continue;
					}
				} continue;
			case gpeOPid_mul:{
					if( !pDST || !pSRC )
						continue;
					switch(oID)
					{
						case gpeOPid_and:
							*pDST &= *pSRC;
							continue;
						case gpeOPid_rem:
							*pDST %= *pSRC;
							continue;
						case gpeOPid_div:
							*pDST /= *pSRC;
							continue;
						case gpeOPid_mul:
							*pDST *= *pSRC;
							continue;
						default:
							*pDST *= *pSRC;
							continue;
					}
				} continue;
			case gpeOPid_add:{
					if( !pDST || !pSRC )
						continue;

					switch(oID)
					{
						case gpeOPid_or:
							*pDST |= *pSRC;
							continue;
						case gpeOPid_sub:
							*pDST -= *pSRC;
							continue;
						case gpeOPid_add:
							*pDST += *pSRC;
							continue;
						default:
							*pDST += *pSRC;
							continue;
					}
				} continue;
			case gpeOPid_sub:{
					if( !pDST || !pSRC )
						continue;
					switch(oID)
					{

						default:
							pD[9] = *pDST == *pSRC;
							continue;
					}
				} continue;
		}


	}
	U4 i = 0x400, ie = aA[6];
	while( i >= ie )
	{
		i--;

	}
	return apOUT[3];
}
