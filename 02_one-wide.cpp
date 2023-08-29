#include <bits/stdc++.h>
#include <windows.h>
#include <random>
#include <ctime>
using namespace std;

/* ================================================= user variable ================================================= */
const int sleeptime = 0; //between time that drop pieces
const int garbagepieces = 8; //every pieces to add garbage
const int garbagelines = 0; //garbage lines at every time
const int succusspieces = 1000; //condition of pieces for success
const int flat_height_diff = 1;
const int flat_blank = 10;
const int one_wide_height_diff = 1;
const int one_wide_blank = 100;
const int one_wide_right_blank = 10000;
const double one_wide_stdev = 0.1;
/* ================================================= user variable ================================================= */

// garbage 8, empty 15
// I, O, Z, S, J, L, T
// 9, 6, 4, 2, 1, 13, 5

mt19937 engine((unsigned int)time(NULL));
uniform_int_distribution<int> distribution(0, 5039);
uniform_int_distribution<int> distribution2(0, 9);
auto generator = bind(distribution, engine);
auto generator2 = bind(distribution2, engine);

int board[30][10], ans = 0, cnt = 0, att = 0;
queue<int> nextlist;

const int color[9] = { 15, 9, 6, 4, 2, 1, 6, 13, 8 };
const char print[9] = { ' ', 'I', 'O', 'Z', 'S', 'J', 'L', 'T', 'X' };
const int first[3] = { 21, 4, 0 };

const int block[8][4][4][2] = {
    {
        {
            {0, 0}, {0, 0}, {0, 0}, {0, 0}
        },
        {
            {0, 0}, {0, 0}, {0, 0}, {0, 0}
        },
        {
            {0, 0}, {0, 0}, {0, 0}, {0, 0}
        },
        {
            {0, 0}, {0, 0}, {0, 0}, {0, 0}
        }
    },

    // I
    {
        {
            {0, -1}, {0, 0}, {0, 1}, {0, 2}
        },
        {
            {1, 1}, {0, 1}, {-1, 1}, {-2, 1}
        },
        {
            {-1, -1}, {-1, 0}, {-1, 1}, {-1, 2}
        },
        {
            {1, 0}, {0, 0}, {-1, 0}, {-2, 0}
        }
    },

    // O
    {
        {
            {0, 0}, {0, 1}, {1, 0}, {1, 1}
        },
        {
            {0, 0}, {0, 1}, {1, 0}, {1, 1}
        },
        {
            {0, 0}, {0, 1}, {1, 0}, {1, 1}
        },
        {
            {0, 0}, {0, 1}, {1, 0}, {1, 1}
        }
    },

    // Z
    {
        {
            {1, -1}, {1, 0}, {0, 0}, {0, 1}
        },
        {
            {1, 1}, {0, 1}, {0, 0}, {-1, 0}
        },
        {
            {-1, 1}, {-1, 0}, {0, 0}, {0, -1}
        },
        {
            {-1, -1}, {0, -1}, {0, 0}, {1, 0}
        }
    },

    // S
    {
        {
            {0, -1}, {1, 0}, {0, 0}, {1, 1}
        },
        {
            {1, 1}, {0, 0}, {0, 0}, {-1, 1}
        },
        {
            {0, 1}, {-1, 0}, {0, 0}, {-1, -1}
        },
        {
            {-1, 0}, {0, -1}, {0, 0}, {1, -1}
        }
    },

    // J
    {
        {
            {1, -1}, {0, -1}, {0, 0}, {0, 1}
        },
        {
            {1, 1}, {1, 0}, {0, 0}, {-1, 0}
        },
        {
            {-1, 1}, {0, 1}, {0, 0}, {0, -1}
        },
        {
            {-1, -1}, {-1, 0}, {0, 0}, {1, 0}
        }
    },

    // L
    {
        {
            {1, 1}, {0, 1}, {0, 0}, {0, -1}
        },
        {
            {-1, 1}, {-1, 0}, {0, 0}, {1, 0}
        },
        {
            {-1, -1}, {0, -1}, {0, 0}, {0, 1}
        },
        {
            {1, -1}, {1, 0}, {0, 0}, {-1, 0}
        }
    },

    // T
    {
        {
            {0, 0}, {0, -1}, {1, 0}, {0, 1}
        },
        {
            {0, 0}, {1, 0}, {0, 1}, {-1, 0}
        },
        {
            {0, 0}, {0, 1}, {-1, 0}, {0, -1}
        },
        {
            {0, 0}, {-1, 0}, {0, -1}, {1, 0}
        }
    }
};

