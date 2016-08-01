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

#ifndef FINITEELEMENTMODEL_H
#define FINITEELEMENTMODEL_H

#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/LU>
using Eigen::MatrixXd;
using Eigen::Matrix2d;
using Eigen::VectorXd;
using Eigen::Vector2d;

#include <KF5/KUnitConversion/KUnitConversion/Converter>
#include <KF5/KUnitConversion/KUnitConversion/Value>
using KUnitConversion::Value;

// define a struct to hold info about solution to
// the finite element problem
typedef struct
{
    VectorXd nodalSolution;
    VectorXd nodalXVals;
    VectorXd boundaryValues;
}
FiniteElementSolution;

// structure to hold matrices to use for solution
typedef struct
{
    Value a; // beginning of interval to solve
    Value b; // end of interval to solve
    Value bc_a; // boundary condition at x=a
    Value bc_b; // boundary condition at x=b

    Value k; // W/mK
    Value q; // W/m^3

    int n; // number of nodes used
    Value dx; // step value
}
FiniteElementParameters;

// define types of Coordinates available to solver
enum class CoordType
{
    CARTESIAN, CYLINDRICAL
};

// define which unit system currently using
enum class UnitSystem
{
    SI, ENGLISH
};

// define new class for finite element model
class FiniteElementModel
{

public:
    FiniteElementModel();
    void set_a( double new_a );
    void set_b( double new_b );
    void set_bc_a( double new_bc_a );
    void set_bc_b( double new_bc_b );
    void set_k( double new_k );
    void set_q( double new_q );
    void set_n( int new_n );
    void set_coord( CoordType new_coord );
    void set_unit_sys( UnitSystem new_units );
    int get_n();
    double get_a();
    QString get_unit_a();
    double get_b();
    QString get_unit_b();
    double get_bc_a();
    QString get_unit_bc_a();
    double get_bc_b();
    QString get_unit_bc_b();
    double get_k();
    QString get_unit_k();
    double get_q();
    QString get_unit_q();
    CoordType get_coord();
    FiniteElementSolution findNodalSolution();

public:

    CoordType modelCoord; // keep track of which coordinate system model is using
    UnitSystem modelUnits; // keep track of unit system
    FiniteElementParameters param; //keep track of FEM parameters
};

#endif // FINITEELEMENTMODEL_H
