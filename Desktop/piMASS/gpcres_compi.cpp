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

	U8 nUTF8, nLAB = 0, u8 = 0;
	double d8 = 0;

	I8x4 lab = 0;
	U1x4 typ = 0;
	I1x4 op = (U4)0;

	bool bMATH = false;
	gpeALF XML = gpeALF_null;
	I4 xmlD = 0;


	gpcADR adr;
	gpcALU	aALU[8];
	I8x2 aAN[8];

	gpmZ(aALU);
	gpcLAZY str;
	U4  nAN = 0, nALU = 0, iI;
	gpcISA* pI = NULL;

	for( pS += gpmNINCS( pS, " \t\r\n" ); pS < pE ? *pS : false; pS += gpmNINCS( pS, " \t\r\n" ) )
	{
		if( gpmbABC( *pS, gpaALFadd ) ) { /// ALF NUM -------------------------------------------------

			if( pI ? op.u4 : false )
				pI = resISA_stp( pI->stp(op), xyWH.a4x2[0] );

			op.null();

			u8 = gpfABCnincs( pS, pE, nUTF8, gpaALFadd );
			aAN[nAN].A( pS, NULL );

			//apA[0] = pS;
			pS += u8;
			u8 = 0;
			d8 = 0.0;
			if( gpmbNUM( *pS ) ) {	/// NUM -------------------------------------------------
				aAN[nAN].num = gpfSTR2U8( pS, &pS );
				pI = resISA_an( aAN[nAN] );

			} else {					/// VAR -------------------------------------------------
				pI = resISA_var( aAN[nAN].alf );

			}
		}
		else if( gpmbNUM( *pS ) || *pS == '.' ) { /// NUM -------------------------------------------------

			if( pI ? op.u4 : false )
				resISA_stp( pI->stp(op), xyWH.a4x2[0] );

			op.null();
			pI = NULL;

			d8 = 0.0;
			if( *pS == '.' )
			{
				u8 = 0;
			} else {
				typ.u4 = gpeTYP_U8;
				u8 = gpfSTR2U8( pS, &pS );
				pS += gpmNINCS( pS, " \t\r\n" );
			}

			if( *pS == '.' )
			{
				typ.u4 = gpeTYP_D;
				pXe = pS;	// ha DOUBLE, ha nem észre vesszük, mert marad a pS == pXe
				d8 = gpmSTR2D( pS );
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

					aALU[0].equ( this, xyWH.a4x2[0], typ, op, u8, d8 );
					aALU[0].ins( this, str );

					pI = resISA_stp( pS[-1], xyWH.a4x2[0] );


					d8 = op.u4 = 0;
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
					/*

					/// = ASSIGN = -------------------------------------
					/// egyenlőségjel reseteli az xyWH-t

				    adr = pI ? pI->an.alf : gpeALF_null;
					adr = this;

					if( pI )
					switch( pI->isa.aISA[0] )
					{
						case gpeISA_nop:
							break;
						default:
							pI = pISA_stp( pS[-1] );
							break;
					}

					xyWH.null(); /// Nesze! Most már null!
					if( adr.pRM )
					{
						aALU[0] = adr.adr2ALU( this );
						aALU[0].zero();
						break;
					}

					if( !adr.an.alf ) // nem lett megadva cél
					while( adr.pRM )	// ez azért, hogy hatékonyabb legyen a keresés, a binFA ne egyetlen jobb ág legyen
                    {
						// nem talált ilyen változót
						adr = (gpeALF)( 1 + U4x2(0).cnt2fract( (U4)gpeALF_AAAAAA, nLAB ) * U4x2( 1, (U4)gpeALF_AAAAAA) );
                        adr = this; // ezzel indítjuk a keresést
                        nLAB++;
                    }

					aALU[0] = adr.ALU( this );
					aALU[0].zero();*/

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
					pI = resISA_stp(pS[-1], xyWH.a4x2[0]);
					pI->pRES = ((gpcRES*)NULL)->compiEASY( pS, pE, &pS, this );


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
					u8 = pS[-1]; // ucsot beolvassa
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
