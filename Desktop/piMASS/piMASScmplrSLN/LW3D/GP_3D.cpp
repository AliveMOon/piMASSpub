#include "stdafx.h"
#include "GP_3D.h"
//#include "GP_ASM.h"




GPC3DACT gp_a_action_man[] =
{
	//			begin,	end,	start,	mix,	fade,	gim,		loop
	GPC3DACT(	  1.0,	31.0,	 1.0,	 0.0,	0.25,	1.0,		1.0		),	//GPE_ACTION_DEF,
	GPC3DACT(	  1.0,	31.0,	 1.0,	 0.0,	0.25,	1.0,		1.0		),	//GPE_ACTION_IDLE,
	GPC3DACT(	 32.0,	34.0,	32.5,	33.5,	0.5,	1.0,		32.0	),	//GPE_ACTION_WALK,
	GPC3DACT(	 36.5,	38.5,	36.5,	38.5,	0.5,	0.3,		37.0	),	//GPE_ACTION_RUN,
	GPC3DACT(	 40.0,	42.0,	40.0,	41.5,	0.5,	0.3,		42.0	),	//GPE_ACTION_JUMP,
	GPC3DACT(	 43.0,	45.0,	43.0,	44.5,	0.5,	1.0,		43.0	),	//GPE_ACTION_SLEFT,
	GPC3DACT(	 46.0,	48.0,	46.0,	47.5,	0.5,	1.0,		46.0	),	//GPE_ACTION_SRIGHT,
	GPC3DACT(	 49.0,	51.0,	49.5,	51.5,	0.5,	0.5,		49.0	),	//GPE_ACTION_FLINCH,
	GPC3DACT(	 53.0,	54.0,	53.0,	53.75,	0.25,	0.1,		54.0	),	//GPE_ACTION_BOX,
	GPC3DACT(	 55.0,	56.0,	55.0,	55.5,	0.5,	0.1,		56.0	),	//GPE_ACTION_KICK,
	GPC3DACT(	 57.0,	58.0,	57.0,	57.75,	0.25,	0.2,		58.0	),	//GPE_ACTION_ABOARD,
	GPC3DACT(	 58.0,	60.0,	58.0,	60.0,	0.25,	0.2,		60.0	),	//GPE_ACTION_DEBUS,
	GPC3DACT(	 61.0,	61.5,	61.0,	61.5,	0.25,	0.1,		61.5	),	//GPE_ACTION_STOW,
	GPC3DACT(	 61.5,	62.0,	61.5,	62.0,	0.25,	0.1,		62.0	),	//GPE_ACTION_STOWUP,
	GPC3DACT(	 63.0,	64.0,	63.0,	63.75,	0.25,	0.1,		64.0	),	//GPE_ACTION_WHAM,
	GPC3DACT(	 65.0,	69.0,	65.0,	68.9,	0.01,	0.1,		69.0	),	//GPE_ACTION_KO,	
	GPC3DACT(	 69.0,	74.0,	69.0,	73.9,	0.5,	0.1,		74.0	),	//GPE_ACTION_ERECT,
};



char gp_man_lws[] =
		"manus\n"
		"bone_mother\n"
		"bone_groin\n"
		"bone_waist\n"
		"bone_throax\n"

		"bone_l.upperarm\n"
		"bone_l.forearm\n"
		"bone_l.hand\n"

		"bone_r.upperarm\n"
		"bone_r.forearm\n"
		"bone_r.hand\n"

		"bone_l.thigh\n"
		"bone_l.shin\n"
		"bone_l.feet\n"

		"bone_r.thigh\n"
		"bone_r.shin\n"
		"bone_r.feet\n"

		"bone_neck\n"
		"bone_head\n\0"
		;

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


GPC3DLWS::GPC3DLWS( char* p_p, char* p_pf, const char* p_file, float dim )
{
	GPM_CLR;
	p_lws_bons = p_lws_bons->dick_add( gp_man_lws );

	if( !p_lws_com )
	{
		p_lws_com = p_lws_com->dick_add( gp_s_lws );
		if( !p_lws_com )
			return;
	}

	p_gim = new GPC3DGIM();
	if( !p_gim )
		return;

	GPM_STRCPY( p_pf, p_file );
	U8 s = 0;
	GPC_ARR* p_lws = ((GPC_ARR*)NULL)->arr_read( p_p, s, 0 ); 
	if( p_lws ? !p_lws->p_alloc : true )
		return;

	//p_gim->dim = dim;
				 	
	char	*p_src = (char*)p_lws->p_alloc,
			*p_end = p_src + GPD_STRLEN( p_lws->p_alloc ), *p_atrib;

	GPC_DICT* p_dick = p_lws_com;
	GPE_LWS_COM com;
	I8 n_obj = -1, n_bone = -1, n_load, pite_id = 0, mom_id = 0, i8;
	//if( !p_gim->track_p_alloc )
	//{
	//	p_gim->track_n_alloc = 0x18;
	//	p_gim->track_p_alloc = (GPCTRCK*)memset( new GPCTRCK[p_gim->track_n_alloc], 0, p_gim->track_n_alloc*sizeof(*p_gim->track_p_alloc) );
	//}
	GPCTRCK	*p_mom = NULL,
			*p_track = NULL;
	GPC3DCNL* p_cnl = NULL;
	p_gim->track_p_load = p_gim->track_p_alloc;
	bool b_off = false;
	//float2 key;
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
					GPCTRCK* p_old = p_gim->track_p_alloc;
					p_gim->track_n_alloc += 0x10;
					p_gim->track_p_alloc = new GPCTRCK[p_gim->track_n_alloc];
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
					GPCTRCK* p_old = p_gim->track_p_alloc;
					p_gim->track_n_alloc += 0x10;
					p_gim->track_p_alloc = new GPCTRCK[p_gim->track_n_alloc];
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
					p_cnl->p_key = new float2[p_cnl->n_key];
				
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
}


