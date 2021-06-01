#pragma once
#ifndef GP_3D
#define GP_3D

#include "stdafx.h"
#include "GP_ASM.h"
//#include "GP_PIC.h"
class GPC3DGR;

class float2sk : public float2
{
public:
	U14	ix,
		bgra;
	float2sk(){};

	float2sk& operator = ( const float2& b )
	{
		gp_memcpy( this, &b, sizeof(b) );
		return *this;
	}
};
/*
inline GPS_VX cell2vx( float2 cell ) 
{
	GPS_VX vx = (GPS_VX)0;
	vx.p = cell_x2p( cell.x );
	
	float	o0 = cell.y/256.0f;				// light offset
	vx.o = floor(o0);
		
	float	n0 = (256.0f/32.0f)*frac(o0),	// normal 0.0->0.226	// (3x3x3)/100 + face
			f0 = floor(n0);					// face 0->2			// 0 side, 1 front, 2 up
		
	n0 = 32.0f*frac(n0);					// normal 0->226		// (3x3x3) + face*100
	
	vx.n.y = n0 / 3.0f; 
	vx.n.x = floor( 3.0f * frac( vx.n.y ));
	vx.n.y = vx.n.y / 3.0f; 
	vx.n.z = floor( 3.0f * frac( vx.n.y ));
	vx.n.y = floor( vx.n.y );
	
	vx.n -= 1.0f;
	vx.n /= 2.0f;
	
	if( abs(f0-1.0f) < 0.25  ) // 1 Z XY     
			vx.t.xy = (vx.p+vx.n).xy;
	else if( abs(f0-2.0f) < 0.25 ) // 2 Y XZ
			vx.t.xy = (vx.p+vx.n).xz;
	else
			vx.t.xy = (vx.p+vx.n).zy; // 0 X YZ 
	vx.t.z = f0/3.0;
	return vx;
}*/

class U142
{
public:
	U14 posn, etc;

	U142(){};
	U142( U142& i )
	{
		gp_memcpy( this, &i, sizeof(i) );
	}
	U142( I4 i )
	{
		posn.u4 = etc.u4 = i;
	}
	U142& operator = ( const float2& cell )
	{
		U4 X = cell.x;
		posn.x = X%GPE_3D_W16;
		posn.y = X/(GPE_3D_W16*GPE_3D_W16); 
		posn.z = (X/GPE_3D_W16)%GPE_3D_W16;
		posn.w = ((U4)cell.y)%32;		// normal
		return *this;
	}
	U4	a0( const U44& mul )
	{
		return (mul&posn).sum_xyz();
	}
	
};

class GPC_CELL
{
public:
	U142	*ap_3d_u142[6];
	float2	*ap_3d_cell[6];
	U4		a_3d_cell_i[6][0x110],
			a_3d_cell_n[6][0x110];
	~GPC_CELL()
	{
		GPM_DELARRAY(ap_3d_u142[0]);
		GPM_DELARRAY(ap_3d_cell[0]);
	}
	gpeDIR GPF_CELL_LIG2FC_hi( float l )
	{
		I8 s = (I8)l & ~0x1f;
		switch( s )
		{
		case gpeLIG_t: 
			return gpeDIR_TOP;
		case gpeLIG_f:
			return gpeDIR_FRONT;
		case gpeLIG_r:
			return gpeDIR_RIGHT;
		case gpeLIG_l:
			return gpeDIR_LEFT;
		case gpeLIG_b: 
			return gpeDIR_BACK;
		case gpeLIG_d:
			return gpeDIR_DOWN;
		}
		return gpeDIR_ERROR;
	};
	float2* GP_A_CELL_get_hi( void );
	GPC_CELL( void )
	{
		gp_zero( a_3d_cell_i, sizeof(a_3d_cell_i) );
		gp_zero( a_3d_cell_n, sizeof(a_3d_cell_n) );

		float2	*p_cell_get = GP_A_CELL_get_hi(),
				*ap_ce[6]; //*ap_cs[6];
		U4	*ap_i[6]; //,*ap_n[6];

		ap_3d_u142[0] = new U142[0x100*12*4*6];
		ap_ce[0] = ap_3d_cell[0] = new float2[0x100*12*4*6];
		ap_i[0] = a_3d_cell_i[0];
		for( U4 i = 1; i < 6; i++ )
		{
			ap_3d_u142[i] = ap_3d_u142[0] + i*0x100*12*4;
			ap_ce[i] = ap_3d_cell[i] = ap_3d_cell[0] + i*0x100*12*4;
			ap_i[i] = a_3d_cell_i[i];
		}

		U1 a_12[0x10], *p_12e, n_bit;
		for( U2 ud = 1, u, un; ud <= 0x100; ud++ )
		{
			u = ud;
			n_bit = 0;
			p_12e = a_12;
			if( un = (u&0x0F) )
			if( un == 0x0F )
			{
				// TOP_FULL
				*p_12e = 32; p_12e++;
				if( un == u )
					 u = 0;
				else if(un = (u & 0xF0))
					u = (u & 0xF0) | ((u & 0xF0) >> 4);
				/**p_12e = 16; p_12e++;
				*p_12e = 18; p_12e++;
				*p_12e = 20; p_12e++;
				*p_12e = 22; p_12e++;*/
			} else {
				if( (un&0x03) == 0x03 )
				{
					// FRONT_D2
					*p_12e = 24; p_12e++;
				}
				else if( (u&0x15) == 1 )
				{
					// FRONT_DL
					*p_12e = 8; p_12e++;	
				}
				else if( (u&0x2a) == 2 )
				{
					// FRONT_DR
					*p_12e = 10; p_12e++;	
				}


				if( (un&0x05) == 0x05 )
				{
					// RIGHT_D2
					*p_12e = 26; p_12e++;
				}
				else if( (u&0x13) == 1 )
				{
					// RIGHT_DB
					*p_12e = 0; p_12e++;	
				}
				else if( (u&0x4C) == 4 )
				{
					// RIGHT_DF
					*p_12e = 2; p_12e++;	
				}

				if( (un&0xA) == 0x0A )
				{	
					// LEFT_D2
					*p_12e = 28; p_12e++;
				}
				else if( (u&0x23) == 2 )
				{
					// LEFT_DB
					*p_12e = 1; p_12e++;	
				}
				else if( (u&0x8C) == 8 )
				{
					// LEFT_DF
					*p_12e = 3; p_12e++;	
				}


				if( (un&0xC) == 0x0C )
				{
					// BACK_D2
					*p_12e = 30; p_12e++;
				}
				else if( (u&0x45) == 4 )
				{
					// BACK_DL
					*p_12e = 9; p_12e++;		
				}
				else if( (u&0x8A) == 8 )
				{
					// BACK_DR
					*p_12e = 11; p_12e++;	
				}
			}

			if(un = (u & 0xF0))
			if(un == 0xF0)
			{
				// DOWN_FULL
				*p_12e = 53; p_12e++;
				if(un == u)
					u = 0;
				else if(un = (u & 0x0f))
					u = (u & 0x0f) | ((u & 0x0f) << 4);

			} else {
				if((un & 0x30) == 0x30)
				{
					// FRONT_U2
					*p_12e = 38; p_12e++;
				} else if((u & 0x51) == 0x10)
				{
					// FRONT_UL
					*p_12e = 12; p_12e++;
				} else if((u & 0xA2) == 0x20)
				{
					// FRONT_UR
					*p_12e = 14; p_12e++;
				}


				if((un & 0x50) == 0x50)
				{
					// RIGHT_U2
					*p_12e = 43; p_12e++;

				} else if((u & 0x31) == 0x10)
				{
					// RIGHT_UB
					*p_12e = 4; p_12e++;
				} else if((u & 0xC4) == 0x40)
				{
					// RIGHT_UF
					*p_12e = 6; p_12e++;
				}

				if((un & 0xA0) == 0xA0)
				{
					// LEFT_U2
					*p_12e = 48; p_12e++;
				} else if((u & 0x32) == 0x20)
				{
					// LEFT_UB
					*p_12e = 5; p_12e++;
				} else if((u & 0xC8) == 0x80)
				{
					// LEFT_UF
					*p_12e = 7; p_12e++;
				}


				if((un & 0xC0) == 0x0C0)
				{
					// BACK_U2
					*p_12e = 51; p_12e++;
				} else if((u & 0x54) == 0x40)
				{
					// BACK_UL
					*p_12e = 13; p_12e++;
				} else if((u & 0xA8) == 0x80)
				{
					// BACK_UR
					*p_12e = 15; p_12e++;
				}
			}




			if( un = (u&0x33) ) 
			if( un == 0x33 )
			{
				// FRONT_FULL
				*p_12e = 39; p_12e++;
				if(un == u)
					u = 0;
				else if(un = (u & 0xCC))
					u = (u & 0xCC) | ((u & 0xCC) >> 2);

			} else {
				if( (un&0x03) == 0x03 )
				{
					// TOP_B2
					*p_12e = 25; p_12e++;
				} 
				else if( (u&0x15) == 1 )
				{
					// TOP_BDL
					*p_12e = 16; p_12e++;		
				}
				else if( (u&0x2a) == 2 )
				{
					// TOP_BDR
					*p_12e = 18;p_12e++;		
				}



				if( (un&0x11) == 0x11 )
				{
					// RIGHT_B2
					*p_12e = 33; p_12e++;
				}

				if( (un&0x22) == 0x22 )
				{
					// LEFT_B2
					*p_12e = 35; p_12e++;
				}



				if( (un&0x30) == 0x30 )
				{
					// DOWN_B2
					*p_12e = 37; p_12e++;
				}
				else if( (u&0x51) == 0x10 )
				{
					// DOWN_BUL
					*p_12e = 17; p_12e++;			
				}
				else if( (u&0xa2) == 0x20 )
				{
					// DOWN_BUR
					*p_12e = 19; p_12e++;			
				}
			}


			if(un = (u & 0xCC))
			if(un == 0xCC)
			{
				// BACK_FULL
				*p_12e = 52; p_12e++;
				if(un == u)
					u = 0;
				else if(un = (u & 0x33))
					u = (u & 0x33) | ((u & 0x33) << 2);

			} else {
				if((un & 0x0C) == 0x0C)
				{
					// TOP_F2
					*p_12e = 31; p_12e++;
				} else if((u & 0x45) == 0x04)
				{
					// TOP_FL
					*p_12e = 20; p_12e++;
				} else if((u & 0x8A) == 0x08)
				{
					// TOP_FR
					*p_12e = 22; p_12e++;
				}



				if((un & 0x44) == 0x44)
				{
					// RIGHT_F2
					*p_12e = 40; p_12e++;
				}


				if((un & 0x88) == 0x88)
				{
					// LEFT_F2
					*p_12e = 45; p_12e++;
				}



				if((un & 0xc0) == 0xC0)
				{
					// DOWN_F2
					*p_12e = 50; p_12e++;
				} else if((u & 0x54) == 0x40)
				{
					// DOWN_FL
					*p_12e = 21; p_12e++;
				} else if((u & 0xa8) == 0x80)
				{
					// DOWN_FR
					*p_12e = 23; p_12e++;
				}
			}




			if( un = (u&0x55) ) 
			if( un == 0x55 )
			{
				// RIGHT_FULL
				*p_12e = 44; p_12e++;
				if(un == u)
					u = 0;
				else if(un = (u & 0xAA))
					u = (u & 0xAA) | ((u & 0xAA) >> 1);
				/**p_12e = 0; p_12e++;
				*p_12e = 2; p_12e++;
				*p_12e = 4; p_12e++;
				*p_12e = 6; p_12e++;*/
			} else {
				if( (un&0x5) == 0x05 )
				{
					// TOP_L2
					*p_12e = 27; p_12e++;
				} 
				if( (un&0x11) == 0x11 )
				{
					// FRONT_L2
					*p_12e = 34; p_12e++;
				}
				if( (un&0x44) == 0x44 )
				{
					// BACK_L2
					*p_12e = 41; p_12e++;
				}
				if( (un&0x50) == 0x50 )
				{
					// DOWN_L2
					*p_12e = 42; p_12e++;
				}
			}

			if( un = (u&0xAA) ) 
			if( un == 0xAA )
			{
				// LEFT_FULL
				*p_12e = 49; p_12e++;
				if(un == u)
					u = 0;
				else if(un = (u & 0x55))
					u = (u & 0x55) | ((u & 0x55) << 1);
			} else {
				if( (un&0x0A) == 0x0A )
				{
					// TOP_R2
					*p_12e = 29; p_12e++;
				}
				if( (un&0x22) == 0x22 )
				{
					// FRONT_R2
					*p_12e = 36; p_12e++;
				}
				if( (un&0x88) == 0x88 )
				{
					// BACK_R2
					*p_12e = 46; p_12e++;
				}
				if( (un&0xA0) == 0xA0 )
				{
					// DOWN_R2
					*p_12e = 47; p_12e++;
				}
			}

			

			

			for( U1 i = 0; i < 6; i++ )
			{
				ap_i[i]++;
				*ap_i[i] = ap_ce[i]-ap_3d_cell[i];
			}
			gpeDIR d;
			for( U4 i = 0, e = p_12e-a_12; i < e; i++ )
			{
				d = GPF_CELL_LIG2FC_hi( p_cell_get[a_12[i]*4].y );
				gp_memcpy( ap_ce[d], p_cell_get + a_12[i]*4, 4*sizeof(*ap_ce[d]) );
				ap_ce[d] += 4;
			}

		}

		for( U1 d = 0; d < 6; d++ )
		{
			*ap_i[d] = ap_ce[d]-ap_3d_cell[d];
			//ap_cs[d] = ap_ce[d];
			for( U4* p_i = a_3d_cell_i[d], *p_n = a_3d_cell_n[d]; p_i < ap_i[d]; p_i++, p_n++ )
			{
				*p_n = p_i[1]-p_i[0];
			}
			
		}
		U142* p_u142 = ap_3d_u142[0];
		float2* p_float2 = ap_3d_cell[0];
		for( U4 i = 0; i < 0x100*12*4*6; i++ )
		{
			p_u142[i] = p_float2[i];
		}
	}
};



__declspec(align(16)) 
class GPC3D96
{
public:
	U1	a3DMSTR[gpe3DMSTR_ALL];
	U1	a3DPAT[gpe3DPAT_ALL];
	U44	hub;
	U14	a3D96[gpe3D96], *p3D80;
	
	GPC3D96( void )
	{
		GPM_CLR;
	}

