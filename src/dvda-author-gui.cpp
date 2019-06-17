/*

dvda-author-gui.cpp  - Author a DVD-Audio DVD with dvda-author

This application uses Qt4.4 . Check Qt's licensing details on http://trolltech.com

Class  dvda:  Main central widget
class DomParser: XML parser for dvda-author-gui projects (*.dvp)

Copyright Fabrice Nicol <fabnicol@users.sourceforge.net> Feb 2009

The latest version can be found at http://dvd-audio.sourceforge.net

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/




#include <QtGui>
#include <QFile>
#include <sys/stat.h>
#include <errno.h>
#include <QModelIndex>

#include <QtXml>
#include "dvda-author-gui.h"


void dvda::addDraggedFiles (QList<QUrl> urls)
{
    currentIndex = tabWidget->currentIndex();
    g_syntax_enabled[currentIndex] = 1;

    for (int i = 0; i < urls.size(); ++i)
        {
            selectedFile[i] = (QString) urls.at (i).toLocalFile();
            project[currentIndex]->addItem (selectedFile[i]);
            inputSize[currentIndex] += QFileInfo (selectedFile[i]).size();
            inputSizeCount += QFileInfo (selectedFile[i]).size();
            outputTextEdit->append (tr ("Added audio file to group %1:\n  size of group %2, total size %3").arg (QString::number (
                                        currentIndex + 1), QString::number (inputSize[currentIndex]), QString::number (inputSizeCount) ) );
        }
}



void dvda::initialize()
{
    adjustSize();
    rank = video_rank = 0;
    inputSizeCount = 0;
    memset (inputSize, 0, 9 * sizeof (qint64) );
    memset (g_syntax_enabled, 0, 9 * sizeof (bool) );
    memset (video_syntax_enabled, 0, 99 * sizeof (bool) );
    i_syntax_enabled = o_syntax_enabled = V_syntax_enabled = 0;
    extractSwitch = 0;
    project_manager_enabled = 0;
    videoTitleRank = 1;
    startProgressBar = startProgressBar2 = startProgressBar3 = 0;
    inputTotalSize = 0;

    for (int i = 0; i < 9; ++i)
        {
            project[i] = NULL;
        }

    for (int i = 0; i < 99; ++i)
        {
            project2[i] = NULL;
        }

    progress2 = progress3 = NULL;
    treeWidget = NULL;
    tempdir = QDir::homePath ();
    readSettings();
}


dvda::dvda (QWidget* parent)  : QDialog (parent)
{
    dialog = new options (this);
    initialize();
    model = new QFileSystemModel;
    model->setReadOnly (false);
    model->setRootPath (QDir::homePath());
    model->sort (Qt::AscendingOrder);
    model->setFilter (QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
    model->setNameFilterDisables (false);
    model->setNameFilters({"*.wav",
                           "*.flac",
                           "*.oga",
                           "*.aif",
                           "*.aifc",
                           "*.aiff",
                           "*.aiffc",
                           "*.al",
                           "*.amb",
                           "*.au",
                           "*.avr",
                           "*.cdda",
                           "*.cdr",
                           "*.cvs",
                           "*.cvsd",
                           "*.cvu",
                           "*.dat",
                           "*.dvms",
                           "*.fssd",
                           "*.gsm",
                           "*.gsrt",
                           "*.hcom",
                           "*.htk",
                           "*.ima",
                           "*.ircam",
                           "*.la",
                           "*.lpc",
                           "*.lpc10",
                           "*.lu",
                           "*.maud",
                           "*.nist",
                           "*.ogg",
                           "*.raw",
                           "*.s1",
                           "*.s16",
                           "*.s2",
                           "*.s24",
                           "*.s3",
                           "*.s32",
                           "*.s4",
                           "*.s8",
                           "*.sb",
                           "*.sf",
                           "*.sl",
                           "*.sln",
                           "*.smp",
                           "*.snd",
                           "*.sndr",
                           "*.sndt",
                           "*.sph",
                           "*.sw",
                           "*.txw",
                           "*.vms",
                           "*.voc",
                           "*.vorbis",
                           "*.vox",
                           "*.wavpcm",
                           "*.wv",
                           "*.wve",
                           "*.xa"});

    treeView = new QTreeView;
    treeView->setModel (model);
    treeView->header()->setSortIndicator (0, Qt::AscendingOrder);
    treeView->header()->setSortIndicatorShown (false);

    treeView->hideColumn(1);
    treeView->hideColumn(2);
    treeView->setMinimumWidth(300);
    treeView->setColumnWidth(0,400);

    treeView->setSelectionMode (QAbstractItemView::ExtendedSelection);
    treeView->setSelectionBehavior (QAbstractItemView::SelectItems);
    QModelIndex index = model->index (QDir::currentPath() );

    treeView->scrollTo(index);
    treeView->resizeColumnToContents (true);
    rightButton = new QToolButton (this);
    rightButton->setObjectName (QString::fromUtf8 ("rightButton") );
    rightButton->setToolTip (tr ("Import selected file to active group tab") );
    const QIcon icon = QIcon (QString::fromUtf8 ( ":/images/document-import.png") );
    rightButton->setIcon (icon);
    rightButton->setIconSize (QSize (22, 22) );
    mainTabWidget = new QTabWidget;
    tabWidget = new QTabWidget;
    project[0] = new QListWidget (this);
    project2[0] = new QListWidget (this);
    tab2Widget = NULL;
    on_hasAudiozoneBox_toggled (dialog->hasAudiozone);
    on_hasVideozoneBox_toggled (dialog->hasVideozone);
    tabWidget->addTab (project[0], tr ("group 1") );
    tabWidget->setToolTip (tr ("List files in each audio group tab") );
    mkdirButton = new QToolButton (this);
    mkdirButton->setToolTip (tr ("Create Directory...") );
    const QIcon iconCreate = QIcon (QString::fromUtf8 ( ":/images/folder-new.png") );
    mkdirButton->setIcon (iconCreate);
    mkdirButton->setIconSize (QSize (22, 22) );
    removeButton = new QToolButton (this);
    removeButton->setToolTip (tr ("Remove directory or file...") );
    const QIcon iconRemove = QIcon (QString::fromUtf8 ( ":/images/edit-delete.png") );
    removeButton->setIcon (iconRemove);
    removeButton->setIconSize (QSize (22, 22) );
    addGroupButton = new QToolButton (this);
    addGroupButton->setToolTip (tr ("Add new DVD-Audio group tab") );
    const QIcon iconNew = QIcon (QString::fromUtf8 ( ":/images/tab-new.png") );
    addGroupButton->setIcon (iconNew);
    addGroupButton->setIconSize (QSize (22, 22) );
    deleteGroupButton = new QToolButton (this);
    deleteGroupButton->setToolTip (tr ("Delete current DVD-Audio group tab") );
    const QIcon iconDelete = QIcon (QString::fromUtf8 ( ":/images/tab-close-other.png") );
    deleteGroupButton->setIcon (iconDelete);
    deleteGroupButton->setIconSize (QSize (22, 22) );
    moveUpItemButton = new QToolButton (this);
    moveUpItemButton->setToolTip (tr ("Move group item up") );
    const QIcon iconUp = QIcon (QString::fromUtf8 ( ":/images/arrow-up.png") );
    moveUpItemButton->setIcon (iconUp);
    moveUpItemButton->setIconSize (QSize (22, 22) );
    retrieveItemButton = new QToolButton (this);
    retrieveItemButton->setToolTip (tr ("Retrieve group item") );
    retrieveItemButton->setObjectName (QString::fromUtf8 ("Retrieve") );
    const QIcon iconRetrieve = QIcon (QString::fromUtf8 ( ":/images/retrieve.png") );
    retrieveItemButton->setIcon (iconRetrieve);
    retrieveItemButton->setIconSize (QSize (22, 22) );
    moveDownItemButton = new QToolButton (this);
    moveDownItemButton->setToolTip (tr ("Move group item down") );
    moveDownItemButton->setObjectName (QString::fromUtf8 ("Down") );
    const QIcon iconDown = QIcon (QString::fromUtf8 ( ":/images/arrow-down.png") );
    moveDownItemButton->setIcon (iconDown);
    moveDownItemButton->setIconSize (QSize (22, 22) );
    killButton = new QToolButton (this);
    killButton->setToolTip (tr ("Kill dvda-author") );
    const QIcon iconKill = QIcon (QString::fromUtf8 ( ":/images/process-stop.png") );
    killButton->setIcon (iconKill);
    killButton->setIconSize (QSize (22, 22) );
    progress = new QProgressBar (this);
    progress->reset();
    progress->setRange (0, maxRange = 100);
    progress->setToolTip (tr ("DVD-Audio structure authoring progress bar") );
    outputTextEdit = new QTextEdit;
    outputTextEdit->setAcceptDrops (false);
    connect (mkdirButton, SIGNAL (clicked() ), this, SLOT (createDirectory() ) );
    connect (removeButton, SIGNAL (clicked() ), this, SLOT (remove() ) );
    connect (addGroupButton, SIGNAL (clicked() ), this, SLOT (addGroup() ) );
    connect (deleteGroupButton, SIGNAL (clicked() ), this, SLOT (deleteGroup() ) );
    connect (killButton, SIGNAL (clicked() ), this, SLOT (killDvda() ) );
    connect (&process, SIGNAL (finished (int, QProcess::ExitStatus) ), this, SLOT (processFinished (int,
             QProcess::ExitStatus) ) );
    connect (&processLplex, SIGNAL (finished (int, QProcess::ExitStatus) ), this, SLOT (processLplexFinished (int,
                                                                                              QProcess::ExitStatus) ) );
    connect (&process2, SIGNAL (finished (int, QProcess::ExitStatus) ), this, SLOT (process2Finished (int,
             QProcess::ExitStatus) ) );

    connect (&process3, SIGNAL (finished (int, QProcess::ExitStatus) ), this, SLOT (process3Finished (int,
             QProcess::ExitStatus) ) );
    connect (rightButton, SIGNAL (clicked() ), this, SLOT (on_rightButton_clicked() ) );
    connect (moveUpItemButton, SIGNAL (clicked() ), this, SLOT (on_moveUpItemButton_clicked() ) );
    connect (moveDownItemButton, SIGNAL (clicked() ), this, SLOT (on_moveDownItemButton_clicked() ) );
    connect (retrieveItemButton, SIGNAL (clicked() ), this, SLOT (on_retrieveItemButton_clicked() ) );
    QHBoxLayout* projectLayout = new QHBoxLayout;
    QVBoxLayout* updownLayout = new QVBoxLayout;
    QVBoxLayout* mkdirLayout = new QVBoxLayout;
    QHBoxLayout* progress1Layout = new QHBoxLayout;
    progressLayout = new QVBoxLayout;
    mkdirLayout->addWidget (mkdirButton);
    mkdirLayout->addWidget (removeButton);
    projectLayout->addLayout (mkdirLayout);
    projectLayout->addWidget (treeView);
    projectLayout->addWidget (rightButton);
    projectLayout->addWidget (mainTabWidget);
    updownLayout->addWidget (moveUpItemButton);
    updownLayout->addWidget (retrieveItemButton);
    updownLayout->addWidget (moveDownItemButton);
    updownLayout->addStretch();
    updownLayout->addWidget (addGroupButton);
    updownLayout->addWidget (deleteGroupButton);
    updownLayout->addStretch();
    projectLayout->addLayout (updownLayout);
    mainLayout = new QVBoxLayout;
    mainLayout->addLayout (projectLayout);
    mainLayout->addWidget (outputTextEdit);
    progressLayout = new QVBoxLayout;
    progress1Layout->addWidget (killButton);
    progress1Layout->addWidget (progress);
    progressLayout->addLayout (progress1Layout);
    mainLayout->addLayout (progressLayout);
    QHBoxLayout* allLayout = new QHBoxLayout;
    managerLayout = new QVBoxLayout;
    allLayout->addLayout (managerLayout);
    allLayout->addLayout (mainLayout);
    setLayout (allLayout);
    setWindowTitle (tr ("dvda-author") );
    const QIcon dvdaIcon = QIcon (QString::fromUtf8 ( ":/images/dvda-author.png") );
    setWindowIcon (dvdaIcon);
}


#ifdef QT_FILE_DIALOG
void dvda::on_openProjectButton_clicked()
{
    if (!dialog)
        {
            dialog = new options (this);
        }

    QString  projectFile = QFileDialog::getOpenFileName (this,  tr ("Open project"), "",  tr ("Projects (*.dvp)") );
    recentProjectFile (projectFile);
}
#endif

void dvda::recentProjectFile (const QString& projectFile)
{
    if (!projectFile.isEmpty() )
        {
            parseFile (projectFile);
            parent->setCurrentFile (projectFile);
            treeWidget->setMinimumSize (treeWidget->sizeHint() );
            project_manager_enabled = 1;
            on_openTreeWidgetButton_clicked();
            treeWidget->resizeColumnToContents (0);
            treeWidget->resizeColumnToContents (1);
        }
}

void dvda::on_helpButton_clicked()
{
    QStringList args;
    args  << QString::fromUtf8 (HTMLDIR "/GUI.shtml") ;
    helpProcess.start ("firefox", args);
}

void dvda::on_openTreeWidgetButton_clicked()
{
    static bool close;

    if (!project_manager_enabled)
        {
            return;
        }

    if (close)
        {
            treeWidget->hide();
            close = 0;
        }

    else
        {
            treeWidget->show();
            close = 1;
        }
}


void dvda::addGroup()
{
    QString number;
    currentMainTabIndex = mainTabWidget->currentIndex();
    currentIndex = 0;

    if (currentMainTabIndex == 0)
        {
            rank++;

            if (rank < 9)
                {
                    number = QString::number (rank + 1);
                }

            else
                {
                    QMessageBox::information (this, tr ("Group"), tr ("A maximum of 9 groups can be created.") );
                    return;
                }

            /* There will be a limited memory leak in case of multiple delete/addGroup operations but this is negliglible */
            tabWidget->insertTab (rank, project[rank] = new QListWidget (this), tr ("group %1").arg (number) );
            tabWidget->setCurrentIndex (rank);
        }

    else
        {
            video_rank++;

            if (video_rank < 99)
                {
                    number = QString::number (video_rank + 1);
                }

            else
                {
                    QMessageBox::information (this, tr ("Titleset"), tr ("A maximum of 99 titlesets can be created.") );
                    return;
                }

            /* There will be a limited memory leak in case of multiple delete/addGroup operations but this is negliglible */
            tab2Widget->insertTab (video_rank, project2[video_rank] = new QListWidget (this), tr ("titleset %1").arg (number) );
            tab2Widget->setCurrentIndex (video_rank);
        }
}

