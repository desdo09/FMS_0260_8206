#include "Disk.h"


/*************************************************
*
*				  Constructor
*
**************************************************/


Disk::Disk()
{

	currDiskSectorNr = 0;


}

Disk::Disk(string fileName, string Owner, ConstructorCod cod)
{
	switch (cod)
	{
	case Create:
		createdisk(fileName, Owner);
		break;
	case Mount:
		mountdisk(fileName);
		break;
	default:
		break;
	}


	currDiskSectorNr = 0;


}

Disk::~Disk()
{

}

/*************************************************
*
*				  Level 0
*
**************************************************/

void Disk::createdisk(string diskName, string owner) {


	if (this->mounted)															// Verify if there is a mounted disk
		throw ProgramExeption("there is a mounted disk", "Disk::createdisk");	// If true then throw an exception

	VerifyAndAddExt(diskName);

	if (ifstream(diskName))														// Verify if the directory exist
		throw ProgramExeption("There is another file with the same name",		// If true then throw an exception
			"Disk::createdisk");

	if (dskfl.is_open())														// In case that is another file opened
		dskfl.close();															// close it


	dskfl.open(diskName.c_str(), ios::out | ios::binary);						// Create and open the file with the name receive



	if (!dskfl.is_open()) {														// In case of error
		string error = "Error to open the file ";
		error += diskName.c_str();
		throw ProgramExeption(error.c_str(), "Disk::createdisk");	// throw an exception
	}

	Sector temp;
	for (int i = 0; i < amountOfSectors * 2; i++)								// Load the number of sectors defined on header into the file
	{
		temp.setSectorNr(i);													// Each sector get his index  
		dskfl.write((char *)&temp, sizeof(Sector));								// write sector into the file (disk)


	}
	//dskfl.flush();

	vhd.SetdiskName(diskName.substr(diskName.find_last_of("\\") + 1, diskName.length() - extensionLenght));	// Set the disk name into volume header

	vhd.SetdiskOwner(owner);													// Set the owner name into volume header

	vhd.Set();
	dat.dat[1598] = 0;
	dat.dat[1599] = 0;
	dskfl.seekp(0, ios::beg);													// Return the dskfl to the begin of the file 	
	dskfl.write((char *)&vhd, sizeof(Sector));									// Write the volume header into the disk
	dskfl.write((char *)&dat, sizeof(Sector));									// Write the DAT into the disk
	dskfl.seekp(1598 * sizeof(Sector), ios::beg);								// Return the dskfl to the begin of the file 

	dskfl.write((char *)&dat, sizeof(Sector));									// Write the DAT into the disk


	dskfl.close();

	seekToSector(4);

	//File doesn't closed
}

void Disk::recreatedisk(string diskName = "")
{
	if (diskName != "" && this->mounted)										// Verify if there is a mounted disk
		throw "there is a mounted disk";										// If true then throw an exception

	if (diskName != "" && !ifstream(diskName))									// Verify if the directory exist
		throw "Disk not found";													// If not exist then throw an exception

	if (diskName != "" && dskfl.is_open())										// In case that is another file opened
		dskfl.close();															// close it

	if (diskName != "")
		dskfl.open(diskName.c_str(), ios::out | ios::in | ios::binary);			// Create and open the file with the name receive

	seekToSector(0);

	Sector temp;
	for (int i = 0; i < amountOfSectors * 2; i++)								// Load the number of sectors defined on header into the file
	{
		temp.setSectorNr(i);													// Each sector get his index  
		dskfl.write((char *)&temp, sizeof(Sector));								// write sector into the file (disk)


	}

	dat.resetDat();

	dat.dat[1598] = 0;
	dat.dat[1599] = 0;


	seekToSector(0);

	dskfl.write((char *)&vhd, sizeof(Sector));									// Write the volume header into the disk

	dskfl.write((char *)&dat, sizeof(Sector));									// Write the DAT into the disk

	seekToSector(1598);

	dskfl.write((char *)&dat, sizeof(Sector));									// Write the DAT into the disk

	//this->mounted = true;														// Disk set as mounted

}

