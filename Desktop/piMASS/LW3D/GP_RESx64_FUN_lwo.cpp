#include "gpstd_x64.h"
//char* GPF_LW3D_Pad( char* p_data )
//{
//	if( (I8)p_data & 1 )
//		p_data++;
//	return p_data;
//}

GPC_RES* GPC_RES::fun_lwo( GPC_WBx64* p_wb )
{
	// load LightWave3D object
	// .res ==	lwo( "\valami.lwo", gio_dim )
	char	*p_file = NULL;
	GPC_RES	*p_out = NULL,
		*p_a = this;
	I8	n_para = 0;
	GPC_AN	an_dst( 0, 0 );
	float gio_dim = 1000.0f;
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
							gio_dim = GPD_IF_THIS( p_a, f4(), 0.0 );
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
	I8 gio_id;
	GPC_GIOx64* p_gio = p_wb->GP_WBx64_GIO_Creat( p_file );
	if( p_gio )
	{
		if( p_gio->gio_dim != gio_dim )
		{
			p_gio->gio_id = -1;
			p_gio->gio_reset();
		}		
		if( p_gio->gio_id > -1 )
		{
			
			return ((GPC_RES*)NULL)->res_equ( p_gio->gio_id );
		}
	} else 
		return ((GPC_RES*)NULL)->res_equ( (I8)-1 );
	
	ULL s = 0;
	GPM_STRCPY( p_wb->wb_p_crs->crs_p_path, p_gio->s_name );
	if( true )
	{
		char* p_ext = strrchr( p_wb->wb_p_crs->crs_p_path, '.' );
		if( p_ext )
		{
			GPM_STRCPY( p_ext, ".gio" );
			if( !GPF_ACE( p_wb->wb_p_crs->crs_s_path, 4 ) )
			{
				GPC_FIX* p_giofile = ((GPC_FIX*)NULL)->fix_read( p_wb->wb_p_crs->crs_s_path, s = 0, 0 );
				p_gio->gio_dim = gio_dim;

				p_gio->gio_read( NULL, NULL, p_giofile );
				GPM_DEL( p_giofile );
				
				p_gio->gio_id = p_wb->GP_WBx64_GIO_ID( p_gio );
				return ((GPC_RES*)NULL)->res_equ( p_gio->gio_id );

			}
			GPM_STRCPY( p_ext, ".lwo " );
		}
	}
	
	GPC_FIX* p_lwo = ((GPC_FIX*)NULL)->fix_read( p_wb->wb_p_crs->crs_s_path, s = 0, 0 ); 
	if( p_lwo ? !p_lwo->fix_p_alloc : true )
		return ((GPC_RES*)NULL)->res_equ( (I8)-1 );

	p_gio->gio_dim = gio_dim;


	char	*p_c = p_lwo->fix_p_alloc,
		*p_ce = p_c+p_lwo->fix_n_alloc;
	
	U4	chunk,
		null = 0,
		*p_u4, *p_u4e, n_step;

	p_u4 = (U4*)p_c;
	if( *p_u4 != LWO_ID_FORM )
	{
		GPM_DEL( p_lwo );
		return ((GPC_RES*)NULL)->res_equ( (I8)-1 );
	}
	p_u4++;
	n_step = GPF_SWAP_U4( (char*)p_u4 );
	p_u4++;

	if( *p_u4 != LWO_ID_LWO2 )
	{
		GPM_DEL( p_lwo );
		return ((GPC_RES*)NULL)->res_equ( (I8)-1 );
	}
	p_c = (char*)p_u4;
	p_ce = p_c + n_step;
	p_u4++;
	p_c = (char*)p_u4;

	GPC_LWO_TAG *p_tags = NULL, *p_kill_tags, *p_tag;

	char	//**pp_tags = NULL,
		**pp_kill_pic = NULL,
		**pp_pic = NULL,
		*p_d, *p_sub, *p_subsub;
	U4 n_tags = 0;
	char	n_bone = 0, n_bone_min = 127, n_bone_max = -127, bone_id;
		
	
	I8 n_p = 0, n_f = 0x10, n_mad = 0, n_pic = 1, n_surf = 0, n_surf_min = 1<<16; // hogy 1-tõl kezdje számolni
	GPC_GIOx64_LAY	*p_kill_lay, *p_lay = NULL;
	GPC_GIO_VX32CUFBI	*p_p = NULL, *p_pe = NULL, *p_pi;
	GPC_LWO_FACE	*p_f = new GPC_LWO_FACE[n_f], *p_fe = p_f;
	memset( p_f, 0, n_f*sizeof(*p_f) );
	bool b_bone = false;
	while( p_c < p_ce )
	{
		p_u4 = (U4*)p_c;
		chunk = *p_u4;
		p_u4++;
		n_step = GPF_SWAP_U4( (char*)p_u4 );
		p_u4++;
		p_c = (char*)p_u4+n_step;
		GPF_CON_FORMAT_Write( "\n%.4s n_byte:%d", (char*)&chunk, n_step );

		//p_d = (char*)(p_u4+1);
		p_d = (char*)p_u4;

		switch( chunk )
		{
			case LWO_ID_TAGS:
				while( p_d < p_c )
				{
					p_kill_tags = p_tags;
					p_tags = new GPC_LWO_TAG[n_tags+1];
					if( n_tags > 0 )
					{
						gp_memcpy( p_tags, p_kill_tags, n_tags*sizeof( GPC_LWO_TAG ) );
					}
					GPM_STRCPY( p_tags[n_tags].s_name, p_d );
					GPF_CON_FORMAT_Write( "\n\t%d. %s", n_tags, p_tags[n_tags].s_name );
					n_tags++;
					GPM_DELARRAY( p_kill_tags );
					p_d = GPF_LW3D_Pad( p_d + GPD_STRLEN( p_d )+1 );
				}
				break;
			case LWO_ID_LAYR:
				
				if( p_lay )
				{
					GPC_GIO_VX32CUFBI	*p_vx = p_lay->solid_vx.VX324ufbi_expand( p_pe-p_p ),
							*p_vxe = p_vx;
					gp_memcpy( p_vx, p_p, (p_pe-p_p)*sizeof(*p_p) );
					p_lay->solid_vx.VX324ufbi_add(p_pe-p_p);
					p_lay->lay_ready( p_f, p_fe, p_tags, n_bone_min, n_bone_max, pp_pic );
				}

				
				
				p_kill_lay = p_gio->p_gio_lay;
				p_gio->p_gio_lay = new GPC_GIOx64_LAY[p_gio->gio_n_lay+1];

				if( p_kill_lay )
					gp_memcpy( p_gio->p_gio_lay, p_kill_lay, p_gio->gio_n_lay*sizeof(GPC_GIOx64_LAY) );

				p_lay = p_gio->p_gio_lay+p_gio->gio_n_lay;
				
				p_gio->gio_n_lay++;
				
				p_lay->lay_clear();
				p_lay->lay_id = (GPF_SWAP_U4( (char*)p_u4 )&0xffff0000)>>16;
				p_lay->parent_id = GPF_SWAP_U4( (char*)p_u4 )&0xffff;
				p_lay->p_lay_gio = p_gio;

				p_u4++;
				p_d = (char*)p_u4;
				p_lay->pivot.XYZ( p_d, &p_d, gio_dim );
				GPM_STRCPY( p_lay->s_name, (char*)p_d );

				GPF_CON_FORMAT_Write( "\n LAYER: %s \n", p_lay->s_name );
				// RESET
				p_pe = p_p;
				p_fe = p_f;
				n_bone = 0;
				b_bone = false;
				break;
			case LWO_ID_PNTS:
				{
					if( n_p < n_step/(3*4) )
					{
						n_p = n_step/(3*4);
						GPM_DELARRAY( p_p );
						p_p = new GPC_GIO_VX32CUFBI[n_p];
					}
					p_pe = p_p;
					GPF_CON_FORMAT_Write( "\n n_p: %d \n", n_p );
					while( p_d < p_c )
					{
						memset( p_pe, 0, sizeof( *p_pe ) );  
						p_pe->pos.XYZ( p_d, &p_d, gio_dim );
						//GPF_CON_FORMAT_Write( "\t%0.3f,\t%0.3f,\t%0.3f,", p_pe->pos.x, p_pe->pos.y, p_pe->pos.z );
						p_pe++;
					}
					GPF_CON_FORMAT_Write( "\n n_p: %d n_e: %d \n", n_p, p_pe-p_p );
				}
				break;
			case LWO_ID_BBOX:
				if( !p_lay )
					break;

				p_lay->bbox[0].XYZ( p_d, &p_d, gio_dim );
				p_lay->bbox[1].XYZ( p_d, &p_d, gio_dim );
				p_lay->r = (p_lay->bbox[1]-p_lay->bbox[0]).len()/2.0f;
				GPF_CON_FORMAT_Write( "\n BBOX0 %0.3f, %0.3f, %0.3f,", p_lay->bbox[0].x, p_lay->bbox[0].y, p_lay->bbox[0].z );
				GPF_CON_FORMAT_Write( "\n BBOX1 %0.3f, %0.3f, %0.3f,", p_lay->bbox[1].x, p_lay->bbox[1].y, p_lay->bbox[1].z );
				break;
			case LWO_ID_POLS:
				p_u4 = (U4*)p_d;
				switch( *p_u4 )
				{
					case LWO_ID_FACE: 
						{	
							p_d += 4;
							while( p_d < p_c )
							{
								if( p_fe-p_f >= n_f )
								{
									GPF_CON_FORMAT_Write( "\n n_FACE: %d", p_fe-p_f );
									GPC_LWO_FACE* p_kill_f = p_f;
									p_f = (GPC_LWO_FACE*)gp_memcpy( new GPC_LWO_FACE[n_f*2], p_kill_f, n_f*sizeof(*p_f) );
									memset( p_f+n_f, 0, n_f*sizeof(*p_f) );
									p_fe = p_f+n_f;
									GPM_DELARRAY( p_kill_f );
									n_f *= 2;
								}

								p_fe->n_p = GPF_SWAP_U2( p_d ) & 0x03ff;
								p_d += 2;
								U4 i = 0;
								for( char *p_de = p_d+p_fe->n_p*2; p_d < p_de; p_d+=2 )
								{
									if( *p_d == -1 )
									{
										p_fe->a_idx[i] = GPF_SWAP_U4(p_d);
										p_d+=2;
									}
									else
										p_fe->a_idx[i] = GPF_SWAP_U2(p_d);
									i++;
								}
								if( p_fe->n_p > 0 )
								{
									GPCV3 norm = ((p_p[p_fe->a_idx[2]].pos-p_p[p_fe->a_idx[1]].pos)^(p_p[p_fe->a_idx[0]].pos-p_p[p_fe->a_idx[1]].pos)).normalize();
									for( U4* p_i = p_fe->a_idx, *p_ie = p_i + p_fe->n_p; p_i < p_ie; p_i++ )
									{
										p_p[*p_i].up += norm;
									}
								} else {
									GPF_CON_FORMAT_Write( "\n n_FACE: %lld n_p: %lld", p_fe-p_f, p_pe-p_p );
								}
								p_fe++;
							}
							for( GPC_GIO_VX32CUFBI *p_pi = p_p; p_pi < p_pe; p_pi++ )
							{
								p_pi->up = p_pi->up.normalize(); 
							}
						}
						GPF_CON_FORMAT_Write( "\n n_FACE: %lld n_p: %lld", p_fe-p_f, p_pe-p_p );
						break;
					case LWO_ID_BONE:
						b_bone = true;
						GPF_CON_FORMAT_Write( "\n n_BONE: %d", p_fe-p_f );
						break;
				}
				
				break;
			case LWO_ID_VMAP:
				{
					p_d += 4;
					U2 dim = GPF_SWAP_U2( p_d );
					U4 n_i, u4;
					float wght, *p_w;
					p_d += 2;
					char	s_name[0x100],
						n_n = sprintf( s_name, "%s", p_d );
					p_d = GPF_LW3D_Pad( p_d+n_n+1 );
					
					for( U4 i = 0; i < n_tags; i++ )
					{
						if( !_stricmp( s_name, p_tags[i].s_name ) )
						{
							n_bone = i;
							break;
						}
					}

					_strlwr( s_name );
					if( strstr( s_name, "bone" ) )
					{
						if( n_bone_min > n_bone )
							n_bone_min = n_bone;
						if( n_bone_max < n_bone )
							n_bone_max = n_bone;
					}

					bone_id = n_bone;
					p_wb->p_lws_bons->dick_find( s_name, GPD_STRLEN(s_name) );
					if( p_wb->p_lws_bons->p_find )
					{
						bone_id = p_wb->p_lws_bons->p_find->n_id;
					} else {
						bone_id = 1;
					}
					GPF_CON_FORMAT_Write( "\n %d %s", bone_id, s_name ); 
					
					switch( *p_u4 )
					{
						case LWO_ID_MNVW:
							break;
						case LWO_ID_WGHT:
							while( p_d < p_c )
							{
								if( *p_d == -1 )
								{
									n_i = GPF_SWAP_U4( p_d );
									p_d += 2;
								} else 
									n_i = GPF_SWAP_U2( p_d );
								p_d += 2;
								u4 = GPF_SWAP_U4( p_d );
								p_d += 4;

								wght = *(float*)&u4;
								if( abs(wght) < 0.05f )
								{
									continue;
								}
								if( abs(wght) < 0.15f )
								{
									continue;
								}

								p_pi = p_p+n_i;
								p_w = (float*)&p_pi->blend;
								for( char i = 0; i < 4; i++ )
								{
									if( abs(p_w[i]) > wght )
										continue;

									p_w[i] = wght;
									((char*)&p_pi->indices)[i] = bone_id+1;
									wght /= 2.0f;
								}
							}
							//n_bone++;
							break;
						case LWO_ID_TXUV:
							while( p_d < p_c )
							{
								if( *p_d == -1 )
								{
									n_i = GPF_SWAP_U4( p_d );
									p_d += 2;
								} else 
									n_i = GPF_SWAP_U2( p_d );
								p_d += 2;

								p_p[n_i].uv.XY( p_d, &p_d ); 
							}
						case LWO_ID_MORF:
							while( p_d < p_c )
							{
								if( *p_d == -1 )
								{
									n_i = GPF_SWAP_U4( p_d );
									p_d += 2;
								} else 
									n_i = GPF_SWAP_U2( p_d );
								p_d += 2;

								p_p[n_i].up.XYZ( p_d, &p_d, gio_dim ); 
							}
					}
				}
				break;
			case LWO_ID_VMAD:
				{
					p_d += 4;
					U2 dim = GPF_SWAP_U2( p_d ), i_point, n_face;
					U4 o_p = n_p;
					float wght, *p_w;
					p_d += 2;
					char	s_name[0x100],
						n_n = sprintf( s_name, "%s", p_d );
					p_d = GPF_LW3D_Pad( p_d+n_n+1 );
					for( U4 i = 0; i < n_tags; i++ )
					{
						if( !_stricmp( s_name, p_tags[i].s_name ) )
						{
							n_bone = i;
							break;
						}
					}
					GPF_CON_FORMAT_Write( "\n %d %s", n_bone, s_name ); 
					switch( *p_u4 )
					{
						case LWO_ID_MNVW:
							break;
						case LWO_ID_WGHT:
							break;
						case LWO_ID_TXUV:
							while( p_d < p_c )
							{
								i_point = GPF_SWAP_U2( p_d );
								p_d += 2;
								n_face = GPF_SWAP_U2( p_d );
								p_d += 2;

								
								if( p_pe-p_p >= n_p )
								{
									GPF_CON_FORMAT_Write( "\n n_FACE: %d", p_fe-p_f );
									GPC_GIO_VX32CUFBI* p_kill_p = p_p;
									p_p = (GPC_GIO_VX32CUFBI*)gp_memcpy( new GPC_GIO_VX32CUFBI[n_p*2], p_kill_p, n_p*sizeof(*p_p) );
									p_pe = p_p+n_p;
									GPM_DELARRAY( p_kill_p );
									n_p *= 2;
								}
								*p_pe = p_p[i_point];
								p_pe->uv.XY( p_d, &p_d );
								//p_f[i_poly].a_idx[idx] = p_pe-p_p;

								for( U2 i = 0; i < p_f[n_face].n_p; i++ )
								{
									if( p_f[n_face].a_idx[i] == i_point )
										p_f[n_face].a_idx[i] = p_pe-p_p;
								}

								p_pe++;
								n_mad++;
							}
							GPF_CON_FORMAT_Write( "\n n_FACE: %lld n_p: %lld n_vx: %lld  n_mad: %lld", p_fe-p_f, p_pe-p_p, o_p, n_mad );
							break;
						case LWO_ID_MORF:
							break;
					}
				}
			
				break;
			case LWO_ID_CLIP:
				if( p_u4[1] != LWO_ID_STIL )
					break;
				pp_kill_pic = pp_pic;
				pp_pic = new char*[n_pic+1];
				if( pp_kill_pic )
				{
					gp_memcpy( pp_pic, pp_kill_pic, n_pic*sizeof(*pp_pic) );
				}
				pp_pic[n_pic] = p_d+10 + GPF_STR_VISZ( p_d+10, p_d+10+GPD_STRLEN( p_d+10 ), "/:" );
				if( *pp_pic[n_pic] == '/' || *pp_pic[n_pic] == ':' )
				{
					*pp_pic[n_pic] = '\\';
				}
				GPF_CON_FORMAT_Write( "%d %lld Clip %s", GPF_SWAP_U4( p_d ), n_pic, pp_pic[n_pic] );
				// oda kell figyelni a LightWave 1-tõl számítja
				n_pic++;
				GPM_DELARRAY( pp_kill_pic );
				break;
			case LWO_ID_SURF:
				{
					char	s_name[0x100],
						n_n = sprintf( s_name, "%s", p_d );
					p_d = GPF_LW3D_Pad( p_d+n_n+1 );
					p_d = GPF_LW3D_Pad( p_d+1 );
					for( U4 i = n_surf_min; i < n_tags; i++ )
					{
						if( !_stricmp( s_name, p_tags[i].s_name ) )
						{
							n_surf = i;
							break;
						}
					}
					p_tag = p_tags+n_surf;
					_strlwr( s_name );
					GPF_CON_FORMAT_Write( "\n%s %d SURF  %s", s_name, n_surf, p_tag->s_name );

					while( p_d < p_c )
					{

						chunk = *(U4*)p_d;
						p_d += sizeof(U4);
						n_step = GPF_SWAP_U2( p_d );
						p_d += sizeof(U2);
						p_sub = p_d;
						p_d += n_step;
						GPF_CON_FORMAT_Write( "\n\t%.4s n_byte:%d", (char*)&chunk, n_step );
						switch( chunk )
						{
							case LWO_ID_BLOK:
								while( p_sub < p_c )
								{
									chunk = *(U4*)p_sub;
									p_sub += sizeof(U4);
									n_step = GPF_SWAP_U2( p_sub );
									p_sub += sizeof(U2);
									p_subsub = p_sub;
									p_sub += n_step;
									GPF_CON_FORMAT_Write( "\n\t\t%.4s n_byte:%d", (char*)&chunk, n_step );
									if( chunk == LWO_ID_IMAG )
									{
										p_tag->a_pic[p_tag->n_p] = GPF_SWAP_U2( p_subsub );
										GPF_CON_FORMAT_Write( "\n\t\t\t%s:%s", p_tag->s_name, pp_pic[p_tag->a_pic[p_tag->n_p]] );
										p_tag->n_p++;
										break;
									}

								}
						}

					}
				}
				break;
			case LWO_ID_PTAG:
				{
					U4 n_ptag = 0, ptag = -1, n_face;
					p_d += 4;
					if( b_bone )
						break;
					switch( *p_u4 )
					{
						case LWO_ID_COLR:
							while( p_d < p_c )
							{
								n_face = GPF_SWAP_U2( p_d );//&0x03ff;
								p_d += 2;
								p_f[n_face].colr = GPF_SWAP_U2( p_d );
								if( ptag != p_f[n_face].colr )
								{
									ptag = p_f[n_face].colr;
									GPF_CON_FORMAT_Write( "\n %d %s", n_ptag, p_tags[ptag].s_name );
								}
								p_d += 2;
								n_ptag++;
							}
						case LWO_ID_SURF:
							while( p_d < p_c )
							{
								n_face = GPF_SWAP_U2( p_d );//&0x03ff;
								p_d += 2;
								if( p_f[n_face].n_surf )
								{
									p_f[n_face].surf = GPF_SWAP_U2( p_d );
									p_f[n_face].n_surf++;
								} else {
									p_f[n_face].surf = GPF_SWAP_U2( p_d );
									p_f[n_face].n_surf++;
								}
								p_d += 2;
								if( ptag != p_f[n_face].surf )
								{
									ptag = p_f[n_face].surf;
									if( n_surf_min > ptag )
										n_surf_min = ptag;
									GPF_CON_FORMAT_Write( "\n %d %s", n_ptag, p_tags[ptag].s_name );
								}
								n_ptag++;
							}
						case LWO_ID_PART:
							while( p_d < p_c )
							{
								n_face = GPF_SWAP_U2( p_d );
								p_d += 2;
								if( !p_f[n_face].n_part )
								{
									p_f[n_face].part = GPF_SWAP_U2( p_d );
									p_f[n_face].n_part++;
								}
								p_d += 2;
								if( ptag != p_f[n_face].part )
								{
									ptag = p_f[n_face].part;
									GPF_CON_FORMAT_Write( "\n %d %s", n_ptag, p_tags[ptag].s_name );
									
								}
								n_ptag++;
							}
					}
					GPF_CON_FORMAT_Write( "\n n_PTAG: %d", n_ptag );
				}
				break;
		}
		
	}
	
	if( p_lay )
	{
		GPC_GIO_VX32CUFBI	*p_vx = p_lay->solid_vx.VX324ufbi_expand( p_pe-p_p ),
				*p_vxe = p_vx;
		gp_memcpy( p_vx, p_p, (p_pe-p_p)*sizeof(*p_p) );
		p_lay->solid_vx.VX324ufbi_add(p_pe-p_p);
		p_lay->lay_ready( p_f, p_fe, p_tags, n_bone_min, n_bone_max, pp_pic );
	}

	GPM_DELARRAY( p_f );
	GPM_DELARRAY( p_p );
	GPM_DELARRAY( p_tags );
	// ha kész kitöltjük a gio_id-t;
	p_gio->gio_id = p_wb->GP_WBx64_GIO_ID( p_gio );
	return ((GPC_RES*)NULL)->res_equ( p_gio->gio_id );
}
