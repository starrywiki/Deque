#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP
#include <cmath>
#include <cstddef>
#include <iostream>

#include "exceptions.hpp"

namespace sjtu {
// initial size for a block
const size_t DEFAULT_BLOCK_SIZE = 350;

template <class T>
class Node {
   public:
    T data;
    Node *prev;
    Node *next;
    Node(const T &val) : data(val), prev(nullptr), next(nullptr) {}
    Node() : prev(nullptr), next(nullptr) {}
};

template <class T>
class double_list {
   private:
    Node<T> *head;
    Node<T> *tail;

   public:
    double_list() {
        head = nullptr;
        tail = nullptr;
    }
    double_list(const double_list<T> &other) : head(nullptr), tail(nullptr) {
        Node<T> *tmp = other.head;
        while (tmp) {
            insert_tail(tmp->data);
            tmp = tmp->next;
        }
    }
    ~double_list() {
        // std::cout << "destructing double_list!" << std::endl;
        clear();
    }
    class iterator {
       public:
        Node<T> *cur;
        iterator() { cur = nullptr; }
        iterator(const iterator &other) { cur = other.cur; }
        iterator(Node<T> *ptr) : cur(ptr){};
        ~iterator() {}
        // iter++
        iterator operator++(int) {
            iterator tmp = *this;
            if (cur == nullptr) {
                throw "invalid";
            }
            cur = cur->next;
            return tmp;
        }

        // ++iter
        iterator &operator++() {
            if (cur == nullptr) {
                throw "invalid";
            }
            cur = cur->next;
            return *this;
        }

        // iter--
        iterator operator--(int) {
            iterator tmp = *this;
            if (cur->prev == nullptr) {
                throw "invalid";
            }
            cur = cur->prev;
            return tmp;
        }

        //--iter
        iterator &operator--() {
            if (cur->prev == nullptr) {
                throw "invalid";
            }
            cur = cur->prev;
            return *this;
        }
        /**
         * if the iter didn't point to a value
         * throw " invalid"
         */
        T &operator*() const {
            if (cur) {
                return cur->data;
            } else {
                throw "invalid";
            }
        }

        // other operation
        T *operator->() const noexcept { return &cur->data; }
        bool operator==(const iterator &rhs) const {
            if (cur == rhs.cur) {
                return true;
            } else {
                return false;
            }
        }
        bool operator!=(const iterator &rhs) const {
            if (cur != rhs.cur) {
                return true;
            } else {
                return false;
            }
        }
    };
    class const_iterator {
       public:
        const Node<T> *cur;
        const_iterator() : cur(nullptr) {}
        const_iterator(const Node<T> *ptr) : cur(ptr) {}
        const_iterator(const const_iterator &other) : cur(other.cur) {}
        const_iterator operator--(int) {
            const_iterator tmp = *this;
            if (cur->prev == nullptr) {
                throw "invalid";
            }
            cur = cur->prev;
            return tmp;
        }

        //--iter
        const_iterator &operator--() {
            if (cur->prev == nullptr) {
                throw "invalid";
            }
            cur = cur->prev;
            return *this;
        }
        const_iterator operator++(int) {
            if (cur == nullptr) {
                throw "invalid";
            }
            const_iterator tmp(*this);
            cur = cur->next;
            return tmp;
        }
        const_iterator &operator++() {
            if (cur == nullptr) {
                throw "invalid";
            }
            cur = cur->next;
            return *this;
        }
        const T &operator*() const {
            if (!cur) {
                // std::cout << "const T &operator*() const" << std::endl;
                throw "invalid";
            }
            return cur->data;
        }
        const T *operator->() const noexcept { return &cur->data; }

        bool operator==(const const_iterator &rhs) const {
            return cur == rhs.cur;
        }
        bool operator!=(const const_iterator &rhs) const {
            return cur != rhs.cur;
        }
    };
    const_iterator begin() const { return const_iterator(head); }
    const_iterator end() const { return const_iterator(nullptr); }

    iterator begin() { return iterator(head); }

    iterator end() { return iterator(nullptr); }