	GPC3D96( U4* p_bgra, U4 w, U4 h, U1* p_msk )
	{
		GPM_CLR;
		p3D80 = a3D96+(1 + gpe3D96y + gpe3D96z )*gpe3D96x;
		
		if( !p_bgra )
			return;
		
		if( w == 512 )
		{
			if( h >= 544 )
			{
				U44* p_stf_tree = NULL;
				U2 n_stf_tree = 0;
 				ins_512x544_32on3( p_bgra, p_msk, 0, p_stf_tree, n_stf_tree ); // sid
				GPM_DELARRAY( p_stf_tree );
			}
			else if( h >= 64 )
				ins_512x64( p_bgra, p_msk, false );
		}
	}
	U14* ins_512x544_32on3( U4* p_bgra, U1* p_msk, U1 sid, U44* &p_stf_tree, U2& n_stf_tree );
	U14* ins_512x64xyz( U4* p_bgra, U1* p_msk, bool b_clr )
	{
		if( b_clr )
		{
			GPMZ( a3D96 );
		}
		// build color find
		U4	*p_bone = p_bgra + 512*32,
			n_stf_tree = 1, n_bone_tree = 1;
		U44	*p_stf_tree = new U44[0x100],
			*p_bone_tree = new U44[0x20];
		p_stf_tree->null();
		p_stf_tree->x = *p_bgra;
		p_bone_tree->null();
		p_bone_tree->x = *p_bone;
		gpeSTF stf;
		for( U2 i = 1; i < 0x100; i++ )
		{
			n_stf_tree = p_stf_tree->tree_add( p_bgra[(i%16) + 512*(i/16)], n_stf_tree );
		}
		for( U2 i = 1; i < 0x20; i++ )
		{
			n_bone_tree = p_bone_tree->tree_add( p_bone[(i%0x20) + 512*(i/0x20)], n_bone_tree );
		}

		p_bgra += 0x20;
		p_bone += 0x20;
		const U44 mul( 1, gpe3D96y, gpe3D96z ), msk(1,1,1);
		U44 xyz;
		hub.null();
		for( U4 src = 0, dst; src < 0x200*0x20; src++ )
		{
			if( (src%0x200) >= (0x200-0x20) )
			{
				src += 0x1f;
				continue;
			}
			
			stf = (gpeSTF)p_stf_tree->tree_fnd( p_bgra[src], n_stf_tree );
			if( p_msk )
			if( !(p_msk[stf]&0xc0) )
				continue;

			xyz.x = src%0x20;
			xyz.y = (src/0x20)%0x10;
			xyz.z = src/0x200;

			dst = (xyz&mul).sum_xyz();
			
			p3D80[dst].stf = stf;
			p3D80[dst].lev = p_bone_tree->tree_fnd( p_bone[src], n_stf_tree )+1;
			p3D80[dst].ent = 1;
			hub += (xyz & msk);
			hub.w++;
		}
		hub /= hub.w;

		GPM_DELARRAY( p_stf_tree );
		GPM_DELARRAY( p_bone_tree );
		return a3D96;
	}
	U14* ins_512x64xzy( U4* p_bgra, char* p_msk, bool b_clr )
	{
		if( b_clr )
		{
			GPMZ( a3D96 );
		}
		// build color find
		U4	*p_bone = p_bgra + 512*32,
			n_stf_tree = 1, n_bone_tree = 1;
		U44	*p_stf_tree = new U44[0x100],
			*p_bone_tree = new U44[0x20];
		p_stf_tree->null();
		p_stf_tree->x = *p_bgra;
		p_bone_tree->null();
		p_bone_tree->x = *p_bone;
		gpeSTF stf;
		for( U2 i = 1; i < 0x100; i++ )
		{
			n_stf_tree = p_stf_tree->tree_add( p_bgra[(i%16) + 512*(i/16)], n_stf_tree );
		}
		for( U2 i = 1; i < 0x20; i++ )
		{
			n_bone_tree = p_bone_tree->tree_add( p_bone[(i%0x20) + 512*(i/0x20)], n_bone_tree );
		}

		p_bgra += 0x20;
		p_bone += 0x20;
		const U44 mul( 1, gpe3D96y, gpe3D96z ), msk(1,1,1);
		U44 xyz;
		hub.null();
		for( U4 src = 0, dst; src < 0x200*0x20; src++ )
		{
			if( (src%0x200) >= (0x200-0x20) )
			{
				src += 0x1f;
				continue;
			}
			
			stf = (gpeSTF)p_stf_tree->tree_fnd( p_bgra[src], n_stf_tree );
			if( p_msk )
			if( p_msk[stf] > -1 )
				continue;

			xyz.x = src%0x20;
			xyz.z = (src/0x20)%0x10;
			xyz.y = src/0x200;

			dst = (xyz&mul).sum_xyz();
			
			p3D80[dst].stf = stf;
			p3D80[dst].lev = p_bone_tree->tree_fnd( p_bone[src], n_stf_tree )+1;
			p3D80[dst].ent = 1;
			hub += (xyz & msk);
			hub.w++;
		}
		hub /= hub.w;

		GPM_DELARRAY( p_stf_tree );
		GPM_DELARRAY( p_bone_tree );
		return a3D96;
	}
	U14* ins_512x64Xizy( U4* p_bgra, char* p_msk, bool b_clr )
	{
		if( b_clr )
		{
			GPMZ( a3D96 );
		}
		// build color find
		U4	*p_bone = p_bgra + 512*32,
			n_stf_tree = 1, n_bone_tree = 1;
		U44	*p_stf_tree = new U44[0x100],
			*p_bone_tree = new U44[0x20];
		p_stf_tree->null();
		p_stf_tree->x = *p_bgra;
		p_bone_tree->null();
		p_bone_tree->x = *p_bone;
		gpeSTF stf;
		for( U2 i = 1; i < 0x100; i++ )
		{
			n_stf_tree = p_stf_tree->tree_add( p_bgra[(i%16) + 512*(i/16)], n_stf_tree );
		}
		for( U2 i = 1; i < 0x20; i++ )
		{
			n_bone_tree = p_bone_tree->tree_add( p_bone[(i%0x20) + 512*(i/0x20)], n_bone_tree );
		}

		p_bgra += 0x20;
		p_bone += 0x20;
		const U44 mul( 1, gpe3D96y, gpe3D96z ), msk(1,1,1,0);
		U44 xyz;
		hub.null();
		for( U4 src = 0, dst; src < 0x200*0x20; src++ )
		{
			if( (src%0x200) >= (0x200-0x20) )
			{
				src += 0x1f;
				continue;
			}
			
			stf = (gpeSTF)p_stf_tree->tree_fnd( p_bgra[src], n_stf_tree );
			if( p_msk )
			if( p_msk[stf] > -1 )
				continue;

			xyz.x = 32-(src%0x20);
			xyz.z = (src/0x20)%0x10;
			xyz.y = src/0x200;

			dst = (xyz&mul).sum_xyz();
			
			p3D80[dst].stf = stf;
			p3D80[dst].lev = p_bone_tree->tree_fnd( p_bone[src], n_stf_tree )+1;
			p3D80[dst].ent = 1;
			hub += (xyz & msk);
			hub.w++;
		}
		hub /= hub.w;

		GPM_DELARRAY( p_stf_tree );
		GPM_DELARRAY( p_bone_tree );
		return a3D96;
	}
	U14* ins_512x64XiZiy( U4* p_bgra, U1* p_msk, bool b_clr )
	{
		if( b_clr )
		{
			GPMZ( a3D96 );
		}
		// build color find
		U4	*p_bone = p_bgra + 512*32,
			n_stf_tree = 1, n_bone_tree = 1;
		U44	*p_stf_tree = new U44[0x100],
			*p_bone_tree = new U44[0x20];
		p_stf_tree->null();
		p_stf_tree->x = *p_bgra;
		p_bone_tree->null();
		p_bone_tree->x = *p_bone;
		gpeSTF stf;
		for( U2 i = 1; i < 0x100; i++ )
		{
			n_stf_tree = p_stf_tree->tree_add( p_bgra[(i%16) + 512*(i/16)], n_stf_tree );
		}
		for( U2 i = 1; i < 0x20; i++ )
		{
			n_bone_tree = p_bone_tree->tree_add( p_bone[(i%0x20) + 512*(i/0x20)], n_bone_tree );
		}

		p_bgra += 0x20;
		p_bone += 0x20;
		const U44 mul( 1, gpe3D96y, gpe3D96z ), msk(1,1,1,0);
		U44 xyz;
		hub.null();
		for( U4 src = 0, dst; src < 0x200*0x20; src++ )
		{
			if( (src%0x200) >= (0x200-0x20) )
			{
				src += 0x1f;
				continue;
			}
			
			stf = (gpeSTF)p_stf_tree->tree_fnd( p_bgra[src], n_stf_tree );
			if( p_msk )
			if( !(p_msk[stf]&0xc0) )
				continue;

			xyz.x = 32-(src%0x20);			// pix
			xyz.z = 16-((src/0x20)%0x10);	// oszlop
			xyz.y = src/0x200;				// sor

			dst = (xyz&mul).sum_xyz();
			
			p3D80[dst].stf = stf;
			p3D80[dst].lev = p_bone_tree->tree_fnd( p_bone[src], n_stf_tree )+1;
			p3D80[dst].ent = 1;

			hub += (xyz & msk);
			hub.w++;
		}
		hub /= hub.w;
		GPM_DELARRAY( p_stf_tree );
		GPM_DELARRAY( p_bone_tree );
		return a3D96;
	}
	U14* ins_512x64( U4* p_bgra, U1* p_msk, bool b_clr )
	{
		if( b_clr )
		{
			GPMZ( a3D96 );
		}
		// build color find
		U4	*p_bone = p_bgra + 512*32,
			n_stf_tree = 1, n_bone_tree = 1;
		U44* p_stf_tree = new U44[0x100],
			*p_bone_tree = new U44[0x20];
		p_stf_tree->null();
		p_stf_tree->x = *p_bgra;
		p_bone_tree->null();
		p_bone_tree->x = *p_bone;
		gpeSTF stf;
		for( U2 i = 1; i < 0x100; i++ )
		{
			n_stf_tree = p_stf_tree->tree_add( p_bgra[(i%16) + 512*(i/16)], n_stf_tree );
		}
		for( U2 i = 1; i < 0x20; i++ )
		{
			n_bone_tree = p_bone_tree->tree_add( p_bone[(i%0x20) + 512*(i/0x20)], n_bone_tree );
		}

		p_bgra += 32;
		p_bone += 32;
		
		for( U4 sx = 0, dst, sy; sx < 0x200-0x20; sx++ )
		{
			dst = (0x2+(sx/0x20))*gpe3D96z + ((sx%0x20)+1)*gpe3D96y + 0x20;
			for( sy = 0, dst+gpe3D96y; sy < 0x20*0x200; sy += 0x200, dst-- )
			{
				stf = (gpeSTF)p_stf_tree->tree_fnd( p_bgra[sy+sx], n_stf_tree );
				if( p_msk )
				if( !(p_msk[stf]&0xc0) )
					continue;

				p3D80[dst].stf = stf;
				p3D80[dst].lev = p_bone_tree->tree_fnd( p_bone[sy+sx], n_stf_tree )+1;
				p3D80[dst].ent = 1;
			}
		}

		GPM_DELARRAY( p_stf_tree );
		GPM_DELARRAY( p_bone_tree );
		return a3D96;
	}

	U14* ins_16x16x16( U4* p_bgra, U14& xyz )
	{
		// ez a a3D96-be dolgozik 
		// a master 0 pontja xyz(16,16,16) -> p_3D
		const U44 trafo( 0x1, gpe3D96y, gpe3D96z, 0 );
		U14* p_dst = a3D96 + ( trafo & xyz).sum();
		for( U2 i = 0; i < 0x1000; i += 0x10 )
		{
			GPM_MEMCPY( p_dst + (((i/0x10)%0x10) + (i/0x100)*gpe3D96y)*gpe3D96y , p_bgra+i, 0x10 );
		}
		return p_dst;
	}
	U1* PROTO_PAT( U1* pPROTO, U1* p_pat )
	{
		if( !pPROTO )
			return NULL;
		gp_zero( pPROTO, gpe3DMAP_SID );
		U44 s4;
		const U44 mul(0,gpe3DPAT_Y,gpe3DPAT_Z,0);
		U4 d = 0, s, se;
		for( s4.z = 0; s4.z < gpe3DMAP_Y;	s4.z++ )
		for( s4.y = 0; s4.y < gpe3DMAP_Y;	s4.y++ )
		{
			s = s4*mul;
			gp_memcpy( pPROTO+d, p_pat+s, gpe3DMAP_Y );
			d += 32;
		}

		return pPROTO;
	}
	gpeMAP* PROTO( gpeMAP* pPROTO, U1* p_msk )
	{
		if( !pPROTO )
			return NULL;
		gp_zero( pPROTO, gpe3DMAP_SID );
		U44 s4;
		const U44 mul(0,gpe3D96y,gpe3D96z,0);
		U4 d = 0, s, se;
		for( s4.z = 0; s4.z < gpe3DMAP_Y;	s4.z++ )
		for( s4.y = 0; s4.y < gpe3DMAP_Y;	s4.y++ )
		{
			for( s = s4*mul, se = s+gpe3DMAP_Y; s < se; d++, s++ )
			{
				if( p_msk[p3D80[s].pln]&0xc0 )
					pPROTO[d] = p3D80[s].pln;
			}
		}

		return pPROTO;
	}
	U1* MSTR( U1* p_msk )
	{
		GPMZ( a3DMSTR );
		U44 s4;
		const U44 mul(0,gpe3D96y,gpe3D96z,0);
		U1 *pMST = a3DMSTR;
		U4 d = 0, s, se;
		for( s4.z = 0; s4.z <= gpe3DPAT_Y;	s4.z++ )
		for( s4.y = 0; s4.y <= gpe3DPAT_Y;	s4.y++ )
		{
			for( s = s4*mul, se = s+gpe3DMSTR_Y; s < se; pMST++, s++ )
			{
				if( p_msk[p3D80[s].stf]&0xc0 )
					*pMST = 1;
			}
		}

		//for( d4.z = 0, s4.z = 0; d4.z < gpe3DMSTR_ALL;	d4.z += gpe3DMSTR_Z,	s4.z += gpe3D96z )
		//for( d4.y = 0, s4.y = 0; d4.y < gpe3DMSTR_Z;	d4.y += gpe3DMSTR_Y,	s4.y += gpe3D96y )
		//for( d4.x = 0, s4.x = 0; d4.x < gpe3DPAT_Yp1;	d4.x++, s4.x++ )
		//{
		//	s = s4.sum_xyz();
		//	if( p_msk[p3D80[s].stf] > -1 )
		//		continue;
		//	d = d4.sum_xyz();
		//	a3DMSTR[d] = 1; //0x80;
		//}
		return a3DMSTR;
	}
	U1* MSTR2PAT( U1* p_dst = NULL )
	{
		if( !p_dst )
			p_dst = a3DPAT;
		// src: 80x65*65
		U8	*p_src = (U8*)a3DMSTR, 
			*p_s = p_src, *p_se = p_s + (gpe3DMSTR_ALL/sizeof(*p_s)),
			
			*p_sx,
			p0 = 0, p1;

		while( p_s < p_se )
		{
			p_sx = p_se; // + gpe3DMSTR_Y_U8;
			p_se -= gpe3DMSTR_Y_U8;
			
			p0 = (*p_sx)<<57;
			while( p_se < p_sx )
			{
				p_sx--;
				p1 = *p_sx;
				
				*p_sx = p0;
				*p_sx |= p1;
				*p_sx |= p1>>7;

				p0 = p1<<57;
			}
		}

		p_se = p_s + ((gpe3DMSTR_ALL-gpe3DMSTR_Z)/sizeof(*p_s));
		
		p_sx = p_s;
		while( p_sx < p_se )
		{
			*p_sx |= p_sx[gpe3DMSTR_Z_U8]<<4;
			p_sx++;
		}

		p_sx = p_s;
		while( p_sx < p_se )
		{
			*p_sx |= p_sx[gpe3DMSTR_Y_U8]<<2;
			p_sx++;
		}

		

		U8*p_d = (U8*)p_dst;
		U44 d4( 0,0,0,0 ), s4( 0,0,0,0 );
		U4 d, s;
		for( ;					d4.z < gpe3DPAT_ALL_U8;		d4.z += gpe3DPAT_Z_U8,	s4.z += gpe3DMSTR_Z_U8 )
		for( d4.y = 0, s4.y= 0;	d4.y < gpe3DPAT_Z_U8;		d4.y += gpe3DPAT_Y_U8,	s4.y += gpe3DMSTR_Y_U8 )
		{
			s = s4.sum_yz();
			d = d4.sum_yz();
			gp_memcpy( p_d+d, p_s+s, gpe3DPAT_Y );
		}
		return p_dst;
		
	}
	U1* MSTR2PAT( U1* p_dst, U14 lim )
	{
		if( !p_dst )
			p_dst = a3DPAT;
		// src: 80x65*65
		U8	*p_src = (U8*)a3DMSTR, 
			*p_s = p_src, *p_se = p_s + (gpe3DMSTR_ALL/sizeof(*p_s)),
			
			*p_sx,
			p0 = 0, p1;

		while( p_s < p_se )
		{
			p_sx = p_se; // + gpe3DMSTR_Y_U8;
			p_se -= gpe3DMSTR_Y_U8;
			
			p0 = (*p_sx)<<57;
			while( p_se < p_sx )
			{
				p_sx--;
				p1 = *p_sx;
				
				*p_sx = p0;
				*p_sx |= p1;
				*p_sx |= p1>>7;

				p0 = p1<<57;
			}
		}

		p_se = p_s + ((gpe3DMSTR_ALL-gpe3DMSTR_Z)/sizeof(*p_s));
		
		p_sx = p_s;
		while( p_sx < p_se )
		{
			*p_sx |= p_sx[gpe3DMSTR_Z_U8]<<4;
			p_sx++;
		}

		p_sx = p_s;
		while( p_sx < p_se )
		{
			*p_sx |= p_sx[gpe3DMSTR_Y_U8]<<2;
			p_sx++;
		}

		if( !lim.B )
			lim.B = gpe3DPAT_Y;
		if( !lim.G )
			lim.G = gpe3DPAT_Y;
		if( !lim.R )
			lim.R = gpe3DPAT_Y;

		U8*p_d = (U8*)p_dst;
		U44 d4( 0,0,0,0 ), s4( 0,0,0,0 );
		U4 d, s;
		for( ;					d4.z < gpe3DPAT_Z_U8*lim.R;		d4.z += gpe3DPAT_Z_U8,	s4.z += gpe3DMSTR_Z_U8 )
		for( d4.y = 0, s4.y= 0;	d4.y < gpe3DPAT_Y_U8*lim.G;		d4.y += gpe3DPAT_Y_U8,	s4.y += gpe3DMSTR_Y_U8 )
		{
			s = s4.sum_yz();
			d = d4.sum_yz();
			gp_memcpy( p_d+d, p_s+s, lim.B );
		}
		return p_dst;
		
	}

};

__declspec(align(16))
class GPC3DBLD
{
public:
	U142	*p_u142_alloc;			
	U4		n_u142_alloc;

	float2		*ap_v2[6];
	float2sk	*ap_v2sk[6];

	U4			a_pat_histi[0x100],
				an_v2_alloc[6],
				an_v2sk_alloc[6],

				an_v2_load[6],
		
				a_IX[6*8*8*8], nSTRIDE;
	U2			a_nIX[6*8*8*8];
	U8			n_cl, n_mxix;
	~GPC3DBLD()
	{
		for( U1 s = 0; s < 6; s++ )
		{
			GPM_DELARRAY( ap_v2[s] );
			GPM_DELARRAY( ap_v2sk[s] );
		}
	}
	GPC3DBLD( GPC_CELL *p_cell, U1* p_pat )
	{
		GPM_CLR;
		build( p_cell, p_pat );
		//U4 n_hi_sum = histi( p_pat );
		//for( U1 s = 0; s < 6; s++ )
		//{
		//	ap_v2[s] = build_cell(
		//							ap_v2[s],
		//							p_pat, a_pat_histi,
		//							p_cell->ap_3d_cell[s],
		//							p_cell->a_3d_cell_i[s],
		//							p_cell->a_3d_cell_n[s],
		//							s,
		//							a_IX+(s*8*8*8)
		//						);
		//	for( U2 i = s*512, e = i+511; i <= e; i++ )
		//	{
		//		if( i < e )
		//		{
		//			a_nIX[i] = ((a_IX[i+1]-a_IX[i])*6)/4;
		//			continue;
		//		}
		//		// ez az ucsó
		//		a_nIX[i] = ((an_v2_load[s]-a_IX[i])*6)/4; 
		//	}
		//}
	}

	U4 histi( U1* p_pat )
	{
		GPMZ( a_pat_histi ); //, sizeof(a_pat_histi) );
		I8 a0_start = 0, a0_end = gpe3DPAT_ALL; // + 1);

		for( I8 a0 = a0_start; a0 < a0_end; a0++ )
		{
			a_pat_histi[p_pat[a0]]++;
		}
		U4 n_sum = 0;
		for( U1 h = 1; h < 0xff; h++ )
			n_sum += a_pat_histi[h];

		return n_sum;
	}
	

	U142* build_cell(	
						GPC3DGR	*pGR, 
						U1		*p_pat, 
						U14		*p3D80,
						U142	*p_u142,
						U4&		n_u142,
						U142	* p_cell,
						U4		*p_s,
						U4		*p_sn,
						U1		s
					);
	GPC3DGR* buildGR( GPC3DGR* pGR, GPC_CELL *p_cell, U1* p_pat, U14* p3D80 );

	float2* build_cell( float2* p_v2, U1* p_pat, U4* p_histi, float2* p_cell, U4* p_s, U4* p_sn, U1 s, U4* p_512 );
	void EXPsk( GPC3D96* p96 );
	GPC3DBLD* build( GPC_CELL *p_cell, U1* p_pat )
	{
		if( !this )
			return NULL;
		n_mxix = nSTRIDE = 0;
		U4 n_hi_sum = histi( p_pat );
		for( U1 s = 0; s < 6; s++ )
		{
			ap_v2[s] = build_cell(
									ap_v2[s],
									p_pat, a_pat_histi,
									p_cell->ap_3d_cell[s],
									p_cell->a_3d_cell_i[s],
									p_cell->a_3d_cell_n[s],
									s,
									a_IX+(s*8*8*8)
								);
			for( U2 i = s*512, e = i+511; i <= e; i++ )
			{
				if( i < e )
				{
					a_nIX[i] = ((a_IX[i+1]-a_IX[i])*6)/4;
					if( n_mxix < a_nIX[i] )
						n_mxix = a_nIX[i];
					continue;
				}
				// ez az ucsó
				a_nIX[i] = ((an_v2_load[s]-a_IX[i])*6)/4;
				if( n_mxix < a_nIX[i] )
					n_mxix = a_nIX[i];
			}
		}
		return this;
	}
};

