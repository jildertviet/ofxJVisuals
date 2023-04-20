#version 150

uniform vec4 globalColor;
out vec4 outputColor;
in vec4 vertexColor;

void main(){
    vec4 color = globalColor;
    outputColor = vertexColor;
}
