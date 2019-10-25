
#include "gpcres.h"
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
			// yz[ dimXY ] 		, w[] = nBYTE*dimXY
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
	gpmDELary( pTREE );
	gpmDELary( pTx );
	gpmDELary( pISA );
	//gpmDELary( pALU );
	return this;
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
gpcALU& gpcADR::ALU( gpcRES* pM )
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
gpcRES* gpcALU::ins( gpcRES* pM, gpcRES* pKID, gpcLAZY& str )
{
	gpcADR adr = alf;
	adr = pM;
	U8 nLAB = 0;
	while( adr.pRM )	// ez azért, hogy hatékonyabb legyen a keresés, a binFA ne egyetlen jobb ág legyen
	{
		// nem talált ilyen változót
		adr = (gpeALF)( 1 + U4x2(0).cnt2fract( (U4)gpeALF_AAAAAA, nLAB ) * U4x2( 1, (U4)gpeALF_AAAAAA) );
		adr = pM; // ezzel indítjuk a keresést
		nLAB++;
	}


	if( !pKID )
		return NULL;

	if( !pM )
		return pKID;


	gpmDEL(pKID);
	return pKID;
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
	typ = 0;
	AN = 0;
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
		*this = pM->ADD( adr.an.alf, ty4.u4, 0 );
	} else
		adr = pM;

	bool	bS1 = typ.x&0x80,
			bF1 = typ.x&0x40,
			bS2 = (typ.x|ty4.x)&0x80;

	if( bF1 )
		return int2flt( pM, xy, ty4 );

	gpcALU tmp;
	tmp.typ = typ;
	tmp.pDAT = pDAT;
	tmp.AN.a4x2[0] = AN.a4x2[0];

	typ.typMX( ty4 );		// typ.x[0b 1w 2l 3q]  // 1half? 2float 3double

	AN.a4x2[0].mx( xy+1 );
	U4x2 	X1( tmp.typ.y, tmp.typ.z ),
			X2( typ.y, typ.z );
	X2.mx( X1 );


	U4	nAN1	= tmp.AN.a4x2[0].area(),
		nX1		= X1.area(),
		nB1		= tmp.typ.w,
		nXB1	= nX1*nB1,
		nLD1	= nAN1*nXB1,

		nAN2	= AN.a4x2[0].area(),
		nX2		= X2.area(),
		nB2		= typ.w,
		nXB2	= nX2*nB2,
		nLD2	= nAN2*nXB2;

	if( pDAT )
	if( nLD2 != nLD1 )
		pDAT = NULL;

	if( pDAT )
		return *this;

	U1		*pS = (U1*)tmp.pDAT,
			*pD = new U1[nLD2];
	gpmZn( pD, nLD2 );
	pDAT = pD;

	if( !pS )
	{
		pM->chg( *this );
		return *this;
	}

	U4x2	T1( nXB1, nXB1*tmp.AN.a4x2[0].x ), xyS,
			T2( nXB2, nXB2*AN.a4x2[0].x );
	U8	u8;
	I8	i8;
	U4	s, d, x,
		sx = tmp.AN.a4x2[0].x,
		sy = tmp.AN.a4x2[0].y;

	if( (typ.u4 == tmp.typ.u4) && ( T1.y == T2.y ) )
	{
		memcpy( pD, pS, T1.y*sy );
	}
	else for( xyS = 0; xyS.y < sy; xyS.y++ )
	if( typ.u4 == tmp.typ.u4 )
	{
		s = xyS*T1;
		d = xyS*T2;
		memcpy( pD+d, pS+s, T1.y );
	} else {
		for( xyS.x = 0; xyS.x < sx; xyS.x++ )
		{
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
gpcALU& gpcALU::int2flt( gpcRES* pM, U4x2 xy, U1x4 ty4 ) {

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
		*this = pM->ADD( adr.an.alf, ty4.u4, 0 );
	} else
		adr = pM;

	bool	bS1 = typ.x&0x80,
			bF1 = typ.x&0x40;

	gpcALU tmp;
	tmp.typ = typ;
	tmp.pDAT = pDAT;
	tmp.AN.a4x2[0] = AN.a4x2[0];

	AN.a4x2[0].mx( xy+1 );
	ty4.x |= 0x40;
	typ.typMX( ty4 );	// typ.x[0b 1w 2l 3q]  // 1half? 2float 3double
	U4x2 	X1( tmp.typ.y, tmp.typ.z ),
			X2( typ.y, typ.z );
	X2.mx( X1 );


	U4	nAN1	= tmp.AN.a4x2[0].area(),
		nX1		= X1.area(),
		nB1		= tmp.typ.w,
		nXB1	= nX1*nB1,
		nLD1	= nAN1*nXB1,

		nAN2	= AN.a4x2[0].area(),
		nX2		= X2.area(),
		nB2		= typ.w,
		nXB2	= nX2*nB2,
		nLD2	= nAN2*nXB2;

	if( nLD2 != nLD1 )
		pDAT = NULL;

	if( pDAT )
	{
		// B X AN is azonos
		if( bF1 )
			return *this;

		// de nem float
		if( nB2 > 4 )
		{
			if( bS1 )
			{
				for( U4 i = 0, ie = nLD1/8; i < ie; i++ )
				{
					((double*)pDAT)[i] = ((I8*)pDAT)[i];
				}
			}
			else for( U4 i = 0, ie = nLD1/8; i < ie; i++ )
			{
				((double*)pDAT)[i] = ((U8*)pDAT)[i];
			}

		} else {
			if( bS1 )
			{
				for( U4 i = 0, ie = nLD1/4; i < ie; i++ )
				{
					((float*)pDAT)[i] = ((I4*)pDAT)[i];
				}
			}
			else for( U4 i = 0, ie = nLD1/4; i < ie; i++ )
			{
				((float*)pDAT)[i] = ((U4*)pDAT)[i];
			}
		}

		pM->chg( *this );
		return *this;
	}

	U1		*pS = (U1*)tmp.pDAT,
			*pD = new U1[nLD2];
	gpmZn( pD, nLD2 );
	pDAT = pD;

	if( !pS )
	{
		pM->chg( *this );
		return *this;
	}

	U4x2	T1( nXB1, nXB1*tmp.AN.a4x2[0].x ), xyS,
			T2( nXB2, nXB2*AN.a4x2[0].x );
	double	src;
	U4	s, d, x,
		sx = tmp.AN.a4x2[0].x,
		sy = tmp.AN.a4x2[0].y;

	for( xyS = 0; xyS.y < sy; xyS.y++ )
	if( bF1 )
	{
		// az elem mérete stimmel
		// csak a cél sorának hossza nagyobb
		if( nXB1 == nXB2 )
		{
			memcpy( pD+xyS*T2, pS+xyS*T1, T1.y );
			continue;
		}

		for( xyS.x = 0; xyS.x < sx; xyS.x++ )
		{
			s = xyS*T1;
			d = xyS*T2;
			if( nB1 == nB2 )
			{
				for( x = 0; x < nX1; x++ )
				{
					memcpy( pD+d, pS+s, nXB1 );
				}
				continue;
			}

			// nB2 itt nagyobb lesz
			// azaz ez double
			for( x = 0; x < nX1; x++ )
				((double*)(pD+d))[x] = ((float*)(pS+s))[x];
		}
	} else {
		for( xyS.x = 0; xyS.x < sx; xyS.x++ )
		{
			s = xyS*T1;
			d = xyS*T2;
			if( bS1 )
			{
				if( nB2 > 4 )
				{
					for( x = 0; x < nX1; x++ )
					{
						if( nB1 > 2 )
						{
							if( nB1 > 8 )
							{
								// I8
								((double*)(pD+d))[x] = ((I8*)(pS+s))[x];
								continue;
							}
							// I4
							((double*)(pD+d))[x] = ((I4*)(pS+s))[x];
							continue;
						}

						if( nB1 > 1 )
						{
							// I2
							((double*)(pD+d))[x] = ((I2*)(pS+s))[x];
							continue;
						}
						// I1
						((double*)(pD+d))[x] = ((I1*)(pS+s))[x];
					}
					continue;
				}

				for( x = 0; x < nX1; x++ )
				{
					if( nB1 > 2 )
					{
						if( nB1 > 8 )
						{
							// I8
							((float*)(pD+d))[x] = ((I8*)(pS+s))[x];
							continue;
						}
						// I4
						((float*)(pD+d))[x] = ((I4*)(pS+s))[x];
						continue;
					}

					if( nB1 > 1 )
					{
						// I2
						((float*)(pD+d))[x] = ((I2*)(pS+s))[x];
						continue;
					}
					// I1
					((float*)(pD+d))[x] = ((I1*)(pS+s))[x];
				}
				continue;
			}

			if( nB2 > 4 )
			{
				for( x = 0; x < nX1; x++ )
				{
					if( nB1 > 2 )
					{
						if( nB1 > 8 )
						{
							// I8
							((double*)(pD+d))[x] = ((U8*)(pS+s))[x];
							continue;
						}
						// I4
						((double*)(pD+d))[x] = ((U4*)(pS+s))[x];
						continue;
					}

					if( nB1 > 1 )
					{
						// I2
						((double*)(pD+d))[x] = ((U2*)(pS+s))[x];
						continue;
					}
					// I1
					((double*)(pD+d))[x] = ((U1*)(pS+s))[x];
				}
				continue;
			}

			for( x = 0; x < nX1; x++ )
			{
				if( nB1 > 2 )
				{
					if( nB1 > 8 )
					{
						// I8
						((float*)(pD+d))[x] = ((U8*)(pS+s))[x];
						continue;
					}
					// I4
					((float*)(pD+d))[x] = ((U4*)(pS+s))[x];
					continue;
				}

				if( nB1 > 1 )
				{
					// I2
					((float*)(pD+d))[x] = ((U2*)(pS+s))[x];
					continue;
				}
				// I1
				((float*)(pD+d))[x] = ((U1*)(pS+s))[x];
			}
		}
	}

	pM->chg( *this );
	return *this;
}


gpcALU& gpcALU::equ( gpcRES* pM, U4x2 xy, U1x4 ty4, I1x4 op4, U8 u8, double d8, U2 x )
{
	if( !pM )
		return null();

	if( ty4.y < 1 )
		ty4.y = 1;
	if( ty4.z < 1 )
		ty4.z = 1;

	U1 OR = typ.x|ty4.x;
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

	ins( pM, xy, ty4 );
	U1* pD = (U1*)pDAT;
	if( !pD )
		return *this;


	U4x2 	X( typ.y, typ.z );

	U4 		nAN = AN.a4x2[0].area(),
			nB	= typ.w,
			nX	= X.area(),
			nXB	= nX*nB;
	U4x2	T( nXB, nXB*AN.a4x2[0].x );
	U4		d = xy*T;
	if( d > nAN*nXB )
		d %= nAN*nXB;
	if( x > nX )
		x %= nX;


	if( nB > 2 )
	{
		if( nB > 4 )
			((U8*)(pD+d))[x] = u8;
		else
			((U4*)(pD+d))[x] = u8;
	}
	else if( nB > 1 )
		((U2*)(pD+d))[x] = u8;
	else
		((U1*)(pD+d))[x] = u8;

	return *this;
}

gpcALU& gpcALU::equSIG( gpcRES* pM, U4x2 xy, U1x4 ty4, I1x4 op4, U8 u8, U2 x )
{
	if( !pM )
		return null();

	bool bSIG = pDAT ? typ.x&0x80 : true;

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
	if( (ty4.x&0xf) <= (typ.x&0xf)  )
	{
		// akkor nem jó ha ugyan akkorát akarunk
		ty4.x = 0x80 + min( 3, (typ.x&0xf)+1 );
	}


	I8 i8 = ( op4.x < 0 ) ? -u8 : u8;

	ins( pM, xy, ty4 );
	U1* pD = (U1*)pDAT;
	if( !pD )
		return *this;


	U4x2 	X( typ.y, typ.z );

	U4 		nAN = AN.a4x2[0].area(),
			nB	= typ.w,
			nX	= X.area(),
			nXB	= nX*nB;
	U4x2	T( nXB, nXB*AN.a4x2[0].x );
	U4		d = xy*T;
	if( d > nAN*nXB )
		d %= nAN*nXB;
	if( x > nX )
		x %= nX;


	if( nB > 2 )
	{
		if( nB > 4 )
			((I8*)(pD+d))[x] = i8;
		else
			((I4*)(pD+d))[x] = i8;
	}
	else if( nB > 1 )
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

	int2flt( pM, xy, ty4 );
	U1* pD = (U1*)pDAT;
	if( !pD )
		return *this;


	U4x2 	X( typ.y, typ.z );

	U4 		nAN = AN.a4x2[0].area(),
			nB	= typ.w,
			nX	= X.area(),
			nXB	= nX*nB;
	U4x2	T( nXB, nXB*AN.a4x2[0].x );
	U4		d = xy*T;
	if( d > nAN*nXB )
		d %= nAN*nXB;
	if( x > nX )
		x %= nX;

	if( nB > 4 )
		((double*)(pD+d))[x] = d8;
	else
		((float*)(pD+d))[x] = d8;

	return *this;
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