    iterator insert(iterator pos, const T &val) {
        Node<T> *newNode = new Node<T>;
        newNode->data = val;
        // delete val;
        if (pos.cur == nullptr) {
            return pos;
        }
        Node<T> *tmp = pos.cur;
        if (tmp->prev) {
            tmp->prev->next = newNode;
            newNode->prev = tmp->prev;
        }
        newNode->next = tmp;
        tmp->prev = newNode;
        if (tmp == head) {
            head = newNode;
        }
        iterator newIter(newNode);
        // delete val;
        return newIter;
    }
    iterator erase(iterator pos) {
        if (pos.cur == nullptr) {
            return pos;
        }
        Node<T> *tmp = pos.cur;
        if (tmp->prev && tmp->next) {
            tmp->prev->next = tmp->next;
            tmp->next->prev = tmp->prev;
        }
        if (tmp->next && !tmp->prev) {
            tmp->next->prev = nullptr;
        }
        if (tmp->prev && !tmp->next) {
            tmp->prev->next = nullptr;
        }
        if (tmp == head) {
            head = tmp->next;
        }
        if (tmp == tail) {
            tail = tmp->prev;
        }
        iterator newIter(tmp->prev);
        if (tmp->data) {
            delete tmp->data;
            tmp->data = nullptr;
        }
        delete tmp;
        tmp = nullptr;
        return newIter;
    }
    iterator get_tail() const { return iterator(tail); }
    iterator get_tail() { return iterator(tail); }
    iterator get_head() const { return iterator(head); }

    void insert_head(const T &val) {
        Node<T> *newNode = new Node<T>(val);
        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
    }
    void insert_tail(const T &val) {
        Node<T> *newNode = new Node<T>(val);
        if (tail == nullptr) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }
    void delete_head() {
        if (!head) return;
        Node<T> *tmp = head;
        head = head->next;
        if (head) head->prev = nullptr;
        delete tmp;
    }
    void delete_tail() {
        if (!tail) return;
        Node<T> *tmp = tail;
        tail = tail->prev;
        if (tail) tail->next = nullptr;
        delete tmp;
    }

    bool empty() {
        if (head == nullptr) {
            return true;
        } else {
            return false;
        }
    }
    bool empty() const {
        if (head == nullptr) {
            return true;
        } else {
            return false;
        }
    }
    void clear() {
        Node<T> *tmp = head;
        while (tmp) {
            Node<T> *next = tmp->next;
            delete tmp->data;
            delete tmp;
            tmp = next;
        }
        head = tail = nullptr;
    }
};

template <class T>
class block {
   public:
    T **arr;
    int front;  // reachable
    int rear;   // unreachable
    int capacity;
    block(size_t cap = DEFAULT_BLOCK_SIZE) : front(0), rear(0), capacity(cap) {
        arr = new T *[capacity];
        for (size_t i = 0; i < capacity; ++i) {
            arr[i] = nullptr;
        }
    }
    block(const block<T> *other) {
        if (other != nullptr) {
            this->capacity = other->capacity;
            this->front = other->front;
            this->rear = other->rear;
            this->arr = new T *[capacity];
        }
        for (size_t i = 0; i < capacity; ++i) {
            if (other->arr[i] == nullptr) {
                this->arr[i] = nullptr;
            } else {
                this->arr[i] = new T(*(other->arr[i]));
            }
        }
    }
    // return size of current elements
    size_t size() const { return (rear - front + capacity) % capacity; }
    // calculate dist
    int dist(int start, int end) const {
        return (end - start + capacity) % capacity;
    }
    ~block() {
        // std::cout << "block destructor" << std::endl;
        for (int i = 0; i < capacity; ++i) {
            if (arr[i] == nullptr) {
                continue;
            }
            delete arr[i];
            arr[i] = nullptr;
        }
        delete[] arr;
        arr = nullptr;
        front = rear = 0;
    }
};

template <class T>
class deque {
   public:
    double_list<block<T> *> *list;  // store pointer to current doublelist
    int totalsize = 0;

    // Merge two blocks into one larger block
    block<T> *merge(block<T> *lhs, block<T> *rhs) {
        // std::cout << "enter the merge func" << std::endl;
        double factor = 2.0;
        int cap = (lhs->size() + rhs->size()) * factor;
        block<T> *newblock = new block<T>(cap);
        newblock->rear = lhs->size() + rhs->size();
        newblock->front = 0;
        // Copy elements from the left block (lhs) to the new block
        for (int i = 0; i < lhs->size(); ++i) {
            newblock->arr[i] = (lhs->arr[(i + lhs->front) % lhs->capacity]);
            (lhs->arr[(i + lhs->front) % lhs->capacity]) = nullptr;
        }
        // Copy elements from the right block (rhs) to the new block
        for (int i = 0; i < rhs->size(); ++i) {
            newblock->arr[i + lhs->size()] =
                rhs->arr[(i + rhs->front) % rhs->capacity];
            (rhs->arr[(i + rhs->front) % rhs->capacity]) = nullptr;
        }
        delete lhs;
        delete rhs;
        return newblock;
    }
    // Split a block into two smaller blocks
    block<T> *split(block<T> *to_split) {
        double factor = 1.003;
        int cap = to_split->capacity, mid = to_split->size() / 2;
        block<T> *lblock = new block<T>(cap);
        lblock->rear = mid;
        // Copy elements from the first half of the original block to the new
        // block
        for (int i = 0; i < lblock->rear; ++i) {
            lblock->arr[i] =
                to_split->arr[(i + to_split->front) % to_split->capacity];
            to_split->arr[(i + to_split->front) % to_split->capacity] = nullptr;
        }
        to_split->front = (to_split->front + mid) % to_split->capacity;
        // delete to_split;
        return lblock;
    }

