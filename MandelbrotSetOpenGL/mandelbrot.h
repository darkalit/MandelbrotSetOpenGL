#pragma once
#include <Complexn.h>

int get_iter(cmpx::Complexn num, int max_steps = 25)
{
	cmpx::Complexn z(0, 0);
	int i = 0;
	while (i < max_steps && z.abs() <= 2)
	{
		z = z * z + num;
		++i;
	}
	return i;
}