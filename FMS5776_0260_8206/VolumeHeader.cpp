#include "VolumeHeader.h"

VolumeHeader::VolumeHeader()
{
	sectorNr = 0;
	clusQty = 0;
	dataClusQty =0;
	addrDAT = 0;
	addrRootDir = 0;
	addrDataStart = 0;
	isFormated = false;
	
}


VolumeHeader::~VolumeHeader()
{
}


#pragma region set

void VolumeHeader::SetdiskName(string)
{
}

void VolumeHeader::SetdiskOwner(string)
{
}

void VolumeHeader::SetprodDate(string)
{
}

void VolumeHeader::SetformaDate(string)
{
}


void VolumeHeader::SetemptyArea(char *)
{
}

void VolumeHeader::SetsectorNr(uint)
{
}

void VolumeHeader::SetclusQty(uint)
{
}

void VolumeHeader::SetdataClusQty(uint)
{
}

void VolumeHeader::SetaddrDAT(uint)
{
}

void VolumeHeader::SetaddrRootDir(uint)
{
}

void VolumeHeader::SetaddrDataStart(uint)
{
}

void VolumeHeader::SetisFormated(uint)
{
}

#pragma endregion

