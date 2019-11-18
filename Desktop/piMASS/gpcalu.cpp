#include "gpcwin.h"
//#include "gpcres.h"
extern U1 gpaALFadd[];

gpcALU& gpcALU::operator += ( gpcALU& b )
{
	if( !b.pDAT )
		return *this;

	//if( b.AN.a4x2[0].area()*(b.typ.y*b.typ.z) == 1 )
	{
		gpcREG bb;
		if( b.typ.x&0x40 )
			bb = b.d8();
		else if( b.typ.x&0x80 )
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
		if( b.typ.x&0x40 )
			bb = b.d8();
		else if( b.typ.x&0x80 )
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
		if( b.typ.x&0x40 )
			bb = b.d8();
		else if( b.typ.x&0x80 )
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
		if( b.typ.x&0x40 )
			bb = b.d8();
		else if( b.typ.x&0x80 )
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
		if( b.typ.x&0x40 )
			bb = b.d8();
		else if( b.typ.x&0x80 )
			bb = b.i8();
		else
			bb = b.u8();

		return *this /= bb;
	}


	return *this;
}
