#ifndef GSP_QUAD
#define GSP_QUAD

#include "Arduino.h"
#include "gspgrouped.h"

#define GSPQUAD_MODE_STR 1
#define GSPQUAD_MODE_CB  2

class gspQuad : 
    public gspGrouped {
    
    public:

	    gspQuad(int pin1, int pin2, void (* cbLower)(int dist), void (* cbHigher)(int dist));
        gspQuad(int pin1, int pin2, const char * doDecrement, const char * doIncrement);


        bool _isr();

        static void startCheckAll() {
            gspGrouped::_isr_checking = true;
            gspGrouped::setInitialInstance(gspQuad::firstInstance);
            gspGrouped::startTimer();
        }    

        inline void minimum(int min) {
            _min = min;
            _usingmin = 1;

            //  adjust position if lower than new minimum
            _position = max(_position, min);
        }

        inline int minimum() {
            return _min;
        }

        inline void maximum(int max) {
            _max = max;
            _usingmax = 1;

            //  adjust position if higher than new maximum
            _position = min(_position, max);
        }

        inline int maximum() {
            return _max;
        }

        inline void nominimum(void) {
            _usingmin = 0;
        }

        inline void nomaximum(void) {
            _usingmax = 0;
        }


        inline int position(void) {
            return _position;
        }

        inline void position(int pos) {
            _position = pos;
        }


    protected:
        void setFirstInstance(gspGrouped * fi) {gspQuad::firstInstance = fi;}
        gspGrouped * getFirstInstance() {return gspQuad::firstInstance;}
        static gspGrouped * firstInstance;   

    private:

        void (* _cbLower)(int);
        void (* _cbHigher)(int);

        inline int _readpins(void) {
            return digitalRead(_pin2) << 1 | digitalRead(_pin1);
        }

        int _curPos, _oldPos;
        int _pin1, _pin2;

        const char * _szDecrement, * _szIncrement;

        volatile int _position, _previous;

        int _min, _max;
        bool _usingmin, _usingmax;

        int operationmode=0;


};
#endif