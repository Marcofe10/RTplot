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


#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Printer.H>
//#include <FL/x.H>

#include <boost/date_time.hpp>
#include <boost/circular_buffer.hpp>

#include <cmath>
#include <iostream>
#include "plotxyfltk.h"

using namespace std;

PlotxyFLTK::PlotxyFLTK(int xp, int yp, int wp, int hp, const char* lp): Fl_Box(xp, yp, wp, hp, lp) {
    /***DEFAULT VALUES***/

    this->trace_min = 512;//min number of values
    this->trace_max = this->trace_min * 64;//max number of values 512*64
    this->view_width = this->trace_min;//numbers showed
    this->samplePerSecond = this->trace_min;

    //Scale Factor
    this->scale_factor_x = 1.0;
    this->scale_factor_y = 1.0;

    //Enable AutoZoom
    this->enableAutoScaleWhileGraph = true;

    //X,Y name asix
    this->xAxis.append("t");
    this->yAxis.append("Attention");

    //Translate Factor
    this->translate_x = (float) x();
    this->translate_y = (float)(y() + (h() / 2));
    /******/

//     trace_pos = 0;
//     view_pos = 0;
    this->view_break = 0;
    this->view = 0;
//     this->offesetDraw = 0;
//     trace = new float[trace_max];


//     this->initial_x = 0;
//     this->initial_y = 0;
    this->vievedMaxValue = 0;
    this->vievedMinValue = 0;

    this->secondTag = 0.0;
//     this->incTagAxesValue = 0;
    this->time = 0.0;
    // this->residueTime = 0;

    this->simulationTime = second_clock::local_time();

    this->plotLineInGraph = false;
    this->plotLineInGraphValue = 0;

    this->dataCB = new boost::circular_buffer<data_element>(this->trace_max);

    this->enableRightMouseMenu = true;

    //Popup menu option list
    rclick_menu->add("Zoom+", 0, zoomDec, (void*)this);
    rclick_menu->add("Zoom-", 0, zoomInc, (void*)this, FL_MENU_DIVIDER);
    rclick_menu->add("Scale", 0, scale, (void*)this);
    rclick_menu->add("AutoScale", 0, autoScaleBehaviour, (void*)this, FL_MENU_TOGGLE | FL_MENU_VALUE);


    // Coordinates as a string

    char s[5][80];

    ///*sprintf(s[0], "x:%d y:%d", (int)Fl::event_x(), (int)Fl::event_y());*/
    sprintf(s[1], "Max:%.4f", this->vievedMaxValue);//Corresponds to max value
    sprintf(s[2], "Min:%.4f", this->vievedMinValue);//Corresponds to min value
    sprintf(s[3], "Pres:%d", this->insertsValues);//Corresponds to insertsValues
    sprintf(s[4], "SampSec:%d", this->samplePerSecond);//Corresponds to insertsValues

    stringToWriteinGraph.push_back(s[0]);
    stringToWriteinGraph.push_back(s[1]);
    stringToWriteinGraph.push_back(s[2]);
    stringToWriteinGraph.push_back(s[3]);
    stringToWriteinGraph.push_back(s[4]);
}


PlotxyFLTK::~PlotxyFLTK() {
    delete [] this->trace;
//     delete [] this->view;
}

/**** STATIC FUNCTION ****/

void PlotxyFLTK::zoomInc(Fl_Widget *widget, void *userdata) {
    cout << "*** ZOOM + ***" << endl;
    PlotxyFLTK *in = (PlotxyFLTK*)userdata;

    in->scale_factor_y /= 2;

    in->view_width /= 2;
    if (in->scale_factor_y < 1) {
        in->scale_factor_y = 1;
    }

    in->scale_factor_x /= 2;
    if (in->scale_factor_x < 1) {
        in->scale_factor_x = 1;
    }

    if (in->view_width < in->trace_min) {
        in->view_width = in->trace_min;
    }


    cout << "Zoom+ | scale_factor_x:" << in->scale_factor_y << " scale_factor_x:" << in->scale_factor_x << " view_width:" << in->view_width << endl;

    in->redraw();

}

