#include "gpcwin.h"
//#include "gpcres.h"
extern U1 gpaALFadd[];




gpcALU::gpcALU( gpcRES* pM )
{
	gpmCLR;
	pRM = pM;
}
gpcALU::~gpcALU()
{
	null();
}

gpcRES* gpcRES::null()
{
	if( !this )
		return NULL;
	if( ppDAT )
	{
		for( U4 a = 0; a < nA; a++ )
		{
			if( !ppDAT[a] )
				continue;
			// typ:
			// x[7s,6f,5r,4p? 	: 3-0 nBYTE = 1<<(x&0xf) ]
			// yz[ dimXY ] 		,  nBYTE = 1<<(x&0xf)
			gpmDELary(ppDAT[a]);
		}
		delete[] ppDAT;
		ppDAT = NULL;
	}
	if( ppR )
	{
		for( U4 a = 0; a < nA; a++ )
		{
			if( !ppR[a] )
				continue;

			//ppR->null();
			gpmDEL(ppR[a]);
			//gpmDEL(pR);
		}
		gpmDELary(ppR);
	}
	gpmDELary( pALF );
	gpmDELary( pOP );
	gpmDELary( pTYP );
	gpmDELary( pAN );
	//gpmDELary( pTREE );
	//gpmDELary( pTx );
	gpmDELary( pISA );
	//gpmDELary( pALU );
	return this;
}



gpcADR& gpcADR::operator = ( gpcRES* pM )
{
	if( an.alf ? !(pRM = pM) : true )
	{
		//ix = 0;
		//pRM = NULL;
		return *this = gpeALF_null;
	}

	nA = pRM->nFND();
	iA = pRM->iFND( an.alf );
	while( iA >= nA )
	{
		pRM = pRM->pRM();
		if( !pRM )
		{
			iA = 0;
			return *this;
		}

		nA = pRM->nFND();
		iA = pRM->iFND( an.alf );
		dp = pM->iL() - pRM->iL();
	}
	return *this;
}
gpcALU& gpcADR::adr2ALU( gpcRES* pM )
{
	gpcALU alu( NULL );
	if( !pRM )
	{
		pRM = pM;
		if( !pRM )
			return alu;

		iA = pRM->nFND();
	}

	return pRM->ALU( iA );
}
U8 gpcALU::ins( gpcRES* pM, gpcLAZY& str )
{
	if( !str.n_load )
		return 0;

	U8 nLAB = 0;
	gpcADR adr = alf;
	adr = pM;
	while( adr.pRM )	// ez azért, hogy hatékonyabb legyen a keresés, a binFA ne egyetlen jobb ág legyen
	{
		// nem talált ilyen változót
		adr = (gpeALF)( 1 + U4x2(0).cnt2fract( (U4)gpeALF_AAAAAA, nLAB ) * U4x2( 1, (U4)gpeALF_AAAAAA) );
		adr = pM; // ezzel indítjuk a keresést
		nLAB++;
	}



	return str.n_load;
}

