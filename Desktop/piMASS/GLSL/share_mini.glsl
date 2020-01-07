char gpsSHDRvx[] =
	"#version 120										\n"
	"attribute	vec2	v_vx;							\n"
	"attribute	vec2	v_uv;							\n"
	"varying	vec2	fr_uv;							\n"
	"void main()										\n"
	"{																	\n"
	"	gl_Position			= vec4( v_vx*vec2(2.0,-2.0)+vec2(-1.0,1.0), 0, 1);		\n"
	"	fr_uv				= v_uv;										\n"
	"}													\n\0";
char gpsSHDR[] =
	"#version 120																	\n"
"varying vec2 fr_uv;															\n"
"uniform sampler2D tex0;						// MINI_CHAR_xXy_zXw.png		\n"
"uniform sampler2D tex1;						// BackGround					\n"
"uniform sampler2D tex2;						// MINI 	ABGR?				\n"
	"										// U4		XYZW				\n"
	"										//			cFcA \n"
"uniform sampler2D tex3;						// CUBE											\n"
"uniform vec2 tgPX;																\n"
"uniform vec2 xyPX;																\n"
"uniform vec2 whPX;																\n"
"uniform vec2 aTX[8];\n"
"vec2 ac_more( vec4 A )\n"
"{\n"
	"return A.ba+vec2(A.g*0x100,0);\n"
"}															\n"
"vec4 cr_lut( vec2 ac, vec3 crl, vec2 lwh ) 							// crl.xyz ColRowLut // lwh LutWH															\n"
"{																				\n"
	"ac /= crl.xz; 														\n"
	"return vec4( 																\n"
	"					floor( vec2(fract(ac.x)*crl.x, ac.x) ), 	// char			\n"
	"					floor( vec2(fract(ac.y)*crl.z, ac.y) )		// LUT		  	\n"
	"				)															\n"
	"		/ vec4(	crl.xy, lwh );															\n"
"}\n"
	"																			\n"
