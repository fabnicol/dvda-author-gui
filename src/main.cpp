/*

main.cpp  - Author a DVD-Audio DVD with dvda-author

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



#include <QApplication>

#include "dvda-author-gui.h"

int main (int argc, char* argv[])
{
    QApplication app (argc, argv);
//    dvda dvda_author(0);
    //  dvda_author.show();
    MainWindow* mainWin = new MainWindow;
    mainWin->show();
    mainWin->setFont(QFont("Verdana"));
    mainWin->setWindowTitle ("dvd-author GUI");
    return app.exec();
}
