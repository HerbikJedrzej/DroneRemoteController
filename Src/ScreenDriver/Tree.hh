#pragma once

#include <cstdint>
#include <stdint.h>
#include <cstdlib>
#include <TreeCell.hh>

template<class T>
class Tree{
public:
	Tree(TreeCell<T>* _root);
	~Tree();
	TreeCell<T>* getCell();
	bool incrementOtp();
	bool decrementOtp();
	bool incrementDir();
	bool decrementDir();
protected:
	TreeCell<T>* pointed;
	TreeCell<T>* root;
};

template<class T>
Tree<T>::Tree(TreeCell<T>* _root):
	pointed(_root),
	root(_root){}

template<class T>
Tree<T>::~Tree(){
	delete root;
}

template<class T>
TreeCell<T>* Tree<T>::getCell(){
	return pointed;
}

template<class T>
bool Tree<T>::incrementOtp(){
	return pointed->incrementOtp();
}

template<class T>
bool Tree<T>::decrementOtp(){
	return pointed->decrementOtp();
}

template<class T>
bool Tree<T>::incrementDir(){
	if(!pointed->getPointed())
		return false;
	pointed = pointed->getPointed();
	return true;
}

template<class T>
bool Tree<T>::decrementDir(){
	if(!pointed->getPredestor())
		return false;
	pointed = pointed->getPredestor();
	return true;
}

