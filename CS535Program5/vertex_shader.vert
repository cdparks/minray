varying vec3 normal;
varying vec3 vertex;

void main(void) {
	gl_Position = ftransform(); // the same as gl_ModelViewProjectionMatrix * gl_Vertex;
	normal = normalize(gl_NormalMatrix * gl_Normal);
	vertex = vec3(gl_ModelViewMatrix * gl_Vertex);
}
