#pragma once
#include "Disk.h"
#include "TestLevel_0.h"
#include "TestLevel_2.h"

enum menu{	clear = 1,																			 // Others
			testLevel0 , createDisk, mountDisk, unmountDisk, flush, printDiskDetails,            // Level 0
			printDat, formatDisk, allocate, allocateext, deallocate,							 // Level 1
			createFile, getFile, extendFile, ShowFileFAT, deleteFile							 // Level 2
		 };

void testLevel1(Disk * d, DATtype & FAT, bool ext = false);
void DATprint(DATtype DAT,char type = 'D');

void main() {
	//system("color a");
	char choice[3],ans;
	TestLevel_0 a;
	Disk * disk = &a.d;
	DATtype FAT;
	for (int i = 25; i < 50; i++)
	{
		FAT[i] = 1;
	}
	do {
		try
		{
		
		
			
			cout << "Select option:" << endl;
			cout << "\n\t\t<100> Exit " << endl;
			cout << "\t\t<1> Clear screen " << endl;
			cout << "\n\tLevel 0:" << endl;
			cout << "\t\t<2> Test level 0 " << endl;
			cout << "\t\t<3> Create disk " << endl;
			cout << "\t\t<4> Mount Disk " << endl;
			cout << "\t\t<5> Unmount Disk " << endl;
			cout << "\t\t<6> Release data into the Disk " << endl;
			cout << "\t\t<7> Print disk details " << endl;
			cout << "\n\tLevel 1:" << endl;
			cout << "\t\t<8> Print DAT " << endl;
			cout << "\t\t<9> Format Disk " << endl;
			cout << "\t\t<10> Allocate sectors " << endl;
			cout << "\t\t<11> Allocate last FAT " << endl;
			cout << "\t\t<12> Deallocate last FAT " << endl;
			cout << "\n\tLevel 2:" << endl;
			cout << "\t\t<13> Create new file " << endl;
			cout << "\t\t<14> Get file header" << endl;
			cout << "\t\t<15> Extend File " << endl;
			cout << "\t\t<16> Get FAT from a file " << endl;
			cout << "\t\t<17> Delete File " << endl;

			cout << "\nYour choice: [    ]\b\b\b\b";


			cin >> choice;


			cout << "**************************************************" << endl;
			switch (atoi(choice))
			{
			case createDisk:
				a.test_create();
				break;
			case mountDisk:
				a.test_mount();
				break;
			case unmountDisk:
				disk->unmountdisk();
				cout << "unmounted!" << endl;
				break;
			case menu::flush:
				disk->flush();
				break;
			case formatDisk:
				cout << "are you sure <Y/N>? " << endl;
				cin >> ans;
				if (ans == 'Y' || ans == 'y')
				{
					disk->format(a.ownerName);
					cout << "Disk formated!" << endl;
				}
				break;
			case printDiskDetails:
				a.printDiskInfo();
				break;
			case testLevel0:
				a.printStructSize();
				break;
			case allocate:
				FAT.reset();
				testLevel1(disk,FAT);
				break;
			case allocateext:
				testLevel1(disk, FAT,true);
				break;
			case deallocate:
				disk->dealloc(FAT);
				cout << "deallocated! " << endl;
				break;
			case printDat:
				if (disk->getMounted())
					DATprint(disk->getDatDAt());
				else
					cout << "There is not a mounted disk" << endl;
				break;
			case createFile: 
				TestLevel_2::createFile(disk);
				break;
			case extendFile:
				TestLevel_2::extFile(disk);
				break;
			case getFile:
				TestLevel_2::printHeader(disk);
				break;
			case deleteFile:
				TestLevel_2::DeleteFile(disk);
				break;
			case ShowFileFAT:
				FAT = TestLevel_2::getFAT(disk);
				DATprint(FAT,'F');
				break;
			case clear: std::system("cls"); break;
			default:
				// std::system("cls");
				break;
			}


		
			cin.get();


		}
		catch (exception ex)
		{
			cout << ex.what() << endl;
		}
		catch (char * msg)
		{
			string err(msg);
			cout << err << endl;
		}
		catch (...)
		{
			cout << "Error" << endl;
		}

		if(atoi(choice) != 1 )
			cout << "**************************************************" << endl;


	} while (atoi(choice) != 100);

	std::system("pause");

	
}

void testLevel1(Disk * d, DATtype & FAT ,bool ext)
{
	short algo;
	int amount;
	cout << "Insert amount of sector to allocate:" << endl;
	cin >> amount;
	cout << "Select algorithm type" << endl;
	do {
		cout << "<1>First fit <2> Best Fit <3> Worst Fit" << endl;
		cin >> algo;
	} while (algo <= 0 && algo > 4);
	if (ext)
		d->allocextend(FAT, amount, (Disk::AlgorithmType)algo);
	else
		d->alloc(FAT, amount, (Disk::AlgorithmType)algo);

	cout << "FAT:" << endl;
	DATprint(FAT,'F');

	
}

void DATprint(DATtype DAT, char type)
{
	
	for (int i = 0; i < amountOfSectors; i++)
	{
		if (type == 'D' && DAT[i] || type != 'D' && !DAT[i])
			cout << (char)219;
		else
			cout << (char)176;

		cout << '|';

		if (!((i + 1) % 50))
			cout << endl;
	}
	cout << endl;

}