#ifndef GPCRES_H
#define GPCRES_H

//#include "piMASS.h"

class gpcALU;
class gpcRES;
class gpcSRC;
class gpcWIN;
#include "gpcSRC.h"

//#include "gpcwin.h"
//class gpcWIN;

#include "gpcSCHL.h"
#include "gpcOPCD.h"

enum gpeTYP:U4
{
	// x[7s,6f,5r,4str : 3-0 nBYTE = 1<<(x&0xf) ]
	// yz dimxy
	// w size
	gpeTYP_STR 	= MAKE_ID( 0x10, 1, 1, 1 ),
	gpeTYP_U1 	= MAKE_ID( 0x00, 1, 1, 1 ),
	gpeTYP_U4 	= MAKE_ID( 0x02, 1, 1, 4 ),
	gpeTYP_I4 	= MAKE_ID( 0x82, 1, 1, 4 ),
	gpeTYP_F  	= MAKE_ID( 0xc2, 1, 1, 4 ),
	gpeTYP_U8 	= MAKE_ID( 0x03, 1, 1, 8 ),
	gpeTYP_I8 	= MAKE_ID( 0x83, 1, 1, 8 ),
	gpeTYP_D  	= MAKE_ID( 0xc3, 1, 1, 8 ),

	gpeTYP_A	= MAKE_ID( 0x93, 1, 1, 8 ),
	gpeTYP_AN 	= MAKE_ID( 0x93, 2, 1, 16 ),


	gpeTYP_U14 	= MAKE_ID( 0x00, 4, 1, 0 ),
	gpeTYP_U44 	= MAKE_ID( 0x02, 4, 1, 0 ),
	gpeTYP_I44 	= MAKE_ID( 0x82, 4, 1, 0 ),

	gpeTYP_STRmsk = ~gpeTYP_STR, //MAKE_ID( 0xef, 0xff, 0xff, 0xff ),
};

class gpcISA
{
public:
	I1x4	isa;			// 0	4		// x:gpeISA y:step
	U2		i, n, ix, iy;	// 4	2x4 8	//
	gpcRES	*pRES;			// 12	4?		// zárójeles cuccok

	U4x2	an;				// 16	8		// vagy AlfNum vagy VarIx
			//trg;			// 24	8		// egy target koordináta
							// 32
	gpcISA(){};
	gpcISA* null()
	{
		if( this )
		{
			if( *isa.aISA == gpeISA_str )
				gpmDELary(an.aSTR[0]);
			gpmDEL( pRES );
			gpmCLR;
		}

		return this;
	}

	gpcISA* str( gpcLZY& str )
	{
		if( !this )
			return NULL;
		isa.null();

		U4 nSTR = min( str.n_load, gpmSTRLEN( str.p_alloc ) );
		if( nSTR )
		{
			an.aSTR[0] = new U1[nSTR+1];
			gpmMEMCPY( an.aSTR[0], str.p_alloc, nSTR );
			an.aSTR[0][nSTR] = 0;
		} else {
			an = 0;
		}

		isa.aISA[0] = gpeISA_str;

		str.lzyRST();
		return this;
	}

	gpcISA* AN( const I8x2& _an )
	{
		if( !this )
			return NULL;
		isa.null();

		an = _an;
		isa.aISA[0] = gpeISA_an;
		return this;
	}
	gpcISA* var( gpeALF v, gpeISA is = gpeISA_var )
	{
		if( !this )
			return NULL;
		isa.null();

		an.var = v;
		ix = 0;
		isa.aISA[0] = is;
		return this;
	}

	gpcISA* trg( const U4x2& t )
	{
		if( !this )
			return NULL;
		isa.null();

		an = t;
		ix = 0;
		isa.aISA[0] = gpeISA_trg;
		return this;
	}

	U1 stp( I1x4 op )
	{
		if( !op.u4 )
			return 0;

		if( op.x )
		{
			return  op.x > 0 ? '+' : '-';
		}

		if( op.z&0x80 )
		{
			// tudom, hogy hülyeség, de egyenlőre legyen így
			return op.z < 0 ? gpeISA_rem : op.z;
		}

		if( op.y )
		{
			return  	abs(op.y) > 1 ?
						 (op.y > 0 ? gpeISA_exp : gpeISA_root )
						:(op.y > 0 ? gpeISA_mul : gpeISA_div ) ;
		}

		return 0;
	}
};

