#ifndef _FILEMNGR_H
#define _FILEMNGR_H

#include <vector>
#include "File.h"

class FileManager {
public:
	
	void remove(File file);
	
	void add(File file);
	
	void rename(File file, String name);
	
	void sortBy(String sortType);
	
	File getFile(String name);
	
private:
	
	list<File> files;
}

#endif // _FILEMNGR_H