class GPC3DIX
{
public:
	D3D11_PRIMITIVE_TOPOLOGY	top;
	ID3D11Buffer				*p_ix;
	U4							n_ix4vx, n_ix_alloc;
	U2							a_idx[0x100];
	HRESULT						hr;

	/*GPC3DIX( D3D11_PRIMITIVE_TOPOLOGY tip )
	{
		GPM_CLR;
		top = tip;
	};*/

	GPC3DIX( D3D11_PRIMITIVE_TOPOLOGY tip, const U2* pIX, U4 nIX )
	{
		GPM_CLR;
		top = tip;
		GPM_MEMCPY(a_idx, pIX, n_ix4vx = nIX );
	};
	/*GPC3DIX( ID3D11Device* p_dev, ID3D11DeviceContext* p_contx, U4 n )
	{
		GPM_CLR;
		upl_ix_tri( p_dev, p_contx, n );
	}*/
	void upl_ix_tri( ID3D11Device* p_dev, ID3D11DeviceContext* p_contx, U4 n )
	{
		if( n_ix_alloc == 0xfff0 )
			return;

		if(n_ix_alloc > n )
			return;

		
		GPM_RELEASE( p_ix );

		/*const U2 a_idx[] = {
								0, 1, 2, 
								2, 3, 0 
							};*/
		//n_ix4vx = GPM_N(a_idx);
		n_ix_alloc = ((n/96)+1)*96;
		if(n_ix_alloc > 0xfff0 )
			n_ix_alloc = 0xfff0;

		D3D11_BUFFER_DESC bd;
		GPMZ( bd );
		bd.Usage = D3D11_USAGE_DYNAMIC;					// D3D11_USAGE_STAGING;// D3D11_USAGE_IMMUTABLE; //D3D11_USAGE_DEFAULT;//    D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
		bd.ByteWidth = n_ix_alloc * sizeof(*a_idx);			// size is the VERTEX struct * 3
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;			// use as a vertex buffer
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		// allow CPU to write in buffer

		hr = p_dev->CreateBuffer(&bd, NULL, &p_ix);      // create the buffer
		if( hr != S_OK )
			return;

		D3D11_MAPPED_SUBRESOURCE ms;
		hr = p_contx->Map( p_ix, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms );    // map the buffer
														  ///--------------
		GPF_MEM_set( ms.pData, n_ix_alloc/n_ix4vx, a_idx, n_ix4vx*sizeof(*a_idx) );
		//for( U4* p_d = ((U4*)ms.pData)+6, *p_de = p_d+(p_wbx64->gp_n_dx_cell_ix-6), plus = 4; p_d < p_de; )

		for( U2* p_d = ((U2*)ms.pData)+n_ix4vx, *p_de = p_d+(n_ix_alloc-n_ix4vx), plus = 4; p_d < p_de; )
		{
			p_d[0] += plus;
			p_d[1] += plus;
			p_d[2] += plus;
			p_d[3] += plus;
			p_d[4] += plus;
			p_d[5] += plus;
			
			p_d += 6;
			plus += 4;
		}
		p_contx->Unmap( p_ix, NULL );
	}

	void upl_ix_line(ID3D11Device* p_dev, ID3D11DeviceContext* p_contx, U4 n)
	{
		if(n_ix_alloc == 0xfff0)
			return;

		if(n_ix_alloc > n)
			return;


		GPM_RELEASE(p_ix);

		/*const U2 a_idx[] = { 
								0, 1,
								1, 2, 
								2, 3, 
								3, 0 
							};*/
		//n_ix4vx = GPM_N(a_idx);
		n_ix_alloc = ((n / 128) + 1) * 128;
		if(n_ix_alloc > 0xfff0)
			n_ix_alloc = 0xfff0;

		D3D11_BUFFER_DESC bd;
		GPMZ(bd);
		bd.Usage = D3D11_USAGE_DYNAMIC;					// D3D11_USAGE_STAGING;// D3D11_USAGE_IMMUTABLE; //D3D11_USAGE_DEFAULT;//    D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
		bd.ByteWidth = n_ix_alloc * sizeof(*a_idx);			// size is the VERTEX struct * 3
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;			// use as a vertex buffer
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		// allow CPU to write in buffer

		D3D11_MAPPED_SUBRESOURCE ms;
		hr = p_dev->CreateBuffer(&bd, NULL, &p_ix);      // create the buffer
		if(hr != S_OK)
			return;

		hr = p_contx->Map(p_ix, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
														  ///--------------
		GPF_MEM_set(ms.pData, n_ix_alloc / n_ix4vx, a_idx, n_ix4vx * sizeof(*a_idx));
		//for( U4* p_d = ((U4*)ms.pData)+6, *p_de = p_d+(p_wbx64->gp_n_dx_cell_ix-6), plus = 4; p_d < p_de; )

		for(U2* p_d = ((U2*)ms.pData) + n_ix4vx, *p_de = p_d + (n_ix_alloc - n_ix4vx), plus = 4; p_d < p_de; )
		{
			*p_d += plus;
			p_d++;
			*p_d += plus;
			p_d++;
			*p_d += plus;
			p_d++;
			*p_d += plus;
			p_d++;
			*p_d += plus;
			p_d++;
			*p_d += plus;
			p_d++;
			*p_d += plus;
			p_d++;
			*p_d += plus;
			p_d++;

			plus += 4;
		}
		p_contx->Unmap(p_ix, NULL);
	}

	void set(  ID3D11Device* p_dev, ID3D11DeviceContext* p_contx, U4 n )
	{
		if( !this )
			return;
		if( top != D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST )
			upl_ix_line(p_dev, p_contx, n);
		else 
			upl_ix_tri( p_dev, p_contx, n );
		p_contx->IASetIndexBuffer( p_ix, DXGI_FORMAT_R16_UINT, 0 );
	}
};


class GPC3DCL
{
public:
	U14							a_tx3D[gpe3DMSTR_TX];
	ID3D11Buffer				*apX11vx[6];
	ID3D11Texture3D				*p_tx3D_bgra;	
	ID3D11ShaderResourceView	*p_smp_bgra;
	U4	stride, tx_w, n_ix_max,
		anX11vx[6],
	//	anX11ix[6],
		a_IX[6*8*8*8];
	U2	a_nIX[6*8*8*8];
	HRESULT hr;
	~GPC3DCL()
	{
		for( U1 i = 0, n = GPM_N(apX11vx); i < n; i++ )
			GPM_RELEASE(apX11vx[i]);

		GPM_RELEASE(p_tx3D_bgra);
		GPM_RELEASE(p_smp_bgra);
	}
	GPC3DCL( ID3D11Device* p_dev, ID3D11DeviceContext* p_contx, GPC3DBLD* p3DBLD, GPC3D96* p3D96, bool b_t3D = false )
	{
		GPM_CLR;
		tx_w = gpe3DMSTR_Y;
		vx_upl( p_dev, p3DBLD );
		if( !b_t3D )
			return;
		tx3D_ins( p3D96->a3D96 + (gpe3D96z+gpe3D96y+1)*8 );
		tx3D_upl( p_dev, p_contx, p3D96 );
	}
	void tx3D_ins( U14* p_ins )
	{
		for( U4 i = 0, j = 0, k; i < gpe3DMSTR_TX; i += gpe3DMSTR_Y, j++ )
		{
			k = ((j%gpe3DMSTR_Y)+(j/gpe3DMSTR_Y)*gpe3D96y)*gpe3D96y;
			gp_memcpy( a_tx3D+i, p_ins+k, gpe3DMSTR_Y );
		}
	}
	void tx3D_upl( ID3D11Device* p_dev, ID3D11DeviceContext* p_contx, GPC3D96* p3D96 )
	{
		bool b_set = false;
	reset:
		if( b_set )
		{

		}
		if( !p_tx3D_bgra )
		{
			D3D11_SUBRESOURCE_DATA tbsd;
			tbsd.SysMemPitch = tx_w*4;
			tbsd.SysMemSlicePitch = 0; //tbsd.SysMemPitch*tx_w*tx_w; // w*h*4; // Not needed since this is a 2d texture
			tbsd.pSysMem = NULL; //p3D96->a3D96;			// (void*)p_pix;
		
			D3D11_TEXTURE3D_DESC desc;
			GPMZ( desc );
			desc.Width = desc.Height = desc.Depth = tx_w;
		
			desc.MipLevels = 1;
					
			desc.Format = GPD_DXGI_FORMAT_GLOBAL_BGRA; //DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; //|D3D11_CPU_ACCESS_READ;
			desc.MiscFlags = 0;
			if( FAILED( p_dev->CreateTexture3D( &desc, NULL, &p_tx3D_bgra )) )
				return;
		}

		if( !p_smp_bgra )
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDSC;
			GPMZ( srvDSC );
			srvDSC.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // DXGI_FORMAT_R8G8B8A8_UNORM;
			srvDSC.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
			srvDSC.Texture3D.MipLevels = 1;

			if( FAILED( p_dev->CreateShaderResourceView( p_tx3D_bgra, &srvDSC, &p_smp_bgra ) ))
				return;
		}

		D3D11_MAPPED_SUBRESOURCE ms;
		if( FAILED(p_contx->Map( p_tx3D_bgra, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms)) )
		{
			b_set = true;
			goto reset;
		}
	
		// copy the data
		if( ms.RowPitch == tx_w*sizeof(U4) )
		{
			gp_memcpy( ms.pData, a_tx3D, ms.DepthPitch*tx_w );
		} else {
			U1 *p_dst = (U1*)ms.pData;
			U1 *p_src = (U1*)a_tx3D;
			U8 n_cpy = tx_w*sizeof(U4);
			if( ms.RowPitch*tx_w == ms.DepthPitch )
			{
				for( U1 *p_de = p_dst + ms.RowPitch*tx_w*tx_w; p_dst < p_de; p_dst += ms.RowPitch, p_src += n_cpy )
				{
					gp_memcpy( p_dst, p_src, n_cpy );
				}
			} else {
				for( U1 *p_dde = p_dst + ms.DepthPitch*tx_w; p_dst < p_dde; p_dst += ms.DepthPitch )
				for( U1 *p_dr = p_dst, *p_dre = p_dr + ms.RowPitch*tx_w; p_dr < p_dre; p_dr += ms.RowPitch, p_src += n_cpy )
				{
					gp_memcpy( p_dr, p_src, n_cpy );
				}
			}
		}
		
		p_contx->Unmap( p_tx3D_bgra, NULL);
	}
	void vx_upl( ID3D11Device* p_dev, GPC3DBLD* p3DBLD )
	{
		for(U2 s = 0; s < GPM_N(apX11vx); s++ )
			GPM_RELEASE( apX11vx[s] );
		// create the vertex buffer
		//=====-===--==---=------- ---  --   -
		// Resource		|Usage		|Default	|Dynamic	|Immutable	|Staging
		//--------------+-----------+-----------+-----------+-----------+--------
		// GPU-Read		|yes		|yes		|yes		|yes1		|
		// GPU-Write	|			|yes		|			|			|yes1
		// CPU-Read		|			|			|			|yes1		|
		// CPU-Write	|			|			|yes		|			|yes1
		//=====-===--==---=------- ---  --   -
		stride = p3DBLD->nSTRIDE; //sizeof( **p_3d->ap_v2 );
		if( n_ix_max < p3DBLD->n_mxix )
				n_ix_max = p3DBLD->n_mxix;
		D3D11_BUFFER_DESC bDSC;
		D3D11_SUBRESOURCE_DATA iniDT;
		GPMZ( bDSC );
		bDSC.Usage = D3D11_USAGE_DEFAULT;				// D3D11_USAGE_STAGING;// D3D11_USAGE_IMMUTABLE; //D3D11_USAGE_DEFAULT;//    D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
		bDSC.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// use as a vertex buffer
		iniDT.SysMemPitch = 0;
		iniDT.SysMemSlicePitch = 0;			
		D3D11_MAPPED_SUBRESOURCE ms;
		bool b_cpy = false;
		for( U1 s = 0; s < 6; s++ )
		{
			if( p3DBLD->an_v2_load[s] )
			{
				bDSC.ByteWidth = p3DBLD->an_v2_load[s] * stride;       // size is the VERTEX struct * 3
				iniDT.pSysMem = p3DBLD->ap_v2sk[s] ? p3DBLD->ap_v2sk[s] : p3DBLD->ap_v2[s];
			
				hr = p_dev->CreateBuffer(&bDSC, &iniDT, &apX11vx[s] );       // create the buffer
				if( hr != S_OK )
					return;
			}  
			anX11vx[s] = p3DBLD->an_v2_load[s];
			//anX11ix[s] = (anX11vx[s]/4)*6;
			
			if( b_cpy )
				continue;

			b_cpy = true;
			gp_memcpy( a_IX, p3DBLD->a_IX, sizeof(a_IX) );
			gp_memcpy( a_nIX, p3DBLD->a_nIX, sizeof(a_nIX) );
		}
	}

};





class GPC3DCNL
{
	public:
		float2	*p_key, *p_load;
		I8	n_key;
	~GPC3DCNL()
	{
		GPM_DELARRAY( p_key );
	}

	float find_sec( float sec )
	{
		if( !this )
			return 0.0f;
		
		if( !p_key )
			return 0.0f;
		
		if( p_key >= p_load )
			return 0.0f;

		if( sec >= p_load[-1].y )
			return p_load[-1].x;

		float2 *p_hi = p_key;
		while( p_hi < p_load )
		{
			if( p_hi->y < sec )
			{
				p_hi++;
				continue;
			} 

			if( p_hi->y > sec )
				break;

			// ilyenkor pontosan megyegyzik a key idõ a seckel
			return p_hi->x;
		}

		if( p_hi == p_key )
			return p_key->x;

		float2	prev =  p_hi[-1],
				next = *p_hi; 
		
		next -= prev;
		sec -= prev.y;
		return next.x*sec/next.y+prev.x;
	}
};


class GPC3DACT
{
public:
	float	begin, end, start, mix, fade, gim, loop, long_s;
	I8		begin_ms, end_ms, start_ms, loop_ms, long_ms;
	GPC3DACT(	float _begin, float _end,
			float _start, float _mix,
			float _fade, float _gim, float _loop )
	{
		begin	= _begin;
		end	= _end;
		start	= _start;
		mix	= _mix;
		fade	= _fade;
		gim	= _gim;
		loop	= _loop;

		long_ms = (I8)((long_s = end-begin)*sec2ms_f);
		begin_ms = (I8)(begin*sec2ms_f);
		end_ms = (I8)(end*sec2ms_f);
		loop_ms = (I8)(loop*sec2ms_f);
		start_ms = (I8)(start*sec2ms_f);
	}
	float sec( I8 ms )
	{
		float sec = 0.0;
		ms += start_ms - begin_ms;
		//I8 act_ms = start_ms + act_ms - begin_ms;

		if( ms < long_ms )
		{
			sec = float(ms)/sec2ms_f;
		}
		else if( end_ms <= loop_ms )
			sec = long_s;
		else
			sec = float(ms%long_ms)/sec2ms_f;

		return sec + begin;
	}
};

class GPCTRCK
{
public:
	char	s_name[0x100];
	
	I8	mom_id, item_id, gio_id, n_level;
	GPC3DCNL	a_cnl[9];
	float3		a_rst[3];
	void clear( void )
	{
		for( I8 c = 0; c < 9; c++ )
		{
			GPM_DELARRAY( a_cnl[c].p_key );
			a_cnl[c].n_key = 0;
		}
		GPM_CLR;
	}
};


class GPC3DGIM
{
public:
	
	GPCTRCK	*track_p_alloc,
			*track_p_load;

	I8		track_n_alloc,
			gim_id,
			n_load, n_update;
	
	float	sec1, sec2, lerp;

	float4x4		a_mx[0x30],
					*p_mxi;
	ID3D11Buffer	*p_x11;

	float3	pos, ypr, scl,
			pos1, ypr1, scl1,
			pos2, ypr2, scl2;
	HRESULT	hr;

