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
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSlider>
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
    QPushButton *pauseButton;
    QPushButton *stopButton;
    QPushButton *saveRecordingButton;
    QPushButton *convertButton;
    QPushButton *saveMusicButton;
    QGroupBox *groupBox_4;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *playTrackButton;
    QPushButton *playAllButton;
    QPushButton *stopPlaybackButton;
    QSpacerItem *horizontalSpacer_13;
    QPushButton *loadWavButton;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_3;
    QComboBox *trackComboBox;
    QPushButton *addTrackButton;
    QPushButton *removeTrackButton;
    QPushButton *clearTrackButton;
    QPushButton *copyTrackButton;
    QPushButton *mergeTrackButton;
    QSpacerItem *horizontalSpacer_12;
    QLabel *trackInfoLabel;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_4;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label;
    QComboBox *instrumentComboBox;
    QSpacerItem *horizontalSpacer_2;
    QLabel *speedLabel;
    QSlider *playbackSpeedSlider;
    QLabel *volumeLabel;
    QSlider *bassVolumeSlider;
    QPushButton *refreshDevicesButton;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *loadMidiButton;
    QPushButton *playMidiButton;
    QPushButton *recordFromMidiButton;
    QPushButton *selectFileButton;
    QPushButton *playFileButton;
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
        Yamitracker->resize(1400, 1000);
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

        pauseButton = new QPushButton(groupBox);
        pauseButton->setObjectName(QString::fromUtf8("pauseButton"));
        pauseButton->setEnabled(false);

        horizontalLayout_3->addWidget(pauseButton);

        stopButton = new QPushButton(groupBox);
        stopButton->setObjectName(QString::fromUtf8("stopButton"));

        horizontalLayout_3->addWidget(stopButton);

        saveRecordingButton = new QPushButton(groupBox);
        saveRecordingButton->setObjectName(QString::fromUtf8("saveRecordingButton"));

        horizontalLayout_3->addWidget(saveRecordingButton);

        convertButton = new QPushButton(groupBox);
        convertButton->setObjectName(QString::fromUtf8("convertButton"));

        horizontalLayout_3->addWidget(convertButton);

        saveMusicButton = new QPushButton(groupBox);
        saveMusicButton->setObjectName(QString::fromUtf8("saveMusicButton"));

        horizontalLayout_3->addWidget(saveMusicButton);


        verticalLayout->addWidget(groupBox);

        groupBox_4 = new QGroupBox(centralwidget);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        horizontalLayout_7 = new QHBoxLayout(groupBox_4);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        playTrackButton = new QPushButton(groupBox_4);
        playTrackButton->setObjectName(QString::fromUtf8("playTrackButton"));

        horizontalLayout_7->addWidget(playTrackButton);

        playAllButton = new QPushButton(groupBox_4);
        playAllButton->setObjectName(QString::fromUtf8("playAllButton"));

        horizontalLayout_7->addWidget(playAllButton);

        stopPlaybackButton = new QPushButton(groupBox_4);
        stopPlaybackButton->setObjectName(QString::fromUtf8("stopPlaybackButton"));

        horizontalLayout_7->addWidget(stopPlaybackButton);

        horizontalSpacer_13 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_13);

        loadWavButton = new QPushButton(groupBox_4);
        loadWavButton->setObjectName(QString::fromUtf8("loadWavButton"));

        horizontalLayout_7->addWidget(loadWavButton);


        verticalLayout->addWidget(groupBox_4);

        groupBox_3 = new QGroupBox(centralwidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        horizontalLayout_6 = new QHBoxLayout(groupBox_3);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_6->addWidget(label_3);

        trackComboBox = new QComboBox(groupBox_3);
        trackComboBox->setObjectName(QString::fromUtf8("trackComboBox"));

        horizontalLayout_6->addWidget(trackComboBox);

        addTrackButton = new QPushButton(groupBox_3);
        addTrackButton->setObjectName(QString::fromUtf8("addTrackButton"));

        horizontalLayout_6->addWidget(addTrackButton);

        removeTrackButton = new QPushButton(groupBox_3);
        removeTrackButton->setObjectName(QString::fromUtf8("removeTrackButton"));

        horizontalLayout_6->addWidget(removeTrackButton);

        clearTrackButton = new QPushButton(groupBox_3);
        clearTrackButton->setObjectName(QString::fromUtf8("clearTrackButton"));

        horizontalLayout_6->addWidget(clearTrackButton);

        copyTrackButton = new QPushButton(groupBox_3);
        copyTrackButton->setObjectName(QString::fromUtf8("copyTrackButton"));

        horizontalLayout_6->addWidget(copyTrackButton);

        mergeTrackButton = new QPushButton(groupBox_3);
        mergeTrackButton->setObjectName(QString::fromUtf8("mergeTrackButton"));

        horizontalLayout_6->addWidget(mergeTrackButton);

        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_12);

        trackInfoLabel = new QLabel(groupBox_3);
        trackInfoLabel->setObjectName(QString::fromUtf8("trackInfoLabel"));

        horizontalLayout_6->addWidget(trackInfoLabel);


        verticalLayout->addWidget(groupBox_3);

        scrollArea = new QScrollArea(centralwidget);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 1380, 300));
        verticalLayout_4 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        label_4 = new QLabel(scrollAreaWidgetContents);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        verticalLayout_4->addWidget(label_4);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);

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

        speedLabel = new QLabel(groupBox_2);
        speedLabel->setObjectName(QString::fromUtf8("speedLabel"));

        horizontalLayout_4->addWidget(speedLabel);

        playbackSpeedSlider = new QSlider(groupBox_2);
        playbackSpeedSlider->setObjectName(QString::fromUtf8("playbackSpeedSlider"));
        playbackSpeedSlider->setOrientation(Qt::Horizontal);
        playbackSpeedSlider->setMinimum(50);
        playbackSpeedSlider->setMaximum(200);
        playbackSpeedSlider->setValue(100);

        horizontalLayout_4->addWidget(playbackSpeedSlider);

        volumeLabel = new QLabel(groupBox_2);
        volumeLabel->setObjectName(QString::fromUtf8("volumeLabel"));

        horizontalLayout_4->addWidget(volumeLabel);

        bassVolumeSlider = new QSlider(groupBox_2);
        bassVolumeSlider->setObjectName(QString::fromUtf8("bassVolumeSlider"));
        bassVolumeSlider->setOrientation(Qt::Horizontal);
        bassVolumeSlider->setMinimum(100);
        bassVolumeSlider->setMaximum(400);
        bassVolumeSlider->setValue(200);

        horizontalLayout_4->addWidget(bassVolumeSlider);

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

        selectFileButton = new QPushButton(groupBox_2);
        selectFileButton->setObjectName(QString::fromUtf8("selectFileButton"));

        horizontalLayout_5->addWidget(selectFileButton);

        playFileButton = new QPushButton(groupBox_2);
        playFileButton->setObjectName(QString::fromUtf8("playFileButton"));

        horizontalLayout_5->addWidget(playFileButton);


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
        Yamitracker->setWindowTitle(QCoreApplication::translate("Yamitracker", "Yamaha PSR-E333 Monitor - Multitrack Recording", nullptr));
        statusLabel->setText(QCoreApplication::translate("Yamitracker", "\320\241\321\202\320\260\321\202\321\203\321\201: \320\223\320\276\321\202\320\276\320\262", nullptr));
        deviceLabel->setText(QCoreApplication::translate("Yamitracker", "\320\243\321\201\321\202\321\200\320\276\320\271\321\201\321\202\320\262\320\276: \320\235\320\265 \320\277\320\276\320\264\320\272\320\273\321\216\321\207\320\265\320\275\320\276", nullptr));
        keysInfoLabel->setText(QCoreApplication::translate("Yamitracker", "\320\235\320\260\320\266\320\260\321\202\320\276 \320\272\320\273\320\260\320\262\320\270\321\210: 0", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Yamitracker", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265 \320\267\320\260\320\277\320\270\321\201\321\214\321\216", nullptr));
        playButton->setText(QCoreApplication::translate("Yamitracker", "\342\226\266 \320\227\320\260\320\277\320\270\321\201\321\214 \321\201 \321\201\320\270\320\275\321\202\320\265\320\267\320\260\321\202\320\276\321\200\320\260", nullptr));
        pauseButton->setText(QCoreApplication::translate("Yamitracker", "\342\217\270 \320\237\320\260\321\203\320\267\320\260", nullptr));
        stopButton->setText(QCoreApplication::translate("Yamitracker", "\342\217\271 \320\241\321\202\320\276\320\277", nullptr));
        saveRecordingButton->setText(QCoreApplication::translate("Yamitracker", "\360\237\222\276 \320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 \320\267\320\260\320\277\320\270\321\201\321\214", nullptr));
        convertButton->setText(QCoreApplication::translate("Yamitracker", "\360\237\224\204 \320\232\320\276\320\275\320\262\320\265\321\200\321\202\320\270\321\200\320\276\320\262\320\260\321\202\321\214 \320\262 BRR", nullptr));
        saveMusicButton->setText(QCoreApplication::translate("Yamitracker", "\360\237\216\265 \320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 \320\274\321\203\320\267\321\213\320\272\321\203 (Pently)", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("Yamitracker", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265 \320\262\320\276\321\201\320\277\321\200\320\276\320\270\320\267\320\262\320\265\320\264\320\265\320\275\320\270\320\265\320\274", nullptr));
        playTrackButton->setText(QCoreApplication::translate("Yamitracker", "\342\226\266 \320\242\320\265\320\272\321\203\321\211\320\260\321\217 \320\264\320\276\321\200\320\276\320\266\320\272\320\260", nullptr));
        playAllButton->setText(QCoreApplication::translate("Yamitracker", "\342\226\266\342\226\266 \320\222\321\201\320\265 \320\264\320\276\321\200\320\276\320\266\320\272\320\270", nullptr));
        stopPlaybackButton->setText(QCoreApplication::translate("Yamitracker", "\342\217\271 \320\241\321\202\320\276\320\277 \320\262\320\276\321\201\320\277\321\200\320\276\320\270\320\267\320\262\320\265\320\264\320\265\320\275\320\270\320\265", nullptr));
        loadWavButton->setText(QCoreApplication::translate("Yamitracker", "\360\237\223\202 \320\227\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214 WAV \320\262 \320\264\320\276\321\200\320\276\320\266\320\272\321\203", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("Yamitracker", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265 \320\264\320\276\321\200\320\276\320\266\320\272\320\260\320\274\320\270", nullptr));
        label_3->setText(QCoreApplication::translate("Yamitracker", "\320\242\320\265\320\272\321\203\321\211\320\260\321\217 \320\264\320\276\321\200\320\276\320\266\320\272\320\260:", nullptr));
        addTrackButton->setText(QCoreApplication::translate("Yamitracker", "\342\236\225 \320\235\320\276\320\262\320\260\321\217 \320\264\320\276\321\200\320\276\320\266\320\272\320\260", nullptr));
        removeTrackButton->setText(QCoreApplication::translate("Yamitracker", "\360\237\227\221 \320\243\320\264\320\260\320\273\320\270\321\202\321\214 \320\264\320\276\321\200\320\276\320\266\320\272\321\203", nullptr));
        clearTrackButton->setText(QCoreApplication::translate("Yamitracker", "\360\237\227\221 \320\236\321\207\320\270\321\201\321\202\320\270\321\202\321\214 \320\264\320\276\321\200\320\276\320\266\320\272\321\203", nullptr));
        copyTrackButton->setText(QCoreApplication::translate("Yamitracker", "\360\237\223\213 \320\232\320\276\320\277\320\270\321\200\320\276\320\262\320\260\321\202\321\214 \320\264\320\276\321\200\320\276\320\266\320\272\321\203", nullptr));
        mergeTrackButton->setText(QCoreApplication::translate("Yamitracker", "\360\237\224\200 \320\236\320\261\321\212\320\265\320\264\320\270\320\275\320\270\321\202\321\214 \320\264\320\276\321\200\320\276\320\266\320\272\320\270", nullptr));
        trackInfoLabel->setText(QCoreApplication::translate("Yamitracker", "\320\224\320\276\321\200\320\276\320\266\320\272\320\260 1: 0 \320\275\320\276\321\202", nullptr));
        label_4->setText(QCoreApplication::translate("Yamitracker", "\320\237\320\260\320\275\320\265\320\273\321\214 \321\203\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\321\217 \320\264\320\276\321\200\320\276\320\266\320\272\320\260\320\274\320\270:", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("Yamitracker", "\320\240\320\260\320\261\320\276\321\202\320\260 \321\201 MIDI \321\204\320\260\320\271\320\273\320\260\320\274\320\270", nullptr));
        label->setText(QCoreApplication::translate("Yamitracker", "\320\230\320\275\321\201\321\202\321\200\321\203\320\274\320\265\320\275\321\202:", nullptr));
        speedLabel->setText(QCoreApplication::translate("Yamitracker", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\321\214: 100%", nullptr));
        volumeLabel->setText(QCoreApplication::translate("Yamitracker", "\320\221\320\260\321\201: 200%", nullptr));
        refreshDevicesButton->setText(QCoreApplication::translate("Yamitracker", "\360\237\224\203 \320\236\320\261\320\275\320\276\320\262\320\270\321\202\321\214 \321\203\321\201\321\202\321\200\320\276\320\271\321\201\321\202\320\262\320\260", nullptr));
        loadMidiButton->setText(QCoreApplication::translate("Yamitracker", "\360\237\223\202 \320\227\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214 MIDI \321\204\320\260\320\271\320\273", nullptr));
        playMidiButton->setText(QCoreApplication::translate("Yamitracker", "\342\226\266 \320\222\320\276\321\201\320\277\321\200\320\276\320\270\320\267\320\262\320\265\321\201\321\202\320\270 MIDI", nullptr));
        recordFromMidiButton->setText(QCoreApplication::translate("Yamitracker", "\342\227\217 \320\227\320\260\320\277\320\270\321\201\320\260\321\202\321\214 \320\270\320\267 MIDI", nullptr));
        selectFileButton->setText(QCoreApplication::translate("Yamitracker", "\360\237\223\201 \320\222\321\213\320\261\321\200\320\260\321\202\321\214 \321\204\320\260\320\271\320\273 \320\264\320\273\321\217 \320\272\320\276\320\275\320\262\320\265\321\200\321\202\320\260\321\206\320\270\320\270", nullptr));
        playFileButton->setText(QCoreApplication::translate("Yamitracker", "\342\226\266 \320\222\320\276\321\201\320\277\321\200\320\276\320\270\320\267\320\262\320\265\321\201\321\202\320\270 WAV", nullptr));
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