void boardclear() {
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 10; j++) board[i][j] = 0;
    }
}

void printboard() {
    for (int i = 29; i >= 0; i--) {
        for (int j = 0; j < 10; j++) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15 * 16 + color[board[i][j]]);
            cout << print[board[i][j]] << " ";
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
        cout << "| \n";
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15 * 16 + 0);
}

void printaction(int h, int w, int rotation) {
    for (int i = 0; i < w - 4; i++) cout << "L";
    for (int i = 0; i < 4 - w; i++) cout << "R";
    if (rotation == 1) cout << "+";
    if (rotation == 2) cout << "O";
    if (rotation == 3) cout << "-";
    cout << "\n";
}

void summon(int h, int w, int rotation, int type) {
    for (int i = 0; i < 4; i++) {
        board[h + block[type][rotation][i][0]][w + block[type][rotation][i][1]] = type;
    }
}

void dispawn(int h, int w, int rotation, int type) {
    for (int i = 0; i < 4; i++) {
        board[h + block[type][rotation][i][0]][w + block[type][rotation][i][1]] = 0;
    }
}

bool can(int w, int rotation, int type) {
    bool ret = false;
    for (int i = 0; i < 4; i++) {
        if (w + block[type][rotation][i][1] < 0 || w + block[type][rotation][i][1] > 9) ret = true;
    }
    return ret;
}

bool hit(int h, int w, int rotation, int type) {
    bool ret = false;
    for (int i = 0; i < 4; i++) {
        if (h + block[type][rotation][i][0] < 0) ret = true;
        if (board[h + block[type][rotation][i][0]][w + block[type][rotation][i][1]] != 0) ret = true;
    }
    return ret;
}

bool gameover() {
    for (int i = 20; i < 30; i++) {
        for (int j = 0; j < 10; j++) if (board[i][j] == 1) return true;
    }
    return false;
}

int remove_line() {
    int rem = 0;
    for (int i = 0; i < 30; i++) {
        int f = 1;
        for (int j = 0; j < 10; j++) if (board[i][j] == 0) f = 0;
        if (f == 1) rem++;
        else {
            for (int j = 0; j < 10; j++) board[i - rem][j] = board[i][j];
        }
    }
    if (rem == 2) att += 1;
    if (rem == 3) att += 2;
    if (rem == 4) att += 4;
    ans += rem;

    return rem;
}

void make_garbage(int num) {
    int pos = generator2();
    for (int i = 29 - num; i >= 0; i--) {
        for (int j = 0; j < 10; j++) board[i + num][j] = board[i][j];
    }
    for (int i = 0; i < num; i++) {
        for (int j = 0; j < 10; j++) {
            if (j != pos) board[i][j] = 8;
            else board[i][j] = 0;
        }
    }
}

void sevenbag() {
    int per = generator();
    int arr[7];
    int ch[7] = { 0, 0, 0, 0, 0, 0, 0 };
    for (int n = 7; n >= 1; n--) {
        int block = 0, idx = 0;
        while (1) {
            if (ch[block] == 0) {
                if (idx == per % n) break;
                idx++;
            }
            block++;
        }

        ch[block] = 1;
        block++;
        per = per / n;
        nextlist.push(block);
    }
}

int flat_score() {
    int scores = 0;
    // height differences between adjacent (minus score)
    int high[10];
    for (int j = 0; j < 10; j++) {
        int i;
        for (i = 29; i >= 0; i--) {
            if (board[i][j] > 0) break;
        }
        high[j] = i;
    }

    for (int j = 0; j < 9; j++) {
        scores -= (high[j] - high[j + 1]) * (high[j] - high[j + 1]) * flat_height_diff;
    }

    // empty space (minus score)
    int topblank[3];
    for (int j = 0; j < 10; j++) {
        for (int i = 0; i < high[j]; i++) {
            if (board[i][j] == 0) {
                scores -= flat_blank;
            }
        }
    }

    return scores;
}

