#include "gpcwin.h"
//#include "gpcres.h"

extern U1 gpaALFadd[];

bool bITT = true; // false; //

gpcRES* gpcRES::run( gpcRES* pOUT, gpcLAZY* pMN, gpcWIN& win, gpcSRC* pSRC, gpcRES* pMOM, U4 deep, gpcSTK* pSTK )
{
	if( this ? !nISA.x : true )
	{
		if( this )
		{
			// takarítunk;
			gpmDELary( pISA );
			nISA = 0;
		}
		return pOUT;
	}

	if( pOUT ? false : !deep )
	{
		pOUT = new gpcRES( pMOM );
	}
	gpcSTK stk( pSTK, pOUT, pSRC );
	U8 u8; I8 i8; double d8, iASG, nOUT = 0;
	U1 sBUFF[0x1000], *pB = sBUFF+0x20, *pS = pB, nB;
	gpcADR fnd;
	gpcALU A, B;
	gpcREG err;
	bool bROW = false;
	for( U4 i = 0; i < nISA.x; i++ )
	{
		gpcISA &IS = pISA[i];
		gpfFLG &flg = stk.main;
		if( (U1)IS.isa.aISA[0] > ' ' )
		{
			*pB = (U1)IS.isa.aISA[0];
			pB++;
		}

		switch( IS.isa.aISA[0] )
        {
			case gpeISA_trg:{	// -------------------------------------------------------------------------------------
					pB += sprintf( (char*)pB, "[%d:%d]", IS.an.x, IS.an.y );
					stk.aTRG[flg.iT] = IS.an;
					//flg.iT++;
				} break;


			case gpeISA_an: {	// -------------------------------------------------------------------------------------
					pB += sprintf( (char*)pB, "%s", IS.an.strA4N( sBUFF ));
					stk.aAN[flg.iA] = IS.an;
					//flg.iA++;
				} break;
			case gpeISA_anFUN: {	// -------------------------------------------------------------------------------------
					pB += sprintf( (char*)pB, "%s", IS.an.strA4N( sBUFF ));
					stk.aAN[flg.iA] = IS.an;
					//flg.iA++;
				} break;

			/// gpcREG D[]	// --------------------------------------------------------------------------------------
 			case gpeISA_u8: {	// --------------------------------------------------------------------------------------
					pB += sprintf( (char*)pB, "%lld", IS.an.u8 );
					stk.D[flg.iD] = IS.an.u8;
					//flg.iD++;
				} break;
			case gpeISA_i8: {	// --------------------------------------------------------------------------------------
					pB += sprintf( (char*)pB, "%lld", -((I8)IS.an.u8) );
					stk.D[flg.iD] = -((I8)IS.an.u8);
					//flg.iD++;
				} break;
			case gpeISA_d8: {	// --------------------------------------------------------------------------------------
					pB += sprintf( (char*)pB, "%f",  IS.an.d8 );
					stk.D[flg.iD] = IS.an.d8;
					//flg.iD++;
				} break;



			/// gpeISA_var	// --------------------------------------------------------------------------------------
 			case gpeISA_var: {	// --------------------------------------------------------------------------------------
					pB += sprintf( (char*)pB, "%s", IS.an.strVAR( sBUFF ) );

					fnd = stk.aVR[flg.iV] = IS.an.var;
					if( !fnd.an.alf )
						break;

					if( pOUT )
					{
						fnd = pOUT;
						if( fnd.pRM )
						{
							// lokális változó és már használta valaki
							B = fnd.pRM->ALU( fnd.iA );
							break;
						}
					}
					// na még mindig nincsen
					fnd = this;
					if( fnd.pRM )
					{
						// forrásként használható
						B = fnd.pRM->ALU( fnd.iA );
						break;
					}
					// na most nézzük meg van e beépített változó rá
					if( win.WINvar( stk.D[flg.iD], fnd.an.alf ) )
					{
						// valamit kapott
						(B = pOUT->ADD( fnd.an.alf, 0,0 ))
						//B
						= stk.D[flg.iD];
						break;
					}

					B = pOUT->ADD( fnd.an.alf, 0,0 );
				} break;

			case gpeISA_FUN: { 	// --------------------------------------------------------------------------------------
					pB += sprintf( (char*)pB, "%s", IS.an.strVAR( sBUFF ) ) ;
					stk.aVR[flg.iV] = IS.an.var;

					//flg.iV++;
				} break;


			case gpeISA_tag: {
					 pB += sprintf( (char*)pB, "%s", IS.an.strVAR( sBUFF ) );
					stk.aTG[flg.iG] = IS.an.var;
					//flg.iG++;
				} break;


			case gpeISA_str: {
					pB += sprintf( (char*)pB, "%s", (stk.apSTR[flg.iS] = IS.an.aSTR[0])) ;
					//flg.iS++;
				} break;
			case gpeISA_row:
				bROW = true;
				break;
			case gpeISA_nop:
			default:
				break;
		}
		// CALL
		if( IS.pRES )
			pOUT = IS.pRES->run( pOUT, pMN, win, pSRC, this, deep+1, &stk );


		switch( IS.isa.aISA[1] )
		{
			case gpeISA_u8:
			case gpeISA_i8:
			case gpeISA_d8:
					{
						B = stk.iD(err);
					} break;

			case gpeISA_an:{} break;
			case gpeISA_anFUN:{} break;
			case gpeISA_var:{

				} break;
			case gpeISA_FUN:{} break;
			case gpeISA_dim:{} break;
			case gpeISA_not:{} break;
			case gpeISA_str:{} break;
			case gpeISA_tag:{} break;
			case gpeISA_dollar:{} break;
			case gpeISA_rem:{} break;
			case gpeISA_and:{} break;
			case gpeISA_brkB:{} break;
			case gpeISA_brkE:{} break;
			case gpeISA_mul:{} break;
			case gpeISA_plus:{} break;
			case gpeISA_sub:{} break;
			case gpeISA_dot:{} break;
			case gpeISA_div:{} break;
			case gpeISA_row:
				bROW = true;
				break;
			case gpeISA_litl:{} break;
			case gpeISA_assign:{

					A = B;
					B = 0;
					break;


					fnd = stk.iVR();
					if( !fnd.an.alf )
					{
						if( flg.iA )
						{
							// ez valami an

							break;
						}
						if( flg.iG )
						{
							// talán TAG?
							break;
						}
						// egyik sem;
						break;
					}


					bROW = false;
					fnd = stk.iVR();
					if( pOUT )
					{
						fnd = pOUT;
						//pOUT->alu =
						//pOUT->alu = this;
						if( fnd.pRM )
						{
							// lokális változó és már használta valaki
							B = fnd.pRM->ALU( fnd.iA );
							break;
						}
					}


					//B = stk.aVR[flg.iV-1];
					fnd = this;
					if( !fnd.pRM )	// na még mindig nincsen
					{
						/// nem talált csinálni kell
						B = pOUT->ADD( fnd.an.alf, 0,0 );
						break;
					}

					// talált egy forrást
					/// be kell másolni a pOUT-ba;
					B = fnd.pRM->ALU( fnd.iA );
				} break;
			case gpeISA_gret:{} break;
			case gpeISA_exp:{} break;
			case gpeISA_root:{} break;
			case gpeISA_brkDB:{} break;
			case gpeISA_brkDE:{} break;
			case gpeISA_equ:{} break;
			case gpeISA_trg:{} break;
			case gpeISA_blkB:{} break;
			case gpeISA_or:{} break;
			case gpeISA_blkE:{} break;

			case gpeISA_nop:
			default:
				break;
		}

		if( (U1)IS.isa.aISA[1] > ' ' )
		{
			//stk.stpFLG();
			*pB = (U1)IS.isa.aISA[1];
			pB++;
		}

		if( !bITT )
			continue;

		if( !bROW )
			continue;

		bROW = false;

		if( pS < pB )
		{
			*pB = 0;
			cout << (char*)pS << endl; // ; //
			pB = pS;
		}
	}
	*pB = 0;

	if( bITT )
	if( pS < pB )
	{
		cout << (char*)pS << endl; // ; //
		pB = pS;
	}

	if( !pSRC )
		return pOUT;

	pSRC->pMINI = pOUT->res2mini(
									pSRC->pMINI, sBUFF,

									( nASG && (pSRC->bSW&gpeMASSdebugMSK) )
									? this : NULL,

									deep
								);

	if( deep )
		return pOUT;

	if( pSRC->pMINI ? !pSRC->pMINI->n_load : false  )
		gpmDEL(pSRC->pMINI);
	return pOUT;
}

