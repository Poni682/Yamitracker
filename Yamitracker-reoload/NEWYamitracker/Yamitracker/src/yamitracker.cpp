#include "yamitracker.h"
#include "ui_yamitracker.h"
#include "styleButton.h"
#include "buttonGroupY.h"
#include "styleListButton.h"

yamitracker::yamitracker(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::yamitracker)
{
    ui->setupUi(this);
//Группы кнопок
    trackGroup = createTrackGroup(ui, this);
    deleteTrackGroup = createDeleteTrackGroup(ui, this);
    selectTrackGroup = createSelectTrackGroup(ui, this);
    playStopGroup = createPlayStopGroup(ui, this);
    whiteNoteGroup = createWhiteNoteGroup(ui, this);
    blackNoteGroup = createBlackNoteGroup(ui, this);

//Стили
    setTarckButtonStyles();
    setSelectTrackButtonStules();
    setDeleteTarckButtonStyles();
    ui->START_button->setStyleSheet(checkPlayStop);
    ui->STOP_button->setStyleSheet(checkPlayStop);
    styleButton::setStyleFromQString(getButtonsFromGroup(blackNoteGroup), clickBlack);


//Дебаг подключение
    connect(trackGroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked),
            this, &yamitracker::onTrackSelected);
    connect(deleteTrackGroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked),
            this, &yamitracker::onDeleteTrackSelected);
    connect(whiteNoteGroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked),
            this, &yamitracker::clickNoteButtonDebag);
    connect(blackNoteGroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked),
            this, &yamitracker::clickNoteButtonDebag);


//Старт стоп клик
    connect(playStopGroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked),
            this, &yamitracker::playStopClicked);

}


QList<QPushButton*> yamitracker::getButtonsFromGroup(QButtonGroup* group) {
    QList<QPushButton*> result;
    for (auto* btn : group->buttons()) {
        if (auto* pushBtn = qobject_cast<QPushButton*>(btn)) {
            result << pushBtn;
        }
    }
    return result;
}

bool yamitracker::isRecording(){
    return ui->START_button->isChecked();
}
bool yamitracker::isPause(){
    return ui->START_button->isChecked();
}

void yamitracker::playStopClicked(QAbstractButton *button){
    if(button == ui->START_button){
        if(isRecording()){
            qDebug() << "Play Strat";
            button->setText("■ Стоп");
        }
        else{
            qDebug() << "End recording";
            button->setText("▶ Старт");
            ui->STOP_button->setChecked(false);
            ui->STOP_button->setText("⏸ Пауза");
        }
    }

    else{ //ui->STOP_button
        if(isRecording()){
            if(button->isChecked()){
                qDebug() << "Pause recording";
                button->setText("▶ Продолжить");
            }
            else{
                qDebug() << "Continuation recording";
                button->setText("⏸ Пауза");
            }
        }
        else{
            button->setChecked(false);
        }
    }

}

void yamitracker::clickNoteButtonDebag(QAbstractButton* button){
    qDebug() << "Нажата клавиша: " << button->text();
}

void yamitracker::onDeleteTrackSelected(QAbstractButton *button){
    qDebug() << "Выбрана дорожка: " << button->text() << " " << getDeleteTrackGroup(button);
}

void yamitracker::onTrackSelected(QAbstractButton *button)
{
    qDebug() << "Выбрана дорожка: " << button->text();
}

void yamitracker::setDeleteTarckButtonStyles(){
    styleButton::buttonClick(getButtonsFromGroup(deleteTrackGroup));
}

void yamitracker::setSelectTrackButtonStules(){
    styleButton::buttonCheck(getButtonsFromGroup(selectTrackGroup));
}

void yamitracker::setTarckButtonStyles()
{
    styleButton::buttonCheck(getButtonsFromGroup(trackGroup));
}

yamitracker::~yamitracker()
{
    delete ui;
}

void yamitracker::on_select_all_trackbutton_clicked()
{
    QList<QPushButton*> buttons = getButtonsFromGroup(selectTrackGroup);


    bool allChecked = true;
    for (QPushButton* btn : buttons){
        if(!btn->isChecked()){
            allChecked = false;
            break;
        }
    }

    for (QPushButton* btn : buttons){
        btn->setChecked(!allChecked);
    }

}