void PlotxyFLTK::zoomDec(Fl_Widget *widget, void *userdata) {
    cout << "*** ZOOM - ***" << endl;
    PlotxyFLTK *in = (PlotxyFLTK*)userdata;

    in->view_width *= 2;

    in->scale_factor_y *= 2;
    if (in->scale_factor_y >= 200000) {
        in->scale_factor_y = 32;
    }

    in->scale_factor_x *= 2;
    if (in->scale_factor_x >= 32) {
        in->scale_factor_x = 32;
        //in->view_width = in->trace_max;
    }

    if (in->view_width > in->trace_max) {
        in->view_width = in->trace_max;
    }

    cout << "Zoom- | scale_factor_x:" << in->scale_factor_y << " scale_factor_x:" << in->scale_factor_x << " view_width:" << in->view_width << endl;
    in->redraw();

}

void PlotxyFLTK::scale(Fl_Widget *widget, void *userdata) {
    cout << "*** AUTOZOOM ***" << endl;
    PlotxyFLTK *in = (PlotxyFLTK*)userdata;

    in->scale_factor_y = abs(in->vievedMaxValue) + abs(in->vievedMinValue);
    cout << "AutoZoom :" << in->scale_factor_y << endl;
    in->redraw();

}

void PlotxyFLTK::autoScaleBehaviour(Fl_Widget* widget, void* userdata) {
    cout << "*** AUTOSCALE ***" << endl;
    PlotxyFLTK *in = (PlotxyFLTK*)userdata;

    in->zoomAuto();

    if (in->rclick_menu[3].value() == 4)
        in->rclick_menu[3].clear();
    else
        in->rclick_menu[3].set();

    cout << "*** END AUTOSCALE ***" << endl;

}

/**** END STATIC FUNCTION ****/


// static void Copy_CB(Fl_Widget*, void *userdata) {
//     printf("*** COPY ***\n");
// //         MyInput *in = (MyInput*)userdata;
// //         in->copy(0);    // text selection clipboard
// //         in->copy(1);    // copy/paste clipboard
// }
// static void Paste_CB(Fl_Widget*, void *userdata) {
//     printf("*** PASTE ***\n");
// //         MyInput *in = (MyInput*)userdata;
// //         Fl::paste(*in);
// }


//Function to handle mouse and keyborad event
int PlotxyFLTK::handle(int e) {
    int ret = Fl_Box::handle(e);
    Fl_Region regione;

    switch (e) {

//     case FL_DRAG:
//
//         if (Fl::event_state() == FL_LEFT_MOUSE) {
//             cout << "entrooo" << endl;
//             fl_color(FL_WHITE);
//             fl_rect(0, 0, (int)Fl::event_x(), (int)Fl::event_y());
//             //fl_draw_box(_FL_DIAMOND_DOWN_BOX,0,0,(int)Fl::event_x(), (int)Fl::event_y(),FL_WHITE);
//         }
//
//         break;

    case FL_PUSH:
        if (this->enableRightMouseMenu) {
            if (Fl::event_button() == FL_RIGHT_MOUSE) {
                const Fl_Menu_Item *m = rclick_menu->popup(Fl::event_x(), Fl::event_y(), 0, 0, 0);

                if (m)
                    m->do_callback(0, m->user_data());
                return(1);          // (tells caller we handled this event)
            }

            return(1);//it allow to use FL_DRAG (see documentation)
        }
        break;
    case FL_RELEASE:
        // RIGHT MOUSE RELEASED? Mask it from Fl_Input
        if (Fl::event_button() == FL_RIGHT_MOUSE) {
            return(1);          // (tells caller we handled this event)
        }

//         if (Fl::event_button() == FL_LEFT_MOUSE) {
//             fl_color(FL_WHITE);
//             fl_rect(initial_x, initial_y, abs((int)Fl::event_x() - initial_x), abs((int)Fl::event_y() - initial_y));
//             initial_x = 0;
//             initial_y = 0;
//             return(1);          // (tells caller we handled this event)
//         }
        break;

    case FL_ENTER:
        ret = 1;                // FL_ENTER: must return(1) to receive FL_MOVE
        break;


    case FL_MOVE:               // FL_MOVE: mouse movement causes 'user damage' and redraw..
//         damage(FL_DAMAGE_USER1);
        ret = 1;
        break;

    case FL_MOUSEWHEEL:

        if (Fl::event_dy() > 0) {
            this->zoomYIncMouseWheel();
        }
        if (Fl::event_dy() < 0) {
            this->zoomYDecMouseWheel();
        }

//         if (Fl::event_dx() < 0) {
//
//         }
//         if (Fl::event_dx() > 0) {
//
//         }
        damage(FL_DAMAGE_USER1);
        ret = 1;
        break;
    }
    return(ret);
}