	GPC3DGIM( void )
	{
		GPM_CLR;
		
		sec1 = sec2 = lerp = 0.0f;
	}
	~GPC3DGIM()
	{
		GPM_DELARRAY( track_p_alloc );
		GPM_RELEASE( p_x11 );
	}
	ID3D11Buffer* upl_inv( ID3D11Device* p_dev )
	{
		if( p_x11 )
			return p_x11;
		if( !p_mxi )
			gim_set( 0, 0, 0, NULL, true );
		
		D3D11_BUFFER_DESC bDSC;
		GPMZ( bDSC );
		bDSC.ByteWidth = sizeof(a_mx) - ((char*)p_mxi-(char*)a_mx);
		bDSC.Usage = D3D11_USAGE_DEFAULT;				// D3D11_USAGE_STAGING;// D3D11_USAGE_IMMUTABLE; //D3D11_USAGE_DEFAULT;//    D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
		bDSC.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// use as a vertex buffer
		
		D3D11_SUBRESOURCE_DATA iniDT;
		GPMZ( iniDT );
		iniDT.pSysMem = p_mxi;

		hr = p_dev->CreateBuffer( &bDSC, &iniDT, &p_x11 );
		return p_x11;
	}
	float4x4* gim_set( float s1, float s2, float l, float4x4* p_mx, bool b_inv )
	{
		U4 n = track_p_load-track_p_alloc;
		if( b_inv )
		{
			if( !p_mxi )
			{
				p_mxi = a_mx+0x18;
				for( GPCTRCK *p_t = track_p_alloc, *p_te = track_p_load; p_t < p_te; p_t++ )
				{
					for( U4 n_para = 12; n_para < 18; n_para++ )
					{
						switch( n_para )
						{
							// ------------ RESET ------------
							// reset_pos
							case 12:
								pos.x = pos2.x = pos1.x = ((float*)p_t->a_rst)[n_para-12];
								break;
							case 13:
								pos.y = pos2.y = pos1.y = ((float*)p_t->a_rst)[n_para-12];
								break;
							case 14:
								pos.z = pos2.z = pos1.z = ((float*)p_t->a_rst)[n_para-12];
								break;
							// reset_ypr
							case 15:
								ypr.x = ypr2.x = ypr1.x = ((float*)p_t->a_rst)[n_para-12];
								break;
							case 16:
								ypr.y = ypr2.y = ypr1.y = ((float*)p_t->a_rst)[n_para-12];
								break;
							case 17:
								ypr.z = ypr2.z = ypr1.z = ((float*)p_t->a_rst)[n_para-12];
								break;
						}
					}
					n = p_t-track_p_alloc;
					//a_mx[n].id();
					a_mx[n].ypr( ypr );
					a_mx[n].t4.float3w1( pos1 );
					if( n != p_t->mom_id )	// azaz sajátmagával
						a_mx[n] *= a_mx[p_t->mom_id];
					p_mxi[n] = a_mx[n].inv();
				}
				scl = scl2 = scl1 = 1.0; //.xyz( 1.0 );
			}
			GPM_MEMCPY( p_mx, p_mxi, n );
			return p_mx;
		}
		if( s1 == sec1 && s2 == sec2 && l == lerp )
		{	
			GPM_MEMCPY( p_mx, a_mx, n );
			return p_mx;
		}
		
		for( GPCTRCK *p_t = track_p_alloc, *p_te = track_p_load; p_t < p_te; p_t++ )
		{
			//I8 mom_id = p_t->mom_id;
			for( U4 n_para = 3; n_para < 18; n_para++ )
			{
				switch( n_para )
				{
					case 3:
						if( l == 0.0f )
						{
							pos.x = p_t->a_cnl[n_para-3].find_sec( s1 );
							break;
						}
						else if( l == 1.0f )
						{
							pos.x = p_t->a_cnl[n_para-3].find_sec( s2 );
							break;
						}
						if( s1 != sec1 )
							pos1.x = p_t->a_cnl[n_para-3].find_sec( s1 );
						if( s2 != sec2 )
							pos2.x = p_t->a_cnl[n_para-3].find_sec( s2 );
						
						pos.x = ((pos2.x-pos1.x)*l) + pos1.x;
						break;
					case 4:
						if( l == 0.0f )
						{
							pos.y = p_t->a_cnl[n_para-3].find_sec( s1 );
							break;
						}
						else if( l == 1.0f )
						{
							pos.y = p_t->a_cnl[n_para-3].find_sec( s2 );
							break;
						}
						if( s1 != sec1 )
							pos1.y = p_t->a_cnl[n_para-3].find_sec( s1 );
						if( s2 != sec2 )
							pos2.y = p_t->a_cnl[n_para-3].find_sec( s2 );
						
						pos.y = ((pos2.y-pos1.y)*l) + pos1.y;
						break;
					case 5:
						if( l == 0.0f )
						{
							pos.z = p_t->a_cnl[n_para-3].find_sec( s1 );
							break;
						}
						else if( l == 1.0f )
						{
							pos.z = p_t->a_cnl[n_para-3].find_sec( s2 );
							break;
						}
						if( s1 != sec1 )
							pos1.z = p_t->a_cnl[n_para-3].find_sec( s1 );
						if( s2 != sec2 )
							pos2.z = p_t->a_cnl[n_para-3].find_sec( s2 );
						
						pos.z = ((pos2.z-pos1.z)*l) + pos1.z;
						break;
					// ypr
					case 6:
						if( l == 0.0f )
						{
							ypr.x = p_t->a_cnl[n_para-3].find_sec( s1 );
							break;
						}
						else if( l == 1.0f )
						{
							ypr.x = p_t->a_cnl[n_para-3].find_sec( s2 );
							break;
						}
						if( s1 != sec1 )
							ypr1.x = p_t->a_cnl[n_para-3].find_sec( s1 );
						if( s2 != sec2 )
							ypr2.x = p_t->a_cnl[n_para-3].find_sec( s2 );
						
						ypr.x = ((ypr2.x-ypr1.x)*l) + ypr1.x;
						break;
					case 7:
						if( l == 0.0f )
						{
							ypr.y = p_t->a_cnl[n_para-3].find_sec( s1 );
							break;
						}
						else if( l == 1.0f )
						{
							ypr.y = p_t->a_cnl[n_para-3].find_sec( s2 );
							break;
						}
						if( s1 != sec1 )
							ypr1.y = p_t->a_cnl[n_para-3].find_sec( s1 );
						if( s2 != sec2 )
							ypr2.y = p_t->a_cnl[n_para-3].find_sec( s2 );
						
						ypr.y = ((ypr2.y-ypr1.y)*l) + ypr1.y;
						break;
					case 8:
						if( l == 0.0f )
						{
							ypr.z = p_t->a_cnl[n_para-3].find_sec( s1 );
							break;
						}
						else if( l == 1.0f )
						{
							ypr.z = p_t->a_cnl[n_para-3].find_sec( s2 );
							break;
						}
						if( s1 != sec1 )
							ypr1.z = p_t->a_cnl[n_para-3].find_sec( s1 );
						if( s2 != sec2 )
							ypr2.z = p_t->a_cnl[n_para-3].find_sec( s2 );
						
						ypr.z = ((ypr2.z-ypr1.z)*l) + ypr1.z;
						break;
					// scale
					case 9:
						if( l == 0.0f )
						{
							scl.x = p_t->a_cnl[n_para-3].find_sec( s1 );
							break;
						}
						else if( l == 1.0f )
						{
							scl.x = p_t->a_cnl[n_para-3].find_sec( s2 );
							break;
						}
						if( s1 != sec1 )
							scl1.x = p_t->a_cnl[n_para-3].find_sec( s1 );
						if( s2 != sec2 )
							scl2.x = p_t->a_cnl[n_para-3].find_sec( s2 );
						
						scl.x = ((scl2.x-scl1.x)*l) + scl1.x;
						break;
					case 10:
						if( l == 0.0f )
						{
							scl.y = p_t->a_cnl[n_para-3].find_sec( s1 );
							break;
						}
						else if( l == 1.0f )
						{
							scl.y = p_t->a_cnl[n_para-3].find_sec( s2 );
							break;
						}
						if( s1 != sec1 )
							scl1.y = p_t->a_cnl[n_para-3].find_sec( s1 );
						if( s2 != sec2 )
							scl2.y = p_t->a_cnl[n_para-3].find_sec( s2 );
						
						scl.y = ((scl2.y-scl1.y)*l) + scl1.y;
						break;
					case 11:
						if( l == 0.0f )
						{
							scl.z = p_t->a_cnl[n_para-3].find_sec( s1 );
							break;
						}
						else if( l == 1.0f )
						{
							scl.z = p_t->a_cnl[n_para-3].find_sec( s2 );
							break;
						}
						if( s1 != sec1 )
							scl1.z = p_t->a_cnl[n_para-3].find_sec( s1 );
						if( s2 != sec2 )
							scl2.z = p_t->a_cnl[n_para-3].find_sec( s2 );
						
						scl.z = ((scl2.z-scl1.z)*l) + scl1.z;
						break;
				}
			}

			if( l == 0.0f || l == 1.0 )
			{
				pos1 = pos2 = pos;
				ypr1 = ypr2 = ypr;
				scl1 = scl2 = scl;
			}

			n = p_t-track_p_alloc;
			
			
			//a_mx[n].id();
			a_mx[n].ypr( ypr );
			a_mx[n].t3 = pos;
			if( n != p_t->mom_id )	// azaz sajátmagával
				a_mx[n] *= a_mx[p_t->mom_id];

		}

		sec1 = s1; 
		sec2 = s2;
		lerp = l;

		GPM_MEMCPY( p_mx, a_mx, n );
		return p_mx;
	}


};


typedef enum GPE_LWS_COM:I8
{
		GPE_LWS_COM_LoadObjectLayer	,
		GPE_LWS_COM_AddBone		,
		GPE_LWS_COM_AddNullObject	,
		GPE_LWS_COM_AddLight		,
		GPE_LWS_COM_AddCamera		,
		GPE_LWS_COM_NumChannels	,
		GPE_LWS_COM_Channel		,
		GPE_LWS_COM_C_open		,
		GPE_LWS_COM_Envelope		,
		GPE_LWS_COM_Key		,
		GPE_LWS_COM_C_close		,
		GPE_LWS_COM_ParentItem	,
		GPE_LWS_COM_BoneName		,
		GPE_LWS_COM_BoneRestPosition	,
		GPE_LWS_COM_BoneRestDirection	,
		GPE_LWS_COM_BoneRestLength	,
} GPT_LWS_COM;
class GPC3DLWS
{
public:
	GPC_DICT	*p_lws_com,
				*p_lws_bons;

	GPC3DGIM	*p_gim;

	GPC3DLWS( char* p_p, char* p_pf, const char* p_file, float dim );
	~GPC3DLWS()
	{
		GPM_DEL( p_lws_com );
		GPM_DEL( p_lws_bons );
	}
};

__declspec(align(16)) class GPC3DMX
{
public:

	float4x4	mxSS,
				a_mx[0x18],
				mxLS;
	float4		sec,hub;

	U4				size;
	HRESULT			hr;
	ID3D11Buffer	*p_x11inv,		// KÖZÖS ez a GIM hozza létre és tárolja 
					*p_x11;			// EGYEDI mozdulat
	
	~GPC3DMX( )
	{
		GPM_RELEASE( p_x11 );
	}
	GPC3DMX( U44 hb )
	{
		mxSS.id();
		GPF_MEM_set( a_mx, GPM_N(a_mx)+1, &mxSS, sizeof(mxSS) );
		//GPF_MEM_set( a_mxi, GPM_N(a_mxi), &mx_w, sizeof(mx_w) );
		sec.null();
		size = GPM_OFF( GPC3DMX, size );
		p_x11inv = p_x11 = NULL;
		hub = hb;
	}

	// ---------- INVERS --------------------------
	//ID3D11Buffer* upl_inv( ID3D11Device* p_dev )
	//{
	//	if( p_inv )
	//		return p_inv;
	//	D3D11_BUFFER_DESC bDSC;
	//	GPMZ( bDSC );
	//	bDSC.ByteWidth = aSIZE[0];
	//	bDSC.Usage = D3D11_USAGE_DEFAULT;				// D3D11_USAGE_STAGING;// D3D11_USAGE_IMMUTABLE; //D3D11_USAGE_DEFAULT;//    D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	//	bDSC.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// use as a vertex buffer
	//	
	//	D3D11_SUBRESOURCE_DATA iniDT;
	//	GPMZ( iniDT );
	//	iniDT.pSysMem = &a_mxi;
	//	hr = p_dev->CreateBuffer( &bDSC, &iniDT, &p_inv );
	//	return p_inv;
	//}
	//void updt_inv( void )
	//{
	//	GPM_RELEASE( p_inv );
	//}
	//void set_inv( ID3D11DeviceContext* p_contx )
	//{
	//	if( !p_x11inv )
	//		return;
	//	p_contx->VSSetConstantBuffers( 1, 1, &p_x11inv );
	//}

	// ---------- ACTION --------------------------
	ID3D11Buffer* upld( ID3D11Device* p_dev )
	{
		if( p_x11 )
			return p_x11;

		D3D11_BUFFER_DESC bDSC;
		GPMZ( bDSC );
		bDSC.ByteWidth = size;
		bDSC.Usage = D3D11_USAGE_DEFAULT;				// D3D11_USAGE_STAGING;// D3D11_USAGE_IMMUTABLE; //D3D11_USAGE_DEFAULT;//    D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
		bDSC.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// use as a vertex buffer
		
		D3D11_SUBRESOURCE_DATA iniDT;
		GPMZ( iniDT );
		iniDT.pSysMem = this;

		hr = p_dev->CreateBuffer( &bDSC, &iniDT, &p_x11 );
		return p_x11;
	}
	void updt( U8 t0 )
	{
		GPM_RELEASE( p_x11 );
		sec.x = double(t0) / sec2ms;
	}
	void set_act( ID3D11Device* p_dev, ID3D11DeviceContext* p_contx )
	{
		if( p_x11inv )
			p_contx->VSSetConstantBuffers( 1, 1, &p_x11inv );

		if( !p_x11 )
			upld( p_dev );

		p_contx->VSSetConstantBuffers( 2, 1, &p_x11 );
	}

};
class GPC3Dvxc
{
public:
	union 
	{
		struct
		{
			float	x, y, z, w;
		};
		struct
		{
			float	x, y, z;
			U14		c4;
		};
		struct
		{
			float3	f3;
			U4		u4;
		};
		struct
		{
			float4 f4;
		};
	};
	char* LWO_xyz( char* pSRC )
	{
		x = GPF_SWAP_F4(pSRC);
		y = GPF_SWAP_F4(pSRC + 4);
		z = GPF_SWAP_F4(pSRC + 8);
		return pSRC + 12;
	}

	GPC3Dvxc* LWOn_xyz( char* pSRC, U4 n, float dim )
	{
		for(U4 i = 0, ie = n; i < ie; i++)
		{
			pSRC = this[i].LWO_xyz( pSRC );
			this[i].u4 = 0;
			this[i].f4 *= dim;
		}
		return this;
	}
	GPC3Dvxc* null(void)
	{
		GPM_CLR;
		return this;
	}
	char* LWO_xy( char* pSRC )
	{
		x = GPF_SWAP_F4(pSRC);
		y = GPF_SWAP_F4(pSRC+4);

		return pSRC+8;
	}
	
	char* LWOi_xy( char* pSRC )
	{
		I8 i = *pSRC;
		if(i == -1)
		{
			i = GPF_SWAP_U4(pSRC);
			pSRC += 4;
		} else {
			i = GPF_SWAP_U2(pSRC);
			pSRC += 2;
		}

		/*this[i].x = GPF_SWAP_F4(pSRC); pSRC += 4;
		this[i].y = GPF_SWAP_F4(pSRC); pSRC += 4;*/

		return this[i].LWO_xy( pSRC );
	}


	
	GPC3Dvxc(){};
};
class NEAR4096
{
public:
	U4	aN[0x1000], aI[0x1000], aX[0x1000];
	U84	nQ;

	NEAR4096(void)
	{
		GPM_CLR;
	}
	void ii(void)
	{
		GPMZ(aI);
		for(U4 i = 0; i < 0xfff; i++)
		{
			aI[i] = aI[0xfff];
			aI[0xfff] += aN[i];
		}
		gp_memcpy(aX, aI, sizeof(aX));
	}
	U84 find( U4* pWORK, float4 *pVX, U4 ie, float4 siz, float4& MNr, char* pBUFF )
	{
		siz /= 16.0;
		I44 ize, i444(1, 16, 256, 0);
		float4	VXi = 0.0;
		 
		for(U4 i = 0; i < ie; i++)
		{
			nQ.w++;
			VXi = (pVX[i] - MNr) / siz;
			ize = VXi;
			ize.w = 0;
			aN[ize * i444]++;
		}
		ii();
		for(U4 i = 0; i < ie; i++)
		{
			nQ.w++;
			VXi = (pVX[i] - MNr) / siz;
			ize = VXi;
			ize.w = 0;
			ize.w = ize * i444;
			pWORK[aX[ize.w]] = i;
			aX[ize.w]++;
		}
		U4 a0, *pW;
		U44 xyz = 0;
		float now;
		for(		; xyz.x < 0xf; xyz.x++)
		for(xyz.y =0; xyz.y < 0xf; xyz.y++)
		for(xyz.z =0; xyz.z < 0xf; xyz.z++)
		{
			a0 = xyz * i444;
			pW = pWORK + ie;

			GPM_MEMCPY(pW, pWORK + aI[a0], aN[a0]);
			pW += aN[a0];
			GPM_MEMCPY(pW, pWORK + aI[a0 + 1], aN[a0 + 1]);
			pW += aN[a0 + 1];
			GPM_MEMCPY(pW, pWORK + aI[a0 + 16], aN[a0 + 16]);
			pW += aN[a0 + 16];
			GPM_MEMCPY(pW, pWORK + aI[a0 + 17], aN[a0 + 17]);
			pW += aN[a0 + 17];

			GPM_MEMCPY(pW, pWORK + aI[a0 + 256], aN[a0 + 256]);
			pW += aN[a0 + 256];
			GPM_MEMCPY(pW, pWORK + aI[a0 + 1 + 256], aN[a0 + 1 + 256]);
			pW += aN[a0 + 1 + 256];
			GPM_MEMCPY(pW, pWORK + aI[a0 + 16 + 256], aN[a0 + 16 + 256]);
			pW += aN[a0 + 16 + 256];
			GPM_MEMCPY(pW, pWORK + aI[a0 + 17 + 256], aN[a0 + 17 + 256]);
			pW += aN[a0 + 17 + 256];

			a0 = pW - pWORK - ie;
			pW = pWORK + ie;
			for(U4 i = 0, j; i < a0; i++)
			{
				VXi = pVX[pW[i]];
				for(j = i + 1; j < a0; j++)
				{
					nQ.w++;
					now = (VXi - pVX[pW[j]]).qlen_xyz();
					if(now == 0.0)
						continue; // ez ugyan az a pont nem számít
					if(now > MNr.w)
						continue;

					nQ.x = pW[i];
					nQ.y = pW[j];
					MNr.w = now;
					if(!pBUFF)
						continue;
					GPF_CON_FORMAT_Write("\r%lldq %d:%d %0.3f %s -> %s", nQ.w, nQ.x, nQ.y, sqrt(now), pVX[nQ.x].xyz().str3(pBUFF), pVX[nQ.y].xyz().str3(pBUFF + 0x80));
				}
			}
		}
		if( !pBUFF )
			return nQ;

		GPF_CON_FORMAT_Write("\r%lldq %d:%d %0.3f %s -> %s", nQ.w, nQ.x, nQ.y, sqrt(now), pVX[nQ.x].xyz().str3(pBUFF), pVX[nQ.y].xyz().str3(pBUFF + 0x80));
		return nQ;
	}
};
class GPC3DvxcBF
{
public:
	ID3D11Buffer	*pX11vx,
					*px11vsc;
	GPC_LAZY		vx;
	// UPLOAD VS
	float4 			MXR, MNr,
					MN0, MN1, 
					ATL, SIZ, 
					CNTR, CHR;
	// UPLOAD VS
	U84	nQ;
	I44 detail;

	U4	*pWORK,
		nWORK, mn0, 
		
		mn1, cntr,
		nVXrndr, stride, 
		
		off, n_reset;

	HRESULT hr, hr2;
	
