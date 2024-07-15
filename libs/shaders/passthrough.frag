#version 150

uniform sampler2DRect tex0;

in vec2 texCoordVarying;
in vec4 globalColor;

out vec4 outputColor;

void main() {
    vec4 color = texture(tex0, texCoordVarying);
    // color = vec4(vec3(1.0) - (color.rgb), 1.0);
    gl_FragColor = color;
}
