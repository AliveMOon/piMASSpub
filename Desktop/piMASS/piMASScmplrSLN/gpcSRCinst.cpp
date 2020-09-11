#include "gpcSRC.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];


#define _b c(gpeCsz_b)
#define _w c(gpeCsz_w)
#define _l c(gpeCsz_l)
#define _q c(gpeCsz_q)
#define _4 c(gpeCsz_4)

#define _ptr c(gpeCsz_ptr)
#define _a c(gpeCsz_a)
#define _c c(gpeCsz_c)

#define _B c(gpeCsz_B)
#define _W c(gpeCsz_W)
#define _L c(gpeCsz_L)
#define _Q c(gpeCsz_Q)

#define _f c(gpeCsz_f)
#define _d c(gpeCsz_d)

#define _K c(gpeCsz_K)
#define _OFF c(gpeCsz_OFF)

#define _move	pMEM->inst( gpeOPid_mov )
#define _add	pMEM->inst( gpeOPid_add )
#define _sub	pMEM->inst( gpeOPid_sub )

#define _mul	pMEM->inst( gpeOPid_mul )
#define _div	pMEM->inst( gpeOPid_div )
#define _rem	pMEM->inst( gpeOPid_rem )

#define A0 An(0)
#define A1 An(1)
#define A2 An(2)
#define A3 An(3)
#define A4 An(4)
#define A5 An(5)
#define A6 An(6)
#define A7 An(7)

#define IA0I IAnI(0)
#define IA1I IAnI(1)
#define IA2I IAnI(2)
#define IA3I IAnI(3)
#define IA4I IAnI(4)
#define IA5I IAnI(5)
#define IA6I IAnI(6)
#define IA7I IAnI(7)


#define D0 Dn(0)
#define D1 Dn(1)
#define D2 Dn(2)
#define D3 Dn(3)
#define D4 Dn(4)
#define D5 Dn(5)
#define D6 Dn(6)
#define D7 Dn(7)