class gpcADR
{
public:
	gpcRES*	pRM;
	I4		dp, iA, nA;
	I8x2	an;

	gpcADR(){ gpmCLR; };

	gpcADR( gpeALF a )
	{
		gpmCLR;
		an.alf = a;
	}
	gpcADR& operator = ( gpeALF a )
	{
		gpmCLR;
		an.alf = a;
		return *this;
	}

	gpcADR& operator = ( gpcRES* pM );


	gpcADR( gpeALF a, gpcRES* pM )
	{
		*this = a;
		*this = pM;
	}

	gpcALU& adr2ALU( gpcRES* pM );


};
class gpcREG
{					// 	pi	x64
	U8		u;		// 	8	8
	I8		i;		// 	8	8
	double	d;		// 	8	8

	U1x4	bD;		// 	4	4	// x[7s,6f,5r,4str : 3-0 nBYTE = 1<<(x&0xf) ]
					//			// yz[ dimXY ] 		,  nBYTE = 1<<(x&0xf)
	U1*		pSTR;	///	4	8
public:
	U4x2	xy;		//	8	8

	gpcREG(){ bD = 0; };
	~gpcREG()
	{
		if( bD.u4 == gpeTYP_STR )
			gpmDELary( pSTR );
	}
	gpcREG& bad()
	{
		u = 1;
		i = -1;
		d = 0.1;
		bD.u4 = 0;
		return *this;
	}
	gpcREG& off()
	{
		if( bD.u4 == gpeTYP_STR )
				gpmDELary( pSTR );
		gpmCLR;
		return *this;
	}
	U1* getSTR()
	{
		if( this ? !pSTR : true )
			return NULL;
		U1* pS = pSTR;
		gpmCLR;
		return pS;
	}

	gpcREG& operator = ( U1* pU )
	{
		pSTR = gpfSTR( pSTR, pU );
		bD.u4 = gpeTYP_STR;
	}

	gpcREG& operator = ( U4 u4 )
	{
		u = u4;
		bD.u4 = 2;
		return *this;
	}
	gpcREG& operator = ( I4 i4 )
	{
		if( i4 < 0 )
		{
			i = i4;
			u = -i;
			bD.u4 = 0x82;
			return *this;
		}
		u = i4;
		bD.u4 = 2;
		return *this;
	}
	gpcREG& operator = ( float f4 )
	{
		i = (d = f4);
		bD.u4 = ( f4 < 0.0 ) ? 0x82 : 0;
		if( bD.u4 || ((float)i != f4) ) // neg? || float?
		{
			u = bD.u4 ? -i : i;
			bD.u4 = 0xc2;
			return *this;
		}

		u = i;
		bD.u4 = 0x42;
		return *this;
	}

	gpcREG& operator = ( U8 u8 )
	{
		u = u8;
		bD.u4 = 3;
		return *this;
	}
	gpcREG& operator = ( I8 i8 )
	{
		if( i8 < 0 )
		{
			i = i8;
			u = -i;
			bD.u4 = 0x83;
			return *this;
		}
		u = i8;
		bD.u4 = 3;
		return *this;
	}
	gpcREG& operator = ( double d8 )
	{
		i = (d = d8);
		bD.u4 = ( d < 0.0 ) ? 0x83 : 0;
		if( bD.u4 || ((double)i != d) ) // neg? || float?
		{
			u = bD.u4 ? -i : i;
			bD.u4 = 0xc3;
			return *this;
		}

		u = i;
		bD.u4 = 0x43;
		return *this;
	}

	U8 u8()
	{
		if( this ? !bD.x : true )
			return 0;
		return u;
	}
	I8 i8()
	{
		if( this ? !bD.x : true )
			return 0;

		if( bD.x&0x80 ) // csak ha 0x80 akkor volt negatív értéke a számnak
			return i;

		return u;

	}
	double d8()
	{
		if( this ? !bD.x : true )
			return 0.0;
		if( bD.u4&0xf0)
			return d;

		return u;
	}
	bool bGD()
	{
		return bD.x; //.u4&0x1f;
	}
	I1 t()
	{
		return bD.x;
	}

};

class gpcALU
{
	U1x4	type;	// typ:
					/// x[7s,6f,5r,4str : 3-0 nBYTE = 1<<(x&0xf) ]
					// yz[ dimXY ] 		,  nBYTE = 1<<(x&0xf)
	U4x4	an;
public:
	I1x4	op, isa;
	U4x2	sub;
	U4		nALL, iA;
	gpcRES	*pRM,
			*pRES;
	void	*pDAT;
	gpeALF 	alf;

