#include "yamitracker.h"
#include "ui_yamitracker.h"

yamitracker::yamitracker(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::yamitracker)
{
    ui->setupUi(this);
}

yamitracker::~yamitracker()
{
    delete ui;
}
