#ifndef GPCOPCD_H_INCLUDED
#define GPCOPCD_H_INCLUDED

#include "gpcSRC.h"
#include "gpcres.h"

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
	gpcOPCD( const gpcOPCD* pTHIS, const char* pS, char a, char m, I8 i, U8 u, double _d, gpeALF t, gpeALF df = gpeALF_null );

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


class gpcKID
{
public:
	gpcLZYdct	*p_kid;
	gpcLZY		*p_iPC;

};
class gpcCMPL
{
public:
	gpeALF	typ, wip;
	U4		iPC, mPC, iKD, iLEV,
			i_dat, n_dat,
			iPUB,
			n_str,
			iDEF, iDEC, iCALL,
			iSPARE, 	// tartalék
						// pl:
						// 1.) U1x4 xyzw(1,2,3,4);			// ez egy változó inicializálás
						// 2.) U1x4 xyzw(1,2,3,4){ .... };	// ez egy fügvény definiálás
						// Melyik is?
			iREDIR, iINI,
			op;

	I8		i8;
	U8		u8;
	double	d;

	I2		nADD, nMUL; //, nASS;
	char*	pINFO;

	gpcLZYdct	*p_kid;
	gpcLZY		*p_iPC;

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
	I1 sDST( U1* pPUB, U4 iFND, char* pS0, const char* pTAB  = "", const char* pSTR = "" );
	gpcLZY* reset( gpcLZY* pCMPL, U1* pPUB )
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

	U4 iKID( gpcLZY* pCMPL, U4 i );


	U4 cmpl_best( gpcLZY* pCMPL, U1* pS, U4 nS );
	U4 cmpl_find( gpcLZY* pCMPL, U1* pS, U4 nS );
	gpcLZY* cmpl_add( gpcLZY* pCMPL, U1* pS, U4 nS );

	gpcCMPL* sKIDlst( U1* pS0, U1* pPUB , gpcLZY* pCMPL, char c = 0 );
	char* sLOG( U1* pPUB, char* pTAB, char* sNDAT, gpcLZY* pCMPL );
	char* sASM( U1* pS0, U1* pPUB, char* sNDAT, gpcLZY* pCMPL, gpcCMPL*pA, gpcCMPL*pB );
};


class gpcCMPLlev
{
public:
	gpcCMPL	*pDEF, *pMOM, *pDotM, *pDot, *pA, *pOP, *pB, *pFND, *pCALL;
	U4 iLEV, nLEV, iDAT, iOFF;
	U1 c;
	gpcCMPLlev(){};
	gpcCMPLlev* DAoBclr( void )
	{
		if( !this )
			return NULL;

		pDEF = pA = pOP = pB = NULL;

		return this;
	}
	gpcCMPLlev* DAoBCclr( void )
	{
		if( !this )
			return NULL;

		pDEF = pA = pOP = pB = pCALL = NULL;

		return this;
	}
	gpcCMPLlev* AoBclr( void )
	{
		if( !this )
			return NULL;

		pA = pOP = pB = NULL;

		return this;
	}
	gpcCMPLlev* AoBCclr( void )
	{
		if( !this )
			return NULL;

		pA = pOP = pB = pCALL = NULL;

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
			pLEV = pLEV->inc( NULL );

		return pLEV;
 	}
 	gpcCMPLlev* inc( gpcCMPL* pM, gpcCMPL* pF = NULL )
 	{
		memcpy( this+1, this , sizeof(*this) );
		this[1].iLEV = iLEV+1;
		this[1].pDotM =
		this[1].pDot = NULL;
		this[1].iOFF = 0;
		//pFND = pF;
		if( pM )
		{
			this[1].pFND = pF;
			this[1].pMOM = pM;
		}
		return this+1;
 	}
	gpcCMPLlev* dec( U4 floor )
	{
		if( iLEV > floor )
		{
			return this-1;
		}
		return this;
	}

};



#endif // GPCOPCD_H_INCLUDED