	GPC3DvxcBF(void)
	{
		GPM_CLR;
	}
	~GPC3DvxcBF()
	{
		GPM_RELEASE(pX11vx);
		GPM_RELEASE(px11vsc);
		GPM_DELARRAY(pWORK);
	}
	GPC3DvxcBF* reset(void)
	{
		nVXrndr = vx.n_load = n_reset;
		return this;
	}
	GPC3Dvxc* add( GPC3Dvxc* pVXC, U8 n, U1 x = 0x4 )
	{
		U8 s = -1;
		vx.lazy_add( pVXC, n*sizeof(*pVXC), s, x );
		if( stride )
			return (GPC3Dvxc*)(vx.p_alloc + s);

		stride = sizeof(*pVXC);
		off = 0;
		return (GPC3Dvxc*)(vx.p_alloc + s);
	}
	U4 nVXld(void)
	{
		if(!this)
			return 0;
		return vx.n_load / stride;
	}
	bool upl_vx( ID3D11Device* p_dev, ID3D11DeviceContext* p_contx )
	{
		if(!this)
			return false;

		if( !vx.n_load )
		if( pX11vx )
			return true;

		nVXrndr = vx.n_load / stride;
		if( !nVXrndr)
		{
			GPM_RELEASE( pX11vx );
			return false;
		}
		
		// create the vertex buffer
		//=====-===--==---=------- ---  --   -
		// Resource		|Usage		|Default	|Dynamic	|Immutable	|Staging
		//--------------+-----------+-----------+-----------+-----------+--------
		// GPU-Read		|yes		|yes		|yes		|yes1		|
		// GPU-Write	|			|yes		|			|			|yes1
		// CPU-Read		|			|			|			|yes1		|
		// CPU-Write	|			|			|yes		|			|yes1
		//=====-===--==---=------- ---  --   -

		D3D11_BUFFER_DESC bDSC;
		D3D11_SUBRESOURCE_DATA iniDT;
		GPMZ(bDSC);
		GPMZ(iniDT);
		bDSC.Usage = D3D11_USAGE_DEFAULT;				// D3D11_USAGE_STAGING;// D3D11_USAGE_IMMUTABLE; //D3D11_USAGE_DEFAULT;//    D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
		bDSC.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// use as a vertex buffer
		

		
		bDSC.ByteWidth = nVXrndr * stride;       // size is the VERTEX struct * 3
		iniDT.pSysMem = vx.p_alloc;

		hr = p_dev->CreateBuffer(&bDSC, &iniDT, &pX11vx );       // create the buffer
		if(hr != S_OK)
			return false;

		// reset
		vx.n_load = 0;
		return true;
	}
	
	bool uplX4(ID3D11Device* p_dev, ID3D11DeviceContext* p_contx)
	{
		if(!this)
			return false;

		if(!vx.n_load)
		if(pX11vx)
			return true;

		nVXrndr = vx.n_load / stride * 4;
		if(!nVXrndr)
		{
			GPM_RELEASE(pX11vx);
			return false;
		}

		// create the vertex buffer
		//=====-===--==---=------- ---  --   -
		// Resource		|Usage		|Default	|Dynamic	|Immutable	|Staging
		//--------------+-----------+-----------+-----------+-----------+--------
		// GPU-Read		|yes		|yes		|yes		|yes1		|
		// GPU-Write	|			|yes		|			|			|yes1
		// CPU-Read		|			|			|			|yes1		|
		// CPU-Write	|			|			|yes		|			|yes1
		//=====-===--==---=------- ---  --   -

		D3D11_BUFFER_DESC bDSC;
		GPMZ(bDSC);
		/*D3D11_BUFFER_DESC bd;
		GPMZ(bd);
		bd.Usage = D3D11_USAGE_DYNAMIC;					// D3D11_USAGE_STAGING;// D3D11_USAGE_IMMUTABLE; //D3D11_USAGE_DEFAULT;//    D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
		bd.ByteWidth = n_ix_alloc * sizeof(*a_idx);			// size is the VERTEX struct * 3
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;			// use as a vertex buffer
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		// allow CPU to write in buffer
		*/

		bDSC.Usage = D3D11_USAGE_DYNAMIC;				// D3D11_USAGE_STAGING;// D3D11_USAGE_IMMUTABLE; //D3D11_USAGE_DEFAULT;//    D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
		bDSC.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// use as a vertex buffer
		bDSC.ByteWidth = nVXrndr * stride;       // size is the VERTEX struct * 3
		bDSC.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		// allow CPU to write in buffer


		D3D11_SUBRESOURCE_DATA iniDT;
		GPMZ(iniDT);
		iniDT.pSysMem = vx.p_alloc;

		hr = p_dev->CreateBuffer(&bDSC, NULL, &pX11vx); //&iniDT, &pX11vx);       // create the buffer
		if(hr != S_OK)
			return false;

		D3D11_MAPPED_SUBRESOURCE ms;
		hr = p_contx->Map(pX11vx, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
		
		GPC3Dvxc	*pVXC = vx.pVXC,
					*pUPL = (GPC3Dvxc*)ms.pData;
		for(U4 i = 0, ie = vx.n_load/stride; i < ie; i++)
		{
			GPF_MEM_set( pUPL, 4, pVXC, sizeof(*pVXC) );
			pUPL[0].c4.B = 0;
			pUPL[1].c4.B = 1;
			pUPL[2].c4.B = 2;
			pUPL[3].c4.B = 3;
			pVXC++;
			pUPL += 4;
		}///--------------
		
		p_contx->Unmap(pX11vx, NULL);


		// reset
		vx.n_load = 0;
		return true;
	}
	ID3D11Buffer* rndr(ID3D11Device* p_dev, ID3D11DeviceContext* p_contx, ID3D11Buffer* pVScb, GPC3DIX* p_ix )
	{
		if(p_ix)
			p_contx->IASetPrimitiveTopology( p_ix->top ); // D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

		if( !upl_vx(p_dev, p_contx) )
			return pVScb;

		U4 nIX = (nVXrndr*p_ix->n_ix4vx) / 4;
		p_ix->set(p_dev, p_contx, nIX);
		p_contx->IASetVertexBuffers( 0, 1, &pX11vx, &stride, &off );

		if(pVScb)
			p_contx->VSSetConstantBuffers(3, 1, &pVScb);
		
		px11vsc = MXR.upl_n(p_dev, p_contx, px11vsc, 8 );
		if(px11vsc)
			p_contx->VSSetConstantBuffers(5, 1, &px11vsc);

		for(U4 i = 0; i < nIX; i += p_ix->n_ix_alloc)
			p_contx->DrawIndexed( min(nIX - i, p_ix->n_ix_alloc), 0, (i / p_ix->n_ix4vx) * 4);
		
		return pVScb;
	}
	GPC3DvxcBF* set_cntr(float4 fnd)
	{
		if(!this)
			return this;

		float4	*pVX = &vx.pVXC->f4;
		CNTR = *pVX;
		float mn = (CNTR - fnd).qlen_xyz();
		for(U4 i = 1, ie = nVXld(); i < ie; i++)
		{
			if( mn < (pVX[i] - fnd).qlen_xyz() )
				continue;
			mn = ((CNTR = pVX[i]) - fnd).qlen_xyz();
			cntr = i;
		}
		CHR.x = '+';
		CHR.y = '*';
		return this;
	}
	GPC3DvxcBF* set_cntr( U4 idx = 0 )
	{
		if(!this)
			return this;
		cntr = idx;
		CNTR = vx.pVXC[cntr].f4;
		CHR.x = '+';
		CHR.y = '*';
		return this;
	}
	GPC3DvxcBF* vxMN(void)
	{
		if(!this)
			return NULL;
		float4	*pVX = &vx.pVXC->f4, // (float4*)vx.p_alloc,
				VXi = 0.0;
		MNr = MXR = *pVX;
		MNr.w = 1.0 / VXi.x;
		MXR = MNr.w * -1.0;
		float now;
		GPF_CON_FORMAT_Write("\r\n vxMN\r\n");
		char aBUFF[0x100];
		for(U4 i = 0, ie = nVXld(); i < ie; i++)
		{
			VXi = pVX[i];
			MXR.mx_xyz(VXi);
			MNr.mn_xyz(VXi);
		}
		SIZ = MXR - MNr;
		MXR.w = sqrt(SIZ.xyz().qlen())/2.0;
		GPF_CON_FORMAT_Write("\r\n BBOX: %s \r\n\t%s -> %s ", SIZ.xyz().str4(aBUFF), MNr.xyz().str4(aBUFF), MXR.str4(aBUFF + 0x80));
		
		
	//	float4 q = SIZ;
	//	q /= 8.0;
		I44 ize, i444( 1, 8, 64, 0 );
		U4 ie = nVXld(), a0, *pW;
		if(nWORK < ie * 2)
		{
			GPM_DELARRAY(pWORK);
			nWORK = ie * 2;
			pWORK = new U4[nWORK];
		}
		NEAR4096* pNEAR = new NEAR4096;
		nQ = pNEAR->find( pWORK, pVX, ie, SIZ, MNr, aBUFF );
		MN0 = pVX[mn0 = nQ.x];
		MN1 = pVX[mn1 = nQ.y];
		GPM_DEL(pNEAR);

		ATL = MN1 - MN0;
		MNr.w = sqrt(ATL.qlen_xyz());

		nQ.z = ie * (ie / 2);

		GPF_CON_FORMAT_Write("\r\n ATL:%s \r\n%s -> %s\r\n nQ %lld/%lld", ATL.str4(aBUFF), MN0.str4(aBUFF + 0x40), MN1.str4(aBUFF + 0x80), nQ.w, nQ.z );
		float mn = ATL.abs().mn_xyz();
		nQ.z = 1.0 / mn;
		detail = SIZ / mn;
		//POS.w = 'N';

		GPF_CON_FORMAT_Write("\r\n nQz: %lld", nQ.z);
		return this;
	}
	
};



class GPC3DGR
{
public:
	ID3D11Buffer	*apX11vx[6], *apX11cb[6];	
	U142			*ap_u142[6];			// 8*13		104
	U4				anVX[6], 
					anIX[6],				
					an_alloc[6],					// 18*4		72		176	
					stride, off, xfnd,				// 4*3		12		188
					aqu, nAQU;						// 4*2		8		196
	U44				anCELL[6],
					anCELLixTRI[6],
					anCELLixLIN[6];
	U1				fac, bRST, rndfc, b_cpy;		// 4				198		
	HRESULT			hr;								
	GPC3DGR( void )
	{
		clear( -1 );
	}
	void clear( U4 x )
	{
		GPM_CLR;
		xfnd = x;
	}
	GPC3DGR* reset( U4 x )
	{
		if( !b_cpy )
		for( U1 i = 0; i < GPM_N(apX11vx); i++ )
		{
			GPM_DELARRAY( ap_u142[i] );
			GPM_RELEASE( apX11vx[i] );
			GPM_RELEASE( apX11cb[i] );
		}
		
		clear( x );
		return this;
	}
	void updt(void)
	{
		if(!this)
			return;
		for(U1 i = 0; i < GPM_N(apX11vx); i++)
		{
			GPM_RELEASE(apX11vx[i]);
		}
	}
	bool upl_vx(  ID3D11Device* p_dev, ID3D11DeviceContext* p_contx, U1 s )
	{
		if( !this )
			return false;
		if( apX11vx[s] )
			return true;

		if(!anVX[s])
		{
			GPM_RELEASE(apX11vx[s]);
			return false;
		}
		U1 f = fac>>s;
		if( !(f&1) )
		{
			GPM_RELEASE( apX11vx[s] );
			return false;
		}

		if( !ap_u142[s] )
		{
			GPM_RELEASE( apX11vx[s] );
			return false;
		}
		// create the vertex buffer
		//=====-===--==---=------- ---  --   -
		// Resource		|Usage		|Default	|Dynamic	|Immutable	|Staging
		//--------------+-----------+-----------+-----------+-----------+--------
		// GPU-Read		|yes		|yes		|yes		|yes1		|
		// GPU-Write	|			|yes		|			|			|yes1
		// CPU-Read		|			|			|			|yes1		|
		// CPU-Write	|			|			|yes		|			|yes1
		//=====-===--==---=------- ---  --   -
		
		D3D11_BUFFER_DESC bDSC;
		D3D11_SUBRESOURCE_DATA iniDT;
		GPMZ( bDSC );
		GPMZ( iniDT );
		bDSC.Usage = D3D11_USAGE_DEFAULT;				// D3D11_USAGE_STAGING;// D3D11_USAGE_IMMUTABLE; //D3D11_USAGE_DEFAULT;//    D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
		bDSC.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// use as a vertex buffer
		
		if( anVX[s] )
		{
			bDSC.ByteWidth = anVX[s] * sizeof(**ap_u142);       // size is the VERTEX struct * 3
			iniDT.pSysMem = ap_u142[s];
			
			hr = p_dev->CreateBuffer(&bDSC, &iniDT, &apX11vx[s] );       // create the buffer
			if( hr != S_OK )
				return false;
		}  
		anIX[s] = (anVX[s]/4)*6; 
		anCELLixTRI[s] = (anCELL[s]/4) * 6;
		anCELLixLIN[s] = (anCELL[s]/4) * 8;
		if( apX11cb[s] )
			return true;

		U14 aU14[2];
		aU14[1] = s;
		apX11cb[s]	= aU14[0].fnd2gr_xZYs5( xfnd ).upl_n( p_dev, p_contx, apX11cb[s], 2 );
					//= aU14[0].fnd2gr_xYZs5(xfnd).upl_n(p_dev, p_contx, apX11cb[s], 2);

		return true;
	}
	ID3D11Buffer* rndr( ID3D11Device* p_dev, ID3D11DeviceContext* p_contx, ID3D11Buffer* pVScb, GPC3DIX* p_ix, U44& u44, bool b_occlu )
	{
		if(p_ix )
			p_contx->IASetPrimitiveTopology(p_ix->top); // D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

		U1 f = b_occlu ? fac : rndfc;//fac;// rndfc;//
		for( u44.y = 0; u44.y < 6; u44.y++, f >>= 1 )
		{
			// FACE loop
			if( !(f&1) )
				continue;
			
			if( !upl_vx( p_dev, p_contx, u44.y ) )
				continue;

			// pVS[0]:  u44( x:gr_sid y:sid_face z:oclu_ix )
			pVScb = u44.upl_n( p_dev, p_contx, pVScb, 1 );
			if( pVScb )
				p_contx->VSSetConstantBuffers( 3, 1, &pVScb );

			anIX[u44.y] = (anVX[u44.y] * p_ix->n_ix4vx)/4;
			p_ix->set( p_dev, p_contx,  anIX[u44.y] );
			
			if( apX11cb[u44.y] )
				p_contx->VSSetConstantBuffers( 4, 1, &apX11cb[u44.y] );
			p_contx->IASetVertexBuffers( 0, 1, &apX11vx[u44.y], &stride, &off );
			//anCELL[u44.y].x
			//if(anCELL[u44.y].w)
			//{
			//	
			//	for(U4 i = 0; i < anCELLix[u44.y].x; i += p_ix->n_ix)
			//		p_contx->DrawIndexed( min(anCELLix[u44.y].x - i, p_ix->n_ix), 0, (i / 6) * 4);
			//	
			//	for(U4 i = 0; i < anCELLix[u44.y].y; i += p_ix->n_ix)
			//		p_contx->DrawIndexed(min(anCELL[u44.y].y - i, p_ix->n_ix), 0, ((i+anCELL[u44.y].x) / 6) * 4);
			//
			//	for(U4 i = 0; i < anCELLix[u44.y].z; i += p_ix->n_ix)
			//		p_contx->DrawIndexed(min(anCELL[u44.y].z - i, p_ix->n_ix), 0, ((i + anCELL[u44.y].x + anCELL[u44.y].y) / 6) * 4);
			//	continue;
			//} 
			for( U4 i = 0; i < anIX[u44.y]; i += p_ix->n_ix_alloc )
				p_contx->DrawIndexed( min( anIX[u44.y]-i, p_ix->n_ix_alloc) , 0, (i/p_ix->n_ix4vx)*4 );
						
		}
		return pVScb;
	}

