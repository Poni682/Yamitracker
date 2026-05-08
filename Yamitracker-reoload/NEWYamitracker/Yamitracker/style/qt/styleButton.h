#ifndef STYLEBUTTONTOP_H
#define STYLEBUTTONTOP_H
#include "qpushbutton.h"
#include <QObject>
#include <QList>
namespace styleButton {
    //Top(Start, Stop...)

    void buttonClick(QList<QPushButton*> button);
    void buttonCheck(QList<QPushButton*> button);
    void buttonBlack(QList<QPushButton*> button);
    void noteBlack(QList<QPushButton*> button);


//общие стили для любой кнопки, не заисимо от того QPushButton или QCheckBox и люболь стиль, не связанный стандартом каким-либо и (её перегрузки функции для разных кнопок будет сдеано в будущем)
    void setStyleFromQString(QList<QPushButton*> button, QString style);
}




#endif // STYLEBUTTONTOP_H
