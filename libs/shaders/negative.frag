#version 120

uniform sampler2DRect tex0;
uniform sampler2DRect mask;
uniform float bDrawSource;

varying vec2 texCoordVarying;
varying vec4 globalColor;

void main()
{
    vec4 color = texture2DRect(tex0, texCoordVarying);
    vec4 maskColor = texture2DRect(mask, texCoordVarying);

    if(maskColor.rgb != vec3(0.0)){ //
        // if(color.rgb != vec3(0.0)){
        float alpha = mix(maskColor.a, color.a, 0.5);
        vec4 outColor = maskColor;
        if(color.rgb == vec3(0.0)){
            outColor = maskColor;
        } else{
            outColor = vec4(1.0 - vec3(color), alpha);
        }

        // outColor = maskColor;
          // if(bDrawSource == 1){
            // outColor = mix(outColor, maskColor, 0.5);
          // }
        // gl_FragColor = outColor;
        // } else{
            gl_FragColor = color;
        // }
    } else{
        gl_FragColor = color;
    }

    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