void dvda::deleteGroup()
{
    currentMainTabIndex = mainTabWidget->currentIndex();
    currentIndex = 0;

    if (currentMainTabIndex == 0)
        {
            currentIndex = tabWidget->currentIndex();

            for (int i = currentIndex; i <= rank ; ++i)
                {
                    tabWidget->removeTab (i);
                    outputTextEdit->append (tr ("Removing tab %1").arg (QString::number (i + 1) ) );
                }

            if (currentIndex < rank)
                {
                    for (int j = currentIndex; j < rank ; j++)
                        {
                            project[j]->clear();
                            project[j] = project[j + 1];
                            tabWidget->insertTab (j, project[j], tr ("group %1").arg (QString::number (j + 1) ) );
                        }
                }

            rank--;
            g_syntax_enabled[currentIndex] = 0;
            inputSizeCount -= inputSize[currentIndex];
            outputTextEdit->append (tr ("Deleted audio group %1:\n  total size is now %2").arg (QString::number (currentIndex + 1),
                                    QString::number (inputSizeCount) ) );
        }

    else
        {
            currentIndex = tab2Widget->currentIndex();

            for (int i = currentIndex; i <= video_rank ; ++i)
                {
                    tab2Widget->removeTab (i);
                    outputTextEdit->append (tr ("Removing tab %1").arg (QString::number (i + 1) ) );
                }

            if (currentIndex < video_rank)
                {
                    for (int j = currentIndex; j < video_rank ; j++)
                        {
                            project2[j]->clear();
                            project2[j] = project2[j + 1];
                            tab2Widget->insertTab (j, project2[j], tr ("titleset %1").arg (QString::number (j + 1) ) );
                        }
                }

            video_rank--;
            video_syntax_enabled[currentIndex] = 0;
            outputTextEdit->append (tr ("Deleted video titleset %1:\n  total size is now %2").arg (QString::number (
                                        currentIndex + 1), QString::number (inputSizeCount) ) );
        }
}


