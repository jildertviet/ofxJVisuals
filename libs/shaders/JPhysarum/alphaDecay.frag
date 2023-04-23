#version 150
precision highp float;

uniform sampler2DRect src;
uniform float decay;
out vec4 outColor;

void main(){
    vec2 st = gl_FragCoord.xy;
    vec4 color = texture2DRect(src, st);

    color.a *= decay;

    outColor = vec4(color);
}
