#ifndef GPCGT_H
#define GPCGT_H


#include "gpcpic.h"

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

#define ZShs1i 0x1
#define ZShs1o 0x2
#define ZShs2i 0x4
#define ZShs2o 0x8



/*#define gpdDrcSTART( p ) (p.CTRL.y&0x1)
#define gpdDrcDONE( p ) (p.CTRL.y&0x2)*/
#define gpdSLMP GTslmpDrc // GTslmp
#define gpdSLMPos GTdrcOS	//GTslmpOS
#define gpdZSnW 	gpmOFFOFF( gpcZS, io128.y, oMxyzEspd )
#define gpdZSnWu2	(gpdZSnW/sizeof(U2))
#define gpdZSnR		sizeof(gpcZS) //gpdZSnW+sizeof(I4x4)
#define gpdZSnRu2	(gpdZSnR/sizeof(U2))
class gpcDrc;

class gpcZS {
public:
	U4x4	DEDI,		// b0
			io128;		// b128
	I4		aPOS[3],	// b256
			aABC[3],	// b352
			apos[3],	// b448
			aabc[3],	// b544
            aJ16[6],	// b640
            aj16[6];	// b832
			// b1024
	I4x4	oMxyzEspd,	// b1024
			oMabcNsnt,	// b1152
			oEkey;		// b1280
			//b1308
	gpcZS& null() { gpmCLR; return *this; }
	gpcZS(){
		null();
	};
	gpcZS& operator = ( const gpcDrc& D );
	gpcZS& operator &= ( const gpcDrc& D );
	gpcZS( const gpcDrc& D );

};

class gpcDrc {
public:
	I4x4 	NMnDIF,
			trgXYZ, oXYZ, iXYZ,
			trgABC, oABC, iABC,
			oxyz, ixyz,
			oabc, iabc,
			aoAX1to6[2], aiAX1to6[2],
			aoax1to6[2], aiax1to6[2];
	U4x4	oCTRL, iCTRL;

	//gpcDrc& outDrc( gpcDrc& pev, gpcDrc& inp );
	I4x4 cage( I4x4* pCAGE, U4 n );
	gpcDrc& operator &= ( gpcDrc& in );
	gpcDrc& operator = ( gpcZS& zs );

	/*gpcDrc& i( const gpcDrc& b )
	{
		iXYZ.xyz_( b.iXYZ );
		iABC.xyz_( b.iABC );
		ixyz.xyz_( b.ixyz );
		iabc.xyz_( b.iabc );
		aiAX1to6[0].xyz_( b.aiAX1to6[0] );
		aiAX1to6[1].xyz_( b.aiAX1to6[1] );
		aiax1to6[0].xyz_( b.aiax1to6[0] );
		aiax1to6[1].xyz_( b.aiax1to6[1] );

		return *this;
	}*/

	bool operator == ( const gpcDrc& b ) const
	{
		if( sizeof(b) == gpmMcmp( &b, this, sizeof(b) ) )
			return true;

		return false;
	}
	bool operator != ( const gpcDrc& b ) const
	{
		return !(*this == b);
	}

	gpcDrc& format( U4 nm = 0 ) {
		gpmCLR;
		if( !nm )
			return *this;

		NMnDIF.x = nm;
		iXYZ = oXYZ = trgXYZ = I4x4( 400,  0, 300+400, gpeZS_POS0)&I4x4(100,100,100, 1);
		iABC = oABC = trgABC = I4x4( 180,  0, 90,gpeZS_DIR0)&I4x4(100,100,100, 1);
		iXYZ.w = gpeZS_iPOS;
		iABC.w = gpeZS_iDIR;
		oXYZ.w = gpeZS_oPOS;
		oABC.w = gpeZS_oDIR;
		// OFFSET - eltolás
		oxyz.w = gpeZS_pos0;
		ixyz.w = gpeZS_ipos;
		oabc.w = gpeZS_dir0;
		iabc.w = gpeZS_idir;

		// TENGELY
		aoAX1to6[0].w = gpeZS_oA13;
		aoAX1to6[1].w = gpeZS_oA46;
		aiAX1to6[0].w = gpeZS_iA13;
		aiAX1to6[1].w = gpeZS_iA46;

		aoax1to6[0].w = gpeZS_oa13;
		aoax1to6[1].w = gpeZS_oa46;
		aiax1to6[0].w = gpeZS_ia13;
		aiax1to6[1].w = gpeZS_ia46;

		oCTRL.x = gpeZS_oCTR;
		iCTRL.x = gpeZS_iCTR;

		return *this;
	}
	gpcDrc() { format(); };
	gpcDrc( const gpcZS& zs, U4 nm = 0 )
	{
		if( nm != NMnDIF.au4x2[0].x )
			format( nm );

		*this = zs;
	}

