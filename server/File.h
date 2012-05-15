#ifndef _FILE_H
#define _FILE_H

#include <String>

class File {
public:
	
	File(String name);
	~File();
	
	void setVisibility(bool visible);
	
	bool isVisible();
	
	void setName(String name);
	
	double getSize();
	
	String getDate();
	
	String getName();
	
private:
	
	String name;
	String date;
	String extension;
	double size;
	bool visible;
}
