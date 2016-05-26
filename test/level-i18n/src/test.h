
#include <QObject>

class Test : public QObject
{
    Q_OBJECT
public:
    Test();
    virtual ~Test() {}

    void print();
};