void dvda::addFileToProject (QListWidget* project, int& currentIndex, int&  currentMainTabIndex)
{
    QItemSelectionModel* selectionModel = treeView->selectionModel();
    QModelIndexList  indexList = selectionModel->selectedIndexes();

    //if (!index.isValid())  return;
    if (indexList.isEmpty() )
        {
            return;
        }

    //selectedFile = QDir::convertSeparators(model->filePath(index));
    QString group_type = (currentMainTabIndex == 0) ? "audio group" : "video group";
    QModelIndex index;

    for (int i = 0; i < indexList.size(); ++i)
        {
            index = indexList.at (i);
            selectedFile[i] = model->filePath (index);

            //QVariant elementSelectionne = modele->data(listeSelections[i], Qt::DisplayRole);
            if (model->fileInfo (index).isFile() )
                {
                    project->addItem (selectedFile[i]);
                    inputSize[currentIndex] += model->fileInfo (index).size();
                    inputSizeCount += model->fileInfo (index).size();
                    outputTextEdit->append (tr (qPrintable ("Added audio file to " + group_type  +
                                                            " %1:\n  size of group %2, total size %3") ).arg (QString::number (currentIndex + 1),
                                                                    QString::number (inputSize[currentIndex]), QString::number (inputSizeCount) ) );
                }

            else
                QMessageBox::information (this, tr ("Select"),
                                          tr ("%1 is not a file, try again.").arg (selectedFile[i]) );
        }
}

void dvda::on_rightButton_clicked()
{
    currentMainTabIndex = mainTabWidget->currentIndex();
    currentIndex = 0;

    if (currentMainTabIndex)
        {
            currentIndex = tab2Widget->currentIndex();
            video_syntax_enabled[currentIndex] = 1;
            addFileToProject (project2[currentIndex], currentIndex, currentMainTabIndex);
            currentIndexMax = (currentIndex > currentIndexMax) ? currentIndex : currentIndexMax;
        }

    else
        {
            currentIndex = tabWidget->currentIndex();
            g_syntax_enabled[currentIndex] = 1;
            addFileToProject (project[currentIndex], currentIndex, currentMainTabIndex);
        }
}


void dvda::on_moveUpItemButton_clicked()
{
    currentMainTabIndex = mainTabWidget->currentIndex();
    currentIndex = 0;

    if (currentMainTabIndex)
        {
            currentIndex = tab2Widget->currentIndex();
            int row = project2[currentIndex]->currentRow();

            if (row)
                {
                    QListWidgetItem* temp = project2[currentIndex]->takeItem (row);
                    QListWidgetItem* temp2 = project2[currentIndex]->takeItem (row - 1);
                    project2[currentIndex]->insertItem (row - 1, temp);
                    project2[currentIndex]->insertItem (row, temp2);
                    project2[currentIndex]->setCurrentRow (row - 1);
                }
        }

    else
        {
            currentIndex = tabWidget->currentIndex();
            int row = project[currentIndex]->currentRow();

            if (row)
                {
                    QListWidgetItem* temp = project[currentIndex]->takeItem (row);
                    QListWidgetItem* temp2 = project[currentIndex]->takeItem (row - 1);
                    project[currentIndex]->insertItem (row - 1, temp);
                    project[currentIndex]->insertItem (row, temp2);
                    project[currentIndex]->setCurrentRow (row - 1);
                }
        }
}


void dvda::on_moveDownItemButton_clicked()
{
    currentMainTabIndex = mainTabWidget->currentIndex();
    currentIndex = 0;

    if (currentMainTabIndex)
        {
            currentIndex = tab2Widget->currentIndex();
            int row = project2[currentIndex]->currentRow();
            QListWidgetItem* temp = project2[currentIndex]->takeItem (row + 1);
            QListWidgetItem* temp2 = project2[currentIndex]->takeItem (row);
            project2[currentIndex]->insertItem (row, temp);
            project2[currentIndex]->insertItem (row + 1, temp2);
            project2[currentIndex]->setCurrentRow (row + 1);
        }

    else
        {
            currentIndex = tabWidget->currentIndex();
            int row = project[currentIndex]->currentRow();
            QListWidgetItem* temp = project[currentIndex]->takeItem (row + 1);
            QListWidgetItem* temp2 = project[currentIndex]->takeItem (row);
            project[currentIndex]->insertItem (row, temp);
            project[currentIndex]->insertItem (row + 1, temp2);
            project[currentIndex]->setCurrentRow (row + 1);
        }
}


