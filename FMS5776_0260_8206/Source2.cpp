#include "Header.h"
#include "ExternalFile.h"
#include "Disk.h"
#include "FCB.h"
#include "DirEntry.h"
#include "Dir.h"

 class Student
{

public:
	int _id;
		
	char _Name[20];
	

	int _year;


	float _average;
	
	void printStudent()
	{
		cout << _id << ": " << _Name << endl;
	}
	Student(int id, string name, int year, float average)
	{
		this->_id = id;
		memcpy(this->_Name,name.c_str(),20);
		this->_year = year;
		this->_average = average;
	}
	Student()
	{

	}
 };


void main()
{
	try 
	{
		Disk d;
		FCB * fcb;
		ExternalFile * file;
	
		cout << sizeof(FileHeader) << endl;
		cout << sizeof(SectorDir)<<endl;
		cout << sizeof(DirEntry) << endl;

		d.mountdisk("C:\\Users\\User\\Desktop\\TheDisk.bin");
		
		fcb = d.openfile("a.stud", "David", enumsFMS::FCBtypeToOpening::inputOutput);
		if (fcb == NULL)
		{
			cout << "Fcb error" << endl;
			system("Pause");
			return;
		}
		char ** allData;
		Student * stud ;

		for (int i = 1; i <= 10; i++)
		{
			cout << "Total rec in file --> " << fcb->fileTotalRec() << endl;
			stud = new Student(i, ((string)"Name " + to_string(i)), i % 6, rand() % 100);
			fcb->write((char *)stud);
			delete stud;
		}

		allData = fcb->getAllFile();
		for (int i = 0; i < fcb->fileTotalRec(); i++)
		{
		
			if (allData[i] != NULL)
			{
				stud = new Student();
				memcpy(stud, allData[i], sizeof(Student));
				stud->printStudent();
				delete stud;
			}
		}
	

	}
	catch (ProgramExeption ex)
	{
		cout << ex.Getsource() << ": " << ex.what()<<endl;
	}
	catch (exception ex)
	{
		cout << ex.what() << endl;
	}
	
	system("Pause");

}