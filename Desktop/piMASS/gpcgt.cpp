#include "gpcgt.h"
#include "gpcSRC.h"
#include "gpcwin.h"

U1 dz_a_favpng[]=
{
	0x89,0x50,0x4E,0x47,0x0D,0x0A,0x1A,0x0A, 0x00,0x00,0x00,0x0D,0x49,0x48,0x44,0x52,
	0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x20, 0x04,0x03,0x00,0x00,0x00,0x81,0x54,0x67,
	0xC7,0x00,0x00,0x00,0x30,0x50,0x4C,0x54, 0x45,0x2C,0x28,0x10,0xC3,0x7C,0x13,0x8C,
	0x8C,0x9C,0x00,0x38,0x5C,0x57,0x29,0x1E, 0xC3,0x88,0x59,0xCC,0xB8,0x98,0x28,0x24,
	0x29,0x8C,0x38,0x10,0xE4,0xD0,0x94,0x80, 0x54,0x38,0xAB,0x6F,0x24,0x34,0x24,0x41,
	0xA4,0xB0,0xC0,0xDC,0xB7,0x7B,0xE0,0xDC, 0xC0,0xE7,0xAC,0x56,0x95,0x00,0x00,0x00,
	0x09,0x70,0x48,0x59,0x73,0x00,0x00,0x0B, 0x13,0x00,0x00,0x0B,0x13,0x01,0x00,0x9A,
	0x9C,0x18,0x00,0x00,0x01,0x76,0x49,0x44, 0x41,0x54,0x28,0x15,0x5D,0x90,0xB1,0x4B,
	0xC3,0x40,0x14,0xC6,0x5F,0xE5,0x42,0x91, 0x66,0x48,0x20,0x82,0x93,0x60,0xFF,0x85,
	0x20,0x38,0x46,0xFA,0x42,0x0B,0x09,0x21, 0x42,0x0F,0x2C,0x85,0x4E,0x11,0x84,0xAC,
	0x01,0x07,0x1D,0x74,0x10,0xBA,0xB9,0x64, 0x29,0x42,0xF7,0x6E,0x0A,0xA9,0x54,0xB2,
	0xDC,0xD0,0xF1,0xC0,0xC9,0xB1,0xA0,0xD0, 0xC1,0x50,0xE8,0x54,0x27,0x17,0xEF,0x2E,
	0x29,0xA2,0x1F,0xB7,0xDC,0xEF,0x7D,0xF7, 0xEE,0x7B,0x0F,0xF8,0x3F,0x81,0xBC,0x33,
	0xFD,0x97,0x2A,0x00,0xE1,0x5F,0xC0,0xDC, 0x6F,0x9D,0x83,0x12,0xE7,0xD2,0x41,0x06,
	0x85,0xCE,0x82,0x7D,0xC4,0xDB,0xA5,0x28, 0x08,0x46,0xD7,0xEF,0x9C,0x51,0x25,0xE5,
	0x74,0xF3,0xFC,0x91,0xF3,0xBA,0x7D,0x6C, 0xDB,0x37,0xA2,0xDC,0x78,0x1D,0xAC,0xD7,
	0x0D,0x9D,0xB7,0x16,0x6F,0x8B,0x45,0x57, 0x00,0xD6,0x17,0x06,0x0F,0x78,0xAB,0x79,
	0xD8,0x6C,0x5A,0x02,0x70,0xC8,0x73,0xD2, 0x25,0x50,0xB7,0x8F,0x6C,0x5B,0x7C,0x0F,
	0x9C,0x01,0x68,0xEE,0x2E,0xC2,0x8E,0x70, 0x18,0x12,0x08,0xE9,0xA1,0x56,0xA4,0x0E,
	0xD8,0x32,0xA0,0x02,0x8C,0x4C,0x69,0x91, 0x22,0xA0,0x0C,0xA1,0x42,0x03,0x4E,0x69,
	0x6F,0x14,0xD0,0x54,0x57,0x80,0x01,0x0D, 0x49,0x40,0xE9,0xC7,0xB0,0xE7,0x28,0xA0,
	0x69,0xE6,0x5E,0x08,0x1E,0x3D,0xEB,0xDF, 0xA5,0x0A,0xD4,0x6A,0xF4,0xD4,0x20,0x82,
	0x50,0x5A,0x81,0xD0,0x34,0xC8,0xF9,0x86, 0xBC,0x4C,0x1F,0x1C,0x5D,0x36,0x65,0x86,
	0xE1,0x46,0x88,0x70,0x32,0xB2,0x6A,0x46, 0x09,0x08,0xFA,0xC9,0xB5,0x83,0x39,0x42,
	0x4D,0x01,0x70,0xA3,0xA4,0x48,0x13,0x91, 0xC3,0x28,0x81,0x1B,0xF9,0xC1,0xF0,0xDE,
	0xC7,0x2F,0xC7,0x54,0x4F,0x48,0xE4,0xCF, 0x67,0xF1,0x45,0xCA,0x71,0x03,0xB2,0x29,
	0xF3,0xDA,0x38,0xFF,0x7C,0x9A,0x2D,0xE7, 0x6D,0x4F,0x05,0x83,0xA8,0x31,0xC6,0x3C,
	0x8E,0x33,0x6F,0x4C,0x22,0xE9,0x20,0x56, 0x30,0xE9,0x78,0xE6,0x41,0x27,0x98,0xA0,
	0xA5,0x40,0x76,0xB5,0x6A,0x27,0x79,0x76, 0xE9,0xAD,0x92,0x6C,0xDB,0x83,0x73,0xF4,
	0xF9,0xB6,0x07,0x27,0x22,0x66,0xA9,0xA8, 0x1A,0xDF,0x7B,0xCE,0x63,0x75,0x9C,0x6A,
	0x41,0x64,0xEB,0xD0,0x2B,0x20,0xF6,0x5C, 0x4A,0x4C,0xFA,0x03,0x31,0xF4,0xF3,0xE7,
	0xFA,0x16,0xFD,0x57,0x00,0x00,0x00,0x00, 0x49,0x45,0x4E,0x44,0xAE,0x42,0x60,0x82,
};

