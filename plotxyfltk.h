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


#ifndef PLOTFLTK_H
#define PLOTFLTK_H

#include <iostream>
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Menu_Item.H>

#include <boost/date_time.hpp>

#define NVERTICAL 5


using namespace std;
using namespace boost::posix_time;
/*
 * Classe inerente alla parte grafica
 */
class PlotxyFLTK: public Fl_Box {
    private:
        int trace_max;//max number of values
        int trace_min;//min number of values
        int view_width;//numbers showed

//         int trace_pos;
//         int view_pos;
        int view_break;
//         int offesetDraw;

        float *trace;//all value
        float *view;//showed values
        int insertsValues;//contains the values number acquired
        int initial_x, initial_y;

        //Insert element in tail
        void insertInTail(float element);

        //Shift vector on left
        void shift_vector_left(float element);

        //Translate Graph along y;
        void translateGraphY();

        //Function used by FLTK
        void draw();

        /***FL_MENU_ITEM FUNCTION***/
        //Zoom static function
        static void zoomInc(Fl_Widget *widget, void *userdata);
        static void zoomDec(Fl_Widget *widget, void *userdata);

        //Autoscale Function
        static void scale(Fl_Widget *widget, void *userdata);

        //Allow enable/disable Autoscale graph during plot
        static void autoScaleBehaviour(Fl_Widget *widget, void *userdata);

        /******/

        float timeOut;//plot Timeout. it used from updateScope funtion

        bool enableAutoScaleWhileGraph;

        // Scale,translate parameters
        float scale_factor_x;
        float scale_factor_y;
        int translate_value;
        float translate_x, translate_y;

        //Min max value along y axis
        float vievedMaxValue;
        float vievedMinValue;

        //Get  Min max value along y axis
        float getMaxValue(float, float);
        float getMinValue(float, float);

        //Axis Name
        string xAxis, yAxis;

        float intermidiateSecondsTag;
        float secondTag;//Use to move second tag (line to sign seconds)
        int time;
        Fl_Menu_Item *rclick_menu;

        ptime simulationTime;

        //Used to plot line into graph
        bool plotLineInGraph;
        float plotLineInGraphValue;




    protected:
        int handle(int e);

        // Draw mouse coords ant other things
        void drawCoordsAndOthers();

    public:
        PlotxyFLTK(int xp, int yp, int wp, int hp, const char *lp = 0);
        ~PlotxyFLTK();
        int insertValuesToPlot(float *value, int nvalue);
        void insertValueToPlot(float value);


        int getZoomXValue();
        int getZoomYValue();
        int getTranslateYValue();
        int getSimulationSeconds();

        void setAutoZoom(bool value);
        void setXAxis(char *name);
        void setYAxis(char *name);
        void setTraceMin(int value);
        void setTraceMax(int value);
        void setViedWidth(int value);


        void zoomXDec();
        void zoomXInc();
        void zoomYDec();
        void zoomYInc();
        void zoomYDecMouseWheel();
        void zoomYIncMouseWheel();
        void zoomAuto();
        void translateX();
        void translateYDown();
        void translateYUp();

        //plot line into graph
        void plotLine(float value);


};
// kate: indent-mode cstyle; space-indent on; indent-width 4; replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;
#endif
