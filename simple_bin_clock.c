/* Simple Binary Clock          *
 * Copyright (c) 2017 Ryan Kerr */

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

struct {
    int time;
    int bin;
    int hour;
    int min;
} s;

struct {
    int x;
    int y;
} dim;

void init() {
    s.bin = 2350;
    s.time = 23*3600+50*60;
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    nodelay(stdscr, 1);
    getmaxyx(stdscr, dim.y, dim.x);
}

void print() {
    s.hour = (s.time)/3600;
    s.min  = (s.time)/60%60;
    s.bin  = (s.hour*100)+s.min;
    if(s.bin == 2400) s.bin = s.time = 0;
    
    for(int i=0; i<12; i++)
        mvprintw(dim.y/2, i-6+dim.x/2, "%c", (((s.bin)>>(11-i))&1)?'1':'0');
    mvprintw(1,0, "%d   ", s.bin);
}

void loop() {
    
    int run = 1;
    
    for(;;) {
        switch(getch()) {
            case 'c':
            case ';':
                run = !run;
                
                if(run) {
                    nodelay(stdscr, 1);
                    mvprintw(0,0, "R");
                }
                else {
                    cbreak();
                    mvprintw(0,0, "P");
                }
                break;
                
            case '2':
                if(!run)
                    s.time += 1800;
                break;
                
            case '1':
                if(!run)
                    s.time += 3600;
                break;
                
            case '0':
                if(!run)
                    s.time=0;
                break;
                
            case KEY_RESIZE:
                endwin();
                init();
                break;
            case 'q':
                endwin();
                exit(0);
                break;
            default:
                if(run) {
                    s.time++;
                    usleep(10000);
                }
                break;
        }
        
        print();
    }
}

int main() {
    init();
    loop();
}
