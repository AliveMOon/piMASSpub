#ifndef GPCSRCLNK_H
#define GPCSRCLNK_H
#include "piMASS.h"


/// gpCNST
#define scpCNST gpmSCP.cnst
/// gpCNST
#ifdef piMASS_DEBUG
	#define scpOBJget (pCNSTi=(gpCNST*)scpCNST.Ux( (cd.lnk-gpeOPid_jsr), sizeof(gpCNST)))[0]
	#define scpOBJadd (pCNSTn=(gpCNST*)scpCNST.Ux( gpmSCP.nCNST, sizeof(gpCNST)))[0]
#else
	#define scpOBJget ((gpCNST*)scpCNST.Ux( (cd.obj-gpeOPid_jsr), sizeof(gpCNST)))[0]
	#define scpOBJadd ((gpCNST*)scpCNST.Ux( gpmSCP.nCNST, sizeof(gpCNST)))[0]
#endif
//#define scpLNK gpmSCP.lnk

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


class gpCNST {
public:
	I8x2	obj;
	gpeTYP	typ;
	gpCNST(){};

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
	U2		*pKID,	/// kid-ben a nativ class ID van
			nLST, nKID;
	U4		sszOF;

	gpC(){}
	~gpC(){
		gpmDELary(pLST);
		gpmDELary(pKID);
	}
	U4 sOF( gpcLZY& cLST )
	{
		if( sszOF )
			return sszOF;

		for( U4 i = 0, iK = 0, nA, Csz; i < nLST; i++ ) {
			nA = pLST[i].coreARY();
			if( (Csz=pLST[i].coreCsz()) < gpeCsz_K )
			{
				sszOF += nA*gpaCsz[Csz];
				continue;
			}
			sszOF += nA*gpmLZYvali( gpC, &cLST )[pKID[iK]].sOF(cLST);
			iK++;
		}
		return sszOF;
	}
	U4 cIDfnd( gpcLZY& cLST, U4 cID, I4& Csz ) {
		if( !this )
			return -1;
		//Csz = gpeCsz_0;
		I4 ix = 0, add;
		U4 nA, nC = cLST.nLD();
		for( U4 i = 0, iK = 0; i < nLST; i++ ) {
			if( pLST[i].coreC() == cID )
			{
				Csz = pLST[i].coreC();
				return ix;
			}

			nA = pLST[i].coreARY();
			if( (Csz=pLST[i].coreCsz()) < gpeCsz_K ) {
				ix += nA*gpaCsz[Csz];
				continue;
			}
			ix += nA*gpmLZYvali( gpC, &cLST )[pKID[iK]].sOF(cLST);
			iK++;
		}
		return 0;
	}



};

class gpO {
public:

	U2	oID,		// 	0	// obj id
		cID,		/// 2	// class id // gpeCsz_ptr *pntr
		iD,		// 	4	// dim id	// 0 1x1
		iM;		// 	6	// mom id	// szülő kapcsota

	U4	iX,		// 	8	// index
		szOF;	// 	12	szOF
				// 	16



	gpO(){}
	gpO& operator = ( const gpO& b )
	{
		gpmMcpyOF( this, &b, 1 );
		return *this;
	}
};
class gpSTK {
public:
	I4	iX, x, oID, cID;
	gpC* pC;

	gpSTK(){}
	gpSTK& null() {
		gpmCLR;
		return *this;
	}
};
class gpINST {
public:
	U1x4 op;
	gpeOPid oID;
	gpeCsz cID;

	U1	iS, xS;
	gpeEA mS;

	U1	iD, xD;
	gpeEA mD;

	I4	aOB[2];

	gpINST& operator = ( const U4 u4 ) {
		gpmCLR;
		op = u4;
		oID = (gpeOPid)op.x;
		if( !oID ) // nop
			return *this;

		iS = op.y&7;
		xS = op.w&7;
		mS = (gpeEA)(op.y>>3);

		iD = op.z&7;
		xS = (op.w>>4)&7;
		mD = (gpeEA)(op.z>>3);
		return *this;
	}
	gpINST& operator = ( const I4x4 x4 ) {
		*this = x4.op;
		aOB[0] = x4.z;
		aOB[1] = x4.w;
		cID = x4.cID;
		return *this;
	}

