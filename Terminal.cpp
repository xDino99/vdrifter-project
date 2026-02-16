#include "Terminal.h"
#include <fcntl.h>

Terminal::Terminal() {
    tcgetattr(STDIN_FILENO, &original);
    struct termios raw = original;
    raw.c_lflag &= ~(ECHO | ICANON);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    hideCursor();
    updateSize();
}

Terminal::~Terminal() {
    showCursor();
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original);
    clearScreen();
}

void Terminal::updateSize() {
    struct winsize ws;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    w = ws.ws_col;
    h = ws.ws_row;
}

int Terminal::width() const { return w; }
int Terminal::height() const { return h; }

bool Terminal::kbhit() {
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv) > 0;
}

char Terminal::getch() {
    char c = 0;
    if (read(STDIN_FILENO, &c, 1) < 0) return 0;
    return c;
}

void Terminal::clearScreen() {
    std::cout << "\033[2J\033[H";
}

void Terminal::hideCursor() {
    std::cout << "\033[?25l";
}

void Terminal::showCursor() {
    std::cout << "\033[?25h";
}