void PlotxyFLTK::drawCoordsAndOthers() {
    int i;
    char str[80];

    fl_color(FL_YELLOW);
    fl_font(FL_HELVETICA, 9);
    sprintf(str, "x:%d y:%d", (int)Fl::event_x(), (int)Fl::event_y());
    //TODO Aggiungere modifica della stringa di vector alla posizione 0
    for (i = 0;i < stringToWriteinGraph.size();i++) {
        fl_draw(stringToWriteinGraph.at(i).c_str(), this->w() - 70, 15 + (11*i));
    }
//     fl_draw(s, this->W - 70, 15);
//     fl_draw(inserts_values, this->w() - 70, 15 + (11*i));
//     fl_draw(tag, this->W - 70, 37);
//     fl_draw(max, this->W - 70, 48);
//     fl_draw(min, this->W - 70, 59);

}


//Draw fucntion
void PlotxyFLTK::draw() {
    int wd = w();
    int ht = h();
    int xo = x();
    int yo = y();
    int j, i, k;
    float incf, stepf;
    float xf, yf;
    char text[20];
    float tempx, tempy;
    float temp;
    int offsettime = 1;
    int plotTextEvery = 0;

    if (this->enableAutoScaleWhileGraph) {
        this->scaleAndTranslateY();
    }

    fl_color(FL_BLACK);
    fl_rectf(xo, yo, wd, ht);
    fl_color(FL_WHITE);

    /*******Plot axes*******/
    fl_line_style(FL_JOIN_BEVEL, 3);
    fl_font(FL_HELVETICA, 10);

    //X asis
    fl_line(0, this->translate_y, wd, this->translate_y);
    fl_line(wd - 7, (this->translate_y) - 7, wd, this->translate_y);
    fl_line(wd - 7, (this->translate_y) + 7, wd, this->translate_y);
    fl_draw(this->xAxis.c_str(), wd - 10, this->translate_y + 15);

    //Y axis
    fl_line(0, 0, 0, ht);
    fl_line(0, 0, 7, 7);
    fl_draw(this->yAxis.c_str(), 5, 15);
    /***********************/

    /***DATA PLOTTING***/
    fl_color(FL_WHITE);
    fl_line_style(FL_JOIN_BEVEL, 1);

    fl_push_matrix();
    fl_translate(translate_x, translate_y);
    fl_scale(wd / this->scale_factor_x, ht / this->scale_factor_y);

    fl_begin_line();
//     fl_begin_points();

    if (this->insertsValues < this->view_width) {
        for (i = 0; i < insertsValues; i++) {
            //         fl_vertex(((float)i / (float)this->trace_min), -view[i]);
            fl_vertex(((float)i / (float)this->trace_min), -dataCB->at(i).value);
            fl_color(FL_GREEN);
        }
    } else {
        for (i = 0, j = (this->insertsValues - this->view_width); j < this->insertsValues; j++, i++) {
            //         fl_vertex(((float)i / (float)this->trace_min), -view[i]);
            fl_vertex(((float)i / (float)this->trace_min), -dataCB->at(j).value);
            fl_color(FL_GREEN);
        }
    }
    fl_end_line();


    plotTextEvery = ((this->view_width / this->samplePerSecond) / 32);
    if (!plotTextEvery)
        plotTextEvery = 1;

    //Tag about X axes
    fl_color(FL_RED);
    fl_line_style(FL_JOIN_BEVEL, 3);
    fl_font(FL_HELVETICA_BOLD, 8);
    if (this->insertsValues < this->view_width) {
        for (i = 0, k = 0; i < insertsValues; i++, k++) {
            if (!(i % (this->samplePerSecond*plotTextEvery)) && (i != 0)) {
                fl_begin_line();
                fl_vertex(((float)i / (float)this->trace_min), (this->scale_factor_y / 70));
                fl_vertex(((float)i / (float)this->trace_min), -(this->scale_factor_y / 70));
                fl_end_line();

                //sprintf(text, "%d", offsettime);
                sprintf(text, "%d", (int)this->dataCB->at(i - 1).time);
                fl_draw(text, 3 + fl_transform_x(((float)i / (float)this->trace_min), 0), -5 + fl_transform_y(((float)i / (float)this->trace_min), 0));
                k = 0;
            }
        }
    } else {
        for (i = 0, k = 0, j = (this->insertsValues - this->view_width); j < this->insertsValues; j++, i++, k++) {
            if (!(j % (this->samplePerSecond*plotTextEvery)) && (i != 0)) {
                fl_begin_line();
                fl_vertex(((float)i / (float)this->trace_min), (this->scale_factor_y / 70));
                fl_vertex(((float)i / (float)this->trace_min), -(this->scale_factor_y / 70));
                fl_end_line();

                //sprintf(text, "%d",(int) this->time + offsettime - (trace_min / this->samplePerSecond));
                sprintf(text, "%d", (int) this->dataCB->at(j - 1).time);
//                 cout <<(j % this->samplePerSecond)<<")Time:" << this->time << " Offesettime:" << offsettime << " (trace_min / this->samplePerSecond):" << (trace_min / this->samplePerSecond)<< endl;
                fl_draw(text, 3 + fl_transform_x(((float)i / (float)this->trace_min), 0), -5 + fl_transform_y(((float)i / (float)this->trace_min), 0));
                k = 0;
            }
        }
    }
//     fl_end_points();
    fl_pop_matrix();
    /***END DATA PLOTTING***/

    /***Plot max value and min value with a little red line***/
    fl_color(FL_RED);
    fl_line_style(FL_JOIN_BEVEL, 2);

    /*Plot line max value*/
    fl_push_matrix();
    fl_translate(translate_x, translate_y);
    fl_scale(wd / this->scale_factor_x, ht / this->scale_factor_y);
    fl_begin_line();
    for (i = 0;i < 5*(view_width / this->trace_min);i++) {
        fl_vertex(((float)i / (float)this->trace_min), -this->vievedMaxValue);
//         cout<<"fl_transform_x:"<<fl_transform_x(((float)i / (float)view_width), -this->vievedMaxValue)<<endl;
    }
    fl_end_line();
    fl_pop_matrix();
    /**/

    /*Plot line min value*/
    fl_push_matrix();
    fl_translate(translate_x, translate_y);
    fl_scale(wd / this->scale_factor_x, ht / this->scale_factor_y);
    fl_begin_line();
    for (i = 0;i < 5*(view_width / this->trace_min);i++) {
        fl_vertex(((float)i / (float)this->trace_min), -this->vievedMinValue);
    }
    fl_end_line();
    fl_pop_matrix();
    /*

    /*Write text for min,max*/
    fl_push_matrix();
    fl_translate(translate_x, translate_y);
    fl_scale(wd / this->scale_factor_x, ht / this->scale_factor_y);
    fl_color(FL_WHITE);
    fl_font(FL_HELVETICA_BOLD, 10);

    //Write Max value
    sprintf(text, "%f", this->vievedMaxValue);
    fl_draw(text, 3 + fl_transform_x(0, -this->vievedMaxValue), fl_transform_y(0, -this->vievedMaxValue));

    //Write Min value
    sprintf(text, "%f", this->vievedMinValue);
    fl_draw(text, 3 + fl_transform_x(0, -this->vievedMinValue), 10 + fl_transform_y(0, -this->vievedMinValue));
    fl_pop_matrix();
    /**/

    /*** ***/

    fl_color(FL_WHITE);
    fl_font(FL_HELVETICA, 9);
    fl_line_style(FL_DOT , 1);

    fl_push_matrix();
    fl_translate(translate_x, translate_y);
    fl_scale(wd / this->scale_factor_x, ht / this->scale_factor_y);


    /*Plot horizontal axis (divide plot into ten parts)*/
    stepf = (fabs(this->vievedMaxValue) + fabs(this->vievedMinValue)) / (NVERTICAL);

    //Negatives Values
    for (i = 0, incf = this->vievedMinValue;incf < 0 ;i++, incf += stepf) {
        fl_begin_line();
        for (j = 0;j < this->view_width;j++) {
            fl_vertex(((float)j / (float)this->trace_min), -incf);
        }
        fl_end_line();

        //Draw values of horizontal negative line
        //Draw values of horizontal positive line
        if ((incf > (this->vievedMinValue) && (incf < 0))) {
            sprintf(text, "%f", incf);
            fl_draw(text, 1 + fl_transform_x(0, -incf), fl_transform_y(0, -incf));
        }
//         sprintf(text, "%f", -temp);
//         fl_draw(text, 1 + fl_transform_x(0, temp), fl_transform_y(0, temp));
    }

    //Positivies Values
    for (i = 0, incf = 0;incf < (this->vievedMaxValue) + stepf;i++, incf += stepf) {
        fl_begin_line();
        for (j = 0;j < this->view_width;j++) {
            fl_vertex(((float)j / (float)this->trace_min), -incf);
        }
        fl_end_line();

        //Draw values of horizontal positive line
        if ((incf > 0) && (incf < (this->vievedMaxValue))) {
            sprintf(text, "%f", incf);
            fl_draw(text, 1 + fl_transform_x(0, -incf), fl_transform_y(0, -incf));
        }
    }

    fl_pop_matrix();
    /**/

    //Plot line into Graph at
    if (this->plotLineInGraph) {
        fl_color(FL_BLUE);
        fl_line_style(FL_JOIN_BEVEL , 2);
        fl_push_matrix();
        fl_translate(translate_x, translate_y);
        fl_scale(wd / this->scale_factor_x, ht / this->scale_factor_y);
        fl_begin_line();

        for (i = 0;i < this->view_width;i++)
            fl_vertex((float) i / (float)this->trace_min, -this->plotLineInGraphValue);

        fl_end_line();

        fl_color(FL_WHITE);

        //Plot value line
        fl_font(FL_HELVETICA, 9);
        sprintf(text, "%f", this->plotLineInGraphValue);
//         cout << "Fl_transform>>> " << fl_transform_x(0, this->plotLineInGraphValue) << ":" << fl_transform_y(0, this->plotLineInGraphValue) << endl;
//         cout << "Fl_transform_d>>> " << fl_transform_dx(0, this->plotLineInGraphValue) << ":" << fl_transform_dy(0, this->plotLineInGraphValue) << endl;
        fl_draw(text, 1 + fl_transform_x(0, -this->plotLineInGraphValue), fl_transform_y(0, -this->plotLineInGraphValue));
        //cout<<"fl_transform_dx:"<<fl_transform_dx(0,this->plotLineInGraphValue)<<" fl_transform_dy:"<<fl_transform_dy(0,this->plotLineInGraphValue)<<endl;
        fl_pop_matrix();
    }

//     cout <<"Fl_transform>>> "<< fl_transform_x(0, 300) << ":" << fl_transform_y(0, 300) << endl;
//     cout <<"Fl_transform_d>>> "<< fl_transform_dx(0, 300) << ":" << fl_transform_dy(0, 300) << endl;


    /*
              tempx= fl_transform_x(0.7 , 0.7);
          tempy= fl_transform_y(0.7 , 0.7);
          cout <<" temp:"<<tempx<<" tempy:"<<tempy<<endl;Z*/

//     stepf =(float) 128 / (this->view_width) ;
//     cout << "stepf:" << stepf << endl;
//     for (xf = 0.0; xf < 1.0;xf += stepf) {
//         //cout << "xf:" << xf << endl;
//         fl_begin_line();
//         for (yf = this->vievedMinValue;yf <= this->vievedMaxValue;yf += 1.0) {
//             fl_vertex(xf, yf);
//         }
//         fl_end_line();
//     }

    drawCoordsAndOthers();
} /* end of draw() method */


