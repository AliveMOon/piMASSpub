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

static const char* gpsTYPsz[] = {
	".b", //0 1
	".w", //1 2
	".l", //2 4
	".q", //3 8
	".x", //4 16
};
static const char* gps68kREG[] = {
	"D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7",
	"A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7",
};
static const char* gps68kADRmod[] = {
	"",
	"D%x",
	"A%x",

	"(A%x)",
	"(A%x)+",
	"-(A%x)",

	"0x%0.4d(A%x)",
	"0x%0.2d(A%x,%s)",

	"0x%0.4d(PC)",
	"0x%0.2d(PC,%s)",

	"0x%0.4x.w",
	"0x%0.8x.l",
	"0x%0.8x",				//direct data
};


class gpcOBJlnk {
public:
	I8x2	obj;
	gpeTYP	typ;
	gpcOBJlnk(){};

	size_t strASM( char* pS, char* pALL, I8x4 *pM0, U4x4 *pL0   );

};


class gpcCD {
public:
	gpeOPid	pre, pst;
	gpeTYP	typ;
	I4		lnk, deep;

	gpcCD(){};

	gpcCD& null() { gpmCLR; return *this; }
	gpcCD& operator ++()
	{
		deep = this[-1].deep+1;
		return *this;
	}

};

#define PC scp.nASM()
class gpcCDsp {
public:
	I4		iCD, refCD, nR;
	gpcCD	*pCD;
	gpcLZY	cd;

	I4		*apSP[0x30], aiSP[0x30];
	gpcLZY	aSP[0x30];

