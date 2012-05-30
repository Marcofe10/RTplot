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

#include <cmath>
#include <iostream>
#include "plotxyfltk.h"

using namespace std;

PlotxyFLTK::PlotxyFLTK(int xp, int yp, int wp, int hp, const char* lp): Fl_Box(xp, yp, wp, hp, lp) {
    this->X = xp;
    this->Y = yp;
    this->W = wp;
    this->H = hp;

    this->trace_max = 16384;//max number of values
    this->trace_min = 512;//max number of values
    this->view_width = this->trace_min;//numbers showed

//     trace_pos = 0;
//     view_pos = 0;
    this->view_break = 0;
//     this->offesetDraw = 0;

//     trace = new float[trace_max];
    this->view = 0;

    this->initial_x = 0;
    this->initial_y = 0;

    this->scale_factor_x = 1;
    this->scale_factor_y = 1;
    this->translate_x = x();
    this->translate_y = (y() + (h() / 2));
    this->enableAutoScaleWhileGraph = true;
    this->vievedMaxValue = 0;
    this->vievedMinValue = 0;
    this->xAxis.append("t");
    this->yAxis.append("Attention");
    this->secondTag = 0.0;

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
    in->enableAutoScaleWhileGraph = !in->enableAutoScaleWhileGraph;
    cout << "enableAutoScaleWhileGraph value:" << in->enableAutoScaleWhileGraph << endl;

    if (in->rclick_menu[3].value() == 4)
        in->rclick_menu[3].clear();
    else
        in->rclick_menu[3].set();

}


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

    case FL_PUSH:
        if (Fl::event_button() == FL_RIGHT_MOUSE) {
            const Fl_Menu_Item *m = rclick_menu->popup(Fl::event_x(), Fl::event_y(), 0, 0, 0);
            if (m)
                m->do_callback(0, m->user_data());
            return(1);          // (tells caller we handled this event)
        }

        return(1);//it allow to use FL_DRAG (see documentation)
        break;
    case FL_RELEASE:
        // RIGHT MOUSE RELEASED? Mask it from Fl_Input
        if (Fl::event_button() == FL_RIGHT_MOUSE) {
            return(1);          // (tells caller we handled this event)
        }

        if (Fl::event_button() == FL_LEFT_MOUSE) {
            fl_color(FL_WHITE);
            fl_rect(initial_x, initial_y, abs((int)Fl::event_x() - initial_x), abs((int)Fl::event_y() - initial_y));
            initial_x = 0;
            initial_y = 0;
            return(1);          // (tells caller we handled this event)
        }
        break;

    case FL_ENTER:
        ret = 1;                // FL_ENTER: must return(1) to receive FL_MOVE
        break;


    case FL_MOVE:               // FL_MOVE: mouse movement causes 'user damage' and redraw..
        //damage(FL_DAMAGE_USER1);
        ret = 1;
        break;

    case FL_MOUSEWHEEL:

//         if (Fl::event_dy() > 0) {
//             translate_y -= 3;
//         }
//
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

void PlotxyFLTK::draw_coords() {
    // Coordinates as a string
    char s[80];
    char pres[80];
    char tag[80];
    char max[10];
    char min[10];
    sprintf(s, "x:%d y:%d", (int)Fl::event_x(), (int)Fl::event_y());
    sprintf(pres, "Pres:%d", this->insertsValues);
    sprintf(tag, "Pres:%.4f", this->secondTag);
    sprintf(max, "Max:%.4f", this->vievedMaxValue);
    sprintf(min, "Min:%.4f", this->vievedMinValue);
    // Black rect
//     fl_color(FL_BLACK);
//     fl_rectf(10, 10, 200, 25);
    // White text
    fl_color(FL_WHITE);
    fl_font(FL_HELVETICA, 10);
    fl_draw(s, this->W - 70, 15);
    fl_draw(pres, this->W - 70, 26);
    fl_draw(tag, this->W - 70, 37);
    fl_draw(max, this->W - 70, 48);
    fl_draw(min, this->W - 70, 59);
}



