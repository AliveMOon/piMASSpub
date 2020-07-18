#ifndef GPCSRCLNK_H
#define GPCSRCLNK_H
#include "piMASS.h"

#define OBJ SCOOP.obj


//static const char* gpasTYPsz[] = {
//
//	".b",		//	0	1		00:00	byte
//	".w",		//	1	2		00:01	word
//	".l",		//	2	4		00:10	long
//	".q",		//	3	8		00:11	quad
//	/// olyan nincsen hogy float és nincs előjel
//	/// azaz ha nincs bepipálva az előjel bit akkor mást jelent
//	"rgba.l",	//  4   1x4		01:00	RGBA	pixel
//	"utf8.b",	//  5   1->0	01:01	string
//										//	12345678901234
//	"alf.q",	//	6	8		01:10	ABCDEFGHIJKLMN
//	"ce.q2",	//  7	16		01:11   ABCDEF 0x00000000 // 2D koordináta?
//	/// előjeles
//	"s.b",		//	8	1		10:00	signed byte
//	"s.w",		//	9	2		10:01	signed word
//	"s.l",		//	a	4		10:10	signed long
//	"s.q",		//	b	8		10:11	signed quad
//	/// lebegőpontos
//	".f",		//	c	4		11:00	float
//	".d",		//  d	8		11:01	double
//	".f4",		//	e	16		11:10	xyzw
//	".d4",		//  f	32		11:11	double xyzw
//};


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
static const char* gpasTYPsz[] = {

	".b",	//	0	1		00:00	byte
	".w",	//	1	2		00:01	word
	".l",	//	2	4		00:10	long
	".q",	//	3	8		00:11	quad
	/// olyan nincsen hogy float és nincs előjel
	/// azaz ha nincs bepipálva az előjel bit akkor mást jelent
	".4",	//  4   1x4		01:00	RGBA	pixel
	".u",	//  5   1->0	01:01	string
										//	12345678901234
	".a",	//	6	8		01:10	ABCDEFGHIJKLMN
	".c",	//  7	16		01:11   ABCDEF 0x00000000 // 2D koordináta?
	/// előjeles
	".B",	//	8	1		10:00	signed byte
	".W",	//	9	2		10:01	signed word
	".L",	//	a	4		10:10	signed long
	".Q",	//	b	8		10:11	signed quad
	/// lebegőpontos
	".f",	//	c	4		11:00	float
	".d",	//  d	8		11:01	double
	".F",	//	e	16		11:10	xyzw
	".K",	//  f	16		11:11	KID
};
typedef enum gpeCsz:U1{
	gpeCsz_b,	//	0	1		00:00	byte
	gpeCsz_w,	//	1	2		00:01	word
	gpeCsz_l,	//	2	4		00:10	long
	gpeCsz_q,	//	3	8		00:11	quad
	/// olyan nincsen hogy float és nincs előjel (lglbb is itt)
	/// azaz ha nincs bepipálva az előjel bit akkor mást jelent
	gpeCsz_4,	//  4   1x4		01:00	RGBA	pixel
	gpeCsz_u,	//  5   1->0	01:01	string
										//	12345678901234
	gpeCsz_a,	//	6	8		01:10	ABCDEFGHIJKLMN
	gpeCsz_C,	//  7	16		01:11   ABCDEF 0x00000000 // 2D koordináta?
	/// előjeles
	gpeCsz_B,	//	8	1		10:00	signed byte
	gpeCsz_W,	//	9	2		10:01	signed word
	gpeCsz_L,	//	a	4		10:10	signed long
	gpeCsz_Q,	//	b	8		10:11	signed quad
	/// lebegőpontos
	gpeCsz_f,	//	c	4		11:00	float
	gpeCsz_d,	//  d	8		11:01	double
	gpeCsz_F,	//	e	16		11:10	xyzw
	gpeCsz_K,	//  f	16		11:11	KID
} gpeCsz_U1;
static const U4 gpaCsz[] = {

	1,	//".b",	//	0	00:00	byte
	2,	//".w",	//	1	00:01	word
	4,	//".l",	//	2	00:10	long
	8,	//".q",	//	3	00:11	quad
	/// olyan nincsen hogy float és nincs előjel
	/// azaz ha nincs bepipálva az előjel bit akkor mást jelent
	4,	//".4",	//  4   01:00	RGBA	pixel
	1,	//".u",	//  5   01:01	string
										//	12345678901234
	8,	//".a",	//	6	01:10	ABCDEFGHIJKLMN
	16,	//".c",	//  7	01:11   ABCDEF 0x00000000 // 2D koordináta?
	/// előjeles
	1,	//".B",	//	8	10:00	signed byte
	2,	//".W",	//	9	10:01	signed word
	4,	//".L",	//	a	10:10	signed long
	8,	//".Q",	//	b	10:11	signed quad
	/// lebegőpontos
	4,	//".f",	//	c	11:00	float
	8,	//".d",	//  d	11:01	double
	16,	//".F",	//	e	11:10	xyzw
	16,	//".K",	//  f	11:11	KID
};
class gpcC {	/// CLASS
	/// ebben lehet bővebben
	/// mert ez a CLASS a "leírás"
public:
	U4x4	*pLST;	/// pLST[i].x&0xf gpeCsz // pLST[i].x>>4 iCnm 28bit 64millio
					/// y = z*w = a4x2[1].area()
	U4		*pKID, szOF, nLST;

