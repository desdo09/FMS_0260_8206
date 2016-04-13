#pragma once

#include <iostream>
#include <string>
using namespace std;

class ProgramExeption : public runtime_error
{
private:
	string msg;
	string source;
	int    exeptionNumber;
public:
	ProgramExeption(string err) : runtime_error(err)
	{
		msg = err;
		int    exeptionNumber = -1;
		source = "Program error";
	}
	ProgramExeption(string err, string source) : runtime_error(err)
	{
		msg = err;
		this->source = source;
		int    exeptionNumber = -1;

	}
	ProgramExeption(string err, int exeptionNumber) : runtime_error(err)
		{
			msg = err;
			this->exeptionNumber = exeptionNumber;
			source = "Program error";
		
		}
	ProgramExeption(string err, string source, int exeptionNumber) : runtime_error(err)
		{
			msg = err;
			this->source = source;
			this->exeptionNumber = exeptionNumber;
		
		}

	const char* what() const throw()
	{
		return (msg.c_str());
	}

	string Getmsg() { return msg; }
	string Getsource() { return source; }
	int    GetexeptionNumber() { return exeptionNumber; }


};





