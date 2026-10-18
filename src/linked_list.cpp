  #include "linked_list.hpp"

LinkedList::LinkedList()
    : head_(nullptr), entry_count_(0) {}

LinkedList::~LinkedList() {
    clear();
}

void LinkedList::insert_or_assign(const std::string& key,
                                  const std::string& value) {
    Node* current = head_;
    while (current != nullptr) {
        if (current->entry.key == key) {
            current->entry.value = value;
            return;
        }
        current = current->next;
    }

    Node* new_node = new Node{
        CacheEntry{
            key,
            value,
            std::chrono::steady_clock::now(),
            std::chrono::steady_clock::time_point{},
            false
        },
        head_
    };
    head_ = new_node;
    ++entry_count_;
}

CacheEntry* LinkedList::find(const std::string& key) {
    Node* current = head_;
    while (current != nullptr) {
        if (current->entry.key == key) {
            return &current->entry;
        }
        current = current->next;
    }
    return nullptr;
}

const CacheEntry* LinkedList::find(const std::string& key) const {
    const Node* current = head_;
    while (current != nullptr) {
        if (current->entry.key == key) {
            return &current->entry;
        }
        current = current->next;
    }
    return nullptr;
}

bool LinkedList::contains(const std::string& key) const {
    return find(key) != nullptr;
}

bool LinkedList::remove(const std::string& key) {
    Node* previous = nullptr;
    Node* current = head_;

    while (current != nullptr) {
        if (current->entry.key == key) {
            if (previous == nullptr) {
                head_ = current->next;
            } else {
                previous->next = current->next;
            }
            delete current;
            --entry_count_;
            return true;
        }
        previous = current;
        current = current->next;
    }
    return false;
}

std::size_t LinkedList::size() const {
    return entry_count_;
}

bool LinkedList::empty() const {
    return entry_count_ == 0;
}

void LinkedList::clear() {
    Node* current = head_;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
    head_ = nullptr;
    entry_count_ = 0;
}