U1* gpcMASS::justDOit( gpcWIN& win ) // U1* sKEYbuff, I4x4& mouseXY, U4* pKT, I4x4& SRCxycr, I4x4& SRCin )
{
	U1* pKEYbuff = win.gpsKEYbuff;
	gpcSRC	tmp, *pSRC;
	U4 xFND;
	if( U4 *pM = mapCR.pMAP )
	if( U4 *pC = mapCR.pCOL )
	if( U4 *pR = mapCR.pROW )
	if( pM < pC )
	for( U4 i = 0, ie = pC-pM; i < ie; i++ )
	{
		if( !pM[i] )
			continue;

		xFND = pM[i];
		pSRC = SRCfnd( xFND );
		if( !pSRC )
			continue;

		gpmDEL( pSRC->apOUT[0] );
		if( pSRC->qBLD() )
		{
			gpmDEL( pSRC->pEXE );
			pSRC->pEXE = pSRC->pEXE->compiEASY( pSRC->pSRCstart( true ), NULL, NULL, NULL );
			pSRC->rdyBLD();
			pSRC->apOUT[0] = pSRC->apOUT[1];
			pSRC->apOUT[1] = pSRC->apOUT[2];
			pSRC->apOUT[2] = pSRC->apOUT[3];
			pSRC->apOUT[3] = NULL;
		}
		else if( pSRC->bSW&gpeMASSloopMSK )
		{
			pSRC->apOUT[0] = pSRC->apOUT[1];
			pSRC->apOUT[1] = pSRC->apOUT[2];
			pSRC->apOUT[2] = pSRC->apOUT[3];
			pSRC->apOUT[3] = NULL;
		}

		if( !pSRC->apOUT[3] )
		{
			if( !pSRC->pEXE )
			{
				pSRC->pMINI->lzy_reset();
				continue;
			}

			pSRC->pMINI->lzy_reset();
			pSRC->apOUT[3] = pSRC->pEXE->run( pSRC->apOUT[3], NULL, win, //this,
																			pSRC, NULL );
		}
	}

	return pKEYbuff;
}
