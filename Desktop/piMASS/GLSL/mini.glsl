	#version 120\n"
	attribute vec2 v_vx;
	varying vec2 v_uv;
	void main()
	{
		gl_Position = vec4( v_vx, 0.0, 1.0f );
		v_uv = v_vx*vec2(0.5,-0.5) + 0.5 ;
	}
	
	
	
	
char gpsSHDR[] =	
	#version 120
	varying vec2 v_uv;
	uniform sampler2D tex0;	// MINI 	ABGR?
									// U4		XYZW
									//			cFcA 
									
	uniform sampler2D tex1;	// MINI_CHAR_xXy_zXw.png
	uniform sampler2D tex2;	// BackGround
	uniform vec2 aFRM[4];
	uniform vec4 divXYwh;
	uniform vec2 txWH;
	void main()
	{
		
		gl_FragColor		= vec4( texture2D( tex2, v_uv ).rgb, 0.0 );		// BG	

		// txMINI
		
		
		vec2	divUV	= v_uv*divXYwh.zw;
				q 		= floor( divUV );
		int	i 		= dot( q, vec2(1,2) ); 
		vec2 	qUV	= q*aFRM[i],
				qUVf	= fract(qUV)/vec2(8.0,32.0);									
		vec4	mini	= texture2D( tex0, (qUV/txWH) + q*0.25 )*0x100;					
		
		// FRAME																
		vec2	fc 	= (mini.ba + vec2( 0xb0, 0 ))/vec2(8.0,4.0),			// 0xb0  176	11*16 22*8
				f_uv =	vec2(
								floor(fract(fc.x)*8.0)/8.0,
								floor(fc.x)/32.0
							)
						+ qUVf,
				c_uv = vec2(
								floor(fract(fc.y)*4.0)/128.0,
								floor(fc.y)/1024.0
							);
		
		gl_FragColor +=	min( mini.b, 1 )										// hátha b == 0 akor 
																					// a töbit már nem csinálja
								* texture2D(tex1, c_uv )						// FRM color
								* texture2D(tex1, f_uv );						// FRM

		// CHAR
		vec2	ac = mini.rg/vec2(8.0,4.0),
				a_uv =	vec2( 														
								floor(fract(ac.x)*8.0)/8.0,						
								floor(ac.x)/32.0									
							) + qUVf;
		c_uv = vec2(
						floor(fract(ac.y)*4.0)/128.0,
						floor(ac.y)/1024.0
					);
		vec4 c = texture2D(tex1, c_uv );
		
		if( mini.r <= 0x60 )
		{
			gl_FragColor += min( mini.r, 1 ) * texture2D(tex1, a_uv ) * c;
			return;
		}
		
		ac = (mini.rr - vec2( 0x20, -0x20 ))/0x10;					
		a_uv =	vec2( 													
						floor(fract(ac.x)*0x10)/128.0,					
						floor(ac.x)/1024.0									
					);															
		c_uv =	vec2( 													
						floor(fract(ac.y)*0x10)/128.0,						
						floor(ac.y)/1024.0									
					);
		ac.x = texture2D(tex1, a_uv ).a;
		ac.y = texture2D(tex1, c_uv ).a;
		ac *= 0x100;
		ac.y += 0x60;
		if( ac.x >= 0x41 &&  ac.x <= 0x5a )
			ac.y += 8;
		ac /= 8.0;
	
		a_uv =	vec2(
								floor(fract(ac.x)*8.0)/8.0,
								floor(ac.x)/32.0
							)
						+ qUVf;
		c_uv =	vec2(
								floor(fract(ac.y)*8.0)/8.0,
								floor(ac.y)/32.0
							)
						+ qUVf;
						
		gl_FragColor += max( texture2D(tex1, a_uv ), texture2D(tex1, c_uv ) ) * c;
	}

char gpsSHDRvx[] =
	"#version 120\n"
	"attribute vec2 v_vx;\n"
	"varying vec2 v_uv;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4( v_vx, 0.0, 1.0f );\n"
	//"	v_uv = (v_vx+1.0)/2.0;\n"
	"	v_uv = v_vx*vec2(0.5,-0.5) + 0.5 ;\n"
	"}\n\0";

