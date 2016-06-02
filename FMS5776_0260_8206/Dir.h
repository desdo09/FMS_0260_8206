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

	//Find the next DirEntry index available 
	short findNextIndex();
	// Check if the file exist into this SectorDir
	bool  fileExist(const char *);
	//Get the DirEntry index by a fileName 
	short findDirByName(const char fileName[12]);

	//insert a directory
	void insert(DirEntry * dir);

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

	/*************************************************
	* FUNCTION
	*    operator[]
	*
	* RETURN VALUE
	*   The file DirEntry
	*
	* PARAMETERS
	*	char fileName[12] - The file name
	*
	* MEANING
	*   Search into the Rootdir if there a file with the
	*	name received, in case yes return the his DirEntry
	*	in case no return null
	*
	**************************************************/

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

	/*************************************************
	* FUNCTION
	*    operator[]
	*
	* RETURN VALUE
	*   The file DirEntry
	*
	* PARAMETERS
	*	int index - The file index
	*
	* MEANING
	*	Return the DirEntry although is empty
	*
	**************************************************/
	DirEntry * operator[](uint i)
	{
		if (i > 27)
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

		if (index == -1)
			throw ProgramExeption("Rootdir full", "RootDir::findNextIndex");

		return index + 14;


	}


	/*************************************************
	* FUNCTION
	*    insert
	*
	* PARAMETERS
	*	DirEntry dir - The file to insert
	*
	* MEANING
	*	The function receive a file and insert this dir
	*
	**************************************************/

	void insert(DirEntry * dir)
	{
		*((*this)[findNextIndex()]) = *dir;
	}


	//no friend for this class

};

