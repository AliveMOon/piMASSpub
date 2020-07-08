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

	/// -------------------------------------
	return NULL;


	static const U1 iMN=0;
	I8x4 *pM0 = (I8x4*)SCOOP.mini.p_alloc;
	U1	//Xs,
		Ys, Xd, Yd, bOBs = false, bOBd = false;
	gpeEA mS, mD;
	char	*pALL = (char*)SCOOP.pALL,
			*pSTR;
	I4		nSTR, a;

	gpcLZY	aMEM[0x10];
	gpcREG	aA[8],
			aD[8+2], *pSRC, *pDST, *pTMP;

	gpcPIK	PIK;
	gpcO	*pDOT, *pOUT, wipO;
	gpcSRC	*pANs;
	char* pDIS = (char*)pDBG->p_alloc;
	for( I4x4* pPCs = (I4x4*)SCOOP.vASM.p_alloc, *pPC = pPCs, *pPCe = pPC+SCOOP.nASM();
			pPC < pPCe; pPC++ )
	{
		pDIS += gpmVAN( pDIS, "\r\n", nL );
		pDIS += gpmNINCS( pDIS, "\r\n" );
		U1x4 op = //*(U1x4*)&
					pPC->op;
		if( !op.x ) // nop
			continue;
		else if( op.x == gpeOPid_dot )
		{
			// keressük meg az OBJ-t
			pDOT = NULL;
			for(U4	ie = aD->i8(), ii=aA[7].i8(); ii<ie; ii++   )
			{
				pTMP = (gpcREG*)( aMEM[op.y&7].Ux(ii,sizeof(gpcREG)) );
				I4 o = pTMP->i8();
				if( o >= 0 )
				{
					gpcOBJlnk& O = ((gpcOBJlnk*)OBJ.Ux( o, sizeof(gpcOBJlnk)))[0];
					gpeTYP typ = O.typ;
					gpeALF alf = O.obj.alf;
					switch( O.typ )
					{
						case gpeTYP_sA8:
							if( pDOT )
							{
								pDOT = pDOT->fnd( PIK, wipO, alf );
								if( pANs ? pANs->bSW&gpeMASSloopMSK : false )
									bSW |= gpeMASSloopMSK;
								//aA[0] = adr.pRM->ALU( adr.iA ).i8();
								continue;
							}
							if( pOUT )
							{
								pOUT = pOUT->fnd( PIK, wipO, alf );
								continue;
							}
							if( pWIN->WINvar( aA[0], alf ) )
							{
								// valamit kapott
								bSW |= gpeMASSloopMSK;
								pSTR = (char*)aA[0].getSTR();
								continue;
							}
							/// ITT kell létrehozni ha nincsen
							pOUT = pOUT->add( PIK, wipO, alf );
							if( !pOUT )
							{
								aA[0] = 0;
								ii = ie;
								continue;
							}

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
				pSRC = aD+(op.y&7);
				break;
			case gpeEA_An:
				pSRC = aA+(op.y&7);
				break;
			case gpeEA_IAnI:
				pSRC = (gpcREG*)( aMEM[op.y&7].Ux( aA[op.y&7].i8(),sizeof(*aA)) );
				break;
			case gpeEA_IAnIp:
				pSRC = (gpcREG*)( aMEM[op.y&7].Ux( aA[op.y&7].i8(),sizeof(*aA)) );
				++aA[op.y&7];
				break;
			case gpeEA_sIAnI:
				--aA[op.y&7];
				pSRC = (gpcREG*)( aMEM[op.y&7].Ux( aA[op.y&7].i8(),sizeof(*aA)) );
				break;
			case gpeEA_d16IAnI:
				a = aA[op.y&7].i8() + pPC->aOB[0];
				pSRC = (gpcREG*)( aMEM[op.y&7].Ux( a,sizeof(*aA)) );
				break;
			case gpeEA_d16IAnDnI:
				a = aA[op.y&7].i8() + pPC->aOB[0] + aD[op.w&7].i8();
				pSRC = (gpcREG*)( aMEM[op.y&7].Ux( a,sizeof(*aA)) );
				break;
			case gpeEA_d16IPcI:
				/*a = (pPC-pPCs)+pPC->aOB[0];
				pSRC = &(aRES[op.y&7].ALU(a));
				break;*/
			case gpeEA_d16IPcDnI:
				pSRC = NULL;
				break;
				/*a = (pPC-pPCs)+pPC->aOB[0]+aD[op.w&7].i8();
				pSRC = &(aRES[op.y&7].ALU(a));
				break;*/
			case gpeEA_num:
			default:{
				pSRC = &(aD[8]=*pPC->aOB);
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
		switch( mD ) {
			case gpeEA_OFF:
				pDST = NULL;
				continue;
			case gpeEA_Dn:
				pDST = aD+(op.z&7);
				break;
			case gpeEA_An:
				pDST = aA+(op.z&7);
				break;
			case gpeEA_IAnI:
				pDST = (gpcREG*)( aMEM[op.z&7].Ux( aA[op.z&7].i8(),sizeof(*aA)) );
				break;
			case gpeEA_IAnIp:
				pDST = (gpcREG*)( aMEM[op.z&7].Ux( aA[op.z&7].i8(),sizeof(*aA)) );
				++aA[op.z&7];
				break;
			case gpeEA_sIAnI:
				--aA[op.z&7];
				pDST = (gpcREG*)( aMEM[op.z&7].Ux( aA[op.z&7].i8(),sizeof(*aA)) );
				break;
			case gpeEA_d16IAnI:
				a = aA[op.z&7].i8() + pPC->aOB[0];
				pDST = (gpcREG*)( aMEM[op.z&7].Ux( a,sizeof(*aA)) );
				break;
			case gpeEA_d16IAnDnI:
				a = aA[op.z&7].i8() + pPC->aOB[0] + aD[op.w>>4].i8();
				pDST = (gpcREG*)( aMEM[op.z&7].Ux( a,sizeof(*aA)) );
				break;
			case gpeEA_d16IPcI:
			case gpeEA_d16IPcDnI:
				pDST = NULL;
				break;
			case gpeEA_num:
			default:{
				pDST = &(aD[9]=*pPC->aOB);
			} break;
		}

		if(op.x>=gpeOPid_jsr)
		{
			// jsr;
			continue;
		}

		switch( gpaOPgrp[op.x] )
		{
			case gpeOPid_entry:{
					switch( (gpeOPid)op.x )
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

					*pDST = *pSRC;
				} continue;
			case gpeOPid_mul:{
					if( !pDST || !pSRC )
						continue;

					*pDST *= *pSRC;
				} continue;
			case gpeOPid_add:{
					if( !pDST || !pSRC )
						continue;

					*pDST += *pSRC;
				} continue;
			case gpeOPid_sub:{
					if( !pDST || !pSRC )
						continue;

					*pDST -= *pSRC;
				} continue;
		}


	}
	U4 i = 0x400, ie = aA[6].i8();
	while( i >= ie )
	{
		i--;

	}
	return apOUT[3];
}