	U2 hs12()
	{
		if(!this )
			return 0;

		return (bHS1i()<<0xc)|(bHS1o()<<0x8)|(bHS2i()<<0x4)|(bHS2o());
	}
	gpcLZY* ANSstat( gpcLZY* pANS ) {
		U1 	sCOM[] = "ABCD";
		U4 &comA = *(U4*)sCOM;

		comA = NMnDIF.x;
		U8 s;
		pANS = pANS->lzyFRMT(
								s = -1,

										"\r\n//\t%s\tHS12:%0.4X\tiX:%4.2fmm iY:%4.2fmm iZ:%4.2fmm "
										"iA:%4.2fdg iB:%4.2fdg iC:%4.2fdg "
										"\r\n//\t\tOFF\t\t\tix:%4.2fmm iy:%4.2fmm iz:%4.2fmm "
										"ia:%4.2fdg ib:%4.2fdg ic:%4.2fdg "

										"\r\n//\t\tOUT:\t\toX:%4.2fmm oY:%4.2fmm oZ:%4.2fmm "
										"oA:%4.2fdg oB:%4.2fdg oC:%4.2fdg "
										"\r\n//\t\tOFF\t\t\tox:%4.2fmm oy:%4.2fmm oz:%4.2fmm "
										"oa:%4.2fdg ob:%4.2fdg oc:%4.2fdg "

										"\r\n//\t\tOUT:\t\ttX:%4.2fmm tY:%4.2fmm tZ:%4.2fmm "
										"tA:%4.2fdg tB:%4.2fdg tC:%4.2fdg "
										"\r\n//\t\tOFF\t\t\tox:%4.2fmm oy:%4.2fmm oz:%4.2fmm "
										"oa:%4.2fdg ob:%4.2fdg oc:%4.2fdg "
										,
								*sCOM ? (char*)sCOM : "?",

								hs12(),
// INP
								double(iXYZ.x)/100.0,
								double(iXYZ.y)/100.0,
								double(iXYZ.z)/100.0,
								double(iABC.x)/100.0,
								double(iABC.y)/100.0,
								double(iABC.z)/100.0,

								double(ixyz.x)/100.0,
								double(ixyz.y)/100.0,
								double(ixyz.z)/100.0,
								double(iabc.x)/100.0,
								double(iabc.y)/100.0,
								double(iabc.z)/100.0,
// OUT
								double(oXYZ.x)/100.0,
								double(oXYZ.y)/100.0,
								double(oXYZ.z)/100.0,
								double(oABC.x)/100.0,
								double(oABC.y)/100.0,
								double(oABC.z)/100.0,

								double(oxyz.x)/100.0,
								double(oxyz.y)/100.0,
								double(oxyz.z)/100.0,
								double(oabc.x)/100.0,
								double(oabc.y)/100.0,
								double(oabc.z)/100.0,
// TRG
								double(trgXYZ.x)/100.0,
								double(trgXYZ.y)/100.0,
								double(trgXYZ.z)/100.0,
								double(trgABC.x)/100.0,
								double(trgABC.y)/100.0,
								double(trgABC.z)/100.0,

								double(oxyz.x)/100.0,
								double(oxyz.y)/100.0,
								double(oxyz.z)/100.0,
								double(oabc.x)/100.0,
								double(oabc.y)/100.0,
								double(oabc.z)/100.0

							);
		return pANS;
	}


	bool bHS1i() const { return !!(iCTRL.y&ZShs1);	}
	bool bHS1o() const { return !!(oCTRL.y&ZShs1);	}
	bool bHS1iNo() const { return bHS1i()&&bHS1o(); }
	bool bHS1iOo() const { return bHS1i()||bHS1o(); }
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

	gpcDrc& judo( gpcZS& inp );
};
#define gpdZSnDnull ((gpcZSnD*)NULL)
class gpcZSnD
{
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

		U1 iDrc( bool bPULL = false )
		{
			if( bPULL )
				ioSW.y |= 1;
			return (ioSW.y>>1)&1;
		}
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


		gpcLZY* pulling( gpcLZY* pOUT, U4x4* pZSrw )
		{
			U8 s = -1;
			U4	i = this ? iDrc( true ):0,
				n = pZSrw[i].w;
			i = pZSrw[i].z;
			if( this )
				pc.x = 0;
			return pOUT->lzyFRMT( s, gpdSLMP_recv_LN4SL6N4, 24, i, n );
		}

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

} __attribute__((packed));
class gpcWIN;
class gpcGT;

