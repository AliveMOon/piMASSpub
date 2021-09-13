#ifndef GPCGT_H
#define GPCGT_H
//~ MIT License
//
//~ Copyright (c) 2019 AliveMOon
//
//~ Permission is hereby granted, free of charge, to any person obtaining a copy
//~ of this software and associated documentation files (the "Software"), to deal
//~ in the Software without restriction, including without limitation the rights
//~ to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//~ copies of the Software, and to permit persons to whom the Software is
//~ furnished to do so, subject to the following conditions:
//
//~ The above copyright notice and this permission notice shall be included in all
//~ copies or substantial portions of the Software.
//
//~ THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//~ IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//~ FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//~ AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//~ LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//~ OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//~ SOFTWARE.

#include "gpcpic.h"
#include "sim7x00.h"

#define gpmGTent (sGTent[0]?(char*)sGTent:"\r\n")

#define gpdSLMPnDEV 512
				          //		             +------- Len: 0x28 = 32+8 = 40 --------+
				          //   SNo.NnSnUn..MsLen.Mtm.Com.Sub.D.Slot..Nw..Data............
				          //   +-->+>+>+-->+>+-->1--4   8  12  16  20  24  28  32  36  40
				          //   500000FF03FF000028000014010000D*0000100004
				          //   +-->+>+>+-->+>+-->.x00.x04.x08.x0C.x10.x14.x18.x1c.x20  28  32  36  40
				          // \n500000FF03FF000020000014010000D*000000000265686f6c
#define gpdSLMP_recv_MdLN4SL6N4 "500000FF03FF00%0.4X000004010000M*%0.6d%0.4X"
#define gpdSLMP_recv_DdLN4SL6N4 "500000FF03FF00%0.4X000004010000D*%0.6d%0.4X"//"0000"
#define gpdSLMP_recv_YxLN4SL6N4 "500000FF03FF00%0.4X000004010000Y*%0.6X%0.4X"

#define gpdSLMP_recv_LN4SL6N4 gpdSLMP_recv_DdLN4SL6N4

						  //   SNo.NnSnUn..MsLen.Mtm.Com.Sub.D.Slot..Nw..
#define gpdSLMP_send_LN4SL6N4 "500000FF03FF00%0.4X000014010000D*%0.6d%0.4X"
//#define gpdRECVn (0x30000/12)
#define gpdGTlzyID		I8x2(1,gpdLZYallGT)
#define gpdGTlzyIDinp( p ) ((p)&gpdGTlzyID)
#define gpdGTlzyIDref( p ) (((p)&gpdGTlzyID)+I8x2(0,1))
#define gpdGTlzyIDusr( p ) (((p)&gpdGTlzyID)+I8x2(0,2))
#define gpdGTlzyIDdif( p ) (((p)&gpdGTlzyID)+I8x2(0,3))


#define gpdZSstrt 1
#define gpdZSbad 10


#define ZShs1 0x1
#define ZShs2 0x2
#define ZShs3 0x4

#define ZShs1i 0x1
#define ZShs1o 0x2
#define ZShs2i 0x4
#define ZShs2o 0x8
#define ZShs3i 0x10
#define ZShs3o 0x20


//#define gpdSLMP GTslmpDrc // GTslmp
//#define gpdSLMPos GTdrcOS	//GTslmpOS
#define gpdZSnW 	gpmOFFOFF( gpcZS, io128.y, oMxyzEspd )
#define gpdZSnWu2	(gpdZSnW/sizeof(U2))
#define gpdZSnR		sizeof(gpcZS) //gpdZSnW+sizeof(I4x4)
#define gpdZSnRu2	(gpdZSnR/sizeof(U2))

#define gpdFRMThtml_D 	"<!DOCTYPE html>\r\n"\
						"<html>\r\n"\
						"<head></head>\r\n"\
						"<body>\r\n"\
						"<h1>%d</h1>\r\n"\
						"</body>\r\n"\
						"</html>\r\n\r\n\0"

#define gpdFRMThtml_S 	"<!DOCTYPE html>\r\n"\
						"<html>\r\n"\
						"<head></head>\r\n"\
						"<body>\r\n"\
						"<h1>%s</h1>\r\n"\
						"</body>\r\n"\
						"</html>\r\n\r\n"