void dvda::on_retrieveItemButton_clicked()
{
    currentMainTabIndex = mainTabWidget->currentIndex();
    currentIndex = 0;
    QModelIndex index = treeView->currentIndex();

    if (!index.isValid() )
        {
            return;
        }

    if (currentMainTabIndex)
        {
            currentIndex = tab2Widget->currentIndex();
            int row = project2[currentIndex]->currentRow();
            project2[currentIndex]->takeItem (row);
            outputTextEdit->append (tr ("Retrieved audio file from group %1:\n  size of group %2, total size %3").arg (
                                        QString::number (currentIndex + 1), QString::number (inputSize[currentIndex]), QString::number (inputSizeCount) ) );
        }

    else
        {
            currentIndex = tabWidget->currentIndex();
            int row = project[currentIndex]->currentRow();
            project[currentIndex]->takeItem (row);
            inputSize[currentIndex] -= model->fileInfo (index).size();
            inputSizeCount -= model->fileInfo (index).size();
            outputTextEdit->append (tr ("Retrieved audio file from group %1:\n  size of group %2, total size %3").arg (
                                        QString::number (currentIndex + 1), QString::number (inputSize[currentIndex]), QString::number (inputSizeCount) ) );
        }
}


void dvda::createDirectory()
{
    QModelIndex index = treeView->currentIndex();

    if (!index.isValid() )
        {
            return;
        }

    QString dirName = QInputDialog::getText (this, tr ("Create Directory"), tr ("Directory name") );

    if (!dirName.isEmpty() )
        {
            if (!model->mkdir (index, dirName).isValid() )
                QMessageBox::information (this, tr ("Create Directory"),
                                          tr ("Failed to create the directory") );
        }
}

void dvda::remove()
{
    remove (NULL);
}

void dvda::remove (const QString& path)
{
    bool ok;

    if (path == NULL)
        {
            QModelIndex index  = treeView->currentIndex();
            QModelIndex parent = model->parent (index);

            if (!index.isValid() )
                {
                    return;
                }

            if (model->fileInfo (index).isDir() )
                {
                    ok = removeDirectory (model->filePath (index) ) ;
                }

            else
                {
                    ok = model->remove (index);
                }



            if (!ok)
                QMessageBox::information (this, tr ("Remove"),
                                          tr ("Failed to remove %1").arg (model->fileName (index) ) );
        }

    else
        if (QFileInfo (path).isDir() )
            {
                ok = removeDirectory (path) ;

                if (!ok)
                    QMessageBox::information (this, tr ("Remove"),
                                              tr ("Failed to remove %1").arg (QDir::convertSeparators (path) ) );
            }
}


bool dvda::removeDirectory (const QString& path)
{
    outputTextEdit -> append("remove top directory: " + path);
    if (path.isEmpty())
    {
        outputTextEdit->append("Path is empty");
        return(false);
    }

    QDir dir(path);
    bool ok = dir.exists();
    if ( ok )
    {
        QFileInfoList entries = dir.entryInfoList( QDir::NoDotAndDotDot |
                                                  QDir::Dirs | QDir::Files );
        foreach ( QFileInfo entryInfo, entries )
        {
            QString path = entryInfo.absoluteFilePath();
            if ( entryInfo.isDir() )
            {
                if ( ! removeDirectory(  path ) )
                {
                    ok = false;
                    break;
                }
            }
            else
            {
                QFile file( path );
                if ( ! file.remove() )
                {
                    ok = false;
                    break;
                }
            }
        }
    }

    if ( ok && ! dir.rmdir( dir.absolutePath() ) )
        ok = false;

    return ok;

}

void dvda::selectInput()
{
    QModelIndex index = treeView->currentIndex();

    if (!index.isValid() )
        {
            return;
        }

    sourceDir = model->filePath (index);
    outputTextEdit->clear();
    i_syntax_enabled = 1;

    if (model->fileInfo (index).isDir() )
        {
            outputTextEdit->append (tr ("Selected input directory: %1").arg (sourceDir) );
        }

    else
        QMessageBox::information (this, tr ("Select"),
                                  tr ("Failed to select %1, try again.").arg (sourceDir) );
}


void dvda::selectOutput (const QString& path, bool recreate)
{
    if (path == "")
        {
            QModelIndex index       = treeView->currentIndex();

            if (!index.isValid() )
                {
                    return;
                }

            targetDir = model->filePath (index);
        }

    else
        {
            targetDir = path;
        }

    /* It is recommended to clean the directory, otherwise ProgressBar is flawed. A Warning pops up for confirmation. I eschewed Qt here */

    if (scanDirectory (targetDir, QStringList() << "*") )
        {
        if (recreate)
        {
            if (QMessageBox::warning (this, tr ("Directory scan"),
                                      tr ("Directory %1 is not empty. Erase and recreate? ").arg (QDir::convertSeparators (targetDir) ),
                                      QMessageBox::Ok | QMessageBox::Cancel)
                    == QMessageBox::Ok)
                {
                    /* you may have to run as root or sudo to root depending on permissions */
                    remove (targetDir);
                }
        }

            QDir targetDirObject (targetDir);

            if (targetDirObject.mkpath (targetDir) == false)
                {
                    QMessageBox::warning (this, tr ("Directory view"), tr ("Failed to create %1").arg (targetDir), QMessageBox::Ok);
                    return;
                }
        }

        outputTextEdit->append (tr ("Selected output directory: %1").arg (QDir::convertSeparators(targetDir)) );
    o_syntax_enabled = 1;
}

void dvda::selectVideo()
{
    QModelIndex index = treeView->currentIndex();

    if (!index.isValid() )
        {
            return;
        }

    videoDir = model->filePath (index);

    if (model->fileInfo (index).isDir() )
        {
            outputTextEdit->append (tr ("Selected VIDEO_TS directory: %1").arg (videoDir) );
            bool ok;
            int i = QInputDialog::getInteger (this, tr ("Rank"),
                                              tr ("Enter rank of video title"), 1, 1, 99, 1, &ok);

            if (ok)
                {
                    videoTitleRank = i;
                }

            else
                {
                    QMessageBox::warning (this, tr ("Video title"), tr ("No title rank was entered. By default, title rank = 1"),
                                          QMessageBox::Ok );
                    videoTitleRank = 1;
                }

            V_syntax_enabled = 1;
        }

    else
        QMessageBox::information (this, tr ("Select"),
                                  tr ("Failed to select %1").arg (model->filePath (index) ) );
}

float dvda::discShare (qint64 directorySize)
{
    if (inputTotalSize > 1024 * 1024 * 1024 * 4.7)
        {
            outputTextEdit->append (tr ("Error: total size exceeds 4.7 GB") );
        }

    float share = 100 * ( (float) directorySize ) / ( (float) inputTotalSize);
    return share;
}


