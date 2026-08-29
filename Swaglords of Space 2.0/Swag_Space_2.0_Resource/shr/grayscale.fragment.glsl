#version 120

uniform sampler2D texture;

void main()
{
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

	//float gray = dot(pixel.rgb, vec3(0.5, 0.5, 0.5));

	float gray = (pixel.r + pixel.g + pixel.b) / 3;

	//gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
	gl_FragColor = vec4(gray, gray, gray, pixel.a);
}

//#version 120
//
//uniform sampler2D texture;
//
//void main()
//{
//	gl_FragColor = vec4(texture2D(texture, gl_TexCoord[0].xy));
//}