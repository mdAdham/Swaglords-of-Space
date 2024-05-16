uniform sampler2D texture;
uniform vec2 lightPosition;
uniform float power;
uniform vec4 lightColor;

void main()
{
	float ambient = 5.20f;
	//Calculate the vector from the fragment to the light source
	vec2 toLight = lightPosition - gl_FragCoord.xy;

	//Calculate the distance to the light source
	float distance = length(toLight);

	//Calculate the attenuation factor based on distance
	float attenuation = 1 / (distance / power);

	//Get the color of the pixel from the texture
	vec4 pixelColor = texture2D(texture, gl_TexCoord[0].xy);
	pixelColor.rgb *= ambient;

	//Calculate the final color with lighting applied
	vec4 finalColor = ((pixelColor * ambient) * attenuation) * lightColor;
	
	finalColor.a = pixelColor.a;
	gl_FragColor = finalColor;
}