    class const_iterator;
    class iterator {
       private:
        /**
         * add data members.
         * just add whatever you want.
         */
       public:
        // Internal iterator for traversing the doublelist
        typename double_list<block<T> *>::iterator iter;
        double_list<block<T> *> *mylist;
        int idx;
        T *p_val;
        iterator() : mylist(nullptr), idx(0), p_val(nullptr), iter(nullptr) {}
        iterator(double_list<block<T> *> *mylist,
                 const typename double_list<block<T> *>::iterator &it,
                 int index, T *p_val)
            : mylist(mylist), iter(it), idx(index), p_val(p_val) {}
        /*
         * return a new iterator which points to the n-next element.
         * if there are not enough elements, the behaviour is undefined.
         * same for operator-.
         */
        iterator operator+(const int &n) const {
            if (n < 0) {
                return *this - (-n);
            }
            iterator tmp = *this;
            int remain = n;
            if (tmp.iter.cur == nullptr || tmp.mylist == nullptr) {
                // std::cout << "in iterator operator+ tmp.iter.cur == nullptr"
                //           << std::endl;
                throw "invalid";
            }

            while (remain > 0) {
                if (tmp.iter.cur == nullptr) {
                    // std::cout
                    //     << "in iterator operator+ tmp.iter.cur == nullptr 2"
                    //     << std::endl;
                    throw "invalid";
                }
                if (tmp.iter.cur->data == nullptr) {
                    // std::cout << "in iterator operator+ curblock == nullptr"
                    //           << std::endl;
                    throw "Invalid block";
                }
                int mod = tmp.iter.cur->data->capacity;
                int avil = (tmp.iter.cur->data->dist(
                               tmp.idx, tmp.iter.cur->data->rear)) %
                           mod;

                if (remain < avil) {
                    tmp.idx = (tmp.idx + remain) % tmp.iter.cur->data->capacity;
                    remain = 0;
                } else {
                    remain -= (avil);
                    if (tmp.iter == tmp.mylist->get_tail()) {
                        if (remain == 0) {
                            tmp.idx = tmp.iter.cur->data->rear;
                            tmp.p_val = nullptr;
                            return tmp;
                        } else {
                            throw "invalid";
                        }
                    } else
                        tmp.iter++;
                    tmp.idx = tmp.iter.cur->data->front;
                }
            }
            tmp.p_val = (tmp.iter.cur->data->arr)[tmp.idx];
            return tmp;
        }
        iterator operator-(const int &n) const {
            if (n < 0) {
                return *this + (-n);
            }
            iterator tmp = *this;
            int remain = n;
            if (tmp.iter.cur == nullptr || tmp.mylist == nullptr) {
                // std::cout << "in iterator operator- tmp.iter.cur == nullptr"
                //           << std::endl;
                throw "invalid";
            }
            block<T> *curblock = tmp.iter.cur->data;

            while (remain > 0) {
                if (tmp.iter.cur == nullptr) {
                    // std::cout
                    //     << "in iterator operator- tmp.iter.cur == nullptr 2"
                    //     << std::endl;
                    throw "invalid";
                }
                if (tmp.iter.cur->data == nullptr) {
                    // std::cout << "in iterator operator- curblock == nullptr"
                    //           << std::endl;
                    throw "Invalid block";
                }
                int avil = tmp.iter.cur->data->dist(tmp.iter.cur->data->front,
                                                    tmp.idx);
                if (tmp.iter == tmp.mylist->get_head() && remain > avil) {
                    // std::cout << "in iterator operator- exceed begin()"
                    //           << std::endl;
                    throw "Invalid";
                }
                if (remain <= avil) {
                    tmp.idx =
                        (tmp.idx - remain + tmp.iter.cur->data->capacity) %
                        tmp.iter.cur->data->capacity;
                    remain = 0;
                } else {
                    remain -= (avil + 1);
                    if (tmp.iter == tmp.mylist->get_head()) {
                        throw "invalid";
                    } else
                        tmp.iter--;
                    tmp.idx = (tmp.iter.cur->data->rear - 1 +
                               tmp.iter.cur->data->capacity) %
                              tmp.iter.cur->data->capacity;
                }
            }
            tmp.p_val = tmp.iter.cur->data->arr[tmp.idx];
            return tmp;
        }