int PlotxyFLTK::insertValuesToPlot(float* value, int nvalue, int samplePerSecond) {
    float step;
    if (nvalue > trace_max)
        return -1;

//     if (view == NULL) {
//         view = new float[trace_max];
//     }
//     this->setTraceMin(trace_min);
//     cout << "this->trace_min" << this->trace_min << endl;

    //Calculates step to secondTag
    this->samplePerSecond = samplePerSecond;
    if (this->sampleTime != 0)
        this->samplePerSecond = this->sampleTime;

    step = (float) w() / this->view_width ;

    for (int i = 0; i < nvalue; i++) {
        this->commonInsertValue(value[i], step);
    }

    this->redraw();
    return nvalue;
}


void PlotxyFLTK::insertValueToPlot(float value, int samplePerSecond) {
    float step;

//     if (view == NULL) {
// //         cout << "Allow view" << endl;
//         view = new float[trace_max];
//     }

    this->samplePerSecond = samplePerSecond;
    if (this->sampleTime != 0)
        this->samplePerSecond = this->sampleTime;

    step = (float) w() / view_width ;//used from secondTag

    this->commonInsertValue(value, step);

    this->redraw();
}

/**** PRIVATE FUNCTION ****/

int PlotxyFLTK::commonInsertValue(float value, float step) {
    data_element prov;

    //Calculates step to secondTag
    if (this->insertsValues < this->trace_max)
        this->insertsValues++;
    else
        this->insertsValues = this->trace_max;

    prov.value = value;
    prov.time = (float) insertsValues / samplePerSecond;
    this->time = prov.time;

    dataCB->push_back(prov);

    if (this->insertsValues >= view_width) {
        //Used to animate secondTag
        this->secondTag += step ;
        if (this->secondTag >= (w() / (float)((view_width / this->samplePerSecond)))) {
            this->secondTag = 0.0;
//             this->incTagAxesValue++;
            this->simulationTime = second_clock::local_time();
            cout << "Time:" << this->simulationTime.time_of_day().seconds() << endl;
        }
    }

//     //Increase time
//     this->residueTime++;
//     if (this->residueTime == (this->samplePerSecond)) {
//         this->time++;
//         this->residueTime = 0;
//     }

    this->maxValue = findMaxValue(value, this->vievedMaxValue);
    this->minValue = findMinValue(value, this->vievedMinValue);

    //Gets min and max value from dataCB in a windows large view_width and put this values in this->vievedMaxValue and this->vievedMinValue
    getMaxMinWindowValue();

    /***autoZoom***/
    if (this->enableAutoScaleWhileGraph) {
        scaleAndTranslateY();
    }
    /******/

}

