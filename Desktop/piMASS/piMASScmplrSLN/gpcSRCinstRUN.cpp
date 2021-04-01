#include "gpcSRC.h"
#include "gpcSRCinst.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];


gpINST& gpINST::instDBG( gpcLZY* pDBG, gpMEM* pMEM, U1* pU1 ) {
	if( pMEM ? !pDBG : true )
		return *this;

	U8 s = -1;
	char sBUFF[0x100], *pB = sBUFF;
	if( op == gpeOPid_nop ) {
		pDBG = pDBG->lzyFRMT(
								(s=-1),
								"\r\n0x%0.8x nop; //----------------",
								(U4)((U1*)this-pU1)
							);
		return *this;
	}

	switch( op )
	{
		case gpeOPid_CMP:
			pB += sprintf( pB, "cmp" );
			break;
		case gpeOPid_SEQ:
			pB += sprintf( pB, "seq" );
			break;
		case gpeOPid_SNE:
			pB += sprintf( pB, "sne" );
			break;
		case gpeOPid_SWAP:
			pB += sprintf( pB, "swap" );
			break;
		case gpeOPid_EXTB:
			pB += sprintf( pB, "extB" );
			break;
		case gpeOPid_EXT:
			pB += sprintf( pB, "ext" );
			break;
		case gpeOPid_EXTL:
			pB += sprintf( pB, "extL" );
			break;
		case gpeOPid_jsr:
			pB += gpfALF2STR( pB, a8x2.alf );
			pDBG = pDBG->lzyFRMT( (s=-1), "\r\n0x%0.8x jsr %s", (U4)((U1*)this-pU1), sBUFF );
			return *this;
		case gpeOPid_mov:
			pB += sprintf( pB, "move" );
			break;
		default:
			pB += gpfALF2STR( pB, pMEM->pMASS->aOP[op].alf );
	}

	pDBG = pDBG->lzyFRMT( (s=-1), "\r\n0x%0.8x %s%s", (U4)((U1*)this-pU1), sBUFF, gpasCsz[cID] );
	U1	*pUs = NULL;
	switch( mS ) {
		case gpeEA_OFF:
			return *this;
		case gpeEA_Dn:
			pDBG = pDBG->lzyFRMT( (s=-1), " D%x", iS );
			break;
		case gpeEA_An:
			pDBG = pDBG->lzyFRMT( (s=-1), " A%x", iS );
			break;
		case gpeEA_IAnI:
			pDBG = pDBG->lzyFRMT( (s=-1), " (A%x)", iS );
			break;
		case gpeEA_IAnIp:
			pDBG = pDBG->lzyFRMT( (s=-1), " (A%x)+", iS );
			break;
		case gpeEA_sIAnI:
			pDBG = pDBG->lzyFRMT( (s=-1), " -(A%x)", iS );
			break;
		case gpeEA_d16IAnI:
			pDBG = pDBG->lzyFRMT( (s=-1), " %lld(A%x)", a8x2.x, iS );
			break;
		case gpeEA_d16IAnDnI:
			pDBG = pDBG->lzyFRMT( (s=-1), " %lld(A%x,D%x)", a8x2.x, iS, xS );
			break;
		case gpeEA_d16IPcI:
			pDBG = pDBG->lzyFRMT( (s=-1), " %lld(PC)", a8x2.x );
			break;

		case gpeEA_d16IPcDnI:
			pDBG = pDBG->lzyFRMT( (s=-1), " %lld(PC,D%x)", a8x2.x, iS );
			break;
		case gpeEA_W:
		case gpeEA_L:
			pDBG = pDBG->lzyFRMT( (s=-1), " 0x%llx", a8x2.x );
			break;

		default:
			return *this;
	}

	switch( mD ) {
		case gpeEA_OFF:
			return *this;
		case gpeEA_Dn:
			pDBG = pDBG->lzyFRMT( (s=-1), ",D%x", iD );
			break;
		case gpeEA_An: {
				pDBG = pDBG->lzyFRMT( (s=-1), ",A%x", iD );
				switch( mS )
				{
					case gpeEA_W:
					case gpeEA_L:
						pUs = pMEM->pUn( a8x2.x, gpaCsz[cID] );
						break;
					default:
						break;
				}
				if( !pUs )
					break;
				s = -1;
				switch( cID )
				{
					case gpeCsz_Q: pDBG = pDBG->lzyFRMT( s, "\t\t; //%lld,", *(I8*)pUs ); break;
					case gpeCsz_q: pDBG = pDBG->lzyFRMT( s, "\t\t; //%lld,", *(U8*)pUs ); break;
					case gpeCsz_L: pDBG = pDBG->lzyFRMT( s, "\t\t; //%d,", *(int*)pUs ); break;
					case gpeCsz_l: pDBG = pDBG->lzyFRMT( s, "\t\t; //%d,", *(U4*)pUs ); break;
					case gpeCsz_W: pDBG = pDBG->lzyFRMT( s, "\t\t; //%d,", *(I2*)pUs ); break;
					case gpeCsz_w: pDBG = pDBG->lzyFRMT( s, "\t\t; //%d,", *(U2*)pUs ); break;
					case gpeCsz_B: pDBG = pDBG->lzyFRMT( s, "\t\t; //%d,", *(I1*)pUs ); break;
					case gpeCsz_b: pDBG = pDBG->lzyFRMT( s, "\t\t; //%d,", *(U1*)pUs ); break;
					case gpeCsz_f: pDBG = pDBG->lzyFRMT( s, "\t\t; //%f,", *(float*)pUs ); break;
					case gpeCsz_d: pDBG = pDBG->lzyFRMT( s, "\t\t; //%f,", *(double*)pUs ); break;
					case gpeCsz_4: pDBG = pDBG->lzyFRMT( s, "\t\t; //%d,", *(I1*)pUs ); break;
					default: break;

				}
			} break;
		case gpeEA_IAnI:
			pDBG = pDBG->lzyFRMT( (s=-1), ",(A%x)", iD );
			break;
		case gpeEA_IAnIp:
			pDBG = pDBG->lzyFRMT( (s=-1), ",(A%x)+", iD );
			break;
		case gpeEA_sIAnI:
			pDBG = pDBG->lzyFRMT( (s=-1), ",-(A%x)", iD );
			break;
		case gpeEA_d16IAnI:
			pDBG = pDBG->lzyFRMT( (s=-1), ",%lld(A%x)", a8x2.y, iD );
			break;
		case gpeEA_d16IAnDnI:
			pDBG = pDBG->lzyFRMT( (s=-1), ",%lld(A%x,D%x)", a8x2.y, iD, xD );
			break;
		case gpeEA_d16IPcI:
			pDBG = pDBG->lzyFRMT( (s=-1), ",%lld(PC)", a8x2.y );
			break;

		case gpeEA_d16IPcDnI:
			pDBG = pDBG->lzyFRMT( (s=-1), ",%lld(PC,D%x)", a8x2.y, iD );
			break;
		case gpeEA_W:
		case gpeEA_L:
			pDBG = pDBG->lzyFRMT( (s=-1), ",0x%llx", a8x2.y );
			break;
		default:
			return *this;
	}



	return *this;
}

