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

	gpeOPid aADD[0x40],aMUL[0x40], strt, now; //aSTRT[0x40];
	I4		nADD, nMUL; //, //nSTRT,
			//A[8], D[8],
	/*		*apSP[0x40], aiSP[0x40];
	gpcLZY	aSP[0x40];*/

	gpcCDsp(){ gpmCLR; refCD=-1; Ai=6; }; //A[7]=0x40; };
	gpcCD* CD() {
		if( refCD == iCD )
			return pCD;
		return pCD = ((gpcCD*)cd.Ux( refCD=iCD, sizeof(gpcCD)));
	}
	gpcCDsp& operator ++() {
		U1 p = CD()[0].pst;
		/*++aiSP[p];
		(apSP[p] = ((I4*)aSP[p].Ux( aiSP[p], sizeof(I4))))[0] = iCD;*/
		++iCD;
		CD()[0].null();
		return *this;
	}
	/*gpcCDsp& operator --() {
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
	}*/
	/*gpcCDsp& operator -= ( U4 nd ) {
		if( !nd )
			return *this;
		for( U4 i = 0; i < nd; i++ )
			--(*this);
		return *this;
	}*/
	/*I4 opi( U1 i ) {
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
	}*/
	I4x4& LOAD_SRC_ADR_A0( gpcSCOOP& scp,I4 n=-1) { /// move.l n(Ai),A0
		/// move.l -1(Ai), A0
		I4x4 &load = isa( PC, gpeOPid_mov, gpeEAszL
							,gpeEA_d16IAnI,Ai,0
							,gpeEA_An,0,0  );
		load.aOB[0] = n;
		return load;
	}
	I4x4& LOAD_SRC_ADR_A1( gpcSCOOP& scp,I4 n=-1) { /// move.l n(Ai),A1
		/// move.l -1(Ai), A1
		I4x4 &load = isa( PC, gpeOPid_mov, gpeEAszL
							,gpeEA_d16IAnI,Ai,0
							,gpeEA_An,1,0  );
		load.aOB[0] = n;
		return load;
	}
	I4x4& LOAD_DST_ADR_A0( gpcSCOOP& scp,I4 n=-1) { /// move.l n(Ai+1),A0
		I4x4 &prev = isa( PC, gpeOPid_mov, gpeEAszL
							,gpeEA_d16IAnI,Ai+1,0
							,gpeEA_An,0,0
						);
		prev.aOB[0] = n;
		return prev;
	}
	I4x4& move_l_IA1I_IA0I(gpcSCOOP& scp) { /// move.l (A1),(A0)
		I4x4& inst = isa( PC, gpeOPid_mov, gpeEAszL
							,gpeEA_IAnI,1,0
							,gpeEA_IAnI,0,0
						);
		return inst;
	}
	I4x4& move_l_IA1I_D0(gpcSCOOP& scp) { /// move.l (A1),D0
		I4x4& inst = isa( PC, gpeOPid_mov, gpeEAszL
							,gpeEA_IAnI,1,0
							,gpeEA_Dn,0,0
						);
		return inst;
	}
	I4x4& move_l_IA1I_D1(gpcSCOOP& scp) { /// move.l (A1),D1
		I4x4& inst = isa( PC, gpeOPid_mov, gpeEAszL
							,gpeEA_IAnI,1,0
							,gpeEA_Dn,1,0
						);
		return inst;
	}
	gpcCDsp& kMOV( gpcSCOOP& scp, U4 iOPe ) {
		isa( PC,	gpeOPid_mov, gpeEAszL,
										gpeEA_An,Ai,0,
										gpeEA_Dn,Ai,0  );
		Ai--;
		isa( PC,	gpeOPid_mov, gpeEAszL,
							gpeEA_An,Ai,0,
							gpeEA_Dn,Ai,0  );
		isa( PC );
		return *this;
	}
	gpcCDsp& kOBJ( gpcSCOOP& scp, U4 iOPe ) {
		// a = 				dp = 0
		// b. c=			dp = 1
		// d. e. f+			dp = 2
		I4 deep = CD()[0].deep, dp = 0;
		for( U4 d = 1; d <= deep; d++ )
		{
			if( pCD[0-d].pst != gpeOPid_dot )
				break;			/// megáll
			dp++;
		}
		gpcCD	&ins = dp ? pCD[0-dp] : pCD[0];
		/// move.l A7,D0
		//A[7] = D[0];
		isa( PC, gpeOPid_mov, gpeEAszL
				,gpeEA_An,7,0
				,gpeEA_Dn,0,0
			);
		while(dp>-1) {
			gpcCD &i = pCD[0-dp];
			I4 pc = PC;

		/// move.l 0xOBJid,-(A7) ; PUSH
			I4x4 &opcd = isa( PC, gpeOPid_mov, gpeEAszL
								,gpeEA_num,0,0
								,gpeEA_sIAnI,7,0
							);
			//--A[7];
			opcd.aOB[0] = (i.lnk > 0) ? i.lnk-iOPe : i.lnk;
			--dp;
		}
		/// "\n 0x%0.4x jsr fndOBJ2A0"
		isa( PC, gpeOPid_dot );
		/// move.l D0,A7
		isa( PC, gpeOPid_mov, gpeEAszL
				,gpeEA_Dn,0,0
				,gpeEA_An,7,0
			);
		/// move.l A0,(Ai)+
		isa( PC, gpeOPid_mov, gpeEAszL
				,gpeEA_An,0,0
				,gpeEA_IAnIp,Ai,0
			);

		isa( PC );
		//A[Ai]++;
		return *this;
	}
	gpcCDsp& kADD( gpcSCOOP& scp, U4 iOPe ) {
		// a =b +c*d 	// nADD 1 // de nem adtam hozzá még a c-t
		//   -1-^
		// a =b+c +d*e  // nADD 2 // de nem adtam hozzá még a d-t
		//   -2-1-^
		// d*e +f*g		// nADD 1 // de nem adtam hozzá még a f-t
		//-2-1-^
		// d*e +f -g*h	// nADD 2 // de nem adtam hozzá még a g-t
		//-3-2 -1-^
		if(!nADD)
			return *this;

		if(nADD==1)
		if(gpaOPgrp[strt]==gpeOPid_mul)
		{
			/// ezt az esetet a kMUL-nak kell végrehajtania
			// d*e +f*g		// nADD 1 // de nem adtam hozzá még a f-t
			//-2-1-^
			nADD = nMUL = 0;
			strt = *aADD;
			scp.vASM.INST( PC );
			return *this;
		}
		else if(strt==gpeOPid_mov) {
			// a =b +c*d 	// nADD 1 // de nem adtam hozzá még a c-t
			//   -1-^
			LOAD_SRC_ADR_A1( scp, -1 );	/// move.l -1(Ai),A1
			LOAD_DST_ADR_A0(scp);	/// move.l -1(Ai+1), A0
			move_l_IA1I_IA0I(scp);	/// move.l (A1),(A0)

			nADD = nMUL = 0;
			strt = aADD[nADD];
			scp.vASM.INST( PC );
			return *this;
		}


		U4 s = ( gpaOPgrp[strt]==gpeOPid_mul ), a = 0, nA = nADD;
		if( s )
		{
			//  strt
			//  |  0  1
			//  V  v  v
			// d*e +f -g*h		// nADD 2 // de nem adtam hozzá még a g-t
			//     -1-^
			//     0 1  2
			//     v v  v
			// d*e +f-g +h*i	// nADD 3 // de nem adtam hozzá még a h-t
			//     -2-1-^
			strt = aADD[a++];
			LOAD_SRC_ADR_A1(scp,s-nADD);	/// move.l n(Ai),A1
			move_l_IA1I_D0(scp);			/// move.l (A1),D0
			--nA;
		} else {
			//   strt
			//   |   0
			//   V   v
			// a +=b +c*d 	// nADD 1 // de nem adtam hozzá még a c-t
			//    -1-^
			//      0  1
			//      v  v
			// a *=b+c +d*e  // nADD 2 // de nem adtam hozzá még a d-t
			//    -2-1-^
			LOAD_SRC_ADR_A1(scp,s-nADD);	/// move.l n(Ai),A1
			move_l_IA1I_D0(scp);			/// move.l (A1),D0
		}

		while( a < nA )
		{
			LOAD_SRC_ADR_A1(scp,s-nADD);	/// move.l n(Ai),A1
			isa( PC, aADD[a], gpeEAszL
					,gpeEA_IAnI,1,0
					,gpeEA_Dn,0,0
			);
			a++;
			s++;
		}

		LOAD_DST_ADR_A0( scp,-1 );	/// move.l -1(Ai+1),A0
		isa( PC, strt, gpeEAszL
					,gpeEA_Dn,0,0
					,gpeEA_IAnI,0,0
			);

		strt = aADD[nADD-1];
		isa( PC );
		nADD = nMUL = 0;
		return *this;
	}
	gpcCDsp& kMUL( gpcSCOOP& scp, U4 iOPe ) {
		if(!nMUL)
			return *this;

		//   strt
		//   V
		// a =b*c +d*d		// nMUL 1
		//   -2-1_^ 		// (Ai)+
		// a =b*c/d +e		// nMUL 2
		//   -3-2-1_^ 		// (Ai)+
		// a+=b*c/d +e		// nMUL 2
		//   -3-2-1_^ 		// (Ai)+

		//   strt
		//   V
		// a +b*c +d		// nMUL 1
		//   -2-1_^ 		// (Ai)+
		// a +b*c/d +e		// nMUL 2
		//   -3-2-1_^ 		// (Ai)+
		LOAD_SRC_ADR_A1(scp,-(nMUL+1));	/// move.l n(Ai),A1
		move_l_IA1I_D0(scp);			/// move.l (A1),D0

		for( U4 i = 0; i < nMUL; i++ )
		{
			LOAD_SRC_ADR_A1(scp,i-nMUL);	/// move.l n(Ai),A1
			isa( PC, aMUL[i], gpeEAszL
					,gpeEA_IAnI,1,0
					,gpeEA_Dn,0,0
			);
			i++;
		}

		LOAD_DST_ADR_A0( scp,-1 );	/// move.l -1(Ai+1),A0
		isa( PC, strt, gpeEAszL
					,gpeEA_Dn,0,0
					,gpeEA_IAnI,0,0
			);

		strt = aMUL[nMUL-1];
		isa( PC );
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
				Ai--; //kMOV( scp, iOPe ); // Ai--; //
				++SP;
				strt = now;
				break;
			case gpeOPid_add: /// +
			case gpeOPid_sub: /// ==
				kOBJ( scp, iOPe );
				// a =b*c +d		// nMUL 1
				// a =b*c/d +e		// nMUL 2
				// a +=b*c/d +e		// nMUL 3
				// a +b*c +e		// nMUL 1
				// a*b +b*c			// nMUL 1
				kMUL( scp, iOPe );
				aADD[nADD++] = now;
				++SP;

				break;
			case gpeOPid_mul: /// *
				// a =b +c*d 	// nADD 1 // de nem adtam hozzá még a c-t
				//   -1-^
				// a =b+c +d*e  // nADD 2 // de nem adtam hozzá még a d-t
				//   -2-1-^
				// d*e +f*g		// nADD 1
				//-2-1-^
				// d*e +f -g*h	// nADD 2
				//-3-2 -1-^
				kADD( scp, iOPe );

				kOBJ( scp, iOPe );
				aMUL[nMUL++] = now;
				++SP;
				++CDC;
				break;
            case gpeOPid_stk: /// ,

				break;
			case gpeOPid_entry: { /// (
					switch( now )
					{
						case gpeOPid_dot:
							break;
						case gpeOPid_brakS:
						default:

							break;
					}
				++SP;
				++CDC;
				} break;
			case gpeOPid_out: /// )
				++SP;
				++CDC;
				break;
		}
		return SP;
	}
	/*gpcLZY& ASMrdy( gpcSCOOP& scp, U4 iOPe, I4x2* aFND, gpeOPid* pL, U1 nL ) {
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
	}*/
};


#endif // GPCSRCLNK_H
