#pragma once
#include "Header.h"
#include "DAT.h"
#include "FileHeader.h"

class Sector
{


private:
	uint sectorNr;
	char rawData[1020];
	void setSectorNr(uint x);


public:
	Sector(uint);
	Sector(DAT *);
	Sector(FileHeader *);
	Sector(const void * x);
	Sector();
	~Sector();

	friend class TestLevel_0;
	friend class Disk;





};

