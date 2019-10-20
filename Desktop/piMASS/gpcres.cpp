
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
	//gpmDELary( pALU );
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

gpcADR& gpcADR::operator = ( gpcRES* pM )
{
	pRM = pM;
	if( an.alf ? !pM : true )
	{
		//ix = 0;
		//pRM = NULL;
		return *this = gpeALF_null;
	}

	n = ix = pM->nFND();
	while( ix >= n )
	{
		pRM = pRM->pRM();
		if( !pRM )
		{
			ix = 0;
			return *this;
		}

		n = pRM->nFND();
		ix = pRM->iFND( an.alf );
		dp = pM->iL() - pRM->iL();
	}
	return *this;
}
gpcALU& gpcADR::ALU( gpcRES* pM )
{
	gpcALU alu( NULL );
	if( !pRM )
	{
		pRM = pM;
		if( !pRM )
			return alu;

		ix = pRM->nFND();
	}

	return pRM->ALU( ix );
}
gpcALU& gpcALU::equ( gpcRES* pM, U4x2 xy, U1x4 ty4, I1x4 op4, U8 u8, U2 dm )
{
	if( !pM )
		return null();

	gpcADR adr = alf;
	if( !alf )
	{
        U8 nCNT = 1;
		adr.an.alf = gpeALF_A;
		adr = pM;
		while( adr.pRM )	// ez azért, hogy hatékonyabb legyen a keresés, a binFA ne egyetlen jobb ág legyen
		{
			// nem talált ilyen változót
			adr = (gpeALF)( 1 + U4x2(0).cnt2fract( (U4)gpeALF_AAAAAA, nCNT ) * U4x2( 1, (U4)gpeALF_AAAAAA) );
			adr = pM; // ezzel indítjuk a keresést
			nCNT++;
		}
		*this = pM->ADD( adr.an.alf, ty4.u4, op4.u4 );
 	} else
		adr = pM;


	/// ha nem megfelelő typus át kell küldözgetni
	/// a megfelelő típusban dolgozza fel
	// U1x4 typ:
	// x[7s,6f,5r,4p? 	: 3-0 nBYTE = 1<<(x&0xf) ]
	// yz[ dimXY ] 		, w[] = nBYTE*dimXY
	if( typ.x&0x40 || (u8&0x8000000000000000) )
	{
		// lebegőpontos volt vagy rohadt nagy, átküldjük double-ba
		double d8 = u8;
		if( op4.y < 0 )
		{
			d8 *= -1;
			op4.y = 0;
		}
		return equ( pM, xy, ty4, op4, d8 );
	}
	if( typ.x&0x80 )
	{
		// signed azaz előjeles
		I8 i8 = u8;
		if( op4.y < 0 )
		{
			i8 *= -1;
			op4.y = 0;
		}
		return equ( pM, xy, ty4, op4, i8 );
	}


	gpcALU tmp;
	// AN
	tmp.AN.a4x2[0] = AN.a4x2[0];
	tmp.AN.a4x2[0].mx( xy );
	tmp.typ = ty4;
	tmp.pDAT = pDAT;

    U4	nL0 = nLOAD(),
		nL1 = tmp.nLOAD();

	if( !pDAT )
	{
		pDAT = new U1[nL1];
		gpmZn( (U1*)pDAT, nL1 );
	}
	if( (nL1 == nL0) && (ty4.u4 == typ.u4) )
	{

		// hát ez tök egyforma
		(((U8*)pDAT)+(xy*U4x2(1,tmp.AN.x)))[dm%tmp.nDIM()] = u8 ;
		return *this;
	}





	return *this;
}

