#include "gpstd_x64.h"
#include "gp_man_lws.h"

char gp_s_lws[] =
		"LoadObjectLayer\n"
		"AddBone\n"
		"AddNullObject\n"
		"AddLight\n"
		"AddCamera\n"
		"NumChannels\n"
		"Channel\n"
		"{\n"
		"Envelope\n"
		"Key\n"
		"}\n"
		"ParentItem\n"
		"BoneName\n"
		"BoneRestPosition\n"
		"BoneRestDirection\n"
		"BoneRestLength\n"
		;
void GPC_WBx64::GP_WBx64_GIM_Reset( void )
{
	p_lws_bons = p_lws_bons->dick_add( gp_man_lws );
}

GPC_GIMx64* GPC_WBx64::GP_WBx64_GIM_Creat( char* p_file )
{
	if( !p_file )
		return NULL;
	if( !*p_file )
		return NULL;

	char* p_name = p_file+GPF_STR_VISZ( p_file, NULL, "\\:" );
	p_name += GPD_NINCS( p_name, "\\:" );

	I8 n_empty = n_gim;
	GPC_GIMx64* p_gim = NULL;
	if( pp_gim )
	for( GPC_GIMx64 **pp_g = pp_gim, **pp_ge = pp_gim+n_gim; pp_g < pp_ge; pp_g++ )
	{
		if( p_gim = *pp_g )
		{
			if( !_stricmp( p_gim->p_name, p_name ) )
			{
				return p_gim;
			} else 
				p_gim = NULL;
		}
		else if( n_empty > pp_g-pp_gim )
		{
			n_empty = pp_g-pp_gim;
		}
	}

	if( n_empty >= n_gim )
	{
		GPC_GIMx64 **pp_old = pp_gim;
		I8 n_old = n_gim;
		n_gim = n_empty+0x10;
		pp_gim = (GPC_GIMx64**)memset( new GPC_GIMx64*[n_gim], 0, n_gim*sizeof(GPC_GIMx64*) );
		if( n_old )
		{
			gp_memcpy( pp_gim, pp_old, n_old*sizeof(GPC_GIMx64*) );
			memset( pp_old, 0, n_old*sizeof(GPC_GIMx64*) );
		}
		GPM_DELARRAY( pp_old );
	}

	pp_gim[n_empty] = p_gim = new GPC_GIMx64( this, p_file );
	p_gim->gim_id = n_empty;
	return p_gim;
}