void PlotxyFLTK::insertInTail(float element) {
    this->shift_vector_left(element);
    this->view[insertsValues-1] = element;
}

void PlotxyFLTK::shift_vector_left(float element) {
    int i = 0;
    for (i = 1;i < insertsValues;i++) {
        this->view[i-1] = this->view[i];
    }
    this->view[i-1] = 0.0;
}

float PlotxyFLTK::findMaxValue(float val, float current_max) {
    if (val > current_max)
        return val;
    else
        return current_max;
}

float PlotxyFLTK::findMinValue(float val, float current_min) {
    if (val < current_min)
        return val;
    else
        return current_min;
}

void PlotxyFLTK::getMaxMinWindowValue() {
    int i;
    int lastElement = this->insertsValues - 1;
    float min = 0, max = 0;

//     cout << "lastElement:" << lastElement << endl;
//     cout << "1max:" << max << " min:" << min << endl;

    for (i = lastElement;(i > 0) && (i > (lastElement - this->view_width));i--) {
//         cout << "i:" << i << endl;
        if (dataCB->at(i).value > max)
            max = dataCB->at(i).value;
        else if (dataCB->at(i).value < min)
            min = dataCB->at(i).value;
    }

    this->vievedMaxValue = max;
    this->vievedMinValue = min;
//     cout << "max:" << max << " min:" << min << endl;
}