	gpcALU(){ gpmCLR; };
	gpcALU( gpcRES* pM ); //= NULL );
	~gpcALU();

	gpcALU& TanDT( gpcALU& b )
	{
		// bizalmatlanok legyünk a b-vel?
		//gpmMEMCPY( this, &b, 1 );
		type = b.type;
		type.w = 0;
		an = b.an;
		an.w = 0;
		pDAT = b.pDAT;
		return *this;
	}

	U1x4& typ()
	{
		return type.typ();
	}
	U1x4& typ( U4 b )
	{
		type.u4 = b;
		type.w = 0;
		return typ();
	}
	U1x4& TYPmx( U4 b )
	{
		type.typMX( b );
		//type.u4 = b;
		//type.w = 0;
		return typ();
	}

	U4x2 tDIM()
	{
		typ();
		return U4x2(type.y,type.z);
	}


	U4 txySOF()
	{
		typ();
		return tDIM().area()*type.w;
	}
	U4 rowSOF()
	{
		return txySOF()*an.x;
	}


	U4x4& AN()
	{
		if( an.w )
			return an;
		/*if( !pDAT )
		{
			return an.null();
		}*/
		if( !an.x )
			an.x = 1;
		if( !an.y )
			an.y = 1;

		an.z = an.a4x2[0].area();
		an.w = an.z*typ().w;
		return an;
	}

	U4x2 trafo()
	{
		return U4x2( 1, AN().x )*txySOF();
	}

	U4x4& AN( U4x2 b )
	{
		an.w = 0;
		an.a4x2[0] = b;
		return AN();
	}
	U4x4& ANmx( U4x2 b )
	{
		an.w = 0;
		an.a4x2[0].mx(b);
		return AN();
	}
	gpcALU( const gpcALU& b )
	{
		gpmCLR;
		*this = b;
	}
	gpcALU& null()
	{
		gpmCLR;
        return *this;
	}
	U4 nDIM()
	{
		return typ().area_yz();
	}
	/*U4 nT1()
	{
		return 1<<(typ.x&0xf);
	}*/
	U4 nLOAD()
	{
		return AN().w;
	}
	U4 nALLOC()
	{
		return gpmPAD( AN().w+1, 0x10 );
	}

	gpcALU& equ( gpcALU& b );

	gpcALU& zero( void );
	U8 ins( gpcRES* pM, gpcLZY& str );
	gpcRES* ins( gpcRES* pM, gpcRES* pKID );
	gpcALU& ins( gpcRES* pM, U4x2 xy, U1x4 ty4 );
	gpcALU& int2flt( gpcRES* pM, U4x2 xy, U1x4 ty4 );

	U1*	ALUdatHARD( gpcRES* pM, U4x2 xy, U1x4 ty4, I1 mul );
	U1*	ALUdat( gpcRES* pM, U4x2 xy, U1x4 ty4, I1x4 op4, U8 u8 = 0, double d8 = 0.0 );
	gpcALU& equ( gpcRES* pM, U4x2 xy, U1x4 ty4, I1x4 op4, U8 u8, double d8, U2 dm = 0 );
	gpcALU& equSIG( gpcRES* pM, U4x2 xy, U1x4 ty4, I1x4 op4, U8 u8, U2 dm = 0 );
	gpcALU& equ( gpcRES* pM, U4x2 xy, U1x4 ty4, I1x4 op4, double u8, U2 dm = 0 );

	gpcALU& operator = ( gpcREG& a );
	gpcALU& operator += ( gpcREG& a );
	gpcALU& operator -= ( gpcREG& a );
	gpcALU& operator *= ( gpcREG& a );
	gpcALU& operator /= ( gpcREG& a );
	gpcALU& operator %= ( gpcREG& a );

	gpcALU& operator += ( gpcALU& b );
	gpcALU& operator -= ( gpcALU& b );
	gpcALU& operator *= ( gpcALU& b );
	gpcALU& operator /= ( gpcALU& b );
	gpcALU& operator %= ( gpcALU& b );


	gpcALU& operator = ( U1* pSTR );

	gpcALU& operator = ( gpeALF a )
	{
		null();
		alf = a;
		return *this;
	}

	gpcALU& operator = ( U4x2& u42 )
	{
		sub = u42;
		return *this;
	}

