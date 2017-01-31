#include "stdafx.h"
#include "CreateSample.hpp"

int main()
{
	CreateSample *cs = new CreateSample(50000, 50000);
	cs->classifyProduct(0.8, 0.2);
	cs->classifyCustomer(0.8, 0.1, 0.1);
	cs->createSample(false,
		10, 0.5, 0.5, 0.5, 2, 2,
		0.5, 10, 0.5, 0.5, 2, 2);
	cs->dumpSample();
	return 0;
}
