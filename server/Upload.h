#ifndef _UPLOAD_H
#define _UPLOAD_H

#include "Sendable.h"

class Upload : public Sendable {
public:
	
	Upload();
	~Upload();
	
	void start();
	
	void resume();
	
private:
	
	double amount;
};
#endif // _UPLOAD_H
