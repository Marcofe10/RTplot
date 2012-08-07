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
#include <boost/circular_buffer.hpp>

#define NVERTICAL 10


using namespace std;
using namespace boost::posix_time;

typedef struct _data_element {
    float value;
    float time;
}data_element;

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
        
         //Calculates the Min and max value along y axis
        float findMaxValue(float val, float current_max);
        float findMinValue(float val, float current_min);

        //Get max and min value in a window of values
        void getMaxMinWindowValue();
        
        //scale and translate along y
        void scaleAndTranslateY();

        /******/

        float timeOut;//plot Timeout. it used from updateScope funtion

        bool enableAutoScaleWhileGraph;

        // Scale,translate parameters
        float scale_factor_x;
        float scale_factor_y;
        int translate_value;
        float translate_x, translate_y;

        //The Vieved min and max value along y axis
        float vievedMaxValue;
        float vievedMinValue;

        //the min and max value along y axis
        float maxValue;
        float minValue;


        //Axis Name
        string xAxis, yAxis;

        float intermidiateSecondsTag;
        float secondTag;//Use to move second tag (line to sign seconds)
//         int incTagAxesValue;//Used to change range of X axes
        int samplePerSecond;//Sample per second. Use to plot secondtag in relationship of view_width
        int sampleTime;//Sample time (at the moment it's similar to samplePerSecond)
        Fl_Menu_Item *rclick_menu;

        float time;//Simulation time calculated by sample
        int residueTime;

        ptime simulationTime;

        //Used to plot line into graph
        bool plotLineInGraph;
        float plotLineInGraphValue;


        //Contains element
        boost::circular_buffer<data_element> *dataCB;

        //Common operation in insertValues e insertValue
        int commonInsertValue(float value, float step);




    protected:
        int handle(int e);

        // Draw mouse coords ant other things
        void drawCoordsAndOthers();

    public:
        PlotxyFLTK(int xp, int yp, int wp, int hp, const char *lp = 0);
        ~PlotxyFLTK();
        int insertValuesToPlot(float *value, int nvalue, int samplePerSecond = 512);
        //int insertValuesToPlot(data_element *value, int nvalue, int samplePerSecond = 512);

        void insertValueToPlot(float value, int samplePerSecond = 512);
        //void insertValueToPlot(data_element value, int samplePerSecond=512);


        int getZoomXValue();
        int getZoomYValue();
        int getTranslateYValue();
        float getSimulationSeconds();
        int getSampleTime();
        float getMaxValue();
        float getMinValue();

        void setAutoZoom(bool value);
        void setXAxis(char *name);
        void setYAxis(char *name);
        void setTraceMin(int value);
        void setTraceMax(int value);
        void setViedWidth(int value);
        void setSampleTime(int sampleTime);

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
#endif
// kate: indent-mode cstyle; space-indent on; indent-width 4; replace-tabs on;  replace-tabs on;
