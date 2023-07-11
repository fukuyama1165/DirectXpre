#include "Easing.h"


template<>
double easeOutQuint<double>(double start, double end, float time)
{
	return (start + (1 - (pow(1 - time, 5))) * (end - start));
}