gpcRES* gpcALU::ins( gpcRES* pM, gpcRES* pKID )
{
	if( !pKID )
		return NULL;

	if( !pM )
		return pKID;

	gpcADR adr = alf;
	adr = pM;



	gpmDEL(pKID);
	return pKID;
}
gpcALU& gpcALU::zero( void )
{
	pDAT = NULL;
	pRES = NULL;
	op = 0;
	type = 0;
	an = 0;
	pRM->chg( *this );

	return *this;
}
gpcALU& gpcALU::equ( gpcALU& b )
{
	if( this == &b )
		return b;
	TanDT(b);

	U1	*pD = new U1[nALLOC()];
	gpmMEMCPY( pD, pDAT, AN().w );
	pD[AN().w] = 0;
	pDAT = pD;

	pRM->chg( *this );
	return *this;
}
gpcALU& gpcALU::ins( gpcRES* pM, U4x2 xy, U1x4 ty4 ) {
	if( !pM )
		return null();

	if( ty4.x&0x40 )
		return int2flt( pM, xy, ty4 );

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
		//*this = pM->ADD( adr.an.alf, ty4.u4, 0 );
	} else
		adr = pM;

	if( !adr.pRM )
	{
		*this = pM->ADD( adr.an.alf, ty4.u4, 0 );
		adr = pM;
		if( !adr.pRM )
			return null();
	}

	bool	bS1 = typ().x&0x80,
			bF1 = typ().x&0x40,
			bS2 = (typ().x|ty4.x)&0x80;

	if( bF1 )
		return int2flt( pM, xy, ty4 );

	gpcALU tmp;
	tmp.TanDT( *this );

	/*
	tmp.typ = typ;
	tmp.pDAT = pDAT;
	tmp.AN.a4x2[0] = AN.a4x2[0];*/

	TYPmx(ty4.u4); //typ.typMX( ty4 );		// typ.x[0b 1w 2l 3q]  // 1half? 2float 3double

	ANmx( xy+1 );
	U4x2 	X1 = tmp.tDIM(), // () tmp.typ.y, tmp.typ.z ),
			X2 = tDIM(); // ( typ.y, typ.z );
	X2.mx( X1 );


	U4	nAN1	= tmp.AN().z,
		nX1		= X1.area(),
		nB1		= tmp.typ().w,
		nXB1	= nX1*nB1,
		nLD1	= tmp.AN().w, //nAN1*nXB1,

		nAN2	= AN().z,
		nX2		= X2.area(),
		nB2		= typ().w,
		nXB2	= nX2*nB2,
		nLD2	= AN().w; //nAN2*nXB2;

	if( pDAT )
	if( AN().w != tmp.AN().w )
		pDAT = NULL;

	if( pDAT )
		return *this;

	U4 nNEW = gpmPAD( nLD2+1, 0x10 );
	U1		*pS = (U1*)tmp.pDAT,
			*pD = new U1[nNEW];
	gpmZn( pD, nNEW );
	pDAT = pD;

	if( !pS )
	{
		pM->chg( *this );
		return *this;
	}

	U4x2	T1 = tmp.trafo(), // nXB1, nXB1*tmp.AN.a4x2[0].x ),
			xyS,
			T2 = trafo(); // nXB2, nXB2*AN.a4x2[0].x );
	U8	u8;
	I8	i8;
	U4	s, d, x,
		sx = tmp.AN().x, //.a4x2[0].x,
		sy = tmp.AN().y; //.a4x2[0].y;

	if( (typ().u4 == tmp.typ().u4) && ( T1.y == T2.y ) )
	{
		memcpy( pD, pS, T1.y*sy );
	}
	else for( xyS = 0; xyS.y < sy; xyS.y++ )
	if( typ().u4 == tmp.typ().u4 )
	{
		s = xyS*T1;
		d = xyS*T2;
		memcpy( pD+d, pS+s, T1.y );
	} else {
		for( xyS.x = 0; xyS.x < sx; xyS.x++ ) {
			s = xyS*T1;
			d = xyS*T2;
 /// SIGNED DST I8 I4 I2 I1 -------------------------------------------------------------
			if( bS2 ) {
				if( bS1 ) {	/// signed src i8 i4 i2 i1 ---------------------------------------------------------
 /// SIGNED DST I8 I4 -------------------------------------------------------
					if( nB2 > 2 ) {
 /// SIGNED DST I8 ------------------------------------------------------
						if( nB2 > 4 ) {
							if( nB1 > 2 ) { /// signed src i8 i4 -----------------------------------------------
								if( nB1 > 4 ) { /// signed src i8 ----------------------------------------------
									for( x = 0; x < nX1; x++ )
										((I8*)(pD+d))[x] = ((I8*)(pS+s))[x];
									continue;
								}
								/// signed src i4 ----------------------------------------------
								for( x = 0; x < nX1; x++ )
									((I8*)(pD+d))[x] = ((I4*)(pS+s))[x];
								continue;
							}
							if( nB1 > 1 ) {	/// signed src i2 ----------------------------------------------
								for( x = 0; x < nX1; x++ )
									((I8*)(pD+d))[x] = ((I2*)(pS+s))[x];
								continue;
							}
							/// signed src i1 ----------------------------------------------
							for( x = 0; x < nX1; x++ )
								((I8*)(pD+d))[x] = ((I1*)(pS+s))[x];
							continue;
						}
 /// SIGNED DST I4 -----------------------------------------------------
						if( nB1 > 2 ) { /// signed src i8 i4 ------------------------------------------------------
							if( nB1 > 4 ) { /// signed src i8 ------------------------------------------------------
								for( x = 0; x < nX1; x++ )
									((I4*)(pD+d))[x] = ((I8*)(pS+s))[x];
								continue;
							}
							/// signed src i4 ------------------------------------------------------
							for( x = 0; x < nX1; x++ )
								((I4*)(pD+d))[x] = ((I4*)(pS+s))[x];
							continue;
						}
						if( nB1 > 1 ) { /// signed src i2 ------------------------------------------------------
							for( x = 0; x < nX1; x++ )
								((I4*)(pD+d))[x] = ((I2*)(pS+s))[x];
							continue;
						}
						/// signed src i1 ------------------------------------------------------
						for( x = 0; x < nX1; x++ )
							((I4*)(pD+d))[x] = ((I1*)(pS+s))[x];
						continue;
					}
 /// SIGNED DST I2 --------------------------------------
					if( nB2 > 1 ) {
						if( nB1 > 2 ) { /// signed src i8 i4 ---------------------------------------
							if( nB1 > 4 ) { /// signed src i8 ---------------------------------------
								for( x = 0; x < nX1; x++ )
									((I2*)(pD+d))[x] = ((I8*)(pS+s))[x];
								continue;
							}
							/// signed src i4 ---------------------------------------
							for( x = 0; x < nX1; x++ )
								((I2*)(pD+d))[x] = ((I4*)(pS+s))[x];
							continue;
						}
						if( nB1 > 1 ) { /// signed src i2 --------------------------------------
							for( x = 0; x < nX1; x++ )
								((I2*)(pD+d))[x] = ((I2*)(pS+s))[x];
							continue;
						}
						/// signed src i1 --------------------------------------
						for( x = 0; x < nX1; x++ )
							((I2*)(pD+d))[x] = ((I1*)(pS+s))[x];

						continue;
					}
 /// SIGNED DST I1 --------------------------------------------------------
					if( nB1 > 2 ) { /// signed src i8 i4 ---------------------------------------
						if( nB1 > 4 ) { /// signed src i8
							for( x = 0; x < nX1; x++ )
								((I1*)(pD+d))[x] = ((I8*)(pS+s))[x];
							continue;
						}
						/// signed src i4 --------------------------------
						for( x = 0; x < nX1; x++ )
							((I1*)(pD+d))[x] = ((I4*)(pS+s))[x];
						continue;
					}
					if( nB1 > 1 ) { /// signed src i2 --------------------------------------
						for( x = 0; x < nX1; x++ )
							((I1*)(pD+d))[x] = ((I2*)(pS+s))[x];
						continue;
					}
					/// signed src i1 --------------------------------------
					for( x = 0; x < nX1; x++ )
						((I1*)(pD+d))[x] = ((I1*)(pS+s))[x];

					continue;
				}

				/// UNsigned src u8 u4 u2 u1 ---------------------------------------------------------

 /// SIGNED DST I8 I4 -------------------------------------------------------
				if( nB2 > 2 ) {
 /// SIGNED DST I8 ------------------------------------------------------
					if( nB2 > 4 ) {
						if( nB1 > 2 ) { /// UNsigned src u8 u4 -----------------------------------------------
							if( nB1 > 4 ) { /// UNsigned src u8 ----------------------------------------------
								for( x = 0; x < nX1; x++ )
									((I8*)(pD+d))[x] = ((U8*)(pS+s))[x];
								continue;
							}
							/// UNsigned src u4 ----------------------------------------------
							for( x = 0; x < nX1; x++ )
								((I8*)(pD+d))[x] = ((U4*)(pS+s))[x];
							continue;
						}
						if( nB1 > 1 ) {	/// signed src u2 ----------------------------------------------
							for( x = 0; x < nX1; x++ )
								((I8*)(pD+d))[x] = ((U2*)(pS+s))[x];
							continue;
						}
						/// signed src u1 ----------------------------------------------
						for( x = 0; x < nX1; x++ )
							((I8*)(pD+d))[x] = ((U1*)(pS+s))[x];

						continue;
					}
 /// SIGNED DST I4 -----------------------------------------------------
					if( nB1 > 2 ) { /// UNsigned src u8 u4 ------------------------------------------------------
						if( nB1 > 4 ) { /// UNsigned src u8 ------------------------------------------------------
							for( x = 0; x < nX1; x++ )
								((I4*)(pD+d))[x] = ((U8*)(pS+s))[x];
							continue;
						}
						/// signed src u4 ------------------------------------------------------
						for( x = 0; x < nX1; x++ )
							((I4*)(pD+d))[x] = ((U4*)(pS+s))[x];
						continue;
					}
					if( nB1 > 1 ) { /// UNsigned src u2 ------------------------------------------------------
						for( x = 0; x < nX1; x++ )
							((I4*)(pD+d))[x] = ((U2*)(pS+s))[x];
						continue;
					}
					/// UNsigned src u1 ------------------------------------------------------
					for( x = 0; x < nX1; x++ )
						((I4*)(pD+d))[x] = ((U1*)(pS+s))[x];
					continue;
				}
 /// SIGNED DST I2 --------------------------------------
				if( nB2 > 1 ) {
					if( nB1 > 2 ) { /// UNsigned src u8 u4---------------------------------------
						if( nB1 > 4 ) { /// UNsigned src u8 ---------------------------------------
							for( x = 0; x < nX1; x++ )
								((I2*)(pD+d))[x] = ((U8*)(pS+s))[x];
							continue;
						}
						/// UNsigned src u4 ---------------------------------------
						for( x = 0; x < nX1; x++ )
							((I2*)(pD+d))[x] = ((U4*)(pS+s))[x];
						continue;
					}
					if( nB1 > 1 ) { /// UNsigned src u2 ---------------------------------------
						for( x = 0; x < nX1; x++ )
							((I2*)(pD+d))[x] = ((U2*)(pS+s))[x];
						continue;
					}
					/// UNsigned src u1 ---------------------------------------
					for( x = 0; x < nX1; x++ )
						((I2*)(pD+d))[x] = ((U1*)(pS+s))[x];

					continue;
				}
 /// SIGNED DST I1 --------------------------------------------------------
				if( nB1 > 2 ) { /// UNsigned src u8 u4 ---------------------------------------
					if( nB1 > 4 ) { /// UNsigned src u8
						for( x = 0; x < nX1; x++ )
							((I1*)(pD+d))[x] = ((U8*)(pS+s))[x];
						continue;
					}
					/// UNsigned src u4 --------------------------------
					for( x = 0; x < nX1; x++ )
						((I1*)(pD+d))[x] = ((U4*)(pS+s))[x];
					continue;
				}
				if( nB1 > 1 ) { /// UNsigned src u2 --------------------------------------
					for( x = 0; x < nX1; x++ )
						((I1*)(pD+d))[x] = ((U2*)(pS+s))[x];
					continue;
				}
				/// UNsigned src u1 --------------------------------------
				for( x = 0; x < nX1; x++ )
					((I1*)(pD+d))[x] = ((U1*)(pS+s))[x];

				continue;
			}	/// if( bS2 )  END --------------------------------------------------




 /// UN SIGNED DST U8 U4 U2 U1 --------------------------------------------------------------------
			if( bS1 ) {	/// signed src i8 i4 i2 i1 ---------------------------------------------------------
 /// UN SIGNED DST U8 U4 -------------------------------------------------------
				if( nB2 > 2 ) {
 /// UN SIGNED DST U8 ------------------------------------------------------
					if( nB2 > 4 ) {
						if( nB1 > 2 ) { /// signed src i8 i4 -----------------------------------------------
							if( nB1 > 4 ) { /// signed src i8 ----------------------------------------------
								for( x = 0; x < nX1; x++ )
									((U8*)(pD+d))[x] = ((I8*)(pS+s))[x];
								continue;
							}
							/// signed src i4 ----------------------------------------------
							for( x = 0; x < nX1; x++ )
								((U8*)(pD+d))[x] = ((I4*)(pS+s))[x];
							continue;
						}
						if( nB1 > 1 ) {	/// signed src i2 ----------------------------------------------
							for( x = 0; x < nX1; x++ )
								((U8*)(pD+d))[x] = ((I2*)(pS+s))[x];
							continue;
						}
						/// signed src i1 ----------------------------------------------
						for( x = 0; x < nX1; x++ )
							((U8*)(pD+d))[x] = ((I1*)(pS+s))[x];

						continue;
					}
 /// UN SIGNED DST U4 -----------------------------------------------------
					if( nB1 > 2 ) {	/// signed src i8 i4 ------------------------------------------------------
						if( nB1 > 4 ) { /// signed src i8 ------------------------------------------------------
							for( x = 0; x < nX1; x++ )
								((U4*)(pD+d))[x] = ((I8*)(pS+s))[x];
							continue;
						}
						/// signed src i4 ------------------------------------------------------
						for( x = 0; x < nX1; x++ )
							((U4*)(pD+d))[x] = ((I4*)(pS+s))[x];
						continue;
					}
					if( nB1 > 1 ) { /// signed src i2 ------------------------------------------------------
						for( x = 0; x < nX1; x++ )
							((U4*)(pD+d))[x] = ((I2*)(pS+s))[x];
						continue;
					}
					/// signed src i1 ------------------------------------------------------
					for( x = 0; x < nX1; x++ )
						((U4*)(pD+d))[x] = ((I1*)(pS+s))[x];
					continue;
				}
 /// UN SIGNED DST U2 --------------------------------------
				if( nB2 > 1 ) {
					if( nB1 > 2 ) { /// signed src i8 i4 ---------------------------------------
						if( nB1 > 4 ) { /// signed src i8 ---------------------------------------
							for( x = 0; x < nX1; x++ )
								((U2*)(pD+d))[x] = ((I8*)(pS+s))[x];
							continue;
						}
						/// signed src i4 ---------------------------------------
						for( x = 0; x < nX1; x++ )
							((U2*)(pD+d))[x] = ((I4*)(pS+s))[x];
						continue;
					}
					if( nB1 > 1 ) { /// signed src i2 ---------------------------------------
						for( x = 0; x < nX1; x++ )
							((U2*)(pD+d))[x] = ((I2*)(pS+s))[x];
						continue;
					}
					/// signed src i1 ---------------------------------------
					for( x = 0; x < nX1; x++ )
						((U2*)(pD+d))[x] = ((I1*)(pS+s))[x];

					continue;
				}

 /// UN SIGNED DST U1 --------------------------------------------------------
				if( nB1 > 2 ) { /// signed src i8 i4 ---------------------------------------
					if( nB1 > 4 ) { /// signed src i8
						for( x = 0; x < nX1; x++ )
							((U1*)(pD+d))[x] = ((I8*)(pS+s))[x];
						continue;
					}
					/// signed src i4 --------------------------------
					for( x = 0; x < nX1; x++ )
						((U1*)(pD+d))[x] = ((I4*)(pS+s))[x];
					continue;
				}
				if( nB1 > 1 ) { /// signed src i2 --------------------------------------
					for( x = 0; x < nX1; x++ )
						((U1*)(pD+d))[x] = ((I2*)(pS+s))[x];
					continue;
				}
				/// signed src i1 --------------------------------------
				for( x = 0; x < nX1; x++ )
					((U1*)(pD+d))[x] = ((I1*)(pS+s))[x];

				continue;
			}

			/// UNsigned src u8 u4 u2 u1 ---------------------------------------------------------

 /// UN SIGNED DST U8 U4 -------------------------------------------------------
			if( nB2 > 2 ) {
 /// UN SIGNED DST U8 ------------------------------------------------------
				if( nB2 > 4 ) {
					if( nB1 > 2 ) { /// UNsigned src u8 u4 -----------------------------------------------
						if( nB1 > 4 ) { /// UNsigned src u8 ----------------------------------------------
							for( x = 0; x < nX1; x++ )
								((U8*)(pD+d))[x] = ((U8*)(pS+s))[x];
							continue;
						}
						/// UNsigned src u4 ----------------------------------------------
						for( x = 0; x < nX1; x++ )
							((U8*)(pD+d))[x] = ((U4*)(pS+s))[x];
						continue;
					}
					if( nB1 > 1 ) {	/// signed src u2 ----------------------------------------------
						for( x = 0; x < nX1; x++ )
							((U8*)(pD+d))[x] = ((U2*)(pS+s))[x];
						continue;
					}
					/// signed src u1 ----------------------------------------------
					for( x = 0; x < nX1; x++ )
						((U8*)(pD+d))[x] = ((U1*)(pS+s))[x];

					continue;
				}
 /// UN SIGNED DST U4 -----------------------------------------------------
				if( nB1 > 2 ) { /// UNsigned src u8 u4 ------------------------------------------------------
					if( nB1 > 4 ) { /// UNsigned src u8 ------------------------------------------------------
						for( x = 0; x < nX1; x++ )
							((U4*)(pD+d))[x] = ((U8*)(pS+s))[x];
						continue;
					}
					/// signed src u4 ------------------------------------------------------
					for( x = 0; x < nX1; x++ )
						((U4*)(pD+d))[x] = ((U4*)(pS+s))[x];
					continue;
				}
				if( nB1 > 1 ) { /// UNsigned src u2 ------------------------------------------------------
					for( x = 0; x < nX1; x++ )
						((U4*)(pD+d))[x] = ((U2*)(pS+s))[x];
					continue;
				}
				/// UNsigned src u1 ------------------------------------------------------
				for( x = 0; x < nX1; x++ )
					((U4*)(pD+d))[x] = ((U1*)(pS+s))[x];
				continue;
			}
 /// UN SIGNED DST I2 --------------------------------------
			if( nB2 > 1 ) {
				if( nB1 > 2 ) { /// UNsigned src u8 u4 ---------------------------------------
					if( nB1 > 4 ) { /// UNsigned src u8 ---------------------------------------
						for( x = 0; x < nX1; x++ )
							((U2*)(pD+d))[x] = ((U8*)(pS+s))[x];
						continue;
					}
					/// UNsigned src u4 ---------------------------------------
					for( x = 0; x < nX1; x++ )
						((U2*)(pD+d))[x] = ((U4*)(pS+s))[x];
					continue;
				}
				if( nB1 > 1 ) { /// UNsigned src u2 ---------------------------------------
					for( x = 0; x < nX1; x++ )
						((U2*)(pD+d))[x] = ((U2*)(pS+s))[x];
					continue;
				}
				/// UNsigned src u1 ---------------------------------------
				for( x = 0; x < nX1; x++ )
					((U2*)(pD+d))[x] = ((U1*)(pS+s))[x];

				continue;
			}
/// UN SIGNED DST U1 --------------------------------------------------------
			if( nB1 > 2 ) { /// UNsigned src u8 u4 ---------------------------------------
				if( nB1 > 4 ) { /// UNsigned src u8
					for( x = 0; x < nX1; x++ )
						((U1*)(pD+d))[x] = ((U8*)(pS+s))[x];
					continue;
				}
				/// UNsigned src u4 --------------------------------
				for( x = 0; x < nX1; x++ )
					((U1*)(pD+d))[x] = ((U4*)(pS+s))[x];
				continue;
			}
			if( nB1 > 1 ) { /// UNsigned src u2 --------------------------------------
				for( x = 0; x < nX1; x++ )
					((U1*)(pD+d))[x] = ((U2*)(pS+s))[x];
				continue;
			}
			/// UNsigned src u1 --------------------------------------
			for( x = 0; x < nX1; x++ )
				((U1*)(pD+d))[x] = ((U1*)(pS+s))[x];
		}
	}



	pM->chg( *this );
	return *this;
}

