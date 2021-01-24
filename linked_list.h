
class linked_list_exception : public std::exception {
private:
    std::string error_message;
public:
    linked_list_exception() = default;
    explicit linked_list_exception(std::string error) {
        error_message = error;
    }
};

template < typename T >
class linked_list {
    struct node {
        T value;
        node * next;
        node * previous;
    };
    node * _end;
    size_t _size;
public:
    class iterator {
        node * element;
    public:
        // Default constructor
        explicit iterator(const node * other) {
            *element = *other;
        }

        ~iterator() {
            delete element;
        }

        iterator & operator=(const iterator & other) {
            *element = *(other.element);
            return *this;
        }

        bool operator==(const iterator & other) const {
            return (this->element == other.element);
        }
        bool operator!=(const iterator & other) const {
            return !(other == *this);
        }
        T & operator*() { return this->element->value; } // Get element reference
        T * operator->() { return &this->element->value; } // Get element pointer

        iterator & operator++() {
            element = element->next;
            return *this;
        }
        iterator operator++(int) {
            iterator & copy = *this;
            element = element->next;
            return copy;
        }
        iterator & operator--() {
            element = element->previous;
            return *this;
        }
        iterator operator--(int) {
            iterator & copy = *this;
            element = element->previous;
            return copy;
        }

        friend linked_list;
    };

    class const_iterator {
        node * element;
    public:
        const_iterator(const node * other) {
            *element = *other;
        }

        ~const_iterator() {
            delete element;
        }

        const_iterator & operator=(const const_iterator & other) {
            *element = other.element;
            return *this;
        }

        bool operator!=(const const_iterator & other) const {
            return !(*this == other);
        }
        bool operator==(const const_iterator & other) const {
            return (element == other.element);
        }
        const T & operator*() const { return this->element->value; }; // Get element reference
        const T * operator->() const { return &this->element->value; }; // Get element pointer

        const_iterator & operator++() {
            element = element->next;
            return *this;
        }
        const_iterator operator++(int) {
            const_iterator & copy = *this;
            element = element->next;
            return copy;
        }
        const_iterator & operator--() {
            element = element->previous;
            return *this;
        }
        const_iterator operator--(int) {
            const_iterator & copy = *this;
            element = element->previous;
            return copy;
        }

        friend linked_list;
    };

    // Create an empty list
    linked_list() {
        _end = new node;
        _end->next = _end;
        _end->previous = _end;
    }
    // Create a new list with elements from another list
    linked_list(const linked_list & other) {
        _end = new node;
        _end->next = _end;
        _end->previous = _end;
        for (auto it : other) {
            push_back(it);
        }
    }
    // Move constructor
    linked_list(linked_list && other) {
        this->clear();
        _end = other._end;
        other._end = nullptr;
    }

    // Destructor
    ~linked_list() {
        this->clear();
        delete _end;
    }

    //
    linked_list & operator=(const linked_list & other) {
        this->clear();
        for (auto it : other) {
            push_back(it);
        }
        return *this;
    }
    linked_list & operator=(linked_list && other) {
        this->clear();
        _end = other._end;
        other._end = nullptr;
        return *this;
    }

    iterator begin() { return iterator(_end->next); }
    const_iterator begin() const { return const_iterator(_end->next); }
    const_iterator cbegin() const { return const_iterator(_end->next); }

    iterator end() { return iterator(_end); }
    const_iterator end() const { return const_iterator(_end); }
    const_iterator cend() const { return const_iterator(_end); }

    // Size of list
    int size() const {
        return _size;
    }
    // "true" if the list is empty, "false" otherwise
    bool empty() const { return (_size == 0); }

    // Reference to the first element
    T & front() {
        if (_end->next == _end) {
            throw linked_list_exception("Trying to get reference to _end value");
        }
        return _end->next->value;
    }
    // Reference to the first element
    const T & front() const {
        if (_end->next == _end) {
            throw linked_list_exception("Trying to get const reference to _end value");
        }
        return _end->next->value;
    }

    // Reference to the last element
    T & back() {
        if (_end->previous == _end) {
            throw linked_list_exception("Trying to get reference to _end value");
        }
        return _end->previous->value;
    }
    // Reference to the last element
    const T & back() const {
        if (_end->previous == _end) {
            throw linked_list_exception("Trying to get const reference to _end value");
        }
        return _end->previous->value;
    }

    // Modificators

    // Add (value) to the element before [before]
    iterator insert(iterator & before, const T & value) {
        auto new_element = new node;
        new_element->value = value;
        new_element->next = before.element;
        new_element->previous = before.element->previous;
        before.element->previous->next = new_element;
        before.element->previous = new_element;
        _size++;
        return *(new iterator(new_element));
    }
    // Delete [index] element
    iterator erase(iterator & index) {
        if (index.element == _end) {
            throw linked_list_exception("Trying to erase _end");
        }
        auto ret_node = index.element->next;
        index.element->previous->next = index.element->next;
        index.element->next->previous = index.element->previous;
        delete index.element;
        _size--;
        return iterator(ret_node);
    }
    // Delete [begin, end] elements
    iterator erase(iterator begin, iterator end) {
        auto current = begin;
        while (current != end) {
            if (current.element == _end) {
                throw linked_list_exception("Trying to erase _end");
            }
            current = this->erase(current);
        }
        auto ret_node = end++;
        this->erase(end);
        return ret_node;
    }
    // Delete all occurrences of (value)
    int remove(const T & value) {
        auto current = begin();
        int count = 0;
        while (current != end()) {
            if (*current == value) {
                current = erase(current);
                ++count;
            } else {
                ++current;
            }
        }
        return count;
    }
    // Clears the list
    void clear() {
        auto it = begin();
        while (it != end()) {
            it = erase(it);
        }
    }

    // Delete the last element
    void pop_back() {
        iterator back = iterator(_end->previous);
        erase(back);
    }
    // Delete the first element
    void pop_front() {
        iterator front = iterator(_end->next);
        erase(front);
    }
    // Add (value) to the end of the list
    void push_back(const T & value) {
        iterator back = iterator(_end);
        insert(back, value);
    }
    // Add (value) to the begin of the list
    void push_front(const T & value) {
        iterator front = iterator(_end->next);
        insert(front, value);
    }

    // Internal operators

    // Concat of lists
    linked_list & operator+=(const linked_list & other) {
        for (auto it : other) {
            this->push_back(it);
        }
        return *this;
    }

    // External operators
    friend bool operator!=(const linked_list<T> & left, const linked_list<T> & right);
    friend bool operator==(const linked_list<T> & left, const linked_list<T> & right);
};
template <typename T>
bool operator!=(const linked_list<T> & left, const linked_list<T> & right) {
    auto it1 = left.begin();
    auto it2 = right.begin();
    while (it1 != left.end() && it2 != right.end()) {
        if (*it1 != *it2) {
            return true;
        }
        it1++;
        it2++;
    }
    return (left.size() != right.size());
}
template <typename T>
bool operator==(const linked_list<T> & left, const linked_list<T> & right) {
    return !operator!=(left, right);
}
