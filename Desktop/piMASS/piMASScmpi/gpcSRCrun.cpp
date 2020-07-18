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
	U8		*pA, *pD;
	I8		sNUM, dNUM,
	U1		*pS, *pD;
	U1x4 op;
	gpeOPid oID;
	U1 iS, iD;
	gpeEA mS, mD;
	U4 szOF;

	for(	core.ini(
						(I4x4*)SCOOP.vASM.p_alloc,SCOOP.nASM(),
						&pA, &pD
					);
			core.pc < core.nPC;
			core.pc++ )
	{
		pDIS += gpmVAN( pDIS, "\r\n", nL );
		pDIS += gpmNINCS( pDIS, "\r\n" );
		pPC = core.iPC( op, oID, iS, mS, iD, mD, szOF );
		if( !pPC )
			continue; // NOP?

		if( oID == gpeOPid_dot ) {

			continue;
		}

		switch( mS ) {
			case gpeEA_OFF:
				pS = NULL;
				continue;
			case gpeEA_Dn:
				pS = pD+iS; // pD[iS];
				break;
			case gpeEA_An:
				pS = pA+iS;
				break;
			case gpeEA_IAnI:
				pS = core.ea( pA[iS] );
				break;
			case gpeEA_IAnIp:
				pS = core.ea( 0, pA[iS] );
				pA[iS] += szOF;
				break;
			case gpeEA_sIAnI:
				pA[iS] += szOF;
				pS = core.ea( 0, pA[iS] );
				break;
			case gpeEA_d16IAnI:
				pS = core.ea( pPC->aOB[0], pA[iS] );
				break;
			case gpeEA_d16IAnDnI:
				pS = core.ea( pPC->aOB[0] + pD[op.w&7], pA[iS] );
				break;
			case gpeEA_d16IPcI:
				pS = pPC->aOB[0]+(I8)pPC;
				break;

			case gpeEA_d16IPcDnI:
				pS = pPC->aOB[0]+pD[op.w&7]+(I8)pPC;
				break;

			case gpeEA_num:
			default:{
				pS = (U1*)&(sNUM=*pPC->aOB);
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
