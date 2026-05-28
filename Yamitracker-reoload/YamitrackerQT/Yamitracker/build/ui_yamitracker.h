/********************************************************************************
** Form generated from reading UI file 'yamitracker.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_YAMITRACKER_H
#define UI_YAMITRACKER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Yamitracker
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *topLayout;
    QPushButton *playButton;
    QPushButton *stopButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;
    QLabel *keysInfoLabel;
    QVBoxLayout *pianoContainerLayout;
    QHBoxLayout *blackKeysLayout;
    QPushButton *key_Cs1;
    QPushButton *key_Ds1;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *key_Fs1;
    QPushButton *key_Gs1;
    QPushButton *key_As1;
    QPushButton *key_Cs2;
    QPushButton *key_Ds2;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *key_Fs2;
    QPushButton *key_Gs2;
    QPushButton *key_As2;
    QPushButton *key_Cs3;
    QPushButton *key_Ds3;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *key_Fs3;
    QPushButton *key_Gs3;
    QPushButton *key_As3;
    QPushButton *key_Cs4;
    QPushButton *key_Ds4;
    QSpacerItem *horizontalSpacer_6;
    QPushButton *key_Fs4;
    QPushButton *key_Gs4;
    QPushButton *key_As4;
    QPushButton *key_Cs5;
    QPushButton *key_Ds5;
    QSpacerItem *horizontalSpacer_7;
    QPushButton *key_Fs5;
    QPushButton *key_Gs5;
    QPushButton *key_As5;
    QHBoxLayout *whiteKeysLayout;
    QPushButton *key_C1;
    QPushButton *key_D1;
    QPushButton *key_E1;
    QPushButton *key_F1;
    QPushButton *key_G1;
    QPushButton *key_A1;
    QPushButton *key_B1;
    QPushButton *key_C2;
    QPushButton *key_D2;
    QPushButton *key_E2;
    QPushButton *key_F2;
    QPushButton *key_G2;
    QPushButton *key_A2;
    QPushButton *key_B2;
    QPushButton *key_C3;
    QPushButton *key_D3;
    QPushButton *key_E3;
    QPushButton *key_F3;
    QPushButton *key_G3;
    QPushButton *key_A3;
    QPushButton *key_B3;
    QPushButton *key_C4;
    QPushButton *key_D4;
    QPushButton *key_E4;
    QPushButton *key_F4;
    QPushButton *key_G4;
    QPushButton *key_A4;
    QPushButton *key_B4;
    QPushButton *key_C5;
    QPushButton *key_D5;
    QPushButton *key_E5;
    QPushButton *key_F5;
    QPushButton *key_G5;
    QPushButton *key_A5;
    QPushButton *key_B5;
    QPushButton *key_C6;
    QHBoxLayout *bottomLayout;
    QLabel *volumeLabel;
    QProgressBar *volumeBar;
    QSpacerItem *horizontalSpacer_2;
    QLabel *statusLabel;

    void setupUi(QMainWindow *Yamitracker)
    {
        if (Yamitracker->objectName().isEmpty())
            Yamitracker->setObjectName("Yamitracker");
        Yamitracker->resize(1492, 500);
        centralwidget = new QWidget(Yamitracker);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        topLayout = new QHBoxLayout();
        topLayout->setObjectName("topLayout");
        playButton = new QPushButton(centralwidget);
        playButton->setObjectName("playButton");

        topLayout->addWidget(playButton);

        stopButton = new QPushButton(centralwidget);
        stopButton->setObjectName("stopButton");

        topLayout->addWidget(stopButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        topLayout->addItem(horizontalSpacer);

        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName("pushButton");

        topLayout->addWidget(pushButton);

        keysInfoLabel = new QLabel(centralwidget);
        keysInfoLabel->setObjectName("keysInfoLabel");

        topLayout->addWidget(keysInfoLabel);


        verticalLayout->addLayout(topLayout);

        pianoContainerLayout = new QVBoxLayout();
        pianoContainerLayout->setObjectName("pianoContainerLayout");
        blackKeysLayout = new QHBoxLayout();
        blackKeysLayout->setObjectName("blackKeysLayout");
        key_Cs1 = new QPushButton(centralwidget);
        key_Cs1->setObjectName("key_Cs1");
        key_Cs1->setMinimumSize(QSize(25, 80));
        key_Cs1->setMaximumSize(QSize(25, 80));

        blackKeysLayout->addWidget(key_Cs1);

        key_Ds1 = new QPushButton(centralwidget);
        key_Ds1->setObjectName("key_Ds1");
        key_Ds1->setMinimumSize(QSize(25, 80));
        key_Ds1->setMaximumSize(QSize(25, 80));

        blackKeysLayout->addWidget(key_Ds1);

        horizontalSpacer_3 = new QSpacerItem(35, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        blackKeysLayout->addItem(horizontalSpacer_3);

        key_Fs1 = new QPushButton(centralwidget);
        key_Fs1->setObjectName("key_Fs1");
        key_Fs1->setMinimumSize(QSize(25, 80));
        key_Fs1->setMaximumSize(QSize(25, 80));

        blackKeysLayout->addWidget(key_Fs1);

        key_Gs1 = new QPushButton(centralwidget);
        key_Gs1->setObjectName("key_Gs1");
        key_Gs1->setMinimumSize(QSize(25, 80));
        key_Gs1->setMaximumSize(QSize(25, 80));

        blackKeysLayout->addWidget(key_Gs1);

        key_As1 = new QPushButton(centralwidget);
        key_As1->setObjectName("key_As1");
        key_As1->setMinimumSize(QSize(25, 80));
        key_As1->setMaximumSize(QSize(25, 80));

        blackKeysLayout->addWidget(key_As1);

        key_Cs2 = new QPushButton(centralwidget);
        key_Cs2->setObjectName("key_Cs2");
        key_Cs2->setMinimumSize(QSize(25, 80));
        key_Cs2->setMaximumSize(QSize(25, 80));

        blackKeysLayout->addWidget(key_Cs2);

        key_Ds2 = new QPushButton(centralwidget);
        key_Ds2->setObjectName("key_Ds2");
        key_Ds2->setMinimumSize(QSize(25, 80));
        key_Ds2->setMaximumSize(QSize(25, 80));

        blackKeysLayout->addWidget(key_Ds2);

        horizontalSpacer_4 = new QSpacerItem(35, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        blackKeysLayout->addItem(horizontalSpacer_4);

        key_Fs2 = new QPushButton(centralwidget);
        key_Fs2->setObjectName("key_Fs2");
        key_Fs2->setMinimumSize(QSize(25, 80));
        key_Fs2->setMaximumSize(QSize(25, 80));

        blackKeysLayout->addWidget(key_Fs2);

        key_Gs2 = new QPushButton(centralwidget);
        key_Gs2->setObjectName("key_Gs2");
        key_Gs2->setMinimumSize(QSize(25, 80));
        key_Gs2->setMaximumSize(QSize(25, 80));

        blackKeysLayout->addWidget(key_Gs2);

        key_As2 = new QPushButton(centralwidget);
        key_As2->setObjectName("key_As2");
        key_As2->setMinimumSize(QSize(25, 80));
        key_As2->setMaximumSize(QSize(25, 80));

        blackKeysLayout->addWidget(key_As2);

        key_Cs3 = new QPushButton(centralwidget);
        key_Cs3->setObjectName("key_Cs3");
        key_Cs3->setMinimumSize(QSize(25, 80));
        key_Cs3->setMaximumSize(QSize(25, 80));

        blackKeysLayout->addWidget(key_Cs3);

        key_Ds3 = new QPushButton(centralwidget);
        key_Ds3->setObjectName("key_Ds3");
        key_Ds3->setMinimumSize(QSize(25, 80));
        key_Ds3->setMaximumSize(QSize(25, 80));

        blackKeysLayout->addWidget(key_Ds3);

        horizontalSpacer_5 = new QSpacerItem(35, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        blackKeysLayout->addItem(horizontalSpacer_5);

        key_Fs3 = new QPushButton(centralwidget);
        key_Fs3->setObjectName("key_Fs3");
        key_Fs3->setMinimumSize(QSize(25, 80));
        key_Fs3->setMaximumSize(QSize(25, 80));

        blackKeysLayout->addWidget(key_Fs3);

        key_Gs3 = new QPushButton(centralwidget);
        key_Gs3->setObjectName("key_Gs3");
        key_Gs3->setMinimumSize(QSize(25, 80));
        key_Gs3->setMaximumSize(QSize(25, 80));

        blackKeysLayout->addWidget(key_Gs3);

        key_As3 = new QPushButton(centralwidget);
        key_As3->setObjectName("key_As3");
        key_As3->setMinimumSize(QSize(25, 80));
        key_As3->setMaximumSize(QSize(25, 80));

        blackKeysLayout->addWidget(key_As3);

        key_Cs4 = new QPushButton(centralwidget);
        key_Cs4->setObjectName("key_Cs4");
        key_Cs4->setMinimumSize(QSize(25, 80));
        key_Cs4->setMaximumSize(QSize(25, 80));

        blackKeysLayout->addWidget(key_Cs4);

        key_Ds4 = new QPushButton(centralwidget);
        key_Ds4->setObjectName("key_Ds4");
        key_Ds4->setMinimumSize(QSize(25, 80));
        key_Ds4->setMaximumSize(QSize(25, 80));

        blackKeysLayout->addWidget(key_Ds4);

        horizontalSpacer_6 = new QSpacerItem(35, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        blackKeysLayout->addItem(horizontalSpacer_6);

        key_Fs4 = new QPushButton(centralwidget);
        key_Fs4->setObjectName("key_Fs4");
        key_Fs4->setMinimumSize(QSize(25, 80));
        key_Fs4->setMaximumSize(QSize(25, 80));

        blackKeysLayout->addWidget(key_Fs4);

        key_Gs4 = new QPushButton(centralwidget);
        key_Gs4->setObjectName("key_Gs4");
        key_Gs4->setMinimumSize(QSize(25, 80));
        key_Gs4->setMaximumSize(QSize(25, 80));

        blackKeysLayout->addWidget(key_Gs4);

        key_As4 = new QPushButton(centralwidget);
        key_As4->setObjectName("key_As4");
        key_As4->setMinimumSize(QSize(25, 80));
        key_As4->setMaximumSize(QSize(25, 80));

        blackKeysLayout->addWidget(key_As4);

        key_Cs5 = new QPushButton(centralwidget);
        key_Cs5->setObjectName("key_Cs5");
        key_Cs5->setMinimumSize(QSize(25, 80));
        key_Cs5->setMaximumSize(QSize(25, 80));

        blackKeysLayout->addWidget(key_Cs5);

        key_Ds5 = new QPushButton(centralwidget);
        key_Ds5->setObjectName("key_Ds5");
        key_Ds5->setMinimumSize(QSize(25, 80));
        key_Ds5->setMaximumSize(QSize(25, 80));

        blackKeysLayout->addWidget(key_Ds5);

        horizontalSpacer_7 = new QSpacerItem(35, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        blackKeysLayout->addItem(horizontalSpacer_7);

        key_Fs5 = new QPushButton(centralwidget);
        key_Fs5->setObjectName("key_Fs5");
        key_Fs5->setMinimumSize(QSize(25, 80));
        key_Fs5->setMaximumSize(QSize(25, 80));

        blackKeysLayout->addWidget(key_Fs5);

        key_Gs5 = new QPushButton(centralwidget);
        key_Gs5->setObjectName("key_Gs5");
        key_Gs5->setMinimumSize(QSize(25, 80));
        key_Gs5->setMaximumSize(QSize(25, 80));

        blackKeysLayout->addWidget(key_Gs5);

        key_As5 = new QPushButton(centralwidget);
        key_As5->setObjectName("key_As5");
        key_As5->setMinimumSize(QSize(25, 80));
        key_As5->setMaximumSize(QSize(25, 80));

        blackKeysLayout->addWidget(key_As5);


        pianoContainerLayout->addLayout(blackKeysLayout);

        whiteKeysLayout = new QHBoxLayout();
        whiteKeysLayout->setObjectName("whiteKeysLayout");
        key_C1 = new QPushButton(centralwidget);
        key_C1->setObjectName("key_C1");
        key_C1->setMinimumSize(QSize(35, 120));
        key_C1->setMaximumSize(QSize(35, 120));

        whiteKeysLayout->addWidget(key_C1);

        key_D1 = new QPushButton(centralwidget);
        key_D1->setObjectName("key_D1");
        key_D1->setMinimumSize(QSize(35, 120));
        key_D1->setMaximumSize(QSize(35, 120));

        whiteKeysLayout->addWidget(key_D1);

        key_E1 = new QPushButton(centralwidget);
        key_E1->setObjectName("key_E1");
        key_E1->setMinimumSize(QSize(35, 120));
        key_E1->setMaximumSize(QSize(35, 120));

        whiteKeysLayout->addWidget(key_E1);

        key_F1 = new QPushButton(centralwidget);
        key_F1->setObjectName("key_F1");
        key_F1->setMinimumSize(QSize(35, 120));
        key_F1->setMaximumSize(QSize(35, 120));

        whiteKeysLayout->addWidget(key_F1);

        key_G1 = new QPushButton(centralwidget);
        key_G1->setObjectName("key_G1");
        key_G1->setMinimumSize(QSize(35, 120));
        key_G1->setMaximumSize(QSize(35, 120));

        whiteKeysLayout->addWidget(key_G1);

        key_A1 = new QPushButton(centralwidget);
        key_A1->setObjectName("key_A1");
        key_A1->setMinimumSize(QSize(35, 120));
        key_A1->setMaximumSize(QSize(35, 120));

        whiteKeysLayout->addWidget(key_A1);

        key_B1 = new QPushButton(centralwidget);
        key_B1->setObjectName("key_B1");
        key_B1->setMinimumSize(QSize(35, 120));
        key_B1->setMaximumSize(QSize(35, 120));

        whiteKeysLayout->addWidget(key_B1);

        key_C2 = new QPushButton(centralwidget);
        key_C2->setObjectName("key_C2");
        key_C2->setMinimumSize(QSize(35, 120));
        key_C2->setMaximumSize(QSize(35, 120));

        whiteKeysLayout->addWidget(key_C2);

        key_D2 = new QPushButton(centralwidget);
        key_D2->setObjectName("key_D2");
        key_D2->setMinimumSize(QSize(35, 120));
        key_D2->setMaximumSize(QSize(35, 120));

        whiteKeysLayout->addWidget(key_D2);

        key_E2 = new QPushButton(centralwidget);
        key_E2->setObjectName("key_E2");
        key_E2->setMinimumSize(QSize(35, 120));
        key_E2->setMaximumSize(QSize(35, 120));

        whiteKeysLayout->addWidget(key_E2);

        key_F2 = new QPushButton(centralwidget);
        key_F2->setObjectName("key_F2");
        key_F2->setMinimumSize(QSize(35, 120));
        key_F2->setMaximumSize(QSize(35, 120));

        whiteKeysLayout->addWidget(key_F2);

        key_G2 = new QPushButton(centralwidget);
        key_G2->setObjectName("key_G2");
        key_G2->setMinimumSize(QSize(35, 120));
        key_G2->setMaximumSize(QSize(35, 120));

        whiteKeysLayout->addWidget(key_G2);

        key_A2 = new QPushButton(centralwidget);
        key_A2->setObjectName("key_A2");
        key_A2->setMinimumSize(QSize(35, 120));
        key_A2->setMaximumSize(QSize(35, 120));

        whiteKeysLayout->addWidget(key_A2);

        key_B2 = new QPushButton(centralwidget);
        key_B2->setObjectName("key_B2");
        key_B2->setMinimumSize(QSize(35, 120));
        key_B2->setMaximumSize(QSize(35, 120));

        whiteKeysLayout->addWidget(key_B2);

        key_C3 = new QPushButton(centralwidget);
        key_C3->setObjectName("key_C3");
        key_C3->setMinimumSize(QSize(35, 120));
        key_C3->setMaximumSize(QSize(35, 120));

        whiteKeysLayout->addWidget(key_C3);

        key_D3 = new QPushButton(centralwidget);
        key_D3->setObjectName("key_D3");
        key_D3->setMinimumSize(QSize(35, 120));
        key_D3->setMaximumSize(QSize(35, 120));

        whiteKeysLayout->addWidget(key_D3);

        key_E3 = new QPushButton(centralwidget);
        key_E3->setObjectName("key_E3");
        key_E3->setMinimumSize(QSize(35, 120));
        key_E3->setMaximumSize(QSize(35, 120));

        whiteKeysLayout->addWidget(key_E3);

        key_F3 = new QPushButton(centralwidget);
        key_F3->setObjectName("key_F3");
        key_F3->setMinimumSize(QSize(35, 120));
        key_F3->setMaximumSize(QSize(35, 120));

        whiteKeysLayout->addWidget(key_F3);

        key_G3 = new QPushButton(centralwidget);
        key_G3->setObjectName("key_G3");
        key_G3->setMinimumSize(QSize(35, 120));
        key_G3->setMaximumSize(QSize(35, 120));

        whiteKeysLayout->addWidget(key_G3);

        key_A3 = new QPushButton(centralwidget);
        key_A3->setObjectName("key_A3");
        key_A3->setMinimumSize(QSize(35, 120));
        key_A3->setMaximumSize(QSize(35, 120));

        whiteKeysLayout->addWidget(key_A3);

        key_B3 = new QPushButton(centralwidget);
        key_B3->setObjectName("key_B3");
        key_B3->setMinimumSize(QSize(35, 120));
        key_B3->setMaximumSize(QSize(35, 120));

        whiteKeysLayout->addWidget(key_B3);

        key_C4 = new QPushButton(centralwidget);
        key_C4->setObjectName("key_C4");
        key_C4->setMinimumSize(QSize(35, 120));
        key_C4->setMaximumSize(QSize(35, 120));

        whiteKeysLayout->addWidget(key_C4);

        key_D4 = new QPushButton(centralwidget);
        key_D4->setObjectName("key_D4");
        key_D4->setMinimumSize(QSize(35, 120));
        key_D4->setMaximumSize(QSize(35, 120));

        whiteKeysLayout->addWidget(key_D4);

        key_E4 = new QPushButton(centralwidget);
        key_E4->setObjectName("key_E4");
        key_E4->setMinimumSize(QSize(35, 120));
        key_E4->setMaximumSize(QSize(35, 120));

        whiteKeysLayout->addWidget(key_E4);

        key_F4 = new QPushButton(centralwidget);
        key_F4->setObjectName("key_F4");
        key_F4->setMinimumSize(QSize(35, 120));
        key_F4->setMaximumSize(QSize(35, 120));

        whiteKeysLayout->addWidget(key_F4);

        key_G4 = new QPushButton(centralwidget);
        key_G4->setObjectName("key_G4");
        key_G4->setMinimumSize(QSize(35, 120));
        key_G4->setMaximumSize(QSize(35, 120));

        whiteKeysLayout->addWidget(key_G4);

        key_A4 = new QPushButton(centralwidget);
        key_A4->setObjectName("key_A4");
        key_A4->setMinimumSize(QSize(35, 120));
        key_A4->setMaximumSize(QSize(35, 120));

        whiteKeysLayout->addWidget(key_A4);

        key_B4 = new QPushButton(centralwidget);
        key_B4->setObjectName("key_B4");
        key_B4->setMinimumSize(QSize(35, 120));
        key_B4->setMaximumSize(QSize(35, 120));

        whiteKeysLayout->addWidget(key_B4);

        key_C5 = new QPushButton(centralwidget);
        key_C5->setObjectName("key_C5");
        key_C5->setMinimumSize(QSize(35, 120));
        key_C5->setMaximumSize(QSize(35, 120));

        whiteKeysLayout->addWidget(key_C5);

        key_D5 = new QPushButton(centralwidget);
        key_D5->setObjectName("key_D5");
        key_D5->setMinimumSize(QSize(35, 120));
        key_D5->setMaximumSize(QSize(35, 120));

        whiteKeysLayout->addWidget(key_D5);

        key_E5 = new QPushButton(centralwidget);
        key_E5->setObjectName("key_E5");
        key_E5->setMinimumSize(QSize(35, 120));
        key_E5->setMaximumSize(QSize(35, 120));

        whiteKeysLayout->addWidget(key_E5);

        key_F5 = new QPushButton(centralwidget);
        key_F5->setObjectName("key_F5");
        key_F5->setMinimumSize(QSize(35, 120));
        key_F5->setMaximumSize(QSize(35, 120));

        whiteKeysLayout->addWidget(key_F5);

        key_G5 = new QPushButton(centralwidget);
        key_G5->setObjectName("key_G5");
        key_G5->setMinimumSize(QSize(35, 120));
        key_G5->setMaximumSize(QSize(35, 120));

        whiteKeysLayout->addWidget(key_G5);

        key_A5 = new QPushButton(centralwidget);
        key_A5->setObjectName("key_A5");
        key_A5->setMinimumSize(QSize(35, 120));
        key_A5->setMaximumSize(QSize(35, 120));

        whiteKeysLayout->addWidget(key_A5);

        key_B5 = new QPushButton(centralwidget);
        key_B5->setObjectName("key_B5");
        key_B5->setMinimumSize(QSize(35, 120));
        key_B5->setMaximumSize(QSize(35, 120));

        whiteKeysLayout->addWidget(key_B5);

        key_C6 = new QPushButton(centralwidget);
        key_C6->setObjectName("key_C6");
        key_C6->setMinimumSize(QSize(35, 120));
        key_C6->setMaximumSize(QSize(35, 120));

        whiteKeysLayout->addWidget(key_C6);


        pianoContainerLayout->addLayout(whiteKeysLayout);


        verticalLayout->addLayout(pianoContainerLayout);

        bottomLayout = new QHBoxLayout();
        bottomLayout->setObjectName("bottomLayout");
        volumeLabel = new QLabel(centralwidget);
        volumeLabel->setObjectName("volumeLabel");

        bottomLayout->addWidget(volumeLabel);

        volumeBar = new QProgressBar(centralwidget);
        volumeBar->setObjectName("volumeBar");
        volumeBar->setValue(0);

        bottomLayout->addWidget(volumeBar);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        bottomLayout->addItem(horizontalSpacer_2);

        statusLabel = new QLabel(centralwidget);
        statusLabel->setObjectName("statusLabel");

        bottomLayout->addWidget(statusLabel);


        verticalLayout->addLayout(bottomLayout);

        Yamitracker->setCentralWidget(centralwidget);

        retranslateUi(Yamitracker);

        QMetaObject::connectSlotsByName(Yamitracker);
    } // setupUi

    void retranslateUi(QMainWindow *Yamitracker)
    {
        Yamitracker->setWindowTitle(QCoreApplication::translate("Yamitracker", "Yamaha PSR-E333 Monitor", nullptr));
        playButton->setText(QCoreApplication::translate("Yamitracker", "Play", nullptr));
        stopButton->setText(QCoreApplication::translate("Yamitracker", "Stop", nullptr));
        pushButton->setText(QCoreApplication::translate("Yamitracker", "NoteLog", nullptr));
        keysInfoLabel->setText(QCoreApplication::translate("Yamitracker", "Keys: 36 white + 25 black = 61 total", nullptr));
        key_Cs1->setText(QCoreApplication::translate("Yamitracker", "C#1", nullptr));
        key_Ds1->setText(QCoreApplication::translate("Yamitracker", "D#1", nullptr));
        key_Fs1->setText(QCoreApplication::translate("Yamitracker", "F#1", nullptr));
        key_Gs1->setText(QCoreApplication::translate("Yamitracker", "G#1", nullptr));
        key_As1->setText(QCoreApplication::translate("Yamitracker", "A#1", nullptr));
        key_Cs2->setText(QCoreApplication::translate("Yamitracker", "C#2", nullptr));
        key_Ds2->setText(QCoreApplication::translate("Yamitracker", "D#2", nullptr));
        key_Fs2->setText(QCoreApplication::translate("Yamitracker", "F#2", nullptr));
        key_Gs2->setText(QCoreApplication::translate("Yamitracker", "G#2", nullptr));
        key_As2->setText(QCoreApplication::translate("Yamitracker", "A#2", nullptr));
        key_Cs3->setText(QCoreApplication::translate("Yamitracker", "C#3", nullptr));
        key_Ds3->setText(QCoreApplication::translate("Yamitracker", "D#3", nullptr));
        key_Fs3->setText(QCoreApplication::translate("Yamitracker", "F#3", nullptr));
        key_Gs3->setText(QCoreApplication::translate("Yamitracker", "G#3", nullptr));
        key_As3->setText(QCoreApplication::translate("Yamitracker", "A#3", nullptr));
        key_Cs4->setText(QCoreApplication::translate("Yamitracker", "C#4", nullptr));
        key_Ds4->setText(QCoreApplication::translate("Yamitracker", "D#4", nullptr));
        key_Fs4->setText(QCoreApplication::translate("Yamitracker", "F#4", nullptr));
        key_Gs4->setText(QCoreApplication::translate("Yamitracker", "G#4", nullptr));
        key_As4->setText(QCoreApplication::translate("Yamitracker", "A#4", nullptr));
        key_Cs5->setText(QCoreApplication::translate("Yamitracker", "C#5", nullptr));
        key_Ds5->setText(QCoreApplication::translate("Yamitracker", "D#5", nullptr));
        key_Fs5->setText(QCoreApplication::translate("Yamitracker", "F#5", nullptr));
        key_Gs5->setText(QCoreApplication::translate("Yamitracker", "G#5", nullptr));
        key_As5->setText(QCoreApplication::translate("Yamitracker", "A#5", nullptr));
        key_C1->setText(QCoreApplication::translate("Yamitracker", "C1", nullptr));
        key_D1->setText(QCoreApplication::translate("Yamitracker", "D1", nullptr));
        key_E1->setText(QCoreApplication::translate("Yamitracker", "E1", nullptr));
        key_F1->setText(QCoreApplication::translate("Yamitracker", "F1", nullptr));
        key_G1->setText(QCoreApplication::translate("Yamitracker", "G1", nullptr));
        key_A1->setText(QCoreApplication::translate("Yamitracker", "A1", nullptr));
        key_B1->setText(QCoreApplication::translate("Yamitracker", "B1", nullptr));
        key_C2->setText(QCoreApplication::translate("Yamitracker", "C2", nullptr));
        key_D2->setText(QCoreApplication::translate("Yamitracker", "D2", nullptr));
        key_E2->setText(QCoreApplication::translate("Yamitracker", "E2", nullptr));
        key_F2->setText(QCoreApplication::translate("Yamitracker", "F2", nullptr));
        key_G2->setText(QCoreApplication::translate("Yamitracker", "G2", nullptr));
        key_A2->setText(QCoreApplication::translate("Yamitracker", "A2", nullptr));
        key_B2->setText(QCoreApplication::translate("Yamitracker", "B2", nullptr));
        key_C3->setText(QCoreApplication::translate("Yamitracker", "C3", nullptr));
        key_D3->setText(QCoreApplication::translate("Yamitracker", "D3", nullptr));
        key_E3->setText(QCoreApplication::translate("Yamitracker", "E3", nullptr));
        key_F3->setText(QCoreApplication::translate("Yamitracker", "F3", nullptr));
        key_G3->setText(QCoreApplication::translate("Yamitracker", "G3", nullptr));
        key_A3->setText(QCoreApplication::translate("Yamitracker", "A3", nullptr));
        key_B3->setText(QCoreApplication::translate("Yamitracker", "B3", nullptr));
        key_C4->setText(QCoreApplication::translate("Yamitracker", "C4", nullptr));
        key_D4->setText(QCoreApplication::translate("Yamitracker", "D4", nullptr));
        key_E4->setText(QCoreApplication::translate("Yamitracker", "E4", nullptr));
        key_F4->setText(QCoreApplication::translate("Yamitracker", "F4", nullptr));
        key_G4->setText(QCoreApplication::translate("Yamitracker", "G4", nullptr));
        key_A4->setText(QCoreApplication::translate("Yamitracker", "A4", nullptr));
        key_B4->setText(QCoreApplication::translate("Yamitracker", "B4", nullptr));
        key_C5->setText(QCoreApplication::translate("Yamitracker", "C5", nullptr));
        key_D5->setText(QCoreApplication::translate("Yamitracker", "D5", nullptr));
        key_E5->setText(QCoreApplication::translate("Yamitracker", "E5", nullptr));
        key_F5->setText(QCoreApplication::translate("Yamitracker", "F5", nullptr));
        key_G5->setText(QCoreApplication::translate("Yamitracker", "G5", nullptr));
        key_A5->setText(QCoreApplication::translate("Yamitracker", "A5", nullptr));
        key_B5->setText(QCoreApplication::translate("Yamitracker", "B5", nullptr));
        key_C6->setText(QCoreApplication::translate("Yamitracker", "C6", nullptr));
        volumeLabel->setText(QCoreApplication::translate("Yamitracker", "Volume:", nullptr));
        statusLabel->setText(QCoreApplication::translate("Yamitracker", "Status: Ready", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Yamitracker: public Ui_Yamitracker {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_YAMITRACKER_H
