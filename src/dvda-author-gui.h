/*

dvda-author-gui.h  - Author a DVD-Audio DVD with dvda-author

This application uses Qt4.4 . Check Qt's licensing details on http://trolltech.com

Class  dvda:  Main central widget
Class  DomParser: XML parser for dvda-author-gui projects (*.dvp)
Class  options: options dialog window.

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




#ifndef DIRECTORYVIEWER_H
#define DIRECTORYVIEWER_H

#include <QMainWindow>

#include <QDialog>
#include <QProcess>
#include <QtGui/QLabel>
#include <QtGui/QToolButton>
#include <QtGui/QLayout>
#include <QtGui/QProgressBar>
#include <QListWidget>
#include <QFileSystemModel>
#ifdef QT_FILE_DIALOG
#include <QFileDialog>
#endif

#include <QCheckBox>
#include <QTreeWidget>

#ifndef VERSION
#define VERSION  "09.02"
#endif

/* Build with make for non-local HTML doc and with make HTMLDIR=-DLOCAL for a local webpage */

#ifndef LOCAL
#define HTMLDIR "/usr/local/share/dvda-author-gui-" VERSION
#else
#define HTMLDIR "."
#endif

class QAction;
class QDirModel;
class QPushButton;
class QTreeView;

class QProcess;
class QCheckBox;
class QTextEdit;
class QListWidget;
class QTabWidget;
class QDomElement;
class QIODevice;
class QTreeWidget;
class QTreeWidgetItem;
class dvda;

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public :
        MainWindow();

        void setCurrentFile (const QString& fileName);
        enum { MaxRecentFiles = 5 };
        bool hasRecentFiles[MaxRecentFiles];
        QStringList recentFiles;
        void updateRecentFileActions();

    protected :

        void dragEnterEvent (QDragEnterEvent* event);
        void dropEvent (QDropEvent* event);

    private slots:

        void on_exitButton_clicked();
        void showMainWidget();
        void openProjectFile();
        void about();

    private :

        bool readFile (const QString& fileName);
        dvda* dvda_author;
        void createActions();
        void createMenus();
        void createToolBars();
        void loadFile (const QString& fileName);

        QString strippedName (const QString& fullFuleName);

        QString curFile;

        QMenu* fileMenu;
        QMenu* processMenu;
        QMenu* ioMenu;
        QMenu* editMenu;
        QMenu* optionsMenu;
        QMenu* aboutMenu;

        QToolBar* fileToolBar;
        QToolBar* processToolBar;
        QToolBar* ioToolBar;
        QToolBar* editToolBar;
        QToolBar* optionsToolBar;
        QToolBar* aboutToolBar;

        QAction* recentFileActions[MaxRecentFiles];
        QAction* separatorAction;
        QAction* openAction;
        QAction* saveAction;
        QAction* burnAction;
        QAction* encodeAction;
        QAction* encodeVideoAction;
        QAction* decodeAction;
        QAction* inputAction;
        QAction* outputAction;
        QAction* videoAction;
        QAction* aboutAction;
        QAction* optionsAction;
        QAction* exitAction;
        QAction* helpAction;
        QAction* displayAction;
        QAction* displayManagerAction;

        //dvda dvda_author(QWidget *parent);


};


class options : public QDialog
{
        Q_OBJECT

    public:

        options (dvda* parent);
        QString mkisofsPath, logPath;
        bool log, runMkisofs;
        bool sox;
        bool debug;
        bool videozone;
        bool audiozone;
        bool burnDisc;
        QString startsector;
        QString dvdwriterPath;
        bool hasVideozone;
        bool hasAudiozone;

    signals:

        void defaultClick (bool);

    private slots:
#ifdef QT_FILE_DIALOG
        void on_logButton_clicked();
        void on_mkisofsButton_clicked();
#endif        
        void on_logBox_checked();
        void on_mkisofsBox_checked();

        void on_soxBox_checked();
        void on_debugBox_checked();

        void on_startsectorLineEdit_changed (const QString& startsectorValue);
        void on_dvdwriterLineEdit_changed (const QString& dvdwriterValue);
        void on_cdrecordBox_checked();

        void on_audiozoneBox_checked();
        void on_videozoneBox_checked();
        void on_hasVideozoneBox_checked();
        void on_hasAudiozoneBox_checked();

    private:

        QPushButton* okButton, *logButton, *mkisofsButton;
        QCheckBox*   mkisofsBox, *logBox;
#ifdef        QT_FILE_DIALOG
        QFileDialog logDialog;
#endif        
        QVBoxLayout* optionsLayout;
        QLabel*      startsectorLabel;
        QLineEdit*   startsectorLineEdit, *dvdwriterLineEdit;

