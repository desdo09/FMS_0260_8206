#pragma once
#include "Header.h"
#include "Disk.h"
#include "TestLevel_0.h"
#include "FileHeader.h"
#include "Students.h"


class TestLevel_2
{
public:


	static void createFile(Disk * d,string fileName = "Students" )
	{
		
		uint sectorSize;

		cout << "file name: "<< fileName << endl;
		cout << "The owner: "<<"David" << endl;
		cout << "Enter the number of sectors" << endl;
		cin >> sectorSize;

		d->createfile(fileName, (string)"David", false, sizeof(Student), sectorSize, "I", 0, 4);
		printHeader(d, fileName.c_str());
	}

	static void extFile(Disk * d)
	{
		string  fileName;
		string  ownerFile;
		uint sectorSize;
		cout << "Enter the file name" << endl;
		cin >> fileName;
		cout << "Enter the owner of the file" << endl;
		cin >> ownerFile;
		cout << "Enter the number of sectors to extend" << endl;
		cin >> sectorSize;

		d->extendfile(fileName, ownerFile, sectorSize);

		printHeader(d,fileName.c_str());
	}

	static void printHeader(Disk * d, const char * file = NULL)
	{
		char fileName[12];

		if (file != NULL)
			memcpy(fileName, file, 12);
		else {
			cout << "Insert the file name:" << endl;
			cin >> fileName;
		}
		DirEntry * dir = d->getDir(fileName);

		if (dir == NULL)
		{
			throw "File doesn't exist";
			return;
		}

		cout << "File Name -->" << dir->filename << endl;
		cout << "File Owner -->" << dir->fileOwner << endl;
		cout << "File Address -->" << dir->fileAddr << endl;
		cout << "Created Date -->" << dir->crDate << endl;
		cout << "File size -->" << dir->fileSize << endl;
		cout << "End of Record Nr -->" << dir->eofRecNr << endl;
		cout << "Max rec size -->" << dir->maxRecSize << endl;
		cout << "Actual Rec Size -->" << dir->actualRecSize << endl;
		cout << "Rec Format -->" << dir->recFormat << endl;
		cout << "Key offset -->" << dir->keyOffset << endl;
		cout << "Key Size -->" << dir->keySize << endl;
		cout << "Key Type -->" << dir->keyType << endl;



	}
	static void printHeader(DirEntry * dir)
	{
	

		if (dir == NULL)
		{
			throw "File doesn't exist";
			return;
		}

		cout << "File Name -->" << dir->filename << endl;
		cout << "File Owner -->" << dir->fileOwner << endl;
		cout << "File Address -->" << dir->fileAddr << endl;
		cout << "Created Date -->" << dir->crDate << endl;
		cout << "File size -->" << dir->fileSize << endl;
		cout << "End of Record Nr -->" << dir->eofRecNr << endl;
		cout << "Max rec size -->" << dir->maxRecSize << endl;
		cout << "Actual Rec Size -->" << dir->actualRecSize << endl;
		cout << "Rec Format -->" << dir->recFormat << endl;
		cout << "Key offset -->" << dir->keyOffset << endl;
		cout << "Key Size -->" << dir->keySize << endl;
		cout << "Key Type -->" << dir->keyType << endl;



	}

	static void DeleteFile(Disk * d)
	{
		string  fileName;
		string  ownerFile;
		char ans;

		cout << "Are you sure? <Y/N>" << endl;
		cin >> ans;
		if (ans == 'y' || ans == 'Y')
		{
			cout << "Enter the file name" << endl;
			cin >> fileName;
			cout << "Enter the owner of the file" << endl;
			cin >> ownerFile;

			d->delfile(fileName, ownerFile);
		}


	}

	static DATtype getFAT(Disk *d, char * file = NULL, DATtype FAT = NULL)
	{
		char fileName[12];

		if (file != NULL)
			memcpy(fileName, file, 12);
		else {
			cout << "Insert the file name:" << endl;
			cin >> fileName;
		}


		DirEntry * dir = d->getDir(fileName);


		if (dir == NULL)
		{
			throw "File doesn't exist";
			return NULL;
		}

		
		FileHeader fh;

		cout << "FAT into address: " << dir->fileAddr << endl;

		d->seekToSector(dir->fileAddr);

		cout << "Reading FAT, please wait..." << endl;
		d->dskfl.read((char *)&fh, sizeof(Sector));
		
		FAT = fh.getFAT();
	


		return FAT;
	}
};
