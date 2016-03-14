#include "DirEntry.h"


class SectorDir
{


public:
	SectorDir();
	~SectorDir();

private:
	int sectorNr;
	DirEntry dirEntry[14];
	char unUse[12];


};

class RootDir
{

public:
	RootDir();
	~RootDir();

private:
	SectorDir msbSector;
	SectorDir lsbSector;
};

