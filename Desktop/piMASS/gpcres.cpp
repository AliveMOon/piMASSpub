
#include "gpcres.h"
extern U1 gpaALFadd[];


gpcALU::gpcALU( gpcRES* pM )
{
	gpmCLR;
	pMOM = pM;
}
gpcALU::~gpcALU()
{
	null();
}

gpcRES& gpcRES::null()
{
	gpcRES* pR;
	if( ppDAT )
	{
		for( U4 a = 0; a < n; a++ )
		{
			if( !ppDAT[a] )
				continue;
			if( !(pTYP[a].x&0x2) )
			{
				gpmDELary(ppDAT[a]);
				continue;
			}
			pR = (gpcRES*)(ppDAT[a]);
			pR->null();
			gpmDEL(pR);
		}
		delete[] ppDAT;
		ppDAT = NULL;
	}
	gpmDELary( pLAB );
	gpmDELary( pOP );
	gpmDELary( pTYP );
	gpmDELary( pAN );
	gpmDELary( pTREE );
	gpmDELary( pTx );
	gpmDELary( pALU );
}
I8x2& I8x2::A( U1* pA, U1** ppA )
{
	alf = gpfSTR2ALF( pA, pA + min( 14, y ), NULL );
	if( !ppA )
		return *this;

	*ppA = NULL;
	return *this;
}

