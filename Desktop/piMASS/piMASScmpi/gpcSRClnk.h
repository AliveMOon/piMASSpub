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

	"%d(A%x)",
	"%d(A%x,%s)",

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
#define CDC (CD()[0])
#define isa scp.vASM.INST
class gpcCDsp {
public:
	I4		iCD, refCD, nR, Ai, Di;
	gpcCD	*pCD;
	gpcLZY	cd;

	gpeOPid aADD[0x40],aMUL[0x40], aSTRT[0x40];
	I4		nADD, nMUL, nSTRT;
			A[8], D[8],
			*apSP[0x40], aiSP[0x40];
	gpcLZY	aSP[0x40];

	gpcCDsp(){ gpmCLR; refCD=-1; Ai=6; A[7]=0x40; };
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

	I4x4& LOAD_SRC_A0( I4 n ) { /// move.l -1(Ai),A0
		/// move.l -1(Ai), A0
		I4x4 &load = isa( PC, gpeOPid_mov, gpeEAszL
							,gpeEA_d16IAnI,Ai,0
							,gpeEA_An,0,0  );
		load.aOB[0] = n;
		return load;
	}
	I4x4& LOAD_SRC_A1( I4 n ) { /// move.l -1(Ai),A1
		/// move.l -1(Ai), A1
		I4x4 &load = isa( PC, gpeOPid_mov, gpeEAszL
							,gpeEA_d16IAnI,Ai,0
							,gpeEA_An,0,0  );
		load.aOB[0] = n;
		return load;
	}
	I4x4& LOAD_DST_ADR_A0() { /// move.l -1(Ai+1), A0
		I4x4 &prev = isa( PC, gpeOPid_mov, gpeEAszL
							,gpeEA_d16IAnI,Ai+1,0
							,gpeEA_An,0,0
						);
		prev.aOB[0] = -1;
		return load;
	}
	gpcCDsp& kMOV( gpcSCOOP& scp, U4 iOPe ) {
		scp.vASM.INST( PC,	gpeOPid_mov, gpeEAszL,
										gpeEA_An,Ai,0,
										gpeEA_Dn,Ai,0  );
		Ai--;
		scp.vASM.INST( PC,	gpeOPid_mov, gpeEAszL,
							gpeEA_An,Ai,0,
							gpeEA_Dn,Ai,0  );
		scp.vASM.INST( PC );
		return *this;
	}
	gpcCDsp& kOBJ( gpcSCOOP& scp, U4 iOPe ) {
		I4 dp = CD()[0].deep;
		for( U4 d = 1; d < dp; d++ )
		{
			if( pCD[0-d].pst == gpeOPid_dot )
				continue;
			dp = d-1;			/// megáll
		}
		gpcCD	&ins = dp ? pCD[-dp] : pCD[0];
		/// move.l A7,D0
		A[7] = D[0];
		scp.vASM.INST( PC,	gpeOPid_mov, gpeEAszL,
										gpeEA_An,7,0,
										gpeEA_Dn,0,0  );
		while(dp>-1) {
			gpcCD &i = pCD[0-dp];
			I4 pc = PC;

		/// move.l 0xOBJid,-(A7) ; PUSH
			I4x4 &opcd = scp.vASM.INST( PC,	gpeOPid_mov, gpeEAszL,
											gpeEA_num,0,0,
											gpeEA_sIAnI,7,0  );
			--A[7];
			opcd.aOB[0] = (i.lnk > 0) ? i.lnk-iOPe : i.lnk;
			--dp;
		}
		/// "\n 0x%0.4x jsr fndOBJ2A0"
		scp.vASM.INST( PC,	gpeOPid_dot );
		/// move.l D0,A7
		scp.vASM.INST( PC,	gpeOPid_mov, gpeEAszL,
										gpeEA_Dn,0,0,
										gpeEA_An,7,0  );
		/// move.l A0,(Ai)+
		scp.vASM.INST( PC,	gpeOPid_mov, gpeEAszL,
										gpeEA_An,0,0,
										gpeEA_IAnIp,Ai,0  );

		scp.vASM.INST( PC );
		A[Ai]++;
		return *this;
	}
	gpcCDsp& kADD( gpcSCOOP& scp, U4 iOPe )
	{
		// a =b +c*d 	// nADD 1
		// a =b+c +d*e  // nADD 2
		// d*e +f*g		// nADD 1
		// d*e +f -g*h	// nADD 2
		if(!nADD)
			return *this;

		if(nADD==1)
		if(gpaOPgrp[*aSTRT]==gpeOPid_mul)
		{
			// ezt az esetet a kMUL-nak kell végrehajtania
			// d*e +f*g		// nADD 1
			//     ^
			nADD = nMUL = 0
			aSTRT[nSTRT++] = aADD[nADD];
			scp.vASM.INST( PC );
			return *this;
		}
		else if(*aSTRT==gpeOPid_mov) {
			// a =b +c*d 	// nADD 1
			//   ^
			LOAD_SRC_A1( -1 ); /// move.l -1(Ai),A1
			LOAD_DST_ADR_A0() { /// move.l -1(Ai+1), A0
			scp.vASM.INST( PC, gpeOPid_mov, gpeEAszL
							,gpeEA_IAnI,1,0
							,gpeEA_IAnI,0,0
						);
			nADD = nMUL = 0
			aSTRT[nSTRT++] = aADD[nADD];
			scp.vASM.INST( PC );
			return *this;
		}



		if(*aSTRT==gpeOPid_mov)
		{
			// a =b+c +d*e  // nADD 2
			//   ^
			LOAD_SRC_A1( -1 ); /// move.l -1(Ai),A1
			/// move.l (Ai),d0
			scp.vASM.INST( PC, last, gpeEAszL
							,gpeEA_IAnI,1,0
							,gpeEA_Dn,0,0
						);
		} else {
			// d*e +f*g		// nADD 1
			/*if( nADD == 1 )
			{
				nADD = nMUL = 0
				aSTRT[nSTRT++] = aADD[nADD];
				return *this;
			}*/
			if( gpaOPgrp[*aSTRT]==gpeOPid_mul )
			// a *=b +c*d 	// nADD 1
			// a +=b+c +d*e  // nADD 2
			// d*e +f*g		// nADD 1

		}







		--nADD; // utolsot a szorzásnak kell
		// 0.on meg tuti nem +- van
		U4 i=1, j=(last==gpeOPid_mul);
		nADD-=j;
		/// LOAD SRC---------
		/// move.l -1(Ai), A0
		I4x4 &load = scp.vASM.INST( PC, gpeOPid_mov,	gpeEAszL
											,gpeEA_d16IAnI,Ai,0
											,gpeEA_An,0,0  );
		load.aOB[0] = i-nADD;
		scp.vASM.INST( PC, gpeOPid_mov,	gpeEAszL
										,gpeEA_IAnI,0,0
										,gpeEA_Dn,0,0
					);

		for( ; i < nADD; i++ )
		{
			/// move.l /i-nADD/(Ai), A0
			I4x4 &ins = scp.vASM.INST( PC, gpeOPid_mov,	gpeEAszL
											,gpeEA_d16IAnI,Ai,0
											,gpeEA_An,0,0
									);
			ins.aOB[0] = i-nADD;
			/// oper.l (A0),D0
			scp.vASM.INST( PC, aADD[i], gpeEAszL
										,gpeEA_IAnI,0,0
										,gpeEA_Dn,0,0
						);
		}

		/// LOAD DST ADR A0------------------------------
		/// move.l -1(Ai+1), A0
		I4x4 &prev = scp.vASM.INST( PC, gpeOPid_mov, gpeEAszL
											,gpeEA_d16IAnI,Ai+1,0
											,gpeEA_An,0,0
									);
		prev.aOB[0] = -1;

		scp.vASM.INST( PC, last, gpeEAszL
								,gpeEA_Dn,0,0
								,gpeEA_IAnI,0,0
						);

		scp.vASM.INST( PC );
		nADD = nMUL = 0;
		return *this;
	}
	gpcCDsp& kMUL( gpcSCOOP& scp, U4 iOPe )
	{
		if(!nMUL)
			return *this;
		//nMUL++;
		/// LOAD SRC---------
		/// move.l -1(Ai), A0
		I4x4 &load = scp.vASM.INST( PC, gpeOPid_mov,	gpeEAszL
											,gpeEA_d16IAnI,Ai,0
											,gpeEA_An,0,0  );
		load.aOB[0] = -nMUL;
		/// move.l (A0),D0
		scp.vASM.INST( PC, gpeOPid_mov,	gpeEAszL
										,gpeEA_IAnI,0,0
										,gpeEA_Dn,0,0
					);
		for( U4 i = 1; i < nMUL; i++ )
		{
			/// move.l /i-nMUL/(Ai), A0
			I4x4 &ins = scp.vASM.INST( PC, gpeOPid_mov,	gpeEAszL
											,gpeEA_d16IAnI,Ai,0
											,gpeEA_An,0,0
									);
			ins.aOB[0] = i-nMUL;
			/// oper.l (A0),D0
			scp.vASM.INST( PC, last, gpeEAszL
										,gpeEA_IAnI,0,0
										,gpeEA_Dn,0,0
						);
		}

		/// LOAD DST ADR ------------------------------
		/// move.l -1(Ai+1), A0
		I4x4 &prev = scp.vASM.INST( PC, gpeOPid_mov, gpeEAszL
											,gpeEA_d16IAnI,Ai+1,0
											,gpeEA_An,0,0
									);
		prev.aOB[0] = -1;

		scp.vASM.INST( PC, last, gpeEAszL
								,gpeEA_Dn,0,0
								,gpeEA_IAnI,0,0
						);

		scp.vASM.INST( PC );
		nADD = nMUL = 0;
		return *this;
	}

	gpcCDsp& knead( gpcSCOOP& scp, U4 iOPe ) {
		gpcCDsp& SP = *this;
		now=CDC.pst;
		switch( gpaOPgrp[now] )
		{
			case gpeOPid_mov: /// =
				kOBJ( scp, iOPe );
				kMOV( scp, iOPe ); //Ai--;
				++SP;
				aSTRT[nSTRT++] = now;
				break;
			case gpeOPid_add: /// +
			case gpeOPid_sub: /// ==
				kOBJ( scp, iOPe );
				kMUL( scp, iOPe );
				aADD[nADD++] = now;
				++SP;

				break;
			case gpeOPid_mul: /// *
				// a =b +c*d 	// nADD 1
				// a =b+c +d*e  // nADD 2
				// d*e +f*g		// nADD 1
				// d*e +f -g*h	// nADD 2
				kADD( scp, iOPe );
				kOBJ( scp, iOPe );
				aMUL[nMUL++] = last = now;
				++SP;
				++CDC;
				break;
            case gpeOPid_stk: /// ,

				break;
			case gpeOPid_entry: /// (
				++SP;
				++CDC;
				break;
			case gpeOPid_out: /// )
				++SP;
				++CDC;
				break;
		}
		return SP;
	}
	gpcLZY& ASMrdy( gpcSCOOP& scp, U4 iOPe, I4x2* aFND, gpeOPid* pL, U1 nL ) {
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
