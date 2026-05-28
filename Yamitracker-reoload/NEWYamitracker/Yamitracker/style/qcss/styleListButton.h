#ifndef STYLELISTTOP_H
#define STYLELISTTOP_H
#include <QString>

QString const blackB =
    "background-color: #262626";

QString const clickButton =
    "QPushButton:hover {"
    "    background-color: #D3D3D3;"
    "    color: #000000"
    "}"
    "QPushButton:click {"
    "    background-color: #4CAF50;"
    "    color: white;"
    "    border: 2px solid #45a049;"
    "}";

QString const checkButton =
    "QPushButton:hover {"
    "    background-color: #D3D3D3;"
    "    color: #000000"
    "}"
    "QPushButton:checked {"
    "    background-color: #4CAF50;"
    "    color: white;"
    "    border: 2px solid #45a049;"
    "}";

//Стиль клавишь, нажатия на кнопку синтезатора

QString const clickBlack =
    "QPushButton {"
    "background-color: #050505;"
    "}"
    "QPushButton:pressed {"
    "    background-color: #1c1c1c;"
    "    color: white;"
    "    border: 2px solid red;"
    "}";

//Нажатие на клавишу старт и её отрисовка
QString const checkPlayStop =
    "QPushButton:checked {"
    "    background-color: red;"
    "    color: black;"
    "    border: 2px solid black;"
    "}";

#endif // STYLELISTTOP_H
