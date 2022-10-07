#version 120

uniform sampler2DRect tex0;
uniform sampler2DRect mask;

varying vec2 texCoordVarying;
varying vec4 globalColor;

void main()
{
    vec4 texel0 = texture2DRect(tex0, texCoordVarying);
    vec4 texel1 = texture2DRect(mask, texCoordVarying * 0.5);
    if(texel0.rgb == vec3(0., 0., 0.)){
        texel1.r = 0.;
    }
    vec4 e = vec4(texel0.rgb, texel1.r);
    
    gl_FragColor = e;
}
