#include <bits/stdc++.h>
#include <windows.h>
#include <random>
#include <ctime>
using namespace std;

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
        cout << "\n";
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

void remove_line() {
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

int score() {
    int scores = 0;
    // 인접한 두 지점의 높이차
    int high[10];
    const int height_diff = 1;
    for (int j = 0; j < 10; j++) {
        int i;
        for (i = 29; i >= 0; i--) {
            if (board[i][j] > 0) break;
        }
        high[j] = i;
    }

    for (int j = 0; j < 9; j++) {
        scores -= (high[j] - high[j + 1]) * (high[j] - high[j + 1]) * height_diff;
    }

    // 빈 공간
    const int blank = 10;
    int topblank[3];
    for (int j = 0; j < 10; j++) {
        for (int i = 0; i < high[j]; i++) {
            if (board[i][j] == 0) {
                scores -= blank;
            }
        }
    }

    return scores;
}

void findfit(int block, int arr[]) {
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
            int s = score();
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

int main() {

    int holdblock = 0;
    int sleeptime = 0;
    int garbagelines = 1;
    int garbagepieces = 16;
    int succusspieces = 1000;

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

        if (hit(first[0], first[1], first[2], block)) {
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