
uniform float time;
uniform sampler2DRect img0;
uniform sampler2DRect img1;
uniform sampler2DRect img2;
uniform sampler2DRect img3;
uniform sampler2DRect img4;

float map(float value, float min1, float max1, float min2, float max2) {
    return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

void main() {
    vec2 pos = gl_FragCoord.xy;
    pos.y = 768.0 - pos.y;
    vec3 color = texture2DRect(img0, pos).rgb;

    float v = map(pos.y, 0.0, 768.0, 0.0, 5.0);
    int idx = int(v);

    if (idx == 0) color = texture2DRect(img4, pos).rgb;
    if (idx == 1) color = texture2DRect(img3, pos).rgb;
    if (idx == 2) color = texture2DRect(img2, pos).rgb;
    if (idx == 3) color = texture2DRect(img1, pos).rgb;
    if (idx == 4) color = texture2DRect(img0, pos).rgb;

    gl_FragColor = vec4(color, 1.0);
}