	gpINST(){};
	gpINST( const I4x4 x4 ) { *this = x4; }
};
class gpCORE {
public:
	gpcLZY
			dLST,	// dim LIST
			cLST,	// CLASS LIST
			oLST,	// OBJ LIST
			mLST;	// mem data

	U4		pc, nPC, mSTK, oDPi,
			nSTK;
	I4x4	*pALL, *pPC;
	I8		aR[8*4], *pA, *pD, *pO, *pC;
	gpcO	aO[8];
	gpSTK	aSTK[0x10];

	~gpCORE(){
		if(	gpO* pO0 = gpmLZYvali( gpO, &oLST ) )
		{
			U4 n = oLST.nLD(sizeof(*pO0));
			for( U4 i = 0; i < n; i++ )
			{
				switch( pO0[i].cID )
				{
					case gpeCsz_ptr: {
							U4* pU4 = (U4*)mLST.Ux( pO0[i].iX, gpaCsz[gpeCsz_ptr], true, sizeof(U1) );
							char** ppSTR = (char**)(pU4+1);
							gpmDELary( *ppSTR );
						} continue;
					default:
						continue;
				}
			}
		}
	}

	gpCORE(){ gpmCLR; }
	I4x4* ini( I4x4* pAx4, U4 n, I8** ppA, I8** ppD, I8** ppO, I8** ppC ) {
		nPC = n;
		if( pALL != pAx4 )
		{
			mSTK = 0x2000;
			gpmZ(aR);
			pc = 0;
			aR[6] = mSTK>>1;
			aR[7] = mSTK;
		}
		if( !pAx4 )
			pc = nPC;

		pA = aR;
		if( ppA )
			*ppA = aR;
		pD = aR+8;
		if( ppD )
			*ppD = pD;
		pC = aR+0x10;
		if( ppC )
			*ppC = aR+0x10;
		pO = aR+0x18;
		if( ppO )
			*ppO = aR+0x18;
		pALL = pAx4;
		return pALL+pc;
	}
	I4x4* iPC( 	U1x4& op, gpeOPid& oID,
				U1& iS, U1& xS, gpeEA& mS,
				U1& iD, U1& xD, gpeEA& mD,
				gpeCsz& cID
			) {
		if( pPC == pALL+pc )
			pc<nPC ? pALL+pc : NULL;

		pPC = pALL+pc;

		op = pPC->op;
		oID = (gpeOPid)op.x;
		if( !oID ) // nop
			return NULL;

		iS = op.y&7;
		xS = op.w&7;
		mS = (gpeEA)(op.y>>3);

		iD = op.z&7;
		xD = (op.w>>4)&7;
		mD = (gpeEA)(op.z>>3);
		cID = pPC->cID; //gpaCsz[pPC->iC];

		return pPC;
	}
	U4 coreLNK( I4x4* pPC, U4 n, I8x4 *pM0, char *pSCPall, gpcLZY* pSCPlnk );
	gpeCsz C( U4& ix, U4 iC, U4 ixO ) {
		U4 off = ix-ixO;
		if( !off )
			return (gpeCsz)iC;
		if(iC<gpeCsz_K)
		{
			ix -= off%gpaCsz[iC];
			return (gpeCsz)iC;
		}
		gpC* pC0 = gpmLZYvali( gpC, &cLST );
		if( !pC0 )
		{
			ix = ixO;
			return gpeCsz_K;
		}

		iC -= gpeCsz_K;
		gpC	*piC = pC0+iC;
		U4x4	*piCL = piC->pLST, *pkCL;
		gpeCsz	Csz;
		U4		ary;
		for( U4 i = 0, n = piC->nLST, iK = 0, ixo = ixO, ixi = ixo, add; i < n; i++ )
		{
			Csz = piCL[i].coreCsz();
			ary = piCL[i].coreARY();
			if( Csz < gpeCsz_K )
			{
				add = ary*gpaCsz[Csz];
				ixi += add;
				if( ixi-ixo < off )
					continue;
				ix = ixi-add;
				return Csz;
			}

			iC = piC->pKID[iK];
			iK++;

			add = pC0[iC].sOF(cLST);
			ixi += add;
			if( ixi-ixo < off )
				continue;
			ixi -= add;

			iK = 0;
			i = 0;
			piC = pC0+iC;
			n = piC->nLST;
			piCL = piC->pLST;
		}

		ix = ixO;
		return gpeCsz_K;
	}