        /**
         * return the distance between two iterators.
         * if they point to different vectors, throw
         * invaild_iterator.
         */
        int operator-(const iterator &rhs) const {
            if (this->mylist != rhs.mylist) {
                // std::cout << "oh" << std::endl;
                throw "invaild_iterator";
            }
            iterator tmp = *this;
            int ans1 = 0, ans2 = 0;
            if (tmp.iter.cur == nullptr || rhs.iter.cur == nullptr) {
                // std::cout << "in int operator-  cur nullptr" << std::endl;
                throw "Iterator out of range";
            }
            block<T> *block1 = (tmp).iter.cur->data;
            block<T> *block2 = (rhs).iter.cur->data;
            if (block1 == nullptr || block2 == nullptr) {
                // std::cout << "in int operator-  block nullptr" << std::endl;
                throw "Invalid block";
            }
            ans1 += block1->dist(tmp.idx, block1->rear);
            ans2 += block2->dist(rhs.idx, block2->rear);
            while (tmp.iter != tmp.mylist->get_tail()) {
                ++(tmp.iter);
                block1 = tmp.iter.cur->data;
                ans1 += block1->size();
            }
            tmp = rhs;
            while (tmp.iter != tmp.mylist->get_tail()) {
                ++(tmp.iter);
                block2 = tmp.iter.cur->data;
                ans2 += block2->size();
            }
            return ans2 - ans1;
        }
        iterator &operator+=(const int &n) {
            *this = (*this) + n;
            return *this;
        }
        iterator &operator-=(const int &n) {
            *this = (*this) - n;
            return *this;
        }

        /**
         * iter++
         */
        iterator operator++(int) {
            iterator tmp = *this;
            *this = (*this) + 1;
            return tmp;
        }
        /**
         * ++iter
         */
        iterator &operator++() {
            *this = (*this) + 1;
            return *this;
        }
        /**
         * iter--
         */
        iterator operator--(int) {
            iterator tmp = *this;
            *this = (*this) - 1;
            return tmp;
        }
        /**
         * --iter
         */
        iterator &operator--() {
            *this = (*this) - 1;
            return *this;
        }

        /**
         * *it
         */
        T &operator*() const {
            if (p_val == nullptr) {
                // std::cout << "invalid * 1" << std::endl;
                throw "invalid";
            }
            return *p_val;
        }
        /**
         * it->field
         */
        T *operator->() const noexcept { return p_val; }

        /**
         * check whether two iterators are the same (pointing to the same
         * memory).
         */
        bool operator==(const iterator &rhs) const {
            if (iter.cur != rhs.iter.cur) {
                return false;
            } else if (idx != rhs.idx || p_val != rhs.p_val) {
                return false;
            }
            return true;
        }
        bool operator==(const const_iterator &rhs) const {
            if (iter.cur != rhs.iter.cur) {
                return false;
            } else if (idx != rhs.idx || p_val != rhs.p_val) {
                return false;
            }
            return true;
        }
        /**
         * some other operator for iterators.
         */
        bool operator!=(const iterator &rhs) const { return !((*this) == rhs); }
        bool operator!=(const const_iterator &rhs) const {
            return !((*this) == rhs);
        }
    };