void PlotxyFLTK::scaleAndTranslateY() {
    int factor;

    factor = exp10(log10(fabs(this->vievedMaxValue) + fabs(this->vievedMinValue)) - 1);
    //calculate scale_factor_y
    this->scale_factor_y = ceil(fabs(this->vievedMaxValue) + fabs(this->vievedMinValue)) + factor;

//     cout << "scale_factor_y:" << scale_factor_y << " this->vievedMaxValue:" << this->vievedMaxValue << endl;

    //Translate it in correct position
    this->translateGraphY();
}



/**** END PRIVATE FUNCTION ****/

void PlotxyFLTK::addStringToGraph(const char* str) {
    stringToWriteinGraph.push_back(str);
}


/**** CONVERTION FUNCTION ****/

int PlotxyFLTK::convertDataElementToFloat(data_element* data, float *dataFloat, int nvalue) {
    int i;

    if (dataFloat == NULL)
        return -2;
    if (data == NULL)
        return -1;

    for (i = 0;i < nvalue;i++) {
        dataFloat[i] = data[i].value;
    }
    return i;
}

/**** ****/


/**** GET FUNCTION ****/
int PlotxyFLTK::getTranslateYValue() {
    return this->translate_y;
}

int PlotxyFLTK::getZoomXValue() {
    return this->scale_factor_x;
}

