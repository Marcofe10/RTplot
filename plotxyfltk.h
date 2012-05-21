#ifndef PLOTFLTK_H
#define PLOTFLTK_H

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Menu_Item.H>

/*
 * Classe inerente alla parte grafica
 */
class PlotxyFLTK: public Fl_Box {
    private:
        int trace_max;//max number of values
        int view_width;//numbers showed

        int trace_pos;
        int view_pos;
        int view_break;

        float *trace;//all value
        float *view;//showed values
        int initial_x, initial_y;

        
        void insertInTail(float element);
        void shift_vector_left(float element);
        
        void draw();
        //Zoom static function
        static void zoomInc(Fl_Widget *widget, void *userdata);
        static void zoomDec(Fl_Widget *widget, void *userdata);
        //Autoscale Function
        static void scale(Fl_Widget *widget, void *userdata);
        //Autoscale graph during plot
        static void autoScaleBehaviour(Fl_Widget *widget, void *userdata);
        
        static void autoScale(Fl_Widget *widget, void *userdata);
        bool enableAutoScaleWhileGraph;
        
        //Many scale,translate parameters
        float scale_factor_x;
        float scale_factor_y;
        int translate_value;
        int translate_x,translate_y;
        
        float vievedMaxValue;
        float vievedMinValue;
        
        float getMaxValue(float,float );
        float getMinValue(float,float );

    protected:
        int handle(int e);
        // Draw mouse coords in small black rectangle
        void draw_coords();

    public:
        PlotxyFLTK(int xp, int yp, int wp, int hp, const char *lp = 0);
        ~PlotxyFLTK();
        //void update_scope(void *);
        void insertValuesToPlot(float *value, int nvalue);
        void insertValueToPlot(float value);
        void plotLine(float value);
        

};
// kate: indent-mode cstyle; space-indent on; indent-width 4; replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;
#endif