U1* gpcALU::ALUdat( gpcRES* pM, U4x2 xy, U1x4 ty4, I1x4 op4, U8 u8, double d8 )
{
	if( !pM )
		return NULL;

	// typ:
	// x[7s,6f,5r,4p? 	: 3-0 nBYTE = 1<<(x&0xf) ]
	// yz[ dimXY ] 		, w[] nBYTE = 1<<(x&0xf)
	if( !(ty4.u4&gpeTYP_STRmsk) )
	{
		if( pRM == pM )
		if( !pDAT )				// nincs forrás
		if( (d8+u8) == 0.0 )
		{
			xy -= sub;
			ANmx( xy+1 ); //AN.a4x2[0].mx( xy+1 );
			typ( gpeTYP_U1 );
			TYPmx(ty4.u4); // if(ty4.x) typ.x |= ty4.x;

			U4 nAN2 = nALLOC(); //gpmPAD( AN().w, 0x10 ); //.a4x2[0].area()+1, 0x10 );
			pDAT = new U1[nAN2];
			gpmZn( (U1*)pDAT, nAN2 );
			pM->chg( *this );

			return (U1*)pDAT;
		}

		ty4.u4 = gpeTYP_U1;
	} else {
		if( ty4.y < 1 )
			ty4.y = 1;
		if( ty4.z < 1 )
			ty4.z = 1;
	}

	U1 OR = typ().x|ty4.x;
	if( op4.x < 0 )
	{
		U1 sub = -op4.x;
		if( sub&1 )
			OR |= 0x80;
		else
			op.x = 0;
	}

	if( d8 != 0.0 ? true : (u8&0x8000000000000000) )
	{
		OR |= 0x40;
	}

	if( OR&0x40 )
	{
		// return equ( pM, xy, ty4, op4, d8, x );
		d8 += u8;
		if( ty4.x&0xf != 3 )
		if( abs(d8) > (double)0xffFFFF )
			ty4.x = 0xc3;
		else
			ty4.x = 0xc2;

		if( op.y < 0 )
			d8 = -d8;

		xy -= sub;
		int2flt( pM, xy, ty4 );
		return (U1*)pDAT;
	}
	if( OR&0x80 )
	{
		//return equSIG( pM, xy, ty4, op4, u8, x );

		bool bSIG = pDAT ? typ().x&0x80 : true;

		if( u8 <= 0x7fFF )
		{
			if( u8 < 0x7f )
				ty4.x = 0x80;
			else
				ty4.x = 0x81;
		}
		else if( u8 <= 0x7fFFffFF )
		{
			ty4.x = 0x82;
		} else
			ty4.x = 0x83;

		if( !bSIG ) // nem volt signed?
		if( (ty4.x&0xf) <= (typ().x&0xf)  )
		{
			// akkor nem jó ha ugyan akkorát akarunk
			ty4.x = 0x80 + min( 3, (typ().x&0xf)+1 );
		}


		I8 i8 = ( op4.x < 0 ) ? -u8 : u8;

		xy -= sub;
		ins( pM, xy, ty4 );
		return (U1*)pDAT;


	}


	if( u8 < 0x10000 )
		ty4.x = u8 > 0xff;
	else
		ty4.x = 2 + (u8>0xffffFFFF);

	xy -= sub;

	ins( pM, xy, ty4 );
	return (U1*)pDAT;
}