//------------------------
// 			HEAD
//------------------------
static const I4x4 gpaCAGEheadBALLhand[] = {
	{ mmX(0), mmX(0), mmX(0),	mmX(130) },
	{ mmX(0), mmX(0), mmX(200),	mmX(100) },
	{ mmX(0), mmX(0), mmX(330),	mmX(30) },
};
static const I4x4 gpaCAGEheadBALLpaint[] = {
	{ mmX(0), mmX(0), mmX(0),	mmX(130) },
	{ mmX(0), mmX(0), mmX(100),	mmX(100) },
	{ mmX(0), mmX(0), mmX(180),	mmX(20) },
};
#define gpaCAGEheadBALL gpaCAGEheadBALLhand //paint
static char gpsJDpub[0x100];

typedef enum gpeCGhd:U4 {
	gpeCGhdP0,
	gpeCGhdP1,
	gpeCGhdL01,
	gpeCGhdAB,
	gpeCGhdPC,
	gpeCGhdPN,
	gpeCGhdPNN,
};

class gpcRES;
class gpcDrc;
class gpcGSM;
class gpcWIRE;

class gpcROB {
public:
	U4x4	DEDI;					// b000	b032 b064  b096
	U4		dedi, name;				// b128 b160
	I2		aXYZ00[3], aABC00[3],	// b192 b208 b224	// b240 b256 b272
			aXYZ01[3], aABC01[3],	// b288 b304 b320	// b336 b352 b368
			aXYZ10[3], aABC10[3],	// b384 b400 b416	// b432 b448 b464
			aXYZ11[3], aABC11[3];	// b480 b496 b512	// b528 b544 b560
	U4		msS, inARY, COMnHS,		// b576 b608 b640
			/// b480 idáig küldjük
			msSLV, msMST, msR2D,	// b672 b704 b736
			pad,
			/// B640 idáig pullingol
			iW, nW, nWu2,
			nR, nRu2;
	/*I4x4	oMxyzEspd,	// b1024
			oMabcNsnt,	// b1280
			oEkey;		// b1312
			//b1308*/

	gpcROB& null() {
		gpmCLR;
		iW = gpmOFF(gpcROB,dedi);
		nW = gpmOFF(gpcROB,msSLV);
		nR = gpmOFF(gpcROB,iW);
		nWu2 = nW/sizeof(U2);
		nRu2 = nR/sizeof(U2);
		return *this;
	}
	gpcROB(){ null(); }
	gpcROB& operator = ( const gpcDrc& D );
	gpcROB& operator &= ( const gpcDrc& D );
	gpcROB( const gpcDrc& D );
	U2* piARY() { return ((U2*)&inARY); }
	U2* pnARY() { return ((U2*)&inARY)+1; }
	U2* pCOM()	{ return ((U2*)&COMnHS); }
	U2* pHS()	{ return ((U2*)&COMnHS)+1; }
	/*U2 ciARY() { return ((U2*)&inARY)[0]; }
	U2 cnARY() { return ((U2*)&inARY)[1]; }
	U2 cCOM() { return ((U2*)&COMnHS)[0]; }
	U2 cHS() { return ((U2*)&COMnHS)[1]; }*/
};

class gpcZS {
public:
	U4x4	DEDI,		// b0
			io128;		// b128
	I4		aPOS[3],	// b256
			aABC[3],	// b352
			apos[3],	// b448	//ms
			aabc[3],	// b544
            aJ16[6],	// b640
            aj16[6];	// b832

	I4x4	oMxyzEspd,	// b1024
			oMabcNsnt,	// b1280
			oEkey;		// b1312
			//b1308
	gpcZS& null() { gpmCLR; return *this; }
	gpcZS(){
		null();
	};
	gpcZS& operator = ( const gpcDrc& D );
	gpcZS& operator &= ( const gpcDrc& D );
	gpcZS( const gpcDrc& D );

};

class gpcWIN;
class gpcGT;

class gpcDrc {
public:
	I4x4 	NMnDIF,
			tXYZ, aoXYZ[4], aiXYZ[2],
			tABC, aoABC[4], aiABC[2],
			txyz, oxyz, ixyz,
			tabc, oabc, iabc;
			//aoAX1to6[2], aiAX1to6[2],
			//aoax1to6[2], aiax1to6[2];
	I2x2	iARY,
			oARY;
	U4x4	oCTRL, iCTRL, JD;	// JD.z error num
	I4x4	okXYZ, okABC, okxyz,
			tGRP, oGRP, iGRP,
			jdPRG, jdPRGtool,

