/* Simple Binary Clock          *
 * Copyright (c) 2017 Ryan Kerr */

#define F_CPU 1000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define S(P) (1<<P)

struct {
    unsigned long time;
    int bin;
    int hour;
    int min;
    int tmp;
} t;

struct {
    int mode;
    int btn;
} m;

void init() {
    
    DDRD     =  0xFF;
    DDRC     =  ~(S(PC6));
    
    PCMSK0  |=  S(PCINT1);
    PCICR   |=  S(PCIE0);
    
    OCR1A    =  0x3D08;
    TIMSK1   =  S(OCIE1A);
    TCCR1B  |=  S(WGM12) | S(CS10) | S(CS12);
    
    sei();
}

void display() {
    
    t.hour  =   t.time/3600;
    t.min   =   t.time/60%60;
    t.bin   =  (t.hour*100)+t.min;
    
    if(t.hour == 24)
        t.time = t.bin = 0;
    
    PORTC   =  (t.bin&0x1F00)>>8;
    PORTD   =   t.bin&0x00FF;
}

int main() {
    
    t.time = 0;
    m.mode = 0;
    m.btn  = 0;
    
    init();
    display();
    
    for(;;)
        _delay_ms(500);
}

ISR(PCINT0_vect) {
    
    m.btn = !m.btn;
    
    if(m.btn)
        m.mode = ++m.mode%4;
}

ISR(TIMER1_COMPA_vect) {
    
    PORTC  ^= S(PC5);
    t.time += (!m.mode?1:m.mode==1?3600:m.mode==2?1800:60);
    
    display();
}
