#version 120
uniform sampler2D texture;
void main()
{
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

	if (pixel.rgb == vec3(1.0))
		pixel.rgb = vec3(gl_TexCoord[0].xy, 0.0);

	gl_FragColor = pixel;
}


//#version 120
//
//uniform sampler2D texture;
//
//void main()
//{
//	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
//
//	float value = (pixel.x + pixel.g + pixel.b) / 3;
//
//	vec4 outputPixel;
//	outputPixel = pixel;
//	if (value <= 0.2)
//		outputPixel.rgb += vec3(gl_TexCoord[0].xy, 0.0);
//
//	gl_FragColor = outputPixel;
//}

//#version 120
//
//uniform sampler2D texture;
//
//void main()
//{
//    float blur_radius = 0.0005;
//
//    vec2 offx = vec2(blur_radius, 0.0);
//    vec2 offy = vec2(0.0, blur_radius);
//
//    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy)               * 4.0 +
//                 texture2D(texture, gl_TexCoord[0].xy - offx)        * 2.0 +
//                 texture2D(texture, gl_TexCoord[0].xy + offx)        * 2.0 +
//                 texture2D(texture, gl_TexCoord[0].xy - offy)        * 2.0 +
//                 texture2D(texture, gl_TexCoord[0].xy + offy)        * 2.0 +
//                 texture2D(texture, gl_TexCoord[0].xy - offx - offy) * 1.0 +
//                 texture2D(texture, gl_TexCoord[0].xy - offx + offy) * 1.0 +
//                 texture2D(texture, gl_TexCoord[0].xy + offx - offy) * 1.0 +
//                 texture2D(texture, gl_TexCoord[0].xy + offx + offy) * 1.0;
//
//    gl_FragColor =  gl_Color * (pixel / 16.0);
//}

//#version 120
//
//uniform sampler2D texture;
//
//void main()
//{
//    float pixel_threshold = 0.001;
//
//    float factor = 1.0 / (pixel_threshold + 0.001);
//    vec2 pos = floor(gl_TexCoord[0].xy * factor + 0.5) / factor;
//    gl_FragColor = texture2D(texture, pos) * gl_Color;
//}
//

//#version 120
//uniform sampler2D texture;
//
//void main()
//{
//    float edge_threshold = 0.2;
//
//    const float offset = 1.0 / 512.0;
//    vec2 offx = vec2(offset, 0.0);
//    vec2 offy = vec2(0.0, offset);
//
//    vec4 hEdge = texture2D(texture, gl_TexCoord[0].xy - offy)        * -2.0 +
//                 texture2D(texture, gl_TexCoord[0].xy + offy)        *  2.0 +
//                 texture2D(texture, gl_TexCoord[0].xy - offx - offy) * -1.0 +
//                 texture2D(texture, gl_TexCoord[0].xy - offx + offy) *  1.0 +
//                 texture2D(texture, gl_TexCoord[0].xy + offx - offy) * -1.0 +
//                 texture2D(texture, gl_TexCoord[0].xy + offx + offy) *  1.0;
//
//    vec4 vEdge = texture2D(texture, gl_TexCoord[0].xy - offx)        *  2.0 +
//                 texture2D(texture, gl_TexCoord[0].xy + offx)        * -2.0 +
//                 texture2D(texture, gl_TexCoord[0].xy - offx - offy) *  1.0 +
//                 texture2D(texture, gl_TexCoord[0].xy - offx + offy) * -1.0 +
//                 texture2D(texture, gl_TexCoord[0].xy + offx - offy) *  1.0 +
//                 texture2D(texture, gl_TexCoord[0].xy + offx + offy) * -1.0;
//
//    vec3 result = sqrt(hEdge.rgb * hEdge.rgb + vEdge.rgb * vEdge.rgb);
//    float edge = length(result);
//    vec4 pixel = gl_Color * texture2D(texture, gl_TexCoord[0].xy);
//    if (edge > (edge_threshold * 8.0))
//        pixel.rgb = vec3(0.0, 0.0, 0.0);
//    else
//        pixel.a = edge_threshold;
//    gl_FragColor = pixel;
//}