			jd0PRG,
			jd0XYZ, jd0ABC, jd0xyz,
			jd1XYZ, jd1ABC, jd1up;
	F4x4	jd0mx, jd0mxTL;
	gpeALF	jdALF;
	I4x4	msSMR2, msSRT3;
	gpcLZY	lzyROAD, lstSTP;
	U4		nMS,	sMS,	AVGms, Ems,
			MPosS,	HS1ms,	n_trd, n_join;
	/// gpmZn ----------------------
	std::thread trd;

	~gpcDrc() {
		if( n_join >= n_trd )
			return;
		trd.join();
	}
	gpcDrc& operator = ( const gpcDrc& d ) {
		gpmMcpy( this, &d, gpmOFF(gpcDrc,n_trd) );
		if( n_join < n_trd )
			trd.join();
		n_trd = n_join = 0;
	}



	I4x4*	pBALLtool( U4 i );
	I4x4 	cageXYZ( I4x4 trg, I4 lim, I4x4* pBOX, U4 nBOX, I4x4* pBALL, U4 nBALL, int rR );
	I4x4 	cageXYZ( I4 lim, U4 id, int rR );

	I4x4 	cageXYZ( I4x4 trg, I4 lim, U4 id, int rR );

	I4x4 	cageBALL( I4x4 T, I4x4* pCAGE, U4 n, int rR );
	I4x4 	cageBOX( I4x4 T, I4x4* pCAGE, U4 n, int rR );

	gpcDrc( const gpcZS& zs, U4 nm = 0  );
	gpcDrc& operator = ( const gpcZS& zs );
	gpcDrc& judoZS( gpcZS& inp, U4 mSEC );

	gpcDrc& operator &= ( gpcDrc& in );
	gpcDrc( const gpcROB& rob, U4 nm = 0 )
	{
		if( nm != NMnDIF.au4x2[0].x )
			format( nm );
		*this = rob;
	}
	gpcDrc& operator = ( gpcROB& rob );


	bool operator == ( const gpcDrc& b ) const {
		if( sizeof(b) == gpmMcmp( &b, this, sizeof(b) ) )
			return true;

		return false;
	}
	bool operator != ( const gpcDrc& b ) const { return !(*this == b); }
	bool asyncSYS( char* pBUFF, U1* pCLI );

	gpcDrc& format( U4 nm = 0 ) {
		U4 nn = gpmOFF(gpcDrc,n_trd);

		gpmZn( this, nn );
		//gpmCLR;
		if( !nm )
			return *this;

		NMnDIF.x = nm;
		aiXYZ[0] = aoXYZ[0] = tXYZ = I4x4( 400,  0,	300+400,	gpeZS_POS0)&I4x2(mmX(1),1).xxxy();
		aiABC[0] = aoABC[0] = tABC = I4x4( 180,  0, 0,			gpeZS_DIR0)&I4x2(degX(1),1).xxxy();
		aiXYZ[0].w = gpeZS_iPOS; 	aiABC[0].w = gpeZS_iDIR;
		aoXYZ[0].w = gpeZS_oPOS; 	aoABC[0].w = gpeZS_oDIR;
		tXYZ.w = gpeZS_tPOS; 		tABC.w = gpeZS_tDIR;
		// OFFSET - eltolás
		oxyz.w = gpeZS_opos;
		ixyz.w = gpeZS_ipos;
		txyz.w = gpeZS_tpos;
		oabc.w = gpeZS_odir;
		iabc.w = gpeZS_idir;
		txyz.w = gpeZS_tpos;

		oCTRL.x = gpeZS_oCTR;
		iCTRL.x = gpeZS_iCTR;
		n_trd = n_join = 0;
		return *this;
	}
	gpcDrc( char* pbuff, I4x4 a, I4x4 b, I4x4 c );
	gpcDrc() { format(); n_trd = n_join = 0; };


	U4 hs123() {
		if(!this )
			return 0;

		return	 (bHS1i()<<0x14)|(bHS1o()<<0x10)
				|(bHS2i()<<0x0c)|(bHS2o()<<0x08)
				|(bHS3i()<<0x04)|(bHS3o());
	}
	int		answINFO( char*		pANS, U4 id, int rR );
	gpcLZY* answINFO( gpcLZY* 	pANS, U4 id, int rR );
	gpcLZY* answSTAT( gpcLZY* 	pANS, U4 id, int rR, char* pPP = "//" );

	int		answINFOX( char*	pANS, U4 id, I4 x );
	gpcLZY* answINFOX( gpcLZY*	pANS, U4 id, I4 x );

	bool bHS1i() const { return !!(iCTRL.y&ZShs1);	}
	bool bHS1o() const { return !!(oCTRL.y&ZShs1);	}
	U4 oHS1o() {
		iCTRL.y&=(~ZShs1);
		return (oCTRL.y|=ZShs1);
	}
	U4 xHS1o() {
		iCTRL.y|=ZShs1;
		return (oCTRL.y&=(~ZShs1));
	}

