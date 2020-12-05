#version 120																
varying vec2 fr_uv;														
uniform sampler2D	tex0,	// cam1
			tex1;	// mask // pena
uniform vec2		tgPX;					
uniform vec2		aTX[8];		
uniform vec4 		aCNL[8];
#define v2base	vec2(20.0, 28.0)
#define v2out	vec2(1.0, 10.0/28.0)
#define v2rg	vec2( 8.0, 8.0)
#define v2bg	vec2(-8.0, 8.0)
#define v2br	vec2(-8.0,-8.0)
#define v2spc	vec2(10.0,20.0)

#define v2rgB	(vec2( 8.0, 8.0)/v2base)
#define v2bgB	(vec2(-8.0, 8.0)/v2base)
#define v2brB	(vec2(-8.0,-8.0)/v2base)
#define v2spcB	(vec2(10.0,20.0)/v2base)

void main()																
{
	vec2	fUV = fr_uv*vec2(1,-1)+vec2(0,1);
	
	gl_FragColor = vec4( texture2D( tex1, fUV ).rgb, 1.0 );	// mask
	vec3	src = vec3(0,0,0),
		spc = vec3(0,0,0);
	
	if( fUV.y < v2out.y )
	{
		vec2	sUV = fUV/v2out,
			Ouv = floor(sUV*4),
			Suv = floor(sUV*vec2(320,160));
		src	= (
				floor(texture2D( tex0, sUV ).rgb*64.0)*4.0
				+ Ouv
			)/255.0;
		spc	= texture2D( tex1, (src.rg*v2rg+v2spc)/v2base ).rgb
			* texture2D( tex1, (src.bg*v2bg+v2spc)/v2base ).rgb
			* texture2D( tex1, (src.br*v2br+v2spc)/v2base ).rgb;
		if( length(spc-vec3(1,0,0))<1.0 )
 			spc = vec3(3,0,0);
 		else if( length(spc-vec3(0,1,0))<1.0 )
 			spc = vec3(0,3,0);
 		else if( length(spc-vec3(0,0,1))<1.0 )
 			spc = vec3(0,0,3);
 		else
 			spc = vec3(0,0,0);
 		
 		gl_FragColor.rgb = max( spc, gl_FragColor.rgb );
	}
	
	
	
	vec2 crs = abs( flpUV*tgPX - aCNL[0].xy );
	gl_FragColor.b += 1 - min( 1, min(abs(crs.x),abs(crs.y)) );
}