char gpsSHDRfr_ref[] =
	"#version 120																		\n"
	"varying vec2 v_uv;																	\n"
	"uniform sampler2D tex0;															\n"
	"uniform sampler2D tex1;															\n"
	"uniform sampler2D tex2;															\n"
	"uniform vec2 crsFRxy;																\n"
	"																					\n"
	"void main()																		\n"
	"{																					\n"
	"	vec4 mini = texture2D(tex0, v_uv)*256.0;										\n"
	"	float 	c = mini.r,			cc = mini.g, 										\n"
	"			f = mini.b+0xb0,	fc = mini.a; 										\n"
	"	vec2 	uv = fract(v_uv*crsFRxy)/vec2(8.0,32.0),								\n"
	"			f_uv =	vec2( 															\n"
	"							floor(fract(f/8.0)*8.0)/8.0,							\n"
	"							floor(f/8.0)/32.0										\n"
	"						) 															\n"
	"					+ uv, 															\n"
	"			fc_uv = vec2( 															\n"
	"							floor(fract(fc/4.0)*4.0)/128.0,							\n"
	"							floor(fc/4.0)/1024.0									\n"
	"						),															\n"
	"			c_uv =	vec2( 															\n"
	"							floor(fract(c/8.0)*8.0)/8.0,							\n"
	"							floor(c/8.0)/32.0										\n"
	"						) 															\n"
	"					+ uv, 															\n"
	"			cc_uv = vec2( 															\n"
	"							floor(fract(cc/4.0)*4.0)/128.0,							\n"
	"							floor(cc/4.0)/1024.0									\n"
	"						);															\n"
	"																					\n"
	"	gl_FragColor =	 																\n"
	"					min( f, 1 ) * texture2D(tex1, f_uv ) * texture2D(tex1, fc_uv )	\n"
	"					+min( c, 1 ) * texture2D(tex1, c_uv ) * texture2D(tex1, cc_uv )	\n"
	"					+vec4( texture2D( tex2, v_uv ).rgb, 0.0 );						\n"
	"}	\n";

		
	
	
	/*#version 120
	varying vec2 v_uv;
	uniform sampler2D tex0;
	uniform sampler2D tex1;
	uniform sampler2D tex2;
	void main()
	{
		vec4 mini = texture2D(tex0, v_uv)*256.0;
		float 	c = mini.r,		cc = mini.g, 
				f = mini.b+176,	fc = mini.a; 
		vec2 	uv = vec2( fract(v_uv.x*93.0)/8.0, fract(v_uv.y*60.0)/32.0 ),		
				f_uv =	vec2( 														
								floor(fract(f/8.0)*8.0)/8.0,						
								floor(f/8.0)/32.0									
							) 														
						+ uv, 
				fc_uv = vec2( 														
								floor(fract(fc/4.0)*4.0)/96.0,						
								floor(fc/4.0)/512.0									
							),
				c_uv =	vec2( 														
								floor(fract(c/8.0)*8.0)/8.0,						
								floor(c/8.0)/32.0									
							) 														
						+ uv, 
				cc_uv = vec2( 														
								floor(fract(cc/4.0)*4.0)/96.0,						
								floor(cc/4.0)/512.0									
							);														
																					
		gl_FragColor =	 															
						min( f, 1 ) * texture2D(tex1, f_uv ) * texture2D(tex1, fc_uv )		
						+min( c, 1 ) * texture2D(tex1, c_uv ) * texture2D(tex1, cc_uv )		
						+vec4( texture2D( tex2, v_uv ).rgb, 0.0 );											
	}*/

