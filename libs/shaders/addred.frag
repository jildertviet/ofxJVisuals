#version 150

uniform sampler2DRect tex0;

in vec2 texCoordVarying;
in vec4 globalColor;

out vec4 outputColor;

void main() {
    vec4 color = texture(tex0, texCoordVarying);
    color.r += 0.4;
    gl_FragColor = color;
}
