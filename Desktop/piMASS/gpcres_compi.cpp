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

	//bool bMATH = false;
	gpeALF XML = gpeALF_null;
	I4 xmlD = 0;


	gpcADR adr;
	gpcALU	aALU[8];
	I8x2 aAN[8];

	gpmZ(aALU);
	gpcLAZY str;
	U4  nAN = 0, nALU = 0;
	gpcISA* pI = NULL;

	for( pS += gpmNINCS( pS, " \t\r\n" ); pS < pE ? *pS : false; pS += gpmNINCS( pS, " \t\r\n" ) )
	{
		if( gpmbABC( *pS, gpaALFadd ) ) { /// ALF NUM -------------------------------------------------

			aAN[nAN].num = gpfABCnincs( pS, pE, nUTF8, gpaALFadd );
			aAN[nAN].A( pS, NULL );

			//apA[0] = pS;
			pS += aAN[nAN].num;
			u8 = 0;
			d8 = 0.0;
			if( gpmbNUM( *pS ) ) {	/// NUM -------------------------------------------------
				aAN[nAN].num = gpfSTR2U8( pS, &pS );
				pI = pISAan( aAN[nAN] );
			} else					/// VAR -------------------------------------------------
				pI = pISAvar( aAN[nAN].alf );

		}
		else if( gpmbNUM( *pS ) ) { /// NUM -------------------------------------------------
			pI = NULL;
			typ.u4 = gpeTYP_U8;
			u8 = gpfSTR2U8( pS, &pS );
			d8 = 0.0;
		}

		pS++;
		switch( pS[-1] ) {
			case '.': {
					if( pI )
					switch( pI->isa.aISA[0] )
					{
						case gpeISA_nop:
							break;
						default:
							pI = pISA_stp( pS[-1] );
							continue;
					}

                    pS--; 		// visszaléptetjük úgy is vissza MáSZIK a gpmSTR2D-val;
					pXe = pS;	// ha DOUBLE, ha nem észre vesszük, mert marad a pS == pXe
					d8 = gpmSTR2D( pS );
					if( pS == pXe)
					{
						// hát ez nem nyert
						// ez csak egy pont
						pS++;
						break;
					}
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
			case '|':{	// OR
					op.z |= 0x20;
				} break;
			case '&':{	// AND
					op.z |= 0x10;
				} break;
			case ':':
			case '=':{

					if( *pS == '=' ) { // equal?
						// ez logikai op nem assign

						op.z |= 0x8;
						break;
					}

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
						aALU[0] = adr.ALU( this );
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
					aALU[0].zero();

				} break;

			case ';': //{	// SOROK
			case ',': {	// vessző OSZLOPOK

					if( str.n_load)
					{
						//gpmDEL(pTMP);
						pTMP = aALU[0].ins( this, pTMP, str );


						// csak akkor kapunk valamit visza ha azt elrakta vagy lecserélte
						pTMP = NULL;
						str.lzy_reset();
					}
					else if( pTMP )
					{
						pTMP = aALU[0].ins( this, pTMP );


						pTMP = NULL;
					} else {
						aALU[0].equ( this, xyWH.a4x2[0], typ, op, u8, d8 );
					}


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




			case '>':

				/*if( bMATH )
				{


					break;
				}*/

			case '}':
			case ')':
			case ']': {

						if( str.n_load)
						{
							//gpmDEL(pTMP);
							pTMP = aALU[0].ins( this, pTMP, str );


							// csak akkor kapunk valamit visza ha azt elrakta vagy lecserélte
							pTMP =  NULL;
							str.lzy_reset();
						}
						else if( pTMP )
						{
							pTMP = aALU[0].ins( this, pTMP );


							pTMP = NULL;
						} else {
							aALU[0].equ( this, xyWH.a4x2[0], typ, op, u8, d8 );
						}


						pE = pS;

					} break;
			case '<':

				/*if( bMATH )
				{


					break;
				}*/
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
					if( pI )
					switch( pI->isa.aISA[0] )
					{
						case gpeISA_nop:
							break;
						default:
							pI = pISA_stp( pS[-1] );
							continue;
					}

					pTMP = ((gpcRES*)NULL)->compiEASY( pS, pE, &pS, this );


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
