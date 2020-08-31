

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

    activeMenuBox = new QCheckBox (tr ("Active menu"), this) ;
    activeMenuBox->setChecked(false);
    activeMenuBox->setToolTip("Active menus remain visible on playback");

    inputRankBox[0] = new QComboBox (this);
    QLabel* rankLabel = new QLabel("Link to video titleset");
    rankList << "No" << "1";
    inputRankBox[0]->addItems(rankList);
    inputRankBox[0]->setEnabled(false);
    inputRankBox[0]->setToolTip("Add link to DVD-VIDEO titleset rank #1");

    mkisofsBox = new QCheckBox (tr ("Create ISO file with mkisofs"), this);
    mkisofsBox->setChecked(true);

    cdrecordBox = new QCheckBox (tr ("Burn disc with cdrecord") );
    cdrecordBox->setChecked (true);

    debugBox = new QCheckBox (tr ("Debugging-level verbosity") );

    soxBox = new QCheckBox (tr ("Enable multiformat input") );
    soxBox->setChecked (true);

    vlcBox = new QCheckBox (tr ("Use VLC for playback") );
    vlcBox->setChecked (false);

    decodeBox = new QCheckBox (tr ("Decode MLP to WAV when extracting audio") );
    decodeBox->setChecked (false);

    log = false;
    runMkisofs = true;
    debug = false;
    burnDisc = true;
    sox = true;
    vlc = false;
    decode = false;
    menu = true;
    startsector = "";

    menuLayout = new QHBoxLayout;
    menuLayout->addWidget (menuBox);
    menuLayout->addStretch();
    menuLayout->addWidget(rankLabel);
    menuLayout->addWidget (inputRankBox[0]);

    QHBoxLayout* activeMenuLayout = new QHBoxLayout;
    activeMenuLayout->addWidget (activeMenuBox);
    activeMenuLayout->addStretch();

    QHBoxLayout* mkisofsLayout = new QHBoxLayout;
    mkisofsLayout->addWidget (mkisofsBox);
    mkisofsLayout->addStretch();
    mkisofsLayout->addWidget (mkisofsButton);

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
    optionsLayout->addLayout (activeMenuLayout);
    optionsLayout->addSpacing (25);
    optionsLayout->addLayout (mkisofsLayout);
    optionsLayout->addSpacing (25);
    optionsLayout->addLayout (cdrecordLayout);
    optionsLayout->addSpacing (25);
    optionsLayout->addLayout (logLayout);
    optionsLayout->addSpacing (25);
    optionsLayout->addWidget (soxBox);
    optionsLayout->addSpacing (25);
    optionsLayout->addWidget (vlcBox);
    optionsLayout->addSpacing (25);
    optionsLayout->addWidget (decodeBox);
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
    connect (vlcBox, SIGNAL (clicked() ), this, SLOT (on_vlcBox_checked() ) );
    connect (decodeBox, SIGNAL (clicked() ), this, SLOT (on_decodeBox_checked() ) );
    connect (cdrecordBox, SIGNAL (clicked() ), this, SLOT (on_cdrecordBox_checked() ) );
    connect (dvdwriterLineEdit, SIGNAL (textChanged (const QString&) ), this, SLOT (on_dvdwriterLineEdit_changed (const QString&) ) );
    connect (menuBox, SIGNAL (clicked() ), this,     SLOT (on_menuBox_checked() ) );
    connect (menuBox, SIGNAL (clicked() ), this,     SLOT (on_activeMenuBox_checked() ) );
    connect (activeMenuBox, SIGNAL (clicked() ), this,     SLOT (on_activeMenuBox_checked() ) );
    connect(inputRankBox[0],
            QOverload<int>::of(&QComboBox::activated),
            [this] {
                emit(sendMessageToConsole("Setting video link to titleset #1"));
                videoTitleRank[0] = inputRankBox[0]->currentIndex();
            });
    setWindowTitle (tr ("Options") );
}

#ifndef WITHOUT_STARTSECTOR
void options::on_startsectorLineEdit_changed (const QString& startsectorValue)
{
    startsector = startsectorValue;
}
#endif

void options::addInputRankBox(int i)
{
    if ( inputRankBox[i] == nullptr)
    {
        inputRankBox[i] = new QComboBox (this);
        inputRankBox[i]->setToolTip("Add link to DVD-VIDEO titleset rank #" + QString::number(i + 1));
        menuLayout->addWidget (inputRankBox[i]);

        connect(inputRankBox[i],
                    QOverload<int>::of(&QComboBox::activated),
                    [this, i] {
                        emit(sendMessageToConsole("Setting video link to titleset #" + QString::number(i + 1)));
                        videoTitleRank[i] = inputRankBox[i]->currentIndex();
                    });
    }
    else inputRankBox[i]->show();
    rankList << QString::number(i+1);
    for (int r = 0 ; r <= i; ++r)
    {
      inputRankBox[r]->clear();
      inputRankBox[r]->addItems(rankList);
    }
    inputRankBox[i]->setEnabled(false);
}

void options::removeInputRankBox(int i)
{
    if (inputRankBox[i] != nullptr) inputRankBox[i]->hide();
    for (int r = 0; r < i ; ++r) inputRankBox[r]->removeItem(i + 1);
    rankList.removeLast();
    for (int r = 0 ; r <= i; ++r)
    {
        inputRankBox[r]->clear();
        inputRankBox[r]->addItems(rankList);
    }
}

void options::on_dvdwriterLineEdit_changed (const QString& dvdwriterValue)
{
    dvdwriterPath = dvdwriterValue;
}

void options::on_menuBox_checked()
{
    menu= menuBox->isChecked();
}


void options::on_activeMenuBox_checked()
{
    activeMenu = activeMenuBox->isChecked() & menuBox->isChecked();
}

void options::on_logBox_checked()
{
    logButton->setEnabled (logBox->isChecked() );
}

void options::on_soxBox_checked()
{
    sox = soxBox->isChecked();
}

void options::on_vlcBox_checked()
{
    vlc = vlcBox->isChecked();
}

void options::on_decodeBox_checked()
{
    decode = decodeBox->isChecked();
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