	gpcC(){}
};
class gpcO { /// OBJ
	/// ez a példány az osztályból
	/// cID azonosítja mi van ebben
public:
	U1		*pALL;	/// a pointer a memóriára
	U4		nmID, cID, szOF, nA;	// dy = szOF/([cID].szOF*dx)
									// ha nA == 0 nem saját mem a pALL-ben
	U4x2	an;
	gpcO(){ gpmCLR; }
	~gpcO(){
		if(!nA)
			return;
		gpmDELary(pALL);
	}

	gpcO& operator = ( const gpcO& a ) {
		/// EZ komplet másolat
		U1* pD = pALL;
		U4 	nD = nA;

		gpmMcpyOF( this, &a, 1 );

		nA = gpmPAD( szOF+1, 0x10 );
		if( nD < nA )
		{
			gpmDELary(pD);
			pALL = new U1[nA+1];
		} else {
			pALL = pD;
			nA = nD;
		}
		gpmMcpy( pALL, a.pALL, szOF )[szOF]=0;
		return *this;
	}

	gpcO* operator = ( const gpcO* pA )
	{
		if( this == pA )
			return this;
		/// EZ referencia átadással müxik
		/// nem szabad törölni a pALL-t mert másé
		gpmMcpyOF( this, pA, 1 );
		nA = 0;
		return this;
	}

};

class gpcPIK {
public:
	gpcLZY	cLST,	// CLASS LIST
			oLST,	// OBJ LIST
			mem;	// foglalt memória
	U4 dcmID, dckID;

	gpcPIK(){ gpmCLR; dcmID = dckID = gpeCsz_L; } // gpeCsz_L kb. int
	U4 szOF( U4 sz )
	{
		if(sz<gpeCsz_K)
			return gpaCsz[sz];
		gpcC *pC = gpmLZYvali( gpcC, &cLST );
		if( !pC )
			return 0;
		sz-=gpeCsz_K;
		if( sz >= cLST.nLD(sizeof(*pC)) )
			return 0;

		return pC[sz].szOF;
	}
	gpcO* fnd( gpcO* pM, gpcO& dot, U4 nmID );
	gpcO* add( gpcO* pM, gpcO& dot, U4 nmID )
	{
		if( !pM )
		{
			U4 iO = oLST.nLD(sizeof(*pM));
			pM = (gpcO*)oLST.Ux( iO, sizeof(*pM) );
			// deklarácio most csak a nevének az ID-jét tudjunk
			gpmZ(*pM);
			pM->nmID = nmID;
			pM->cID = dcmID;
			return pM;
		}
		gpcO* pO = NULL;

		if( &dot != pO ) {
			dot = pO;
			pO = &dot;
		}

		gpcC *pCm = gpmLZYvali( gpcC, &cLST );
		if( !pCm )
			return NULL;
		pCm += pM->cID;
		if( pM->cID == dckID  )
		{
			/// nem lehet c ben c
		}
		/*pC->nmID = nmID;
		pC->cID = dckID;*/




		return pO;
	}

};

class gpC {
public:
	U4x4	*pLST;	/// pLST[i].x&0xf gpeCsz // pLST[i].x>>4 iCnm 28bit 64millio
					/// y = z*w = a4x2[1].area()
	U2		*pKID, nLST;
	U4		sOF;

	gpC(){}
};

class gpO {
public:

	U2	iNM,	// 0	// name id
		iC,		// 2	// class id
		iD,		// 4	// dim id	// 0 1x1
		iM;		// 6	// mom id	// szülő kapcsota

	U4	iX;		// 8	// index
				// 12	szOF



	gpO(){}
	gpO& operator = ( const gpO& b )
	{
		gpmMcpyOF( this, b, 1 );
		return *this;
	}
};

class gpCORE {
public:
	gpcLZY	amLST[0x11],	// mem data
			cLST,			// OBJ LIST
			dLST,
			oLST;			// CLASS LIST
	U4		pc, nPC;
	I4x4	*pALL, *pPC;
	I4x2	aR[8*4];


