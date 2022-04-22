#version 450 core

in vec4 Coords;

out vec4 FragColor;

uniform int maxIterations;
uniform dvec2 offset;
uniform double zoom;
uniform dvec2 jnum;
uniform int fractalType;
uniform int coloringType;

vec3 iterNewt(dvec2 coord, int coloring_type);
vec3 iterBurn(dvec2 coord, int coloring_type);
vec3 iterMandel(dvec2 coord, int coloring_type);
vec3 iterJulia(dvec2 coord, dvec2 num, int coloring_type);

void main()
{
	if (Coords.x <= 0.005 && Coords.x >= -0.005 &&
		Coords.y <= 0.005 && Coords.y >= -0.005)
		FragColor = vec4(0.0, 1.0, 0.0, 1.0);
	else
		switch (fractalType)
		{
		case 0:
			FragColor = vec4(iterBurn(dvec2(Coords.x, Coords.y)/zoom - offset, coloringType), 1.0);
			break;
		case 1:
			FragColor = vec4(iterNewt(dvec2(Coords.x, Coords.y)/zoom - offset, coloringType), 1.0);
			break;
		case 2:
			FragColor = vec4(iterJulia(dvec2(Coords.x, Coords.y)/zoom - offset, jnum, coloringType), 1.0);
			break;
		case 3:
			FragColor = vec4(iterMandel(dvec2(Coords.x, Coords.y)/zoom - offset, coloringType), 1.0);
			break;
		}
}

dvec2 sqrIm(dvec2 number)
{
	return dvec2(
		number.x * number.x - number.y * number.y,
		2 * number.x * number.y
	);
}

dvec2 cbc(dvec2 number)
{
	return dvec2(
		number.x * number.x * number.x - 3 * number.x * number.y * number.y,
		3 * number.x * number.x * number.y - number.y * number.y * number.y
	);
}

dvec2 divC(dvec2 n1, dvec2 n2)
{
	return dvec2(
	(n1.x * n2.x + n1.y * n2.y) / (n2.x * n2.x + n2.y * n2.y), 
	(n1.y * n2.x - n1.x * n2.y) / (n2.x * n2.x + n2.y * n2.y)
	);
}

dvec2 mulC(dvec2 n1, dvec2 n2)
{
	return dvec2(
	n1.x * n2.x - n1.y * n2.y,
	n1.x * n2.y + n1.y * n2.x
	);
}

vec3 iterNewt(dvec2 coord, int coloring_type)
{
	dvec2 z = coord;

	dvec2 roots[3] =
	{
		dvec2(1.0, 0.0), 
		dvec2(-0.5,  sqrt(3.0)/2.0), 
		dvec2(-0.5, -sqrt(3.0)/2.0)
	};

	vec3 color[3] =
	{
		vec3(1.0, 0.0, 0.0),
		vec3(0.0, 1.0, 0.0),
		vec3(0.0, 0.0, 1.0)
	};

	for(int i = 0; i < maxIterations; i++)
	{
		z = z - mulC(dvec2(1.0, 0.0), divC((cbc(z) - dvec2(1.0, 0.0)), (mulC(dvec2(3.0, 0.0), sqrIm(z)))));

		for (int j = 0; j < 3; j++)
		{
			dvec2 diff = z - roots[j];
			if ((abs(diff.x) < 0.0001) && (abs(diff.y) < 0.0001))
			{
				if (coloring_type == 0)
					return mix(color[j], vec3(0.0), 
					sqrt((float(i) - log2(log2(float(length(z))))) / float(maxIterations) ) );
				else if (coloring_type == 1)
					return mix(color[j], vec3(0.0), 
					sqrt((float(i) - log2(log2(float(length(diff+1.0))))) / float(maxIterations) ) );
			}
		}
	}
	return vec3(0.0);
}

vec3 iterBurn(dvec2 coord, int coloring_type)
{
	dvec2 z = dvec2(0, 0);

	for(int i = 0; i < maxIterations; i++)
	{
		z = sqrIm(dvec2(abs(z.x), 0.0) + dvec2(0.0, -1.0) * (0.0, abs(z.y))) + coord;
		if (length(z) > 2)
			if (coloring_type == 0)
				return vec3(1.0) - vec3((i % 32 * 8), (i % 32 * 8), (i % 64 * 4)) / 256.0;
			else if (coloring_type == 1)
				return mix(vec3(0.0, 0.0, 1.0), vec3(1.0, 1.0, 1.0), 
				sqrt((float(i) + 1.0 - log2(log2(float(length(z))))) / float(maxIterations) ) );
	}
	return vec3(0.0);
}

vec3 iterMandel(dvec2 coord, int coloring_type)
{
	dvec2 z = dvec2(0, 0);

	for(int i = 0; i < maxIterations; i++)
	{
		//z = dvec2(z.x, -z.y);
		z = sqrIm(z) + coord;
		if (length(z) > 2.0)
			if (coloring_type == 0)
				return vec3((i % 4 * 64) / 255.0, (i % 8 * 32) / 255.0, (i % 16 * 16) / 255.0);
			else if (coloring_type == 1)
				return mix(vec3(1.0, 0.0, 0.0), vec3(1.0, 1.0, 0.0), 
				sqrt((float(i) - log2(log2(float(length(z))))) / float(maxIterations) ) );
	}
	return vec3(0.0);
}



vec3 iterJulia(dvec2 coord, dvec2 num, int coloring_type)
{
	dvec2 z = coord;

	for(int i = 0; i < maxIterations; i++)
	{
		z = sqrIm(z) + num;
		//z = sqrIm(dvec2(abs(z.x), 0.0) + dvec2(0.0, -1.0) * (0.0, abs(z.y))) + num;
		if (length(z) > 2.0)
			if (coloring_type == 0)
				return vec3((i % 4 * 64) / 255.0, (i % 8 * 32) / 255.0, (i % 16 * 16) / 255.0);
			else if (coloring_type == 1)
				return mix(vec3(1.0, 0.0, 0.0), vec3(1.0, 1.0, 0.0), 
				sqrt((float(i) - log2(log2(float(length(z))))) / float(maxIterations) ) );
	}
	return vec3(0.0);
}

