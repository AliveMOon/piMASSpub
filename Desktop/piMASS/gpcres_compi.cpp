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
		*apA[2], *apN[2],
		*pSTR = pS, *pXe;

	gpmZ(apA);
	gpmZ(apN);
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
	gpcALU	aA[8];
	gpmZ(aA);

	for( pS += gpmNINCS( pS, " \t\r\n" ); pS < pE ? *pS : false; pS += gpmNINCS( pS, " \t\r\n" ) )
	{
		if( gpmbABC( *pS, gpaALFadd ) )
		{
			if( apA[0] )
			{
				apA[1] = apA[0];
				lab.w = lab.y;
			}
			lab.a8x2[0].num = gpfABCnincs( pS, pE, nUTF8, gpaALFadd );

			apN[0] = NULL;
			apA[0] = pS;
			u8 = 0;
			pS += lab.a8x2[0].num;
		}

		if( gpmbNUM( *pS ) )
		{
			if( apA[0] )
			{
				// az ötlet lényege, hogy nem lesz a apA[0] -ban szöveg ha AN
				// viszont ha nem volt utánna szám  akkor a típus az apA[0], a label meg az apA[1] ben lesz
				apA[1] = apA[0];
				apA[0] = NULL;
			}
			apN[0] = pS;
			typ.u4 = gpeTYP_U8;
			u8 = gpfSTR2U8( pS, &pS );
		}

		pS++;
		switch( pS[-1] ) {
			case '.': {
					lab.AB( apA[0], apA[1], apA, apA+1 );

                    if( apA[0] )
                    {
						// már megvan lab.AB() //lab.a8x2[0].alf = gpfSTR2ALF( apA[0], apA[0] + min( 14, lab.y ), NULL );
						if( apA[1] )
						{
							// ó kető van egymás után // ez valami típus?
							// már megvan lab.aa() //lab.a8x2[1].alf = gpfSTR2ALF( apA[1], apA[1] + min( 14, lab.w ), NULL );

						}
						// ez csak egy kulcsszó megpontozva

						break;
                    }

                    if( apA[1] && apN[0] )
                    {
						// ez egy AN megpontozva
						// már megvan lab.aa() //lab.a8x2[1].alf = gpfSTR2ALF( apA[1], apA[1] + min( 14, lab.w ), NULL );

						break;
                    }

                    //ez egy float szám ?
                    pS--; // visszaléptetjük úgy is vissza mászik a gpmSTR2D-val;
                    pXe = pS;
					d8 = gpmSTR2D( pS );
					if( pS == pXe)
					{
						// hát ez nem nyert
						// ez csak egy pont
						pS++;
						break;
					}
				} break;
			case '+':{

					lab.AB( apA[0], apA[1], apA, apA+1 );



					op.x++;
				} break;

			case '-':{
					lab.AB( apA[0], apA[1], apA, apA+1 );

					op.x--;
				} break;


			case '*':{
					lab.AB( apA[0], apA[1], apA, apA+1 );

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
						default:
							lab.AB( apA[0], apA[1], apA, apA+1 );

							op.y--;
							break;
					}
				} break;

			case '%':{ // OSZTÁS maradék
					lab.AB( apA[0], apA[1], apA, apA+1 );

					op.z |= 0x80;
				} break;

			/// LOGIC switch -------------------------------------
			case '!':{ // NOT
					lab.AB( apA[0], apA[1], apA, apA+1 );

					op.z = (op.z & ~0x40) | (op.z&0x40 ? 0 : 0x40);
				} break;
			case '|':{	// OR
					lab.AB( apA[0], apA[1], apA, apA+1 );

					op.z |= 0x20;
				} break;
			case '&':{	// AND
					lab.AB( apA[0], apA[1], apA, apA+1 );

					op.z |= 0x10;
				} break;

			case '=':{
					if( apA[0] )
					{
						// valami turpisság készül
						lab.a8x2[0].A( apA[0], apA );
						if( apA[1] )
						{
							lab.a8x2[1].A( apA[1], apA+1 );

						}
					} else {
						if( apA[1] )
						{
							lab.a8x2[1].A( apA[1], apA+1 );
							// AN A0:A1.A0 stb
						}
					}

					if( *pS == '=' ) { // equal?
						// ez logikai op nem assign

						op.z |= 0x8;
						break;
					}

					/// = ASSIGN = -------------------------------------
					/// egyenlőségjel reseteli az xyWH-t
					xyWH.null(); /// Nesze! Most már null!
				    adr = lab.a8x2[0].alf;
					adr = this;

					if( !adr.an.alf ) // nem lett megadva cél
					while( adr.pRM )	// ez azért, hogy hatékonyabb legyen a keresés, a binFA ne egyetlen jobb ág legyen
                    {
						// nem talált ilyen változót
						adr = (gpeALF)( 1 + U4x2(0).cnt2fract( (U4)gpeALF_AAAAAA, nLAB ) * U4x2( 1, (U4)gpeALF_AAAAAA) );
                        adr = this; // ezzel indítjuk a keresést
                        nLAB++;
                    }

					aA[0] = adr.ALU( this );
					aA[0].zero();
					/*break;

                    // x[7s,6f,5r,4p? : 3-0 nBYTE = 1<<(x&0xf) ]
					if( typ.x&0x40) // lebeg
						pM->equ( lab.a8x2[0].alf, op.u4, typ.u4, d8 );
					else
						pM->equ( lab.a8x2[0].alf, op.u4, typ.u4, lab.y );
					// új változó*/
				} break;

			case ':': {
					// ez valami név?
					if( apA[0] )
					{
						// valami turpisság készül
						lab.a8x2[0].A( apA[0], apA );
						if( apA[1] )
						{
							lab.a8x2[1].A( apA[1], apA+1 );


						}

						op.z |= 0x8;	// nevet vagy típust is meghatároz


					}


				} break;

			case ';': //{	// SOROK
			case ',': {	// vessző OSZLOPOK
					if( pTMP )
					{
						pTMP = aA[0].ins( this, pTMP );
					} else {
						if( apA[0] )
						{
							// valami turpisság készül
							lab.a8x2[0].A( apA[0], apA );
							if( apA[1] )
							{
								lab.a8x2[1].A( apA[1], apA+1 );

							}
							if( aA[0].alf != lab.a8x2[0].alf )
							{
								adr = lab.a8x2[0].alf;
								adr = this;
							}

						} else {
							if( apA[1] )
							{
								lab.a8x2[1].A( apA[1], apA+1 );
								// AN A0:A1.A0 stb
							}
						}

						aA[0].equ( this, xyWH.a4x2[0], typ, op, u8, d8 );
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
			/*case ';': {	// SOROK
					lab.AB( apA[0], apA[1], apA, apA+1 );
					bMATH = false;

					op.u4 = 0;
					xyWH.x = 0;
					xyWH.y++;
				} break;*/



			case '>':
				lab.AB( apA[0], apA[1], apA, apA+1 );

				if( bMATH )
				{


					break;
				}

			case '}':
			case ')':
			case ']': {
						pE = pS;
						break;
					}
			case '<':
				lab.AB( apA[0], apA[1], apA, apA+1 );

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
					lab.AB( apA[0], apA[1], apA, apA+1 );
					pTMP = ((gpcRES*)NULL)->compiEASY( pS, pE, &pS, this );


				} break;




			case '\"': {


					pSTR = pS;
					pS += gpmVAN( pS, "\"", nUTF8 );
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
