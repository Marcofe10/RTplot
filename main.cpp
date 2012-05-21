#include <iostream>
#include <math.h>
#include"plotxyfltk.h"
#define N 1024
#define PLOTTEDN 512

using namespace std;
static PlotxyFLTK *scope = 0;
int k;

void update_scope(void *valori) {
    float *val =(float*)valori;
    scope->insertValueToPlot(val[k]);
    k++;
    Fl::repeat_timeout(0.01, update_scope,valori);
}

int main(int argc, char **argv) {
    int i;
    float valori[N];
    float j;

//     float *valori;
//     valori= new float[512];

    Fl_Double_Window *main_win = new Fl_Double_Window(800, 600, "Scope Window");


    main_win->begin();
    scope = new PlotxyFLTK(0, 0, 800, 600);
    main_win->end();

    main_win->show(argc, argv);

    for (int i = 0; i < N; i++) {
        valori[i] =5*` sin((double)(i * 5 * 2 * M_PI / (double)N));
    }
    k=i;
//     for (i = 0,j=0.0;i < 2*N;j+=0.1,i++) {
//         valori[i] = j;
//     }
    scope->insertValuesToPlot(valori, PLOTTEDN);
    cout<<"ok"<<endl;
    Fl::add_timeout(0.5, update_scope, valori);

    return Fl::run();
}
// kate: indent-mode cstyle; space-indent on; indent-width 4; replace-tabs on;  replace-tabs on;  replace-tabs on;