//<meta http-equiv="Content-Type" content="text/html; charset=UTF-8"><meta name="viewport" content="width=device-width">
const char *dz_s_http_meta =	"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"><meta name=\"viewport\" content=\"width=device-width\">";
//<style type="text/css">body,html {background:#fff;font-family:"Bitstream Vera Sans","Lucida Grande","Lucida Sans Unicode",Lucidux,Verdana,Lucida,sans-serif;}tr:nth-child(even) {background:#f4f4f4;}th,td {padding:0.1em 0.5em;}th {text-align:left;font-weight:bold;background:#eee;border-bottom:1px solid #aaa;}#list {border:1px solid #aaa;width:100%;}a {color:#a33;}a:hover {color:#e33;}</style>
const char *dz_s_http_style =	"<style type=\"text/css\">body,html {background:#fff;font-family:\"Bitstream Vera Sans\",\"Lucida Grande\",\"Lucida Sans Unicode\",Lucidux,Verdana,Lucida,sans-serif;}"
								"tr:nth-child(even) {background:#f4f4f4;}"
								"th,td {padding:0.1em 0.5em;}"
								"th {text-align:left;font-weight:bold;background:#eee;border-bottom:1px solid #aaa;}"
								"#list {border:1px solid #aaa;width:100%;}"
								"a {color:#a33;}a:hover {color:#e33;}</style>";
// <table id="list" cellpadding="0.1em" cellspacing="0">
const char *dz_s_http_table =	"<table id=\"list\" cellpadding=\"0.1em\" cellspacing=\"0\">";
// <colgroup><col width="55%"><col width="20%"><col width="25%"></colgroup>
const char *dz_s_http_col =		"<colgroup><col width=\"55%\"><col width=\"20%\"><col width=\"25%\"></colgroup>";
// <a href="http://nightly.yam.ch/?C=N&amp;O=A">File Name</a>
const char *dz_s_link_sHST_sSW_sTASK_nIDX_sTXT =			"<a href=\"http://%s/%s%s0%llx\">%s</a></td><td>";
const char *dz_s_link_sHST_sSW_sTASK_nIDX_sTXTpp =			"<a href=\"http://%s/%s%s%llx\">%s\\</a></td><td>";
const char *dz_s_link_sHST_sSW_idTASK_nCLK_sTXT =		"<a href=\"http://%s/%s0%llx_%llx\">%s</a>";		// nullal kikényszerítem, hogy hexa esetén is
const char *dz_s_link_sHST_sSW_idTASK_nCLK_nIDX_sTXT =	"<a href=\"http://%s/%s0%llx_%llx_%llx\">%s</a>";

const char *dz_s_http_dwnl_sFN_sFNdzr_nS =	"HTTP/1.0 200 OK\r\n" \
											"Server: piMASS\r\n" \
											"Connection: Keep-Alive\r\n" \
											"Content-Type: application/octet-stream\r\n" \
											"Content-Disposition: form-data; name=\"%s\"; filename=\"%s.dzr\"\r\n"\
											"Content-Transfer-Encoding: binary\r\n"\
											"Content-Length: %lld\r\n" \
											"\r\n";
const char *dz_s_http_dwnl_sFN_sFNpEXP_nS =	"HTTP/1.0 200 OK\r\n" \
											"Server: piMASS\r\n" \
											"Connection: Keep-Alive\r\n" \
											"Content-Type: application/octet-stream\r\n" \
											"Content-Disposition: form-data; name=\"%s\"; filename=\"%s\"\r\n"\
											"Content-Transfer-Encoding: binary\r\n"\
											"Content-Length: %lld\r\n" \
											"\r\n";

gpcLZY* gpcLZY::qEVENT(void)
{
	if(!this)
		return NULL;
	if(aWIP[gpeLZYwip] >= gpeWIP_esc)
		return NULL;

	U4 nREP = 10, nLOCK = 0;
	while( nREP )
	{
		nREP--;
		if( aWIP[gpeLZYwip] != gpeWIP_done )
		{
			usleep(100);
			continue;
		}
		nLOCK++;
		aWIP[gpeLZYwipSTK] = aWIP[gpeLZYwip];
		if( !n_load )
		{
			DO();
			return NULL;
		}

		//gt_wip_stk = gt_wip;
		aWIP[gpeLZYwip] = gpeWIP_idle;
		return this;
	}
	if( !nLOCK )
		return NULL;

	DO();
	return NULL;
}

gpcLZY* gpcGT_DWNL::join( gpcLZY* pOUT, gpcGT& mom, gpcLZY* pEXE )
{
	if( pOUT ? pOUT->n_alloc : false )
		return pOUT;
	U8 n_size = -1, n_send = 0, s;
	switch( an.alf )
	{
		case gpeALF_FAVICON:
			n_send = sizeof(dz_a_favpng);
			pOUT = pOUT->lzyFRMT( n_size, dz_s_http_dwnl_sFN_sFNdzr_nS, "favicon", "favicon.png", n_send );
			pOUT = pOUT->lzyINS( dz_a_favpng, n_send, n_size, 0 );
			return pOUT->lzy_reqCLOSE();
		case gpeALF_S:
			n_send = pEXE ? pEXE->n_alloc : 0;
			if( !n_send )
				return pOUT;
			pOUT = pOUT->lzyFRMT( n_size, dz_s_http_dwnl_sFN_sFNpEXP_nS, "setup", "setup.exe", n_send );
			pOUT = pOUT->lzyPLUS( pEXE, n_size );
			return pOUT;
		default:
			break;
	}
}

gpcHUD* gpcHUD::put( const void* p_void, I8 n_byte)
{
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
}


