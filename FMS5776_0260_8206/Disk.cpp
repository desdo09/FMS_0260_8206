#include "Disk.h"





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
		throw ProgramExeption("there is a mounted disk");						// If true then throw an exception

	VerifyAndAddExt(diskName);

	if (ifstream(diskName))														// Verify if the directory exist
		throw ProgramExeption("There is another file with the same name");		// If true then throw an exception

	if (dskfl.is_open())														// In case that is another file opened
		dskfl.close();															// close it


	dskfl.open(diskName.c_str(), ios::out | ios::binary);						// Create and open the file with the name receive


	if (!dskfl.is_open()) {														// In case of error
		throw ProgramExeption("Error to open the file!");						// throw an exception
	}

	Sector temp;
	for (int i = 0; i < amountOfSectors * 2; i++)								// Load the number of sectors defined on header into the file
	{
		temp.setSectorNr(i);													// Each sector get his index  
		dskfl.write((char *)&temp, sizeof(Sector));								// write sector into the file (disk)


	}
	//dskfl.flush();

	vhd.SetdiskName(diskName.substr(0, diskName.length() - extensionLenght));	// Set the disk name into volume header

	vhd.SetdiskOwner(owner);													// Set the owner name into volume header

	vhd.Set();
	dat.dat[800] = 0;
	dat.dat[1000] = 0;
	dskfl.seekp(0, ios::beg);													// Return the dskfl to the begin of the file 	
	dskfl.write((char *)&vhd, sizeof(Sector));									// Write the volume header into the disk
	dskfl.write((char *)&dat, sizeof(Sector));									// Write the DAT into the disk
	dskfl.seekp(80 * sizeof(Sector), ios::beg);									// Return the dskfl to the begin of the file 

	dskfl.write((char *)&dat, sizeof(Sector));									// Write the DAT into the disk

	dskfl.close();



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

	if(diskName != "")
		dskfl.open(diskName.c_str(), ios::out | ios::in | ios::binary);			// Create and open the file with the name receive

	dat.resetDat();

	dat.dat[800] = 0;
	dat.dat[1000] = 0;

	seekToSector(0);

	dskfl.write((char *)&vhd, sizeof(Sector));									// Write the volume header into the disk

	dskfl.write((char *)&dat, sizeof(Sector));									// Write the DAT into the disk



	seekToSector(80);							

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
		throw "File cannot be opened";

	this->dskfl.read((char *)&vhd, sizeof(Sector));								// Copy the first sector into volume header object

	this->dskfl.read((char *)&dat, sizeof(Sector));								// Copy the second sector into DAT object

	this->currDiskSectorNr = 2;													// Set current Disk Sector Number as 2

	this->mounted = true;														// The disk is mounted
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
		throw ProgramExeption("There not mounted disk", "WriteSector");

	dskfl.write((char *)&sec, sizeof(sec));

	this->currDiskSectorNr++;													// Update Current Disk Sector Number
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

	dskfl.read((char *)&sec, sizeof(sec));

	this->currDiskSectorNr++;													// Update Current Disk Sector Number

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
uint Disk::firstFit(uint sectoresAmount, uint indexStart = 4)
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

uint Disk::bestFit(uint sectoresAmount, uint indexStart = 4)
{
	int index = -1;
	int temp = sectoresAmount;
	int small = 1600;
	for (int i = indexStart; i < amountOfSectors; i++)
	{
		if(dat.dat[i])
			temp--;

		if (!dat.dat[i] || i == amountOfSectors - 1 )
		{
			if (temp <= 0 && small > temp*(-1))
			{
				small = temp*(-1) - ((i == amountOfSectors - 1)?1:0);
				index = i - sectoresAmount - small;
			}
			temp = sectoresAmount;

		}
		
			
	}

	return index;



}

uint Disk::worstFit(uint sectoresAmount, uint indexStart = 4)
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


/*Public functions*/
void Disk::format(string & owner)
{
	if (!this->mounted)
		throw ProgramExeption("There is not mounted disk", "format");


	if (this->vhd.GetdiskOwner() != owner)
		throw ProgramExeption("This user cannot format this disk", "Format");

//  dat.resetDat();
//	writeSector(1, new Sector(&dat));
	this->recreatedisk();
	vhd.SetformaDate();
	vhd.isFormated = true;
}

int Disk::howmuchempty(uint index = 0)
{
	if(index == 0)
		return dat.dat.count();
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

	if(!vhd.isFormated)
		throw ProgramExeption("This disk is not formatted", "alloc");


	if (howmuchempty() < sectoresAmount)
		throw ProgramExeption("No space left", "alloc");

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
		this->alloc(FAT, 1, algo);
	}
	else {

		for (int i = 0; i < sectoresAmount; i++)
		{
			FAT.set(index + i, 1);
			dat.dat.set(index + i, 0);
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
		this->alloc(FAT, --sectoresAmount, algo);
		this->alloc(FAT, 1, algo);
	}
	else {

		for (int i = 0; i < sectoresAmount; i++)
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
		throw ProgramExeption("There is not disk mounted","flush");				// If true then throw an exception

	if(!dskfl.is_open())
		throw ProgramExeption("Disk file error","flush");

	seekToSector(0);

	dskfl.write((char *)&vhd, sizeof(Sector));									// Write the volume header into the disk

	dskfl.write((char *)&dat, sizeof(Sector));									// Write the DAT into the disk

	seekToSector(80);

	dskfl.write((char *)&dat, sizeof(Sector));									// Write the DAT into the disk

																				//this->mounted = true;														// Disk set as mounted
}



/*************************************************
*
*				  Level 2
*
**************************************************/

void Disk::createfile(string & fileName, string & ownerFile, bool dinamyc , uint regSize, uint sectorSize, string & keyType, uint offset, uint offsetSize)
{
	if (ifstream(fileName))
		throw ProgramExeption("File already exist!", "createfile");

	FileHeader fh;
	//fh.fileDesc.filename = fileName.c_str();

	/* FileHeader
	uint sectorNr;								// Sector number
	DirEntry fileDesc;							// File descriptor
	DATtype FAT;								// FAT
	*/
	/* 
	DirEntry: fileDesc
	char filename[12];          // File name
	char fileOwner[12];			// File Owner
	uint fileAddr;				// File Address (First sector)
	char crDate[10];			// Created date
	uint fileSize;				// File size (Numbers of sectors)
	uint eofRecNr;				// Register number when the file is allocated
	uint maxRecSize;			// Register size max
	uint actualRecSize;			// Register size actual
	char recFormat[2];			//
	uint keyOffset;				//
	uint keySize;				//
	char keyType[2];			// Key Type - (I)nt,(F)loat,(D)ouble,(C)har *
	unsigned short entryStatus;	// 0 - empty 1 - Active 2- inactive
	*/


}