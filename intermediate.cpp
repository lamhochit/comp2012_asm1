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

    int i = 0;
    if (table.numEntries == 0)
    {
        head = nullptr;
        tail = nullptr;
    }
    else
    {
        for (EntryNode *p = new EntryNode; i < table.numEntries; i++) {
//            p->entry = new string[numAttrs];
            p->entry = table.entries[i];

            if (i == 0) {
                head = p;
                p->prev = nullptr;
                p->next = new EntryNode;
                p->next->prev = p;
            } else if (i == table.numEntries - 1) {
                p->next = nullptr;
                tail = p;
            } else {
                p->next = new EntryNode;
                p->next->prev = p;
            }
            p = p->next;
        }
    }
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
        EntryNode* temp_tail;
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
            else if(p->entry[pos] == value)
            {
                temp_tail = p;
                p = p->next;
            }
        }
        tail = temp_tail;
        tail->next = nullptr;
        return *this;
    }
    else if (mode == CONTAINS)
    {
        int count = 0;
        EntryNode* temp_tail;
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
            else if(p->entry[pos].find(value) != string::npos)
            {
                temp_tail = p;
                p = p->next;
            }
        }
        tail = temp_tail;
        tail->next = nullptr;
        return *this;
    }
    else
    {
        return *this;
    }
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
    if (limit > length)
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

    int i = 0;
    for (EntryNode* p = head; i < limit-1; p = p->next)
    {
        i++;
        tail = p->next;
    }

    EntryNode* temp;
    for (EntryNode* q = tail->next; q != nullptr;)
    {
        temp = q->next;
        delete q;
        q = temp;
    }
    tail->next = nullptr;
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
    int* len = new int[this->numAttrs];
    if (this->numAttrs == 0) {delete [] len; return;}
    else
    {
        for (int i = 0; i < this->numAttrs; i++)
        {
            len[i] = this->attrs[i].length();
        }
        for (EntryNode* p = head; p != nullptr;)
        {
            for (int pos = 0; pos < this->numAttrs; pos++)
            {
                if (p->entry[pos].length() > len[pos])
                {
                    len[pos] = p->entry[pos].length();
                }
            }
            p = p->next;
        }
    }

    if (attrs == nullptr)
    {
        for (int i = 0; i < this->numAttrs; i++)
        {
            if (i < this->numAttrs-1)
            {
                cout << _left_pad_until(this->attrs[i], len[i]) << " | ";
            }
            else
            {
                cout << _left_pad_until(this->attrs[i], len[i]);
            }
        }
        cout << endl;
        for (EntryNode* p = head; p != nullptr;)
        {
            for (int pos = 0; pos < this->numAttrs; pos++)
            {
                if (pos != this->numAttrs-1)
                {
                    cout << _left_pad_until(p->entry[pos], len[pos]) << " | ";
                } else {cout << _left_pad_until(p->entry[pos], len[pos]);}
            }
            p = p->next;
            cout << endl;
        }
        delete [] len;
    }
    else
    {
        int* temp_pos = new int[numAttrs];
        for (int i = 0; i < numAttrs; i++)
        {
            for (int j = 0; j < this->numAttrs; j++)
            {
                if (attrs[i] == this->attrs[j])
                {
                    temp_pos[i] = j;
                }
            }
        }

        for (int pos = 0; pos < numAttrs; pos++)
        {
            for (int i = 0; i < this->numAttrs; i++)
            {
                if (i == temp_pos[pos])
                {
                    if (i != temp_pos[numAttrs-1])
                    {
                        cout << _left_pad_until(this->attrs[i], len[i]) << " | ";
                    }
                    else
                    {
                        cout << _left_pad_until(this->attrs[i], len[i]);
                    }
                }
            }
        }
        cout << endl;
        for (EntryNode* p = head; p != nullptr;)
        {
            for (int i = 0; i < numAttrs; i++)
            {
                for (int pos = 0; pos < this->numAttrs; pos++)
                {
                    if (pos == temp_pos[i])
                    {
                        if (pos != temp_pos[numAttrs-1])
                        {
                            cout << _left_pad_until(p->entry[pos], len[pos]) << " | ";
                        } else {cout << _left_pad_until(p->entry[pos], len[pos]);}
                    }
                }
            }
            p = p->next;
            cout << endl;
        }
        delete [] len;
        delete [] temp_pos;
    }
}

