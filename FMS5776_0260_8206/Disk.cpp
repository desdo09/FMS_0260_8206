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


	if(this->mounted)															// Verify if there is a mounted disk
		throw ProgramExeption("there is a mounted disk");						// If true then throw an exception


	if (ifstream(diskName))														// Verify if the directory exist
		throw ProgramExeption ("There is another file with the same name");		// If true then throw an exception

	if (dskfl.is_open())														// In case that is another file opened
		dskfl.close();															// close it
	
	VerifyAndAddExt(diskName);													
	
	dskfl.open(diskName.c_str(),  ios::out| ios::binary);						// Create and open the file with the name receive
	
	
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
	dskfl.seekp(0, ios::beg);													// Return the dskfl to the begin of the file 	
	dskfl.write((char *)&vhd, sizeof(Sector));									// Write the volume header into the disk
	dskfl.write((char *)&dat, sizeof(Sector));									// Write the DAT into the disk
	dskfl.seekp(80* sizeof(Sector), ios::beg);									// Return the dskfl to the begin of the file 
	
	dskfl.write((char *)&dat, sizeof(Sector));									// Write the DAT into the disk

	dskfl.close();


	
	//File doesn't closed
}

void Disk::mountdisk(string  fileName)
{
	if (this->mounted)															
		throw new runtime_error("There another disk mounted");

	if(dskfl.is_open())
		dskfl.close();


	VerifyAndAddExt(fileName);

	dskfl.open(fileName, ios::binary | ios::out | ios::in);
	
	dskfl.seekp(0, ios::beg);													// Return the dskfl to the begin of the file 	

	if(!dskfl.good())
			throw "File cannot be opened";

	this->dskfl.read((char *)&vhd, sizeof(Sector));								// Copy the first sector into volume header object

	this->dskfl.read((char *)&dat, sizeof(Sector));								// Copy the second sector into DAT object

	this->currDiskSectorNr = 2;													// Set current Disk Sector Number as 2

	this->mounted = true;														// The disk is mounted
}

void Disk::unmountdisk()
{
	if(!this->mounted)														
		throw ProgramExeption("No disk mounted");

	if (!dskfl.is_open())
		throw ProgramExeption("Disk problems, file are not opened");

	seekToSector(0);

	dskfl.write((char *)&vhd, sizeof(Sector));									// Update the volume header

	dskfl.write((char *)&dat, sizeof(Sector));									// Update the DAT

	this->mounted = false;														
	
	dskfl.close();																// Close file

}

void Disk::recreatedisk(string diskName)
{
	if (this->mounted)															// Verify if there is a mounted disk
		throw "there is a mounted disk";										// If true then throw an exception

	if (!ifstream(diskName))													// Verify if the directory exist
		throw "Disk not found";													// If not exist then throw an exception

	if (dskfl.is_open())														// In case that is another file opened
		dskfl.close();															// close it

	dskfl.open(diskName.c_str(), ios::out | ios::in |ios::binary);				// Create and open the file with the name receive

	seekToSector(0);															
	
	dskfl.write((char *)&vhd, sizeof(Sector));									// Write the volume header into the disk

	dskfl.write((char *)&dat, sizeof(Sector));									// Write the DAT into the disk

	dskfl.flush();

	this->mounted = true;														// Disk set as mounted

}

fstream * Disk::getdskfl(void)
{

	if(this->dskfl.is_open())
		return &dskfl ;

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

	dskfl.write((char *) &sec, sizeof(sec));

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
uint Disk::firstFit(uint sectoresAmount)
{
	int index = -1;
	int temp = sectoresAmount;
	for (int i = 4; i < amountOfSectors; i++)
	{
		if (!dat.dat[i])
			temp = sectoresAmount;
		else
			temp--;

		if (!temp)
		{
			index = i - sectoresAmount;
			break;
		}
	}
	return index;
}
 
uint Disk::bestFit(uint sectoresAmount)
{
	int index = -1;
	int temp = sectoresAmount;
	int small = 1600;
	for (int i = 4; i < amountOfSectors; i++)
	{
		if (!dat.dat[i] || i == amountOfSectors -1)
		{
			if (temp <= 0 && small>temp*(-1))
			{
				small = temp*(-1);
				index = i - sectoresAmount - small;
			}
			temp = sectoresAmount;

		}
		else
			temp--;
	}

	return index;

	

}

uint Disk::worstFit(uint sectoresAmount)
{
	int index = -1;
	int temp = sectoresAmount;
	int big =0;
	for (int i = 4; i < amountOfSectors; i++)
	{
		if (!dat.dat[i] || i == amountOfSectors - 1)
		{
			if (temp <= 0 && big<temp*(-1))
			{
				big = temp*(-1);
				index = i - sectoresAmount - big;
			}
			temp = sectoresAmount;

		}
		else
			temp--;
	}

	return index;
}


/*Public functions*/
void Disk::format(string & owner)
{
	if (!this->mounted)
		throw ProgramExeption("There is not mounted disk", "format");

	if(this->vhd.GetdiskOwner() != owner)
		throw ProgramExeption("This user cannot format this disk", "Format");

	dat.resetDat();
	writeSector(1,new Sector(&dat));

}

int Disk::howmuchempty()
{
	return dat.dat.count();
	/*
	if (!this->mounted)
		throw ProgramExeption("There is not mounted disk", "howmuchempty");
	int count;
	for (int i = 0; i < amountOfSectors; i*=2)
	{
		if (!dat.dat[i] & !dat.dat[i + 1])
			count++;
	}
	return count;
	*/
}

void Disk::alloc(DATtype & FAT, uint sectoresAmount, AlgorithmType algo)
{
	if (!this->mounted)
		throw ProgramExeption("There is not mounted disk", "howmuchempty");

	if(howmuchempty()<sectoresAmount)
		throw ProgramExeption("No space left", "howmuchempty");

	int index =0;

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
		this->alloc(FAT, sectoresAmount--, algo);
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
		throw ProgramExeption("There is not mounted disk", "howmuchempty");

	if (howmuchempty()>sectoresAmount)
		throw ProgramExeption("No space left", "howmuchempty");
	int index;

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
	
	for (int i = 0; i < sectoresAmount; i++)
	{
		FAT.set(index + i, 0);
		dat.dat.set(index + i, 1);
	}
}

void Disk::dealloc(DATtype & FAT)
{
	for (int i = 0; i < amountOfSectors; i++)
		if (FAT[i])
			dat.dat[i] = 1;
}
