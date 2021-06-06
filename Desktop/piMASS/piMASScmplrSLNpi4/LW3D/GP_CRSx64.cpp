#include "gpstd_x64.h"
void GPC_TAB::tab_enter( GPC_CRS& crs )
{
	if( !b_left )
		return;
	if( this == crs.crs_p_root )
		return;
	
	this[-1].b_right = b_left  = false;
	
	if( tab_p_up )
	{
		if( tab_p_up->tab_p_down == this )
			tab_p_up->tab_p_down = NULL;
		else 
			tab_p_up = NULL;
	}
	
	if( crs.crs_p_AN == this )
	{
		tab_p_up = this-1;
		while( tab_p_up->b_left )
			tab_p_up--;
	} else
		tab_p_up = crs.crs_p_AN;

	tab_p_up->tab_p_down = crs.crs_p_AN = this;
	
}
GPC_TAB* GPC_TAB::tab_insert( GPC_CRS& crs, char* p_src )
{
	ULL	n_load = crs.crs_n_load + 1,
		n_old = crs.crs_n_alloc;

	GPC_TAB* p_old = crs.crs_p_root;
	if( n_old <= n_load)
	{
		// PRE COPY
		crs.crs_n_alloc += 0x10;
		crs.crs_p_root = new GPC_TAB[crs.crs_n_alloc];
		ULL n_cpy = this + 1 - p_old;
		gp_memcpy( crs.crs_p_root, p_old, n_cpy*sizeof(*p_old) );
		if( crs.crs_p_tab_sel )
		{
			crs.crs_p_tab_sel = crs.crs_p_root+(crs.crs_p_tab_sel-p_old);
		}
	}
	GPC_TAB	*p_post_srce = crs.crs_p_end,
		*p_post_src = this + 1,
		*p_post_dste = crs.crs_p_root + (crs.crs_p_end-p_old) + 1;

	// POST COPY (BACK)
	while (p_post_srce > p_post_src)
	{
		p_post_srce--;
		p_post_dste--;

		*p_post_dste = *p_post_srce;
	}

	GPC_TAB	*p_new = p_post_dste-1,
		*p_this = p_new-1;
	
	// NEW INIT&LOAD
	gp_zero(p_new, sizeof(*p_new));
	if( GPC_FIX* p_src_fx = p_this->tab_p_src )
	{
		// NEW
		ULL n_s = 0, n_new = p_src_fx->fix_p_alloc+p_src_fx->fix_n_alloc - (p_src+1);
		p_new->tab_p_src = p_new->tab_p_src->fix_insert(p_src+1, n_new, n_s, 0);
		// OLD
		n_s = p_src-p_src_fx->fix_p_alloc;
		p_src_fx->fix_insert(NULL, 0, n_s, p_src_fx->fix_n_alloc);

	}
	
	if( p_old != crs.crs_p_root )
	{
		for (GPC_TAB* p_s = crs.crs_p_root; p_s < p_this + 1; p_s++)
		{
			if (p_s->tab_p_up)
				p_s->tab_p_up = crs.crs_p_root + (p_s->tab_p_up - p_old);
			if (p_s->tab_p_down)
				p_s->tab_p_down = crs.crs_p_root + (p_s->tab_p_down - p_old);
		}
		gp_zero(p_old, crs.crs_n_load*sizeof(*p_old));
		crs.crs_p_edit = crs.crs_p_root + (crs.crs_p_edit - p_old);
		crs.crs_p_AN = crs.crs_p_root + (crs.crs_p_AN - p_old);
		GPM_DELARRAY(p_old);

	}
	crs.crs_n_load = n_load;
	crs.crs_p_end = crs.crs_p_root + crs.crs_n_load;
	
	return p_this;
}


void GPC_TAB::tab_size( GPC_CRS& crs )
{
	if (crs.crs_p_tab_sel != this)
	if (switch64 & GPE_SW64_NOO)
	{
		tab_y = tab_x = 3;
		return;
	}
	else if( switch64&GPE_SW64_NO )
	{
		tab_y = tab_x = 1;
		return;
	}
	char	*p_end,
		*p_str = tab_str(&crs, crs.crs_p1, &p_end );
		
	int t, n = GPF_STR_len(p_str, p_end, tab_x, tab_y, t );
	if( n != p_end - p_str )
	{
		// valami nem stimmel?
		p_end = p_str + n;
	}
	tab_x += 1;

	if( tab_x < 9 )
		tab_x = 9;
	
	GPC_TAB* p_u = tab_p_up;
	while (p_u)
	{
		if( p_u->tab_x == tab_x )
			break;
		else if( p_u->tab_x > tab_x )
		{
			tab_x = p_u->tab_x;
			break;
		}

		p_u->tab_x = tab_x;
		p_u = p_u->tab_p_up;
	}
	
	if( !b_left )
		return;
	// az a lényeg, minden tab a legelsõ elembe rögzití a magasságát, ha nagyobb
	

	if( crs.crs_pp_row[tab_an.n] )
	if( crs.crs_pp_row[tab_an.n]->tab_y < tab_y )
	{
		crs.crs_pp_row[tab_an.n]->tab_y = tab_y;
	} else {
		tab_y = crs.crs_pp_row[tab_an.n]->tab_y;
	}
}

