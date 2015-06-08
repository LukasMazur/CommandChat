#include "Window.hpp"

using namespace std;
using namespace Client;

CWindow::CWindow ():
    window(nullptr, delwin)
{}

CWindow::CWindow (int lines, int cols, int y, int x):
    lines(lines ? lines : LINES - y),
    cols(cols ? cols : COLS - x),
    window(newwin(lines, cols, y, x), delwin)
{}

CWindow & CWindow::operator= (CWindow && other)
{
    if (this != &other) {
        swap(lines, other.lines);
        swap(cols, other.cols);
        swap(window, other.window);
    }
    
    return *this;
}

void CWindow::Refresh (void) const
{
    box(window.get(), '|', '-');
    wnoutrefresh(window.get());
}

const CWindow & CWindow::MoveTo (int line, int col) const
{
    POS pos {min(max(line, 0), lines), min(max(col, 0), cols)};
    
    wmove(window.get(), pos.line, pos.col);
    
    return *this;
}

const CWindow & CWindow::MoveTo (DIR dir) const
{
    auto pos = GetPos();
    
    switch (dir) {
        case DIR::UP: return MoveTo(pos.line - 1, pos.col); 
        case DIR::DOWN: return MoveTo(pos.line + 1, pos.col);
        case DIR::LEFT: return MoveTo(pos.line, pos.col - 1);
        case DIR::RIGHT: return MoveTo(pos.line, pos.col + 1);
        case DIR::TOP: return MoveTo(1, pos.col);
        case DIR::BOTTOM: return MoveTo(lines - 2, pos.col);
        case DIR::START: return MoveTo(pos.line, 1);
        default: return *this;
    }
}

const CWindow & CWindow::Print (const string & str, int attr) const
{
    int width = cols - 2;
    int lines = (str.size() / width) + 1;
    int currentLine = GetPos().line;
    
    MoveTo(currentLine - lines, 1);
    for (int i = 0; i < lines; i++) {
        PrintLine(str.substr(i * width, width), attr);
    }
    
    return MoveTo(currentLine - lines, 1);
}

const CWindow &  CWindow::PrintLine (const string & str, int attr) const
{
    wattron(window.get(), attr);
    auto pos = GetPos();
    waddnstr(window.get(), str.c_str(), cols - pos.col);
    wattroff(window.get(), attr);
    
    return MoveTo(DIR::DOWN).MoveTo(DIR::START);
}

const CWindow &  CWindow::Print (char c, int attr) const
{
    waddch(window.get(), c | attr);
    
    return *this;
}

CWindow::POS CWindow::GetPos() const
{
    POS pos;
    getyx(window.get(), pos.line, pos.col);
    return pos;
}

void CWindow::EnableKeypad (bool enable) const
{
    keypad(window.get(), enable);
}

int CWindow::GetCh (void) const
{
    return wgetch(window.get());
}

void CWindow::Clear (void) const
{
    wclear(window.get());
    MoveTo(1, 1);
}
