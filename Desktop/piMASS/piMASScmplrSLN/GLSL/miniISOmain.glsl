
// char gpsSHDRvx[] =
	#version 120										
	attribute	vec2	v_vx;							
	attribute	vec2	v_uv;							
	varying	vec2	fr_uv;							
	void main()										
	{																	
		gl_Position			= vec4( v_vx*vec2(2.0,-2.0)+vec2(-1.0,1.0), 0, 1);		
		fr_uv				= v_uv;										
	}
	
// char gpsSHDRisoFRG[] =
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
#define gpdTXiz	vec2( 0, 3.0/1536 )																			
																												
#define sb 0.1																									
#define gpdTXbg tex1																							
#define gpdTXut tex2																							
																												
vec4 cr_lut( vec4 rgba, vec3 crl, vec2 lwh ) 				// crl.xyz ColRowLut 	// lwh LutWH				
{																												
	vec2 ac = (rgba.rg/crl.xz);																					
	vec4 o = vec4( 																								
					floor( vec2(fract(ac.x)*crl.x, ac.x)	),							// char					
					floor( vec2(fract(ac.y)*crl.z, ac.y)	)  							// LUT					
				) / vec4( crl.xy, lwh ) + vec2(0.0, rgba.b*0.25).xyxx;											
	if( ac.y > crl.z )																							
		o.w += 1.0;																								
	return o;																									
}																												
																												
