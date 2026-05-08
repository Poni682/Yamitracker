#ifndef BUTTONGROUPY_H
#define BUTTONGROUPY_H

#include <QButtonGroup>
#include <QObject>
#include "ui_yamitracker.h"

QButtonGroup* createTrackGroup(Ui::yamitracker *ui, QObject *parent);
QButtonGroup* createDeleteTrackGroup(Ui::yamitracker *ui, QObject *parent);
QButtonGroup* createSelectTrackGroup(Ui::yamitracker *ui, QObject *parent);
// QButtonGroup* createWhiteNoteGroup(Ui::yamitracker *ui, QObject *parent);
QButtonGroup* createPlayStopGroup(Ui::yamitracker *ui, QObject *parent);
QButtonGroup* createWhiteNoteGroup(Ui::yamitracker *ui, QObject *parent);
QButtonGroup* createBlackNoteGroup(Ui::yamitracker *ui, QObject *parent);

#endif // BUTTONGROUPY_H
