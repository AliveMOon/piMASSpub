#include "gpcres.h"
extern U1 gpaALFadd[];

gpcRES* gpcRES::compiEASY( U1* pS, U1* pE, U1** ppE, gpcRES* pM )
{
	if( pE )
	if( pS >= pE )
		return this;

	gpcRES* pTMP = this ? NULL : new gpcRES( pM );
	if( pTMP )
		return pTMP->compiEASY( pS, pE, ppE, pM );
	else if( !this )
		return NULL;

	if( ppE )
		*ppE = pS;

	if( !pE )
		pE = pS+strlen((char*)pS);
	null();
	U4x4 xyWH = 0;
	U4 deep = 0, nALF;
	U1 	d, *pBEG = NULL,
		// *apA[2], *apN[2],
		*pSTR = pS, *pXe;

	//gpmZ(apA);
	//gpmZ(apN);
	//gpmZ(apP);

	U8 nUTF8, nLAB = 0, nSTR; //, u8 = 0;
	//double d8 = 0;

	I8x4 lab = 0;
	U1x4 typ = 0;
	I1x4 aOP[2] = {0};

	bool	bMATH = false,
			bCROSS = false; //,
			//bOP;
	gpeALF XML = gpeALF_null;
	I4 xmlD = 0;


	gpcADR adr;
	//gpcALU	aALU[8];

	//U4x2 sub = 0;

	I8x2 an; //aAN[8];

	//gpmZ(aALU);
	gpcLAZY str;
	U4  nAN = 0, nALU = 0, //iI,
		iTRG = 0;
	gpcISA	*pI = NULL, *pUD8 = NULL;
	nASG = 0;

	for( 	pS += gpmNINCS( pS, " \t\r\n" );
			pS < pE ? *pS : false;
			pS += gpmNINCS( pS, " \t\r\n" ), aOP[1] = aOP[0] ) // korábbi OP ot elrakjuk hátha minuszolni akarjuk a következő dolgot vagy valami
	{
		if( aOP[0].u4 )
		{
			if( str.n_load )
				pI = resISA_str( str );

			if( pI )
				resISA_stp( pI->stp(aOP[0]) );
			else if( pUD8 )
				resISA_stp( pUD8->stp(aOP[0]) );

			pI = pUD8 = NULL;
			aOP[0] = 0;

		}

		if( gpmbABC( *pS, gpaALFadd ) ) { /// ALF NUM -------------------------------------------------
			if( str.n_load )
				pI = resISA_str( str );

			if( pI || pUD8 )
			{
				if( pI ) // ne volt már operátor
					resISA_stp( pI->stp(aOP[1]) );
				else if( pUD8 )
				{
					switch( *pUD8->isa.aISA )
					{
						case gpeISA_d8:
							typ.u4 = gpeTYP_D;
							alu.equ(	this, xyWH.a4x2[0], //-sub,
										typ, aOP[1], 0, pUD8->an.d8 );
							break;
						case gpeISA_u8:
							typ.u4 = gpeTYP_U8;
							alu.equ( 	this, xyWH.a4x2[0], //-sub,
										 typ, aOP[1], pUD8->an.u8, 0.0 );
							break;
					}
					xyWH.x++;
					if( xyWH.z < xyWH.x )
						xyWH.z = xyWH.x;
				}
				pI = pUD8 = NULL;
			}

			if( !aOP[1].u4 )	/// ITT a TRG ------------------------------------------------------
				iTRG = resISA_trg( iTRG, xyWH.a4x2[0] );

			/// a stingől itt lesz ALF --------------
			an.num = gpfABCnincs( pS, pE, nUTF8, gpaALFadd );
			an = pS;
			pS += an.num;	// majd átlépjük

			if( bCROSS ) { /// "#"CROSS -------------------------------------------------
				bCROSS = false;
				if( !gpmbNUM( *pS ) )
				{
					// valami #teg
					pI = resISA_tag( an.alf );
					pUD8 = NULL;
					//d8 =
					aOP[0] = 0;
					continue;
				}
				// meg volt #-izélva,  mondjuk a #abcd1234-t (ABCD, 1234) számnak tárolja
				// az ötlet az hogy #abcd1234-t megtalálja a reALLOCátor és
				// ha abcd1234 el lett mozdítva frissíti ezt is

				typ.u4 = gpeTYP_U8;
				alu.equ( this, xyWH.a4x2[0], //-sub,
						 typ, aOP[1], an.x, 0.0 );
				xyWH.a4x2[0].x++;

				an.num = gpfSTR2U8( pS, &pS );
				alu.equ( this, xyWH.a4x2[0], //-sub,
						 typ, aOP[1], an.num, 0.0 );
				xyWH.a4x2[0].x++;
				if( xyWH.z < xyWH.x )
					xyWH.z = xyWH.x;

				pI = pUD8 = NULL;
				aOP[0] = 0;

				continue;
			}

			if( gpmbNUM( *pS ) ) {	/// NUM -------------------------------------------------
				an.num = gpfSTR2U8( pS, &pS );
				pI = resISA_an( an );
			} else {					/// VAR -------------------------------------------------
				( alu = an.alf ) = xyWH.a4x2[0];
				pI = resISA_var( alu.alf );
			}
		}
		else if( gpmbNUM( *pS ) || *pS == '.' ) { /// NUM -------------------------------------------------
			if( str.n_load )
				pI = resISA_str( str );

			if( pI || pUD8 )
			{
				if( pI ) // ne volt már operátor
					resISA_stp( pI->stp(aOP[1]) );
				else if( pUD8 )
				{
					switch( *pUD8->isa.aISA )
					{
						case gpeISA_d8:
							typ.u4 = gpeTYP_D;
							alu.equ( this, xyWH.a4x2[0], //-sub,
									 typ, aOP[1], 0, pUD8->an.d8 );
							break;
						case gpeISA_u8:
							typ.u4 = gpeTYP_U8;
							alu.equ( this, xyWH.a4x2[0], //-sub,
									 typ, aOP[1], pUD8->an.u8, 0.0 );
							break;
					}
					xyWH.x++;
					if( xyWH.z < xyWH.x )
						xyWH.z = xyWH.x;
				}
				pI = pUD8 = NULL;

			}

			pI = NULL;
			pUD8 = resISA();

			*pUD8->isa.aISA = gpeISA_u8;
			//pUD8->trg = xyWH.a4x2[0];
			pUD8->an.u8 = gpfSTR2U8( pS, &pS );

			if( *pS != '.' )
				pS += gpmNINCS( pS, " \t\r\n" );

			if( *pS == '.' )
			{
				*pUD8->isa.aISA = gpeISA_d8;
				pUD8->an.d8 = pUD8->an.u8;
				//typ.u4 = gpeTYP_D;
				pXe = pS;	// ha DOUBLE, ha nem észre vesszük, mert marad a pS == pXe
				pUD8->an.d8 += gpmSTR2D( pS );
				if( pS == pXe )
					pS++;

			}
			continue;
		}


		bCROSS = false;
		pS++;
		switch( pS[-1] ) {
			case '#':
				bCROSS = true;
				continue;
			case '.': {
					pI = resISA_stp( pS[-1] );
				} break;

			case ';': //{	// SOROK
			case ',': {	// vessző OSZLOPOK
					if( str.n_load )
						pI = resISA_str( str );

					if( pI )
						resISA_stp( pS[-1] );
					else if( pUD8 )
					{
						switch( *pUD8->isa.aISA )
						{
							case gpeISA_d8:
								typ.u4 = gpeTYP_D;
								alu.equ( this, xyWH.a4x2[0], //-sub,
										 typ, aOP[0], 0, pUD8->an.d8 );
								break;
							case gpeISA_u8:
								typ.u4 = gpeTYP_U8;
								alu.equ( this, xyWH.a4x2[0], //-sub,
										 typ, aOP[0], pUD8->an.u8, 0.0 );
								break;

						}
					}
					pI = pUD8 = NULL;
					aOP[0] = 0;

					if( pS[-1] == ',' )
					{
						xyWH.x++;

					} else {
						xyWH.x = 0;
						xyWH.y++;
						xyWH.w = xyWH.y;
					}
					/// ITT a TRG ------------------------------------------------------
					//iTRG = resISA_trg( iTRG, xyWH.a4x2[0] );
					if( xyWH.z >= xyWH.x )
						break;
					// bővíteni kell
					xyWH.z = xyWH.x;
				} break;


			/// ASSIGN & EQUALL ------------------------------------------------------------------------------
			case ':':
					{
						if( pI )
						if( pI > pISA )
						if( pI[-1].isa.x == gpeISA_an )
						{
							// mátrix
							//iI = pI-pISA;
							pI = resISA_stp( pS[-1] );
							continue;
						}
					}	// NINCS brake DIREKT!!!!
			case '=':{
					if( *pS == pS[-1] ) { // equal?
						// ez logikai op nem assign

						aOP[0].z |= 0x8;
						break;
					}

					nASG++;
					//xyWH.null();
					// /// ITT a TRG ------------------------------------------------------
					// pTRG = resISA_trg(pTRG,xyWH.a4x2[0]);
					resISA_stp( pS[-1] );
					pI = pUD8 = NULL;

				} break;
			/// ALU ------------------------------------------------------------------------------
			case '+': // öSSZEADáS
			case '-': // KIVONáS
				{
					aOP[0].x += (pS[-1] == '+') ? +1 : -1 ;
					for( pS += gpmNINCS( pS, " \t\r\n" ); pS < pE ? *pS : false; pS += gpmNINCS( pS, " \t\r\n" ) )
					{
						switch( *pS )
						{
							case '+':
								aOP[0].x++;
								pS++;
								continue;
							case '-':
								aOP[0].x--;
								pS++;
								continue;
						}
						break;
					}

				} break;

			case '*': { // SZORZÁS
					aOP[0].y++;
				} break;
			case '/': {
					switch( *pS ) {
						case '*':
							pS = (U1*)strstr( (char*)pS+1, "*/" );
							if( pS ? (pS >= pE) : true )
							{
								pS = pE;
								break;
							}

							pS += 2;
							break;

						case '/':
							pS += gpmVAN( pS, "\n", nUTF8 );
							break;
						default: // OSZTÁS hányados
							aOP[0].y--;
							break;
					}
				} break;

			case '%':{ // OSZTÁS maradék
					aOP[0].z |= 0x80;
				} break;

			/// LOGIC switch -------------------------------------
			case '!':{ // NOT
					aOP[0].z = (aOP[0].z & ~0x40) | (aOP[0].z&0x40 ? 0 : 0x40);
				} break;
			case '&':{	// AND
					aOP[0].z |= 0x20;
				} break;
			case '|':{	// OR
					aOP[0].z |= 0x10;
				} break;


			case '>':

				if( bMATH )
				{


					break;
				}

			case '}':
			case ')':
			case ']': {
						//pI = resISA_stp(pS[-1], xyWH.a4x2[0]);

						pE = pS;
					} break;
			case '<':

				if( bMATH )
				{


					break;
				}
				/// ezt be kell majd zavarni egy XML parserba
				pS += gpmNINCS( pS, " \t\r\n" );
				if( pS >= pE )
					continue;

				XML = gpfSTR2ALF( pS, pE, &pS );
				pXe = pS;
                while( pXe = (U1*)strstr( (char*)pXe, "</" ) )
                {
					pXe += 2;
					if( pXe >= pE )
					{
						pXe = NULL;
						break;
					}
					pXe += gpmNINCS( pXe, " \t\r\n" );
					if( XML != gpfSTR2ALF( pXe, pE, &pXe ) )
						continue;
                    pS = pXe+gpmVAN( pXe, ">", nUTF8 );
					break;
                }
                if( !pXe )
                {
					pE = pS;
					break;
                }
                if( *pS == '>' )
					pS++;


				break;

			case '(':
			case '{':
			case '[': {
					pI = resISA();
					U1 stp = pS[-1];
					pI->pRES = ((gpcRES*)NULL)->compiEASY( pS, pE, &pS, this );
					pI = resISA_stp(stp);


				} break;

			case '\"': {
					pSTR = pS;
					pS += gpmVAN( pS, "\"", nUTF8 );
					U8 s = -1;
					str.lzy_ins( pSTR, pS-pSTR, s, -1 );

					if( *pS == '\"' )
					{
						pS++;
						break;
					}
					pS = pE;
				} break;
			case '\'': {
					pS += gpmVAN( pS, "\'", nUTF8 );

					pUD8 = pI ? pI : resISA();
					pI = NULL;
					*pUD8->isa.aISA = gpeISA_u8;
					pUD8->an.u8 = pS[-1]; // ucsot beolvassa
					if( *pS == '\'' )
					{
						pS++;
						break;
					}
					pS = pE;
				} break;
		}
	}

	if( ppE )
		*ppE = pS;

	if( str.n_load )
		pI = resISA_str( str );

	if( pI )
		resISA_stp( pI->stp(';') );
	else if( pUD8 )
		resISA_stp( pUD8->stp(';') );

	pI = pUD8 = NULL;
	aOP[0] = 0;

	/*if( pI )
		resISA_stp( pS[-1] );
	else if( pUD8 )
	{
		switch( *pUD8->isa.aISA )
		{
			case gpeISA_d8:
				typ.u4 = gpeTYP_D;
				alu.equ( this, xyWH.a4x2[0], //-sub,
						 typ, aOP[1], 0, pUD8->an.d8 );
				break;
			case gpeISA_u8:
				typ.u4 = gpeTYP_U8;
				alu.equ( this, xyWH.a4x2[0], //-sub,
						 typ, aOP[1], pUD8->an.u8, 0.0 );
				break;

		}
		pUD8 = NULL;
	}*/

	gpmDEL( pTMP ) // nem lett sehove elrakva? Arroe KONYEC!

	return this;
}