	gpO* oIDfnd( U4 oID, U4& iO ) {
		iO = 0;
		gpO* pO0 = gpmLZYvali( gpO, &oLST ), *pOi = NULL;
		if(	!pO0 )
			return NULL;

		for( U4 nO = oLST.nLD(sizeof(gpO)); iO < nO; iO++ )
		{
			if( pO0[iO].oID != oID )
				continue;
			return pO0+oID;
		}
		return NULL;
	}
	U4 sOF( U4 cID ) {
		if( cID < gpeCsz_K )
			return gpaCsz[cID];

		gpC* pC = gpmLZYvali(gpC,&cLST);
		if( !pC )
			return 0;
		cID -= gpeCsz_K;
		if( cID >= cLST.nLD(sizeof(gpcC)) )
			return 0;

		return pC[cID].sOF(cLST);
	}
	U4 ix_str( char* pSTR, U4 nSTR, U4 oID ) {

		U4 iO;
		gpO* pOi = oIDfnd( oID, iO );
		if( !pOi )
		{
			pOi = (gpO*)oLST.Ux( iO, sizeof(gpO) );
			pOi->oID = oID;
			pOi->iX = mLST.nLD();
			pOi->iD = 0;
			pOi->szOF = gpaCsz[pOi->cID=gpeCsz_ptr];
		}
		else if( pOi->cID == gpeCsz_ptr )
		{
			char** ppSTR = (char**)mLST.Ux( pOi->iX, pOi->szOF, true, sizeof(U1) );
			gpmDELary( *ppSTR );
		} else {
			pOi->iD = 0;
			if( pOi->szOF < sOF(pOi->cID) )
			{
				// felszabadítjuk az object hejét is
				pOi->oID = -1;
				pOi->cID = gpeCsz_0;

				// végére teszem
				pOi = (gpO*)oLST.Ux( iO, sizeof(gpO) );
				pOi->oID = oID;
				pOi->iX = mLST.nLD();
			}
			pOi->szOF = gpaCsz[pOi->cID=gpeCsz_ptr];
		}
		if( !nSTR )
			nSTR = gpmSTRLEN(pSTR);

		char** ppSTR = (char**)mLST.Ux( pOi->iX, pOi->szOF, true, sizeof(U1) );
		gpmDELary( *ppSTR );
		gpmMcpy( *ppSTR = new char[nSTR+1], pSTR, nSTR )[nSTR] = 0;
		return pOi->iX;
	}

