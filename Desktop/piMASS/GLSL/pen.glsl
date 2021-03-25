 /* if(length(aTX[3]) > 0) {
                        vec2 RG = ( gl_FragColor.rg*vec2( 8.0/20.0, 8.0/30.0) + vec2(1.0/2.0,2.0/3.0) )
                                  *vec2(1.0,-1.0) + vec2(0.0,1.0),
                             BG = ( gl_FragColor.bg*vec2(-8.0/20.0,-8.0/30.0) + vec2(1.0/2.0,2.0/3.0) )
                                  *vec2(1.0,-1.0) + vec2(0.0,1.0),
                             BR = ( gl_FragColor.br*vec2(-8.0/20.0, 8.0/30.0) + vec2(1.0/2.0,2.0/3.0) )
                                  *vec2(1.0,-1.0) + vec2(0.0,1.0);
                        vec3 RGB =  texture2D( tex3, RG ).rgb
                                   +texture2D( tex3, BG ).rgb
                                   +texture2D( tex3, BR ).rgb;
                        gl_FragColor.rgb += RGB/3.0;
                   } */ 