char* GPC_TAB::tab_str( GPC_CRS* p_crs, ULL pos, char** pp_end )
{
	char	*p_str = NULL,
		*p_end = p_str;
	bool b_edit = (p_crs->crs_p_tab_sel == this);

	if( !b_edit )
	if(tab_p_mini)
	if(tab_p_mini->fix_p_alloc)
	{
		p_str = tab_p_mini->fix_p_alloc;
		p_end = p_str + tab_p_mini->fix_n_alloc;
	}

	if (!p_str)
	{
		tab_src( p_crs );

		if( tab_p_src )
		if( tab_p_src->fix_p_alloc )
		{
			p_str = tab_p_src->fix_p_alloc;
			p_end = p_str + tab_p_src->fix_n_alloc;
			if( b_edit ?  pos >= tab_txt : true )
				p_str += tab_txt;
		}
	}

	if( pp_end )
		*pp_end = p_end;

	return p_str;
}
void GPC_TAB::tab_clear(void)
{
	GPM_DEL(tab_p_src);
	GPM_DEL(tab_p_exe);
	GPM_DEL(tab_p_mini);

	GPM_DEL(tab_p_in);
	GPM_DEL(tab_p_res);
	
}
bool GPC_TAB::tab_tab(GPC_CRS& crs)
{
	GPC_TAB* p_tab = this;
	if( !p_tab->tab_p_src )
	if( p_tab->tab_p_res )
	{
		if( p_tab->tab_an.a == GPE_ALFA_A )
			p_tab->b_left = false;
		p_tab->tab_p_src = p_tab->tab_src( &crs );
	} else {
		if( p_tab->tab_an.a == GPE_ALFA_A )
			p_tab->b_left = false;
		p_tab->tab_p_src = p_tab->tab_src( &crs );
	}

tab_again:
	if( p_tab >= crs.crs_p_end )
		return false;
	
	if( p_tab->tab_p_src ? !p_tab->tab_p_src->fix_p_alloc : true )
	{
		
		// KILL
		// ki lett nyirva ?
		if( !p_tab->tab_p_src )
		{
			if( crs.crs_n_load < 2)
			{
				ULL s = 0;
				p_tab->tab_p_src = p_tab->tab_p_src->fix_insert( " \a ", 3, s, 0 );
				crs.crs_n_load = 1;
				crs.crs_p_end = crs.crs_p_root + crs.crs_n_load;
				goto tab_again;
			}
			p_tab->tab_clear();
			GPC_TAB* p_t = p_tab;
			for (GPC_TAB*p_e = crs.crs_p_root + crs.crs_n_load - 1; p_t < p_e; p_t++)
			{
				p_t[0] = p_t[1];
			}
			memset(p_t, 0, sizeof(*p_t));
			if (p_tab > crs.crs_p_root)
				p_tab--;

			crs.crs_n_load--;
			crs.crs_p_end = crs.crs_p_root + crs.crs_n_load-1;
			goto tab_again;
		}
	}

	if( p_tab != crs.crs_p_root )
	{
		GPC_TAB	*p_back = p_tab - 1,
			*p_up;

		p_back->b_right = p_tab->b_left = true;
		
		if( p_back->tab_p_up )
		if( p_back->tab_p_up->b_right )
		{
			p_tab->tab_p_up = p_back->tab_p_up+1;
			p_tab->tab_p_up->tab_p_down = p_tab;
		}
	}
	p_tab->b_right = p_tab != crs.crs_p_end-1;
	p_tab->tab_p_down = NULL;
	
	char	*p_begin = p_tab->tab_p_src->fix_p_alloc,
		*p_src = p_begin,
		*p_end = p_begin + p_tab->tab_p_src->fix_n_alloc;

	while( *p_src != 7 && p_src < p_end )
	{
		p_src++;
	}
	if( *p_src == 7 )
	{
		p_src++;
		// store_txt
		p_tab->tab_txt = p_src - p_begin;

		p_src += GPD_VAN( p_src, "\a" );
		if (*p_src == 7)
		{
			//hozzá kell adni egy új tabot a rendszerhez
			p_tab = p_tab->tab_insert(crs, p_src);
			p_tab->tab_ver = crs.crs_ver + 1;
			p_tab++;
			p_tab->tab_ver = crs.crs_ver + 1;
			p_tab--;
			//crs.crs_p_AN = p_tab;
			//while( crs.crs_p_AN->tab_p_left )
			//	crs.crs_p_AN = crs.crs_p_AN->tab_p_left;
			goto tab_again;
		}
	}
	else if( p_tab > crs.crs_p_root )
	{
		// nem volt benne
		// azaz hozzá kell csapni az elözõhöz
		// és kivenni ezt balra shiftelni
		GPC_TAB* p_back = p_tab - 1;
		ULL s = -1;
		p_back->tab_p_src = p_back->tab_p_src->fix_insert( p_begin, GPD_STRLEN(p_begin), s, 0 );
		p_tab->tab_clear();
		GPC_TAB* p_t = p_tab;
		crs.crs_n_load--;
		crs.crs_p_end--;
		for( GPC_TAB* p_e = crs.crs_p_end; p_t < p_e; p_t++ )
		{
			p_t[0] = p_t[1];
			if( p_t[0].tab_p_up )
			{
				p_t[0].tab_p_up->tab_p_down = NULL;
			}
			p_t[0].tab_p_up = p_t[0].tab_p_down = NULL;
			p_t[0].b_left = true;
			p_t[0].b_right = p_t < crs.crs_p_end-1;
			p_t[0].tab_ver = crs.crs_ver + 1;
		}

		memset(p_t, 0, sizeof(*p_t));
		crs.crs_p_edit = p_tab = p_back;
		if( crs.crs_p_tab_sel )
			crs.crs_p_tab_sel--;
		//else
		//	crs.crs_p_tab_sel = NULL;
		crs.crs_p1 = p_tab->tab_txt;
		if( p_tab->tab_p_up )
		{
			p_tab->tab_p_up->tab_p_down = NULL;
			p_tab->tab_p_up = NULL;
		}
		p_tab->tab_p_down = NULL;
		p_tab->b_right = p_tab < crs.crs_p_end-1;
		p_tab->tab_ver = crs.crs_ver + 1;
		goto tab_again;
	}
	
	if( crs.crs_p_AN > p_tab )
	{
		crs.crs_p_AN = p_tab;
	}
	p_begin = p_tab->tab_p_src->fix_p_alloc;
	p_end = p_begin + p_tab->tab_txt-1;

	// dolgozzuk fel itt a setup-ot,
	// a IN-be kell kapcsolokat paramétereket csinálni
	I8 d0 = GPF_ALFA_n( p_begin, p_end, GPE_ALFA_ENTER );
	if( d0 > 0 )
		p_tab->tab_enter(crs);
	//bool b_enter = false;
	//while( p_begin < p_end )
	//{
	//	p_begin += GPD_VAN(p_begin, "\aABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
	//	d0 = GPF_STR2ALFA(p_begin, p_end, &p_begin);
	//	switch (d0)
	//	{
	//		case GPE_ALFA_ENTER:
	//			p_tab->tab_enter(crs);
	//	}
	//}

	if( crs.crs_p_AN == p_tab && p_tab->b_left )
	while( crs.crs_p_AN->b_left )
		crs.crs_p_AN--;

	if (p_tab->tab_ver <= crs.crs_ver)
		return false;

	GPM_DEL(p_tab->tab_p_exe);
	GPM_DEL(p_tab->tab_p_in);
	return true;
}
GPC_CRS::~GPC_CRS( void )
{

	GPC_TAB* p_loop = crs_p_root;
	while( p_loop < crs_p_end )
	{
		GPM_DEL( p_loop->tab_p_src );
		GPM_DEL( p_loop->tab_p_exe );
		GPM_DEL( p_loop->tab_p_mini );
		GPM_DEL( p_loop->tab_p_compi );
		
		GPM_DEL( p_loop->tab_p_res );
		GPM_DEL( p_loop->tab_p_mini );
		p_loop++;
	}

	GPM_DELARRAY( crs_p_root );
	*crs_p_path = 0;
}
GPC_CRS::GPC_CRS( GPC_WBx64* p_wb, char* p_file, char* p_file_e, GPC_CRS* p_mom )
{
	GPM_CLR;
	if( p_file ? !*p_file : true )
		p_file = "no_name.sht";
	//crs_p_pic_e = crs_p_pic = NULL;

	crs_p_cam_e = crs_p_cam = crs_s_empty;
	crs_p_cam_e += sprintf( crs_p_cam_e, "empty!" );
	memset( this, 0, this->crs_s_file - (char*)this );
	crs_p_wb = p_wb;
	mom_p_crs = p_mom;

	crs_p_path = crs_s_path;
	*crs_p_path = 0;

	if( mom_p_crs )
	{
		GPM_STRCPY( crs_s_file, mom_p_crs->crs_s_path );
		crs_p_file = crs_s_file + (mom_p_crs->crs_p_path - mom_p_crs->crs_s_path);
	}
	
	p_file += GPD_NINCS( p_file, "\" \t\r\n" );
	if( p_file_e <= p_file )
	{
		p_file_e = p_file + GPD_VAN( p_file, "\" \t\r\n" );
		I8 step;
		while( *p_file_e == ' ' )
		{
			step = GPD_VAN( p_file_e, "\"\t\r\n\\" );
			if( p_file_e[step] != '\\' )
			{
				// nincs több '\'
				break;
			}
			p_file_e += step;
			p_file_e += GPD_NINCS( p_file_e, " " );
			p_file_e += GPD_VAN( p_file_e, "\" \t\r\n" );
		}
	}
	if( p_file ? *p_file : false )
	{
		
		if( p_file < p_file_e )
		{
			char *p_sw = p_file+GPD_VAN( p_file, "!:\\" );
			switch( *p_sw )
			{
				case '!':
					crs_p_file = crs_s_file;
					*crs_p_file = 0;
					return;
				case ':':
					p_file = p_sw-1;
					(gp_strncpy( crs_p_file = crs_s_file, p_file, p_file_e-p_file ))[ p_file_e-p_file] = 0;
					crs_p_file += GPD_VISZVAN( crs_s_file, NULL, "\\:" );
					if( *crs_p_file == '\\' )
					{
						crs_p_file++;
					
						GPM_STRCPY( crs_s_path, crs_s_file );
						crs_p_path = crs_s_path+(crs_p_file-crs_s_file);
						return;
					}

					// ez akkor fordul elõ ha van : nincsen '\\' rossz a path 
					// kikapcsoljuk a mentést
					
					crs_p_file = crs_s_file;
					*crs_p_file = 0;
					return;
				case '\\':
					p_file++;
				case 0:
				default:
					if( !crs_p_file )
					{
						GetCurrentDirectoryA( sizeof(crs_s_file), crs_s_file );
						crs_p_file = crs_s_file+GPD_STRLEN(crs_s_file);
						crs_p_file += sprintf( crs_p_file, "\\" );
					}
					(gp_strncpy( crs_p_file, p_file, p_file_e-p_file ))[ p_file_e-p_file] = 0;
					crs_p_file = crs_s_file+GPD_VISZVAN( crs_s_file, NULL, "\\:" );
					if( *crs_p_file == '\\' )
					{
						crs_p_file++;
					
						GPM_STRCPY( crs_s_path, crs_s_file );
						crs_p_path = crs_s_path+(crs_p_file-crs_s_file);
						return;
					}
			}
		}
	}
	


	if( !p_file_e )
		p_file_e = p_file+GPD_STRLEN(p_file);

	if( p_file_e <= p_file )
	{
		p_file += GPD_VAN(p_file, "\"");
	}
	
	GPM_STRCPY(crs_s_file, p_file);
	
	if( (p_file = strrchr(crs_s_file, '\"')) )
		*p_file = 0;
	

	if( (crs_p_file = strrchr(crs_s_file, '\\')) )
		crs_p_file++;

	GPM_STRCPY( crs_s_path, crs_s_file );
	crs_p_path = crs_s_path+(crs_p_file-crs_s_file);

	
		
}
void GPC_CRS::crs_undo_record( void )
{
	// ! Nem akció után, hanem elötte kell használni !
	if( !this || !crs_p_tab_sel )
		return;
	
	GPC_FIX* p_fix = NULL;
	p_fix = crs_p_tab_sel->tab_src( this );

	if( !p_fix )
		return;
	
	GPC_UNDO* p_u = NULL;
	if( p_undo > a_undo )
	if( p_undo[-1].an == crs_p_tab_sel->tab_an )
	{
		if( p_undo[-1].p_str )
		if( !strcmp( p_fix->fix_p_alloc, p_undo[-1].p_str ) )
		{
			return;
		}
	}
				
	if( !p_undo->p_str )
		p_undo->n_str = 0;

	I8	n_diff = p_fix->fix_n_alloc-p_undo->n_str;

	if( n_diff > 0 || n_diff < -0x20 )
	{
		GPM_DELARRAY( p_undo->p_str );
		p_undo->p_str = new char[(p_undo->n_str = p_fix->fix_n_alloc)+1];
	}

	if( !p_undo->p_str )
		return;
	
	GPM_STRCPY( p_undo->p_str, p_fix->fix_p_alloc );
	p_undo->an = crs_p_tab_sel->tab_an;
	p_undo->pos = crs_p1;
	p_undo++;

	if( p_undo-a_undo >= GPD_CRS_NUNDO/2 )
	{
for( GPC_UNDO* p_u = a_undo, *p_e = p_u+GPD_CRS_NUNDO/4; p_u < p_e; p_u++ )
		{
			GPM_DELARRAY( p_u->p_str );
			p_u->n_str = 0;
		}
		gp_memcpy( a_undo, a_undo+GPD_CRS_NUNDO/4, GPD_CRS_NUNDO/4*sizeof(*a_undo) );
		memset( a_undo+GPD_CRS_NUNDO/4, 0, GPD_CRS_NUNDO/4*sizeof(*a_undo) );
		p_undo -= GPD_CRS_NUNDO/4;
	}

	p_redo = p_undo;
}
void GPC_CRS::crs_redo_record( void )
{
	// ! Nem akció után, hanem elötte kell használni !
	if( !this || !crs_p_tab_sel )
		return;
	if( p_redo == a_undo )
		return;

	GPC_FIX* p_fix = NULL;
	p_fix = crs_p_tab_sel->tab_src( this );

	if( !p_fix )
		return;
				
	if( !p_redo->p_str )
		p_redo->n_str = 0;

	I8	n_diff = p_fix->fix_n_alloc-p_redo->n_str;

	if( n_diff > 0 || n_diff < -0x20 )
	{
		GPM_DELARRAY( p_redo->p_str );
		p_redo->p_str = new char[(p_redo->n_str = p_fix->fix_n_alloc)+1];
	}

	if( !p_redo->p_str )
		return;
	
	GPM_STRCPY( p_redo->p_str, p_fix->fix_p_alloc );
	p_redo->an = crs_p_tab_sel->tab_an;
	p_redo->pos = crs_p1;
}	
void GPC_CRS::crs_init( GPC_TAB* p_tab_mom )
{

	if( crs_n_load )
		return;
	
	p_undo = p_redo = a_undo;
	if(!crs_n_alloc)
	{
		crs_n_alloc = 0x10;
		crs_n_load = 0;
	}

	if (!crs_p_root)
	{
		crs_p_root = (GPC_TAB*)memset(new GPC_TAB[crs_n_alloc], 0, crs_n_alloc*sizeof(*crs_p_root));
	}

	crs_n_load = 1;
	ULL	s = -1;
	crs_p_root->tab_ver = 1;
	crs_p_root->tab_p_src = crs_p_root->tab_p_src->fix_read(crs_s_file, s, 0);
	if( !crs_p_root->tab_p_src )
	{
		char s_buff[0x100], *p_str = s_buff;
		p_str += sprintf( p_str, "\a\n\t\t- Welcome in GP_Rules -\n\t\t\t- -- X64&DX11 -- -\n\t\t\t\t- 2014 -\n\npath:\n%s\nREADY\n", crs_s_file );
		ULL s = -1;
		crs_p_root->tab_p_src = crs_p_root->tab_p_src->fix_insert( s_buff, p_str-s_buff, s, 0 );
		crs_p_tab_sel = crs_p_root;
		crs_p1 = crs_p_root->tab_p_src->fix_n_alloc;
	}
	else if (*crs_p_root->tab_p_src->fix_p_alloc == '\a')
	{
		s = 0;
		crs_p_root->tab_p_src->fix_expand(s, 1, 0);
	}
	crs_p_edit = crs_p_root;
	crs_p_end = crs_p_edit + crs_n_load;
	
	if( !mom_p_crs )
	{
		// a szülõ õ a dx tõl kapja a win méreteket
		crs_p_wb->GP_DX11_ReSize(-1);
		return;
	}

	crs_win.z = crs_win.x = p_tab_mom->tab_l*crs_p_wb->mper2_x;
	crs_win.w = crs_win.y = p_tab_mom->tab_u*crs_p_wb->mper2_y;
	crs_win.z += p_tab_mom->tab_x*crs_p_wb->mper2_x;
	crs_win.w += p_tab_mom->tab_y*crs_p_wb->mper2_y;
	
	//U4 n_set = 0;
	//if( win_r != crs_win.z / crs_p_wb->mper2_x + win_l)
	//{
	//	win_r = crs_win.z / crs_p_wb->mper2_x + win_l;
	//	n_set++;
	//}
	//if( win_d != crs_win.w / crs_p_wb->mper2_y + win_u)
	//{
	//	win_d = crs_win.w / crs_p_wb->mper2_y + win_u;
	//	n_set++;
	//}

}
bool GPC_CRS::crs_save( char* p_file )
{
	if(!this)
		return false;
	GPC_FIX* p_fix = NULL;
	GPC_TAB* p_loop = crs_p_root;
	char* p_src;
	while( p_loop < crs_p_end )
	{
		p_src = p_loop->tab_src(this)->fix_p_alloc;
		if( !p_src )
		{
			if( p_loop->tab_p_src )
				p_src = p_loop->tab_p_src->fix_p_alloc;
			else
				p_src = p_loop->tab_src(this)->fix_p_alloc;
			if( !p_src )
				p_src = p_loop->b_left ? " \a " : GPD_SAFE_enter;
		}

		if( p_loop->switch64&GPE_SW64_IN )
		if( p_loop->tab_p_res )
		if( p_loop->tab_p_res->res_type == GPE_NET_sht )
		if( GPC_CRS* p_crs = *((GPC_CRS**)p_loop->tab_p_res->fix_p_alloc ) )
		{
			p_crs->crs_save();
		}

		p_fix = p_fix->fix_format( "\a%s", p_src );
		p_loop++;
	}

	if( !p_file )
		p_file = crs_s_file;
	else if( !*p_file )
		p_file = crs_s_file;

	crs_n_undo = p_fix->fix_write( p_file, crs_n_undo );
	return true;
}
char	s_head[]		= "<HTML>\r\n<HEAD>\r\n<TITLE>HTML GP_RULESx64</TITLE>\r\n</HEAD>\r\n<BODY><TABLE BGCOLOR=#e0e0e0 BORDER=0 CELLPADDING=0 cellspacing=0 valign=top >\r\n<TR>\r\n<TD BGCOLOR=#%0.2x%0.2x%0.2x valign=top >",
	s_new_row[]	= "</TD>\r\n</TR>\r\n<TR>\r\n<TD BGCOLOR=#%0.2x%0.2x%0.2x BORDER=0 CELLPADDING=0 valign=top >",
	s_new_col[]	= "</TD>\r\n<TD BGCOLOR=#%0.2x%0.2x%0.2x valign=top >",
	s_pic[]		= "<A HREF=%s><IMG SRC=%s ALT=""%s"" width=""240"" height=""%d""></A><BR>",
	s_end[]		= "</TD>\r\n</TR></TABLE>\r\n</BODY>\r\n</HTML>\r\n";
char	s_sepi[]		= "[____________________________________________________________________________]";

#define GPD_TAB_SEPI 32

