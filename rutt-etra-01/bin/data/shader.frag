
uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
uniform sampler2DRect img;

void main() {
    vec2 uv = gl_FragCoord.xy;
    uv.y = 1080. - uv.y;

    float amount = 0.0;

    amount = (1.0 + sin(time * 6.0) * 0.5);
    amount *= (1.0 + sin(time * 7.0) * 0.5) * 1.2;
    amount *= (1.0 + sin(time * 9.0) * 0.5) * 1.2;
    amount *= (1.0 + sin(time * 27.0) * 0.5) * 0.8;
    amount = pow(amount, 3.0);

    amount *= 0.5;
    amount += 3.0;

    vec3 col;
    col.r = texture2DRect(img, vec2(uv.x + amount, uv.y)).r;
    col.g = texture2DRect(img, uv).g;
    col.b = texture2DRect(img, vec2(uv.x - amount, uv.y)).b;

    gl_FragColor = vec4(col, 1.0);
}