void Disk::mountdisk(string  fileName)
{
	if (this->mounted)
		throw ProgramExeption("There is another disk mounted", "mounted");

	if (dskfl.is_open())
		dskfl.close();


	VerifyAndAddExt(fileName);

	dskfl.open(fileName, ios::binary | ios::out | ios::in);

	dskfl.seekp(0, ios::beg);													// Return the dskfl to the begin of the file 	

	if (!dskfl.good())
		throw ProgramExeption("File cannot be opened", "Disk::mountdisk");

	this->dskfl.read((char *)&vhd, sizeof(Sector));								// Copy the first sector into volume header object

	this->dskfl.read((char *)&dat, sizeof(Sector));								// Copy the second sector into DAT object

	this->dskfl.read((char *)&rootdir, sizeof(RootDir));						// Copy the second sector into DAT object


	this->currDiskSectorNr = 2;													// Set current Disk Sector Number as 2

	this->mounted = true;														// The disk is mounted

	this->seekToSector(4);
}

void Disk::unmountdisk()
{
	if (!this->mounted)
		throw ProgramExeption("No disk mounted");

	if (!dskfl.is_open())
		throw ProgramExeption("Disk problems, file are not opened");

	seekToSector(0);

	dskfl.write((char *)&vhd, sizeof(Sector));									// Update the volume header

	dskfl.write((char *)&dat, sizeof(Sector));									// Update the DAT

	seekToSector(80);															// Return the dskfl to the begin of the file 

	dskfl.write((char *)&dat, sizeof(Sector));									// Write the DAT into the disk

	this->mounted = false;

	dskfl.close();																// Close file


}

fstream * Disk::getdskfl(void)
{

	if (this->dskfl.is_open())
		return &dskfl;

	return nullptr;
}

void Disk::seekToSector(DATtype FAT, uint index, int fileheaderSec)
{
	index += 1;												// the file header

	int i;
	//Get the file first sector
	for (i = this->firstIndex(FAT, false) / 2; i < amountOfSectors && index > 1; i++)
	{


		if (FAT[i] && index > 1)							// if is last than 2 then stop 
			index -= 2;										// 2 sectors	



	}

	if (index > 1)
		throw ProgramExeption("Overflow error", "Disk::seekToSector");

	if (fileheaderSec != -1 && i * 2 == fileheaderSec)
		index = 1;

	seekToSector(i * 2 + index);							   // index can be 0 or 1

}

void Disk::seekToSector(uint index)
{

	this->dskfl.seekg(ios::beg);												// Points the dskfl (for read) to the begging

	this->dskfl.seekp(ios::beg);												// Points the dskfl (for write) to the begging

	if (index > 0)
	{
		this->dskfl.seekg(index * sizeof(Sector));

		this->dskfl.seekp(index * sizeof(Sector));
	}

	this->currDiskSectorNr = index;												// Update Current Disk Sector Number
}

void Disk::writeSector(uint index, Sector * sec)
{


	seekToSector(index);

	writeSector(sec);

}

void Disk::writeSector(Sector * sec)
{
	if (!this->mounted)
		throw ProgramExeption("There not mounted disk", "Disk::WriteSector");

	if (!dskfl.is_open())
		throw ProgramExeption("File's problem", "Disk::WriteSector");

	if (sec->sectorNr == 99999999)
		sec->sectorNr = currDiskSectorNr;

	dskfl.write((char *)sec, sizeof(Sector));

	this->currDiskSectorNr++;													// Update Current Disk Sector Number

	dskfl.flush();
}

void Disk::readSector(uint index, Sector * sec)
{
	seekToSector(index);

	readSector(sec);
}

void Disk::readSector(Sector * sec)
{
	if (!this->mounted)
		throw ProgramExeption("There not mounted disk", "WriteSector");

	dskfl.read((char *)sec, sizeof(Sector));

	this->seekToSector(currDiskSectorNr);									// return to the current sector


}