	U4 ix_reg( gpcREG& reg, U4 oID ) {
		char* pSTR = (char*)reg.getSTR();
		if( pSTR )
			return ix_str( pSTR, gpmSTRLEN(pSTR), oID );

		U4 iO;
		gpO* pOi = oIDfnd( oID, iO );
		if( !pOi ) {
			pOi = (gpO*)oLST.Ux( iO, sizeof(gpO) );
			pOi->oID = oID;
			pOi->iD = 0;
			pOi->iX = mLST.nLD();
			pOi->szOF = gpaCsz[pOi->cID = gpeCsz_Q];
		} else {
			pOi->iD = 0;
			if( pOi->szOF < sOF(pOi->cID) )
			{
				// felszabadítjuk az object hejét is
				/// ide kell majd egy realloc
				pOi->oID = -1;
				pOi->cID = gpeCsz_0;

				// végére teszem
				pOi = (gpO*)oLST.Ux( iO, sizeof(gpO) );
				pOi->oID = oID;
				pOi->iX = mLST.nLD();
			}
			pOi->szOF = gpaCsz[pOi->cID=gpeCsz_Q];
		}

		I8* pI = (I8*)mLST.Ux( pOi->iX, pOi->szOF, true, sizeof(U1) );
		*pI = reg.i8();
		return pOi->iX;
	}
	U1* ea( U4 ix, I8* pO = NULL, I8* pC = NULL ) {
		if( pO ? ix >= mSTK : false )
		if(	gpO* pO0 = gpmLZYvali( gpO, &oLST ) ) {
			U4	L = 0,
				H = oLST.nLD(sizeof(*pO0)),
				i = H/2;
			pO[0] = 0;
			while( i>L ? i < H : false )
			{
				if( pO0[i].iX <= ix )
				{
					if( pO0[i].iX == ix )
					{
						L = i+1;
						break;
					}
					if( i+1 >= H )
					{
						L = i+1;
						i = 0;
						break;
					}
					if( pO0[i+1].iX > ix )
					{
						L = i+1;
						break;
					}
					H = i;
				} else
					L = i;
				i = (H+L)/2;
			}
			pO[0] = i;
			if( i==L)
				pO0+=L;

			U4 iC = (U4)C( ix, pO0[0].cID, pO0[0].iX );
			if( ix == pO0[0].iX )
				iC = pO0[0].cID;

			if( pC )
				*pC = iC;


		}
		return mLST.Ux( ix, sizeof(I4x4), true, sizeof(U1) );
	}



	U4 entryOBJ2A0( I8x4 *pM0, char	*pSCPall, gpcLZY* pSCPlnk, gpcWIN* pWIN, U8& bSW, U4 newC = gpeCsz_L );
	gpC* pCLASS( U4 iC )
	{
		if( iC < gpeCsz_K )
			return NULL;
		return gpmLZYvali( gpC, &cLST ) + iC-gpeCsz_K;
	}
	gpO* pOBJlnk( U4 i, U4 oID, U4 cID, bool bALLOC ) {
		U4 iO;
		gpO* p_o = oIDfnd( oID, iO );
		if( !p_o )
		{
			p_o = (gpO*)oLST.Ux( i, sizeof(gpO) );
			p_o->oID = oID;
			p_o->cID = cID;
			p_o->iD = 0;
			p_o->iX = 0;
			p_o->szOF = sOF(cID);
		}
		aSTK[0].iX = p_o->iX;
		aSTK[0].oID = p_o->oID;
		aSTK[0].pC = pCLASS( aSTK[0].cID = p_o->cID );

		if( !bALLOC )
			return p_o;
		mLST.Ux( p_o->iX = mLST.nLD(), p_o->szOF, true, sizeof(U1) );
		return p_o;
	}
	gpC* pCLASSlnk( U4 nSTK ) {
		if( !nSTK )
			return NULL;

		aSTK[nSTK].iX = aSTK[nSTK-1].iX;
		aSTK[nSTK].x = aSTK[nSTK-1].pC->cIDfnd( cLST, aSTK[nSTK].oID, aSTK[nSTK].cID );
		if( aSTK[nSTK].x > -1 )
			return aSTK[nSTK-1].pC;	/// megvan

		gpC* pC = aSTK[nSTK-1].pC;
		if( !pC )
			aSTK[nSTK-1].pC = pC = (gpC*)cLST.Ux( cLST.nLD(sizeof(*pC)), sizeof(*pC), true );

		U4x4 *poLST = pC->pLST;
		pC->pLST = new U4x4[pC->nLST+1];
		gpmMcpyOF( pC->pLST, poLST, pC->nLST );
		gpmDELary(poLST);

		if( aSTK[nSTK].cID >= gpeCsz_K )
		{
			U2* poKID = pC->pKID;
			pC->pKID = new U2[pC->nKID+1];
			gpmMcpyOF( pC->pKID, poKID, pC->nKID );
			gpmDELary(poKID);
			pC->pLST[pC->nLST] = gpeCsz_K;
			++(pC->nKID);
		} else {
			pC->pLST[pC->nLST] = aSTK[nSTK].cID;
		}
		++(pC->nLST);

		return aSTK[nSTK-1].pC;
	}
};

