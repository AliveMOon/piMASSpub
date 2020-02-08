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
#define ZShs1 0x1
#define ZShs2 0x2

#define ZShs1i 0x1
#define ZShs1o 0x2
#define ZShs2i 0x4
#define ZShs2o 0x8



/*#define gpdZSnDrcSTART( p ) (p.CTRL.y&0x1)
#define gpdZSnDrcDONE( p ) (p.CTRL.y&0x2)*/
#define gpdSLMP GTslmp_ZSnDrc // GTslmp
#define gpdSLMPos GTzsndOS	//GTslmpOS
class gpcZSnDrc
{
public:
	I4x4 	NMnDIF,
			oXYZ, iXYZ,
			oABC, iABC,
			oxyz, ixyz,
			oabc, iabc,
			aoAX1to6[2], aiAX1to6[2],
			aoax1to6[2], aiax1to6[2];
	U4x4	CTRL;
	gpcZSnDrc& out( gpcZSnDrc& pev, gpcZSnDrc& inp );
	gpcZSnDrc& operator &= ( gpcZSnDrc& in );

	gpcZSnDrc& i( gpcZSnDrc& ZS )
	{
		iXYZ._yzw( ZS.iXYZ );
		iABC._yzw( ZS.iABC );
		ixyz._yzw( ZS.ixyz );
		iabc._yzw( ZS.iabc );
		aiAX1to6[0]._yzw( ZS.aiAX1to6[0] );
		aiAX1to6[1]._yzw( ZS.aiAX1to6[1] );
		aiax1to6[0]._yzw( ZS.aiax1to6[0] );
		aiax1to6[1]._yzw( ZS.aiax1to6[1] );

		return *this;
	}

	bool bHS1()	{ return !!(CTRL.y&ZShs1);	}
	U4 setHS1( gpcZSnDrc& D ) {
		D.CTRL.y&=(~ZShs1);
		return (CTRL.y|=ZShs1);
	}
	U4 rstHS1( gpcZSnDrc& D ) {
		D.CTRL.y|=ZShs1;
		return (CTRL.y&=(~ZShs1));
	}

	bool bHS2()	{ return !!(CTRL.y&ZShs2);	}
	U4 setHS2( gpcZSnDrc& D ) {
		D.CTRL.y&=(~ZShs2);
		return (CTRL.y|=ZShs2);
	}
	U4 rstHS2( gpcZSnDrc& D ) {
		D.CTRL.y|=ZShs2;
		return (CTRL.y&=(~ZShs2));
	}




	bool bHS1i()	{ return !!(CTRL.y&ZShs1i);		}
	U4 setHS1i() 	{ return (CTRL.y|=ZShs1i); 		}
	U4 rsetHS1i()	{ return (CTRL.y&=(~ZShs1i));	}

	bool bHS1o()	{ return !!(CTRL.y&ZShs1o);		}
	U4 setHS1o()	{ return (CTRL.y|=ZShs1o); 		}
	U4 rsetHS1o()	{ return (CTRL.y&=(~ZShs1o));	}



	bool bHS2i() 	{ return !!(CTRL.z&ZShs2i);		}
	U4 setHS2i() 	{ return (CTRL.z|=ZShs2i); 		}
	U4 rsetHS2i() 	{ return (CTRL.z&=(~ZShs2i));	}

	bool bHS2o() 	{ return !!(CTRL.z&ZShs2o);		}
	U4 setHS2o() 	{ return (CTRL.z|=ZShs2o); 		}
	U4 rsetHS2o() 	{ return (CTRL.z&=(~ZShs2o));	}

	bool operator == ( const gpcZSnDrc& b ) const
	{
		if( sizeof(b) == gpmMEMCMP( &b, this, sizeof(b) ) )
			return true;

		return false;
	}
	bool operator != ( const gpcZSnDrc& b ) const
	{
		return !(*this == b);
	}

	gpcZSnDrc* DnZSfrm( U4 nm = 0 ) {
		if( !this )
			return NULL;
		gpmCLR;
		if( !nm )
			return this;

		NMnDIF.x = nm;

		oXYZ.x = gpeZS_POS0;
		iXYZ.x = gpeZS_iPOS;
		oABC.x = gpeZS_DIR0;
		iABC.x = gpeZS_iDIR;

		// TENGELY
		aoAX1to6[0].x = gpeZS_oA13;
		aoAX1to6[1].x = gpeZS_oA46;
		aiAX1to6[0].x = gpeZS_iA13;
		aiAX1to6[1].x = gpeZS_iA46;

		aoax1to6[0].x = gpeZS_oa13;
		aoax1to6[1].x = gpeZS_oa46;
		aiax1to6[0].x = gpeZS_ia13;
		aiax1to6[1].x = gpeZS_ia46;

		CTRL.x = gpeZS_CTRL;
		// OFFSET - eltolás
		oxyz.x = gpeZS_pos0;
		ixyz.x = gpeZS_ipos;

		oabc.x = gpeZS_dir0;
		iabc.x = gpeZS_idir;
		return this;
	}
	gpcZSnDrc() { DnZSfrm(); };

	/*U4 bSTAT( gpcZSnDrc& out )
	{
		if( !this )
			return 0;

		return gpdZSnDrcSTART( out );
	}*/

	gpcLZY* ANSstat( gpcLZY* pANS )
	{
		U1 	sCOM[] = "ABCD";
		U4 &comA = *(U4*)sCOM;

		comA = NMnDIF.x;
		U8 s;
		pANS = pANS->lzyFRMT(
								s = -1,

										"\r\n%s X:%.2fmm Y:%.2fmm Z:%.2fmm "
										"A:%.2fdg B:%.2fdg C:%.2fdg "
										"\r\nOFF x:%.2fmm y:%.2fmm z:%.2fmm "
										"a:%.2fdg b:%.2fdg c:%.2fdg "
										,
								sCOM,
								double(oXYZ.y)/100.0,
								double(oXYZ.z)/100.0,
								double(oXYZ.w)/100.0,

								double(oABC.y)/100.0,
								double(oABC.z)/100.0,
								double(oABC.w)/100.0,

								double(oxyz.y)/100.0,
								double(oxyz.z)/100.0,
								double(oxyz.w)/100.0,

								double(oabc.y)/100.0,
								double(oabc.z)/100.0,
								double(oabc.w)/100.0

							);
		return pANS;
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



		//gpcLZY*	gpcGTzsndSTAT( gpcLZY* pANS, gpcZSnDrc& zs );
		gpcLZY* GTzsndOS( gpcLZY* pANS, U1* pSTR, gpcMASS& mass, SOCKET sockUSR );
		void 	GTslmp_ZSnDrc( gpcGT& mom, gpcWIN* pWIN, gpcGTall* pALL );

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