float gp_a_cell_hi[] =
{	
	// N	BE	lookdir	NOT
	//=============================================================================
	//
	//			SIDE YZ
	//
	//=============================================================================
	//								YZ DOWN BACK	
	// 0.	0x01	RIGHT	!0x02																	//      ___  /__/ +2   Y     	//
	0 , GPD_LIG_r+23 ,	0 , GPD_LIG_r+26 ,	0 , GPD_LIG_r+17 ,	0 , GPD_LIG_r+14 ,					//     /\\\\ \\\\/      \   X	//
	// 1.	0x02	LEFT	!0x01																	//      /  /    		 \ /    //
	1 , GPD_LIG_l+24 ,	1 , GPD_LIG_l+21 ,	1 , GPD_LIG_l+12 ,	1 , GPD_LIG_l+15 ,					//        +1         z ---+		//
	//								YZ DOWN FRONT														
	//																								//           \__\               //
	// 2.	0x04	RIGHT	!0x08																	//      __\ 8/._/\     Y     	//
	80 , GPD_LIG_r+20 ,	80 , GPD_LIG_r+23 ,	80 , GPD_LIG_r+14 ,	80 , GPD_LIG_r+11 ,					//    \/\\\\ \\\\/      \   X	//
	// 3.	0x08	LEFT	!0x04																	//     \/__/    		 \ /    //
	81 , GPD_LIG_l+21 ,	81 , GPD_LIG_l+18 ,	81 , GPD_LIG_l+9 ,	81 , GPD_LIG_l+12 ,					//    4°             z ---+		//
	//								YZ UP BACK							
	//																								//               32             //
	// 4.	0x10	RIGHT	!0x20																	//      ___  /__/°     Y     	//
	6400 , GPD_LIG_r+14 ,	6400 , GPD_LIG_r+17 ,	6400 , GPD_LIG_r+8 ,	6400 , GPD_LIG_r+5 ,	//     /\\/\ \\\\/\     \   X	//
	// 5.	0x20	LEFT	!0x10																	//      /°16\ \  \   	 \ /    //
	6401 , GPD_LIG_l+15 ,	6401 , GPD_LIG_l+12 ,	6401 , GPD_LIG_l+3 ,	6401 , GPD_LIG_l+6 ,	//      \  \         z ---+		//
	//								YZ UP FRONT			
	//																								//         128.__               //
	// 6.	0x40	RIGHT	!0x80																	//      ___  /__/\     Y     	//
	6480 , GPD_LIG_r+11 ,	6480 , GPD_LIG_r+14 ,	6480 , GPD_LIG_r+5 ,	6480 , GPD_LIG_r+2 ,	//  64 /\\/\ \\\\/\     \   X	//
	// 7.	0x80	LEFT	!0x40																	//    ° /_\/\ \  \   	 \ /    //
	6481 , GPD_LIG_l+12 ,	6481 , GPD_LIG_l+9 ,	6481 , GPD_LIG_l+0 ,	6481 , GPD_LIG_l+3 ,	//      \  \         z ---+		//
	//=============================================================================
	//
	//			FRONT XY
	//
	//=============================================================================
	//				XY DOWN LEFT
	// 8.	0x01	FRONT	!0x04																	//      FRONT      __		//
	0 , GPD_LIG_f+26 ,	0 , GPD_LIG_f+25 ,	0 , GPD_LIG_f+16 ,	0 , GPD_LIG_f+17 ,					//             -> /_\_		//
	// 9.	0x04	BACK	!0x01																	//      __    ->  \_/_.1	//
	80 , GPD_LIG_b+19 ,	80 , GPD_LIG_b+20 ,	80 , GPD_LIG_b+11 ,	80 , GPD_LIG_b+10 ,					//     __/\ <-				//
																									// 4.__\/<-   BACK	        //
	//								XY DOWN RIGHT			
	//																								//			//
	// 10.	0x02	FRONT	!0x08																	//      FRONT     \__\ 		//
	1 , GPD_LIG_f+25 ,	1 , GPD_LIG_f+24 ,	1 , GPD_LIG_f+15 ,	1 , GPD_LIG_f+16 ,					//             ->\/\__\.2	//
	// 11.	0x08	BACK	!0x02																	//     \__\   ->  \/__/		//
	81 , GPD_LIG_b+18 ,	81 , GPD_LIG_b+19 ,	81 , GPD_LIG_b+10 ,	81 , GPD_LIG_b+9 ,					//     8\\/\ <-				//
																									//    /_\/<-     BACK       //
	//								XY UP LEFT			
	//																								//							//
	// 12.	0x10	FRONT	!0x40																	//               __			//
	6400 , GPD_LIG_f+17 ,	6400 , GPD_LIG_f+16 ,	6400 , GPD_LIG_f+7 ,	6400 , GPD_LIG_f+8 ,	//      __    ->/_\_.16		//
	// 13.	0x40	BACK_UL	!0x10																	// 64 .__/\ <-  \_/_		//
	6480 , GPD_LIG_b+10 ,	6480 , GPD_LIG_b+11 ,	6480 , GPD_LIG_b+2 ,	6480 , GPD_LIG_b+1 ,	//      _\/\     \			//
																									//      \					//
	//								XY UP RIGH			
	//																								//							//
	// 14.	0x20	FRONT	!0x80																	//                __.32 	//
	6401 , GPD_LIG_f+16 ,	6401 , GPD_LIG_f+15 ,	6401 , GPD_LIG_f+6 ,	6401 , GPD_LIG_f+7 ,	//  128.__   <-->/_\_ 		//
	// 15.	0x80	BACK_UR	!0x20																	//     __/\ <--> \_/_		//
	6481 , GPD_LIG_b+9 ,	6481 , GPD_LIG_b+10 ,	6481 , GPD_LIG_b+1 ,	6481 , GPD_LIG_b+0 ,	//      _\/\      \			//
																									//      \					//
	//=============================================================================
	//
	//			TOP XZ
	//
	//=============================================================================
	//								XZ LEFT	BACK			
	//																								//       /  /  /		//
	// 16.	0x01	TOP	!0x10																		//      /__16./\/		//
	0 , GPD_LIG_t+26 ,	0 , GPD_LIG_t+23 ,	0 , GPD_LIG_t+22 ,	0 , GPD_LIG_t+25 ,					//      \ _\//\/_/		//
	// 17.	0x10	BOTTOM	!0x01																	//      /__////\/		//
	6400 , GPD_LIG_d+4 ,	6400 , GPD_LIG_d+5 ,	6400 , GPD_LIG_d+8 ,	6400 , GPD_LIG_d+7 ,	// 		\__\__\.1		//
	//								XZ RIGHT BACK			
	//																								//       ______.32		//
	// 18.	0x02	TOP	!0x20																		//      /\__\__\		//
	1 , GPD_LIG_t+22 ,	1 , GPD_LIG_t+21 ,	1 , GPD_LIG_t+24 ,	1 , GPD_LIG_t+25 ,					//     /\/__////_		//
	// 19.	0x20	BOTTOM	!0x02																	//      / /__////\.2		//
	6401 , GPD_LIG_d+3 ,	6401 , GPD_LIG_d+4 ,	6401 , GPD_LIG_d+7 ,	6401 , GPD_LIG_d+6 ,	//       /\__\__\/	           //
	//								XZ LEFT	FRONT			
	//																								//        /  /  /		//
	// 20.	0x04	TOP_FL	!0x40																	//    64./__/__/\/		//
	80 , GPD_LIG_t+22 ,	80 , GPD_LIG_t+23 ,	80 , GPD_LIG_t+20 ,	80 , GPD_LIG_t+19 ,					//       \//\__\/_/		//
	// 21.	0x40	BOTTOM	!0x04																	//         ////__/\/		//
	6480 , GPD_LIG_d+1 ,	6480 , GPD_LIG_d+2 ,	6480 , GPD_LIG_d+5 ,	6480 , GPD_LIG_d+4 ,	//       4.\__\__\/		//
	//								XZ RIGHT FRONT
	//																								//   128.______		//
	// 22.	0x08	TOP_FR	!0x80																	//      /\__\__\		//
	81 , GPD_LIG_t+21 ,	81 , GPD_LIG_t+22 ,	81 , GPD_LIG_t+19 ,	81 , GPD_LIG_t+18 ,					//     /\////__/_		//
	// 23.	0x80	BOTTOM	!0x08																	//      / ////__/\		//
	6481 , GPD_LIG_d+0 ,	6481 , GPD_LIG_d+1 ,	6481 , GPD_LIG_d+4 ,	6481 , GPD_LIG_d+3 ,	//       /\/8\__\/	           //


	// 0x03 -----------------------------------
	// 24. 0x03 FRONT_BD
	0 , GPD_LIG_f+25 ,	0 , GPD_LIG_f+16 ,	1 , GPD_LIG_f+16 ,	1 , GPD_LIG_f+25 ,
	// 25. 0x03 TOP
	0 , GPD_LIG_t+22 ,	0 , GPD_LIG_t+25 ,	1 , GPD_LIG_t+25 ,	1 , GPD_LIG_t+22 ,
	
	// 0x05 -----------------------------------
	// 26. 0x05 RIGHT_DBF
	0 , GPD_LIG_r+14 ,	0 , GPD_LIG_r+23 ,	80 , GPD_LIG_r+23 ,	80 , GPD_LIG_r+14 ,
	// 27. 0x05 TOP
	0 , GPD_LIG_t+23 ,	0 , GPD_LIG_t+22 ,	80 , GPD_LIG_t+22 ,	80 , GPD_LIG_t+23 ,

	// 0x0A -----------------------------------
	// 28. 0x0a LEFT_D2
	1 , GPD_LIG_l+21 ,	1 , GPD_LIG_l+12 ,	81 , GPD_LIG_l+12 ,	81 , GPD_LIG_l+21 ,
	// 29. 0x0a TOP_R2
	1 , GPD_LIG_t+22 ,	1 , GPD_LIG_t+21 ,	81 , GPD_LIG_t+21 ,	81 , GPD_LIG_t+22 ,

	// 0x0C -----------------------------------
	// 30. 0x0c BACK_D2
	80 , GPD_LIG_b+10 ,	80 , GPD_LIG_b+19 ,	81 , GPD_LIG_b+19 ,	81 , GPD_LIG_b+10 ,
	// 31. 0x0c TOP_F2
	80 , GPD_LIG_t+19 ,	80 , GPD_LIG_t+22 ,	81 , GPD_LIG_t+22 ,	81 , GPD_LIG_t+19 ,

	// 0x0F -----------------------------------
	// 32. 0x0f TOP
	0 , GPD_LIG_t+22 ,	80 , GPD_LIG_t+22 ,	81 , GPD_LIG_t+22 ,	1 , GPD_LIG_t+22 ,


	// 0x11 -----------------------------------
	// 33.  0x11 RIGHT_B2
	0 , GPD_LIG_r+17 ,	0 , GPD_LIG_r+14 ,	6400 , GPD_LIG_r+14 ,	6400 , GPD_LIG_r+17 ,
	// 34.  0x11 FRONT_R2
	0 , GPD_LIG_f+16 ,	0 , GPD_LIG_f+17 ,	6400 , GPD_LIG_f+17 ,	6400 , GPD_LIG_f+16 ,
	

	// 0x22 -----------------------------------
	// 35.  0x22 LEFT
	1 , GPD_LIG_l+12 ,	1 , GPD_LIG_l+15 ,	6401 , GPD_LIG_l+15 ,	6401 , GPD_LIG_l+12 ,
	// 36.  0x22 FRONT_L
	1 , GPD_LIG_f+15 ,	1 , GPD_LIG_f+16 ,	6401 , GPD_LIG_f+16 ,	6401 , GPD_LIG_f+15 ,

	// 0x30 -----------------------------------
	// 37. 0x30 DOWN
	6400 , GPD_LIG_d+7 ,	6400 , GPD_LIG_d+4 ,	6401 , GPD_LIG_d+4 ,	6401 , GPD_LIG_d+7 ,
	// 38. 0x30 FRONT_U2
	6400 , GPD_LIG_f+16 ,	6400 , GPD_LIG_f+7 ,	6401 , GPD_LIG_f+7 ,	6401 , GPD_LIG_f+16 ,

	// 0x33 -----------------------------------
	// 39. 0x33 FRONT_FULL
	6400 , GPD_LIG_f+16 ,	0 , GPD_LIG_f+16 ,	1 , GPD_LIG_f+16 ,	6401 , GPD_LIG_f+16 ,


	// 0x44 -----------------------------------
	// 40.  0x44 RIGHT_F
	80 , GPD_LIG_r+14 ,	80 , GPD_LIG_r+11 ,	6480 , GPD_LIG_r+11 ,	6480 , GPD_LIG_r+14 ,
	// 41.  0x44 BACK_R
	80 , GPD_LIG_b+11 ,	80 , GPD_LIG_b+10 ,	6480 , GPD_LIG_b+10 ,	6480 , GPD_LIG_b+11 ,

	// 0x50 -----------------------------------
	// 42.  0x44 DOWN_R
	6400 , GPD_LIG_d+4 ,	6400 , GPD_LIG_d+5 ,	6480 , GPD_LIG_d+5 ,	6480 , GPD_LIG_d+4 ,
	// 43.  0x44 RIGHT_U
	6400 , GPD_LIG_r+5 ,	6400 , GPD_LIG_r+14 ,	6480 , GPD_LIG_r+14 ,	6480 , GPD_LIG_r+5 ,
	
	// 0x55 -----------------------------------
	// 44. 0x55 RIGHT
	0 , GPD_LIG_r+14 ,	6400 , GPD_LIG_r+14 ,	6480 , GPD_LIG_r+14 ,	80 , GPD_LIG_r+14 ,

	// 0x88 -----------------------------------
	// 45.  0x88 LEFT_F
	81 , GPD_LIG_l+9 ,	81 , GPD_LIG_l+12 ,	6481 , GPD_LIG_l+12 ,	6481 , GPD_LIG_l+9 ,
	// 46.  0x88 BACK_L
	81 , GPD_LIG_b+10 ,	81 , GPD_LIG_b+9 ,	6481 , GPD_LIG_b+9 ,	6481 , GPD_LIG_b+10 ,

	// 0xA0 -----------------------------------
	// 47. 0xA0 DOWN_L
	6401 , GPD_LIG_d+3 ,	6401 , GPD_LIG_d+4 ,	6481 , GPD_LIG_d+4 ,	6481 , GPD_LIG_d+3 ,
	// 48. 0xA0 LEFT_U
	6401 , GPD_LIG_l+12 ,	6401 , GPD_LIG_l+3 ,	6481 , GPD_LIG_l+3 ,	6481 , GPD_LIG_l+12 ,

	// 0xAA -----------------------------------
	// 49. 0xAA LEFT
	6401 , GPD_LIG_l+12 ,	1 , GPD_LIG_l+12 ,	81 , GPD_LIG_l+12 ,	6481 , GPD_LIG_l+12 ,

	// 0xC0 -----------------------------------
	// 50. 0xC0 DOWN_F
	6480 , GPD_LIG_d+4 ,	6480 , GPD_LIG_d+1 ,	6481 , GPD_LIG_d+1 ,	6481 , GPD_LIG_d+4 , 
	// 51. 0xC0 BACK_U
	6480 , GPD_LIG_b+1 ,	6480 , GPD_LIG_b+10 ,	6481 , GPD_LIG_b+10 ,	6481 , GPD_LIG_b+1 , 

	// 0xCC -----------------------------------
	// 52. 0xCC BACK
	80 , GPD_LIG_b+10 ,	6480 , GPD_LIG_b+10 ,	6481 , GPD_LIG_b+10 ,   81 , GPD_LIG_b+10 ,

	// 0xF0 -----------------------------------
	// 53. 0xCC DOWN
	6400 , GPD_LIG_d+4 ,	6401 , GPD_LIG_d+4 ,	6481 , GPD_LIG_d+4 ,	6480 , GPD_LIG_d+4 ,

	// 0x1F -----------------------------------
	// 54.
	6400 , GPD_LIG_f+16,	80 , GPD_LIG_t+22 ,	81 , GPD_LIG_t+22 ,	6400 , GPD_LIG_f+17 ,
	// 55.
	6400 , GPD_LIG_r+14,	6400 , GPD_LIG_r+17 ,	81 , GPD_LIG_t+22 ,	1 , GPD_LIG_t+22 ,

};

