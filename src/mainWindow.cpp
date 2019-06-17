#include <QtGui>
#include <QFile>
#include <QtGui>
#include <QtXml>
#include "dvda-author-gui.h"

/* AUTHOR NOTE
   MainWindow code is strongly inspired by code written by Jasmin Blanchette and Mark Summerfiled for their original bok C++ GUI programming with Qt4, 2006 */

/*

mainWindow.cpp  - Main Window for dvda-author-gui

This application uses Qt4.4 . Check Qt's licensing details on http://trolltech.com


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




MainWindow::MainWindow()
{
    dvda_author = new dvda (0);
    dvda_author->parent = this;
    setCentralWidget (dvda_author);
    dvda_author->setAcceptDrops (false);
    setAcceptDrops (true);
    createActions();
    createMenus();
    /*
    createContextMenu();*/
    createToolBars();
    /*
    createStatusBar();
    */
    setWindowIcon (QIcon (":/images/dvda-author.png") );
    memset (hasRecentFiles, 0, MaxRecentFiles);
    setCurrentFile ("");
}



void MainWindow::updateRecentFileActions()
{
    QMutableStringListIterator i (recentFiles);

    while (i.hasNext() )
        {
            if (!QFile::exists (i.next() ) )
                {
                    i.remove();
                }
        }

    for (int j = 0 ; j < MaxRecentFiles ; ++j)
        {
            if (j < recentFiles.count() )
                {
                    hasRecentFiles[j] = 1;
                    QString  text = tr ("&%1 %2").arg (j + 1).arg (strippedName (recentFiles[j]) );
                    recentFileActions[j]->setText (text);
                    recentFileActions[j]->setData (QVariant (recentFiles[j]) );
                    recentFileActions[j]->setVisible (true);
                }

            else
                {
                    hasRecentFiles[j] = 0;
                    recentFileActions[j]->setVisible (false);
                }
        }

    separatorAction->setVisible (!recentFiles.isEmpty() );
}


void MainWindow::setCurrentFile (const QString& fileName)
{
    curFile = fileName;
    setWindowModified (false);
    QString shownName = "Untitled";

    if (!curFile.isEmpty() )
        {
            shownName = strippedName (curFile);
            recentFiles.removeAll (curFile);
            recentFiles.prepend (curFile);
            updateRecentFileActions();
        }

    setWindowTitle (tr ("%1[*] - %2").arg (shownName).arg (tr ("dvda-author") ) );
}

QString MainWindow::strippedName (const QString& fullFileName)
{
    return QFileInfo (fullFileName).fileName();
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu ("&File");
    editMenu = menuBar()->addMenu ("&Edit");
    processMenu = menuBar()->addMenu ("&Process");
    ioMenu = menuBar()->addMenu ("&Data");
    optionsMenu = menuBar()->addMenu ("&Options");
    aboutMenu = menuBar()->addMenu ("&Help");
    fileMenu->addAction (openAction);
    fileMenu->addAction (saveAction);
    separatorAction = fileMenu->addSeparator();

    for (int i = 0; i < MaxRecentFiles ; ++i)
        {
            fileMenu->addAction (recentFileActions[i]);
        }

    fileMenu->addSeparator();
    fileMenu->addAction (exitAction);
    editMenu->addAction (displayAction);
    editMenu->addAction (displayManagerAction);
    processMenu->addAction (burnAction);
    processMenu->addAction (encodeAction);
    processMenu->addAction (encodeVideoAction);
    processMenu->addAction (decodeAction);
    ioMenu->addAction (inputAction);
    ioMenu->addAction (videoAction);
    ioMenu->addAction (outputAction);
    optionsMenu->addAction (optionsAction);
    aboutMenu->addAction (helpAction);
    aboutMenu->addAction (aboutAction);
}

