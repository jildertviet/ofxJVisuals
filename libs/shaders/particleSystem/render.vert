#version 120
#extension GL_EXT_gpu_shader4 : enable
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect posTex;
uniform sampler2DRect alpha;
uniform vec2 screen;

void main() {
    // use the texture coordinates as an index into the position texture
    vec2 st = gl_MultiTexCoord0.xy;

    // read position data from texture
    vec2 pos = texture2DRect(posTex, st).xy;
    float a = texture2DRect(alpha, st).b;
    vec2 vel = texture2DRect(alpha, st).xy;
//    float g = texture2DRect(alpha, st).y;

    // Maps the position from the texture (from 0.0 to 1.0) to
    // the screen position (0 - screenWidth/screenHeight)

//    pixPos -= vec2(0.5);
//    pixPos *= 2.0;

    gl_PointSize = 1.0;
    gl_Position = gl_ModelViewProjectionMatrix * vec4(pos.x * screen.x, pos.y * screen.y, 0., 1.0);
    gl_FrontColor = vec4(1.0, 1.0, 1.0, a);
//    v_vertex_color =  vec4(1.0, 0., 0., 1.0);
}
