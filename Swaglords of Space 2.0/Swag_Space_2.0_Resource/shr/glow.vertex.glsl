#version 120

varying vec2 localPosition;

void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    // CircleShape vertices are in local coordinates
    localPosition = gl_Vertex.xy;
}