	bool bHS2i() const  { return !!(iCTRL.y&ZShs2);	}
	bool bHS2o() const  { return !!(oCTRL.y&ZShs2);	}
	bool bHS2iNo() const { return bHS2i()&&bHS2o(); }
	bool bHS2iOo() const { return bHS2i()||bHS2o(); }
	U4 oHS2o() {
		iCTRL.y&=(~ZShs2);
		return (oCTRL.y|=ZShs2);
	}
	U4 xHS2o() {
		iCTRL.y|=ZShs2;
		return (oCTRL.y&=(~ZShs2));
	}

	bool bHS3i() const  { return !!(iCTRL.y&ZShs3);	}
	bool bHS3o() const  { return !!(oCTRL.y&ZShs3);	}
	bool bHS3iNo() const { return bHS3i()&&bHS3o(); }
	bool bHS3iOo() const { return bHS3i()||bHS3o(); }
	U4 oHS3o() { iCTRL.y&=(~ZShs3); return (oCTRL.y|=ZShs3); }
	U4 xHS3o() { iCTRL.y|=ZShs3; return (oCTRL.y&=(~ZShs3)); }

	bool jdPRGstp( U4 mSEC, gpcGT* pGT, gpcROBnD *pROBnD );
	/// HD ------------------------------
	I4x4	cageXYZhd0( I4x4& N, I4x4* pHD, I4 lim, U4 id, int rR  );
	I4x4 	cageXYZhd( I4x4& N, I4x4* pHD, I4 lim, I4x4* pBOX, U4 nBOX, I4x4* pBALL, U4 nBALL, int rR );
	I4x4 	cageBALLnts( I4x4& N, I4x4 T, I4x4 S, I4x4* pCAGE, U4 n, int rR );
	I4x4 	cageBOXnts( I4x4& N, I4x4 T, I4x4 S, I4x4* pCAGE, U4 n, int rR );
	/// .... ...  ..   .
	I4x4 judoCAGE( I4x4& jXYZ, I4x4& jABC, I4x4& jxyz, I4x4& jN, U4 i, F4x4& iMX );
	I4x4 judoCAGEin( 	I4x4& jXYZ, I4x4& jABC, I4x4& jxyz,
						I4x4& iiXYZ, I4x4& iiABC, I4x4& iixyz,
						I4x4& jN, U4 i, F4x4& iMX );

	gpcDrc& judo2( gpcROB& iR, U4 mSEC, U4 iD0, gpcGT* pGT );
	gpcDrc& judo( gpcROB& iR, U4 mSEC, U4 iD0, gpcGT* pGT, gpcROBnD *pROBnD );
	gpcDrc& judo2( gpcROB& iR, U4 mSEC, U4 iD0, gpcGT* pGT, gpcROBnD *pROBnD );
	gpcDrc& judo_OHNEnew( gpcROB& iR, U4 mSEC, U4 iD0 );
	gpcDrc& JUDO( gpcROB& iR, U4 mSEC );
};
#define gpdROBnDnull ((gpcROBnD*)NULL)
#define gpdID pROBnD->aROBpID
#define gpdRD pROBnD->lzyRD
#define gpdRM pROBnD->pRM
#define gpdROBrd pROBnD->robROAD
#define gpdSRF pROBnD->pROBsrf
#define gpdDIF pROBnD->pROBdif

class gpcROBnD {
	public:
		U4x4	pc,
				ioSW;
		gpcROB 	aROBio[6];
		gpcDrc	aDrc[2];

		/// ----------------------------
		///			rPIC
		/// ----------------------------
		I4		aROBpID[2];
		SDL_Surface	*pROBsrf, *pROBdif;
		U4			*pRM;
		gpcLZY		lzyRD, robROAD;
		/// ----------------------------

		gpcROBnD() { gpmCLR; };
		~gpcROBnD() {
			gpmSDL_FreeSRF(pROBsrf);
			gpmSDL_FreeSRF(pROBdif);
			gpmDELary(pRM);
		};