char gpsSHDR[] = 
	"#version 120\n"
	"varying vec2 v_uv;\n"
	"uniform sampler2D tex0;	// MINI 	ABGR?\n"
	"								// U4		XYZW\n"
	"								//			cFcA \n"
	"								\n"
	"uniform sampler2D tex1;	// MINI_CHAR_xXy_zXw.png\n"
	"uniform sampler2D tex2;	// BackGround\n"
	"uniform vec2 crsFRxy;\n"
	"uniform vec4 divXYwh;\n"
	"uniform vec2 txWH;\n"
	"void main()\n"
	"{\n"
	"	\n"
	"	gl_FragColor =	vec4( texture2D( tex2, v_uv ).rgb, 0.0 );		// BG	\n"
	"	vec2	big 	= crsFRxy,												\n"
	"			divUV 	= v_uv*big*divXYwh.zw;									\n"
	"	if( max( divUV.x / big.x, divUV.y/ big.y ) > 1.0 ) return; 				\n"
	"	vec4	mini	= texture2D( tex0, divUV/txWH )*0x100;					\n"
	"	// FRAME																\n"
	"	vec2	fc 	= (mini.ba + vec2( 0xb0, 0 ))/vec2(8.0,4.0),			// 0xb0  176	11*16 22*8\n"
	"			uv 	= fract(divUV)/vec2(8.0,32.0),\n"
	"			f_uv =	vec2(\n"
	"							floor(fract(fc.x)*8.0)/8.0,\n"
	"							floor(fc.x)/32.0\n"
	"						)\n"
	"					+ uv,\n"
	"			c_uv = vec2(\n"
	"							floor(fract(fc.y)*4.0)/128.0,\n"
	"							floor(fc.y)/1024.0\n"
	"						);\n"
	"	\n"
	"	gl_FragColor +=	min( mini.b, 1 )										// hátha b == 0 akor \n"
	"																				// a töbit már nem csinálja\n"
	"							* texture2D(tex1, c_uv )						// FRM color\n"
	"							* texture2D(tex1, f_uv );						// FRM\n"

	"	// CHAR\n"
	"	vec2	ac = mini.rg/vec2(8.0,4.0),\n"
	"			a_uv =	vec2( 														\n"
	"							floor(fract(ac.x)*8.0)/8.0,						\n"
	"							floor(ac.x)/32.0									\n"
	"						) + uv;\n"
	"	c_uv = vec2(\n"
	"					floor(fract(ac.y)*4.0)/128.0,\n"
	"					floor(ac.y)/1024.0\n"
	"				);\n"
	"	vec4 c = texture2D(tex1, c_uv );\n"
	"	\n"
	"	if( mini.r <= 0x60 )\n"
	"	{\n"
	"		gl_FragColor += min( mini.r, 1 ) * texture2D(tex1, a_uv ) * c;\n"
	"		return;\n"
	"	}	\n"
	"	\n"
	"	ac = (mini.rr - vec2( 0x20, -0x20 ))/0x10;\n"
	"	a_uv =	vec2( 														\n"
	"					floor(fract(ac.x)*0x10)/128.0,						\n"
	"					floor(ac.x)/1024.0									\n"
	"				);\n"
	"	c_uv =	vec2( 														\n"
	"					floor(fract(ac.y)*0x10)/128.0,						\n"
	"					floor(ac.y)/1024.0									\n"
	"				);\n"
	"	ac.x = texture2D(tex1, a_uv ).a;\n"
	"	ac.y = texture2D(tex1, c_uv ).a;\n"
	"	ac *= 0x100;\n"
	"	ac.y += 0x60;\n"
	"	if( ac.x >= 0x41 &&  ac.x <= 0x5a )\n"
	"		ac.y += 8;\n"
	"	ac /= 8.0;\n"
	"	\n"
	"	a_uv =	vec2(\n"
	"							floor(fract(ac.x)*8.0)/8.0,\n"
	"							floor(ac.x)/32.0\n"
	"						)\n"
	"					+ uv;\n"
	"	c_uv =	vec2(\n"
	"							floor(fract(ac.y)*8.0)/8.0,\n"
	"							floor(ac.y)/32.0\n"
	"						)\n"
	"					+ uv;\n"
	"					\n"
	"	gl_FragColor += max( texture2D(tex1, a_uv ), texture2D(tex1, c_uv ) ) * c;\n"
	"}	\n"
	
	
	
	
	