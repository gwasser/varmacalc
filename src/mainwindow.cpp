
/**
  *  This file is part of VarmaCalc, Copyright (C)2014 Garret Wassermann.
  *  Portions of this software include libraries copyright
  *  the Eigen and KDE teams.
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
#include "finiteelementmodel.h"

#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/LU>
using Eigen::MatrixXd;
using Eigen::Matrix2d;
using Eigen::VectorXd;
using Eigen::Vector2d;

#include <math.h>
#include <QApplication>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QBoxLayout>
#include <QFormLayout>
#include <QPen>


#include <KF5/KPlotting/KPlotObject>
#include <KF5/KPlotting/KPlotWidget>
#include <KF5/KPlotting/KPlotAxis>
#include <KF5/KUnitConversion/KUnitConversion/Converter>
#include <KF5/KUnitConversion/KUnitConversion/Value>
using KUnitConversion::Value;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    QWidget *w = new QWidget(this);
    QHBoxLayout *hlay = new QHBoxLayout(w);
    QVBoxLayout *vlay = new QVBoxLayout();

    QLabel *lblTitle = new QLabel(w);
    lblTitle->setTextFormat(Qt::RichText);
    lblTitle->setText( "<h1>"+ QApplication::applicationName() + "</h1><h2>"
                       + QApplication::applicationVersion() + "</h2>" );

    model = new FiniteElementModel();

    plot = new KPlotWidget(w);
    plot->setMinimumSize(500, 500);
    plot->setAntialiasing(true);
    plot->axis(KPlotWidget::BottomAxis)->setLabel("Position");
    plot->axis(KPlotWidget::LeftAxis)->setLabel("Temperature");
    po1 = new KPlotObject(Qt::cyan,  KPlotObject::Lines, 2);
    po2 = new KPlotObject(Qt::red,  KPlotObject::Lines, 2);
    po3 = new KPlotObject(Qt::yellow,  KPlotObject::Lines, 2);

    unitSystemSelector = new QComboBox(w);
    unitSystemSelector->addItem("SI (Metric)");
    unitSystemSelector->addItem("US/English");
    connect(unitSystemSelector, &QComboBox::currentTextChanged, this, &MainWindow::updateUnitSystem);

    geometrySelector = new QComboBox(w);
    geometrySelector->addItem("Cartesian");
    geometrySelector->addItem("Cylindrical");
    connect(geometrySelector, &QComboBox::currentTextChanged, this, &MainWindow::updateCoordSystem);

    elemGeometrySelector = new QComboBox(w);
    elemGeometrySelector->addItem("Linear");
    elemGeometrySelector->addItem("Quadratic");
    connect(elemGeometrySelector, &QComboBox::currentTextChanged, this, &MainWindow::updateCoordSystem);

    // take defaults from model and put into text boxes
    editNumberElements = new QLineEdit();
    editNumberElements->setText( QString::number(model->get_n()) );
    editValA = new QLineEdit();
    editValA->setText( QString::number(model->get_a()) );
    editValB = new QLineEdit();
    editValB->setText( QString::number(model->get_b()) );
    editValBCA = new QLineEdit();
    editValBCA->setText( QString::number(model->get_bc_a()) );
    editValBCB = new QLineEdit();
    editValBCB->setText( QString::number(model->get_bc_b()) );
    editValK = new QLineEdit();
    editValK->setText( QString::number(model->get_k()) );
    editValQ = new QLineEdit();
    editValQ->setText( QString::number(model->get_q()) );
    // set up grid of info to set values for finite element model
    QFormLayout *numElemLayout = new QFormLayout();
    numElemLayout->addRow(tr("Unit System:"), unitSystemSelector);
    numElemLayout->addRow(tr("Geometry Used:"), geometrySelector);
    numElemLayout->addRow(tr("Element Geometry:"), elemGeometrySelector);
    numElemLayout->addRow(tr("&Number of Elements Used:"), editNumberElements);
    numElemLayout->addRow(tr("Interval Start") + " [" + model->get_unit_a() + "]", editValA);
    numElemLayout->addRow(tr("Interval End") + " [" + model->get_unit_b() + "]", editValB);
    numElemLayout->addRow(tr("Boundary Value at Interval Start") + " [" + model->get_unit_bc_a() + "]", editValBCA);
    numElemLayout->addRow(tr("Boundary Value at Interval End") + " [" + model->get_unit_bc_b() + "]", editValBCB);
    numElemLayout->addRow(tr("Thermal Conductivity") + " [" + model->get_unit_k() + "]", editValK);
    numElemLayout->addRow(tr("Heat Generation Rate") + " [" + model->get_unit_q() + "]", editValQ);

    btnUpdateGraph = new QPushButton(w);
    btnUpdateGraph->setText("Update Graph");
    connect(btnUpdateGraph, &QPushButton::clicked, this, &MainWindow::updateGraph);

    QPushButton *btnSavePlot = new QPushButton(w);
    btnSavePlot->setText("Save Current Plot to PNG");
    connect(btnSavePlot, &QPushButton::clicked, this, &MainWindow::savePlot);

    chkShowAnalyticSolution = new QCheckBox(w);
    chkShowAnalyticSolution->setText("Show the Real Solution");
    connect(chkShowAnalyticSolution, &QCheckBox::stateChanged, this, &MainWindow::updateAnalyticalGraph);

    vlay->addWidget(lblTitle);
    vlay->addLayout(numElemLayout);
    vlay->addWidget(chkShowAnalyticSolution);
    vlay->addWidget(btnUpdateGraph);
    vlay->addWidget(btnSavePlot);

    hlay->addLayout(vlay);
    hlay->addWidget(plot);

    setCentralWidget(w);

    updateGraph();
}

MainWindow::~MainWindow()
{

}

void MainWindow::updateGraph()
{
    // set value and solve new finite element system
    model->set_n( editNumberElements->text().toInt() );
    model->set_a( editValA->text().toDouble() );
    model->set_b( editValB->text().toDouble() );
    model->set_bc_a( editValBCA->text().toDouble() );
    model->set_bc_b( editValBCB->text().toDouble() );
    model->set_k( editValK->text().toDouble() );
    model->set_q( editValQ->text().toDouble() );
    // calculate new solution
    FiniteElementSolution sol =  model->findNodalSolution();
    int n = model->get_n();
    double a = model->get_a();
    double b = model->get_b();
    plot->setLimits( a- 0.05, b+0.05, 250, 850);

    // create new plot
    po2->clearPoints();
    for (int i = 0; i < n; i++)
    {
        po2->addPoint( sol.nodalXVals(i), sol.nodalSolution(i) );
    }

    plot->addPlotObject(po2);

    // make sure the analytical solution is also updated
    updateAnalyticalGraph();

    plot->update();

}

void MainWindow::updateAnalyticalGraph()
{
    if (chkShowAnalyticSolution->isChecked())
    {
        // get values from model
        double a = model->get_a();
        double b = model->get_b();
        double bc_a = model->get_bc_a();
        double bc_b = model->get_bc_b();
        double q = model->get_q();
        double k = model->get_k();

        // constants for analytical solution
        double C1 = 0.0;
        double C2 = 0.0;
        if (model->get_coord() == CoordType::CARTESIAN)
        {
            C1 = ( (bc_b-bc_a)-(q/(2*k))*(a*a - b*b) )/(b-a);
            C2 = bc_a + (q/(2*k))*a*a - C1*a;
        }
        else if (model->get_coord() == CoordType::CYLINDRICAL)
        {
            C1 = 0.0;
            C2 = bc_b + q/(4.0*k)*b*b;
        }


        po3->clearPoints();
        for (double x = a-0.05; x <= b+0.05; x += 0.01) {
            // analytical solution for steady state, constant uniform heat gen
            if (model->get_coord() == CoordType::CARTESIAN)
            {
                po3->addPoint( x, (-1*q)/(2*k)*x*x + C1*x + C2  );
            }
            else
            {
                po3->addPoint( x, -q/(4.0*k)*x*x + C1 + C2);
            }
        }
        plot->addPlotObject(po3);
        plot->update();
    }
    else
    {
        // just clear points and show nothing
        po3->clearPoints();
        plot->update();
    }

}

void MainWindow::updateCoordSystem(QString currentCoordText)
{
    if (currentCoordText == "Cylindrical")
    {
        model->set_coord( CoordType::CYLINDRICAL );
        plot->axis(KPlotWidget::BottomAxis)->setLabel("Radius");
    }
    else
    {
        model->set_coord( CoordType::CARTESIAN );
        plot->axis(KPlotWidget::BottomAxis)->setLabel("Position");
    }
    plot->update();
}

void MainWindow::updateUnitSystem(QString currentUnitText)
{
    if (currentUnitText == "US/English")
    {
        model->set_unit_sys( UnitSystem::ENGLISH );
    }
    else // default is SI
    {
        model->set_unit_sys( UnitSystem::SI);
    }
    // update text boxes to show new values in new units
    editNumberElements->setText( QString::number( model->get_n() ) );
    editValA->setText( QString::number( model->get_a() ) );
    editValB->setText( QString::number( model->get_b() ) );
    editValBCA->setText( QString::number( model->get_bc_a() ) );
    editValBCB->setText( QString::number( model->get_bc_b() ) );
    editValK->setText( QString::number( model->get_k() ) );
    editValQ->setText( QString::number( model->get_q() ) );
}

void MainWindow::savePlot()
{
    //saves plot widget as a PNG file
    plot->grab().save("plot.png");
}
