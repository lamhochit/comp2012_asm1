//
// Created by 林灝哲 on 18/3/2020.
//
#include "table.h"
#include <iostream>

using namespace std;

Table::Table()
{
    numAttrs = 0;
    numEntries = 0;
    attrs = nullptr;
    entries = nullptr;
}

Table::Table(const Table &another)
{
    this->numAttrs = another.numAttrs;
    this->numEntries = another.numEntries;
    this->attrs = new string[numAttrs];
    this->entries = new string*[numEntries];

    for (int i = 0; i < numAttrs; i++)
    {
        this->attrs[i] = another.attrs[i];
    }
    for (int i = 0; i < numEntries; i++)
    {
        string* temp_entry = new string[numAttrs];
        for (int j = 0; j < numAttrs; j++)
        {
            temp_entry[j] = another.entries[i][j];
        }
        entries[i] = temp_entry;
    }
}

Table::~Table()
{
    if (attrs != nullptr and numAttrs > 0)
    {
        delete[] attrs;
        attrs = nullptr;
    }
    if (entries != nullptr and numEntries > 0)
    {
        for (int i = 0; i < numEntries; i++) {
            delete[] entries[i];
            entries[i] = nullptr;
        }
        delete[] entries;
        entries = nullptr;
    }
}

bool Table::addAttribute(const string &attr, int index, const string &default_value)
{
    string* temp_attrs = new string[numAttrs+1];
    if (index == -1 or index == numAttrs)
    {
        for (int i = 0; i < numAttrs; i++)
        {
            temp_attrs[i] = attrs[i];
        }
        temp_attrs[numAttrs] = attr;
        delete [] attrs;
        attrs = temp_attrs;
    }
    else if (index > -1 and index < numAttrs)
    {
        for (int i = 0; i < index; i++)
        {
            temp_attrs[i] = attrs[i];
        }
        temp_attrs[index] = attr;
        for (int i = index; i < numAttrs; i++)
        {
            temp_attrs[i+1] = attrs[i];
        }
        delete [] attrs;
        attrs = temp_attrs;
    } else
    {
        return false;
    }

    if (numEntries == 0)
    {
        numAttrs += 1;
        return true;
    }
    else if (numEntries > 0)
    {
        for (int row = 0; row < numEntries; row++)
        {
            string* temp_entry = new string[numAttrs+1];
            if (index == -1 or index == numAttrs)
            {
                for (int i = 0; i < numAttrs; i++)
                {
                    temp_entry[i] = entries[row][i];
                }
                temp_entry[numAttrs] = default_value;
                delete [] entries[row];
                entries[row] = temp_entry;
            }
            else if (index > -1 and index < numAttrs)
            {
                for (int i = 0; i < index; i++)
                {
                    temp_entry[i] = entries[row][i];
                }
                temp_entry[index] = default_value;
                for (int i = index; i < numAttrs; i++)
                {
                    temp_entry[i+1] = entries[row][i];
                }
                delete [] entries[row];
                entries[row] = temp_entry;
            }
        }
        numAttrs += 1;
        return true;
    }
    return false;
}

bool Table::addEntry(const string *entry, int index)
{
    if (index == -1 or index == numEntries)
    {
        if (numEntries != 0)
        {
            string** temp_entries = new string*[numEntries+1];
            string* temp_entry = new string[numAttrs];
            for (int row = 0; row < numEntries; row++)
            {
                temp_entries[row] = entries[row];
            }
            for (int i = 0; i < numAttrs; i++)
            {
                temp_entry[i] = entry[i];
            }
            temp_entries[numEntries] = temp_entry;
            delete [] entries;
            entries = temp_entries;
            numEntries += 1;
            return true;
        }
        else if (numEntries == 0)
        {
            string** temp_entries = new string*[numEntries+1];
            string* temp_entry = new string[numAttrs];
            for (int i = 0; i < numAttrs; i++)
            {
                temp_entry[i] = entry[i];
            }
            temp_entries[numEntries] = temp_entry;
            delete [] entries;
            entries = temp_entries;
            numEntries += 1;
            return true;
        }
    }
    else if (index > -1 and index <numEntries)
    {
        string** temp_entries = new string*[numEntries+1];
        for (int row = 0; row < index; row++)
        {
            temp_entries[row] = entries[row];
        }
        string* temp_entry = new string[numAttrs];
        for (int i = 0; i < numAttrs; i++)
        {
            temp_entry[i] = entry[i];
        }
        for (int row = index; row < numEntries; row++)
        {
            temp_entries[row+1] = entries[row];
        }
        temp_entries[index] = temp_entry;
        delete [] entries;
        entries = temp_entries;
        numEntries += 1;
        return true;
    }
    else
    {
        return false;
    }
}

