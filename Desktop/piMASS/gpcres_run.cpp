#define gpdNEW_CMPLR

#include "gpcwin.h"

bool bITT =  false; //true; // false; //
extern U1 gpaALFsub[];


//gpcRES* gpcRES::RESrun( gpcRES* pOUT, gpcLZY* pMN, gpcWIN& win, gpcSRC* pSRC, gpcRES* pMOM, U4 deep, gpcSTK* pSTK ) {
//	if( win.pM ? !this : true )
//		return pOUT;
//
//	if( !nISA.x )
//	{
//		gpmDELary( pISA );
//		return pOUT;
//	}
//
//	if( pOUT ? false : !deep )
//	{
//		pOUT = new gpcRES( pMOM );
//	}
//	gpcSTK stk( pSTK, pOUT, pSRC );
//	U4 iASG, nOUT = 0;
//
//	U8 u8;
//	I8 i8;
//	double d8;
//
//	U1	sBUFF[0x1000],
//		*pB = sBUFF+0x20, *pS = pB, nB;
//
//	gpcREG err;
//	I1x4 isa = 0; //, back;
//	bool bROW = false;
//
//	gpcMASS& mass = *win.piMASS;
//
//
//
//	U4	mZ = win.mZ,
//		mN = win.mN, mxZN = mZ*mN + 1; //, znI = mxZN;
//	U4x2 trg = 0;
//	U4 anFND = 0;
//	gpcADR A0;
//	gpcREG D0;
//	gpcALU A, B, FND;
//	gpcSRC	*pANS = NULL;
//	gpcRES	*pANcall = NULL, *pDOT = NULL;
//	U1		*pSTR = NULL;
//
//	for( U4 i = 0; i < nISA.x; i++ )
//	{
//		gpcISA &IS = pISA[i];
//		gpfFLG &flg = stk.main;
//		if( (U1)IS.isa.aISA[0] > ' ' )
//		{
//			*pB = (U1)IS.isa.aISA[0];
//			pB++;
//		}
//
//		switch( IS.isa.aISA[0] )
//        {
//			case gpeISA_trg: {	// -------------------------------------------------------------------------------------
//					trg = IS.an;
//				} continue;
//
//
//			case gpeISA_an: {	// -------------------------------------------------------------------------------------
//						//pB += sprintf( (char*)pB, "%s", IS.an.pSTRalf4n( sBUFF ));
//						anFND = 0;
//						B = gpeALF_null;
//						if( IS.an.x ? IS.an.x > mZ : true )
//							break;
//						if( IS.an.y < mN )
//						if(	anFND = win.pM[IS.an.y*mZ + IS.an.x-1] )
//						if( pANS = mass.srcFND( anFND ) )
//							pSTR = pANS->pB+1;
//					} break;
//
//			case gpeISA_anFUN: {	// -------------------------------------------------------------------------------------
//					//pB += sprintf( (char*)pB, "%s", IS.an.pSTRalf4n( sBUFF ));
//					stk.aAN[flg.iA] = IS.an;
//					//flg.iA++;
//				} break;
//
//
//			/// gpcREG D[]	// --------------------------------------------------------------------------------------
// 			case gpeISA_u8: {	// --------------------------------------------------------------------------------------
//					D0 = IS.an.u8;
//					B = gpeALF_null;
//				} break;
//			case gpeISA_i8: {	// --------------------------------------------------------------------------------------
//					D0 = -((I8)IS.an.u8);
//					B = gpeALF_null;
//				} break;
//			case gpeISA_d8: {	// --------------------------------------------------------------------------------------
//					D0 = IS.an.d8;
//					B = gpeALF_null;
//				} break;
//
//
//
//			/// gpeISA_var	// --------------------------------------------------------------------------------------
// 			case gpeISA_var: {	// --------------------------------------------------------------------------------------
//					A0 = IS.an.var;	/// gpcADR
//					if( !A0.an.alf )
//						break;
//
//					if( pDOT ) /// pANS->apOUT[3]
//					{
//						A0 = pDOT;
//						pDOT = NULL;
//						if( !A0.pRM )
//							break;
//						if( pANS ? pANS->bSW&gpeMASSloopMSK : false )
//							pSRC->bSW |= gpeMASSloopMSK;
//						// lokális változó és már használta valaki
//						isa = pISA[i-1-IS.i].isa;
//						if( isa.aISA[0] == gpeISA_trg )
//							isa = pISA[i-2-IS.i].isa;
//						B = A0.pRM->ALU( A0.iA );
//						break;
//					}
//
//					if( pOUT )
//					{
//						A0 = pOUT;
//						if( A0.pRM )
//						{
//							(IS.isa.aISA[1] == gpeISA_assign ? FND : B) = A0.pRM->ALU( A0.iA );
//							break;
//						}
//					}
//					// na még mindig nincsen
//					A0 = this;
//					if( A0.pRM )
//					{
//						// forrásként használható
//						if( IS.isa.aISA[1] == gpeISA_assign )
//						{
//							FND = pOUT->ADD( A0.an.alf, 0,0 );
//							FND.equ( A0.pRM->ALU( A0.iA ) );
//						} else
//							B = A0.pRM->ALU( A0.iA );
//						break;
//					}
//					// na most nézzük meg van e beépített változó rá
//					if( win.WINvar( D0, A0.an.alf ) )
//					{
//						// valamit kapott
//						pSRC->bSW |= gpeMASSloopMSK;
//						pSTR = D0.getSTR();
//						break;
//					}
//
//					(IS.isa.aISA[1] == gpeISA_assign ? FND : B) = pOUT->ADD( A0.an.alf, 0,0 );
//				} break;
//
//			case gpeISA_FUN: { 	// --------------------------------------------------------------------------------------
//					//pB += sprintf( (char*)pB, "%s", IS.an.strVAR( sBUFF ) ) ;
//					stk.aVR[flg.iV] = IS.an.var;
//					//flg.iV++;
//				} break;
//
//
//			case gpeISA_tag: {
//					 //pB += sprintf( (char*)pB, "%s", IS.an.strVAR( sBUFF ) );
//					stk.aTG[flg.iG] = IS.an.var;
//					//flg.iG++;
//				} break;
//
//
//			case gpeISA_str: {
//					pSTR = IS.an.apSTR[0];
//					//pB += sprintf( (char*)pB, "%s", (stk.apSTR[flg.iS] = IS.an.aSTR[0])) ;
//					//flg.iS++;
//				} break;
//			case gpeISA_row:
//				bROW = true;
//				break;
//			case gpeISA_nop:
//			default:
//				break;
//		}
//		// CALL
//		if( IS.pRES )
//			pOUT = IS.pRES->RESrun( pOUT, pMN, win, pSRC, this, deep+1, &stk );
//
//
//		switch( isa.aISA[1] ) {
//			case gpeISA_u8:
//			case gpeISA_i8:
//			case gpeISA_d8: {
//						B = D0;
//					} break;
//
//			case gpeISA_an:{} break;
//			case gpeISA_anFUN:{} break;
//			case gpeISA_var:{
//
//				} break;
//			case gpeISA_FUN:{} break;
//			case gpeISA_dim:{} break;
//			case gpeISA_not:{} break;
//			case gpeISA_str:{} break;
//			case gpeISA_tag:{} break;
//			case gpeISA_dollar:{} break;
//			case gpeISA_rem:{
//					if( A.pRM ? D0.bGD() : false )
//						A %= D0;
//				} break;
//			case gpeISA_and:{} break;
//			case gpeISA_brkB:{} break;
//			case gpeISA_brkE:{} break;
//			case gpeISA_mul:{
//					if( A.pRM ? D0.bGD() : false )
//						A *= D0;
//				} break;
//			case gpeISA_plus:{
//					if( A.alf && B.alf )
//					{
//						A += B;
//						B = gpeALF_null;
//						break;
//					}
//
//					if( A.pRM ? D0.bGD() : false )
//						A += D0;
//				} break;
//			case gpeISA_sub:{
//					if( A.pRM ? D0.bGD() : false )
//						A -= D0;
//				} break;
//			case gpeISA_div:{
//					if( A.pRM ? D0.bGD() : false )
//						A /= D0;
//				} break;
//
//
//
//
//			case gpeISA_dot:{
//					pANcall = NULL;
//					if( anFND )
//						pDOT = ( pANS = mass.srcFND( anFND ) ) ? pANS->apOUT[3] : NULL;
//					else
//						pDOT = NULL;
//				} break;
//
//
//			case gpeISA_row:
//				bROW = true;
//				break;
//			case gpeISA_litl:{} break;
//
//
//
//			case gpeISA_assign:{
//					if( D0.bGD() )
//						A = D0;
//					else if( pSTR )
//					{
//                        A = pSTR;
//                        pSTR = NULL;
//					}
//					if( A.alf && B.alf )
//					{
//						A.equ( B );
//					}
//					else if( pANcall )
//					{
//						pANcall;
//					}
//					B = gpeALF_null;
//				} break;
//
//
//
//			case gpeISA_gret:{} break;
//			case gpeISA_exp:{} break;
//			case gpeISA_root:{} break;
//			case gpeISA_brkDB:{} break;
//			case gpeISA_brkDE:{} break;
//			case gpeISA_equ:{} break;
//			case gpeISA_trg:{} break;
//			case gpeISA_blkB:{} break;
//			case gpeISA_or:{} break;
//			case gpeISA_blkE:{} break;
//
//			case gpeISA_nop:
//			default:
//				break;
//		}
//		isa = IS.isa;
//		if( (U1)isa.aISA[1] > ' ' )
//		{
//			//B.isa = IS.isa;
//			//stk.stpFLG();
//			*pB = (U1)IS.isa.aISA[1];
//			pB++;
//		}
//
//		switch( isa.aISA[1] )
//		{
//			case gpeISA_assign:
//				A = FND;
//				B = gpeALF_null;
//				break;
//			case gpeISA_dot:{
//					pANcall = NULL;
//					if( anFND )
//						pDOT = ( pANS = mass.srcFND( anFND ) ) ? pANS->apOUT[3] : NULL;
//					else
//						pDOT = NULL;
//				} break;
//		}
//
//		if( !bITT )
//			continue;
//
//		if( !bROW )
//			continue;
//
//		bROW = false;
//
//		if( pS < pB )
//		{
//			*pB = 0;
//			if(bSTDcout){gpdCOUT << (char*)pS <<gpdENDL;}; // ; //
//			pB = pS;
//		}
//	}
//	*pB = 0;
//
//	if( bITT )
//	if( pS < pB )
//	{
//		if(bSTDcout){gpdCOUT << (char*)pS <<gpdENDL;}; // ; //
//		pB = pS;
//	}
//
//	if( !pSRC )
//		return pOUT;
//
//	pSRC->pMINI = pOUT->res2mini(
//									pSRC->pMINI, sBUFF,
//
//									( nASG && (pSRC->bSW&gpeMASSdebugMSK) )
//									? this : NULL,
//
//									deep
//								);
//
//	if( deep )
//		return pOUT;
//
//	if( pSRC->pMINI ? !pSRC->pMINI->n_load : false  )
//		gpmDEL(pSRC->pMINI);
//	return pOUT;
//}

