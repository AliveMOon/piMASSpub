#include "gpcSRC.h"
#include "gpcSRCinst.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];


gpINST& gpINST::dbg( gpcLZY* pDBG, gpMEM* pMEM, U1* pU1 ) {
	if( pMEM ? !pDBG : true )
		return *this;

	U8 s = -1;
	if( op == gpeOPid_nop )
	{
		pDBG = pDBG->lzyFRMT(
								(s=-1),
								"\r\n0x%0.8x nop; //----------------",
								(U4)((U1*)this-pU1)
							);
		return *this;
	}
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
gpBLOCK* gpcSRC::srcINSTmov2( char* pS, gpBLOCK *pBLKm, gpBLOCK* pBLK ) {
	if( !pBLKm )
		pBLKm = lzyBLOCK.pSTPdwn( pBLK->bIDm );

	gpROW *pR0 = pBLK->pROW();
	I4 iPC, sOF = 0, cID = -1;
	U1* pU1 = NULL;
	for( I4 i = pBLK->nROW()-1, j = 0, nR = pBLK->nROW(); i >= 0; i-- )
	{
		iPC = iPCrow( pR0[i], sOF, true );
		pU1 = srcMEMiPC( iPC, sOF );
		if( !j )
		{
			j = 1;
			_move._L.EAl( iPC ).A0;
			_move.c( (gpeCsz)(cID=pR0[i].cID) ).IA0I.D0;
			continue;
		}

		if( cID != pR0[i].cID )
		{
			/// convert D0 -> pR0[i].cID
			cID = pR0[i].cID;
		}
		if( (gpeOPid)pR0[i].pstOP == gpeOPid_mov )
		{
			if( cID<gpeCsz_K )
				_move.c((gpeCsz)cID).D0.IA0I;
			else
				_move._L.D0.IA0I;
			continue;
		}

		_move._q.D0.D1;

		if( cID<gpeCsz_K )
			_move.c((gpeCsz)cID).IA0I.D0;
		else
			_move._L.IA0I.D0;

		if( cID<gpeCsz_K )
		{
			switch( pR0[i].pstOP )
			{
				case gpeOPid_addM: _add.c((gpeCsz)cID).D1.D0; break;
				case gpeOPid_subM: _sub.c((gpeCsz)cID).D1.D0; break;
				case gpeOPid_mulM: _mul.c((gpeCsz)cID).D1.D0; break;
				case gpeOPid_divM: _div.c((gpeCsz)cID).D1.D0; break;
				case gpeOPid_remM: _rem.c((gpeCsz)cID).D1.D0; break;
				default: break;
			}
		}

		if( cID<gpeCsz_K )
		{
			_move.c((gpeCsz)cID).D0.IA0I;
			continue;
		}

		_move._L.D0.IA0I;


	}

	gpROW *pRm = pBLKm->pROW(pBLK->bIDmR);
	iPC = iPCrow( *pRm, sOF, false );
	pU1 = srcMEMiPC( iPC, sOF );
	_move._L.EAl(iPC).A0;
	_move._L.D0.IA0I;
	_nop;
	return pBLKm;
}
gpBLOCK* gpcSRC::srcINSTmov( char* pS, gpBLOCK *pBLKm, gpBLOCK* pBLK ) {
	if( !pBLKm )
		pBLKm = lzyBLOCK.pSTPdwn( pBLK->bIDm );

	I4	nR = pBLK->nROW(),
		iPCm = 0, 	iPCa = 0, 	iPCb = 0,
		sOFm = 0,	sOFa = 0,	sOFb = 0,
		cIDm = -1,	cIDa = -1,	cIDb = -1,
		nM = 0,		nA = 0, 	nB = 1;


	gpROW	*pR0 = pBLK->pROW(),

			// *pRa = NULL,
			*pRb = NULL,
			*pRm = NULL;

	gpOBJ	*pOa = NULL,
			*pOb = NULL,
			*pOm = NULL;

	U1		*pUm = NULL,
			*pUa = NULL,
			*pUb = NULL;

	bool bD0 = false;

	//for( I4 i = nR-1; i >= 0; i-- )
	for( gpROW* pRa = pR0+nR-1; pRa >= pR0; pRb = pRa, pRa-- )
	{
		iPCb = iPCa; sOFb = sOFa;
		pOb = pOa;
		if( nA < 2 )
			nB = 1;
		else
			nB = pOb ? pOb->d2D.area() : 1;
		cIDb = cIDa;
		pUb = pUa;

		iPCa 	= iPCrow( *pRa, sOFa, true );
		pOa 	= srcOBJfnd( pRa->mNdID );
		nA 		= pOa ? pOa->d2D.area() : 1;

		pUa = srcMEMiPC( iPCa, sOFa );

		if( !pRb )
		{
			pRm 	= pBLKm->pROW(pBLK->bIDmR);
			iPCm 	= pRm ? iPCrow( *pRm, sOFm, false ) : 0;
			pOm 	= pRm ? srcOBJfnd( pRm->mNdID ) : NULL;
			cIDm	= pRm ? pRm->cID : gpeCsz_L;
			nM		= pOm ? pOm->d2D.area() : 0;

			pUm		= srcMEMiPC( iPCm, sOFm );
			continue;
		}

		if( nB < 2 )
		{
			/// SINGLE VARI
			if( cIDa != pRa->cID ) {
				/// convert D0 -> pR0[i].cID
				cIDa = pRa->cID;
			}

			if( !bD0 ) {
				/// SRC
				// B ------------
				_move._L.EAl( iPCb ).A0;
				_move.c((gpeCsz)cIDa).IA0I.D0;
			}

			if( (gpeOPid)pRa->pstOP == gpeOPid_mov ) {
				/// Síma egyenlő
				if( cIDa<gpeCsz_K )
					_move.c((gpeCsz)cIDa).D0.IA0I;
				else
					_move._L.D0.IA0I;
				continue;
			}

			/// müvelet+egyenlő
			/// SRC ig mst B ---------------------
			_move._q.D0.D1;	// elöző kör lesz a forás

			/// TRG A ---------------------
			if( cIDa<gpeCsz_K )
				_move.c((gpeCsz)cIDa).IA0I.D0;
			else
				_move._L.IA0I.D0;

			/// doITop ---------------------
			if( cIDa<gpeCsz_K )
			switch( pRa->pstOP )
			{
				case gpeOPid_addM: _add.c((gpeCsz)cIDa).D1.D0; break;
				case gpeOPid_subM: _sub.c((gpeCsz)cIDa).D1.D0; break;
				case gpeOPid_mulM: _mul.c((gpeCsz)cIDa).D1.D0; break;
				case gpeOPid_divM: _div.c((gpeCsz)cIDa).D1.D0; break;
				case gpeOPid_remM: _rem.c((gpeCsz)cIDa).D1.D0; break;
				default: break;
			}


			if( cIDa<gpeCsz_K )
			{
				_move.c((gpeCsz)cIDa).D0.IA0I;
				continue;
			}

			_move._L.D0.IA0I;
			continue;
		}

		if( !pRa )
			continue;
		/// ARRAY VARI
		if( (gpeOPid)pRa->pstOP == gpeOPid_mov )
		{
			/// Síma egyenlő művelet többel
            // ezt valami REFERENCE átadásal próbáljuk
			pOa->movREF( pOb );
			*pRa = *pOa;
			iPCa 	= iPCrow( *pRa, sOFa, true );
			nA 		= pOa ? pOa->d2D.area() : 1;
			continue;
		}

		/// itt LOOP-olni kell majd

	}

	if( nB > 1 )
	{
		if( pOm )
		{
			pOm->movREF( pOa );
		}
		return pBLKm;
	}

	_move._L.EAl(iPCm).A0;
	_move._L.D0.IA0I;
	_nop;
	return pBLKm;
}

gpBLOCK* gpcSRC::srcINSTadd( char* pS, gpBLOCK *pBLKm, gpBLOCK* pBLK ) {
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
gpBLOCK* gpcSRC::srcINSTmul( char* pS, gpBLOCK *pBLKm, gpBLOCK* pBLK  ) {
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
gpcLZY* gpcSRC::srcINSTmini( gpcLZY* pLZY, gpcMASS* pMASS, gpcWIN* pWIN ) {
	pLZY->lzyRST();
	std::cout << stdMINI " MIN" stdRESET << std::endl;

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
			continue;
		}
		pLZY = pLZY->lzyFRMT( (s=-1), "\r\n0x%x ", obj.iPC );
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
		/*(s=-1);
		pLZY = pLZY->lzyFRMT( s, "\t\t// 0x%x", obj.iPC );*/

	}


	return pLZY;
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
