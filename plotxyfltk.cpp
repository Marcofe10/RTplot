#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Printer.H>

#include <math.h>
#include <iostream>
#include "plotxyfltk.h"

using namespace std;

PlotxyFLTK::PlotxyFLTK(int xp, int yp, int wp, int hp, const char* lp): Fl_Box(xp, yp, wp, hp, lp) {
    trace_max = 1024;//max number of values
    view_width = 512;//numbers showed

    trace_pos = 0;
    view_pos = 0;
    view_break = 0;

    trace = new float[trace_max];
    view = 0;

    initial_x = 0;
    initial_y = 0;

    this->scale_factor = 1;
    this->translate_x = x();
    this->translate_y = (y() + (h() / 2));
    this->enableAutoScaleWhileGraph = 0;
    this->vievedMaxValue = 0;
    this->vievedMinValue = 0;
}


PlotxyFLTK::~PlotxyFLTK() {
    delete [] this->trace;
    delete [] this->view;
}

void PlotxyFLTK::zoomInc(Fl_Widget *widget, void *userdata) {
    cout << "*** ZOOM + ***" << endl;
    PlotxyFLTK *in = (PlotxyFLTK*)userdata;

    in->scale_factor /= 2;
    if (in->scale_factor <= 0)
        in->scale_factor = 1;
    cout << "Zoom +:" << in->scale_factor << endl;
    in->redraw();

}

void PlotxyFLTK::zoomDec(Fl_Widget *widget, void *userdata) {
    cout << "*** ZOOM - ***" << endl;
    PlotxyFLTK *in = (PlotxyFLTK*)userdata;
    if (in->scale_factor >= 200000)
        in->scale_factor = 200000;
    in->scale_factor *= 2;
    cout << "Zoom -:" << in->scale_factor << endl;
    in->redraw();

}

void PlotxyFLTK::autoScale(Fl_Widget *widget, void *userdata) {
    cout << "*** AUTOZOOM ***" << endl;
    PlotxyFLTK *in = (PlotxyFLTK*)userdata;

    in->scale_factor = abs(in->vievedMaxValue) + abs(in->vievedMinValue);
    cout << "AutoZoom :" << in->scale_factor << endl;
    in->redraw();

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


int PlotxyFLTK::handle(int e) {
    int ret = Fl_Box::handle(e);
    Fl_Region regione;

    switch (e) {

    case FL_DRAG:
        cout << "miaooo " << hex << Fl::event_state() << endl;

        if (Fl::event_state() == FL_LEFT_MOUSE) {
            cout << "entrooo" << endl;
            fl_color(FL_WHITE);
            fl_rect(0, 0, (int)Fl::event_x(), (int)Fl::event_y());
            //fl_draw_box(_FL_DIAMOND_DOWN_BOX,0,0,(int)Fl::event_x(), (int)Fl::event_y(),FL_WHITE);
        }

        break;

    case FL_PUSH:
        if (Fl::event_button() == FL_RIGHT_MOUSE) {
            Fl_Menu_Item rclick_menu[] = {
                { "Zoom +", 0, zoomDec, (void*)this },
                { "AutoScale", 0, autoScale, (void*)this },
                { "Zoom -", 0, zoomInc, (void*)this },
                { 0 }
            };
            const Fl_Menu_Item *m = rclick_menu->popup(Fl::event_x(), Fl::event_y(), 0, 0, 0);
            if (m)
                m->do_callback(0, m->user_data());
            return(1);          // (tells caller we handled this event)
        }
        return(1);

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
        damage(FL_DAMAGE_USER1);
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
    sprintf(s, "x:%d y:%d", (int)Fl::event_x(), (int)Fl::event_y());
    // Black rect
//     fl_color(FL_BLACK);
//     fl_rectf(10, 10, 200, 25);
    // White text
    fl_color(FL_WHITE);
    fl_font(FL_HELVETICA, 10);
    fl_draw(s, 15, 15);
}



void PlotxyFLTK::draw() {
    int wd = w();
    int ht = h();
    int xo = x();
    int yo = y();

    fl_color(FL_BLACK);
    fl_rectf(xo, yo, wd, ht);
    fl_color(FL_WHITE);

    //Plot axis
    fl_line_style(FL_JOIN_BEVEL, 3);
    fl_line(0, ht / 2, wd, ht / 2);
    fl_line(3, 0, 3, ht);
    fl_line_style(FL_JOIN_BEVEL, 1);

    fl_push_matrix();
    fl_translate(translate_x, translate_y);
    fl_scale(wd / this->scale_factor, ht / this->scale_factor);

    fl_begin_line();

    for (int i = 0; i < view_width; i++) {
        if (i == view_break) {
            fl_end_line();
            fl_begin_line();
        }

        fl_vertex(((float)i / (float)view_width), view[i]);
        fl_color(FL_RED);

        fl_color(FL_GREEN);

//         if((i-20)>0)
//             fl_vertex(((float)i / (float)view_width), view[i-20]);
    }
    fl_end_line();

    fl_pop_matrix();

    draw_coords();
} /* end of draw() method */

// void PlotFLTK::update_scope(void *)
// {
//         for (int i = 0; i < 7; i++) {
//                 view[view_pos] = trace[trace_pos];
//                 trace_pos++;
//                 if(trace_pos >= trace_max) trace_pos = 0;
//                 view_pos++;
//                 if(view_pos >= view_width) view_pos = 0;
//         }
//         view_break = view_pos;
//         Fl::repeat_timeout(0.05, update_scope);
//         this->redraw();
// }



void PlotxyFLTK::insertValuesToPlot(float* value, int nvalue) {
    if (view == NULL) {
        view = new float[nvalue];
    }
    for (int i = 0; i < nvalue; i++) {

        view[i] = - value[i]; //minus sign is necessary to plot a correct graph
                this->vievedMaxValue = getMaxValue(view[i], this->vievedMaxValue);
        this->vievedMinValue = getMinValue(view[i], this->vievedMinValue);
    }
    this->scale_factor = fabs(this->vievedMaxValue) + fabs(this->vievedMinValue);

    cout << "Max Value:" << fabs(this->vievedMaxValue) << endl;
    cout << "Min Value:" << fabs(this->vievedMinValue) << endl;
    cout << "Scale factor:" << this->scale_factor << endl;
}

void PlotxyFLTK::insertValueToPlot(float value) {
    insertInTail(value);
    this->redraw();
}

void PlotxyFLTK::insertInTail(float element) {
    this->shift_vector_left(element);
    this->view[view_width-1] = element;
}

void PlotxyFLTK::shift_vector_left(float element) {
    int i = 0;
    for (i = 1;i < view_width;i++) {
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




// kate: indent-mode cstyle; space-indent on; indent-width 4; replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;