gpINST& gpINST::dbg( gpcLZY* pDBG, gpMEM* pMEM, U1* pU1 ) {
	if( pMEM ? !pDBG : true )
		return *this;

	U8 s = -1;
	char sBUFF[0x100], *pB = sBUFF;
	pB += gpfALF2STR( pB, pMEM->pMASS->aOP[op].alf );

	pDBG = pDBG->lzyFRMT( (s=-1), "\r\n0x%0.8x %s%s", (U4)((U1*)this-pU1), sBUFF, gpasCsz[cID] );
	U1	*pUs = NULL;
	switch( mS )
	{
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

	switch( mD )
	{
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
						pUs = pMEM->pSRC->srcMEMiPC( a8x2.x, gpaCsz[cID] );
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
gpBLOCK* gpcSRC::srcINSTmov( char* pS, gpBLOCK *pBLKm, gpBLOCK* pBLK )
{
	if( !pBLKm )
		pBLKm = lzyBLOCK.pSTPdwn( pBLK->bIDm );

	gpROW *pR0 = pBLK->pROW();
	I4 iPC, sOF = 0;
	U1* pU1 = NULL;
	for( I4 i = pBLK->nROW()-1, nR = pBLK->nROW(); i >= 0; i-- )
	{
		iPC = iPCrow( pR0+i, sOF );
		pU1 = srcMEMiPC( iPC, sOF );
		//_move.l #iPC.Aa;
		_move._L.EAl( iPC ).A0;
		if( !i ){
			// move.l (A0),d0
			_move._L.IA0I.D0;
			continue;
		}
		// move.l d0,(A0)
		pMEM->inst( (gpeOPid)pR0[i].pstOP )._L.D0.IA0I;
	}

	gpROW *pRm = pBLKm->pROW(pBLK->bIDmR);
	iPC = iPCrow( pRm, sOF );
	pU1 = srcMEMiPC( iPC, sOF );
	_move._L.EAl(iPC).A0;
	_move._L.D0.IA0I;
	return pBLKm;
}

gpBLOCK* gpcSRC::srcINSTadd( char* pS, gpBLOCK *pBLKm, gpBLOCK* pBLK ) {
	if( !pBLKm )
		pBLKm = lzyBLOCK.pSTPdwn( pBLK->bIDm );

	gpROW	*pR = pBLK->pROW();
	I4 iPC, sOF = 0;
	U1* pU1 = NULL;
	for( I4 i = 0, nR = pBLK->nROW(); i < nR; i++ )
	{
		iPC = iPCrow( pR+i, sOF );
		pU1 = srcMEMiPC( iPC, sOF );
		//_move.l #iPC.Aa;
		_move._L.EAl(iPC).A0;
		if( !i ){
			// move.l (A0),d0
			_move._L.IA0I.D0;
			continue;
		}
		// add.l (A0),d0
		pMEM->inst( (gpeOPid)pR[i].pstOP )._L.IA0I.D0;
	}

	pR = pBLKm->pROW(pBLK->bIDmR);
	iPC = iPCrow( pR, sOF );
	pU1 = srcMEMiPC( iPC, sOF );
	_move._L.EAl( iPC ).A0;
	_move._L.D0.IA0I;
	// move.l d0, -(A6)
	return pBLKm;
}
gpBLOCK* gpcSRC::srcINSTmul( char* pS, gpBLOCK *pBLKm, gpBLOCK* pBLK  ) {
	if( !pBLKm )
		pBLKm = lzyBLOCK.pSTPdwn( pBLK->bIDm );

	gpROW	*pR = pBLK->pROW();
	I4 iPC = 0 , sOF = 0;
	U1* pU1 = NULL;
	for( I4 i = 0, nR = pBLK->nROW(); i < nR; i++ )
	{
		iPC = iPCrow( pR+i, sOF );
		pU1 = srcMEMiPC( iPC, sOF );
		//_move.l #iPC.Aa;
		_move._L.EAl(iPC).A0;
		if( !i ){
			// move.l (A0),d0
			_move._L.IA0I.D0;
			continue;
		}
		// add.l (A0),d0
		pMEM->inst( (gpeOPid)pR[i].pstOP )._L.IA0I.D0;
	}

	pR = pBLKm->pROW(pBLK->bIDmR);
	iPC = iPCrow( pR, sOF );
	pU1 = srcMEMiPC( iPC, sOF );
	_move._L.EAl( iPC ).A0;
	_move._L.D0.IA0I;
	return pBLKm;
}
bool gpcSRC::srcINSTrun( gpcMASS* pMASS, gpcWIN* pWIN ) {
	if( pMEM ? !pMEM->nCD : true )
		return false;

	I4 nPC = pMEM->nCD;
	gpINST*pI;
	while( pMEM->pc < nPC )
	{
		pI = pMEM->instALU();
	}


	return true;
}
gpcLZY* gpcSRC::srcINSTmini( gpcLZY* pLZY, gpcMASS* pMASS, gpcWIN* pWIN ) {
	pLZY->lzyRST();
	I4 nO = 0, sOF = 0;
	if( pMEM ? !(nO=pMEM->lzyOBJ.nLD(sizeof(gpOBJ))) : true )
		return pLZY;
	char sBUFF[0x100];
	gpOBJ* pO0 = gpmLZYvali( gpOBJ, &pMEM->lzyOBJ ); //(gpOBJ*)pMEM->lzyOBJ.Ux( 0, sizeof(*pO0) );
	U4x4* pL0 = aSCOOP[0].pLNK();
	I8x4* pMN0 = aSCOOP[0].pMN();
	U1* pU1, *pSRC = aSCOOP[0].p_str;
	const char *pS;
	U8 s = -1, nS;
	for( U4 i = 0, iMN; i < nO; i++ )
	{
		gpOBJ& obj = pO0[i];
		if( !(sOF=obj.sOF()) )
			continue;
		if( obj.dctID < 0 )
		{
			if( obj.iPC < 4)
				continue;
			pLZY = pLZY->lzyFRMT( (s=-1), "\r\n " );
			pU1 = srcMEMiPC( obj.iPC, obj.sOF() );
			pLZY = pLZY->lzyFRMT( (s=-1), "%s,", pU1 );

			//pLZY = pLZY->lzyADD( pU1, obj.sOF(), (s=-1), -1 );
			continue;
		}
		pLZY = pLZY->lzyFRMT( (s=-1), "\r\n " );
		//iMN = pL0[obj.dctID].x;
		pS = aSCOOP[0].lzyDCT.sSTRix(obj.dctID, NULL);
		nS = aSCOOP[0].lzyDCT.nSTRix(obj.dctID);
		pLZY = pLZY->lzyADD( //pSRC+pMN0[iMN].iS, pMN0[iMN].nS,
								pS, nS,
								(s=-1), -1 );
		pLZY = pLZY->lzyFRMT( (s=-1), "=" );
		(s=-1);

		pU1 = srcMEMiPC( obj.iPC, obj.sOF() );
		switch( obj.cID() )
		{
			case gpeCsz_Q: pLZY = pLZY->lzyFRMT( s, " %lld,", *(I8*)pU1 ); break;
			case gpeCsz_q: pLZY = pLZY->lzyFRMT( s, " %lld,", *(U8*)pU1 ); break;
			case gpeCsz_L: pLZY = pLZY->lzyFRMT( s, " %d,", *(int*)pU1 ); break;
			case gpeCsz_l: pLZY = pLZY->lzyFRMT( s, " %d,", *(U4*)pU1 ); break;
			case gpeCsz_W: pLZY = pLZY->lzyFRMT( s, " %d,", *(I2*)pU1 ); break;
			case gpeCsz_w: pLZY = pLZY->lzyFRMT( s, " %d,", *(U2*)pU1 ); break;
			case gpeCsz_B: pLZY = pLZY->lzyFRMT( s, " %d,", *(I1*)pU1 ); break;
			case gpeCsz_b: pLZY = pLZY->lzyFRMT( s, " %d,", *(U1*)pU1 ); break;
			case gpeCsz_f: pLZY = pLZY->lzyFRMT( s, " %f,", *(float*)pU1 ); break;
			case gpeCsz_d: pLZY = pLZY->lzyFRMT( s, " %f,", *(double*)pU1 ); break;
			case gpeCsz_4: pLZY = pLZY->lzyFRMT( s, " %d,", *(I1*)pU1 ); break;
			default: break;

		}
	}


	return pLZY;
}
gpINST* gpMEM::instALU()
{
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

	return &ins;
}