gpcALU& gpcALU::equ( gpcRES* pM, U4x2 xy, U1x4 ty4, I1x4 op4, I8 i8, U2 dm )
{
	if( !pM )
		return null();

	gpcADR adr = alf;
	if( !alf )
	{
        U8 nCNT = 1;
		adr.an.alf = gpeALF_A;
		adr = pM;
		while( adr.pRM )	// ez azért, hogy hatékonyabb legyen a keresés, a binFA ne egyetlen jobb ág legyen
		{
			// nem talált ilyen változót
			adr = (gpeALF)( 1 + U4x2(0).cnt2fract( (U4)gpeALF_AAAAAA, nCNT ) * U4x2( 1, (U4)gpeALF_AAAAAA) );
			adr = pM; // ezzel indítjuk a keresést
			nCNT++;
		}
		*this = pM->ADD( adr.an.alf, ty4.u4, op4.u4 );
 	} else
		adr = pM;


	/// ha nem megfelelő typus át kell küldözgetni
	/// a megfelelő typus dolgozza fel
	// typ:
	// x[7s,6f,5r,4p? 	: 3-0 nBYTE = 1<<(x&0xf) ]
	// yz[ dimXY ] 		, w[] = nBYTE*dimXY
	if( typ.x&0x40 )
	{
		// lebegőpontos volt vagy rohadt nagy, átküldjük double-ba
		double d8 = i8;
		if( op4.y < 0 )
		{
			d8 *= -1;
			op4.y = 0;
		}
		return equ( pM, xy, ty4, op4, d8 );
	}
	if( !(typ.x&0x80) )
	{
		if( op4.y < 0 )
		{
			i8 *= -1;
			op4.y = 0;
		}
		return equ( pM, xy, ty4, op4, i8 );
	}


	gpcALU tmp;
	tmp.typ = ty4;
	tmp.AN.a4x2[0] = AN.a4x2[0];
	tmp.pDAT = pDAT;

	if( AN.a4x2[0].x <= xy.x )
		tmp.AN.a4x2[0].x = xy.x+1;
	if( AN.a4x2[0].y <= xy.y )
		tmp.AN.a4x2[0].y = xy.y+1;

    U4	nL0 = nLOAD(),
		nL1 = tmp.nLOAD();

	if( !pDAT )
	{
		pDAT = new U1[nL1];
		gpmZn( (U1*)pDAT, nL1 );
	}
	if( (nL1 == nL0) && (ty4.u4 == typ.u4) )
	{

		// hát ez tök egyforma
		(((I8*)pDAT)+(xy*U4x2(1,tmp.AN.x)))[dm%tmp.nDIM()] = i8 ;
		return *this;
	}



	return *this;
}
gpcALU& gpcALU::equ( gpcRES* pM, U4x2 xy, U1x4 ty4, I1x4 op4, double d8, U2 dm )
{
	if( !pM )
		return null();

	if( !alf )
	{
        gpcADR adr;
        U8 nCNT = 1;
		adr.an.alf = gpeALF_A;
		adr = pM;
		while( adr.pRM )	// ez azért, hogy hatékonyabb legyen a keresés, a binFA ne egyetlen jobb ág legyen
		{
			// nem talált ilyen változót
			adr = (gpeALF)( 1 + U4x2(0).cnt2fract( (U4)gpeALF_AAAAAA, nCNT ) * U4x2( 1, (U4)gpeALF_AAAAAA) );
			adr = pM; // ezzel indítjuk a keresést
			nCNT++;
		}
		*this = pM->ADD( adr.an.alf, ty4.u4, op4.u4 );
 	}


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
				// viszont ha nem volt utánna szám  akkor a típus az apA[0], a label meg az apA[1] ben lesz
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
				    //if( deep )
					//	break;
					adr = lab.a8x2[0].alf;
					adr = this;

					//U4 iFND = iFNDadr( lab.a8x2[0].alf, &apMOM[0] );
					if( !adr.an.alf )
					while( adr.pRM )	// ez azért, hogy hatékonyabb legyen a keresés, a binFA ne egyetlen jobb ág legyen
                    {
						// nem talált ilyen változót
						adr = (gpeALF)( 1 + U4x2(0).cnt2fract( (U4)gpeALF_AAAAAA, nLAB ) * U4x2( 1, (U4)gpeALF_AAAAAA) );
                        adr = this; // ezzel indítjuk a keresést
                        nLAB++;
                    }

					aA[0] = adr.ALU( this );
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


			case ',': {	// vessző OSZLOPOK

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



					if( typ.x&0x40 )
						aA[0].equ( this, xyWH.a4x2[0], typ, op, d8 );
					else
						aA[0].equ( this, xyWH.a4x2[0], typ, op, u8 );

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
