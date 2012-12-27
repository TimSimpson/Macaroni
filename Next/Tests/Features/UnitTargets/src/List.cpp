#include <List.h>
#include <Dog.h>

int List::GetCount() const {
    return items.size();
}

std::string List::GetItem(int index) const {
    return items[index];
}
