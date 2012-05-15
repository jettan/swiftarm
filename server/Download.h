#ifndef _DOWNLOAD_H
#define _DOWNLOAD_H

#include "Sendable.h"

class Download : public Sendable {
public:
	
	Download();
	~Download();
	
	void start();
	
	void resume();
	
private:
	
	double percentage;
};
#endif // _DOWNLOAD_H