    class const_iterator {
       private:
        /**
         * add data members.
         * just add whatever you want.
         */
       public:
        typename double_list<block<T> *>::iterator iter;
        double_list<block<T> *> *mylist;
        int idx;
        T *p_val;
        const_iterator()
            : mylist(nullptr), idx(0), p_val(nullptr), iter(nullptr) {}
        const_iterator(double_list<block<T> *> *mylist,
                       const typename double_list<block<T> *>::iterator &it,
                       int index, T *p_val)
            : mylist(mylist), iter(it), idx(index), p_val(p_val) {}
        /**
         * return a new iterator which points to the n-next element.
         * if there are not enough elements, the behaviour is undefined.
         * same for operator-.
         */
        const_iterator operator+(const int &n) const {
            if (n < 0) {
                return *this - (-n);
            }
            const_iterator tmp = *this;
            int remain = n;
            if (tmp.iter.cur == nullptr || tmp.mylist == nullptr) {
                // std::cout
                //     << "in const_iterator operator+ tmp.iter.cur == nullptr"
                //     << std::endl;
                throw "invalid";
            }

            while (remain > 0) {
                if (tmp.iter.cur == nullptr) {
                    // std::cout << "in const_iterator operator+ tmp.iter.cur ==
                    // "
                    //              "nullptr 2"
                    //           << std::endl;
                    throw "invalid";
                }
                if (tmp.iter.cur->data == nullptr) {
                    // std::cout
                    //     << "in const_iterator operator+ curblock == nullptr"
                    //     << std::endl;
                    throw "Invalid block";
                }
                int mod = tmp.iter.cur->data->capacity;
                int avil = (tmp.iter.cur->data->dist(
                               tmp.idx, tmp.iter.cur->data->rear)) %
                           mod;

                if (remain < avil) {
                    tmp.idx = (tmp.idx + remain) % tmp.iter.cur->data->capacity;
                    remain = 0;
                } else {
                    remain -= (avil);
                    if (tmp.iter == tmp.mylist->get_tail()) {
                        if (remain == 0) {
                            tmp.idx = tmp.iter.cur->data->rear;
                            tmp.p_val = nullptr;
                            return tmp;
                        } else {
                            // std::cout << "OMG2" << std::endl;
                            throw "invalid";
                        }
                    } else
                        tmp.iter++;
                    tmp.idx = tmp.iter.cur->data->front;
                }
            }
            tmp.p_val = (tmp.iter.cur->data->arr)[tmp.idx];
            return tmp;
        }
        const_iterator operator-(const int &n) const {
            if (n < 0) {
                return *this + (-n);
            }
            const_iterator tmp = *this;
            int remain = n;
            if (tmp.iter.cur == nullptr || tmp.mylist == nullptr) {
                // std::cout
                //     << "in const_iterator operator- tmp.iter.cur == nullptr"
                //     << std::endl;
                throw "invalid";
            }
            block<T> *curblock = tmp.iter.cur->data;

            while (remain > 0) {
                if (tmp.iter.cur == nullptr) {
                    // std::cout << "in const_iterator operator- tmp.iter.cur ==
                    // "
                    //              "nullptr 2"
                    //           << std::endl;
                    throw "invalid";
                }
                if (tmp.iter.cur->data == nullptr) {
                    // std::cout
                    //     << "in const_iterator operator- curblock == nullptr"
                    //     << std::endl;
                    throw "Invalid block";
                }
                int avil = tmp.iter.cur->data->dist(tmp.iter.cur->data->front,
                                                    tmp.idx);
                if (tmp.iter == tmp.mylist->get_head() && remain > avil) {
                    // std::cout << "in const_iterator operator- exceed begin()"
                    //           << std::endl;
                    throw "Invalid";
                }
                if (remain <= avil) {
                    tmp.idx =
                        (tmp.idx - remain + tmp.iter.cur->data->capacity) %
                        tmp.iter.cur->data->capacity;
                    remain = 0;
                } else {
                    remain -= (avil + 1);
                    if (tmp.iter == tmp.mylist->get_head()) {
                        throw "invalid";
                    } else
                        tmp.iter--;
                    tmp.idx = (tmp.iter.cur->data->rear - 1 +
                               tmp.iter.cur->data->capacity) %
                              tmp.iter.cur->data->capacity;
                }
            }
            tmp.p_val = tmp.iter.cur->data->arr[tmp.idx];
            return tmp;
        }

        /**
         * return the distance between two const_iterators.
         * if they point to different vectors, throw
         * invaild_const_iterator.
         */
        int operator-(const const_iterator &rhs) const {
            if (this->mylist != rhs.mylist) {
                // std::cout << "oh" << std::endl;
                throw "invaild_iterator";
            }
            const_iterator tmp = *this;
            int ans1 = 0, ans2 = 0;
            if (tmp.iter.cur == nullptr || rhs.iter.cur == nullptr) {
                // std::cout << "in int operator-  cur nullptr" << std::endl;
                throw "const_iterator out of range";
            }
            block<T> *block1 = (tmp).iter.cur->data;
            block<T> *block2 = (rhs).iter.cur->data;
            if (block1 == nullptr || block2 == nullptr) {
                // std::cout << "in int operator-  block nullptr" << std::endl;
                throw "Invalid block";
            }
            ans1 += block1->dist(tmp.idx, block1->rear);
            ans2 += block2->dist(rhs.idx, block2->rear);
            while (tmp.iter != tmp.mylist->get_tail()) {
                ++(tmp.iter);
                block1 = tmp.iter.cur->data;
                ans1 += block1->size();
            }
            tmp = rhs;
            while (tmp.iter != tmp.mylist->get_tail()) {
                ++(tmp.iter);
                block2 = tmp.iter.cur->data;
                ans2 += block2->size();
            }
            return ans2 - ans1;
        }
        const_iterator &operator+=(const int &n) {
            *this = (*this) + n;
            return *this;
        }
        const_iterator &operator-=(const int &n) {
            *this = (*this) - n;
            return *this;
        }

