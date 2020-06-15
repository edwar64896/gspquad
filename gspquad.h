#ifndef GSP_QUAD
#define GSP_QUAD

#include "Arduino.h"
#include "gspgrouped.h"

class gspQuad : 
    public gspGrouped {
    
    public:

        static gspQuad * makeOne (
            int _pin1,
            int _pin2,
            void (* _cbLower)(int dist),
            void (* _cbHigher)(int dist)        
        ) {
            gspQuad * instance = new gspQuad(_pin1,_pin2,_cbLower,_cbHigher);
            gspGrouped::register_instance(instance);
            return instance;
        }

	    gspQuad(int pin1, int pin2, void (* cbLower)(int dist), void (* cbHigher)(int dist));
        bool check();
        void reset();

        static void checkAll() {
            gspGrouped::checkAll(gspQuad::firstInstance);
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

        inline int _readpins(void) {
            return digitalRead(_pin2) << 1 | digitalRead(_pin1);
        }

    protected:
        void setFirstInstance(gspGrouped * fi) {gspQuad::firstInstance = fi;}
        gspGrouped * getFirstInstance() {return gspQuad::firstInstance;}
        static gspGrouped * firstInstance;   

    private:

        void (* _cbLower)(int);
        void (* _cbHigher)(int);

        int _curPos;
        int _oldPos;

        int _readpins(void);
        int _pin1, _pin2;

        volatile int _position;

        int _min, _max;
        bool _usingmin, _usingmax;

        volatile int _previous;


};
#endif