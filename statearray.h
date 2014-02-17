#ifndef STATEARRAY_H
#define STATEARRAY_H

#define nullptr (void*)0

template<typename T>
struct Node {
    Node* next;
    Node* prev;
    T data;

    ~Node() {
        if (next) {
            delete next;
            next = nullptr;
            prev = nullptr;
        }
    }
};

template<typename T>
class StateArray
{
private:
    Node<T> *m_linkedListFirst;
    Node<T> *m_linkedListLast;

public:
    StateArray();

    void append(T obj) {
        if (m_linkedListLast) {
            Node<T> *n = new Node<T>;

            n->prev = m_linkedListLast;
            n->next = nullptr;
            n->data = obj;

            m_linkedListLast->next = n;
            m_linkedListLast = n;
        } else {
            m_linkedListFirst = m_linkedListLast =
                    new Node<T>;

            m_linkedListFirst->next = nullptr;
            m_linkedListFirst->prev = nullptr;
            m_linkedListFirst->data = obj;
        }
    }

    void prepend(T obj) {
        if (m_linkedListFirst) {
            Node<T> *n = new Node<T>;
            n->prev = nullptr;
            n->next = m_linkedListFirst;
            n->data = obj;

            m_linkedListFirst->prev = n;
            m_linkedListFirst = n;
        } else {
            m_linkedListFirst = m_linkedListLast =
                    new Node<T>;

            m_linkedListFirst->next = nullptr;
            m_linkedListFirst->prev = nullptr;
            m_linkedListFirst->data = obj;
        }
    }

    // Stack Methods
    inline void push(T obj) { this->append(obj); }
    T pop() {
        if (m_linkedListLast) {
            Node<T> data = m_linkedListLast->data;
            Node<T> *ptr = m_linkedListLast;

            if (m_linkedListLast->prev) {
                m_linkedListLast->prev->next = nullptr;
                m_linkedListLast = m_linkedListLast->prev;
            } else {
                m_linkedListLast = m_linkedListFirst = nullptr;
            }

            delete ptr;
            return data;
        }
    }

    T top();

    // Circular Buffer methods
    T rotateReverse();
    T rotateForward();

    virtual ~StateArray();
};

#endif // STATEARRAY_H
