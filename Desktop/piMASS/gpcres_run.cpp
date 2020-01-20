#include "gpcwin.h"
//#include "gpcres.h"

bool bITT =  false; //true; // false; //
extern U1 gpaALFadd[];


gpcRES* gpcRES::RESrun( gpcRES* pOUT, gpcLZY* pMN, gpcWIN& win, gpcSRC* pSRC, gpcRES* pMOM, U4 deep, gpcSTK* pSTK )
{
	if( win.pM ? !this : true )
		return pOUT;

	if( !nISA.x )
	{
		gpmDELary( pISA );
		return pOUT;
	}

	if( pOUT ? false : !deep )
	{
		pOUT = new gpcRES( pMOM );
	}
	gpcSTK stk( pSTK, pOUT, pSRC );
	U4 iASG, nOUT = 0;

	U8 u8;
	I8 i8;
	double d8;

	U1	sBUFF[0x1000],
		*pB = sBUFF+0x20, *pS = pB, nB;

	gpcREG err;
	I1x4 isa = 0; //, back;
	bool bROW = false;

	gpcMASS& mass = *win.piMASS;



	U4	mZ = win.mZ,
		mN = win.mN, mxZN = mZ*mN + 1; //, znI = mxZN;
	U4x2 trg = 0;
	U4 anFND = 0;
	gpcADR A0;
	gpcREG D0;
	gpcALU A, B, FND;
	gpcSRC	*pANS = NULL;
	gpcRES	*pANcall = NULL, *pDOT = NULL;
	U1		*pSTR = NULL;

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
			case gpeISA_trg: {	// -------------------------------------------------------------------------------------
					trg = IS.an;
				} continue;


			case gpeISA_an: {	// -------------------------------------------------------------------------------------
						//pB += sprintf( (char*)pB, "%s", IS.an.strA4N( sBUFF ));
						anFND = 0;
						B = gpeALF_null;
						if( IS.an.x ? IS.an.x > mZ : true )
							break;
						if( IS.an.y < mN )
						if(	anFND = win.pM[IS.an.y*mZ + IS.an.x-1] )
						if( pANS = mass.SRCfnd( anFND ) )
							pSTR = pANS->pB+1;
					} break;
					/*
					stk.aAN[flg.iA] = IS.an;
					znI = mxZN; pANS = NULL;
					if( IS.an.x ? (IS.an.y>=mN) : true )
						break;

					znI = IS.an.y*mZ + IS.an.x-1;


					if( pANS ? pANS->IX != ixFND: true )
					{
						pANS = mass.SRCfnd( ixFND );
					}
					if( !pANS )
						pANcall = NULL;
					else {
						pANcall = pANS->apOUT[3];
						if( !pANcall )
						{
							pANcall = pANS->apOUT[3] = pANS->pEXE->RESrun( pANS->apOUT[3], NULL, win, pANS, NULL );
							if( !pANcall )
							{

								pSRC->bSW |= gpeMASSloopMSK;
							}
						}
					}


					flg.iA++;
				} break;*/
			case gpeISA_anFUN: {	// -------------------------------------------------------------------------------------
					//pB += sprintf( (char*)pB, "%s", IS.an.strA4N( sBUFF ));
					stk.aAN[flg.iA] = IS.an;
					//flg.iA++;
				} break;


			/// gpcREG D[]	// --------------------------------------------------------------------------------------
 			case gpeISA_u8: {	// --------------------------------------------------------------------------------------
					D0 = IS.an.u8;
					B = gpeALF_null;
				} break;
			case gpeISA_i8: {	// --------------------------------------------------------------------------------------
					D0 = -((I8)IS.an.u8);
					B = gpeALF_null;
				} break;
			case gpeISA_d8: {	// --------------------------------------------------------------------------------------
					D0 = IS.an.d8;
					B = gpeALF_null;
				} break;



			/// gpeISA_var	// --------------------------------------------------------------------------------------
 			case gpeISA_var: {	// --------------------------------------------------------------------------------------
					A0 = IS.an.var;
					if( !A0.an.alf )
						break;

					if( pDOT )
					{
						A0 = pDOT;
						pDOT = NULL;
						if( !A0.pRM )
							break;
						if( pANS ? pANS->bSW&gpeMASSloopMSK : false )
							pSRC->bSW |= gpeMASSloopMSK;
						// lokális változó és már használta valaki
						isa = pISA[i-1-IS.i].isa;
						if( isa.aISA[0] == gpeISA_trg )
							isa = pISA[i-2-IS.i].isa;
						B = A0.pRM->ALU( A0.iA );
						break;
					}

					if( pOUT )
					{
						A0 = pOUT;
						if( A0.pRM )
						{
							(IS.isa.aISA[1] == gpeISA_assign ? FND : B) = A0.pRM->ALU( A0.iA );
							break;
						}
					}
					// na még mindig nincsen
					A0 = this;
					if( A0.pRM )
					{
						// forrásként használható
						if( IS.isa.aISA[1] == gpeISA_assign )
						{
							FND = pOUT->ADD( A0.an.alf, 0,0 );
							FND.equ( A0.pRM->ALU( A0.iA ) );
						} else
							B = A0.pRM->ALU( A0.iA );
						break;
					}
					// na most nézzük meg van e beépített változó rá
					if( win.WINvar( D0, A0.an.alf ) )
					{
						// valamit kapott
						pSRC->bSW |= gpeMASSloopMSK;
						pSTR = D0.getSTR();
						break;
					}

					(IS.isa.aISA[1] == gpeISA_assign ? FND : B) = pOUT->ADD( A0.an.alf, 0,0 );
				} break;

			case gpeISA_FUN: { 	// --------------------------------------------------------------------------------------
					//pB += sprintf( (char*)pB, "%s", IS.an.strVAR( sBUFF ) ) ;
					stk.aVR[flg.iV] = IS.an.var;
					//flg.iV++;
				} break;


			case gpeISA_tag: {
					 //pB += sprintf( (char*)pB, "%s", IS.an.strVAR( sBUFF ) );
					stk.aTG[flg.iG] = IS.an.var;
					//flg.iG++;
				} break;


			case gpeISA_str: {
					pSTR = IS.an.aSTR[0];
					//pB += sprintf( (char*)pB, "%s", (stk.apSTR[flg.iS] = IS.an.aSTR[0])) ;
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
			pOUT = IS.pRES->RESrun( pOUT, pMN, win, pSRC, this, deep+1, &stk );


		switch( isa.aISA[1] ) {
			case gpeISA_u8:
			case gpeISA_i8:
			case gpeISA_d8: {
						B = D0;
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
			case gpeISA_rem:{
					if( A.pRM ? D0.bGD() : false )
						A %= D0;
				} break;
			case gpeISA_and:{} break;
			case gpeISA_brkB:{} break;
			case gpeISA_brkE:{} break;
			case gpeISA_mul:{
					if( A.pRM ? D0.bGD() : false )
						A *= D0;
				} break;
			case gpeISA_plus:{
					if( A.alf && B.alf )
					{
						A += B;
						B = gpeALF_null;
						break;
					}

					if( A.pRM ? D0.bGD() : false )
						A += D0;
				} break;
			case gpeISA_sub:{
					if( A.pRM ? D0.bGD() : false )
						A -= D0;
				} break;
			case gpeISA_div:{
					if( A.pRM ? D0.bGD() : false )
						A /= D0;
				} break;




			case gpeISA_dot:{
					pANcall = NULL;
					if( anFND )
						pDOT = ( pANS = mass.SRCfnd( anFND ) ) ? pANS->apOUT[3] : NULL;
					else
						pDOT = NULL;
				} break;


			case gpeISA_row:
				bROW = true;
				break;
			case gpeISA_litl:{} break;



			case gpeISA_assign:{
					if( D0.bGD() )
						A = D0;
					else if( pSTR )
					{
                        A = pSTR;
                        pSTR = NULL;
					}
					if( A.alf && B.alf )
					{
						A.equ( B );
					}
					else if( pANcall )
					{
						pANcall;
					}
					B = gpeALF_null;
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
		isa = IS.isa;
		if( (U1)isa.aISA[1] > ' ' )
		{
			//B.isa = IS.isa;
			//stk.stpFLG();
			*pB = (U1)IS.isa.aISA[1];
			pB++;
		}

		switch( isa.aISA[1] )
		{
			case gpeISA_assign:
				A = FND;
				B = gpeALF_null;
				break;
			case gpeISA_dot:{
					pANcall = NULL;
					if( anFND )
						pDOT = ( pANS = mass.SRCfnd( anFND ) ) ? pANS->apOUT[3] : NULL;
					else
						pDOT = NULL;
				} break;
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
	win.nJDOIT.y = win.nJDOIT.x;

	U1* pKEYbuff = win.gpsKEYbuff;
	gpcSRC	tmp, *pSRC;
	U4 xFND;
	int pic_id = 0;
	I4x4 sprt[2] = {0}, trgWH = 0;
	win.mZ = mapCR.mapZN44.z;
	win.mN = mapCR.mapZN44.w;
	win.mZN = mapCR.mapZN44.a4x2[1].area();
	I8x4 anRio;
	if( U4 *pM = win.pM = mapCR.pMAP )
	if( U4 *pC = win.pC = mapCR.pCOL )
	if( U4 *pR = win.pR = mapCR.pROW )
	if( pM < pC )
	for( U4 i = 0, ie = pC-pM; i < ie; i++ )
	{
		if( pSRC && pic_id )
			pSRC->picID = pic_id;

		if( !pM[i] )
			continue;

		xFND = pM[i];
		pSRC = SRCfnd( xFND );
		if( !pSRC )
			continue;

		pic_id = pSRC->picID = 0;
		gpmDEL( pSRC->apOUT[0] );
		if( pSRC->qBLD() )
		{
			pSRC->msBLD = win.mSEC.x + pSRC->msBLTdly;
			pSRC->rdyBLD();
		}

		if( pSRC->msBLD ? pSRC->msBLD <= win.mSEC.x : false )
		{
			gpmDEL( pSRC->pEXE );
			pSRC->pEXE = pSRC->pEXE->compiEASY( pSRC->pSRCstart( true ), NULL, NULL, NULL );

			if( !pSRC->msBLTdly )
				pSRC->msBLTdly = gpdSYNmSEC;
			else
				win.pSYNwin = win.pSYNwin->syncADD( gpcSYNC( gpeNET4_0SRC, (i%win.mZ)+((i/win.mZ)<<16) , win.mSEC.y, pSRC->iGT, 0 ), win.msSYN );
			pSRC->msBLD = 0;

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
				pSRC->pMINI->lzyRST();
				continue;
			}
			win.nJDOIT.x++;
			pSRC->pMINI->lzyRST();
			pSRC->apOUT[3] = pSRC->pEXE->RESrun( pSRC->apOUT[3], NULL, win, pSRC, NULL );
			if( !pSRC->apOUT[3] )
				continue;
		}

		/// READY
		gpcRES& res = *pSRC->apOUT[3];
		gpcPIC	*pTRG = NULL, *pPIC = NULL, *pSPR = NULL;

		for( U4 a = 0, ae = res.nFND(); a < ae; a++ )
		{
			gpcALU& alu = res.ALU(a);
			if( alu.alf < gpeALF_AAAAAA )
			{
				if( alu.alf < gpeALF_AAA )
				{

				}
				else if( alu.alf < gpeALF_AAAA )
				{
					switch( alu.alf )
					{
						case gpeALF_CLR: {

							} break;
						case gpeALF_CAM:{
								I4 iCAM = alu.u8();
								if( !iCAM )
									break;
								if( pPIC = PIC.PIC( I8x2(alu.alf,iCAM) ) )
								{
									if( !pCAM )
										pCAM = new gpcPICAM;
									U1* pRGB = pPIC->getPIX( pCAM, win.mSEC.y ); //50 );
									if( pRGB )
									{
										if( pCAM->bGD() )
										{
											gpmDEL( pPIC->pPACK );
											win.pSYNwin = win.pSYNwin->syncADD( gpcSYNC( gpeNET4_0PIC, pPIC->id+1, win.mSEC.y, INVALID_SOCKET, 0 ), win.msSYN );
										}

										if( !win.pPICbg )
										{
											win.pPICbg = pPIC;
										}
									}
									else if( pPIC->pSHR )
											win.pPICbg = pPIC;
								}
							} break;

						case gpeALF_PIC:{
								if( alu.typ().x & 0x10 )
								{
									// ez a kép neve
									pic_id = PIC.alfFND( (U1*)alu.pDAT );
								} else
									pic_id = alu.u8();

								// be kell mapolni a rublika háterének
								if( pPIC = PIC.PIC( pic_id ) )
								{
									pic_id++;
									break;
								}

								I8x2 alfN(0,14);
								U1* pS = (U1*)alu.pDAT;
								alfN = pS;
								alfN.num = gpfSTR2U8( pS+alfN.num, &pS );
								if( !(pPIC = PIC.PIC( alfN )) )
									break;

							} break;
						case gpeALF_SDX:
							sprt[1].x = alu.i8();
							break;
						case gpeALF_SDY:
							sprt[1].y = alu.i8();
							break;
						case gpeALF_SDW:
							sprt[1].z = alu.i8();
							break;
						case gpeALF_SDH:{
							// ez indítja a rajzolást MOST
							/// nem így lesz csak próba
								sprt[1].w = alu.i8();
								if( !(sprt[0].a4x2[1].area()*sprt[1].a4x2[1].area()) )
									break;
								if( !pTRG || !pSPR )
									break;

								if( !pTRG->pSRF )
                                {
									if( !trgWH.z )
										trgWH.z = 640;
									if( !trgWH.w )
										trgWH.w = 480;

									pTRG->pSRF = SDL_CreateRGBSurface( 0, trgWH.z, trgWH.w, 32, 0,0,0,0 ); // rmask, gmask,bmask, amask );
									if( !pTRG->pSRF )
										break;
									if( trgWH.a4x2[0] != trgWH.a4x2[1] )
									{
										SDL_SetSurfaceBlendMode( pTRG->pSRF, SDL_BLENDMODE_BLEND );
										SDL_SetSurfaceAlphaMod( pTRG->pSRF, 128 );
										SDL_SetRenderDrawBlendMode( win.pSDLrndr, SDL_BLENDMODE_BLEND);
										trgWH.a4x2[0] = 0;
										SDL_FillRect( pTRG->pSRF, &trgWH.xyWH, 0 ); //amask );
										trgWH.a4x2[0] = trgWH.a4x2[1];
									}
                                }
                                if( !pSPR->surDRW() )
                                {
									pSPR->TnID.an2str( (U1*)win.gppMASSfile, (U1*)".png" );
									pSPR->pSRF = IMG_Load( win.gpsMASSpath );
									if( !pSPR->pSRF )
										break;
                                }

								gpdBLTs( pSPR->surDRW(), &sprt[0].xyWH, pTRG->pSRF, &sprt[1].xyWH );
								pTRG->pREF = NULL;
								//SDL_UpdateWindowSurface( win.pSDLwin );
							} break;
						case gpeALF_SSX:
							sprt[0].x = alu.i8();
							break;
						case gpeALF_SSY:
							sprt[0].y = alu.i8();
							break;
						case gpeALF_SSW:
							sprt[0].z = alu.i8();
							break;
						case gpeALF_SSH:
							sprt[0].w = alu.i8();
							break;

						case gpeALF_TRG:{
								U8 trg_id = 0;

								if( alu.typ().x & 0x10 )
								{
									// ez a kép neve
									trg_id = PIC.alfFND( (U1*)alu.pDAT );
								} else
									trg_id = alu.u8();

								if( pTRG = PIC.PIC( trg_id ) )
                                    break;

								I8x2 alfN(0,14);
								U1* pS = (U1*)alu.pDAT;
								alfN = pS;
								alfN.num = gpfSTR2U8( pS+alfN.num, &pS );
								if( !(pTRG = PIC.PIC( alfN )) )
									break;

							} break;
						default:
							break;
					}
				} else { //	gpeALF_ZZZZZ // 5 char
					switch( alu.alf )
					{
						case gpeALF_SYNC:
							if( alu.typ().x & 0x10 )
							if( gpcGT* pGT = GTcnct.GT( alu.alf, (U1*)alu.pDAT, alu.nLOAD() ) )
								pGT->GTcnct( win );

							break;
						case gpeALF_RINP:
							anRio.a8x2[0].x = alu.i8();
							anRio.a8x2[0].y = anRio.a8x2[0].x&0xff;
							anRio.a8x2[0].x >>= 0x10;
							break;
						case gpeALF_ROUT:
							anRio.a8x2[1].x = alu.i8();
							anRio.a8x2[1].y = anRio.a8x2[1].x&0xff;
							anRio.a8x2[1].x >>= 0x10;
							break;
						case gpeALF_SLMP:
							if( anRio.a8x2[0].x*anRio.a8x2[1].x < 1 )
								break;

							if( alu.typ().x & 0x10 )
							if( gpcGT* pGT = GTcnct.GT( alu.alf, (U1*)alu.pDAT, alu.nLOAD() ) )
								pGT->GTcnct( win );

							break;
						case gpeALF_TRGH:
							trgWH.w = alu.u8();
							break;
						case gpeALF_TRGW:
							trgWH.z = alu.u8();
							break;
						case gpeALF_UNSEL:
							if( alu.u8()&1 )
								pSRC->bSW |= gpeMASSunselMSK;
							else
								pSRC->bSW &= ~gpeMASSunselMSK;
							break;
						case gpeALF_GPIO:

							break;
						default:
							break;
					}
				}

			}
			else switch( alu.alf )
			{
				case gpeALF_SPRITE: {
						U8 spr_id = 0;
						if( alu.typ().x & 0x10 )
						{
							// ez a kép neve
							spr_id = PIC.alfFND( (U1*)alu.pDAT );
						} else
							spr_id = alu.u8();

						if( pSPR = PIC.PIC( spr_id ) )
							break;

						I8x2 alfN(0,14);
						U1* pS = (U1*)alu.pDAT;
						alfN = pS;
						alfN.num = gpfSTR2U8( pS+alfN.num, &pS );
						if( !(pSPR = PIC.PIC( alfN )) )
							break;


					} break;
				case gpeALF_TELNET: {
						I4 port = alu.u8();
						if( gpcGT* pGT = GTacpt.GT( alu.alf, port ) )
						{
							gpcGT& gt = *pGT;

							gt.GTlst( win, GTcnct );
							if( gpcLZY* pEVNT = gt.pEVENT->qEVENT() )
							{
								U1x4* pBGRA;
								U4 nGD = 0; U8 s;
								gpcHUD* pEV = (gpcHUD*)pEVNT->p_alloc;
								for(U4 i = 0, n = pEVNT->n_load / sizeof(gpcHUD); i < n; i++)
								{
									switch(pEV[i].id)
									{
										case gpeNET4_0EYE:
											if( gpcGT* pGT = gt.GTacc.iGT(pEV[i].n) ) {
												U4	rmask, gmask, bmask, amask;
												#if SDL_BYTEORDER == SDL_BIG_ENDIAN
												  int shift = (req_format == STBI_rgb) ? 8 : 0;
												  rmask = 0xff000000 >> shift;
												  gmask = 0x00ff0000 >> shift;
												  bmask = 0x0000ff00 >> shift;
												  amask = 0x000000ff >> shift;
												#else // little endian, like x86
												  rmask = 0x000000ff;
												  gmask = 0x0000ff00;
												  bmask = 0x00ff0000;
												  amask = 0; //(req_format == STBI_rgb) ? 0 : 0xff000000;
												#endif
												I4x4	src( 0,0, win.pSRFwin->w, win.pSRFwin->h ),
														dst( 0,0, win.pSRFwin->w/2, win.pSRFwin->h/2 );

												if( win.pSRFwin )
												{
													if( win.pSRFwin->w != src.z || win.pSRFwin->h != src.w )
													{
														gpmSDL_FreeSRF(win.pSRFwin);
														win.pSRFwin = SDL_CreateRGBSurface( 0, src.z, src.w, 24, rmask, gmask, bmask, amask );
													}

													SDL_RenderReadPixels( win.pSDLrndr,  &src.xyWH, win.pSRFwin->format->format, win.pSRFwin->pixels, win.pSRFwin->pitch );

												}

												if( !win.pSRFsnd )
													win.pSRFsnd = SDL_CreateRGBSurface(	0, dst.z, dst.w, 24, rmask, gmask, bmask, amask  );

												if( SDL_Surface* pSURF = win.pSRFsnd )
												{
													src.a4x2[0] = 0;
													gpdBLTs( win.pSRFwin, &src.xyWH, pSURF, &dst.xyWH );

													pJPGsnd = NULL; //gpfSRF2JPG( pJPGsnd, pSURF, 57 );
													if( pJPGsnd )
													{
														pGT->pHUD = pGT->pHUD->put( pJPGsnd->p_alloc, pJPGsnd->n_load );
														pGT->pHUD->id = gpeNET4_0EYE;
														pJPGsnd->lzyRST();
														//gpmDEL(pLZY);
													} else {
														gpmDEL(pJPGsnd);
														if( !gpfSRFjpgSAVE( (U1*)"/mnt/ram/tmp.tmp", pSURF, 57 ) )
															IMG_SavePNG( pSURF, "/mnt/ram/tmp.tmp" );

														gpcLZY* pPNG = ((gpcLZY*)NULL)->lzyRD( "/mnt/ram/tmp.tmp", s = -1 );
														if( pPNG )
														{
															nGD++;
															pGT->pHUD = pGT->pHUD->put( pPNG->p_alloc, pPNG->n_load );
															pGT->pHUD->id = gpeNET4_0EYE;
															gpmDEL(pPNG);
														}
														//SDL_FreeSurface(pSURF);
														if( gpfACE( gpdPICbg, 4) > -1 )
														{
															rename( gpdPICbg, "/mnt/ram/bg.kill" );
														}
														rename( "/mnt/ram/tmp.tmp", gpdPICbg );

														if( gpfACE("/mnt/ram/bg.kill", 4) > -1 )
														{
															remove( "/mnt/ram/bg.kill" );
														}
													}
												}
											} break;
										case gpeNET4_0HUD:
											if( win.pPICbg )
											if( gpcGT* pGT = gt.GTacc.iGT(pEV[i].n) ) {
												nGD++;
												pGT->pHUD = pGT->pHUD->put( win.pPICbg->getPIX(), win.pPICbg->nPIX/6 );
												pGT->pHUD->id = pEV[i].id;
											} break;
										case gpeNET4_PREV:
											if( win.pPICbg )
											if( gpcGT* pGT = gt.GTacc.iGT(pEV[i].n) ) {
												nGD++;
												pGT->pHUD = pGT->pHUD->put( "\u001B[2J\r", strlen("\u001B[2J\r") );
												pGT->pHUD->id = gpeNET4_PREV;
												pT = pTXT;
												while( *pT )
												{
													pGT->pHUD = pGT->pHUD->put( pT, strlen(pT) );
													pGT->pHUD = pGT->pHUD->put( "\r\n", 2 );
													pT += strlen(pT)+1;
												}
												pGT->pHUD->id = gpeNET4_PREV;
											} break;
										default:
											break;
									}
								}
								if( nGD )
									pEVNT->lzyRST();
								pEVNT->DO();
							}
						}

					} break;
				default:
					break;
			}

		}
	}


	return pKEYbuff;
}
