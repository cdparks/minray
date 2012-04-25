varying vec3 normal;
varying vec3 vertex;

void main(void) {
	vec3 light = normalize(vec3(2.0, 5.0, -1.0) - vertex);
	vec3 eye = normalize(-vertex); 
	vec3 r = normalize(-reflect(light, normal));

	float shiny = 5.0;

	vec4 amb = vec4(0.2, 0.2, 0.2, 1.0);
	vec4 diff = vec4(0.1, 0.2, 0.8, 1.0) * max(dot(normal, light), 0.0);
	vec4 spec = vec4(0.5, 0.5, 0.5, 1.0) * pow(max(dot(r, eye), 0.0), shiny);
	gl_FragColor = amb + diff + spec;
}
