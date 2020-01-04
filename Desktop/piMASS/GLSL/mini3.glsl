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
	uniform sampler2D tex0;						// MINI 	ABGR?				
														// U4		XYZW				
														//			cFcA 				
	uniform sampler2D tex1;						// MINI_CHAR_xXy_zXw.png		
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
							floor( vec2(fract(ac.y)*4.0, ac.y) )		// LUT  
						)
				/ vec4(
							8.0,32.0,
							128.0,1024.0
						);
	}
	void main()																	
	{																				
		vec2	frm1 = fr_uv*whPX,
				big_in = fract(frm1),													
				frm0 = frm1/aTX[0];													
		vec4	big = texture2D( tex0, frm0 )*0x100;
		
		gl_FragColor	=	vec4( texture2D( tex2, fr_uv ).rgb, 0.0 );		// BG	

		vec4	cl = cr_lut(big.rg),
				l_rgb = texture2D( tex1, cl.zw );
		if( mini.r <= 0x60 )
		{
			gl_FragColor += l_rgb * texture2D( tex1, cl.xy+big_in );	
		}

		gl_FragColor	+=	vec4( 0, big_in*0.25, 1.0 );				// BG	
																					
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
	"#version 120\n"
	"varying vec2 v_uv;\n"
	"uniform sampler2D tex0;	// MINI 	ABGR?\n"
	"								// U4		XYZW\n"
	"								//			cFcA \n"
	"uniform sampler2D tex1;	// MINI_CHAR_xXy_zXw.png\n"
	"uniform sampler2D tex2;	// BackGround\n"
	"uniform vec2 tg;\n"
	"uniform vec2 xy;\n"
	"uniform vec2 wh;\n"
	"uniform vec2 aTX[0x10];\n"
	"void main()\n"
	"{\n"
	"	gl_FragColor	=	vec4( texture2D( tex2, v_uv ).rgb, 0.0 );		// BG\n"
	"	\n"
	"	\n"
	"}\n\0";