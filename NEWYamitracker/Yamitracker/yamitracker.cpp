#include "yamitracker.h"
#include "ui_yamitracker.h"

yamitracker::yamitracker(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::yamitracker)
{
    ui->setupUi(this);

    //Радио кнопка дорожки
    trackGroup = new QButtonGroup(this);
    trackGroup->addButton(ui->track_button_1, 1);
    trackGroup->addButton(ui->track_button_2, 2);
    trackGroup->addButton(ui->track_button_3, 3);
    trackGroup->addButton(ui->track_button_4, 4);
    trackGroup->addButton(ui->track_button_5, 5);
    trackGroup->addButton(ui->track_button_6, 6);
    trackGroup->addButton(ui->track_button_7, 7);
    trackGroup->setExclusive(true);
    setTarckButtonStyles();
    connect(trackGroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked),
            this, &yamitracker::onTrackSelected);

    deleteTrackGroup = new QButtonGroup(this);
    deleteTrackGroup->addButton(ui->delete_track_button_1, 1);
    deleteTrackGroup->addButton(ui->delete_track_button_2, 2);
    deleteTrackGroup->addButton(ui->delete_track_button_3, 3);
    deleteTrackGroup->addButton(ui->delete_track_button_4, 4);
    deleteTrackGroup->addButton(ui->delete_track_button_5, 5);
    deleteTrackGroup->addButton(ui->delete_track_button_6, 6);
    deleteTrackGroup->addButton(ui->delete_track_button_7, 7);
    setDeleteTarckButtonStyles();
    connect(deleteTrackGroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked),
            this, &yamitracker::onDeleteTrackSelected);

    selectTrackGroup = new QButtonGroup(this);
    selectTrackGroup->addButton(ui->delete_track_button_1, 1);
    selectTrackGroup->addButton(ui->delete_track_button_2, 2);
    selectTrackGroup->addButton(ui->delete_track_button_3, 3);
    selectTrackGroup->addButton(ui->delete_track_button_4, 4);
    selectTrackGroup->addButton(ui->delete_track_button_5, 5);
    selectTrackGroup->addButton(ui->delete_track_button_6, 6);
    selectTrackGroup->addButton(ui->delete_track_button_7, 7);
    setSelectTrackButtonStules();

}

void yamitracker::onDeleteTrackSelected(QAbstractButton *button){
    qDebug() << "Выбрана дорожка: " << button->text() << " " << getDeleteTrackGroup(button);
}

void yamitracker::onTrackSelected(QAbstractButton *button)
{
    qDebug() << "Выбрана дорожка: " << button->text();

    // Ваша логика выбора дорожки
    // switch(trackGroup->id(button)) {
    // case 1:
    //     // Активировать дорожку 1
    //     break;
    // case 2:
    //     // Активировать дорожку 2
    //     break;
    // case 3:
    //     // Активировать дорожку 3
    //     break;
    // case 4:
    //     // Активировать дорожку 4
    //     break;
    // case 5:
    //     // Активировать дорожку 5
    //     break;
    // case 6:
    //     // Активировать дорожку 6
    //     break;
    // }
}

void yamitracker::setDeleteTarckButtonStyles(){
    QList<QPushButton*> buttons = {
        ui->delete_track_button_1, ui->delete_track_button_2,
        ui->delete_track_button_3, ui->delete_track_button_4,
        ui->delete_track_button_5, ui->delete_track_button_6,
        ui->delete_track_button_7
    };

    QString styleSheet =
        "QPushButton:hover {"
        "    background-color: #D3D3D3;"
        "    color: #000000"
        "}"
        "QPushButton:click {"
        "    background-color: #4CAF50;"
        "    color: white;"
        "    border: 2px solid #45a049;"
        "}";

    for (QPushButton *btn : buttons){
        btn->setStyleSheet(styleSheet);
    }
}

void yamitracker::setSelectTrackButtonStules(){
    QList<QPushButton*> button = {
        ui->select_button_1, ui->select_button_2,
        ui->select_button_3, ui->select_button_4,
        ui->select_button_, ui->select_button_,
        ui->select_button_,
    };

    QString styleSheet =
        "QPushButton:hover {"
        "    background-color: #D3D3D3;"
        "    color: #000000"
        "}"
        "QPushButton:checked {"
        "    background-color: #4CAF50;"
        "    color: white;"
        "    border: 2px solid #45a049;"
        "}";

    for (QPushButton *btn : button){
        btn->setCheckable(true);
        btn->setStyleSheet(styleSheet);
    }
}

void yamitracker::setTarckButtonStyles()
{
    QList<QPushButton*> buttons = {
        ui->track_button_1, ui->track_button_2, ui->track_button_3,
        ui->track_button_4, ui->track_button_5, ui->track_button_6,
        ui->track_button_7
    };

    QString styleSheet =
        "QPushButton:hover {"
        "    background-color: #D3D3D3;"
        "    color: #000000"
        "}"
        "QPushButton:checked {"
        "    background-color: #4CAF50;"
        "    color: white;"
        "    border: 2px solid #45a049;"
        "}";

    // Применяем стиль к каждой кнопке
    for (QPushButton *btn : buttons) {
        btn->setCheckable(true);
        btn->setStyleSheet(styleSheet);
    }
}

yamitracker::~yamitracker()
{
    delete ui;
}
