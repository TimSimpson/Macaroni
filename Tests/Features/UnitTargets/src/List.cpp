#include "List.h"


std::string List::GetCount() const {
    return items.size();
}

std::string List::GetItem(int index) const {
    return items[index];
}
