#include "FileManager.h"


FileManager(){
	
}

~FileManager(){
	
}

void remove(File file){
	files.remove(file);
}

void add(File file){
	files.push_back(file);
}

File getFile(String name){
	for(int i = 0; i < files.size(); i++){
		if(files[i].getName().strcmp(name) == 0){
			return files[i];
		}
	}
	return null;
}

void rename(String filename, String name){
	getFile(filename).setName(name);
}

