#version 150

uniform sampler2DRect tex0;
uniform sampler2DRect mask;
uniform float bDrawSource;

in vec2 texCoordVarying;
in vec4 globalColor;

// out vec4 outputColor;

void main() {
    vec4 color = texture(tex0, texCoordVarying);
    vec4 maskColor = texture(mask, texCoordVarying);

    if (maskColor.rgb != vec3(0.0)) {
        float alpha = mix(maskColor.a, color.a, 0.5);
        vec4 outColor = maskColor;
        if (color.rgb == vec3(0.0)) {
            outColor = maskColor;
        } else {
            outColor = vec4(1.0 - vec3(color), alpha);
        }
        gl_FragColor = outColor;
    } else {
        gl_FragColor = color;
    }
}