void dvda::run_dvda(bool modify_command_line)
{
    QStringList args;
    QString command;
    qint64 totalSize, inputSizeCount = 0;
    progress->reset();

    if (progress3 != NULL)
    {
        if (!dialog->burnDisc)
        {
            progressLayout->removeWidget (progress3);
            delete (progress3);
            progressLayout->removeWidget (killCdrecordButton);
            delete (killCdrecordButton);
            progress3 = NULL;
        }

        else
        {
            progress3->reset();
        }
    }

    if (progress2 != NULL)
    {
        if (!dialog->runMkisofs)
        {
            progressLayout->removeWidget (progress2);
            delete (progress2);
            progressLayout->removeWidget (killMkisofsButton);
            delete (killMkisofsButton);
            progress2 = NULL;
        }

        else
        {
            progress2->reset();
        }
    }

    for (int i = 0; i < 9; ++i)
        if (g_syntax_enabled[i])
        {
            inputSizeCount += inputSize[i];
        }

    if ( (!i_syntax_enabled) && (!g_syntax_enabled) )
    {
        i_syntax_enabled = 1;
        sourceDir = QString::fromUtf8 ("/input");
    }

    totalSize = (sourceDir.isEmpty() ) ? 0 : scanDirectory (sourceDir, QStringList() << "*.*");
    totalSize += inputSizeCount;
    inputTotalSize = totalSize;

    if (totalSize == 0)
    {
        processFinished (EXIT_FAILURE, QProcess::NormalExit);
        return;
    }

    args << "-n" << "-P0" << "-W";

    if (dialog->sox)
    {
        args << "-S";
    }

    if (dialog->debug)
    {
        args << "-d";
    }

    if (modify_command_line && dialog->videozone && video_syntax_enabled && video_syntax_enabled[0])
    {
        args << "-n";
        args << "--no-refresh-outdir";
    }

    if (dialog->startsector != "")
    {
        args << "-p" << dialog->startsector;
    }

    selectOutput (targetDir, false);

    if (i_syntax_enabled)
    {
        args << "-i" << sourceDir;
    }

    args << "-o" << targetDir;

    if (V_syntax_enabled)
    {
        args << "-V" <<  videoDir << "-T" << QString::number (videoTitleRank) ;
        qint64 videoDirSize = scanDirectory (videoDir, QStringList() << "*.*");
        outputTextEdit->append (tr ("Adding Video directory, size %1").arg (QString::number (videoDirSize) ) );
        totalSize += videoDirSize;
    }

    if (dialog->log)
    {
        args << "-l" << dialog->logPath;
    }

    for (int i = 0; i < 9; ++i)
    {
        if (g_syntax_enabled[i])
        {
            args << "-g" ;

            for (int row = 0; project[i]->item (row) != 0; ++row)
            {
                args <<  project[i]->item (row)->text();
            }
        }
    }

    outputTextEdit->append (tr ("Processing input directory...") );
    outputTextEdit->append (tr ("Size of input %1").arg (QString::number (totalSize) ) );
    command = args.join (" ");
    outputTextEdit->append (tr ("Command line : %1").arg (QDir::convertSeparators (command) ) );
    startProgressBar = 1;
    outputType = "DVD-Audio authoring";
    process.start (/*"konsole"*/ "dvda-author-dev", args);
}


void dvda::run()
{

    if (dialog->videozone && video_syntax_enabled && video_syntax_enabled[0])
    {
        runLplex();
    }
    else
    run_dvda(false);
 }

void dvda::runLplex()
{
    QStringList args;
    QString command;
    QString workpath = QDir::convertSeparators(QCoreApplication::applicationDirPath() + "/temp");

    removeDirectory(QDir::convertSeparators(targetDir));
    removeDirectory(QDir::convertSeparators(workpath));

    if (!o_syntax_enabled)
        {
            o_syntax_enabled = 1;
            /* If no explicit path is entered, output directory will be adjacent to executable */
            targetDir = tempdir + QDir::separator() + QString::fromUtf8 ("output");
        }

    for (int i = 0; i < 99 && project2 != NULL && project2[i] != NULL; ++i)
        {
            if (video_syntax_enabled[i])
                {
                    if (i)
                        {
                            args << "ts" ;
                        }

                    for (int row = 0; project2[i]->item (row) != 0; ++row)
                        {
                            args <<  project2[i]->item (row)->text();
                        }
                }
        }

    args << "--create" << "dvd" << "--dir" << QDir::convertSeparators(targetDir) << "-P" << "no" << "--video" << "pal" << "--jpeg" << QDir::convertSeparators(QCoreApplication::applicationDirPath()  + "/data/black_PAL_720x576.jpg") << "--workpath" << workpath << "-x" << "no";
    outputTextEdit->append (tr ("Processing input directory...") );
    command = args.join (" ");
    outputTextEdit->append (tr ("Command line : lplex %1").arg (QDir::convertSeparators (command) ) );
    startProgressBar = 1;
    outputType = "audio DVD-Video disc authoring";

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
#if defined __unix__ || defined __apple__ || defined __linux__
    env.insert("LD_LIBRARY_PATH", "$PWD/bin");
    env.insert("PATH", "$PWD:$PATH:bin");

#else
#if defined _WIN32

   env.insert("PATH", "%PATH%;bin");
#endif
#endif
    process.setProcessEnvironment(env);
    processLplex.start("lplex " + command);


}


void dvda::processLplexFinished (int exitCode,  QProcess::ExitStatus exitStatus)
{

    if (exitStatus == QProcess::CrashExit)
    {
        outputTextEdit->append (tr ("lplex crashed") );
        progress->setValue (0);
        return;
    }

    else
        if (exitCode == EXIT_FAILURE)
    {
        outputTextEdit->append (outputType + tr (" failed") );
        progress->setValue (0);
        return;
    }

    removeDirectory("temp");


    if (dialog->audiozone && g_syntax_enabled && g_syntax_enabled[0])
    {
        progress->setValue (maxRange/2);
        run_dvda(true);
    }
}


void dvda::runMkisofs()
{
    QStringList  argsMkisofs;
    argsMkisofs << "-dvd-hybrid" << "-o" << dialog->mkisofsPath << targetDir;

    QFile(dialog->mkisofsPath).remove();

    if (progress2 == NULL)
    {
        killMkisofsButton = new QToolButton (this);
        killMkisofsButton->setToolTip (tr ("Kill mkisofs") );
        const QIcon iconKill = QIcon (QString::fromUtf8 ( ":/images/process-stop.png") );
        killMkisofsButton->setIcon (iconKill);
        killMkisofsButton->setIconSize (QSize (22, 22) );
        connect (killMkisofsButton, SIGNAL (clicked() ), this, SLOT (killMkisofs() ) );
        progress2 = new QProgressBar (this);
        progress2->setRange (0, maxRange = 100);
        progress2->setToolTip (tr ("ISO file creation progress bar") );
        QHBoxLayout* progress2Layout = new QHBoxLayout;
        progress2Layout->addWidget (killMkisofsButton);
        progress2Layout->addWidget (progress2);
        progressLayout->addLayout (progress2Layout);
    }

    progress2->reset();
    startProgressBar2 = 1;
    outputTextEdit->append (tr ("Starting Mkisofs...") );
    process2.start ("bin/mkisofs", argsMkisofs);
    if (process2.waitForStarted())
        outputTextEdit->append (tr ("Mkisofs started...") );

}


