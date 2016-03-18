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
	dskfl.close();
}





void Disk::createdisk(string diskName, string owner) {


	if(this->mounted)															// Verify if there is a mounted disk
		throw ProgramExeption("there is a mounted disk");						// If true then throw an exception


	if (ifstream(diskName))														// Verify if the directory exist
		throw ProgramExeption ("There is another file with the same name");		// If true then throw an exception

	if (dskfl.is_open())														// In case that is another file opened
		dskfl.close();															// close it
	
	VerifyAndAddExt(diskName);													
	
	dskfl.open(diskName.c_str(),  ios::out, ios::binary);						// Create and open the file with the name receive
	
	
	if (!dskfl.is_open()) {														// In case of error
		throw ProgramExeption("Error to open the file!");						// throw an exception
	}
	
	for (int i = 0; i < amountOfSectors; i++)									// Load the number of sectors defined on header into the file
	{
		Sector temp(i);															// Each sector get his index  
		dskfl.write((char *)&temp, sizeof(Sector));								// write sector into the file (disk)
	}

	dskfl.flush();
	
	dskfl.seekp(ios::beg);														// Return the dskfl to the begin of the file 	
										
	vhd.SetdiskName(diskName.substr(0,diskName.length() - extensionLenght));	// Set the disk name into volume header

	vhd.SetdiskOwner(owner);													// Set the owner name into volume header

	vhd.Start();

	dskfl.write((char *)&vhd, sizeof(Sector));									// Write the volume header into the disk

	dskfl.write((char *)&dat, sizeof(Sector));									// Write the DAT into the disk

	dskfl.flush();

	
	//File doesn't closed
}



void Disk::mountdisk(string  fileName)
{
	if (this->mounted)															
		throw new runtime_error("There another disk mounted");

	if(dskfl.is_open())
		dskfl.close();

	VerifyAndAddExt(fileName);

	this->dskfl.open(fileName, ios::in, ios::binary);

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
		throw new exception("No disk mounted");

	if (!dskfl.good())
		throw new exception("Disk problems, file are not opened");

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

	dskfl.open(diskName.c_str(), ios::out, ios::binary);						// Create and open the file with the name receive

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
