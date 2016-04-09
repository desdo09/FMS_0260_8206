#pragma once
#include "Disk.h"
#include "TestLevel_0.h"

enum { testLevel0 = 1, createDisk, mountDisk, unmountDisk, printDiskDetails, printDat, alocate, dealocate, clear };

DATtype testLevel1(Disk * d);
 void DATprint(DATtype DAT,char type = 'D');

void main() {
	char choice[3];
	TestLevel_0 a;
	Disk * disk = &a.d;
	DATtype FAT;
	do {
		try
		{
		
		


			cout << "Select option:" << endl;
			cout << "1 - Test level 0" << endl;
			cout << "2 - Create disk" << endl;
			cout << "3 - Mount Disk" << endl;
			cout << "4 - Unmount Disk" << endl;
			cout << "5 - Print disk details" << endl;
			cout << "6 - Print DAT" << endl;
			cout << "7 - Allocate sectors" << endl;
			cout << "8 - Deallocate last FAT" << endl;
			cout << "9 - Clear screen" << endl;
			cout << "0 - Exit" << endl;

			cout << "your choice: ";


			cin >> choice;



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
			case printDiskDetails:
				a.printDiskInfo();
				break;
			case testLevel0:
				a.test_0();
				break;
			case alocate:
				FAT = testLevel1(disk);
				break;
			case dealocate:
				disk->dealloc(FAT);
				cout << "deallocated! " << endl;
				break;
			case printDat:
				DATprint(disk->getDatDAt());
				break;

			case clear: system("cls"); break;
			default:
				break;
			}





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
	} while (choice != 0);

	system("pause");
}

DATtype testLevel1(Disk * d)
{
	short algo;
	int amount;
	DATtype FAT;
	cout << "Insert amount of sector to allocate:" << endl;
	cin >> amount;
	cout << "Select algorithm type" << endl;
	do {
		cout << "<1>First fit <2> Best Fit <3> Worst Fit" << endl;
		cin >> algo;
	} while (algo <= 0 && algo > 4);
	d->alloc(FAT, amount, (Disk::AlgorithmType)algo);
	cout << "FAT:" << endl;
	DATprint(FAT,'F');

	return FAT;
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