        /**
         * iter++
         */
        const_iterator operator++(int) {
            const_iterator tmp = *this;
            *this = (*this) + 1;
            return tmp;
        }
        /**
         * ++iter
         */
        const_iterator &operator++() {
            *this = (*this) + 1;
            return *this;
        }
        /**
         * iter--
         */
        const_iterator operator--(int) {
            const_iterator tmp = *this;
            *this = (*this) - 1;
            return tmp;
        }
        /**
         * --iter
         */
        const_iterator &operator--() {
            *this = (*this) - 1;
            return *this;
        }

        /**
         * *it
         */
        const T &operator*() const {
            if (p_val == nullptr) {
                // std::cout << "invalid *" << std::endl;
                throw "invalid";
            }
            return *p_val;
        }
        /**
         * it->field
         */
        const T *operator->() const noexcept { return p_val; }

        /**
         * check whether two const_iterators are the same (pointing to the same
         * memory).
         */
        bool operator==(const iterator &rhs) const {
            if (iter.cur != rhs.iter.cur) {
                return false;
            } else if (idx != rhs.idx || p_val != rhs.p_val) {
                return false;
            }
            return true;
        }
        bool operator==(const const_iterator &rhs) const {
            if (iter.cur != rhs.iter.cur) {
                return false;
            } else if (idx != rhs.idx || p_val != rhs.p_val) {
                return false;
            }
            return true;
        }
        /**
         * some other operator for iterators.
         */
        bool operator!=(const iterator &rhs) const { return !((*this) == rhs); }
        bool operator!=(const const_iterator &rhs) const {
            return !((*this) == rhs);
        }
    };

    /**
     * constructors.
     */
    deque() {
        list = new double_list<block<T> *>;
        block<T> *block1 = new block<T>();
        list->insert_tail(block1);
    }
    deque(const deque &other) {
        list = new double_list<block<T> *>;
        typename double_list<block<T> *>::iterator it = other.list->begin();
        while (it != other.list->end()) {
            block<T> *newblock = new block<T>(*it);
            list->insert_tail(newblock);
            ++it;
        }
        totalsize = other.totalsize;
    }

    /**
     * deconstructor.
     */
    ~deque() {
        // std::cout<<"~deque" << std::endl;
        // for(auto x=list->get_head();x!=list->end();++x){
        //     delete x.cur->data;
        // }
        delete list;
        list = nullptr;
    }

    /**
     * assignment operator.
     */
    deque &operator=(const deque &other) {
        if (&other == this) {
            return *this;
        }
        delete list;
        totalsize = 0;
        list = new double_list<block<T> *>;
        typename double_list<block<T> *>::iterator otherit =
            other.list->begin();
        while (otherit != other.list->end()) {
            block<T> *newblock = new block<T>(*otherit);
            list->insert_tail(newblock);
            ++otherit;
        }
        totalsize = other.totalsize;
        return *this;
    }

    /**
     * access a specified element with bound checking.
     * throw index_out_of_bound if out of bound.
     */
    T &at(const size_t &pos) {
        if (pos >= totalsize) {
            // std::cout << "T &at" << std::endl;
            throw "index_out_of_bound";
        }
        return *(begin() + pos);
    }
    const T &at(const size_t &pos) const {
        if (pos >= totalsize) {
            // std::cout << "T &at const" << std::endl;
            throw "index_out_of_bound";
        }
        return *(cbegin() + pos);
    }
    T &operator[](const size_t &pos) {
        if (pos >= totalsize) {
            // std::cout << "T &operator[]" << std::endl;
            throw "index_out_of_bound";
        }
        return *(begin() + pos);
    }
    const T &operator[](const size_t &pos) const {
        if (pos >= totalsize) {
            // std::cout << "T &operator[] const" << std::endl;
            throw "index_out_of_bound";
        }
        return *(cbegin() + pos);
    }

    /**
     * access the first element.
     * throw container_is_empty when the container is empty.
     */
    const T &front() const {
        if (totalsize == 0) {
            // std::cout << "const T &front()" << std::endl;
            throw "container_is_empty";
        }
        return *cbegin();
    }
    /**
     * access the last element.
     * throw container_is_empty when the container is empty.
     */
    const T &back() const {
        if (totalsize == 0) {
            // std::cout << "const T &front()" << std::endl;
            throw "container_is_empty";
        }
        return *(cend() - 1);
    }

    /**
     * return an iterator to the beginning.
     */
    iterator begin() {
        block<T> *block1 = list->begin().cur->data;
        return iterator(list, list->begin(), block1->front,
                        block1->arr[block1->front]);
    }
    const_iterator cbegin() const {
        block<T> *block1 = list->begin().cur->data;
        return const_iterator(list, list->begin(), block1->front,
                              block1->arr[block1->front]);
    }