I8x4& I8x4::AB( U1* pA, U1* pB, U1** ppA, U1** ppB )
{
	a8x2[0].A( pA, ppA );
	a8x2[1].A( pB, ppB );
	return *this;
}
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

			apN[0] =
			NULL;
			apA[0] = pS;
			u8 = 0;
			pS += lab.a8x2[0].num;
		}

		if( gpmbNUM( *pS ) )
		{
			if( apA[0] )
			{
				// az ötlet lényege, hogy nem lesz a apA[0] -ban szöveg ha AN
				// viszont ha nem volt utánna szám  akkor a típus az apA[0] a label meg az apA[1] ben lesz
				apA[1] = apA[0];
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
						// már megvan lab.aa() //lab.a8x2[0].alf = gpfSTR2ALF( apA[0], apA[0] + min( 14, lab.y ), NULL );
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
					d8 = gpmSTR2D( pS ) + u8;
					if( pS == pXe)
					{
						// hát ez nem nyert
						// ez csak egy pont
						pS++;
						break;
					}
					typ.u4 = gpeTYP_D;
					if( op.x < 0 )
					{
						// volt néhány minusz elötte
						d8 *= -1.0;
						op.x++;
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

			case ' = ':{
					lab.AB( apA[0], apA[1], apA, apA+1 );
					if( *pS == '=' ) { // equal?
						// ez logikai op nem assign

						op.z |= 0x8;
						break;
					}

					/// egyenlőségjel resetelei az xyWH-t
					xyWH.null(); // Nesze! Most már null!
				    //if( deep )
					//	break;



					U4 i_fnd = iFND( lab.labe );
					gpcRES* pM = this;
					while( i_fnd >= pM->nFND() )
					{
						pM = pM->pMOM;
						i_fnd = pM->iFND( lab.labe );
					}

					while( !lab.labe )	// ez azért, hogy hatékonyabb legyen a keresés, a binFA ne egyetlen jobb ág legyen
                    {
						lab.x = ( 1 + U4x2(0).cnt2fract( (U4)gpeALF_AAAAAA, nLAB ) * U4x2( 1, (U4)gpeALF_AAAAAA) );
                        i_fnd = iFND( lab.labe );
                        if( i_fnd < nFND() )
							lab.x = gpeALF_null;
                    }

                    // x[7s,6f,5r,4p? : 3-0 nBYTE = 1<<(x&0xf) ]
					if( typ.x&0x40) // lebeg
						pM->equ( lab.labe, typ.u4, d8 );
					else
						pM->equ( lab.labe, typ.u4, lab.y );
					// új változó
				} break;



			case ',': {	// vessző OSZLOPOK
					lab.AB( apA[0], apA[1], apA, apA+1 );


					op.u4 = 0;
					xyWH.x++;
					if( xyWH.z >= xyWH.x )
						break;
					// bővíteni kell
					xyWH.z = xyWH.x;
				} break;
			case ';': {	// SOROK
					lab.AB( apA[0], apA[1], apA, apA+1 );
					bMATH = false;

					op.u4 = 0;
					xyWH.x = 0;
					xyWH.y++;
				} break;



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
					pTMP = compiEASY( pS, pE, &pS, this );


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


gpcREStrs* gpcREStrs::REScompiAN( U1* pS, U1* pE, U4* pMAP, gpcLZYdct* pDICT )
{
	if( !this )
	{
		return new gpcREStrs( pS, pE );
	}
	null();

	U4x4 xyWH = 0;
	I4x4 nOP = 0;
	U4 deep = 0;
	U1* pU = pS, *pB, d;
	U8 nLEN;
	*pMAP = 0;
	U4 nM = 1;
	for( pS += gpmNINCS( pS, " \t\r\n" ); pS < pE ? *pS : false; pS += gpmNINCS( pS, " \t\r\n" ) )
	{
		pS++;
		switch( pS[-1] ) {
			case ',':	// vessző OSZLOPOK
				if( deep )
					break;

				xyWH.x++;
				if( xyWH.z >= xyWH.x )
					break;
				// bővíteni kell
				xyWH.z = xyWH.x;
				break;

			case ';':	// SOROK
				if( deep )
					break;

				xyWH.x = 0;
				xyWH.y++;
				break;

			case ')': {
						if( !d )
						{
							pE = pS;
							break;
						}
						if( d != '(' )
							break;

						if( deep )
						{
							deep--;
							if( deep )
								break;

							d = 0;
						} else
							pE = pS;
						break;
					}

			case '}': {
						if( !d )
						{
							pE = pS;
							break;
						}

						if( d != '{' )
							break;

						if( deep )
						{
							deep--;
							if( deep )
								break;

							d = 0;
						} else
							pE = pS;
						break;
					}

			case ']': {
						if( !d )
						{
							pE = pS;
							break;
						}

						if( d != ']' )
							break;

						if( deep )
						{
							deep--;
							if( deep )
								break;

							d = 0;
						} else
							pE = pS;
						break;
					}

			case '(':
			case '{':
			case '[':
				if( !d )
				{
					d = pS[-1];
					deep = 1;
					break;
				}

				if( d == pS[-1] )
						deep++;

				break;

			case '/':
				switch( *pS )
				{
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
						pS += gpmVAN( pS, "\n", nLEN );
						break;
				}
				break;
			case '\"':
				pS += gpmVAN( pS, "\"", nLEN );
				if( *pS == '\"' )
				{
					pS++;
					break;
				}
				pS = pE;
				break;
			case '\'':
				pS += gpmVAN( pS, "\'", nLEN );
				if( *pS == '\'' )
				{
					pS++;
					break;
				}
				pS = pE;
				break;
		}
	}

	if( pE > pS )
		pE = pS;

	pS = pU;

    xyWH.a4x2[1] += 1;

    a = xyWH.a4x2[1].x;
    n = xyWH.a4x2[1].y;
	an = a*n;

	xyWH.a4x2[0] = 0;

	typ = gpcNET4_NULL;

	bool bABC;
	U1 c, *pBEGIN;
	U8 nNINCS, nVAN, u8;
	I8 i8;
	double d8;

	for( pS += gpmNINCS( pS, " \t\r\n" ); pS < pE ? *pS : false; pS += gpmNINCS( pS, " \t\r\n" ) )
	{
		c = *pS;
		pS++;

		switch( c )
		{


			case ')': {
						if( !d )
						{
							pE = pS;
							break;
						}
						if( d != '(' )
							break;

						if( deep )
						{
							deep--;
							if( deep )
								break;


							pBEGIN = NULL;
							d = 0;
							nOP = 0;
						} else
							pE = pS;
						break;
					}

			case '}': {
						if( !d )
						{
							pE = pS;
							break;
						}

						if( d != '{' )
							break;

						if( deep )
						{
							deep--;
							if( deep )
								break;

							pBEGIN = NULL;
							d = 0;
							nOP = 0;
						} else
							pE = pS;
						break;
					}

			case ']': {
						if( !d )
						{
							pE = pS;
							break;
						}

						if( d != '[' )
							break;

						if( deep )
						{
							deep--;
							if( deep )
								break;


							pBEGIN = NULL;
							d = 0;
							nOP = 0;
						} else
							pE = pS;
						break;
					}

			case '(':
			case '{':
			case '[':
				if( !d )
				{
					pBEGIN = pS;
					d = c;
					deep = 1;
					break;
				}

				if( d == c )
						deep++;

				break;


			case '\"':
				pS += gpmVAN( pS, "\"", nLEN );
				if( *pS == '\"' )
				{
					pS++;
					break;
				}
				pS = pE;
				break;
			case '\'':
				pS += gpmVAN( pS, "\'", nLEN );
				if( *pS == '\'' )
				{
					pS++;
					break;
				}
				pS = pE;
				break;


			case ',':	// vessző OSZLOPOK
				if( deep )
					break;

				nOP = 0;
				xyWH.x++;
				if( xyWH.z >= xyWH.x )
					break;
				// bővíteni kell
				xyWH.z = xyWH.x;
				break;

			case ';':	// SOROK
				if( deep )
					break;

				nOP = 0;
				xyWH.x = 0;
				xyWH.y++;
				break;

			case '+':
				if( !deep )
					 nOP.x++;
				break;
			case '-':
				if( !deep )
					 nOP.x--;
				break;

			case '/':
				switch( *pS )
				{
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
						pS += gpmVAN( pS, "\n", nLEN );
						break;
				}
				break;

			default:
				if( deep )
					break;

				pB = pS-1;
				u8 = gpfSTR2U8( pS-1, &pS );
				if( *pS == '.' )
				{
					d8 = gpmSTR2D( pS ) + u8;
					pS += gpmNINCS( pS, ".0123456789fF" );

                    equAN( xyWH.a4x2[0], nOP.x < 0 ? -d8 : d8 );
					break;
				}

				if( pB == pS )
				{
					// visza lépet mert egyetlen számot sem talált

					// és nem is azért mert '.' azaz pöst
					// ? nem valós számot szeretett volna a felhasználó
					pS++;
					break;
				}

				if( nOP.x > -1 )
				{
					equAN( xyWH.a4x2[0], u8 );
					break;
				}

				i8 = u8;
				equAN( xyWH.a4x2[0], -i8 );
				break;

		}

		continue;

		/// off ----------------------

		if( bABC = gpmbABC( pS[-1], gpaALFadd) ) {

		}



	}


	return this;
}

gpcREStrs::gpcREStrs(  U1* pS, U1* pE )
{
	gpmCLR;
	if( pE > pS ? !*pS : true )
	{
		return;
	}

	REScompiAN( pS, pE );

}


gpcREStrs::gpcREStrs()
{
	gpmCLR;
}

gpcREStrs::~gpcREStrs()
{
	null();
}

gpcREStrs& gpcREStrs::operator = ( const gpcREStrs& b )
{
	if (this == &b )
		return *this; // handle self assignment

	if( !b.typ )
		return null();

	memcpy( this, &b, sizeof(b) );
	if( !a )
		return *this;

	switch( typ )
	{
		case gpeNET4_MAS:
			return null();

			/// ki van kapcsolva
			/// MASS
			if( an < 2 )
			{
				pDAT = (U1*)new gpcMASS( *((gpcMASS*)b.pDAT) );
				return *this;
			}

			pDAT = (U1*)new gpcMASS[an];
			for( gpcMASS* pR = (gpcMASS*)pDAT, *pRe = pR+an, *pS = (gpcMASS*)b.pDAT; pR < pRe; pR++, pS++ )
				*pR = *pS;

			return *this;

		case gpeNET4_RES:
			if( an < 2 )
			{
				pDAT = (U1*)new gpcREStrs( *((gpcREStrs*)b.pDAT) );
				return *this;
			}

			pDAT = (U1*)new gpcREStrs[an];
			for( gpcREStrs* pR = (gpcREStrs*)pDAT, *pRe = pR+an, *pS = (gpcREStrs*)b.pDAT; pR < pRe; pR++, pS++ )
				*pR = *pS;

			return *this;

		default:
			if(	!*aT )
				return null();

			pDAT = new U1[*aT * an];
			memcpy( pDAT, b.pDAT, *aT * an );
			return *this;
	}

	return *this;
}

/*gpcREStrs& gpcREStrs::operator = ( const gpcREStrs& b )
{
	if (this == &b ) return *this; // handle self assignment

	*this = b;
	return *this;
}*/
