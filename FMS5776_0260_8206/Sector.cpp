#include "Sector.h"



Sector::Sector()
{
}

Sector::Sector(uint index)
{
	this->sectorNr = index;
}


Sector::~Sector()
{
	//delete[] rawData;
}

void Sector::setSectorNr(uint x)
{
	this->sectorNr = x;
}