	U8 u8() {
		if( this ? !pDAT : true )
			return 0;
		// typ:
		// x[7s,6f,5r,4p? 	: 3-0 nBYTE = 1<<(x&0xf) ]
		// yz[ dimXY ] 		,  nBYTE = 1<<(x&0xf)
		if( typ().x&0x40 )
		{
			if( typ().w > 4 )
				return ((double*)pDAT)[0];

			return ((float*)pDAT)[0];
		}
		if( typ().x&0x80 )
		{
			if( typ().w > 2 )
			{
				if( typ().w > 4 )
					return ((I8*)pDAT)[0];

				return ((I4*)pDAT)[0];
			}
			if( typ().w > 1 )
				return ((I2*)pDAT)[0];

			return ((I1*)pDAT)[0];
		}
		if( typ().w > 2 )
		{
			if( typ().w > 4 )
				return ((U8*)pDAT)[0];

			return ((U4*)pDAT)[0];
		}
		if( typ().w > 1 )
			return ((U2*)pDAT)[0];
		else
			return ((U1*)pDAT)[0];

		return 0;
	}
	I8 i8() {
		if( this ? !pDAT : true )
			return 0;

		if( typ().x&0x40 )
		{
			if( typ().w > 4 )
				return ((double*)pDAT)[0];

			return ((float*)pDAT)[0];
		}
		if( typ().x&0x80 )
		{
			if( typ().w > 2 )
			{
				if( typ().w > 4 )
					return ((I8*)pDAT)[0];

				return ((I4*)pDAT)[0];
			}
			if( typ().w > 1 )
				return ((I2*)pDAT)[0];

			return ((I1*)pDAT)[0];
		}
		if( typ().w > 2 )
		{
			if( typ().w > 4 )
				return ((U8*)pDAT)[0];

			return ((U4*)pDAT)[0];
		}
		if( typ().w > 1 )
			return ((U2*)pDAT)[0];
		else
			return ((U1*)pDAT)[0];

		return 0;
	}
	double d8() {
		if( this ? !pDAT : true )
			return 0.0;

		if( typ().x&0x40 )
		{
			if( typ().w > 4 )
				return ((double*)pDAT)[0];

			return ((float*)pDAT)[0];
		}
		if( typ().x&0x80 )
		{
			if( typ().w > 2 )
			{
				if( typ().w > 4 )
					return ((I8*)pDAT)[0];

				return ((I4*)pDAT)[0];
			}
			if( typ().w > 1 )
				return ((I2*)pDAT)[0];

			return ((I1*)pDAT)[0];
		}
		if( typ().w > 2 )
		{
			if( typ().w > 4 )
				return ((U8*)pDAT)[0];

			return ((U4*)pDAT)[0];
		}
		if( typ().w > 1 )
			return ((U2*)pDAT)[0];
		else
			return ((U1*)pDAT)[0];

		return 0.0;
	}

};


class gpfFLG
{
public:
	U4	iT = 0,	// trg	TRG
		iA = 0,	// an	AN
		iV = 0,	// var	VR
		iD = 0, // reg	D
		iG = 0, // tag	TG
		iS = 0;	// str	STR;
	gpfFLG& null() { gpmCLR; };
	gpfFLG(){};


};
class gpcSTK
{
public:

	gpcSTK* pMOM;
	gpcRES* pCres;
	gpcSRC* pCsrc;

	U4x2	aTRG[8],
			aAN[8];
	gpcREG	D[8];
	gpeALF	aVR[8],
			aTG[8];
	U1*		apSTR[8];
	gpfFLG 	main,
			aFLG[8];
	U4	iF = 0, nF = 0;

	gpcSTK( gpcSTK* pM, gpcRES* pR, gpcSRC* pS ){ gpmCLR; pMOM = pM; pCres = pR; pCsrc = pS; };
	U4 stpFLG( void )
	{
		aFLG[nF] = main;
		iF = nF;
		nF++;
		main.null();
		return nF;
	}
	gpfFLG& iFLG()
	{
		return aFLG[iF];
	}
	gpeALF iVR( void )
	{
		if( aFLG[iF].iV >= main.iV )
			return gpeALF_null;

		return aVR[aFLG[iF].iV];
	}
	gpcREG& iD( gpcREG& o )
	{
		if( aFLG[iF].iD >= main.iD )
			return o.off();

		return D[aFLG[iF].iD];
	}
};

