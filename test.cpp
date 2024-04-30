#include "words.hpp"

void main_menu();
void make_test(vector<string>&, int);
void type();
int judge(vector<string>&, const vector<string>&, int, int);
void disp_singleword(vector<vector<int>>&, vector<string>&, const vector<string>&, int, int, int);
void disp_multi_oneline(vector<vector<int>>&, vector<string>&, const vector<string>&, int);
void finished(double, double);
double calcres(vector<vector<int>>&);
double calcwpm(vector<vector<int>>&, double);
void tab_menu();



int y, x;


int main(){
    initscr();
    noecho();
    //curs_set(0);
    start_color();
    getmaxyx(stdscr, y, x);
    init_pair(1, COLOR_CYAN, COLOR_BLACK); // color of correct
    init_pair(2, COLOR_RED, COLOR_BLACK); // color of wrong
    main_menu();
    endwin();
    return 0;
}


void make_test(vector<string>& test, int n){
    vector<string> tmp = words_list;
    random_shuffle(tmp.begin(), tmp.end());
    for(int i=0; i<n; i++){
        test.push_back(tmp[i]);
    }
}


void main_menu(){
    //noecho();
    //curs_set(0);
    chrono::system_clock::time_point start = chrono::system_clock::now();
    while(true){
        clear();
        mvprintw(y/2, x/2, "Type Type");
        int ch = getch();
        flushinp();
        if(ch == K_ESC){
            clear();
            endwin();
            exit(0);
        }
        else if(ch == K_ENTER || ch == K_SPACE) type();
        else if(ch == K_TAB) tab_menu();

    }
    return;
}

void type(){
    //noecho();
    //curs_set(0);
    chrono::system_clock::time_point start;
    vector<string> test;
    make_test(test, 10);
    vector<string> input;
    vector<vector<int>> res;
    int p = 0;
    int word_idx = 0;
    clear();
    input.push_back("");
    res.push_back(vector<int>());
    while(true){
        //disp_singleword(res, input, test, word_idx, y/2, x/2);
        curs_set(0);
        disp_multi_oneline(res, input, test, word_idx); // display words in one line
        curs_set(1);
        move(y/2, x/2+p);
        refresh();
        int ch = getch();
        //flushinp();

        if(ch==K_TAB){
            // open menu
            tab_menu();
        }else if(ch==K_BACKSPACE){
            // backspace
            if(p>0) {
                input[word_idx].pop_back();
                res[word_idx].pop_back();
                p--;
                }
        }else if(ch==K_SPACE || ch==K_ENTER){
            // next word
            if(word_idx==0 && p==0) start = chrono::system_clock::now(); // start timer
            else if(word_idx+1 < test.size()){
                input.push_back("");
                res.push_back(vector<int>());
                p = 0;
                word_idx++;
            }else{
                // finish
                chrono::system_clock::time_point end = chrono::system_clock::now(); // stop timer
                auto time = end - start;
                double msec =  chrono::duration_cast<chrono::milliseconds>(time).count();
                double wpm = calcwpm(res, msec);
                double result = calcres(res);
                finished(result, wpm);
                break;
            }
        }else if((ch>='A' && ch<='Z') || (ch>='a' && ch<='z')){
            // input alphabet
            if(word_idx==0 && p==0) start = chrono::system_clock::now(); // start timer
            input[word_idx].push_back(char(ch));
            res[word_idx].push_back(judge(input, test, word_idx, p));
            p++;
        }
    }
    return;
}

int judge(vector<string>& input, const vector<string>& test, int idx, int p){
    if(input[idx][p]==test[idx][p]) return 1; // correct
    else return 2; // wrong
}

void disp_singleword(vector<vector<int>>& res, vector<string>& input, const vector<string>& test, int idx, int y, int x){
    if(idx < input.size()){
        int si = input[idx].size();
        int st = test[idx].size();
        if(si>=st){
            for(int i=0; i<st; i++){
                attron(COLOR_PAIR(res[idx][i]));
                mvprintw(y, x+i, "%s", test[idx].substr(i,1).c_str());
                attroff(COLOR_PAIR(res[idx][i]));
            }
            attron(COLOR_PAIR(2));
            mvprintw(y, x+st, "%s", input[idx].substr(st).c_str());
            attroff(COLOR_PAIR(2));
            refresh();
        }else{
            for(int i=0; i<si; i++){
                attron(COLOR_PAIR(res[idx][i]));
                mvprintw(y, x+i, "%s", test[idx].substr(i,1).c_str());
                attroff(COLOR_PAIR(res[idx][i]));
            }
            mvprintw(y, x+si, "%s", test[idx].substr(si).c_str());
            refresh();
        }
    }else{
        mvprintw(y, x, "%s", test[idx].c_str());
        refresh();
    }
}

void disp_multi_oneline(vector<vector<int>>& res, vector<string>& input, const vector<string>& test, int idx){
    clear();
    disp_singleword(res, input, test, idx, y/2, x/2);
    int offset = 0;
    for(int i=idx-1; i>=0; i--){
        int si = input[i].size();
        int st = test[i].size();
        if(si>=st){
            offset += si+1;
            disp_singleword(res, input, test, i, y/2, x/2-offset);
        }else{
            offset += st+1;
            disp_singleword(res, input, test, i, y/2, x/2-offset);
        }
        
    }
    offset = max(input[idx].size(), test[idx].size())+1;
    for(int i=idx+1; i<test.size(); i++){
        disp_singleword(res, input, test, i, y/2, x/2+offset);
        refresh();
        offset += test[i].size()+1;
    }
    
}

void finished(double result, double wpm){
    clear();
    mvprintw(y/2, x/2, "finished!");
    mvprintw(y/2 + 1, x/2, "acc: %3.2f", result*100);
    mvprintw(y/2 + 2, x/2, "wpm: %5.2f", wpm);
    while(true){
        int ch = getch();
        if(ch==K_ENTER) main_menu();
    }
    return;
}

double calcres(vector<vector<int>>& res){
    double result = 0;
    int size = 0;
    for(int i=0; i<res.size(); i++){
        size += res[i].size();
        for(int j=0; j<res[i].size(); j++){
            if(res[i][j]==1) result++;
        }
    }
    if(size == 0) result = 0;
    else result /= size;
    return result;
}

double calcwpm(vector<vector<int>>& res, double msec){
    double wpm = 0;
    for(int i=0; i<res.size(); i++){
        bool ans = true;
        if(res[i].size()==0) ans = false;
        else{
           for(int j=0; j<res[i].size(); j++){
                if(res[i][j]!=1){
                    ans = false;
                    break;
                }
            } 
        }
        
        if(ans) wpm++;
    }
    
    wpm *= 60000 / msec;
    return wpm;
}

void tab_menu(){
    //noecho();
    //curs_set(0);
    
    while(true){
        clear();
        mvprintw(y/2, x/2, "enter to restart");
        mvprintw(y/2 + 2, x/2, "esc to return");
        int ch = getch();
        flushinp();
        if(ch == K_ESC){
            clear();
            endwin();
            exit(0);
        }
        else if(ch == K_ENTER) main_menu();
        else if(ch == K_TAB) return;
    }
    return;
}