void PlotxyFLTK::draw() {
    int wd = w();
    int ht = h();
    int xo = x();
    int yo = y();
    int j, i;
    char text[20];

    fl_color(FL_BLACK);
    fl_rectf(xo, yo, wd, ht);
    fl_color(FL_WHITE);

    /*******Plot axes*******/
    fl_line_style(FL_JOIN_BEVEL, 1);
    fl_font(FL_HELVETICA, 10);

    //X asis
//     fl_line(0, ht / 2, wd, ht / 2);
//     fl_line(wd - 5, (ht / 2) - 5, wd, ht / 2);
//     fl_line(wd - 5, (ht / 2) + 5, wd, ht / 2);
//     fl_draw(this->xAxis.c_str(), wd - 10, ht / 2 + 15);


    fl_line(0, this->translate_y, wd, this->translate_y);
    fl_line(wd - 5, (this->translate_y) - 5, wd, this->translate_y);
    fl_line(wd - 5, (this->translate_y) + 5, wd, this->translate_y);
    fl_draw(this->xAxis.c_str(), wd - 10, this->translate_y + 15);

    //Y axis
    fl_line(0, 0, 0, ht);
    fl_line(0, 0, 5, 5);
    fl_draw(this->yAxis.c_str(), 5, 15);

    fl_line_style(FL_JOIN_BEVEL, 2);
    /***********************/

    fl_color(FL_WHITE);
    fl_font(FL_HELVETICA, 10);


    //FIXME Risolvere problema per ottenere i tag per piu' secondi in una sola schermata
    /***Second tag***/

//     for (j = 1;j <= (view_width / 512);j++) {
//
//         if (this->insertsValues >= view_width) {
//             secondTag += (float) wd / view_width ;
//             if (this->secondTag >= wd) {
//                 this->secondTag = 0.0;
//             }
//
//         } /*else {
//
//
//         }*/
//         sprintf(text, "%d", j);
//         fl_line_style(FL_JOIN_BEVEL, 2);
//         fl_line((wd - secondTag) , (ht / 2) - 5 , (wd  - secondTag), (ht / 2) + 5);
//         fl_draw(text, (wd - secondTag), (ht / 2) + 13);
//     }
    /******/

    fl_line_style(FL_JOIN_BEVEL, 1);

    fl_push_matrix();
    fl_translate(translate_x, translate_y);
    fl_scale(wd / this->scale_factor_x, ht / this->scale_factor_y);

    fl_begin_line();

    for (i = 0; i < insertsValues; i++) {
//         if (i == view_break) {
//             fl_end_line();
//             fl_begin_line();
//         }
        fl_vertex(((float)i / (float)view_width), view[i]);
        fl_color(FL_GREEN);
    }
    fl_end_line();

    fl_pop_matrix();

    draw_coords();
} /* end of draw() method */


int PlotxyFLTK::insertValuesToPlot(float* value, int nvalue) {
    if (nvalue > trace_max)
        return -1;

    if (view == NULL) {
        view = new float[trace_max];
    }
    this->insertsValues = nvalue;

    for (int i = 0; i < insertsValues; i++) {

        view[i] = - value[i]; //minus sign is necessary to plot a correct graph
        this->vievedMaxValue = getMaxValue(view[i], this->vievedMaxValue);
        this->vievedMinValue = getMinValue(view[i], this->vievedMinValue);

    }
    this->scale_factor_y = ceil(fabs(this->vievedMaxValue) + fabs(this->vievedMinValue)) + 1;
    this->translateGraphY();

//     cout << "Max Value:" << fabs(this->vievedMaxValue) << endl;
//     cout << "Min Value:" << fabs(this->vievedMinValue) << endl;
//     cout << "Scale factor y:" << this->scale_factor_y << endl;
    return 0;
}

void PlotxyFLTK::insertValueToPlot(float value) {
    if (view == NULL) {
//         cout << "Allow view" << endl;
        view = new float[trace_max];
    }

    this->insertsValues++;
    if (insertsValues > view_width) {

        this->insertsValues = view_width;
        insertInTail(-value);//minus sign is necessary to plot a correct graph
    } else {
        view[insertsValues-1] = -value;
    }

    if (this->enableAutoScaleWhileGraph) {
        this->vievedMaxValue = getMaxValue(value, this->vievedMaxValue);
        this->vievedMinValue = getMinValue(value, this->vievedMinValue);
        this->scale_factor_y = ceil(fabs(this->vievedMaxValue) + fabs(this->vievedMinValue)) + 1;

        this->translateGraphY();
//         cout << "Max Value:" << fabs(this->vievedMaxValue) << endl;
//         cout << "Min Value:" << fabs(this->vievedMinValue) << endl;
//         cout << "Scale factor y:" << this->scale_factor_y << endl;
    }
    this->redraw();
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

void PlotxyFLTK::setXAxis(char* name) {
    this->xAxis.append(name);
}

void PlotxyFLTK::setYAxis(char* name) {
    this->yAxis.append(name);
}

void PlotxyFLTK::translateGraphY() {
    if ((this->vievedMinValue == 0) && (this->vievedMaxValue != 0))
        this->translate_y = (y() + (h()));
    else if ((this->vievedMinValue != 0) && (this->vievedMaxValue == 0))
        this->translate_y = (y() + (h()));
    else
        this->translate_y = (y() + (h() / 2));
}

void PlotxyFLTK::plotLine() {
    return ;
}
