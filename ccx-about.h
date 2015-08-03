#ifndef CCXABOUT_H
#define CCXABOUT_H

#include <QWidget>

namespace Ui {
class CCXAbout;
}

class CCXAbout : public QWidget
{
    Q_OBJECT

public:
    explicit CCXAbout(QWidget *parent = 0);
    ~CCXAbout();

private:
    Ui::CCXAbout *ui;
};

#endif // CCXABOUT_H
