//
// Created by 林灝哲 on 18/3/2020.
//

#include "intermediate.h"
#include "table.h"
#include <iostream>
#include <string>

using namespace std;

Intermediate::Intermediate(const Table &table)
{
    numAttrs = table.numAttrs;
    attrs = table.attrs;
    EntryNode* head = new EntryNode;
    head->entry = table.entries[0];

    int i = 1;
    EntryNode* p;
    for (p = head; i < numAttrs; p = p->next)
    {
        p->next = new EntryNode;
        p->next->entry = table.entries[i];
        p->next->prev = p;

        i += 1;
    }

    EntryNode* tail = new EntryNode;
    tail = p;
    tail->next = nullptr;
    head->prev = nullptr;
}

Intermediate::~Intermediate()
{
    EntryNode* temp;
    for (EntryNode* p = head; p != nullptr;)
    {
        temp = p->next;
        delete p;
        p = temp;
    }
    head = nullptr;
    tail = nullptr;
}

Intermediate& Intermediate::where(const string &attr, enum compare mode, const string &value)
{
    int pos = -1;
    for (int i = 0; i < numAttrs; i++)
    {
        if (attrs[i] == attr) {pos = i;}
    }
    if (pos == -1) {return *this;}

    EntryNode* temp;
    if (mode == EQ)
    {
        int count = 0;
        for (EntryNode* p = head; p != nullptr; count++)
        {
            if (p->entry[pos] != value)
            {
                if (p == head)
                {
                    head = p->next;
                    temp = p->next;
                    delete p;
                    p = temp;
                }
                else if (p == tail)
                {
                    tail = p->prev;
                    delete p;
                    p = nullptr;
                }
                else
                {
                    p->prev->next = p->next;
                    p->next->prev = p->prev;
                    temp = p->next;
                    delete p;
                    p = temp;
                }
            }
        }
        return *this;
    }
    else if (mode == CONTAINS)
    {
        int count = 0;
        for (EntryNode* p = head; p != nullptr; count++)
        {
            if (p->entry[pos].find(value) == string::npos)
            {
                if (p == head)
                {
                    head = p->next;
                    temp = p->next;
                    delete p;
                    p = temp;
                }
                else if (p == tail)
                {
                    tail = p->prev;
                    delete p;
                    p = nullptr;
                }
                else
                {
                    p->prev->next = p->next;
                    p->next->prev = p->prev;
                    temp = p->next;
                    delete p;
                    p = temp;
                }
            }
        }
        return *this;
    }

    return *this;
}

Intermediate& Intermediate::orderBy(const string &attr, enum order order)
{
    int pos = -1;
    for (int i = 0; i < numAttrs; i++)
    {
        if (attrs[i] == attr) {pos = i;}
    }
    if (pos == -1) {return *this;}

    int length = 0;
    for (EntryNode* p = head; p != nullptr; p = p->next)
    {
        length += 1;
    }

    if (order == ASCENDING)
    {
        for (int i = 1; i < length; i++)
        {
            EntryNode* current = head;
            for (int j = 0; j < length-i; j++)
            {
                if (current->entry[pos].compare(current->next->entry[pos]) > 0)
                {
                    string* temp;
                    temp = current->entry;
                    current->entry = current->next->entry;
                    current->next->entry = temp;
                }
                current = current->next;
            }
        }
        return *this;
    }
    else if (order == DESCENDING)
    {
        for (int i = 1; i < length; i++)
        {
            EntryNode* current = head;
            for (int j = 0; j < length-i; j++)
            {
                if (current->entry[pos].compare(current->next->entry[pos]) < 0)
                {
                    string* temp;
                    temp = current->entry;
                    current->entry = current->next->entry;
                    current->next->entry = temp;
                }
                current = current->next;
            }
        }
        return *this;
    }
    return *this;
}

Intermediate& Intermediate::limit(unsigned int limit)
{
    int length = 0;
    for (EntryNode* p = head; p != nullptr; p = p->next)
    {
        length += 1;
    }
    if (length < limit)
    {
        return *this;
    }
    else if (limit == 0)
    {
        EntryNode* temp;
        for (EntryNode* p = head; p != nullptr;)
        {
            temp = p->next;
            delete p;
            p = temp;
        }
        head = nullptr;
        tail = nullptr;
        return *this;
    }

    EntryNode* p = head;
    for (int i = 0; i < limit-1; p = p->next)
    {
        i++;
    }
    tail = p;

    EntryNode* temp;
    for (EntryNode* q = p->next; q != nullptr;)
    {
        temp = q->next;
        delete q;
        q = temp;
    }

    return *this;
}

void Intermediate::update(const string &attr, const string &new_value) const
{
    int pos = -1;
    for (int i = 0; i < numAttrs; i++)
    {
        if (attrs[i] == attr) {pos = i;}
    }
    if (pos == -1) {return;}

    for (EntryNode* p = head; p != nullptr;)
    {
        p->entry[pos] = new_value;
        p = p->next;
    }
}

void Intermediate::select(const string *attrs, int numAttrs) const
{
//    if (attrs == nullptr)
//    {
//        for (int i = 0; i < this->numAttrs; i++)
//        {
//            if (i != numAttrs-1)
//            {
//                cout << this->attrs[i] << ' | ';
//            } else {cout << this->attrs[i];}
//        }
//        cout << endl;
//        for
//
//    }
}

