#include "gpcSRC.h"
#include "gpcSRCinst.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];

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

	bool bD0 = true;

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

		if( nB < 2 ) {
			/// SINGLE VARI
			if( bD0 ) {
				/// SRC
				// B ------------
				_move._L.EAl( iPCb ).A0;
				_move.c((gpeCsz)cIDb).IA0I.D0;
				bD0 = false;
			}

			if( cIDa != pRa->cID ) {
				/// convert D0 -> pR0[i].cID
				cIDa = pRa->cID;
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
			*pRm = *pOm;
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
gpBLOCK* gpcSRC::srcINSTmul2( char* pS, gpBLOCK *pBLKm, gpBLOCK* pBLK ) {
	if( !pBLKm )
		pBLKm = lzyBLOCK.pSTPdwn( pBLK->bIDm );

	I4	nR = pBLK->nROW(),
		iPCm = 0, 	iPCa = 0, 	iPCb = 0,
		sOFm = 0,	sOFa = 0,	sOFb = 0,
		cIDm = -1,	cIDa = -1,	cIDb = -1, cID0 = gpeCsz_L,
		nM = 0,		nA = 0, 	nB = 1;


	gpROW	*pRa = pBLK->pROW(),

			// *pRa = NULL,
			*pRb = NULL,
			*pRm = NULL;

	gpOBJ	*pOa = NULL,
			*pOb = NULL,
			*pOm = NULL;

	U1		*pUm = NULL, nUm,
			*pUa = NULL, nUa,
			*pUb = NULL, nUb, nU0;

	bool	bD0 = true,
			bS0, bF0,
			bSa, bFa;

	for( gpROW* pRe = pRa+nR; pRa < pRe; pRb = pRa, pRa++ )
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
		cIDa 	= pRa ? pRa->cID : gpeCsz_L;
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

		//if( nB < 2 )
		{
			/// SINGLE VARI
			if( bD0 ) {
			/// DST B 	D0 ----------------
				// B ------------
				cID0 = cIDb;
				_move._L.EAl( iPCb ).A0;
				_move.c((gpeCsz)cID0).IA0I.D0;
				bD0 = false;
				bS0 = cID0&((I4)gpeCsz_B);
				bF0 = bS0&(cID0&((I4)gpeCsz_4));
				nU0 = gpaCsz[cID0];
			}

			/// SRC A 	D2 ---------------------
			_move._L.EAl( iPCa ).A0;
			if( cIDa < gpeCsz_K )
			{
				// A   0    0
				// b x b -> w
				// w x w -> l
				// l x l -> q
				// q x q -> q
				_move.c((gpeCsz)cIDa).IA0I.D1;

				bSa = cIDa & ((I4)gpeCsz_B);
				bFa = bSa & (cIDa&((I4)gpeCsz_4));
				nUa = gpaCsz[cIDa];
				bS0 |= bSa;


				if( bSa )
				{
					if( nUa == nU0) {

					}
					else if( nUa < nU0) {
						switch( nUa )
						{
							case 1: _extB._Q.D1; break;
							case 2: _ext._Q.D1; break;
							case 4: _extL._Q.D1; break;
							default:
								break;
						}
						nUa = nU0;
					}
					else {

					}
					bS0 = true;
				} else {
					if( nUa < nU0) {
						nUa = nU0;
					}
				}
				if( bFa )
				{
					/// át kell vini a számolást FPU regiszterekbe
				}
			}
			else
				_move._L.IA0I.D1;




			/// doITop ---------------------
			if( cID0<gpeCsz_K )
			{
				switch( pRb->pstOP )
				{
					case gpeOPid_mul:
						_mul.c((gpeCsz)cID0).D1.D0;
						break;
					case gpeOPid_div:
						_div.c((gpeCsz)cID0).D1.D0;
						break;
					case gpeOPid_rem:
						_rem.c((gpeCsz)cID0).D1.D0;
						//_swap.c((gpeCsz)cID0).d0;
						break;
					default:
						pMEM->inst( (gpeOPid)pRb->pstOP ).c((gpeCsz)cID0).D1.D0;
					break;
				}
			}
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

//	if( nB > 1 ) {
//		if( pOm )
//		{
//			pOm->movREF( pOa );
//			*pRm = *pOm;
//		}
//		return pBLKm;
//	}

	_move._L.EAl(iPCm).A0;
	_move._L.D0.IA0I;
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
