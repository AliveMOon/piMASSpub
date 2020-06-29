#include "gpcwin.h"
//#include "gpcres.h"
extern U1 gpaALFsub[];
U1* gpcALU::ALUdatHARD( gpcRES* pM, U4x2 xy, U1x4 ty4, I1 mul )
{
	if( !pM )
		return NULL;

	xy -= sub;

	// typ:
	/// x[7s,6f,5r,4str : 3-0 nBYTE = 1<<(x&0xf) ]
	// yz[ dimXY ] 		, w[] nBYTE = 1<<(x&0xf)
	if( !(ty4.u4&gpeTYP_STRmsk) )
	{
		if( pRM == pM )
		if( !pDAT )				// nincs forrás
		//if( (d8+u8) == 0.0 )
		{
			//xy -= sub;
			ANmx( xy+1 ); //AN.a4x2[0].mx( xy+1 );
			typ( gpeTYP_U1 );
			TYPmx(ty4.u4); // if(ty4.x) typ.x |= ty4.x;

			U4 nAN2 = nALLOC(); //gpmPAD( AN().w, 0x10 ); //.a4x2[0].area()+1, 0x10 );
			pDAT = new U1[nAN2];
			gpmZnOF( (U1*)pDAT, AN().w );
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

	U1 OR = !pDAT ? ty4.x : typ().x|ty4.x;
	if( OR&0x40 )
	{
		// return equ( pM, xy, ty4, op4, d8, x );
		ty4.x = ((ty4.x&0xf) < 2 ) ? 0xc2 : 0xc3;
		int2flt( pM, xy, ty4 );
		return (U1*)pDAT;
	}
    if( !pDAT )
	{
		ins( pM, xy, ty4 );
		return (U1*)pDAT;
	}

	if( OR&0x80 )
		mul = !(typ().x&0x80);

	if( mul > 0 )
	{
		ty4.x = (ty4.x&0xf0) | ((typ().x&0xf) < 2 ? (typ().x&0xf)+1 : 3);
	}
	else if( mul < 0 )
	{
		ty4.x = (ty4.x&0xf0) | ( (typ().x&0xf) > 1 ? (typ().x&0xf)-1 : 0  );
	}

	ins( pM, xy, ty4 );
	return (U1*)pDAT;
}

gpcALU& gpcALU::operator += ( gpcALU& b )
{
	if( !b.pDAT )
		return *this;

	//if( b.AN.a4x2[0].area()*(b.typ.y*b.typ.z) == 1 )
	{
		gpcREG bb;
		if( b.type.x&0x40 )
			bb = b.d8();
		else if( b.type.x&0x80 )
			bb = b.i8();
		else
			bb = b.u8();

		return *this += bb;
	}


	return *this;
}

gpcALU& gpcALU::operator -= ( gpcALU& b )
{
	if( !b.pDAT )
		return *this;

	//if( b.AN.a4x2[0].area()*(b.typ.y*b.typ.z) == 1 )
	{
		gpcREG bb;
		if( b.type.x&0x40 )
			bb = b.d8();
		else if( b.type.x&0x80 )
			bb = b.i8();
		else
			bb = b.u8();

		return *this += bb;
	}


	return *this;
}
gpcALU& gpcALU::operator *= ( gpcALU& b )
{
	if( !b.pDAT )
		return *this;

	//if( b.AN.a4x2[0].area()*(b.typ.y*b.typ.z) == 1 )
	{
		gpcREG bb;
		if( b.type.x&0x40 )
			bb = b.d8();
		else if( b.type.x&0x80 )
			bb = b.i8();
		else
			bb = b.u8();

		return *this *= bb;
	}


	return *this;
}
gpcALU& gpcALU::operator /= ( gpcALU& b )
{
	if( !b.pDAT )
		return *this;

	//if( b.AN.a4x2[0].area()*(b.typ.y*b.typ.z) == 1 )
	{
		gpcREG bb;
		if( b.type.x&0x40 )
			bb = b.d8();
		else if( b.type.x&0x80 )
			bb = b.i8();
		else
			bb = b.u8();

		return *this /= bb;
	}


	return *this;
}
gpcALU& gpcALU::operator %= ( gpcALU& b )
{
	if( !b.pDAT )
		return *this;

	//if( b.AN.a4x2[0].area()*(b.typ.y*b.typ.z) == 1 )
	{
		gpcREG bb;
		if( b.type.x&0x40 )
			bb = b.d8();
		else if( b.type.x&0x80 )
			bb = b.i8();
		else
			bb = b.u8();

		return *this /= bb;
	}


	return *this;
}
