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

#include "finiteelementmodel.h"
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/LU>
using Eigen::MatrixXd;
using Eigen::Matrix2d;
using Eigen::VectorXd;
using Eigen::Vector2d;

#include <KF5/KUnitConversion/KUnitConversion/Converter>
#include <KF5/KUnitConversion/KUnitConversion/Value>
using KUnitConversion::Value;
using KUnitConversion::Unit;

FiniteElementModel::FiniteElementModel()
{
    // finite element test

    // solve k*T'' + q = r*c*Tt on 0.05 < x < 0.25,
    // with BC T(0.05) = 400 K and T(0.25) = 600 K

    param.a = Value(0.0,"m"); // beginning of interval to solve
    param.b = Value(2.0,"m"); // end of interval to solve
    param.bc_a = Value(700.0,"K"); // boundary condition at x=a
    param.bc_b = Value(300.0, "K"); // boundary condition at x=b

    param.k = Value(2.0,"W/mK"); // W/mK
    param.q = Value(1000.0, "W/m^3"); // W/m^3

    param.n = 6; // number of nodes used
    param.dx = Value( (param.b.number() - param.a.number())/(param.n-1), "m"); // step value

    // set default coordinate system to be cartesian and SI units
    modelCoord = CoordType::CARTESIAN;
    modelUnits = UnitSystem::SI;

}

FiniteElementSolution FiniteElementModel::findNodalSolution()
{
    // local matrices for linear element, derived by hand

    double k = param.k.number();
    double q = param.q.number();
    double dx = param.dx.number();
    double a = param.a.number();
    //double b = param.b.number();
    double bc_a = param.bc_a.number();
    double bc_b = param.bc_b.number();
    int n = param.n;

    Matrix2d ke;
    ke << 1, -1,
         -1, 1;
    ke = (k/dx)*ke;

    Matrix2d me;
    if (modelCoord == CoordType::CYLINDRICAL)
    {
        me << 1, -1,
             -1, 1;
        me = (k/2.0)*me;
    }
    else
    {
        // make 0s if not using cylindrical coords
        me << 0.0, 0.0,
              0.0, 0.0;
    }

    Vector2d fe;
    fe << 1,1;
    fe = (q*dx/2.0)*fe;

    // build global matrices

    MatrixXd K = MatrixXd::Zero(n,n);
    VectorXd F = VectorXd::Zero(n);
    for (int i = 0; i < n-1; i++)
    {
        K(i,i) = K(i,i) + ke(0,0) + me(0,0);
        K(i,i+1) = K(i,i+1) + ke(0,1) + me(0,1);
        K(i+1,i) = K(i+1,i) + ke(1,0) + me(1,0);
        K(i+1,i+1) = K(i+1,i+1) + ke(1,1) + me(1,1);
        F(i) = F(i) + fe(0);
        F(i+1) = F(i+1) + fe(1);
    }

    // solve for missing nodes

    VectorXd answer = VectorXd::Zero(n-2); // two less since ignore values at ends of interval
    MatrixXd Ki = K.block(1,1,n-2,n-2); //sub-matrix starting at (1,1) and going 2 elements over
    VectorXd Fi = F.segment(1,n-2);
    // subtract off values at boundaries
    for (int i = 0; i < n-2; i++)
    {
        Fi(i) = Fi(i) - K(1+i,0)*bc_a - K(1+i,n-1)*bc_b;
    }
    //solve using Householder QR decomposition
    answer = Ki.fullPivHouseholderQr().solve(Fi);

    // Post-Processing -- go back and get boundary conditions for derivatives

    // set new nodes values into node vector
    VectorXd nodes = VectorXd::Zero(n);
    nodes(0) = bc_a;
    for (int i = 1; i < n-1; i++)
    {
        nodes(i) = answer(i-1);
    }
    nodes(n-1) = bc_b;

    // now solve for BC vector:

    VectorXd bcvec = VectorXd::Zero(n);
    bcvec = K * nodes - F;

    // make vect of x values for each node
    VectorXd xvals = VectorXd::Zero(n);
    for (int i = 0; i < n; i++)
    {
        xvals(i) = a + i*dx;
    }

    // build solution struct
    FiniteElementSolution fes;
    fes.boundaryValues = bcvec;
    fes.nodalSolution = nodes;
    fes.nodalXVals = xvals;

    return fes;
}