void MainWindow::about()
{
    QMessageBox::about (this, tr ("About dvda-author"),
                        tr ("<h2>dvda-author graphical user interface " VERSION "</h2>"
                            "<p>Copyright &copy; 2009 Fabrice Nicol</p>"
                            "<p>dvda-author-gui is a Qt4.4 graphical user interface for dvda-author.</p>"
                            "<p>dvda-author-gui is licensed under the terms of the General Public Licence, version 3 or higher, "
                            "combined with Qt's specific open-source licensing conditions, check  <a href=\"http://trolltech.com\">Trolltech's site</a>.</p>"
                            "</p>"
                            "<p>This application uses dvda-author as its data-processing software to build a DVD-Audio disc.</p>"
                            "<p>This application optionally uses Lplex as its data-processing software to build an audio DVD-Video disc.</p>"
                            "<p></p>"
                            "<h2>dvda-author</h2>"
                            "<p>Copyright &copy; 2005-2009 David Chapman, Fabrice Nicol, Lee and Tim Feldkamp</p>"
                            "<p>dvda-author creates high-definition DVD-Audio discs.</p>"
                            "<p>dvda-author is licensed under the terms of the General Public Licence, version 3 or higher.</p>"
                            "<p></p>"
                            "<h2>Lplex</h2>"
                            "<p>Copyright &copy; Bahman Negahban</p>"
                            "<p>Lplex creates high-definition audio DVD-Video discs.</p>"
                            "<p>Lplex is licensed under the terms of the General Public Licence, version 2 or higher.</p>"
                            "<p></p>"
                            "<h2>Project Website</h2>"
                            "<a href=\"http://dvd-audio.sourceforge.net\">DVD audio tools webpage</a>"
                            "<p></p>") );
}

void MainWindow::dragEnterEvent (QDragEnterEvent* event)
{
    event->acceptProposedAction();
}

void MainWindow::dropEvent (QDropEvent* event)
{
    QList<QUrl> urls = event->mimeData()->urls();

    if (urls.isEmpty() )
        {
            return;
        }

    QString fileName = urls.first().toLocalFile();

    if (fileName.isEmpty() )
        {
            return;
        }

//if (readFile(fileName))
    setWindowTitle (tr ("%1 - %2").arg (fileName).arg (tr ("Drag File") ) );
//dvda_author->project[0]->addItem(fileName);
    dvda_author->addDraggedFiles (urls);
}



void MainWindow::createActions()
{
    openAction = new QAction (tr ("&Open project"), this);
    openAction->setIcon (QIcon (":/images/open-project.png") );
    connect (openAction, SIGNAL (triggered() ), dvda_author, SLOT (on_openProjectButton_clicked() ) );
    saveAction = new QAction (tr ("&Save project"), this);
    saveAction->setIcon (QIcon (":/images/document-save.png") );
    connect (saveAction, SIGNAL (triggered() ), dvda_author, SLOT (saveProject() ) );
    burnAction = new QAction (tr ("&Burn"), this);
    burnAction->setIcon (QIcon (":/images/burn.png") );
    connect (burnAction, SIGNAL (triggered() ), dvda_author, SLOT (on_cdrecordButton_clicked() ) );
    encodeAction = new QAction (tr ("&Encode all disc"), this);
    encodeAction->setIcon (QIcon (":/images/encode.png") );
    connect (encodeAction, SIGNAL (triggered() ), dvda_author, SLOT (run() ) );
    encodeVideoAction = new QAction (tr ("En&code VIDEO_TS"), this);
    encodeVideoAction->setIcon (QIcon (":/images/encodeVideo.png") );
    connect (encodeVideoAction, SIGNAL (triggered() ), dvda_author, SLOT (runLplex() ) );
    decodeAction = new QAction (tr ("&Decode"), this);
    decodeAction->setIcon (QIcon (":/images/decode.png") );
    connect (decodeAction, SIGNAL (triggered() ), dvda_author, SLOT (extract() ) );
    inputAction = new QAction (tr ("&Audio input"), this);
    inputAction->setIcon (QIcon (":/images/input.png") );
    connect (inputAction, SIGNAL (triggered() ), dvda_author, SLOT (selectInput() ) );
    outputAction = new QAction (tr ("&Output"), this);
    outputAction->setIcon (QIcon (":/images/output.png") );
    connect (outputAction, SIGNAL (triggered() ), dvda_author, SLOT (selectOutput() ) );
    videoAction = new QAction (tr ("&Video input"), this);
    videoAction->setIcon (QIcon (":/images/video.png") );
    connect (videoAction, SIGNAL (triggered() ), dvda_author, SLOT (selectVideo() ) );
    optionsAction = new QAction (tr ("&Options"), this);
    optionsAction->setIcon (QIcon (":/images/configure.png") );
    connect (optionsAction, SIGNAL (triggered() ), dvda_author, SLOT (on_optionsButton_clicked() ) );
    helpAction = new QAction (tr ("&Help"), this);
    helpAction->setIcon (QIcon (":/images/help-contents.png") );
    connect (helpAction, SIGNAL (triggered() ), dvda_author, SLOT (on_helpButton_clicked() ) );
    displayAction = new QAction (tr ("&Show maximized/normal"), this);
    displayAction->setIcon (QIcon (":/images/show-maximized.png") );
    connect (displayAction, SIGNAL (triggered() ), this, SLOT (showMainWidget() ) );
    displayManagerAction = new QAction (tr ("Show/Close &Manager"), this);
    const QIcon iconViewList = QIcon (QString::fromUtf8 ( ":/images/view-list-tree.png") );
    displayManagerAction->setIcon (iconViewList);
    connect (displayManagerAction, SIGNAL (triggered() ), dvda_author, SLOT (on_openTreeWidgetButton_clicked() ) );
    exitAction = new QAction (tr ("&Exit"), this);
    exitAction->setIcon (QIcon (":/images/application-exit.png") );
    connect (exitAction, SIGNAL (triggered() ), this, SLOT (on_exitButton_clicked() ) );
    aboutAction = new QAction (tr ("&About"), this);
    aboutAction->setIcon (QIcon (":/images/about.png") );
    connect (aboutAction, SIGNAL (triggered() ), this, SLOT (about() ) );

    for (int i = 0; i < MaxRecentFiles ; ++i)
        {
            recentFileActions[i] = new QAction (this);
            recentFileActions[i]->setVisible (false);
            connect (recentFileActions[i], SIGNAL (triggered() ), this, SLOT (openProjectFile() ) );
        }
}

