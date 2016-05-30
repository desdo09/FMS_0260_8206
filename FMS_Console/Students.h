#pragma once
#include "Header.h"
#include <time.h>
 


class Student
{
public:
	int id;
	char name[20];
	int year;
	float average;

	static int used;

	

	void setStudent()
	{
		id = ++used;
		cout << "id = " << id << endl;
		cout << "Enter name" << endl;
		cin >> name;
		cout << "Enter year" << endl;
		cin >> year;
		cout << "Enter average" << endl;
		cin >> average;
	}
	void updateStudent()
	{
		cout << "id = " << id << endl;
		cout << "Enter name" << endl;
		cin >> name;
		cout << "Enter year" << endl;
		cin >> year;
		cout << "Enter average" << endl;
		cin >> average;
	}

	void printStudent()
	{
		cout << "Id: " << id;
		cout << "  Name: " << name;
		cout << "  Year: " << year;
		cout << "  average: " << average << endl;
	}

	static	Student newStudentTest()
	{
		int number = rand() % 9999 + 1;
		Student toReturn;
		string name = "Name ";

		name += to_string(number);

		toReturn.id = number;

		memcpy(toReturn.name, name.c_str(), 20);

		toReturn.year = rand() % 6 + 1;

		toReturn.average = rand() % 101;
		
		return toReturn;
	}
};

//int Student::used = 0;