class gpcRES
{
	gpeALF	*pALF;
	I1x4	*pOP;	// x add/sub // y mul/div // z[7rem:mod 6not:! 5or:| 4and:& ]  //w ?
					// w size

	U1x4	*pTYP;	// x[7s,6f,5r,4str : 3-0 nBYTE = 1<<(x&0xf) ]
					// yz dimxy
	U4x4	*pAN;
	//U8x4	*pTREE;


	U4		// *pTx,
			*pnALL,
			nA, t,
			i, ig,
			iLEV, nASG;

	void	**ppDAT;
	gpcRES	**ppR, *pMOM;
	gpcALU	alu;

	gpcISA* pISA;
	U4x2	nISA;

public:
	gpcISA*	resISA( void )
	{
		if( !pISA )
			nISA = 0;

		if( nISA.x >= nISA.y )
		{
			gpcISA* pK = pISA;
			nISA.y = nISA.x+0x10;
            pISA = new gpcISA[nISA.y];
            if( nISA.x )
				gpmMEMCPY( pISA, pK, nISA.x );
			gpmZn( pK, nISA.x ); // muszáj nehogy felszabaditsa az an.aSTR[0]-okat
			gpmZn( pISA+nISA.x, nISA.y-nISA.x );

			gpmDELary(pK);
		}
		return pISA ? pISA+nISA.x : NULL;
	}
	U4 resISA_trg( U4 iT, const U4x2& t )
	{
		if( pISA )
		if( pISA[iT].isa.aISA[0] == gpeISA_trg )
		if( pISA[iT].an == t )
			return iT;

		iT = nISA.x;
		resISA()->trg( t );
		nISA.x++;
		resISA()->null();
		return iT;
	}
	gpcISA*	resISA_stp( U1 stp )
	{
		if( this ? !pISA : true )
			return NULL;
		gpcISA* pIS = resISA();
		if( pIS ? !pIS->isa.x : true )
			return NULL;

		pIS->isa.y = stp;
		if( !nISA.x )
		{
			nISA.x++;
			return resISA()->null();
		}


		nISA.x++;
		pIS = resISA()->null();
		switch( stp )
		{
			case '.':
				/*pIS->i = pIS[-1].i+1;
				pIS[-pIS->isa.z].n = pIS->i;
				if( !pIS[-pIS->isa.z].i )
					break;
				pIS[-pIS->isa.z].n -= pIS[-pIS->isa.z].i;*/
				pIS->i = pIS[-1].i+1;
				pIS[-pIS->i].n = pIS->i;
				if( !pIS[-pIS->i].i )
					break;
				pIS[-pIS->i].n -= pIS[-pIS->i].i;
			case '(':
				if( pIS[-1].isa.aISA[0] == gpeISA_var )
					pIS[-1].isa.aISA[0] = gpeISA_FUN;
		}

		return pIS; //resISA();
	}

	gpcISA*	resISA_an( I8x2& an )
	{
		return resISA()->AN( an );
	}
	gpcISA*	resISA_var( gpeALF a )
	{
		return resISA()->var( a );
	}
	gpcISA*	resISA_tag( gpeALF a )
	{
		return resISA()->var( a, gpeISA_tag );
	}

	gpcISA*	resISA_str( gpcLZY& str )
	{
		return resISA()->str( str );
	}

	U4 iL()
	{
		return iLEV;
	}
	gpcRES* pRM()
	{
		return pMOM;
	}
	gpcRES* null();

	gpcRES( gpcRES* pM = NULL )
	{
        gpmCLR;
        if( !pM )
			return;

		if( pMOM = pM )
			iLEV = pMOM->iLEV+1;
	}

	~gpcRES()
	{
		null();
	}

	gpcRES* 	compiEASY( U1* pS, U1* pE, U1** ppE, gpcRES* pMOM );
	//gpcRES* 	compiHARD( U1* pS, U1* pE, U1** ppE, gpcRES* pMOM );
	gpcLZY* 	res2mini( gpcLZY* pLZY, U1* pBUFF, gpcRES* pMOM, U4 deep );
	gpcRES* 	RESrun( gpcRES* pOUT, gpcLZY* pLZY, gpcWIN& win, gpcSRC* pSRC, gpcRES* pMOM, U4 deep = 0, gpcSTK* pSM = NULL  );