float2* GPC_CELL::GP_A_CELL_get_hi( void )
{
	return (float2*)gp_a_cell_hi;
}
void GPC3DBLD::EXPsk( GPC3D96* p96 )
{
	for( U1 s = 0; s < 6; s++ )
	{
		if( !an_v2_alloc[s] )
		{
			GPM_DELARRAY( ap_v2sk[s] );
			continue;
		}
		if( an_v2sk_alloc[s] < an_v2_alloc[s] )
		{
			GPM_DELARRAY( ap_v2sk[s] );
			ap_v2sk[s] = new float2sk[ (an_v2sk_alloc[s] = an_v2_alloc[s]) ];
		}
		float2* p_v2 = ap_v2[s];
		float2sk* p_v2sk = ap_v2sk[s];
		
		nSTRIDE = sizeof(*p_v2sk);
		
		U4 a0;
		float3 p;
		for( U8 e = an_v2_load[s], i = 0; i < e; i++ )
		{
			p_v2sk[i] = p_v2[i];
			p = p_v2[i].cell_x2p();
			a0 = p.x + p.z*gpe3D96y + p.y * gpe3D96z;
			if( !p96->p3D80[a0].A )
			{
				p = p_v2[i].cell_x2p();
				continue;
			}
			p_v2sk[i].ix = p96->p3D80[a0].lev; 
			p_v2sk[i].bgra = p96->p3D80[a0].stf;
		}
	}
}
U142* GPC3DBLD::build_cell(
						GPC3DGR	*pGR, 
						U1		*p_pat, 
						U14		*p3D80,
						U142	*p_u142,
						U4&		n_u142,
						U142	* p_cell,
						U4		*p_s,
						U4		*p_sn,
						U1		s
					)
{
	U4 n_cl = 0;

	U4 a_histi[0x100];
	GPMZ( a_histi );
	
	U4 n, n_cell = 0;
	for( U1 h = 1; h < 0xff; h++ )
	{
		n = a_pat_histi[h];
		if( !n || !p_sn[h] )
			continue;	// nem minden sikban van polygonja a celnek pl. 0x55 ez  egy füg. lap
		
		a_histi[h] = n;
		n *= p_sn[h];
		n_cell += n;
	}

	U4 n_load = 0;
	
	if( !n_cell )
	{
		// üres
		return p_u142;
	}

	// elég lesz a korábban foglalt?
	if( !p_u142 || n_u142 < n_cell )
	{
		GPM_DELARRAY(p_u142);
		p_u142 = new U142[ n_u142 = n_cell ];
	}

	U142 *p_dst;

	n_cl += n_cell;
	
	p_dst = p_u142;
	U1 ptrn;

	I44 xyz_a0; U4 a0, b0;
	for( xyz_a0.z = 0;	xyz_a0.z < gpe3DPAT_ALL;	xyz_a0.z += gpe3DPAT_Z )
	for( xyz_a0.y = 0;	xyz_a0.y < gpe3DPAT_Z;		xyz_a0.y += gpe3DPAT_Y )
	for( xyz_a0.x = 0;	xyz_a0.x < gpe3DPAT_Y;		xyz_a0.x++ )
	{
		a0 = xyz_a0.sum_xyz();
		ptrn = p_pat[a0];
		if( !ptrn || ptrn == 0xff )
			continue;

		if( !a_histi[ptrn] )
			continue; // nem minden sikban van polygonja a celnek

		b0 = xyz_a0.x + ((xyz_a0.y/gpe3DPAT_Y)*256 + (xyz_a0.z/gpe3DPAT_Z))*256; 
		gp_memcpy( p_dst, p_cell+p_s[ptrn], (n = p_sn[ptrn])*sizeof(*p_dst) );
		for( U4 i = 0; i < n; i++ )
		{
			p_dst[i].posn.u4 += b0;
		}
		p_dst += n;
	}
		
	n_load = p_dst-p_u142;
		
	if( !n_load )
	{
		GPM_DELARRAY( pGR->ap_u142[s] );
		return p_u142;
	}

	if( pGR->an_alloc[s] < n_load )
	{
		GPM_DELARRAY( pGR->ap_u142[s] );
		pGR->ap_u142[s] = new U142[ (pGR->an_alloc[s] = n_load) ];
	}
		
	U142* p_u142c = pGR->ap_u142[s];
	U44 p;
	gpeMAP etc;
	const U44 mul( 1,96*96,96 );
	pGR->stride = sizeof(*p_u142c);
	GPM_MEMCPY( p_u142c, p_u142, n_load );
	if(  true )
	{
		for( U4 i = 0; i < n_load; i++ )
		{
			p = p_u142c[i].posn;
			a0 = (p&mul).sum_xyz();
			if( !(i%4) )
				etc = p3D80[a0].pln;

			p_u142c[i].etc = p3D80[a0];
			p_u142c[i].etc.w = etc;
		}
	}
	else for( U4 i = 0; i < n_load; i+=4 )
	{
		p = p_u142c[i].posn;
		a0 = (p&mul).sum_xyz();
		p3D80[a0].A = p3D80[a0].B;
		p_u142c[i+3].etc = p_u142c[i+2].etc = p_u142c[i+1].etc = p_u142c[i].etc = p3D80[a0];

		
	}
	pGR->anVX[s] = n_load;
	pGR->fac |= 1<<s;
	return p_u142;
}
	