"void main()																	\n"
"{																				\n"
	"vec2	frm1 = fr_uv*whPX,													\n"
	"		Ain = fract(frm1)/aTX[0],										\n"
	"		frm0 = frm1/aTX[2],													\n"
	"		off0 = vec2( 1.0/4.0, 1.0/6.0 );								\n"
	"if( xyPX.x < 1 )						\n"
 "		off0.x = 0.0;						\n"
	"if( xyPX.y < 1 )						\n"
 "		off0.y = 0.0;						\n"
	"vec4	A = texture2D( tex2, frm0 + off0 )*0x100,\n"
	"		B;							\n"
	"																			\n"
	"gl_FragColor = vec4( texture2D( tex1, fr_uv ).rgb, 0 );			// BG	\n"
	"vec4	fr		= cr_lut( A.ba+vec2(0xb0,0), vec3(8, 32, 4), vec2( 128, 1024 ) ),						\n"
	"		fr_rgb	= texture2D( tex0, fr.zw ),									\n"
	"		cl		= cr_lut( A.rg, vec3(8, 32, 4), vec2( 128, 1024 ) ),										\n"
	"		cl_rgb	= texture2D( tex0, cl.zw );									\n"
	"if( A.b != 0 )															\n"
	"	gl_FragColor += texture2D( tex0, fr.xy+Ain )*fr_rgb;					\n"
	"																			\n"
	"if( A.r != 0 ) {																			\n"
	"	if( A.r <= 0x60 )															\n"
	"		B = texture2D( tex0, cl.xy+Ain )*cl_rgb;					\n"
	"	else {																		\n"
	"		cl.zw = (A.rr - vec2( 0x20, -0x20 ))/0x10;							\n"
	"																				\n"
	"		cl.z = texture2D( tex0,													\n"
	"										floor(vec2(fract(cl.z)*0x10, cl.z)) 	\n"
	"										/ vec2( 128, 1024 )					\n"
	"							).a;												\n"
	"		cl.w = texture2D( tex0,													\n"
	"										floor(vec2(fract(cl.w)*0x10, cl.w)) 	\n"
	"										/ vec2( 128, 1024 )					\n"
	"							).a;												\n"
	"		cl.zw *= 0x100;															\n"
	"		if( cl.z >= 0x41 &&  cl.z <= 0x5a )										\n"
	"			cl.w += 0x68;														\n"
	"		else																	\n"
	"			cl.w += 0x60;														\n"
	"																				\n"
	"		cl.zw /= 8;																\n"
	"		cl = vec4( 																			\n"
	"						floor(vec2(fract(cl.z)*0x8, cl.z)),									\n"
	"						floor(vec2(fract(cl.w)*0x8, cl.w))									\n"
	"					) / vec2( 8, 32 ).xyxy + Ain.xyxy;								\n"
	"		B = max( texture2D(tex0, cl.xy ), texture2D(tex0, cl.zw ) ) * cl_rgb;	\n"
	"	}\n"
	"	\n"
	"	if( B.a > 0.75 )\n"
	"	{\n"
	"		gl_FragColor = B;\n"
	"		return;\n"
	"	}\n"
	"	\n"
	"	gl_FragColor += B;\n"
	"}\n"
	"\n"
	"\n"
	"frm0 *= 2.0;	// frm0 = fr_uv * whPX*2 / aTX[2] \n"
	"off0 *= 2.0;\n"
	"off0.y += 1.0/3.0;\n"
	"vec2 off_in = 1.0/aTX[2];\n"
	"A = texture2D( tex2, frm0 + off0 )*0x100;\n"
	"B = texture2D( tex2, frm0 + off0 + off_in*vec2( -1, 0 ) )*0x100;\n"
	"		\n"
	"vec4	C = texture2D( tex2, frm0 + off0 + off_in*vec2(  0, 1 ) )*0x100,\n"
	"		D = texture2D( tex2, frm0 + off0 + off_in*vec2( -1, 1 ) )*0x100,\n"
	"		E = texture2D( tex2, frm0 + off0 + off_in*vec2(  0, 2 ) )*0x100,\n"
	"		F = texture2D( tex2, frm0 + off0 + off_in*vec2( -1, 2 ) )*0x100;\n"
	"frm1 *= 2.0;\n"
	"Ain = fract(frm1)/aTX[3];\n"
	"off_in = vec2(8, 8)/vec2(256, 1536);\n"
	"\n"
	"vec4	frnt = 0;\n"
	"cl		= cr_lut( ac_more(E), vec3(16, 32, 4), vec2( 128, 1024 ) );										\n"
	"E 		= texture2D( tex3, cl.xy+Ain+off_in*vec2( 0, -2 ));\n"
	"if( E.g*E.a > 0.75 )\n"
	"	frnt += E.ggga*texture2D( tex0, cl.zw );\n"
	"\n"
	"cl		= cr_lut( ac_more(B), vec3(16, 32, 4), vec2( 128, 1024 ) );										\n"
	"B		= texture2D( tex3, cl.xy+Ain+off_in*vec2( 1, 0 ));\n"
	"if( B.g*B.a > 0.75 )\n"
	"	frnt += B.ggga*texture2D( tex0, cl.zw );\n"
	"if( B.a >= E.a )	\n"
	"	E = B;\n"
	"	\n"
	"cl		= cr_lut( ac_more(C), vec3(16, 32, 4), vec2( 128, 1024 ) );										\n"
	"C		= texture2D( tex3, cl.xy+Ain+off_in*vec2( 0, -1 ));\n"
	"if( C.g*C.a > 0.75 )\n"
	"	frnt += C.ggga*texture2D( tex0, cl.zw );\n"
	"if( C.a >= E.a )	\n"
	"	E = C;\n"
	"\n"
	"cl		= cr_lut( ac_more(F), vec3(16, 32, 4), vec2( 128, 1024 ) );										\n"
	"F		= texture2D( tex3, cl.xy+Ain+off_in*vec2( 1, -2 ));\n"
	"if( F.g*F.a > 0.75 )\n"
	"	frnt += F.ggga*texture2D( tex0, cl.zw );\n"
	"if( F.a >= E.a )	\n"
	"	E = F;\n"
	"\n"
	"cl		= cr_lut( ac_more(A), vec3(16, 32, 4), vec2( 128, 1024 ) );										\n"
	"A 		= texture2D( tex3, cl.xy+Ain );\n"
	"if( A.g*A.a > 0.75 )\n"
	"	frnt += A.ggga*texture2D( tex0, cl.zw );\n"
	"if( F.a >= E.a )	\n"
	"	E = F;\n"
	"	\n"
	"cl		= cr_lut( ac_more(D), vec3(16, 32, 4), vec2( 128, 1024 ) );										\n"
	"D		= texture2D( tex3, cl.xy+Ain+off_in*vec2(-1, -1 ));\n"
	"if( D.g*D.a > 0.75 )\n"
	"	frnt += D.ggga*texture2D( tex0, cl.zw );\n"
	"if( D.a >= E.a )	\n"
	"	E = D;\n"
	"	\n"
	"gl_FragColor += frnt+E;\n"
	"\n"
"}\n";