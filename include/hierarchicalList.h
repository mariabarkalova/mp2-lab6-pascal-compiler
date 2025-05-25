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
public:
    struct Node {
        string data; //[хранит данные узла
        Node* next;
        Node* down;
        bool executed = false;
        Node(std::string x) : data(x), next(nullptr), down(nullptr) {}
        Node() : data(""), next(nullptr), down(nullptr) {} // Конструктор по умолчанию
    };

    //////////////////////////////////////////////////////////////////////  
    Node* root = nullptr;

public:
    class hl_iterator;

    HierarchicalList() : root(nullptr) {}
    Node* GetRoot() const { return root; }
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
    public:
        TStack<Node*> stack; // здесь ваш стек
        Node* current;
        Node* first;
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
    void PrintRec(Node* p);
    int level;
    Node* ReadRec(ifstream& File, TStack<Node*>& blockStack);
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
        for (int i = 0; i < level; i++)
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
    TStack<Node*> st;
    if (file)
    {
        level = 0;
        root = ReadRec(file, st);
    }
}
std::string trim(const std::string& s) {
    if (s.empty()) return s;

    auto start = s.begin();
    while (start != s.end() && std::isspace(*start)) {
        ++start;
    }

    if (start == s.end()) {
        return ""; // Строка состоит только из пробелов
    }

    auto end = s.end();
    do {
        --end;
    } while (end != start && std::isspace(*end));

    return std::string(start, end + 1);
}

HierarchicalList::Node* HierarchicalList::ReadRec(ifstream& File, TStack<Node*>& blockStack) {
    Node* pHead = nullptr;
    Node* ptl = nullptr;
    string StrBuf;

    while (!File.eof()) {
        getline(File, StrBuf);
        StrBuf = trim(StrBuf);
        if (StrBuf.empty()) continue;

        // Обработка program
        if (StrBuf.find("program") == 0) {
            pHead = ptl = new Node(StrBuf);
            continue;
        }
        // Обработка var
        if (StrBuf == "var") {
            if (!pHead) pHead = ptl = new Node(StrBuf);
            else {
                ptl->next = new Node(StrBuf);
                ptl = ptl->next;
            }

            Node* varBlock = nullptr;
            Node* varTail = nullptr;
            while (!File.eof()) {
                streampos pos = File.tellg();
                getline(File, StrBuf);
                StrBuf = trim(StrBuf);

                if (StrBuf == "begin") {
                    File.seekg(pos);
                    break;
                }

                if (!StrBuf.empty()) {
                    if (!varBlock) {
                        varBlock = varTail = new Node(StrBuf);
                    }
                    else {
                        varTail->next = new Node(StrBuf);
                        varTail = varTail->next;
                    }
                }
            }
            ptl->down = varBlock;
            continue;
        }

        // Обработка begin
        if (StrBuf == "begin") {
            Node* beginNode = new Node(StrBuf);
            if (!pHead) pHead = beginNode;
            if (ptl) ptl->next = beginNode;
            ptl = beginNode;

            // Сохраняем текущий контекст
            blockStack.Push(ptl);

            // Рекурсивно обрабатываем блок
            beginNode->down = ReadRec(File, blockStack);
            continue;
        }

        // Обработка end
        if (StrBuf == "end." || StrBuf == "end;") {
            if (!blockStack.IsEmpty()) {
                Node* blockStart = blockStack.Pop();
                // Добавляем end на том же уровне, что и begin
                blockStart->next = new Node(StrBuf);
                ptl = blockStart->next;
            }
            break;
        }

        // Обработка if 
        if (StrBuf.find("if ") == 0) {
            Node* ifNode = new Node(StrBuf);
            if (!pHead) pHead = ifNode;
            if (ptl) ptl->next = ifNode;
            ptl = ifNode;

            size_t thenPos = StrBuf.find("then");
            if (thenPos != string::npos) {
                // Проверяем, есть ли begin на этой же строке
                string thenPart = trim(StrBuf.substr(thenPos + 4));

                if (thenPart.empty()) {
                    // Если после then ничего нет, читаем следующую строку
                    streampos prevPos = File.tellg();
                    getline(File, StrBuf);
                    StrBuf = trim(StrBuf);

                    if (StrBuf == "begin") {
                        // Обрабатываем begin на новой строке
                        Node* beginNode = new Node("begin");
                        ifNode->down = beginNode;
                        Node* savedPtl = ptl;
                        blockStack.Push(ifNode);

                        beginNode->down = ReadRec(File, blockStack);

                        Node* endNode = new Node("end;");
                        beginNode->next = endNode;

                        ptl = savedPtl;
                        if (!blockStack.IsEmpty() && blockStack.Peek() == ifNode) {
                            blockStack.Pop();
                        }
                    }
                    else {
                        // Если не begin, возвращаемся назад и обрабатываем как одиночный оператор
                        File.seekg(prevPos);
                        ifNode->down = new Node(StrBuf);
                    }
                }
                else if (thenPart == "begin") {
                    // Обработка begin на той же строке
                    Node* beginNode = new Node("begin");
                    ifNode->down = beginNode;

                    Node* savedPtl = ptl;
                    blockStack.Push(ifNode);

                    beginNode->down = ReadRec(File, blockStack);

                    Node* endNode = new Node("end;");
                    beginNode->next = endNode;

                    ptl = savedPtl;
                    if (!blockStack.IsEmpty() && blockStack.Peek() == ifNode) {
                        blockStack.Pop();
                    }
                }

            }
            continue;
        }
        // Обычные операторы
        Node* newNode = new Node(StrBuf);
        if (!pHead) {
            pHead = ptl = newNode;
        }
        else {
            ptl->next = newNode;
            ptl = newNode;
        }
    }
    return pHead;
}
