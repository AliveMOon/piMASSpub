#ifndef GPCOPCD_H_INCLUDED
#define GPCOPCD_H_INCLUDED

#include "gpcSRC.h"
enum gpePRG : I2
{
	gpePRG_reset,	//r0
	gpePRG_class,	//r1
	gpePRG_type,	//r2
	gpePRG_typec,	//r3
	gpePRG_name,	//r4
	gpePRG_ready,	//r5
	gpePRG_r0x6,
	gpePRG_r0x7,
	gpePRG_r0x8,
	gpePRG_r0x9,
	gpePRG_r0xA,
	gpePRG_r0xB,
	gpePRG_r0xC,
	gpePRG_r0xD,
	gpePRG_r0xE,
	gpePRG_r0xF,
	gpePRG_all,		//r0x10
};

class gpcOPCD
{
public:
	I2		nADD, nMUL;
	//gpePRG
	I2		nWIP;

	U4		alvDCT[2],
			aixDCT[2],
			iDT, nDT;

	I8		i8;
	U8		u8, nSTR, nLEN;
	gpeALF	lab, typ;
	U1		*pSTR,
			*apSTR[gpePRG_all];
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