gpcALU& gpcALU::equ( gpcRES* pM, U4x2 xy, U1x4 ty4, I1x4 op4, U8 u8, double d8, U2 x )
{
	if( !pM )
		return null();


	if( ty4.y < 1 )
		ty4.y = 1;
	if( ty4.z < 1 )
		ty4.z = 1;

	U1 OR = typ().x|ty4.x;
	if( op4.x < 0 )
	{
		U1 sub = -op4.x;
		if( sub&1 )
			OR |= 0x80;
		else
			op.x = 0;
	}

	if( d8 != 0.0 ? true : (u8&0x8000000000000000) )
	{
		OR |= 0x40;
	}

	if( OR&0x40 )
	{
		d8 += u8;
		return equ( pM, xy, ty4, op4, d8, x );
	}
	if( OR&0x80 )
		return equSIG( pM, xy, ty4, op4, u8, x );

	if( u8 < 0x10000 )
		ty4.x = u8 > 0xff;
	else
		ty4.x = 2 + (u8>0xffffFFFF);

	xy -= sub;

	ins( pM, xy, ty4 );
	U1* pD = (U1*)pDAT;
	if( !pD )
		return *this;


	//U4x2 	X( typ.y, typ.z );

	/*U4 		nAN = AN().z, //.a4x2[0].area(),
			nB	= typ().w,
			nX	= X.area(),
			nXB	= nX*nB;*/

	U4x2	T = trafo(); // nXB, nXB*AN.a4x2[0].x );
	U4		d = xy*T;
	if( d > AN().w )
		d %= AN().w;
	x %= tDIM().area();

	if( typ().w > 2 )
	{
		if( typ().w > 4 )
			((U8*)(pD+d))[x] = u8;
		else
			((U4*)(pD+d))[x] = u8;
	}
	else if( typ().w > 1 )
		((U2*)(pD+d))[x] = u8;
	else
		((U1*)(pD+d))[x] = u8;

	return *this;
}