void dvda::processFinished (int exitCode,  QProcess::ExitStatus exitStatus)
{

    startProgressBar = 0;
    startProgressBar3 = 0;

    if (exitStatus == QProcess::CrashExit)
        {
            outputTextEdit->append (tr ("dvda-author crashed") );
            progress->setValue (0);
            return;
        }

    else
        if (exitCode == EXIT_FAILURE)
            {
                outputTextEdit->append (outputType + tr (" failed") );
                progress->setValue (0);
                return;
            }

        else
            {
                outputTextEdit->append ("\n" + outputType + tr (" completed, output directory is %1").arg (QDir::convertSeparators(targetDir)) );
                scanDirectory (targetDir, QStringList() << "*.*");
                progress->setValue (maxRange);

                if (dialog->runMkisofs)
                    {
                       runMkisofs();
                    }
            }
}

void dvda::process2Finished (int exitCode,  QProcess::ExitStatus exitStatus)
{
    startProgressBar2 = 0;

    if (exitStatus == QProcess::CrashExit)
        {
            outputTextEdit->append (tr ("mkisofs crashed") );
            progress2->setValue (0);
        }

    else
        if (exitCode == EXIT_FAILURE)
            {
                outputTextEdit->append (tr ("ISO disc authoring failed") );
                progress->setValue (0);
            }

        else
            {
                outputTextEdit->append (tr ("\nISO file %1 created").arg (dialog->mkisofsPath) );
                progress2->setValue (maxRange);
                outputTextEdit->append (tr ("You can now burn your DVD-Audio disc") );
                if (dialog->burnDisc)
                {
                    on_cdrecordButton_clicked();
                }
            }
}

void dvda::process3Finished (int exitCode,  QProcess::ExitStatus exitStatus)
{
    startProgressBar3 = 0;

    if (exitStatus == QProcess::CrashExit)
        {
            outputTextEdit->append (tr ("cdrecord crashed") );
            progress3->setValue (0);
        }

    else
        if (exitCode == EXIT_FAILURE)
            {
                outputTextEdit->append (tr ("DVD-Audio disc was not burned") );
                progress3->setValue (0);
            }

        else
            {
                progress3->setValue (maxRange);

                process3.close();
                outputTextEdit->append (tr ("Disc burned.") );
            }
}


qint64 dvda::scanDirectory (const QString& path, const QStringList& filters)
{
    QDir dir (path);
    qint64 size = 0;

    foreach (QString file, dir.entryList (filters, QDir::Files) )
        {
            size += QFileInfo (dir, file).size();
        }

    foreach (QString subDir, dir.entryList (QDir::Dirs | QDir::NoDotAndDotDot) )
        {
            size += scanDirectory (path + QDir::separator() + subDir, filters);
        }

    outputTextEdit->append ( (tr ("Size of ") + QDir::convertSeparators (path) + tr (" is %1") ).arg (QString::number (
                                 size) ) );
    return size;
}


void dvda::on_cdrecordButton_clicked()
{
    if (!dialog  || ! dialog->burnDisc)
        {
            return;
        }

    QStringList argsCdrecord;

    if (!dialog->runMkisofs)
        {
            QMessageBox::warning (this, tr ("Record"),
                                  tr ("You need to create an ISO file first to be able to burn a DVD-Audio disc."), QMessageBox::Ok );
            return;
        }

    dialog->burnDisc = (!dialog->dvdwriterPath.isEmpty() );

    if (dialog->dvdwriterPath.isEmpty() )
        {
            QMessageBox::warning (this, tr ("Record"), tr ("You need to enter the path to a valid DVD writer device.<br>By defaut, choosing 0,0,0...."),
                                  QMessageBox::Ok );

            dialog->dvdwriterPath = "0,0,0";
         }

    QFileInfo f (dialog->mkisofsPath);
    f.refresh();

    if (! f.isFile() )
        {
            QMessageBox::warning (this, tr ("Record"), tr ("No valid ISO file path was entered:\n %1").arg (dialog->mkisofsPath),
                                  QMessageBox::Ok );
            return;
        }

    outputTextEdit->append (tr ("\nBurning disc...please wait.") );
    argsCdrecord << "-eject" << "blank=fast" << "dev=" + dialog->dvdwriterPath << dialog->mkisofsPath;
    outputTextEdit->append (tr ("Command line: cdrecord %1").arg (argsCdrecord.join (" ") ) );

    if (progress3 == NULL)
        {
            progress3 = new QProgressBar (this);
            killCdrecordButton = new QToolButton (this);
            killCdrecordButton->setToolTip (tr ("Kill cdrecord") );
            const QIcon iconKill = QIcon (QString::fromUtf8 ( ":/images/process-stop.png") );
            killCdrecordButton->setIcon (iconKill);
            killCdrecordButton->setIconSize (QSize (22, 22) );
            connect (killCdrecordButton, SIGNAL (clicked() ), this, SLOT (killCdrecord() ) );
            QHBoxLayout* progress3Layout = new QHBoxLayout;
            progress3Layout->addWidget (killCdrecordButton);
            progress3Layout->addWidget (progress3);
            progressLayout->addLayout (progress3Layout);
        }

    progress3->setRange (0, maxRange = 100);
    progress3->setToolTip (tr ("Burning DVD-Audio disc with cdrecord") );
    progress3->reset();
    startProgressBar3 = 1;
    process3.start ("bin/cdrecord", argsCdrecord);
    process3.waitForFinished();

}

void dvda::writeSettings()
{
    if (!dialog)
        {
            return;
        }

    QSettings settings ("dvda-author", "GUI");
    settings.setValue ("runMkisofs", dialog->runMkisofs);
    settings.setValue ("log", dialog->log);
    settings.setValue ("burnDisc", dialog->burnDisc);
    settings.setValue ("dvdwriterPath", dialog->dvdwriterPath);
    settings.setValue ("mkisofsPath", dialog->mkisofsPath);
    settings.setValue ("logPath", dialog->logPath);
    settings.setValue ("sox", dialog->sox);
    settings.setValue ("debug", dialog->debug);
    settings.setValue ("videozone", dialog->videozone);
    settings.setValue ("audiozone", dialog->audiozone);
    settings.setValue ("startsector", dialog->startsector);
    settings.setValue ("hasVideozone", dialog->hasVideozone);
    settings.setValue ("hasAudiozone", dialog->hasAudiozone);
}

void dvda::readSettings()
{
    QSettings settings ("dvda-author", "GUI");
    dialog->runMkisofs = settings.value ("runMkisofs", false).toBool();
    dialog->log = settings.value ("log", false).toBool();
    dialog->mkisofsPath = settings.value ("mkisofsPath").toString();
    dialog->logPath = settings.value ("logPath").toString();
    dialog->sox = settings.value ("sox", true).toBool();
    dialog->debug = settings.value ("debug", true).toBool();
    dialog->startsector = settings.value ("startsector", "").toString();
    dialog->burnDisc = settings.value ("burnDisc", dialog->burnDisc).toBool();
    dialog->dvdwriterPath = settings.value ("dvdwriterPath", dialog->dvdwriterPath).toString();
    dialog->videozone = settings.value ("videozone", true).toBool();
    dialog->videozone = settings.value ("audiozone", true).toBool();
    dialog->hasVideozone = settings.value ("hasVideozone", true).toBool();
    dialog->hasAudiozone = settings.value ("hasAudiozone", true).toBool();
}


