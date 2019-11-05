#include "gpcgt.h"
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
gpcGT* gpcGTall::GTacc( SOCKET sock, I4 port )
{
	gpcGT *pACC = NULL, *p_a;
	if( iGTfr < nGTld )
	if( pACC = ppGTalloc[iGTfr] )
	if( pACC->bGTdie )
	{
		pACC->TnID.alf = gpeALF_ACCEPT;
		pACC->TnID.num = iGTfr;
		pACC->port = port;
		if( pACC->socket != INVALID_SOCKET )
			gpfSOC_CLOSE( pACC->socket );
		pACC->socket = sock;
		// +-- --  -
		pACC->bGTdie = false;
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
			if( !p_a->bGTdie )
				continue;

			pACC = p_a;
			pACC->TnID.alf = gpeALF_ACCEPT;
			pACC->TnID.num = a;
			pACC->port = port;
			if( pACC->socket != INVALID_SOCKET )
				gpfSOC_CLOSE( pACC->socket );
			pACC->socket = sock;
			// +-- --  -
			pACC->bGTdie = false;
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
				gpmMEMCPY( ppGTalloc, pp_kill, n_kill );
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

U1 gpcGT::GTopti( char* p_error, char** pp_error, int no_daley )
{
	*p_error = 0;
	p_error += sprintf( p_error, "\n\t\t-= SOCKET_OPT =-" );

	int yes = 1;
	int sndlow;
	int rcvlow;
	int setsndlow = 0xd5;
	int setrcvlow = 0xd5;
	int sndbuf = sizeof(s_buff)/3;
	int rcvbuf = sizeof(s_buff)/3;
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

	return 1;
}


char* gpcGT::GTrecv( char* p_err )
{
	// ha nincsen hiba p_err[0] == 0
	// egyébként bele ír valamit akkor nem
	p_err[0] = 0;
	I8 n = recv( socket, s_buff, sizeof(s_buff), 0 );
	if( n == SOCKET_ERROR )
	{
		char* p_error = s_buff;
		p_err += GTopti( p_err, &p_err, gpdGT_NoDALEY );
		bGTdie = true;
		sprintf( p_err, "\nERROR: recv SOCKET_ERROR DIE" );
		return p_err;
	}

	s_buff[n] = 0;
	U8 s = -1;
	pINP = pINP->lzy_ins( (U1*)s_buff, n, s, 0 );

	if( n )
		return p_err;


	gpfSOC_CLOSE( socket );
	bGTdie = true;

	sprintf( p_err, "\nERROR: recv n == 0 DIE" );
	return p_err;
}
char* gpcGT::GTsend( char* p_err )
{
	// ha nincsen hiba p_err[0] == 0
	// egyébként bele ír valamit akkor nem
	p_err[0] = 0;
	U8 s;
	if( !pOUT )
	{
		if( !pPUB )
			return p_err;

		pOUT = pOUT->lzy_plus( pPUB, s = -1 );
		gpmDEL( pPUB );
		if( !aGTcrs[1] )
		if( aGTcrs[0] )
		{
			pOUT = pOUT->lzy_format( s = -1, ";" );
		} else {
			pOUT = pOUT->lzy_format( s = -1, "\r\n%x>", socket );
			if( pINP )
				pOUT = pOUT->lzy_format( s = -1, "%s", pINP->p_alloc ? (char*)pINP->p_alloc : "" );
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

	if( n > sizeof(s_buff) )
		n = sizeof(s_buff);

	n = send( socket, (char*)pOUT->p_alloc, n, 0 );
	if( n == SOCKET_ERROR )
	{
		char* p_error = s_buff;
		p_err += GTerr( p_error, &p_error );
		bGTdie = true;
		sprintf( p_err, "\nERROR: send SOCKET_ERROR DIE" );
		return p_err;
	}
	if( n < 1 )
	{
		sprintf( p_err, "\nERROR: send 0 byte" );
		return p_err;
	}

	bool b_close = pOUT->qCLOSE();
	pOUT->lzy_ins( NULL, 0, s = 0, n );
	if( !pOUT->n_load )
		gpmDEL( pOUT );

	if( !b_close )
		return p_err;

	bGTdie = true;
	//sprintf( p_err, "\nGOOD only END! %x", socket );
	n = send( socket, NULL, 0, 0 );
	//gpfSOC_CLOSE( socket );
	int h = gpfSOC_CLOSE( socket );
	//socket = INVALID_SOCKET;

	return p_err;
}

static const char gp_sHELLO[] = " -= Welcome in piMASS 2019 =-\r\n    -= Writen by Dezso Bodor =-\r\n  -= more info use 'help' command =-\r\n%X>";
static const char gp_sHELLO_acc[] = "account;\r -= Welcome in piMASS 2019 =-\r\n    -= Writen by Dezso Bodor =-\r\n  -= more info use 'help' command =-\r\n%X>";
I8 gpcGT::GTlst()
{
	if( socket == INVALID_SOCKET )
	{
		struct addrinfo ainfo;
		gpmZ( ainfo );
		ainfo.ai_family = AF_INET;
		ainfo.ai_socktype = SOCK_STREAM;
		ainfo.ai_protocol = IPPROTO_TCP;
		ainfo.ai_flags = AI_PASSIVE;
		ainfo.ai_addrlen = sizeof(ainfo);
		char	*p_print = sGTpub,
				s_port[32],
				*p_err = p_print;
		sprintf( s_port, "%d", port);
		int error = getaddrinfo( NULL, s_port, &ainfo, &p_ainf );
		if( error != 0 )
			GTerr( p_err, &p_err );
		socket = ::socket( p_ainf->ai_family, p_ainf->ai_socktype, p_ainf->ai_protocol );

		p_err += sprintf( p_err, "OK" );
		GTopti( p_err, &p_err, gpdGT_NoDALEY );
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
	char *p_err = sGTpub;
	*sGTpub = 0;
	if( aGTfd[gpeFD_recv].isFD( socket ) ) // FD_ISSET( socket, &a_fdset[gpeFD_recv]) )
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
				GTclr();
				/*gpmDEL( pACC->pPUB );
				gpmDEL( pACC->pOUT );
				gpmDEL( pACC->pINP );
				gpmDEL( pACC->pDWN );*/
				//gpmMEMCPY( &pACC->addr, &clientaddr, n_clientaddr );
				U8 n_start = -1;
				if( pACC->port & 1 )// telnetek
				{
					pACC->pOUT = pACC->pOUT->lzy_format(n_start, gp_sHELLO_acc, pACC->iCNT );
					pACC->iCNT++;
					pACC->aGTcrs[0] = 'a';
					pACC->GTos(*this);
				}
				else switch( pACC->port )
				{
					case 23: // telnet
						pACC->pOUT = pACC->pOUT->lzy_format( n_start,  gp_sHELLO, pACC->iCNT );
						pACC->aGTcrs[0] = 'a';
						pACC->GTos( *this );
						break;
					case 80: // http
						pACC->aGTcrs[1] = 'h';
						break;
					default:
						pACC->pOUT = pACC->pOUT->lzy_format( n_start,  gp_sHELLO, pACC->iCNT );

				}

				pACC->GTopti( p_err, &p_err, gpdGT_NoDALEY );
				getifaddrs(&pIFadr);
				char	*pB = s_buff+gpeRECVn/2, *pBi = pB;
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
		if( p_gt ? p_gt->bGTdie : true )
			continue;

		if( aGTfd[gpeFD_recv].isFD( p_gt->socket ) ) // FD_ISSET( p_gt->socket, &a_fdset[gpeFD_recv] ) )
		{
			p_err = p_gt->GTrecv( p_err );
			if( *p_err )
				cerr << p_err << endl;

			if( p_gt )
			{
				if( p_gt->aGTcrs[1] == 'h' )
					p_gt += 0; //->gtOS_html( *this );
				else
					p_gt->GTos( *this );
			}

			if( pOUT ) /// a maimi kapott választ azaz mindenkinek leadjuk a drotot
			for( U8 p = 0, s = -1; p < GTacc.nGTld; p )
			{
				p_gt = GTacc.iGT(p);
				if( p_gt ? p_gt->bGTdie : true )
					continue;
				p_gt->pPUB = p_gt->pPUB->lzy_plus( pOUT, s );
			}
		}

		if( aGTfd[gpeFD_send].isFD( p_gt->socket ) ) // FD_ISSET( p_gt->socket, &a_fdset[gpeFD_send] ) )
		{
			U8 s;
			if(!p_gt->pOUT)
			{
				if(p_gt->pHUD ? p_gt->pHUD->p_alloc : NULL )
				{
					p_gt->pOUT = p_gt->pOUT->lzy_format( s = -1, " hud" );

					p_gt->pOUT = p_gt->pOUT->lzy_add(p_gt->pHUD, 8, s = -1);
					p_gt->pOUT = p_gt->pOUT->lzy_add(p_gt->pHUD->p_alloc, p_gt->pHUD->n, s = -1);
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

			p_err = p_gt->GTsend( p_err );
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
		if( p_gt ? (p_gt->bGTdie || p_gt->socket == INVALID_SOCKET ) : true ) {
			if( p_gt )
			if(
					aGTfd[gpeFD_send].isFD( p_gt->socket ) //FD_ISSET( p_gt->socket, &a_fdset[gpeFD_send] )
				|| 	aGTfd[gpeFD_recv].isFD( p_gt->socket )
			)
			{
				cerr << "miért?" << endl;
			} else
				gpfSOC_CLOSE( p_gt->socket );

			if( GTacc.iGTfr > nFDe )
				GTacc.iGTfr = nFDe;
			continue;
		}

		aGTfd[gpeFD_recv].setFD(p_gt->socket);
		//FD_SET( p_gt->socket, &aGTfd[gpeFD_recv] );

		// ennek az az értelme, ha nincsen buffer nem érdekklödik, hogy írható e.
		if( p_gt->pOUT || p_gt->pDWN || p_gt->pHUD )
			aGTfd[gpeFD_send].setFD(p_gt->socket);	//FD_SET( p_gt->socket, &a_fdset[gpeFD_send] );

		if( aGTfd[gpeFD_recv].nFD >= FD_SETSIZE )
			break;
	}

	aGTfd[gpeFD_recv].setFD( socket );
	U8 n_soc = aGTfd[gpeFD_recv].nFD+aGTfd[gpeFD_send].nFD;
	if( !n_soc )
	{
		usleep( 50 );
		return 0;
	}
	struct timeval tv;   // sleep for tenr minutes!
	tv.tv_sec = 0;
	tv.tv_usec = 25000;

	int nS = select(
					aGTfd[gpeFD_recv].maxSCK+1,
					&aGTfd[gpeFD_recv].fdS,
					(aGTfd[gpeFD_send].nFD ? &aGTfd[gpeFD_send].fdS : NULL),
					NULL, //(aGTfd[gpeFD_excp].fd_count ? &aGTfd[gpeFD_excp] : NULL),
					&tv
				);

	return nS;
}

