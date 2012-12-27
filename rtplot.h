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

/**
 * \brief Contains time and corrispond value
 *
 */
typedef struct _data_element {
    float time;/**< Time*/
    float value;/**< Value */
}data_element;

/*
 * Class relative to graphic part
 */
class PlotxyFLTK: public Fl_Box {
    private:
        //*** VARIABLE ***//

        int trace_max;//max number of values
        int trace_min;//min number of values
        int view_width;//numbers showed


        float *trace;//all value
        float *view;//showed values
        int insertsValues;//contains the values number acquired

        //Bool to enable autoscale while graphing
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

        //Contains element
        boost::circular_buffer<data_element> *dataCB;

        float secondTag;//Use to move second tag (line to sign seconds)

        int samplePerSecond;//Sample per second. Use to plot secondtag in relationship of view_width
        int sampleTime;//Sample time (at the moment it's similar to samplePerSecond)

        //Menu Item
        Fl_Menu_Item rclick_menu[5];

        float time;//Simulation time calculated by sample
        //int residueTime;

        ptime simulationTime;

        //Used to plot line into graph
        bool plotLineInGraph;
        float plotLineInGraphValue;

        //Used to save clicked position
        int initial_x, initial_y;
        bool flag_initialxy;

        //******//

        // *** FUNCTIONS ***
        //Insert element in tail
        void insertInTail(float element);

        //Shift vector on left
        void shift_vector_left(float element);

        //Translate Graph along y;
        void translateGraphY();

        //Function used by FLTK
        void draw();

        //Enable/Disable menu with right click
        bool enableRightMouseMenu;

        /***FL_MENU_ITEM FUNCTION***/

        //Zoom static function
        static void zoomInc(Fl_Widget *widget, void *userdata);
        static void zoomDec(Fl_Widget *widget, void *userdata);

        //Autoscale Function
        static void scale(Fl_Widget *widget, void *userdata);

        //Allow enable/disable Autoscale graph during plot
        static void autoScaleBehaviour(Fl_Widget *widget, void *userdata);

        /**
         * \brief Find the Min and max value along y axis
         *
         * \dete This function finds min or max from val and current_xxx (with xxx min or max)
         **********************************************/

        float findMaxValue(float val, float current_max);
        float findMinValue(float val, float current_min);

        /**
         * \brief Get max and min value in a window of values
        */
        void getMaxMinWindowValue();

        //scale and translate along y
        void scaleAndTranslateY();

        //Common operation in insertValues e insertValue
        int commonInsertValue(float value, float step);

        vector<string> stringToWriteinGraph;
        /******/

    protected:
        int handle(int e);

        // Draw mouse coords ant other things
        void drawCoordsAndOthers();

    public:

        /**
         * \brief Creates window  to plot data
         * \details Creates a window (wp,hp) to plot graph data

         * \param xp corresponds to x initial point
         * \param yp corresponds to y initial point
         * \param wp corresponds to width of window
         * \param hp corresponds to height of window
         * \param lp the window's name
         */
        PlotxyFLTK(int xp, int yp, int wp, int hp, const char *lp = 0);
        ~PlotxyFLTK();

        /**
         * \brief Plots the values into window
         * \details Plots the values into window

         * \param value Values to plot
         * \param nvalue number of values
         */
        int insertValuesToPlot(float *value, int nvalue);
        //int insertValuesToPlot(float *value, int nvalue, int samplePerSecond = 512);



        /**
         * \brief Plots the value into window
         * \details Plots the values into window

         * \param value Value to plot
         */
        void insertValueToPlot(float value);
        //void insertValueToPlot(data_element value, int samplePerSecond=512);

        void addStringToGraph(const char *str);

        //CONVERTION FUNCTIONS
        int convertDataElementToFloat(data_element *dataelement, float *dataFloat, int nvalue);

        //******** GET FUNCTION ********

        /**
        * \brief Gets x zoom value
        * \details Gets x zoom value
        **********************************************/
        int getZoomXValue();


        /**
        * \brief Gets Y zoom value
        * \details Gets Y zoom value
        **********************************************/
        int getZoomYValue();
        int getTranslateYValue();


        /**
        * \brief Gets simulation seconds
        * \details Gets simulation seconds
        **********************************************/
        float getSimulationSeconds();


          /**
        * \brief Gets sample Time
        * \details Gets sample Time
        **********************************************/
        int getSampleTime();

        /**
        * \brief Gets max value
        * \details Gets max value
        **********************************************/
        float getMaxValue();

        /**
        * \brief Gets min value
        * \details Gets min value
        **********************************************/
        float getMinValue();

        //******** END GET FUNCTION ********

        //******** SET FUNCTION ********
        void setAutoZoom(bool value);
        void setXAxis(char *name);
        void setYAxis(char *name);
        void setTraceMin(int value);
        void setTraceMax(int value);
        void setViedWidth(int value);
        void setSampleTime(int sampleTime);
        void setenableRightMouseMenu(bool value);

        //******** END SET FUNCTION ********

        //Increments X width and scale factor along x (It corresponds to zoom decrement)
        void zoomXDec();

        //Decrements X width and scale factor along x (It corresponds to zoom increment)
        void zoomXInc();

        //Increments Y scale factor along y (It corresponds to zoom decrement)
        void zoomYDec();

        //Decrements Y  scale factor along y (It corresponds to zoom increment)
        void zoomYInc();

        //Increments Y  scale factor along y (It corresponds to zoom decrement) with mouse wheel
        void zoomYDecMouseWheel();

        //Decrements Y scale factor along y (It corresponds to zoom increment) with mouse wheel
        void zoomYIncMouseWheel();

        //Enable/Disable Autozoom
        void zoomAuto();

        void translateX();//??

        //Translates y downwards
        void translateYDown();

        //TRanslates y upwards
        void translateYUp();

        //Plot line into graph
        void plotLine(float value);


};
#endif
