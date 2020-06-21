#ifndef GPCSRCLNK_H
#define GPCSRCLNK_H
#include "piMASS.h"

#define OBJ SCOOP.obj
/*
#ifdef piMASS_DEBUG
	/// gpcCDR
	#define cd (pCD=CDsp.CD())[0]
	/// gpcOBJlnk
	#define OBJget (pOBi=(gpcOBJlnk*)OBJ.Ux( (cd.obj-iOPe), sizeof(gpcOBJlnk)))[0]
	#define OBJadd (pOBn=(gpcOBJlnk*)OBJ.Ux( SCOOP.nOBJ, sizeof(gpcOBJlnk)))[0]
#else
	/// gpcCDR
	#define cd CDsp.CDR().p_cd[0]
	/// gpcOBJlnk
	#define OBJget ((gpcOBJlnk*)OBJ.Ux( (cd.obj-iOPe), sizeof(gpcOBJlnk)))[0]
	#define OBJadd ((gpcOBJlnk*)OBJ.Ux( SCOOP.nOBJ, sizeof(gpcOBJlnk)))[0]
#endif
#define aaOPid gpaaOPid[lnk.y]*/

class gpcOBJlnk {
public:
	I8x2	obj;
	gpeTYP	typ;
	gpcOBJlnk(){};

	size_t strASM( char* pS, char* pALL, I8x4 *pM0, U4x4 *pL0   );

};


class gpcCD {
public:
	U1		pre, pst;
	gpeTYP	typ;
	I4		lnk, nD;

	gpcCD(){};

	gpcCD& null() { gpmCLR; return *this; }
	gpcCD& operator ++()
	{
		++nD;
	}
};

class gpcCDsp {
public:
	I4		iCD, refCD;
	gpcCD	*pCD;
	gpcLZY	cd;

	I4		*apSP[0x30], aiSP[0x30];
	gpcLZY	aSP[0x30];

	gpcCDsp(){ gpmCLR; refCD = -1; };
	gpcCDsp& operator ++()
	{
		U1 p = CD()[0].pst;
		++aiSP[p];
		(apSP[p] = ((I4*)aSP[p].Ux( aiSP[p], sizeof(I4))))[0] = iCD;
		++iCD;
		CD()[0].null();
		return *this;
	}
	gpcCDsp& operator --()
	{
		CD()[0].null();
		--iCD;
		if( iCD < 0 )
		{
			iCD = 0;
			refCD = -1;
			return *this;
		}
		U1 p = CD()[0].pst;
		(apSP[p] = ((I4*)aSP[p].Ux( aiSP[p], sizeof(I4))))[0] = 0;
		--aiSP[p];
		CD();
		return *this;
	}
	gpcCDsp& operator -= ( U4 nd )
	{
		if( !nd )
			return *this;
		for( U4 i = 0; i < nd; i++ )
			--(*this);

		return *this;
	}
	gpcCD* CD()
	{
		if( refCD != iCD )
			pCD = ((gpcCD*)cd.Ux( refCD=iCD, sizeof(gpcCD)));

		return pCD;
	}
	I4 opi( U1 i )
	{
		return aiSP[i] ? (apSP[i]?*apSP[i]:-1) : -1;
	}
	U1 opi( I4x2* pO, gpeOPid* pL, U1 nL )
	{
		U1 i = 0, j = 0;
		for( ; i < nL; i++ )
		{
			if( !pL[i] )
				break;
			if( opi(pL[i]) < 0 )
				continue;
			pO[j].null().pst = pL[i];
			pO[j].iCD = opi(pL[i]);
			++j;
		}
		return j;
	}
};


#endif // GPCSRCLNK_H