        QCheckBox* debugBox;
        QCheckBox* videozoneBox;
        QCheckBox* audiozoneBox;
        QCheckBox* soxBox;
        QCheckBox* cdrecordBox;
        QCheckBox* hasVideozoneBox;
        QCheckBox* hasAudiozoneBox;

        void checkZones();

};


class dvda : public QDialog
{
        Q_OBJECT

    public:

        dvda (QWidget* parent);
        void setOutputTextEdit (QString filename);
        void writeSettings();
        MainWindow* parent;
        enum { MaxRecentFiles = 5 };
        void addDraggedFiles (QList<QUrl> urls);
        void recentProjectFile (const QString& projectFile);
        QTabWidget* tab2Widget;
        QTabWidget* tabWidget;
        QString tempdir;

    public slots:

        void on_hasVideozoneBox_toggled (bool);
        void on_hasAudiozoneBox_toggled (bool);


    private slots:

        void selectInput();
        void selectVideo();
        void on_rightButton_clicked();
        void on_moveUpItemButton_clicked();
        void on_moveDownItemButton_clicked();
        void on_retrieveItemButton_clicked();
        bool on_optionsButton_clicked();

        void on_cdrecordButton_clicked();
        void on_openTreeWidgetButton_clicked();

        void extract();
        void createDirectory();
        void remove();
        void run();
        void runLplex();
        void processFinished (int exitCode, QProcess::ExitStatus exitStatus);
        void processLplexFinished (int exitCode, QProcess::ExitStatus exitStatus);
        void process2Finished (int exitCode,  QProcess::ExitStatus exitStatus);
        void process3Finished (int exitCode,  QProcess::ExitStatus exitStatus);
        void addGroup();
        void deleteGroup();
        void killMkisofs();
        void killDvda();
        void killCdrecord();
        void on_helpButton_clicked();
#ifdef QT_FILE_DIALOG        
        void saveProject();
        void on_openProjectButton_clicked();
#endif        
        bool parseFile (const QString fileName);
        void assignGroupFiles (int group_index, QString file);
        void assignVariables (QString& variable, QString& value);
        void runMkisofs();

    private:

        QIcon iconShowMaximized;
        QIcon iconShowNormal;
        QTreeView* treeView;

        QFileSystemModel* model;

        QToolButton* mkdirButton;
        QToolButton* removeButton;
        QToolButton* rightButton;
        QToolButton* moveUpItemButton;
        QToolButton* moveDownItemButton;
        QToolButton* retrieveItemButton;
        QToolButton* addGroupButton;
        QToolButton* deleteGroupButton;
        QToolButton* killMkisofsButton;
        QToolButton* killButton;
        QToolButton* killCdrecordButton;

        QProcess process, process2, process3;
        QProcess processLplex;
        QProcess helpProcess;

        QCheckBox*  debugCheckBox;

        QTabWidget* mainTabWidget;

        QVBoxLayout* mainLayout;
        QVBoxLayout* progressLayout;
        QVBoxLayout* managerLayout;
        QHBoxLayout* allLayout;
        QProgressBar* progress, *progress2, *progress3;

        bool startProgressBar, startProgressBar2, startProgressBar3;

        qint64 inputSizeCount;
        qint64 inputSize[9];
        qint64 inputTotalSize;
        qint64 value;


        int currentIndex;
        int currentMainTabIndex;
        int currentIndexMax;

        void initialize();
        qint64 scanDirectory (const QString& path, const QStringList& filters);
        bool removeDirectory (const QString& path);
        void addFileToProject (QListWidget* project, int& currentIndex, int&  currentMainTabIndex);
        float discShare (qint64 directorySize);
        void readSettings();
        void remove (const QString& path);
        void selectOutput (const QString& path = "", bool = true);
        options* dialog;
        void run_dvda(bool modify_command_line);

    protected:

        QTextEdit*  outputTextEdit;
        QTreeWidget* treeWidget;
        QListWidget* project[9];
        QListWidget* project2[99];

        QString     sourceDir;
        QString     targetDir;
        QString     videoDir;
        QString     selectedFile[100];
        QString     g_command_line;
        QString     outputType;

        int rank;
        int video_rank;
        int maxRange;
        int videoTitleRank;

        bool g_syntax_enabled[9];
        bool video_syntax_enabled[99];
        bool i_syntax_enabled;
        bool o_syntax_enabled;
        bool V_syntax_enabled;

        bool extractSwitch;
        bool project_manager_enabled;
};

class DomParser : public dvda
{
        Q_OBJECT

    public:
        DomParser (QIODevice* device, QTreeWidget* tree, dvda* parent);

    signals:

        void parseGroup (int, QString );
        void parseVariables (QString&, QString&);

    private:
        void parseEntry (const QDomElement& element, QTreeWidgetItem* parent);
        //dvda *mainWidget ;
};





#endif