	gpCORE(){ gpmCLR; }
	I4x4* ini( I4x4* pA, U4 n, I4x2** ppA, I4x2** ppD ) {
		nPC = n;
		if( pALL != pA )
		{
			gpmZ(apR);
			pc = 0;
		}
		if( !pA )
			pc = nPC;
		if( ppA )
			*ppA = aR;
		if( ppD )
			*ppD = aR+8;
		pALL = pA;
		return pALL+pc;
	}
	I4x4* iPC( 	U1x4& op, gpeOPid& oID,
				U1& iS, gpeEA& mS,
				U1& iD, gpeEA& mD,
				U4& szOF
			) {
		if( pPC == pALL+pc )
			pc<nPC ? pALL+pc : NULL;

		pPC = pALL+pc;

		op = pPC->op;
		oID = (gpeOPid)op.x;
		if( !oID ) // nop
			return NULL;

		iS = op.y&7;
		mS = (gpeEA)(op.y>>3);

		iD = op.z&7;
		mD = (gpeEA)(op.z>>3);
		szOF = gpaEAsz[pPC->sz];

		return pPC;
	}
	gpO* adr( const I4x2& ix )
	{
		if( ix.x < 0x10 )
			return gpmLZYvali( gpO, amLST[ix.x] );

		return  (gpO*)oLST.Ux( ix.y,sizeof(gpO*));
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
	I4		iCD, refCD, nR, Ad, As, Di, nSTRT, iPC;
	gpcCD	*pCD;
	gpcLZY	cd, spA, spM, spSAM;

	gpeOPid *pADD, *pMUL, *pSTRT, now; //aSTRT[0x40];
	I4x4	iSAM, lSAM, nSAM, allSAM;

	gpcCDsp(){ gpmCLR; refCD=-1; Ad=6; LEVrst(); }; //A[7]=0x40; };
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

	I4x4& LOAD_SRC_ADR_nA0( gpcSCOOP& scp,I4 n=-1) { /// move.l n(iA),A0
		/// move.l -1(Ai), A0
		I4x4 &load = isa( PC, gpeOPid_mov, gpeEAszL
							,gpeEA_d16IAnI,As,0
							,gpeEA_An,0,0  );
		load.aOB[0] = n;
		return load;
	}
	I4x4& LOAD_SRC_ADR_nA1( gpcSCOOP& scp,I4 n=-1) { /// move.l n(iA),A1
		/// move.l -1(Ai), A1
		I4x4 &load = isa( PC, gpeOPid_mov, gpeEAszL
							,gpeEA_d16IAnI,As,0
							,gpeEA_An,1,0  );
		load.aOB[0] = n;
		return load;
	}
	I4x4& LOAD_DST_ADR_A0( gpcSCOOP& scp ) { /// move.l (iA+1),A0

		I4x4 &prev = isa( PC, gpeOPid_mov, gpeEAszL
							,gpeEA_IAnI,Ad,0
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
        pSPsam[-1].op = CD()[0].lnk >= gpeOPid_jsr ? CD()[0].lnk : 0;
        lSAM.null();

        LEVaddEXP();
        LEVmulEXP();

        pSTRT   = &pSPsam[0].pre;
        pADD    = (gpeOPid*)spA.Ux( iSAM.z, sizeof(gpeOPid), false );
        pMUL    = (gpeOPid*)spM.Ux( iSAM.w, sizeof(gpeOPid), false );


		isa( PC );
        isa( PC, gpeOPid_mov, gpeEAszL
				,gpeEA_An,As,0
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
				,gpeEA_sIAnI,Ad,0
			).aOB[0] = 1;
        //inst.aOB[0] = 1;
        isa( PC );
        return *this;
	}

	gpcCDsp& LEVdwn( gpcSCOOP& scp, //I4 iOPe,
						gpeOPid dwn ) {
        if( !pSTRT )
            return LEVrst();

        /// ITT MÉG FENT
        kEND( scp ); //,iOPe );
        *pSTRT = dwn;

        I4x4 *pSPsam = (I4x4*)spSAM.Ux(--nSTRT, sizeof(I4x4));

		/// ITT MÁR LENT
		if( pSPsam[0].op )
		{
			isa( PC, gpeOPid_jsr, gpeEAszL
				,gpeEA_num
			).aOB[0] = pSPsam[0].op-gpeOPid_jsr; //-gpeOPid_jsr;
		}
        lSAM.a4x2[1] = pSPsam[1].a4x2[1]-(iSAM.a4x2[1] = pSPsam[0].a4x2[1]);
        allSAM.a4x2[0] = allSAM.a4x2[1]-iSAM.a4x2[1];

        pSTRT   = &pSPsam[0].pre;
        pADD    = (gpeOPid*)spA.Ux( pSPsam[0].z, sizeof(gpeOPid));
        pMUL    = (gpeOPid*)spM.Ux( pSPsam[0].w, sizeof(gpeOPid));


        isa( PC, gpeOPid_mov, gpeEAszL
				,gpeEA_IAnIp,7,0
				,gpeEA_An,As,0
			);
        isa( PC );
		isa( PC, gpeOPid_mov, gpeEAszL
				,gpeEA_IAnIp,Ad,0
				,gpeEA_IAnIp,As
			);
        isa( PC );
        return *this;
	}


	gpcCDsp& kMOV( gpcSCOOP& scp
					//, U4 iOPe
					) {
		As = Ad;
		return *this;
//		/*isa( PC,	gpeOPid_mov, gpeEAszL
//                ,gpeEA_An,Ai,0
//                ,gpeEA_Dn,Ai,0
//            );*/
//		if( As >= Ad )
//		{
//			As = Ad-1;
//			isa( PC );
//			return *this;
//		}
//		As = Ad-1;
//		/*isa( PC, gpeOPid_mov, gpeEAszL
//                ,gpeEA_An,Ai,0
//				,gpeEA_Dn,Ai,0
//            );*/
//		isa( PC );
//		return *this;
	}
	gpcCDsp& kOBJ( gpcSCOOP& scp
				//, U4 iOPe
				) {
        switch( (gpeOPid)CD()[0].lnk ) {
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
			opcd.aOB[0] = (i.lnk > 0) ? i.lnk-gpeOPid_jsr : i.lnk;
			--dp;
		}
		/// "\n 0x%0.4x jsr fndOBJ2A0"
		isa( PC, gpeOPid_dot );
        if( gpaOPgrp[now] == gpeOPid_mov )	/// move.l A0,-(Ai)
            isa( PC, gpeOPid_mov, gpeEAszL
                    ,gpeEA_An,0,0
                    ,gpeEA_sIAnI,As,0
                );
		else 								/// move.l A0,(Ai)+
            isa( PC, gpeOPid_mov, gpeEAszL
                    ,gpeEA_An,0,0
                    ,gpeEA_IAnIp,As,0
                );

		isa( PC );
		As = Ad-1;
		return *this;
	}
	gpcCDsp& kADD( gpcSCOOP& scp
				//, U4 iOPe
				) {
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
	gpcCDsp& kMUL( gpcSCOOP& scp, //U4 iOPe,
					bool b_end ) {

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
	gpcCDsp& kEND( gpcSCOOP& scp ){
		// a =b +c*d 	// iADD 1 // de nem adtam hozzá még a c-t
		//   -1-^
		// a =b+c +d*e  // iADD 2 // de nem adtam hozzá még a d-t
		//   -2-1-^
		// d*e +f*g		// iADD 1
		//-2-1-^
		// d*e +f -g*h	// iADD 2
		//-3-2 -1-^
		kADD(scp);
		kOBJ(scp);
        // a =b*c +d		// iMUL 1
		// a =b*c/d +e		// iMUL 2
		// a +=b*c/d +e		// iMUL 3
		// a +b*c +e		// iMUL 1
		// a*b +b*c			// iMUL 1
		kMUL(scp,true);

		isa( PC );
        return *this;
    }

	/// azaz gyúrjuk
	gpcCDsp& knead( gpcSCOOP& scp ){ //, U4 iOPe ) {
		gpcCDsp& SP = *this;
		now=CDC.pst;
		switch( gpaOPgrp[now] )
		{
			case gpeOPid_mov: /// =
				kMOV(scp); //, iOPe ); // Ai--; // Ai--; //
				kOBJ(scp); //, iOPe );
				++SP;
				*pSTRT = now;
				break;
			case gpeOPid_add: /// +
			case gpeOPid_sub: /// ==
				kOBJ(scp); //, iOPe );
				// a =b*c +d		// iMUL 1
				// a =b*c/d +e		// iMUL 2
				// a +=b*c/d +e		// iMUL 3
				// a +b*c +e		// iMUL 1
				// a*b +b*c			// iMUL 1
				kMUL(scp,false);
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
				kADD(scp); //, iOPe );
				kOBJ(scp); //, iOPe );
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
							LEVup(scp);
							break;
					}
					++SP;
					++CDC;
				} break;
			case gpeOPid_out: /// )
                /// ITT MÉG FENT
                LEVdwn(scp,now);
                /// ITT MÁR LENT
				++SP;
				++CDC;
				break;
            case gpeOPid_stk: /// ,
				kEND(scp);
				break;
		}
		return SP;
	}

};



#endif // GPCSRCLNK_H
