uniform sampler2D texture;
uniform vec2 size;

bool canBloom(vec4 col)
{
	if((col.x+col.y+col.z)>2.0)
		return true;
	return false;
}

void main()
{
    // lookup the pixel in the texture
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    // multiply it by the color
	vec4 colour = gl_Color * pixel;
	
	for(int i = 0; i < 8; i++)
    {
        for(int j = 0;  j < 8;  j++)
        {

        }
    }
	
	
    gl_FragColor = colour;
}