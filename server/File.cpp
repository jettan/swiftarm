#include "File.h"

File(String name){
	this.name = name
	// set date
	// set size
	visibile = true;
}

~File(){
	
}

void setVisibility(bool visible){
	this.visibility = visible;
}

bool isVisible(){
	return visible;
}

void setName(String n){
	this.name = name;
}

double getSize(){
	return size;
}

String getDate(){
	return date;
}

String getName(){
	return name;
}