		gpcROBnD* format() {
			if( !this )
				return NULL;
			// OUT -------------
			for( U1 iD = 0, e = gpmN(aDrc); iD < e; iD++ )
				aROBio[iD*2] = aDrc[iD].format( iD ? gpeZS_JOHN : gpeZS_BILL );
			// INP ------------
			aROBio[1].null();
			aROBio[3].null();
			// REF -------------
			aROBio[4].null();
			aROBio[5].null();
			return this;
		}
		gpcROBnD* reset( U1 iBILL ) {
			if( !this )
				return NULL;
			gpmCLR;
			if(iBILL)
				iBILL <<= 1;

			ioSW.y = iBILL+1;	// BILL PULL
			ioSW.z = iBILL+4;	// BILL CMP
			ioSW.w = iBILL;
			return format();
		}
		U4 stpPULL() {
			ioSW.y += 2;
			return ioSW.y;
		}
		U4 stpPUSH( bool bSW ) {
			/// bSW ? true BnJ : false only Bill
			ioSW.w -= bSW ? 2 : 4;
			ioSW.z = ioSW.w+4;
			ioSW.y = ioSW.w+1;
			return !!(ioSW.w&2);
		}
		bool bWAIT() {
			if( !this )
				return true;	// akkor is várni kell ha !this azaz nincs semi
			// ha ioSW.w >= ioSW.y igaz, még nem jött válasz
			pc.x++;
			return ioSW.w >= ioSW.y;
		}
		bool bNEXT() {
			// ha ioSW.y >= ioSW.z igaz már vége a körnek
			return ioSW.y >= ioSW.z;
		}
		bool bPULL() {
			if( bWAIT() )
				return false;
			// ioSW.w < ioSW.y
			if( bNEXT() )
				return false;
			// 0 Write
			if( !iWR() )
				return false;
			// 1 Read pullingolni lehet
			ioSW.w = ioSW.y+1;
			return true;
		}

		U1 iDrc( bool bPULL = false );
		U1 iWR() {
			// 0 Write
			// 1 Read
			return ioSW.y&1;
		}

		U2* pROBioU2() {
			if( !this )
				return NULL;
			return (U2*)&(aROBio[ioSW.y&3]);
		}
		gpcROB& ioROB() {
			return aROBio[ioSW.y&3];
		}
		gpcLZY* pull( gpcLZY* pOUT, U4x4* pZSrw );

		gpcDrc* pDrc() {
			if( !this )
				return NULL;
			return &aDrc[ioSW.y&3];
		}
		U2* pU2cr() {
			if( !this )
				return NULL;

			return (U2*)pDrc();
		}

		U2* pU2out() {
			if( !this )
				return NULL;
			return (U2*)&(aROBio[(ioSW.y&2)+0]);
		}
		U2* pU2inp() {
			if( !this )
				return NULL;
			return (U2*)&(aROBio[(ioSW.y&2)+1]);
		}

};

#define gpdZSnDnull ((gpcZSnD*)NULL)
class gpcZSnD {
	public:
		U4x4	pc,
				ioSW;
		gpcZS 	aZSio[6];
		gpcDrc	aDrc[2];
		gpcZSnD() { gpmCLR; };

		gpcZSnD* format()
		{
			if( !this )
				return NULL;
			// OUT -------------
			for( U1 iD = 0, e = gpmN(aDrc); iD < e; iD++ )
				aZSio[iD*2] = aDrc[iD].format( iD ? gpeZS_JOHN : gpeZS_BILL );
			// INP ------------
			aZSio[1].null();
			aZSio[3].null();
			// REF -------------
			aZSio[4].null();
			aZSio[5].null();
			return this;
		}
		gpcZSnD* reset( U1 iBILL )
		{
			if( !this )
				return NULL;
			gpmCLR;
			if(iBILL)
				iBILL <<= 1;

			ioSW.y = iBILL+1;	// BILL PULL
			ioSW.z = iBILL+4;	// BILL CMP
			ioSW.w = iBILL;
			return format();
		}
		U4 stpPULL()
		{
			ioSW.y += 2;
			return ioSW.y;
		}
		U4 stpPUSH( bool bSW )
		{
			/// bSW ? true BnJ : false only Bill
			ioSW.w -= bSW ? 2 : 4;
			ioSW.z = ioSW.w+4;
			ioSW.y = ioSW.w+1;
			return !!(ioSW.w&2);
		}
		bool bWAIT()
		{
			if( !this )
				return true;	// akkor is várni kell ha !this azaz nincs semi
			// ha ioSW.w >= ioSW.y igaz, még nem jött válasz
			pc.x++;
			return ioSW.w >= ioSW.y;
		}
		bool bNEXT()
		{
			// ha ioSW.y >= ioSW.z igaz már vége a körnek
			return ioSW.y >= ioSW.z;
		}
		bool bPULL()
		{
			if( bWAIT() )
				return false;
			// ioSW.w < ioSW.y
			if( bNEXT() )
				return false;
			// 0 Write
			if( !iWR() )
				return false;
			// 1 Read pullingolni lehet
			ioSW.w = ioSW.y+1;
			return true;
		}

