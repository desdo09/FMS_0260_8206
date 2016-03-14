#include "Disk.h"

#pragma region constructor
Disk::Disk()
{
	
	currDiskSectorNr = 0;


}
//ctor
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
#pragma endregion



void Disk::createdisk(string & diskName, string & owner) {

	//Open a file
	dskfl.open(diskName, ios::in | ios::out, ios::binary);

	// in case that does open it, throw an exeption
	if (!dskfl.is_open()) {
		throw new exception ("Error to open the file!");
		return;
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
	if(dskfl.is_open())
		dskfl.close();


	dskfl.open(fileName, ios::in | ios::out, ios::binary);

	dskfl.read(buffer, 1024);
	currDiskSectorNr = sizeof(Sector);

	importBufferToObject(&vhd, sizeof(VolumeHeader));

}

void Disk::unmountdisk()
{
}

void Disk::recreatedisk(string &)
{
}

fstream Disk::getdskfl()
{
	return fstream();
}

void Disk::seekToSector(uint)
{
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

void Disk::redSector(Sector *)
{
}

void Disk::importBufferToObject(void * object, int size)
{
	for (int i = 0; i < size && i < 1024; i++)
	{
		*((char *)object + i) = *(buffer+i);
	}
}