//typical setter functions here:

void FiniteElementModel::set_n(int new_n)
{
    param.n = new_n;
    //QString oldunit = param.dx.unit()->symbol(); // update value without changing unit
    QString oldunit = param.dx.unit().symbol(); // update value without changing unit
    param.dx = Value( (param.b.number() - param.a.number())/(param.n-1), oldunit); // step value
}

void FiniteElementModel::set_a(double new_a)
{
    param.a = Value(new_a, "m");
    QString oldunit = param.dx.unit().symbol(); // update value without changing unit
    param.dx = Value( (param.b.number() - param.a.number())/(param.n-1), oldunit); // step value
}

void FiniteElementModel::set_b(double new_b)
{
    param.b = Value(new_b, "m");
    QString oldunit = param.dx.unit().symbol(); // update value without changing unit
    param.dx = Value( (param.b.number() - param.a.number())/(param.n-1), oldunit); // step value
}

void FiniteElementModel::set_bc_a(double new_bc_a)
{
    param.bc_a = Value(new_bc_a, "K");
}

void FiniteElementModel::set_bc_b(double new_bc_b)
{
    param.bc_b = Value(new_bc_b, "K");
}

void FiniteElementModel::set_k(double new_k)
{
    param.k = Value(new_k, "W/mK");
}

void FiniteElementModel::set_q(double new_q)
{
    param.q = Value(new_q, "W/m^3");
}

void FiniteElementModel::set_coord( CoordType new_coord )
{
    modelCoord = new_coord;
}

void FiniteElementModel::set_unit_sys( UnitSystem new_units )
{
    modelUnits = new_units;
    if (modelUnits == UnitSystem::ENGLISH)
    {
        param.a = param.a.convertTo("ft");
        param.b = param.b.convertTo("ft");
        param.bc_a = param.bc_a.convertTo("F");
        param.bc_b = param.bc_b.convertTo("F");
        param.k = param.k.convertTo("Btu/ft-hr-F");
        param.q = param.q.convertTo("Btu/hr/ft^3");
    }
    else // default is SI
    {
        param.a = param.a.convertTo("m");
        param.b = param.b.convertTo("m");
        param.bc_a = param.bc_a.convertTo("K");
        param.bc_b = param.bc_b.convertTo("K");
        param.k = param.k.convertTo("W/mK");
        param.q = param.q.convertTo("W/m^3");
    }
}


//getter functions here:

int FiniteElementModel::get_n()
{
    return param.n;
}

double FiniteElementModel::get_a()
{
    return param.a.number();
}

QString FiniteElementModel::get_unit_a()
{
    return param.a.unit().symbol();
}

double FiniteElementModel::get_b()
{
    return param.b.number();
}

QString FiniteElementModel::get_unit_b()
{
    return param.b.unit().symbol();
}

double FiniteElementModel::get_bc_a()
{
    return param.bc_a.number();
}

QString FiniteElementModel::get_unit_bc_a()
{
    return param.bc_a.unit().symbol();
}

double FiniteElementModel::get_bc_b()
{
    return param.bc_b.number();
}

QString FiniteElementModel::get_unit_bc_b()
{
    return param.bc_b.unit().symbol();
}

double FiniteElementModel::get_k()
{
    return param.k.number();
}

QString FiniteElementModel::get_unit_k()
{
    return param.k.unit().symbol();
}

double FiniteElementModel::get_q()
{
    return param.q.number();
}

QString FiniteElementModel::get_unit_q()
{
    return param.q.unit().symbol();
}

CoordType FiniteElementModel::get_coord()
{
    return modelCoord;
}
