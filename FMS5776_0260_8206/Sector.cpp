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

Sector::Sector(FileHeader * fh)
{
	for (int i = 0; i < 1024 && i<sizeof(fh); i++)
		this->rawData[i] = *((char *)fh + i);
}

Sector::Sector(const char * data)
{
	sectorNr = 99999999;
	memcpy(rawData, data, 1020);
}

Sector::Sector(uint index, char * data)
{
	this->sectorNr = index;
	memcpy(this->rawData, data,1020);

}

Sector::~Sector()
{
	//delete[] rawData;
}


void Sector::setSectorNr(uint x)
{
	this->sectorNr = x;
}

