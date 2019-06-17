
#include <QtGui>
#include <QFile>
#include <QAbstractButton>
#include <QCheckBox>

#include "dvda-author-gui.h"


options::options (dvda* parent)
{
    okButton = new QPushButton (tr ("&Ok") ) ;
    logButton = new QPushButton (tr ("&Log output ") );
    mkisofsButton = new QPushButton (tr ("Browse...") );
    logBox = new QCheckBox (tr ("Log process"), this) ;
    mkisofsBox = new QCheckBox (tr ("Create ISO file with mkisofs"), this);
    cdrecordBox = new QCheckBox (tr ("Burn disc with cdrecord") );
    cdrecordBox->setChecked (false);
    debugBox = new QCheckBox (tr ("Debugging-level verbosity") );
    videozoneBox = new QCheckBox (tr ("Create VIDEO_TS directory") );
    videozoneBox->setChecked (parent->tab2Widget != NULL);
    audiozoneBox = new QCheckBox (tr ("Create AUDIO_TS directory") );
    audiozoneBox->setChecked (parent->tabWidget != NULL);
    soxBox = new QCheckBox (tr ("Enable multiformat input") );
    soxBox->setChecked (true);
    startsectorLabel = new QLabel (tr ("&Start sector:") );
    startsectorLineEdit = new QLineEdit;
    startsectorLabel->setBuddy (startsectorLineEdit);
    log = runMkisofs = debug = burnDisc = 0;
    sox = videozone = audiozone = 1;
    startsector = "";
    hasAudiozone = (parent->tabWidget != NULL);
    hasVideozone = (parent->tab2Widget != NULL);
    QHBoxLayout* mkisofsLayout = new QHBoxLayout;
    mkisofsLayout->addWidget (mkisofsBox);
    mkisofsLayout->addStretch();
    mkisofsLayout->addWidget (mkisofsButton);
    mkisofsPath = "dvd.iso";
    QLabel* dvdwriterLabel = new QLabel (tr ("DVD writer:") );
    dvdwriterLineEdit = new QLineEdit;
    dvdwriterLabel->setBuddy (dvdwriterLineEdit);
    QHBoxLayout* cdrecordLayout = new QHBoxLayout;
    cdrecordLayout->addWidget (cdrecordBox);
    cdrecordLayout->addSpacing (50);
    cdrecordLayout->addWidget (dvdwriterLabel);
    cdrecordLayout->addWidget (dvdwriterLineEdit);
    QHBoxLayout* logLayout = new QHBoxLayout;
    logButton->setEnabled (false);
    mkisofsButton->setEnabled (false);
    dvdwriterLineEdit->setEnabled (false);
    logLayout->addWidget (logBox);
    logLayout->addWidget (debugBox);
    logLayout->addWidget (logButton);
    QVBoxLayout* optionsLayout = new QVBoxLayout;
    QHBoxLayout* okLayout = new QHBoxLayout;
    optionsLayout->addSpacing (25);
    optionsLayout->addLayout (mkisofsLayout);
    optionsLayout->addSpacing (25);
    optionsLayout->addLayout (cdrecordLayout);
    optionsLayout->addSpacing (25);
    optionsLayout->addLayout (logLayout);
    optionsLayout->addSpacing (25);
    optionsLayout->addWidget (soxBox);
    optionsLayout->addSpacing (25);
    hasAudiozoneBox = new QCheckBox ("Author DVD-Audio zone");
    hasAudiozoneBox->setChecked (hasAudiozone);
    hasVideozoneBox = new QCheckBox ("Author DVD-Video zone with audio content");
    hasVideozoneBox->setChecked (hasVideozone);
    optionsLayout->addWidget (audiozoneBox);
    optionsLayout->addWidget (hasAudiozoneBox);
    optionsLayout->addWidget (videozoneBox);
    optionsLayout->addWidget (hasVideozoneBox);
    optionsLayout->addSpacing (25);
    optionsLayout->addWidget (startsectorLabel);
    QHBoxLayout* startsectorLineEditLayout = new QHBoxLayout;
    startsectorLineEditLayout->addWidget (startsectorLineEdit);
    startsectorLineEditLayout->addStretch();
    optionsLayout->addLayout (startsectorLineEditLayout);
    okLayout->addStretch();
    okLayout->addWidget (okButton);
    optionsLayout->addLayout (okLayout);
    setLayout (optionsLayout);
    connect (logBox, SIGNAL (clicked() ), this,     SLOT (on_logBox_checked() ) );
    connect (mkisofsBox, SIGNAL (clicked() ), this, SLOT (on_mkisofsBox_checked() ) );
    connect (okButton, SIGNAL (clicked() ), this,   SLOT (accept() ) );
    connect (logButton, SIGNAL (clicked() ), this,  SLOT (on_logButton_clicked() ) );
    connect (mkisofsButton, SIGNAL (clicked() ), this, SLOT (on_mkisofsButton_clicked() ) );
    connect (debugBox, SIGNAL (clicked() ), this, SLOT (on_debugBox_checked() ) );
    connect (soxBox, SIGNAL (clicked() ), this, SLOT (on_soxBox_checked() ) );
    connect (cdrecordBox, SIGNAL (clicked() ), this, SLOT (on_cdrecordBox_checked() ) );
    connect (startsectorLineEdit, SIGNAL (textChanged (const QString&) ), this,
             SLOT (on_startsectorLineEdit_changed (const QString&) ) );
    connect (dvdwriterLineEdit, SIGNAL (textChanged (const QString&) ), this,
             SLOT (on_dvdwriterLineEdit_changed (const QString&) ) );
    connect (audiozoneBox, SIGNAL (clicked() ), this, SLOT (on_audiozoneBox_checked() ) );
    connect (videozoneBox, SIGNAL (clicked() ), this, SLOT (on_videozoneBox_checked() ) );
    connect (hasVideozoneBox, SIGNAL (clicked() ), this, SLOT (on_hasVideozoneBox_checked() ) );
    connect (hasAudiozoneBox, SIGNAL (clicked() ), this, SLOT (on_hasAudiozoneBox_checked() ) );
    connect (hasVideozoneBox, SIGNAL (clicked (bool) ), parent, SLOT (on_hasVideozoneBox_toggled (bool) ) );
    connect (hasAudiozoneBox, SIGNAL (clicked (bool) ), parent, SLOT (on_hasAudiozoneBox_toggled (bool) ) );
    connect (this, SIGNAL (defaultClick (bool) ), parent, SLOT (on_hasAudiozoneBox_toggled (bool) ) );
    setWindowTitle (tr ("Options") );
}


