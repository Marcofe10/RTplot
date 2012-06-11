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

WinPlotXYFLTK::WinPlotXYFLTK(int X, int Y, int W, int H, const char* l): Fl_Double_Window(X, Y, W, H, l) {

    this->X = x();
    this->Y = y();
    this->W = w();
    this->H = h();

    int xw = x() + 1;
    int yw = y() + 1;
    int ww = w() - 150;
    int hw = h() - 1;


    this->plot = new PlotxyFLTK(xw, yw, ww, hw);
    //this->plot->show();
    this->box(FL_PLASTIC_UP_BOX);

    this->buttonZoomXInc = new Fl_Button(ww + 2, 10, BUTTON_SIZE, 20, "ZoomX +");
    this->buttonZoomXDec = new Fl_Button(ww + 2, 30, BUTTON_SIZE, 20, "ZoomX -");
    this->valueOutputZoomX = new Fl_Value_Output(ww + BUTTON_SIZE + 5, 20, 45, 20);

    this->buttonZoomYInc = new Fl_Button(ww + 2, 70, BUTTON_SIZE, 20, "ZoomY +");
    this->buttonZoomYDec = new Fl_Button(ww + 2, 90, BUTTON_SIZE, 20, "ZoomY -");
    this->valueOutputZoomY = new Fl_Value_Output(ww + BUTTON_SIZE + 5, 80, 45, 20);

    this->buttonTranslateYUp = new Fl_Button(ww + 2, 130, BUTTON_SIZE, 20, "Translate+");
    this->buttonTranslateYDown = new Fl_Button(ww + 2, 150, BUTTON_SIZE, 20, "Translate-");
    this->valueOutputTranslateY = new Fl_Value_Output(ww + BUTTON_SIZE + 5, 140, 45, 20);

    this->checkButtonAutoScale = new Fl_Check_Button(ww + 2, 190, 110, 20, "Auto Zoom");

    this->valueOutputTimeSimulations = new Fl_Value_Output(ww + 2, 300, 110, 20, "Auto Zoom");

    this->checkButtonFullScreen = new Fl_Check_Button(ww + 2, 330, 110, 20, "FullScreen");

    /***CALLBACKS***/
    this->buttonZoomXDec->callback((Fl_Callback *) this->zoomXDec, this);
    this->buttonZoomXInc->callback((Fl_Callback *) this->zoomXInc, this);
    this->buttonZoomYDec->callback((Fl_Callback *) this->zoomYDec, this);
    this->buttonZoomYInc->callback((Fl_Callback *) this->zoomYInc, this);
    this->buttonTranslateYDown->callback((Fl_Callback *) this->translateYDown, this);
    this->buttonTranslateYUp->callback((Fl_Callback *) this->translateYUp, this);
    this->checkButtonAutoScale->callback((Fl_Callback *) this->zoomAuto, this);
    this->checkButtonFullScreen->callback((Fl_Callback *) this->fullScreen, this);


    /***Properties of many buttons***/
    this->buttonZoomXDec->box(FL_PLASTIC_UP_BOX);
    this->buttonZoomYDec->box(FL_PLASTIC_UP_BOX);
    this->buttonZoomXInc->box(FL_PLASTIC_UP_BOX);
    this->buttonZoomYInc->box(FL_PLASTIC_UP_BOX);
    this->buttonTranslateYDown->box(FL_PLASTIC_UP_BOX);
    this->buttonTranslateYUp->box(FL_PLASTIC_UP_BOX);
    this->checkButtonAutoScale->box(FL_PLASTIC_UP_BOX);

    this->plot->setAutoZoom(false);

    /*** Initial Values***/
    this->valueOutputTranslateY->value(plot->getTranslateYValue());
    this->valueOutputZoomX->value(plot->getZoomXValue());
    this->valueOutputZoomY->value(plot->getZoomYValue());
}