I8 GPC_GISx64::gis_lws_load( char* p_file, float dim )
{
	GPC_GIMx64* p_gim = p_wb->GP_WBx64_GIM_Creat( p_file );
	if( !p_gim )
		return -1;

	if( p_gim->dim == dim )
		return p_gim->gim_id;
	
	if( !p_wb->p_lws_com )
	{
		p_wb->p_lws_com = p_wb->p_lws_com->dick_add( gp_s_lws );
		if( !p_wb->p_lws_com )
			return -1;
	}
	
	p_file = p_gim->s_file + GPD_NINCS( p_gim->s_file, " \t\\" );
	GPM_STRCPY( p_wb->wb_p_crs->crs_p_path, p_file );
	ULL s = 0;
	GPC_FIX* p_lws = ((GPC_FIX*)NULL)->fix_read( p_wb->wb_p_crs->crs_s_path, s, 0 ); 
	if( p_lws ? !p_lws->fix_p_alloc : true )
		return -1;

	p_gim->dim = dim;
				 	
	char	*p_src = p_lws->fix_p_alloc,
		*p_end = p_src + GPD_STRLEN( p_lws->fix_p_alloc ), *p_atrib;

	GPC_DICK* p_dick = p_wb->p_lws_com;
	GPE_LWS_COM com;
	I8 n_obj = -1, n_bone = -1, n_load, pite_id = 0, mom_id = 0, i8;
	//if( !p_gim->track_p_alloc )
	//{
	//	p_gim->track_n_alloc = 0x18;
	//	p_gim->track_p_alloc = (GPC_TRACKx64*)memset( new GPC_TRACKx64[p_gim->track_n_alloc], 0, p_gim->track_n_alloc*sizeof(*p_gim->track_p_alloc) );
	//}
	GPC_TRACKx64	*p_mom = NULL,
			*p_track = NULL;
	GPC_CNLx64* p_cnl = NULL;
	p_gim->track_p_load = p_gim->track_p_alloc;
	bool b_off = false;
	//GPCV2 key;
	while( p_src < p_end )
	{
		p_src += GPD_NINCS( p_src, " \t\r\n" );
		p_atrib = p_src+GPD_VAN( p_src, " \t\r\n" );
		p_dick->dick_find( p_src, p_atrib-p_src );
		if( !p_dick->p_find )
		{
			p_atrib += GPD_VAN( p_atrib, "\r\n" );
			p_atrib += GPD_NINCS( p_atrib, "\r\n" );
			p_src = p_atrib;
			continue;
		}

		p_atrib += GPD_NINCS( p_atrib, " \t\r\n" );
		com = (GPE_LWS_COM)p_dick->p_find->n_id;
		switch( com )
		{
			case GPE_LWS_COM_LoadObjectLayer:
				// layer id;
				i8 = strtol( p_atrib, &p_atrib, 10 );
				p_atrib += GPD_NINCS( p_atrib, " \t" );
				n_obj++;
				if( p_gim->track_n_alloc < ((n_load = p_gim->track_p_load-p_gim->track_p_alloc)+1) )// a régi kerül az n_load-ba és  +1-gyel hasonlitja össze
				{
					I8 n_old = p_gim->track_n_alloc;
					GPC_TRACKx64* p_old = p_gim->track_p_alloc;
					p_gim->track_n_alloc += 0x10;
					p_gim->track_p_alloc = new GPC_TRACKx64[p_gim->track_n_alloc];
					if( n_old )
					{
						gp_memcpy( p_gim->track_p_alloc, p_old, n_old*sizeof(*p_gim->track_p_alloc) );
						memset( p_old, 0, n_old*sizeof(*p_gim->track_p_alloc) ); // hogy nehogy destruktor kinyírja a trak[t].CNL[x].p_key-eket
					
					}
					memset( p_gim->track_p_alloc+n_old, 0, (p_gim->track_n_alloc-n_old)*sizeof(*p_gim->track_p_alloc) );
					p_gim->track_p_load = p_gim->track_p_alloc+n_load;
					GPM_DELARRAY( p_old );
				}
				p_track = p_gim->track_p_load;
				p_track->clear();
				
				p_track->item_id = ( 0x10000000 | ( 0xffff & n_obj ) );

				GPF_CON_FORMAT_Write( "AddObj %d, %d", n_load, n_obj );
				p_atrib += GPF_STR_VISZ( p_atrib, p_atrib+GPD_VAN( p_atrib, " \t\r\n" ), " \a\t:\r\n\\/" );
				p_atrib += GPD_NINCS( p_atrib, " \a\t:\r\n\\/" );
				p_src = p_atrib;
				p_atrib += GPD_VAN( p_atrib, " \t\r\n\a/" );
				(gp_strncpy( p_track->s_name, p_src, p_atrib-p_src ))[p_atrib-p_src] = 0;


				b_off = false;
				p_gim->track_p_load++;
				break;
			case GPE_LWS_COM_AddBone:
				n_bone++;
				if( p_gim->track_n_alloc < ((n_load = p_gim->track_p_load-p_gim->track_p_alloc)+1) )
				{
					I8 n_old = p_gim->track_n_alloc;
					GPC_TRACKx64* p_old = p_gim->track_p_alloc;
					p_gim->track_n_alloc += 0x10;
					p_gim->track_p_alloc = new GPC_TRACKx64[p_gim->track_n_alloc];
					if( n_old )
					{
						gp_memcpy( p_gim->track_p_alloc, p_old, n_old*sizeof(*p_gim->track_p_alloc) );
						memset( p_old, 0, n_old*sizeof(*p_gim->track_p_alloc) ); // hogy nehogy destruktor kinyírja a trak[t].CNL[x].p_key-eket
					
					}
					memset( p_gim->track_p_alloc+n_old, 0, (p_gim->track_n_alloc-n_old)*sizeof(*p_gim->track_p_alloc) );
					p_gim->track_p_load = p_gim->track_p_alloc+n_load;
					GPM_DELARRAY( p_old );
				}
				p_track = p_gim->track_p_load;
				p_track->clear();

				
				p_track->item_id = ( 0x40000000 | (( 0x0fff & n_bone ) << 16) | ( 0xffff & n_obj ) ); 
				
				GPF_CON_FORMAT_Write( "AddBone %d, %d", n_load, n_bone );
				
				b_off = false;
				p_gim->track_p_load++;
				p_src = p_atrib;
				continue;
			case GPE_LWS_COM_AddNullObject:
				b_off = true;
				break;
			case GPE_LWS_COM_AddLight:
				b_off = true;
				break;
			case GPE_LWS_COM_AddCamera:
				b_off = true;
				break;
			case GPE_LWS_COM_NumChannels:
				if( b_off )
					break;

				
				break;
			case GPE_LWS_COM_Channel:
				if( b_off )
					break;

				i8 = strtol( p_atrib, &p_atrib, 10 );
				p_cnl = NULL;
				if( i8 > 8 )
					break;

				if( p_track )
				{
					p_cnl = p_track->a_cnl+i8;		
				}
				break;
			case GPE_LWS_COM_C_open:
				if( b_off )
				{
					p_atrib += GPD_VAN( p_atrib, "}" );
					if( *p_atrib == '}' )
						p_atrib++;
					break;
				}
				p_src = p_atrib;
				continue;
			case GPE_LWS_COM_Envelope:
				if( b_off )
					break;
				if( !p_cnl )
				{
					b_off = true;
					break;
				}
				i8 = strtol( p_atrib, &p_atrib, 10 );
				if( p_cnl->n_key != i8 )
				{
					p_cnl->n_key = i8;
					GPM_DELARRAY( p_cnl->p_key )
				}

				if( i8 > 0 && !p_cnl->p_key )
					p_cnl->p_key = new GPCV2[p_cnl->n_key];
				
				p_cnl->p_load = p_cnl->p_key;
				break;
			case GPE_LWS_COM_Key:
				if( b_off )
					break;
				if( p_cnl->p_load < p_cnl->p_key ) 
					p_cnl->p_load = p_cnl->p_key;
				else if( p_cnl->p_load >= p_cnl->p_key+p_cnl->n_key )
				{
					p_cnl->p_load = p_cnl->p_key+p_cnl->n_key;
					break;
				}

				p_cnl->p_load->x = strtod( p_atrib, &p_atrib );
				p_atrib += GPD_VAN( p_atrib, GPD_09PM );
				p_cnl->p_load->y = strtod( p_atrib, &p_atrib );

				if( p_cnl-p_track->a_cnl < 3 )
					p_cnl->p_load->x *= dim;

				p_cnl->p_load++;
				break;
			case GPE_LWS_COM_C_close:
				if( b_off )
					break;
				p_src = p_atrib;
				continue;
			case GPE_LWS_COM_ParentItem:
				if( !b_off )
				{
					I8 pite = strtol( p_atrib, &p_atrib, 16 );
					p_mom = p_track;
					while( p_gim->track_p_alloc <= p_mom )
					{
						if( p_mom->item_id == pite )
							break;
						p_mom--;
					}
					if( p_mom < p_gim->track_p_alloc )
						p_mom = NULL;
					else {
						p_track->mom_id = p_mom-p_gim->track_p_alloc;
						p_track->n_level = p_mom->n_level+1;
						p_mom = NULL;
					}
				}
				break;
			case GPE_LWS_COM_BoneName:
				if( b_off )
					break;
				p_src = p_atrib;
				p_atrib += GPD_VAN( p_atrib, " \t\r\n\a" );
				(gp_strncpy( p_track->s_name, p_src, p_atrib-p_src ))[p_atrib-p_src] = 0;
				break;
			case GPE_LWS_COM_BoneRestPosition:
				if( b_off )
					break;
				p_track->a_rst[0].x = strtod( p_atrib, &p_atrib ); 
				p_atrib += GPD_VAN( p_atrib, GPD_09PM );
				p_track->a_rst[0].y = strtod( p_atrib, &p_atrib ); 
				p_atrib += GPD_VAN( p_atrib, GPD_09PM );
				p_track->a_rst[0].z = strtod( p_atrib, &p_atrib ); 
				p_atrib += GPD_VAN( p_atrib, GPD_09PM );
				
				p_track->a_rst[0] *= dim;
				break;
			case GPE_LWS_COM_BoneRestDirection:
				if( b_off )
					break;
				p_track->a_rst[1].x = strtod( p_atrib, &p_atrib ); 
				p_atrib += GPD_VAN( p_atrib, GPD_09PM );
				p_track->a_rst[1].y = strtod( p_atrib, &p_atrib ); 
				p_atrib += GPD_VAN( p_atrib, GPD_09PM );
				p_track->a_rst[1].z = strtod( p_atrib, &p_atrib ); 
				p_atrib += GPD_VAN( p_atrib, GPD_09PM );

				p_track->a_rst[1] *= PI/180.0f;
				break;
			case GPE_LWS_COM_BoneRestLength:
				if( b_off )
					break;
				break;
		}

		p_atrib += GPD_VAN( p_atrib, "\r\n" );
		//p_atrib += GPD_NINCS( p_atrib, "\r\n" );
		p_src = p_atrib;
	}
	char	s_tab[] = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t",
		*p_tab = s_tab+sizeof(s_tab)-1;

	for( I8 i = 0; i < p_gim->track_p_load-p_gim->track_p_alloc; i++ )
	{
		p_track = p_gim->track_p_alloc+i;
		p_track->item_id = i;
		GPF_CON_FORMAT_Write( "\n%.4d %.4d%s%s", p_track->item_id, p_track->mom_id, p_tab-p_track->n_level, p_track->s_name );
	}
	p_gim->n_load = 1;
	GPM_DEL( p_lws );
	return p_gim->gim_id;
}

