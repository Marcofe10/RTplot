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


#include <iostream>
#include <math.h>
#include"winplotxyfltk.h"

#define N 2048
#define PLOTTEDN 512

using namespace std;
static WinPlotXYFLTK *main_win = 0;
// static WinPlotXYFLTK *main_win1 = 0;
// static PlotxyFLTK *scope1 =0;
int k;

void update_scope(void *valori) {
    int num;
    data_element *val = (data_element*)valori;

    main_win->insertValueToPlot(val[k].value, 64);
    k++;

//     if (random() % 2)
//         num = (random() % 2048) + 1;
//     else
//         num = -(random() % 2048) - 1;
//
//     main_win->insertValueToPlot(num, 64);


//         k += main_win->insertValuesToPlot(val, 300);


    //0.002
    Fl::repeat_timeout(0.1, update_scope, valori);
}

// void update_scope1(void *valori) {
//     float *val = (float*)valori;
//     main_win1->insertValueToPlot(val[k]);
//
//     k++;
//     Fl::repeat_timeout(0.01, update_scope1, valori);
// }

int main(int argc, char **argv) {
    int i;
    //float valori[10*N];
    data_element valori[10*N];
    float *valoriFloat;
    float j;
    srand(time(NULL));
    float value_time;
    int sample_time = 64;
    int fattore = 1;
    int nvalues = 2000;

//     float *valori;
//     valori= new float[512];

    main_win = new WinPlotXYFLTK(0, 0, 1000, 600, "Scope Window");
    main_win->setAutoZoom(true);
    main_win->show();

//     main_win1 = new WinPlotXYFLTK(0, 0, 800, 600, "Scope Window");
//     main_win1->show();

    valoriFloat = new float[nvalues];

    main_win->setSampleTime(sample_time);
    value_time = 1 / 64;
    for (i = 0; i < 10*N; i++) {

        valori[i].time = value_time * i;

        if (i < (N / 2)) {
            if (i % (N / 4))
                fattore++;
        } else
            fattore--;

        valori[i].value = fattore * (sin((double)(i * 5 * 2 * M_PI / (double)N)));
    }
    /*
        for (i = 0; i < 10*N; i++) {
            valori[i].time = value_time * i;
            if (sin((double)(i * 5 * 2 * M_PI / (double)N)) > 0)
                valori[i].value = sin((double)(i * 5 * 2 * M_PI / (double)N));
            else
                valori[i].value =7* sin((double)(i * 5 * 2 * M_PI / (double)N));

        }*/
    cout << "Valori Generati:" << 10*N << endl;
    k = 0;
    cout << "K:" << k << endl;
//    main_win->convertDataElementToFloat(valori, valoriFloat, nvalues);
//    k = main_win->insertValuesToPlot(valoriFloat, nvalues, sample_time);

    Fl::add_timeout(0.5, update_scope, valori);
    main_win->plotLine(0.5);
    return Fl::run();

}
