#version 150

uniform vec2 iResolution;
uniform float iTime;

uniform float bus_0;
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
    vec2 uv = gl_FragCoord.xy / iResolution.xy;

    // Define points A and B
    vec2 A = vec2(bus_0, bus_1);
    vec2 B = vec2(bus_2, bus_3);

    // Define colors for points A and B
    vec3 colorA = vec3(bus_4, bus_5, bus_6);
    vec3 colorB = vec3(bus_7, bus_8, bus_9);
    // vec3 colorB = vec3(1.0, 0.0, 0.0);

    // Calculate the rectangle bounds
    vec2 minPoint = min(A, B);
    vec2 maxPoint = max(A, B);

    // Check if the fragment is within the rectangle bounds
    if (uv.x >= minPoint.x && uv.x <= maxPoint.x && uv.y >= minPoint.y && uv.y <= maxPoint.y) {
        // Calculate the interpolation factor
        float factor = (uv.x - minPoint.x) / (maxPoint.x - minPoint.x);
        // Interpolate color based on the factor
        vec3 color = mix(colorA, colorB, factor);
        gl_FragColor = vec4(color, 0.7);
    } else {
        gl_FragColor = vec4(0.0); // Outside the rectangle
    }
}

