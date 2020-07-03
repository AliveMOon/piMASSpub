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

static const char* gpasTYPsz[] = {

	".b",	///0 1	8	byte
	".w",	// 1 2	16	word
	".l",	// 2 4	32	long
	".q",	// 3 8	64	quad

	".o", 	// 4 16 128	4x4		// egy byte-os float nincs ezért ez jó lehet 128bit-esnek is

	".h",	// 5 2	16	half
	".f", 	// 6 4	32	float
	".d", 	// 7 8	64	double
	".fx", 	// 8 9	128 f4x4
};
static const char* gps68kREG[] = {
	"D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7",
	"A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7",

	"F0", "F1", "F2", "F3", "F4", "F5", "F6", "F7",
	"O0", "O1", "O2", "O3", "O4", "O5", "O6", "O7",
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

#define PC (iPC=scp.nASM())
#define CDC (CD()[0])
#define isa scp.vASM.INST
#define lADD lSAM.z
#define nADD allSAM.x
#define alADD allSAM.z
#define lMUL lSAM.w
#define nMUL allSAM.y
#define alMUL allSAM.w


class gpcCDsp {
public:
	I4		iCD, refCD, nR, Ai, Di, nSTRT, iPC;
	gpcCD	*pCD;
	gpcLZY	cd, spA, spM, spSAM;

	gpeOPid *pADD, *pMUL, *pSTRT, now; //aSTRT[0x40];
	I4x4	iSAM, lSAM, nSAM, allSAM;

	gpcCDsp(){ gpmCLR; refCD=-1; Ai=6; LEVrst(); }; //A[7]=0x40; };
	gpcCD* CD() {
		if( refCD == iCD )
			return pCD;
		return pCD = ((gpcCD*)cd.Ux( refCD=iCD, sizeof(gpcCD)));
	}

	gpcCDsp& operator ++() {
		U1 p = CD()[0].pst;
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
		}
		CD();
		return *this;
	}

	I4x4& LOAD_SRC_ADR_nA0( gpcSCOOP& scp,I4 n=-1) { /// move.l n(Ai),A0
		/// move.l -1(Ai), A0
		I4x4 &load = isa( PC, gpeOPid_mov, gpeEAszL
							,gpeEA_d16IAnI,Ai,0
							,gpeEA_An,0,0  );
		load.aOB[0] = n;
		return load;
	}
	I4x4& LOAD_SRC_ADR_nA1( gpcSCOOP& scp,I4 n=-1) { /// move.l n(Ai),A1
		/// move.l -1(Ai), A1
		I4x4 &load = isa( PC, gpeOPid_mov, gpeEAszL
							,gpeEA_d16IAnI,Ai,0
							,gpeEA_An,1,0  );
		load.aOB[0] = n;
		return load;
	}
	I4x4& LOAD_DST_ADR_A0( gpcSCOOP& scp ) { /// move.l (Ai+1),A0

		I4x4 &prev = isa( PC, gpeOPid_mov, gpeEAszL
							,gpeEA_IAnI,Ai+1,0
							,gpeEA_An,0,0
						);
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

	gpeOPid* LEVaddEXP() {
        if( pADD ? lADD < nADD : false )
            return pADD;

        nADD = lADD+0x10;
        if( alADD < iSAM.z+nADD )
            spA.Ux(  alADD = iSAM.z+nADD, sizeof(gpeOPid) );

        return pADD = (gpeOPid*)spA.Ux( iSAM.z, sizeof(gpeOPid));
    }
    gpeOPid* LEVmulEXP() {
        if( pMUL ? lMUL < nMUL : false )
            return pMUL;

        nMUL = lMUL+0x10;
        if( alMUL < iSAM.w+nMUL )
            spA.Ux(  alMUL = iSAM.w+nMUL, sizeof(gpeOPid) );

        return pMUL = (gpeOPid*)spA.Ux( iSAM.w, sizeof(gpeOPid));
    }
	gpcCDsp& LEVrst() {
        nSTRT = 0;
        I4x4* pSPsam = (I4x4*)spSAM.Ux( 0, sizeof(I4x4));
        allSAM.a4x2[0] = I4x2( 0x10, 0x10 );
        iSAM = lSAM.null();
        LEVaddEXP();
        LEVmulEXP();

        pSTRT   = &pSPsam[0].pre;
        pADD    = (gpeOPid*)spA.Ux( iSAM.z, sizeof(gpeOPid));
        pMUL    = (gpeOPid*)spM.Ux( iSAM.w, sizeof(gpeOPid));
        return *this;
	}

	gpcCDsp& LEVup( gpcSCOOP& scp ) {
        if( !pSTRT )
            LEVrst();

        I4x4* pSPsam = (I4x4*)spSAM.Ux( ++nSTRT, sizeof(I4x4));
        pSPsam[0].a4x2[1] = (iSAM.a4x2[1]+=lSAM.a4x2[1]);
        pSPsam[-1].op = CD()[0].lnk >= gpeOPid_n ? CD()[0].lnk : 0;
        lSAM.null();

        LEVaddEXP();
        LEVmulEXP();

        pSTRT   = &pSPsam[0].pre;
        pADD    = (gpeOPid*)spA.Ux( iSAM.z, sizeof(gpeOPid), false );
        pMUL    = (gpeOPid*)spM.Ux( iSAM.w, sizeof(gpeOPid), false );


		isa( PC );
        isa( PC, gpeOPid_mov, gpeEAszL
				,gpeEA_An,Ai,0
				,gpeEA_sIAnI,7,0
			);
        isa( PC );
        isa( PC );
        isa( PC, gpeOPid_xor, gpeEAszL
				,gpeEA_Dn,0,0
				,gpeEA_Dn
			);
        //I4x4& inst =
		isa( PC, gpeOPid_mov, gpeEAszL
				,gpeEA_Dn,0,0
				,gpeEA_sIAnI,Ai+1,0
			).aOB[0] = 1;
        //inst.aOB[0] = 1;
        isa( PC );
        return *this;
	}

	gpcCDsp& LEVdwn( gpcSCOOP& scp, I4 iOPe ) {
        if( !pSTRT )
            return LEVrst();

        /// ITT MÉG FENT
        kEND( scp,iOPe );
        I4x4 *pSPsam = (I4x4*)spSAM.Ux(--nSTRT, sizeof(I4x4));
		/// ITT MÁR LENT
		if( pSPsam[0].op )
		{
			isa( PC, gpeOPid_n, gpeEAszL
				,gpeEA_num
			).aOB[0] = pSPsam[0].op-iOPe; //-gpeOPid_n;
		}
        lSAM.a4x2[1] = pSPsam[1].a4x2[1]-(iSAM.a4x2[1] = pSPsam[0].a4x2[1]);
        allSAM.a4x2[0] = allSAM.a4x2[1]-iSAM.a4x2[1];

        pSTRT   = &pSPsam[0].pre;
        pADD    = (gpeOPid*)spA.Ux( pSPsam[0].z, sizeof(gpeOPid));
        pMUL    = (gpeOPid*)spM.Ux( pSPsam[0].w, sizeof(gpeOPid));


        isa( PC, gpeOPid_mov, gpeEAszL
				,gpeEA_IAnIp,7,0
				,gpeEA_An,Ai,0
			);
        isa( PC );
		isa( PC, gpeOPid_mov, gpeEAszL
				,gpeEA_IAnIp,Ai+1,0
				,gpeEA_IAnIp,Ai
			);
        isa( PC );
        return *this;
	}


	gpcCDsp& kMOV( gpcSCOOP& scp, U4 iOPe ) {

		/*isa( PC,	gpeOPid_mov, gpeEAszL
                ,gpeEA_An,Ai,0
                ,gpeEA_Dn,Ai,0
            );*/
		Ai--;
		/*isa( PC, gpeOPid_mov, gpeEAszL
                ,gpeEA_An,Ai,0
				,gpeEA_Dn,Ai,0
            );*/
		isa( PC );
		return *this;
	}
	gpcCDsp& kOBJ( gpcSCOOP& scp, U4 iOPe ) {
        switch( (gpeOPid)CD()[0].lnk )
        {
            case gpeOPid_end:
            case gpeOPid_dimE:
            case gpeOPid_brakE:
                return *this;
            default:
                break;
        }
		// a = 				dp = 0
		// b. c=			dp = 1
		// d. e. f+			dp = 2
		I4 deep = pCD[0].deep, dp = 0, iPC;
		for( I4 d = 1; d <= deep; d++ )
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
		/*/// move.l D0,A7
		isa( PC, gpeOPid_mov, gpeEAszL
				,gpeEA_Dn,0,0
				,gpeEA_An,7,0
			);*/
        if( gpaOPgrp[now] == gpeOPid_mov )
        {
             /// move.l A0,(Ai)
            isa( PC, gpeOPid_mov, gpeEAszL
                    ,gpeEA_An,0,0
                    ,gpeEA_IAnI,Ai,0
                );
        } else
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
		// a =b +c*d 	// iADD 1 // de nem adtam hozzá még a c-t
		//   -1-^
		// a =b+c +d*e  // iADD 2 // de nem adtam hozzá még a d-t
		//   -2-1-^
		// d*e +f*g		// iADD 1 // de nem adtam hozzá még a f-t
		//-2-1-^
		// d*e +f -g*h	// iADD 2 // de nem adtam hozzá még a g-t
		//-3-2 -1-^
		if(!lADD)
			return *this;

		if( lADD==1 )
		if(gpaOPgrp[*pSTRT]==gpeOPid_mul)
		{
			/// ezt az esetet a kMUL-nak kell végrehajtania
			// d*e +f*g		// iADD 1 // de nem adtam hozzá még a f-t
			//-2-1-^
			lADD = lMUL = 0;
			*pSTRT = *pADD;
			//scp.vASM.INST( PC );
			return *this;
		}
		else switch( *pSTRT )
		{
            case gpeOPid_mov:
            case gpeOPid_nop: {
                    // a =b +c*d 	// iADD 1 // de nem adtam hozzá még a c-t
                    //   -1-^
                    LOAD_SRC_ADR_nA1( scp, -1 );	/// move.l -1(Ai),A1
                    LOAD_DST_ADR_A0(scp);	/// move.l (Ai+1),A0
                    move_l_IA1I_IA0I(scp);	/// move.l (A1),(A0)

                    lADD = lMUL = 0;
                    *pSTRT = pADD[lADD];
                    scp.vASM.INST( PC );
                }
                return *this;
            default:
                break;
		}


		U4 s = ( gpaOPgrp[*pSTRT]==gpeOPid_mul ), a = 0, nA = lADD;
		if( s )
		{
			//  strt
			//  |  0  1
			//  V  v  v
			// d*e +f -g*h		// iADD 2 // de nem adtam hozzá még a g-t
			//     -1-^
			//     0 1  2
			//     v v  v
			// d*e +f-g +h*i	// iADD 3 // de nem adtam hozzá még a h-t
			//     -2-1-^
			*pSTRT = pADD[a++];
			LOAD_SRC_ADR_nA1(scp,s-lADD);	/// move.l n(Ai),A1
			move_l_IA1I_D0(scp);			/// move.l (A1),D0
			--nA;
		} else {
			//   strt
			//   |   0
			//   V   v
			// a +=b +c*d 	// iADD 1 // de nem adtam hozzá még a c-t
			//    -1-^
			//      0  1
			//      v  v
			// a *=b+c +d*e  // iADD 2 // de nem adtam hozzá még a d-t
			//    -2-1-^
			LOAD_SRC_ADR_nA1(scp,s-lADD);	/// move.l n(Ai),A1
			move_l_IA1I_D0(scp);			/// move.l (A1),D0
		}

		while( a < nA )
		{
			LOAD_SRC_ADR_nA1(scp,s-lADD);	/// move.l n(Ai),A1
			isa( PC, pADD[a], gpeEAszL
					,gpeEA_IAnI,1,0
					,gpeEA_Dn,0,0
			);
			a++;
			s++;
		}

		LOAD_DST_ADR_A0( scp );	/// move.l (Ai+1),A0
		isa( PC, *pSTRT, gpeEAszL
                ,gpeEA_Dn,0,0
				,gpeEA_IAnI,0,0
			);

		*pSTRT = pADD[lADD-1];
		isa( PC );
		lADD = lMUL = 0;
		return *this;
	}
	gpcCDsp& kMUL( gpcSCOOP& scp, U4 iOPe, bool b_end ) {

		if(!lMUL)
		{
			/// EZ ITT A VÉGE !!!!
            if( b_end	? (gpaOPgrp[*pSTRT] != gpeOPid_mul)
						: (
							   (gpaOPgrp[*pSTRT] == gpeOPid_add)
							|| (gpaOPgrp[*pSTRT] == gpeOPid_sub)
						)
			)
            {
                LOAD_SRC_ADR_nA1(scp,-1);	/// move.l n(Ai),A1
                move_l_IA1I_D0(scp);		/// move.l (A1),D0
                LOAD_DST_ADR_A0( scp );		/// move.l -(Ai+1),A0
                isa( PC, *pSTRT, gpeEAszL
                            ,gpeEA_Dn,0,0
                            ,gpeEA_IAnI,0,0
                    );
				isa( PC );

            }
			return *this;
		}

		//   strt
		//   V
		// a =b*c +d*d		// iMUL 1
		//   -2-1_^ 		// (Ai)+
		// a =b*c/d +e		// iMUL 2
		//   -3-2-1_^ 		// (Ai)+
		// a+=b*c/d +e		// iMUL 2
		//   -3-2-1_^ 		// (Ai)+

		//   strt
		//   V
		// a +b*c +d		// iMUL 1
		//   -2-1_^ 		// (Ai)+
		// a +b*c/d +e		// iMUL 2
		//   -3-2-1_^ 		// (Ai)+
		LOAD_SRC_ADR_nA1(scp,-(lMUL+1));	/// move.l n(Ai),A1
		move_l_IA1I_D0(scp);			/// move.l (A1),D0

		for( I4 i = 0; i < lMUL; i++ )
		{
			LOAD_SRC_ADR_nA1(scp,i-lMUL);	/// move.l n(Ai),A1
			isa( PC, pMUL[i], gpeEAszL
					,gpeEA_IAnI,1,0
					,gpeEA_Dn,0,0
			);
		}

		LOAD_DST_ADR_A0( scp );	/// move.l (Ai+1),A0
		isa( PC, *pSTRT, gpeEAszL
					,gpeEA_Dn,0,0
					,gpeEA_IAnI,0,0
			);

		*pSTRT = pMUL[lMUL-1];
		isa( PC );
		lADD = lMUL = 0;
		return *this;
	}
	gpcCDsp& kEND( gpcSCOOP& scp, I4 iOPe ) {
		// a =b +c*d 	// iADD 1 // de nem adtam hozzá még a c-t
		//   -1-^
		// a =b+c +d*e  // iADD 2 // de nem adtam hozzá még a d-t
		//   -2-1-^
		// d*e +f*g		// iADD 1
		//-2-1-^
		// d*e +f -g*h	// iADD 2
		//-3-2 -1-^
		kADD( scp, iOPe );
		kOBJ( scp, iOPe );
		kMUL( scp, iOPe, true );
        // a =b*c +d		// iMUL 1
		// a =b*c/d +e		// iMUL 2
		// a +=b*c/d +e		// iMUL 3
		// a +b*c +e		// iMUL 1
		// a*b +b*c			// iMUL 1
		//kOBJ( scp, iOPe );
		//kMUL( scp, iOPe );

		isa( PC, gpeOPid_sub, gpeEAszL
                            ,gpeEA_num,0,0
                            ,gpeEA_An,Ai+1,0
                    ).aOB[0] = 1;
		isa( PC );
        return *this;
    }

	gpcCDsp& knead( gpcSCOOP& scp, U4 iOPe ) {
		gpcCDsp& SP = *this;
		now=CDC.pst;
		switch( gpaOPgrp[now] )
		{
			case gpeOPid_mov: /// =
				kOBJ( scp, iOPe );
				kMOV( scp, iOPe ); // Ai--; // Ai--; //
				++SP;
				*pSTRT = now;
				break;
			case gpeOPid_add: /// +
			case gpeOPid_sub: /// ==
				kOBJ( scp, iOPe );
				// a =b*c +d		// iMUL 1
				// a =b*c/d +e		// iMUL 2
				// a +=b*c/d +e		// iMUL 3
				// a +b*c +e		// iMUL 1
				// a*b +b*c			// iMUL 1
				kMUL( scp, iOPe, false );
				LEVaddEXP()[lADD++] = now;
                ++SP;
                break;
			case gpeOPid_mul: /// *
				// a =b +c*d 	// iADD 1 // de nem adtam hozzá még a c-t
				//   -1-^
				// a =b+c +d*e  // iADD 2 // de nem adtam hozzá még a d-t
				//   -2-1-^
				// d*e +f*g		// iADD 1
				//-2-1-^
				// d*e +f -g*h	// iADD 2
				//-3-2 -1-^
				kADD( scp, iOPe );
				kOBJ( scp, iOPe );
				LEVmulEXP()[lMUL++] = now;
				++SP;
				++CDC;
				break;

			case gpeOPid_entry: { /// (
					switch( now )
					{
						case gpeOPid_dot:
							break;
						case gpeOPid_brakS:
						default:
							LEVup( scp );
							break;
					}
				++SP;
				++CDC;
				} break;
			case gpeOPid_out: /// )
                /// ITT MÉG FENT
                LEVdwn( scp, iOPe );
                /// ITT MÁR LENT


				++SP;
				++CDC;
				break;
            case gpeOPid_stk: /// ,
				kEND( scp, iOPe );
				Ai++;
                if( Ai > 6 )
                    Ai = 6;
				break;
		}
		return SP;
	}

};



#endif // GPCSRCLNK_H
