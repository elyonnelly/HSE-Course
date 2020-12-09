#pragma once

#include <cstddef>


namespace task {

    class list {

    public:

        list();

        list(size_t count, const int &value = int());

        list(const task::list &other);

        ~list();

        list &operator=(const list &other);


        int &front();

        const int &front() const;

        int &back();

        const int &back() const;


        bool empty() const;

        size_t size() const;

        void clear();


        void push_back(const int &value);

        void pop_back();

        void push_front(const int &value);

        void pop_front();

        void resize(size_t count);

        void swap(list &other);

        void remove(const int &value);

        void unique();

        void sort();

    private:

        struct Node {
            int value;
            Node *prev = nullptr;
            Node *next = nullptr;

            explicit Node(int value_, Node *prev_, Node *next_) : value(value_), prev(prev_), next(next_) {}
        };

        Node *head;
        Node *tail;
        size_t list_size;

        void remove_node(Node *node);
    };

}
