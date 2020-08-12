#include "gpcwin.h"
extern U1 gpaALFsub[];

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
			bF1 = typ().x&0x40;

	gpcALU tmp;
	tmp.TanDT(*this);
	/*tmp.typ = typ;
	tmp.pDAT = pDAT;
	tmp.AN.a4x2[0] = AN.a4x2[0];*/


	ANmx( xy+1 ); //AN.a4x2[0].mx( xy+1 );
	ty4.x |= 0x40;
	TYPmx(ty4.u4); // typ.typMX( ty4 );	// typ.x[0b 1w 2l 3q]  // 1half? 2float 3double
	U4x2 	X1 = tmp.tDIM(), // ( tmp.typ.y, tmp.typ.z ),
			X2 = tDIM(); // ( typ.y, typ.z );
	X2.mx( X1 );


	U4	nAN1	= tmp.AN().z, //.a4x2[0].area(),
		nX1		= X1.area(),
		nB1		= tmp.typ().w,
		nXB1	= nX1*nB1,
		nLD1	= tmp.AN().w, //nAN1*nXB1,

		nAN2	= AN().z, //.a4x2[0].area(),
		nX2		= X2.area(),
		nB2		= typ().w,
		nXB2	= nX2*nB2,
		nLD2	= AN().w; //nAN2*nXB2;

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
			*pD = new U1[nALLOC()];
	gpmZn( pD, AN().w );
	pD[AN().w] = 0;
	pDAT = pD;

	if( !pS )
	{
		pM->chg( *this );
		return *this;
	}

	U4x2	T1 = tmp.trafo(), // ( nXB1, nXB1*tmp.AN.a4x2[0].x ),
			xyS,
			T2 = trafo(); //( nXB2, nXB2*AN.a4x2[0].x );
	double	src;
	U4	s, d, x,
		sx = tmp.AN().x, //.AN.a4x2[0].x,
		sy = tmp.AN().y; //.AN.a4x2[0].y;

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
