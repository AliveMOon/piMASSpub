#include "gpcSRC.h"
#include "gpcSRCinst.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];
gpBLOCK* gpcSRC::srcBLKent( char* pS, I4 mnID, gpBLOCK* pBLK, gpeOPid opID, gpcLZY* pDBG ) {
	/// + a0.b
	/// * a0.b

	gpROW* pRl = pBLK->pLSTrow();
	if( !pRl )
		return pBLK;

	gpOBJ* pO = srcOBJfnd(pRl->mNdID);
	switch( pO ? pO->cAN : gpeCsz_OFF )
	{
		case gpeCsz_a:
		case gpeCsz_c:
			/// a0.b
			/// b.a0
			return srcBLKup( pS, pBLK, opID );
		case gpeCsz_b:
			if( pO->bSTR() )
			{
				// itt lehetne egy név alapján keresni

			}
		default: break;
	}
	return pBLK;
}

gpBLOCK* gpcSRC::srcINSTent( char* pS, gpBLOCK *pBLKm, gpBLOCK* pBLK ) {
	if( !pBLKm )
		pBLKm = lzyBLOCK.pSTPdwn( pBLK->bIDm );

	I4	nR = pBLK->nROW(),
		iPCm = pBLK->iPC, 	iPCa = 0, 	iPCb = 0,	iPCin = 0,
		sOFm = 0,			sOFa = 0,	sOFb = 0,	sOFin = 0,
		cIDm = -1,			cIDa = -1,	cIDb = -1, cID0 = gpeCsz_L,
		nM = 0,				nA = 0, 	nB = 1;


	gpROW	*pRa = pBLK->pROW(),

			// *pRa = NULL,
			*pRb = NULL,
			*pRm = NULL;

	gpOBJ	*pOa = NULL,
			*pOb = NULL,
			*pOm = NULL;

	U1		*pUm = srcMEMiPC( iPCm, sOFm ), nUm,
			*pUa = NULL, nUa,
			*pUb = NULL, nUb, nU0, *pUin;
	gpcSRC	*pSRCa = NULL,
			*pSRCb = NULL;

	bool	bD0 = true,
			bS0, bF0,
			bSa, bFa;

	for( gpROW* pRe = pRa+nR; pRa < pRe; pRb = pRa, pRa++ ) {
		iPCb = iPCa; sOFb = sOFa;
		pOb = pOa;
		pSRCb = pSRCa;

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
		pUa		= srcMEMiPC( iPCa, sOFa );
		if( pOa->bAN() )
		{
			U4 xfnd = pMEM->pMASS->getXFNDan( pOa->AN );
			pSRCa = pMEM->pMASS->srcFND( xfnd );
			if( pSRCa )
			{
				if( !pMEM->pLZYsrcXFND )
				{
					if( pMEM->pLZYsrcXFNDall )
					{
						pMEM->pLZYsrcXFNDall->lzyRST();
					} else {
						pMEM->pLZYsrcXFNDall = new gpcLZY;
					}
					pMEM->nXFND = 0;
					pMEM->pLZYsrcXFND = pMEM->pLZYsrcXFNDall;
				}
                U4* pXFND = (U4*)(pMEM->pLZYsrcXFND->Ux( pMEM->nXFND, sizeof(*pXFND) ));
                (*pXFND) = xfnd;

			} else {
				// nincsen PÁPÁ
				return pBLKm;
			}
		} else {
			pSRCa = NULL;
			if(pOa->bALF())
			if( pSRCb ){
				if( !pSRCb->pMEM )
				{
					pSRCb->msBLD = pMEM->pWIN->mSEC.x + pSRCb->msBLTdly;
				}
				if( !pSRCb->srcBLD( pMEM->pWIN, pMEM->pLZYsrcXFND ) )
					return pBLKm;

				if( gpOBJ* pOin = pSRCb->pMEM->pOBJ(pOa->AN.alf) )
				{
				#ifdef stdOFF
					std::cout << stdALU "pSRCb" << std::endl;
                #endif // stdOFF
					pUin = pSRCb->srcMEMiPC( iPCin = pOin->iPC, sOFin );
					if( pOin->bSTR() )
					{
						std::cout << stdMINI << (char*)pUin << std::endl;
					}
					return pBLKm;
				}
			}
		}
		if( !pRb )
		{
			pRm 	= pBLKm->pROW(pBLK->bIDmR);
			//iPCm 	= pRm ? iPCrow( *pRm, sOFm, false ) : 0;
			pOm 	= pRm ? srcOBJfnd( pRm->mNdID ) : NULL;
			cIDm	= pRm ? pRm->cID : gpeCsz_L;
			nM		= pOm ? pOm->d2D.area() : 0;
			continue;
		}

	}

	return pBLKm;
}
gpBLOCK* gpcSRC::srcINSTmov( char* pS, gpBLOCK *pBLKm, gpBLOCK* pBLK ) {
	if( !pBLKm )
		pBLKm = lzyBLOCK.pSTPdwn( pBLK->bIDm );

	I4	nR = pBLK->nROW(),
		iPCm = 0, 	iPCa = 0, 	iPCb = 0,
		sOF0 = 0,
		sOFm = 0,	sOFa = 0,	sOFb = 0,
		cID0 = -1,
		cIDm = -1,	cIDa = -1,	cIDb = -1,
		nM = 0,		nA = 0, 	nB = 1;
	I8x2	anA = 0, anB;

	gpROW	*pR0 = pBLK->pROW(),

			// *pRa = NULL,
			*pRb = NULL,
			*pRm = NULL;

	gpOBJ	*pOa = NULL,
			*pOb = NULL,
			*pOm = NULL;

	U1		*pUm = NULL,
			*pUa = NULL,
			*pUb = NULL,
			nU0, nUa, nUb, nUm;

	bool	bD0 = true,
			bS0, bSa, bSb, bSm;

	//for( I4 i = nR-1; i >= 0; i-- )
	for( gpROW* pRa = pR0+nR-1; pRa >= pR0; pRb = pRa, pRa-- )
	{
		iPCb = iPCa; sOFb = sOFa;
		pOb = pOa;
		anB = anA;

		if( nA < 2 )
			nB = 1;
		else
			nB = pOb ? pOb->d2D.area() : 1;
		cIDb = cIDa;
		bSb = bSa;
		pUb = pUa;
		nUb = nUa;

		iPCa 	= iPCrow( *pRa, sOFa, true );
		cIDa	= pRa ? pRa->cID : gpeCsz_L;
		bSa 	= cIDa&((I4)gpeCsz_B);
		if( pOa = srcOBJfnd( pRa->mNdID ) ) {
			anA = pOa->AN;
		}
		nA 		= pOa ? pOa->d2D.area() : 1;
		nUa		= sOFa/nA;

		pUa = srcMEMiPC( iPCa, sOFa );

		if( cID0<0 )
		{
			pRm 	= pBLKm->pROW(pBLK->bIDmR);
			iPCm 	= pRm ? iPCrow( *pRm, sOFm, false ) : 0;
			pOm 	= pRm ? srcOBJfnd( pRm->mNdID ) : NULL;
			cIDm	= pRm ? pRm->cID : gpeCsz_L;
			nM		= pOm ? pOm->d2D.area() : 0;
			bSm		= cIDm&((I4)gpeCsz_B),
			pUm		= srcMEMiPC( iPCm, sOFm );

			sOF0	= sOF0;
			nU0		= nUa;
			cID0	= cIDa;
			bS0 	= bSa;
			continue;
		}

		if( nB < 2 ) {
			/// SINGLE VARI
			if( bD0 ) {
				/// SRC
				// B ------------
				if( nUb < 8 )
					_xor._q.D0.D0;
				_move._l.EAl( iPCb ).A0;
				switch( anB.alf )
				{
					case gpeALF_FPS:
						_jsr.EAl( anB.alf );
						break;
					default:
						break;
				}
				_move.c((gpeCsz)cIDb).IA0I.D0;
				bD0 = false;
			}

			if( cIDa != cID0 ) {
				/// convert D0 -> pR0[i].cID
				if( bS0 )
				{
					if( nUa > nU0 )
					{
						switch( nU0 )
						{
							case 1: _extB._Q.D0; break;
							case 2: _ext._Q.D0; break;
							case 4: _extL._Q.D0; break;
							default:
								break;
						}
					}

					if( !bSa )
					{
						switch( cIDa )
						{
							case gpeCsz_b: cIDa = gpeCsz_B; break;
							case gpeCsz_w: cIDa = gpeCsz_W; break;
							case gpeCsz_l: cIDa = gpeCsz_L; break;
							case gpeCsz_q: cIDa = gpeCsz_Q; break;
							default:
								break;
						}
						bSa = bS0;

						pOa->REcID( cIDa );
						*pRa = *pOa;
					}
				}
				cID0 = cIDa;
			}

			/// TRG iPCa ---------------------
			_move._l.EAl( iPCa ).A0;
			if( (gpeOPid)pRa->pstOP == gpeOPid_mov ) {
				/// Síma egyenlő
				if( cID0<gpeCsz_K ) {
					_move.c((gpeCsz)cID0).D0.IA0I;
				} else
					_move._L.D0.IA0I;
				continue;
			}

			/// müvelet+egyenlő
			/// SRC ig mst B ---------------------
			_move._q.D0.D1;	// elöző kör lesz a forás



			if( cID0<gpeCsz_K )
				_move.c((gpeCsz)cID0).IA0I.D0;
			else
				_move._L.IA0I.D0;

			/// doITop ---------------------
			if( cID0<gpeCsz_K )
			switch( pRa->pstOP )
			{
				case gpeOPid_add:
					_add.c((gpeCsz)cID0).D1.D0; break;
				case gpeOPid_sub:
					_sub.c((gpeCsz)cID0).D1.D0; break;
				case gpeOPid_addM: _add.c((gpeCsz)cID0).D1.D0; break;
				case gpeOPid_subM: _sub.c((gpeCsz)cID0).D1.D0; break;
				case gpeOPid_mulM: _mul.c((gpeCsz)cID0).D1.D0; break;
				case gpeOPid_divM: _div.c((gpeCsz)cID0).D1.D0; break;
				case gpeOPid_remM: _rem.c((gpeCsz)cID0).D1.D0; break;
				default: break;
			}


			if( cID0<gpeCsz_K )
			{
				_move.c((gpeCsz)cID0).D0.IA0I;
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
			/// Síma egyenlő művelet
            // ezt valami REFERENCE átadásal próbáljuk
			pOa->movREF( pOb );
			*pRa = *pOa;
			iPCa 	= iPCrow( *pRa, sOFa, true );
			nA 		= pOa ? pOa->d2D.area() : 1;
			continue;
		}

		/// itt LOOP-olni kell majd

	}

	if( nB > 1 ) {
		_nop;_nop;
		if( pOm )
		{
			pOm->movREF( pOa );
			*pRm = *pOm;
		}
		return pBLKm;
	}

	if( iPCa == iPCm )
	{

		_nop;
		return pBLKm;
	}
	_move._l.EAl(iPCm).A0;
	_move._L.D0.IA0I;
	_nop;
	return pBLKm;
}
gpBLOCK* gpcSRC::srcINSTadd( char* pS, gpBLOCK *pBLKm, gpBLOCK* pBLK ) {
	if( !pBLKm )
		pBLKm = lzyBLOCK.pSTPdwn( pBLK->bIDm );

	I4	nR = pBLK->nROW(),
		iPCm = pBLK->iPC, 	iPCa = 0, 	iPCb = 0,
		sOFm = 0,			sOFa = 0,	sOFb = 0,
		cIDm = -1,			cIDa = -1,	cIDb = -1, cID0 = gpeCsz_L,
		nM = 0,				nA = 0, 	nB = 1;


	gpROW	*pRa = pBLK->pROW(),

			// *pRa = NULL,
			*pRb = NULL,
			*pRm = NULL;

	gpOBJ	*pOa = NULL,
			*pOb = NULL,
			*pOm = NULL;

	U1		*pUm = srcMEMiPC( iPCm, sOFm ), nUm,
			*pUa = NULL, nUa,
			*pUb = NULL, nUb, nU0;

	bool	bD0 = true,
			bS0, bF0,
			bSa, bFa;

	for( gpROW* pRe = pRa+nR; pRa < pRe; pRb = pRa, pRa++ ) {
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
			//iPCm 	= pRm ? iPCrow( *pRm, sOFm, false ) : 0;
			pOm 	= pRm ? srcOBJfnd( pRm->mNdID ) : NULL;
			cIDm	= pRm ? pRm->cID : gpeCsz_L;
			nM		= pOm ? pOm->d2D.area() : 0;


			continue;
		}

		//if( nB < 2 )
		{
			/// SINGLE VARI
			if( bD0 ) {
			/// DST B 	D0 ----------------
				// B ------------
				if( gpaCsz[cIDb] < 4 )
					_xor._q.D0.D0;
				cID0 = cIDb;
				_move._l.EAl( iPCb ).A0;
				_move.c((gpeCsz)cID0).IA0I.D0;
				bD0 = false;
				bS0 = cID0&((I4)gpeCsz_B);
				bF0 = bS0&(cID0&((I4)gpeCsz_4));
				nU0 = gpaCsz[cID0];
			}

			/// SRC A 	D1 ---------------------
			_move._l.EAl( iPCa ).A0;
			if( cIDa < gpeCsz_K )
			{
				if( gpaCsz[cIDa] < 4 )
					_xor._q.D1.D1;
				// A   0    0
				// b + b -> w
				// w + w -> l
				// l + l -> q
				// q + q -> q

				_move.c((gpeCsz)cIDa).IA0I.D1;

				bSa = cIDa&((I4)gpeCsz_B);
				bFa = bSa&(cIDa&((I4)gpeCsz_4));
				nUa = gpaCsz[cIDa];
				bS0 |= bSa;

				if( pRb->pstOP == gpeOPid_sub )
				{
					bSa = true;
				}

				if( bSa )
				{
					if( nUa != nU0 ) {
						if( nUa < nU0) {
							switch( nUa )
							{
								case 1: _extB._Q.D1; break;
								case 2: _ext._Q.D1; break;
								case 4: _extL._Q.D1; break;
								default:
									break;
							}
							nUa = nU0;
							switch( nUa )
							{
								case 0:
								case 1: cID0 = gpeCsz_B; break;
								case 2: cID0 = gpeCsz_W; break;
								case 4: cID0 = gpeCsz_L; break;
								case 8: cID0 = gpeCsz_Q; break;
								default:
									break;
							}
						}
						else if( nUa > nU0 ) {
							nU0 = nUa;
							switch( nUa )
							{
								case 0:
								case 1: cID0 = gpeCsz_B; break;
								case 2: cID0 = gpeCsz_W; break;
								case 4: cID0 = gpeCsz_L; break;
								case 8: cID0 = gpeCsz_Q; break;
								default:
									break;
							}
						}
					}
					else if( pRb->pstOP == gpeOPid_sub ){
						switch( nUa )
						{
							case 0:
							case 1: cID0 = gpeCsz_B; break;
							case 2: cID0 = gpeCsz_W; break;
							case 4: cID0 = gpeCsz_L; break;
							case 8: cID0 = gpeCsz_Q; break;
							default:
								break;
						}
					}

					bS0 = true;
				}
				else if( nUa != nU0 )
				{
					if( nUa < nU0) {
						nUa = nU0;
					}
					else if( nUa > nU0 ) {
						nU0 = nUa;
						switch( nUa )
						{
							case 0:
							case 1: cID0 = gpeCsz_b; break;
							case 2: cID0 = gpeCsz_w; break;
							case 4: cID0 = gpeCsz_l; break;
							case 8: cID0 = gpeCsz_q; break;
							default:
								break;
						}
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
					case gpeOPid_add:
									_add.c((gpeCsz)cID0).D1.D0; break;
					case gpeOPid_sub:
									_sub.c((gpeCsz)cID0).D1.D0; break;

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
	if( cIDm != cID0 )
	{
		if( pOm ) {
			pOm->REcID(cID0);
			*pRm = *pOm;
		}
		nUm = gpaCsz[cID0];
		if( bS0 )
		{
			switch( nU0 )
			{
				case 1: _extB._Q.D0; break;
				case 2: _ext._Q.D0; break;
				case 4: _extL._Q.D0; break;
				default:
					break;
			}

		}

	}

	_move._l.EAl(iPCm).A0;
	_move._L.D0.IA0I;
	_nop;
	return pBLKm;
}
gpBLOCK* gpcSRC::srcINSTmul( char* pS, gpBLOCK *pBLKm, gpBLOCK* pBLK ) {
	if( !pBLKm )
		pBLKm = lzyBLOCK.pSTPdwn( pBLK->bIDm );

	I4	nR = pBLK->nROW(),
		iPCm = pBLK->iPC,	iPCa = 0, 	iPCb = 0,
		sOFm = 0,			sOFa = 0,	sOFb = 0,
		cIDm = -1,			cIDa = -1,	cIDb = -1, cID0 = gpeCsz_L,
		nM = 0,				nA = 0, 	nB = 1;


	gpROW	*pRa = pBLK->pROW(),

			// *pRa = NULL,
			*pRb = NULL,
			*pRm = NULL;

	gpOBJ	*pOa = NULL,
			*pOb = NULL,
			*pOm = NULL;

	U1		*pUm = srcMEMiPC( iPCm, sOFm ), nUm,
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
			//iPCm 	= pRm ? iPCrow( *pRm, sOFm, false ) : 0;
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
				if( gpaCsz[cID0] < 4 )
					_xor._q.D0.D0;
				_move._l.EAl( iPCb ).A0;
				_move.c((gpeCsz)cID0).IA0I.D0;
				bD0 = false;
				bS0 = cID0&((I4)gpeCsz_B);
				bF0 = bS0&(cID0&((I4)gpeCsz_4));
				nU0 = gpaCsz[cID0];
			}

			/// SRC A 	D1 ---------------------
			_move._l.EAl( iPCa ).A0;
			if( cIDa < gpeCsz_K )
			{
				if( gpaCsz[cIDa] < 4 )
					_xor._q.D1.D1;
				// A   0    0
				// b + b -> w
				// w + w -> l
				// l + l -> q
				// q + q -> q
				_move.c((gpeCsz)cIDa).IA0I.D1;

				bSa = cIDa&((I4)gpeCsz_B);
				bFa = bSa&(cIDa&((I4)gpeCsz_4));
				nUa = gpaCsz[cIDa];
				bS0 |= bSa;
				if( bSa )
				{
					if( nUa != nU0 ) {
						if( nUa < nU0) {
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
						else if( nUa > nU0 ) {
							nU0 = nUa;
							switch( nUa )
							{
								case 0:
								case 1: cID0 = gpeCsz_B; break;
								case 2: cID0 = gpeCsz_W; break;
								case 4: cID0 = gpeCsz_L; break;
								case 8: cID0 = gpeCsz_Q; break;
								default:
									break;
							}
						}
					}

					bS0 = true;
				}
				else if( nUa != nU0 )
				{
					if( nUa < nU0) {
						nUa = nU0;
					}
					else if( nUa > nU0 ) {
						nU0 = nUa;
						switch( nUa )
						{
							case 0:
							case 1: cID0 = gpeCsz_b; break;
							case 2: cID0 = gpeCsz_w; break;
							case 4: cID0 = gpeCsz_l; break;
							case 8: cID0 = gpeCsz_q; break;
							default:
								break;
						}
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

	_move._l.EAl(iPCm).A0;
	_move._L.D0.IA0I;
	_nop;
	return pBLKm;
}

gpINST* gpMEM::instALU() {
	//std::cout << stdALU "+";
	gpINST& ins = pINST[pc];
	pc++;

	U4 sOF = gpaCsz[ins.cID];
	U1* p_src, *p_dst, *pALL = lzyMEM.p_alloc, *pPC = ((U1*)&ins);
	switch( ins.mS ) {
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

	switch( ins.mD ) {
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
								case gpeOPid_jsr: {
										instVAR( pALL + pA[0], ins );
									} break;
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
								case gpeCsz_q:
									gpmMUL( U8, ins.op, mx );
									break;
								case gpeCsz_L:
									gpmMUL( int, ins.op, mx );
									break;
								case gpeCsz_l: gpmMUL( U4, ins.op, mx ); break;
								case gpeCsz_W: gpmMUL( I2, ins.op, mx ); break;
								case gpeCsz_w: gpmMUL( U2, ins.op, mx ); break;
								case gpeCsz_B: gpmMUL( I1, ins.op, mx ); break;
								case gpeCsz_b: gpmMUL( U1, ins.op, mx ); break;
								case gpeCsz_f: gpmFMUL( F, ins.op, mx ); break;
								case gpeCsz_d: gpmFMUL( D, ins.op, mx ); break;
								case gpeCsz_4: gpmFMUL( U1x4, ins.op, mx ); break;
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
								case gpeCsz_L:{
										switch(ins.op)
										{
											case gpeOPid_or:
												*((I4*)p_dst) |= *((I4*)p_src);
												break;
											case gpeOPid_sub:
												*((I4*)p_dst) -= *((I4*)p_src);
												break;
											default:
												*((I4*)p_dst) += *((I4*)p_src);
												break;
										}
									} break;
								case gpeCsz_l:{
										switch(ins.op)
										{
											case gpeOPid_or:
												*((U4*)p_dst) |= *((U4*)p_src);
												break;
											case gpeOPid_sub:
												*((U4*)p_dst) -= *((U4*)p_src);
												break;
											default:
												*((U4*)p_dst) += *((U4*)p_src);
												break;
										}
									} break;
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