	ID3D11Buffer* rndr_PnL(ID3D11Device* p_dev, ID3D11DeviceContext* p_contx, ID3D11Buffer* pVScb, GPC3DIX* p_ix, U44& u44, U1 PnL, bool b_occlu)
	{
		if(p_ix)
			p_contx->IASetPrimitiveTopology(p_ix->top);

		U1 f = b_occlu ? fac : rndfc;//fac;// rndfc;//
		for(u44.y = 0; u44.y < 6; u44.y++, f >>= 1)
		{
			// FACE loop
			if(!(f & 1))
				continue;

			if(!upl_vx(p_dev, p_contx, u44.y))
				continue;

			// pVS[0]:  u44( x:gr_sid y:sid_face z:oclu_ix )
			pVScb = u44.upl_n(p_dev, p_contx, pVScb, 1);
			if(pVScb)
				p_contx->VSSetConstantBuffers(3, 1, &pVScb);

			anIX[u44.y] = (anVX[u44.y] * p_ix->n_ix4vx) / 4;
			p_ix->set(p_dev, p_contx, anIX[u44.y]);
			if(apX11cb[u44.y])
				p_contx->VSSetConstantBuffers(4, 1, &apX11cb[u44.y]);
			p_contx->IASetVertexBuffers(0, 1, &apX11vx[u44.y], &stride, &off);
			//anCELL[u44.y].x
			if(PnL)
			{
				if(anCELL[u44.y].w)
				if(p_ix->top != D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
				{
					for(U4 i = 0; i < anCELLixLIN[u44.y].x; i += p_ix->n_ix_alloc)
						p_contx->DrawIndexed(min(anCELLixLIN[u44.y].x - i, p_ix->n_ix_alloc), 0, (i / 8) * 4);

					if(PnL & 0x40)
						for(U4 i = 0; i < anCELLixLIN[u44.y].y; i += p_ix->n_ix_alloc)
							p_contx->DrawIndexed(min(anCELLixLIN[u44.y].y - i, p_ix->n_ix_alloc), 0, ((i + anCELLixLIN[u44.y].x) / 8) * 4);

					if(PnL & 0x80)
						for(U4 i = 0; i < anCELLixLIN[u44.y].z; i += p_ix->n_ix_alloc)
							p_contx->DrawIndexed(min(anCELLixLIN[u44.y].z - i, p_ix->n_ix_alloc), 0, ((i + anCELLixLIN[u44.y].x + anCELLixLIN[u44.y].y) / 8) * 4);
					continue;
				} else {
					for(U4 i = 0; i < anCELLixTRI[u44.y].x; i += p_ix->n_ix_alloc)
						p_contx->DrawIndexed(min(anCELLixTRI[u44.y].x - i, p_ix->n_ix_alloc), 0, (i / 6) * 4);

					if( PnL&0x40 )
					for(U4 i = 0; i < anCELLixTRI[u44.y].y; i += p_ix->n_ix_alloc)
						p_contx->DrawIndexed(min(anCELLixTRI[u44.y].y - i, p_ix->n_ix_alloc), 0, ((i + anCELLixTRI[u44.y].x) / 6) * 4);

					if( PnL&0x80 )
					for(U4 i = 0; i < anCELLixTRI[u44.y].z; i += p_ix->n_ix_alloc)
						p_contx->DrawIndexed(min(anCELLixTRI[u44.y].z - i, p_ix->n_ix_alloc), 0, ((i + anCELLixTRI[u44.y].x + anCELLixTRI[u44.y].y) / 6) * 4);
					continue;
				}

				continue;
			}
			

			
			for(U4 i = 0; i < anIX[u44.y]; i += p_ix->n_ix_alloc)
				p_contx->DrawIndexed(min(anIX[u44.y] - i, p_ix->n_ix_alloc), 0, (i / p_ix->n_ix4vx) * 4);

		}
		return pVScb;
	}
};

typedef enum gpeLWO:U4
{
	LWO_ID_FORM = MAKE_ID('F', 'O', 'R', 'M'),
	LWO_ID_LWO2 = MAKE_ID('L', 'W', 'O', '2'),

	/**  PRIMARY CHUNK ID  **/
	LWO_ID_LAYR = MAKE_ID('L', 'A', 'Y', 'R'),
	LWO_ID_PNTS = MAKE_ID('P', 'N', 'T', 'S'),
	LWO_ID_VMAP = MAKE_ID('V', 'M', 'A', 'P'),

	LWO_ID_VMAD = MAKE_ID('V', 'M', 'A', 'D'),

	LWO_ID_POLS = MAKE_ID('P', 'O', 'L', 'S'),
	LWO_ID_TAGS = MAKE_ID('T', 'A', 'G', 'S'),
	LWO_ID_PTAG = MAKE_ID('P', 'T', 'A', 'G'),
	LWO_ID_ENVL = MAKE_ID('E', 'N', 'V', 'L'),
	LWO_ID_CLIP = MAKE_ID('C', 'L', 'I', 'P'),
	LWO_ID_SURF = MAKE_ID('S', 'U', 'R', 'F'),
	LWO_ID_BBOX = MAKE_ID('B', 'B', 'O', 'X'),
	LWO_ID_DESC = MAKE_ID('D', 'E', 'S', 'C'),
	LWO_ID_TEXT = MAKE_ID('T', 'E', 'X', 'T'),
	LWO_ID_ICON = MAKE_ID('I', 'C', 'O', 'N'),

	/**  POLS TYPE  **/
	LWO_ID_FACE = MAKE_ID('F', 'A', 'C', 'E'),
	LWO_ID_CRVS = MAKE_ID('C', 'U', 'R', 'V'),
	LWO_ID_PCHS = MAKE_ID('P', 'T', 'C', 'H'),
	LWO_ID_MBAL = MAKE_ID('M', 'B', 'A', 'L'),
	LWO_ID_BONE = MAKE_ID('B', 'O', 'N', 'E'),

	/**  PTAG TYPE  **/
	//	LWO_ID_SURF	= MAKE_ID('S','U','R','F'),
	LWO_ID_BNID = MAKE_ID('B', 'N', 'I', 'D'),
	LWO_ID_SGMP = MAKE_ID('S', 'G', 'M', 'P'),
	LWO_ID_PART = MAKE_ID('P', 'A', 'R', 'T'),

	/**  IMAGE SUB-CHUNK ID  */
	LWO_ID_STIL = MAKE_ID('S', 'T', 'I', 'L'),
	LWO_ID_ISEQ = MAKE_ID('I', 'S', 'E', 'Q'),
	LWO_ID_ANIM = MAKE_ID('A', 'N', 'I', 'M'),
	LWO_ID_XREF = MAKE_ID('X', 'R', 'E', 'F'),
	LWO_ID_STCC = MAKE_ID('S', 'T', 'C', 'C'),
	LWO_ID_CONT = MAKE_ID('C', 'O', 'N', 'T'),
	LWO_ID_BRIT = MAKE_ID('B', 'R', 'I', 'T'),
	LWO_ID_SATR = MAKE_ID('S', 'A', 'T', 'R'),
	LWO_ID_HUE = MAKE_ID('H', 'U', 'E', ' '),
	LWO_ID_GAMM = MAKE_ID('G', 'A', 'M', 'M'),
	LWO_ID_NEGA = MAKE_ID('N', 'E', 'G', 'A'),
	LWO_ID_CROP = MAKE_ID('C', 'R', 'O', 'P'),
	LWO_ID_ALPH = MAKE_ID('A', 'L', 'P', 'H'),
	LWO_ID_COMP = MAKE_ID('C', 'O', 'M', 'P'),
	LWO_ID_IFLT = MAKE_ID('I', 'F', 'L', 'T'),
	LWO_ID_PFLT = MAKE_ID('P', 'F', 'L', 'T'),

	/**  ENVELOPE SUB-CHUNK  **/
	LWO_ID_PRE = MAKE_ID('P', 'R', 'E', ' '),
	LWO_ID_POST = MAKE_ID('P', 'O', 'S', 'T'),
	LWO_ID_KEY = MAKE_ID('K', 'E', 'Y', ' '),
	LWO_ID_SPAN = MAKE_ID('S', 'P', 'A', 'N'),
	LWO_ID_CHAN = MAKE_ID('C', 'H', 'A', 'N'),

	/**  SURFACE SUB-CHUNK ID  */
	LWO_ID_COLR = MAKE_ID('C', 'O', 'L', 'R'),
	LWO_ID_DIFF = MAKE_ID('D', 'I', 'F', 'F'),
	LWO_ID_LUMI = MAKE_ID('L', 'U', 'M', 'I'),
	LWO_ID_SPEC = MAKE_ID('S', 'P', 'E', 'C'),
	LWO_ID_REFL = MAKE_ID('R', 'E', 'F', 'L'),
	LWO_ID_TRAN = MAKE_ID('T', 'R', 'A', 'N'),
	LWO_ID_TRNL = MAKE_ID('T', 'R', 'N', 'L'),
	LWO_ID_GLOS = MAKE_ID('G', 'L', 'O', 'S'),
	LWO_ID_SHRP = MAKE_ID('S', 'H', 'R', 'P'),
	LWO_ID_BUMP = MAKE_ID('B', 'U', 'M', 'P'),
	LWO_ID_SIDE = MAKE_ID('S', 'I', 'D', 'E'),
	LWO_ID_SMAN = MAKE_ID('S', 'M', 'A', 'N'),
	LWO_ID_RFOP = MAKE_ID('R', 'F', 'O', 'P'),
	LWO_ID_RIMG = MAKE_ID('R', 'I', 'M', 'G'),
	LWO_ID_RSAN = MAKE_ID('R', 'S', 'A', 'N'),
	LWO_ID_RIND = MAKE_ID('R', 'I', 'N', 'D'),
	LWO_ID_CLRH = MAKE_ID('C', 'L', 'R', 'H'),
	LWO_ID_TROP = MAKE_ID('T', 'R', 'O', 'P'),
	LWO_ID_TIMG = MAKE_ID('T', 'I', 'M', 'G'),
	LWO_ID_CLRF = MAKE_ID('C', 'L', 'R', 'F'),
	LWO_ID_ADTR = MAKE_ID('A', 'D', 'T', 'R'),
	LWO_ID_GLOW = MAKE_ID('G', 'L', 'O', 'W'),
	LWO_ID_LINE = MAKE_ID('L', 'I', 'N', 'E'),
	//LWO_ID_ALPH	= MAKE_ID('A','L','P','H'),
	LWO_ID_AVAL = MAKE_ID('A', 'V', 'A', 'L'),
	LWO_ID_GVAL = MAKE_ID('G', 'V', 'A', 'L'),
	LWO_ID_BLOK = MAKE_ID('B', 'L', 'O', 'K'),
	LWO_ID_LCOL = MAKE_ID('L', 'C', 'O', 'L'),
	LWO_ID_LSIZ = MAKE_ID('L', 'S', 'I', 'Z'),
	LWO_ID_CMNT = MAKE_ID('C', 'M', 'N', 'T'),

	/**  TEXTURE LAYER  **/
	//LWO_ID_CHAN	= MAKE_ID('C','H','A','N'),
	LWO_ID_TYPE = MAKE_ID('T', 'Y', 'P', 'E'),
	LWO_ID_NAME = MAKE_ID('N', 'A', 'M', 'E'),
	LWO_ID_ENAB = MAKE_ID('E', 'N', 'A', 'B'),
	LWO_ID_OPAC = MAKE_ID('O', 'P', 'A', 'C'),
	LWO_ID_FLAG = MAKE_ID('F', 'L', 'A', 'G'),
	LWO_ID_PROJ = MAKE_ID('P', 'R', 'O', 'J'),
	LWO_ID_STCK = MAKE_ID('S', 'T', 'C', 'K'),
	LWO_ID_TAMP = MAKE_ID('T', 'A', 'M', 'P'),

	/**  TEXTURE MAPPING  **/
	LWO_ID_TMAP = MAKE_ID('T', 'M', 'A', 'P'),
	LWO_ID_AXIS = MAKE_ID('A', 'X', 'I', 'S'),
	LWO_ID_CNTR = MAKE_ID('C', 'N', 'T', 'R'),
	LWO_ID_SIZE = MAKE_ID('S', 'I', 'Z', 'E'),
	LWO_ID_ROTA = MAKE_ID('R', 'O', 'T', 'A'),
	LWO_ID_OREF = MAKE_ID('O', 'R', 'E', 'F'),
	LWO_ID_FALL = MAKE_ID('F', 'A', 'L', 'L'),
	LWO_ID_CSYS = MAKE_ID('C', 'S', 'Y', 'S'),

	/**  IMAGE MAP  **/
	LWO_ID_IMAP = MAKE_ID('I', 'M', 'A', 'P'),
	LWO_ID_IMAG = MAKE_ID('I', 'M', 'A', 'G'),
	LWO_ID_WRAP = MAKE_ID('W', 'R', 'A', 'P'),
	LWO_ID_WRPW = MAKE_ID('W', 'R', 'P', 'W'),
	LWO_ID_WRPH = MAKE_ID('W', 'R', 'P', 'H'),
	//	LWO_ID_VMAP	= MAKE_ID('V','M','A','P'),
	LWO_ID_AAST = MAKE_ID('A', 'A', 'S', 'T'),
	LWO_ID_PIXB = MAKE_ID('P', 'I', 'X', 'B'),

	/**  PROCUDUAL TEXTURE  **/
	LWO_ID_PROC = MAKE_ID('P', 'R', 'O', 'C'),
	//	LWO_ID_COLR	= MAKE_ID('C','O','L','R'),
	LWO_ID_VALU = MAKE_ID('V', 'A', 'L', 'U'),
	LWO_ID_FUNC = MAKE_ID('F', 'U', 'N', 'C'),
	LWO_ID_FTPS = MAKE_ID('F', 'T', 'P', 'S'),
	LWO_ID_ITPS = MAKE_ID('I', 'T', 'P', 'S'),
	LWO_ID_ETPS = MAKE_ID('E', 'T', 'P', 'S'),

	/**  GRADIENT **/
	LWO_ID_GRAD = MAKE_ID('G', 'R', 'A', 'D'),
	LWO_ID_GRST = MAKE_ID('G', 'R', 'S', 'T'),
	LWO_ID_GREN = MAKE_ID('G', 'R', 'E', 'N'),

	/**  SHADER PLUGIN  */
	LWO_ID_SHDR = MAKE_ID('S', 'H', 'D', 'R'),
	LWO_ID_DATA = MAKE_ID('D', 'A', 'T', 'A'),


	/**  VMAP TYPE  **/
	LWO_ID_MNVW = MAKE_ID('M', 'N', 'V', 'W'),
	LWO_ID_MORF = MAKE_ID('M', 'O', 'R', 'F'),
	LWO_ID_TXUV = MAKE_ID('T', 'X', 'U', 'V'),
	LWO_ID_WGHT = MAKE_ID('W', 'G', 'H', 'T'),
	/**  saját id-k **/
	LWO_ID_LIGHT = MAKE_ID('l', 'i', 'g', 'h'),
	LWO_ID_COLOR = MAKE_ID('c', 'o', 'l', 'o'),
};

//class GPC_LWO_TAG
//{
//public:
//	char s_name[0x20];
//	U4 a_pic[0x10];
//	U2 n_p;
//	GPC_LWO_TAG(void)
//	{
//		GPM_CLR;
//	}
//};
class GPC_LWO_FC
{
public:
	U4 aIX0[0x10], aIX1[0x10];
	U2 n_p, part, surf, colr, n_part, n_surf;

	char* load( char* pD )
	{
		n_p = GPF_SWAP_U2((char*)pD) & 0x03ff;
		pD += 2;
		for( U2 i = 0; i < n_p; i++ )
		{
			if( *pD == -1 )
			{
				aIX0[i] = GPF_SWAP_U4(pD);
				pD += 4;
				continue;
			}
			aIX0[i] = GPF_SWAP_U2(pD);
			pD += 2;
		}
		GPM_MEMCPY( aIX1, aIX0, n_p );
		return pD;
	}
};
class GPC_LWO_FC_BF
{
public:
	GPC_LWO_FC* pFC;
	U4 nFC, nFCld;
	U2	aFChisti[0x100];
	GPC_LWO_FC_BF(void)
	{
		GPM_CLR;
	}
	GPC_LWO_FC* get(U4 i)
	{
		if( i >= nFCld )
			return NULL;

		return pFC + i;
	}
	GPC_LWO_FC* add(void)
	{
		if(pFC)
			nFCld++;

		if( nFCld >= nFC )
		{
			GPC_LWO_FC* pKILL = pFC;
			nFC += 0x100;
			pFC = new GPC_LWO_FC[nFC];
			GPM_MEMCPY( pFC, pKILL, nFCld );
			GPM_DELARRAY(pKILL);
		}

		GPMZ(pFC[nFCld]);
		return pFC + nFCld;
	}
};
class GPC3DLWO
{
public:
	GPC_LAZY		*pSRC,
					*pWORK;
	GPC_LAZY_DICT	*pTAG;

	GPC3DvxcBF		**ppVXC,
					*pPNTS,
					*pUV;
	GPC_LWO_FC_BF	**ppFC;
	U4	nVXC, nVXCld,
		*pLAY, 
		nLAY, nLAYld;
	

	float dim;

	GPC3DLWO(char* p_p, char* p_pf, const char* p_file, float d )
	{
		GPM_CLR;
		lwo_load(p_p, p_pf, p_file, d);
		
	}
	~GPC3DLWO()
	{
		clear();
		GPM_DEL(pSRC);
	}
	GPC3DLWO* clear(void)
	{
		if(ppVXC)
		for(U4 i = 0; i < nVXCld; i++)
		{
			GPM_DEL(ppVXC[i]);
		}
		if(pLAY)
		for(U4 i = 0; i < nLAYld; i++)
		{
			GPM_DEL(ppFC[i]);
		}
		GPM_DEL(pTAG);
		GPM_DEL(pWORK);
		GPM_DELARRAY(pLAY);
		GPM_DELARRAY(ppFC);
		GPM_DELARRAY(ppVXC);
		nLAY = nVXC = nLAYld = nVXCld = 0;
		
		return this;
	}
	GPC3DLWO* lwo_load( char* p_p, char* p_pf, const char* p_file, float d )
	{
		U8 s = 0;
		GPM_STRCPY(p_pf, p_file);
		pSRC = pSRC->lazy_read(p_p, s, 0);
		dim = d;
		clear();
		return this;
	}
	
