#ifndef _SENDABLE_H
#define _SENDABLE_H

#include <iostream>
#include <list>

class Sendable {
public:
	
	Sendable();
	~Sendable();
	
	void stop();
	
	void pause();
	
	
private:
	
	File file;
	list<String> trackers;
	list<String> seeders;
	double speed;
}

#endif // _SENDABLE_H