void MainWindow::openProjectFile()
{
    QString fullFileName = qobject_cast<QAction*> (sender() )->data().toString();

    if (!fullFileName.isEmpty() )
        {
            setCurrentFile (fullFileName);
            dvda_author->recentProjectFile (fullFileName);
        }
}

void MainWindow::on_exitButton_clicked()
{
    dvda_author->writeSettings();
    exit (1);
}


void MainWindow::createToolBars()
{
    fileToolBar = addToolBar (tr ("&File") );
    fileToolBar->setIconSize (QSize (22, 22) );
    editToolBar = addToolBar (tr ("&Edit") );
    editToolBar->setIconSize (QSize (22, 22) );
    processToolBar = addToolBar (tr ("&Process") );
    processToolBar->setIconSize (QSize (22, 22) );
    ioToolBar = addToolBar (tr ("&Data") );
    ioToolBar->setIconSize (QSize (22, 22) );
    optionsToolBar = addToolBar (tr ("&Options") );
    optionsToolBar->setIconSize (QSize (22, 22) );
    aboutToolBar = addToolBar (tr ("&Help") );
    aboutToolBar->setIconSize (QSize (22, 22) );
    fileToolBar->addAction (openAction);
    fileToolBar->addAction (saveAction);
    fileToolBar->addAction (exitAction);
    fileToolBar->addSeparator();
    editToolBar->addAction (displayAction);
    editToolBar->addAction (displayManagerAction);
    processToolBar->addAction (burnAction);
    processToolBar->addAction (encodeAction);
    processToolBar->addAction (encodeVideoAction);
    processToolBar->addAction (decodeAction);
    ioToolBar->addAction (inputAction);
    ioToolBar->addAction (videoAction);
    ioToolBar->addAction (outputAction);
    optionsToolBar->addAction (optionsAction);
    aboutToolBar->addAction (helpAction);
    aboutToolBar->addAction (aboutAction);
}



void MainWindow::showMainWidget()
{
    Qt::WindowStates full = this->windowState() ^ Qt::WindowFullScreen;
    setWindowState (full);

    if (full)
        {
            displayAction->setIcon (QIcon (":/images/show-normal.png") );
        }

    else
        {
            displayAction->setIcon (QIcon (":/images/show-maximized.png") );
        }
}