void options::checkZones()
{
    if (hasVideozone + hasAudiozone == 0)
        {
            QMessageBox::warning (this, "Error",
                                  "You should author at least one DVD zone (either AUDIO_TS or VIDEO_TS)\nSetting AUDIO_TS as default.");
            hasAudiozone = 1;
            hasAudiozoneBox->setChecked (true);
            emit (defaultClick (true) );
        }
}

void options::on_hasVideozoneBox_checked()
{
    hasVideozone = hasVideozoneBox->isChecked();
    checkZones();
}

void options::on_hasAudiozoneBox_checked()
{
    hasAudiozone = hasAudiozoneBox->isChecked();
    checkZones();
}


void options::on_startsectorLineEdit_changed (const QString& startsectorValue)
{
    startsector = startsectorValue;
}

void options::on_dvdwriterLineEdit_changed (const QString& dvdwriterValue)
{
    dvdwriterPath = dvdwriterValue;
}

void options::on_logBox_checked()
{
    logButton->setEnabled (logBox->isChecked() );
}


void options::on_soxBox_checked()
{
    sox = soxBox->isChecked();
}

void options::on_debugBox_checked()
{
    debug = debugBox->isChecked();
}

void options::on_videozoneBox_checked()
{
    videozone = videozoneBox->isChecked();
}

void options::on_audiozoneBox_checked()
{
    audiozone = audiozoneBox->isChecked();
}



void options::on_mkisofsBox_checked()
{
    mkisofsButton->setEnabled (mkisofsBox->isChecked() );
    runMkisofs = mkisofsBox->isChecked();
}

void options::on_cdrecordBox_checked()
{
    burnDisc = cdrecordBox->isChecked();
    dvdwriterLineEdit->setEnabled (cdrecordBox->isChecked() );
    mkisofsBox->setChecked(burnDisc);
    runMkisofs = burnDisc;
}

#ifdef QT_FILE_DIALOG
void options::on_logButton_clicked()
{
    logPath = QFileDialog::getSaveFileName (this,  tr ("Set log file"), "");
    logPath = QDir::convertSeparators (logPath);
    log = (!logPath.isEmpty() );
}
#endif
#ifdef QT_FILE_DIALOG
void options::on_mkisofsButton_clicked()
{
    mkisofsPath = QFileDialog::getSaveFileName (this,  tr ("Set mkisofs iso file"), "C:\\Users\\Public\\dvd.iso");
    mkisofsPath = QDir::convertSeparators (mkisofsPath);
    runMkisofs = (!mkisofsPath.isEmpty() );
}
#endif

