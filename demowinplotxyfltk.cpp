#include <iostream>
#include <math.h>
#include"winplotxyfltk.h"

#define N 2048
#define PLOTTEDN 512

using namespace std;
static WinPlotXYFLTK *main_win = 0;
// static PlotxyFLTK *scope1 =0;
int k;

void update_scope(void *valori) {
    float *val =(float*)valori;
    main_win->insertValueToPlot(val[k]);
    k++;
    Fl::repeat_timeout(0.01, update_scope,valori);
}

int main(int argc, char **argv) {
    int i;
    float valori[10*N];
    float j;

//     float *valori;
//     valori= new float[512];

    main_win = new WinPlotXYFLTK(0,0,800, 600, "Scope Window");
    main_win->show();

    for (i = 0; i < 10*N; i++) {
        
        if(i>10)
        valori[i] =3* sin((double)(i * 5 * 2 * M_PI / (double)N));
        else
            valori[i]=0;
    }
    cout<<"Valori Generati"<<endl;
    
    Fl::add_timeout(0.5, update_scope, valori);
    return Fl::run();
}
// kate: indent-mode cstyle; space-indent on; indent-width 4; replace-tabs on;  replace-tabs on;  replace-tabs on;
