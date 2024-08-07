#version 150

uniform vec2 iResolution;
uniform float iTime;

uniform float bus_0; // is this reserved?
uniform float bus_1;
uniform float bus_2;
uniform float bus_3;
uniform float bus_4;
uniform float bus_5;
uniform float bus_6;
uniform float bus_7;
uniform float bus_8;
uniform float bus_9;

void main() {
    vec4 col = vec4(0.0);
    if(gl_FragCoord.y > iResolution.y * bus_0){
      col = vec4(bus_1, bus_2, bus_3, 0.5);
    }
    gl_FragColor = col;
    // gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}

