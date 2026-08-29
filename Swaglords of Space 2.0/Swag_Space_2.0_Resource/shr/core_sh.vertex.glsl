#version 420

varying out vec4 vert_pos;

void main()
{
    vert_pos = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

	gl_FrontColor = gl_Color;
}