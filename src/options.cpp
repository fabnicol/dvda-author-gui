

#include "dvda-author-gui.h"


options::options (dvda* parent)
{
    okButton = new QPushButton (tr ("&Ok") ) ;
    logButton = new QPushButton (tr ("&Log output ") );
    mkisofsButton = new QPushButton (tr ("Browse...") );

    logBox = new QCheckBox (tr ("Log process"), this) ;
    mkisofsBox = new QCheckBox (tr ("Create ISO file with mkisofs"), this);
    mkisofsBox->setChecked(true);

    cdrecordBox = new QCheckBox (tr ("Burn disc with cdrecord") );
    cdrecordBox->setChecked (true);

    debugBox = new QCheckBox (tr ("Debugging-level verbosity") );

    soxBox = new QCheckBox (tr ("Enable multiformat input") );
    soxBox->setChecked (true);

    startsectorLabel = new QLabel (tr ("&Start sector:") );
    startsectorLineEdit = new QLineEdit;
    startsectorLabel->setBuddy (startsectorLineEdit);

    log = false;
    runMkisofs = true;
    debug = false;
    burnDisc = true;
    sox = true;
    startsector = "";

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
    optionsLayout->addSpacing (25);
    optionsLayout->addLayout (mkisofsLayout);
    optionsLayout->addSpacing (25);
    optionsLayout->addLayout (cdrecordLayout);
    optionsLayout->addSpacing (25);
    optionsLayout->addLayout (logLayout);
    optionsLayout->addSpacing (25);
    optionsLayout->addWidget (soxBox);
    optionsLayout->addSpacing (25);

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


    setWindowTitle (tr ("Options") );
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

