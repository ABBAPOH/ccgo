#include "class.h"
#include <QSharedData>

class ClassData : public QSharedData {
public:
};

Class::Class() : data(new ClassData)
{
}

Class::Class(const Class &rhs) : data(rhs.data)
{
}

Class &Class::operator=(const Class &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

Class::~Class()
{
}