bool GPC_CRS::crs_save_html( char* p_path, char* p_file )
{
	if(!this)
		return false;
	if( !crs_p_wb )
		return false;

	GPC_FIX* p_fix_html = NULL, *p_fix_src = NULL;
	GPC_TAB* p_loop = crs_p_root;
	GPCI4 max_an( 0,0,0,0 );
	while( p_loop < crs_p_end )
	{
		if( !p_loop->b_right )
		{
			if( max_an.z < max_an.x )
				max_an.z = max_an.x;
			max_an.x = 0;
			max_an.y++;
		}
		max_an.x++;
		p_loop++;
	}
	char s_buff[0x200], *p_buff, *p_str;
	U1	row = 0xff,
		cnl = row-10;

	p_fix_html = p_fix_html->fix_format( s_head, cnl, cnl, cnl );
	I8 n_alfa, id8;
	
	for( I8 col = 0; col <= max_an.z; col++ )
	{
		cnl = (row-0x40)-(col%2)*0x10;
		if( !col )
		{
			sprintf( s_buff, "[_num/alfa_]" );
			p_buff = s_buff;
		} else {
			GPF_ALFA2STR( s_buff, col );
			_strupr( s_buff );
			n_alfa = GPD_STRLEN( s_buff );
			p_str = p_buff = s_buff+n_alfa+1;

			gp_strncpy( p_str, s_sepi, (GPD_TAB_SEPI-n_alfa)/2 );
			p_str += (GPD_TAB_SEPI-n_alfa)/2;
			GPM_STRCPY( p_str, s_buff );
			p_str += n_alfa;
			gp_strncpy( p_str, s_sepi+sizeof(s_sepi)-(GPD_TAB_SEPI-n_alfa)/2, (GPD_TAB_SEPI-n_alfa)/2 );
			p_str += (GPD_TAB_SEPI-n_alfa)/2;
			*p_str = 0;
		}
		p_fix_html = p_fix_html->fix_format( "%s", p_buff );

		if( col < max_an.z )
			p_fix_html = p_fix_html->fix_format( s_new_col, cnl, cnl, cnl );
	}
	
	cnl = row-0x10;
	
	
	char* p_src;
	p_loop = crs_p_root;
	max_an.x = 0;
	
	while( p_loop < crs_p_end )
	{
		if( !p_loop->b_left )
		{
			row = (row == 0xff) ? 0xef : 0xff;
			
			cnl = row-0x40;
			p_fix_html = p_fix_html->fix_format( s_new_row, cnl, cnl, cnl );

			p_fix_html = p_fix_html->fix_format( "%d", max_an.w );
			max_an.w++;
			
			cnl = row-0x10;
			p_fix_html = p_fix_html->fix_format( s_new_col, cnl, cnl, cnl );

			max_an.x = 2;
		}

		cnl = row-((max_an.x%2)*0x10);
		
		
		
		//-----------------------------------------------------------------
		//		begin p_src
		//-----------------------------------------------------------------
		if(true)
		{
			if( p_loop->tab_p_exe )
			if( GPC_RES* p_res = p_loop->tab_p_res )
			{
				
				if( p_res->res_id == GPE_ALFA_PIC )
				{
					id8 = GPD_IF_THIS( p_res, i8(), 0 );
					if( id8 < 0 || id8 >= crs_p_wb->n_pic )
						break;
							
					if( GPC_PICx64* p_pic = crs_p_wb->pp_pic[id8] )
					if( p_pic->pic_w )
					{
						sprintf( p_file, "%s.jpg", p_pic->s_name_ );
						if( p_pic->pic_save( crs_p_wb, p_path ) )
						{
							p_fix_html = p_fix_html->fix_format( s_pic, p_file, p_file, p_file, p_pic->pic_h*240/p_pic->pic_w );	
						}
					}
				}
				else if( p_res->res_type == GPE_NET_res )
				{
					GPC_RES	**pp_res = (GPC_RES**)p_res->fix_p_alloc,
						**pp_end = (GPC_RES**)(p_res->fix_p_alloc + p_res->fix_n_alloc);
					while( pp_res < pp_end )
					{
						if( p_res = *pp_res )
						switch( p_res->res_id )
						{
							case GPE_ALFA_PIC:
								id8 = GPD_IF_THIS( p_res, i8(), 0 );
								if( id8 < 0 || id8 >= crs_p_wb->n_pic )
									break;
							
								if( GPC_PICx64* p_pic = crs_p_wb->pp_pic[id8] )
								{
									sprintf( p_file, "%s.jpg", p_pic->p_name_ );
									if( p_pic->pic_save( crs_p_wb, p_path ) )
									{
										p_fix_html = p_fix_html->fix_format( s_pic, p_file, p_file, p_file );	
									}
								}
								break;
						}
						pp_res++;
					}
				}

				if( !p_loop->tab_p_mini )
				{
					p_loop->tab_p_mini = p_loop->tab_p_mini->fix_res2str( p_loop->tab_p_res, true );
				}
			}
			
			p_src = "";
			if( p_loop->tab_p_mini ? p_loop->tab_p_mini->fix_p_alloc : false )
			{
				p_src = p_loop->tab_p_mini->fix_p_alloc;
			} else {
				if( p_fix_src = p_loop->tab_src(this) )
				{
					p_src = p_fix_src->fix_p_alloc;
					p_src += p_loop->tab_txt;
				}
			}

			if( p_loop->switch64&GPE_SW64_GPU )
				p_fix_html = p_fix_html->fix_format( "<FONT SIZE=1>" );

			p_str = p_src;
			ULL s = -1, n_row = 0;
			while( *p_str )
			{
				p_src += GPD_VAN( p_str, "\r\n" );
				s = -1;
				p_fix_html = p_fix_html->fix_insert( p_str, p_src-p_str, s, 0 );
				if( *p_str )
				{
					p_fix_html = p_fix_html->fix_format( "%s", "<BR>" );
					n_row++;
				}
				p_str = (p_src+=GPD_NINCS( p_src, "\r\n" ));
			}
			p_fix_html = p_fix_html->fix_format( "%s", p_src );

			if( p_loop->switch64&GPE_SW64_GPU )
				p_fix_html = p_fix_html->fix_format( "%s</FONT>", p_src );
			p_src = "";
			if( p_loop->tab_p_exe )
			{
				if( p_fix_src = p_loop->tab_src(this) )
				{
					p_src = p_fix_src->fix_p_alloc;
					p_src += p_loop->tab_txt;
				}

				p_str = p_src;
				ULL s = -1, n_row = 0;
				
				p_fix_html = p_fix_html->fix_format( "<FONT SIZE=2>" );
				while( *p_str )
				{
					p_src += GPD_VAN( p_str, "\r\n" );
					s = -1;
					p_fix_html = p_fix_html->fix_insert( p_str, p_src-p_str, s, 0 );
					if( *p_str )
					{
						p_fix_html = p_fix_html->fix_format( "%s", "<BR>" );
						n_row++;
					}
					p_str = (p_src+=GPD_NINCS( p_src, "\r\n" ));
				}
				p_fix_html = p_fix_html->fix_format( "%s</FONT>", p_src );
			}
		}
		//-----------------------------------------------------------------
		//		end p_src
		//-----------------------------------------------------------------

		if( p_loop->b_right )
		{
			p_fix_html = p_fix_html->fix_format( s_new_col, cnl, cnl, cnl );
			max_an.x++;
		}
		else for( I8 col = max_an.x; col <= max_an.z; col++ )
		{
			cnl = (row-0x40)-(col%2)*0x20;
			if( !col )
			{
				sprintf( s_buff, "num/alfa" );
			} else {
				GPF_ALFA2STR( s_buff, col );
			}
				
			n_alfa = GPD_STRLEN( s_buff );
			_strupr( s_buff );
			p_str = p_buff = s_buff+n_alfa+1;

			gp_strncpy( p_str, s_sepi, (GPD_TAB_SEPI-n_alfa)/2 );
			p_str += (GPD_TAB_SEPI-n_alfa)/2;
			GPM_STRCPY( p_str, s_buff );
			p_str += n_alfa;
			gp_strncpy( p_str, s_sepi+sizeof(s_sepi)-(GPD_TAB_SEPI-n_alfa)/2, (GPD_TAB_SEPI-n_alfa)/2 );
			p_str += (GPD_TAB_SEPI-n_alfa)/2;
			*p_str = 0;

			p_fix_html = p_fix_html->fix_format( s_new_col, cnl, cnl, cnl );
			p_fix_html = p_fix_html->fix_format( "%s", p_buff );
		}
		 
		p_loop++;
	}


	p_fix_html = p_fix_html->fix_format( "%s", s_end );

	GPM_STRCPY( p_file, "\\index.html" );
	p_fix_html->fix_write( p_path, -1 );

	GPM_DEL(p_fix_html);
	//GPM_DEL(p_fix_src);
	return true;
}
GPC_TAB* GPC_CRS::crs_tab_a_n( GPE_ALFA a, I8 n )
{
	if( !crs_pp_row )
	{
		crs_n_row = 0;
		GPC_TAB* p_d = crs_p_root, *p_r;
		//GPE_ALFA a;
		I8 n;
		while (p_d)
		{
			p_d->tab_an.an( GPE_ALFA_A, crs_n_row );
			p_r = p_d;
			if( p_r->b_right )
			{
				p_r++;
				p_r->tab_an.an( (GPE_ALFA)(p_r - p_d + 1 ), crs_n_row );
			}
			
			crs_n_row++;
			p_d = p_d->tab_p_down;
		}

		crs_pp_row = new GPC_TAB*[crs_n_row];

		// SET N_ROW!
		p_d = crs_p_root;
		n = 0;
		while(p_d)
		{
			crs_pp_row[n] = p_d;
			n++;
			p_d = p_d->tab_p_down;
		}
	}
	
	

	GPC_TAB* p_adr = crs_pp_row[n];
	if(!p_adr)
		return NULL;
	if( a < 1 )
		a = GPE_ALFA_A;

	p_adr += a-GPE_ALFA_A;
	
	if ((n + 1) >= crs_n_row)
	{
		if (p_adr >= crs_p_end )
			return NULL;
	}
	else if( p_adr >= crs_pp_row[n+1] ) 
		return NULL;

	return p_adr;
}
GPC_TAB* GPC_CRS::crs_tab_an( GPC_AN& an, GPC_TAB* ref, bool b_touch )
{
	if( an.a < GPE_ALFA_A )
		return NULL;

	if( !crs_pp_row )
	{
		crs_n_row = 0;
		GPC_TAB* p_d = crs_p_root, *p_r;
		I8 n;
		while (p_d)
		{
			p_d->tab_an.an( GPE_ALFA_A, crs_n_row );
			p_r = p_d;
			if( p_r->b_right )
			{
				p_r++;
				p_r->tab_an.an( (GPE_ALFA)(p_r - p_d + 1 ), crs_n_row );
			}
			
			crs_n_row++;
			p_d = p_d->tab_p_down;
		}

		crs_pp_row = new GPC_TAB*[crs_n_row];

		// SET N_ROW!
		p_d = crs_p_root;
		n = 0;
		while (p_d)
		{
			crs_pp_row[n] = p_d;
			n++;
			p_d = p_d->tab_p_down;
		}
	}


	if( crs_n_row <= an.n )
		return NULL;
	GPC_TAB* p_adr = crs_pp_row[an.n];
	if( !p_adr )
		return NULL;

	p_adr += an.a-GPE_ALFA_A;

	if( (an.n + 1) >= crs_n_row )
	{
		if( p_adr >= crs_p_end )
			return NULL;
	}
	else if( p_adr >= crs_pp_row[an.n + 1] )
		return NULL;
	
	if( !ref || p_adr != crs_p_tab_sel )
		return p_adr;

	ref->switch64 |= GPE_SW64_REF;
	return p_adr;
}
ULL GPC_CRS::crs_ascii_ins(U1* p_ascii, ULL n_ascii )
{
	if( !p_ascii || !n_ascii || !this )
		return 0;
	

	GPC_TAB* p_tab = crs_tab_a_n( crs_a1, crs_n1 );
	if( !p_tab )
	{
		p_tab = crs_p_root;
	}
	
	if( !crs_p_tab_sel )
	{
		if( !p_tab )
			p_tab = crs_p_root;

		crs_p_edit = crs_p_tab_sel = p_tab;
		if( !crs_p_tab_sel )
		crs_a1 = GPE_ALFA_A;
		crs_n1 = 0;
		crs_p1 = crs_p_tab_sel->tab_txt;
	} else {
		crs_a1 = crs_p_tab_sel->tab_an.a;
		crs_n1 = crs_p_tab_sel->tab_an.n;
	}

	if( !crs_p_tab_sel->tab_p_src )
		return 0;
	
	GPC_FIX* p_fix = crs_p_tab_sel->tab_p_src;
	char	*p_src = p_fix->fix_p_alloc,
		*p_end = p_src+p_fix->fix_n_alloc;
	if( !p_src )
		return 0;

	GPC_TAB* p_crs_tabo = crs_p_tab_sel;

	if( crs_p1 > p_fix->fix_n_alloc )
		crs_p1 = p_fix->fix_n_alloc;

	ULL	cs = crs_p1,
		ce = cs;

	char	*p_ins = NULL;
	ULL	n_ins = 0;

	if (!*p_ascii)
		return 0;
	
	crs_p_edit = crs_p_tab_sel;
	I8 e2;
	GPC_TAB* p_jmp;
	if( *p_ascii > 0 && *p_ascii < 5 )
	{
		// move TEXT CURSOR (arrow)
		switch (*p_ascii)
		{
			case 1:
				// up
				if( crs_p1 == crs_p_tab_sel->tab_txt )
				{
					// belépünk a setup-ba
					crs_p1--;
					break;
				}
				//crs_p1--;
				e2 = GPF_STR_VISZ(p_src, p_src + crs_p1, "\a\r\n");
				if( e2 != crs_p_tab_sel->tab_txt-1 && e2 > 0 )
				{
					// vannak sorok
					I8 e1 = GPF_STR_VISZ(p_src, p_src + e2, "\a\r\n")+1;
					if( e1 + crs_e < e2 )
					{
						crs_p1 = e1 + crs_e;
					} else {
						crs_p1 = e2;
					}

					if( crs_p1 == crs_p_tab_sel->tab_txt )
					if( p_src[crs_p1] != '\n' )
						crs_p1++;
					
					break;
				}

				// kilép a rublikából
				crs_p1 = 0;
				if( p_jmp = crs_p_tab_sel->tab_p_up )
				{
					if( p_jmp->switch64&GPE_SW64_FORBID )
					{
						p_jmp = NULL;
						break;
					}

					if( p_jmp->switch64&GPE_SW64_JUMP )
					{
						GPC_AN an = p_jmp->tab_an;
						for( an.n--; an.n > -1; an.n-- )
						{
							p_jmp = crs_tab_an( an );
							if( !p_jmp )
								continue;

							if( p_jmp->switch64&GPE_SW64_FORBID )
							{	
								p_jmp = NULL;
								break;
							}

							if( !(p_jmp->switch64&GPE_SW64_JUMP) )
								break;
						}
					}
				}

				if( p_jmp )
				{
					// UNDO_REC ---------------------------------------------
					crs_undo_record();
					
					// tud felfele lépni
					crs_p_tab_sel = p_jmp;
					if( !crs_p_tab_sel->tab_p_src )
						break;

					p_fix = crs_p_tab_sel->tab_p_src;
					p_src = p_fix->fix_p_alloc;
					if (!p_src)
						break;

					e2 = crs_p1 = p_fix->fix_n_alloc;
					
					I8 e1 = GPF_STR_VISZ(p_src, p_src + e2, "\a\r\n")+1;
					if (e1 + crs_e < e2)
					{
						crs_p1 = e1 + crs_e;
					} else {
						crs_p1 = e2;
					}
					if( crs_p1 == crs_p_tab_sel->tab_txt )
						crs_p1++;
					break;
				}
				
				if( !crs_p_tab_sel->b_left )
				{
					if( mom_p_crs )
					if(GPC_TAB* p_mom = mom_p_crs->crs_tab_an( mom_tab_an ) )
					{
						crs_p1 = mom_p_crs->crs_p1 = 0;
						crs_p_tab_sel = NULL;
					}
					break;
				}
				// felfele nem tudott,
				// de megprobálhatjuk balra
				p_jmp = crs_p_tab_sel;
				while( p_jmp )
				{
					p_jmp--;
					if( p_jmp < crs_p_root )
					{
						p_jmp = NULL;
						break;
					}

					if( p_jmp->switch64&GPE_SW64_FORBID )
					{	
						p_jmp = NULL;
						break;
					}

					// tehát nem forbid
					if( !(p_jmp->switch64&GPE_SW64_JUMP) )
						break; // és nem is jump ezt kerestük

					if( !p_jmp->b_left )
					{
						// viszont ebben a sorban nincsen több
						// pápá a ciklust elhagyjuk
						// és nem lesz p_jmp sem ezért nem megy sehova
						p_jmp = NULL;
						break;
					}
				}
				if( !p_jmp )
					break;

				p_fix = p_jmp->tab_p_src;
				p_src = p_fix->fix_p_alloc;
				if (!p_src)
					break;

				// UNDO_REC ---------------------------------------------
				crs_undo_record();

				// nincs fel akkor balra rakjuk
				crs_p_edit = crs_p_tab_sel = p_jmp;

				crs_p1 = crs_p_tab_sel->tab_txt;
				crs_p1 += GPD_VAN(p_src + crs_p1, "\n" );
				if( !p_src[crs_p1] )
					crs_p1--;
				
				break;

			case 3:
				// down
				crs_p1 += GPD_VAN(p_src + crs_p1, "\a\r\n" );
				if( crs_p1 < p_fix->fix_n_alloc )
				{
					crs_p1++;
					e2 = GPD_VAN(p_src + crs_p1, "\a\r\n");
					if (crs_e <= e2)
						crs_p1 += crs_e;
					else
						crs_p1 += e2;

					if( crs_p_tab_sel->switch64&GPE_SW64_IN )
					if( crs_p1 >= crs_p_tab_sel->tab_txt )
					if( crs_p_tab_sel->tab_p_res )
					if( crs_p_tab_sel->tab_p_res->res_type == GPE_NET_sht )
					if( GPC_CRS* p_crs = *((GPC_CRS**)crs_p_tab_sel->tab_p_res->fix_p_alloc ) )
					{
						GPC_FIX* p_fix = p_crs->crs_p_root->tab_src( p_crs );
						p_crs->crs_p_tab_sel = p_crs->crs_p_root;
						p_crs->crs_p1 = p_crs->crs_p_root->tab_txt;
						if( p_crs->win_l != 0 || p_crs->win_u != 0 )
						{
							p_crs->win_r -= p_crs->win_l;
							p_crs->win_l = 0;
							p_crs->win_r += p_crs->win_l;
							
							p_crs->win_d -= p_crs->win_u;
							p_crs->win_u = 0;
							p_crs->win_d += p_crs->win_u;
							p_crs->crs_p_edit = p_crs->crs_p_root;
						}
					}
					break;
				}
				
				
				crs_p_edit = crs_p_tab_sel; // mivan? ez lefele megy, miért megy lefele a p_edit?
				p_jmp = NULL;
				{
					GPC_AN	jmp_an = crs_p_tab_sel->tab_an,
						step_an;
					bool b_break = false;
					while( !p_jmp )
					{
						jmp_an.n++;
						if( crs_n_row <= jmp_an.n )
						{
							if( !mom_p_crs )
								break;
							
							// ilyenkor kifelélép
							GPC_AN	jmp_out_an = mom_tab_an;
							jmp_out_an.n++;
							if( mom_p_crs->crs_n_row > jmp_out_an.n )
							if( GPC_TAB* p_out = mom_p_crs->crs_tab_an(jmp_out_an)  )
							{
								crs_p_tab_sel = mom_p_crs->crs_p_tab_sel = p_out;
								if( crs_p_tab_sel->switch64&GPE_SW64_IN )
								if( crs_p_tab_sel->tab_p_res )
								if( crs_p_tab_sel->tab_p_res->res_type == GPE_NET_sht )
								if( GPC_CRS* p_crs = *((GPC_CRS**)crs_p_tab_sel->tab_p_res->fix_p_alloc ) )
								{
									GPC_FIX* p_fix = p_crs->crs_p_root->tab_src( p_crs );
									p_crs->crs_p_tab_sel = p_crs->crs_p_root;
									p_crs->crs_p1 = p_crs->crs_p_root->tab_txt;
									if( p_crs->win_l != 0 || p_crs->win_u != 0 )
									{
										p_crs->win_r -= p_crs->win_l;
										p_crs->win_l = 0;
										p_crs->win_r += p_crs->win_l;
							
										p_crs->win_d -= p_crs->win_u;
										p_crs->win_u = 0;
										p_crs->win_d += p_crs->win_u;
										p_crs->crs_p_edit = p_crs->crs_p_root;
									}
								}
								crs_p_tab_sel = NULL;
							}
							break;
						}
						step_an = jmp_an;
						while( step_an.a > 0 )
						{
							p_jmp = crs_tab_an( step_an );
							if( p_jmp )
								break;
							step_an = step_an.plus( -1, 0 );
						}
						if( p_jmp )
						{
							if( p_jmp->switch64&GPE_SW64_FORBID )
							{
								break;
							}

							if( p_jmp->switch64&GPE_SW64_JUMP )
							{
								p_jmp = NULL;
								continue;
							}
						}
					}
				}
				
				if( p_jmp )
				{
					// UNDO_REC ---------------------------------------------
					crs_undo_record();

					crs_p_tab_sel = p_jmp;
					
					p_fix = crs_p_tab_sel->tab_p_src;
					if( !(p_src = p_fix ? p_fix->fix_p_alloc : NULL ) )
						break;
					
					crs_p1 = crs_p_tab_sel->tab_txt;
					e2 = GPD_VAN(p_src + crs_p1, "\a\r\n");
					if( crs_e <= e2 )
						crs_p1 += crs_e;
					else
						crs_p1 += e2;

					break;
				}

				if( crs_p_tab_sel ? !crs_p_tab_sel->b_right : true )
					break;

				
				p_jmp = crs_p_tab_sel;
				while( p_jmp )
				{
					p_jmp++;
					if( p_jmp >= crs_p_end )
					{
						p_jmp = NULL;
						break;
					}

					if( p_jmp->switch64&GPE_SW64_FORBID )
					{	
						p_jmp = NULL;
						break;
					}

					// tehát nem forbid
					if( !(p_jmp->switch64&GPE_SW64_JUMP) )
						break; // és nem is jump ezt kerestük

					if( !p_jmp->b_right )
					{
						// viszont ebben a sorban nincsen több
						// pápá a ciklust elhagyjuk
						// és nem lesz p_jmp sem ezért nem megy sehova
						p_jmp = NULL;
						break;
					}
				}
				if( !p_jmp )
					break;

				// UNDO_REC ---------------------------------------------
				crs_undo_record();

				crs_p_tab_sel++;
				crs_p1 = crs_p_tab_sel->tab_txt;
				break;
				

			case 2:
				// right
				if( crs_p1 < p_fix->fix_n_alloc )
				{
					crs_p1++;
					crs_e = crs_p1 - (GPF_STR_VISZ(p_src, p_src + crs_p1, "\r\n\a") + 1);
					break;
				}

				if( !crs_p_tab_sel->b_right )
				{
					if( !mom_p_crs )
						break;
					
					// ilyenkor kifelélép
					if( GPC_TAB* p_out = mom_p_crs->crs_tab_an(mom_tab_an) )
					if( p_out->b_right )
					{
						p_out++;
						crs_p_tab_sel = mom_p_crs->crs_p_tab_sel = p_out;
						if( crs_p_tab_sel->switch64&GPE_SW64_IN )
						if( crs_p_tab_sel->tab_p_res )
						if( crs_p_tab_sel->tab_p_res->res_type == GPE_NET_sht )
						if( GPC_CRS* p_crs = *((GPC_CRS**)crs_p_tab_sel->tab_p_res->fix_p_alloc ) )
						{
							GPC_FIX* p_fix = p_crs->crs_p_root->tab_src( p_crs );
							p_crs->crs_p_tab_sel = p_crs->crs_p_root;
							p_crs->crs_p1 = p_crs->crs_p_root->tab_txt;
							if( p_crs->win_l != 0 || p_crs->win_u != 0 )
							{
								p_crs->win_r -= p_crs->win_l;
								p_crs->win_l = 0;
								p_crs->win_r += p_crs->win_l;
							
								p_crs->win_d -= p_crs->win_u;
								p_crs->win_u = 0;
								p_crs->win_d += p_crs->win_u;
								p_crs->crs_p_edit = p_crs->crs_p_root;
							}
						}
						crs_p_tab_sel = NULL;
					}
					break;
				}

				p_jmp = crs_p_tab_sel;
				while( p_jmp )
				{
					p_jmp++;
					if( p_jmp >= crs_p_end )
					{
						p_jmp = NULL;
						break;
					}

					if( p_jmp->switch64&GPE_SW64_FORBID )
					{	
						p_jmp = NULL;
						break;
					}

					// tehát nem forbid
					if( !(p_jmp->switch64&GPE_SW64_JUMP) )
						break; // és nem is jump ezt kerestük

					if( !p_jmp->b_right )
					{
						// viszont ebben a sorban nincsen több
						// pápá a ciklust elhagyjuk
						// és nem lesz p_jmp sem ezért nem megy sehova
						p_jmp = NULL;
						break;
					}
				}
				if( !p_jmp )
					break;

				// UNDO_REC ---------------------------------------------
				crs_undo_record();
				
				crs_p_tab_sel = p_jmp;
				crs_p1 = crs_p_tab_sel->tab_txt;
				crs_e = 0;
				break;
			
			case 4:
				// left
				if( !crs_p1 || (crs_p1 == crs_p_tab_sel->tab_txt) )
				{
					if( crs_p_tab_sel->b_left )
					{
						p_jmp = crs_p_tab_sel;
						while( p_jmp )
						{
							p_jmp--;
							if( p_jmp < crs_p_root )
							{
								p_jmp = NULL;
								break;
							}

							if( p_jmp->switch64&GPE_SW64_FORBID )
							{	
								p_jmp = NULL;
								break;
							}

							// tehát nem forbid
							if( !(p_jmp->switch64&GPE_SW64_JUMP) )
								break; // és nem is jump ezt kerestük

							if( !p_jmp->b_left )
							{
								// viszont ebben a sorban nincsen több
								// pápá a ciklust elhagyjuk
								// és nem lesz p_jmp sem ezért nem megy sehova
								p_jmp = NULL;
								break;
							}
						}
						if( !p_jmp )
							break;

						// UNDO_REC ---------------------------------------------
						crs_undo_record();
						
						crs_p_tab_sel = p_jmp;
						crs_p1 = crs_p_tab_sel->tab_p_src ? crs_p_tab_sel->tab_p_src->fix_n_alloc : 0;
						break;
					}
					else if( mom_p_crs ) 
					{
						// ilyenkor kifelélép
						if( GPC_TAB* p_out = mom_p_crs->crs_tab_an(mom_tab_an) )
						{
							if( p_out->b_left )
							{
								p_out--;
								crs_p_tab_sel = mom_p_crs->crs_p_tab_sel = p_out;
								if( crs_p_tab_sel->switch64&GPE_SW64_IN )
								if( crs_p_tab_sel->tab_p_res )
								if( crs_p_tab_sel->tab_p_res->res_type == GPE_NET_sht )
								if( GPC_CRS* p_crs = *((GPC_CRS**)crs_p_tab_sel->tab_p_res->fix_p_alloc ) )
								{
									GPC_FIX* p_fix = p_crs->crs_p_root->tab_src( p_crs );
									p_crs->crs_p_tab_sel = p_crs->crs_p_root;
									p_crs->crs_p1 = p_crs->crs_p_root->tab_txt;
									if( p_crs->win_l != 0 || p_crs->win_u != 0 )
									{
										p_crs->win_r -= p_crs->win_l;
										p_crs->win_l = 0;
										p_crs->win_r += p_crs->win_l;
							
										p_crs->win_d -= p_crs->win_u;
										p_crs->win_u = 0;
										p_crs->win_d += p_crs->win_u;
										p_crs->crs_p_edit = p_crs->crs_p_root;
									}
								}
								
								crs_p_tab_sel = NULL;
								break;
							}
							if( p_jmp = p_out->tab_p_up )
							{
								
								if( p_jmp->switch64&GPE_SW64_FORBID )
								{
									p_jmp = NULL;
									break;
								}

								if( p_jmp->switch64&GPE_SW64_JUMP )
								{
									GPC_AN an = p_jmp->tab_an;
									for( an.n--; an.n > -1; an.n-- )
									{
										p_jmp = crs_tab_an( an );
										if( !p_jmp )
											continue;

										if( p_jmp->switch64&GPE_SW64_FORBID )
										{	
											p_jmp = NULL;
											break;
										}

										if( !(p_jmp->switch64&GPE_SW64_JUMP) )
											break;
									}
								}
								// UNDO_REC ---------------------------------------------
								crs_undo_record();
								crs_p_tab_sel = NULL;
								mom_p_crs->crs_p_tab_sel = p_jmp;
								mom_p_crs->crs_p1 = p_jmp->tab_p_src ? p_jmp->tab_p_src->fix_n_alloc : 0;
								break;
							}
						}

					}
					
					if( p_jmp = crs_p_tab_sel->tab_p_up )
					{
						if( p_jmp->switch64&GPE_SW64_FORBID )
						{
							p_jmp = NULL;
							break;
						}

						if( p_jmp->switch64&GPE_SW64_JUMP )
						{
							GPC_AN an = p_jmp->tab_an;
							for( an.n--; an.n > -1; an.n-- )
							{
								p_jmp = crs_tab_an( an );
								if( !p_jmp )
									continue;

								if( p_jmp->switch64&GPE_SW64_FORBID )
								{	
									p_jmp = NULL;
									break;
								}

								if( !(p_jmp->switch64&GPE_SW64_JUMP) )
									break;
							}
						}
						// UNDO_REC ---------------------------------------------
						crs_undo_record();

						crs_p_tab_sel = crs_p_tab_sel->tab_p_up;
						crs_p1 = crs_p_tab_sel->tab_p_src ? crs_p_tab_sel->tab_p_src->fix_n_alloc : 0;
						//crs_p1 = crs_p_tab_sel->tab_p_src->fix_n_alloc;
					}
					break;
				}
				
				crs_p1--;
				crs_e = crs_p1 - (GPF_STR_VISZ(p_src, p_src + crs_p1, "\a\r\n") + 1);
				break;
		}

		if( !crs_p_tab_sel )
		{
			// oo kiugrott
			return n_ascii - 1;
		}

		if( crs_p_edit > crs_p_tab_sel )
			crs_p_edit = crs_p_tab_sel;
		if( p_crs_tabo )
		if( p_crs_tabo != crs_p_tab_sel )
		{
			p_crs_tabo->tab_irq( this );
			crs_redo_record();

			if( crs_p_tab_sel->switch64&GPE_SW64_IN )
			if( crs_p1 >= crs_p_tab_sel->tab_txt )
			if( crs_p_tab_sel->tab_p_res )
			if( crs_p_tab_sel->tab_p_res->res_type == GPE_NET_sht )
			if( GPC_CRS* p_crs = *((GPC_CRS**)crs_p_tab_sel->tab_p_res->fix_p_alloc ) )
			{
				GPC_FIX* p_fix = p_crs->crs_p_root->tab_src( p_crs );
				p_crs->crs_p_tab_sel = p_crs->crs_p_root;
				p_crs->crs_p1 = p_crs->crs_p_root->tab_txt;
				if( p_crs->win_l != 0 || p_crs->win_u != 0 )
				{
					p_crs->win_r -= p_crs->win_l;
					p_crs->win_l = 0;
					p_crs->win_r += p_crs->win_l;
							
					p_crs->win_d -= p_crs->win_u;
					p_crs->win_u = 0;
					p_crs->win_d += p_crs->win_u;
					p_crs->crs_p_edit = p_crs->crs_p_root;
				}
			}

			if( p_crs_tabo->switch64&GPE_SW64_IN )
			if( p_crs_tabo->tab_p_res )
			if( p_crs_tabo->tab_p_res->res_type == GPE_NET_sht )
			if( GPC_CRS* p_crs = *((GPC_CRS**)p_crs_tabo->tab_p_res->fix_p_alloc ) )
			{
				GPC_FIX* p_fix = p_crs->crs_p_root->tab_src( p_crs );
				if( p_crs->crs_p_tab_sel )
					p_crs->crs_p_tab_sel = NULL;
				p_crs->crs_p1 = p_crs->crs_p_root->tab_txt;
				if( p_crs->win_l != 0 || p_crs->win_u != 0 )
				{
					p_crs->win_r -= p_crs->win_l;
					p_crs->win_l = 0;
					p_crs->win_r += p_crs->win_l;
							
					p_crs->win_d -= p_crs->win_u;
					p_crs->win_u = 0;
					p_crs->win_d += p_crs->win_u;
					p_crs->crs_p_edit = p_crs->crs_p_root;
				}
			}
		}
		return n_ascii - 1;
	}

	I8	n_pre_enter = 0,
		n_post_enter = 0;
	bool	b_join_tab = false,
		b_txt_enter = false,
		b_txt_tab = false;

	char	*p_str = p_src,
		*p_a = p_str+p_tab->tab_txt-1;
	
	crs_undo_record();

	if( !p_tab->b_left )
		n_pre_enter = p_tab->tab_n_enter( this );

	U1 c = *p_ascii;
	//n_ascii--;
	//cpy_down( p_ascii, p_ascii + 1, n_ascii );

	switch( c )
	{
		case 0x5:
			n_ascii--;
			cpy_down( p_ascii, p_ascii + 1, n_ascii );
			
			// del
			ce++;
			b_join_tab = (cs < crs_p_tab_sel->tab_txt) && (ce >= crs_p_tab_sel->tab_txt);
			
			p_ins = NULL;
			n_ins = 0;
			break;
		case 0x7:
			n_ascii--;
			cpy_down( p_ascii, p_ascii + 1, n_ascii );
			
			// ctrl_tab
			// új CELL-a
			if( cs < crs_p_tab_sel->tab_txt )
				break;	// csak txt-be lehet új tabot kérni

			p_ins = " \a \a ";
			n_ins = 5;
			crs_p1 += n_ins;
			b_txt_tab = true;
			break;
		case 0x8:
			n_ascii--;
			cpy_down( p_ascii, p_ascii + 1, n_ascii);
			
			// back_space
			if( ce == cs )
				cs--;
			if( (cs < crs_p_tab_sel->tab_txt) && (ce >= crs_p_tab_sel->tab_txt) )
				cs = crs_p_tab_sel->tab_txt;
			if( ce == cs )
				break;

			p_ins = NULL;
			n_ins = 0;
			crs_p1--;
			break;
		//case 0x9:
			// tab
			// ezt simán be lehet irni 
			// break;
		case 0xa:
			// ctrl_enter
			// új CELL-sor
			if( cs < crs_p_tab_sel->tab_txt )
				break;	// csak txt-be lehet új tabot kérni

			b_txt_enter = true;
			p_ins = GPD_SAFE_ENTER;
			n_ins = GPD_STRLEN(p_ins);
			crs_p1 += n_ins;
			break;
		default:
			{
				//char* p_van = "\1\2\3\4\5\a\b\n";
				p_ins = (char*)p_ascii;
				n_ins = GPD_VAN( p_ins, "\1\2\3\4\5\a\b\n" ); // \a == \7
				GPF_STR_replace( p_ins, n_ins, '\r', '\n' );
				crs_p1 += n_ins;
			}
	}

	if( crs_p_edit > crs_p_tab_sel )
		crs_p_edit = crs_p_tab_sel;

	

	p_fix->fix_insert(p_ins, n_ins, cs, ce);
	p_tab->tab_irq( this );
	//p_tab->tab_ver = crs_ver + 1;

	crs_redo_record();

	n_post_enter = p_tab->tab_n_enter( this );
	// n_pre_enter = hány enter volt?  
	// n_post_enter = hány enter lett?
	// b_join_tab = ki lett törölve '/a' ?
	// b_txt_enter = be lett ütve a txt-be "ctrl+enter"?
	// b_txt_tab = be lett ütve a txt-be "ctrl+tab"?
	GPC_TAB* p_loop = crs_p_root;
	if( false )
	{
		GPF_CON_Write( "\r\re enter off!!" );
	}
	else if( n_pre_enter > 0 && !n_post_enter )
	{
		// felsõ sort folytatjuk ezzel a sorral
		while( p_loop <  crs_p_end )
		{
			if( p_loop != crs_p_tab_sel )
				p_loop->tab_reN_kill_enter( this, crs_p_tab_sel->tab_an, crs_p_tab_sel[-1].tab_an.a );

			p_loop++;
		}
	}
	else if( !n_pre_enter && n_post_enter > 0 )
	{
		n_post_enter = p_tab->tab_n_enter( this );
		// a tabout és utánna következõ maradék sort új sorba tesszük
		while( p_loop < crs_p_end )
		{
			if( p_loop != crs_p_tab_sel )
				p_loop->tab_reN_enter( this, crs_p_tab_sel->tab_an );

			p_loop++;
		}
	}
	else if( b_txt_enter )
	{
		while( p_loop < crs_p_end )
		{
			if( p_loop != crs_p_tab_sel )
				p_loop->tab_reN_enter( this, crs_p_tab_sel->tab_an );

			p_loop++;
		}
	}
	else if( b_txt_tab )
	{
		while( p_loop < crs_p_end )
		{
			if( p_loop != crs_p_tab_sel )
				p_loop->tab_reA_right( this, crs_p_tab_sel->tab_an ); //.plus( 1, 0 ) );

			p_loop++;
		}
	}
	else if( b_join_tab )
	{
		while( p_loop < crs_p_end )
		{
			if( p_loop != crs_p_tab_sel )
				p_loop->tab_reA_left( this, crs_p_tab_sel->tab_an ); //.plus( 1, 0 ) );

			p_loop++;
		}
	}

	

	GPM_DEL( p_tab->tab_p_exe );
	GPM_DEL( p_tab->tab_p_compi );

	if( p_ins == (char*)p_ascii )
	{
		if( n_ascii < n_ins )
		{
			n_ascii -= n_ins;
			cpy_down(p_ascii, p_ascii + n_ins, n_ascii);
		} else
			n_ascii = 0;

		return n_ascii;
	} 
	
	return n_ascii ? n_ascii - 1 : 0;
}
I8 gp_s_mounth[] = {	
			// 2012
			31,
			29+31,				//29
			31+28+31,
			30+31+28+31,
			
			31+30+31+28+31,
			30+31+30+31+28+31,
			31+30+31+30+31+28+31,
			31+31+30+31+30+31+28+31,
			
			30+31+31+30+31+30+31+28+31,
			31+30+31+31+30+31+30+31+28+31,
			30+31+30+31+31+30+31+30+31+28+31,
			31+30+31+30+31+31+30+31+30+31+28+31,
			// 2013
			31,
			28+31,				// 28
			31+28+31,
			30+31+28+31,
			
			31+30+31+28+31,
			30+31+30+31+28+31,
			31+30+31+30+31+28+31,
			31+31+30+31+30+31+28+31,
			
			30+31+31+30+31+30+31+28+31,
			31+30+31+31+30+31+30+31+28+31,
			30+31+30+31+31+30+31+30+31+28+31,
			31+30+31+30+31+31+30+31+30+31+28+31,
			// 2014
			31,
			28+31,				// 28
			31+28+31,
			30+31+28+31,
			
			31+30+31+28+31,
			30+31+30+31+28+31,
			31+30+31+30+31+28+31,
			31+31+30+31+30+31+28+31,
			
			30+31+31+30+31+30+31+28+31,
			31+30+31+31+30+31+30+31+28+31,
			30+31+30+31+31+30+31+30+31+28+31,
			31+30+31+30+31+31+30+31+30+31+28+31,
			// 2015
			31,
			28+31,				// 28
			31+28+31,
			30+31+28+31,
			
			31+30+31+28+31,
			30+31+30+31+28+31,
			31+30+31+30+31+28+31,
			31+31+30+31+30+31+28+31,
			
			30+31+31+30+31+30+31+28+31,
			31+30+31+31+30+31+30+31+28+31,
			30+31+30+31+31+30+31+30+31+28+31,
			31+30+31+30+31+31+30+31+30+31+28+31,
			};
