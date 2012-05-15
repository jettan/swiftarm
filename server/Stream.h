#ifndef _STREAM_H
#define _STREAM_H

#include "Sendable.h"

class Stream : public Sendable {
public:
	
	Stream();
	~Stream();
	
	void play();
	
private:
	
	double amount;
};
#endif // _STREAM_H