int PlotxyFLTK::getZoomYValue() {
    return this->scale_factor_y;
}

float PlotxyFLTK::getSimulationSeconds() {
    return this->time;
}

int PlotxyFLTK::getSampleTime() {
    return this->sampleTime;
}

float PlotxyFLTK::getMaxValue() {
    return this->maxValue;
}

float PlotxyFLTK::getMinValue() {
    return this->minValue;
}


/**** END GET FUNCTION ****/

/**** SET FUNCTION ****/
void PlotxyFLTK::setAutoZoom(bool value) {
    //Enable/Disable Autoscale
    this->enableAutoScaleWhileGraph = value;

    this->scaleAndTranslateY();

    //Redraw everything
    this->redraw();
}


void PlotxyFLTK::setXAxis(char* name) {
    this->xAxis.append(name);
}

void PlotxyFLTK::setYAxis(char* name) {
    this->yAxis.append(name);
}

void PlotxyFLTK::setTraceMax(int value) {
    if (!(value % 128))
        this->trace_max = value;
    else
        cout << "Default value set! Value must be power of 2" << endl;
}

void PlotxyFLTK::setTraceMin(int value) {
    if (!(value % 128)) {
        this->trace_min = value;
        this->view_width = this->trace_min;
    } else
        cout << "Default value set! Value must be power of 2" << endl;
}

void PlotxyFLTK::setViedWidth(int value) {
    if (value <= 32) {
        this->scale_factor_x = value;
        this->view_width *= value;
    } else {
        this->scale_factor_x = 32;
        this->view_width = this->trace_max;
    }
}

void PlotxyFLTK::setSampleTime(int sampleTime) {
    this->sampleTime = sampleTime;
}