bool dvda::on_optionsButton_clicked()
{
    if (dialog)
        {
            delete dialog;
        }

    dialog = new options (this);
    dialog->show();
    return 1;
}

void dvda::killDvda()
{
    process.kill();
    outputTextEdit->append (outputType + tr (" was killed (SIGKILL)") );
    processFinished (EXIT_FAILURE, QProcess::NormalExit );
}

void dvda::killMkisofs()
{
    process2.kill();
    outputTextEdit->append (tr ("mkisofs processing was killed (SIGKILL)") );
    process2Finished (EXIT_FAILURE, QProcess::NormalExit);
}

void dvda::killCdrecord()
{
    process3.kill();
    outputTextEdit->append (tr ("cdrecord processing was killed (SIGKILL)") );
    process3Finished (EXIT_FAILURE, QProcess::NormalExit);
}

void dvda::on_hasVideozoneBox_toggled (bool checked)
{
    currentIndex = 0;
    int count = mainTabWidget->count();

    if (checked == true)
        {
            if (tab2Widget == NULL)
                {
                    tab2Widget = new QTabWidget;
                }

            mainTabWidget->insertTab (1, tab2Widget, tr ("VIDEO") );
            tab2Widget->addTab (project2[currentIndex], tr ("titleset 1") );
            tab2Widget->setToolTip (tr ("List files in each video titleset tab") );
        }

    else
        {
            if (tab2Widget != NULL)
                {
                    if (count)
                        {
                            mainTabWidget->removeTab (count - 1);
                        }

                    tab2Widget = NULL;
                }
        }
}

void dvda::on_hasAudiozoneBox_toggled (bool checked)
{
    currentIndex = 0;

    if (checked == true)
        {
            if (tabWidget == NULL)
                {
                    tabWidget = new QTabWidget;
                }

            mainTabWidget->insertTab (0, tabWidget, tr ("AUDIO") );
            tabWidget->addTab (project[currentIndex], tr ("group 1") );
            tabWidget->setToolTip (tr ("List files in each audio group tab") );
        }

    else
        {
            if (tabWidget != NULL)
                {
                    mainTabWidget->removeTab (0);
                    tabWidget = NULL;
                }
        }
}

void dvda::extract()
{
    QStringList args;
    progress->reset();
    outputType = "Audio recovery";

    if (sourceDir.isEmpty() )
        {
            QMessageBox::warning (this, tr ("Extract"), tr ("Directory path is empty. Please select disc structure."),
                                  QMessageBox::Ok | QMessageBox::Cancel);
            return;
        }

    args << "-P0";
    extractSwitch = 1;
    args << "-x" << sourceDir;
    selectOutput (targetDir);

    if (!o_syntax_enabled)
        {
            o_syntax_enabled = 1;
            /* PATCH 09.08: previous default output not user-friendly (/usr/bin/output) for *nix */
            targetDir = tempdir + QString::fromUtf8 ("/output");
        }

    args << "-o" << targetDir;

    if (dialog->log)
        {
            args << "-l" << dialog->logPath << "-d";
        }

    inputTotalSize = (sourceDir.isEmpty() ) ? 0 : scanDirectory (sourceDir, QStringList() << "*.AOB");

    if (inputTotalSize == 0)
        {
            processFinished (EXIT_FAILURE, QProcess::NormalExit);
            return;
        }

    outputTextEdit->append (tr ("Processing DVD-Audio structure %1").arg (sourceDir) );
    outputTextEdit->append (tr ("Size of audio content %1").arg (QString::number (inputTotalSize) ) );
    QString command = args.join (" ");
    outputTextEdit->append (tr ("Command line : %1").arg (command) );
    startProgressBar3 = 1;
    dialog->runMkisofs = 0;
    process.start (/*"konsole"*/ "dvda-author", args);
}

#ifdef QT_FILE_DIALOG
void dvda::saveProject()
{
    QString projectName = QFileDialog::getSaveFileName (this,  tr ("Set project file name"), "");
    QFile projectFile (projectName);
    QErrorMessage* errorMessageDialog = new QErrorMessage (this);

    if (!dialog)
        {
            return;
        }

    if (!projectFile.open (QIODevice::WriteOnly) )
        {
            errorMessageDialog->showMessage (tr ("Cannot open file for writing") + qPrintable (projectFile.errorString() ) );
            QLabel* errorLabel = new QLabel;
            errorLabel->setText (tr ("If the box is unchecked, the message "
                                     "won't appear again.") );
            return;
        }

    QTextStream out (&projectFile);
#define GROUP(X)  "  <switch label=\""+ QString(X) + "\">\n"
#define SWITCH(X) GROUP(X) + "    <value>"
#define EO_SWITCH QString("</value>\n  </switch>\n")
    out.setCodec ("UTF-8");
    out << "<?xml version=\"1.0\"?>\n" << "<project>\n";
    out << " <system>\n";
    out << SWITCH ("startsector") << dialog->startsector << EO_SWITCH;
    out << SWITCH ("sox") << QString::number (dialog->sox) << EO_SWITCH;
    out << SWITCH ("videozone") << QString::number (dialog->videozone) << EO_SWITCH;
    out << SWITCH ("audiozone") << QString::number (dialog->audiozone) << EO_SWITCH;
    out << SWITCH ("hasAudiozone") << QString::number (tabWidget != NULL) << EO_SWITCH;
    out << SWITCH ("hasVideozone") << QString::number (tab2Widget != NULL) << EO_SWITCH;
    out << SWITCH ("log") << QString::number (dialog->log) << EO_SWITCH;

    if (dialog->log)
        {
            out << SWITCH ("logPath") << dialog->logPath << EO_SWITCH;
        }

    out << SWITCH ("runMkisofs") << QString::number (dialog->runMkisofs) << EO_SWITCH;

    if (dialog->runMkisofs)
        {
            out << SWITCH ("mkisofsPath") << dialog->mkisofsPath << EO_SWITCH;
        }

    out << SWITCH ("burnDisc") << QString::number (dialog->burnDisc) << EO_SWITCH;

    if (dialog->burnDisc)
        {
            out << SWITCH ("dvdwriterPath") << dialog->dvdwriterPath << EO_SWITCH;
        }

    out << SWITCH ("debug") << QString::number (dialog->debug) << EO_SWITCH;
    out << " </system>\n";
    out << " <data>\n";
    out << SWITCH ("videodir") << videoDir << EO_SWITCH;
    out << SWITCH ("videoTitleRank") << QString::number (videoTitleRank) << EO_SWITCH;
    out << SWITCH ("sourceDir") << sourceDir << EO_SWITCH;

    for (int i = 0; i <= rank; ++i)
        {
            out << GROUP ("group " + QString::number (i + 1) ) ;

            for (int row = 0; project[i]->item (row) != 0; ++row)
                {
                    out << "    <file>" << project[i]->item (row)->text() << "</file>\n";
                }

            out <<  "  </switch>\n";
        }

    for (int i = MaxRecentFiles - 1; i >= 0;  i--)
        {
            if (!parent->hasRecentFiles[i])
                {
                    continue;
                }

            out << SWITCH ("parent->recentFiles[" + QString::number (i) + "]") << parent->recentFiles[i] << EO_SWITCH;
        }

    out << SWITCH ("targetDir") << targetDir << EO_SWITCH;

    if (extractSwitch)
        {
            out << SWITCH ("x") << sourceDir << EO_SWITCH;
        }

    out << " </data>\n";
    out << "</project>\n";
}
#endif

