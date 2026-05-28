#include "buttonGroupY.h"
#include "ui_yamitracker.h"

QButtonGroup* createTrackGroup(Ui::yamitracker *ui, QObject *parent) {
    QButtonGroup *trackGroup = new QButtonGroup(parent);
    trackGroup->addButton(ui->track_button_1, 1);
    trackGroup->addButton(ui->track_button_2, 2);
    trackGroup->addButton(ui->track_button_3, 3);
    trackGroup->addButton(ui->track_button_4, 4);
    trackGroup->addButton(ui->track_button_5, 5);
    trackGroup->addButton(ui->track_button_6, 6);
    trackGroup->addButton(ui->track_button_7, 7);
    trackGroup->setExclusive(true);
    return trackGroup;
}


QButtonGroup* createDeleteTrackGroup(Ui::yamitracker *ui, QObject *parent){
    QButtonGroup *deleteTrackGroup = new QButtonGroup(parent);
    deleteTrackGroup->addButton(ui->delete_track_button_1, 1);
    deleteTrackGroup->addButton(ui->delete_track_button_2, 2);
    deleteTrackGroup->addButton(ui->delete_track_button_3, 3);
    deleteTrackGroup->addButton(ui->delete_track_button_4, 4);
    deleteTrackGroup->addButton(ui->delete_track_button_5, 5);
    deleteTrackGroup->addButton(ui->delete_track_button_6, 6);
    deleteTrackGroup->addButton(ui->delete_track_button_7, 7);
    return deleteTrackGroup;
}


QButtonGroup* createSelectTrackGroup(Ui::yamitracker *ui, QObject *parent){
    QButtonGroup *selectTrackGroup = new QButtonGroup(parent);
    selectTrackGroup->addButton(ui->select_button_1, 1);
    selectTrackGroup->addButton(ui->select_button_2, 2);
    selectTrackGroup->addButton(ui->select_button_3, 3);
    selectTrackGroup->addButton(ui->select_button_4, 4);
    selectTrackGroup->addButton(ui->select_button_5, 5);
    selectTrackGroup->addButton(ui->select_button_6, 6);
    selectTrackGroup->addButton(ui->select_button_7, 7);
    selectTrackGroup->setExclusive(false);
    return selectTrackGroup;
}
//key_[A-G][1-5] 36
QButtonGroup* createWhiteNoteGroup(Ui::yamitracker *ui, QObject *parent){
    QButtonGroup *whiteNoteGroup = new QButtonGroup(parent);

    for (int i = 1; i < 6; i++){
        for (int j = 0; j < 7; j++){
            QString nameButton = QString("key_%1%2").arg(QChar('A' + j)).arg(i);
            QPushButton *button = ui->centralwidget->findChild<QPushButton*>(nameButton);
            if (button){
                whiteNoteGroup->addButton(button);
            }
        }

    }
    whiteNoteGroup->addButton(ui->key_C6);
    return whiteNoteGroup;
}

QButtonGroup* createBlackNoteGroup(Ui::yamitracker *ui, QObject *parent){
    QButtonGroup *blackNoteGroup = new QButtonGroup(parent);

    for (int i = 1; i < 6; i++){
        for (int j = 0; j < 7; j++){
            QString nameButton = QString("key_%1s%2").arg(QChar('A' + j)).arg(i);
            QPushButton *button = ui->centralwidget->findChild<QPushButton*>(nameButton);
            if (button){
                blackNoteGroup->addButton(button);
            }
        }
    }
    return blackNoteGroup;
}




QButtonGroup* createPlayStopGroup(Ui::yamitracker *ui, QObject *parent){
    QButtonGroup *playStopGroup = new QButtonGroup(parent);
    playStopGroup->addButton(ui->STOP_button, 1);
    playStopGroup->addButton(ui->START_button, 2);
    playStopGroup->setExclusive(false);
    return playStopGroup;
}