void PlotxyFLTK::setenableRightMouseMenu(bool value) {
    this->enableRightMouseMenu = value;
}



/**** END SET FUNCTION ****/

void PlotxyFLTK::translateGraphY() {
    if ((this->vievedMinValue == 0) && (this->vievedMaxValue != 0))
        this->translate_y = (y() + (h() - 15));//When i've only positive values
    else if ((this->vievedMinValue != 0) && (this->vievedMaxValue == 0))
        this->translate_y = (y()); //When i've only negative values
    else {
        //When i've negative and positive values
        this->translate_y = (y() + (h() * (this->vievedMaxValue / this->scale_factor_y)));
    }
}

void PlotxyFLTK::plotLine(float value) {

    this->plotLineInGraph = true;
    this->plotLineInGraphValue = value;
    return ;
}

void PlotxyFLTK::zoomXInc() {
    this->secondTag = 0.0;
    this->scale_factor_x /= 2;
    if (this->scale_factor_x < 1) {
        this->scale_factor_x = 1;
    }

    this->view_width /= 2;
    if (this->view_width < this->trace_min) {
        this->view_width = this->trace_min;
    }
    cout << "Zoom+ | scale_factor_x:" << this->scale_factor_x << " view_width:" << this->view_width << endl;
    this->redraw();
}

void PlotxyFLTK::zoomXDec() {
    this->secondTag = 0.0;
    this->scale_factor_x *= 2;
    if (this->scale_factor_x > 64) {
        this->scale_factor_x = 64;
    }

    this->view_width *= 2;
    if (this->view_width > this->trace_max) {
        this->view_width = this->trace_max;
    }

    cout << "Zoom- |  scale_factor_x:" << this->scale_factor_x << " view_width:" << this->view_width << endl;
    this->redraw();
}

void PlotxyFLTK::zoomYInc() {
    this->scale_factor_y /= 2;
    if (this->scale_factor_y < 1) {
        this->scale_factor_y = 1;
    }
    cout << "Zoom+ | scale_factor_y:" << this->scale_factor_y << endl;
    this->redraw();
}

void PlotxyFLTK::zoomYDec() {
    this->scale_factor_y *= 2;
    if (this->scale_factor_y >= 200000) {
        this->scale_factor_y = 200000;
    }
    cout << "Zoom- | scale_factor_y:" << this->scale_factor_y << endl;
    this->redraw();
}

void PlotxyFLTK::zoomAuto() {
    this->enableAutoScaleWhileGraph = !this->enableAutoScaleWhileGraph;
    cout << "enableAutoScaleWhileGraph value:" << this->enableAutoScaleWhileGraph << endl;

    this->scaleAndTranslateY();

    //Redraw everything
    this->redraw();
}

void PlotxyFLTK::translateX() {
    this->translate_x = x();
    this->translate_y = (y() + (h() / 2));
}

void PlotxyFLTK::translateYUp() {
    this->translate_y -= 10;
    cout << "Translate+ | translate_y:" << this->translate_y << " Y:" << y() << endl;
    if (this->translate_y <= y())
        this->translate_y = y();
    this->redraw();
}

void PlotxyFLTK::translateYDown() {
    this->translate_y += 10;
    cout << "Translate- | translate_y:" << this->translate_y << " H:" << h() << endl;
    if (this->translate_y >= h())
        this->translate_y = h();
    this->redraw();
}


void PlotxyFLTK::zoomYIncMouseWheel() {
    this->scale_factor_y -= 10;
    if (this->scale_factor_y < 1) {
        this->scale_factor_y = 1;
    }
    cout << "Zoom+ | scale_factor_y:" << this->scale_factor_y << endl;
    this->redraw();
}

void PlotxyFLTK::zoomYDecMouseWheel() {
    this->scale_factor_y += 10;
    if (this->scale_factor_y >= 200000) {
        this->scale_factor_y = 200000;
    }
    cout << "Zoom- | scale_factor_y:" << this->scale_factor_y << endl;
    this->redraw();
}


