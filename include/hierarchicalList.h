#pragma once
#include<iostream>
#include <fstream>
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

    void Print()
    {
        level = 0;
        PrintRec(root);
    }
    void Read(string FileName);

    void DelRec(Node* p)
    {
        if (p)
        {
            DelRec(p->down);
            DelRec(p->next);
            delete p;
        }
    }

    ~HierarchicalList()
    {
        DelRec(root);
    }

    hl_iterator begin() { return hl_iterator(root); }
    hl_iterator end() { return hl_iterator(nullptr); }

    hl_iterator insert_next(hl_iterator& it); // подумать над интерфейсом. Смысл: вставить следующее звено после it. Возвращает итератор на новое звено

    hl_iterator insert_down(hl_iterator& it); // подумать над интерфейсом. Сысл: вставить звено it->down. Возвращает итератор на новое звено 


    ////////////////////////////////////////////////////////////////////// 
    class hl_iterator {
        TStack<Node*> stack; // здесь ваш стек
        Node* current;
        Node* first;

    public:
        friend class HierarchicalList;
        hl_iterator(Node* startNode = nullptr) : current(startNode)
        {
            if (startNode != nullptr)
            {
                //stack.Push(startNode);
                first = startNode;
                if (current->next != nullptr) stack.Push(current->next);
                if (current->down != nullptr) stack.Push(current->down);

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
            if (stack.IsEmpty() || current == nullptr)
            {
                current = nullptr;
                return *this;
            }
               
            current = stack.Peek(); stack.Pop();  
            if (current != first)
            {   // первая строка текста уже была обработана 
                if (current->next != nullptr) stack.Push(current->next);
                if (current->down != nullptr) stack.Push(current->down);
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

        string& operator*() { return current->data; }
        string* operator->() { return &(current->data); }

        bool operator==(const hl_iterator& other) const { return current == other.current; }
        bool operator!=(const hl_iterator& other) const { return current != other.current; }
        ~hl_iterator() {}
    };
private:
    void PrintRec(Node *p);
    int level;
    Node* ReadRec(ifstream& File);
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


void HierarchicalList::PrintRec(Node* p)
{
    if (p)
    {
        for (int i = 0;i < level;i++)
            cout << "    ";
        cout << p->data << endl;
        level++;
        PrintRec(p->down);
        level--;
        PrintRec(p->next);
    }
}


void HierarchicalList::Read(string FileName)
{
    ifstream file(FileName);
    if (file)
    {
        level = 0;
        root = ReadRec(file);
    }
}

HierarchicalList::Node* HierarchicalList::ReadRec(ifstream& File)
{
    Node* pHead, * ptl;
    pHead = ptl = nullptr;
    string StrBuf;
    while (File.eof() == 0)
    {
        getline(File, StrBuf);
        if (StrBuf == "end;" || StrBuf == "end."|| StrBuf == "end")
        { 
            level--; 
            break; 
        }
        else if (StrBuf == "begin")
        { //рекурсия
            level++;
            ptl->down = ReadRec(File);
        }
        else { // присоединение следующей строки
            if (pHead == NULL)
            {
                pHead = ptl = new Node(StrBuf);
            }
            else
            {
                ptl->next = new Node(StrBuf);
                ptl = ptl->next;
            }
        }
    }
    return pHead;
}