gpcALU& gpcALU::equSIG( gpcRES* pM, U4x2 xy, U1x4 ty4, I1x4 op4, U8 u8, U2 x )
{
	if( !pM )
		return null();

	bool bSIG = pDAT ? typ().x&0x80 : true;

	if( u8 <= 0x7fFF )
	{
		if( u8 < 0x7f )
			ty4.x = 0x80;
		else
			ty4.x = 0x81;
	}
	else if( u8 <= 0x7fFFffFF )
	{
		ty4.x = 0x82;
	} else
		ty4.x = 0x83;

	if( !bSIG ) // nem volt signed?
	if( (ty4.x&0xf) <= (typ().x&0xf)  )
	{
		// akkor nem jó ha ugyan akkorát akarunk
		ty4.x = 0x80 + min( 3, (typ().x&0xf)+1 );
	}


	I8 i8 = ( op4.x < 0 ) ? -u8 : u8;

	xy -= sub;
	ins( pM, xy, ty4 );
	U1* pD = (U1*)pDAT;
	if( !pD )
		return *this;


	/*U4x2 	X( typ.y, typ.z );

	U4 		nAN = AN.a4x2[0].area(),
			nB	= typ.w,
			nX	= X.area(),
			nXB	= nX*nB;
	U4x2	T( nXB, nXB*AN.a4x2[0].x );
	U4		d = xy*T;
	if( d > nAN*nXB )
		d %= nAN*nXB;
	if( x > nX )
		x %= nX;*/


	U4x2	T = trafo();
	U4		d = xy*T;
	if( d > AN().w )
		d %= AN().w;
	x %= tDIM().area();


	if( typ().w > 2 )
	{
		if( typ().w > 4 )
			((I8*)(pD+d))[x] = i8;
		else
			((I4*)(pD+d))[x] = i8;
	}
	else if( typ().w > 1 )
		((I2*)(pD+d))[x] = i8;
	else
		((I1*)(pD+d))[x] = i8;

	return *this;
}

