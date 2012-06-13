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
#include <FL/x.H>

#include <boost/date_time.hpp>

#include <cmath>
#include <iostream>
#include "plotxyfltk.h"

using namespace std;

PlotxyFLTK::PlotxyFLTK(int xp, int yp, int wp, int hp, const char* lp): Fl_Box(xp, yp, wp, hp, lp) {
    /***DEFAULT VALUES***/
    this->trace_max = 65536;//max number of values
    this->trace_min = 512;//min number of values
    this->view_width = this->trace_min;//numbers showed

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
//     this->offesetDraw = 0;

//     trace = new float[trace_max];
    this->view = 0;

    this->initial_x = 0;
    this->initial_y = 0;
    this->vievedMaxValue = 0;
    this->vievedMinValue = 0;

    this->secondTag = 0.0;
    this->time = 0;

    this->simulationTime = second_clock::local_time();

    this->plotLineInGraph = false;
    this->plotLineInGraphValue = 0;


//     //FIXME don't show Zoom+ and Zoom-
//     //Popup menu option list
//     rclick_menu = new Fl_Menu_Item[4];
//     rclick_menu->insert(0, "Zoom+", 0, zoomDec, (void*)this);
//     rclick_menu->insert(1, "Zoom-", 0, zoomInc, (void*)this, FL_MENU_DIVIDER);
//     rclick_menu->insert(2, "Scale", 0, scale, (void*)this);
//     rclick_menu->insert(3, "AutoScale", 0, autoScaleBehaviour, (void*)this, FL_MENU_TOGGLE | FL_MENU_VALUE);
//        rclick_menu->add( 0 }

}


PlotxyFLTK::~PlotxyFLTK() {
    delete [] this->trace;
    delete [] this->view;
}

/**** STATIC FUNCTION ****/

// void PlotxyFLTK::zoomInc(Fl_Widget *widget, void *userdata) {
//     cout << "*** ZOOM + ***" << endl;
//     PlotxyFLTK *in = (PlotxyFLTK*)userdata;
//
//     in->scale_factor_y /= 2;
//
//     in->view_width /= 2;
//     if (in->scale_factor_y < 1) {
//         in->scale_factor_y = 1;
//     }
//
//     in->scale_factor_x /= 2;
//     if (in->scale_factor_x < 1) {
//         in->scale_factor_x = 1;
//
//     }
//
//     if (in->view_width < in->trace_min) {
//         in->view_width = in->trace_min;
//     }
//
//
//     cout << "Zoom+ | scale_factor_x:" << in->scale_factor_y << " scale_factor_x:" << in->scale_factor_x << " view_width:" << in->view_width << endl;
//
//     in->redraw();
//
// }


// void PlotxyFLTK::zoomDec(Fl_Widget *widget, void *userdata) {
//     cout << "*** ZOOM - ***" << endl;
//     PlotxyFLTK *in = (PlotxyFLTK*)userdata;
//
//     in->view_width *= 2;
//
//     in->scale_factor_y *= 2;
//     if (in->scale_factor_y >= 200000) {
//         in->scale_factor_y = 32;
//     }
//
//     in->scale_factor_x *= 2;
//     if (in->scale_factor_x >= 32) {
//         in->scale_factor_x = 32;
//         //in->view_width = in->trace_max;
//     }
//
//     if (in->view_width > in->trace_max) {
//         in->view_width = in->trace_max;
//     }
//
//
//
//     cout << "Zoom- | scale_factor_x:" << in->scale_factor_y << " scale_factor_x:" << in->scale_factor_x << " view_width:" << in->view_width << endl;
//     in->redraw();
//
// }



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

}

/********/


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


