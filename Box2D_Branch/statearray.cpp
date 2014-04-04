#include "statearray.h"

template<typename T>
StateArray<T>::StateArray()
{
}

template<typename T>
StateArray<T>::~StateArray() {
    if (m_linkedListFirst) delete m_linkedListFirst;

    m_linkedListFirst = m_linkedListLast = nullptr;
}
