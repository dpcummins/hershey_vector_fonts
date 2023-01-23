#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "hershey_font.h"

#define WIDTH 720
#define HEIGHT 360
#define HSIZE 2

void draw_hchar(Display* display, Window* win, GC* gc, struct hchar* hc, int* pos) {

    int i;
    int C = HSIZE;
    int x1, y1, x2, y2;

    for(i=0; i<hc->nvert-1; i++) {
        if((hc->cp[i+1][0] == -50) && (hc->cp[i+1][1] == 0)) {
            i++;
        } else {
            x1 = hc->cp[i][0]-hc->lhp;
            y1 = hc->cp[i][1];
            x2 = hc->cp[i+1][0]-hc->lhp;
            y2 = hc->cp[i+1][1];
            XDrawLine(display, *win, *gc, pos[0]+C*x1, pos[1]+C*y1,
                pos[0]+C*x2, pos[1]+C*y2);
        }
    }
}

void draw_hstring(Display* display, Window* win, GC* gc, struct hfont* hf, char* string, int* pos) {

    int w, h;
    int nchars = strlen(string);
    int C = HSIZE;

    for(int i=0; i<nchars; i++) {
        if((int)string[i]-32 < 0 || (int)string[i]-32 > 95) {
            printf("Invalid character %c\n", (int)string[i]-32);
            draw_hchar(display, win, gc, &(hf->hchars[0]), pos);
            w = hf->hchars[0].rhp - hf->hchars[0].lhp;
            pos[0] += C*w;
        } else {
//            printf("Drawing character ASCII %d\n", (int)string[i]-32);
            draw_hchar(display, win, gc, &(hf->hchars[(int)string[i]-32]), pos);
            w = hf->hchars[(int)string[i]-32].rhp -
                hf->hchars[(int)string[i]-32].lhp;
            pos[0] += C*w;
        }
    }
}

void draw_title(Display* display, Window* win, GC* gc, struct hfont* hf, char* title) {

    int w, h;
    int nchars = strlen(title);
    int C = HSIZE;
    int pos[2] = {0, 0};

    get_hstring_extent(hf, C, title, &w, &h);

    pos[0] = WIDTH*HSIZE/2 - w/2;
    pos[1] = h+20*C;

    draw_hstring(display, win, gc, hf, title, pos);
}

void draw_hfont(Display* display, Window* win, GC* gc, struct hfont* hf) {

    int w, h;
    int nchars = 96;
    int C = HSIZE;
    int pos[] = {8*C, 100*C};
    int init_xpos = pos[0];
    int cell_width = 44*C;
    int cell_height = 35*C;
    int chars_per_row = 16;

    for(int i=0; i<nchars; i++) {
        w = hf->hchars[i].rhp - hf->hchars[i].lhp;
        pos[0] = init_xpos + (i%chars_per_row)*cell_width;
        pos[0] += cell_width/2;
        pos[0] -= C*w/2;
        draw_hchar(display, win, gc, &(hf->hchars[i]), pos);
        if(i%chars_per_row == (chars_per_row-1)) {
            pos[1] += cell_height;
        }
    }
}

void draw_qwerty(Display* display, Window* win, GC* gc, struct hfont* hf) {

    char* qwerty[4] = {"`1234567890-=", "qwertyuiop[]", "asdfghjkl;'#", "\\zxcvbnm,./"};
    int pos[2] = {20, 100};

    for(int i=0; i<4; i++) {
        draw_hstring(display, win, gc, hf, qwerty[i], pos);
        pos[0] = 20;
        pos[1] += 35;
    }
}

void draw_shift_qwerty(Display* display, Window* win, GC* gc, struct hfont* hf) {

    char* shift_qwerty[4] = {"¬!\"£$%^&*()_+", "QWERTYUIOP{}", "ASDFGHJKL:@~", "|ZXCVBNM<>?"};
    int pos[2] = {380, 100};

    for(int i=0; i<4; i++) {
        draw_hstring(display, win, gc, hf, shift_qwerty[i], pos);
        pos[0] = 380;
        pos[1] += 35;
    }
}

void save_to_file(Display* display, Window win, GC gc) {
    int w = WIDTH*HSIZE;
    int h = HEIGHT*HSIZE;
    char fname[20];
    sprintf(fname, "%s", "testbmp.bmp");
    Pixmap bmp = XCreatePixmap(display, win, w, h,
        DefaultDepth(display, DefaultScreen(display)));
    XCopyArea(display, win, bmp, gc, 0, 0, w, h, 0, 0);
    XWriteBitmapFile(display, fname, bmp, w, h, -1, -1);
    XFreePixmap(display, bmp);
}

void close_x(Display* display, Window win, GC gc) {
    save_to_file(display, win, gc);
    XFreeGC(display, gc);
    XDestroyWindow(display, win);
    XCloseDisplay(display);
    exit(1);
}

int main(int argc, char* argv[]) {

    struct hfont hf;
    get_hfont(&hf, argv[1]);

    Display* display;
    int screen;
    Window win;
    GC gc;

    unsigned long black, white;
    XEvent event;
    KeySym key;
    char text[255];

    XTextProperty tp1, tp2;
    char* prog_name = "Hershey vector fonts";
    char* icon_name = "Testing";
    XStringListToTextProperty(&prog_name, 1, &tp1);

    display = XOpenDisplay((char *)0);
    if(display == NULL) {
        printf("Error connecting to X server\n");
        exit(-1);
    }

    screen = DefaultScreen(display);
    black = BlackPixel(display, screen);
    white = WhitePixel(display, screen);

    win = XCreateSimpleWindow(display, XDefaultRootWindow(display),
        0, 0, WIDTH*HSIZE, HEIGHT*HSIZE, 4, black, white);

    XSetWMName(display, win, &tp1);
    XStringListToTextProperty(&icon_name, 1, &tp2);
    XSetWMIconName(display, win, &tp2);
    XMapWindow(display, win);
    XSelectInput(display, win, KeyPressMask | ButtonPressMask | ExposureMask);

    gc = XCreateGC(display, win, 0, 0);
    XSetLineAttributes(display, gc, 1, LineSolid, CapRound, JoinRound);

    while(1) {
        XNextEvent(display, &event);
        if(event.type == Expose && !event.xexpose.count) {
            draw_title(display, &win, &gc, &hf, argv[1]);
//            draw_qwerty(display, &win, &gc, &hf);
//            draw_shift_qwerty(display, &win, &gc, &hf);
            draw_hfont(display, &win, &gc, &hf);
        }
        if(event.type==KeyPress && XLookupString(&event.xkey, text, 255, &key, 0)) {
            if(text[0]=='q') {
                close_x(display, win, gc);
            }
        }
    }
    return(0);
}
