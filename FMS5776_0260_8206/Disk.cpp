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
* RETURN VALUE
*	This function does not return parameters
*
* PARAMETERS
*    diskName – The name of the disk that will be created
*	 owner    - The disk owner name
*
* MEANING
*     This functions will create a new disk in a few steps:
*			1. Create the file and insert all sectors
*			2. Create the volume header and insert it
*			3. Create the DAT and insert it
*
*	 Important: The function make a new disk just in case:
*		   1. There is not another disk mounted
*		   2. The file doesn't exist 
*                 
* SEE ALSO
*	  mountdisk function 
*
**************************************************/

void Disk::createdisk(string & diskName, string & owner) {


	if(this->mounted)															// Verify if there is a mounted disk
		throw new exception("there is a mounted disk");							// If true then throw an exception

	diskName += diskType;														// Add the extension of the file (disk)  

	if (ifstream(diskName))														// Verify if the directory exist
		throw new exception("There is another file with the same name");	    // If true then throw an exception

	if (dskfl.is_open())														// In case that is another file opened
		dskfl.close();															// close it

	dskfl.open(diskName.c_str(),  ios::out, ios::binary);						// Create and open the file with the name receive
	
	
	if (!dskfl.is_open()) {														// In case of error
		throw new exception ("Error to open the file!");						// throw an exception
	}
	
	for (int i = 0; i < amountOfSectors; i++)									// Load the number of sectors defined on header into the file
	{
		Sector temp;
		temp.sectorNr = i;														// Each sector get his index  
		dskfl.write((char *)&temp, sizeof(Sector));								// write sector into the file (disk)
	}

	dskfl.flush();
	
	dskfl.seekp(ios::beg);														// Return the dskfl to the begin of the file 	
											/* Setting Volume Header */
	
	vhd.sectorNr = 0;															// Volume Header sector index 
	diskName.copy(vhd.diskName, 12);											// The disk name
	owner.copy(vhd.diskOwner, 10);												// The disk owner
	_strdate(vhd.prodDate);														// The current date
	vhd.isFormated = false;														// set isFormanted object as false
	strcpy(vhd.formatDate, " ");												// set the formated date as empty
	strcpy(vhd.emptyArea, " ");													// set the empty area empty
	vhd.clusQty = amountOfSectors;												// Sector numbers existing
	vhd.dataClusQty = amountOfSectors - 4;										// Sector numbers for data only
	vhd.addrDAT = 1;															// The sector index when is allocate the DAT
	vhd.addrRootDir = 1;														// The sector index when is allocate the root directory
	vhd.addrRootDirCpy = 1000;													// The root directory address
	vhd.addrDATcpy = 800;														// The DAT address
	vhd.addrDataStart = 2;														// The first sector for data

													/* end */
	dskfl.write((char *)&vhd, sizeof(Sector));									// Write the volume header into the disk
	dskfl.flush();

											/* Setting DAT */

	dat.sectorNr = 1;															// DAT sector index 		
	dat.dat.set() ;																// Set all bits 1
	for (int i = 0; i < 4; i++)													// Set The first 4 bits 0 
		dat.dat.set(i, 0);
														/* end */

	dskfl.write((char *)&dat, sizeof(Sector));									// Write the DAT into the disk

	//File doesn't closed 
}

/*************************************************
* FUNCTION
*    mountdisk
*
* PARAMETERS
*   This function does not receive parameters
*
* RETURN VALUE
*	This function does not return parameters
*
* PARAMETERS
*    FileName – The file name of the disk
*
* MEANING
*     This functions mount a disk and load the  
*	  volume header and the DAT 
*
*	 Important: The function mount the disk just in case:
*		   1. There is not another disk mounted
*		   2. The disk exist
*
* SEE ALSO
*	  unmountdisk function
*
**************************************************/

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

/*************************************************
* FUNCTION
*    unmountdisk
*
* PARAMETERS
*   This function does not receive parameters
*
* RETURN VALUE
*	Returns the fstream object that opened the file
*
* MEANING
*     This functions unmount this disk and unload
*	  the volume header and the DAT
*
*	 Important: The function unmount the disk just in case:
*		   1. The disk is mounted
*
* SEE ALSO
*	  unmountdisk function
*
**************************************************/
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
