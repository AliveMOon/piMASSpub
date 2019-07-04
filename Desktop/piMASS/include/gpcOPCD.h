#ifndef GPCOPCD_H_INCLUDED
#define GPCOPCD_H_INCLUDED

#include "gpcSRC.h"
enum gpePRG : I2
{
	gpePRG_newclass = -1,	//r0
	gpePRG_typ_name,	//r1
	gpePRG_var_name,	//r2
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
	gpePRG_end,		//r0x10
};

class gpcOPCD
{
public:
	I2		nADD, nMUL;

	I2		nWIP;

	U4		iLEV, ixDCT,
			iDAT, nDAT;

	I8		i8;
	U8		u8, nSTR, nLEN;
	gpeALF	lab, typ, wip, aTYP[gpePRG_end];
	U1		*pSTR,
			*apSTR[gpePRG_end],
			iSTR;
	double	d;
	gpcOPCD(){};
	//gpcOPCD( const char* pS, char a, char m, I8 i, U8 u, double _d, gpeALF t );
	gpcOPCD( const gpcOPCD* pTHIS, const char* pS, char a, char m, I8 i, U8 u, double _d, gpeALF t, gpeALF df = gpeALF_zero );

	gpcOPCD& null()
	{
		gpmCLR;
	}
};


#endif // GPCOPCD_H_INCLUDED
class gpcCMPL
{
public:
	gpeALF	typ, wip;
	U4		iPC, mPC, iLEV,
			i_dat, n_dat,
			i_str, n_str,
			iDEF, iDEC, iINI;


	I2		nADD, nMUL, nASS;
	char*	pINFO;

	gpcLZYdct	*p_kid;
	gpcLAZY		*p_iPC;

	gpcLAZY* reset( gpcLAZY* pCMPL )
	{
		pCMPL = cmpl_add( pCMPL, (U1*)"false", strlen("false") );

		return pCMPL;
	}
	gpcCMPL& null( void )
	{
		gpmCLR;
	}
	gpcCMPL(){};
	gpcCMPL( I8 mom )
	{
		gpmCLR;
		mPC = mom;
	}
	U4 nPC( gpcLAZY* pCMPL )
	{
		gpcCMPL	**ppC = pCMPL ? (gpcCMPL**)pCMPL->p_alloc : NULL;
		if( !ppC )
			return 0;

		return pCMPL->n_load/sizeof(this);

	}
	gpcCMPL* pPC( gpcLAZY* pCMPL, U4 pc )
	{
		gpcCMPL	**ppC = pCMPL ? (gpcCMPL**)pCMPL->p_alloc : NULL;
		if( !ppC )
			return NULL;
		if( pc < pCMPL->n_load/sizeof(this) )
			return ppC[pc];
		return NULL;
	}
	U4 cmpl_find( gpcLAZY* pCMPL, U1* pS, U4 nS );
	gpcLAZY* cmpl_add( gpcLAZY* pCMPL, U1* pS, U4 nS );



};
