#pragma once
#include<iostream>
#include<string>
#include<vector>
#include <memory>

#ifndef HIERARCHICAL_LIST_H
#define HIERARCHICAL_LIST_H
using namespace std;

template<typename T>
class TStack {
private:
    vector<T> data;
public:
    TStack() {}

    void Push(const T& value)
    {
        data.push_back(value);
    }

    T Pop()
    {
        if (IsEmpty())
        {
            throw out_of_range("Stack is empty");
        }
        T topElement = data.back();
        data.pop_back();
        return topElement;
    }

    T Peek() const //посмотреть на верхний эл-т не снимая его
    {
        if (IsEmpty())
        {
            throw out_of_range("Stack is empty");
        }
        return data.back();
    }

    bool IsEmpty() const
    {
        return data.empty();
    }

    size_t Size() const
    {
        return data.size();
    }
};

class HierarchicalList
{
private:
    struct Node {
        string data; //[хранит данные узла
        Node* next;
        Node* down;
        Node(std::string x) : data(x), next(nullptr), down(nullptr) {}
        Node() : data(""), next(nullptr), down(nullptr) {} // Конструктор по умолчанию
    };

    //////////////////////////////////////////////////////////////////////  
    Node* root = nullptr;

public:
    class hl_iterator;
    
    HierarchicalList() : root(nullptr) {}

    void Load(std::string path) // загрузка из файла
    {
    }

    friend ostream& operator<<(ostream& os, HierarchicalList& l)
    {
        hl_iterator it = l.begin();
        while (it != l.end())
        {
            os << *it << endl;
            ++it;
        }
        return os;
    }

    ~HierarchicalList()
    {
        Node* current = root;
        while (current)
        {
            Node* next = current->next;
            Node* down = current->down;
            while (current->down) { //освобождает все нижние узлы
                Node* temp = current->down;
                current->down = temp->next;
                delete temp;
            }
            delete current;
            current = next;
            if (root != nullptr) {
                root = root->next;
            }
        }
    }

    hl_iterator begin() { return hl_iterator(root); }
    hl_iterator end() { return hl_iterator(nullptr); }

    hl_iterator insert_next(hl_iterator& it); // подумать над интерфейсом. Смысл: вставить следующее звено после it. Возвращает итератор на новое звено

    hl_iterator insert_down(hl_iterator& it); // подумать над интерфейсом. Сысл: вставить звено it->down. Возвращает итератор на новое звено 



    ////////////////////////////////////////////////////////////////////// 
    class hl_iterator {
        TStack<Node*> stack; // здесь ваш стек
        Node* current;

    public:
        friend class HierarchicalList;
        //было изначально написано
        /*hl_iterator(Node* x = nullptr) {}
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
        */
        
        hl_iterator(Node* startNode = nullptr) : current(startNode)
        {
            if (startNode != nullptr)
            {
                stack.Push(startNode);
            }
        }
        hl_iterator(const HierarchicalList::hl_iterator& it) : current(it.current), stack(it.stack) {} 

        hl_iterator& operator=(const hl_iterator& i) {
            if (this != &i) {
                current = i.current;
                stack = i.stack;
            }
            return *this;
        }
        
        hl_iterator& operator++()
        {
            if (current == nullptr)
                return *this;
            if (current->down) //переход на уровень ниже если существует
            {
                stack.Push(current->down);
                current = current->down;
                return *this;
            }
            //если нет, то переход к след справа, пока не найдем
            while (true)
            {
                if (current->next)
                {
                    current = current->next;
                    return *this;
                }
                else
                {
                    //у нас нет следующего мы идем вверх
                    if (!stack.IsEmpty())
                    {
                        current = stack.Peek();
                        stack.Pop();
                    }
                    else
                    {
                        current = nullptr;
                        return *this;
                    }
                }
            }
            return *this;
        }


        hl_iterator& operator++(int) 
        {
            hl_iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool is_last() const { return current == nullptr; }

        std::string& operator*() { return current->data; }
        std::string* operator->() { return &(current->data); }

        bool operator==(const hl_iterator& other) const { return current == other.current; }
        bool operator!=(const hl_iterator& other) const { return current != other.current; }
        ~hl_iterator() {}
    };
};
#endif


HierarchicalList::hl_iterator HierarchicalList::insert_next(HierarchicalList::hl_iterator& it)
{
    Node* newNode = new Node();
    if (root == nullptr)
    {
        root = newNode;
        return hl_iterator(root);
    }
    if (it.current == nullptr)
    {
        return end();
    }
    newNode->next = it.current->next;
    it.current->next = newNode;
    return hl_iterator(newNode);
}

HierarchicalList::hl_iterator HierarchicalList::insert_down(HierarchicalList::hl_iterator& it)
{
    if (it.current == nullptr)
    {
        return end();
    }
    Node* newNode = new Node();
    newNode->down = it.current->down;
    it.current->down = newNode;
    return hl_iterator(newNode);
}