#define scpPC (iPC=scp.nASM())
#define CDC (CD()[0])
#define scpINST scp.vASM.INST
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
		I4x4 &load = scpINST( scpPC, gpeOPid_mov, gpeCsz_l
							,gpeEA_d16IAnI,As,0
							,gpeEA_An,0,0  );
		load.aOB[0] = n*gpaCsz[gpeCsz_l];
		return load;
	}
	I4x4& LOAD_SRC_ADR_nA1( gpcSCOOP& scp,I4 n=-1) { /// move.l n(iA),A1
		/// move.l -1(Ai), A1
		I4x4 &load = scpINST( scpPC, gpeOPid_mov, gpeCsz_l
							,gpeEA_d16IAnI,As,0
							,gpeEA_An,1,0  );
		load.aOB[0] = n*gpaCsz[gpeCsz_l];
		return load;
	}
	I4x4& LOAD_DST_ADR_A0( gpcSCOOP& scp ) { /// move.l (iA+1),A0

		I4x4 &prev = scpINST( scpPC, gpeOPid_mov, gpeCsz_l
							,gpeEA_IAnI,Ad,0
							,gpeEA_An,0,0
						);
		return prev;
	}

	I4x4& move_l_IA1I_IA0I(gpcSCOOP& scp) { /// move.l (A1),(A0)
		I4x4& inst = scpINST( scpPC, gpeOPid_mov, gpeCsz_l
							,gpeEA_IAnI,1,0
							,gpeEA_IAnI,0,0
						);
		return inst;
	}
	I4x4& move_l_IA1I_D0(gpcSCOOP& scp) { /// move.l (A1),D0

		I4x4& inst = scpINST( scpPC, gpeOPid_mov, gpeCsz_l
							,gpeEA_IAnI,1,0
							,gpeEA_Dn,0,0
						);
		return inst;
	}
	I4x4& move_l_IA1I_D1(gpcSCOOP& scp) { /// move.l (A1),D1

		I4x4& inst = scpINST( scpPC, gpeOPid_mov, gpeCsz_l
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


		scpINST( scpPC );
        scpINST( scpPC, gpeOPid_mov, gpeCsz_l
				,gpeEA_An,As,0
				,gpeEA_sIAnI,7,0
			);
        scpINST( scpPC );
        scpINST( scpPC );
        scpINST( scpPC, gpeOPid_xor, gpeCsz_l
				,gpeEA_Dn,0,0
				,gpeEA_Dn
			);
        //I4x4& inst =
		scpINST( scpPC, gpeOPid_mov, gpeCsz_l
				,gpeEA_Dn,0,0
				,gpeEA_sIAnI,Ad,0
			).aOB[0] = 1;
        //inst.aOB[0] = 1;
        scpINST( scpPC );
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
			scpINST( scpPC, gpeOPid_jsr, gpeCsz_l
				,gpeEA_num
			).aOB[0] = pSPsam[0].op-gpeOPid_jsr; //-gpeOPid_jsr;
		}
        lSAM.a4x2[1] = pSPsam[1].a4x2[1]-(iSAM.a4x2[1] = pSPsam[0].a4x2[1]);
        allSAM.a4x2[0] = allSAM.a4x2[1]-iSAM.a4x2[1];

        pSTRT   = &pSPsam[0].pre;
        pADD    = (gpeOPid*)spA.Ux( pSPsam[0].z, sizeof(gpeOPid));
        pMUL    = (gpeOPid*)spM.Ux( pSPsam[0].w, sizeof(gpeOPid));


        scpINST( scpPC, gpeOPid_mov, gpeCsz_l
				,gpeEA_IAnIp,7,0
				,gpeEA_An,As,0
			);
        scpINST( scpPC );
		scpINST( scpPC, gpeOPid_mov, gpeCsz_l
				,gpeEA_IAnIp,Ad,0
				,gpeEA_IAnIp,As
			);
        scpINST( scpPC );
        return *this;
	}


	gpcCDsp& kMOV( gpcSCOOP& scp ) { As = Ad; return *this; }
	gpcCDsp& kOBJ( gpcSCOOP& scp ) {
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
		scpINST( scpPC, gpeOPid_mov, gpeCsz_l
				,gpeEA_An,7,0
				,gpeEA_Dn,0,0
			);
		while(dp>-1) {
			gpcCD &i = pCD[0-dp];

		/// move.l 0xOBJid,-(A7) ; PUSH
			I4x4 &opcd = scpINST( scpPC, gpeOPid_mov, gpeCsz_l
								,gpeEA_num,0,0
								,gpeEA_sIAnI,7,0
							);
			//--A[7];
			opcd.aOB[0] = (i.lnk > 0) ? i.lnk-gpeOPid_jsr : i.lnk;
			--dp;
		}
		/// "\n 0x%0.4x jsr fndOBJ2A0"
		scpINST( scpPC, gpeOPid_dot );
        if( gpaOPgrp[now] == gpeOPid_mov )	/// move.l A0,-(Ai)
            scpINST( scpPC, gpeOPid_mov, gpeCsz_l
                    ,gpeEA_An,0,0
                    ,gpeEA_sIAnI,As,0
                );
		else 								/// move.l A0,(Ai)+
            scpINST( scpPC, gpeOPid_mov, gpeCsz_l
                    ,gpeEA_An,0,0
                    ,gpeEA_IAnIp,As,0
                );

		scpINST( scpPC );
		As = Ad-1;
		return *this;
	}
	gpcCDsp& kADD( gpcSCOOP& scp ) {
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
                    scp.vASM.INST( scpPC );
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
			scpINST( scpPC, pADD[a], gpeCsz_l
					,gpeEA_IAnI,1,0
					,gpeEA_Dn,0,0
			);
			a++;
			s++;
		}

		LOAD_DST_ADR_A0( scp );	/// move.l (Ai+1),A0
		scpINST( scpPC, *pSTRT, gpeCsz_l
                ,gpeEA_Dn,0,0
				,gpeEA_IAnI,0,0
			);

		*pSTRT = pADD[lADD-1];
		scpINST( scpPC );
		lADD = lMUL = 0;
		return *this;
	}
	gpcCDsp& kMUL( gpcSCOOP& scp, bool b_end ) {

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
                scpINST( scpPC, *pSTRT, gpeCsz_l
                            ,gpeEA_Dn,0,0
                            ,gpeEA_IAnI,0,0
                    );
				scpINST( scpPC );

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
			scpINST( scpPC, pMUL[i], gpeCsz_l
					,gpeEA_IAnI,1,0
					,gpeEA_Dn,0,0
			);
		}

		LOAD_DST_ADR_A0( scp );	/// move.l (Ai+1),A0
		scpINST( scpPC, *pSTRT, gpeCsz_l
					,gpeEA_Dn,0,0
					,gpeEA_IAnI,0,0
			);

		*pSTRT = pMUL[lMUL-1];
		scpINST( scpPC );
		lADD = lMUL = 0;
		return *this;
	}
	gpcCDsp& kEND( gpcSCOOP& scp ) {
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

		scpINST( scpPC );
        return *this;
    }

	/// azaz gyúrjuk
	gpcCDsp& knead( gpcSCOOP& scp ) { //, U4 iOPe ) {
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
