/********************************************************************************
** Form generated from reading UI file 'yamitracker.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_YAMITRACKER_H
#define UI_YAMITRACKER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
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
    QHBoxLayout *horizontalLayout;
    QLabel *statusLabel;
    QSpacerItem *horizontalSpacer;
    QLabel *deviceLabel;
    QHBoxLayout *horizontalLayout_2;
    QLabel *keysInfoLabel;
    QProgressBar *volumeBar;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *playButton;
    QPushButton *stopButton;
    QPushButton *saveRecordingButton;
    QPushButton *convertButton;
    QPushButton *selectFileButton;
    QPushButton *playFileButton;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label;
    QComboBox *instrumentComboBox;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *refreshDevicesButton;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *loadMidiButton;
    QPushButton *playMidiButton;
    QPushButton *recordFromMidiButton;
    QLabel *label_2;
    QListWidget *midiFilesList;
    QWidget *pianoWidget;
    QVBoxLayout *verticalLayout_2;
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
    QHBoxLayout *blackKeysLayout;
    QSpacerItem *horizontalSpacer_1;
    QPushButton *key_Cs1;
    QPushButton *key_Ds1;
    QSpacerItem *horizontalSpacer_21;
    QPushButton *key_Fs1;
    QPushButton *key_Gs1;
    QPushButton *key_As1;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *key_Cs2;
    QPushButton *key_Ds2;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *key_Fs2;
    QPushButton *key_Gs2;
    QPushButton *key_As2;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *key_Cs3;
    QPushButton *key_Ds3;
    QSpacerItem *horizontalSpacer_6;
    QPushButton *key_Fs3;
    QPushButton *key_Gs3;
    QPushButton *key_As3;
    QSpacerItem *horizontalSpacer_7;
    QPushButton *key_Cs4;
    QPushButton *key_Ds4;
    QSpacerItem *horizontalSpacer_8;
    QPushButton *key_Fs4;
    QPushButton *key_Gs4;
    QPushButton *key_As4;
    QSpacerItem *horizontalSpacer_9;
    QPushButton *key_Cs5;
    QPushButton *key_Ds5;
    QSpacerItem *horizontalSpacer_10;
    QPushButton *key_Fs5;
    QPushButton *key_Gs5;
    QPushButton *key_As5;
    QSpacerItem *horizontalSpacer_11;

    void setupUi(QMainWindow *Yamitracker)
    {
        if (Yamitracker->objectName().isEmpty())
            Yamitracker->setObjectName(QString::fromUtf8("Yamitracker"));
        Yamitracker->resize(1200, 800);
        centralwidget = new QWidget(Yamitracker);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        statusLabel = new QLabel(centralwidget);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));

        horizontalLayout->addWidget(statusLabel);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        deviceLabel = new QLabel(centralwidget);
        deviceLabel->setObjectName(QString::fromUtf8("deviceLabel"));

        horizontalLayout->addWidget(deviceLabel);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        keysInfoLabel = new QLabel(centralwidget);
        keysInfoLabel->setObjectName(QString::fromUtf8("keysInfoLabel"));

        horizontalLayout_2->addWidget(keysInfoLabel);

        volumeBar = new QProgressBar(centralwidget);
        volumeBar->setObjectName(QString::fromUtf8("volumeBar"));
        volumeBar->setValue(0);

        horizontalLayout_2->addWidget(volumeBar);


        verticalLayout->addLayout(horizontalLayout_2);

        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout_3 = new QHBoxLayout(groupBox);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        playButton = new QPushButton(groupBox);
        playButton->setObjectName(QString::fromUtf8("playButton"));

        horizontalLayout_3->addWidget(playButton);

        stopButton = new QPushButton(groupBox);
        stopButton->setObjectName(QString::fromUtf8("stopButton"));

        horizontalLayout_3->addWidget(stopButton);

        saveRecordingButton = new QPushButton(groupBox);
        saveRecordingButton->setObjectName(QString::fromUtf8("saveRecordingButton"));

        horizontalLayout_3->addWidget(saveRecordingButton);

        convertButton = new QPushButton(groupBox);
        convertButton->setObjectName(QString::fromUtf8("convertButton"));

        horizontalLayout_3->addWidget(convertButton);

        selectFileButton = new QPushButton(groupBox);
        selectFileButton->setObjectName(QString::fromUtf8("selectFileButton"));

        horizontalLayout_3->addWidget(selectFileButton);

        playFileButton = new QPushButton(groupBox);
        playFileButton->setObjectName(QString::fromUtf8("playFileButton"));

        horizontalLayout_3->addWidget(playFileButton);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_3 = new QVBoxLayout(groupBox_2);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label = new QLabel(groupBox_2);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_4->addWidget(label);

        instrumentComboBox = new QComboBox(groupBox_2);
        instrumentComboBox->setObjectName(QString::fromUtf8("instrumentComboBox"));

        horizontalLayout_4->addWidget(instrumentComboBox);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);

        refreshDevicesButton = new QPushButton(groupBox_2);
        refreshDevicesButton->setObjectName(QString::fromUtf8("refreshDevicesButton"));

        horizontalLayout_4->addWidget(refreshDevicesButton);


        verticalLayout_3->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        loadMidiButton = new QPushButton(groupBox_2);
        loadMidiButton->setObjectName(QString::fromUtf8("loadMidiButton"));

        horizontalLayout_5->addWidget(loadMidiButton);

        playMidiButton = new QPushButton(groupBox_2);
        playMidiButton->setObjectName(QString::fromUtf8("playMidiButton"));

        horizontalLayout_5->addWidget(playMidiButton);

        recordFromMidiButton = new QPushButton(groupBox_2);
        recordFromMidiButton->setObjectName(QString::fromUtf8("recordFromMidiButton"));

        horizontalLayout_5->addWidget(recordFromMidiButton);


        verticalLayout_3->addLayout(horizontalLayout_5);

        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout_3->addWidget(label_2);

        midiFilesList = new QListWidget(groupBox_2);
        midiFilesList->setObjectName(QString::fromUtf8("midiFilesList"));

        verticalLayout_3->addWidget(midiFilesList);


        verticalLayout->addWidget(groupBox_2);

        pianoWidget = new QWidget(centralwidget);
        pianoWidget->setObjectName(QString::fromUtf8("pianoWidget"));
        verticalLayout_2 = new QVBoxLayout(pianoWidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        whiteKeysLayout = new QHBoxLayout();
        whiteKeysLayout->setObjectName(QString::fromUtf8("whiteKeysLayout"));
        key_C1 = new QPushButton(pianoWidget);
        key_C1->setObjectName(QString::fromUtf8("key_C1"));

        whiteKeysLayout->addWidget(key_C1);

        key_D1 = new QPushButton(pianoWidget);
        key_D1->setObjectName(QString::fromUtf8("key_D1"));

        whiteKeysLayout->addWidget(key_D1);

        key_E1 = new QPushButton(pianoWidget);
        key_E1->setObjectName(QString::fromUtf8("key_E1"));

        whiteKeysLayout->addWidget(key_E1);

        key_F1 = new QPushButton(pianoWidget);
        key_F1->setObjectName(QString::fromUtf8("key_F1"));

        whiteKeysLayout->addWidget(key_F1);

        key_G1 = new QPushButton(pianoWidget);
        key_G1->setObjectName(QString::fromUtf8("key_G1"));

        whiteKeysLayout->addWidget(key_G1);

        key_A1 = new QPushButton(pianoWidget);
        key_A1->setObjectName(QString::fromUtf8("key_A1"));

        whiteKeysLayout->addWidget(key_A1);

        key_B1 = new QPushButton(pianoWidget);
        key_B1->setObjectName(QString::fromUtf8("key_B1"));

        whiteKeysLayout->addWidget(key_B1);

        key_C2 = new QPushButton(pianoWidget);
        key_C2->setObjectName(QString::fromUtf8("key_C2"));

        whiteKeysLayout->addWidget(key_C2);

        key_D2 = new QPushButton(pianoWidget);
        key_D2->setObjectName(QString::fromUtf8("key_D2"));

        whiteKeysLayout->addWidget(key_D2);

        key_E2 = new QPushButton(pianoWidget);
        key_E2->setObjectName(QString::fromUtf8("key_E2"));

        whiteKeysLayout->addWidget(key_E2);

        key_F2 = new QPushButton(pianoWidget);
        key_F2->setObjectName(QString::fromUtf8("key_F2"));

        whiteKeysLayout->addWidget(key_F2);

        key_G2 = new QPushButton(pianoWidget);
        key_G2->setObjectName(QString::fromUtf8("key_G2"));

        whiteKeysLayout->addWidget(key_G2);

        key_A2 = new QPushButton(pianoWidget);
        key_A2->setObjectName(QString::fromUtf8("key_A2"));

        whiteKeysLayout->addWidget(key_A2);

        key_B2 = new QPushButton(pianoWidget);
        key_B2->setObjectName(QString::fromUtf8("key_B2"));

        whiteKeysLayout->addWidget(key_B2);

        key_C3 = new QPushButton(pianoWidget);
        key_C3->setObjectName(QString::fromUtf8("key_C3"));

        whiteKeysLayout->addWidget(key_C3);

        key_D3 = new QPushButton(pianoWidget);
        key_D3->setObjectName(QString::fromUtf8("key_D3"));

        whiteKeysLayout->addWidget(key_D3);

        key_E3 = new QPushButton(pianoWidget);
        key_E3->setObjectName(QString::fromUtf8("key_E3"));

        whiteKeysLayout->addWidget(key_E3);

        key_F3 = new QPushButton(pianoWidget);
        key_F3->setObjectName(QString::fromUtf8("key_F3"));

        whiteKeysLayout->addWidget(key_F3);

        key_G3 = new QPushButton(pianoWidget);
        key_G3->setObjectName(QString::fromUtf8("key_G3"));

        whiteKeysLayout->addWidget(key_G3);

        key_A3 = new QPushButton(pianoWidget);
        key_A3->setObjectName(QString::fromUtf8("key_A3"));

        whiteKeysLayout->addWidget(key_A3);

        key_B3 = new QPushButton(pianoWidget);
        key_B3->setObjectName(QString::fromUtf8("key_B3"));

        whiteKeysLayout->addWidget(key_B3);

        key_C4 = new QPushButton(pianoWidget);
        key_C4->setObjectName(QString::fromUtf8("key_C4"));

        whiteKeysLayout->addWidget(key_C4);

        key_D4 = new QPushButton(pianoWidget);
        key_D4->setObjectName(QString::fromUtf8("key_D4"));

        whiteKeysLayout->addWidget(key_D4);

        key_E4 = new QPushButton(pianoWidget);
        key_E4->setObjectName(QString::fromUtf8("key_E4"));

        whiteKeysLayout->addWidget(key_E4);

        key_F4 = new QPushButton(pianoWidget);
        key_F4->setObjectName(QString::fromUtf8("key_F4"));

        whiteKeysLayout->addWidget(key_F4);

        key_G4 = new QPushButton(pianoWidget);
        key_G4->setObjectName(QString::fromUtf8("key_G4"));

        whiteKeysLayout->addWidget(key_G4);

        key_A4 = new QPushButton(pianoWidget);
        key_A4->setObjectName(QString::fromUtf8("key_A4"));

        whiteKeysLayout->addWidget(key_A4);

        key_B4 = new QPushButton(pianoWidget);
        key_B4->setObjectName(QString::fromUtf8("key_B4"));

        whiteKeysLayout->addWidget(key_B4);

        key_C5 = new QPushButton(pianoWidget);
        key_C5->setObjectName(QString::fromUtf8("key_C5"));

        whiteKeysLayout->addWidget(key_C5);

        key_D5 = new QPushButton(pianoWidget);
        key_D5->setObjectName(QString::fromUtf8("key_D5"));

        whiteKeysLayout->addWidget(key_D5);

        key_E5 = new QPushButton(pianoWidget);
        key_E5->setObjectName(QString::fromUtf8("key_E5"));

        whiteKeysLayout->addWidget(key_E5);

        key_F5 = new QPushButton(pianoWidget);
        key_F5->setObjectName(QString::fromUtf8("key_F5"));

        whiteKeysLayout->addWidget(key_F5);

        key_G5 = new QPushButton(pianoWidget);
        key_G5->setObjectName(QString::fromUtf8("key_G5"));

        whiteKeysLayout->addWidget(key_G5);

        key_A5 = new QPushButton(pianoWidget);
        key_A5->setObjectName(QString::fromUtf8("key_A5"));

        whiteKeysLayout->addWidget(key_A5);

        key_B5 = new QPushButton(pianoWidget);
        key_B5->setObjectName(QString::fromUtf8("key_B5"));

        whiteKeysLayout->addWidget(key_B5);

        key_C6 = new QPushButton(pianoWidget);
        key_C6->setObjectName(QString::fromUtf8("key_C6"));

        whiteKeysLayout->addWidget(key_C6);


        verticalLayout_2->addLayout(whiteKeysLayout);

        blackKeysLayout = new QHBoxLayout();
        blackKeysLayout->setObjectName(QString::fromUtf8("blackKeysLayout"));
        horizontalSpacer_1 = new QSpacerItem(15, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        blackKeysLayout->addItem(horizontalSpacer_1);

        key_Cs1 = new QPushButton(pianoWidget);
        key_Cs1->setObjectName(QString::fromUtf8("key_Cs1"));

        blackKeysLayout->addWidget(key_Cs1);

        key_Ds1 = new QPushButton(pianoWidget);
        key_Ds1->setObjectName(QString::fromUtf8("key_Ds1"));

        blackKeysLayout->addWidget(key_Ds1);

        horizontalSpacer_21 = new QSpacerItem(15, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        blackKeysLayout->addItem(horizontalSpacer_21);

        key_Fs1 = new QPushButton(pianoWidget);
        key_Fs1->setObjectName(QString::fromUtf8("key_Fs1"));

        blackKeysLayout->addWidget(key_Fs1);

        key_Gs1 = new QPushButton(pianoWidget);
        key_Gs1->setObjectName(QString::fromUtf8("key_Gs1"));

        blackKeysLayout->addWidget(key_Gs1);

        key_As1 = new QPushButton(pianoWidget);
        key_As1->setObjectName(QString::fromUtf8("key_As1"));

        blackKeysLayout->addWidget(key_As1);

        horizontalSpacer_3 = new QSpacerItem(15, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        blackKeysLayout->addItem(horizontalSpacer_3);

        key_Cs2 = new QPushButton(pianoWidget);
        key_Cs2->setObjectName(QString::fromUtf8("key_Cs2"));

        blackKeysLayout->addWidget(key_Cs2);

        key_Ds2 = new QPushButton(pianoWidget);
        key_Ds2->setObjectName(QString::fromUtf8("key_Ds2"));

        blackKeysLayout->addWidget(key_Ds2);

        horizontalSpacer_4 = new QSpacerItem(15, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        blackKeysLayout->addItem(horizontalSpacer_4);

        key_Fs2 = new QPushButton(pianoWidget);
        key_Fs2->setObjectName(QString::fromUtf8("key_Fs2"));

        blackKeysLayout->addWidget(key_Fs2);

        key_Gs2 = new QPushButton(pianoWidget);
        key_Gs2->setObjectName(QString::fromUtf8("key_Gs2"));

        blackKeysLayout->addWidget(key_Gs2);

        key_As2 = new QPushButton(pianoWidget);
        key_As2->setObjectName(QString::fromUtf8("key_As2"));

        blackKeysLayout->addWidget(key_As2);

        horizontalSpacer_5 = new QSpacerItem(15, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        blackKeysLayout->addItem(horizontalSpacer_5);

        key_Cs3 = new QPushButton(pianoWidget);
        key_Cs3->setObjectName(QString::fromUtf8("key_Cs3"));

        blackKeysLayout->addWidget(key_Cs3);

        key_Ds3 = new QPushButton(pianoWidget);
        key_Ds3->setObjectName(QString::fromUtf8("key_Ds3"));

        blackKeysLayout->addWidget(key_Ds3);

        horizontalSpacer_6 = new QSpacerItem(15, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        blackKeysLayout->addItem(horizontalSpacer_6);

        key_Fs3 = new QPushButton(pianoWidget);
        key_Fs3->setObjectName(QString::fromUtf8("key_Fs3"));

        blackKeysLayout->addWidget(key_Fs3);

        key_Gs3 = new QPushButton(pianoWidget);
        key_Gs3->setObjectName(QString::fromUtf8("key_Gs3"));

        blackKeysLayout->addWidget(key_Gs3);

        key_As3 = new QPushButton(pianoWidget);
        key_As3->setObjectName(QString::fromUtf8("key_As3"));

        blackKeysLayout->addWidget(key_As3);

        horizontalSpacer_7 = new QSpacerItem(15, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        blackKeysLayout->addItem(horizontalSpacer_7);

        key_Cs4 = new QPushButton(pianoWidget);
        key_Cs4->setObjectName(QString::fromUtf8("key_Cs4"));

        blackKeysLayout->addWidget(key_Cs4);

        key_Ds4 = new QPushButton(pianoWidget);
        key_Ds4->setObjectName(QString::fromUtf8("key_Ds4"));

        blackKeysLayout->addWidget(key_Ds4);

        horizontalSpacer_8 = new QSpacerItem(15, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        blackKeysLayout->addItem(horizontalSpacer_8);

        key_Fs4 = new QPushButton(pianoWidget);
        key_Fs4->setObjectName(QString::fromUtf8("key_Fs4"));

        blackKeysLayout->addWidget(key_Fs4);

        key_Gs4 = new QPushButton(pianoWidget);
        key_Gs4->setObjectName(QString::fromUtf8("key_Gs4"));

        blackKeysLayout->addWidget(key_Gs4);

        key_As4 = new QPushButton(pianoWidget);
        key_As4->setObjectName(QString::fromUtf8("key_As4"));

        blackKeysLayout->addWidget(key_As4);

        horizontalSpacer_9 = new QSpacerItem(15, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        blackKeysLayout->addItem(horizontalSpacer_9);

        key_Cs5 = new QPushButton(pianoWidget);
        key_Cs5->setObjectName(QString::fromUtf8("key_Cs5"));

        blackKeysLayout->addWidget(key_Cs5);

        key_Ds5 = new QPushButton(pianoWidget);
        key_Ds5->setObjectName(QString::fromUtf8("key_Ds5"));

        blackKeysLayout->addWidget(key_Ds5);

        horizontalSpacer_10 = new QSpacerItem(15, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        blackKeysLayout->addItem(horizontalSpacer_10);

        key_Fs5 = new QPushButton(pianoWidget);
        key_Fs5->setObjectName(QString::fromUtf8("key_Fs5"));

        blackKeysLayout->addWidget(key_Fs5);

        key_Gs5 = new QPushButton(pianoWidget);
        key_Gs5->setObjectName(QString::fromUtf8("key_Gs5"));

        blackKeysLayout->addWidget(key_Gs5);

        key_As5 = new QPushButton(pianoWidget);
        key_As5->setObjectName(QString::fromUtf8("key_As5"));

        blackKeysLayout->addWidget(key_As5);

        horizontalSpacer_11 = new QSpacerItem(15, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        blackKeysLayout->addItem(horizontalSpacer_11);


        verticalLayout_2->addLayout(blackKeysLayout);


        verticalLayout->addWidget(pianoWidget);

        Yamitracker->setCentralWidget(centralwidget);

        retranslateUi(Yamitracker);

        QMetaObject::connectSlotsByName(Yamitracker);
    } // setupUi

    void retranslateUi(QMainWindow *Yamitracker)
    {
        Yamitracker->setWindowTitle(QCoreApplication::translate("Yamitracker", "Yamaha PSR-E333 Monitor", nullptr));
        statusLabel->setText(QCoreApplication::translate("Yamitracker", "\320\241\321\202\320\260\321\202\321\203\321\201: \320\223\320\276\321\202\320\276\320\262", nullptr));
        deviceLabel->setText(QCoreApplication::translate("Yamitracker", "\320\243\321\201\321\202\321\200\320\276\320\271\321\201\321\202\320\262\320\276: \320\235\320\265 \320\277\320\276\320\264\320\272\320\273\321\216\321\207\320\265\320\275\320\276", nullptr));
        keysInfoLabel->setText(QCoreApplication::translate("Yamitracker", "\320\235\320\260\320\266\320\260\321\202\320\276 \320\272\320\273\320\260\320\262\320\270\321\210: 0", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Yamitracker", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265 \320\267\320\260\320\277\320\270\321\201\321\214\321\216", nullptr));
        playButton->setText(QCoreApplication::translate("Yamitracker", "\320\227\320\260\320\277\320\270\321\201\321\214 \321\201 \321\201\320\270\320\275\321\202\320\265\320\267\320\260\321\202\320\276\321\200\320\260", nullptr));
        stopButton->setText(QCoreApplication::translate("Yamitracker", "\320\241\321\202\320\276\320\277", nullptr));
        saveRecordingButton->setText(QCoreApplication::translate("Yamitracker", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 \320\267\320\260\320\277\320\270\321\201\321\214", nullptr));
        convertButton->setText(QCoreApplication::translate("Yamitracker", "\320\232\320\276\320\275\320\262\320\265\321\200\321\202\320\270\321\200\320\276\320\262\320\260\321\202\321\214 \320\262 BRR", nullptr));
        selectFileButton->setText(QCoreApplication::translate("Yamitracker", "\320\222\321\213\320\261\321\200\320\260\321\202\321\214 \321\204\320\260\320\271\320\273 \320\264\320\273\321\217 \320\272\320\276\320\275\320\262\320\265\321\200\321\202\320\260\321\206\320\270\320\270", nullptr));
        playFileButton->setText(QCoreApplication::translate("Yamitracker", "\320\222\320\276\321\201\320\277\321\200\320\276\320\270\320\267\320\262\320\265\321\201\321\202\320\270 WAV", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("Yamitracker", "\320\240\320\260\320\261\320\276\321\202\320\260 \321\201 MIDI \321\204\320\260\320\271\320\273\320\260\320\274\320\270", nullptr));
        label->setText(QCoreApplication::translate("Yamitracker", "\320\230\320\275\321\201\321\202\321\200\321\203\320\274\320\265\320\275\321\202:", nullptr));
        refreshDevicesButton->setText(QCoreApplication::translate("Yamitracker", "\320\236\320\261\320\275\320\276\320\262\320\270\321\202\321\214 \321\203\321\201\321\202\321\200\320\276\320\271\321\201\321\202\320\262\320\260", nullptr));
        loadMidiButton->setText(QCoreApplication::translate("Yamitracker", "\320\227\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214 MIDI \321\204\320\260\320\271\320\273", nullptr));
        playMidiButton->setText(QCoreApplication::translate("Yamitracker", "\320\222\320\276\321\201\320\277\321\200\320\276\320\270\320\267\320\262\320\265\321\201\321\202\320\270 MIDI", nullptr));
        recordFromMidiButton->setText(QCoreApplication::translate("Yamitracker", "\320\227\320\260\320\277\320\270\321\201\320\260\321\202\321\214 \320\270\320\267 MIDI", nullptr));
        label_2->setText(QCoreApplication::translate("Yamitracker", "\320\224\320\276\321\201\321\202\321\203\320\277\320\275\321\213\320\265 MIDI \321\204\320\260\320\271\320\273\321\213:", nullptr));
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
    } // retranslateUi

};

namespace Ui {
    class Yamitracker: public Ui_Yamitracker {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_YAMITRACKER_H