void gpcGTall::clr()
{
	for( U4 a = 0; a < nGTld; a++ )
	{
		gpmDEL( ppGTalloc[a] );
	}
	gpmDELary(ppGTalloc);
	gpmCLR;
}
/*gpcGT::~gpcGT()
{
	GTclose();
}*/
gpcGT* gpcGTall::GT( SOCKET sock )
{
	if( !this )
		return NULL;
	for( U4 g = 0; g < nGTld; g++ )
	{
		if( !ppGTalloc[g] )
			continue;
		if( ppGTalloc[g]->socket == sock )
			return ppGTalloc[g];

		gpcGT* pGT = ppGTalloc[g]->GTacc.GT( sock );
		if( pGT )
			return pGT;
	}
	return NULL;
}
gpcGT* gpcGTall::GT( gpeALF alf, U1* pIPA, U4 nIPA )
{
	int port = 80, nCMP = nIPA, p;
	U8 nLEN;
	U1	*pS = pIPA+gpmNINCS( pIPA, " \t\r\n!" ),
		*pE = pS+gpmVAN( pS, " \a\t\r\n,:;", nLEN );

	nCMP = pE-pS;
	//gpmMEMCPYoff( sPUB, pS, nCMP );
	//sPUB[nCMP] = 0;

	p = gpfSTR2I8( pE, NULL );
	if( p )
		port = p;
	I8x2 an( alf, port );

	for( U4 g = 0; g < nGTld; g++ )
	{
		if( !ppGTalloc[g] )
		{
			if( iGTfr > g )
				iGTfr = g;
			continue;
		}

		if( ppGTalloc[g]->port != port )
			continue;
		if( ppGTalloc[g]->TnID.alf != an.alf )
			continue;

		pGT = ppGTalloc[g];
		if( !pGT->bGTdie() )
			return pGT;

		if( gpmMEMCMP( pS, pGT->s_ip + (*pGT->s_ip == '!'), nCMP ) == nCMP )
			return pGT;

		*pGT->s_ip = '!';
		gpmMEMCPYoff( pGT->s_ip+1, pS, nCMP );
		pGT->s_ip[nCMP+1] = 0;
		return pGT;
	}

	if( iGTfr < nGTld )
	{
		pGT = new gpcGT( an, port );
		gpmMEMCPYoff( pGT->s_ip, pS, nCMP );
		pGT->s_ip[nCMP] = 0;

		return ppGTalloc[iGTfr] = pGT;
	}

	nGTld++;
	if( nGTld >= nGTalloc )
	{
		nGTalloc += 0x10;
		gpcGT	**ppKILL = ppGTalloc;
		ppGTalloc = new gpcGT*[nGTalloc];
		gpmMEMCPYoff( ppGTalloc, ppKILL, iGTfr ); // mert nGTld == iGTfr+1
		gpmDELary(ppKILL);
	}

	pGT = new gpcGT( an, port );
	gpmMEMCPYoff( pGT->s_ip, pS, nCMP );
	pGT->s_ip[nCMP] = 0;

	return ppGTalloc[iGTfr] = pGT;
}
gpcGT* gpcGTall::GT( gpeALF alf, I4 port )
{
	if( pGT )
	if( pGT->port == port )
	if( pGT->TnID.alf == alf )
		return pGT;

	pGT = NULL;
	if( iGTfr > nGTld)
		iGTfr = nGTld;
	else if( !ppGTalloc )
		iGTfr = nGTld = nGTalloc = 0;
	else if( ppGTalloc[iGTfr] )
		iGTfr = nGTld;

	for( U4 g = 0; g < nGTld; g++ )
	{
		if( !ppGTalloc[g] )
		{
			if( iGTfr > g )
				iGTfr = g;
			continue;
		}

		if( ppGTalloc[g]->port != port )
			continue;
		if( ppGTalloc[g]->TnID.alf != alf )
			continue;

		pGT = ppGTalloc[g];
		return pGT;
	}
	if( iGTfr < nGTld )
		return ppGTalloc[iGTfr] = pGT = new gpcGT( I8x2( alf, 0 ), port );

	nGTld++;
	if( nGTld >= nGTalloc )
	{
		nGTalloc += 0x10;
		gpcGT	**ppKILL = ppGTalloc;
		ppGTalloc = new gpcGT*[nGTalloc];
		gpmMEMCPYoff( ppGTalloc, ppKILL, iGTfr ); // mert nGTld == iGTfr+1
		gpmDELary(ppKILL);
	}

	return ppGTalloc[iGTfr] = pGT = new gpcGT( I8x2( alf, 0 ), port );
}
gpcGT* gpcGTall::GTacc( SOCKET sock, I4 port )
{
	gpcGT *pACC = NULL, *p_a;
	if( iGTfr < nGTld )
	if( pACC = ppGTalloc[iGTfr] )
	if( pACC->bGTdie() )
	{
		pACC->TnID.alf = gpeALF_ACCEPT;
		pACC->TnID.num = iGTfr;
		pACC->port = port;
		gpfSOC_CLOSE( pACC->socket );
		pACC->socket = sock;
		// +-- --  -
		pACC->msGTdie = 0;
		iGTfr = nGTld;
	} else {
		pACC = NULL;
	}

	if( !pACC )
	for( I8 a = 0; a < nGTld; a++ )
	{
		p_a = ppGTalloc[a];
		if( p_a )
		{
			if( !p_a->bGTdie() )
				continue;

			pACC = p_a;
			pACC->TnID.alf = gpeALF_ACCEPT;
			pACC->TnID.num = a;
			pACC->port = port;
			gpfSOC_CLOSE( pACC->socket );
			pACC->socket = sock;
			// +-- --  -
			pACC->msGTdie = 0;
			break;
		}

		if( iGTfr > a )
				iGTfr = a;
	}

	if( !pACC )
	{
		pACC = new gpcGT( I8x2( gpeALF_ACCEPT, iGTfr ), port, sock );
		if( iGTfr >= nGTld )
		{
			gpcGT** pp_kill = ppGTalloc;
			U8 n_kill = nGTalloc;
			nGTalloc += FD_SETSIZE;
			ppGTalloc = new gpcGT*[nGTalloc];
			if( pp_kill )
			{
				gpmMEMCPYoff( ppGTalloc, pp_kill, n_kill );
			}
			gpmZn( ppGTalloc+n_kill+1, (nGTalloc-n_kill-1) );
			gpmDELary( pp_kill );
		}
		else if( ppGTalloc[iGTfr] )
		{
			delete ppGTalloc[iGTfr];
		}
		ppGTalloc[iGTfr] = pACC;
		while( iGTfr < nGTalloc )
		{
			if( !ppGTalloc[iGTfr] )
				break;

			iGTfr++;
			if( nGTld < iGTfr )
				nGTld = iGTfr;
		}

	}

	return pACC;
}


int gpcGT::GTerr( char* p_error, char** pp_error )
{
	*p_error = 0;
	int	out = true;
	switch ( errno )
	{
		case ENOTSOCK:
			p_error += sprintf( p_error, "NOTSOCK The descriptor is not a socket!!\n" );
			out = false;
			break;
		/*case ENOBUFFS:
			p_error += sprintf( p_error, "WSAENOBUFS No buffer space available, try again!!\n" );
			out = false;
			break;*/
		case EFAULT:
			p_error += sprintf( p_error, "WSAEFAULT Bad address, try again!!\n" );
			out = false;
			break;
		case EINTR:
			p_error += sprintf( p_error, "WSAEINTR Interrupted function call!!\n" );
			out = true;
			break;
		case ECONNRESET:
			p_error += sprintf( p_error, "WSAECONNRESET Connection reset by peer!!\n");
			out = false;
			break;
		/*case ENOTINITIALISED:
			p_error += sprintf( p_error, "WSANOTINITIALISED WSAStartup not yet performed!!\n");
			p_error += sprintf( p_error, "\tWSAStartup - " );
			WSADATA wsaData;
			if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
			{
				p_error += sprintf( p_error, "ERROR\n" );
				out = false;
				break;
			}
			p_error += sprintf( p_error, "OK\n" );
			out = true;
			break;*/
		case ECONNREFUSED:
			p_error += sprintf( p_error, "WSAECONNREFUSED Connection refused!\n");
			out = false;
			break;
		case EADDRINUSE:
			p_error += sprintf( p_error, "WSAEADDRINUSE Port already in use!\n");
			out = false;
			break;
		case EWOULDBLOCK:
			p_error += sprintf( p_error, "WSAEWOULDBLOCK Event block!\n");
			out = 2;
			break;
		default:
			p_error += sprintf( p_error, "ERROR: %x\n", errno );
			out = false;
	}
	if( pp_error )
		*pp_error = p_error;
	return out;
}

