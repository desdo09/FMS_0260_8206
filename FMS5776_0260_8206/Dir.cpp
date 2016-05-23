#include "Dir.h"



SectorDir::SectorDir()
{
}

SectorDir::~SectorDir()
{
}


short SectorDir::findNextIndex()
{
	for (int i = 0; i < 14; i++)
		if (this->dirEntry[i].entryStatus == 0 || this->dirEntry[i].entryStatus == 2)
			return i;
	return -1;
}
bool SectorDir::fileExist(const char * fileName)
{
	for (int i = 0; i < 14; i++)
	{
		if (this->dirEntry[i].entryStatus == 1 && !strcmp(this->dirEntry[i].filename, fileName) )
			return true;
	}
	return false;
}

short SectorDir::findDirByName(const char * fileName)
{
	for (short i = 0; i < 14; ++i)
	{
		if (!strcmp(this->dirEntry[i].filename, fileName))
			return i;
	}

	return -1;
}


//Root Dir

RootDir::RootDir()
{
	msbSector.sectorNr = 2;
	lsbSector.sectorNr = 3;
}

RootDir::~RootDir()
{
}


