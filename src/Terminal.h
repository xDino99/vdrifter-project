#pragma once
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <iostream>

class Terminal {
    struct termios original;
    int w, h;

public:
    Terminal();
    ~Terminal();
    void updateSize();
    int width() const;
    int height() const;
    static bool kbhit();
    static char getch();
    void clearScreen();
    void hideCursor();
    void showCursor();
};