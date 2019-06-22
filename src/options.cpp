

#include "dvda-author-gui.h"
#include <QComboBox>

options::options (dvda* parent)
{
    okButton = new QPushButton (tr ("&Ok") ) ;
    logButton = new QPushButton (tr ("&Log output ") );
    mkisofsButton = new QPushButton (tr ("Browse...") );

    logBox = new QCheckBox (tr ("Log process"), this) ;

    menuBox = new QCheckBox (tr ("Create DVD menu"), this) ;
    menuBox->setChecked(true);

    inputRankBox = new QComboBox (this);
    QLabel* rankLabel = new QLabel("Link to video titleset");
    rankList << "1";
    inputRankBox->addItems(rankList);
    inputRankBox->setEnabled(false);

    mkisofsBox = new QCheckBox (tr ("Create ISO file with mkisofs"), this);
    mkisofsBox->setChecked(true);

    cdrecordBox = new QCheckBox (tr ("Burn disc with cdrecord") );
    cdrecordBox->setChecked (true);

    debugBox = new QCheckBox (tr ("Debugging-level verbosity") );

    soxBox = new QCheckBox (tr ("Enable multiformat input") );
    soxBox->setChecked (true);
    log = false;
    runMkisofs = true;
    debug = false;
    burnDisc = true;
    sox = true;
    menu = true;
    startsector = "";

    QHBoxLayout* menuLayout = new QHBoxLayout;
    menuLayout->addWidget (menuBox);
    menuLayout->addStretch();
    menuLayout->addWidget(rankLabel);
    menuLayout->addWidget (inputRankBox);

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
    mkisofsButton->setEnabled (true);
    dvdwriterLineEdit->setEnabled (true);
    logLayout->addWidget (logBox);
    logLayout->addWidget (debugBox);
    logLayout->addWidget (logButton);
    QVBoxLayout* optionsLayout = new QVBoxLayout;
    QHBoxLayout* okLayout = new QHBoxLayout;

    optionsLayout->addLayout (menuLayout);
    optionsLayout->addSpacing (25);
    optionsLayout->addLayout (mkisofsLayout);
    optionsLayout->addSpacing (25);
    optionsLayout->addLayout (cdrecordLayout);
    optionsLayout->addSpacing (25);
    optionsLayout->addLayout (logLayout);
    optionsLayout->addSpacing (25);
    optionsLayout->addWidget (soxBox);
    optionsLayout->addSpacing (25);

#ifndef WITHOUT_STARTSECTOR
    startsectorLabel = new QLabel (tr ("&Start sector:") );
    optionsLayout->addWidget (startsectorLabel);
    startsectorLineEdit = new QLineEdit;
    startsectorLabel->setBuddy (startsectorLineEdit);
    connect (startsectorLineEdit, SIGNAL (textChanged (const QString&) ), this,
             SLOT (on_startsectorLineEdit_changed (const QString&) ) );
    QHBoxLayout* startsectorLineEditLayout = new QHBoxLayout;
    startsectorLineEditLayout->addWidget (startsectorLineEdit);
    startsectorLineEditLayout->addStretch();
    optionsLayout->addLayout (startsectorLineEditLayout);
#endif

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
    connect (dvdwriterLineEdit, SIGNAL (textChanged (const QString&) ), this, SLOT (on_dvdwriterLineEdit_changed (const QString&) ) );
    connect (menuBox, SIGNAL (clicked() ), this,     SLOT (on_menuBox_checked() ) );
    connect(inputRankBox, SIGNAL(currentIndexChanged(int)), this, SLOT(selectVideoLinkRank(int)));
    setWindowTitle (tr ("Options") );
}

#ifndef WITHOUT_STARTSECTOR
void options::on_startsectorLineEdit_changed (const QString& startsectorValue)
{
    startsector = startsectorValue;
}
#endif


void options::selectVideoLinkRank(int i)
{
     videoTitleRank = i + 1;
}


void options::on_dvdwriterLineEdit_changed (const QString& dvdwriterValue)
{
    dvdwriterPath = dvdwriterValue;
}

void options::on_menuBox_checked()
{
    menu= menuBox->isChecked();
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
    logPath = QDir::toNativeSeparators(logPath);
    log = (!logPath.isEmpty() );
}
#endif
#ifdef QT_FILE_DIALOG
void options::on_mkisofsButton_clicked()
{
    mkisofsPath = QFileDialog::getSaveFileName (this,  tr ("Set mkisofs iso file"), "C:\\Users\\Public\\dvd.iso");
    mkisofsPath = QDir::toNativeSeparators(mkisofsPath);
    runMkisofs = (!mkisofsPath.isEmpty() );
}
#endif

