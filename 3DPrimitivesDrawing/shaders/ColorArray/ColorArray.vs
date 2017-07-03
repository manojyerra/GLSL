attribute vec4 vertexColor;
attribute vec4 vertex;

varying vec4 fragColor;

void main(void)
{
	fragColor = vertexColor;

	gl_Position = gl_ModelViewProjectionMatrix * vertex;
}