GPC3DGR* GPC3DBLD::buildGR( GPC3DGR* pGR, GPC_CELL *p_cell, U1* p_pat, U14* p3D80 )
{
	n_mxix = nSTRIDE = 0;
	U4 n_hi_sum = histi( p_pat );
	if( !pGR )
		pGR = new GPC3DGR;
	for( U1 f = 0; f < 6; f++ )
	{
		p_u142_alloc = build_cell(		
										pGR,
											
										p_pat,
										p3D80,

										p_u142_alloc,
										n_u142_alloc,
										
										p_cell->ap_3d_u142[f],
										p_cell->a_3d_cell_i[f],
										p_cell->a_3d_cell_n[f],
										f
									);
	}
	return pGR;
}

float2* GPC3DBLD::build_cell(	
				float2* p_v2,
				U1* p_pat, U4* p_histi,
				float2* p_cell, U4* p_s, U4* p_sn,
				U1 s,
				U4* p_512
			)
{
	n_cl = 0;
	float2 *p_lock, *p_dst;

	U4 a_histi[0x100];
	GPMZ( a_histi );
	
	U4 n, n_cell = 0;
	for( U1 h = 1; h < 0xff; h++ )
	{
		n = p_histi[h];
		if( !n || !p_sn[h] )
			continue;	// nem minden sikban van polygonja a celnek pl. 0x55 ez  egy füg. lap
		
		a_histi[h] = n;
		n *= p_sn[h];
		n_cell += n;
	}

	an_v2_load[s] = 0;
	
	if( !n_cell )
	{
		// üres
		return p_v2;
	}

	// elég lesz a korábban foglalt?
	if( !p_v2 || an_v2_alloc[s] < n_cell )
	{
		GPM_DELARRAY(p_v2);
		an_v2sk_alloc[s] = 0;
		p_v2 = new float2[ an_v2_alloc[s] = n_cell ];
	}

	n_cl += n_cell;
	
	p_dst = p_lock = p_v2;
	U1 ptrn;

	//GPE_STF safe_stf, stf;
	I44 xyz_e0, xyz_a0;
	U4 e0, a0, *p_512e = p_512, b0;
	for( xyz_e0.z = 0;		xyz_e0.z < gpe3DPAT_ALL;	xyz_e0.z += gpe3DPAT_Z*8 )
	for( xyz_e0.y = 0;		xyz_e0.y < gpe3DPAT_Z;		xyz_e0.y += gpe3DPAT_Y*8 )
	for( xyz_e0.x = 0;		xyz_e0.x < gpe3DPAT_Y;		xyz_e0.x += 8 )
	{
		*p_512e = p_dst-p_lock;
		p_512e++;
		e0 = xyz_e0.sum_xyz();
		for( xyz_a0.z = 0;	xyz_a0.z < gpe3DPAT_Z*8;	xyz_a0.z += gpe3DPAT_Z )
		for( xyz_a0.y = 0;	xyz_a0.y < gpe3DPAT_Y*8;	xyz_a0.y += gpe3DPAT_Y )
		for( xyz_a0.x = 0;	xyz_a0.x < 8;			xyz_a0.x++ )
		{
			a0 = xyz_a0.sum_xyz() + e0;
			ptrn = p_pat[a0];
			if( !ptrn || ptrn == 0xff )
				continue;

			if( !a_histi[ptrn] )
				continue; // nem minden sikban van polygonja a celnek
			
			n = p_sn[ptrn];
			if( !n )
				continue;

			b0 = (a0%gpe3DPAT_Y)+ ((a0%gpe3DPAT_Z)/gpe3DPAT_Y)*gpe3DMSTR_Y + (a0/gpe3DPAT_Z)*gpe3DMSTR_YY;
			gp_memcpy( p_dst, p_cell+p_s[ptrn], n*sizeof(*p_dst) );
			for( U4 i = 0; i < n; i++ )
			{
				p_dst[i].x += b0;
			}
			p_dst += n;
		}
	}
	an_v2_load[s] = p_dst-p_lock;
	if( nSTRIDE ? false : an_v2_load[s] )
		nSTRIDE = sizeof(*p_v2);
	
	return p_v2;
}

