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
    int ww = W - 150;
    int hw = H;
    this->plot = new PlotxyFLTK(xw, yw, ww, hw);
    //this->plot->show();
    this->box(FL_PLASTIC_UP_BOX);

//     this->labelZoomX = new Fl_Label(ww+2,10,48,20);
    this->buttonZoomXInc = new Fl_Button(ww + 2, 30, 110, 20, "ZoomX +");
    this->buttonZoomXDec = new Fl_Button(ww + 2, 10, 110, 20, "ZoomX -");
    

//     this->labelZoomY = new Fl_Label();
    this->buttonZoomYInc = new Fl_Button(ww + 2, 90, 110, 20, "ZoomY +");
    this->buttonZoomYDec = new Fl_Button(ww + 2, 70, 110, 20, "ZoomY -");
    
    this->buttonTranslateYUp = new Fl_Button(ww + 2, 150, 110, 20, "Translate+");
    this->buttonTranslateYDown = new Fl_Button(ww + 2, 130, 110, 20, "Translate-");
    
    this->checkButtonAutoScale = new Fl_Check_Button(ww +2, 190, 110, 20,"Auto Zoom");

    /***CALLBACKS***/
    this->buttonZoomXDec->callback((Fl_Callback *) this->zoomXDec, this);
    this->buttonZoomXInc->callback((Fl_Callback *) this->zoomXInc, this);
    this->buttonZoomYDec->callback((Fl_Callback *) this->zoomYDec, this);
    this->buttonZoomYInc->callback((Fl_Callback *) this->zoomYInc, this);
     this->buttonTranslateYDown->callback((Fl_Callback *) this->translateYDown, this);
    this->buttonTranslateYUp->callback((Fl_Callback *) this->translateYUp, this);
    this->checkButtonAutoScale->callback((Fl_Callback *) this->zoomAuto, this);


    /***Properties of many buttons***/
    this->buttonZoomXDec->box(FL_PLASTIC_UP_BOX);
    this->buttonZoomYDec->box(FL_PLASTIC_UP_BOX);
    this->buttonZoomXInc->box(FL_PLASTIC_UP_BOX);
    this->buttonZoomYInc->box(FL_PLASTIC_UP_BOX);
    this->buttonTranslateYDown->box(FL_PLASTIC_UP_BOX);
    this->buttonTranslateYUp->box(FL_PLASTIC_UP_BOX);
    this->checkButtonAutoScale->box(FL_PLASTIC_UP_BOX);

    this->plot->setAutoZoom(false);
}

// void WinPlotXYFLTK::draw()
// {
//
//     Fl_Window::draw();
// }


void WinPlotXYFLTK::insertValueToPlot(float value) {
    this->plot->insertValueToPlot(value);
}

int WinPlotXYFLTK::insertValuesToPlot(float* value, int nvalue) {
    this->plot->insertValuesToPlot(value, nvalue);
}

void WinPlotXYFLTK::plotLine() {
    this->plot->plotLine();
}

void WinPlotXYFLTK::setXAxis(char* name) {
    this->plot->setXAxis(name);
}

void WinPlotXYFLTK::setYAxis(char* name) {
    this->plot->setYAxis(name);
}


void WinPlotXYFLTK::zoomXDec(Fl_Widget* widget, void* userdata) {
    WinPlotXYFLTK *in = (WinPlotXYFLTK*)userdata;
    in->plot->zoomXDec();
}

void WinPlotXYFLTK::zoomXInc(Fl_Widget* widget, void* userdata) {
    WinPlotXYFLTK *in = (WinPlotXYFLTK*)userdata;
    in->plot->zoomXInc();
}

void WinPlotXYFLTK::zoomYDec(Fl_Widget* widget, void* userdata) {
    WinPlotXYFLTK *in = (WinPlotXYFLTK*)userdata;
    in->plot->zoomYDec();
}

void WinPlotXYFLTK::zoomYInc(Fl_Widget* widget, void* userdata) {
    WinPlotXYFLTK *in = (WinPlotXYFLTK*)userdata;
    in->plot->zoomYInc();
}

void WinPlotXYFLTK::zoomAuto(Fl_Widget* widget, void* userdata) {
    WinPlotXYFLTK *in = (WinPlotXYFLTK*)userdata;
    cout << "Value:" << in->checkButtonAutoScale->value() << endl;

    if (in->checkButtonAutoScale->value()) {
        in->checkButtonAutoScale->set();
    } else {
        in->checkButtonAutoScale->clear();
    }

    in->plot->zoomAuto();
}
void WinPlotXYFLTK::translateYDown(Fl_Widget* widget, void* userdata) {
    WinPlotXYFLTK *in = (WinPlotXYFLTK*)userdata;
    in->plot->translateYDown();

}
void WinPlotXYFLTK::translateYUp(Fl_Widget* widget, void* userdata) {
    WinPlotXYFLTK *in = (WinPlotXYFLTK*)userdata;
    in->plot->translateYUp();
}

