#include "ccx-about.h"
#include "ui_ccx-about.h"

CCXAbout::CCXAbout(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CCXAbout)
{
    ui->setupUi(this);
    this->setWindowTitle("About CCExtractor");
    this->setFixedSize(this->width(), this->height());
}

CCXAbout::~CCXAbout()
{
    delete ui;
}
