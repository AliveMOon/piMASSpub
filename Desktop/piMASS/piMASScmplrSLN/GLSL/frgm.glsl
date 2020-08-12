#version 120																
varying vec2 fr_uv;														
uniform sampler2D	tex0;					// MINI_CHAR_xXy_zXw.png		
uniform vec4 		aCNL[8];				// CNL							
void main()																
{																		
	gl_FragColor = texture2D( tex0, fr_uv )*aCNL[0];					
	if( gl_FragColor.a < (1/0x100) )									
 		discard;														
}																			