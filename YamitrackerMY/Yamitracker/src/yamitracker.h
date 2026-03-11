#ifndef YAMITRACKER_H
#define YAMITRACKER_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class yamitracker;
}
QT_END_NAMESPACE

class yamitracker : public QMainWindow
{
    Q_OBJECT

public:
    yamitracker(QWidget *parent = nullptr);
    ~yamitracker();

private:
    Ui::yamitracker *ui;
};
#endif // YAMITRACKER_H
