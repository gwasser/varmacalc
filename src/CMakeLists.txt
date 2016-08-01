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

set(CPP_SOURCES
    main.cpp
    mainwindow.cpp
    finiteelementmodel.cpp
)

add_executable(varmacalc ${CPP_SOURCES})

target_link_libraries(varmacalc
    Qt5::Widgets
    KF5::UnitConversion
    KF5::Plotting
)

#install( TARGETS varmacalc ${INSTALL_TARGETS_DEFAULT_ARGS} )
