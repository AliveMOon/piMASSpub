#ifndef GPCGT_H
#define GPCGT_H


#include "gpcpic.h"
//#define gpdRECVn (0x30000/12)
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
	gpcLAZY* join( gpcLAZY* pOUT, gpcGT& mom, gpcLAZY* pEXE = NULL );
};

class gpcHUD
{
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

class gpcFD
{
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



class gpcGTall
{
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
		I4			port, iCNT;

		SOCKET		socket, sockAT, sockCNCT;
		SOCKADDR	sockAddr;
		addrinfo	*p_ainf;
		SOCKADDR_IN	*p_ai, addr_in;
		gpcGTall	GTacc;

		gpcLAZY		*pPUB,
					*pOUT,
					*pINP,
					*pEVENT,
					*pSYNgt;
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

		U1		aGTcrs[2],
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
			/*if( !mSEC.x )
				return false;

			return mSEC.x == mSEC.y;*/
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

		~gpcGT() { GTclose(); };
		gpcGT( I8x2 id, I4 prt, SOCKET sock = INVALID_SOCKET )
		{
			gpmCLR;
			TnID = id;
			port = prt;
			socket = sock;
			sockAT = sockCNCT = INVALID_SOCKET;
		}
		gpcGT* GTclr()
		{
			if( this )
			{
				GTacc.clr();
				gpmDEL( pPUB );
				gpmDEL( pOUT );
				gpmDEL( pINP );
				gpmDEL( pDWN );
				gpmDEL( pHUD );
				gpmDEL( pEVENT );
				gpmDEL( pSYNgt );
			}
			return this;
		}
		gpcGT* GTclose()
		{
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

		U4 GTprmpt( void )
		{
			if(!this)
				return 0;
			U8 s;
			if( !aGTcrs[0] )
				pOUT = pOUT->lzyFRMT( s = -1, "\r\n%x>", iCNT );

			return iCNT;
		}
		void		GTos( gpcGT& mom, gpcWIN* pWIN = NULL, gpcGTall* pALL = NULL );
		gpcLAZY*	GTos_GATELIST( gpcLAZY *p_out, const char* p_enter, const char* pTAB );


	protected:

	private:
};



#endif // GPCGT_H
