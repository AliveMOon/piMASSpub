#include "gpcgt.h"
extern U1 gpaALFadd[];
extern char gpsTAB[];
gpcLAZY* gpcGT::GTos_GATELIST( gpcLAZY *p_out, const char* p_enter, const char* pTAB )
{
	if( !this )
		return p_out;

	U1 s_type[0x20], *pE = s_type + gpfALF2STR( s_type, TnID.alf );
	U8 s;
	p_out = p_out->lzy_format(
									s = -1,
									"%s%0.8x %s %s %d %s %s",
									pTAB, socket, s_type, s_ip,
									port, ((socket < 0) ? "die" : "live"),
									p_enter
								);
	pTAB--;
	for( U8 a = 0, s; a < GTacc.nGTld; a++ )
		p_out = GTacc.iGT( a )->GTos_GATELIST( p_out, p_enter, pTAB );


	return p_out;
}
void gpcGT::GTos( gpcGT& mom )
{
	if( !this )
		return;
	U1  s_com[0x200], s_answ[0x200];
	char s_atrib[0x200], s_prompt[0x100], s_cell[0x100];
	U8 nOUT = pOUT ? pOUT->n_load : 0, s, nC = 0;
	if( U1* p_str = pINP ? pINP->p_alloc : NULL )
	if( pINP->n_load ) {
		U1	*p_sub = p_str,
				*p_end = p_str+pINP->n_load;

		while( p_str < p_end && !*p_str )
			p_str++;
		p_sub = p_str;

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
				p_sub = p_end = p_str;
				break;
			}
			p_sub = p_str,
			p_end = p_str+pINP->n_load;
			p_back = p_str+gpdVAN( (char*)p_str, "\b" );
		}
		if( b_back )
		if( !aGTcrs[0] )
		{
			pOUT = pOUT->lzy_format( s = -1, "  \r%x>", socket );
			if( pINP )
				pOUT = pOUT->lzy_format( s = -1, "%s", pINP->p_alloc ? (char*)pINP->p_alloc : "" );
		}
		I8x2 cAN;
		U8 nUTF8;
		if( p_str < p_end )
		{
			U1	*p_row = (U1*)p_str+gpmNINCS( p_str, " \t;\r\n" ),
				*p_row_end = p_row+gpdVAN( (char*)p_row, ";\r\n" ),
				*p_next = p_row_end+gpmNINCS( p_row_end, ";\r\n" ),
				*p_answ = s_answ,
				*p_com = s_com,
				*p_cell;
			p_sub = p_row;
			I8 n_com, n_row, n_cpy, n_atrib;
			//I84 io_cnt;
			if( p_row_end < p_next )
			{
				*s_prompt = 0;
				iCNT++;
				if( *p_row_end != ';' )
				{
					aGTcrs[0] = 0;
					sprintf( s_prompt, "\r\n%x>", iCNT );
				} else {

					gpmSTRCPY( s_prompt, ";" );
					aGTcrs[0] = ';';
				}
				p_str = p_next;
				if( p_row < p_row_end )
				{
					n_row = gpdVAN( (char*)p_row, ";\r\n" );
					n_com = gpdVAN( (char*)p_row, "+-0123456789 \t;\r\n" );
					n_cpy = min( n_com, (sizeof(s_com)-1) );
					((U1*)gpmMEMCPY( p_com, p_row, n_cpy ))[n_cpy] = 0;
					n_com += gpmNINCS( p_row+n_com, " \t" );
					n_atrib = min( (n_row-n_com), (sizeof(s_atrib)-1) );
					if( n_atrib )
					{
						// jött valami atributum
						((char*)gpmMEMCPY( s_atrib, p_row+n_com, n_atrib ))[n_atrib] = 0;

					}
					cAN.num = gpfABCnincs( p_row, p_row_end, nUTF8, gpaALFadd );
					cAN = (U1*)p_row; //cAN.an( p_row );
					switch( cAN.alf )
					{
						case gpeALF_GATELIST:
							pOUT = mom.GTos_GATELIST( pOUT, "\r\n", gpsTAB );
							break;
						case gpeALF_HELP:
							pOUT = pOUT->lzy_format( s = -1, "%sHELP?", !*s_prompt ? "\r\n":"" );
							break;
						case gpeALF_EYE:
							isEVNT.null();
							isEVNT.id = gpeNET4_0EYE;
							isEVNT.n = TnID.num;
							mom.pEVNT = mom.pEVNT->lzy_add( &isEVNT, sizeof(isEVNT), s = -1 );
							pOUT = pOUT->lzy_format(s = -1, "%s event", !*s_prompt ? "\r\n" : "");
							break;
						case gpeALF_PREV:
							isEVNT.null();
							isEVNT.id = gpeNET4_PREV;
							isEVNT.n = TnID.num;
							mom.pEVNT = mom.pEVNT->lzy_add( &isEVNT, sizeof(isEVNT), s = -1 );
							pOUT = pOUT->lzy_format(s = -1, "%s event", !*s_prompt ? "\r\n" : "");
							break;
						default:
							pOUT = pOUT->lzy_format( s = -1, "%snonsens", !*s_prompt ? "\r\n":"" );
					}
					p_sub = p_next; // p_row_end;
				}

			}
			if( p_str < p_row )
			{
				p_str = p_row;
			}
		}
		if( pINP ? (p_sub > pINP->p_alloc) : false )
		{
			U8 s = 0;
			pINP->lzy_ins( NULL, 0, s, p_sub-pINP->p_alloc );
			if( pOUT )
			if( nOUT < pOUT->n_alloc )
			{
				if( aGTcrs[0] )
				{
					pOUT = pOUT->lzy_format( s = -1, ";" );
				} else {
					pOUT = pOUT->lzy_format( s = -1, "\b\r\n%x>", iCNT );
				}
			}
		}
	}

	if( pINP )
	if( !pINP->n_load )
		gpmDEL(pINP);

}