U4 gpcMASS::jDOitREF( gpcWIN* pWIN, U4 i, U4& ie, U4 **ppM, U4 **ppC, U4 **ppR, U4* pZ ) {
	/// U4 i éppen hol tartott, ha át kell méretezni a pSRC mapot majd relocalja az it
	if( pWIN->mZN == mapCR.mapZN44.a4x2[1].area() ) {
		if(ppM)
			*ppM = pWIN->pM;
		if(ppC)
			*ppC = pWIN->pC;
		if(ppR)
			*ppR = pWIN->pR;
		if(pZ)
			*pZ = mapCR.mapZN44.z;
		ie = pWIN->pC-pWIN->pM;
		return i;
	}

	pWIN->mZ = mapCR.mapZN44.z;
	pWIN->mN = mapCR.mapZN44.w;
	pWIN->mZN = mapCR.mapZN44.a4x2[1].area();
	U4 iz = i%pWIN->mZ, in = i/pWIN->mZ;

	pWIN->pM = mapCR.pMAP;
	if(ppM)
		*ppM = pWIN->pM;
	pWIN->pC = mapCR.pCOL;
	if(ppC)
		*ppC = pWIN->pC;
	pWIN->pR = mapCR.pROW;
	if(ppR)
		*ppR = pWIN->pR;
	if(pZ)
		*pZ = pWIN->mZ;
	ie = pWIN->pC-pWIN->pM;
	return in*pWIN->mZ+iz;
}
char gpsPUB[0x1000];
U1* gpcMASS::justDOit( gpcWIN* pWIN ) { // U1* sKEYbuff, I4x4& mouseXY, U4* pKT, I4x4& SRCxycr, I4x4& SRCin )

	pWIN->nJDOIT.w++;
	pWIN->nJDOIT.y = pWIN->nJDOIT.x;
	U1* pKEYbuff = pWIN->gpsKEYbuff;
	gpcSRC	tmp, *pSRC = NULL, *pS2;
	U4 xFND, x_fnd;
	//int aGLpic[0] = 0;
	I4x4 sprt[2] = {0}, trgWH = 0;
	pWIN->mZ = mapCR.mapZN44.z;
	pWIN->mN = mapCR.mapZN44.w;
	pWIN->mZN = mapCR.mapZN44.a4x2[1].area();
	I8x4 anRio;
	gpcLZY hex; U8 s;
	gpcGL *pGL = pWIN->pGL;
	char* pVTX = NULL;
	gpcPIC	*aGLpPIC[0x10],
			*aGLpBOB[0x10];
	SDL_Texture* apTX[0x10];

	U4	*pM,*pC,*pR,
		ie,z;
	jDOitREF( pWIN, 0, ie, &pM, &pC, &pR, &z );

	for( U4 i = 0; i < ie; i++ ) {
		if( pSRC  ) {
			if( aGLpic[0] )
				pSRC->picID = aGLpic[0];
			if( aGLpic[0xf] )
				pSRC->bobID = aGLpic[0xf];

			aGLpic[0xf] = 0;
		}

		if( !pM[i] )
			continue;

		xFND = pM[i];
		pSRC = srcFND( xFND );
		if( !pSRC )
			continue;

		aGLpic[0] = pSRC->picID = 0;
		//gpmDEL( pSRC->apOUT[0] );
		if( pSRC->qBLD() ) {
			pSRC->msBLD = pWIN->mSEC.x + pSRC->msBLTdly;
			pSRC->rdyBLD();
		}

		if( pSRC->srcBLD( pWIN, NULL ) == 1 ) {

			//gpmDEL( pSRC->pEXE0 );

			if( !pSRC->msBLTdly )
				pSRC->msBLTdly = gpdSYNmSEC;
			else
				pWIN->pSYNwin =
				pWIN->pSYNwin->syncADD(
											gpcSYNC(
														gpeNET4_0SRC,
														(i%pWIN->mZ)+((i/pWIN->mZ)<<16),
														pWIN->mSEC.y, pSRC->iGT, 0
													),
											pWIN->msSYN
										);
			pSRC->msBLD = 0;
		}


		if( !pSRC->srcINSTrun() )
			continue;

		pSRC->pMINI = pSRC->srcINSTmini(pSRC->pMINI); //,this,pWIN);
		pWIN->nJDOIT.x++;
	}

	return pKEYbuff;
}
U1* gpcMASS::justDOitOLD( gpcWIN* pWIN ) { // U1* sKEYbuff, I4x4& mouseXY, U4* pKT, I4x4& SRCxycr, I4x4& SRCin )

	pWIN->nJDOIT.w++;
	pWIN->nJDOIT.y = pWIN->nJDOIT.x;
	U1* pKEYbuff = pWIN->gpsKEYbuff;
	gpcSRC	tmp, *pSRC = NULL, *pS2;
	U4 xFND, x_fnd;
	//int aGLpic[0] = 0;
	I4x4 sprt[2] = {0}, trgWH = 0;
	pWIN->mZ = mapCR.mapZN44.z;
	pWIN->mN = mapCR.mapZN44.w;
	pWIN->mZN = mapCR.mapZN44.a4x2[1].area();
	I8x4 anRio;
	gpcLZY hex; U8 s;
	gpcGL *pGL = pWIN->pGL;
	char* pVTX = NULL;
	gpcPIC	*aGLpPIC[0x10],
			*aGLpBOB[0x10];
	SDL_Texture* apTX[0x10];

	U4	*pM,*pC,*pR,
		ie,z;
	jDOitREF( pWIN, 0, ie, &pM, &pC, &pR, &z );

	for( U4 i = 0; i < ie; i++ ) {
		if( pSRC  ) {
			if( aGLpic[0] )
				pSRC->picID = aGLpic[0];
			if( aGLpic[0xf] )
				pSRC->bobID = aGLpic[0xf];

			aGLpic[0xf] = 0;
		}

		if( !pM[i] )
			continue;

		xFND = pM[i];
		pSRC = srcFND( xFND );
		if( !pSRC )
			continue;

		aGLpic[0] = pSRC->picID = 0;
		//gpmDEL( pSRC->apOUT[0] );
		if( pSRC->qBLD() ) {
			pSRC->msBLD = pWIN->mSEC.x + pSRC->msBLTdly;
			pSRC->rdyBLD();
		}

		if( pSRC->msBLD ? pSRC->msBLD <= pWIN->mSEC.x : false ) {

			pSRC->srcBLD( pWIN, NULL );

			//gpmDEL( pSRC->pEXE0 );
			//pSRC->pEXE0 = pSRC->pEXE0->compiEASY( pSRC->pSRCstart( true, 4 ), NULL, NULL, NULL );

			if( !pSRC->msBLTdly )
				pSRC->msBLTdly = gpdSYNmSEC;
			else
				pWIN->pSYNwin = pWIN->pSYNwin->syncADD( gpcSYNC( gpeNET4_0SRC, (i%pWIN->mZ)+((i/pWIN->mZ)<<16) , pWIN->mSEC.y, pSRC->iGT, 0 ), pWIN->msSYN );
			pSRC->msBLD = 0;

			/* pSRC->apOUT[0] = pSRC->apOUT[1];
			pSRC->apOUT[1] = pSRC->apOUT[2];
			pSRC->apOUT[2] = pSRC->apOUT[3];
			pSRC->apOUT[3] = NULL;*/
		}
		else if( pSRC->bSW&gpeMASSloopMSK )
		{
			/* pSRC->apOUT[0] = pSRC->apOUT[1];
			pSRC->apOUT[1] = pSRC->apOUT[2];
			pSRC->apOUT[2] = pSRC->apOUT[3];
			pSRC->apOUT[3] = NULL;*/
		}

		//if( !pSRC->apOUT[3] )
		//{
		if( !pSRC->srcINSTrun() ) //this,pWIN) )
			continue;

		pWIN->nJDOIT.x++;
		pSRC->pMINI = pSRC->srcINSTmini(pSRC->pMINI); //,this,pWIN);
		//}



		continue;


		/// READY
		gpcRES res; // = NULL; //*pSRC->apOUT[3];
		gpcPIC	*pTRG = NULL, *pPIC = NULL, *pSPR = NULL;

		for(
			U4	a = 0, ae = res.nFND(),
				nCNL = 0, mskPIC = 0, mCLR;
				a < ae;
				a++
		) {
			gpcALU& alu = res.ALU(a);
			if( alu.alf < gpeALF_AAAAAA )
			{
				if( alu.alf < gpeALF_AAA )
				{

				}
				else if( alu.alf < gpeALF_AAAA ) {
					switch( alu.alf ) {
						case gpeALF_CLR: mCLR = alu.u8(); break;
						case gpeALF_CAM:{
								I8 iCAM = alu.u8();
								if( !iCAM )
									break;
								if( aGLpPIC[0] = PIC.PIC( I8x2(alu.alf,iCAM) ) ) {
									if( !pCAM )
										pCAM = new gpcPICAM;
									U1* pRGB = aGLpPIC[0]->getPIX( pCAM, pWIN->mSEC.y ); //50 );
									if( pRGB )
									{
										if( pCAM->bGD() )
										{
											gpmDEL( aGLpPIC[0]->pPACK );
											pWIN->pSYNwin = pWIN->pSYNwin->syncADD( gpcSYNC( gpeNET4_0PIC, aGLpPIC[0]->id+1, pWIN->mSEC.y, INVALID_SOCKET, 0 ), pWIN->msSYN );
										}

										if( !pWIN->pPICbg )
										{
											pWIN->pPICbg = aGLpPIC[0];
										}
									}
									else if( aGLpPIC[0]->pSHR )
											pWIN->pPICbg = aGLpPIC[0];
								}
							} break;
						case gpeALF_BOB: {
								aGLpic[0xf] = alu.bSTR() ?
											PIC.alfFND( (U1*)alu.pDAT ) 	// ez a kép neve
											: alu.u8();						// száma
								if( aGLpBOB[0] = PIC.PIC( aGLpic[0xf] ) )
								{
									aGLpic[0xf]++;
									break;
								}
							} break;
						case gpeALF_PIC: {
								aGLpic[0] =	alu.bSTR() ?
											PIC.alfFND( (U1*)alu.pDAT ) 	// ez a kép neve
											: alu.u8();						// száma
								// be kell mapolni a rublika háterének
								if( aGLpPIC[0] = PIC.PIC( aGLpic[0] ) )
								{
									aGLpic[0]++;
									break;
								}

								I8x2 alfN(0,14);
								U1* pS = (U1*)alu.pDAT;
								alfN = pS;
								alfN.num = gpfSTR2U8( pS+alfN.num, &pS );
								if( !(aGLpPIC[0] = PIC.PIC( alfN )) )
									break;

							} break;
				///----------------------------------------------------
				///
				///				OpenGL render
				///
				///----------------------------------------------------
				/// Vertex SHADERT
						case gpeALF_VTX: if( pGL ? alu.bSTR() : false ) {
								pVTX = (char*)alu.pDAT;
								if( !*pVTX )
									pVTX = NULL;
							} break;
				/// Pixel SHADERT Compile?
						case gpeALF_PIX: if( pGL ? alu.bSTR() : false ) {
								char* pPIX = (char*)alu.pDAT;
								U4 i = res.iFND(gpeALF_NAME);
								gpcALU& aNM = res.ALU(i);
								pGL->GLSLset( aNM, pPIX, pVTX );
							} break;
				/// Target PICTURE
						case gpeALF_TRG:{
								U8 trg_id =	alu.bSTR() ?
											PIC.alfFND( (U1*)alu.pDAT ) 	// ez a kép neve
											: alu.u8();						// száma
								if( pTRG = PIC.PIC( trg_id ) )
									break;

								I8x2 alfN(0,14);
								U1* pS = (U1*)alu.pDAT;
								alfN = pS;
								alfN.num = gpfSTR2U8( pS+alfN.num, &pS );
								if( !(pTRG = PIC.PIC( alfN )) )
									break;

							} break;
				/// DRAW with GPU
				/*	case gpeALF_GPU:if( pTRG && pGL ) {
								for( I4 i = 0, msk = mskPIC; msk; i++, msk>>=1 )
								{
									apTX[i] = 	aGLpPIC[i]
												? aGLpPIC[i]->surDRWtx(pGL->pRNDR)
												: NULL;
								}
								pGL
								->glSETtrg( pTRG, trgWH.a4x2[1], true, true )
								->GLSLset( alu )
								->glSETbox( trgWH.a4x2[0], trgWH.a4x2[1] )
								->glSETcnl( 0, aGLcnl, nCNL )
								->glSETtx( mskPIC, aGLpPIC )
								->glDRW( trgWH.a4x2[0], trgWH.a4x2[1] )
								->glDONE();
							} break;*/
				///----------------------------------------------------

				///----------------------------------------------------
				///
				///				SDL draw SPRITE
				///
				///----------------------------------------------------
						case gpeALF_SDX: sprt[1].x = alu.i8(); break;
						case gpeALF_SDY: sprt[1].y = alu.i8(); break;
						case gpeALF_SDW: sprt[1].z = alu.i8(); break;
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
										SDL_SetRenderDrawBlendMode( pWIN->pSDLrndr, SDL_BLENDMODE_BLEND);
										trgWH.a4x2[0] = 0;
										SDL_FillRect( pTRG->pSRF, &trgWH.xyWH, 0 ); //amask );
										trgWH.a4x2[0] = trgWH.a4x2[1];
									}
								}
								if( !pSPR->surDRW() )
								{
									pSPR->TnID.an2str( (U1*)pWIN->gppMASSfile, (U1*)".png" );
									pSPR->pSRF = IMG_Load( pWIN->gpsMASSpath );
									if( !pSPR->pSRF )
										break;
								}

								gpdBLTs( pSPR->surDRW(), &sprt[0].xyWH, pTRG->pSRF, &sprt[1].xyWH );
								pTRG->pREF = NULL;
								//SDL_UpdateWindowSurface( pWIN->pSDLwin );
							} break;
						case gpeALF_SSX: sprt[0].x = alu.i8(); break;
						case gpeALF_SSY: sprt[0].y = alu.i8(); break;
						case gpeALF_SSW: sprt[0].z = alu.i8(); break;
						case gpeALF_SSH: sprt[0].w = alu.i8(); break;
				///----------------------------------------------------


						default:
							break;
					}
				}
				else switch( alu.alf ) { //	gpeALF_ZZZZZ // 5 char

					case gpeALF_SYNC: if(alu.bSTR()) {
						if( gpcGT* pGT = GTcnct.GT( alu.alf, (U1*)alu.pDAT, alu.nLOAD() ) )
							pGT->GTcnct( pWIN );
					} break;

					case gpeALF_PICO:{
							aGLpPIC[0] = PIC.aluFND( alu );
							if( !aGLpPIC[0] )
								break;

							aGLpic[0] = aGLpPIC[0]->id+1;
							mskPIC |= 1;
						} break;
					case gpeALF_PICI:{
							aGLpPIC[1] = PIC.aluFND( alu );
							if( !aGLpPIC[1] )
								break;

							aGLpic[1] = aGLpPIC[1]->id+1;
							mskPIC |= 1<<1;
						} break;
					case gpeALF_PICii:{
							aGLpPIC[2] = PIC.aluFND( alu );
							if( !aGLpPIC[2] )
								break;

							aGLpic[2] = aGLpPIC[2]->id+1;
							mskPIC |= 1<<2;
						} break;
					case gpeALF_PICiv:{
							aGLpPIC[4] = PIC.aluFND( alu );
							if( !aGLpPIC[4] )
								break;

							aGLpic[4] = aGLpPIC[4]->id+1;
							mskPIC |= 1<<4;
						} break;
					case gpeALF_PICV:{
							aGLpPIC[5] = PIC.aluFND( alu );
							if( !aGLpPIC[5] )
								break;

							aGLpic[5] = aGLpPIC[5]->id+1;
							mskPIC |= 1<<5;
						} break;
					case gpeALF_PICvi:{
							aGLpPIC[6] = PIC.aluFND( alu );
							if( !aGLpPIC[6] )
								break;

							aGLpic[6] = aGLpPIC[6]->id+1;
							mskPIC |= 1<<6;
						} break;
					case gpeALF_PICX:{
							aGLpPIC[10] = PIC.aluFND( alu );
							if( !aGLpPIC[10] )
								break;

							aGLpic[10] = aGLpPIC[10]->id+1;
							mskPIC |= 1<<10;
						} break;
					case gpeALF_PICix:{
						aGLpPIC[9] = PIC.aluFND( alu );
						if( !aGLpPIC[9] )
							break;
						aGLpic[9] = aGLpPIC[9]->id+1;
						mskPIC |= 1<<9;
					} break;
					case gpeALF_PICxi:{
							aGLpPIC[11] = PIC.aluFND( alu );
							if( !aGLpPIC[11] )
								break;
							aGLpic[11] = aGLpPIC[11]->id+1;
							mskPIC |= 1<<11;
						} break;


/*
					case gpeALF_CNLX:	case gpeALF_CNLY:	case gpeALF_CNLZ:	case gpeALF_CNLW:	{
						aGLcnl[0].aXYZW[(alu.alf-gpeALF_CNLT)%4] = alu.d8();
						if( nCNL < 1 )
							nCNL = 1;
						} break;
					case gpeALF_CNLoX:	case gpeALF_CNLoY:	case gpeALF_CNLoZ:	case gpeALF_CNLoW:	{
						aGLcnl[0].aXYZW[(alu.alf-gpeALF_CNLoT)%4] = alu.d8();
						if( nCNL < 1 )
							nCNL = 1;
						} break;
					case gpeALF_CNLiX:	case gpeALF_CNLiY:	case gpeALF_CNLiZ:	case gpeALF_CNLiW:	{
						aGLcnl[1].aXYZW[(alu.alf-gpeALF_CNLiT)%4] = alu.d8();
						if( nCNL < 2 )
							nCNL = 2;
						} break;
					case gpeALF_CNLvX:	case gpeALF_CNLvY:	case gpeALF_CNLvZ:	case gpeALF_CNLvW:	{
						aGLcnl[5].aXYZW[(alu.alf-gpeALF_CNLvT)%4] = alu.d8();
						if( nCNL < 6 )
							nCNL = 6;
						} break;
					case gpeALF_CNLxX:	case gpeALF_CNLxY:	case gpeALF_CNLxZ:	case gpeALF_CNLxW:	{
						aGLcnl[10].aXYZW[(alu.alf-gpeALF_CNLxT)%4] = alu.d8();
						if( nCNL < 11 )
							nCNL = 11;
						} break;
*/


					case gpeALF_RINP: {
						anRio.a8x2[0].x = alu.i8();
						anRio.a8x2[0].y = anRio.a8x2[0].x&0xff;
						anRio.a8x2[0].x >>= 0x10;
						} break;
					case gpeALF_ROUT: {
						anRio.a8x2[1].x = alu.i8();
						anRio.a8x2[1].y = anRio.a8x2[1].x&0xff;
						anRio.a8x2[1].x >>= 0x10;
						} break;

					/*case gpeALF_SLMPo: if(alu.bSTR()) break; { // OFF -------------
						if( anRio.a8x2[0].x*anRio.a8x2[1].x < 1 )
							break;
						gpcGT* pGT = GTcnct.GT( alu.alf, (U1*)alu.pDAT, alu.nLOAD() );
						if( !pGT )
							break;

							I4 cnt = pGT->iCNT;
							pGT->GTcnct( pWIN );
							if( cnt == pGT->iCNT )
								anRio.a8x2[1].x = anRio.a8x2[0].x = 0;

							if( anRio.a8x2[0].x )
							if( gpcLZY *pLZYinp = pWIN->piMASS->GTlzyALL.LZY(gpdGTlzyIDinp(pGT->TnID)) )
							if( gpcZSnD	*pZSnD = gpmLZYvali( gpcZSnD, pLZYinp ) )
							for( U1 i = 0; i < 2; i++ )
							{
								x_fnd = pWIN->piMASS->getXFNDan( anRio.a8x2[0]+I8x2( i, 0 ) );
								pS2 = x_fnd ? pWIN->piMASS->srcFND( x_fnd ) : NULL;
								i = jDOitREF( pWIN, i, ie, &pM, &pC, &pR );

								if( !pS2 )
								{
									pS2 = pWIN->piMASS->SRCnew( tmp, NULL, anRio.a8x2[0], -1 );
									if( !pS2 )
										continue;
								}
								if( pS2 == pSRC )
									continue;
								hex.lzyFRMT( s=0, "\r\n\"\r\n" );

								//hex.lzyRST()->lzyZSnDstat( s=-1, *pZSnD, i );

								hex.lzyFRMT( s=-1, "\"\r\n" );
								pS2->SRCcpy( hex.p_alloc, hex.p_alloc+hex.n_load );
								pS2->srcUPDT();
								pZSnD->aDrc[i].async( gpsPUB, alu, &res );

							}

							if( !anRio.a8x2[1].x )
								break;

							// INP
							if( gpcLZY *pLZYin = pWIN->piMASS->GTlzyALL.LZY(gpdGTlzyIDinp(pGT->TnID)) )
							if( pLZYin->n_load )
							{
								x_fnd = pWIN->piMASS->getXFNDan( anRio.a8x2[1] );
								pS2 = x_fnd ? pWIN->piMASS->srcFND( x_fnd ) : NULL;
								i = jDOitREF( pWIN, i, ie, &pM, &pC, &pR );

								if( !pS2 )
								{
									pS2 = pWIN->piMASS->SRCnew( tmp, NULL, anRio.a8x2[1], -1 );
									if( !pS2 )
										continue;
								}
								if( pS2 == pSRC )
									continue;

								hex.lzyRST()->lzyHEXl( s = 0, pLZYin->p_alloc, pLZYin->n_load );
								pS2->SRCcpy( hex.p_alloc, hex.p_alloc+hex.n_load );
								pS2->srcUPDT();


							}
							anRio.a8x2[0].x = 0;
						} break;*/
					case gpeALF_SLMP: if(alu.bSTR()) {
						if( anRio.a8x2[0].x*anRio.a8x2[1].x < 1 )
							break;
						gpcGT* pGT = GTcnct.GT( alu.alf, (U1*)alu.pDAT, alu.nLOAD() );
						if( !pGT )
							break;

							I4 cnt = pGT->iCNT;
							pGT->GTcnct( pWIN );
							if( cnt == pGT->iCNT )
								anRio.a8x2[1].x = anRio.a8x2[0].x = 0;

							if( anRio.a8x2[0].x )
							if( gpcLZY *pLZYinp = pWIN->piMASS->GTlzyALL.LZY(gpdGTlzyIDinp(pGT->TnID)) )
							if( gpcROBnD *pROBnD = gpmLZYvali( gpcROBnD, pLZYinp ) )
							for( U1 i = 0; i < 2; i++ )
							{
								x_fnd = pWIN->piMASS->getXFNDan( anRio.a8x2[0]+I8x2( i, 0 ) );
								pS2 = x_fnd ? pWIN->piMASS->srcFND( x_fnd ) : NULL;
								i = jDOitREF( pWIN, i, ie, &pM, &pC, &pR );

								if( !pS2 )
								{
									pS2 = pWIN->piMASS->SRCnew( tmp, NULL, anRio.a8x2[0], -1 );
									if( !pS2 )
										continue;
								}
								if( pS2 == pSRC )
									continue;
								hex.lzyFRMT( s=0, "\r\n\"\r\n" );

								hex.lzyRST()->lzyROBnDstat( s=-1, *pROBnD, i );

								hex.lzyFRMT( s=-1, "\"\r\n" );
								pS2->SRCcpy( hex.p_alloc, hex.p_alloc+hex.n_load );
								pS2->srcUPDT();
								//pROBnD->aDrc[i].async( gpsPUB, alu, &res );

							}

							if( !anRio.a8x2[1].x )
								break;

							// INP
							if( gpcLZY *pLZYin = pWIN->piMASS->GTlzyALL.LZY(gpdGTlzyIDinp(pGT->TnID)) )
							if( pLZYin->n_load )
							{
								x_fnd = pWIN->piMASS->getXFNDan( anRio.a8x2[1] );
								pS2 = x_fnd ? pWIN->piMASS->srcFND( x_fnd ) : NULL;
								i = jDOitREF( pWIN, i, ie, &pM, &pC, &pR );

								if( !pS2 )
								{
									pS2 = pWIN->piMASS->SRCnew( tmp, NULL, anRio.a8x2[1], -1 );
									if( !pS2 )
										continue;
								}
								if( pS2 == pSRC )
									continue;

								hex.lzyRST()->lzyHEXl( s = 0, pLZYin->p_alloc, pLZYin->n_load );
								pS2->SRCcpy( hex.p_alloc, hex.p_alloc+hex.n_load );
								pS2->srcUPDT();


							}
							anRio.a8x2[0].x = 0;
						} break;

					case gpeALF_TRGH: trgWH.w = alu.u8(); break;
					case gpeALF_TRGW: trgWH.z = alu.u8(); break;
					case gpeALF_UNSEL:
						if( alu.u8()&1 )
							pSRC->bSW |= gpeMASSunselMSK;
						else
							pSRC->bSW &= ~gpeMASSunselMSK;

						break;
					case gpeALF_GPIO:

						break;
					case gpeALF_TOOL: if( pTRG ) {
						U4 i = alu.u8();
						//SDL_Surface* apP[0x5];
						switch( i )
						{
						case 1: { /// SPACE TOOL ----------------------------
							pTRG->TOOLspace(
								pWIN->piMASS->PIClzyALL, aGLpPIC,
								pWIN->gpsMASSname,
								pWIN->gpsMASSpath ,pWIN->gppMASSfile );

							} break;
						case 2: { /// EXPLODE TOOL ----------------------------
							pTRG->TOOLexplode(
								pWIN->piMASS->PIClzyALL, aGLpPIC,
								pWIN->gpsMASSname,
								pWIN->gpsMASSpath ,pWIN->gppMASSfile );

							} break;
						default:{
							}break;
						}



						} break;
					default:{
					} break;
				}
				//}
			}
			else switch( alu.alf ) {
					/*
				case gpeALF_CNLiiX: case gpeALF_CNLiiY: case gpeALF_CNLiiZ: case gpeALF_CNLiiW: {
					aGLcnl[2].aXYZW[(alu.alf-gpeALF_CNLiiT)%4] = alu.d8();
					if( nCNL < 3 )
						nCNL = 3;
					} break;
				case gpeALF_CNLiiiX: case gpeALF_CNLiiiY: case gpeALF_CNLiiiZ: case gpeALF_CNLiiiW: {
					aGLcnl[3].aXYZW[(alu.alf-gpeALF_CNLiiiT)%4] = alu.d8();
					if( nCNL < 4 )
						nCNL = 4;
					} break;
				case gpeALF_CNLivX: case gpeALF_CNLivY: case gpeALF_CNLivZ: case gpeALF_CNLivW: {
					aGLcnl[4].aXYZW[(alu.alf-gpeALF_CNLivT)%4] = alu.d8();
					if( nCNL < 5 )
						nCNL = 5;
					} break;
				case gpeALF_CNLviX: case gpeALF_CNLviY: case gpeALF_CNLviZ: case gpeALF_CNLviW: {
					aGLcnl[6].aXYZW[(alu.alf-gpeALF_CNLviT)%4] = alu.d8();
					if( nCNL < 7 )
						nCNL = 7;
					} break;
				case gpeALF_CNLviiX: case gpeALF_CNLviiY: case gpeALF_CNLviiZ: case gpeALF_CNLviiW: {
					aGLcnl[7].aXYZW[(alu.alf-gpeALF_CNLviiT)%4] = alu.d8();
					if( nCNL < 8 )
						nCNL = 8;
					} break;
				case gpeALF_CNLixX: case gpeALF_CNLixY: case gpeALF_CNLixZ: case gpeALF_CNLixW: {
						aGLcnl[8].aXYZW[(alu.alf-gpeALF_CNLixT)%4] = alu.d8();
						if( nCNL < 9 )
							nCNL = 9;
					} break;
				case gpeALF_CNLxiX: case gpeALF_CNLxiY: case gpeALF_CNLxiZ: case gpeALF_CNLxiW: {
					aGLcnl[11].aXYZW[(alu.alf-gpeALF_CNLxiT)%4] = alu.d8();
					if( nCNL < 12 )
						nCNL = 12;
					} break;
				case gpeALF_CNLxiiX: case gpeALF_CNLxiiY: case gpeALF_CNLxiiZ: case gpeALF_CNLxiiW: {
					aGLcnl[12].aXYZW[(alu.alf-gpeALF_CNLxiiT)%4] = alu.d8();
					if( nCNL < 13 )
						nCNL = 13;
					} break;
*/


				case gpeALF_PICiii:{
						aGLpPIC[3] = PIC.aluFND( alu );
						if( !aGLpPIC[3] )
							break;
						aGLpic[3] = aGLpPIC[3]->id+1;
						mskPIC |= 1<<3;
					} break;
				case gpeALF_PICvii:{
						aGLpPIC[7] = PIC.aluFND( alu );
						if( !aGLpPIC[7] )
							break;
						aGLpic[7] = aGLpPIC[7]->id+1;
						mskPIC |= 1<<7;
					} break;
				case gpeALF_PICviii:{
						aGLpPIC[8] = PIC.aluFND( alu );
						if( !aGLpPIC[8] )
							break;
						aGLpic[8] = aGLpPIC[8]->id+1;
						mskPIC |= 1<<8;
					} break;

				case gpeALF_PICxii:{
						aGLpPIC[12] = PIC.aluFND( alu );
						if( !aGLpPIC[12] )
							break;
						aGLpic[12] = aGLpPIC[12]->id+1;
						mskPIC |= 1<<12;
					} break;



				case gpeALF_PICCPY: if( pTRG ) {
							U4 i = alu.u8();
							if( !i )
								break;
							if( i < gpmN( aGLpPIC ) ? !aGLpPIC[i] : true )
								break;


							if( !aGLpPIC[i]->pSRF )
							{
								if( aGLpPIC[i]->glRNDR.x )
									aGLpPIC[i]->pSRF = SDL_CreateRGBSurface( 0,	aGLpPIC[i]->txWH.a4x2[1].x,
																				aGLpPIC[i]->txWH.a4x2[1].y, 32, 0,0,0,0 );
								/*if( pTRG->pRTX )
								{
									int w=0, h=0, acc=0;
									U4 frm;
									SDL_QueryTexture( pTRG->pRTX, &frm, &acc, &w, &h );
									aGLpPIC[i]->pSRF = SDL_CreateRGBSurface( 0, w, h, 32, 0,0,0,0 );
								}*/
							}
							if( !aGLpPIC[i]->pSRF )
								break;
							glTextureSubImage2D( GL_TEXTURE_2D, 0, GL_RGBA, 0, 0,	aGLpPIC[i]->txWH.a4x2[1].x,
																					aGLpPIC[i]->txWH.a4x2[1].y, 0, aGLpPIC[i]->pSRF->pixels );
							/*SDL_RenderReadPixels(	pWIN->pSDLrndr, NULL, 0,
													aGLpPIC[i]->pSRF->pixels,
													aGLpPIC[i]->pSRF->pitch 	);*/
							aGLpPIC[i]->pREF = NULL;

						} break;

				case gpeALF_SPRITE: {
						U8 spr_id = 0;
						if(alu.bSTR())
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
						if( gpcGT* pGT = GTacpt.GT( alu.alf, port ) ) {
							gpcGT& gt = *pGT;
							gt.GTlst( pWIN, GTcnct );
							if( gpcLZY* pEVNT = gt.pEVENT->qEVENT() ) {
								U1x4* pBGRA;
								U4 nGD = 0; U8 s;
								gpcHUD* pEV = (gpcHUD*)pEVNT->p_alloc;
								for(U4 i = 0, n = pEVNT->n_load / sizeof(gpcHUD); i < n; i++) {
									switch(pEV[i].id) {
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
												I4x4	src( 0,0, pWIN->pSRFwin->w, pWIN->pSRFwin->h ),
														dst( 0,0, pWIN->pSRFwin->w/2, pWIN->pSRFwin->h/2 );

												if( pWIN->pSRFwin )
												{
													if( pWIN->pSRFwin->w != src.z || pWIN->pSRFwin->h != src.w )
													{
														gpmSDL_FreeSRF(pWIN->pSRFwin);
														pWIN->pSRFwin = SDL_CreateRGBSurface( 0, src.z, src.w, 24, rmask, gmask, bmask, amask );
													}

													SDL_RenderReadPixels( pWIN->pSDLrndr,  &src.xyWH, pWIN->pSRFwin->format->format, pWIN->pSRFwin->pixels, pWIN->pSRFwin->pitch );

												}

												if( !pWIN->pSRFsnd )
													pWIN->pSRFsnd = SDL_CreateRGBSurface(	0, dst.z, dst.w, 24, rmask, gmask, bmask, amask  );

												if( SDL_Surface* pSURF = pWIN->pSRFsnd )
												{
													src.a4x2[0] = 0;
													gpdBLTs( pWIN->pSRFwin, &src.xyWH, pSURF, &dst.xyWH );

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
											if( pWIN->pPICbg )
											if( gpcGT* pGT = gt.GTacc.iGT(pEV[i].n) ) {
												nGD++;
												pGT->pHUD = pGT->pHUD->put( pWIN->pPICbg->getPIX(), pWIN->pPICbg->nPIX/6 );
												pGT->pHUD->id = pEV[i].id;
											} break;
										case gpeNET4_PREV:
											if( pWIN->pPICbg )
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
