#version 120

varying vec2 localPosition;

uniform float radius;
uniform vec4 shapeColor;

void main()
{
    // Distance from circle center
    float dist = length(localPosition - vec2(radius, radius));

    // 0 = center, 1 = edge
    float edge = smoothstep(
        radius * 0.2,
        radius * 0.80,
        dist
    );

    // Dark center -> bright edge
    float brightness = mix(
        0.8,
        1.5,
        edge
    );

    gl_FragColor = vec4(
        shapeColor.rgb * brightness,
        shapeColor.a
    );
}