gpcALU& gpcALU::equ( gpcRES* pM, U4x2 xy, U1x4 ty4, I1x4 op4, double d8, U2 x )
{
	if( !pM )
		return null();

	if( ty4.x&0xf != 3 )
	if( abs(d8) > (double)0xffFFFF )
		ty4.x = 0xc3;
	else
		ty4.x = 0xc2;

	if( op.y < 0 )
		d8 = -d8;

	xy -= sub;
	int2flt( pM, xy, ty4 );
	U1* pD = (U1*)pDAT;
	if( !pD )
		return *this;


	/*U4x2 	X( typ.y, typ.z );

	U4 		nAN = AN.a4x2[0].area(),
			nB	= typ.w,
			nX	= X.area(),
			nXB	= nX*nB;
	U4x2	T( nXB, nXB*AN.a4x2[0].x );
	U4		d = xy*T;
	if( d > nAN*nXB )
		d %= nAN*nXB;
	if( x > nX )
		x %= nX; */

	U4x2	T = trafo();
	U4		d = xy*T;
	if( d > AN().w )
		d %= AN().w;
	x %= tDIM().area();

	if( typ().w > 4 )
		((double*)(pD+d))[x] = d8;
	else
		((float*)(pD+d))[x] = d8;

	return *this;
}

gpcALU& gpcALU::operator = ( gpcREG& a )
{
	U1x4 t = 0;
	t.x = a.t();
	if( t.x&0x40 )
		equ( pRM, a.xy, t, I1x4(0), 0, a.d8() );
	else if( t.x&0x80 )
		equ( pRM, a.xy, t, I1x4(-1), a.u8(), 0.0 );
	else
		equ( pRM, a.xy, t, 0, a.u8(), 0.0  );

	return *this;
}