GPC3DGLB* GPC3DGLB::build( GPC_PROTO* pPROTO, GPC3DGLB* pDST, ID3D11Device* p_dev, ID3D11DeviceContext* p_contx, U4* pBGR, GPC3DBLD* pBLD, GPC3D96* p96, GPC_CELL* pCELL, U1* pMSK )
{
	if( !pBGR )
		return pDST;

	if( !this )
		return new GPC3DGLB( pPROTO, p_dev, p_contx, pBGR, pBLD, p96, pCELL, pMSK );
		
	for( U1 i = 0; i < 6; i++ )
		a3DGR[i].reset( i<<15 );

	const U14 lim( 32, 32, 32, 0 );
	U1* pMST, s = 3;
	U44	*p_stf_tree = NULL;
	U2 n_stf_tree = 0;

	for( s = 0; s < 6; s++ )
	{
		p96->ins_512x544_32on3( pBGR, pMSK, s, p_stf_tree, n_stf_tree );
		pMST = p96->MSTR( pMSK );
		pBLD->buildGR( a3DGR+s, pCELL, p96->MSTR2PAT( NULL, lim ), p96->p3D80 );
			
		if( !pPROTO )
			continue;
		if( !s )
		{
			for( U2 i = 0; i < n_stf_tree; i++ )
				pPROTO->aBGRA[i].u4 = p_stf_tree[i].x;
		}
			
		p96->PROTO_PAT( pPROTO->aPAT+gpe3DMAP_SID*s, p96->a3DPAT );
		p96->PROTO( pPROTO->aPROTO + gpe3DMAP_SID*s, pMSK );
		//pPROTO += gpe3DMAP_SID;
	}
	GPM_DELARRAY( p_stf_tree );
	return this;
}
GPC3DGLB* GPC3DGLB::build(GPC_PROTO* pPROTO, GPC3DGLB* pDST, ID3D11Device* p_dev, ID3D11DeviceContext* p_contx, U4* pBGR, GPC3DBLD* pBLD, GPC_MSTR* pMSTR, GPC_CELL* pCELL, U1* pMSK)
{
	if(!pBGR)
		return pDST;

	if(!this)
		return new GPC3DGLB(pPROTO, p_dev, p_contx, pBGR, pBLD, pMSTR, pCELL, pMSK);

	for(U1 i = 0; i < 6; i++)
		a3DGR[i].reset(i << 15);

	const U14 lim(32, 32, 32, 0);
	U1* pMST, s = 3;
	U44	*p_stf_tree = NULL;
	U2 n_stf_tree = 0;

	for(s = 0; s < 6; s++)
	{
		pMSTR->ins_512x544_32on3(pBGR, pMSK, s, p_stf_tree, n_stf_tree );
		pMSTR->mstr_pat(pPROTO->aMSK);
		for( U1 f = 0; f < 6; f++ )
		{
			pMSTR->build_cell2(
									a3DGR + s,

									pPROTO->p_cell->ap_3d_u142[f],
									pPROTO->p_cell->a_3d_cell_i[f],
									pPROTO->p_cell->a_3d_cell_n[f],
									f
								);
		}
		pMSTR->PAT2PROTO( pPROTO->aPAT + gpe3DMAP_SID*s, pMSTR->aPAT );
		pMSTR->MAP2PROTO( pPROTO->aPROTO + gpe3DMAP_SID*s, pMSK);
	}
	GPM_DELARRAY(p_stf_tree);
	return this;
}

ID3D11Buffer* GPC3DCAM::rndr( ID3D11Device* p_dev, ID3D11DeviceContext* p_contx, ID3D11Buffer* pVScb, GPC3DGR* p3DGR, GPC3DIX* p_ix, U44& u44, bool bSS, bool b_occlu )
{
	upl( p_dev, bSS );
	p_contx->VSSetConstantBuffers( 0, 1, &p_x11 );
	pVScb = p3DGR->rndr( p_dev, p_contx, pVScb, p_ix, u44, bSS ? b_occlu : false );
	return pVScb;
}

ID3D11Buffer* GPC3DCAM::rndr( ID3D11Device* p_dev, ID3D11DeviceContext* p_contx, ID3D11Buffer* pVScb, GPC3DGLB* p3DmxGLOB, GPC3DIX* p_ix, bool bSS, bool b_occlu )
{
	upl( p_dev, bSS );
	p_contx->VSSetConstantBuffers( 0, 1, &p_x11 );
	U44 u44 = 0;
	for( u44.x = 0; u44.x < 6; u44.x++ )
		pVScb = p3DmxGLOB->a3DGR[u44.x].rndr( p_dev, p_contx, pVScb, p_ix, u44, bSS ? b_occlu : false );
	return pVScb;
}

