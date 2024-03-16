/**
  *  This file is part of VarmaCalc, Copyright (C)2014 Garret Wassermann.
  *  Portions of this software include libraries copyright
  *  the Eigen and KPlotting (KDE) teams.
  *
  *  VarmaCalc is free software: you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation, either version 3 of the License, or
  *  (at your option) any later version.
  *
  *  VarmaCalc is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with VarmaCalc.  If not, see <http://www.gnu.org/licenses/>.
  *
  */

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("VarmaCalc");
    a.setApplicationDisplayName("VarmaCalc");
    a.setApplicationVersion("0.0.3");

    MainWindow w;
    w.setWindowTitle("Home");
    w.show();

    return a.exec();
}