	gpcCDsp(){ gpmCLR; refCD = -1; };
	gpcCD* CD() {
		if( refCD == iCD )
			return pCD;
		return pCD = ((gpcCD*)cd.Ux( refCD=iCD, sizeof(gpcCD)));
	}
	gpcCDsp& operator ++() {
		U1 p = CD()[0].pst;
		++aiSP[p];
		(apSP[p] = ((I4*)aSP[p].Ux( aiSP[p], sizeof(I4))))[0] = iCD;
		++iCD;
		CD()[0].null();
		return *this;
	}
	gpcCDsp& operator --() {
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
	gpcCDsp& operator -= ( U4 nd ) {
		if( !nd )
			return *this;
		for( U4 i = 0; i < nd; i++ )
			--(*this);
		return *this;
	}
	I4 opi( U1 i ) {
		return aiSP[i] ? (apSP[i]?*apSP[i]:-1) : -1;
	}
	U1 opi( I4x2* pO, gpeOPid* pL, U1 nL ) {
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

	gpcCDsp& ASMdeepMOV( gpcSCOOP& scp, U4 iOPe )
	{
		if( !CD()[0].deep )
			return *this;
		I4		nDP = CD()[0].deep;
		gpcCD	&ins = nDP ? pCD[-nDP] : pCD[0];
		switch( ins.pst )
		{
			case gpeOPid_dot: {
					// move.q A7,D7
					scp.vASM.INST( PC,	gpeOPid_mov, gpeEAszL,
										gpeEA_An,7,0,
										gpeEA_Dn,7,0  );
					while( nDP > -1) {
						gpcCD &i = pCD[0-nDP];
						I4 pc = PC;
						// move.q 0xOBJid,-(A7) ;
						I4x4 &opcd = scp.vASM.INST( PC,	gpeOPid_mov, gpeEAszL,
														gpeEA_num,0,0,
														gpeEA_sIAnI,7,0  );
						opcd.aOB[0] = (i.lnk > 0) ? i.lnk-iOPe : i.lnk;
						--nDP;
					}
					// move.l A7,D0
					scp.vASM.INST( PC,	gpeOPid_mov, gpeEAszL,
										gpeEA_An,7,0,
										gpeEA_Dn,0,0  );
					// move.l D7,D0
					scp.vASM.INST( PC,	gpeOPid_sub, gpeEAszL,
										gpeEA_Dn,7,0,
										gpeEA_Dn,0,0  );
					// "\n 0x%0.4x jsr fndOBJ2SP"
					scp.vASM.INST( PC,	gpeOPid_dot );
				} break;
			default:
				break;
		}



		return *this;
	}
	gpcCDsp& ASMdeepADD( gpcSCOOP& scp, U4 iOPe )
	{
		for( U4 d = 0, dp = CD()[0].deep; d < dp; d++ )
		{


		}
		return *this;
	}
	gpcCDsp& ASMdeep( gpcSCOOP& scp, U4 iOPe ) {
		if( CD()[0].deep )
		{
			switch( gpaOPgrp[pCD[0].pst] )
			{
				case gpeOPid_mov:
					ASMdeepMOV( scp, iOPe );
					++(*this);
					break;
				case gpeOPid_add:
					ASMdeepADD( scp, iOPe );
					++(*this);
					break;
				case gpeOPid_mul:
					break;
				case gpeOPid_sub:
					ASMdeepADD( scp, iOPe );
					++(*this);
					break;
				case gpeOPid_stk:
					break;
				case gpeOPid_entry:
					++(*this);
					++(*pCD);
					break;
				case gpeOPid_out:
					break;
			}
			return *this;
		}

		switch( gpaOPgrp[pCD[0].pst] )
		{
			case gpeOPid_mov: /// =
				++(*this);
				break;
			case gpeOPid_add: /// +
				++(*this);
				break;
			case gpeOPid_mul: /// *
				++(*this);
				++(*pCD);
				break;
			case gpeOPid_sub: /// ==
				++(*this);
				break;
			case gpeOPid_stk: /// ,

				break;
			case gpeOPid_entry: /// (
				++(*this);
				++(*pCD);
				break;
			case gpeOPid_out: /// )
				++(*this);
				++(*pCD);
				break;
		}
		return *this;
	}
	gpcLZY& ASMrdy( gpcSCOOP& scp, U4 iOPe, I4x2* aFND, gpeOPid* pL, U1 nL )
	{
		U4 nFND = opi( aFND, pL, nL );
		aFND->median( nFND, aFND+nFND+1 );
		bool bFLIP = false;
		for( I4 up = iCD, dwn = aFND[0].iCD, dp; dwn < up; up-- )
		{
			dp = CD()[0].deep;
			/// valseg itt kel majd egy JMP ha gyorsítani kell
			if( dp )
			if( pCD[-dp].pst != gpeOPid_dot )
			{
				I4x4 &INS = ((I4x4*)scp.vASM.Ux( scp.nASM(), sizeof(INS) ))[0];
				gpcCD &dst = pCD[-1];
				INS =  I4x4( dst.pst,
									(pCD[0].lnk > 0)	? pCD[0].lnk-iOPe	: pCD[0].lnk,
									(dst.lnk > 0)		? dst.lnk-iOPe		: dst.lnk,
									nR
							);
				--up;
				for( U4 i = 2; i <= dp; i++ )
				{
					I4x4 &INS = ((I4x4*)scp.vASM.Ux( scp.nASM(), sizeof(INS) ))[0];
					gpcCD &mom = pCD[-i], &kid = pCD[1-i];
					INS = I4x4( mom.pst,
										(kid.lnk > 0) ? kid.lnk-iOPe : kid.lnk,
										(mom.lnk > 0) ? mom.lnk-iOPe : mom.lnk,
										nR
								);
					--up;
				}
			}
			else while( dp )
			{
				I4x4 &INS = ((I4x4*)scp.vASM.Ux( scp.nASM(), sizeof(INS) ))[0];
				gpcCD &mom = pCD[-dp];
				if( dp < 2 )
				{
					INS =  I4x4( mom.pst,
										(pCD[0].lnk > 0) ? pCD[0].lnk-iOPe : pCD[0].lnk,
										(mom.lnk > 0) ? mom.lnk-iOPe : mom.lnk,
										nR
								);
					INS.swpZY();
					--up;
					break;
				}
				--up;
				--dp;
				gpcCD &kid = pCD[-dp];
				INS = I4x4( mom.pst,
									(kid.lnk > 0) ? kid.lnk-iOPe : kid.lnk,
									(mom.lnk > 0) ? mom.lnk-iOPe : mom.lnk,
									nR
							);
				INS.swpZY();
			}
			bFLIP = false;
			I4x4 &INS = ((I4x4*)scp.vASM.Ux( scp.nASM(), sizeof(INS) ))[0];
			*this -= pCD[0].deep;
			INS.y = (CD()[0].lnk > 0) ? pCD[0].lnk-iOPe : pCD[0].lnk;
			--*this;
			INS.z = (CD()[0].lnk > 0) ? pCD[0].lnk-iOPe : pCD[0].lnk;
			INS.x = pCD[0].pst;
			INS.w = nR;
			//if( INS.x == gpeOPid_dot )
			//	INS.swpZY();

		}
		return scp.vASM;
	}
};


#endif // GPCSRCLNK_H
