#version 120

uniform sampler2DRect tex0;
uniform sampler2DRect mask;

varying vec2 texCoordVarying;
varying vec4 globalColor;

void main()
{
    vec4 color = texture2DRect(tex0, texCoordVarying);
    vec4 maskColor = texture2DRect(mask, texCoordVarying);
    
    if(maskColor.rgb != vec3(0.0)){ //
        if(color.rgb != vec3(0.0)){
            gl_FragColor = vec4(1.0 - vec3(color), color.a);
        } else{
            gl_FragColor = color;
        }
    } else{
        gl_FragColor = color;
    }
}
