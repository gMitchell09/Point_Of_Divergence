#ifndef IMENUITEM_H
#define IMENUITEM_H

class IMenuItem {
public:
    virtual ~IMenuItem() {}
    virtual void setListener(void* listener) = 0;
    virtual void saveValue() = 0;
};

#endif // IMENUITEM_H
