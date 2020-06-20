#ifndef GPCSRCLNK_H
#define GPCSRCLNK_H
#include "piMASS.h"

class gpcOBJlnk {
public:
	I8x2	obj;
	gpeTYP	typ;
	gpcOBJlnk(){};
};


class gpcCD {
public:
	U1		pre, pst;
	gpeTYP	typ;
	U4		obj;

	gpcCD& null() { gpmCLR; return *this; }
};

class gpcCDsp {
public:
	U4		iCD, refCD;
	gpcCD	*pCD;
	gpcLZY	cd;

	U4		*apSP[0x30], aiSP[0x30];
	gpcLZY	aSP[0x30];

	gpcCDsp(){ gpmCLR; refCD = -1; };
	gpcCDsp& operator ++()
	{
		U1 p = CD()->pst;
		++aiSP[p];
		(apSP[p] = ((U4*)aSP[p].Ux( aiSP[p], sizeof(U4))))[0] = iCD;
		++iCD;
		return *this;
	}
	gpcCD* CD()
	{
		if( refCD != iCD )
			pCD = ((gpcCD*)cd.Ux( refCD=iCD, sizeof(gpcCD)));

		return pCD;
	}
	U4 opi( U1 i )
	{
		return aiSP[i] ? (apSP[i]?*apSP[i]:(U4)-1) : (U4)-1;
	}
	U4* opi( U4* pO, U1* pL, U1 nL )
	{
		for( U1 i = 0; i < nL; i++ )
			pO[i] = opi(pL[i]);
		return pO;
	}
};


#endif // GPCSRCLNK_H
