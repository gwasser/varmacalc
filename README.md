# VarmaCalc #

## Description ##

VarmaCalc displays the approximate finite element solution for
some heat transfer scenarios. You can also compare it
to the analytical solution.

This is really just a demo of using Qt/KF5 to
make scientific software. If I have more time,
I'd like to spiff it up. We'll see. But
maybe it will be useful in its current form to someone.

## Building ##

VarmaCalc is primarily written in C++ using Qt5 with the KDE
Frameworks 5 libraries.

More specifically, you need:
* Qt5
* Eigen >= 3.2.2  (MPL2 license)
* KPlotting 5.24.0  (LGPLv3 license)
* KUnitConversion 5.24.0 (LGPLv3 license)

Currently, KUnitConversion does not contain heat transfer units.
I've submitted them for inclusion in a future release,
so until then, the units don't work for the thermodynamic units.
You will get funny errors going back and forth between
metric and english units, for example.


## Licensing ##

VarmaCalc - a finite element modeling software
Copyright (C) 2014 Garret Wassermann

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
