#version 120

uniform sampler2DRect tex0;
uniform sampler2DRect mask;

varying vec2 texCoordVarying;
varying vec4 globalColor;

void main()
{
    vec4 color = texture2DRect(tex0, texCoordVarying);
    vec4 maskColor = texture2DRect(mask, texCoordVarying);
    
    gl_FragColor = texel0;
}
