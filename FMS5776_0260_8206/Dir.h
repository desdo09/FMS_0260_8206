#pragma once
#include "header.h"
#include "DirEntry.h"


class SectorDir
{
private:
	int sectorNr;
	DirEntry dirEntry[14];
	char unUsed[12];

public:
	SectorDir();
	~SectorDir();

	short findNextIndex();
	bool  fileExist(const char *);
	short findDirByName(const char fileName[12]);

	friend class Disk;
	friend class RootDir;


};

class RootDir
{
private:
	SectorDir msbSector;
	SectorDir lsbSector;
	
public:
	RootDir();
	~RootDir();

	// The operator get a name and return his DirEntry
	DirEntry * operator[](const char fileName[12])
	{
		int index = this->lsbSector.findDirByName(fileName);
		if (index != -1 && this->lsbSector.dirEntry[index].entryStatus == 1)
			return &this->lsbSector.dirEntry[index];
		else
		{
			index = this->msbSector.findDirByName(fileName);
			if (index == -1 || this->msbSector.dirEntry[index].entryStatus != 1)
				return NULL;

			return 	&msbSector.dirEntry[index];
		}
	}

	// The operator get an index and return his DirEntry
	DirEntry * operator[](uint i)
	{
		if(i>27)
			throw ProgramExeption("Root directory only contains 28 files", "Rootdir");

		if (i < 14)
			return &msbSector.dirEntry[i];

		if (i >= 14)
			return &lsbSector.dirEntry[i];

		return NULL;

	}

	// Return the next index available
	int findNextIndex()
	{
		int index = msbSector.findNextIndex();

		if (index != -1)
			return index;

		index = lsbSector.findNextIndex();

		return (index != -1)? (index + 14 ): index;


	}
	
	//no friend for this class

};