I8 GPC_TIME::get_sys( bool b_touch )
{
	SYSTEMTIME local_time;
	GetLocalTime(&local_time);
	year = local_time.wYear-2012;
	mounth = year*12 + local_time.wMonth;
	day = year*365 + (year/4) + 1 + gp_s_mounth[(year%4)*12+local_time.wMonth]+local_time.wDay;
	hour = day*24 + local_time.wHour;
	minute = hour*60 + local_time.wMinute;
	sec = minute*60 + local_time.wSecond;
	I8 i8_sec = sec*1000 + local_time.wMilliseconds;
	if( msec < i8_sec )
		msec = i8_sec;
	dsec = double(msec%(24*60*60*1000))/1000.0;
	if( reset_ms < 1 )
		reset_ms = msec;
	else
		on_ms = msec-reset_ms;
	
	
	if( b_touch != (touch_ms&1) )
	{
		touch_ms = on_ms&GPD_RULES_LOGIC;
		if( b_touch )
			touch_ms += 1;
	}
	return on_ms&GPD_RULES_LOGIC;
}
GPC_CRS* GPC_CRS::crs_touch( bool b_lb, bool b_mb, bool b_rb, bool b_shift, bool b_ctrl, bool b_alt )
{
	if( !this )
		return NULL;

	GPC_CRS* p_out = this;
	//crs_time.get_sys();
	if( crs_fsec_pre != crs_time.dsec )
	{
		crs_elapse_t = crs_time.dsec - crs_fsec_pre;
		crs_fps += 1.0/crs_elapse_t;
		crs_fps /= 2.0f;
		crs_fsec_pre = crs_time.dsec;
	}

	crs_prev_mxy_wzw = crs_mxy_wzw;

	POINT mxy, physicxy;
	GetPhysicalCursorPos( &physicxy );
	GetCursorPos( &mxy );
	
	GPC_CRS* p_mom = this;
	while( p_mom )
	{
		p_mom->crs_m_pysic.zw( p_mom->crs_m_pysic.xy() );
		p_mom->crs_m_pysic.xy( physicxy.x, physicxy.y );

		p_mom->crs_m_paqu = p_mom->crs_m_aqu;
		p_mom->crs_m_aqu += GPCI4( mxy.x-p_mom->crs_m_aqu.z, mxy.y-p_mom->crs_m_aqu.w,0,0 );
	

		p_mom->crs_mxy_wzw.x = mxy.x;
		p_mom->crs_mxy_wzw.y = mxy.y;
		p_mom->crs_mxy_wzw.z = p_mom->crs_mxy_wzw.x-p_mom->crs_win.x - crs_brd.x;
		p_mom->crs_mxy_wzw.w = p_mom->crs_mxy_wzw.y-p_mom->crs_win.y - crs_brd.y;

		p_mom = p_mom->mom_p_crs;
	}
	if( !crs_b_touch )
	{
		crs_mxy_wzw_lb = crs_mxy_wzw;
		win_lurd_lb = GPCI4( win_l, win_u, win_r, win_d );

		//crs_mb_lr.y = 0;
		crs_mxy_wzw_rb = crs_mxy_wzw;
		win_lurd_rb = GPCI4( win_l, win_u, win_r, win_d );

		plb = b_lb;
		prb = b_rb;
		pctrl = b_ctrl;
		pshift = b_shift;
		palt = b_alt;
		crs_m_aqu.null();
		return p_out;
	}

	//if( crs_p_tab_sel )
	//	crs_p_tab_sel->tab_an.str(crs_s_tab_sel);
	//else 
	//	*crs_s_tab_sel = 0;

	if( b_mb )
	{
		crs_b_touch = false;
		SetWindowPos(crs_p_wb->wb_hWnd, HWND_BOTTOM, 0, 0, 0, 0, 
						//SWP_NOSIZE | 
						SWP_NOMOVE | SWP_NOACTIVATE );
		SwitchToThisWindow( crs_p_wb->wb_hWnd, true );
		//EnableWindow( crs_p_wb->wb_hWnd, false );
		//SwitchToThisWindow( crs_p_wb->wb_hWnd, true );
		return p_out;
	}
		
	
	crs_m_aqu.zw( mxy.x, mxy.y );

	crs_m_on_luan.x = crs_mxy_wzw.z / crs_p_wb->mper2_x + win_l;
	crs_m_on_luan.y = crs_mxy_wzw.w / crs_p_wb->mper2_y + win_u;

	GPC_TAB* p_tab = crs_tab_an( crs_m_on_luan.an(), NULL, true );
	if(p_tab)
	{
		tab01.x =	float(crs_mxy_wzw.z - ((p_tab->tab_l-win_l)*crs_p_wb->mper2_x))
					/ float( p_tab->tab_x*crs_p_wb->mper2_x);
		tab01.x -= float((int)tab01.x);

		tab01.y =	float(crs_mxy_wzw.w - ((p_tab->tab_u-win_u)*crs_p_wb->mper2_y))
					/ float(  p_tab->tab_y*crs_p_wb->mper2_y );
		tab01.y -= float((int)tab01.y);
	}
	if( p_tab == crs_p_tab_sel)
	if( p_tab ? p_tab->switch64&GPE_SW64_IN : false )
	if( p_tab->tab_p_res )
	if( p_tab->tab_p_res->res_type == GPE_NET_sht )
	if( GPC_CRS* p_in = *((GPC_CRS**)p_tab->tab_p_res->fix_p_alloc ) )
	{
		p_in->crs_b_touch = true;
		p_in = p_in->crs_touch( b_lb, b_mb, b_rb, b_shift, b_ctrl, b_alt );
		if( p_in )
			p_out = p_in;
	}				
	

	crs_mwheel = crs_p_wb->wb_p_crs->crs_mwheel;
	if( p_out == this )
	if( crs_mwheel != 0 )
	{
		if( b_ctrl )
		{
			I8	u = (win_r-win_l),
				v = (win_d-win_u),
				cu = (u/2)+win_l,
				cv = (v/2)+win_u,
				oldx = crs_p_wb->mini_x, newx = oldx;

			crs_p_wb->mini_x += 4*(crs_mwheel/WHEEL_DELTA);
			if( crs_p_wb->mini_x < 4 )
				crs_p_wb->mini_x = 4;

			newx = crs_p_wb->mini_x;
			crs_p_wb->mini_y = (crs_p_wb->mini_x*crs_p_wb->char_x)/crs_p_wb->char_y;

			crs_p_wb->mper2_x = crs_p_wb->mini_x/2;
			crs_p_wb->mper2_y = crs_p_wb->mini_y/2;

			if( oldx != newx )
			{
				win_l = (cu-(((u/2)*oldx)/newx));
				win_r = (crs_win.z/crs_p_wb->mper2_x) + win_l;
				

				win_u = (cv-(((v/2)*oldx)/newx));
				win_d = (crs_win.w/crs_p_wb->mper2_y) + win_u;
				
			}

		}
		else if( b_shift )
		{
			win_r -= win_l;
			win_l -= crs_mwheel/(WHEEL_DELTA/2);
			win_r += win_l;
		} else {
			win_d -= win_u;
			win_u -= crs_mwheel/(WHEEL_DELTA/2);
			win_d += win_u;
		}
		I8 d = sqrt( win_u*win_u + win_l*win_l );
		if( d < 2 )
		{
			win_r -= win_l;
			win_l = 0;
			win_d -= win_u;
			win_u = 0;
		}
		crs_p_wb->wb_p_crs->crs_mwheel = 0;
		return p_out;
	}
	
	

	crs_mxy_wzw_lbdir.null();
	crs_mxy_wzw_rbdir.null();
	// mouse LEFT button
	if( b_lb )		//== 0x8001 )
	{
		crs_mxy_wzw_lbdir = crs_mxy_wzw - crs_mxy_wzw_lb;
	} else {
		crs_mxy_wzw_lb = crs_mxy_wzw;
		win_lurd_lb = GPCI4( win_l, win_u, win_r, win_d );
	}
	
	GPCV2 move = crs_mxy_wzw_lbdir.xy_v2()/GPCV2( crs_p_wb->mper2_x, crs_p_wb->mper2_y );
	if( p_tab ? !(p_tab->switch64&GPE_SW64_UNSEL) : true )
	if( move.qlen() > 1.0f )
	{
		move = crs_mxy_wzw_lbdir.xy_v2()/GPCV2( crs_p_wb->mper2_x, crs_p_wb->mper2_y );
		win_r = win_lurd_lb.z-win_lurd_lb.x;
		win_l = win_lurd_lb.x-move.x;
		win_r += win_l;

		win_d = win_lurd_lb.w-win_lurd_lb.y;
		win_u = win_lurd_lb.y-move.y;
		win_d += win_u;
		return p_out;
	}

	// mouse RIGHT button
	if( b_rb )
	{
		crs_mxy_wzw_rbdir = crs_mxy_wzw - crs_mxy_wzw_rb;
	} else {
		crs_mxy_wzw_rb = crs_mxy_wzw;
	}

	

	if( plb && !b_lb )
	{
		char	s_ascii[0x100],
			s_num1[0x20],
			s_num2[0x20];
		// Mouse Left Button felenged
		if( crs_p_tab_sel ? crs_p_tab_sel->tab_an == crs_m_on_luan.an() : false )
		{
			// ugyan az a tab de
			// akkor CRS_POS pozició változtatás
			crs_p1 = crs_p_tab_sel->tab_xy2pos( crs_m_on_inxy.x, crs_m_on_inxy.y, *this );
		}
		else if( b_alt && !b_ctrl )
		{

			char n_ascii =	b_shift
					?
					sprintf( s_ascii, " thisa%s, thisn%s ", GPF_STR_PLUS_MINUS( s_num1, crs_m_on_luan.z-crs_p_tab_sel->tab_an.a ), GPF_STR_PLUS_MINUS( s_num2, crs_m_on_luan.w-crs_p_tab_sel->tab_an.n ) )
					:
					sprintf( s_ascii, " %lld, %lld ", crs_m_on_luan.z, crs_m_on_luan.w );
					
			if( crs_p_wb->wb_p_crs_sel )
			while( n_ascii > 0 )
			{
				n_ascii = crs_p_wb->wb_p_crs_sel->crs_ascii_ins( (U1*)s_ascii, n_ascii );
			}
		}
		else if( b_ctrl )
		{
			// CTRL folyamatosan lenyomva
			// insert crs_p_wb->wb_s_man
			if( *crs_p_wb->wb_s_tab_on )
			if( crs_p_wb->wb_p_crs_sel == this )
			{
				char n_ascii = sprintf( s_ascii, "%s%s", b_shift ? "#" : "", crs_p_wb->wb_s_tab_on );
				while( n_ascii > 0 )
				{
					n_ascii = crs_p_wb->wb_p_crs_sel->crs_ascii_ins( (U1*)s_ascii, n_ascii );
				}
			}
		} else {
			// CRS_TAB változtatás
			if( p_tab ? !(p_tab->switch64&GPE_SW64_UNSEL) : true )
			{
				bool b_clear = false;
				if( mom_p_crs )
				{
					if( mom_p_crs->crs_m_on_luan.zAN() != mom_tab_an )
					{
						plb	= b_lb;
						prb	= b_rb;
						pctrl	= b_ctrl;
						pshift	= b_shift;
						palt	= b_alt;
						
						//p_out = mom_p_crs; //->crs_tab_an( mom_tab_an );
						p_out->crs_p_tab_sel = NULL;
						b_clear = true;
						while( p_out->mom_p_crs )
						{
							p_out = p_out->mom_p_crs;
							p_out->crs_p_tab_sel = NULL;
						
							p_out->plb		= plb;
							p_out->prb		= prb;
							p_out->pctrl	= pctrl;
							p_out->pshift	= pshift;
							p_out->palt	= palt;
						}
					}
				} else 
					p_out = this;
				GPC_TAB* p_o_tab; 
				if( p_o_tab = p_out->crs_tab_an( p_out->crs_m_on_luan.an(), NULL, true ) )
				{	
					if( p_out->crs_p_edit > p_out->crs_p_tab_sel )
						p_out->crs_p_edit = p_out->crs_p_tab_sel;

					p_out->crs_undo_record();

					p_o_tab->tab_ver = p_out->crs_ver + 1;
					p_out->crs_p_tab_sel = p_o_tab;
					p_out->crs_a1 = p_out->crs_p_tab_sel->tab_an.a;
					p_out->crs_n1 = p_out->crs_p_tab_sel->tab_an.n;
					p_out->crs_p1 = p_out->crs_p_tab_sel->tab_txt;

					p_out->crs_redo_record();
				}
				/*else if( p_out->crs_p_tab_sel )
				{
					p_o_tab = crs_p_tab_sel;
					p_out->crs_a2 = p_o_tab->tab_an.a;
					p_out->crs_n2 = p_o_tab->tab_an.n;
					p_out->crs_p2 = p_out->crs_p1;
				}*/

				if( b_clear )
					p_out = NULL;
			}
			 
		}
	}

	plb = b_lb;
	prb = b_rb;
	pctrl = b_ctrl;
	pshift = b_shift;
	palt = b_alt;
	return p_out;
}
void GPC_CRS::crs_ready( GPC_TAB* p_tab_mom )
{
	if(!this)
		return;

	crs_init( p_tab_mom );
		
	if( !crs_n_loop && crs_p_edit >= crs_p_end )
		return;
	
	// SETUP
	bool b_compi = false;
	ULL n_edit = crs_p_edit - crs_p_root;
	if( crs_p_edit < crs_p_end )
	{
		crs_p_AN = crs_p_edit;
		if( crs_p_AN )
		{
			while( crs_p_AN->b_left )
				crs_p_AN--;
		} else {
			crs_p_AN = crs_p_edit = crs_p_root;
		}
	}
	
	GPC_TAB *p_d, *p_r, *p_irq = NULL;
	
	if( crs_p_edit < crs_p_end )
	{
		p_r = crs_p_edit;
		while( p_r < crs_p_end )
		{
			if( p_r->tab_p_up )
			{
				p_r->tab_p_up->tab_p_down = NULL;
				p_r->tab_p_up = NULL;
			}
			p_r->tab_p_down = NULL;

			p_r->b_left = (p_r > crs_p_root);
			p_r->b_right = (p_r < (crs_p_end-1));
			p_r++;
		}

		while( crs_p_edit < crs_p_end )
		{
			//if( crs_p_edit->switch64&GPE_SW64_INCLUDE )
			//{
			//	crs_p_edit->switch64 &= GPE_SW64_INCLUDE;
			//	//return NULL;
			//	crs_p_edit->tab_p_res = crs_p_edit->tab_p_res->res_sht( this, crs_p_edit->tab_s_name, crs_p_edit->tab_an );
			//	if( crs_p_edit->tab_p_res )
			//	if( crs_p_edit->tab_p_res->res_type != GPE_NET_sht )
			//	if( GPC_CRS* p_crs = *((GPC_CRS**)crs_p_edit->tab_p_res->fix_p_alloc) )
			//	{	
			//		p_crs->crs_ready( crs_p_edit );
			//	}
			//}
			b_compi |= crs_p_edit->tab_tab(*this);
			crs_p_edit++;
		}
	}
	if( n_edit > (crs_p_end - crs_p_root) )
	{
		// újabb tabok lettek hozzá adva
		n_edit = crs_p_end - crs_p_root;
	}

	p_d = crs_p_root;
	// TAB_AN
	I8 n = 0;
	while (p_d)
	{
		p_d->tab_an.an( GPE_ALFA_A, n );
		p_r = p_d;
		while( p_r->b_right )
		{
			p_r++;
			p_r->tab_an.an( (GPE_ALFA)(p_r - p_d + 1 ), n );
		}
		n++;
		p_d = p_d->tab_p_down;
	}

	// ALLOC N_ROW ?
	if( n > crs_n_row )
	{
		GPM_DELARRAY(crs_pp_row);
		crs_n_row = n; 
		crs_pp_row = new GPC_TAB*[crs_n_row];
	}
	
	// SET N_ROW!
	p_d = crs_p_root;
	n = 0;
	while (p_d)
	{
		crs_pp_row[n] = p_d;
		n++;
		p_d = p_d->tab_p_down;
	}
	if( (crs_n_row - n) > 0 )
		gp_zero( crs_pp_row + n, (crs_n_row - n)*sizeof(GPC_TAB*) );
	
	// COMPILE
	ULL ver, maxver = 0;
	if( b_compi )
	{
		crs_p_edit = crs_p_root;
		while( crs_p_edit < crs_p_end )
		{
			if( crs_p_edit == crs_p_tab_sel )
			{
				// az a lényeg
				// ha éppen editálunk, nem a crs_p_tab_sel->tab_p_exe-be forditjuk
				// hanem a crs_p_tab_sel->tab_p_compi-ba
				// így nem all neki lefuttatni :)
				crs_p_edit++;
				GPC_FIX* p_exe = crs_p_tab_sel->tab_p_exe;
				if( !p_exe )
				if( !(p_exe = crs_p_tab_sel->tab_p_compi) )
				{
					p_exe = crs_p_tab_sel->tab_p_compi = crs_p_tab_sel->tab_compi();
				}

				if( p_exe )
				{
					GPC_CALC	*p_c = (GPC_CALC*)p_exe->fix_p_alloc,
						*p_e = (GPC_CALC*)(p_exe->fix_p_alloc+p_exe->fix_n_alloc);
					GPC_AN *p_an, an1,an2;
					GPC_TAB* p_tab;
					while( p_c < p_e )
					{
						if( p_c->calc_fun == GPE_ALFA_ADR )
						if( GPC_RES* p_res = p_c->calc_p_res )
						if( p_res->fix_p_alloc )
						{
							p_an = (GPC_AN*)(p_res->fix_p_alloc+p_res->fix_n_alloc);
							if( p_res->fix_n_alloc > sizeof(GPC_AN)*1 )
							{
								an1 = p_an[-2];
								an2 = p_an[-1];
								for( GPC_AN an = an1; an.n <= an2.n; an.n++ )
								{
									for( an.a = an1.a; an.a <= an2.a; an = an.plus(1,0) )
									{
										p_tab = crs_tab_an( an );
										if( !p_tab )
											continue;
										//if( !p_tab->tab_p_exe )
										//	continue;
										p_tab->switch64 |= GPE_SW64_ERROR;
										GPM_DEL( p_tab->tab_p_mini ); 
									}
								}
							}
							else if( p_res->fix_n_alloc )
							{
								an1 = p_an[-1];
								p_tab = crs_tab_an( an1 );
								if( p_tab )
								{
									p_tab->switch64 |= GPE_SW64_ERROR;
									GPM_DEL( p_tab->tab_p_mini );
								}
							}
						}

						p_c++;
					}
				}
				continue;
			}
			
			
			if( crs_p_edit->tab_p_exe || !crs_p_edit->tab_p_src )
			{
				ver = crs_p_edit->tab_ver;
			} else {
				GPC_FIX* p_exe = crs_p_edit->tab_compi();
				if( p_exe )
				{
					GPM_DEL( crs_p_edit->tab_p_exe );
					p_exe->fix_sw64 |= crs_p_edit->switch64 & GPE_SW64_ERROR;
					crs_p_edit->switch64 = p_exe->fix_sw64;
					ver = p_exe->fix_ver;

					if( p_exe->fix_n_alloc )
						crs_p_edit->tab_p_exe = p_exe;
					else
						GPM_DEL( p_exe );
				} else
					ver = crs_p_edit->tab_ver;
			}
			if( maxver < ver )
				maxver = ver;
			crs_p_edit++;
		}
	}

	if( crs_ver < maxver )
		crs_ver = maxver;

	p_stack = a_stack;
	p_stack++;
	p_stack->x = GPF_GET_n_res_alloc();
	p_stack->y = GPF_GET_n_res_free();
	p_stack++;
	//-----------------------------------------------------
	// RUN ------------------------------------
	//----------------------------
	crs_n_loop = crs_run( false );
	
	p_stack--;
	p_stack->z = GPF_GET_n_res_alloc();
	p_stack->w = GPF_GET_n_res_free();
	p_stack->zw_v2( p_stack->zw_v2()-p_stack->xy_v2() );
	p_stack--;
	if( p_stack[0].z-p_stack[0].w <= p_stack[1].z-p_stack[1].w )
	{
		p_stack[0].zw_v2( p_stack[1].zw_v2() );
	}
		
	maxver = 0;
	
	crs_p_edit = crs_p_root;
	
	while( crs_p_edit < crs_p_end )
	{
		crs_p_edit->tab_src( this );
		if( crs_p_edit->tab_ver > crs_ver )
		{
			maxver = crs_p_edit->tab_ver;
			GPM_DEL( crs_p_edit->tab_p_exe );
		}
		crs_p_edit++;
	}
	
	if( crs_ver < maxver )
		crs_ver = maxver;

	

	// TAB SIZE
	ULL max_y, u = 0, l;
	p_d = crs_p_root;
	while( p_d )
	{
		p_d->tab_size( *this );
		p_r = p_d;
		while( p_r->b_right )
		{
			p_r++;
			p_r->tab_size( *this );
		}

		p_r = p_d;
		p_r->tab_u = u;
		max_y = p_r->tab_y;
		while( p_r->b_right )
		{
			p_r++;
			p_r->tab_u = u;
			if( p_r->tab_y < max_y )
				p_r->tab_y = max_y;
		}
		u += max_y;
		p_d = p_d->tab_p_down;
	}
	p_d = crs_p_root;
	while( p_d )
	{
		p_r = p_d;
		p_r->tab_l = 0;
		l = p_r->tab_x;
		while( p_r->b_right )
		{
			p_r++;
			p_r->tab_l = l;
			l += p_r->tab_x;
		}
		p_d = p_d->tab_p_down;
	}

	// TAB CURSOR
	if( crs_p_tab_sel )
	if( !crs_p_wb )
		crs_p_tab_sel = NULL;
	else if( crs_p_wb->wb_p_crs )
	if( crs_p_wb->wb_p_crs->crs_b_push )
	{
		crs_b_push = false;
		crs_p_tab_sel->tab_crsxy( *this );
		I8 mx = (win_r-win_l)/2 + 1;
		if( mx > 9 )
			mx = 9;
		I8 my = (win_d-win_u)/2 + 1;
		if( my > 3 )
			my = 3;
		if( win_l > crs_x )
		{
			win_r -= win_l;
			win_l = crs_x-mx;
			if( win_l < 0 )
				win_l = 0;
			win_r += win_l;
		}
		if( win_u > crs_y )
		{
			win_d -= win_u;
			win_u = crs_y-my;
			if( win_u < 0 )
				win_u = 0;
			win_d += win_u;
		}
		if( win_r <= crs_x )
		{
			win_l = win_r - win_l;
			win_r = crs_x+mx;
			win_l = win_r - win_l;
		}
		if( win_d <= crs_y )
		{
			win_u = win_d - win_u;
			win_d = crs_y+my;
			win_u = win_d - win_u;
		}
	}
	crs_n_ready++;
	//SetThreadPriority( crs_p_wb->wb_hWnd, THREAD_PRIORITY_HIGHEST );
	
}
I8 GPC_CRS::crs_run( bool b_no_mini )
{
	
	GPC_TAB* p_loop = crs_p_root;
	I8 n_loop = 0;
	p_loop = crs_p_root;
	GPF_GET_n_res_reset();
	crs_n_debug = 0;
	while( p_loop < crs_p_end )
	{
		if( p_loop->switch64&GPE_SW64_loop )
		{
			// LOOP!
			if( !p_loop->tab_p_src )
			{
				if( p_loop->tab_p_res )
					p_loop->tab_p_src = p_loop->tab_src( this );
			}

			if( !(p_loop->switch64&GPE_SW64_IN) )
			if( p_loop->tab_p_src )
			{
				GPM_DEL( p_loop->tab_p_res );
				GPM_DEL( p_loop->tab_p_mini );
			}
			n_loop++;
		}
		if( p_loop->switch64&GPE_SW64_degug )
		{
			crs_n_debug++;
		}
		p_loop++;
	}

	crs_b_no_mini = b_no_mini;
	crs_p_wb ? crs_p_wb->n_cam_use = 0 : false;
	p_loop = crs_p_root;
	//I8 n_res;
	
	while( p_loop < crs_p_end )
	{
		if( p_loop == crs_p_tab_sel )
		if( !(p_loop->switch64&GPE_SW64_IN) )
		{
			p_loop++;
			continue;
		}
		//n_res = GPF_GET_n_res_alloc();
		p_loop->tab_run( *this );
		//if( p_loop->tab_p_src )
		//if( GPF_GET_n_res_alloc()-n_res > 3 )
		//{
		//	GPF_CON_FORMAT_Write( "\n Na mi van? %s", p_loop->tab_p_src->fix_p_alloc );
		//}
		p_loop++;
	}
	if(crs_b_win_move)
	if( true )
	{
		LONG lStyle = GetWindowLong( crs_p_wb->wb_hWnd, GWL_STYLE);
		lStyle &=	~(	
						WS_CAPTION
						//WS_BORDER
						| WS_THICKFRAME
						| WS_MINIMIZEBOX | WS_MAXIMIZEBOX 
						| WS_SYSMENU
						| WS_CHILD
					);
					//(WS_OVERLAPPED |
					//WS_POPUP;
					//~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
		lStyle |= WS_OVERLAPPED | WS_POPUP;
		SetWindowLong(	crs_p_wb->wb_hWnd, GWL_STYLE, lStyle );
		SetWindowPos(
			crs_p_wb->wb_hWnd,
			//HWND_TOP,  
			HWND_DESKTOP,
			//HWND_BOTTOM,
			crs_win_trg.x,
			crs_win_trg.y,
			crs_win_trg.z,
			crs_win_trg.w,
			0
			// | SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER
		);
		crs_b_win_move = false;
	} else {
		WINDOWINFO wi;
		GetWindowInfo( crs_p_wb->wb_hWnd, &wi );
		//MoveWindow( crs_p_wb->wb_hWnd, crs_win_trg.x, crs_win_trg.y, crs_win_trg.z, crs_win_trg.w, crs_b_win_show );
		GPCI4 border( wi.rcWindow.top-wi.rcClient.top,wi.rcWindow.right-wi.rcClient.right,wi.rcWindow.bottom-wi.rcClient.bottom, wi.rcWindow.left-wi.rcClient.left );
		SetWindowPos(	
						crs_p_wb->wb_hWnd, 
						HWND_BOTTOM, 
						crs_win_trg.x-wi.cxWindowBorders, 
						crs_win_trg.y - +wi.cyWindowBorders,
						crs_win_trg.z+wi.cxWindowBorders*2,
						crs_win_trg.w+wi.cyWindowBorders*2, 
						0
						//SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER
					);
		GetWindowInfo( crs_p_wb->wb_hWnd, &wi );
		crs_win_trg.x = wi.rcClient.left;
		crs_win_trg.y = wi.rcClient.top;
		crs_win_trg.z = wi.rcClient.right - wi.rcClient.left;
		crs_win_trg.w = wi.rcClient.bottom - wi.rcClient.top;
		crs_b_win_move = false;
	}
	if( !crs_b_no_mini && !crs_b_win_show && crs_p_wb )
	{
		ShowWindow(  crs_p_wb->wb_hWnd, SW_SHOW );
		crs_b_win_show = true;
	}
	crs_b_no_mini = false;
	return n_loop;
}
GPC_CRS* GPC_CRS::crs_build( GPC_TXT_C4* p_mc4, GPC_TAB* p_tab_mom, GPCV2* p_tab_pos, bool b_gui, U2 n_deep ) //, bool b_main )
{
	GPC_CRS* p_out = NULL;
	U4 n_set = (crs_n_render)&1 ? 1 : 0;

	I8 start_ms = sht_time.get_sys( true );
	if( !mom_p_crs )
	{
		// a szülõ õ a dx tõl kapja a win méreteket
		if( crs_p_wb->GP_DX11_ReSize( crs_p_wb->gpc_pic_out_id ) )
		n_set++;
		crs_build_cwh.xy(2.0f / float(win_r - win_l), -2.0f / float(win_d - win_u));

		crs_build_xywh.xy( 0, 0 );
		crs_build_xywh.zw( win_r-win_l, win_d-win_u );
		

		
		//-----------------------------------------------------------
		//
		//		WB MINI_C4	
		//
		//----------------------------------------
		if( crs_p_wb->mini_col != crs_build_xywh.z || crs_p_wb->mini_row != crs_build_xywh.w )
		{
			crs_p_wb->mini_col = crs_build_xywh.z;
			crs_p_wb->mini_row = crs_build_xywh.w;
			if( crs_p_wb->mini_cr != crs_p_wb->mini_col*crs_p_wb->mini_row )
			{
				crs_p_wb->mini_cr = crs_p_wb->mini_col*crs_p_wb->mini_row;
				if(crs_p_wb->mini_cr%4 )
					crs_p_wb->mini_cr += 4 - (crs_p_wb->mini_cr%4);
				GPM_DELARRAY(crs_p_wb->p_mini_c4);
				crs_p_wb->p_mini_c4 = new GPC_COLOR4[crs_p_wb->mini_cr*2];
				gp_zero( crs_p_wb->p_mini_c4, crs_p_wb->mini_cr*2*sizeof(*crs_p_wb->p_mini_c4) );

				GPM_DELARRAY(crs_p_wb->p_frame_c4);
				crs_p_wb->p_frame_c4 = new GPC_COLOR4[crs_p_wb->mini_cr*2];
				gp_zero( crs_p_wb->p_frame_c4, crs_p_wb->mini_cr*2*sizeof(*crs_p_wb->p_frame_c4) );
			}
		} else {
			gp_memcpy( crs_p_wb->p_mini_c4+crs_p_wb->mini_cr, crs_p_wb->p_mini_c4, crs_p_wb->mini_cr*sizeof(*crs_p_wb->p_mini_c4) );
			gp_zero( crs_p_wb->p_mini_c4, crs_p_wb->mini_cr*sizeof(*crs_p_wb->p_mini_c4) );

			gp_memcpy( crs_p_wb->p_frame_c4+crs_p_wb->mini_cr, crs_p_wb->p_frame_c4, crs_p_wb->mini_cr*sizeof(*crs_p_wb->p_frame_c4) );
			gp_zero( crs_p_wb->p_frame_c4, crs_p_wb->mini_cr*sizeof(*crs_p_wb->p_frame_c4) );
		}
		if( !crs_p_wb->p_mini_c4 )
		{
			p_mc4 = NULL;
		}
		else if( !p_mc4 )
		{
			mc4.p_mini_c4 = crs_p_wb->p_mini_c4;
			mc4.p_frame_c4 = crs_p_wb->p_frame_c4;
			mc4.col = crs_p_wb->mini_col;
			mc4.row = crs_p_wb->mini_row;
			mc4.n_cr = crs_p_wb->mini_cr;
			p_mc4 = &mc4;
		}

	} else {
		crs_win = mom_p_crs->crs_win;
		crs_win.z = (crs_win.x += crs_build_xywh.x*crs_p_wb->mper2_x);
		crs_win.w = (crs_win.y += crs_build_xywh.y*crs_p_wb->mper2_y);

		crs_win.z += p_tab_mom->tab_x*crs_p_wb->mper2_x;
		crs_win.w += p_tab_mom->tab_y*crs_p_wb->mper2_y;
	
		U4 n_set = 0;
		if( win_r != p_tab_mom->tab_x + win_l)
		{
			win_r = p_tab_mom->tab_x + win_l;
			n_set++;
		}
		if( win_d != p_tab_mom->tab_y + win_u)
		{
			win_d = p_tab_mom->tab_y + win_u;
			n_set++;
		}
		crs_build_cwh = *p_tab_pos;
	}


	if( !n_deep )
		GPF_CON_pos( 0, GPE_CLST_31 );
	else 
		GPF_CON_Write( "\r\n" );

	crs_mini_vx.VX324_reset();
	crs_line_vx.VX324_reset();
	crs_solid_vx.VX324_reset();

	if( !p_tab_mom )
		crs_n_build++;
	if( b_gui )
	{

		I8 y = win_u, d;
		ULL n_u = 0, n_d = crs_n_row, n_h = n_d>>1;
		GPC_TAB *p_h, *p_do = NULL;
	
		//if( p_tab_pos )
		//	crs_pos_wh += *p_tab_pos;
		while( !p_do )
		{
			if( p_h = crs_pp_row[n_h] )
			{
				if (n_h == n_u)
				{
					p_do = p_h;
					break;
				}

				d = p_h->tab_u + p_h->tab_y;
				if( win_u >= p_h->tab_u )
				{
					if( win_u < d )
					{
						p_do = p_h;
						break;
					}
				
					n_u = n_h;
					n_h += (n_d - n_h) >> 1;
				} else {
					n_d = n_h;
					n_h >>= 1;
				}
				continue;
			}

			n_d = n_h;
			n_h >>= 1;
		}

		crs_m_on_luan.zw( 0,0 );
	
		if (!p_do)
			return NULL;

		//bool b_include = false;
		while( p_do )
		{ 
			if (p_do->tab_u > win_d)
				break;
	
			p_h = p_do;
		
			while( (p_h->tab_l + p_h->tab_x) <= win_l )
			{
				if( !p_h->b_right )
					break;

				p_h++;
			}
			if( p_h->tab_l > win_r )
			{
				p_do = p_do->tab_p_down;
				continue;
			}
		
		
			while( p_h->tab_l < win_r )
			{
				if( mom_p_crs ? (mom_p_crs->crs_p_tab_on == p_tab_mom) : true )
				if( p_h->tab_q_in_lu( crs_m_on_luan ) )
				{
					crs_p_tab_on = p_h;
					// crs_man.zw-be kerül csak érték
					crs_m_on_luan = p_h->tab_an;
					crs_m_on_inxy.xy( crs_m_on_luan.x - p_h->tab_l, crs_m_on_luan.y - p_h->tab_u );

					crs_m_on_luan.an().str( crs_s_tab_on ); // csinálja az str-t
					crs_p_wb->wb_p_tab_on += sprintf( crs_p_wb->wb_p_tab_on, "%s%s", p_tab_mom ? "." : "", crs_s_tab_on );
				}

				/// !!! nem egészen jó most csak akkor fut, ha látszik !!!
				/// jobban meg kell oldani
				if( p_h->switch64&GPE_SW64_IN )
				{
					if( p_h->tab_p_res )
					if( p_h->tab_p_res->res_type == GPE_NET_sht )
					if( GPC_CRS* p_crs = *((GPC_CRS**)p_h->tab_p_res->fix_p_alloc ) )
					{
						if( p_h == crs_p_tab_sel )
						{
							GPC_FIX* p_src = p_h->tab_src( p_crs );
							// enek a funkciója, hogy ha az anya tab-setup-ban vagyunk, az includ win_lurd-t beállítja, hogy elférjena setup
							if( p_h->tab_txt > crs_p1 )
							{
								char	*p_str = p_src->fix_p_alloc,
									*p_end = p_str+p_h->tab_txt;
								I8 n_n = 1;
								while( p_str < p_end )
								{
									switch( *p_str )
									{
										case '\n':
										case '\a':
											n_n++;
									}
									p_str++;
								}
								if( p_crs->win_l != 0 || p_crs->win_u != -n_n )
								{
									p_crs->win_r -= p_crs->win_l;
									p_crs->win_l = 0;
									p_crs->win_r += p_crs->win_l;
							
									p_crs->win_d -= p_crs->win_u;
									p_crs->win_u = -n_n;
									p_crs->win_d += p_crs->win_u;
									p_crs->crs_p_edit = p_crs->crs_p_root; 
								}
							}
						}

						p_crs->crs_time = crs_time;
						p_crs->crs_build_xywh.xy( p_h->tab_l-win_l+crs_build_xywh.x, p_h->tab_u-win_u+crs_build_xywh.y );
						p_crs->crs_build_xywh.zw( p_crs->crs_build_xywh.x+p_h->tab_x, p_crs->crs_build_xywh.y+p_h->tab_y );
						if( p_out )
							p_crs->crs_build( p_mc4, p_h, &crs_build_cwh, true, n_deep+1 );
						else 
							p_out = p_crs->crs_build( p_mc4, p_h, &crs_build_cwh, true, n_deep+1 );
					
						crs_mini_vx += p_crs->crs_mini_vx;
						crs_line_vx += p_crs->crs_line_vx;

						crs_solid_vx += p_crs->crs_solid_vx;
						//b_include = true;
						p_crs->crs_n_build = crs_n_build;
					}
				}

				//if( !b_include )
				if( !(p_h->switch64&GPE_SW64_NO) || p_h == crs_p_tab_sel ) // mindenképpen rajzolja ki akkor is ha NO, mert rajta van a cursor
				{
					//------------------------------------------------
					//	TAB_MINI
					//------------------------------------------------
					//p_h->tab_mini( *this, crs_build_cwh, false );
					p_h->tab_mini_c4( p_mc4, *this, false );
				}
				if( !(p_h->switch64&GPE_SW64_OFF) || p_h == crs_p_tab_sel )
				{
					//p_h->tab_line(*this, crs_build_cwh, NULL  );
					//------------------------------------------------
					//	TAB_FRAME
					//------------------------------------------------
					p_h->tab_frame_c4( p_mc4, *this, p_h->tab_p_res );

				}
				p_h->tab_solid(*this, crs_build_cwh, p_h->tab_p_res );
				if( !p_h->b_right )
					break;
				p_h++;
			}
			p_do = p_do->tab_p_down;
		}

		if( crs_p_tab_sel )
		if( crs_p_tab_sel->switch64&GPE_SW64_IN )
		{
			GPC_FIX* p_src = crs_p_tab_sel->tab_src( this );
			// enek a funkciója, hogy ha az anya tab-setup-ban vagyunk, az includ win_lurd-t beállítja, hogy elférjena setup
			if( crs_p_tab_sel->tab_txt > crs_p1 )
			{
				if( p_out )
				{
					p_out->crs_p_tab_sel = NULL;
				}
				p_out = this;
			}
		} else {
			if( !p_out )
				p_out = this;
		}
	

		if( p_out && !p_tab_mom )
		{
			if( p_h = p_out->crs_p_tab_sel )
			{
				p_crs_vx = crs_mini_vx.p_alloc;
				p_crs_vxe = crs_mini_vx.p_vx;
				
				//------------------------------------------------
				//	TAB_MINI
				//------------------------------------------------
				//p_h->tab_mini( *p_out, p_out->crs_build_cwh, true, false, &crs_mini_vx );
				p_crs_vx = crs_mini_vx.p_alloc+(p_crs_vxe-p_crs_vx);
				p_crs_vxe = crs_mini_vx.p_vx;

				p_h->tab_mini_c4( p_mc4, *p_out, true, false );
			} else {
				p_crs_vx = p_crs_vxe = 0;
		}

		} else {
			p_crs_vx = p_crs_vxe = 0;
		}

	}
	I8 res_ms = sht_time.get_sys( true );
	for( GPC_TAB *p_tab = crs_p_root; p_tab < crs_p_end; p_tab++ )
	{
		if( p_tab->switch64&GPE_SW64_IN )
		{
			if( p_tab->tab_p_res )
			if( p_tab->tab_p_res->res_type == GPE_NET_sht )
			if( GPC_CRS* p_crs = *((GPC_CRS**)p_tab->tab_p_res->fix_p_alloc ) )
			if( p_crs->crs_n_build != crs_n_build || p_crs->crs_n_render != crs_n_render )
			{
				p_crs->crs_time = crs_time;

				p_crs->crs_line_vx.VX324_reset();
				p_crs->crs_solid_vx.VX324_reset();


				p_crs->crs_build_xywh.xy( p_tab->tab_l-win_l+crs_build_xywh.x, p_tab->tab_u-win_u+crs_build_xywh.y );
				p_crs->crs_build_xywh.zw( p_crs->crs_build_xywh.x+p_tab->tab_x, p_crs->crs_build_xywh.y+p_tab->tab_y );

				p_crs->crs_build( p_mc4, p_tab, &crs_build_cwh, false, n_deep+1 );

				p_crs->crs_n_build = crs_n_build;
				p_crs->crs_n_render = crs_n_render;

				crs_line_vx += p_crs->crs_line_vx;
				crs_solid_vx += p_crs->crs_solid_vx;
			}
		}
		crs_p_wb->n_cam_cnt++;
		//if( !(crs_p_wb->n_cam_cnt%4) )
		if( p_tab->tab_an.a == GPE_ALFA_A )
		if( !(p_tab->tab_an.n%0x40) )
		if( crs_p_wb->n_cam )
		{
			GPC_CAMx64* p_cam_busy = crs_p_wb->p_cam_busy;
			if( crs_p_wb->cam_busy_id < 0 || crs_p_wb->cam_busy_id > crs_p_wb->n_cam )
						p_cam_busy = NULL;
			else if( crs_p_wb->p_cam_busy != crs_p_wb->pp_cam[crs_p_wb->cam_busy_id] )
			{
				crs_p_wb->p_cam_busy = p_cam_busy = NULL;
			}

			if( p_cam_busy )
			{
				if( p_cam_busy->b_cam_die && !p_cam_busy->b_cam_die_ack )
				{
					GPF_CON_FORMAT_Write("Camera %s died!", p_cam_busy->s_name);
					p_cam_busy->b_cam_die_ack = true;
					// Load monoscope
					p_cam_busy->GPC_CAMx64_BOS_MonoScope(crs_p_wb);
					p_cam_busy->n_done = p_cam_busy->n_qc;
					crs_p_wb->p_cam_busy = NULL;
					continue;
				}

				if( crs_p_wb->q_show_done() )
				{
					if( p_cam_busy->GPC_CAMx64_BOS_t20_b30_b80(crs_p_wb) )
					{
						// arra vár, hogy a foglalt kamera végezzen
						p_cam_busy->n_done = p_cam_busy->n_qc;
						crs_p_wb->p_cam_busy = NULL;
					}
					crs_p_wb->show_go();
				}
			} else {
				if( crs_p_wb->q_show_done() )
				{	
					// ha kiürült a p_cam_busy
					// kamerákat választunk, ameikeknél b_cam_use == true
					if( crs_p_wb->cam_busy_id >= crs_p_wb->n_cam )
						crs_p_wb->cam_busy_id = -1;

					for( crs_p_wb->cam_busy_id++; crs_p_wb->cam_busy_id < crs_p_wb->n_cam; crs_p_wb->cam_busy_id++ )
					{
						if( !(p_cam_busy = crs_p_wb->pp_cam[crs_p_wb->cam_busy_id]) )
							continue;

						if( p_cam_busy->n_qc <= p_cam_busy->n_done )
							continue;

						// nem kell mert ha indul akor úgy is kitölti
						//crs_p_wb->p_cam_busy = p_cam_busy; 

						if( !p_cam_busy->GPC_CAMx64_BOS_tDONE_bSYNC( crs_p_wb ) )
						{
							//p_cam_busy->n_qc = p_cam_busy->n_done;
							//crs_p_wb->cam_busy_id--;
							break;
						}
						p_cam_busy = NULL;
						break;
					}
					crs_p_wb->show_go();
				} 
			}
		}
		res_hud_id = res_target_id = -1;
		res_p_pic = NULL;
		res_n_pic = 0;
		res_gpu_id = -1;
		res_b_clr = false;
		res_p_mini_vx = res_p_line_vx = res_p_solid_vx = NULL;
				
		p_tab->tab_res( *this, p_tab->tab_p_res );
	}

	I8 end_ms = sht_time.get_sys( true );
	
	if( !n_deep )
		GPF_CON_pos( 0, GPE_CLST_30 );
	GPF_CON_FORMAT_Write( "ShtBLD: %s %lldms 1000/ms:%lldn RES: %lld 1000/ms:%lldn", crs_p_file, res_ms-start_ms, (res_ms-start_ms) ? sec2ms/(res_ms-start_ms) : 0, (end_ms-res_ms), (end_ms-res_ms) ? sec2ms/(end_ms-res_ms): 0 );
	return p_out;
}

void  GPC_CRS::crs_build_plus(void)
{
	for( GPC_TAB *p_tab = crs_p_root; p_tab < crs_p_end; p_tab++ )
	{
		p_tab->tab_res_plus( *this, p_tab->tab_p_res );
	}
}