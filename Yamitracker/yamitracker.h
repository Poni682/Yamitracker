#ifndef YAMITRACKER_H
#define YAMITRACKER_H

#include <QMainWindow>
#include <QThread>
#include <QPushButton>
#include <QSet>
#include <QTimer>
#include <QHash>

QT_BEGIN_NAMESPACE
namespace Ui { class Yamitracker; }
QT_END_NAMESPACE

class SerialReader : public QThread
{
    Q_OBJECT
public:
    explicit SerialReader(QObject *parent = nullptr) : QThread(parent) {}

protected:
    void run() override;

signals:
    void noteOnReceived(const QString &note, int velocity);
    void noteOffReceived(const QString &note);
    void error(const QString &message);
};

class Yamitracker : public QMainWindow
{
    Q_OBJECT

public:
    Yamitracker(QWidget *parent = nullptr);
    ~Yamitracker();

private slots:
    void onNoteOnReceived(const QString &note, int velocity);
    void onNoteOffReceived(const QString &note);
    void onError(const QString &message);
    void onPlayClicked();
    void onStopClicked();
    void clearStatusMessage();

private:
    Ui::Yamitracker *ui;
    SerialReader *serialReader;
    QHash<QString, QPushButton*> keyButtons;
    QSet<QString> currentlyPressedKeys;

    void initializeKeyButtons();
    void highlightKey(const QString &note);
    void clearKey(const QString &note);
    void clearAllHighlights();
};

#endif // YAMITRACKER_H