    /**
     * return an iterator to the end.
     */
    iterator end() {
        block<T> *block1 = list->get_tail().cur->data;
        return iterator(list, list->get_tail(), block1->rear, nullptr);
    }
    const_iterator cend() const {
        block<T> *block1 = list->get_tail().cur->data;
        return const_iterator(list, list->get_tail(), block1->rear, nullptr);
    }

    /**
     * check whether the container is empty.
     */
    bool empty() const { return totalsize == 0; }

    /**
     * return the number of elements.
     */
    size_t size() const { return totalsize; }

    /**
     * clear all contents.
     */
    void clear() {
        if (totalsize == 0) return;
        delete list;
        list = new double_list<block<T> *>;
        totalsize = 0;
        block<T> *block1 = new block<T>();
        list->insert_tail(block1);
    }

    /**
     * insert value before pos.
     * return an iterator pointing to the inserted value.
     * throw if the iterator is invalid or it points to a wrong place.
     */
    iterator insert(iterator pos, const T &value) {
        if (pos.mylist != list) {
            // std::cout << "xiba" << std::endl;
            throw "invalid";
        }
        if (pos.iter == nullptr) {
            throw "invalid";
        }
        block<T> *curblock = pos.iter.cur->data;
        ++totalsize;
        // Find the target position in the current block
        int target = curblock->dist(curblock->front, pos.idx);
        if (curblock->size() >= curblock->capacity - 2) {
            // Split the block when nearly full
            block<T> *newblock = split(curblock);
            // Insert the new block in the list
            list->insert(pos.iter, newblock);
            if (target < newblock->size()) {
                curblock = newblock;
                newblock = nullptr;
                pos.iter--;
            } else {
                target = target - newblock->size();
            }
        }
        int mod = curblock->capacity;
        int nowrear = curblock->rear;
        int nowfront = curblock->front;
        int posidx = (target + curblock->front) % mod;
        // std::cout << "front " << curblock->front << std::endl;
        int tmpidx = nowrear;  // Temporary index for traversing the block
        // If the insertion is at the rear position
        if (posidx == nowrear) {
            curblock->rear = (nowrear + 1) % mod;
            if (curblock->arr[nowrear]) {
                *(curblock->arr[nowrear]) = value;
            } else {
                curblock->arr[nowrear] = new T(value);
            }
            pos.idx = nowrear;
            pos.p_val = curblock->arr[nowrear];
            return pos;
        }
        // If the insertion is at the front position
        if (posidx == nowfront) {
            curblock->front = (nowfront - 1 + mod) % mod;
            // std::cout << "again front " << curblock->front << std::endl;

            if (curblock->arr[curblock->front]) {
                *(curblock->arr[curblock->front]) = value;
            } else {
                curblock->arr[curblock->front] = new T(value);
            }
            pos.p_val = curblock->arr[curblock->front];
            // std::cout<<std::endl;
            //  for (int i = 0; i < curblock->capacity; ++i) {
            //           if (curblock->arr[i] != nullptr)
            //               std::cout << *(curblock->arr[i]) << " ";
            //           else
            //               std::cout << "NULL ";
            //       }
            pos.idx = curblock->front;
            return pos;
        }
        // If inserting in the middle, shift elements and insert the value at
        // the correct position
        while (tmpidx != posidx) {
            if (curblock->arr[tmpidx]) {
                *(curblock->arr[tmpidx]) =
                    *(curblock->arr[(tmpidx - 1 + mod) % mod]);
            } else {
                curblock->arr[tmpidx] =
                    new T(*(curblock->arr[(tmpidx - 1 + mod) % mod]));
            }
            tmpidx = (tmpidx - 1 + mod) % mod;
        }
        *(curblock->arr[tmpidx]) = value;
        curblock->rear = (nowrear + 1) % mod;
        pos.p_val = (curblock->arr[tmpidx]);
        pos.idx = tmpidx;
        return pos;
    }

