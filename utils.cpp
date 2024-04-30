#include <bits/stdc++.h>
#include <ncurses.h>

using namespace std;

void mvprintcenter(int y, int x, string s){
    int len = s.size();
    int pos = x - len/2;
    if(pos<0) pos = 0;
    mvprintw(y, pos, s.c_str());
}