	GPC3DLWO* vxMN( void )
	{
		for( U4 l = 0; l < nLAYld; l++ )
		{
			ppVXC[l]->vxMN();
		}
		return this;
	}
	GPC3DvxcBF* vxc( U4 i )
	{
		if( !this )
			return NULL;

		if( nVXCld <= i ? !!ppVXC : !pSRC )
			return NULL;


		if(ppVXC ? ppVXC[i] : false)
			return ppVXC[i];
		if(!pSRC)
			return NULL;

		GPC_LWO_FC* pFC;
		U1	*p_u1 = pSRC->p_alloc,
			*p_u1e = p_u1 + pSRC->n_load;
		char *pD;

		U4	chunk, nSTEP, iSTR, nD, nFCld, nI,
			*p_u4 = (U4*)p_u1;
		if(*p_u4 != LWO_ID_FORM)
		{
			GPM_DEL(pSRC);
			return NULL;
		}
		p_u4++;
		nSTEP = GPF_SWAP_U4((char*)p_u4);
		p_u4++;

		if(*p_u4 != LWO_ID_LWO2)
		{
			GPM_DEL(pSRC);
			return NULL;
		}
		p_u1 = (U1*)p_u4;
		p_u1e = p_u1 + nSTEP;
		p_u4++;
		p_u1 = (U1*)p_u4;

		
		
		while(p_u1 < p_u1e)
		{
			p_u4 = (U4*)p_u1;
			chunk = *p_u4;
			p_u4++;
			nSTEP = GPF_SWAP_U4((char*)p_u4);
			p_u4++;
			p_u1 = (U1*)p_u4 + nSTEP;
			GPF_CON_FORMAT_Write("\n%.4s n_byte:%d", (char*)&chunk, nSTEP);

			pD = (char*)p_u4;
			switch(chunk)
			{
				case LWO_ID_TAGS:
					while( pD < (char*)p_u1 )
					{
						nD = GPD_STRLEN(pD);
						iSTR = (pTAG = pTAG->dict_add(pD, nD))->i_str();
						GPF_CON_FORMAT_Write("\n\t%d. %s", pTAG->nIX()-1, pTAG->sSTRix( pTAG->nIX() - 1 ) );
						pD = GPF_LW3D_Pad( pD + nD + 1 );
					}
					break;
				case LWO_ID_LAYR:
					{
						if(pLAY)
						{



							nLAYld++;
						}
							
						if(nLAYld >= nLAY)
						{
							nLAY += 0x10;
							U4* p_kill = pLAY;
							GPC_LWO_FC_BF **ppFCkill = ppFC;

							pLAY = new U4[nLAY];
							GPM_MEMCPY(pLAY, p_kill, nLAYld);
							GPMZn(pLAY + nLAYld, nLAY - nLAYld);

							ppFC = new GPC_LWO_FC_BF*[nLAY];
							GPM_MEMCPY(ppFC, ppFCkill, nLAYld);
							GPMZn(ppFC + nLAYld, nLAY - nLAYld);
						}
						pLAY[nLAYld] = nVXCld;
						ppFC[nLAYld] = new GPC_LWO_FC_BF;

						pPNTS = pUV = NULL;
					}
					break;
				case LWO_ID_PNTS:
					{
						
						if( nVXCld >= nVXC )
						{
							nVXC += 0x8;
							GPC3DvxcBF** pp_kill = ppVXC;
							ppVXC = new GPC3DvxcBF*[nVXC];
							GPM_MEMCPY( ppVXC, pp_kill, nVXCld );
							GPMZn(ppVXC + nVXCld, nVXC - nVXCld );
						}

						ppVXC[nVXCld] = pPNTS = new GPC3DvxcBF;
						GPC3Dvxc* pP = pPNTS->add( NULL, nSTEP/(3 * 4), 2 );
						GPF_CON_FORMAT_Write( "\n n_p: %d \n", pPNTS->nVXld() );
						pP->LWOn_xyz( pD, pPNTS->nVXld(), dim );
						nVXCld++;
					}
					break;
				case LWO_ID_POLS:
					{
						p_u4 = (U4*)pD;
						switch(*p_u4)
						{
							case LWO_ID_FACE:
								{
									pD += 4;
									while( pD < (char*)p_u1 )
									{
										pD = (pFC = ppFC[nLAYld]->add())->load( pD );
										ppFC[nLAYld]->aFChisti[pFC->n_p]++;
									}
								}
								GPF_CON_FORMAT_Write("\n n_FACE: %d\n n_p:", ppFC[nLAYld]->nFCld );
								for(U4 i = 0; i < GPM_N(ppFC[nLAYld]->aFChisti); i++)
								{
									if( !ppFC[nLAYld]->aFChisti[i] )
										continue;
									GPF_CON_FORMAT_Write("n:%d %d", i, ppFC[nLAYld]->aFChisti[i] );
								}
								break;
							case LWO_ID_BONE:
								GPF_CON_FORMAT_Write( "\n n_BONE: %d", 0 );
								break;
						}
					}
					break;
				case LWO_ID_VMAP:
					{
						char sCHUNK[5];
						((U1*)gp_memcpy(sCHUNK, p_u4, 4))[4] = 0;

						chunk = *p_u4;
						pD += 4;
						U2 dm = GPF_SWAP_U2(pD);
						pD += 2;
						char	s_name[0x100],
								n_n = sprintf(s_name, "%s", pD);
						pD = GPF_LW3D_Pad(pD + n_n + 1);
						GPF_CON_FORMAT_Write("\n %s: d%d %s", sCHUNK, dm, s_name);
						switch( chunk )
						{
							case LWO_ID_MNVW:
								break;
							case LWO_ID_WGHT:
								break;
							case LWO_ID_TXUV:
								//GPF_CON_FORMAT_Write("\n TXUV: d%d %s", dm, s_name);

								if(nVXCld >= nVXC)
								{
									nVXC += 0x8;
									GPC3DvxcBF** pp_kill = ppVXC;
									ppVXC = new GPC3DvxcBF*[nVXC];
									GPM_MEMCPY(ppVXC, pp_kill, nVXCld);
									GPMZn(ppVXC + nVXCld, nVXC - nVXCld);
								}
								ppVXC[nVXCld] = pUV = new GPC3DvxcBF;

								GPC3Dvxc* pU = pUV->add( NULL, pPNTS->nVXld(), 2 );
								GPMZn(pU, pPNTS->nVXld());
								while( pD < (char*)p_u1 )
								{
									pD = pU->LWOi_xy( pD );
								}
								nVXCld++;
								break;
						}
					}
					break;
				case LWO_ID_VMAD:
					{
						char sCHUNK[5];
						((U1*)gp_memcpy(sCHUNK, p_u4, 4))[4] = 0;

						chunk = *p_u4;
						pD += 4;
						U2 dm = GPF_SWAP_U2(pD); pD += 2;
						char	s_name[0x100],
							n_n = sprintf(s_name, "%s", pD);
						pD = GPF_LW3D_Pad(pD + n_n + 1);
						GPF_CON_FORMAT_Write("\n %s: d%d %s", sCHUNK, dm, s_name);

						switch(chunk)
						{
							case LWO_ID_MNVW:
								break;
							case LWO_ID_WGHT:
								break;
							case LWO_ID_TXUV:
								//GPF_CON_FORMAT_Write("\n TXUV: d%d %s", dm, s_name);
								if(!pUV)
								{
									if(nVXCld >= nVXC)
									{
										nVXC += 0x8;
										GPC3DvxcBF** pp_kill = ppVXC;
										ppVXC = new GPC3DvxcBF*[nVXC];
										GPM_MEMCPY(ppVXC, pp_kill, nVXCld);
										GPMZn(ppVXC + nVXCld, nVXC - nVXCld);
									}
									(ppVXC[nVXCld] = pUV = new GPC3DvxcBF)->add(NULL, pPNTS->nVXld(), 2);
								}

								I8 iP, iPld, iF, iI;
								while(pD < (char*)p_u1)
								{
									if( *pD == -1 )
									{ 
										iP = GPF_SWAP_U4(pD); pD += 4;
									} else {
										iP = GPF_SWAP_U2(pD); pD += 2;
									}
									if(*pD == -1)
									{
										iF = GPF_SWAP_U4(pD); pD += 4;
									} else {
										iF = GPF_SWAP_U2(pD); pD += 2;
									}
									
									GPC3Dvxc	*pP = pPNTS->add(NULL, 1, 2),
												*pP0 = (GPC3Dvxc*)pPNTS->vx.p_alloc,
												*pU = pUV->add(NULL, 1, 2),
												*pU0 = (GPC3Dvxc*)pUV->vx.p_alloc;

									GPC3Dvxc xyz = pP0[iP];

									pFC = ppFC[nLAYld]->get(iF);
									for(iI = 0; iI < pFC->n_p; iI++)
									{
										if(pFC->aIX0[iI] != iP)
											continue;
										pFC->aIX1[iI] = pP - pP0;
										*pP = xyz;
										pU->null()->LWO_xy( pD );
										break;
									}
									if(iI == pFC->n_p)
									{
										for(iI = 0; iI < pFC->n_p; iI++)
										{
											if(pFC->aIX0[iI] != iP)
												continue;
											pFC->aIX1[iI] = pP - pP0;
											*pP = xyz;
											pU->null()->LWO_xy(pD);
											break;
										}
									}
									pD += 8;
									//pD = pP->MC2I_i_xy(pD);
								}
								break;
						}
				}
				break;
			}
		}
		if(pLAY)
		{

			nLAYld++;
		}
		return ppVXC[i];
	}

};

__declspec(align(16)) class GPC_MSTR
{
public:
	U2	aSC[0x100], aSP[0x100], aSL[0x100],
		aHC[0x100], aHP[0x100], aHL[0x100];
	U44	hub;
	U4	aBC[gpe3D32on3],
		aBP[gpe3D32on3],
		aBL[gpe3D32on3],
		xfnd, chk;
	U8	ms;

	U14	aBGRA[gpe3D48 * 2];

	U1	aPAT[gpe3D48],		// &0x40
		aLIQ[gpe3D48],		// &0x80

		sFILE[GPD_MAX_PATH - 1], fc_rndr;

	~GPC_MSTR();
	/*{
		*sFILE = 0;
	}*/

	GPC_MSTR(void)
	{
		GPM_CLR;
	}
	U1* PAT2PROTO( U1* pPROT, U1* pPAT )
	{
		U44 s4;
		const U44 mul(0, gpe3D48y, gpe3D48z, 0);
		U4 s, se;
		for(s4.z = 0; s4.z < gpe3D32x; s4.z++)
		for(s4.y = 0; s4.y < gpe3D32x; s4.y++)
		{
			s = s4 * mul;
			gp_memcpy(pPROT, pPAT + s, gpe3D32x);
			pPROT += 32;
		}
		return pPROT - gpe3D32on3;
	}
	gpeMAP* MAP2PROTO( gpeMAP* pPROTO, U1* p_msk )
	{
		if(!pPROTO)
			return NULL;
		gp_zero(pPROTO, gpe3DMAP_SID);
		U44 s4;
		const U44 mul(0, gpe3D48y, gpe3D48z, 0);
		U4 d = 0, s, se;
		for(s4.z = 0; s4.z < gpe3D32x; s4.z++)
		for(s4.y = 0; s4.y < gpe3D32x; s4.y++)
		for(s = s4 * mul, se = s + gpe3D32x; s < se; d++, s++)
		{
			if(p_msk[aBGRA[s].pln] & 0xc0)
				pPROTO[d] = aBGRA[s].pln;
		}

		return pPROTO;
	}
	U14* aBGRAzero(void)
	{
		GPMZn(aBGRA, GPM_N(aBGRA)/2);
		return aBGRA;
	}
	U14* aBGRAx32( U14* p_x32 )
	{
		if( !p_x32 )
			p_x32 = new U14[gpe3D32on3];

		for( U4 i = 0; i < gpe3D32on3; i += gpe3D32x )
		{
			GPM_MEMCPY( p_x32 + i, aBGRA + ((i/gpe3D32x)%gpe3D32x)*gpe3D48y + (i/gpe3D32on2)*gpe3D48z, gpe3D32x );
		}
		return p_x32;
	}

	U14* x32aBGRA(U14* p_x32)
	{
		if(!p_x32)
			return aBGRAzero();

		for(U4 i = 0; i < gpe3D32on3; i += gpe3D32x)
		{
			GPM_MEMCPY( aBGRA + ((i / gpe3D32x) % gpe3D32x)*gpe3D48y + (i / gpe3D32on2)*gpe3D48z, p_x32 + i,  gpe3D32x);
		}
		return aBGRA;
	}

	U14* ins_512x64XiZiy(U4* p_bgra, U1* p_msk, U44* &p_stf_tree, U2& n_stf_tree, U44* &p_bone_tree, U2& n_bone_tree, bool b_clr = true );
	U14* ins_512x544_32on3(U4* p_bgra, U1* p_msk, U1 sid, U44* &p_stf_tree, U2& n_stf_tree, bool b_clr = true);

	GPC_MSTR* neighbor(GPC_MSTR** ppN)
	{
		U14* pD14, *pS14;
		for(U1 o = 1; o < 8; o++)
		{
			pD14 = aBGRA;
			pS14 = ppN[o] ? ppN[o]->aBGRA : NULL;
			if(!pS14)
			{
				switch(o)
				{
				case 1:
					pD14 += 32;
					for(U4 i = 0; i < (48 * 33 * 32); i += 48)
						pD14[i] = 0;
					break;
				case 2:
					pD14 += 32 * 48;
					for(U4 i = 0; i < (48 * 33 * 32); i += 48 * 33)
						GPMZn(pD14 + i, 32);
					break;
				case 3:
					pD14 += 32 * 48 + 32;
					for(U4 i = 0; i < (48 * 33 * 32); i += 48 * 33)
						pD14[i] = 0;
					break;
				case 4:
					pD14 += 33 * 48 * 32;
					for(U4 i = 0; i < (48 * 32); i += 48)
						GPMZn(pD14 + i, 32);
					break;
				case 5:
					pD14 += 33 * 48 * 32 + 32;
					for(U4 i = 0; i < (48 * 32); i += 48)
						pD14[i] = 0;
					break;
				case 6:
					pD14 += 33 * 48 * 33 - 48;
					GPMZn(pD14, 32);
					break;
				default:
					pD14 += 33 * 48 * 33 - 48 + 32;
					*pD14 = 0;
				}
			} else switch(o)
			{
			case 1:
				pD14 += 32;
				for(U4 i = 0; i < (48 * 33 * 32); i += 48)
					pD14[i] = pS14[i];
				break;
			case 2:
				pD14 += 32 * 48;
				for(U4 i = 0; i < (48 * 33 * 32); i += 48 * 33)
					GPM_MEMCPY(pD14 + i, pS14 + i, 32);
				break;
			case 3:
				pD14 += 32 * 48 + 32;
				for(U4 i = 0; i < (48 * 33 * 32); i += 48 * 33)
					pD14[i] = pS14[i];
				break;
			case 4:
				pD14 += 33 * 48 * 32;
				for(U4 i = 0; i < (48 * 32); i += 48)
					GPM_MEMCPY(pD14 + i, pS14 + i, 32);
				break;
			case 5:
				pD14 += 33 * 48 * 32 + 32;
				for(U4 i = 0; i < (48 * 32); i += 48)
					pD14[i] = pS14[i];
				break;
			case 6:
				pD14 += 33 * 48 * 33 - 48;
				GPM_MEMCPY(pD14, pS14, 32);
				break;
			default:
				pD14 += 33 * 48 * 33 - 48 + 32;
				*pD14 = *pS14;
			}
		}

		return this;
	}

	bool bDONE(void)
	{
		U4	n = (U14*)gp_memcmp(aBGRA, aBGRA + gpe3D48, gpe3D48 * sizeof(*aBGRA)) - aBGRA,
			nn = gpe3D48 - n;

		if(!nn)
			return true;

		GPM_MEMCPY(aBGRA + gpe3D48 + n, aBGRA + n, nn);
		return false;
	}
	U1* mstr_MAP2PAT(U1* p_pat, U4 row, U4 lay, U8 all)
	{
		//U4 shr, shl;
		//gp_ddd_lut( gp_memcpy(p_pat, p_stf, 0x1000), pMSK, 0x1000, u4 );
		gp_PATx(p_pat, all);
		gp_PATz(p_pat, all, lay);
		gp_PATy(p_pat, all, row);

		/*gp_ddd_z(p_pat, 0x10, shr = 2); //, u4 );
		gp_ddd_x(p_pat, 0x100, shl = 1);
		gp_ddd_y(p_pat, shr = 4);*/
		return p_pat;
	}
	bool mstr_pat( U1* p_msk)
	{
		if(bDONE())
			return true;


		GPMZ(aPAT);
		GPMZ(aLIQ);
		U4 n = 0;
		U1 pat, liq;
		if( p_msk )
		{
			for( U4 i = 0; i < sizeof(aPAT); i++ )
			{
				pat = p_msk[aBGRA[i].pln];
				if(!pat)
					continue;
				if( (pat&0xc0) == 0xc0 )
				{
					aLIQ[i] = aPAT[i] = 1;
					continue;
				}

				if( pat&0x40 )
					aPAT[i] = 1;
				if( pat&0x80 )
					aLIQ[i] = 1;
				n++;
			}
		} else {
			for(U4 i = 0; i < sizeof(aPAT); i++)
			{
				if(!aBGRA[i].pln)
					continue;
				aPAT[i] = 1;
			}
		}  
		mstr_MAP2PAT( aPAT, gpe3D48y, gpe3D48z, sizeof(aPAT) );
		mstr_MAP2PAT( aLIQ, gpe3D48y, gpe3D48z, sizeof(aLIQ) );
		
		GPMZ(aHC);
		GPMZ(aHP);
		GPMZ(aHL);

		U44 xyz_a0 = 0;
		static const U44 ms(1, gpe3D48y, gpe3D48z);
		U4 aC, a1, aP, aL, b0, b1;
		
		for(xyz_a0.z = 0; xyz_a0.z < gpe3D32x; xyz_a0.z++)
		for(xyz_a0.y = 0; xyz_a0.y < gpe3D32x; xyz_a0.y++)
		{
			a1 = xyz_a0 * ms;
			for( ; xyz_a0.x < gpe3D32x; xyz_a0.x++)
			{
				aC = a1 + xyz_a0.x; // xyz_a0 * ms;
				pat = aPAT[aC];
				liq = aLIQ[aC];
				if(pat == liq)
				{
					if(liq ? liq == 0xff : true)
						continue;

					aHC[pat]++;
					continue;
				}

				if(pat ? pat != 0xff : false)
					aHP[pat]++;

				if(liq ? liq == 0xff : true)
					continue;

				aHL[liq]++;
			}
			xyz_a0.x = 0;
		}

		aC = aP = aL = 0;
		for(U1 h = 0; h < 0xff; h++)
		{
			aSC[h] = aC;
			aC += aHC[h];

			aSP[h] = aP;
			aP += aHP[h];

			aSL[h] = aL;
			aL += aHL[h];
		}
		U2	aSCi[0x100], aSPi[0x100], aSLi[0x100];
		GPM_MEMCPY(aSCi, aSC, 0x100);
		GPM_MEMCPY(aSPi, aSP, 0x100);
		GPM_MEMCPY(aSLi, aSL, 0x100);

		xyz_a0.x = 0;
		for(xyz_a0.z = 0; xyz_a0.z < gpe3D32x; xyz_a0.z++)
		for(xyz_a0.y = 0; xyz_a0.y < gpe3D32x; xyz_a0.y++)
		{
			a1 = xyz_a0 * ms;
			b1 = (xyz_a0.z + xyz_a0.y * 0x100) * 0x100;
			for(; xyz_a0.x < gpe3D32x; xyz_a0.x++)
			{
				aC = a1+xyz_a0.x;
				pat = aPAT[aC];
				liq = aLIQ[aC];
			
				if(pat == liq)
				{
					if(liq ? liq == 0xff : true)
						continue;

					b0 = xyz_a0.x + b1;
					aBC[aSCi[pat]] = b0;
					aSCi[pat]++;
					continue;
				}

				b0 = xyz_a0.x + b1;
				if(pat ? pat != 0xff : false)
				{
					aBP[aSPi[pat]] = b0;
					aSPi[pat]++;
				}

				if(liq ? liq == 0xff : true)
					continue;

				aBL[aSLi[liq]] = b0;
				aSLi[liq]++;
			}
			xyz_a0.x = 0;
		}


		return false;
	}

	void build_cell2(
		GPC3DGR	*pGR,

		U142	*p_cell,
		U4		*p_s,
		U4		*p_sn,
		U1		s
	);
	
	GPC3DGR* build( GPC3DGR* pGR, GPC_CELL *p_cell )
	{
		if(!pGR)
			pGR = new GPC3DGR;
		for(U1 f = 0; f < 6; f++)
		{
			build_cell2(
							pGR,

							p_cell->ap_3d_u142[f],
							p_cell->a_3d_cell_i[f],
							p_cell->a_3d_cell_n[f],
							f
						);
		}
		return pGR;
	}

	U142* cell_set(U142* p_dst, U4* pB0, U2* pH, U142* pC, U4* pS, U4* pSN)
	{
		U142 __declspec(align(16)) a_add[4], *p_frst;
		GPMZ(a_add);

		const U44 mul(1, 48 * 33, 48), mul2(1, 48, 48 * 33);
		I4 aA0[0x20], *pA0, a0,  b0, i, in, j, jn;
		U14 etc, *pBGRA;
		
		for(U4 h = 1; h < 0xff; h++)
		{
			if(!pH[h] || !pSN[h])
			{
				pB0 += pH[h];
				continue;
			}

			in = pH[h];
			jn = pSN[h];
			p_frst = pC + pS[h];
			GPF_MEM_set( p_dst, in, p_frst, jn * sizeof(*p_dst));
			
			for(i = 0; i < in; i++)
			{
				a_add->posn.u4 = pB0[i];
				gp_memset_q(a_add + 1, *(U8*)a_add, 0x18 );

				for(j = 0; j < jn; j += 4)
				{
					gp_i4_add( p_dst+j, a_add );
					
					etc.B = aBGRA[p_dst[j].a0(mul)].B;
					etc.G = aBGRA[p_dst[j+1].a0(mul)].B;
					etc.R = aBGRA[p_dst[j+2].a0(mul)].B;
					etc.A = aBGRA[p_dst[j+3].a0(mul)].B;

					p_dst[j].etc =
					p_dst[j + 1].etc =
					p_dst[j + 2].etc =
					p_dst[j + 3].etc = etc;
				}
				p_dst += jn;
			}
			pB0 += in;
		}

		return p_dst;
	}

	GPC_MSTR* fill(gpeMAP* p8, U1* p_dit8, U4 n_dit8)
	{
		U1 a8x[] = {
			0,1, 2,3, 4,5, 6,7,
		};

		GPMZ(aBGRA);
		U14* pBGRA;
		for(U2 j = 0; j < n_dit8; j++)
		{
			pBGRA = aBGRA + ((j % 16) + (((j / 16) % 16) + (j / (16 * 16)) * 33) * 48) * 2;

			pBGRA[0].pln =
				pBGRA[1].pln =
				pBGRA[48 + 0].pln =
				pBGRA[48 + 1].pln =
				pBGRA[48 * 33 + 0].pln =
				pBGRA[48 * 33 + 1].pln =
				pBGRA[48 * 33 + 48 + 0].pln =
				pBGRA[48 * 33 + 48 + 1].pln =
				p8[p_dit8[j]];
		}

		return this;
	}

	void bite(gpeMAP* pS0); //, U1* p_hori, U1* p_veri );

	GPC_MSTR* erode(U14* pFILT)
	{
		gp_memcpy(pFILT, aBGRA, sizeof(aBGRA));
		I44 ixyz,
			mul(1, 48, 48 * 33);
		U4 a0;
		U14* pBGRA;

		for(ixyz.z = 0; ixyz.z < 48 * 33 * 32; ixyz.z += 48 * 33)
			for(ixyz.y = 0; ixyz.y < 48 * 32; ixyz.y += 48)
				for(ixyz.x = 0; ixyz.x < 32; ixyz.x++)
				{
					a0 = ixyz.sum_xyz();
					if(pFILT[a0].pln)
						continue;
					pBGRA = aBGRA + a0;

					if(ixyz.x)
						pBGRA[-1].pln = gpeMAPspc;

					if(ixyz.y)
						pBGRA[-48].pln = gpeMAPspc;

					if(ixyz.z)
						pBGRA[-48 * 33].pln = gpeMAPspc;

					pBGRA[1].pln =
						pBGRA[48].pln =
						pBGRA[48 * 33].pln = gpeMAPspc;
				}

		/*for( ixyz.z = 0; ixyz.z < 32; ixyz.z++ )
		for( ixyz.y = 0; ixyz.y < 32; ixyz.y++ )
		for( ixyz.x = 0; ixyz.x < 32; ixyz.x++ )
		{
			a0 = (ixyz&mul).sum_xyz();
			if( pFILT[a0].pln )
				continue;
			pBGRA = aBGRA+a0;

			if( ixyz.x )
				pBGRA[-1].pln = gpeMAPspc;

			if( ixyz.y )
				pBGRA[-48].pln = gpeMAPspc;

			if( ixyz.z )
				pBGRA[-48*33].pln = gpeMAPspc;

			pBGRA[1].pln =
			pBGRA[48].pln =
			pBGRA[48*33].pln = gpeMAPspc;
		}*/
		return this;
	}
};
class GPC_DZR;
class GPC3DGR_CACHE
{
public:
	GPC_LAZY	*p_cache,
				*p_lst;
	U4			nLST, nRNDR;
	GPC3DGR_CACHE* reset( GPC_DZR** pp_dzr );
	~GPC3DGR_CACHE( void )
	{
		reset( NULL );
		GPM_DEL(p_cache);
		GPM_DEL(p_lst);
	}
	GPC3DGR_CACHE( void )
	{
		GPM_CLR;
	}
	GPC3DGR* get_i( U4 i )
	{
		if( this ? !p_cache : true )
			return NULL;
		if( i >= nLST )
			return NULL;

		return ((GPC3DGR*)p_cache->p_alloc)+i;
	}
	