GPC_RES* GPC_RES::fun_lws( GPC_WBx64* p_wb )
{
	// load LightWave3D object
	// .res ==	lwo( "\valami.lwo", gio_dim )
	char	*p_file = NULL;
	GPC_RES	*p_out = NULL,
		*p_a = this;
	I8	n_para = 0, n_qc = 0;
	GPC_AN	an_dst( 0, 0 );
	float dim = 1000.0f;
	if( res_type == GPE_NET_res )
	{
		GPC_RES	**pp_s = (GPC_RES**)fix_p_alloc,
			**pp_r = pp_s,
			**pp_e = (GPC_RES**)(fix_p_alloc + fix_n_alloc);
		while (pp_r < pp_e)
		{
			if (!*pp_r)
			{
				pp_r++;
				continue;
			}
			p_a = *pp_r;
			switch( p_a->res_type )
			{
				case GPE_NET_str:
					p_file = p_a->fix_p_alloc;
					break;
				default:
					
					switch( n_para = pp_r-pp_s )
					{
						case 1:
							n_qc = GPD_IF_THIS( p_a, i8(), 0 );
							break;
						case 2:
							dim = GPD_IF_THIS( p_a, f4(), 0.0 );
						default:
							pp_r = pp_e;
 					}
			}
			p_a = NULL;
			pp_r++;
		}
	}
	else if( res_type == GPE_NET_str )
	{
		p_file = fix_p_alloc;
	}

	if( !p_file )
		return NULL;

	I8 gim_id;
	GPC_GIMx64* p_gim = p_wb->GP_WBx64_GIM_Creat( p_file );
	if( p_gim )
	{
		if( p_gim->dim == dim )
		if( n_qc%2 )
		if( p_gim->n_load >= n_qc )
			return ((GPC_RES*)NULL)->res_equ( p_gim->gim_id );
	} else
		return ((GPC_RES*)NULL)->res_equ( (I8)-1 );

	p_gim->dim = dim;

	if( !p_wb->p_lws_com )
	{
		p_wb->p_lws_com = p_wb->p_lws_com->dick_add( gp_s_lws );
		if( !p_wb->p_lws_com )
			return ((GPC_RES*)NULL)->res_equ( (I8)-1 );
	}

	GPM_STRCPY( p_wb->wb_p_crs->crs_p_path, p_gim->s_file );
	ULL s = 0;
	GPC_FIX* p_lws = ((GPC_FIX*)NULL)->fix_read( p_wb->wb_p_crs->crs_s_path, s, 0 ); 
	if( p_lws ? !p_lws->fix_p_alloc : true )
		return ((GPC_RES*)NULL)->res_equ( (I8)-1 );

	char	*p_src = p_lws->fix_p_alloc,
		*p_end = p_src + GPD_STRLEN( p_lws->fix_p_alloc ), *p_atrib;

	GPC_DICK* p_dick = p_wb->p_lws_com;
	GPE_LWS_COM com;
	I8 n_obj = -1, n_bone = -1, n_load, pite_id = 0, mom_id = 0, i8;
	//if( !p_gim->track_p_alloc )
	//{
	//	p_gim->track_n_alloc = 0x18;
	//	p_gim->track_p_alloc = (GPC_TRACKx64*)memset( new GPC_TRACKx64[p_gim->track_n_alloc], 0, p_gim->track_n_alloc*sizeof(*p_gim->track_p_alloc) );
	//}
	GPC_TRACKx64	*p_mom = NULL,
			*p_track = NULL;
	GPC_CNLx64* p_cnl = NULL;
	p_gim->track_p_load = p_gim->track_p_alloc;
	bool b_off = false;
	//GPCV2 key;
	while( p_src < p_end )
	{
		p_src += GPD_NINCS( p_src, " \t\r\n" );
		p_atrib = p_src+GPD_VAN( p_src, " \t\r\n" );
		p_dick->dick_find( p_src, p_atrib-p_src );
		if( !p_dick->p_find )
		{
			p_atrib += GPD_VAN( p_atrib, "\r\n" );
			p_atrib += GPD_NINCS( p_atrib, "\r\n" );
			p_src = p_atrib;
			continue;
		}

		p_atrib += GPD_NINCS( p_atrib, " \t\r\n" );
		com = (GPE_LWS_COM)p_dick->p_find->n_id;
		switch( com )
		{
			case GPE_LWS_COM_LoadObjectLayer:
				// layer id;
				i8 = strtol( p_atrib, &p_atrib, 10 );
				p_atrib += GPD_NINCS( p_atrib, " \t" );
				n_obj++;
				if( p_gim->track_n_alloc < ((n_load = p_gim->track_p_load-p_gim->track_p_alloc)+1) )// a régi kerül az n_load-ba és  +1-gyel hasonlitja össze
				{
					I8 n_old = p_gim->track_n_alloc;
					GPC_TRACKx64* p_old = p_gim->track_p_alloc;
					p_gim->track_n_alloc += 0x10;
					p_gim->track_p_alloc = new GPC_TRACKx64[p_gim->track_n_alloc];
					if( n_old )
					{
						gp_memcpy( p_gim->track_p_alloc, p_old, n_old*sizeof(*p_gim->track_p_alloc) );
						memset( p_old, 0, n_old*sizeof(*p_gim->track_p_alloc) ); // hogy nehogy destruktor kinyírja a trak[t].CNL[x].p_key-eket
					
					}
					memset( p_gim->track_p_alloc+n_old, 0, (p_gim->track_n_alloc-n_old)*sizeof(*p_gim->track_p_alloc) );
					p_gim->track_p_load = p_gim->track_p_alloc+n_load;
					GPM_DELARRAY( p_old );
				}
				p_track = p_gim->track_p_load;
				p_track->clear();
				
				p_track->item_id = ( 0x10000000 | ( 0xffff & n_obj ) );

				GPF_CON_FORMAT_Write( "AddObj %d, %d", n_load, n_obj );
				p_atrib += GPF_STR_VISZ( p_atrib, p_atrib+GPD_VAN( p_atrib, " \t\r\n" ), " \a\t:\r\n\\/" );
				p_atrib += GPD_NINCS( p_atrib, " \a\t:\r\n\\/" );
				p_src = p_atrib;
				p_atrib += GPD_VAN( p_atrib, " \t\r\n\a/" );
				(gp_strncpy( p_track->s_name, p_src, p_atrib-p_src ))[p_atrib-p_src] = 0;


				b_off = false;
				p_gim->track_p_load++;
				break;
			case GPE_LWS_COM_AddBone:
				n_bone++;
				if( p_gim->track_n_alloc < ((n_load = p_gim->track_p_load-p_gim->track_p_alloc)+1) )
				{
					I8 n_old = p_gim->track_n_alloc;
					GPC_TRACKx64* p_old = p_gim->track_p_alloc;
					p_gim->track_n_alloc += 0x10;
					p_gim->track_p_alloc = new GPC_TRACKx64[p_gim->track_n_alloc];
					if( n_old )
					{
						gp_memcpy( p_gim->track_p_alloc, p_old, n_old*sizeof(*p_gim->track_p_alloc) );
						memset( p_old, 0, n_old*sizeof(*p_gim->track_p_alloc) ); // hogy nehogy destruktor kinyírja a trak[t].CNL[x].p_key-eket
					
					}
					memset( p_gim->track_p_alloc+n_old, 0, (p_gim->track_n_alloc-n_old)*sizeof(*p_gim->track_p_alloc) );
					p_gim->track_p_load = p_gim->track_p_alloc+n_load;
					GPM_DELARRAY( p_old );
				}
				p_track = p_gim->track_p_load;
				p_track->clear();

				
				p_track->item_id = ( 0x40000000 | (( 0x0fff & n_bone ) << 16) | ( 0xffff & n_obj ) ); 
				
				GPF_CON_FORMAT_Write( "AddBone %d, %d", n_load, n_bone );
				
				b_off = false;
				p_gim->track_p_load++;
				p_src = p_atrib;
				continue;
			case GPE_LWS_COM_AddNullObject:
				b_off = true;
				break;
			case GPE_LWS_COM_AddLight:
				b_off = true;
				break;
			case GPE_LWS_COM_AddCamera:
				b_off = true;
				break;
			case GPE_LWS_COM_NumChannels:
				if( b_off )
					break;

				
				break;
			case GPE_LWS_COM_Channel:
				if( b_off )
					break;

				i8 = strtol( p_atrib, &p_atrib, 10 );
				p_cnl = NULL;
				if( i8 > 8 )
					break;

				if( p_track )
				{
					p_cnl = p_track->a_cnl+i8;		
				}
				break;
			case GPE_LWS_COM_C_open:
				if( b_off )
				{
					p_atrib += GPD_VAN( p_atrib, "}" );
					if( *p_atrib == '}' )
						p_atrib++;
					break;
				}
				p_src = p_atrib;
				continue;
			case GPE_LWS_COM_Envelope:
				if( b_off )
					break;
				if( !p_cnl )
				{
					b_off = true;
					break;
				}
				i8 = strtol( p_atrib, &p_atrib, 10 );
				if( p_cnl->n_key != i8 )
				{
					p_cnl->n_key = i8;
					GPM_DELARRAY( p_cnl->p_key )
				}

				if( i8 > 0 && !p_cnl->p_key )
					p_cnl->p_key = new GPCV2[p_cnl->n_key];
				
				p_cnl->p_load = p_cnl->p_key;
				break;
			case GPE_LWS_COM_Key:
				if( b_off )
					break;
				if( p_cnl->p_load < p_cnl->p_key ) 
					p_cnl->p_load = p_cnl->p_key;
				else if( p_cnl->p_load >= p_cnl->p_key+p_cnl->n_key )
				{
					p_cnl->p_load = p_cnl->p_key+p_cnl->n_key;
					break;
				}

				p_cnl->p_load->x = strtod( p_atrib, &p_atrib );
				p_atrib += GPD_VAN( p_atrib, GPD_09PM );
				p_cnl->p_load->y = strtod( p_atrib, &p_atrib );

				if( p_cnl-p_track->a_cnl < 3 )
					p_cnl->p_load->x *= dim;

				p_cnl->p_load++;
				break;
			case GPE_LWS_COM_C_close:
				if( b_off )
					break;
				p_src = p_atrib;
				continue;
			case GPE_LWS_COM_ParentItem:
				if( !b_off )
				{
					I8 pite = strtol( p_atrib, &p_atrib, 16 );
					p_mom = p_track;
					while( p_gim->track_p_alloc <= p_mom )
					{
						if( p_mom->item_id == pite )
							break;
						p_mom--;
					}
					if( p_mom < p_gim->track_p_alloc )
						p_mom = NULL;
					else {
						p_track->mom_id = p_mom-p_gim->track_p_alloc;
						p_track->n_level = p_mom->n_level+1;
						p_mom = NULL;
					}
				}
				break;
			case GPE_LWS_COM_BoneName:
				if( b_off )
					break;
				p_src = p_atrib;
				p_atrib += GPD_VAN( p_atrib, " \t\r\n\a" );
				(gp_strncpy( p_track->s_name, p_src, p_atrib-p_src ))[p_atrib-p_src] = 0;
				break;
			case GPE_LWS_COM_BoneRestPosition:
				if( b_off )
					break;
				p_track->a_rst[0].x = strtod( p_atrib, &p_atrib ); 
				p_atrib += GPD_VAN( p_atrib, GPD_09PM );
				p_track->a_rst[0].y = strtod( p_atrib, &p_atrib ); 
				p_atrib += GPD_VAN( p_atrib, GPD_09PM );
				p_track->a_rst[0].z = strtod( p_atrib, &p_atrib ); 
				p_atrib += GPD_VAN( p_atrib, GPD_09PM );
				
				p_track->a_rst[0] *= dim;
				break;
			case GPE_LWS_COM_BoneRestDirection:
				if( b_off )
					break;
				p_track->a_rst[1].x = strtod( p_atrib, &p_atrib ); 
				p_atrib += GPD_VAN( p_atrib, GPD_09PM );
				p_track->a_rst[1].y = strtod( p_atrib, &p_atrib ); 
				p_atrib += GPD_VAN( p_atrib, GPD_09PM );
				p_track->a_rst[1].z = strtod( p_atrib, &p_atrib ); 
				p_atrib += GPD_VAN( p_atrib, GPD_09PM );

				p_track->a_rst[1] *= PI/180.0f;
				break;
			case GPE_LWS_COM_BoneRestLength:
				if( b_off )
					break;
				break;
		}

		p_atrib += GPD_VAN( p_atrib, "\r\n" );
		//p_atrib += GPD_NINCS( p_atrib, "\r\n" );
		p_src = p_atrib;
	}
	char	s_tab[] = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t",
		*p_tab = s_tab+sizeof(s_tab)-1;

	for( I8 i = 0; i < p_gim->track_p_load-p_gim->track_p_alloc; i++ )
	{
		p_track = p_gim->track_p_alloc+i;
		p_track->item_id = i;
		GPF_CON_FORMAT_Write( "\n%.4d %.4d%s%s", p_track->item_id, p_track->mom_id, p_tab-p_track->n_level, p_track->s_name );
	}
	p_gim->n_load = n_qc;

	GPM_DEL( p_lws );

	return ((GPC_RES*)NULL)->res_equ( p_gim->gim_id );
}

