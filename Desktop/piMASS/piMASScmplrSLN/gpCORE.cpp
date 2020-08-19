#include "gpcSRC.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];

U4 gpCORE::entryOBJ2A0( I8x4 *pM0, char	*pSCPall, gpcLZY* pSCPlnk, gpcWIN* pWIN, U8& bSW, U4 newC ) {
	nSTK = 0;
	gpcREG wVAR;
	gpO	*pOlnk;

	for( U4 iI = pD[0]-4, eI=pA[7]; iI>=eI; iI-=4, nSTK++ ) {
		//p_src = ea( iI, pO+7, pC+7 );
		aSTK[nSTK].null();

		int oID = *(int*)ea( iI, pO+7, pC+7 ); // p_src;
		if( oID < 0 )
		{
			oID *= -1;
			char* pSTR = pSCPall + pM0[oID].iMNi+1;
			return ix_str( pSTR, pM0[oID].iMNn-2, oID );
		}

		gpCNST& lnk = *((gpCNST*)pSCPlnk->Ux( oID, sizeof(gpCNST)));
		//gpeTYP typ = O.typ;
		U4 iO;


		switch( lnk.typ )
		{
			case gpeTYP_sA8: {
					/// ha win variable akkor is csinál ide egy változatot belölle
					/// hogy felül lehessen írni
					if( !nSTK )
					{
						pOlnk = oIDfnd( oID, iO );
						if( pOlnk )
						{
							aSTK[nSTK].iX = pOlnk->iX;
							aSTK[nSTK].pC =
								(aSTK[nSTK].cID = pOlnk->cID) < gpeCsz_K
								? NULL
								: gpmLZYvali( gpC, &cLST ) + pOlnk->cID-gpeCsz_K;
							continue;
						}

						gpeALF alf = lnk.obj.alf;
						if( pWIN->WINvar( wVAR, alf ) )
						{
							// valamit kapott
							bSW |= gpeMASSloopMSK;
							return ix_reg( wVAR, oID );
						}

						pOlnk = pOBJlnk( iO, oID, newC, true ); //(gpO*)oLST.Ux( iO, sizeof(gpO) );

						aSTK[nSTK].iX = pOlnk->iX;
						aSTK[nSTK].pC =
							(aSTK[nSTK].cID = pOlnk->cID) < gpeCsz_K
							? NULL
							: gpmLZYvali( gpC, &cLST ) + pOlnk->cID-gpeCsz_K;
						continue;
					}
					aSTK[nSTK].iX = aSTK[nSTK-1].iX;
					aSTK[nSTK].x = aSTK[nSTK-1].pC->cIDfnd( cLST, oID, aSTK[nSTK].cID );
					if( aSTK[nSTK].x > -1 )
					{
						aSTK[nSTK].iX += aSTK[nSTK].x;
						continue;
					}
					aSTK[nSTK].oID = oID;
					aSTK[nSTK].cID = newC;

				} break;
			case gpeTYP_sA8N:
				//if( pBF ) O.obj.an2str( pBF );
				break;
			case gpeTYP_U1:
				//if( pBF ) sprintf( pBF, "0x%0.2x", O.obj.uy );
				switch( newC )
				{
					case gpeCsz_L:
						pOlnk = oIDfnd( oID, iO );
						if( pOlnk ? pOlnk->iX : false )
							return pOlnk->iX;

						pOlnk = (gpO*)oLST.Ux( iO, sizeof(gpO) );
						pOlnk->oID = oID;
						pOlnk->cID = newC;
						pOlnk->iD = 0;
						pOlnk->iX = mLST.nLD();
						pOlnk->szOF = sOF(newC);
						mLST.Ux( pOlnk->iX, pOlnk->szOF, true, sizeof(U1) );

						int* p_dst = (int*)mLST.Ux( pOlnk->iX, sizeof(int), false, 1 );
						*p_dst = lnk.obj.uy;

						return pOlnk->iX;

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
U4 gpCORE::coreLNK( I4x4* pPC, U4 nPC, I8x4 *pM0, char *pSCPall, gpcLZY* pSCPlnk )
{
	gpcINST inst;
	I4	aoID[0x10];
	U4 nO = 0, iO;
	gpO* pOlnk;
	I4 newC = gpeCsz_L;
	for( U4 i = 0, d0 = 0; i<nPC; i++ )
	{
		inst = pPC[i];
		if( !inst.oID ) // nop
			continue;
		if( inst.oID == gpeOPid_dot )
		{
			if( !d0 )
				continue;
			for( U4 nSTK = 0; nSTK < d0; nSTK++ )
			{
				aSTK[nSTK].null();
				if( aoID[nSTK]<0)
					aoID[nSTK] *= -1;

				gpCNST& lnk = *((gpCNST*)pSCPlnk->Ux( aoID[nSTK], sizeof(gpCNST)));
				//gpeTYP typ = lnk.typ;
				//gpeALF alf = lnk.obj.alf;
				switch( lnk.typ )
				{
					case gpeTYP_sA8: {
							if( !nSTK ) {
								pOlnk = oIDfnd( aoID[nSTK], iO );
								if( pOlnk ) {
									aSTK[nSTK].iX = pOlnk->iX;
									aSTK[nSTK].pC =
										(aSTK[nSTK].cID = pOlnk->cID) < gpeCsz_K
										? NULL
										: gpmLZYvali( gpC, &cLST ) + pOlnk->cID-gpeCsz_K;
									continue;
								}

								pOlnk = pOBJlnk( iO, aoID[nSTK], newC, false );
								aSTK[nSTK].iX = pOlnk->iX;
								aSTK[nSTK].pC =
								(aSTK[nSTK].cID = pOlnk->cID) < gpeCsz_K
										? NULL
										: gpmLZYvali( gpC, &cLST ) + pOlnk->cID-gpeCsz_K;
								continue;
							}

							aSTK[nSTK].oID = aoID[nSTK];
							aSTK[nSTK].cID = newC;
							aSTK[nSTK-1].pC = pCLASSlnk( nSTK );
						} continue;
				}
			}

			d0 = 0;
			nO++;
			continue;
		}

		if( inst.mD != gpeEA_sIAnI )
			continue;
		if( inst.iD != 7 )
			continue;
		if( inst.mS != gpeEA_num )
			continue;

		aoID[d0] = inst.aOB[0];
		d0++;
	}
	for( U4 o = 0, nO = oLST.nLD(sizeof(*pO)); o<nO; o++ ) {
		pOlnk = (gpO*)oLST.Ux( o, sizeof(*pOlnk), true );
		pOlnk->szOF = sOF(pOlnk->cID);
	}
	return nO;
}
