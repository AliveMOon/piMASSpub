#version 120																							
varying vec2 fr_uv;																					
uniform sampler2D tex0;					// MINI_CHAR_xXy_zXw.png									
uniform sampler2D tex1;					// BackGround												
uniform sampler2D tex2;					// MINI 	ABGR?											
										// U4		XYZW											
										//			cFcA 											
//uniform sampler2D tex3;				// CUBE														
uniform vec2 tgPX;																						
uniform vec2 DIVxy;																					
uniform vec2 FRMwh;																					
uniform vec2 aTX[8];
#define gpdTXmin	tex0																				
#define gpdTXbg		tex1																					
#define gpdTXutf	tex2
#define gpdTXcrl	vec3( aTX[0], 4 )		// aTX[0] = vec2( 32, 32 )									
#define gpdTXwh	vec2( 1024, 1536 )																	
#define gpdTXiz	vec2( 0, 3.0/1536 )	
vec4 cr_lut( vec4 rgba, vec3 crl, vec2 lwh ) 				// crl.xyz ColRowLut 	// lwh LutWH		
{
	// MINI
	// U4		
	// XYZW
	// ABGR?
	// cFcA
	vec2 ac = (rgba.rg/crl.xz);																			
	vec4 o = vec4( 																						
					floor( vec2( fract(ac.x)*crl.x, ac.x )	),							// char			
					floor( vec2( fract(ac.y)*crl.z, ac.y+(1.0/3.0) )	)  				// LUT			
				) / vec4( crl.xy, lwh ) + vec2(0.0, rgba.b*0.25).xyxx;									
	if( ac.y > crl.z )																					
		o.w += 1.0;																						
	return o;																							
}
void main()																							
{
	const vec4 m0p13 = vec3(-1,0,1, 1.0/3.0);
	vec2	frm1 = fr_uv*FRMwh,																			
			frm0 = frm1/aTX[2],								// atx[2] == TXwh							
			t2x3 = vec2( 1.0/2.0, m0p13.w )/gpdTXcrl.xy,	// aTX[0] == mn_iso_CR_32x32				
			Ain = fract(frm1)*t2x3,							// atx[0] == mnCR							
			off0 = vec2( 1.0/2.0, 1.0/4.0 ),															
			off2x3 = 1.0/aTX[2];							// aTX[3] == mn_iso_CR	
			
	gl_FragColor = vec4( texture2D( gpdTXbg, fr_uv ).rgb, 0 );	// BG						
	
	if( DIVxy.x < 1 )																					
 		off0.x = 0.0;																					
	if( DIVxy.y < 1 )																					
 		off0.y = 0.0;																					
	
	// FRM -------------------------------------------------------------------------------
	vec4	O, A, B,
			C, D,
			E,F = texture2D( gpdTXutf, frm0 + off0+vec2(0.0,0.5) )*0x100,
			cl;
	if( F.r > 0 ) {																									
		cl = cr_lut( F, gpdTXcrl*vec3(1,3,1), gpdTXwh );
		F = texture2D( gpdTXmin, cl.xy+Ain+vec2(0,5.0/32.0) );
	}
	// UTF		// MIN		
	// -1 0		t2x3*vec2	// UT		// MIN														
	// +-+-+	+--+--+		// -1 0		//  0 1																				
	// |A|B|-1	|00|10|		// +-+-+	// +-+-+																		
	// +-+-+	+--+--+		// |D|C| -1	// |E|F| 0																	
	// |C|D|0	|01|11|		// +-+-+	// +-+-+																			
	// +-+-+	+--+--+		// |B|A| 0	// |A|B| 1																		
	// |E|F|1	|02|12|		// +-+-+	// +-+-+																		
	// +-+-+	+--+--+     // |F|E| 1	// |C|D| 2	
	// MINI ----------------// +-+-+	// +-+-+	--------------------------------------
	if( F.a < 0.25 ) {
		E = texture2D( gpdTXutf, frm0 + off0 )*0x100;
			cl = cr_lut( E, gpdTXcrl, gpdTXwh );
		if( E.r > 0 ) {
			E = texture2D( gpdTXmin, cl.xy+Ain+t2x3*vec2( 0, 2 )-gpdTXiz );
		} else 
			E = m0p13.yyyy;
		if( cl.w >= 1.0 ){
			// ki van jelölve
			if( (E.a*E.g)>0.5 )
				F = texture2D( gpdTXmin, cl.zw );
			else
				F = m0p13.yyyz;
		} else {
			E *= texture2D( gpdTXmin, cl.zw );
			
			A = texture2D( gpdTXutf, frm0 + off0 + off2x3*vec2( 0, 2) )*0x100;	
			B = texture2D( gpdTXutf, frm0 + off0 + off2x3*vec2(-1, 2) )*0x100;	
			C = texture2D( gpdTXutf, frm0 + off0 + off2x3*vec2( 0, 1) )*0x100;	
			D = texture2D( gpdTXutf, frm0 + off0 + off2x3*vec2(-1, 1) )*0x100;	
			F = texture2D( gpdTXutf, frm0 + off0 + off2x3*vec2(-1, 0) )*0x100;	
			if( A.r > 0 ) {
				cl = cr_lut( A, gpdTXcrl, gpdTXwh );
				A = texture2D( gpdTXmin, cl.xy+Ain+t2x3*vec2( 0, 0 )-gpdTXiz )*texture2D( gpdTXmin, cl.zw );
			} else 
				A = m0p13.yyyy; 
			if( B.r > 0 ) {
				cl = cr_lut( B, gpdTXcrl, gpdTXwh );
				B = texture2D( gpdTXmin, cl.xy+Ain+t2x3*vec2( 1, 0 )-gpdTXiz )*texture2D( gpdTXmin, cl.zw );
			} else 
				B = m0p13.yyyy; 
			
			if( C.r > 0 ) {
				cl = cr_lut( C, gpdTXcrl, gpdTXwh );
				C = texture2D( gpdTXmin, cl.xy+Ain+t2x3*vec2( 0, 1 )-gpdTXiz )*texture2D( gpdTXmin, cl.zw );
			} else
				C = m0p13.yyyy;
			
			if( D.r > 0 ) {
				cl = cr_lut( D, gpdTXcrl, gpdTXwh );
				D = texture2D( gpdTXmin, cl.xy+Ain+t2x3*vec2( 1, 1 )-gpdTXiz )*texture2D( gpdTXmin, cl.zw );
			} else 
				D = m0p13.yyyy; 
			
			if( F.r > 0 ) {
				cl = cr_lut( F, gpdTXcrl, gpdTXwh );
				F = texture2D( gpdTXmin, cl.xy+Ain+t2x3*vec2( 1, 2 )-gpdTXiz )*texture2D( gpdTXmin, cl.zw );
			} else
				F = m0p13.yyyy; 
			
			F = max(max(max(A,B),C), 
					max(max(D,E),F));
			
		}
	} else 
		F *= texture2D( gpdTXmin, cl.zw );
	
	F.a *= dot(F.rgb, m0p13.www );
 	if( F.a < 0.25 ) 
		discard; 
}