U1 gpcGT::GTopt( char* p_error, char** pp_error, int no_daley, U4 n_buff )
{
	*p_error = 0;
	p_error += sprintf( p_error, "\n\t\t-= SOCKET_OPT =-" );

	int yes = 1;
	int sndlow;
	int rcvlow;
	int setsndlow = 0xd5;
	int setrcvlow = 0xd5;
	int sndbuf = n_buff/2;
	int rcvbuf = n_buff/2;
	socklen_t lenp = sizeof(socklen_t);
	int result = 0;
	int sndtout = sndbuf;
	int rcvtout = rcvbuf;

	bool b_keep = true;

	int	b_len = sizeof(b_keep);
	int	i_len = sizeof(int);
	int	i_keep;
	u_long iMode = 1;

	//ioctlsocket(socket, FIONBIO, &iMode );
	result = setsockopt(	socket,		/* socket affected */
							IPPROTO_TCP,	/* set option at TCP level */
							TCP_NODELAY,	/* name of option */
							(char *) &no_daley,	/* the cast is historical cruft */
							sizeof(int));	/* length of option value */
	if( result == SOCKET_ERROR )
		GTerr( p_error, &p_error );
	p_error += sprintf( p_error, "\n\t\tDEF IPPROTO_TCP: %d", no_daley );

	result = getsockopt( socket, SOL_SOCKET, SO_SNDBUF, (char*)&sndbuf, &lenp );
	if( result == SOCKET_ERROR )
		GTerr( p_error, &p_error );
	p_error += sprintf( p_error, "\n\t\tDEF SO_SNDBUF: %d", sndbuf );

	if( sndbuf != sndtout )
	{
		//DZF_FORMAT_Write( "\n\t\tSO_SNDBUF SET: %d", sndtout );
		setsockopt( socket, SOL_SOCKET, SO_SNDBUF, (char *)&sndtout, lenp );

		result = getsockopt( socket, SOL_SOCKET, SO_SNDBUF, (char*)&sndbuf, &lenp );
		if( result == SOCKET_ERROR )
			GTerr( p_error, &p_error );

		p_error += sprintf( p_error, "\tResult: %d", sndbuf );
	}


	p_error += sprintf( p_error, "\n\t\tDEF SO_RCVBUF: " );
	result = getsockopt( socket, SOL_SOCKET, SO_RCVBUF, (char*)&rcvbuf, &lenp );
	if( result == SOCKET_ERROR )
		GTerr( p_error, &p_error );

	p_error += sprintf( p_error, "\n\t\tDEF SO_RCVBUF: %d", rcvbuf );


	if( rcvbuf != rcvtout )
	{
		p_error += sprintf( p_error, "\n\t\tSO_RCVBUF SET: %d", rcvtout );
		setsockopt( socket, SOL_SOCKET, SO_RCVBUF, (char *)&rcvtout, lenp );

		result = getsockopt( socket, SOL_SOCKET, SO_RCVBUF, (char*)&rcvbuf, &lenp );
		if( result == SOCKET_ERROR )
			GTerr( p_error, &p_error );
		p_error += sprintf( p_error, "\tResult: %d", rcvbuf );
	}



	p_error += sprintf( p_error, "\n\t\tDEF IPPROTO_IP - IP_TTL: " );
	int ttl;  // Allocate space for TTL.
	socklen_t sizeofttl = sizeof(ttl); // Create an integer that contains the size of the TTL value.
	getsockopt(
				socket, IPPROTO_IP, IP_TTL,
				(char *)&ttl,
				&sizeofttl
			);
	p_error += sprintf( p_error, "%d", ttl );

	if( ttl != 156 )
	{
		ttl = 156;
		p_error += sprintf( p_error, "\n\t\tSET IPPROTO_IP - IP_TTL: " );
		setsockopt( socket, IPPROTO_IP, IP_TTL, (char *)&ttl, sizeofttl);
		p_error += sprintf( p_error, "%d", ttl );

		p_error += sprintf( p_error, "\n\t\tCHK IPPROTO_IP - IP_TTL: " );
		getsockopt( socket, IPPROTO_IP, IP_TTL, (char *)&ttl, &sizeofttl);
		p_error += sprintf( p_error, "%d", ttl );
	}

	/*unsigned long addr;			//Allocate space for address.
	int sizeofaddr = sizeof(addr); //Create an integer containing size of*/

	p_error += sprintf( p_error, "n\t\tSOL_SOCKET - ");
	setsockopt(	socket,
			SOL_SOCKET,
			SO_REUSEADDR,
			(char *)&yes,
			sizeof(yes) );

	/*int nbio = 0;
	::ioctl(socket,FIONBIO,&nbio);*/
	return 1;
}