	GPC3DGR_CACHE* add_xfnd( GPC3DGR* pGR, U4 xfnd, U4& is, U4& n )
	{
		if( !this )
		{
			GPC3DGR_CACHE* p_this = new GPC3DGR_CACHE;
			if( !p_this )
				return p_this;

			return p_this->add_xfnd( pGR, xfnd, is, n );
		}
		is = nLST;
		p_lst = p_lst->tree_add( xfnd, nLST );
		n = nLST;
		if( is >= nLST )
			return this;
		
		U8 s; 
		p_cache = p_cache->lazy_add( pGR, sizeof( *pGR ), s = -1 );

		if( pGR )
			pGR->b_cpy = true;
		else if( pGR = get_i(nLST-1) )
			pGR->clear( xfnd );
		
		return this;
	}
	GPC3DGR_CACHE* undo( U4& n )
	{
		if( !this )
		{
			GPC3DGR_CACHE* p_this = new GPC3DGR_CACHE;
			if( !p_this )
				return p_this;

			return p_this->undo( n );
		}
		U4 is = nLST-1;
		
		if( U44* p_u44 = (U44*)( p_lst ? p_lst->p_alloc : NULL ) )
		for( U8 i = 0; i < is; i++ )
		{
			if( p_u44[i].z == is )
			{
				p_u44[i].z = 0;
				nLST = is;
				break;
			}
				
			if( p_u44[i].w != is )
				continue;

			p_u44[i].w = 0;
			nLST = is;
			break;
		}
		n = nLST;
		if( is != nLST )
			return this;

		p_lst->n_load -= sizeof(U44*);
		p_cache->n_load -= sizeof(GPC3DGR);
		GPC3DGR* pGR = ((GPC3DGR*)p_cache->p_alloc) + nLST;
		pGR->reset( -1 );

		return this;
	}
	GPC3DGR* fnd( U4 xfnd )
	{
		if( !this )
			return NULL;

		U4 fnd = p_lst->tree_fnd( xfnd, nLST );
		if( fnd >= nLST )
			return NULL;

		GPC3DGR* pGR = (GPC3DGR*)(p_cache ? p_cache->p_alloc : NULL);
		if( !pGR )
			return NULL;

		return pGR+fnd;
	}
	
	ID3D11Buffer* rndr( ID3D11Device* p_dev, ID3D11DeviceContext* p_contx, ID3D11Buffer* pVScb, GPC3DIX* p_ix, U1 x2 )
	{
		if( !this )
			return pVScb;

		if( nRNDR > nLST )
			nRNDR = nLST;

		if( !nRNDR )
			return pVScb;

		U44 u44(0,0,0,1<<x2);
		GPC3DGR* pGR = (GPC3DGR*)p_cache->p_alloc;
		
		for( U4 i = 0; i < nLST; i++ )
			pVScb = pGR[i].rndr( p_dev, p_contx, pVScb, p_ix, u44, false );

		return pVScb;
	}

	ID3D11Buffer* rndr_pnl(ID3D11Device* p_dev, ID3D11DeviceContext* p_contx, ID3D11Buffer* pVScb, GPC3DIX* p_ix, U1 x2, U1 PnL )
	{
		if(!this)
			return pVScb;

		if(nRNDR > nLST)
			nRNDR = nLST;

		if(!nRNDR)
			return pVScb;

		U44 u44(0, 0, 0, 1 << x2);
		GPC3DGR* pGR = (GPC3DGR*)p_cache->p_alloc;

		for(U4 i = 0; i < nLST; i++)
			pVScb = pGR[i].rndr_PnL(p_dev, p_contx, pVScb, p_ix, u44, PnL, false);

		return pVScb;
	}
};
class GPC3DGLB : public GPC3DMX 
{
public:
	GPC3DGR	a3DGR[6]; //GPC3DGR	*ap3DCL[6];
	//U1		*pPAT;
	~GPC3DGLB()
	{
		//for( U1 i = 0; i < 6; i++ )
		//	GPM_DEL( ap3DGR[i] );
		//GPM_DELARRAY(pPAT);
	}
	GPC3DGLB( GPC_PROTO* p_proto, ID3D11Device* p_dev, ID3D11DeviceContext* p_contx, U4* pBGR, GPC3DBLD* pBLD, GPC3D96* p96, GPC_CELL* pCELL, U1* pMSK );
	GPC3DGLB(GPC_PROTO* pPROTO, ID3D11Device* p_dev, ID3D11DeviceContext* p_contx, U4* pBGR, GPC3DBLD* pBLD, GPC_MSTR* pMSTR, GPC_CELL* pCELL, U1* pMSK);
	GPC3DGLB* build( GPC_PROTO* pPROTO, GPC3DGLB* pDST, ID3D11Device* p_dev, ID3D11DeviceContext* p_contx, U4* pBGR, GPC3DBLD* pBLD, GPC3D96* p96, GPC_CELL* pCELL, U1* pMSK );
	GPC3DGLB* build( GPC_PROTO* pPROTO, GPC3DGLB* pDST, ID3D11Device* p_dev, ID3D11DeviceContext* p_contx, U4* pBGR, GPC3DBLD* pBLD, GPC_MSTR* pMSTR, GPC_CELL* pCELL, U1* pMSK);
	
};

class GPC3DCAM
{
public:
	float4x4	mx_ss_w, mx_ss_v, mx_ss_p, mx_ss_wvp,
				mx_ls_w, mx_ls_v, mx_ls_p, mx_ls_wvp;
	float4		sec;
	
	U4				size;
	HRESULT			hr;
	ID3D11Buffer	*p_x11;

	float3		w_eye, w_center, w_up;
	float2		ccd;
	float4		mfdo;	//mount,flen, dist, obj;
		
	GPC3DCAM( float o, float d )
	{
		GPM_CLR;
	
		mx_ss_w.id();
		GPF_MEM_set( &mx_ss_v, 7, &mx_ss_w, sizeof(mx_ss_w) );
		
		hr = 0;
		size = GPM_OFF( GPC3DCAM, size );

		optic( float2( 4.8, 3.6 ), 17.5, 8, d, o );
	}
	void optic( float2 c, float m, float f, float d, float o )
	{
		if( ccd == c )
		if( mfdo == float4( m, f, d, o ) )
			return;
		ccd = c;
		mfdo = float4( m, f, d, o );
		mx_ss_p.proj( ccd, mfdo.x, mfdo.y, mfdo.z, mfdo.w );
		GPM_RELEASE( p_x11 );
	}
	void lookat( float3 &eye, float3 &center, float3 &up )
	{
		if( eye == w_eye )
		if( center == w_center )
		if( up == w_up )
			return;

		w_eye = eye;
		w_center = center;
		w_up = up;

		mx_ss_v.lookat( w_eye, w_center, w_up );
		GPM_RELEASE( p_x11 );
	};
	
	void swap_ss2ls( void )
	{
		mx_ls_p = mx_ss_p;
		mx_ls_v = mx_ss_v;
		mx_ls_wvp = mx_ss_wvp;
	}
	void updt( void )
	{
		GPM_RELEASE( p_x11 );
	}
	ID3D11Buffer* upl( ID3D11Device* p_dev, bool bSS )
	{
		if( p_x11 )
			return p_x11;

		if( bSS )
			mx_ss_wvp = mx_ss_v * mx_ss_p;
		else
			mx_ss_wvp = mx_ss_v; // * mx_ls_p;
		
		D3D11_BUFFER_DESC bDSC;
		GPMZ( bDSC );
		bDSC.ByteWidth = size;
		bDSC.Usage = D3D11_USAGE_DEFAULT;				// D3D11_USAGE_STAGING;// D3D11_USAGE_IMMUTABLE; //D3D11_USAGE_DEFAULT;//    D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
		bDSC.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// use as a vertex buffer
		
		D3D11_SUBRESOURCE_DATA iniDT;
		GPMZ( iniDT );
		iniDT.pSysMem = this;

		hr = p_dev->CreateBuffer( &bDSC, &iniDT, &p_x11 );
		return p_x11;
	}
	/*void updt( void )
	{
		GPM_RELEASE( ap_x11[0] );
	}*/

	void rndr( ID3D11Device* p_dev, ID3D11DeviceContext* p_contx, GPC3DCL* p_x11cl, GPC3DIX* p_ix, bool bSS )
	{
		U4	*p_512s, off = 0, n;
		U2	*p_512n;

		upl( p_dev, bSS );

		p_ix->set( p_dev, p_contx,  p_x11cl->n_ix_max );
		if(p_ix)
			p_contx->IASetPrimitiveTopology(p_ix->top);
		p_contx->VSSetConstantBuffers( 0, 1, &p_x11 );

		for( U1 s = 0; s < 6; s++ )
		{ 
			if( !p_x11cl->anX11vx[s] )
				continue;

			n = p_x11cl->anX11vx[s];
			p_512s = p_x11cl->a_IX + (s*8*8*8);
			p_512n = p_x11cl->a_nIX + (s*8*8*8);

			p_contx->IASetVertexBuffers( 0, 1, &p_x11cl->apX11vx[s], &p_x11cl->stride, &off );

			for( U2 i = 0; i < 0x200; i++ )
			{
				if( p_512s[i] >= n )
					break;
				if( !p_512n[i] )
					continue;

				p_contx->DrawIndexed( p_512n[i], 0, p_512s[i] );
			}

		}
	}

	void rndr( ID3D11Device* p_dev, ID3D11DeviceContext* p_contx, GPC3DCL* p_x11cl, GPC3DIX* p_ix, U44& u44, bool bSS )
	{
		U4	*p_512s, off = 0, n;
		U2	*p_512n;

		upl( p_dev, bSS );

		p_ix->set( p_dev, p_contx,  p_x11cl->n_ix_max );
		if(p_ix)
			p_contx->IASetPrimitiveTopology(p_ix->top);
		p_contx->VSSetConstantBuffers( 0, 1, &p_x11 );
		ID3D11Buffer	*pVS = NULL;
		for( u44.y = 0; u44.y < 6; u44.y++ )
		{
			if( !p_x11cl->anX11vx[u44.y] )
				continue;
			 
			n = p_x11cl->anX11vx[u44.y];
			p_512s = p_x11cl->a_IX + (u44.y*8*8*8);
			p_512n = p_x11cl->a_nIX + (u44.y*8*8*8);
			pVS = u44.upl_n( p_dev, p_contx, pVS, 1 );
			if( pVS )
				p_contx->VSSetConstantBuffers( 3, 1, &pVS );

			p_contx->IASetVertexBuffers( 0, 1, &p_x11cl->apX11vx[u44.y], &p_x11cl->stride, &off );

			for( U2 i = 0; i < 0x200; i++ )
			{
				if( p_512s[i] >= n )
					break;
				if( !p_512n[i] )
					continue;

				p_contx->DrawIndexed( p_512n[i], 0, p_512s[i] );
			}
			GPM_RELEASE(pVS);
		}
		
	}
	ID3D11Buffer* rndr( ID3D11Device* p_dev, ID3D11DeviceContext* p_contx, ID3D11Buffer* pVScb, GPC3DGR* p3DGR, GPC3DIX* p_ix, U44& u44, bool bSS, bool b_occlu );
	ID3D11Buffer* rndr( ID3D11Device* p_dev, ID3D11DeviceContext* p_contx, ID3D11Buffer* pVScb, GPC3DGLB* p3DmxGLOB, GPC3DIX* p_ix, bool bSS, bool b_occlu );
	ID3D11Buffer* rndr( ID3D11Device* p_dev, ID3D11DeviceContext* p_contx, ID3D11Buffer* pVScb, GPC3DGLB* p3DmxGLOB, GPC3DIX* p_ix, U44& u44, bool bSS, bool b_occlu );
	
	ID3D11Buffer* rndr_PnL(ID3D11Device* p_dev, ID3D11DeviceContext* p_contx, ID3D11Buffer* pVScb, GPC3DGLB* p3DmxGLOB, GPC3DIX* p_ix, U44& u44, U1 PnL, bool bSS, bool b_occlu);
	ID3D11Buffer* rndr_PnL(ID3D11Device* p_dev, ID3D11DeviceContext* p_contx, ID3D11Buffer* pVScb, GPC3DGR* pGR, GPC3DIX* p_ix, U44& u44, U1 PnL, bool bSS, bool b_occlu);

};
class GPC3DSHDR
{
public:
	char		s_name[0x100],
				*p_err;
	GPC_ARR		*p_arr;

	ID3D10Blob			*p_VS = NULL,
						*p_PS = NULL,
						*p_er = NULL;
	ID3D11VertexShader	*p_vs;
	ID3D11PixelShader	*p_ps;
	ID3D11InputLayout	*p_lay_u142,
						*p_lay_cl,
						*p_lay_clsk,
						*p_lay_vxc;
	HRESULT		hr;

	GPC3DSHDR(){};
	~GPC3DSHDR( )
	{
		GPM_DEL( p_arr );
		GPM_RELEASE( p_VS );
		GPM_RELEASE( p_PS );
		GPM_RELEASE( p_er );
		GPM_RELEASE( p_vs );
		GPM_RELEASE( p_ps );
		GPM_RELEASE( p_lay_u142 );
		GPM_RELEASE( p_lay_cl );
		GPM_RELEASE( p_lay_clsk );
		GPM_RELEASE( p_lay_vxc );
	}

	GPC3DSHDR( const char* p_name )
	{
		GPM_CLR;
		GPM_STRCPY( s_name, p_name );
	}

	GPC3DSHDR* compi_v1( ID3D11Device* p_dev, const char* p_src, const char* p_name ); //, bool b_u124 );
	void set( ID3D11DeviceContext* p_contx, ID3D11RasterizerState* p_rstr, gpeVX vx )
	{
		p_contx->VSSetShader( p_vs, 0, 0 );
		p_contx->PSSetShader( p_ps, 0, 0 );
		switch( vx )
		{
			case gpeVXu142:
				p_contx->IASetInputLayout( p_lay_u142 );
				break;
			case gpeVXcl:
				p_contx->IASetInputLayout( p_lay_cl );
				break;
			case gpeVXclsk:
				p_contx->IASetInputLayout( p_lay_clsk );
				break;
			case gpeVXc:
				p_contx->IASetInputLayout(p_lay_vxc);
		}
		p_contx->RSSetState( p_rstr );
	}
};
#endif