gpcALU& gpcALU::operator += ( gpcREG& a )
{
	//U4x2 xy = 0;
	U4 x = 0;

	U1x4 t = 0;
	t.x = a.t();
	U1* pD = NULL;
	if( t.x&0x40 ) {	/// float ----------------------------------
		pD = ALUdat( pRM, a.xy, t, I1x4(0), 0, a.d8() );
		if( !pD )
			return *this;

		//U4x2	T = trafo();
		U4		d = (a.xy-sub)*trafo();
		if( d > AN().w )
			d %= AN().w;
		x %= tDIM().area();

		/*U4x2 	X( typ.y, typ.z );
		U4 		nAN = AN.a4x2[0].area(),
				nB	= typ.w,
				nX	= X.area(),
				nXB	= nX*nB;
		U4x2	T( nXB, nXB*AN.a4x2[0].x );
		U4		d = (a.xy-sub)*T;
		if( d > nAN*nXB )
			d %= nAN*nXB;
		if( x > nX )
			x %= nX;*/

		if( typ().w > 4 )
			((double*)(pD+d))[x] += a.d8();
		else
			((float*)(pD+d))[x] += a.d8();

		return *this;
	}
	if( t.x&0x80 ) {	/// SIGNED ----------------------------------
		pD = ALUdat( pRM, 0, t, I1x4(-1), a.u8(), 0.0 );
		if( !pD )
			return *this;

		U4		d = (a.xy-sub)*trafo();
		if( d > AN().w )
			d %= AN().w;
		x %= tDIM().area();

		/*U4x2 	X( typ.y, typ.z );
		U4 		nAN = AN.a4x2[0].area(),
				nB	= typ.w,
				nX	= X.area(),
				nXB	= nX*nB;
		U4x2	T( nXB, nXB*AN.a4x2[0].x );
		U4		d =  (a.xy-sub)*T;
		if( d > nAN*nXB )
			d %= nAN*nXB;
		if( x > nX )
			x %= nX;*/


		if( typ().w > 2 )
		{
			if( typ().w > 4 )
				((I8*)(pD+d))[x] += a.i8();
			else
				((I4*)(pD+d))[x] += a.i8();
		}
		else if( typ().w > 1 )
			((I2*)(pD+d))[x] += a.i8();
		else
			((I1*)(pD+d))[x] += a.i8();

		return *this;
	}
	/// unSIGNED ----------------------------------
	pD = ALUdat( pRM, 0, t, 0, a.u8(), 0.0  );
	if( !pD )
		return *this;

	U4		d = (a.xy-sub)*trafo();
		if( d > AN().w )
			d %= AN().w;
		x %= tDIM().area();

	/*U4x2 	X( typ.y, typ.z );

	U4 		nAN = AN.a4x2[0].area(),
			nB	= typ.w,
			nX	= X.area(),
			nXB	= nX*nB;
	U4x2	T( nXB, nXB*AN.a4x2[0].x );
	U4		d =  (a.xy-sub)*T;
	if( d > nAN*nXB )
		d %= nAN*nXB;
	if( x > nX )
		x %= nX;*/


	if( typ().w > 2 )
	{
		if( typ().w > 4 )
			((U8*)(pD+d))[x] += a.u8();
		else
			((U4*)(pD+d))[x] += a.u8();
	}
	else if( typ().w > 1 )
		((U2*)(pD+d))[x] += a.u8();
	else
		((U1*)(pD+d))[x] += a.u8();

	return *this;
}
gpcALU& gpcALU::operator -= ( gpcREG& a )
{
	//U4x2 xy = 0;
	U4 x = 0;

	U1x4 t = 0;
	t.x = a.t();
	U1* pD = NULL;
	if( t.x&0x40 ) {	/// float ----------------------------------
		pD = ALUdat( pRM, a.xy, t, I1x4(0), 0, a.d8() );
		if( !pD )
			return *this;

		U4		d = (a.xy-sub)*trafo();
		if( d > AN().w )
			d %= AN().w;
		x %= tDIM().area();

		/*U4x2 	X( typ.y, typ.z );
		U4 		nAN = AN.a4x2[0].area(),
				nB	= typ.w,
				nX	= X.area(),
				nXB	= nX*nB;
		U4x2	T( nXB, nXB*AN.a4x2[0].x );
		U4		d = (a.xy-sub)*T;
		if( d > nAN*nXB )
			d %= nAN*nXB;
		if( x > nX )
			x %= nX;*/

		if( typ().w > 4 )
			((double*)(pD+d))[x] -= a.d8();
		else
			((float*)(pD+d))[x] -= a.d8();

		return *this;
	}
	if( t.x&0x80 ) {	/// SIGNED ----------------------------------
		pD = ALUdat( pRM, 0, t, I1x4(-1), a.u8(), 0.0 );
		if( !pD )
			return *this;

		U4		d = (a.xy-sub)*trafo();
		if( d > AN().w )
			d %= AN().w;
		x %= tDIM().area();

		/*U4x2 	X( typ.y, typ.z );
		U4 		nAN = AN.a4x2[0].area(),
				nB	= typ.w,
				nX	= X.area(),
				nXB	= nX*nB;
		U4x2	T( nXB, nXB*AN.a4x2[0].x );
		U4		d =  (a.xy-sub)*T;
		if( d > nAN*nXB )
			d %= nAN*nXB;
		if( x > nX )
			x %= nX;*/


		if( typ().w > 2 )
		{
			if( typ().w > 4 )
				((I8*)(pD+d))[x] -= a.i8();
			else
				((I4*)(pD+d))[x] -= a.i8();
		}
		else if( typ().w > 1 )
			((I2*)(pD+d))[x] -= a.i8();
		else
			((I1*)(pD+d))[x] -= a.i8();

		return *this;
	}
	/// unSIGNED ----------------------------------
	pD = ALUdat( pRM, 0, t, 0, a.u8(), 0.0  );
	if( !pD )
		return *this;

	U4 d = (a.xy-sub)*trafo();
	if( d > AN().w )
		d %= AN().w;
	x %= tDIM().area();

		/*U4x2 	X( typ.y, typ.z );

	U4 		nAN = AN.a4x2[0].area(),
			nB	= typ.w,
			nX	= X.area(),
			nXB	= nX*nB;
	U4x2	T( nXB, nXB*AN.a4x2[0].x );
	U4		d =  (a.xy-sub)*T;
	if( d > nAN*nXB )
		d %= nAN*nXB;
	if( x > nX )
		x %= nX;*/


	if( typ().w > 2 )
	{
		if( typ().w > 4 )
			((U8*)(pD+d))[x] -= a.u8();
		else
			((U4*)(pD+d))[x] -= a.u8();
	}
	else if( typ().w > 1 )
		((U2*)(pD+d))[x] -= a.u8();
	else
		((U1*)(pD+d))[x] -= a.u8();

	return *this;
}
gpcALU& gpcALU::operator *= ( gpcREG& a )
{
	//U4x2 xy = 0;
	U4 x = 0;

	U1x4 t = 0;
	t.x = a.t();
	U1* pD = NULL;
	if( t.x&0x40 ) {	/// float ----------------------------------
		pD = ALUdat( pRM, a.xy, t, I1x4(0), 0, a.d8() );
		if( !pD )
			return *this;

		U4		d = (a.xy-sub)*trafo();
		if( d > AN().w )
			d %= AN().w;
		x %= tDIM().area();

		/*U4x2 	X( typ.y, typ.z );
		U4 		nAN = AN.a4x2[0].area(),
				nB	= typ.w,
				nX	= X.area(),
				nXB	= nX*nB;
		U4x2	T( nXB, nXB*AN.a4x2[0].x );
		U4		d = (a.xy-sub)*T;
		if( d > nAN*nXB )
			d %= nAN*nXB;
		if( x > nX )
			x %= nX;*/

		if( typ().w > 4 )
			((double*)(pD+d))[x] *= a.d8();
		else
			((float*)(pD+d))[x] *= a.d8();

		return *this;
	}
	if( t.x&0x80 ) {	/// SIGNED ----------------------------------
		pD = ALUdat( pRM, 0, t, I1x4(-1), a.u8(), 0.0 );
		if( !pD )
			return *this;

		U4		d = (a.xy-sub)*trafo();
		if( d > AN().w )
			d %= AN().w;
		x %= tDIM().area();

		/*U4x2 	X( typ.y, typ.z );
		U4 		nAN = AN.a4x2[0].area(),
				nB	= typ.w,
				nX	= X.area(),
				nXB	= nX*nB;
		U4x2	T( nXB, nXB*AN.a4x2[0].x );
		U4		d =  (a.xy-sub)*T;
		if( d > nAN*nXB )
			d %= nAN*nXB;
		if( x > nX )
			x %= nX;*/


		if( typ().w > 2 )
		{
			if( typ().w > 4 )
				((I8*)(pD+d))[x] *= a.i8();
			else
				((I4*)(pD+d))[x] *= a.i8();
		}
		else if( typ().w > 1 )
			((I2*)(pD+d))[x] *= a.i8();
		else
			((I1*)(pD+d))[x] *= a.i8();

		return *this;
	}
	/// unSIGNED ----------------------------------
	pD = ALUdat( pRM, 0, t, 0, a.u8(), 0.0  );
	if( !pD )
		return *this;

	U4		d = (a.xy-sub)*trafo();
		if( d > AN().w )
			d %= AN().w;
		x %= tDIM().area();

		/*U4x2 	X( typ.y, typ.z );

	U4 		nAN = AN.a4x2[0].area(),
			nB	= typ.w,
			nX	= X.area(),
			nXB	= nX*nB;
	U4x2	T( nXB, nXB*AN.a4x2[0].x );
	U4		d =  (a.xy-sub)*T;
	if( d > nAN*nXB )
		d %= nAN*nXB;
	if( x > nX )
		x %= nX;*/


	if( typ().w > 2 )
	{
		if( typ().w > 4 )
			((U8*)(pD+d))[x] *= a.u8();
		else
			((U4*)(pD+d))[x] *= a.u8();
	}
	else if( typ().w > 1 )
		((U2*)(pD+d))[x] *= a.u8();
	else
		((U1*)(pD+d))[x] *= a.u8();

	return *this;
}
gpcALU& gpcALU::operator /= ( gpcREG& a )
{
	//U4x2 xy = 0;
	U4 x = 0;

	U1x4 t = 0;
	t.x = a.t();
	U1* pD = NULL;
	if( t.x&0x40 ) {	/// float ----------------------------------
		pD = ALUdat( pRM, a.xy, t, I1x4(0), 0, a.d8() );
		if( !pD )
			return *this;

		U4		d = (a.xy-sub)*trafo();
		if( d > AN().w )
			d %= AN().w;
		x %= tDIM().area();

		/*U4x2 	X( typ.y, typ.z );
		U4 		nAN = AN.a4x2[0].area(),
				nB	= typ.w,
				nX	= X.area(),
				nXB	= nX*nB;
		U4x2	T( nXB, nXB*AN.a4x2[0].x );
		U4		d = (a.xy-sub)*T;
		if( d > nAN*nXB )
			d %= nAN*nXB;
		if( x > nX )
			x %= nX;*/

		if( typ().w > 4 )
			((double*)(pD+d))[x] /= a.d8();
		else
			((float*)(pD+d))[x] /= a.d8();

		return *this;
	}
	if( t.x&0x80 ) {	/// SIGNED ----------------------------------
		pD = ALUdat( pRM, 0, t, I1x4(-1), a.u8(), 0.0 );
		if( !pD )
			return *this;

		U4		d = (a.xy-sub)*trafo();
		if( d > AN().w )
			d %= AN().w;
		x %= tDIM().area();

		/*U4x2 	X( typ.y, typ.z );
		U4 		nAN = AN.a4x2[0].area(),
				nB	= typ.w,
				nX	= X.area(),
				nXB	= nX*nB;
		U4x2	T( nXB, nXB*AN.a4x2[0].x );
		U4		d =  (a.xy-sub)*T;
		if( d > nAN*nXB )
			d %= nAN*nXB;
		if( x > nX )
			x %= nX;*/


		if( typ().w > 2 )
		{
			if( typ().w > 4 )
				((I8*)(pD+d))[x] /= a.i8();
			else
				((I4*)(pD+d))[x] /= a.i8();
		}
		else if( typ().w > 1 )
			((I2*)(pD+d))[x] /= a.i8();
		else
			((I1*)(pD+d))[x] /= a.i8();

		return *this;
	}
	/// unSIGNED ----------------------------------
	pD = ALUdat( pRM, 0, t, 0, a.u8(), 0.0  );
	if( !pD )
		return *this;

	U4		d = (a.xy-sub)*trafo();
		if( d > AN().w )
			d %= AN().w;
		x %= tDIM().area();

		/*U4x2 	X( typ.y, typ.z );

	U4 		nAN = AN.a4x2[0].area(),
			nB	= typ.w,
			nX	= X.area(),
			nXB	= nX*nB;
	U4x2	T( nXB, nXB*AN.a4x2[0].x );
	U4		d =  (a.xy-sub)*T;
	if( d > nAN*nXB )
		d %= nAN*nXB;
	if( x > nX )
		x %= nX;*/


	if( typ().w > 2 )
	{
		if( typ().w > 4 )
			((U8*)(pD+d))[x] /= a.u8();
		else
			((U4*)(pD+d))[x] /= a.u8();
	}
	else if( typ().w > 1 )
		((U2*)(pD+d))[x] /= a.u8();
	else
		((U1*)(pD+d))[x] /= a.u8();

	return *this;
}

