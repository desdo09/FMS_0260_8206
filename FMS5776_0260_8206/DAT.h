#include "Header.h"



class DAT
{
private:

	uint    sectorNr;
	DATtype dat;
	char    emptyArea[816];

public:
	DAT();
	~DAT();



	//get
	uint    GetsectorNr() { return sectorNr; }
	DATtype Getdat() { return dat; }
	char *  GetemptyArea(){ return emptyArea; }

	//set
	void    SetsectorNr(uint);
	void    Setdat(DATtype);
	void    SetemptyArea(char *);


	friend class Disk;
};

