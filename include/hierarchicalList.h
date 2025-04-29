#pragma once
#include <string>
//#include "Stack.h" // здесь ваш стек

template<typename T>
class TStack {
public:
	TStack() {}
};

class HierarchicalList
{
private:
  struct Node {
    std::string data;
    Node* next;
	Node* down;
    Node(std::string x): data(x), next(nullptr), down(nullptr) {}
  };

//////////////////////////////////////////////////////////////////////  
  Node* root = nullptr;

public:
  class hl_iterator; 

  HierarchicalList() {}

  void Load(std::string path) // загрузка из файла
  {  
  }

  friend std::ostream& operator<<(std::ostream& os, const HierarchicalList& l) // печать с выделением уровней (печатать отступы или |)
  {
  }

  ~HierarchicalList() {  }

  hl_iterator begin() {  }
  hl_iterator end() {  }
  
  hl_iterator insert_next(hl_iterator& it); // подумать над интерфейсом. Смысл: вставить следующее звено после it. Возвращает итератор на новое звено
  
  hl_iterator insert_down(hl_iterator& it); // подумать над интерфейсом. Сысл: вставить звено it->down. Возвращает итератор на новое звено 
  
  
////////////////////////////////////////////////////////////////////// 
  class hl_iterator {
    TStack<Node*> stack; // здесь ваш стек
    Node* current;
  
  public:
    friend class HierarchicalList;

    hl_iterator(Node* x = nullptr) {}
    hl_iterator(const HierarchicalList::hl_iterator& it) { }
	
    hl_iterator& operator=(const hl_iterator& i) {
    }
	
	hl_iterator& operator++();
	hl_iterator& operator++(int);
	bool is_last() const {}	
	std::string& operator*() { return current->data; }
    std::string* operator->() { return &(current->data); }
	bool operator==(const hl_iterator& it2) {  }
	bool operator!=(const hl_iterator& it2) {  }

    ~hl_iterator() {}
  };
 
};