char* gpcGT::GTrcv( char* p_err, char* s_buff, U4 n_buff )
{
	if( bGTdie() )
		return p_err;
	// ha nincsen hiba p_err[0] == 0
	// egyébként bele ír valamit akkor nem
	p_err[0] = 0;
	int n = recv( socket, s_buff, n_buff, 0 );
	if( n == SOCKET_ERROR )
	{
		char* p_error = s_buff;
		p_err += GTopt( p_err, &p_err, gpdGT_NoDALEY, n_buff );
		msGTdie = 1;
		sprintf( p_err, "\nERROR: recv SOCKET_ERROR DIE" );
		return p_err;
	}

	s_buff[n] = 0;
	U8 s = -1;
	pINP = pINP->lzyINS( (U1*)s_buff, n, s, 0 );

	if( n )
		return p_err;

	msGTdie = 3;
	sprintf( p_err, "\nrecv n == 0?" );
	return p_err;

	gpfSOC_CLOSE( socket );
	msGTdie = 1;

	sprintf( p_err, "\nERROR: recv n == 0 DIE" );
	return p_err;
}
char* gpcGT::GTsnd( char* p_err, char* s_buff, U4 n_buff )
{
	if( bGTdie() )
		return p_err;
	// ha nincsen hiba p_err[0] == 0
	// egyébként bele ír valamit akkor nem
	p_err[0] = 0;
	U8 s;
	if( !pOUT )
	{
		if( !pPUB )
			return p_err;

		pOUT = pOUT->lzyPLUS( pPUB, s = -1 );
		gpmDEL( pPUB );
		if( !aGTcrs[1] )
		if( aGTcrs[0] )
		{
			pOUT = pOUT->lzyFRMT( s = -1, ";" );
		} else {
			pOUT = pOUT->lzyFRMT( s = -1, "\r\n%x>", socket );
			if( pINP )
				pOUT = pOUT->lzyFRMT( s = -1, "%s", pINP->p_alloc ? (char*)pINP->p_alloc : "" );
		}
	}

	//SetThreadPriority( whd, THREAD_PRIORITY_HIGHEST );
	I8 n = pOUT->n_load, nSKIP;
	if( false )
	if( aGTcrs[0]  )
	{
		if( pOUT->p_alloc[n] )
			pOUT->p_alloc[n] = 0;
		char a_str[] = " ";
		a_str[0] = aGTcrs[0];
		n = gpdVAN( (char*)pOUT->p_alloc, a_str );
		nSKIP = gpmNINCS((char*)pOUT->p_alloc + n, a_str);
		n += nSKIP;
	}

	if( n > n_buff )
		n = n_buff;

	n = send( socket, (char*)pOUT->p_alloc, n, 0 );
	if( n == SOCKET_ERROR )
	{
		char* p_error = s_buff;
		p_err += GTerr( p_error, &p_error );
		msGTdie = 1;
		sprintf( p_err, "\nERROR: send SOCKET_ERROR DIE" );
		return p_err;
	}
	if( n < 1 )
	{
		sprintf( p_err, "\nERROR: send 0 byte" );
		return p_err;
	}
	U4 sub = n, nREPETA = 0;
	if( false )
	while( sub < pOUT->n_load )
	{
		n = pOUT->n_load-sub;
		if( n > n_buff )
			n = n_buff;

		n = send( socket, ((char*)pOUT->p_alloc)+sub, n, 0 );
		if( n == SOCKET_ERROR )
		{
			pOUT->lzyINS( NULL, 0, s = 0, sub );
			char* p_error = s_buff;
			p_err += GTerr( p_error, &p_error );
			msGTdie = 1;
			sprintf( p_err, "\nERROR: send SOCKET_ERROR DIE" );
			return p_err;
		}

		sub += n;
		nREPETA++;
	}


	bool b_close = pOUT->qCLOSE();
	pOUT->lzyINS( NULL, 0, s = 0, sub ); //n );
	if( !pOUT->n_load )
		gpmDEL( pOUT );

	if( !b_close )
		return p_err;

	msGTdie = 1;
	//sprintf( p_err, "\nGOOD only END! %x", socket );
	n = send( socket, NULL, 0, 0 );
	//gpfSOC_CLOSE( socket );
	int h = gpfSOC_CLOSE( socket );
	//socket = INVALID_SOCKET;

	return p_err;
}

static const char gp_sHELLO[] = " -= Welcome in piMASS 2019 =-\r\n    -= Writen by Dezso Bodor =-\r\n  -= more info use 'help' command =-\r\n%X>";
static const char gp_sHELLO_acc[] = "account 0x%x;\r -= Welcome in piMASS 2019 =-\r\n    -= Writen by Dezso Bodor =-\r\n  -= more info use 'help' command =-\r\n%X>";

/// ---------- SLMP -------------
// SNo.NnSnUn..MsLen.Mtm.
// +-->+>+>+-->+>+-->1--4   8  12  16  20  24	len 24 0x18
// |234|2|2|234|2|234|234|234|234|2|23456|234
// 500000FF03FF000018000004010000D*0000120001
// +-->+>+>+-->+>+-->+-->5    11        21 24
// D00000FF03FF0000440000
// 0032 000A DEAD 0000
// 0000 0000 0000 0000
// 0000 0000 0000 0000
// 0000 0000 0000 0000

