#version 120

uniform sampler2DRect src;
//varying vec2 texCoordVarying;
uniform float decay;

void main(){
    vec2 st = gl_TexCoord[0].st;
    vec4 color = texture2DRect(src, st);

//    color.a = 0.0;
    float a = color.a;
    a *= decay;
        
//    a -= 0.01;
    
//    if(color.a < 0.0)
//        color.a = 0.0;
    
//    color = vec4(color.r, color.g, color.b, color.a);
//    color = vec4(1.0, 1.0, 1.0, 1.0);
    gl_FragColor = vec4(color.rgb, a);
}
