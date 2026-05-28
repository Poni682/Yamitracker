#include "styleButton.h"
#include "styleListButton.h"

void styleButton::buttonClick(QList<QPushButton*> button){
    if (button.empty()){
        return;
    }
    for (QPushButton* btn : button){
        btn->setStyleSheet(clickButton);
    }
}

void styleButton::buttonCheck(QList<QPushButton*> button){
    if (button.empty()){
        return;
    }
    for (QPushButton *btn : button){
        btn->setCheckable(true);
        btn->setStyleSheet(checkButton);
    }
}

void styleButton::buttonBlack(QList<QPushButton*> button){
    if (button.empty()){
        return;
    }
    for (QPushButton* btn : button){
        btn->setStyleSheet(blackB);
    }
}

void styleButton::setStyleFromQString(QList<QPushButton*> button, QString style){
    if (button.empty()){
        return;
    }
    for (auto *btn : button){
        btn->setStyleSheet(style);
    }
}
