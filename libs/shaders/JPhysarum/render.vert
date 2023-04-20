#version 150

uniform mat4 modelViewMatrix;

in vec2 texcoord;
in vec4 color;
out vec4 vertexColor;

uniform sampler2DRect posTex;
uniform sampler2DRect alpha;
uniform vec2 screen;

void main() {
    // Read position data from texture.
    vec4 pixPos = texture(posTex, texcoord);
    float a = texture(alpha, texcoord).b;

    pixPos *= 2.f;
    pixPos -= 1.f;
    // Map the position from the texture (from 0.0 to 1.0) to
    // the screen position (0 - screenWidth/screenHeight)
    // pixPos.x *= 2;
    // pixPos.x -= 1;
    // pixPos.y *= screen.y;

    gl_Position = vec4(pixPos.x, pixPos.y, 0.0, 1.0);
    vertexColor = vec4(1.0, 1.0, 1.0, a);
}