void WinPlotXYFLTK::draw() {
    this->valueOutputZoomX->value(this->plot->getZoomXValue());
    this->valueOutputZoomY->value(this->plot->getZoomYValue());
    this->valueOutputTranslateY->value(this->plot->getTranslateYValue());
//     this->plot->redraw();
     Fl_Window::draw();
}


void WinPlotXYFLTK::insertValueToPlot(float value) {
    this->plot->insertValueToPlot(value);

    this->valueOutputZoomX->value(this->plot->getZoomXValue());
    this->valueOutputZoomY->value(this->plot->getZoomYValue());
    this->valueOutputTranslateY->value(this->plot->getTranslateYValue());
}

int WinPlotXYFLTK::insertValuesToPlot(float* value, int nvalue) {
    int rit;
    rit=this->plot->insertValuesToPlot(value, nvalue);
    this->plot->redraw();
    return rit;
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

void WinPlotXYFLTK::setAutoZoom(bool value) {
    this->checkButtonAutoScale->value(value);
    this->plot->setAutoZoom(value);

    if (this->checkButtonAutoScale->value()) {
        this->checkButtonAutoScale->set();

        this->buttonTranslateYDown->deactivate();
        this->buttonTranslateYUp->deactivate();
//         this->buttonZoomXDec->deactivate();
//         this->buttonZoomXInc->deactivate();
        this->buttonZoomYDec->deactivate();
        this->buttonZoomYInc->deactivate();
    } else {
        this->checkButtonAutoScale->clear();

        this->buttonTranslateYDown->activate();
        this->buttonTranslateYUp->activate();
//         this->buttonZoomXDec->activate();
//         this->buttonZoomXInc->activate();
        this->buttonZoomYDec->activate();
        this->buttonZoomYInc->activate();
    }

    this->redraw();
}

void WinPlotXYFLTK::setViedWidth(int value)
{
    plot->setViedWidth(value);
}

int WinPlotXYFLTK::getSimulationSeconds()
{
    this->plot->getSimulationSeconds();
}


/* * * STATIC FUNCTION * * */

void WinPlotXYFLTK::zoomXDec(Fl_Widget* widget, void* userdata) {
    WinPlotXYFLTK *in = (WinPlotXYFLTK*)userdata;
    in->plot->zoomXDec();
    in->redraw();
}

void WinPlotXYFLTK::zoomXInc(Fl_Widget* widget, void* userdata) {
    WinPlotXYFLTK *in = (WinPlotXYFLTK*)userdata;
    in->plot->zoomXInc();
    in->redraw();
}

void WinPlotXYFLTK::zoomYDec(Fl_Widget* widget, void* userdata) {
    WinPlotXYFLTK *in = (WinPlotXYFLTK*)userdata;
    in->plot->zoomYDec();
    in->redraw();
}

void WinPlotXYFLTK::zoomYInc(Fl_Widget* widget, void* userdata) {
    WinPlotXYFLTK *in = (WinPlotXYFLTK*)userdata;
    in->plot->zoomYInc();
    in->redraw();
}

void WinPlotXYFLTK::zoomAuto(Fl_Widget* widget, void* userdata) {
    WinPlotXYFLTK *in = (WinPlotXYFLTK*)userdata;
    cout << "Value:" << in->checkButtonAutoScale->value() << endl;

    if (in->checkButtonAutoScale->value()) {
        in->checkButtonAutoScale->set();

        in->buttonTranslateYDown->deactivate();
        in->buttonTranslateYUp->deactivate();
//         in->buttonZoomXDec->deactivate();
//         in->buttonZoomXInc->deactivate();
        in->buttonZoomYDec->deactivate();
        in->buttonZoomYInc->deactivate();
    } else {
        in->checkButtonAutoScale->clear();

        in->buttonTranslateYDown->activate();
        in->buttonTranslateYUp->activate();
//         in->buttonZoomXDec->activate();
//         in->buttonZoomXInc->activate();
        in->buttonZoomYDec->activate();
        in->buttonZoomYInc->activate();
    }
    
    in->plot->zoomAuto();
}

void WinPlotXYFLTK::translateYDown(Fl_Widget* widget, void* userdata) {
    WinPlotXYFLTK *in = (WinPlotXYFLTK*)userdata;
    in->plot->translateYDown();
    in->redraw();
}
void WinPlotXYFLTK::translateYUp(Fl_Widget* widget, void* userdata) {
    WinPlotXYFLTK *in = (WinPlotXYFLTK*)userdata;
    in->plot->translateYUp();
    in->redraw();
}

void WinPlotXYFLTK::fullScreen(Fl_Widget* widget, void* userdata) {
    WinPlotXYFLTK *in = (WinPlotXYFLTK*)userdata;
    int xw, yw, ww, hw;

    if (in->checkButtonFullScreen->value()) {     
        in->checkButtonFullScreen->set();

        in->fullscreen();  
        
        xw = in->x() + 1;
        yw = in->y() + 1;
        ww = in->w() - 150;
        hw = in->h() - 1;

        in->plot->resize(xw, yw, ww, hw);
        in->buttonZoomXInc->resize(ww + 2, 10, BUTTON_SIZE, 20);
        in->buttonZoomXDec->resize(ww + 2, 30, BUTTON_SIZE, 20);
        in->valueOutputZoomX->resize(ww + BUTTON_SIZE + 5, 20, 45, 20);
        in->buttonZoomYInc->resize(ww + 2, 70, BUTTON_SIZE, 20);
        in->buttonZoomYDec->resize(ww + 2, 90, BUTTON_SIZE, 20);
        in->valueOutputZoomY->resize(ww + BUTTON_SIZE + 5, 80, 45, 20);
        in->buttonTranslateYUp->resize(ww + 2, 130, BUTTON_SIZE, 20);
        in->buttonTranslateYDown->resize(ww + 2, 150, BUTTON_SIZE, 20);
        in->valueOutputTranslateY->resize(ww + BUTTON_SIZE + 5, 140, 45, 20);
        in->checkButtonAutoScale->resize(ww + 2, 190, 110, 20);
        in->valueOutputTimeSimulations->resize(ww + 2, 300, 110, 20);
        in->checkButtonFullScreen->resize(ww + 2, 330, BUTTON_SIZE, 20);
    } else {
        in->checkButtonFullScreen->clear();
        
        in->fullscreen_off(in->X,in->Y,in->W,in->H);
        
        xw=in->X+1;
        yw=in->Y+1;
        ww=in->W-150;
        hw=in->H-1;
        
        in->plot->resize(xw, yw, ww, hw);
        in->buttonZoomXInc->resize(ww + 2, 10, BUTTON_SIZE, 20);
        in->buttonZoomXDec->resize(ww + 2, 30, BUTTON_SIZE, 20);
        in->valueOutputZoomX->resize(ww + BUTTON_SIZE + 5, 20, 45, 20);
        in->buttonZoomYInc->resize(ww + 2, 70, BUTTON_SIZE, 20);
        in->buttonZoomYDec->resize(ww + 2, 90, BUTTON_SIZE, 20);
        in->valueOutputZoomY->resize(ww + BUTTON_SIZE + 5, 80, 45, 20);
        in->buttonTranslateYUp->resize(ww + 2, 130, BUTTON_SIZE, 20);
        in->buttonTranslateYDown->resize(ww + 2, 150, BUTTON_SIZE, 20);
        in->valueOutputTranslateY->resize(ww + BUTTON_SIZE + 5, 140, 45, 20);
        in->checkButtonAutoScale->resize(ww + 2, 190, 110, 20);
        in->valueOutputTimeSimulations->resize(ww + 2, 300, 110, 20);
        in->checkButtonFullScreen->resize(ww + 2, 330, BUTTON_SIZE, 20);
    }
    in->plot->redraw();
    in->redraw();
}

/* * * E N D STATIC FUNCTION  * * */


