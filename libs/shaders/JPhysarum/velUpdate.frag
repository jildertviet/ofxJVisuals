#version 150
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect velocityTex;
uniform sampler2DRect positionTex;
uniform sampler2DRect trailTex;
uniform sampler2DRect externalVelocity;

uniform float sensorDistance;
uniform float sensorAngle;
uniform float turnAngle;
uniform float speedMul;
uniform vec2 resolution;
uniform float depositAmount;
uniform float time;
uniform float balance;
uniform float blurMix;
uniform int bExternalVelocity;

in vec2 texCoord;

float random (vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
                 43758.5453123);
}

vec2 rotate(vec2 v, float a) {
    float s = sin(a);
    float c = cos(a);
    mat2 m = mat2(c, -s, s, c);
    return m * v;
}

uniform vec2 offsets[9] = vec2[9](
    vec2(-1., -1.), vec2(0., -1.), vec2(1., -1.),
    vec2(-1., 0.), vec2(0., 0.), vec2(1., 0.),
    vec2(-1., 1.), vec2(0., 1.), vec2(1., 1.)
                          );

void main(void){
    // vec2 st = gl_TexCoord[0].st;
    vec2 st = gl_FragCoord.xy;
    vec2 pos = texture2DRect(positionTex, st).xy;
    vec2 vel = texture2DRect(velocityTex, st).xy;
    float mass = texture2DRect(positionTex, st).z;
    vec2 prevVel = vel;
    vel -= 0.5;
    vel *= 2.; // -1, 1 range now
    float alpha = 0.;

    float sensorDistanceScaled = sensorDistance * (1. + mass);

    vec3 sensorValues = vec3(0.0, 0.0, 0.0);
    vec2 rotatedVelocity = rotate(normalize(vel), radians(sensorAngle));
    vec2 rotatedVelocityNegative = rotate(normalize(vel), radians(-sensorAngle));

    for(int i=0; i<9; i++){
        sensorValues[1] += texture2DRect(trailTex,  (vec2(resolution * pos) + (normalize(vel) * sensorDistanceScaled)) +          offsets[i].xy).a;
        sensorValues[0] += texture2DRect(trailTex,   (vec2(resolution * pos) + (rotatedVelocity * sensorDistanceScaled)) +         offsets[i].xy).a;
        sensorValues[2] += texture2DRect(trailTex,   (vec2(resolution * pos) + (rotatedVelocityNegative * sensorDistanceScaled)) + offsets[i].xy).a;
    }

    sensorValues /= 9.;

    if(sensorValues[0]  < sensorValues[1] && sensorValues[1] > sensorValues[2]){
        alpha = depositAmount/255.;
//        alpha = 1.0;
    } else if(sensorValues[0] > sensorValues[1] && sensorValues[1] < sensorValues[2]){
        if(random(vec2(time+st.x)) > 0.5){
            vel = rotate(normalize(vel), radians(turnAngle));
        } else{
            vel = rotate(normalize(vel), radians(-turnAngle));
        }
    } else if(sensorValues[0] < sensorValues[1] && sensorValues[1] < sensorValues[2]){
        vel = rotate(normalize(vel), radians(turnAngle));
    } else if(sensorValues[0] > sensorValues[1] && sensorValues[1] > sensorValues[2]){
        vel = rotate(normalize(vel), radians(-turnAngle));
    }

    if(bExternalVelocity == 1){
        vec2 extForce = texture2DRect(externalVelocity, resolution * pos * 0.1).xy; // 0.1 for scale @ vecF
        extForce -= 0.5; // 0 <> 1 --> -0.5 <> 0.5
        extForce *= 2.; // -1 <> 1
        extForce *= 0.7;
        vel = mix(vel * 2., extForce, 0.5);
        vel *= 0.5;

        vel /= 2.;
        vel += 0.5;
    } else{
        vel /= 2.;
        vel += 0.5;
    }

    if(random(vec2(1.0)) > 0.1){
      alpha += (1.0 / 255.0);
    }

//    balance = balance * mass;
    vel = (prevVel * balance) + (vel * (1.-balance));

    gl_FragColor = vec4(vel.x, vel.y, alpha, 1.0);
}
