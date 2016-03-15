#include "Disk.h"

Disk::Disk()
{
	
	currDiskSectorNr = 0;


}
//Constructor
Disk::Disk(string & fileName, string & Owner, diskCod cod)
{
	switch (cod)
	{
	case create:
		createdisk(fileName, Owner);
		break;
	case mount:
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



/*************************************************
* FUNCTION
*    createdisk
*
* PARAMETERS
*    diskName – The name of the disk that will be created
*	 owner    - The disk owner name
*
* MEANING
*     This functions will create a new Disk in a few steps:
*			1. Create the file and insert all sectors
*			2. Create the volume header and insert it
*			3. Create the DAT and insert it
*                 
* SEE ALSO
*	  mountdisk function 
*
**************************************************/

void Disk::createdisk(string & diskName, string & owner) {

	if (dskfl.is_open())
		dskfl.close();


	diskName += ".bin";
//	if(!ifstream(diskName))
	//Open a file
	dskfl.open(diskName.c_str(), ios::in | ios::out, ios::binary);
	
	// in case that doesn't open it, throw an exception
	if (!dskfl.is_open()) {
		throw new exception ("Error to open the file!");
	}
	
	for (int i = 0; i < 1600; i++)
	{
		Sector temp;
		temp.sectorNr = i;
		dskfl.write((char *)&temp, sizeof(Sector));
	}

	dskfl.flush();

	dskfl.seekp(ios::beg);
	vhd.sectorNr = 0;
	diskName.copy(vhd.diskName, 12);
	owner.copy(vhd.diskOwner, 10);
	_strdate(vhd.prodDate);
	vhd.isFormated = false;
	strcpy(vhd.formatDate, " ");
	strcpy(vhd.emptyArea, " ");
	vhd.clusQty = 1600;
	vhd.dataClusQty = 1596;
	vhd.addrDAT = 1;
	vhd.addrRootDir = 1;
	vhd.addrDataStart = 0;
	vhd.addrDATcpy = 800;
	vhd.addrRootDirCpy = 1000;


	dskfl.write((char *)&vhd, sizeof(Sector));
	dskfl.flush();



	dat.sectorNr = 1;
	dat.dat.set() ;
	for (int i = 0; i < 4; i++)
		dat.dat.set(i, 0);
	dskfl.write((char *)&dat, sizeof(Sector));


}

void Disk::mountdisk(string & fileName)
{
	if (this->mounted)
		throw new exception("There another disk mounted already");

	if(dskfl.is_open())
		dskfl.close();


	this->dskfl.open(fileName, ios::in | ios::out, ios::binary);

	this->dskfl.read(buffer, 1024);
	this->currDiskSectorNr = 1;

	importBufferToObject(&vhd, sizeof(VolumeHeader));

	this->dskfl.read(buffer, 1024);
	this->currDiskSectorNr = 2;

	importBufferToObject(&dat, sizeof(DAT));

	this->mounted = true;
}

void Disk::unmountdisk()
{
	if (!dskfl.is_open())
		throw new exception("No disk mounted");

	dskfl.seekp(ios::beg);

	dskfl.write((char *)&vhd, sizeof(Sector));

	dskfl.write((char *)&dat, sizeof(Sector));

	this->currDiskSectorNr = 0;

	this->mounted = false;

	dskfl.close();

}

void Disk::recreatedisk(string &)
{

}

/*************************************************
* FUNCTION
*    getdskfl
* PARAMETERS
*   The function does not get a parameter
* RETURN VALUE
*	Returns the fstream object that opened the file    
*
* MEANING
*     The function will verify if there a file opened
*	  if yes the function return the fstream object
*	  if not the function return a nullptr
*
***************************************************/
fstream * Disk::getdskfl(void)
{

	if(this->dskfl.is_open())
		return &dskfl ;

	return nullptr;
}

void Disk::seekToSector(uint number)
{
	this->dskfl.seekg(ios::beg);
	this->dskfl.seekg(ios::beg);

	this->dskfl.seekg(number * sizeof(Sector));
	this->dskfl.seekg(number * sizeof(Sector));
}

void Disk::writeSector(uint, Sector *)
{
}

void Disk::writeSector(Sector *)
{
}

void Disk::readSector(int, Sector *)
{
}

void Disk::readSector(Sector *)
{
}

void Disk::importBufferToObject(void * object, int size)
{
	for (int i = 0; i < size && i < 1024; i++)
	{
		*((char *)object + i) = *(buffer+i);
	}
}
