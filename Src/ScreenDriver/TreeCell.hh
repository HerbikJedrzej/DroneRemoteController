#pragma once

#include <cstdint>
#include <stdint.h>
#include <cstdlib>

template<class T>
class TreeCell{
public:
	TreeCell(T* _val, TreeCell<T>* const _predestor);
	~TreeCell();
	T* getVal();
	void add(T* val);
	bool add(TreeCell<T>* val);
	TreeCell<T>* getNextOtp();
	TreeCell<T>* getPrevOtp();
	TreeCell<T>* getPointed();
	TreeCell<T>* getPredestor();
	bool incrementOtp();
	bool decrementOtp();
	unsigned int getSize();
private:
protected:
	T* val;
	unsigned int size;
	TreeCell<T>** list;
	unsigned int pionted;
	TreeCell<T>* const predestor; 
};


template<class T>
TreeCell<T>::TreeCell(T* _val, TreeCell<T>* const _predestor):
	val(_val),
	size(0),
	list(nullptr),
	pionted(0),
	predestor(_predestor){}

template<class T>
TreeCell<T>::~TreeCell(){
	if(list){
		for(unsigned int i = 0; i < size; i++)
			delete list[i];
		delete [] list;
	}
	if(val)
		delete val;
}

template<class T>
unsigned int TreeCell<T>::getSize(){
	return size;
}

template<class T>
bool TreeCell<T>::incrementOtp(){
	if(!list)
		return false;
	pionted++;
	if(pionted == size)
		pionted = 0;
	return true;
}

template<class T>
bool TreeCell<T>::decrementOtp(){
	if(!list)
		return false;
	if(pionted == 0)
		pionted = size;
	pionted--;
	return true;
}


template<class T>
TreeCell<T>* TreeCell<T>::getNextOtp(){
	if(!list)
		return nullptr;
	unsigned int tmp = pionted + 1;
	if(tmp == size)
		return list[0];
	return list[tmp];
}

template<class T>
TreeCell<T>* TreeCell<T>::getPrevOtp(){
	if(!list)
		return nullptr;
	return list[
		(pionted == 0)? size - 1 : pionted - 1
	];
}

template<class T>
TreeCell<T>* TreeCell<T>::getPointed(){
	if(!list)
		return nullptr;
	return list[pionted];
}

template<class T>
TreeCell<T>* TreeCell<T>::getPredestor(){
	return predestor;
}

template<class T>
T* TreeCell<T>::getVal(){
	return val;
}

template<class T>
bool TreeCell<T>::add(TreeCell<T>* val){
	if(val->getPredestor() != this)
		return false;
	if(list){
		TreeCell<T>** tmp = new TreeCell<T>*[++size] ; 
		unsigned int i = 0;
		for(; i < size - 1; i++)
			tmp[i] = list[i];
		tmp[i] = val;
		delete [] list;
		list = tmp;
	}
	else{
		list = new TreeCell<T>*[++size] ; 
		list[0] = val;
	}
	return true;
}

template<class T>
void TreeCell<T>::add(T* val){
	if(list){
		TreeCell<T>** tmp = new TreeCell<T>*[++size] ; 
		unsigned int i = 0;
		for(; i < size - 1; i++)
			tmp[i] = list[i];
		tmp[i] = new TreeCell<T>(val, this);
		delete [] list;
		list = tmp;
	}
	else{
		list = new TreeCell<T>*[++size] ; 
		list[0] = new TreeCell<T>(val, this);
	}
}