    /**
     * remove the element at pos.
     * return an iterator pointing to the following element. if pos points to
     * the last element, return end(). throw if the container is empty,
     * the iterator is invalid, or it points to a wrong place.
     */
    iterator erase(iterator pos) {
        if (pos.mylist != list) {
            // std::cout << "cao" << std::endl;
            throw "invalid";
        }
        if (pos.iter == nullptr) {
            // std::cout << "cao?" << std::endl;
            throw "invalid";
        }
        int target =
            pos.iter.cur->data->dist(pos.iter.cur->data->front, pos.idx);
        
            // Merging blocks if the block size is too small
        if (pos.iter.cur->data->size() < std::sqrt(totalsize) / 2) {
            // std::cout<<"merge!!!"<<std::endl;
            if (pos.iter.cur->next != nullptr) {
                block<T> *newblock =
                    merge(pos.iter.cur->data, pos.iter.cur->next->data);
                pos.iter.cur->data = newblock;
                typename double_list<block<T> *>::iterator iter1(
                    pos.iter.cur->next);
                // delete pos.iter.cur->next->data;
                pos.iter.cur->next->data = nullptr;
                pos.iter = list->erase(iter1);
            }
        }

        --totalsize;
        int nowrear = pos.iter.cur->data->rear;
        int mod = pos.iter.cur->data->capacity;
        int posidx = (target + pos.iter.cur->data->front) % mod;
        if (posidx == pos.iter.cur->data->rear) {
            // std::cout << "iterator erase(iterator pos) end()" << std::endl;
            throw "invalid";
        }
        delete pos.iter.cur->data->arr[posidx];
        pos.iter.cur->data->arr[posidx] = nullptr;
        // update the front index and adjust the block 
        // if the position is at the front
        if (posidx == pos.iter.cur->data->front) {
            pos.iter.cur->data->front = (pos.iter.cur->data->front + 1) % mod;
            if (pos.iter.cur->data->front == pos.iter.cur->data->rear) {
                if (pos.mylist->get_tail() == pos.iter && totalsize != 0) {
                    list->erase(pos.iter);
                    return end();
                }
                auto nextit = pos.iter.cur->next;
                list->erase(pos.iter);
                if (totalsize == 0) {
                    block<T> *block1 = new block<T>();
                    list->insert_tail(block1);
                    return end();
                }
                pos.iter = nextit;
                // pos.idx = 0;
                // pos.p_val = nullptr;
            }
            pos.idx = pos.iter.cur->data->front;
            pos.p_val = pos.iter.cur->data->arr[pos.iter.cur->data->front];
            // std::cout << std::endl;
            //  for (int i = 0; i < pos.iter.cur->data->capacity; ++i) {
            //      if (pos.iter.cur->data->arr[i] != nullptr)
            //          std::cout << *(pos.iter.cur->data->arr[i]) << " ";
            //      else
            //          std::cout << "NULL ";
            //  }
            return pos;
        }
        // update the rear index if the position is at the rea
        if (posidx == (nowrear - 1 + pos.iter.cur->data->capacity) %
                          pos.iter.cur->data->capacity) {
            if (pos.mylist->get_tail() == pos.iter) {
                pos.iter.cur->data->rear = (nowrear - 1 + mod) % mod;
                pos.p_val = nullptr;
                if (pos.iter.cur->data->front == pos.iter.cur->data->rear) {
                    pos.iter = list->erase(pos.iter);
                }
                // std::cout << std::endl;
                //  for (int i = 0; i < pos.iter.cur->data->capacity; ++i) {
                //      if (pos.iter.cur->data->arr[i] != nullptr)
                //          std::cout << *(pos.iter.cur->data->arr[i]) << " ";
                //      else
                //          std::cout << "NULL ";
                //  }
                // pos.idx = 0;
                // pos.p_val = nullptr;
                return end();
            }

            pos.iter.cur->data->rear = (nowrear - 1 + mod) % mod;
            // std::cout << std::endl;
            //  for (int i = 0; i < pos.iter.cur->data->capacity; ++i) {
            //      if (pos.iter.cur->data->arr[i] != nullptr)
            //          std::cout << *(pos.iter.cur->data->arr[i]) << " ";
            //      else
            //          std::cout << "NULL ";
            //  }
            pos.iter++;
            pos.p_val = pos.iter.cur->data->arr[pos.iter.cur->data->front];
            pos.idx = pos.iter.cur->data->front;
            return pos;
        }
        // Shift the elements and remove the value at the target position
        pos.p_val = pos.iter.cur->data->arr[(posidx + 1) % mod];
        pos.idx = (posidx) % mod;
        while (posidx != nowrear) {
            pos.iter.cur->data->arr[posidx] =
                pos.iter.cur->data->arr[(posidx + 1) % mod];
            posidx = (posidx + 1) % mod;
        }
        pos.iter.cur->data->rear = (nowrear - 1 + mod) % mod;
        return pos;
    }

    /**
     * add an element to the end.
     */
    void push_back(const T &value) { insert(end(), value); }

    /**
     * remove the last element.
     * throw when the container is empty.
     */
    void pop_back() {
        if (totalsize == 0) {
            // std::cout << "pop_front() empty" << std::endl;
            throw "invalid";
        }
        erase(end() - 1);
    }

    /**
     * insert an element to the beginning.
     */
    void push_front(const T &value) { insert(begin(), value); }

    /**
     * remove the first element.
     * throw when the container is empty.
     */
    void pop_front() {
        if (totalsize == 0) {
            // std::cout << "pop_front() empty" << std::endl;
            throw "invalid";
        }
        erase(begin());
    }
};

}  // namespace sjtu
#endif