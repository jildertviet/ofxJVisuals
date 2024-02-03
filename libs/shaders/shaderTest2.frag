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
    vec4 bgColor = vec4(0.0);

    // Draw a grid of white horizontal lines centered on the screen
    float numLines = bus_1; // Adjust the number of lines // 10
    float lineSpacing = iResolution.y / numLines; // Adjust the spacing between lines
    float lineWidth = 0.02 * iResolution.y; // Adjust the line width as needed
    float centerY = 0.5 * iResolution.y; // Center of the screen vertically

    // Rotation based on time
    float rotationSpeed = bus_2; // Adjust the rotation speed 0.5
    float time = iTime * rotationSpeed * 0.001;
    mat2 rotationMatrix;

    // Apply rotation to coordinates
    vec2 rotatedCoords;

    if (distance(gl_FragCoord.xy, iResolution.xy * 0.5) < 0.3 * iResolution.y) {
        rotationMatrix = mat2(cos(time), -sin(time), sin(time), cos(time));
        rotatedCoords = rotationMatrix * (gl_FragCoord.xy - vec2(0.5 * iResolution.xy));
    } else {
        rotationMatrix = mat2(sin(time), -cos(time), cos(time), sin(time));
        rotatedCoords = rotationMatrix * (gl_FragCoord.xy - vec2(0.5 * iResolution.xy));
    }

    float lineIndex = floor((rotatedCoords.y - centerY) / lineSpacing);
    float distanceToLine = abs(rotatedCoords.y - (centerY + lineIndex * lineSpacing));

    // Smoothstep for a smoother transition
    float smoothness = bus_3; // Adjust the smoothness // 40
    float smoothStepValue = smoothstep(0.5 * lineWidth - smoothness, 0.5 * lineWidth + smoothness, distanceToLine);

    gl_FragColor = mix(vec4(1.0, 1.0, 1.0, 1.0), bgColor, smoothStepValue);
}

