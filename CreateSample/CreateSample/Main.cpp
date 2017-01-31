#include "stdafx.h"
#include "CreateSample.hpp"

int main()
{
	CreateSample *cs = new CreateSample(500000, 500000);
	cs->classifyProduct(0.8, 0.2);
	cs->classifyCustomer(0.8, 0.1, 0.1);
	cs->createSample(false,
		10, 2, 0.7, 0.3, 0.5, 0.1,
		2, 10, 0.7, 0.3, 0.1, 0.5);
	cs->dumpSample();
	return 0;
}
