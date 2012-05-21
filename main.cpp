#include <iostream>
#include"plotxyfltk.h"
#define N 512

int main(int argc, char **argv) {
    int i;
    float valori[N];
    float j;
    
//     float *valori;
//     valori= new float[512];

    Fl_Double_Window *main_win = new Fl_Double_Window(800, 600, "Scope Window");
    static PlotxyFLTK *scope = 0;

    main_win->begin();
    scope = new PlotxyFLTK(5, 5, 790, 590);
    main_win->end();

    main_win->show(argc, argv);
    
    for (i = 0,j=0.0;i < N;j+=0.1,i++) {
        valori[i] = j;
    }
    scope->insertValueToPlot(valori, N);
    
    return Fl::run();
    return 0;
}
// kate: indent-mode cstyle; space-indent on; indent-width 4; replace-tabs on; 
