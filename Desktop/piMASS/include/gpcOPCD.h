#ifndef GPCOPCD_H_INCLUDED
#define GPCOPCD_H_INCLUDED

#include "gpcSRC.h"

class gpcOPCD
{
public:
	I2		nADD, nMUL, nWIP;
	I8		i8;
	U8		u8, nSTR, nLEN;
	gpeALF	lab, typ;
	U1		*pSTR;
	double	d;
	gpcOPCD(){};
	//gpcOPCD( const char* pS, char a, char m, I8 i, U8 u, double _d, gpeALF t );
	gpcOPCD( const gpcOPCD* pTHIS, const char* pS, char a, char m, I8 i, U8 u, double _d, gpeALF t );

	gpcOPCD& null()
	{
		gpmCLR;
	}
};

#endif // GPCOPCD_H_INCLUDED
