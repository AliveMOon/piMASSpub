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
	I1x4 op = (U4)0;

	bool bMATH = false;
	gpeALF XML = gpeALF_null;
	I4 xmlD = 0;


	gpcADR adr;
	gpcALU	aALU[8];

	//U4x2 aAN[8];

	I8x2 aAN[8];

	gpmZ(aALU);
	gpcLAZY str;
	U4  nAN = 0, nALU = 0, iI;
	gpcISA* pI = NULL, *pUD8;

	for( pS += gpmNINCS( pS, " \t\r\n" ); pS < pE ? *pS : false; pS += gpmNINCS( pS, " \t\r\n" ) )
	{
		if( gpmbABC( *pS, gpaALFadd ) ) { /// ALF NUM -------------------------------------------------
			if( pI || pUD8 )
			{
				if( pI ? op.u4 : false )
					pI = resISA_stp( pI->stp(op), xyWH.a4x2[0] );
				else if( pI ) {
					// ne volt már operátor
					resISA_stp( 0, xyWH.a4x2[0] );
					pI = NULL;
				}
				else if( pUD8 ? op.u4 : false )
					resISA_stp( pUD8->stp(op), xyWH.a4x2[0] );

				op.null();
			}

			nSTR = gpfABCnincs( pS, pE, nUTF8, gpaALFadd );
			aAN[nAN].A( pS, NULL );

			//apA[0] = pS;
			pS += nSTR;
			//u8 = 0;
			//d8 = 0.0;
			if( gpmbNUM( *pS ) ) {	/// NUM -------------------------------------------------
				aAN[nAN].num = gpfSTR2U8( pS, &pS );
				pI = resISA_an( aAN[nAN] );

			} else {					/// VAR -------------------------------------------------
				pI = resISA_var( aAN[nAN].alf );

			}
		}
		else if( gpmbNUM( *pS ) || *pS == '.' ) { /// NUM -------------------------------------------------

			if( pI || pUD8 )
			{
				if( pI ? op.u4 : false )
					pI = resISA_stp( pI->stp(op), xyWH.a4x2[0] );
				else if( pUD8 ? op.u4 : false )
					resISA_stp( pUD8->stp(op), xyWH.a4x2[0] );

				op.null();
			}
			pUD8 = resISA();
			pI = NULL;

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

		pS++;
		switch( pS[-1] ) {
			case '.': {
					pI = resISA_stp( pS[-1], xyWH.a4x2[0] );
				} break;

			case ';': //{	// SOROK
			case ',': {	// vessző OSZLOPOK
					if( pI )
						resISA_stp( pS[-1], xyWH.a4x2[0] );
					else if( pUD8 )
					{
						switch( *pUD8->isa.aISA )
						{
							case gpeISA_d8:
								typ.u4 = gpeTYP_D;
								aALU[0].equ( this, pUD8->trg, typ, op, 0, pUD8->an.d8 );
								break;
							case gpeISA_u8:
								typ.u4 = gpeTYP_U8;
								aALU[0].equ( this, pUD8->trg, typ, op, pUD8->an.u8, 0.0 );
								break;

						}
						pUD8 = NULL;
					}
					aALU[0].ins( this, str );

					pI = pUD8 = NULL;
					//d8 =
					op.u4 = 0;
					if( pS[-1] == ',' )
						xyWH.x++;
					else {
						xyWH.x = 0;
						xyWH.y++;
					}
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
							iI = pI-pISA;
							pI = resISA_stp( pS[-1], xyWH.a4x2[0] );
							continue;
						}
					}	// NINCS brake DIREKT!!!!
			case '=':{


					if( *pS == pS[-1] ) { // equal?
						// ez logikai op nem assign

						op.z |= 0x8;
						break;
					}


					xyWH.null();
					pI = resISA_stp( pS[-1], xyWH.a4x2[0] );


				} break;
			/// ALU ------------------------------------------------------------------------------
			case '+': // öSSZEADáS
			case '-': // KIVONáS
				{
					op.x += (pS[-1] == '+') ? +1 : -1 ;
					for( pS += gpmNINCS( pS, " \t\r\n" ); pS < pE ? *pS : false; pS += gpmNINCS( pS, " \t\r\n" ) )
					{
						switch( *pS )
						{
							case '+':
								op.x++;
								pS++;
								continue;
							case '-':
								op.x--;
								pS++;
								continue;
						}
						break;
					}

				} break;

			case '*': { // SZORZÁS
					op.y++;

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
							op.y--;
							break;
					}
				} break;

			case '%':{ // OSZTÁS maradék
					op.z |= 0x80;
				} break;

			/// LOGIC switch -------------------------------------
			case '!':{ // NOT
					op.z = (op.z & ~0x40) | (op.z&0x40 ? 0 : 0x40);
				} break;
			case '&':{	// AND
					op.z |= 0x20;
				} break;
			case '|':{	// OR
					op.z |= 0x10;
				} break;


			case '>':

				if( bMATH )
				{


					break;
				}

			case '}':
			case ')':
			case ']': {
						pI = resISA_stp(pS[-1], xyWH.a4x2[0]);

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
					pI = resISA_stp(stp, xyWH.a4x2[0]);


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

	gpmDEL( pTMP ) // nem lett sehove elrakva? Arroe KONYEC!

	return this;
}
