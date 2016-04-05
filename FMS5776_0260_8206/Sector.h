#include "Header.h"

class Sector
{

public:
	Sector(uint);
	Sector();
	~Sector();
	friend class TestLevel_0;
	friend class Disk;
private:
	uint sectorNr;
	char rawData[1020];
	void setSectorNr(uint x);
};

