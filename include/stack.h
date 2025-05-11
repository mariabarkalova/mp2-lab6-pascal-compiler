#pragma once
#ifndef _Stack_H_
#define _Stack_H_

#include <iostream>
#include <vector>
//using namespace std;

template<typename T>
class TVectorStack {
	int top; 
	std::vector<T> mem;
public:
	TVectorStack() : top(-1) {}
	size_t size() const { return top + 1; } 
	bool IsEmpty() const { return top == -1; } 
	void Push(const T& val) { 
		mem.push_back(val); top++;
	}
	T Pop() { 
		if (IsEmpty()) {
			throw std::out_of_range("try to get element from empty stack");
		}
		T val = mem.back();
		mem.pop_back(); top--; return val;
	}
	T Top() { 
		if (IsEmpty()) {
			throw std::out_of_range("try to check element from empty stack");
		}
		return mem.back();
	}
};

#endif

