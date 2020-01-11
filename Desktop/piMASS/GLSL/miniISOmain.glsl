char gpsSHDRisoFRG2[] =
#version 120																							
varying vec2 fr_uv;																					
uniform sampler2D tex0;					// MINI_CHAR_xXy_zXw.png								
uniform sampler2D tex1;					// BackGround											
uniform sampler2D tex2;					// MINI 	ABGR?										
											// U4		XYZW										
											//			cFcA 										
//uniform sampler2D tex3;					// CUBE													
uniform vec2 tgPX;																						
uniform vec2 DIVxy;																					
uniform vec2 FRMwh;																					
uniform vec2 aTX[8];

#define gpdTXlo 	tex0																				
#define gpdTXcrl	vec3( aTX[0], 4 )		// aTX[0] = vec2( 32, 32 )																
#define gpdTXwh	vec2( 1024, 1536 )																	
				
#define gpdTXbg tex1																					
#define gpdTXut tex2
																										
vec4 cr_lut( vec4 rgba, vec3 crl, vec2 lwh ) 	// crl.xyz ColRowLut // lwh LutWH						
{																										
	vec2 ac = rgba.rg/ crl.xz; 																						
	return vec4( 																						
					floor( vec2(fract(ac.x)*crl.x, ac.x) ), 	// char								
					floor( vec2(fract(ac.y)*crl.z, ac.y) )		// LUT		  						
				)																					
				/ vec4(	crl.xy, lwh );																		
}
void main()																							
{																										
	vec2	frm1 = fr_uv*FRMwh,																			
			frm0 = frm1/aTX[2],								// atx[2] == TXwh
			t2x3 = vec2( 1.0/2.0, 1.0/3.0 )/gpdTXcrl.xy,	// aTX[0] == mn_iso_CR_32x32
			t1x1 = t2x3*vec2( 2.0, 3.0 ),										
			Ain = fract(frm1)*t2x3,							// atx[0] == mnCR							
			off0 = vec2( 1.0/2.0, 1.0/4.0 ),
			off2x3 = 1.0/aTX[2];							// aTX[3] == mn_iso_CR			
	
	if( DIVxy.x < 1 )																					
 		off0.x = 0.0;																					
	if( DIVxy.y < 1 )																					
 		off0.y = 0.0;																					
			
	gl_FragColor = vec4( texture2D( gpdTXbg, fr_uv ).rgb, 0 );				// BG						
	gl_FragColor *= gl_FragColor;
/*	
	cl	= cr_lut( 
					texture2D( gpdTXut, frm0 + off0 + vec2( 0, 1.0/2.0 ) ), 
					gpdTXcrl, gpdTXwh );											
	vec4 FR = texture2D( gpdTXlo, cl.xy+Ain+t2x3*vec2( 0, 2 ) );
	 */
	// -1 0
	// +-+-+
	// |B|A| 0
	// +-+-+
	// |D|C| 1	
	// +-+-+
	// |F|E| 2
	// +-+-+
	
	// F --------------------------------------------------------
	vec4 	A = texture2D( gpdTXut, frm0 + off0 + off2x3*vec2( -1, 2 ) )*0x100,
			cl	= cr_lut( A, gpdTXcrl, gpdTXwh );											
	A = texture2D( gpdTXlo, cl.xy+Ain+t2x3*vec2( 1, 0 ) );							
	if( cl.w >= 1.0 )																		
	{																						
		cl.w = fract(cl.w);																	
		gl_FragColor = texture2D( gpdTXlo, cl.zw )*(1.0-A.g);								
	}																						
	else if( A.a > (1.0/256) )
	if( A.g*A.a > 0.75 )
	{
		gl_FragColor = A.ggga*texture2D( gpdTXlo, cl.zw );
	} else 
		gl_FragColor = A;
	
	// E --------------------------------------------------------
	A = texture2D( gpdTXut, frm0 + off0 + off2x3*vec2( 0, 2 ) )*0x100;
	if( A.r > 0 )
	{
		cl	= cr_lut( A, gpdTXcrl, gpdTXwh );											
		A 	= texture2D( gpdTXlo, cl.xy+Ain+t2x3*vec2( 0, 0 ) );								
		if( cl.w >= 1.0 )																		
		{																						
			cl.w = fract(cl.w);																	
			gl_FragColor = texture2D( gpdTXlo, cl.zw )*(1.0-A.g);								
		}																						
		else if( A.a > (1.0/256) )
		if( A.g*A.a > 0.75 )
		{
			gl_FragColor = A.ggga*texture2D( gpdTXlo, cl.zw );
		} else 
			gl_FragColor = A;
	}
	// D --------------------------------------------------------
	A = texture2D( gpdTXut, frm0 + off0 + off2x3*vec2(-1, 1 ) )*0x100;
	if( A.r > 0 )
	{
		cl	= cr_lut( A, gpdTXcrl, gpdTXwh );											
		A 	= texture2D( gpdTXlo, cl.xy+Ain+t2x3*vec2( 1, 1 ) );										
		if( cl.w >= 1.0 )																		
		{																						
			cl.w = fract(cl.w);																	
			gl_FragColor = texture2D( gpdTXlo, cl.zw )*(1.0-A.g);								
		}																						
		else if( A.a > (1.0/256) )
		if( A.g*A.a > 0.75 )
		{
			gl_FragColor = A.ggga*texture2D( gpdTXlo, cl.zw );
		} else 
			gl_FragColor = A;
	}
	
	// C --------------------------------------------------------
	A = texture2D( gpdTXut, frm0 + off0 + off2x3*vec2( 0, 1 ) )*0x100;
	if( A.r > 0 )
	{
		cl	= cr_lut( A, gpdTXcrl, gpdTXwh );											
		A 	= texture2D( gpdTXlo, cl.xy+Ain+t2x3*vec2( 0, 1 ) );										
		if( cl.w >= 1.0 )																		
		{																						
			cl.w = fract(cl.w);																	
			gl_FragColor = texture2D( gpdTXlo, cl.zw )*(1.0-A.g);								
		}																						
		else if( A.a > (1.0/256) )
		if( A.g*A.a > 0.75 )
		{
			gl_FragColor = A.ggga*texture2D( gpdTXlo, cl.zw );
		} else 
			gl_FragColor = A;
	}
	// B --------------------------------------------------------
	A = texture2D( gpdTXut, frm0 + off0 + off2x3*vec2( -1, 0 ) )*0x100;
	if( A.r > 0 )
	{
		cl	= cr_lut( A, gpdTXcrl, gpdTXwh );											
		A 	= texture2D( gpdTXlo, cl.xy+Ain+t2x3*vec2( 1, 2 ) );										
		if( cl.w >= 1.0 )																		
		{																						
			cl.w = fract(cl.w);																	
			gl_FragColor = texture2D( gpdTXlo, cl.zw )*(1.0-A.g);								
		}																						
		else if( A.a > (1.0/256) )
		if( A.g*A.a > 0.75 )
		{
			gl_FragColor = A.ggga*texture2D( gpdTXlo, cl.zw );
		} else 
			gl_FragColor = A;
	}
	
	// A --------------------------------------------------------
	A = texture2D( gpdTXut, frm0 + off0 )*0x100;
	if( A.r > 0 )
	{
		cl	= cr_lut( A, gpdTXcrl, gpdTXwh );											
		A 	= texture2D( gpdTXlo, cl.xy+Ain+t2x3*vec2( 0, 2 ) );										
		if( cl.w >= 1.0 )																		
		{																						
			cl.w = fract(cl.w);																	
			gl_FragColor = texture2D( gpdTXlo, cl.zw )*(1.0-A.g);								
		}																						
		else if( A.a > (1.0/256) )
		if( A.g*A.a > 0.75 )
		{
			gl_FragColor = A.ggga*texture2D( gpdTXlo, cl.zw );
		} else 
			gl_FragColor = A;
	}
	
}