gpcALU& gpcALU::operator %= ( gpcREG& a )
{
	//U4x2 xy = 0;
	U4 x = 0;

	U1x4 t = 0;
	t.x = a.t();
	U1* pD = NULL;
	if( t.x&0x40 ) {	/// float ----------------------------------
		pD = ALUdat( pRM, a.xy, t, I1x4(0), 0, a.d8() );
		if( !pD )
			return *this;

		U4		d = (a.xy-sub)*trafo();
		if( d > AN().w )
			d %= AN().w;
		x %= tDIM().area();

		/*U4x2 	X( typ.y, typ.z );
		U4 		nAN = AN.a4x2[0].area(),
				nB	= typ.w,
				nX	= X.area(),
				nXB	= nX*nB;
		U4x2	T( nXB, nXB*AN.a4x2[0].x );
		U4		d = (a.xy-sub)*T;
		if( d > nAN*nXB )
			d %= nAN*nXB;
		if( x > nX )
			x %= nX;*/

		double dd, ad = abs(a.d8());
		if( typ().w > 4 )
			dd = abs(((double*)(pD+d))[x]);
		else
			dd = abs(((float*)(pD+d))[x]);

        U8 u = dd/ad;
		dd -= u;

		if( typ().w > 4 )
			((double*)(pD+d))[x] = dd;
		else
			((float*)(pD+d))[x] = dd;

		return *this;
	}
	if( t.x&0x80 ) {	/// SIGNED ----------------------------------
		pD = ALUdat( pRM, 0, t, I1x4(-1), a.u8(), 0.0 );
		if( !pD )
			return *this;

		U4		d = (a.xy-sub)*trafo();
		if( d > AN().w )
			d %= AN().w;
		x %= tDIM().area();

		/*U4x2 	X( typ.y, typ.z );
		U4 		nAN = AN.a4x2[0].area(),
				nB	= typ.w,
				nX	= X.area(),
				nXB	= nX*nB;
		U4x2	T( nXB, nXB*AN.a4x2[0].x );
		U4		d =  (a.xy-sub)*T;
		if( d > nAN*nXB )
			d %= nAN*nXB;
		if( x > nX )
			x %= nX;*/


		if( typ().w > 2 )
		{
			if( typ().w > 4 )
				((I8*)(pD+d))[x] %= a.i8();
			else
				((I4*)(pD+d))[x] %= a.i8();
		}
		else if( typ().w > 1 )
			((I2*)(pD+d))[x] %= a.i8();
		else
			((I1*)(pD+d))[x] %= a.i8();

		return *this;
	}
	/// unSIGNED ----------------------------------
	pD = ALUdat( pRM, 0, t, 0, a.u8(), 0.0  );
	if( !pD )
		return *this;

	U4		d = (a.xy-sub)*trafo();
		if( d > AN().w )
			d %= AN().w;
		x %= tDIM().area();

		/*U4x2 	X( typ.y, typ.z );

	U4 		nAN = AN.a4x2[0].area(),
			nB	= typ.w,
			nX	= X.area(),
			nXB	= nX*nB;
	U4x2	T( nXB, nXB*AN.a4x2[0].x );
	U4		d =  (a.xy-sub)*T;
	if( d > nAN*nXB )
		d %= nAN*nXB;
	if( x > nX )
		x %= nX;*/


	if( typ().w > 2 )
	{
		if( typ().w > 4 )
			((U8*)(pD+d))[x] %= a.u8();
		else
			((U4*)(pD+d))[x] %= a.u8();
	}
	else if( typ().w > 1 )
		((U2*)(pD+d))[x] %= a.u8();
	else
		((U1*)(pD+d))[x] %= a.u8();

	return *this;
}

gpcALU& gpcALU::operator = ( U1* pSTR )
{
	if( pSTR ? !*pSTR : true )
	{
		// törölni kell nem volt benne semmi
		return *this;
	}

	U4 n = gpmSTRLEN( pSTR );

	U1* pD = ALUdat( pRM, U4x2(n,0), gpeTYP_STR, I1x4(0) );
	if( !pD )
		return *this;
	gpmMEMCPY( pD, pSTR, n );
	pD[n] = 0;
	return *this;
}