	gpcALU& ALU( U4 iA )
	{
		alu = this;

		if( iA >= nA )
			return alu;

		alu.iA 		= iA;
		alu.alf		= pALF	? pALF[iA]	: gpeALF_null;
		alu.op		= pOP	? pOP[iA]	: 0;
		alu.typ( pTYP ? pTYP[iA].u4	: 0 );	// x[7s,6f,5r,4str : 3-0 nBYTE = 1<<(x&0xf) ]
												// yz dimxy
		alu.AN( pAN	? pAN[iA].a4x2[0]	: 0 );
		alu.pDAT	= ppDAT	? ppDAT[iA]	: NULL;
		alu.pRES	= ppR	? ppR[iA]	: NULL;
		alu.nALL	= pnALL ? pnALL[iA]	: 0;
		return alu;
	}
	gpcRES* chg( gpcALU& ali )
	{
		if( !this )
			return NULL;

		if( ali.iA >= nA )
			return this;

		if( pALF[ali.iA] == ali.alf ) {

			if( !ppDAT )
			{
				if( ali.pDAT )
				{
					ppDAT = new void*[nA];
					gpmZn( ppDAT, nA );
				}
			}
			else if( ppDAT[ali.iA] != ali.pDAT )
				gpmDELary( ppDAT[ali.iA] );
			if( ppDAT )
				ppDAT[ali.iA] = ali.pDAT;



			if( !ppR )
			{
				if( ali.pRES )
				{
					ppR = new gpcRES*[nA];
					gpmZn( ppR, nA );
				}
			}
			else if( ppR[ali.iA] != ali.pRES )
				gpmDEL( ppR[ali.iA] );

			if( ppR )
				ppR[ali.iA] = ali.pRES;



			if( pOP )
				pOP[ali.iA] = ali.op;
			if( pTYP )
				pTYP[ali.iA] = ali.typ();
			if( pAN )
				pAN[ali.iA] = ali.AN();

			return this;
		}
		gpmDELary( ali.pDAT );
		gpmDELary( ali.pRES );

		return this;
	}

	gpcALU& ADD( gpeALF alf, U4 typ, U4 op ) {
		U4 nCPY = nA;
		nA++;


		/// ---------------------------------------------
		void* pKILL = ppDAT;
		ppDAT = new void*[nA];
		if( pKILL )
		{
			memcpy( ppDAT, pKILL, nCPY*sizeof(*ppDAT) );
			delete[] pKILL;
		}
		ppDAT[nCPY] = NULL;

		pKILL = pnALL;
		pnALL = new U4[nA];
		if( pKILL )
		{
			memcpy( pnALL, pKILL, nCPY*sizeof(*pnALL) );
			delete[] pKILL;
		}
		pnALL[nCPY] = 0;
		/// ---------------------------------------------


		pKILL = pALF;
		pALF = new gpeALF[nA];
		if( pKILL )
		{
			memcpy( pALF, pKILL, nCPY*sizeof(*pALF) );
			delete[] pKILL;
		}
		pALF[nCPY] = alf;

		pKILL = pOP;
		if( op || pKILL )
		{
			pOP = new I1x4[nA];
			pOP->u4 = 0;
			gpfMEMSET( pOP+1, nCPY, pOP, sizeof(*pOP) );
			if( pKILL )
			{
				memcpy( pOP, pKILL, nCPY*sizeof(*pOP) );
				delete[] pKILL;
			}
			pOP[nCPY].u4 = op;
		}

		pKILL = pTYP;
		pTYP = new U1x4[nA];
		if( pKILL )
		{
			memcpy( pTYP, pKILL, nCPY*sizeof(*pTYP) );
			delete[] pKILL;
		}
		pTYP[nCPY].u4 = typ;

		pKILL = pAN;
		pAN = new U4x4[nA];
		if( pKILL )
		{
			memcpy( pAN, pKILL, nCPY*sizeof(*pAN) );
			delete[] pKILL;
		}
		pAN[nCPY] = 0;


		return ALU( nCPY );
	}

	U4 nFND()
	{
		return this ? nA : 0;
	}
    U4 iFND( gpeALF alf )
    {
		if( !this )
			return 0;

		if( !nA )
			return nA;

		if( i < nA )
		if( pALF[i] == alf )
			return i;

		for( U4 j = 0, x; j < nA; j++ )
		{
			if( !pALF[j] )
				continue;

			if( pALF[j] != alf )
				continue;

			return i = j;
		}

		return nA;
	}



};
























#endif // GPCRES_H