void Disk::VerifyAndAddExt(string & file)
{

	string ext = file.substr(file.length() - extensionLenght, file.length());

	if (ext.find(diskExtension))
	{
		file += diskExtension;												// Add the extension of the file (disk)  
	}
}


/*************************************************
*
*				  Level 1
*
**************************************************/

/*Private functions*/
uint Disk::firstFit(uint sectoresAmount, uint indexStart = 0)
{
	int index = -1;
	int temp = sectoresAmount;
	for (int i = indexStart; i < amountOfSectors; i++)
	{
		if (!dat.dat[i])
			temp = sectoresAmount;
		else
			temp--;

		if (!temp)
		{
			index = i - sectoresAmount + 1;
			break;
		}
	}
	return index;
}

uint Disk::bestFit(uint sectoresAmount, uint indexStart = 0)
{
	int index = -1;
	int temp = sectoresAmount;
	int small = 1600;
	for (int i = indexStart; i < amountOfSectors; i++)
	{
		if (dat.dat[i])
			temp--;

		if (!dat.dat[i] || i == amountOfSectors - 1)
		{
			if (temp <= 0 && small > temp*(-1))
			{
				small = temp*(-1) - ((i == amountOfSectors - 1) ? 1 : 0);
				index = i - sectoresAmount - small;
			}
			temp = sectoresAmount;

		}


	}

	return index;



}

uint Disk::worstFit(uint sectoresAmount, uint indexStart = 0)
{
	int index = -1;
	int temp = sectoresAmount;
	int big = -1;

	for (int i = indexStart; i < amountOfSectors; i++)
	{
		if (dat.dat[i])
			temp--;

		if (!dat.dat[i] || i == amountOfSectors - 1)
		{
			if (temp <= 0 && big < temp*(-1))
			{
				big = temp*(-1) - ((i == amountOfSectors - 1) ? 1 : 0);
				index = i - sectoresAmount - big;
			}
			temp = sectoresAmount;

		}

	}

	return index;
}

int Disk::firstIndex(DATtype DAT, bool isDAT, uint indexStart)
{
	indexStart /= 2;
	for (int i = indexStart; i < amountOfSectors; i++)
	{
		if (isDAT && !DAT[i] || !isDAT & DAT[i])
		{
			return i * 2;
			break;
		}
	}
	return -1;
}

int Disk::lastIndex(DATtype DAT, bool isDAT)
{

	int index = -1;
	for (int i = 0; i < amountOfSectors; i++)
	{
		if (isDAT && !DAT[i] || !isDAT & DAT[i])
			index = i * 2;

	}
	return index;
}

/*Public functions*/
void Disk::format(string & owner)
{
	if (!this->mounted)
		throw ProgramExeption("There is not mounted disk", "format");


	if (this->vhd.GetdiskOwner() != owner)
		throw ProgramExeption("This user cannot format this disk", "Format");


	this->recreatedisk();


	RootDir root;
	rootdir = root;
	seekToSector(2);
	dskfl.write((char *)&root, sizeof(Sector));										// RootDir take 2 sectors
	vhd.SetformaDate();
	vhd.isFormated = true;
	this->flush();
}

uint Disk::howmuchempty(uint index)
{
	if (index == 0)
		return (int)dat.dat.count();
	int counter = 0;
	for (int i = index; i < amountOfSectors; i++)
	{
		if (dat.dat[i])
			counter++;
	}
	return counter;
}

