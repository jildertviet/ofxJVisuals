#version 120

uniform sampler2DRect tex0;

varying vec2 texCoordVarying;

uniform float contrast;
uniform float brightness;

void main(){
    vec4 color = texture2DRect(tex0, texCoordVarying);
    // Apply contrast
    vec3 colorRGB = color.rgb;
    colorRGB = ((colorRGB.rgb - 0.5) * contrast) + 0.5;
    colorRGB = colorRGB + brightness;
    gl_FragColor = vec4(colorRGB.r, colorRGB.g, colorRGB.b, color.a);
}