//Function to
int PlotxyFLTK::handle(int e) {
    int ret = Fl_Box::handle(e);
    Fl_Region regione;

    switch (e) {

//     case FL_DRAG:
//         cout << "miaooo " << hex << Fl::event_state() << endl;
//
//         if (Fl::event_state() == FL_LEFT_MOUSE) {
//             cout << "entrooo" << endl;
//             fl_color(FL_WHITE);
//             fl_rect(0, 0, (int)Fl::event_x(), (int)Fl::event_y());
//             //fl_draw_box(_FL_DIAMOND_DOWN_BOX,0,0,(int)Fl::event_x(), (int)Fl::event_y(),FL_WHITE);
//         }
//
//         break;

//     case FL_PUSH:
//         if (Fl::event_button() == FL_RIGHT_MOUSE) {
//             const Fl_Menu_Item *m = rclick_menu->popup(Fl::event_x(), Fl::event_y(), 0, 0, 0);
//             if (m)
//                 m->do_callback(0, m->user_data());
//             return(1);          // (tells caller we handled this event)
//         }
//
//         return(1);//it allow to use FL_DRAG (see documentation)
//         break;
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
    // Coordinates as a string
    char s[5][80];
    char pres[80];
    char tag[80];
    char max[10];
    char min[10];
    int i;

    sprintf(s[0], "x:%d y:%d", (int)Fl::event_x(), (int)Fl::event_y());
//     sprintf(pres, "Pres:%d", this->insertsValues);
//     sprintf(tag, "Pres:%.4f", this->secondTag);
    sprintf(s[1], "Max:%.4f", this->vievedMaxValue);
    sprintf(s[2], "Min:%.4f", this->vievedMinValue);

    fl_color(FL_WHITE);
    fl_font(FL_HELVETICA, 9);
    for (i = 0;i < 3;i++) {
        fl_draw(s[i], this->w() - 70, 15 + (11*i));
    }
//     fl_draw(s, this->W - 70, 15);
//     fl_draw(pres, this->W - 70, 26);
//     fl_draw(tag, this->W - 70, 37);
//     fl_draw(max, this->W - 70, 48);
//     fl_draw(min, this->W - 70, 59);
}



void PlotxyFLTK::draw() {
    int wd = w();
    int ht = h();
    int xo = x();
    int yo = y();
    int j, i;
    float incf, stepf;
    float xf, yf;
    char text[20];

    if (this->enableAutoScaleWhileGraph) {
        //Recalculate scale_factor_y
        this->scale_factor_y = ceil(fabs(this->vievedMaxValue) + fabs(this->vievedMinValue)) + 1;

        //Translate it in correct position
        this->translateGraphY();

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


    /***Second tag***/
    fl_color(FL_RED);
    fl_font(FL_HELVETICA, 10);
    fl_line_style(FL_JOIN_BEVEL, 3);
    for (j = 1;j <= (view_width / this->trace_min);j++) {
        fl_line(((wd*j) / (view_width / this->trace_min)) - this->secondTag , (this->translate_y) - 5 , (((wd*j) / (view_width / this->trace_min))  - this->secondTag), (this->translate_y) + 5);
        sprintf(text, "%d", j + time);
        fl_draw(text, (((wd*j) / (view_width / this->trace_min)) - this->secondTag), (this->translate_y) + 13);
    }

    /******/

    //     fl_line_style(FL_DOT, 1);
//     fl_color(FL_WHITE);
//     for (j = 1;j <= (NVERTICAL*(view_width / this->trace_min));j++) {
//         fl_line(((wd*j) / (NVERTICAL*(view_width / this->trace_min))) - this->intermidiateSecondsTag , 0 , (((wd*j) / (NVERTICAL*(view_width / this->trace_min)))  - this->intermidiateSecondsTag), ht);
//     }
//     fl_draw("val", 100, ((ht / this->scale_factor_y)));

    fl_color(FL_WHITE);
    fl_line_style(FL_JOIN_BEVEL, 1);

    fl_push_matrix();
    fl_translate(translate_x, translate_y);
    fl_scale(wd / this->scale_factor_x, ht / this->scale_factor_y);

    fl_begin_line();
//     fl_begin_points();

    for (i = 0; i < insertsValues; i++) {
        fl_vertex(((float)i / (float)this->trace_min), -view[i]);
        fl_color(FL_GREEN);
    }
    fl_end_line();
//     fl_end_points();
    fl_pop_matrix();

    /***Plot max value and min value***/
    fl_color(FL_RED);
    fl_line_style(FL_JOIN_BEVEL, 2);

    /*Plot max value*/
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

    /*Plot min value*/
    fl_push_matrix();
    fl_translate(translate_x, translate_y);
    fl_scale(wd / this->scale_factor_x, ht / this->scale_factor_y);
    fl_begin_line();
    for (i = 0;i < 5*(view_width / this->trace_min);i++) {
        fl_vertex(((float)i / (float)this->trace_min), -this->vievedMinValue);
    }
    fl_end_line();

    fl_pop_matrix();
    /**/

    /*** ***/

    fl_color(FL_WHITE);
    fl_line_style(FL_DOT , 1);
    fl_push_matrix();
    fl_translate(translate_x, translate_y);
    fl_scale(wd / this->scale_factor_x, ht / this->scale_factor_y);


    //Plot horizontal axis (divide plot into ten parts)
    stepf = (fabs(this->vievedMaxValue) + fabs(this->vievedMinValue)) / 10;
    for (incf = this->vievedMinValue;incf < 0 ;incf += stepf) {
        fl_begin_line();
        for (j = 0;j < this->view_width;j++) {
            fl_vertex(((float)j / (float)this->trace_min), -incf);
        }
        fl_end_line();
    }

    for (incf = 0;incf < (this->vievedMaxValue) + stepf;incf += stepf) {
        fl_begin_line();
        for (j = 0;j < this->view_width;j++) {
            fl_vertex(((float)j / (float)this->trace_min), -incf);
        }
        fl_end_line();
    }
    fl_pop_matrix();

    //Plot line into Graph at
    if (this->plotLineInGraph) {
        fl_color(FL_BLUE);
        fl_line_style(FL_JOIN_BEVEL , 2);
        fl_push_matrix();
        fl_translate(translate_x, translate_y);
        fl_scale(wd / this->scale_factor_x, ht / this->scale_factor_y);
        fl_begin_line();

        for (i = 0;i < this->insertsValues;i++)
            fl_vertex((float) i / (float)this->trace_min, this->plotLineInGraphValue);

        fl_end_line();
        fl_pop_matrix();
    }

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


int PlotxyFLTK::insertValuesToPlot(float* value, int nvalue) {
    float step;
    if (nvalue > trace_max)
        return -1;

    if (view == NULL) {
        view = new float[trace_max];
    }
    //Calculates step to secondTag
    step = (float) w() / view_width ;

    for (int i = 0; i < nvalue; i++) {
        this->insertsValues++;

        //Used to animate secondTag
//         if (this->insertsValues >= view_width) {
//             this->secondTag += step ;
//             if (this->secondTag >= w() / (float)((view_width / this->trace_min))) {
//                 this->secondTag = 0.0;
//                 time++;
//             }
//         }
        if (this->insertsValues > view_width) {
            this->insertsValues = view_width;
            insertInTail(value[i]);//minus sign is necessary to plot a correct graph

            //Used to animate secondTag
            this->secondTag += step ;
            if (this->secondTag >= w() / (float)((view_width / this->trace_min))) {
                this->secondTag = 0.0;
                time++;
            }
        } else {
            view[this->insertsValues] = value[i];
        }

        //view[i] = value[i]; //minus sign is necessary to plot a correct graph
        this->vievedMaxValue = getMaxValue(value[i], this->vievedMaxValue);
        this->vievedMinValue = getMinValue(value[i], this->vievedMinValue);

        /***autoZoom***/
        if (this->enableAutoScaleWhileGraph) {
            this->scale_factor_y = ceil(fabs(this->vievedMaxValue) + fabs(this->vievedMinValue)) + 1;
            this->translateGraphY();
        }
        /******/


    }

//     cout << "Max Value:" << fabs(this->vievedMaxValue) << endl;
//     cout << "Min Value:" << fabs(this->vievedMinValue) << endl;
//     cout << "Scale factor y:" << this->scale_factor_y << endl;

    this->redraw();
    return nvalue;
}

void PlotxyFLTK::insertValueToPlot(float value) {
    float valore = value;
    float step;
    if (view == NULL) {
//         cout << "Allow view" << endl;
        view = new float[trace_max];
    }

    this->insertsValues++;

    step = (float) w() / view_width ;

    //Used to animate secondTag
    if (this->insertsValues >= view_width) {
        this->secondTag += step ;
        if (this->secondTag >= w() / (float)((view_width / this->trace_min))) {
            this->secondTag = 0.0;
            time++;
            this->simulationTime = second_clock::local_time();
            cout << "Time:" << this->simulationTime.time_of_day().seconds() << endl;
        }
    }

    //Vertical line like to break view_width into NVERTICAL quadranti
//     if (this->insertsValues >= view_width) {
//         this->intermidiateSecondsTag += step ;
//         if (this->intermidiateSecondsTag > (w() / (NVERTICAL*((view_width / this->trace_min))))) {
//             this->intermidiateSecondsTag = 0.0;
//         }
//     }

    if (insertsValues > view_width) {
        this->insertsValues = view_width;
        insertInTail(valore);//minus sign is necessary to plot a correct graph
    } else {
        view[insertsValues-1] = valore;
    }

    this->vievedMaxValue = getMaxValue(valore, this->vievedMaxValue);
    this->vievedMinValue = getMinValue(valore, this->vievedMinValue);

    /***autoZoom***/
    if (this->enableAutoScaleWhileGraph) {
        this->scale_factor_y = ceil(fabs(this->vievedMaxValue) + fabs(this->vievedMinValue)) + 1;
        this->translateGraphY();
    }
    /******/

    this->redraw();
}

/**** PRIVATE FUNCTION ****/
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
/**** END PRIVATE FUNCTION ****/

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


float PlotxyFLTK::getMaxValue(float val, float max) {
    if (val > max)
        return val;
    else
        return max;
}

float PlotxyFLTK::getMinValue(float val, float min) {
    if (val < min)
        return val;
    else
        return min;
}


int PlotxyFLTK::getSimulationSeconds() {
    return this->time;
}

/**** END GET FUNCTION ****/

/**** SET FUNCTION ****/
void PlotxyFLTK::setAutoZoom(bool value) {
    //Enable/Disable Autoscale
    this->enableAutoScaleWhileGraph = value;

    //Recalculate scale_factor_y
    this->scale_factor_y = ceil(fabs(this->vievedMaxValue) + fabs(this->vievedMinValue)) + 1;

    //Translate it in correct position
    this->translateGraphY();

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
    if (!(value % 128))
        this->trace_min = value;
    else
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

/**** END SET FUNCTION ****/

void PlotxyFLTK::translateGraphY() {
    if ((this->vievedMinValue == 0) && (this->vievedMaxValue != 0))
        this->translate_y = (y() + (h() - 15));
    else if ((this->vievedMinValue != 0) && (this->vievedMaxValue == 0))
        this->translate_y = (y());
    else
        this->translate_y = (y() + (h() / 2));
}

void PlotxyFLTK::plotLine(float value) {

    this->plotLineInGraph = true;
    this->plotLineInGraphValue = -value;
    return ;
}

void PlotxyFLTK::zoomXInc() {
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
    this->scale_factor_x *= 2;
    if (this->scale_factor_x >= 32) {
        this->scale_factor_x = 32;
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

    //Recalculate scale_factor_y
    this->scale_factor_y = ceil(fabs(this->vievedMaxValue) + fabs(this->vievedMinValue)) + 1;

    //Translate it in correct position
    this->translateGraphY();

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
// kate: indent-mode cstyle; space-indent on; indent-width 4; replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;