void Disk::alloc(DATtype & FAT, uint sectoresAmount, AlgorithmType algo)
{
	if (!this->mounted)
		throw ProgramExeption("There is not mounted disk", "alloc");

	if (!vhd.isFormated)
		throw ProgramExeption("This disk is not formatted", "alloc");

	if (sectoresAmount % 2)
		sectoresAmount++;

	if (howmuchempty() < sectoresAmount)
		throw ProgramExeption("No space left", "alloc");

	sectoresAmount /= 2;																			// cluster = 2 sectores

	int index = 0;

	switch (algo)
	{
	case Disk::first_Fit:
		index = firstFit(sectoresAmount);
		break;
	case Disk::best_Fit:
		index = bestFit(sectoresAmount);
		break;
	case Disk::worst_Fit:
		index = worstFit(sectoresAmount);
		break;
	default:
		break;
	}

	if (index == -1)
	{
		this->alloc(FAT, --sectoresAmount, algo);
		this->alloc(FAT, 2, algo);
	}
	else {
		Sector temp; // using to reset the sectors
		for (uint i = 0; i < sectoresAmount; i++)
		{
			/*Reset clusters*/
			temp.sectorNr = (index + i) * 2;
			this->writeSector((index + i) * 2, &temp);

			temp.sectorNr = ((index + i) * 2) + 1;
			this->writeSector(((index + i) * 2) + 1, &temp);
			/*Set fat and DAT*/
			FAT[index + i] = 1;
			dat.dat[index + i] = 0;
		}
	}

}

void Disk::allocextend(DATtype & FAT, uint sectoresAmount, AlgorithmType algo)
{
	if (!this->mounted)
		throw ProgramExeption("There is not mounted disk", "allocextend");

	if (!vhd.isFormated)
		throw ProgramExeption("This disk is not formatted", "allocextend");


	if (howmuchempty() < sectoresAmount)
		throw ProgramExeption("No space left", "alloc");

	sectoresAmount /= 2;																			// cluster = 2 sectores



	if (howmuchempty() < sectoresAmount)
		throw ProgramExeption("No space left", "allocextend");

	int startindex;																// Object to save the last index in FAT

	//Search the last index into the FAT
	for (int i = 0; i < amountOfSectors; i++)
	{
		if (FAT[i])
			startindex = i;
	}

	if (howmuchempty(startindex) > sectoresAmount)								// In case that doesn't have place to allocate after the last index
		startindex = 4;															// then reset index (4)

	int index;

	switch (algo)
	{
	case Disk::first_Fit:
		index = firstFit(sectoresAmount, startindex);
		break;
	case Disk::best_Fit:
		index = bestFit(sectoresAmount, startindex);
		break;
	case Disk::worst_Fit:
		index = worstFit(sectoresAmount, startindex);
		break;
	default:
		break;
	}

	if (index == -1)
	{
		this->alloc(FAT, sectoresAmount - 2, algo);
		this->alloc(FAT, 2, algo);
	}
	else {

		if (sectoresAmount % 2)											// a cluster are 2 sectors
			sectoresAmount++;

		for (uint i = 0; i < sectoresAmount; i++)
		{
			FAT.set(index + i, 1);
			dat.dat.set(index + i, 0);
		}
	}
}

void Disk::dealloc(DATtype & FAT)
{
	if (!this->mounted)
		throw ProgramExeption("There is not mounted disk", "dealloc");

	for (int i = 0; i < amountOfSectors; i++)
		if (FAT[i])
			dat.dat[i] = 1;
}

void Disk::flush()
{
	if (!this->mounted)															// Verify if there is a mounted disk
		throw ProgramExeption("There is not disk mounted", "flush");				// If true then throw an exception

	if (!dskfl.is_open())
		throw ProgramExeption("Disk file error", "flush");

	//dskfl.flush();

	seekToSector(0);

	dskfl.write((char *)&vhd, sizeof(Sector));									// Write the volume header into the disk

	dskfl.write((char *)&dat, sizeof(Sector));									// Write the DAT into the disk

	if (vhd.isFormated)
		dskfl.write((char *)&rootdir, sizeof(RootDir));							// Write the Root disk into the disk 

	seekToSector(1595);

	dskfl.write((char *)&vhd, sizeof(Sector));									// Write a copy of the volume header into the disk

	dskfl.write((char *)&dat, sizeof(Sector));									// Write a copy of the DAT into the disk

	if (vhd.isFormated)
		dskfl.write((char *)&rootdir, sizeof(RootDir));								// Write a copy of the Root disk into the disk 											// Disk set as mounted

	dskfl.flush();

	seekToSector(0);

}