		U1 iDrc( bool bPULL = false );
		U1 iWR()
		{
			// 0 Write
			// 1 Read
			return ioSW.y&1;
		}

		U2* pZSioU2()
		{
			if( !this )
				return NULL;
			return (U2*)&(aZSio[ioSW.y&3]);
		}
		gpcZS& ioZS()
		{
			return aZSio[ioSW.y&3];
		}
		gpcLZY* pulling( gpcLZY* pOUT, U4x4* pZSrw );

		gpcDrc* pDrc()
		{
			if( !this )
				return NULL;
			return &aDrc[ioSW.y&3];
		}
		U2* pU2cr()
		{
			if( !this )
				return NULL;

			return (U2*)pDrc();
		}

		U2* pU2out()
		{
			if( !this )
				return NULL;
			return (U2*)&(aZSio[(ioSW.y&2)+0]);
		}
		U2* pU2inp()
		{
			if( !this )
				return NULL;
			return (U2*)&(aZSio[(ioSW.y&2)+1]);
		}

};


class gpcSLMP {
public:
	union{
		struct {
			U1 aU1[21];
		};
#ifdef _WIN64
		//PACK(
		__pragma(pack(push, 1))
			struct {
				U2 	SER;		// 2
				U1	nNET,		// 3
					sRNG;		// 4
				U2 	nUio;		// 6
				U1 	MlDrp;		// 7
				U2 	nLEN,		// 9

					tMON,		// 11	//  2
					com,		// 13	//  4
					subC;		// 15	//  6
				U1 	iDev[3],	// 18	//  9
					d0;			// 19	// 10
				U2 	nDev;		// 21	// 12
								// 21
			}
		__pragma(pack(pop)); //);
#else
		struct {
			U2 	SER			__attribute__((packed));	// 2
			U1	nNET 		__attribute__((packed)),	// 3
				sRNG 		__attribute__((packed));	// 4
			U2 	nUio		__attribute__((packed));	// 6
			U1 	MlDrp		__attribute__((packed));	// 7
			U2 	nLEN 		__attribute__((packed)),	// 9

				tMON		__attribute__((packed)),	// 11	//  2
				com			__attribute__((packed)),	// 13	//  4
				subC		__attribute__((packed));	// 15	//  6
			U1 	iDev[3]		__attribute__((packed)),	// 18	//  9
				d0			__attribute__((packed));	// 19	// 10
			U2 	nDev		__attribute__((packed));	// 21	// 12
														// 21
		}__attribute__((packed));
#endif
	};
	gpcSLMP(){};
	gpcSLMP( 	U2 _SER,
				U1 _nNET, U1 _sRNG,
				U2 _nUio,
				U1 _MlDrp,
				U2 _nLEN,

				U2 _tMON,
				U2 _com,
				U2 _subC,
				U4 _iDev,
				U1 _d0,
				U2 _nDev
	)
	{
		SER 		= _SER;
		nNET 		= _nNET;
		sRNG		= _sRNG;	// station range
		nUio		= _nUio;
		MlDrp		= _MlDrp;
		nLEN		= _nLEN;	// RespDatLen
		tMON		= _tMON;	// EndCD
		com 		= _com;
		subC		= _subC;
		*(U4*)iDev	= _iDev;
		d0			= _d0;
		nDev		= _nDev;
	}
	size_t iEndCD()
	{
		return gpmOFF( gpcSLMP, tMON );
	}
	size_t nEND()
	{
		if( !this )
			return ~0;
		return nLEN*sizeof(U2) + iEndCD();
	}
	U1* pCPY()
	{
		return ((U1*)this)+iEndCD();
	}
	U4 setIDev( U4 i )
	{
		*(U4*)iDev = (i&0xffffff)|(((U4)d0)<<24);
		return (i&0xffffff);
	}

};


class gpcGT_DWNL {
public:
	I8x2 an;
	U8	nIDX,
		*p_dix, n_dix, dw_il, dw_i, dw_j, dw_je;
	U1 s_file_name[gpdMAX_PATH];
	gpcGT_DWNL( U8 _dt )
	{
		gpmCLR;
		nIDX = _dt;
	}
	gpcGT_DWNL( I8x2& anDWN )
	{
		gpmCLR;
		anDWN.an2str( s_file_name );
		an = anDWN;
	}
	gpcGT_DWNL( char* p_file );
	gpcLZY* join( gpcLZY* pOUT, gpcGT& mom, gpcLZY* pEXE = NULL );
};