class gpcGT_DWNL
{
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
	/*{
		if( p_void ? !n_byte : true )
			return this;
		if(!this)
		{
			gpcHUD* p_this = new gpcHUD;
			if(!p_this)
				return NULL;
			return p_this->put(p_void, n_byte);
		}
		if( p_alloc )
		{
			U1	*p_kill = p_alloc;
			p_alloc = new U1[n+n_byte];
			memcpy( p_alloc, p_kill, n );
			memcpy( p_alloc+n, p_void, n_byte );
			n += n_byte;
			gpmDELary(p_kill);
			return this;
		}
		n = n_byte;
		p_alloc = (U1*)memcpy( new U1[n+(0x10-(n%0x10))], p_void, n );
		return this;
	}*/

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
			maxSCK = max( maxSCK, s );
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

class gpcGT
{
	public:
		I8x2		TnID, gt_ip;
		I4			port, iCNT, nOSin;

		SOCKET		socket, sockAT, sockCNCT;
		SOCKADDR	sockAddr;
		addrinfo	*p_ainf;
		SOCKADDR_IN	*p_ai, addr_in;
		gpcGTall	GTacc;

		gpcLZY		*pPUB,
					*pINP, *pMISi,
					*pEVENT,
					*pSYNgt;

	private:
		gpcLZY		*pOUT, *pMISo;

	public:
		gpcGT_DWNL	*pDWN;
		gpcHUD		*pHUD, isEVNT;

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

		U1		sGTent[4],
				sHOST[0x100],
				sUSER[0x100],
				sFILE[0x100],
				*pHOST, *pUSER, *pFILE;

		bool bTEL()
		{
			return !(mSEC.x&mSEC.y);
		}
		bool bLOOP()
		{
			return bSW&2;
		}
		bool bGTdie()
        {
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

			U8 s = -1;
			pOUT = pOUT->lzyFRMT( s, "%s%x>", bENT?"\r\n":"    \r",iCNT );
			return true;
		}
		gpcLZY* GTback()
		{
			if( !this )
				return NULL;
			GTprmpt();
			//pOUT = pOUT->lzyFRMT( s = -1, "  \r0x%x>", iCNT );
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
		~gpcGT() { GTclose(); };
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
				gpmDEL( pPUB );
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
		I8		GTcnct( gpcWIN& win ); //, gpcGTall& acpt );
		I8		GTlst( gpcWIN& win, gpcGTall& );
		int		GTerr( char* p_err, char** pp_err );
		U1		GTopt( char* p_error, char** pp_error, int no_daley, U4 n_buff );
		char*	GTrcv( char* p_err, char* s_buff, U4 n_buff );
		char*	GTsnd( char* p_err, char* s_buff, U4 n_buff );



		//gpcLZY*	gpcGTdrcSTAT( gpcLZY* pANS, gpcDrc& zs );
		gpcLZY* GTdrcOS( gpcLZY* pANS, U1* pSTR, gpcMASS& mass, SOCKET sockUSR );
		void 	GTslmpDrc( gpcGT& mom, gpcWIN* pWIN, gpcGTall* pALL );

		gpcLZY* gpcGTslmpSTAT( gpcLZY* pANS, U2* pU2 );
		gpcLZY* GTslmpOS( gpcLZY* pANS, U1* pSTR, gpcMASS& mass, SOCKET sockUSR );

		void 	GTslmp( gpcGT& mom, gpcWIN* pWIN, gpcGTall* pALL );
		void	GTrealMITSUB( gpcGT& mom, gpcWIN* pWIN, gpcGTall* pALL );

		gpcLZY* GTslmpOSref( gpcLZY* pANS, U1* pSTR, gpcMASS& mass, SOCKET sockUSR );
		void 	GTslmpREF( gpcGT& mom, gpcWIN* pWIN, gpcGTall* pALL );
		void 	GTslmpBINref( gpcGT& mom, gpcWIN* pWIN = NULL, gpcGTall* pALL = NULL );

		void	GTos( gpcGT& mom, gpcWIN* pWIN = NULL, gpcGTall* pALL = NULL );
		gpcLZY*	GTos_GATELIST( gpcLZY *p_out, const char* p_enter, const char* pTAB );

		U8 GTout( gpcWIN* pWIN );

	protected:

	private:
};



#endif // GPCGT_H
