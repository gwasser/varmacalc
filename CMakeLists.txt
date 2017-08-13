#***************************************************************************
#*   Copyright (C) 2016 Garret Wassermann                                  *
#*                                                                         *
#*   This program is free software; you can redistribute it and/or modify  *
#*   it under the terms of the GNU General Public License version 3        *
#*   as published by the Free Software Foundation.                         *
#*                                                                         *
#*   A copy of the full license is included in the COPYING file            *
#*   in the main folder of this source code distribution.                  *
#*                                                                         *
#*   This program is distributed in the hope that it will be useful,       *
#*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
#*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
#*   GNU General Public License for more details.                          *
#*                                                                         *
#*   You should have received a copy of the GNU General Public License     *
#*   along with this program; if not, write to the                         *
#*   Free Software Foundation, Inc.,                                       *
#*   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA            *
#***************************************************************************/

project(varmacalc)

cmake_minimum_required(VERSION 3.0)
set(QT_MIN_VERSION "5.6.0")
# KF5 5.28.0 includes the thermal units in upstream
set(KF5_MIN_VERSION "5.28.0")

# Locate extra-cmake-modules and make it a required package
find_package(ECM 1.0.0 REQUIRED NO_MODULE)

# Set value of CMAKE_MODULE_PATH variable where cmake will search for modules
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# include KF5 cmake modules
include(ECMInstallIcons)
include(KDEInstallDirs)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(FeatureSummary)

# Find Qt5 modules
find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
    Core
    Gui
    Widgets
)

# Find KDE modules
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    UnitConversion
    Plotting
)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

add_subdirectory(src)



# kate: indent-width 4; replace-tabs on;