class gpcHUD {
public:
	gpeNET4		id;
	U4			n;
	U1			*p_alloc;
	gpcHUD(void)
	{
		gpmCLR;
	}
	~gpcHUD()
	{
		gpmDELary(p_alloc);
	}
	gpcHUD& null(void)
	{
		gpmCLR;
		return *this;
	}
	gpcHUD& reset(void)
	{
		gpmDELary(p_alloc);
		gpmCLR;
		return *this;
	}
	gpcHUD* put( const void* p_void, I8 n_byte);

};

class gpcFD {
public:
	int		nFD, maxSCK;
	fd_set	fdS;

	bool isFD( SOCKET s )
	{
		bool b = FD_ISSET( s, &fdS );
		return b;
	}
	U4 setFD( SOCKET s )
	{
		FD_SET( s, &fdS );
		if( FD_ISSET( s, &fdS ) )
		{
			nFD++;
			maxSCK = gpmMAX( maxSCK, s );
		}
		return nFD;
	}
	void zero( U4 n = 1 )
	{
		gpcFD* p_Z = this;
		for( U4 i = 0; i < n; i++ )
		{
			FD_ZERO( &this[i].fdS );
			this[i].nFD = this[i].maxSCK = 0;
		}
	}
};



class gpcGTall {
public:
	gpcGT	**ppGTalloc, *pGT;
	U4		nGTalloc, iGTfr, nGTld;
	U1		sPUB[gpdMAX_PATH];
	gpcGTall() { gpmCLR; };
	void	clr( );
	gpcGT*	GTacc( SOCKET sock, I4 port );
	gpcGT* iGT( U4 i )
	{
		if( this ? i >= nGTld : true )
			return NULL;

		return ppGTalloc ? ppGTalloc[i] : NULL;
	}
	gpcGT* GT( gpeALF alf, I4 port );
	gpcGT* GT( gpeALF alf, U1* pIPA, U4 nIPA );
	gpcGT* GT( SOCKET sock );
};

class gpcGT {
	public:
		I8x2		TnID, gt_ip;
		I4			port, iCNT, nOSin;

		SOCKET		socket, sockAT, sockCNCT;
		SOCKADDR	sockAddr;
		addrinfo	*p_ainf;
		SOCKADDR_IN	*p_ai, addr_in;
		gpcGTall	GTacc;
        gpcGT       *pGTm;
		gpcLZY		*pPUBgt,
					*pINP, *pMISi,
					*pEVENT,
					*pSYNgt;

	private:
		gpcLZY		*pOUT, *pMISo;

	public:
		gpcGT_DWNL	*pDWN;
		gpcHUD		*pHUD, isEVNT;
		#ifdef _WIN64
		MIB_IPADDRTABLE* p_IPTable;
		#endif
		hostent			*p_host_ips;
		struct ifaddrs 	*pIFadr,
						*pIFa;

		gpcFD	aGTfd[gpeFDn];

		U4		nFDs, nFDr, nFDe,

				nSYN, nSYNsum, nSYNdo,
				msSYNgt, msSYNwin, msGTdie;
		U4x4 	mSEC;
		U8		bSW;	// 1 same file | 2 loop

		char	s_ip[0x400],
				s_telnet[80*25+4];

		U1		sGTent[8],
				sHOST[0x100],
				sUSER[0x100],
				sFILE[0x100],
				*pHOST, *pUSER, *pFILE;

		bool bTEL() {
			return !(mSEC.x&mSEC.y);
		}
		bool bLOOP() {
			return bSW&2;
		}
		bool bGTdie() {
            if( !this )
                return true;
            if( socket == INVALID_SOCKET )
            {
                if( msGTdie&1 )
                    return true;

                msGTdie = 1;
                return true;
            }

            return msGTdie&1;
        }
        bool GTprmpt( bool bENT = true  ) {
			if( this ? sGTent[0] : true )
				return false;
			if( pOUT ? pOUT->qCLOSE() : false )
				return false;
			U8 s = -1;
			pOUT = pOUT->lzyFRMT( s, "%s%x>", bENT?"\r\n":"    \r",iCNT );
			return true;
		}

