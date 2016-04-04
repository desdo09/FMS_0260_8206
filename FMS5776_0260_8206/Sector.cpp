#include "Sector.h"



Sector::Sector()
{
}

Sector::Sector(uint index)
{
	this->sectorNr = index;
}

Sector::Sector(DAT * dat)
{
	for (int i = 0; i < sizeof(Sector); i++)
		*((char *)this + i) = *((char *)dat + i);

}

Sector::Sector(const void * x)
{
	for (int i = 0; i < 1020 && i<sizeof(x); i++)
		this->rawData[i] = *((char *)x + i);

	
}


Sector::~Sector()
{
	//delete[] rawData;
}


void Sector::setSectorNr(uint x)
{
	this->sectorNr = x;
}
