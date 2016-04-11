#include "VolumeHeader.h"

VolumeHeader::VolumeHeader()
{
	
	
}


VolumeHeader::~VolumeHeader()
{
}


#pragma region set

void VolumeHeader::SetdiskName(string name)
{
	memcpy(diskName, name.c_str(),12);
}

void VolumeHeader::SetdiskOwner(string name)
{
	memcpy(diskOwner, name.c_str(), 12);
}

void VolumeHeader::SetprodDate(string date)
{
	memccpy(diskName, date.c_str(), 0, 12);
}

void VolumeHeader::SetformaDate()
{
	_strdate(formatDate);
}

void VolumeHeader::SetisFormated(bool flag)
{
	this->isFormated = flag;
}

void VolumeHeader::Set()
{
	this->sectorNr = 0;														     	// Volume Header sector index 								
	_strdate(this->prodDate);														// The current date
	this->isFormated = false;														// set isFormanted object as false
	strcpy(this->formatDate, " ");												    // set the formated date as empty
	strcpy(this->emptyArea, " ");													// set the empty area empty
	this->clusQty = amountOfSectors;												// Sector numbers existing
	this->dataClusQty = amountOfSectors - 4;										// Sector numbers for data only
	this->addrDAT = 1;															    // The sector index when is allocate the DAT
	this->addrRootDir = 1;														    // The sector index when is allocate the root directory
	this->addrRootDirCpy = 1000;													// The root directory address
	this->addrDATcpy = 800;														    // The DAT address
	this->addrDataStart = 2;														// The first sector for data
}



#pragma endregion