void main()																									
{																												
	vec2	frm1 = fr_uv*FRMwh,																					
			frm0 = frm1/aTX[2],								// atx[2] == TXwh									
			t2x3 = vec2( 1.0/2.0, 1.0/3.0 )/gpdTXcrl.xy,	// aTX[0] == mn_iso_CR_32x32						
			Ain = fract(frm1)*t2x3,							// atx[0] == mnCR									
			off0 = vec2( 1.0/2.0, 1.0/4.0 ),																	
			off2x3 = 1.0/aTX[2];							// aTX[3] == mn_iso_CR								
																												
	if( DIVxy.x < 1 )																							
 		off0.x = 0.0;																							
	if( DIVxy.y < 1 )																							
 		off0.y = 0.0;																							
																												
	gl_FragColor = vec4( texture2D( gpdTXbg, fr_uv ).rgb, 0 );				// BG								
	gl_FragColor *= gl_FragColor*0.25;																			
	// FRM -------------------------------------------------------												
	float B = 0, Aga; 																							
	vec4 A = texture2D( gpdTXut, frm0 + off0+vec2(0.0,0.5) )*0x100, cl, Bc = vec4(0,0,0,0);						
	if( A.r > 0 )																								
	{																											
		cl	= cr_lut( A, vec3( aTX[0]*vec2(1,3), 4 ), gpdTXwh );												
		gl_FragColor += texture2D( gpdTXlo, cl.xy+Ain+vec2(0,5.0/32.0) )*texture2D( gpdTXlo, cl.zw );			
	}																											
	off0.y += off2x3.y;																							
	// -1 0		//  0 1																							
	// +-+-+	// +-+-+																						
	// |D|C| -1	// |E|F| 0																						
	// +-+-+	// +-+-+																						
	// |B|A| 0	// |A|B| 1																						
	// +-+-+	// +-+-+																						
	// |F|E| 1	// |C|D| 2																						
	// +-+-+	// +-+-+																						
	// C --------------------------------------------------------												
	A = texture2D( gpdTXut, frm0 + off0 + off2x3*vec2( 0, -1 ) )*0x100;											
	cl	= cr_lut( A, gpdTXcrl, gpdTXwh );												/// !!!! ------------			
	if( A.r > 0 )																								
	{																											
		A 	= texture2D( gpdTXlo, cl.xy+Ain+t2x3*vec2( 0, 2 )-gpdTXiz );										
		Aga = A.g*A.a; 																							
		if( cl.w >= 1.0 )																						
		{																										
			cl.w = fract(cl.w);																					
			B += (1.0-Aga);																						
			Bc += texture2D( gpdTXlo, cl.zw );																	
		}																										
		else if( A.a > (1.0/256) )																				
		{																										
			if( Aga > 0.5 )																						
			{																									
				Aga = (Aga-0.5)*2.0;																			
				if( Aga > 0.9 )																					
				{																								
					B += Aga;																					
					Bc += texture2D( gpdTXlo, cl.zw );															
				}																								
			} //else																							
			gl_FragColor = max( gl_FragColor, Aga*texture2D( gpdTXlo, cl.zw ));									
		}																										
	}																									
	else if( cl.w >= 1.0 )																				
		{																								
			cl.w = fract(cl.w);																			
			B += 1;																						
			Bc += texture2D( gpdTXlo, cl.zw );															
		}																								
	// D --------------------------------------------------------										
	A = texture2D( gpdTXut, frm0 + off0 + off2x3*vec2(-1, -1 ) )*0x100;									
	if( A.r > 0 )																						
	{																									
		cl	= cr_lut( A, gpdTXcrl, gpdTXwh );															
		A 	= texture2D( gpdTXlo, cl.xy+Ain+t2x3*vec2( 1, 2 )-gpdTXiz );										
		Aga = A.g*A.a; 		
		if( cl.w >= 2.0 )																				
		{																								
			cl.w = fract(cl.w);
			B += (1.0-Aga);
			Bc += texture2D( gpdTXlo, cl.zw );										
		}																								
		else if( A.a > (1.0/256) )
		{
			if( Aga > 0.5 )																			
			{
				Aga = (Aga-0.5)*2.0;
				if( Aga > 0.9 )
				{
					B += Aga;
					Bc += texture2D( gpdTXlo, cl.zw );
				}			
			} //else																		
			gl_FragColor = max( gl_FragColor, Aga*texture2D( gpdTXlo, cl.zw ));							
		}																			
	}																									


	// B --------------------------------------------------------										
	A = texture2D( gpdTXut, frm0 + off0 + off2x3*vec2( -1, 0 ) )*0x100;						
	if( A.r > 0 )																						
	{																									
		cl	= cr_lut( A, gpdTXcrl, gpdTXwh );															
		A 	= texture2D( gpdTXlo, cl.xy+Ain+t2x3*vec2( 1, 1 )-gpdTXiz );										
		Aga = A.g*A.a; 		
		if( cl.w >= 2.0 )																				
		{																								
			cl.w = fract(cl.w);
			B += (1.0-Aga);
			Bc += texture2D( gpdTXlo, cl.zw );										
		}																								
		else if( A.a > (1.0/256) )
		{
			if( Aga > 0.5 )																			
			{
				Aga = (Aga-0.5)*2.0;
				if( Aga > 0.9 )
				{
					B += Aga;
					Bc += texture2D( gpdTXlo, cl.zw );
				}			
			} // else																		
			gl_FragColor = max( gl_FragColor, Aga*texture2D( gpdTXlo, cl.zw ));							
		}																			
	}																									

	// A --------------------------------------------------------										
	A = texture2D( gpdTXut, frm0 + off0 ) *0x100;														
	cl	= cr_lut( A, gpdTXcrl, gpdTXwh );																	/// !!!! ------------
	if( A.r > 0 )																						
	{																									
		A 	= texture2D( gpdTXlo, cl.xy+Ain+t2x3*vec2( 0, 1 )-gpdTXiz );										
		Aga = A.g*A.a; 		
		if( cl.w >= 1.0 )																				
		{																								
			cl.w = fract(cl.w);
			B += (1.0-Aga);
			Bc += texture2D( gpdTXlo, cl.zw );										
		}																								
		else if( A.a > (1.0/256) )
		{
			if( Aga > 0.5 )																			
			{
				Aga = (Aga-0.5)*2.0;
				if( Aga > 0.9 )
				{
					B += Aga;
					Bc += texture2D( gpdTXlo, cl.zw );
				}			
			}  //else																
			gl_FragColor = max( gl_FragColor, Aga*texture2D( gpdTXlo, cl.zw ));		
		}																			
	}																				
	else if( cl.w >= 1.0 )															
		{																			
			cl.w = fract(cl.w);														
			B += 1.0;																
			Bc += texture2D( gpdTXlo, cl.zw );										
		}																			
																					
	// E --------------------------------------------------------					
	A = texture2D( gpdTXut, frm0 + off0 + off2x3*vec2( 0, 1 ) )*0x100;				
	if( A.r > 0 )																	
	{																				
		cl	= cr_lut( A, gpdTXcrl, gpdTXwh );										
		A 	= texture2D( gpdTXlo, cl.xy+Ain+t2x3*vec2( 0, 0 )-gpdTXiz );			
		Aga = A.g*A.a; 																
		if( cl.w >= 2.0 )															
		{																			
			cl.w = fract(cl.w);														
			B += (1.0-Aga);															
			Bc += texture2D( gpdTXlo, cl.zw );										
		}																			
		else if( A.a > (1.0/256) )													
		{																			
			if( Aga > 0.5 )															
			{																		
				Aga = (Aga-0.5)*2.0;												
				if( Aga > 0.9 )														
				{																	
					B += Aga;														
					Bc += texture2D( gpdTXlo, cl.zw );								
				}																	
			}  																		
			gl_FragColor = max( gl_FragColor, Aga*texture2D( gpdTXlo, cl.zw ));		
		}																			
	}																				
																					
	// F --------------------------------------------------------					
	A = texture2D( gpdTXut, frm0 + off0 + off2x3*vec2( -1, 1 ) )*0x100;				
	if( A.r > 0 )																	
	{																				
		cl	= cr_lut( A, gpdTXcrl, gpdTXwh );										
		A = texture2D( gpdTXlo, cl.xy+Ain+t2x3*vec2( 1, 0 )-gpdTXiz );				
 		Aga = A.g*A.a; 																
		if( cl.w >= 2.0 )															
		{																			
			cl.w = fract(cl.w);														
			B += (1.0-Aga);															
			Bc += texture2D( gpdTXlo, cl.zw );										
		}																			
		else if( A.a > (1.0/256) )
		{
			if( Aga > 0.5 )																			
			{
				Aga = (Aga-0.5)*2.0;
				if( Aga > 0.9 )
				{
					B += Aga;
					Bc += texture2D( gpdTXlo, cl.zw );
				}			
			}  //else																		
			gl_FragColor = max( gl_FragColor, Aga*texture2D( gpdTXlo, cl.zw ));							
		}																				
	}																									
	
	if( B < 0.5 )																						
	{																									
		if( gl_FragColor.a < 0.33 )																						
 			discard;
 		return;
	}																									
	gl_FragColor += Bc/(6+B+gl_FragColor.a);											

};