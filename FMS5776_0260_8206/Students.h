#pragma once
#include "Header.h"



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
		cout << "Id: " << id << endl;
		cout << "Name: " << name << endl;
		cout << "Year: " << year << endl;
		cout << "average: " << average << endl;
	}
};

//int Student::used = 0;