bool Table::deleteAttribute(int index)
{
    string* temp_attrs = new string[numAttrs-1];
    if (index == numAttrs-1)
    {
        for (int i = 0; i < numAttrs-1; i++)
        {
            temp_attrs[i] = attrs[i];
        }
        delete [] attrs;
        attrs = temp_attrs;
    }
    else if (index > -1 and index < numAttrs-1)
    {
        for (int i = 0; i < index; i++)
        {
            temp_attrs[i] = attrs[i];
        }
        for (int i = index+1; i < numAttrs; i++)
        {
            temp_attrs[i-1] = attrs[i];
        }
        delete [] attrs;
        attrs = temp_attrs;
    }
    else
    {
        return false;
    }

    if (numEntries == 0)
    {
        numAttrs -= 1;
        return true;
    }
    else if (numEntries > 0)
    {
        for (int row = 0; row < numEntries; row++)
        {
            if (index == -1 or index == numAttrs-1)
            {
                string* temp_entry = new string[numAttrs-1];
                for (int i = 0; i < numAttrs-1; i++)
                {
                    temp_entry[i] = entries[row][i];
                }
                delete [] entries[row];
                entries[row] = temp_entry;
            }
            else if (index > -1 and index < numAttrs)
            {
                string* temp_entry = new string[numAttrs-1];
                for (int i = 0; i < index; i++)
                {
                    temp_entry[i] = entries[row][i];
                }
                for (int i = index+1; i < numAttrs; i++)
                {
                    temp_entry[i-1] = entries[row][i];
                }
                delete [] entries[row];
                entries[row] = temp_entry;
            }
        }
        numAttrs -= 1;
        return true;
    }
    return false;
}

bool Table::deleteEntry(int index)
{
    if (index == numEntries-1)
    {
        string** temp_entries = new string*[numEntries-1];
        for (int row = 0; row < numEntries-1; row++)
        {
            temp_entries[row] = entries[row];
        }
        delete [] entries;
        entries = temp_entries;
        numEntries -= 1;
        return true;
    }
    else if (index > -1 and index < numEntries-1)
    {
        string** temp_entries = new string*[numEntries-1];
        for (int row = 0; row < index; row++)
        {
            temp_entries[row] = entries[row];
        }
        for (int row = index+1; row < numEntries; row++)
        {
            temp_entries[row-1] = entries[row];
        }
        delete [] entries;
        entries = temp_entries;
        numEntries -= 1;
        return true;
    }
    else
    {
        return false;
    }
}

bool Table::append(const Table &another)
{
    if (this->numAttrs != another.numAttrs)
    {
        return false;
    }
    else
    {
        for (int i = 0; i < this->numAttrs; i++)
        {
            if (this->attrs[i] != another.attrs[i]) {return false;}
        }
    }

    string** temp_entries = new string*[numEntries*2];
    for (int i = 0; i < numEntries; i++)
    {
        temp_entries[i] = entries[i];
    }
    for (int i = numEntries; i < numEntries*2; i++)
    {
        string* temp_entry = new string[numAttrs];
        for (int j = 0; j < numAttrs; j++)
        {
            temp_entry[j] = another.entries[i-numEntries][j];
        }
        temp_entries[i] = temp_entry;
    }
    numEntries += numEntries;
    this->entries = temp_entries;
    return true;
}

void Table::print() const
{
    cout << "Num of Attrs: " << numAttrs << endl;
    cout << "Num of Entries: " << numEntries << endl << "Attributes: " << endl;
    for (int i = 0; i < numAttrs; i++)
    {
        cout << attrs[i] << " ";
    }
    cout << endl << "Entries: " << endl;
    for (int i = 0; i < numEntries; i++)
    {
        for (int j = 0; j < numAttrs; j++)
        {
            cout << entries[i][j] << " ";
        }
        cout << endl;
    }
}