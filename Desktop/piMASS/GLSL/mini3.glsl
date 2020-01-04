char gpsSHDRvx[] =	
	#version 120										
	attribute	vec2	v_vx;						
	attribute	vec2	v_uv;						
	varying	vec2	fr_uv;						
	void main()										
	{													
		gl_Position			= vec4( v_vx*vec2(2.0,-2.0)+vec2(-1.0,1.0), 0, 1);		
		fr_uv				= v_uv;										
	}													
	
char gpsSHDR[] =	
	#version 120																	
	varying vec2 fr_uv;															
	uniform sampler2D tex0;						// MINI_CHAR_xXy_zXw.png		
	uniform sampler2D tex1;						// MINI 	ABGR?				
														// U4		XYZW				
														//			cFcA 				
	uniform sampler2D tex2;						// BackGround					
	uniform vec2 tgPX;																
	uniform vec2 xyPX;																
	uniform vec2 whPX;																
	uniform vec2 aTX[8];															
	vec4 cr_lut( vec2 ac )															
	{																				
		ac /= vec2(8.0,4.0);														
		return vec4( 																
							floor( vec2(fract(ac.x)*8.0, ac.x) ), 	// char			
							floor( vec2(fract(ac.y)*4.0, ac.y) )	// LUT		  	
						)															
				/ vec4(																
							8.0,32.0,												
							128.0,1024.0											
						);															
	}																				
	void main()																	
	{																				
		vec2	frm1 = fr_uv*whPX,													
				big_in = fract(frm1)/aTX[0],										
				frm0 = frm1/aTX[2];													
		vec4	big = texture2D( tex2, frm0 )*0x100;								
																					
		gl_FragColor = vec4( texture2D( tex1, fr_uv ).rgb, 0.0 );			// BG	
		vec4	fr			= cr_lut( big.ba+vec2(0xb0,0) ),
				fr_rgb	= texture2D( tex0, fr.zw ),
				cl			= cr_lut(big.rg),												
				cl_rgb	= texture2D( tex0, cl.zw );									
		if( big.b != 0 )															
			gl_FragColor += texture2D( tex0, fr.xy+big_in )*fr_rgb;					
		
		if( big.r != 0 )															
		{																			
			gl_FragColor += texture2D( tex0, cl.xy+big_in )*cl_rgb;					
		}																			
		gl_FragColor	+=	vec4( 0, big_in*0.25, 1.0 ) + big/0x200;	// BG		
																					
	}																																				
	
	
	char gpsSHDRvx[] =	
	"#version 120\n"
	"attribute	vec4	v4;\n"
	"varying		vec2	v_uv;\n"
	"void main()\n"
	"{\n"
	"	gl_Position		= vec4( v4.xy, 0.0, 1.0f );\n"
	"	v_uv				= v4.zw;\n"
	"}\n\0";
	
	char gpsSHDR[] =	
	"#version 120																	\n"
	"varying vec2 fr_uv;															\n"
	"uniform sampler2D tex0;						// MINI_CHAR_xXy_zXw.png		\n"
	"uniform sampler2D tex1;						// MINI 	ABGR?				\n"
	"													// U4		XYZW				\n"
	"													//			cFcA 				\n"
	"uniform sampler2D tex2;						// BackGround					\n"
	"uniform vec2 tgPX;																\n"
	"uniform vec2 xyPX;																\n"
	"uniform vec2 whPX;																\n"
	"uniform vec2 aTX[8];															\n"
	"vec4 cr_lut( vec2 ac )															\n"
	"{																				\n"
	"	ac /= vec2(8.0,4.0);														\n"
	"	return vec4( 																\n"
	"						floor( vec2(fract(ac.x)*8.0, ac.x) ), 	// char			\n"
	"						floor( vec2(fract(ac.y)*4.0, ac.y) )	// LUT		  	\n"
	"					)															\n"
	"			/ vec4(																\n"
	"						8.0,32.0,												\n"
	"						128.0,1024.0											\n"
	"					);															\n"
	"}																				\n"
	"void main()																	\n"
	"{																				\n"
	"	vec2	frm1 = fr_uv*whPX,													\n"
	"			big_in = fract(frm1)/aTX[0],										\n"
	"			frm0 = frm1/aTX[2];													\n"
	"	vec4	big = texture2D( tex2, frm0 )*0x100;								\n"
	"																				\n"
	"	gl_FragColor = vec4( texture2D( tex1, fr_uv ).rgb, 0.0 );			// BG	\n"
	"	vec4	fr			= cr_lut( big.ba+vec2(0xb0,0) ),\n"
	"			fr_rgb	= texture2D( tex0, fr.zw ),\n"
	"			cl			= cr_lut(big.rg),												\n"
	"			cl_rgb	= texture2D( tex0, cl.zw );									\n"
	"	if( big.b != 0 )															\n"
	"		gl_FragColor += texture2D( tex0, fr.xy+big_in )*fr_rgb;					\n"
	"	\n"
	"	if( big.r != 0 )															\n"
	"	{																			\n"
	"		gl_FragColor += texture2D( tex0, cl.xy+big_in )*cl_rgb;					\n"
	"	}																			\n"
	"	gl_FragColor	+=	vec4( 0, big_in*0.25, 1.0 ) + big/0x200;	// BG		\n"
	"																				\n"
	"}\n"