/*************************************************
*
*				  Level 2
*
**************************************************/

void Disk::createfile(string & fileName, string & ownerFile, bool dynamic, uint regSize, uint sectorSize, string  keyType, uint offset, uint keySize)
{

	if (!this->mounted)
		throw ProgramExeption("There is not mounted disk", "Disk::createfile");

	if (!vhd.isFormated)
		throw ProgramExeption("This disk is not formatted", "Disk::createfile");

	if (this->rootdir[fileName.c_str()] != NULL)
		throw ProgramExeption("File already exist!", "Disk::createfile");

	if (keyType.compare("I") && keyType.compare("D") && keyType.compare("L"))
		throw ProgramExeption("Invalid key type", "Disk::createfile");




	FileHeader fh;
	fh.FAT.reset();
	int firsSectorIndex;
	alloc(fh.FAT, sectorSize, Disk::AlgorithmType::best_Fit);

	firsSectorIndex = this->firstIndex(fh.FAT, false);
	fh.fileDesc.fileAddr = firsSectorIndex;									  // File Address (First sector)
	memcpy(fh.fileDesc.filename, fileName.c_str(), 12);				          // File name
	memcpy(fh.fileDesc.fileOwner, ownerFile.c_str(), 12);					  // File Owner
	_strdate_s(fh.fileDesc.crDate);											  // Created date
	fh.fileDesc.fileSize = sectorSize;										  // File size (Numbers of sectors)
	memcpy(fh.fileDesc.keyType, keyType.c_str(), 2);						  // Key Type - (I)nt,(L)ong,(D)ouble
	fh.fileDesc.keyOffset = offset;											  // Where is the key into the record
	fh.fileDesc.keySize = keySize;										      // The key size
	fh.fileDesc.eofRecNr = 0;												  // Last record number where the file is allocated			
	fh.fileDesc.maxRecSize = regSize;										  // Register size max
	fh.fileDesc.actualRecSize = regSize;									  // Register size actual
	memcpy(fh.fileDesc.recFormat, (dynamic) ? ("V") : ("F"), 1);
	fh.fileDesc.recFormat[1] = '\0';
	fh.fileDesc.entryStatus = 1; 											  // 0 - empty 1 - Active 2- inactive

	int dirEntryIndex;


	dirEntryIndex = rootdir.findNextIndex();

	if (dirEntryIndex != -1)
		*this->rootdir[dirEntryIndex] = fh.fileDesc;
	else
		throw ProgramExeption("Folder Full", "createfile");


	fh.sectorNr = firsSectorIndex;

	this->seekToSector(firsSectorIndex);

	dskfl.write((char *)&fh, sizeof(Sector));

	this->flush();
}

void Disk::extendfile(string & fileName, string & owner, uint sectorSize)
{
	if (!this->mounted)
		throw ProgramExeption("There is not mounted disk", "Disk::extendfile");

	if (!vhd.isFormated)
		throw ProgramExeption("This disk is not formatted", "Disk::extendfile");

	DirEntry * dir = rootdir[fileName.c_str()];

	if(dir == NULL)
		throw ProgramExeption("File not exist!", "Disk::extendfile");

	if (dir->fileOwner != owner)
		throw ProgramExeption("The user does not have permission to modify the file", "Disk::extendfile");

	if (!dskfl.is_open())
		throw ProgramExeption("Files problem", "Disk::extendfile");

	FileHeader fh = getFileHeader(dir);

	allocextend(fh.FAT, sectorSize, Disk::AlgorithmType::first_Fit);

	seekToSector(dir->fileAddr);

	dskfl.write((char *)&fh, sizeof(Sector));




}

