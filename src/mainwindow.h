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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class QComboBox;
class QLineEdit;
class QLabel;
class QPushButton;
class QCheckBox;
class QVBoxLayout;
class KPlotWidget;
class KPlotObject;

#include "finiteelementmodel.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void updateGraph();
    void updateAnalyticalGraph();
    void updateCoordSystem(QString currentCoordText);
    void updateUnitSystem(QString currentUnitText);
    void savePlot();

private:
    //QVBoxLayout *vlay;
    QComboBox *unitSystemSelector;
    QComboBox *geometrySelector;
    QComboBox *elemGeometrySelector;
    QLineEdit *editNumberElements;
    QLineEdit *editValA;
    QLineEdit *editValB;
    QLineEdit *editValBCA;
    QLineEdit *editValBCB;
    QLineEdit *editValK;
    QLineEdit *editValQ;
    QPushButton *btnUpdateGraph;
    QCheckBox *chkShowAnalyticSolution;
    FiniteElementModel *model;
    KPlotWidget *plot;
    KPlotObject *po1, *po2, *po3;
};

#endif // MAINWINDOW_H