		gpcLZY* GTback() {
			if( !this )
				return NULL;
			// ott van felette
			GTprmpt();
			if( !pINP )
				return pOUT;
			U8 s;

			pOUT = pOUT->lzyFRMT(
									s = -1,
									"%s",
									pINP->p_alloc ? (char*)pINP->p_alloc : ""
								);
			return pOUT;
		}
		~gpcGT() {
			GTclose();
		};
		gpcGT( I8x2 id, I4 prt, SOCKET sock = INVALID_SOCKET ) {
			gpmCLR;
			TnID = id;
			port = prt;
			socket = sock;
			sockAT = sockCNCT = INVALID_SOCKET;
		}
		gpcGT* GTclr() {
			if( this )
			{
				GTacc.clr();
				gpmDEL( pPUBgt );
				gpmDEL( pOUT ); gpmDEL( pMISo );
				gpmDEL( pINP ); gpmDEL( pMISi );
				gpmDEL( pDWN );
				gpmDEL( pHUD );
				gpmDEL( pEVENT );
				gpmDEL( pSYNgt );
				pUSER = sUSER;
				pHOST = sHOST;
				pFILE = sFILE;
				*sUSER = 0;
				*sHOST = 0;
				*sFILE = 0;
				nSYN = 0;
			}
			return this;
		}
		gpcGT* GTclose() {
			GTclr();
			gpfSOC_CLOSE( socket );
			gpfSOC_CLOSE( sockCNCT );
			return this;
		}
		gpcWIRE*	GTwire( gpcWIN* pWIN, int msRUN );
		gpcGSM*     GTgsm( gpcWIN* pWIN );
		I8		GTcnct( gpcWIN* pWIN );
		I8		GTlst( gpcWIN* pWIN, gpcGTall& );
		int		GTerr( char* p_err, char** pp_err );
		U1		GTopt( char* p_error, char** pp_error, int no_daley, U4 n_buff );
		char*	GTrcv( char* p_err, char* s_buff, U4 n_buff );
		char*	GTsnd( char* p_err, char* s_buff, U4 n_buff );

        gpcLZY*	GTwireOS( gpcLZY* pANS, U1* pSTR, gpcMASS* pMASS, SOCKET sockUSR, U4 mSEC );
		gpcLZY*	GTgsmOS( gpcLZY* pANS, U1* pSTR, gpcMASS* pMASS, SOCKET sockUSR, U4 mSEC );


		gpcLZY* GTdrcOSrob( gpcLZY* pANS, U1* pSTR, gpcMASS* pMASS, SOCKET sockUSR, U4 mSEC );
		void	GTslmpDrcRob( gpcGT& mom, gpcWIN* pWIN, gpcGTall* pALL );

		gpcLZY* GTdrcOSzs( gpcLZY* pANS, U1* pSTR, gpcMASS& mass, SOCKET sockUSR, U4 mSEC );
		void 	GTslmpDrcZS( gpcGT& mom, gpcWIN* pWIN, gpcGTall* pALL );

		gpcLZY* gpcGTslmpSTAT( gpcLZY* pANS, U2* pU2 );
		gpcLZY* GTslmpOS( gpcLZY* pANS, U1* pSTR, gpcMASS& mass, SOCKET sockUSR );

		//void 	GTslmp( gpcGT& mom, gpcWIN* pWIN, gpcGTall* pALL );
		void	GTrealMITSUB( gpcGT& mom, gpcWIN* pWIN, gpcGTall* pALL );

		gpcLZY* GTslmpOSref( gpcLZY* pANS, U1* pSTR, gpcMASS& mass, SOCKET sockUSR );
		void 	GTslmpREF( gpcGT& mom, gpcWIN* pWIN, gpcGTall* pALL );
		void 	GTslmpBINref( gpcGT& mom, gpcWIN* pWIN = NULL, gpcGTall* pALL = NULL );

		void	GTos( gpcGT& mom, gpcWIN* pWIN = NULL, gpcGTall* pALL = NULL );
		gpcLZY*	GTos_GATELIST( gpcLZY *p_out, const char* p_enter, const char* pTAB );

		size_t GTout( gpcWIN* pWIN );

		gpcLZY*	GThtmlOS( gpcLZY* pOUT, gpcGT& mom, gpcWIN* pWIN, void* pGET, void* pHOST );
		//gpcLZY*	GTwireOS( gpcLZY* pOUT, gpcGT& mom, gpcWIN* pWIN, void* pGET, void* pHOST );
        gpcLZY* pGTout() {
            if( !this )
                return NULL;
            if( pOUT )
                return pOUT;
            return (pOUT = new gpcLZY());
        }
	protected:

	private:
};



#endif // GPCGT_H