int one_wide_score() {
    int scores = 0;

    // height differences between adjacent (minus score)
    int high[10];
    for (int j = 0; j < 10; j++) {
        int i;
        for (i = 29; i >= 0; i--) {
            if (board[i][j] > 0) break;
        }
        high[j] = i;
    }

    for (int j = 0; j < 8; j++) {
        scores -= (high[j] - high[j + 1]) * (high[j] - high[j + 1]) * one_wide_height_diff;
    }

    // empty space (minus score)
    int topblank[3];
    for (int j = 0; j < 10; j++) {
        for (int i = 0; i < high[j]; i++) {
            if (board[i][j] == 0) {
                scores -= one_wide_blank;
            }
        }
    }

    // in one-wide mode, blank of 9th is not allowed
    for (int i = 0; i <= high[9]; i++) {
        if (board[i][9] > 0) {
            scores -= one_wide_right_blank;
        }
    }

    // the better score, the smaller stdev of height
    int mean = 0, stdev = 0;
    for (int j = 0; j < 10; j++) mean += high[j];
    for (int j = 0; j < 10; j++) stdev += high[j] * high[j] * 10;
    stdev -= mean * mean;
    scores -= stdev * one_wide_stdev;

    return scores;
}

int countblank() {
    int high[10], cnt = 0;
    const int height_diff = 1;
    for (int j = 0; j < 10; j++) {
        int i;
        for (i = 29; i >= 0; i--) {
            if (board[i][j] > 0) break;
        }
        high[j] = i;
    }
    int topblank[3];
    for (int j = 0; j < 10; j++) {
        for (int i = 0; i < high[j]; i++) {
            if (board[i][j] == 0) cnt++;
        }
    }
    return cnt;
}

void brute_force(int block, int arr[], string type) {
    int h, w, r;
    int max_ = -1e9;
    for (int j = 0; j < 10; j++) {
        for (int k = 0; k < 4; k++) {
            if (can(j, k, block)) continue;
            int i = 21;
            while (1) {
                if (hit(i, j, k, block)) break;
                i--;
            }
            i++;
            summon(i, j, k, block);
            int s;
            if (type == "flat") s = flat_score();
            if (type == "one-wide") s = one_wide_score();
            if (s > max_) {
                h = i;
                w = j;
                r = k;
                max_ = s;
            }
            dispawn(i, j, k, block);
        }
    }
    arr[0] = max_; arr[1] = h; arr[2] = w; arr[3] = r; arr[4] = block;
    return;
}

void findfit(int block, int arr[]) {
    int high[10];
    const int height_diff = 1;
    for (int j = 0; j < 10; j++) {
        int i;
        for (i = 29; i >= 0; i--) {
            if (board[i][j] > 0) break;
        }
        high[j] = i;
    }

    // if there are more than three blanks, switch mode to flat mode
    if (countblank() >= 3) {
        brute_force(block, arr, "flat");
        return;
    }

    // tetris: if other row heights >= 9th row height + 4, Imino into 9th row
    int nineWideHeight = 30;
    for (int j = 0; j < 9; j++) nineWideHeight = min(nineWideHeight, high[j]);
    if (block == 1 && nineWideHeight >= high[9] + 4) {
        int i = 21;
        while (1) {
            if (hit(i, 9, 3, block)) break;
            i--;
        }
        i++;
        arr[0] = 10000; arr[1] = i; arr[2] = 9; arr[3] = 3; arr[4] = block;
        return;
    }

    brute_force(block, arr, "one-wide");
}

int main() {

    int holdblock = 0;
    while (1) {

        if (nextlist.size() <= 7) sevenbag();
        if (cnt % garbagepieces == 0) {
            make_garbage(garbagelines);
        }

        Sleep(sleeptime);
        system("cls");

        int block = nextlist.front();
        nextlist.pop();
        //cout << block << "\n";

        if (gameover()) {
            cout << "Game Over\n";
            cout << "pieces: " << cnt << "\n";
            cout << "break " << ans << " lines\n";
            cout << "attacks " << att << " lines\n";
            exit(0);
        }
        if (cnt >= succusspieces) {
            cout << "Game Clear\n";
            cout << "pieces: " << cnt << "\n";
            cout << "break " << ans << " lines\n";
            cout << "attacks " << att << " lines\n";
            exit(0);
        }

        int ret1[5], ret2[5];
        if (holdblock != 0) {
            findfit(holdblock, ret1);
        }
        else {
            findfit(nextlist.front(), ret1);
        }
        findfit(block, ret2);
        
        if (ret1[0] > ret2[0]) {
            summon(ret1[1], ret1[2], ret1[3], ret1[4]);
            printaction(ret1[1], ret1[2], ret1[3]);
            if (holdblock == 0) nextlist.pop();
            holdblock = block;
        }
        else {
            summon(ret2[1], ret2[2], ret2[3], ret2[4]);
            printaction(ret1[1], ret1[2], ret1[3]);
        }

        remove_line();
        cnt++;

        printboard();
    }

    return 0;
}
