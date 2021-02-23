#include "gpcgt.h"
#include "gpcwin.h"
extern U1 gpaALFsub[];
extern char gpsTAB[], *gppTAB;
gpcLZY* gpcGT::GTos_GATELIST( gpcLZY *p_out, const char* p_enter, const char* pTAB )
{
	if( !this )
		return p_out;

	U1 s_type[0x20], *pE = s_type + gpfALF2STR( s_type, TnID.alf );
	U8 s;
	p_out = p_out->lzyFRMT(
									s = -1,
									"%s%s 0x%0.8x %s"
									" %d %s"
									" HST:%s USR:%s FIL:%s"
									" %d %d %s"
									" %d/%d%s",
									pTAB, s_type, socket,  s_ip,
									port, bGTdie() ? "die" : "live", //(socket < 0) ? "die" : "live"),
                                    sHOST, sUSER, pFILE > sFILE ? (char*)sFILE : "",
                                    mSEC.x, mSEC.y, bLOOP() ? "LP" : "-",
                                    nSYNdo, nSYNsum,
									p_enter
								);
	pTAB--;
	for( U8 a = 0, s; a < GTacc.nGTld; a++ )
		p_out = GTacc.iGT( a )->GTos_GATELIST( p_out, p_enter, pTAB );


	return p_out;
}
U8 gpcGT::GTout( gpcWIN* pWIN )
{
	if( !this )
		return 0;

	U8 nOUT = pOUT ? pOUT->n_load : 0, s, nC = 0;
	if( nOUT || TnID.alf == gpeALF_SLMP )
		return nOUT;

	// üres a pOUT töltsünk bele valamit
	U4 nMISo = gpdRECVn;
	if( !(bTEL()|bLOOP()) )
	if( (sUSER < pUSER) && (sHOST < pHOST) && (sFILE < pFILE) )
	if( msSYNwin < pWIN->msSYN )
	{
		pOUT = pWIN->pSYNwin->putSYN( pOUT, msSYNwin, socket, bSW );
		msSYNwin = pWIN->msSYN;
		nOUT = pOUT ? pOUT->n_load : 0;
		if( nOUT )
			nMISo = gpdHUDn;
	}

	if( pMISo ? pMISo->n_load : false )
	{
		if( pHUD )
			nMISo = (gpdHUDn > nOUT) ? (gpdHUDn - nOUT) : 0;
		else
			nMISo *= 0x400;

		if( nMISo )
		{
			if( nMISo > pMISo->n_load )
				nMISo = pMISo->n_load;

			pOUT = pOUT->putZN(
									pMISo->p_alloc, nMISo,			/// pD, nD,
									gpeNET4_0NYL,					/// NET4,
									pMISo->n_load,					/// Z,
									pWIN ? pWIN->mSEC.x : mSEC.x 	/// N
							);
			pMISo->lzyINS( NULL, 0, s = 0, nMISo );
		}
	}
	if( gpmLZYload(pOUT, U1) )
		return pOUT ? pOUT->n_load : 0;

	if( !gpmLZYload(pPUBgt, U1) )
		return 0;

	gpmDEL( pOUT );
	if( pOUT = pPUBgt )
	{
		pPUBgt = NULL;
		GTprmpt();
		if( pINP )
			pOUT = pOUT->lzyFRMT( s = -1, "%s", pINP->p_alloc ? (char*)pINP->p_alloc : "" );
	}
	return pOUT ? pOUT->n_load : 0;
}
void gpcGT::GTos( gpcGT& mom, gpcWIN* pWIN, gpcGTall* pALL  )
{
	if( !this )
		return;
	switch( sGTent[2] )
	{
		case 'h':	//HTML
		case 's':	//SLMP
			return;
		case 'a': 	//ACCEPT telnet
		default:
			break;
	}

	U8 nOUT = GTout( pWIN ), s;
    /*if( nOUT )
		return;*/

	U1		s_com[0x400], s_answ[0x400];
	char	s_atrib[0x400], s_prompt[0x100], s_cell[0x100];


	U1* p_str = pINP ? pINP->p_alloc : NULL;
	if( !p_str )
		return;

	if( !*p_str )
	if( pINP->n_load < sizeof(gpcSYNC) )
		return;
	else
		nSYN = ((gpcSYNC*)p_str)->nB();

	if( nSYN ? pINP->n_load < nSYN : false )
			return;

	while( nSYN )
	{
		sGTent[0] = ';';

		U1* pDAT = p_str+sizeof(gpcSYNC);
		gpcSYNC& syn = ((gpcSYNC*)pDAT)[-1];
		switch( syn.typ() )
		{
			case gpeNET4_0NYL:{
					U4* pU4 = (U4*)pDAT;
					U1* pA = (U1*)(pU4+1);
					U8 nA = syn.nB()-(pA-(U1*)&syn), s = -1;
					pMISi = pMISi->lzyADD( pA, nA, s );
				} break;

			case gpeNET4_0SYN:
				nSYNsum += syn.nS();
				for( U4 iS = 0, nS = syn.nS()-1; iS < nS; iS++ )
				{
					gpcSYNC& isyn = ((gpcSYNC*)pDAT)[iS];
					//isyn.ms = pWIN->mSEC.x; // most érkeztek
					switch( isyn.typ() )
					{
						case gpeNET4_0SYN:
							break;
						case gpeNET4_0PIC:
							if( !isyn.id )
								break;

							pSYNgt = pSYNgt->syncADD( isyn, msSYNgt );
							break;
						//case gpeNET4_0SRC:

						default:
							pSYNgt = pSYNgt->syncADD( isyn, msSYNgt );
							break;

					}
				}
				break;
			case gpeNET4_0PIC:{
					U1	*pPNT = pDAT+gpdVAN( (char*)pDAT, "." ),
						*pEND = pPNT+gpdVAN( (char*)pPNT, " \a\t;\"\'" );
					I8x2 TnID( (I8)0, (I8)(pPNT-pDAT) );
					TnID = pDAT;
					TnID.num = gpfSTR2I8( pDAT+TnID.num, NULL );
					if( !*pEND )
						pEND++;
					if( gpcPIC* pPIC = pWIN->piMASS->PIC.PIC( TnID ) )
					{
						SDL_Surface* pKILL = pPIC->pSHR;

						U4	nD = pEND-pDAT,
							nS = pEND-(U1*)&syn;

						SDL_RWops *pRW = SDL_RWFromMem( pEND, syn.nB()-nS );
						pPIC->pSHR = IMG_Load_RW( pRW, 1 );
						if( !pPIC->pSHR )
							pPIC->pSHR = pKILL;
						else
							SDL_FreeSurface(pKILL);
					}
				} break;

			case gpeNET4_0SRC:{
					U4* pZN = (U4*)pDAT;
					U1* pS = (U1*)(pZN+1);
					U4 nS = syn.nB()-(pS-(U1*)&syn);
                    I4x2 an( ((U2)*pZN+1), (*pZN)>>0x10 );
					gpcSRC	tmp,
							*pSRC = pWIN->piMASS->SRCnew( tmp, pS, an, socket, nS );
					if( !pSRC )
						break;

				} break;
			default:
				break;
		}

		if( pMISi ? pMISi->n_load >= sizeof(gpcSYNC) : false )
		{
			U4 nMISi = ((gpcSYNC*)pMISi->p_alloc)->nB();
			if( pMISi->n_load >= nMISi )
			{
				pINP = pINP->lzyINS( pMISi->p_alloc, nMISi, s = 0, nSYN );
				pMISi = pMISi->lzySUB( s = 0, nMISi );
				nSYN = 0;
			}
		}
		if( nSYN )
			pINP->lzyINS( NULL, 0, s = 0, nSYN );
		nSYN = 0;

		p_str = pINP ? pINP->p_alloc : NULL;
		if( p_str )
		{
			if( *p_str )
				break;


			if( pINP->n_load < sizeof(gpcSYNC) )
			if( pSYNgt ? pSYNgt->n_load : false )
				break;
			else
				return;

			nSYN = ((gpcSYNC*)p_str)->nB();
			if( pINP->n_load < nSYN )
				return;
		}
	}

	if( pSYNgt ? pSYNgt->n_load : false )
	{
		nSYNdo += pSYNgt->n_load/sizeof(gpcSYNC);
		for( U4 i = 0, ie = pSYNgt->n_load/sizeof(gpcSYNC); i < ie; i++ )
		{
			gpcSYNC& isyn = ((gpcSYNC*)pSYNgt->p_alloc)[i];
			/*if( isyn.ms < pWIN->mSEC.x )
				continue;*/
			switch( isyn.typ() )
			{
				case gpeNET4_0SYN:
					break;
				case gpeNET4_0PIC:
					pOUT = pOUT->lzyFRMT( s = -1, "pic 0x%x;", isyn.id );
					break;
				case gpeNET4_0SRC:
					pOUT = pOUT->lzyFRMT( s = -1, "src 0x%x;", isyn.id );
					break;
				default:
					break;
			}
		}

		//msSYNgt = pWIN->mSEC.x;
		gpmDEL(pSYNgt);
	}



	if( !p_str )
		return;

	U1	*p_sub = p_str,
		*p_null = p_str + gpmSTRLEN( p_str ),
		*pINPload = p_str + pINP->n_load;

	U1 *p_back = p_str + gpdVAN( (char*)p_str, "\b" );
	bool b_back = false;
	while( *p_back == '\b' )
	{
		b_back = !sGTent[0];
		I8	n_sub = p_sub-pINP->p_alloc,
			n_back = gpmNINCS( p_back, "\b" ),
			n_str = p_back-pINP->p_alloc;
		if( n_str <= n_back )
		{
			n_str = 0;
		} else {
			n_str -= n_back;
		}
		n_back += p_back-pINP->p_alloc;
		if( n_sub > n_str )
		{
			n_str = 0;
		}
		U8 s = n_str;
		pINP->lzyINS( NULL, 0, s, n_back );

		p_str = pINP ? pINP->p_alloc : NULL;
		if( !p_str )
		{
			// elfogyott;
			gpmDEL( pINP );
			p_sub = p_null = pINPload = p_str;
			break;
		}
		p_sub = p_str;
		p_null = p_str + gpmSTRLEN( p_str );
		pINPload = p_str+pINP->n_load;
		p_back = p_str+gpdVAN( (char*)p_str, "\b" );
	}

	if( nOSin != pINP->n_load )
	if( p_str < p_null )
	switch( p_null[-1] )
	{
		case '\r':
		case '\n':
		case ';':
			break;
		default:
			nOSin = pINP->n_load;
			if( GTprmpt( false ) )
				pOUT = pOUT->lzyFRMT( s = -1, "%s", pINP->p_alloc ? (char*)pINP->p_alloc : "" );
			break;
	}

	I8x2 cAN( gpeALF_NONSENSE );
	U1* pSKIP = NULL; U4 nSKIP = 0;
	if( p_str < p_null )
	{
		U1	*p_row = p_str+gpmNINCS( p_str, " \t;>\r\n" ),
			*p_row_end = p_row+gpdVAN( (char*)p_row, ";\r\n" ),
			*p_next = p_row_end+gpmNINCS( p_row_end, ";\r\n" ),
			*p_answ = s_answ,
			*p_com = s_com;

		p_sub = p_row;
		I8 n_com, n_row, n_cpy, n_atrib;
		if( p_row_end < p_next )
		{
			iCNT++;
			if( *p_row_end == ';' )
			{
				sGTent[0] = ';';
			} else {
				sGTent[0] = 0;
				p_next += gpdVAN( (char*)p_next, ">" );
				p_next += gpmNINCS( p_next, ">" );
			}

			p_str = p_next;
			if( p_row < p_row_end )
			{
				n_row = p_row_end-p_row; // gpdVAN( (char*)p_row, ";\r\n" );
				n_com = gpdVAN( (char*)p_row, "+-0123456789 \t;\r\n" );
				if( n_com ){
					n_cpy = gpmMIN( n_com, (sizeof(s_com)-1) );
					((U1*)gpmMcpyOF( s_com, p_row, n_cpy ))[n_cpy] = 0;
				} else
					*s_com = 0;

				n_com += gpmNINCS( p_row+n_com, " \t" );
				cAN.num = n_cpy;
				cAN = s_com;

				if( n_atrib = n_row-n_com ) {
					// jött valami atributum
					if( n_atrib > (sizeof(s_atrib)-1) )
						n_atrib = (sizeof(s_atrib)-1);

					gpmMcpyOF( s_atrib, p_row+n_com, n_atrib )[n_atrib] = 0;
				} else
					*s_atrib = 0;


				switch( cAN.alf )
				{
					case gpeALF_GSM:{
							U1* pA;
							SOCKET sockGSM = gpfSTR2U8( (U1*)s_atrib, &pA );
							gpcGT* pGT = pALL->GT( sockGSM );
							if( pGT ? pGT->TnID.alf == gpeALF_GSM : false )
							{
								if( pWIN ? pWIN->piMASS : NULL )
									pOUT = pGT->GTgsmOS( pOUT, pA, pWIN->piMASS, socket, pWIN->mSEC.x );
							} else {
								pOUT = pOUT->lzyFRMT( s = -1, "Which?\r\n" );
								for( U4 i = 0, e = pALL->nGTld; i < e; i++ )
								{
									if( !pALL->ppGTalloc[i] )
										continue;
									pOUT = pALL->ppGTalloc[i]->GTos_GATELIST( pOUT, (sGTent[0]?(char*)sGTent:"\r\n"), gppTAB );
								}
							}
						} break;
					case gpeALF_SLMP:{
							U1* pA;
							SOCKET sockSLMP = gpfSTR2U8( (U1*)s_atrib, &pA );
							gpcGT* pGT = pALL->GT( sockSLMP );
							if( pGT ? pGT->TnID.alf == gpeALF_SLMP : false )
							{
								if( pWIN ? pWIN->piMASS : NULL )
									pOUT = pGT->GTdrcOSrob( pOUT, pA, *(pWIN->piMASS), socket, pWIN->mSEC.x );
							} else {
								pOUT = pOUT->lzyFRMT( s = -1, "Which?\r\n" );
								for( U4 i = 0, e = pALL->nGTld; i < e; i++ )
								{
									if( !pALL->ppGTalloc[i] )
										continue;
									pOUT = pALL->ppGTalloc[i]->GTos_GATELIST( pOUT, (sGTent[0]?(char*)sGTent:"\r\n"), gppTAB );
								}
							}
						} break;
					/*case gpeALF_SLMPo:{
							U1* pA;
							SOCKET sockSLMP = gpfSTR2U8( (U1*)s_atrib, &pA );
							gpcGT* pGT = pALL->GT( sockSLMP );
							if( pGT ? pGT->TnID.alf == gpeALF_SLMP : false )
							{
								if( pWIN ? pWIN->piMASS : NULL )
									pOUT = pGT->GTdrcOSzs( pOUT, pA, *(pWIN->piMASS), socket, pWIN->mSEC.x );
							} else {
								pOUT = pOUT->lzyFRMT( s = -1, "Which?\r\n" );
								for( U4 i = 0, e = pALL->nGTld; i < e; i++ )
								{
									if( !pALL->ppGTalloc[i] )
										continue;
									pOUT = pALL->ppGTalloc[i]->GTos_GATELIST( pOUT, (sGTent[0]?(char*)sGTent:"\r\n"), gppTAB );
								}
							}
						} break;*/
					case gpeALF_ACCOUNT:
						// én vagyok a KLIENS,
						// a SERVER megkér, hogy azonosítsam magam
						// elküldöm a onMSEC-et és a socket számomat
						// ha a SERVER rákeres a KLIENSei között erre a
						// socket-re és az iső stimmel
						// nagy valszeg, hogy ez egy LOOP
						if( pWIN->bINI_hst_usr() ) {
							sockAT = gpfSTR2U8( (U1*)s_atrib, NULL );
							const char* pENT = (sGTent[0]?(char*)sGTent:"\r\n");
								mSEC.y = pWIN ? pWIN->mSEC.x+1 : 0;
								pOUT = pOUT->lzyFRMT(
															s = -1,	"%shost %s;"
																	"%suser %s;"
																	"%sfile %s;"
																	"%ssock 0x%x;"
																	"%smsec 0x%x;",
															pENT, pWIN->sHOST,
															pENT, pWIN->sUSER,
															pENT, pWIN->gpsMASSname,
															pENT, socket,
															pENT, mSEC.y

														);
						} break;
					case gpeALF_HOST: {
							pHOST = sHOST+sprintf( (char*)sHOST, "%s", s_atrib );
						} break;
					case gpeALF_USER: {
							pUSER = sUSER+sprintf( (char*)sUSER, "%s", s_atrib );
						} break;
					case gpeALF_FILE: {
							U4 nF = sprintf( (char*)sFILE, "%s", s_atrib );
							pFILE = sFILE+nF;

							if( nF == gpmMcmp( sFILE, pWIN->gpsMASSname, nF ) )
								bSW |= 1;
							else
								bSW &= ~1;
						} break;
					case gpeALF_SOCK: {
							mSEC.z = gpfSTR2U8( (U1*)s_atrib, NULL );
							if( gpcGT* pGT = pALL->GT( (SOCKET)mSEC.z ) ) //GTacc.GT( (SOCKET)mSEC.z ) )
							if( pGT->sockAT == socket )
							{
								mSEC.y = pGT->mSEC.y;
								bSW |= 2;
								pGT->bSW |= 2;
							}

						} break;
					case gpeALF_MSEC: {
							mSEC.x = gpfSTR2U8( (U1*)s_atrib, NULL );
						} break;


					case gpeALF_GATELIST:
						pOUT = mom.GTos_GATELIST( pOUT, (sGTent[0]?(char*)sGTent:"\r\n"), gppTAB ); //gpsTAB );
						if( !pALL )
							break;
						for( U4 i = 0, e = pALL->nGTld; i < e; i++ )
						{
							if( !pALL->ppGTalloc[i] )
								continue;
							pOUT = pALL->ppGTalloc[i]->GTos_GATELIST( pOUT, gpmGTent, gppTAB );
						}
						break;
					case gpeALF_MSG:{
							mom.pOUT = mom.pOUT->lzyFRMT( s = -1, "msg 0x%0.8x: %s%s", (U4)socket, (sGTent[0]?(char*)sGTent:"\r\n"), s_atrib );
						} break;
					case gpeALF_HELP:
						pOUT = pOUT->lzyFRMT( s = -1, "%sHELP?", gpmGTent );
						break;
					case gpeALF_EYE: {
							isEVNT.null();
							isEVNT.id = gpeNET4_0EYE;
							isEVNT.n = TnID.num;
							mom.pEVENT = mom.pEVENT->lzyADD( &isEVNT, sizeof(isEVNT), s = -1 );
							pOUT = pOUT->lzyFRMT(s = -1, "%s event", gpmGTent ); //(sGTent[0]?(char*)sGTent:"\r\n") );
						} break;



					case gpeALF_LIST: {
							U1* pS = (U1*)s_atrib;
							I8x4 an = 0;
							U8 nLEN;
							for( U1 i = 0; i < 2; i++ )
							{
								nLEN = gpmNINCS( pS, ": \t\r\n" );
								pS += nLEN;
								an.a8x2[i].num = n_atrib - (pS-(U1*)s_atrib);
								an.a8x2[i] = pS;
								pS += an.a8x2[i].num;
								an.a8x2[i].num = gpfSTR2U8( pS, &pS );
							}

							//= gpfSTR2U8( pSTR, &pSTR ), j = gpfSTR2U8( pSTR, &pSTR );
							sprintf( s_atrib, "0x%x>", iCNT );
							pMISo = pWIN->putLIST( pMISo, gpeNET4_0LST, an, s_atrib );
						} break;
					case gpeALF_MINI: {
							U1* pS = (U1*)s_atrib;
							I8x4 an = 0;
							U8 nLEN;
							for( U1 i = 0; i < 2; i++ )
							{
								nLEN = gpmNINCS( pS, ": \t\r\n" );
								pS += nLEN;
								an.a8x2[i].num = n_atrib - (pS-(U1*)s_atrib);
								an.a8x2[i] = pS;
								pS += an.a8x2[i].num;
								an.a8x2[i].num = gpfSTR2U8( pS, &pS );
							}

							//= gpfSTR2U8( pSTR, &pSTR ), j = gpfSTR2U8( pSTR, &pSTR );
							sprintf( s_atrib, "0x%x>", iCNT );
							pMISo = pWIN->putMINI( pMISo, gpeNET4_0LST, an, s_atrib );
						} break;
					case gpeALF_DBG: {
							U1* pS = (U1*)s_atrib;
							I8x4 an = 0;
							U8 nLEN;
							for( U1 i = 0; i < 2; i++ )
							{
								nLEN = gpmNINCS( pS, ": \t\r\n" );
								pS += nLEN;
								an.a8x2[i].num = n_atrib - (pS-(U1*)s_atrib);
								an.a8x2[i] = pS;
								pS += an.a8x2[i].num;
								an.a8x2[i].num = gpfSTR2U8( pS, &pS );
							}

							//= gpfSTR2U8( pSTR, &pSTR ), j = gpfSTR2U8( pSTR, &pSTR );
							sprintf( s_atrib, "0x%x>", iCNT );
							pMISo = pWIN->putDBG( pMISo, gpeNET4_0LST, an, s_atrib );
						} break;
					case gpeALF_PIC: {


							U4 	nO = pOUT ? pOUT->n_load : 0,
								iPIC = gpfSTR2U8( (U1*)s_atrib, NULL );

							if( iPIC )
							if( gpcPIC* pPIC = pWIN->piMASS->PIC.PIC( iPIC-1 ) )
                            if( pMISo ? (pMISo->n_load < pPIC->nPKavg) : true )
							{
								if( !pPIC->pPACK )
								if( SDL_Surface *pSRF = pPIC->pSRF )
								{
									if( pPIC->pFILE < pPIC->sFILE )
											pPIC->pFILE = pPIC->sFILE;
									if( !*pPIC->pFILE )
									{
										if( pPIC->pFILE <= pPIC->sFILE )
											pPIC->pFILE = pPIC->sFILE+sprintf( (char*)pPIC->sFILE, "/mnt/ram/" );
										pPIC->TnID.an2str( pPIC->pFILE, (U1*)".jpg", true );
									}

									if( !gpfSRFjpgSAVE( (U1*)"/mnt/ram/tmp.tmp", pSRF, 57 ) )
										IMG_SavePNG( pSRF, "/mnt/ram/tmp.tmp" );

									//gpmDEL( pPIC->pPACK );
									pPIC->pPACK = ((gpcLZY*)NULL)->lzyRD( "/mnt/ram/tmp.tmp", s = -1 );
									pPIC->nPKavg += pPIC->pPACK->n_load;
									pPIC->nPKavg /= 2;
									//SDL_FreeSurface(pSURF);
									if( gpfACE( (char*)pPIC->sFILE, 4) > -1 )
									{
										rename( (char*)pPIC->sFILE, "/mnt/ram/bg.kill" );
									}
									rename( "/mnt/ram/tmp.tmp", (char*)pPIC->sFILE );

									if( gpfACE("/mnt/ram/bg.kill", 4) > -1 )
									{
										remove( "/mnt/ram/bg.kill" );

									}
								}

								if( pPIC->pPACK )
								if( pPIC->pPACK->n_load > 0x400 )
									pMISo = pMISo->putPIC( pPIC->pPACK->p_alloc, pPIC->pPACK->n_load, pPIC->pFILE, pWIN->mSEC.x );
								else
									pOUT = pOUT->putPIC( pPIC->pPACK->p_alloc, pPIC->pPACK->n_load, pPIC->pFILE, pWIN->mSEC.x );

							}

							if( nO < (pOUT ? pOUT->n_load : 0) )
								nOUT = pOUT->n_load;

							nSKIP = p_next-p_row;
							if( nSKIP )
							{
								((U1*)gpmMcpyOF( s_atrib, p_row, nSKIP ))[nSKIP] = 0;
								pSKIP = (U1*)s_atrib;
							}

						} break;
					case gpeALF_SRC: {
							if( pWIN ? !pWIN->mZ : true )
								break;

							U4	nO = pOUT ? pOUT->n_load : 0,
								iZN = gpfSTR2U8( (U1*)s_atrib, NULL ), nA = 0;		// (i%win.mZ)+((i/win.mZ)<<16)

							U4x2 zn( (U2)iZN, iZN>>0x10 );
							U1* pA = NULL;

							if( zn.x < pWIN->mZ )
							if( zn.y < pWIN->mN )
							{
								U4 iM = zn*U4x2(1,pWIN->mZ);
								if( U4 xFND = pWIN->pM[iM] )
								if( gpcSRC* pSRC = pWIN->piMASS->srcFND( xFND ) )
								{
									pA = pSRC->pA;
									nA = pSRC->n_ld_add();
								}
							}

							pOUT = pOUT->putZN( pA, nA, gpeNET4_0SRC, iZN, pWIN->mSEC.x );

							if( nO < (pOUT ? pOUT->n_load : 0) )
								nOUT = pOUT->n_load;

							nSKIP = p_next-p_row;
							if( nSKIP )
							{
								((U1*)gpmMcpyOF( s_atrib, p_row, nSKIP ))[nSKIP] = 0;
								pSKIP = (U1*)s_atrib;
							}

						} break;
					case gpeALF_OK:
					case gpeALF_NONSENS:
					case gpeALF_NONSENSE:
							nSKIP = p_next-p_row;
							if( nSKIP )
							{
								((U1*)gpmMcpyOF( s_atrib, p_row, nSKIP ))[nSKIP] = 0;
								pSKIP = (U1*)s_atrib;
							}
						break;
					/*case gpeALF_PREV:
						isEVNT.null();
						isEVNT.id = gpeNET4_PREV;
						isEVNT.n = TnID.num;
						mom.pEVENT = mom.pEVENT->lzyADD( &isEVNT, sizeof(isEVNT), s = -1 );
						pOUT = pOUT->lzyFRMT(s = -1, "%s event", !*s_prompt ? "\r\n" : "");
						break;*/
					default:
						pOUT = pOUT->lzyFRMT( s = -1, "%snonsens", gpmGTent );
						break;
				}
				p_sub = p_next; // p_row_end;
			}
		}

		if( p_row_end < pINPload )
		if( !*p_row_end )
			p_sub = p_row_end;
		/*if( p_str < p_row )
		{
			p_str = p_row;
		}*/
	}

	if( pINP ? (p_sub > pINP->p_alloc) : false )
	{
		U8 s = 0, n_str;
		pINP->lzyINS( NULL, 0, s, p_sub-pINP->p_alloc );
		if( nSKIP )
		{
			n_str = gpmSTRLEN( pINP->p_alloc );
			while( nSKIP < n_str )
			{
				p_sub = (U1*)strstr( (char*)pINP->p_alloc, (char*)pSKIP );
				if( !p_sub )
					break;

				s = p_sub-pINP->p_alloc;
				pINP->lzyINS( NULL, 0, s, nSKIP );
				if( !pINP->n_load )
					break;
				n_str = gpmSTRLEN( pINP->p_alloc );
			}
		}
		if( pOUT )
		if( nOUT < pOUT->n_load )
		if( sGTent[0] )
		{
			pOUT = pOUT->lzyFRMT( s = -1, (char*)sGTent );
		} else {
			GTprmpt();
			//pOUT = pOUT->lzyFRMT( s = -1, "\b\r\n0x%x>", iCNT );
		}

	}


	if( pINP ? !pINP->n_load : true )
	{
		gpmDEL(pINP);
		return;
	}
	if( *pINP->p_alloc )
		return;
    if( pINP->n_load < sizeof(gpcSYNC) )
        return;
	gpcSYNC* pSYN = (gpcSYNC*)pINP->p_alloc;
	nSYN = pSYN->nB();

}
