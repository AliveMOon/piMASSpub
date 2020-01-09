#version 120																		varying vec2 fr_uv;																uniform sampler2D tex0;						// MINI_CHAR_xXy_zXw.png			uniform sampler2D tex1;						// BackGround						
uniform sampler2D tex2;						// MINI 	ABGR?					
											// U4		XYZW					
											//			cFcA 	
uniform sampler2D tex3;						// CUBE												
uniform vec2 tgPX;																	
uniform vec2 xyPX;																	
uniform vec2 whPX;																	
uniform vec2 aTX[8];

#define gpdTX_lo 	tex0
#define gpdTX_loCRL	vec3( aTX[0], 4 )
#define gpdTX_loWH	vec2( 128, 1024 )

#define gpdTX_bg tex1
#define gpdTX_ut tex2

#define gpdTX_hi tex3
#define gpdTX_hiCRL	vec3( aTX[3], 4 )
#define gpdTX_hiWH	vec2( 256, 1536 )

vec2 ac_more( vec4 A )	
{	
	return A.rg; //+vec2(A.g*0x100,0);																		
}																
vec4 cr_lut( vec2 ac, vec3 crl, vec2 lwh ) 							// crl.xyz ColRowLut // lwh LutWH																
{																					
	ac /= crl.xz; 															
	return vec4( 																	
						floor( vec2(fract(ac.x)*crl.x, ac.x) ), 	// char				
						floor( vec2(fract(ac.y)*crl.z, ac.y) )		// LUT		  		
					)																
			/ vec4(	crl.xy, lwh );																
}	
																					
void main()																		
{																					
	vec2	frm1 = fr_uv*whPX,														
			Ain = fract(frm1)/aTX[0],											
			frm0 = frm1/aTX[2],														
			off0 = vec2( 1.0/4.0, 1.0/6.0 );									
	if( xyPX.x < 1 )							
 		off0.x = 0.0;							
	if( xyPX.y < 1 )							
 		off0.y = 0.0;							
	vec4	A = texture2D( gpdTX_ut, frm0 + off0 )*0x100,	
			B;								
																					
	gl_FragColor = vec4( texture2D( gpdTX_bg, fr_uv ).rgb, 0 );			// BG		
	vec4	fr		= cr_lut( A.ba+vec2(0xb0,0), gpdTX_loCRL, gpdTX_loWH ),							
			fr_rgb	= texture2D( gpdTX_lo, fr.zw ),										
			cl		= cr_lut( A.rg, gpdTX_loCRL, gpdTX_loWH ),											
			cl_rgb	= texture2D( gpdTX_lo, cl.zw );										
	if( A.b != 0 )																
		gl_FragColor += texture2D( gpdTX_lo, fr.xy+Ain )*fr_rgb;						
																					
	if( A.r != 0 ) {																				
		if( A.r <= 0x60 )																
			B = texture2D( gpdTX_lo, cl.xy+Ain )*cl_rgb;						
		else {																			
			cl.zw = (A.rr - vec2( 0x20, -0x20 ))/0x10;								
																						
			cl.z = texture2D( gpdTX_lo,														
											floor(vec2(fract(cl.z)*0x10, cl.z)) 		
											/ gpdTX_loWH						
								).a;													
			cl.w = texture2D( gpdTX_lo,														
											floor(vec2(fract(cl.w)*0x10, cl.w)) 		
											/ gpdTX_loWH						
								).a;													
			cl.zw *= 0x100;																
			if( cl.z >= 0x41 &&  cl.z <= 0x5a )											
				cl.w += 0x68;															
			else																		
				cl.w += 0x60;															
																						
			cl.zw /= 8;																	
			cl = vec4( 																				
							floor(vec2(fract(cl.z)*0x8, cl.z)),										
							floor(vec2(fract(cl.w)*0x8, cl.w))										
						) / vec2( 8, 32 ).xyxy + Ain.xyxy;									
			B = max( texture2D(gpdTX_lo, cl.xy ), texture2D(gpdTX_lo, cl.zw ) ) * cl_rgb;		
		}	
			
		if( (B.r+B.g+B.b) > 1.0 )	
		{	
			gl_FragColor = B;	
			return;	
		}	
			
		gl_FragColor += B;	
	}																	
																		
																		
	frm0 *= 2.0;	// frm0 = fr_uv * whPX*2 / aTX[2]																	
	off0 *= 2.0;																	
	//off0.y += 1.0/3.0;																	
	vec2 off_in = 1.0/aTX[2];																	
	A = texture2D( gpdTX_ut, frm0 + off0 )*0x100;																	
	B = texture2D( gpdTX_ut, frm0 + off0 + off_in*vec2( -1, 0 ) )*0x100;																	
	gl_FragColor += A/0x100;			
	vec4	C = texture2D( gpdTX_ut, frm0 + off0 + off_in*vec2(  0, 1 ) )*0x100,	
			D = texture2D( gpdTX_ut, frm0 + off0 + off_in*vec2( -1, 1 ) )*0x100,	
			E = texture2D( gpdTX_ut, frm0 + off0 + off_in*vec2(  0, 2 ) )*0x100,	
			F = texture2D( gpdTX_ut, frm0 + off0 + off_in*vec2( -1, 2 ) )*0x100;	
	frm1 *= 2.0;	
	Ain = fract(frm1)/aTX[3];	
	off_in = vec2(8, 32/3.0)/vec2(128, 1024);	
		
	vec4	frnt = vec4(0.0,0.0,0.0,0.0);	
	cl		= cr_lut( ac_more(E), gpdTX_hiCRL, gpdTX_loCRL );											
	E 		= texture2D( gpdTX_hi, cl.xy+Ain+off_in*vec2( 0, -2 ));	
	if( E.g*E.a > 0.75 )	
		frnt += E.ggga*texture2D( gpdTX_hi, cl.zw );	
		
	cl		= cr_lut( ac_more(B), gpdTX_hiCRL, gpdTX_loCRL );											
	B		= texture2D( gpdTX_hi, cl.xy+Ain+off_in*vec2( 1, 0 ));	
	if( B.g*B.a > 0.75 )	
		frnt += B.ggga*texture2D( gpdTX_hi, cl.zw );	
	if( B.a >= E.a )		
		E = B;	
			
	cl		= cr_lut( ac_more(C), gpdTX_hiCRL, gpdTX_loCRL );											
	C		= texture2D( gpdTX_hi, cl.xy+Ain+off_in*vec2( 0, -1 ));	
	if( C.g*C.a > 0.75 )	
		frnt += C.ggga*texture2D( gpdTX_hi, cl.zw );	
	if( C.a >= E.a )		
		E = C;	
		
	cl		= cr_lut( ac_more(F), gpdTX_hiCRL, gpdTX_loCRL );											
	F		= texture2D( gpdTX_hi, cl.xy+Ain+off_in*vec2( 1, -2 ));	
	if( F.g*F.a > 0.75 )	
		frnt += F.ggga*texture2D( gpdTX_hi, cl.zw );	
	if( F.a >= E.a )		
		E = F;	
		
	cl		= cr_lut( ac_more(A), gpdTX_hiCRL, gpdTX_loCRL );											
	A 		= texture2D( gpdTX_hi, cl.xy+Ain );	
	if( A.g*A.a > 0.75 )	
		frnt += A.ggga*texture2D( gpdTX_hi, cl.zw );	
	if( F.a >= E.a )		
		E = F;	
			
	cl		= cr_lut( ac_more(D), gpdTX_hiCRL, gpdTX_loCRL );											
	D		= texture2D( gpdTX_hi, cl.xy+Ain+off_in*vec2(-1, -1 ));	
	if( D.g*D.a > 0.75 )	
		frnt += D.ggga*texture2D( gpdTX_hi, cl.zw );	
	if( D.a >= E.a )		
		E = D;	
			
	gl_FragColor += frnt+E;	
		
}	