ID3D11Buffer* GPC3DCAM::rndr( ID3D11Device* p_dev, ID3D11DeviceContext* p_contx, ID3D11Buffer* pVScb, GPC3DGLB* p3DmxGLOB, GPC3DIX* p_ix, U44& u44, bool bSS, bool b_occlu )
{
	upl( p_dev, bSS );
	p_contx->VSSetConstantBuffers( 0, 1, &p_x11 );
	for( u44.x = 0; u44.x < 6; u44.x++ )
		pVScb = p3DmxGLOB->a3DGR[u44.x].rndr( p_dev, p_contx, pVScb, p_ix, u44, bSS ? b_occlu : false );
	return pVScb;
}

ID3D11Buffer* GPC3DCAM::rndr_PnL(ID3D11Device* p_dev, ID3D11DeviceContext* p_contx, ID3D11Buffer* pVScb, GPC3DGLB* p3DmxGLOB, GPC3DIX* p_ix, U44& u44, U1 PnL, bool bSS, bool b_occlu)
{
	upl(p_dev, bSS);
	p_contx->VSSetConstantBuffers(0, 1, &p_x11);
	for(u44.x = 0; u44.x < 6; u44.x++)
		pVScb = p3DmxGLOB->a3DGR[u44.x].rndr_PnL(p_dev, p_contx, pVScb, p_ix, u44, PnL, bSS ? b_occlu : false);
	return pVScb;
}
ID3D11Buffer* GPC3DCAM::rndr_PnL(ID3D11Device* p_dev, ID3D11DeviceContext* p_contx, ID3D11Buffer* pVScb, GPC3DGR* pGR, GPC3DIX* p_ix, U44& u44, U1 PnL, bool bSS, bool b_occlu)
{
	upl(p_dev, bSS);
	p_contx->VSSetConstantBuffers(0, 1, &p_x11);
	//for(u44.x = 0; u44.x < 6; u44.x++)
	pVScb = pGR->rndr_PnL(p_dev, p_contx, pVScb, p_ix, u44, PnL, bSS ? b_occlu : false);
	return pVScb;
}

