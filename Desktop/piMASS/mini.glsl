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
	void main()
	{
		vec4 mini = floor(texture2D(tex0, v_uv)*255.0);
		vec2 cuv =	vec2( 
								floor(fract(mini.a/8.0)*8.0)/8.0,
								floor(mini.a/8.0)/32.0
							) 
					+	vec2( fract(v_uv.x*93.0)/8.0, fract(v_uv.x*60.0)/32.0 );
		
		gl_FragColor =	max( texture2D(tex0, v_uv),
							 texture2D(tex1, cuv) );
	}
	
	
	"#version 120\n"
	"varying vec2 v_uv;\n"
	"uniform sampler2D tex0;\n"
	"uniform sampler2D tex1;\n"
	"void main()\n"
	"{\n"
	"	vec4 mini = floor(texture2D(tex0, v_uv)*255.0);\n"
	"	vec2 cuv =	vec2( \n"
	"							floor(frac(mini.a/8.0)*8.0)/8.0,\n"
	"							floor(mini.a/8.0)/32.0\n"
	"						) \n"
	"				+	vec2( frac(v_uv.x*93.0)/8.0, frac(v_uv.x*60.0)/32.0 );\n"
	"	\n"
	"	gl_FragColor =	max( texture2D(tex0, v_uv),\n"
	"						 texture2D(tex1, cuv) );\n"
	"}\n";