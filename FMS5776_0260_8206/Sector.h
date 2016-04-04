#include "Header.h"
#include "DAT.h"

class Sector
{


private:
	uint sectorNr;
	char rawData[1020];
	void setSectorNr(uint x);


public:
	Sector(uint);
	Sector(DAT *);
	Sector(const void * x);
	Sector();
	~Sector();

	friend class TestLevel_0;
	friend class Disk;





};

