#version 150
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect positionTex;
uniform sampler2DRect velocityTex;
//uniform sampler2DRect trailTex;

out vec4 outputColor;

uniform float speedMul;

float random (vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
                 43758.5453123);
}

void main(void){
    vec2 st = gl_FragCoord.xy; // / vec2(512.0, 512.0);
    vec2 pos = texture2DRect(positionTex, st).xy; // ... in order to look at a particulary place on it
    vec2 vel = texture2DRect(velocityTex, st).xy;
    float mass = texture2DRect(positionTex, st).z;

    vel -= 0.5;
    vel *= 2.0;

    vel *= mass;
    vel /= 1000.;
    vel *= speedMul;

    pos += vel;
    pos = fract(pos);

    outputColor = vec4(pos.x, pos.y, mass, 1.0);
    // outputColor = vec4(1.0, 0.0, 0.0, 1.0);
}