#version 120																	
varying vec2 fr_uv;															
uniform sampler2D tex0;						// MINI_CHAR_xXy_zXw.png		
uniform sampler2D tex1;						// BackGround					
uniform sampler2D gpdTX_ut;						// MINI 	ABGR?				
											// U4		XYZW				
											//			cFcA 
uniform sampler2D tex3;						// CUBE											
uniform vec2 tgPX;																
uniform vec2 xyPX;																
uniform vec2 whPX;																
uniform vec2 aTX[8];
vec2 ac_more( vec4 A )
{
	return A.ba+vec2(A.g*0x100,0);
}															
vec4 cr_lut( vec2 ac, vec3 crl, vec2 lwh ) 							// crl.xyz ColRowLut // lwh LutWH															
{																				
	ac /= crl.xz; 														
	return vec4( 																
						floor( vec2(fract(ac.x)*crl.x, ac.x) ), 	// char			
						floor( vec2(fract(ac.y)*crl.z, ac.y) )		// LUT		  	
					)															
			/ vec4(	crl.xy, lwh );															
}
																				
void main()																	
{																				
	vec2	frm1 = fr_uv*whPX,													
			Ain = fract(frm1)/aTX[0],										
			frm0 = frm1/aTX[2],													
			off0 = vec2( 1.0/4.0, 1.0/6.0 );								
	if( xyPX.x < 1 )						
 		off0.x = 0.0;						
	if( xyPX.y < 1 )						
 		off0.y = 0.0;						
	vec4	A = texture2D( gpdTX_ut, frm0 + off0 )*0x100,
			B;							
																				
	gl_FragColor = vec4( texture2D( gpdTX_bg, fr_uv ).rgb, 0 );			// BG	
	vec4	fr		= cr_lut( A.ba+vec2(0xb0,0), vec3(8, 32, 4), vec2( 128, 1024 ) ),						
			fr_rgb	= texture2D( tex0, fr.zw ),									
			cl		= cr_lut( A.rg, vec3(8, 32, 4), vec2( 128, 1024 ) ),										
			cl_rgb	= texture2D( tex0, cl.zw );									
	if( A.b != 0 )															
		gl_FragColor += texture2D( tex0, fr.xy+Ain )*fr_rgb;					
																				
	if( A.r != 0 ) {																			
		if( A.r <= 0x60 )															
			B = texture2D( tex0, cl.xy+Ain )*cl_rgb;					
		else {																		
			cl.zw = (A.rr - vec2( 0x20, -0x20 ))/0x10;							
																					
			cl.z = texture2D( tex0,													
											floor(vec2(fract(cl.z)*0x10, cl.z)) 	
											/ vec2( 128, 1024 )					
								).a;												
			cl.w = texture2D( tex0,													
											floor(vec2(fract(cl.w)*0x10, cl.w)) 	
											/ vec2( 128, 1024 )					
								).a;												
			cl.zw *= 0x100;															
			if( cl.z >= 0x41 &&  cl.z <= 0x5a )										
				cl.w += 0x68;														
			else																	
				cl.w += 0x60;														
																					
			cl.zw /= 8;																
			cl = vec4( 																			
							floor(vec2(fract(cl.z)*0x8, cl.z)),									
							floor(vec2(fract(cl.w)*0x8, cl.w))									
						) / vec2( 8, 32 ).xyxy + Ain.xyxy;								
			B = max( texture2D(tex0, cl.xy ), texture2D(tex0, cl.zw ) ) * cl_rgb;	
		}
		
		if( B.a > 0.75 )
		{
			gl_FragColor = B;
			return;
		}
		
		gl_FragColor += B;
	}
	
	
	frm0 *= 2.0;	// frm0 = fr_uv * whPX*2 / aTX[2] 
	off0 *= 2.0;
	off0.y += 1.0/3.0;
	vec2 off_in = 1.0/aTX[2];
	A = texture2D( gpdTX_ut, frm0 + off0 )*0x100;
	B = texture2D( gpdTX_ut, frm0 + off0 + off_in*vec2( -1, 0 ) )*0x100;
			
	vec4	C = texture2D( gpdTX_ut, frm0 + off0 + off_in*vec2(  0, 1 ) )*0x100,
			D = texture2D( gpdTX_ut, frm0 + off0 + off_in*vec2( -1, 1 ) )*0x100,
			E = texture2D( gpdTX_ut, frm0 + off0 + off_in*vec2(  0, 2 ) )*0x100,
			F = texture2D( gpdTX_ut, frm0 + off0 + off_in*vec2( -1, 2 ) )*0x100;
	frm1 *= 2.0;
	Ain = fract(frm1)/aTX[3];
	off_in = vec2(8, 8)/vec2(256, 1536);
	
	vec4	frnt = 0;
	cl		= cr_lut( ac_more(E), vec3(16, 32, 4), vec2( 128, 1024 ) );										
	E 		= texture2D( tex3, cl.xy+Ain+off_in*vec2( 0, -2 ));
	if( E.g*E.a > 0.75 )
		frnt += E.ggga*texture2D( tex0, cl.zw );
	
	cl		= cr_lut( ac_more(B), vec3(16, 32, 4), vec2( 128, 1024 ) );										
	B		= texture2D( tex3, cl.xy+Ain+off_in*vec2( 1, 0 ));
	if( B.g*B.a > 0.75 )
		frnt += B.ggga*texture2D( tex0, cl.zw );
	if( B.a >= E.a )	
		E = B;
		
	cl		= cr_lut( ac_more(C), vec3(16, 32, 4), vec2( 128, 1024 ) );										
	C		= texture2D( tex3, cl.xy+Ain+off_in*vec2( 0, -1 ));
	if( C.g*C.a > 0.75 )
		frnt += C.ggga*texture2D( tex0, cl.zw );
	if( C.a >= E.a )	
		E = C;
	
	cl		= cr_lut( ac_more(F), vec3(16, 32, 4), vec2( 128, 1024 ) );										
	F		= texture2D( tex3, cl.xy+Ain+off_in*vec2( 1, -2 ));
	if( F.g*F.a > 0.75 )
		frnt += F.ggga*texture2D( tex0, cl.zw );
	if( F.a >= E.a )	
		E = F;
	
	cl		= cr_lut( ac_more(A), vec3(16, 32, 4), vec2( 128, 1024 ) );										
	A 		= texture2D( tex3, cl.xy+Ain );
	if( A.g*A.a > 0.75 )
		frnt += A.ggga*texture2D( tex0, cl.zw );
	if( F.a >= E.a )	
		E = F;
		
	cl		= cr_lut( ac_more(D), vec3(16, 32, 4), vec2( 128, 1024 ) );										
	D		= texture2D( tex3, cl.xy+Ain+off_in*vec2(-1, -1 ));
	if( D.g*D.a > 0.75 )
		frnt += D.ggga*texture2D( tex0, cl.zw );
	if( D.a >= E.a )	
		E = D;
		
	gl_FragColor += frnt+E;
	
};