#define ASSIGN(X)  if (variable == #X) { dialog->X = value; }
#define ASSIGN_BOOL(X) if (variable == #X) {dialog->X = (bool) value.toShort(); }
#define ASSIGN_MAIN_INT(X) if (variable == #X) { X = value.toUInt(); }
#define ASSIGN_MAIN(X,Y) if (variable == #X) { X = value; Y=1;}
#define ASSIGN_RECENT(X) if (variable == #X) { parent->setCurrentFile(value);}

void dvda::assignVariables (QString& variable, QString& value)
{
    ASSIGN (startsector)
    ASSIGN_BOOL (sox)
    else  ASSIGN_BOOL (videozone)
        else  ASSIGN_BOOL (audiozone)
            else  ASSIGN_BOOL (hasVideozone)
                else  ASSIGN_BOOL (hasAudiozone)
                    else  ASSIGN_BOOL (log)
                        else
                            // if (dialog->log)
                            ASSIGN (logPath)
                            else  ASSIGN_BOOL (debug)
                                else  ASSIGN_BOOL (runMkisofs)
                                    else
                                        //if (dialog->runMkisofs)
                                        ASSIGN (mkisofsPath)
                                        else  ASSIGN_BOOL (burnDisc)
                                            else
                                                //if (dialog->burnDisc)
                                                ASSIGN (dvdwriterPath)
                                                else  ASSIGN_MAIN (videoDir, V_syntax_enabled)
                                                    else  ASSIGN_MAIN_INT (videoTitleRank)
                                                        else  ASSIGN_MAIN (sourceDir, i_syntax_enabled)
                                                            else  ASSIGN_MAIN (targetDir, o_syntax_enabled)
                                                                else  ASSIGN_RECENT (parent->recentFiles[4])
                                                                    else  ASSIGN_RECENT (parent->recentFiles[3])
                                                                        else  ASSIGN_RECENT (parent->recentFiles[2])
                                                                            else  ASSIGN_RECENT (parent->recentFiles[1])
                                                                                else  ASSIGN_RECENT (parent->recentFiles[0])
                                                                                }


void dvda::assignGroupFiles (int group_index, QString file)
{
    static int last_group_index;

    if (last_group_index != group_index)
        {
            addGroup();
            last_group_index = group_index;
        }

    g_syntax_enabled[group_index] = 1;
    project[group_index]->addItem (file);
    update();
}

bool dvda::parseFile (const QString fileName)
{
    QStringList labels;
    labels << QObject::tr ("Settings") << QObject::tr ("Values");

    if (treeWidget)
        {
            managerLayout->removeWidget (treeWidget);
            delete (treeWidget);
        }

    treeWidget = new QTreeWidget;
    managerLayout->addWidget (treeWidget);
    on_openTreeWidgetButton_clicked();
    treeWidget->setHeaderLabels (labels);
    treeWidget->setWindowTitle (QObject::tr ("Project manager") );
    treeWidget->hide();
    QFile file (fileName);
    DomParser domparser (&file, treeWidget, this);
    return (1);
}


DomParser::DomParser (QIODevice* device, QTreeWidget* tree, dvda* parent) : dvda (0)
{
    treeWidget = tree;
    QString errorStr;
    int errorLine;
    int errorColumn;
    QTreeWidgetItem* item = new QTreeWidgetItem (treeWidget);
    connect (this, SIGNAL (parseGroup (int, QString) ), parent, SLOT (assignGroupFiles (int, QString) ) );
    connect (this, SIGNAL (parseVariables (QString&, QString&) ), parent, SLOT (assignVariables (QString&, QString&) ) );
    QDomDocument doc;

    if (!doc.setContent (device, true, &errorStr, &errorLine, &errorColumn) )
        {
            QMessageBox::warning (0, QObject::tr ("DOM Parser"),
                                  QObject::tr ("Parse error at line %1, " "column %2:\n%3").arg (errorLine).arg (errorColumn).arg (errorStr) );
            return;
        }

    QDomElement root = doc.documentElement();

    if (root.tagName() != "project")
        {
            return;
        }

    QDomNode node = root.firstChild();

    if (node.toElement().tagName() != "system")
        {
            return;
        }

    item->setText (0, "system");
    item->setExpanded (true);
    QDomNode subnode = node.firstChild();

    while (!subnode.isNull() )
        {
            if (subnode.toElement().tagName() == "switch")
                {
                    parseEntry (subnode.toElement(), item);
                }

            subnode = subnode.nextSibling();
        }

    node = node.nextSibling();
    QTreeWidgetItem* item2 = new QTreeWidgetItem (treeWidget);

    if (node.toElement().tagName() != "data")
        {
            return;
        }

    item2->setText (0, "data");
    item2->setExpanded (true);
    subnode = node.firstChild();

    while (!subnode.isNull() )
        {
            if (subnode.toElement().tagName() == "switch")
                {
                    parseEntry (subnode.toElement(), item2);
                }

            subnode = subnode.nextSibling();
        }
}


void DomParser::parseEntry (const QDomElement& element, QTreeWidgetItem* parent)
{
    QTreeWidgetItem* item;

    if (parent)
        {
            item = new QTreeWidgetItem (parent);
        }

    else
        {
            item = new QTreeWidgetItem (treeWidget);
        }

    QString variable = element.attribute ("label");
    item->setText (0, variable);
    QDomNode node = element.firstChild();

    while (!node.isNull() )
        {
            if (node.toElement().tagName() == "switch" )
                {
                    parseEntry (node.toElement(), item);
                }

            else
                if (node.toElement().tagName() == "value")
                    {
                        QDomNode childNode = node.firstChild();

                        while (!childNode.isNull() )
                            {
                                if (childNode.nodeType() == QDomNode::TextNode)
                                    {
                                        QString value = childNode.toText().data();
                                        item->setText (1, value);
                                        emit (parseVariables (variable, value) );
                                        break;
                                    }

                                childNode = childNode.nextSibling();
                            }
                    }

                else
                    if (node.toElement().tagName() == "file")
                        {
                            QDomNode childNode = node.firstChild();
                            QString allFiles;
                            int group_index;
                            group_index = variable.right (1).toInt() - 1;
                            allFiles = "\n";

                            while (!childNode.isNull() )
                                {
                                    if (childNode.nodeType() == QDomNode::TextNode)
                                        {
                                            QString file = childNode.toText().data();
                                            allFiles = item->text (1);
                                            allFiles += "\n" + file;
                                            emit (parseGroup (group_index, file) );
                                            item->setText (1, allFiles);
                                            break;
                                        }

                                    childNode = childNode.nextSibling();
                                }
                        }

            node = node.nextSibling();
        }
}

