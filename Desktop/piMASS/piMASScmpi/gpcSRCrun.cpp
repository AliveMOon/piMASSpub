#include "gpcSRC.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];


U4 gpCORE::entryOBJ2A0( I8x4 *pM0, char	*pSCPall, gpcLZY* pSCPobj, gpcWIN* pWIN, U8& bSW, U4 newC ) {
	nSTK = 0;
	gpcREG wVAR;
	gpO* pOo;

	for( U4 iI = pD[0]-4, eI=pA[7]; iI>=eI; iI-=4, nSTK++ ) {
		//p_src = ea( iI, pO+7, pC+7 );
		aSTK[nSTK].null();

		int oID = *(int*)ea( iI, pO+7, pC+7 ); // p_src;
		if( oID < 0 )
		{
			oID *= -1;
			//int i = -*(int*)p_src;
			char* pSTR = pSCPall + pM0[oID].iMNi+1;
			//n = ;

			return ix_str( pSTR, pM0[oID].iMNn-2, oID );
		}

		gpcLNK& O = *((gpcLNK*)pSCPobj->Ux( oID, sizeof(gpcLNK)));
		gpeTYP typ = O.typ;
		U4 iO;
		switch( O.typ )
		{
			case gpeTYP_sA8: {
					/// ha win variable akkor is csinál ide egy változatot belölle
					/// hogy felül lehessen írni
					if( !nSTK )
					{
						/// most kezdődik
						//U4 nO = oLST.nLD(sizeof(gpO));

						pOo = oIDfnd( oID, iO );
						if( pOo )
						{
							aSTK[nSTK].iX = pOo->iX;
							aSTK[nSTK].pC =
								(aSTK[nSTK].iC = pOo->iC) < gpeCsz_K
								? NULL
								: gpmLZYvali( gpC, &cLST ) + pOo->iC-gpeCsz_K;
							continue;
						}

						gpeALF alf = O.obj.alf;
						if( pWIN->WINvar( wVAR, alf ) )
						{
							// valamit kapott
							bSW |= gpeMASSloopMSK;
							return ix_reg( wVAR, oID );
						}

						pOo = (gpO*)oLST.Ux( iO, sizeof(gpO) );
						pOo->iO = oID;
						pOo->iC = newC;
						pOo->iD = 0;
						pOo->iX = mLST.nLD();
						pOo->szOF = sOF(newC);
						mLST.Ux( pOo->iX, pOo->szOF, true, sizeof(U1) );

						aSTK[nSTK].iX = pOo->iX;
						aSTK[nSTK].pC =
							(aSTK[nSTK].iC = pOo->iC) < gpeCsz_K
							? NULL
							: gpmLZYvali( gpC, &cLST ) + pOo->iC-gpeCsz_K;
						continue;
					}
					aSTK[nSTK].iX = aSTK[nSTK-1].iX;
					aSTK[nSTK].x = aSTK[nSTK-1].pC->cIDfnd( cLST, oID, aSTK[nSTK].iC );
					if( aSTK[nSTK].x > -1 )
					{
						aSTK[nSTK].iX += aSTK[nSTK].x;
						continue;
					}
					aSTK[nSTK].aO = oID;
					aSTK[nSTK].aC = newC;

				} break;
			case gpeTYP_sA8N:
				//if( pBF ) O.obj.an2str( pBF );
				break;
			case gpeTYP_U1:
				//if( pBF ) sprintf( pBF, "0x%0.2x", O.obj.uy );
				switch( newC )
				{
					case gpeCsz_L:
						pOo = oIDfnd( oID, iO );
						if( pOo ? pOo->iX : false )
							return pOo->iX;

						pOo = (gpO*)oLST.Ux( iO, sizeof(gpO) );
						pOo->iO = oID;
						pOo->iC = newC;
						pOo->iD = 0;
						pOo->iX = mLST.nLD();
						pOo->szOF = sOF(newC);
						mLST.Ux( pOo->iX, pOo->szOF, true, sizeof(U1) );

						int* p_dst = (int*)mLST.Ux( pOo->iX, sizeof(int), false, 1 );
						*p_dst = O.obj.uy;

						return pOo->iX;

				}
				break;
			case gpeTYP_U2:
				//if( pBF ) sprintf( pBF, "0x%0.4x", O.obj.uy );
				break;
			case gpeTYP_U4:
				//if( pBF ) sprintf( pBF, "0x%0.8x", O.obj.uy );
				break;
			case gpeTYP_U8:
				//if( pBF ) sprintf( pBF, "0x%0.16llx", O.obj.uy );
				break;

			case gpeTYP_I1:
				//if( pBF ) sprintf( pBF, "0x%.3d", O.obj.num );
				break;
			case gpeTYP_I2:
				//if( pBF ) sprintf( pBF, "0x%.6d", O.obj.num );
				break;
			case gpeTYP_I4:
				//if( pBF ) sprintf( pBF, "0x%.9d", O.obj.num );
				break;
			case gpeTYP_I8:
				//if( pBF ) sprintf( pBF, "0x%.12lld", O.obj.num );
				break;

			case gpeTYP_D:
			case gpeTYP_F:
				//if( pBF ) sprintf( pBF, "%0.7f", O.obj.dy );
				break;
			default:
				//if( pBF ) sprintf( pBF, "%x", O.typ );
				break;
		}
	}
	if( nSTK>1 )
		return aSTK[nSTK-1].iX;

	return 0;
}


gpCORE* gpcSRC::SRCmnMILLrun( gpcMASS* pMASS, gpcWIN* pWIN, gpCORE* pMOM ) {
	if( !this )
		return NULL;

	SRCmnMILLdbg( pMASS->OPER, 0 );
	if( !pDBG->nLD() )
		return NULL;

	//return NULL;

	static const U1 iMN=0;
	I8x4 *pM0 = (I8x4*)SCOOP.mini.p_alloc;
	char	*pALL = (char*)SCOOP.pALL,
			*pSTR;
	I4		nSTR, a, n;
	char sBF[0x100],
		 *pDIS = (char*)pDBG->p_alloc, *pBF = NULL;
	pBF = sBF+1;
	if( pBF ) *sBF = ' ';

	if( !pCORE )
		pCORE = new gpCORE;

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
						(I4x4*)SCOOP.vASM.p_alloc,SCOOP.nASM(),
						&pA, &pD, &pO, &pC
					);
			/// loop CMP --------------
			core.pc < core.nPC;
			/// loop INCRMNT --------------
			core.pc++
		)
	{
		pDIS += gpmVAN( pDIS, "\r\n", nL );
		pDIS += gpmNINCS( pDIS, "\r\n" );
		pPC = core.iPC( op, iOP, iS, xS, mS, iD, xD, mD, iC );
		if( !pPC )
			continue; /// - NOP! --------------

		if( iOP == gpeOPid_dot ) {
			pA[0] = core.entryOBJ2A0( pM0, pALL, &LNK, pWIN, bSW ); //, pD[1] );
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
		szOF = gpaCsz[pPC->iC];

		if( oID == gpeOPid_dot ) {
			/// jsr entryOBJ2A0
			// keressük meg az OBJ-t
			pDOT = NULL;
			for( U4 ie = pD[0], ii=aA[7]; ii<ie; ii++ ) {
				I4 o = *(I8*)( aMEM[iS].Ux(ii,sizeof(I8)) );
				if( o >= 0 ) {
					gpcLNK& O = ((gpcLNK*)LNK.Ux( o, sizeof(gpcLNK)))[0];
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
					NUM = (gpcLNK*)OBJ.Ux( ob, sizeof(gpcLNK));
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
