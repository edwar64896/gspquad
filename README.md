# gspquad

## introduction 

gspquad is a simplified implementation of a quadrature encoder interface, allowing similar implementation style to other similar
gsp classes when using arduino.

## how to use

It is requried to instantiate gspquad with 4 parameters:

1. Quad pin 1
2. Quad pin 2
3. Callback for Incremental turn (turn right)
4. Callback fro Decremental turn (turn left)
...or
3. Stream output string for Increments
4. Stream output string for Decrements

For instance:

```
gspQuad myEncoder(1,2,"GoingUp","GoingDown");
```
or...
```
int pos=0;
void showPos(){Serial.println(pos);}

void doUp(){pos++;showPos()};
void doDown(){pos--;showPos();};

gspQuad myOtherEncoder(1,2,doUp,doDown);

```
## other requirements

1. include the library header file <gspquad.h>
2. include the library dependencies in the project: gspquad,gspgrouped,functional-avr


The only other requirements for code is to "register" each Quad instance with gspgrouped in the setup function and to then
start the checking process by calling gspQuad::startCheckAll().

This checking process is slightly different with gspQuad as we use interrupts to drive the encoder so we don't check it in the loop
function.

```
void setup() {
  gspGrouped::register_instance(&myEncoder);
  gspGrouped::register_instance(&myOtherEncoder);
  gspQuad::startCheckAll();
}
void loop() {
}
```
