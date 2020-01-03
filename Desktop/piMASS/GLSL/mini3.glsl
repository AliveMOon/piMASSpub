char gpsSHDRvx[] =	
	#version 120
	attribute	vec4	v4;
	varying		vec2	v_uv;
	void main()
	{
		gl_Position		= vec4( v4.xy, 0.0, 1.0f );
		v_uv				= v4.zw;
	}
	
char gpsSHDR[] =	
	#version 120
	varying vec2 v_uv;
	uniform sampler2D tex0;	// MINI 	ABGR?
									// U4		XYZW
									//			cFcA 
	uniform sampler2D tex1;	// MINI_CHAR_xXy_zXw.png
	uniform sampler2D tex2;	// BackGround
	uniform vec2 xy;
	uniform vec4 wh;
	uniform vec2 aTX[0x10];
	void main()
	{
		gl_FragColor	=	vec4( texture2D( tex2, v_uv ).rgb, 0.0 );		// BG
		
		
	}