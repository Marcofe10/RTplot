/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2012  <copyright holder> <email>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


#include "winplotxyfltk.h"

WinPlotXYFLTK::WinPlotXYFLTK(int X, int Y, int W, int H, const char* l): Fl_Window(X, Y, W, H, l) {
    int xw = X;
    int yw = Y;
    int ww = W - 100;
    int hw = H;
    this->plot = new PlotxyFLTK(xw, yw, ww, hw);
    //this->plot->show();
    this->zoomXMinus = new Fl_Button(ww + 10, 10, 20, 20, "X-");
    //this->zoomXMinus->callback()
    this->zoomXPlus = new Fl_Button(ww + 35, 10, 20, 20, "X+");
    this->zoomYMinus = new Fl_Button(ww + 10, 35, 20, 20, "Y-");
    this->zoomYPlus = new Fl_Button(ww + 35, 35, 20, 20, "Y+");
    this->show();
}

void WinPlotXYFLTK::draw()
{
    plot->redraw();
    Fl_Window::draw();
}


void WinPlotXYFLTK::insertValueToPlot(float value) {
    this->plot->insertValueToPlot(value);
    this->redraw();
}



// kate: indent-mode cstyle; space-indent on; indent-width 4; replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;
