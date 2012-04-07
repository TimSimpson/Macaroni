#ifndef __INCLUDE_LIST_H
#define __INCLUDE_LIST_H

#include <string>
#include <vector>


class List
{
    private:
        std::vector<std::string> items;

    public:
        int GetCount() const;

        std::string GetItem(int index) const;
};

#endif
