#ifndef GPCOPCD_H_INCLUDED
#define GPCOPCD_H_INCLUDED

#include "gpcSRC.h"
/*enum gpePRG : I2
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
};*/

class gpcOPCD
{
public:
	I2		nADD, nMUL;

	I2		nWIP;

	U4		iLEV, ixDCT,
			iDAT, nDAT;

	I8		i8;
	U8		u8, nSTR, nLEN;
	gpeALF	lab, typ, wip; //, aTYP[gpePRG_end];
	U1		*pSTR,
			//*apSTR[gpePRG_end],
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

enum gpeKID:U1
{
	gpeKIDstuff,
	gpeKIDcons,
	gpeKIDfunc,
	gpeKIDpara,
	gpeKIDn,
};

#endif // GPCOPCD_H_INCLUDED
class gpcKID
{
public:
	gpcLZYdct	*p_kid;
	gpcLAZY		*p_iPC;

};
class gpcCMPL
{
public:
	gpeALF	typ, wip;
	U4		iPC, mPC, iKD, iLEV,
			i_dat, n_dat,
			iPUB,
			n_str,
			iDEF, iDEC, iINI,
			op;

	I8		i8;
	U8		u8;
	double	d;

	I2		nADD, nMUL, nASS;
	char*	pINFO;

	gpcLZYdct	*p_kid;
	gpcLAZY		*p_iPC;

	gpeKID kid( gpeALF t )
	{
		switch(t)
		{
			case gpeALF_FUNC:
				return gpeKIDfunc;
			case gpeALF_CONSTR:
				return gpeKIDcons;
			case gpeALF_PARA:
				return gpeKIDpara;
			default:
				return gpeKIDstuff;
		}
		return gpeKIDstuff;
	}
	I1 sOP( char* pPUB );
	I1 sDST( U1* pPUB, U4 iFND, char* pS0, char* pTAB  = "", char* pSTR = "" );
	gpcLAZY* reset( gpcLAZY* pCMPL, U1* pPUB )
	{
		pCMPL = cmpl_add( pCMPL, pPUB, strlen((char*)pPUB) );

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

	U4 nKID( void )
	{
		if( !this )
			return 0;

		U4 n = p_iPC ? p_iPC->n_load/sizeof(U4) : 0;
		return n;
	}

	U4 iKID( gpcLAZY* pCMPL, U4 i );

	/*U4 nPC( gpcLAZY* pCMPL )
	{
		gpcCMPL	**ppC = pCMPL ? (gpcCMPL**)pCMPL->p_alloc : NULL;
		if( !ppC )
			return 0;

		return pCMPL->n_load/sizeof(this);

	}*/
	gpcCMPL* pPC( gpcLAZY* pCMPL, U4 pc )
	{
		gpcCMPL	**ppC = pCMPL ? (gpcCMPL**)pCMPL->p_alloc : NULL;
		if( !ppC )
			return NULL;
		if( pc < pCMPL->n_load/sizeof(this) )
			return ppC[pc];
		return NULL;
	}
	U4 cmpl_best( gpcLAZY* pCMPL, U1* pS, U4 nS );
	U4 cmpl_find( gpcLAZY* pCMPL, U1* pS, U4 nS );
	gpcLAZY* cmpl_add( gpcLAZY* pCMPL, U1* pS, U4 nS );

	gpcCMPL* sKIDlst( U1* pS0, U1* pPUB , gpcLAZY* pCMPL, char c = 0 );
	char* sDECL( U1* pPUB, char* sNDAT, gpcLAZY* pCMPL );
	char* sASM( U1* pS0, U1* pPUB, char* sNDAT, gpcLAZY* pCMPL, gpcCMPL*pA, gpcCMPL*pB );
};


class gpcCMPLlev
{
public:
	gpcCMPL	*pDEF, *pMOM, *pA, *pOP, *pB;
	U4 iLEV, nLEV;
	gpcCMPLlev(){};

	gpcCMPLlev* AoBclr( void )
	{
		if( !this )
			return NULL;

		pA = pOP = pB = NULL;

		return this;
	}
	gpcCMPLlev* newROOT( U4 n = 0x100 )
	{
		if( this )
			return this;

		gpcCMPLlev* pLEV = new gpcCMPLlev[n];
		gpmZ( *pLEV );
		pLEV->nLEV = n;
		return pLEV;
 	}
 	gpcCMPLlev* get( U4 i )
 	{
		gpcCMPLlev* pLEV = this;
		while( pLEV->iLEV < i)
		{
			pLEV = pLEV->inc();
		}
		return pLEV;
 	}
 	gpcCMPLlev* inc( gpcCMPL* pM )
 	{
		memcpy( this+1, this , sizeof(*this) );
		this[1].iLEV = iLEV+1;
		if( pM )
			this[1].pMOM = pM;
		return this+1;
 	}
	gpcCMPLlev* dec( void )
	{
		if( iLEV )
			return this-1;


	}

};

