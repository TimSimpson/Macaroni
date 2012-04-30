#include <List.h>
#include <Dog.hpp>

int List::GetCount() const {
    return items.size();
}

std::string List::GetItem(int index) const {
    return items[index];
}
