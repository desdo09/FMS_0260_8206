#pragma once
#include "Header.h"


class Sector
{


private:
	uint sectorNr;
	char rawData[1020]{ '\0' };
	void setSectorNr(uint x);
	

public:
	Sector();
	~Sector();

	char * getData() { return rawData; }
	void setData(char * data) { memcpy(rawData, data, 1020); }
	uint getSectorNumber() { return this->sectorNr; }
	void setSectorNumbe(uint num) { this->sectorNr = num; }
	Sector(uint);
	Sector(DAT *);
	Sector(FileHeader *);
	Sector(const char * x);
	Sector(uint index, char * data);
	friend class TestLevel_0;
	friend class Disk;





};