void Disk::delfile(string & fileName, string & owner)
{

	if (!this->mounted)
		throw ProgramExeption("There is not mounted disk", "Disk::delfile");

	if (!vhd.isFormated)
		throw ProgramExeption("This disk is not formatted", "Disk::delfile");

	
		

	DirEntry * dir = rootdir[fileName.c_str()];

	if(dir == NULL)
		throw ProgramExeption("File not exist!", "Disk::delfile");

	if (dir->fileOwner != owner)
		throw ProgramExeption("The user does not have permission to modify the file", "Disk::extendfile");

	if (!dskfl.is_open())
		throw ProgramExeption("Files problem", "extendfile");

	FileHeader fh;

	seekToSector(dir->fileAddr);

	dskfl.read((char *)&fh, sizeof(Sector));

	dealloc(fh.FAT);

	seekToSector(dir->fileAddr);

	dir->entryStatus = 2;

	this->flush();
}

DirEntry * Disk::getDir(const char * fileName)
{

	if (!this->mounted)
		throw ProgramExeption("There is not mounted disk", "Disk::getDir");

	if (!vhd.isFormated)
		throw ProgramExeption("This disk is not formatted", "Disk::getDir");


	return rootdir[fileName];

}

FileHeader Disk::getFileHeader(DirEntry * dir)
{
	FileHeader fh;

	seekToSector(dir->fileAddr);

	dskfl.read((char *)&fh, sizeof(Sector));

	return fh;
}

/*************************************************
*
*				  Level 3
*
**************************************************/

FCB * Disk::openfile(string fileName, string userName, enumsFMS::FCBtypeToOpening type)
{
	if (!this->mounted)
		throw ProgramExeption("There is not mounted disk", "Disk::openfile");

	if (!vhd.isFormated)
		throw ProgramExeption("This disk is not formatted", "Disk::openfile");

	DirEntry * fileDir = this->getDir(fileName.c_str());

	if (fileDir == NULL)
		return NULL;

	if (type != enumsFMS::FCBtypeToOpening::output && userName.compare(fileDir->fileOwner))
		throw ProgramExeption("The user is not allowed to write in this file", "Disk::openfile");
	//Find next sector after File Header
	uint firstSec = firstIndex(getFileHeader(fileDir).getFAT(), false, firstIndex(getFileHeader(fileDir).getFAT(), false));

	this->readSector(firstSec, &this->buffer);

	char * key = new char[fileDir->keySize];

	memcpy(key, (buffer.rawData + fileDir->keyOffset), fileDir->keySize);

	return (new FCB(this, *fileDir, getFileHeader(fileDir).getFAT(),
		NULL, 0, 0, type));




}

uint Disk::updateFile(DirEntry file)
{
	if (!this->mounted)
		throw ProgramExeption("There is not mounted disk", "Disk::updateFile");

	if (!vhd.isFormated)
		throw ProgramExeption("This disk is not formatted", "Disk::updateFile");

	*rootdir[file.filename] = file;

	FileHeader fh = getFileHeader(&file);
	fh.fileDesc = file;

	writeSector(file.fileAddr, (Sector *)&fh);

	return file.fileAddr + 1;
}

/*************************************************
*
*				  Functions to Dll
*
**************************************************/

DirEntry * Disk::getDirEntry(int index, SectorDir * sector)
{
	//throw ProgramExeption(to_string( this->rootdir.msbSector.dirEntry[4].entryStatus));

	if (!this->mounted)
		throw ProgramExeption("There is not mounted disk", "Disk::getDirEntry");

	if (!vhd.isFormated)
		throw ProgramExeption("This disk is not formatted", "Disk::getDirEntry");

	if (sector != NULL)
		if (index < 14)
			return &sector->dirEntry[index];
		else
			throw ProgramExeption("Each folder contains max 14 files", "Disk::getDirEntry");

	if (this->rootdir[index] != NULL && this->rootdir[index]->entryStatus == 1)
		return this->rootdir[index];

	return NULL;
}

bool Disk::dirExist(int index, SectorDir * sec)
{

	if (sec != NULL && sec[index].dirEntry->entryStatus == 1)
		return true;

	if (this->rootdir[index] != NULL && this->rootdir[index]->entryStatus == 1)
		return true;

	return false;

}

bool Disk::IsFormated()
{
	return this->vhd.isFormated;
}