gpcLZY* gpcSRC::srcINSTmini( gpcLZY* pLZY ) { //, gpcMASS* pMASS, gpcWIN* pWIN ) {
	pLZY->lzyRST();
#ifdef stdON
	if(bSTDcout){std::cout << stdMINI " MIN" stdRESET << std::endl;}
#endif // stdOFF

	I4 nO = 0, sOF = 0;
	if( pMEM ? !(nO=pMEM->lzyOBJ.nLD(sizeof(gpOBJ))) : true )
		return pLZY;

	pMEM->pWgl = pMEM->pWIN ? pMEM->pWIN->pGL : NULL;
	pMEM->pMgl->nBUILD();
	char sBUFF[0x100];
	gpOBJ	*pOmn = pMEM->pOBJ(gpeALF_MINI),
			*pO0 = gpmLZYvali( gpOBJ, &pMEM->lzyOBJ ); //(gpOBJ*)pMEM->lzyOBJ.Ux( 0, sizeof(*pO0) );

	U4x4* pL0 = aSCOOP[0].pLNK();
	I8x4* pMN0 = aSCOOP[0].pMN();
	U1* pU1, *pSRC = aSCOOP[0].p_str, *pUdbg = NULL;
	const char *pS;
	U8 s = -1, nS;
	bool bTMP;
	U4 cID, area = 1;
	gpPTR* pPTR = NULL;

	for( U4 i = 0; i < nO; i++ ) {
		gpOBJ& obj = pO0[i];
		if( !obj.AN.alf )
			continue;
		if( !(sOF=obj.sOF()) )
			continue;
        if( obj.iPTR < 0)
				continue;
		gpPTR* pP = obj.pPTRu1();
		if( pP->iPC < 0 )
			continue;

		pU1 = pP->pU1(obj.pMEM);

		if( pOmn ) {
			if( pOmn != &obj ) {
				pMEM->instDOit( obj, pU1 );
				continue;
			}
			cID = pP->cID();
			if( pP->cID() == gpeCsz_b ){
				pLZY = pLZY->lzyFRMT( (s=-1), "%s", pU1?(char*)pU1+1:"?" );
				if( pLZY->nLD() ) {
					pLZY->n_load--;
					pLZY->p_alloc[pLZY->nLD()]=0;
				}
			}
			continue;
		} else {
			cID = pP->cID();
			area = pP->pd2D()->area();
		}

		bTMP = obj.dctID < 0 || (obj.cAN != gpeCsz_a);
		pLZY = pLZY->lzyFRMT( (s=-1), "\r\n%s0x%x ", bTMP?"//":"  ",obj.iPTR );
		pUdbg = pLZY ? pLZY->p_alloc : NULL;


        nS = aSCOOP[0].lzyDCT.nSTRix(obj.dctID);
        if( !nS )
			continue;

		//cID = pP->cID();
		pS = aSCOOP[0].lzyDCT.sSTRix(obj.dctID, NULL);
		pLZY = pLZY->lzyADD( pS, nS, (s=-1), -1 );
		pLZY = pLZY->lzyFRMT( (s=-1), "=" );

		/// ---------------------------------
		///
		/// 			instDOit
		///
		/// ---------------------------------
		int iDi = pMEM->instDOit( obj, pU1 );
//		if( iDi == gpeCsz_OFF )  // nem volot sikeres
//            iDi = gpeCsz_L; // akkor L azaz signed int


		(s=-1);
		switch( cID ) {
            case gpeCsz_ptr:
            case gpeCsz_c:
            case gpeCsz_a:
				if( area > 1 ) {
					switch( cID )
					{
						case gpeCsz_B:
						case gpeCsz_b:
								pLZY = pLZY->lzyFRMT( s, " %s,", pU1?(char*)pU1:"?" );
						default:
							break;
					}
				} break;
			case gpeCsz_Q: pLZY = pLZY->lzyFRMT( s, " %lld,", *(I8*)pU1 ); break;
			case gpeCsz_q: pLZY = pLZY->lzyFRMT( s, " 0x%0.16llx,", *(U8*)pU1 ); break;
			case gpeCsz_L: pLZY = pLZY->lzyFRMT( s, " %d,", *(int*)pU1 ); break;
			case gpeCsz_l: pLZY = pLZY->lzyFRMT( s, " 0x%0.8x,", *(U4*)pU1 ); break;
			case gpeCsz_W: pLZY = pLZY->lzyFRMT( s, " %d,", *(I2*)pU1 ); break;
			case gpeCsz_w: pLZY = pLZY->lzyFRMT( s, " 0x%0.4x,", *(U2*)pU1 ); break;
			case gpeCsz_B: pLZY = pLZY->lzyFRMT( s, " %d,", *(I1*)pU1 ); break;
			case gpeCsz_b:	if( area > 1 ) {
								pLZY = pLZY->lzyFRMT( s, " %s,", pU1?(char*)pU1:"?" );
								break;
							}
							pLZY = pLZY->lzyFRMT( s, " 0x%0.2x,", *(U1*)pU1 );
							break;
			case gpeCsz_f: pLZY = pLZY->lzyFRMT( s, " %f,", *(float*)pU1 ); break;
			case gpeCsz_d: pLZY = pLZY->lzyFRMT( s, " %f,", *(double*)pU1 ); break;
			case gpeCsz_4: pLZY = pLZY->lzyFRMT( s, " %d,", *(I1*)pU1 ); break;
			default: break;

		}
	}
	pUdbg = pLZY ? pLZY->p_alloc : NULL;
	return pLZY;
}

bool gpcSRC::srcINSTrun() {
	if( pMEM ? !pMEM->nCD : true )
		return false;
	I4 nPC = pMEM->nCD;
	if( pMEM->pc >= nPC )
	{
		if( pMEM->msRUN ? pMEM->msRUN >= pMEM->pWIN->mSEC.x : true )
			return false;
		pMEM->pc = 0;
	}
#ifdef stdON
	if(bSTDcout){std::cout << stdRUN " RUN" stdRESET; // << std::endl;}
#endif // stdOFF

	gpINST*pI;
	while( pMEM->pc < nPC )
	{
		pI = pMEM->instALU();
	}


	return true;
}



