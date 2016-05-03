#pragma once
#include "Header.h"
#include "Disk.h"

class TestLevel_0
{
	
public:

	string diskName = "TheDisk";
	string ownerName = "Da&Jo";
	Disk d;

	TestLevel_0()
	{

	}

	TestLevel_0(string name,string owner)
	{
		string diskName = name;
		string ownerName = owner;
	}

	

	void printStructSize()
	{
		cout << "start" << endl;
		cout << "Size Of Disk -->" << sizeof(Disk) << endl;
		cout << "Size Of Sector -->" << sizeof(Sector) << endl;
		cout << "Size Of VolumeHeader -->" << sizeof(VolumeHeader) << endl;
		cout << "Size Of DAT -->" << sizeof(DAT) << endl;
		cout << "Size Of DirEntry -->" << sizeof(DirEntry) << endl;
		cout << "Size Of SectorDir -->" << sizeof(SectorDir) << endl;
		cout << "Size Of FileHeader -->" << sizeof(FileHeader) << endl;
	//	cout<< "Size Of DAT -->" << sizeof(DAT) << endl;
		cout << "Size Of RootDir -->" << sizeof(RootDir) << endl;
	}

	void printDiskInfo()
	{
		VolumeHeader* vh = &d.vhd;

		cout << "	disk name:        " << vh->diskName << endl;
		cout << "	Owner Name:       " << vh->diskOwner << endl;
		cout << "	prodDate:         " << vh->prodDate << endl;
		cout << "	formatDate:       " << vh->formatDate << endl;
		cout << "	isFormated:       " << vh->isFormated << endl;
		cout << "	addrDataStart:    " << vh->addrDataStart << endl;

		cout << "	ClusQty:          " << vh->clusQty << endl;
		cout << "	dataClusQty:      " << vh->dataClusQty << endl;

		cout << "	addrDAT:          " << vh->addrDAT << endl;
		cout << "	addrRootDir:      " << vh->addrRootDir << endl;
		cout << "	addrDATcpy:       " << vh->addrDATcpy << endl;
		cout << "	addrRootDirCpy:   " << vh->addrRootDirCpy << endl << endl;

	}

	void test_create()
	{
	
	//	cout << "\npre createdisk: " << endl;
		//printDiskInfo();
	//	cout << "post createdisk: " << endl;
		d.createdisk(diskName, ownerName);
	//	printDiskInfo();
		cout << "Disk created!" << endl;
	}

	void test_mount()
	{

		//cout << "\npre mountdisk: " << endl;
	//	printDiskInfo();
		this->d.mountdisk(diskName);
	//	cout << "post mountdisk: " << endl;
	//	printDiskInfo();
	//	d.unmountdisk();
		cout << "mounted!" << endl;
	}


	void test_rwSector()
	{

		Sector sector;
		d.mountdisk(diskName);

		cout << "\nread sector: " << endl;
		d.readSector(8, &sector);
		strcpy_s(sector.rawData, "this is write temp sector");
		d.writeSector(8, &sector);
		d.unmountdisk();

	}


	void test_0()
	{

		

		printStructSize();
		test_create();
		test_mount();

	}
};
