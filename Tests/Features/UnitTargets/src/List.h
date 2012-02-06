#ifndef __INCLUDE_LIST_H
#define __INCLUDE_LIST_H

#include <string>
#include <vector>


class List
{
    private:
        vector<string> items;

    public:
        std::string GetCount() const;

        std::string GetItem(int index) const;
};
