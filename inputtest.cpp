#include <ncurses.h>
#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;

const int K_ENTER = 10;
const int K_ESC = 27;
const int K_SPACE = 32;
const int K_BACKSPACE = 127;
const int K_TAB = 9;

void main_menu();
void type();
void tab_menu();



int y, x;



int main(){
    vector<string> str;
    str.push_back("");
    str[0].push_back(char(65));
    cout << str[0] << endl;
    return 0;
}
