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
char gpsSHDRfr[] =
		#version 120
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
	}		
	
	
	#version 120
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
	}	
	
	
	
	
	