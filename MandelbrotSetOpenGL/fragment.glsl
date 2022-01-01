#version 450 core

in vec4 Coords;

out vec4 FragColor;

uniform int maxIterations;
uniform dvec2 offset;
uniform double zoom;

dvec2 sqrtIm(dvec2 number){
	return dvec2(
		number.x * number.x - number.y * number.y,
		2 * number.x * number.y
	);
}

double iterMandel(dvec2 coord){
	dvec2 z = dvec2(0, 0);
	for(int i = 0; i < maxIterations; i++){
		z = sqrtIm(z) + coord;
		if (length(z) > 2) 
			return i / float(maxIterations);
	}
	return float(maxIterations);
}

void main()
{

	double t = iterMandel(((dvec2(Coords.x, Coords.y))/zoom) - offset);
//	double r = 9.48 * (1.0 - t) * t * t * t;
//	double g = 16.0 * (1.0 - t) * (1.0 - t) * t * t;
//	double b = 9.48 * (1.0 - t) * (1.0 - t) * (1.0 - t) * t;
	FragColor = vec4(1-t, 1-t, 1-t, 1.0f);
}