GPC_RES* GPC_RES::fun_track( GPC_CRS& crs, GPC_TAB* p_tab )
{
	//. == orbit2( item_A, item_N, dir.xyz, turn.xyz, action_id )
	if( !this )
		return NULL;
	
	if( res_type != GPE_NET_res )
		return NULL;
	//--------------------------------------------
	//
	//		PARAMÉTEREK BEOLVASÁSA
	//	gis_id, item_id, [dir x,y,z], [rot y,p,r], action_id
	//
	//-------------------------------------------
	GPC_RES	**pp_s = (GPC_RES**)fix_p_alloc,
		**pp_r = pp_s,
		**pp_e = (GPC_RES**)(fix_p_alloc+fix_n_alloc);
	
	I8	n_para = 0, gim_id = -1, start_id = -1, n_row = 0;
	float	sec = 0.0f, f, dim = 1000.0f;
	GPC_AN	item_AN;	
	GPCV3	v_dir( 0.0f, 0.0f, 0.0f ),
		r_ypr( 0.0f, 0.0f, 0.0f );

	GPC_RES *p_a, *p_tack_id = NULL, *p_sec = NULL;
	while( pp_r < pp_e )
	{
		if (!*pp_r)
		{
			pp_r++;
			continue;
		}
		p_a = *pp_r;
		switch( n_para = pp_r-pp_s )
		{
			case 0:	// gim_id
				gim_id = GPD_IF_THIS( p_a, i8(), 0 );
				break;
			
			case 1:	
				start_id = GPD_IF_THIS( p_a, i8(), 0 );
				break;
			case 2:	
				n_row = GPD_IF_THIS( p_a, i8(), 0 );
				if( n_row > 0 )
					break;
				return NULL;

			case 3:
				sec = GPD_IF_THIS( p_a, f4(), 0.0 );
				break;

			case 4:	// item_A
				item_AN.a = (GPE_ALFA)GPD_IF_THIS( p_a, i8(), 0 );
				break;
			case 5:	// item_N
				item_AN.n = GPD_IF_THIS( p_a, i8(), 0 );
				break;
			default:
				pp_e = pp_r;
		}
		p_a = NULL;
		pp_r++;
	}
	
	GPC_WBx64* p_wb = crs.crs_p_wb;
	if( gim_id < 0 || gim_id >= p_wb->n_gim )
		return NULL;
		
	GPC_GIMx64* p_gim = p_wb->pp_gim[gim_id];
	if( !p_gim )
		return NULL;
	
	
	I8 item_id, mom_id, gio_id;

	GPC_TAB *p_item_row = crs.crs_tab_an( item_AN ), *p_tb;	// megkereste az item sorát
	if( !p_item_row )
		return NULL;
	
	if( n_row > p_gim->track_p_load-p_gim->track_p_alloc )
	{
		n_row = p_gim->track_p_load-p_gim->track_p_alloc;
	}

	for( GPC_TRACKx64 *p_t = p_gim->track_p_alloc, *p_te = p_gim->track_p_alloc+n_row; p_t < p_te; p_t++ )
	{
		p_tb = p_item_row;
		while( p_tb )
		{
			if( !p_tb->tab_p_exe )
			{
				// csak akkor írja felül ha nincsen benne valami program hagyjuk hagy számolja az
				switch( n_para = p_tb-p_item_row )
				{
					case 0:
						item_id = p_t->item_id+start_id;
						if( GPD_IF_THIS( p_tb->tab_run( crs ), i8(), 0 ) != item_id )
						{
							p_tb->tab_p_res = p_tb->tab_p_res->res_equ( item_id );
							p_tb->tab_irq_res(crs);
						}
						break;
					case 1:
						mom_id = p_t->mom_id+start_id;
						if( item_id == mom_id )
							break;	// ez olyan elem ami magára hivatkozik
						if( GPD_IF_THIS( p_tb->tab_run( crs ), i8(), 0 ) != mom_id )
						{
							p_tb->tab_p_res = p_tb->tab_p_res->res_equ( mom_id );
							p_tb->tab_irq_res(crs);
						}
						break;
					case 2:
						// gio_id egyenlõre hagyjuk inkább békén
						break;
					default: //pxyz 345 dyaw 678 swhd 901 rpxyz 234 rdypr 567
						if( n_para > 17 )
						{
							p_tb = NULL;
							continue; // megszakítjuk a sort elég volt, icse több adat
						}
						else if( n_para < 12 )
							f = p_t->a_cnl[n_para-3].GPC_CNLx64_find( sec );
						else {
							float* p_f = (float*)p_t->a_rst;
							f = p_f[n_para-12];
						}
						p_a = p_tb->tab_run( crs );
						if( p_a ? (GPD_IF_THIS( p_a, f4(), 0.0 ) != f) : true )
						{
							p_tb->tab_p_res = p_tb->tab_p_res->res_equ( f );
							p_tb->tab_irq_res(crs);
						}
				}


			}
			if( !p_tb->b_right)
			{
				break;
			}
			p_tb++;
		}
		
		if( !p_item_row->tab_p_down )
			break;
		p_item_row = p_item_row->tab_p_down;
	}

	return ((GPC_RES*)NULL)->res_equ( sec );
}
GPC_RES* GPC_RES::fun_gis( GPC_WBx64* p_wb )
{
	GPC_RES* p_out = NULL, *p_a = this;
	char	*p_file = NULL;

	GPCV3	cam_pos( 0.0f, 1.0f,-1.0f ),
		cam_trg( 0.0f, 0.0f, 0.0f ),
		cam_up( 0.0f, 1.0f, 0.0f );
	I8	n_para = 0;

	GPC_GISx64* p_gis = NULL;
	if( res_type == GPE_NET_res )
	{
		GPC_RES	**pp_s = (GPC_RES**)fix_p_alloc,
			**pp_r = pp_s,
			**pp_e = (GPC_RES**)(fix_p_alloc + fix_n_alloc);
		while (pp_r < pp_e)
		{
			if (!*pp_r)
			{
				pp_r++;
				continue;
			}
			p_a = *pp_r;
			switch( p_a->res_type )
			{
				case GPE_NET_str:
					p_file = p_a->fix_p_alloc;
					break;
				default:
					switch( n_para = pp_r-pp_s )
					{
						case 1:
							cam_pos.x = GPD_IF_THIS( p_a, f4(), 0.0 );
							break;
						case 2:
							cam_pos.y = GPD_IF_THIS( p_a, f4(), 0.0 );
							break;
						case 3:	
							cam_pos.z = GPD_IF_THIS( p_a, f4(), 0.0 );
							break;
						case 4:
							cam_trg.x = GPD_IF_THIS( p_a, f4(), 0.0 );
							break;
						case 5:
							cam_trg.y = GPD_IF_THIS( p_a, f4(), 0.0 );
							break;
						case 6:	
							cam_trg.z = GPD_IF_THIS( p_a, f4(), 0.0 );
							break;
						case 7:
							cam_up.x = GPD_IF_THIS( p_a, f4(), 0.0 );
							break;
						case 8:
							cam_up.y = GPD_IF_THIS( p_a, f4(), 0.0 );
							break;
						case 9:	
							cam_up.z = GPD_IF_THIS( p_a, f4(), 0.0 );
							break;
						default:
							pp_r = pp_e;
 					}
			}
			p_a = NULL;
			pp_r++;
		}
	}
	else if( res_type == GPE_NET_str )
	{
		p_file = fix_p_alloc;
	}

	if( !p_file )
		return NULL;

	I8 gis_id = p_wb->GPC_GISx64_load( p_file );
	if( gis_id > -1 && gis_id < p_wb->n_gis && n_para > 1 )
	{
		if( GPC_GISx64* p_gis = p_wb->pp_gis[gis_id] )
		{
			p_gis->b_use = true;
			if( (p_gis->cam_pos-cam_pos).qlen() > 0.01f )
			{
				p_gis->cam_pos = cam_pos;
				p_gis->cam_trg = cam_trg;
				p_gis->cam_up = cam_up;
				p_gis->mx_cbuff.b_upd = true;
			}
			else if( (p_gis->cam_trg-cam_trg).qlen() > 0.01f )
			{
				p_gis->cam_trg = cam_trg;
				p_gis->cam_up = cam_up;
				p_gis->mx_cbuff.b_upd = true;
			}
			else if( (p_gis->cam_up-cam_up).qlen() > 0.01f )
			{
				p_gis->cam_up = cam_up;
				p_gis->mx_cbuff.b_upd = true;
			}
		}
	}
	return ((GPC_RES*)NULL)->res_equ( gis_id );
}