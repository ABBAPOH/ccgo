#ifndef CLASS_H
#define CLASS_H

#include <QSharedDataPointer>

class ClassData;

class Class
{
public:
    Class();
    Class(const Class &);
    Class &operator=(const Class &);
    ~Class();

private:
    QSharedDataPointer<ClassData> data;
};

#endif // CLASS_H
