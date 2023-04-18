#version 150

uniform vec4 globalColor;
uniform float brightness;
uniform float contrast;
uniform float saturation;

out vec4 outputColor;

void main(){
    vec4 color = globalColor;
    color.rgb += brightness;
    color.rgb = (color.rgb - 0.5) * contrast + 0.5;

    float average = (color.r + color.g + color.b) / 3.0;
    color.rgb += (average - color.rgb) * (1.0 - saturation);

    outputColor = color;
}
