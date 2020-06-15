#include "gspquad.h"

gspGrouped * gspQuad::firstInstance=nullptr;

const int _half [4][4] = {
    { 0, -1, 1, 0 },		//  00 -> 10 is CW
    { 1, 0, 0, -1 },		//  01 -> 00 is CW
    { -1, 0, 0, 1 },		//  10 -> 11 is CW
    { 0, 1, -1, 0 }		//  11 -> 01 is CW
};

const int _full [4][4] = {
    { 0, 0, 0, 0 },		//  00 -> 10 is silent CW
    { 1, 0, 0, -1 },		//  01 -> 00 is CW
    { -1, 0, 0, 1 },		//  10 -> 11 is CW
    { 0, 0, 0, 0 }		//  11 -> 01 is silent CW
};

gspQuad::gspQuad(
    int pin1, int pin2, 
    void (* cbLower)(int dist), void (* cbHigher)(int dist)
):
gspGrouped(),
_pin1(pin1), _pin2(pin2), 
_position(0),
_min(0), _max(0), 
_usingmin(0), _usingmax(0),
_cbLower(cbLower), _cbHigher(cbHigher)
 {
    pinMode(pin1, INPUT);
    pinMode(pin2, INPUT);
    digitalWrite(pin1, HIGH);		//  activate internal pullups
    digitalWrite(pin2, HIGH);

    _previous = _readpins();		//  read initial position

    // TODO: move this to gspGrouped
    // what's the thought about using interrupts for everything?
    //TIMSK2 |= (1 << TOIE2);		//  enable timer 2 overflow interrupt
}

bool gspQuad::_isr() {

    int quadbits = _readpins();

    if (quadbits != _previous) {
        int position = _position +
            _full[_previous][quadbits];

        //  limit to minimum if assigned
        position = _usingmin ? 
            max(_min, position) : position;

        //  limit to maximum if assigned
        _position = _usingmax ?
            min(_max, position) : position;

        _previous = quadbits;
    }

    _curPos = (position() / 2);
    if (_curPos != _oldPos) {
        int _difPos= _curPos-_oldPos;
        if (_difPos>0) {
            _cbHigher(_difPos);
        } else {
             _cbLower(_difPos);
        }
        if (position() > 100) { // zero the rotary encoder count if too high or low
            position(0);
        }
        if (position() < -100) {
            position(0);
        }
        _oldPos=_curPos;
      }
      return true;
}

//void gspQuad::reset(){}