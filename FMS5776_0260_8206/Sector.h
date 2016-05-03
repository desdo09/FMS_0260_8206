#pragma once
#include "Header.h"


class Sector
{


private:
	uint sectorNr;
	char rawData[1020];
	void setSectorNr(uint x);
	

public:
	Sector();
	~Sector();

	char * getData() { return rawData; }
	void setData(char * data) { memcpy(rawData, data, 1020); }
	Sector(uint);
	Sector(DAT *);
	Sector(FileHeader *);
	Sector(const void * x);
	friend class TestLevel_0;
	friend class Disk;





};

