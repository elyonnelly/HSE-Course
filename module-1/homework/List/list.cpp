#include "list.h"
#include <list>
#include <vector>
#include <algorithm>

task::list::list() {
    list_size = 0;
    head = nullptr;
    tail = nullptr;
}

task::list::list(size_t count, const int& value) : list() {
    for (size_t i = 0; i < count; i++) {
        push_back(value);
    }
}

task::list::~list() {
    clear();
}

task::list& task::list::operator=(const task::list& other) {
    if (this != &other) {
        clear();
        Node* other_head = other.head;
        for (size_t i = 0; i < other.list_size; i++) {
            push_back(other_head->value);
            other_head = other_head->next;
        }
        list_size = other.list_size;
    }
    return *this;
}

int& task::list::front() {
    return head->value;
}

const int& task::list::front() const {
    return head->value;
}

int& task::list::back() {
    return tail->value;
}

const int& task::list::back() const {
    return tail->value;
}

bool task::list::empty() const {
    return list_size == 0;
}

size_t task::list::size() const {
    return list_size;
}

void task::list::clear() {
    while (list_size != 0) {
        pop_back();
    }
}

void task::list::push_back(const int& value) {
    Node* new_tail = new Node(value, tail, nullptr);
    Node* prev_tail = tail;
    if (prev_tail != nullptr) {
        prev_tail->next = new_tail;
    } else {
        if (head != nullptr) {
            head->next = new_tail;
        }
    }
    tail = new_tail;
    if (head == nullptr) {
        head = new_tail;
    }
    list_size++;
}

void task::list::pop_back() {
    if (empty()) {
        return;
    }
    list_size--;
    Node* last_tail = tail;
    Node* new_tail = tail->prev;
    tail = new_tail;
    if (tail != nullptr) {
        tail->next = nullptr;
    }
    if (empty()) {
        head = nullptr;
    }

    delete last_tail;
}

void task::list::push_front(const int& value) {
    Node* new_head = new Node(value, nullptr, head);
    Node* prev_head = head;
    if (prev_head != nullptr) {
        prev_head->prev = new_head;
    }
    head = new_head;
    if (tail == nullptr) {
        tail = new_head;
    }
    list_size++;
}

void task::list::pop_front() {
    if (head == nullptr) {
        return;
    }
    list_size--;
    Node* last_head = head;
    Node* new_head = head->next;
    if (new_head != nullptr) {
        new_head->prev = nullptr;
    }
    head = new_head;
    if (empty()) {
        tail = nullptr;
    }
    delete last_head;
}

void task::list::resize(size_t count) {
    while (list_size > count) {
        pop_back();
    }
    while (list_size < count) {
        push_back(0);
    }
}

void task::list::swap(task::list& other) {
    std::swap(list_size, other.list_size);
    std::swap(head, other.head);
    std::swap(tail, other.tail);
}

void task::list::remove(const int& value) {
    int key = value;
    Node* node = head;
    while (node != nullptr) {
        if (node->value == key) {
            Node* to_remove = node;
            node = node->next;
            remove_node(to_remove);
        } else {
            node = node->next;
        }
    }
}

void task::list::remove_node(task::list::Node* node) {
    if (node == tail && node == head) {
        tail = nullptr;
        head = nullptr;
        list_size = 0;
        delete node;
    } else if (node == head) {
        pop_front();
    } else if (node == tail) {
        pop_back();
    } else {
        Node* prev_node = node->prev;
        Node* next_node = node->next;
        prev_node->next = next_node;
        next_node->prev = prev_node;
        list_size--;
        delete node;
    }
}

void task::list::unique() {
    if (empty()) {
        return;
    }
    Node* node = head->next;
    while (node != nullptr) {
        while (node != nullptr && node->value == node->prev->value) {
            Node* non_unique_node = node;
            node = node->next;
            remove_node(non_unique_node);
        }
        if (node != nullptr) {
            node = node->next;
        }
    }
}

void task::list::sort() {
    std::vector<int> values;

    Node* node = head;
    while (node != nullptr) {
        values.push_back(node->value);
        node = node->next;
    }
    std::sort(values.begin(), values.end());
    clear();
    if (!values.empty()) {
        head = new Node(values[0], nullptr, nullptr);
        tail = head;
        list_size = 1;
        for (size_t i = 1; i < values.size(); i++) {
            push_back(values[i]);
        }
    }
}

task::list::list(const task::list& other) : list() {
    if (this != &other) {
        clear();
        Node* other_head = other.head;
        for (size_t i = 0; i < other.list_size; i++) {
            push_back(other_head->value);
            other_head = other_head->next;
        }
        list_size = other.list_size;
    }
}
