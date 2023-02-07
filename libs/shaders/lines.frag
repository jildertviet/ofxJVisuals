#version 150

uniform sampler2DRect tex0;

//uniform sampler2DRect tex1;
//uniform sampler2DRect tex2;
uniform vec2 size;
uniform float numLines;
uniform float lineWidth;
uniform float diagonalScale;
uniform float diagonalPow;

in vec2 texCoordVarying;

out vec4 fragColor;

const float HALF_PI = 1.570796327;

vec2 rotate(vec2 v, float a) {
    float s = sin(a);
    float c = cos(a);
    mat2 m = mat2(c, -s, s, c);
    return m * v;
}

vec2 rotateUV(vec2 uv, vec2 pivot, float rotation) {
    float sine = sin(rotation);
    float cosine = cos(rotation);

    uv -= pivot;
    uv.x = uv.x * cosine - uv.y * sine;
    uv.y = uv.x * sine + uv.y * cosine;
    uv += pivot;

    return uv;
}

mat2 rotate2d(float _angle){
    return mat2(cos(_angle),-sin(_angle),
                sin(_angle),cos(_angle));
}

void main (){
    vec2 pos = texCoordVarying;

    vec3 src = texture(tex0, pos).rgb;

//    vec3 vertical = texture(tex1, pos).rgb * src.r;
    vec3 vertical = vec3(step(1.0 - (numLines * 0.002 * lineWidth), fract((pos.x / size.x) * numLines)));
    vertical *= step(0.6, src.r); // Only > 0.5 gives
    vec3 horizontal = vec3(step(1.0 - (numLines * 0.002 * lineWidth), fract((pos.y / size.y) * numLines)));
    horizontal *= ceil(src.r) * (1.0 - step(0.6, src.r)); //
    vertical = vertical + horizontal;

    vec2 rotatedPos = rotate(pos, radians(45));
//    vec2 rotatedPos = rotateUV(pos, vec2(0.0), degrees(45.));
    rotatedPos.x = mod(rotatedPos.x, size.x);
    rotatedPos.y = mod(rotatedPos.y, size.y);
//    vec3 diagonal = texture(tex2, rotatedPos).rgb * src.g;

    // green 127 is -45, green 255 is 45
    float angle = round(src.g * 2.0) * 2.0; // 0<>2
    angle -= 1.0;
    angle *= 45.0;
    float y = rotate(pos, radians(angle)).y / (size.y); // Range 0<>1

//    y *= numLines;
//    y += 1000.0; // This removes an artifact...
//    y = fract(y);
//    y = step(1.0 - (numLines * 0.001 * lineWidth), y);
    y = sin(y * 50. * diagonalScale) * 0.5 + 0.5;
    y = pow(y, diagonalPow);
    y = step(0.5, y);
//    y =
    vec3 diagonal = vec3(y) * ceil(src.g);

    vec3 blackOrWhite = vec3(step(0.6, src.b)); // 0 or 1

    vec3 result = vertical + diagonal + blackOrWhite;

    // fragColor = vec4(result, 1.0);
    fragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
