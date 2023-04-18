#version 120
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect positionTex;
uniform sampler2DRect velocityTex;
//uniform sampler2DRect trailTex;

uniform float speedMul;

float random (vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
                 43758.5453123);
}

void main(void){
    vec2 st = gl_TexCoord[0].st;    // gets the position of the pixel that it´s dealing with...
    vec2 pos = texture2DRect(positionTex, st).xy; // ... in order to look at a particulary place on it
    vec2 vel = texture2DRect(velocityTex, st).xy;
    float mass = texture2DRect(positionTex, st).z;
    
    vel -= 0.5;
    vel *= 2.0;
    
    vel *= mass;
    vel /= 1000.;
    vel *= speedMul;

    pos += vel;
//    pos = min(pos, 1.0);
    pos = fract(pos);
    gl_FragColor = vec4(pos.x, pos.y, mass, 1.0);
}

