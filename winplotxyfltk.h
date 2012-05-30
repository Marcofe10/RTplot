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


#ifndef WINPLOTXYFLTK_H
#define WINPLOTXYFLTK_H

#include "plotxyfltk.h"
#include "FL/Fl_Double_Window.H"
#include "FL/Fl_Button.H"
#include "FL/Fl_Radio_Button.H"

class WinPlotXYFLTK: public Fl_Window {
    private:

        PlotxyFLTK *plot;
        Fl_Button *buttonZoomXInc, *buttonZoomXDec, *buttonZoomYInc, *buttonZoomYDec;
        Fl_Label *labelZoomX, *labelZoomY;
        Fl_Radio_Button *radioAutoScale;

        static void zoomXInc(Fl_Widget *widget, void *userdata);
        static void zoomXDec(Fl_Widget *widget, void *userdata);
        static void zoomYInc(Fl_Widget *widget, void *userdata);
        static void zoomYDec(Fl_Widget *widget, void *userdata);
        static void zoomAuto(Fl_Widget *widget, void *userdata);

//         void draw();
    public:
        WinPlotXYFLTK(int X, int Y, int W, int H, const char* l);
        int insertValuesToPlot(float *value, int nvalue);
        void insertValueToPlot(float value);
        void setXAxis(char *name);
        void setYAxis(char *name);

        //plot line into graph
        void plotLine();
    protected:

};

#endif // WINPLOTXYFLTK_H
// kate: indent-mode cstyle; space-indent on; indent-width 4; replace-tabs on; 