//                                   SNo.NnSnUn..MsLen.Mtm.
//                                   +-->+>+>+-->+>+-->1--4   8  12  16  20  24	len 24 0x18
//                                   +-->+>+>+-->+>+-->123456789012345678901234
//                                   123456789012345678901234567890123456789012
//                                   |234|2|2|234|2|234|234|234|234|2|23456|234
//                                   500000FF03FF000018000004010000D*0000120001rn
// static const char gp_sSLMP_read[] = "500000FF03FF00%0.4x000004010000D*%0.6x%0.4x";
I8 gpcGT::GTcnct( gpcWIN& win )
{
	if( this ? msGTdie > win.mSEC.x : true )
		return 0;

	struct timeval tv;   // sleep for tenr minutes!
	tv.tv_sec = 0;
	tv.tv_usec = gpdGT_LIST_tOUT;

	bool bNEWip = *s_ip == '!';
	U8 s;
	if( bNEWip || bGTdie() ) //socket == INVALID_SOCKET )
	{
        switch( msGTdie )
        {
			case 1:
				gpfSOC_CLOSE( socket );
				msGTdie = (win.mSEC.x+1500)|1;
				return 0;
			case 3:
				gpfSOC_CLOSE( sockCNCT );
				msGTdie = (win.mSEC.x+1500)|1;
				return 0;
        }

		pUSER = sUSER;
		pHOST = sHOST;
		pFILE = sFILE;
		*sUSER = 0;
		*sHOST = 0;
		*sFILE = 0;
		gpmDEL(pINP);
		gpmDEL(pMISi);
		nSYN = 0;

		if( bNEWip )
		{
			sprintf( s_ip, "%s", s_ip+1 );
			gpfSOC_CLOSE( sockCNCT );
		}

		char	*p_print = (char*)win.sGTpub,
				s_port[32],
				*p_err = p_print;

		if( sockCNCT != INVALID_SOCKET )
			socket = sockCNCT;
		else {
			struct addrinfo ainfo;
			gpmZ( ainfo );
			ainfo.ai_family = AF_INET;
			ainfo.ai_socktype = SOCK_STREAM;
			ainfo.ai_protocol = IPPROTO_TCP;
			ainfo.ai_flags = AI_PASSIVE;
			ainfo.ai_addrlen = sizeof(ainfo);


			sprintf( s_port, "%d", port);
			int error = getaddrinfo( NULL, s_port, &ainfo, &p_ainf );
			if( error != 0 )
				GTerr( p_err, &p_err );

			socket = ::socket( p_ainf->ai_family, p_ainf->ai_socktype, p_ainf->ai_protocol );

			GTopt( p_err, &p_err, gpdGT_NoDALEY, sizeof(win.sGTbuff) );
			p_err += sprintf( p_err, "OK NEW CNCT" );

			addr_in.sin_family = AF_INET;
			addr_in.sin_port = htons(port);
			addr_in.sin_addr.s_addr = inet_addr( s_ip );
			// LISTEN: addr_in.sin_addr.s_addr = INADDR_ANY;
			memset(&(addr_in.sin_zero), '\0', 8);

			// set non blocking
			U8 a = fcntl( socket, F_GETFL, NULL );
			a |= O_NONBLOCK;
			fcntl(socket, F_SETFL, a );

			error = connect(
								socket,
								(struct sockaddr *)&addr_in, 			// (struct sockaddr *)&addr,
								sizeof(addr_in)
						);
		}

		// BIND
		p_err += sprintf( p_err, "\n\t\t - try CNCT - %d", msGTdie );
		cout << p_print << endl;
		p_print = p_err;

		fd_set cnct_w;
		FD_ZERO( &cnct_w );
		FD_SET( socket, &cnct_w );
		int rc = select( socket+1, NULL, &cnct_w, NULL, &tv );
		if( rc > 0 && FD_ISSET( socket, &cnct_w ) )
		{
			sockCNCT = INVALID_SOCKET;
		} else {
			// még nem sikerült eldugjuk ne szabadítsák fel
			sockCNCT = socket;
			socket = INVALID_SOCKET;
			msGTdie = (win.mSEC.x+1500)|1;
			return 0;
		}
		switch( TnID.alf )
		{
			case gpeALF_SLMP:{
				//U4 nDEV = 512;
				pOUT = pOUT->lzyFRMT( s = 0, gpdSLMP_read, 0x18, 0, gpdSLMPnDEV );
				break;
			}
			default:
				if( !(port&1) )// telnetek
					break;

				pOUT = pOUT->lzyFRMT( s = 0, gp_sHELLO_acc, socket, iCNT );
				iCNT++;
				aGTcrs[0] = 'a';
				break;
		}
		msGTdie = 0;
	}

	char	*p_err = (char*)win.sGTpub;
			*win.sGTpub = 0;

	if( aGTfd[gpeFDrcv].isFD( socket ) ) // FD_ISSET( p_gt->socket, &a_fdset[gpeFDrcv] ) )
	{
		p_err = GTrcv( p_err, (char*)win.sGTbuff, sizeof(win.sGTbuff) );
		if( *p_err )
			cerr << p_err << endl;

		switch( TnID.alf )
		{
			case gpeALF_SLMP:
				//GTslmp( *this, &win );
				break;
			default:
				if( aGTcrs[1] == 'h' )
					break;

				GTos( *this, &win );
				break;
		}

	}
	else if( !pOUT )
	if( pINP ? pINP->n_load : false )
	{
		switch( TnID.alf )
		{
			case gpeALF_SLMP:
				//GTslmp( *this, &win );
				break;
			default:
				if( aGTcrs[1] == 'h' )
					break;

				GTos( *this, &win );
				break;
		}
	}

	switch( TnID.alf )
	{
		case gpeALF_SLMP:
			GTslmp( *this, &win );
			break;
	}


		/*if( aGTcrs[1] != 'h' )
			GTos( *this, &win );
	}*/

	if( aGTfd[gpeFDsnd].isFD(socket ) ) // FD_ISSET( p_gt->socket, &a_fdset[gpeFDsnd] ) )
	{
		U8 s, nOUT = GTout( &win );
		if( nOUT <= (gpdHUDn*2) )
		{
			if(pHUD ? pHUD->p_alloc : NULL )
			{
				pOUT = pOUT->lzyFRMT( s = -1, " hud" );

				pOUT = pOUT->lzyADD(pHUD, 8, s = -1);
				pOUT = pOUT->lzyADD(pHUD->p_alloc, pHUD->n, s = -1);
				gpmDEL(pHUD);
				GTprmpt();
				//break;
			}


			if( pDWN )
			{
				pOUT = pDWN->join( pOUT, *this );
				if( !pOUT )
				if( pDWN->dw_i >=  pDWN->dw_il )
				{
					gpmDEL(pDWN);
				}
			}
		}

		p_err = GTsnd( p_err, (char*)win.sGTbuff, sizeof(win.sGTbuff) );
		if( *p_err )
			cerr << p_err << endl;
	}

	aGTfd[gpeFDrcv].setFD( socket );
	aGTfd[gpeFDsnd].setFD( socket );



	int nS = select(
					aGTfd[gpeFDrcv].maxSCK+1,
					&aGTfd[gpeFDrcv].fdS,
					(aGTfd[gpeFDsnd].nFD ? &aGTfd[gpeFDsnd].fdS : NULL),
					NULL, //(aGTfd[gpeFDexcp].fd_count ? &aGTfd[gpeFDexcp] : NULL),
					&tv
				);

	return nS;
}
I8 gpcGT::GTlst( gpcWIN& win, gpcGTall& cnct )
{
	if( this ? msGTdie > win.mSEC.x : true )
		return 0;

	if( socket == INVALID_SOCKET )
	{

		switch( msGTdie )
        {
			case 1:
				gpfSOC_CLOSE( socket );
				msGTdie = (win.mSEC.x+1500)|1;
				return 0;
        }

		pUSER = sUSER;
		pHOST = sHOST;
		pFILE = sFILE;
		*sUSER = 0;
		*sHOST = 0;
		*sFILE = 0;
		gpmDEL(pINP);
		nSYN = 0;

		struct addrinfo ainfo;
		gpmZ( ainfo );
		ainfo.ai_family = AF_INET;
		ainfo.ai_socktype = SOCK_STREAM;
		ainfo.ai_protocol = IPPROTO_TCP;
		ainfo.ai_flags = AI_PASSIVE;
		ainfo.ai_addrlen = sizeof(ainfo);
		char	*p_print = (char*)win.sGTpub,
				s_port[32],
				*p_err = p_print;
		sprintf( s_port, "%d", port);
		int error = getaddrinfo( NULL, s_port, &ainfo, &p_ainf );
		if( error != 0 )
			GTerr( p_err, &p_err );
		socket = ::socket( p_ainf->ai_family, p_ainf->ai_socktype, p_ainf->ai_protocol );

		p_err += sprintf( p_err, "OK" );
		GTopt( p_err, &p_err, gpdGT_NoDALEY, sizeof(win.sGTbuff) );
		cout << p_print << endl;
		p_print = p_err;

		// BIND
		p_err += sprintf( p_err, "\n\t\tBIND - " );
		/* bind */

		addr_in.sin_family = AF_INET;
		addr_in.sin_addr.s_addr = INADDR_ANY;
		addr_in.sin_port = htons(port);
		memset(&(addr_in.sin_zero), '\0', 8);

		if( (error = bind(socket, (struct sockaddr *)&addr_in, sizeof(addr_in))) == -1)
		{

			perror("Server-bind() error lol!");

			exit(1);

		}

		//error = bind( socket, p_ainf->ai_addr, p_ainf->ai_addrlen );
		if( error == SOCKET_ERROR )
		{
			GTerr( p_err, &p_err );
			return -1;
		}
		p_err += sprintf( p_err, "OK" );
		cout << p_print << endl;
		p_print = p_err;

		p_err += sprintf( p_err, "\n\t\tLITEN LISTENER - " );
		error = listen( socket, SOMAXCONN );
	}
	//SOCKET acpt_soc;
	char *p_err = (char*)win.sGTpub;
	*win.sGTpub = 0;
	if( aGTfd[gpeFDrcv].isFD( socket ) ) // FD_ISSET( socket, &a_fdset[gpeFDrcv]) )
	{
		// LISTENER
		//struct sockaddr_in
		sockaddr_in clientaddr;
		socklen_t n_clientaddr = sizeof(clientaddr);
		SOCKET	acpt_soc = INVALID_SOCKET;

		if( (acpt_soc = accept( socket, (struct sockaddr *)&clientaddr, &n_clientaddr )) == INVALID_SOCKET  )
		{
			p_err += GTerr( p_err, &p_err );
		} else {

			gt_ip.num++;
			I8x2 gt_an_ip( gpeALF_null, 0 );

			// localhost-ot külön kell ellenőrizni (sajnos)
			const U4 localhost_ip = 0x0100007f;
			if( localhost_ip == addr_in.sin_addr.s_addr ) //.S_un.S_addr )
			{
				gt_an_ip.alf = gpeALF_LOCAL; // gpeGATE_NET_TYPE_LOCALHOST;
			}

			if( p_host_ips && (gt_an_ip.alf == gpeALF_null) ) {
				//if( !p_IPTable )
				//{
				//	DWORD dwSizeReq = 0;
				//	DWORD dwRet = GetIpAddrTable( p_IPTable, &dwSizeReq, TRUE );
				//	if( dwRet == ERROR_INSUFFICIENT_BUFFER )
				//	{
				//		p_IPTable = (PMIB_IPADDRTABLE)(new BYTE[dwSizeReq]);
				//		dwRet = GetIpAddrTable( p_IPTable, &dwSizeReq, TRUE );
				//	}
				//}

				int i, j, idx;
				gt_an_ip.alf = gpeALF_INTERNET;

				//gethostname (char *host)
				//{
				//	struct hostent *p_hp = malloc (sizeof(*p_host_ips) );
				//
				//	int res;
				//	int herr;
				//
				//	size_t nHLEN = 1024;
				//	char *p_tmphstbuf = malloc (nHLEN);
				//
				//	while ((res = gethostbyname_r ( s_ip, p_hp, p_tmphstbuf, nHLEN,
				//								 &p_host_ips, &herr)) == ERANGE)
				//	{
				//		/* Enlarge the buffer.  */
				//		nHLEN *= 2;
				//		p_tmphstbuf = realloc( p_tmphstbuf, nHLEN );
				//	}
				//
				//	free( p_tmphstbuf );
				//
				//}

				/*for( i = 0; p_host_ips->h_addr_list[i]; i++ )
				{
					idx = -1;
					for( j = 0; j < p_IPTable->dwNumEntries; j++ )
					{
						if( p_IPTable->table[j].dwAddr == *((DWORD*)p_host_ips->h_addr_list[i]) )
						{
							idx = j;
							break;
						}
					}
					if( idx )
					{
						DWORD dwinaddr = (DWORD)clientaddr.sin_addr.S_un.S_addr;
						DWORD dwhostaddr = *((DWORD*)p_host_ips->h_addr_list[i]);

						dwinaddr &= p_IPTable->table[idx].dwMask;
						dwhostaddr &= p_IPTable->table[idx].dwMask;

						if( dwinaddr == dwhostaddr )
						{
							gt_an_ip.a = gpeALF_LOCAL;
							break;
						}
					}
				}*/
			}
			else
			if( !p_host_ips  ) //|| !p_IPTable )
			{
				gt_an_ip.alf = gpeALF_UNDEF;
			}

			gpcGT *pACC = GTacc.GTacc( acpt_soc, port );

			if( pACC )
			{
				pACC->addr_in = clientaddr;
				pACC->GTclr();
				U8 n_start = -1;
				if( pACC->port & 1 )// telnetek
				{
					pACC->pOUT = pACC->pOUT->lzyFRMT(n_start, gp_sHELLO_acc, pACC->socket, pACC->iCNT );
					pACC->iCNT++;
					pACC->aGTcrs[0] = 'a';
					pACC->GTos(*this);
				}
				else switch( pACC->port )
				{
					case 23: // telnet
						pACC->pOUT = pACC->pOUT->lzyFRMT( n_start,  gp_sHELLO, pACC->iCNT );
						pACC->aGTcrs[0] = 'a';
						pACC->GTos( *this );
						break;
					case 80: // http
						pACC->aGTcrs[1] = 'h';
						break;
					default:
						pACC->pOUT = pACC->pOUT->lzyFRMT( n_start,  gp_sHELLO, pACC->iCNT );

				}

				pACC->GTopt( p_err, &p_err, gpdGT_NoDALEY, sizeof(win.sGTbuff) );
				getifaddrs(&pIFadr);
				char	*s_buff = (char*)win.sGTpub,
						*pB = s_buff+gpdRECVn/2, *pBi = pB;
				void	*pIFtmp = NULL;
				for( pIFa = pIFadr; pIFa != NULL; pIFa = pIFa->ifa_next )
				{
					if( !pIFa->ifa_addr)
						continue;

					if (pIFa->ifa_addr->sa_family == AF_INET)
					{ // check it is IP4
						// is a valid IP4 Address
						pIFtmp = &((struct sockaddr_in *)pIFa->ifa_addr)->sin_addr;
						inet_ntop(AF_INET, pIFtmp, s_buff, INET_ADDRSTRLEN);
						pBi += sprintf( pBi, "%s IPA: %s \n", pIFa->ifa_name, s_buff );
						//printf("%s IP Address %s\n", pIFa->ifa_name, aADRbuf);
					} else if (pIFa->ifa_addr->sa_family == AF_INET6) { // check it is IP6
						// is a valid IP6 Address
						pIFtmp = &((struct sockaddr_in6 *)pIFa->ifa_addr)->sin6_addr;
						inet_ntop(AF_INET6, pIFtmp, s_buff, INET6_ADDRSTRLEN);
						pBi += sprintf( pBi, "%s IPA: %s \n", pIFa->ifa_name, s_buff );
						//printf("%s IP Address %s\n", pIFa->ifa_name, aADRbuf);
					}
					gpmSTRCPY( s_buff, pIFa->ifa_name );
				}
				//gpmSTRCPY( s_buff, inet_ntoa(clientaddr.sin_addr) );


				if( strcasecmp( s_buff, pACC->s_ip ) )
					gpmSTRCPY( pACC->s_ip, inet_ntoa(pACC->addr_in.sin_addr));
				else
					pACC->gt_ip.num++;

				//DZF_CON_FORMAT_Write( "\n%x accept IP: %s ", pACC->socket, pACC->s_ip );
			}
		}
	}
	gpcGT* p_gt;
	for( ; nFDs < nFDe; nFDs++ )
	{
		p_gt = GTacc.iGT(nFDs);
		if( p_gt->bGTdie() )
			continue;

		if( aGTfd[gpeFDrcv].isFD( p_gt->socket ) ) // FD_ISSET( p_gt->socket, &a_fdset[gpeFDrcv] ) )
		{
			p_err = p_gt->GTrcv( p_err, (char*)win.sGTbuff, sizeof(win.sGTbuff) );
			if( *p_err )
				cerr << p_err << endl;

			if( p_gt )
			{
				if( p_gt->aGTcrs[1] == 'h' )
					p_gt += 0; //->gtOS_html( *this );
				else
					p_gt->GTos( *this, &win, &cnct ); //, win.piMASS ? &win.piMASS->GTcnct : NULL );
			}

			if( pOUT ) /// a mami kapott választ azaz mindenkinek leadjuk a drotot
			for( U8 p = 0, s = -1; p < GTacc.nGTld; p )
			{
				p_gt = GTacc.iGT(p);
				if( p_gt->bGTdie() )
					continue;
				p_gt->pPUB = p_gt->pPUB->lzyPLUS( pOUT, s );
			}
		}
		else if( p_gt->pINP ? !p_gt->pOUT : false )
		{
            if( p_gt )
			{
				if( p_gt->aGTcrs[1] == 'h' )
					p_gt += 0; //->gtOS_html( *this );
				else
					p_gt->GTos( *this, &win, &cnct );
			}

			if( pOUT ) /// a mami kapott választ azaz mindenkinek leadjuk a drotot
			for( U8 p = 0, s = -1; p < GTacc.nGTld; p )
			{
				p_gt = GTacc.iGT(p);
				if( p_gt->bGTdie() )
					continue;
				p_gt->pPUB = p_gt->pPUB->lzyPLUS( pOUT, s );
			}
		}


		if( aGTfd[gpeFDsnd].isFD( p_gt->socket ) ) // FD_ISSET( p_gt->socket, &a_fdset[gpeFDsnd] ) )
		{
			U8 s, nOUT = p_gt->GTout( &win );
			if( nOUT <= (gpdHUDn*2) ) //if( nOUT < 0x400 )
			{
				if(p_gt->pHUD ? p_gt->pHUD->p_alloc : NULL )
				{
					p_gt->pOUT = p_gt->pOUT->lzyFRMT( s = -1, " hud" );

					p_gt->pOUT = p_gt->pOUT->lzyADD(p_gt->pHUD, 8, s = -1);
					p_gt->pOUT = p_gt->pOUT->lzyADD(p_gt->pHUD->p_alloc, p_gt->pHUD->n, s = -1);
					gpmDEL(p_gt->pHUD);
					p_gt->GTprmpt();
					//break;
				}


				if( p_gt->pDWN )
				{
					p_gt->pOUT = p_gt->pDWN->join( p_gt->pOUT, *this );
					if( !p_gt->pOUT )
					if( p_gt->pDWN->dw_i >=  p_gt->pDWN->dw_il )
					{
						gpmDEL(p_gt->pDWN);
					}
				}


			}

			p_err = p_gt->GTsnd( p_err, (char*)win.sGTbuff, sizeof(win.sGTbuff) );
			if( *p_err )
				cerr << p_err << endl;
		}
	}
	aGTfd->zero(3);

	if( nFDe >= GTacc.nGTld )
		nFDs = 0;
	else
		nFDs = nFDe;

	for( nFDe = nFDs; nFDe < GTacc.nGTld; nFDe++ )
	{
		p_gt = GTacc.iGT(nFDe);
		if( p_gt->bGTdie() ) {
			if(
					aGTfd[gpeFDsnd].isFD( p_gt->socket ) //FD_ISSET( p_gt->socket, &a_fdset[gpeFDsnd] )
				|| 	aGTfd[gpeFDrcv].isFD( p_gt->socket )
			)
			{
				cerr << "miért?" << endl;
			} else
				gpfSOC_CLOSE( p_gt->socket );

			if( GTacc.iGTfr > nFDe )
				GTacc.iGTfr = nFDe;
			continue;
		}

		aGTfd[gpeFDrcv].setFD(p_gt->socket);
		//FD_SET( p_gt->socket, &aGTfd[gpeFDrcv] );

		// ennek az az értelme, ha nincsen buffer nem érdekklödik, hogy írható e.
		if( !!p_gt->GTout( &win ) || !!p_gt->pDWN ) // || p_gt->pHUD )
			aGTfd[gpeFDsnd].setFD(p_gt->socket);	//FD_SET( p_gt->socket, &a_fdset[gpeFDsnd] );

		if( aGTfd[gpeFDrcv].nFD >= FD_SETSIZE )
			break;
	}

	aGTfd[gpeFDrcv].setFD( socket );
	U8 n_soc = aGTfd[gpeFDrcv].nFD+aGTfd[gpeFDsnd].nFD;
	if( !n_soc )
	{
		usleep( gpdGT_LIST_tOUT );
		return 0;
	}
	struct timeval tv;   // sleep for tenr minutes!
	tv.tv_sec = 0;
	tv.tv_usec = gpdGT_LIST_tOUT;

	int nS = select(
					aGTfd[gpeFDrcv].maxSCK+1,
					&aGTfd[gpeFDrcv].fdS,
					(aGTfd[gpeFDsnd].nFD ? &aGTfd[gpeFDsnd].fdS : NULL),
					NULL, //(aGTfd[gpeFDexcp].fd_count ? &aGTfd[gpeFDexcp] : NULL),
					&tv
				);

	return nS;
}