const D3D11_INPUT_ELEMENT_DESC gp_lay_u142[] = 
{
	{ "POSITION",		0,	DXGI_FORMAT_R8G8B8A8_UNORM,		0,	0,		D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",			0,	DXGI_FORMAT_R8G8B8A8_UNORM,		0,	4,		D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
const D3D11_INPUT_ELEMENT_DESC gp_lay_cell[] = 
{
	{ "POSITION",	0,	DXGI_FORMAT_R32G32_FLOAT,			0,	0*4,							D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
const D3D11_INPUT_ELEMENT_DESC gp_lay_cell_skin[] = 
{
	{ "POSITION",		0,	DXGI_FORMAT_R32G32_FLOAT,		0,	0*4,							D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BLENDINDICES",	0,	DXGI_FORMAT_R8G8B8A8_UINT,		0,	4*2,							D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",			0,	DXGI_FORMAT_B8G8R8A8_UNORM,		0,	4*3,							D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

const D3D11_INPUT_ELEMENT_DESC gp_lay_vox[] = 
{
	{ "POSITION",	0,	DXGI_FORMAT_R32G32B32A32_FLOAT,		0,	0*4,							D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0,	DXGI_FORMAT_R32G32B32A32_FLOAT,		0,	4*4,							D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",		0,	DXGI_FORMAT_R32G32B32A32_FLOAT,		0,	8*4,							D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0,	DXGI_FORMAT_R32G32B32A32_FLOAT,		0,	12*4,							D3D11_INPUT_PER_VERTEX_DATA, 0 },
};


const D3D11_INPUT_ELEMENT_DESC gp_lay_VX32C[] = 
{
	{ "POSITION",	0,	 DXGI_FORMAT_R32G32B32_FLOAT,		0,	 0,								D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0,	 DXGI_FORMAT_R32G32_FLOAT,			0,	 3*4,							D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0,	 DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	 5*4,							D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
	

const D3D11_INPUT_ELEMENT_DESC gp_lay_VX32CU[] = 
{
	{ "POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,		0,	0*4,							D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0,	DXGI_FORMAT_R32G32_FLOAT,			0,	3*4,							D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0,	DXGI_FORMAT_R32G32B32A32_FLOAT,		0,	(3+2)*4,						D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",		0,	DXGI_FORMAT_R32G32B32_FLOAT,		0,	(3+2+4)*4,						D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

// INST ----------------------------
const D3D11_INPUT_ELEMENT_DESC gp_lay_VX32CU_inst[] = 
{
	{ "POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,		0,	0*4,							D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0,	DXGI_FORMAT_R32G32_FLOAT,			0,	3*4,							D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0,	DXGI_FORMAT_R32G32B32A32_FLOAT,		0,	(3+2)*4,						D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",		0,	DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },

	{ "TEXCOORD",	1,	DXGI_FORMAT_R32G32B32A32_FLOAT,		1,	0,								D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "COLOR",		1,	DXGI_FORMAT_R32G32B32A32_FLOAT,		1,	16,								D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "COLOR",		2,	DXGI_FORMAT_R32G32B32A32_FLOAT,		1,	32,								D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "COLOR",		3,	DXGI_FORMAT_R32G32B32A32_FLOAT,		1,	48,								D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "COLOR",		4,	DXGI_FORMAT_R32G32B32A32_FLOAT,		1,	64,								D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		
};

const D3D11_INPUT_ELEMENT_DESC gp_lay_VX32CUFBI[] = 
{
	{ "POSITION",		0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	0*4,							D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",		0,	DXGI_FORMAT_R32G32_FLOAT,		0,	3*4,							D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",			0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	(3+2)*4,						D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",			0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	(3+2+4)*4,						D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",			1,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	(3+2+4+3)*4,					D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BLENDWEIGHT",	0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	(3+2+4+3+3)*4,					D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BLENDINDICES",	0,	DXGI_FORMAT_R8G8B8A8_UINT,		0,	(3+2+4+3+3+4)*4,				D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
	
const D3D11_INPUT_ELEMENT_DESC gp_lay_VX32CUFBI_inst[] = 
{
	{ "POSITION",		0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	0*4,							D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",		0,	DXGI_FORMAT_R32G32_FLOAT,		0,	3*4,							D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",			0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	(3+2)*4,						D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",			0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	(3+2+4)*4,						D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",			1,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	(3+2+4+3)*4,					D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BLENDWEIGHT",	0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	(3+2+4+3+3)*4,					D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BLENDINDICES",	0,	DXGI_FORMAT_R8G8B8A8_UINT,		0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },

	{ "TEXCOORD",		1,	DXGI_FORMAT_R32G32_FLOAT,		1,	0,								D3D11_INPUT_PER_INSTANCE_DATA, 1 },
};
	
const D3D11_INPUT_ELEMENT_DESC gp_lay_vxc[] =
{
	{ "POSITION",		0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	0,		D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",			0,	DXGI_FORMAT_R8G8B8A8_UNORM,		0,	3 * 4,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
};


GPC3DSHDR* GPC3DSHDR::compi_v1( ID3D11Device* p_dev, const char* p_src, const char* p_name ) //, bool b_u124 )
{
	if( !this )
	{
		GPC3DSHDR* p_shdr = new GPC3DSHDR( p_name );
		if( !p_shdr )
			return NULL;
		return p_shdr->compi_v1( p_dev, p_src, p_name ); //, b_u124 );
	}
	U4 n_src = GPD_STRLEN( p_src );
	GPM_DELARRAY( p_err );
	
	if( FAILED(D3DX11CompileFromMemory( p_src, n_src, NULL, NULL, NULL, "VShader", "vs_4_0", 0, 0, NULL, &p_VS, &p_er, NULL ) ))
	{		
		p_err = (char*)p_er->GetBufferPointer();
		GPF_CON_FORMAT_Write( "\n VS ERROR:\n%s", p_err );
		return this;
	}

	// HRESULT hr_gs = E_FAIL; 
	// D3DX11CompileFromMemory( p_src, n_src, NULL, NULL, NULL, "GShader", "gs_4_0", 0, 0, NULL, &p_GS, &p_er, NULL );

	if( FAILED(D3DX11CompileFromMemory( p_src, n_src, NULL, NULL, NULL, "PShader", "ps_4_0", 0, 0, NULL, &p_PS, &p_er, NULL) ))
	{
		p_err = (char*)p_er->GetBufferPointer();
		GPF_CON_FORMAT_Write( "\n PS ERROR:\n%s", p_err );
		return this;
	}
		
	GPM_RELEASE( p_vs );
	GPM_RELEASE( p_ps );

	hr = p_dev->CreateVertexShader(p_VS->GetBufferPointer(), p_VS->GetBufferSize(), NULL, &p_vs );
	hr = p_dev->CreatePixelShader(p_PS->GetBufferPointer(), p_PS->GetBufferSize(), NULL, &p_ps );
	
	/*D3D11_INPUT_ELEMENT_DESC lay[] = 
	{
		{ "POSITION",	0,	 DXGI_FORMAT_R32G32B32_FLOAT,		0,	 0,		D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0,	 DXGI_FORMAT_R32G32_FLOAT,		0,	 3*4,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",	0,	 DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	 (3+2)*4,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};*/

	U4 n_lay = ARRAYSIZE(gp_lay_cell);
	hr = p_dev->CreateInputLayout( gp_lay_cell, n_lay, p_VS->GetBufferPointer(), p_VS->GetBufferSize(), &p_lay_cl );

	n_lay = ARRAYSIZE(gp_lay_cell_skin);
	hr = p_dev->CreateInputLayout( gp_lay_cell_skin, n_lay, p_VS->GetBufferPointer(), p_VS->GetBufferSize(), &p_lay_clsk );

	n_lay = ARRAYSIZE(gp_lay_u142);
	hr = p_dev->CreateInputLayout( gp_lay_u142, n_lay, p_VS->GetBufferPointer(), p_VS->GetBufferSize(), &p_lay_u142 );

	n_lay = ARRAYSIZE(gp_lay_vxc);
	hr = p_dev->CreateInputLayout( gp_lay_vxc, n_lay, p_VS->GetBufferPointer(), p_VS->GetBufferSize(), &p_lay_vxc );


	GPM_DELARRAY( p_err );
	return this;
}
const I44 gp_a_16x17[] = {

		{ 1, 16, 0, 0 },	
		{ 0, 15, 0, 1 },



		{  7, 16,  1,  1 },
		{  6, 15,  1,  2 },
	
		{  5, 14,  2,  2 },
		{  4, 13,  2,  3 },
		//   6    -------------
		{  3, 12,  1,  2 },
		{  2, 11,  3,  2 },
		{  1, 10,  3,  4 },
		{  0,  9,  5,  4 },



		{ 13, 16,  5,  0 },
		{ 12, 15,  1,  0 },
		//    12    --------- src step
		{ 11, 14,   1,   1 },
		{ 10, 13,-512,-512 },
		//    14    --------- dst off
		{  9, 12, gpe3D96z*32,					gpe3D96z*32 + gpe3D96y*32	},
		{  8, 11, gpe3D96z*32 + gpe3D96y*31,	gpe3D96y*32 },
		//    16    --------- dst step
		{  7, 10,  gpe3D96y, -gpe3D96z },
		{  6,  9, -gpe3D96y,  gpe3D96z },
		{  5,  8 },
		{  4,  7 },
		{  3,  6 },
		{  2,  5 },
		{  1,  4 },
		{  0,  3 },



		{ 13, 10 },
		{ 12,  9 },
		{ 11,  8 },
		{ 10,  7 },
		{  9,  6 },
		{  8,  5 },
		{  7,  4 },
		{  6,  3 },

};

U14* GPC3D96::ins_512x544_32on3( U4* p_bgra, U1* p_msk, U1 sid, U44* &p_stf_tree, U2& n_stf_tree )
{
	GPM_CLR;
	p3D80 = a3D96+(1 + gpe3D96y + gpe3D96z )*gpe3D96x;

	// no bone sk
	gpeMAP pln;
	if( !p_stf_tree )
	{
		p_stf_tree = new U44[0x100];
		n_stf_tree = 0;
	}
	if( !n_stf_tree )
	{
		p_stf_tree->null();
		p_stf_tree->x = *p_bgra;
		n_stf_tree = 1;
		for( U2 i = 1; i < 0x100; i++ )
		{
			n_stf_tree = p_stf_tree->tree_add( p_bgra[(i%16) + 512*(i/16)], n_stf_tree );
		}
	}
	U14 *p_src, *p_dst;	
	I4 src, dst, sid2 = sid%2, s_step, d_step, nb;
	U1 s1 = sid+1;
	for( U4 lev = 0; lev < GPM_N(gp_a_16x17); lev++ )
	{
		src =	  ( gp_a_16x17[lev].x+gp_a_16x17[sid].z)*32 
				+ ((gp_a_16x17[lev].y-gp_a_16x17[sid].w)*32 + 31)*512;
		for( U4 z = 0, y; z < 32; z++ )
		{
			p_src = (U14*)(p_bgra + src - z*512);
			p_dst = p3D80 + lev + z*gpe3D96z;
			for( y = 0; y < 32; y++ )
			{
				pln = (gpeMAP)p_stf_tree->tree_fnd( p_src[y].u4, n_stf_tree ); //( p_bgra[src - z*512 + y], n_stf_tree );
				if( p_msk )
				if( !(p_msk[pln]&0xc0) )
					continue;
				//dst =  lev + y*gpe3D96y + z*gpe3D96z;
				//p3D80[dst].pln = pln;
				//p3D80[dst].lev = sid;
				//p3D80[dst].ent = 1;

				dst = y*gpe3D96y; 
				p_dst[dst].pln = pln;
				p_dst[dst].lev = sid;
				p_dst[dst].ent = 1;
			}
		}

		nb = gp_a_16x17[sid+6].z;
		src = (gp_a_16x17[lev].x+gp_a_16x17[nb].z)*32 + ((gp_a_16x17[lev].y-gp_a_16x17[nb].w)*32 + 31)*512;
		s_step = gp_a_16x17[sid2+12].z;
		dst = gp_a_16x17[sid2+14].z+lev;
		d_step = gp_a_16x17[sid2+16].z;
		for( U4 i = 0; i < 32; i++, src += s_step, dst += d_step )
		{
			pln = (gpeMAP)p_stf_tree->tree_fnd( p_bgra[src], n_stf_tree );
			if( p_msk )
			if( p_msk[pln] > -1 )
				continue;
			p3D80[dst].pln = pln;
			p3D80[dst].lev = sid;
			p3D80[dst].ent = 1;
			//dst += add;
		}

		nb = gp_a_16x17[sid+6].w;
		src = (gp_a_16x17[lev].x+gp_a_16x17[nb].z)*32 + ((gp_a_16x17[lev].y-gp_a_16x17[nb].w)*32 + 31)*512;
		s_step = gp_a_16x17[sid2+12].w;
		dst = gp_a_16x17[sid2+14].w+lev;
		d_step = gp_a_16x17[sid2+16].w;
		for( U4 i = 0; i < 32; i++, src += s_step, dst += d_step )
		{
			pln = (gpeMAP)p_stf_tree->tree_fnd( p_bgra[src], n_stf_tree );
			if( p_msk )
			if( p_msk[pln] > -1 )
				continue;
			p3D80[dst].pln = pln;
			p3D80[dst].lev = sid;
			p3D80[dst].ent = 1;
			//dst += add;
		}
	}
	
	//GPM_DELARRAY( p_stf_tree );
	return a3D96;
}
//#define _mivan
//float4x4 gp_a_mxSID[] =
//{
//	// w:ID
//	{
//		  1.0f,  0.0f, 0.0f, 0.0f,
//		  0.0f,  1.0f, 0.0f, 0.0f,
//		  0.0f,  0.0f, 1.0f, 0.0f,
//		  0.0f,  0.0f, 0.0f, 1.0f,
//	},
//	// a_mx[0]:D
//	{
//		  0.0f, -1.0f,  0.0f, 0.0f,
//		  1.0f,  0.0f,  0.0f, 0.0f,
//		  0.0f,  0.0f,  1.0f, 0.0f,
//		-16.5f,  0.0f,-16.5f, 1.0f,
//	},
//	//  a_mx[1:R
//	{
//		  1.0f,  0.0f,  0.0f, 0.0f,
//		  0.0f,  1.0f,  0.0f, 0.0f,
//		  0.0f,  0.0f,  1.0f, 0.0f,
//		  0.0f,-16.5f,-16.5f, 1.0f,
//	},
//
//	//  a_mx[2:F
//	{
//		  0.0f,  0.0f,  1.0f, 0.0f,
//		  0.0f,  1.0f,  0.0f, 0.0f,
//		 -1.0f,  0.0f,  0.0f, 0.0f,
//		 16.5f,-16.5f,  0.0f, 1.0f,
//	},
//	//  a_mx[3:U
//	{
//		  0.0f,  1.0f,  0.0f, 0.0f,
//		  0.0f,  0.0f, -1.0f, 0.0f, // y
//		 -1.0f,  0.0f,  0.0f, 0.0f,
//		 16.5f,  0.0f, 16.5f, 1.0f,
//	},
//
//	//  a_mx[4:L
//	{
//		 -1.0f,  0.0f,  0.0f, 0.0f,
//		  0.0f,	 0.0f, -1.0f, 0.0f,
//		  0.0f, -1.0f,  0.0f, 0.0f,
//		  0.0f, 16.5f, 16.5f, 1.0f,
//	},
//	//  a_mx[5:b
//	{
//		  0.0f,  0.0f,-1.0f, 0.0f,
//		  1.0f,	 0.0f, 0.0f, 0.0f,
//		  0.0f, -1.0f, 0.0f, 0.0f,
//		-16.5f, 16.5f, 0.0f, 1.0f,
//	},
//	
//};
float4x4 gp_a_mxSID[] =
{
	// w:ID
	{
		  1.0f,  0.0f, 0.0f, 0.0f,
		  0.0f,  1.0f, 0.0f, 0.0f,
		  0.0f,  0.0f, 1.0f, 0.0f,
		  0.0f,  0.0f, 0.0f, 1.0f,
	},
	// a_mx[0]:D
	{
		  0.0f, -1.0f,  0.0f, 0.0f,
		  1.0f,  0.0f,  0.0f, 0.0f,
		  0.0f,  0.0f,  1.0f, 0.0f,
		  0.0f,  0.0f,  0.0f, 1.0f,
	},
	//  a_mx[1:R
	{
		  1.0f,  0.0f,  0.0f, 0.0f,
		  0.0f,  1.0f,  0.0f, 0.0f,
		  0.0f,  0.0f,  1.0f, 0.0f,
		  0.0f,  0.0f,  0.0f, 1.0f,
	},

	//  a_mx[2:F
	{
		  0.0f,  0.0f,  1.0f, 0.0f,
		  0.0f,  1.0f,  0.0f, 0.0f,
		 -1.0f,  0.0f,  0.0f, 0.0f,
		  0.0f,  0.0f,  0.0f, 1.0f,
	},
	//  a_mx[3:U
	{
		  0.0f,  1.0f,  0.0f, 0.0f,
		  0.0f,  0.0f, -1.0f, 0.0f, // y
		 -1.0f,  0.0f,  0.0f, 0.0f,
		  0.0f,  0.0f,  0.0f, 1.0f,
	},

	//  a_mx[4:L
	{
		 -1.0f,  0.0f,  0.0f, 0.0f,
		  0.0f,	 0.0f, -1.0f, 0.0f,
		  0.0f, -1.0f,  0.0f, 0.0f,
		  0.0f,  0.0f,  0.0f, 1.0f,
	},
	//  a_mx[5:b
	{
		  0.0f,  0.0f,-1.0f, 0.0f,
		  1.0f,	 0.0f, 0.0f, 0.0f,
		  0.0f, -1.0f, 0.0f, 0.0f,
		  0.0f,  0.0f,  0.0f, 1.0f,
	},
	
};
GPC3DGLB::GPC3DGLB( GPC_PROTO* pPROTO, ID3D11Device* p_dev, ID3D11DeviceContext* p_contx, U4* pBGR, GPC3DBLD* pBLD, GPC3D96* p96, GPC_CELL* pCELL, U1* pMSK ):GPC3DMX( p96->hub )
{
	gp_memcpy( &mxSS, gp_a_mxSID, sizeof(gp_a_mxSID) );
	build( pPROTO, this, p_dev, p_contx, pBGR, pBLD, p96, pCELL, pMSK );
}
GPC3DGLB::GPC3DGLB(GPC_PROTO* pPROTO, ID3D11Device* p_dev, ID3D11DeviceContext* p_contx, U4* pBGR, GPC3DBLD* pBLD, GPC_MSTR* pMSTR, GPC_CELL* pCELL, U1* pMSK) : GPC3DMX(pMSTR->hub)
{
	gp_memcpy(&mxSS, gp_a_mxSID, sizeof(gp_a_mxSID));
	build(pPROTO, this, p_dev, p_contx, pBGR, pBLD, pMSTR, pCELL, pMSK);
}
GPC3DGR_CACHE* GPC3DGR_CACHE::reset( GPC_DZR** pp_dzr )
{
	if( !this )
		return this;
	GPC3DGR *pGR = (GPC3DGR*)(p_cache ? p_cache->p_alloc : NULL);
	if( !pp_dzr )
	{
		for(U4 i = 0, xfnd; i < nLST; i++)
		{
			xfnd = pGR[i].xfnd;
			pGR[i].reset(-1);
		}
	}
	else for( U4 i = 0, xfnd; i < nLST; i++ ) 
	{
		xfnd = pGR[i].xfnd;
		pGR[i].reset( -1 );
		GPM_DEL( pp_dzr[xfnd] );
	}

	p_cache->lazy_reset();
	p_lst->lazy_reset();
	nLST = 0;
	return this;
}