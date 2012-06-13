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

#define BUTTON_SIZE 95

#include "plotxyfltk.h"
#include "FL/Fl_Double_Window.H"
#include "FL/Fl_Button.H"
#include "FL/Fl_Check_Button.H"
#include "FL/Fl_Value_Output.H"

class WinPlotXYFLTK: public Fl_Double_Window {
    private:

        PlotxyFLTK *plot;
        
        Fl_Button *buttonZoomXInc, *buttonZoomXDec, *buttonZoomYInc, *buttonZoomYDec, *buttonTranslateYUp;
        Fl_Button *buttonTranslateYDown;
        
        Fl_Check_Button *checkButtonAutoScale,*checkButtonFullScreen;;
        Fl_Value_Output *valueOutputZoomX, *valueOutputZoomY, *valueOutputTranslateY,*valueOutputTimeSimulations;
        
        int X,Y,W,H;

        static void zoomXInc(Fl_Widget *widget, void *userdata);
        static void zoomXDec(Fl_Widget *widget, void *userdata);
        static void zoomYInc(Fl_Widget *widget, void *userdata);
        static void zoomYDec(Fl_Widget *widget, void *userdata);
        static void zoomAuto(Fl_Widget *widget, void *userdata);
        static void translateX(Fl_Widget *widget, void *userdata);
        static void translateYDown(Fl_Widget *widget, void *userdata);
        static void translateYUp(Fl_Widget *widget, void *userdata);
        static void fullScreen(Fl_Widget *widget, void *userdata);
        void draw();

    public:
        WinPlotXYFLTK(int X, int Y, int W, int H, const char* l);
        int insertValuesToPlot(float* value, int nvalue,int trace_min=512);
        void insertValueToPlot(float value);
        
        void setXAxis(char *name);
        void setYAxis(char *name);
        void setAutoZoom(bool value);
        void setViedWidth(int value);
        void setTraceMin(int value);
        
        
        int getSimulationSeconds();


        //plot line into graph
        void plotLine(float value);
    protected:

};

#endif // WINPLOTXYFLTK_H
