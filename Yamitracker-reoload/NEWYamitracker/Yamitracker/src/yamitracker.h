#ifndef YAMITRACKER_H
#define YAMITRACKER_H

#include <QMainWindow>
#include <QButtonGroup>
#include <QList>

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
    void onTrackSelected(QAbstractButton *button);
    void onDeleteTrackSelected(QAbstractButton *button);
    void onSelectTrackSelect(QAbstractButton *button);
    void setTarckButtonStyles();
    void setDeleteTarckButtonStyles();
    void setSelectTrackButtonStules();
    int getDeleteTrackGroup(QAbstractButton *button){return deleteTrackGroup->id(button);};
    int getSelectTrackGroup(QAbstractButton *button){return selectTrackGroup->id(button);}


private slots:
    void on_select_all_trackbutton_clicked();

private:
    Ui::yamitracker *ui;
    QButtonGroup *trackGroup;
    QButtonGroup *deleteTrackGroup;
    QButtonGroup *selectTrackGroup;
};
#endif // YAMITRACKER_H
