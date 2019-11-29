#include "gpcgt.h"
#include "gpcwin.h"
extern U1 gpaALFadd[];
extern char gpsTAB[], *gppTAB;
gpcLAZY* gpcGT::GTos_GATELIST( gpcLAZY *p_out, const char* p_enter, const char* pTAB )
{
	if( !this )
		return p_out;

	U1 s_type[0x20], *pE = s_type + gpfALF2STR( s_type, TnID.alf );
	U8 s;
	p_out = p_out->lzyFRMT(
									s = -1,
									"%s%0.8x %s %s %d %s %s %s %d %d %s",
									pTAB, socket, s_type, s_ip,
									port, ((socket < 0) ? "die" : "live"),
                                    sHOST, sUSER, mSEC.x, mSEC.y,
									p_enter
								);
	pTAB--;
	for( U8 a = 0, s; a < GTacc.nGTld; a++ )
		p_out = GTacc.iGT( a )->GTos_GATELIST( p_out, p_enter, pTAB );


	return p_out;
}
void gpcGT::GTos( gpcGT& mom, gpcWIN* pWIN  )
{
	if( !this )
		return;

	U1		s_com[0x200], s_answ[0x200];
	char	s_atrib[0x200], s_prompt[0x100], s_cell[0x100];
	U8 nOUT = pOUT ? pOUT->n_load : 0, s, nC = 0;

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
		U1* pDAT = p_str+sizeof(gpcSYNC);
		gpcSYNC& syn = ((gpcSYNC*)pDAT)[-1];
		switch( syn.typ() )
		{
			case gpeNET4_0SYN:
				for( U4 iS = 0, nS = syn.nS()-1; iS < nS; iS++ )
				{
					gpcSYNC& isyn = ((gpcSYNC*)pDAT)[iS];
					switch( isyn.typ() )
					{
						case gpeNET4_0SYN:
							break;
						case gpeNET4_0PIC:
							if( !isyn.id )
								break;

							pSYNgt = pSYNgt->syncADD( isyn, msSYNgt );
							break;
						default:
							break;
					}
				}
				break;
			case gpeNET4_0PIC:{
					U1	*pPNT = pDAT+gpdVAN( (char*)pDAT, "." ),
						*pEND = pPNT+gpdVAN( (char*)pPNT, " \a\t;\"\'" );
					I8x2 TnID( 0, pPNT-pDAT );
					TnID = pDAT;
					TnID.num = gpfSTR2I8( pDAT+TnID.num, NULL );
					if( gpcPIC* pPIC = pWIN->piMASS->PIC.PIC( TnID ) )
					{
						SDL_Surface* pKILL = pPIC->pSHR;
						SDL_RWops *pRW = SDL_RWFromMem( pEND+1, syn.nB()-((pEND+1)-pDAT) );
						pPIC->pSHR = IMG_Load_RW( pRW, 1 );
						if( !pPIC->pSHR )
							pPIC->pSHR = pKILL;
						else
							SDL_FreeSurface(pKILL);
					}
				} break;
			default:
				break;
		}

		pINP->lzy_ins( NULL, 0, s = 0, nSYN );
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
		}
	}

	if( pSYNgt ? pSYNgt->n_load : false )
	{
		for( U4 i = 0, ie = pSYNgt->n_load/sizeof(gpcSYNC); i < ie; i++ )
		{
			gpcSYNC& isyn = ((gpcSYNC*)pSYNgt->p_alloc)[i];
			switch( isyn.typ() )
			{
				case gpeNET4_0SYN:
					break;
				case gpeNET4_0PIC:
					pOUT = pOUT->lzyFRMT( s = -1, "pic 0x%x;", isyn.id );
					break;
				default:
					break;
			}
		}
		gpmDEL(pSYNgt);
	}

	if( !p_str )
	{

		return;
	}

	U1	*p_sub = p_str + gpmSTRLEN( p_str ),
		*pINPload = p_str + pINP->n_load;

	U1 *p_back = p_str + gpdVAN( (char*)p_str, "\b" );
	bool b_back = false;
	while( *p_back == '\b' )
	{
		b_back = !aGTcrs[0];
		I8	n_sub = p_sub-pINP->p_alloc,
			n_back = gpmNINCS( p_back, "\b" ), n_str = p_back-pINP->p_alloc;
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
		pINP->lzy_ins( NULL, 0, s, n_back );

		p_str = pINP ? pINP->p_alloc : NULL;
		if( !p_str )
		{
			// elfogyott;
			gpmDEL( pINP );
			p_sub = pINPload = p_str;
			break;
		}
		p_sub = p_str,
		pINPload = p_str+pINP->n_load;
		p_back = p_str+gpdVAN( (char*)p_str, "\b" );
	}
	if( b_back )
	if( !aGTcrs[0] )
	{
		pOUT = pOUT->lzyFRMT( s = -1, "  \r0x%x>", iCNT );
		if( pINP )
			pOUT = pOUT->lzyFRMT( s = -1, "%s", pINP->p_alloc ? (char*)pINP->p_alloc : "" );
	}

	I8x2 cAN;
	if( p_str < p_sub )
	{
		U1	*p_row = p_str+gpmNINCS( p_str, " \t;>\r\n" ),
			*p_row_end = p_row+gpdVAN( (char*)p_row, ";\r\n" ),
			*p_next = p_row_end+gpmNINCS( p_row_end, ";\r\n" ),
			*p_answ = s_answ,
			*p_com = s_com;
			//*p_cell;

		p_sub = p_row;
		I8 n_com, n_row, n_cpy, n_atrib;
		//I84 io_cnt;
		if( p_row_end < p_next )
		{
			iCNT++;
			if( *p_row_end == ';' )
			{
				aGTcrs[0] = ';';
			} else {
				aGTcrs[0] = 0;
				p_next += gpdVAN( (char*)p_next, ">" );
				p_next += gpmNINCS( p_next, ">" );
			}

			p_str = p_next;
			if( p_row < p_row_end )
			{
				n_row = p_row_end-p_row; // gpdVAN( (char*)p_row, ";\r\n" );
				n_com = gpdVAN( (char*)p_row, "+-0123456789 \t;\r\n" );

				n_cpy = min( n_com, (sizeof(s_com)-1) );
				((U1*)gpmMEMCPY( s_com, p_row, n_cpy ))[n_cpy] = 0;

				n_com += gpmNINCS( p_row+n_com, " \t" );
				n_atrib = min( (n_row-n_com), (sizeof(s_atrib)-1) );
				if( n_atrib )
				{
					// jött valami atributum
					((char*)gpmMEMCPY( s_atrib, p_row+n_com, n_atrib ))[n_atrib] = 0;

				}

				cAN.num = n_cpy;
				cAN = s_com;

				switch( cAN.alf )
				{
					case gpeALF_ACCOUNT: if( pWIN->bINIThu() ) {
							pOUT = pOUT->lzyFRMT(
														s = -1,	"%shost %s;"
																"%suser %s;"
																"%smsec 0x%x;",
														aGTcrs[0] ? "" : "\r\n", pWIN->sHOST,
														aGTcrs[0] ? "" : "\r\n", pWIN->sUSER,
														aGTcrs[0] ? "" : "\r\n", pWIN->mSEC.x
													);
						} break;
					case gpeALF_USER: {
							pUSER = sUSER+sprintf( (char*)sUSER, "%s", s_atrib );
						} break;
					case gpeALF_HOST: {
							pHOST = sHOST+sprintf( (char*)sHOST, "%s", s_atrib );
						} break;
					case gpeALF_MSEC: if( pWIN ) {
							mSEC.y = pWIN ? pWIN->mSEC.x : 0;
							mSEC.x = gpfSTR2U8( (U1*)s_atrib, NULL );
						} break;
					case gpeALF_GATELIST:
						pOUT = mom.GTos_GATELIST( pOUT, "\r\n", gppTAB ); //gpsTAB );
						break;
					case gpeALF_HELP:
						pOUT = pOUT->lzyFRMT( s = -1, "%sHELP?", aGTcrs[0] ? "" : "\r\n" );
						break;
					case gpeALF_EYE: {
							isEVNT.null();
							isEVNT.id = gpeNET4_0EYE;
							isEVNT.n = TnID.num;
							mom.pEVENT = mom.pEVENT->lzyADD( &isEVNT, sizeof(isEVNT), s = -1 );
							pOUT = pOUT->lzyFRMT(s = -1, "%s event", aGTcrs[0] ? "" : "\r\n" );
						} break;
					case gpeALF_PIC: {
							U4 iPIC = gpfSTR2U8( (U1*)s_atrib, NULL );
							if( iPIC )
							if( gpcPIC* pPIC = pWIN->piMASS->PIC.PIC( iPIC-1 ) )
							if( SDL_Surface *pSRF = pPIC->pSRF )
							{
								if( !gpfSRFjpgSAVE( (U1*)"/mnt/ram/tmp.tmp", pSRF, 57 ) )
									IMG_SavePNG( pSRF, "/mnt/ram/tmp.tmp" );
								gpcLAZY* pPNG = ((gpcLAZY*)NULL)->lzyRD( "/mnt/ram/tmp.tmp", s = -1 );
								if( pPNG )
								{
									//nGD++;
									if( pPIC->pFILE < pPIC->sFILE )
										pPIC->pFILE = pPIC->sFILE;
									if( !*pPIC->pFILE )
									{
										pPIC->TnID.an2str( pPIC->pFILE, (U1*)".jpg", true );
									}
									pOUT = pOUT->putPIC( pPNG->p_alloc, pPNG->n_load, pPIC->pFILE, pWIN->mSEC.x );
									gpmDEL(pPNG);
								}
								//SDL_FreeSurface(pSURF);
								if( gpfACE( gpdPICbg, 4) > -1 )
								{
									rename( gpdPICbg, "/mnt/ram/bg.kill" );
								}
								rename( "/mnt/ram/tmp.tmp", gpdPICbg );

								if( gpfACE("/mnt/ram/bg.kill", 4) > -1 )
								{
									remove( "/mnt/ram/bg.kill" );

								}
							}
						} break;
					case gpeALF_OK:
					case gpeALF_NONSENS:
					case gpeALF_NONSENSE: {

						} break;
					/*case gpeALF_PREV:
						isEVNT.null();
						isEVNT.id = gpeNET4_PREV;
						isEVNT.n = TnID.num;
						mom.pEVENT = mom.pEVENT->lzyADD( &isEVNT, sizeof(isEVNT), s = -1 );
						pOUT = pOUT->lzyFRMT(s = -1, "%s event", !*s_prompt ? "\r\n" : "");
						break;*/
					default:
						pOUT = pOUT->lzyFRMT( s = -1, "%snonsens", aGTcrs[0] ? "" : "\r\n" );
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
		U8 s = 0;
		pINP->lzy_ins( NULL, 0, s, p_sub-pINP->p_alloc );
		if( pOUT )
		if( nOUT < pOUT->n_load )
		if( aGTcrs[0] )
		{
			pOUT = pOUT->lzyFRMT( s = -1, ";" );
		} else {
			pOUT = pOUT->lzyFRMT( s = -1, "\b\r\n0x%x>", iCNT );
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
