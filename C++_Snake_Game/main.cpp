#include <iostream>
#include <string>
#include <Windows.h> 
#include<conio.h>
#include<time.h>
#include <fstream> // 파일 입출력 스트림

#include <mmsystem.h> // wav 파일 사운드 재생
#pragma comment(lib,"winmm.lib")

// 수정 안한 파일

// 키보드 값 정의
#define UP 0 
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define SUBMIT 4
#define BACK 5

// 색상 값 정의
#define BLACK 0 
#define BLUE 1
#define RED 4 
#define GREEN 2
#define WHITE 15
#define YELLOW 14
#define CYAN 3

// 창 중심 축
#define C_MID_X 40 // 가로
#define C_MID_Y 14 //세로
using namespace std;

class Snake { // 뱀 클래스
public:
    int snake_len = 3; // 시작 길이
    int score = 0;     // 점수
    int speed = 170;   // 이동 스피드
    int item_cnt = 0;  // 먹은 아이템 개수
    int x = 0, y = 0;  // 이동 좌표 x,y를 가짐
    ~Snake() { }
};

class Item {
public:
    int x = 0, y = 0;
    Item() {}
};

// 시작 설정(초기화) 함수
void init() {

    // system함수 : 운영체제에 직접 명령어 전달
    system("mode con cols=90 lines=30 | title Snake Game"); // 콘솔 크기 | 제목 지정

    // 커서 숨기기
    CONSOLE_CURSOR_INFO ConsoleCursor = { 0, };
    ConsoleCursor.bVisible = false; // false or 0 : 숨기기
    ConsoleCursor.dwSize = 1;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ConsoleCursor);
}

// 커서 위치 이동 함수
void CursorXY(int x, int y) {

    COORD Pos = { x ,y }; // 위치 받아 이동하기
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos); // 콘솔 핸들 가져오기
}

void textcolor(int foreground, int background) { // 글씨, 배경 색상 바꾸기 

    int color = foreground + background * 16;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// 입력 키 값 읽기 함수
int keyControl() {

    int key = _getch(); // _getch() : 키보드의 키 1개를 사용자로부터 입력받는 함수
    if (key == 13) return SUBMIT; // Enter가 선택 키
    if (key == 98 || key == 66) return BACK; // B(b)가 선택 키

    if (_kbhit()) { // _kbhit() : 현재 키보드가 눌렸는지 여부 확인(bool형 반환)

        if (key == 224) { // 방향키는 2바이트짜리 확장키의 하나이다. _getch()는 한번에 1바이트만 읽음

            key = _getch(); // 방향키는 아스키 코드로 2개가 반환된다. (위 : 224,72 | 아래 : 224,80) 그래서 2번 읽어오는 것
            switch (key) {
            case 72:
                return UP;
            case 75:
                return LEFT;
            case 77:
                return RIGHT;
            case 80:
                return DOWN;
            }

        }
    }
}

// 메뉴 출력 함수
int MenuDraw() {
    int y = 17;

    textcolor(WHITE, BLACK);
    CursorXY(C_MID_X - 2, y);
    cout << "> 게임 시작" << endl;
    CursorXY(C_MID_X, y + 1);
    cout << "게임 방법" << endl;
    CursorXY(C_MID_X, y + 2);
    cout << "기     록" << endl;
    CursorXY(C_MID_X, y + 3);
    cout << "게임 종료" << endl;

    CursorXY(C_MID_X - 3, y + 5);
    cout << "Enter을 누르세요..." << endl;

    while (1) {

        int key = keyControl(); // 키보드 이벤트를 키 값으로 받아오기
        switch (key) {

        case UP: { // 입력한 키 값이 UP(위)인 경우
            if (y > 17) { // y는 17 ~ 20 까지만 이동
                CursorXY(C_MID_X - 2, y);
                cout << " " << endl; // x-2를 하는 이유는 ">"를 두 칸 이전에 출력하기 위해
                CursorXY(C_MID_X - 2, --y);
                cout << ">" << endl; // 새로 이동한 위치에 ">" 다시 그리기
            }
            break;
        }

        case DOWN: {
            if (y < 20) { // 최대 20
                CursorXY(C_MID_X - 2, y);
                cout << " " << endl;
                CursorXY(C_MID_X - 2, ++y);
                cout << ">" << endl;
            }
            break;
        }

        case SUBMIT: {
            return y - 17; // y 시작 위치가 17이므로 y-17을 하면 0(시작),1(방법),2(랭킹),3(종료) 세 숫자를 받을 수 있다.
        }
        } // end switch

    } // end while
}

// 맵 테두리 그리기 함수
void MapDraw() {

    system("cls");

    // 실습실 pc 에서 깨짐
    //for (int i = 0; i <= 27; i++) {
    //    // 위쪽 벽
    //    CursorXY(x + i, y);
    //    cout << "■" << endl;

    //    // 아래쪽 벽
    //    CursorXY(x + i, y + 25);
    //    cout << "■" << endl;
    //}

    CursorXY(4, 2);
    cout << "■■■■■■■■■■■■■■■■■■■■■■■■■■■■" << endl;

    CursorXY(4, 27);
    cout << "■■■■■■■■■■■■■■■■■■■■■■■■■■■■" << endl;


    for (int i = 1; i <= 24; i++) {
        //왼쪽 벽
        CursorXY(4, 2 + i);
        cout << "■" << endl;

        // 오른쪽 벽
        CursorXY(58, 2 + i);
        cout << "■" << endl;
    }


    CursorXY(66, 16);
    cout << "―――――――――";

    CursorXY(68, 17);
    cout << "Score │  Speed";

    CursorXY(66, 18);
    cout << "―――――――――";

    CursorXY(65, 19);
    textcolor(RED, BLACK);
    cout << "♥  ";
    textcolor(WHITE, BLACK);
    cout << "+10  │   3↑";

    CursorXY(65, 20);
    textcolor(YELLOW, BLACK);
    cout << "★  ";
    textcolor(WHITE, BLACK);
    cout << "+20  │   5↑";

}

// 뱀 움직이기
void move_snake(Snake* snake) {

    static int snake_head = -1;

    if (_kbhit()) {

        int key = keyControl(); // 머리 방향 바꾸기
        switch (key) {
            case UP: snake_head = 0; break;
            case DOWN: snake_head = 1; break;
            case LEFT: snake_head = 2; break;
            case RIGHT: snake_head = 3; break;
        }
    }

    if (snake_head != -1) {
        int i;

        CursorXY(snake[snake->snake_len - 1].x, snake[snake->snake_len - 1].y);
        cout << "  " << endl;

        for (i = snake->snake_len - 1; i > 0; i--)
            snake[i] = snake[i - 1];

        CursorXY(snake[1].x, snake[1].y);
        textcolor(GREEN, BLACK);
        cout << "◇" << endl;
        textcolor(WHITE, BLACK);
    }

    switch (snake_head) {
        case 0: snake[0].y--; break;//위
        case 1: snake[0].y++; break;//아래
        // ◆가 출력될 때 2칸씩 먹으니 x좌표 이동 시 2칸씩 이동
        case 2: snake[0].x -= 2; break;//왼쪽 
        case 3: snake[0].x += 2; break;//오른쪽
    }

    CursorXY(snake[0].x, snake[0].y);
    textcolor(GREEN, BLACK);
    cout << "◆" << endl;
    textcolor(WHITE, BLACK);

}

// 벽 or 몸에 부딫힘 체크
int check_bump(Snake* snake) {

    // 맵 테두리 벽
    if (snake[0].x == 4 || snake[0].y == 2 || snake[0].x == 58 || snake[0].y == 27) return 0;

    // 1단계 벽
    if (snake->item_cnt >= 5) {
        for (int i = 6; i <= 26; i++) {
            if (snake[0].x == i && snake[0].y == 7) return 0;
        }
        for (int i = 36; i <= 57; i++) {
            if (snake[0].x == i && snake[0].y == 22) return 0;
        }
    }

    // 2단계 벽
    if (snake->item_cnt >= 10) {
        for (int i = 3; i <= 11; i++) {
            if (snake[0].x == 44 && snake[0].y == i) return 0;
        }
        for (int i = 18; i <= 26; i++) {
            if (snake[0].x == 20 && snake[0].y == i) return 0;
        }
    }

    for (int i = 1; i < snake->snake_len; i++) {
        // 몸통
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) return 0;
    }

    return 1;
}

// 게임 오버 창 그리기
void Game_Over(int score) {

    PlaySound(L"dead_bgm.wav", 0, SND_FILENAME | SND_ASYNC);

    system("cls");

    CursorXY(C_MID_X - 22, C_MID_Y - 6);
    cout << " .--.                          .--.   \n";
    CursorXY(C_MID_X - 22, C_MID_Y - 5);
    cout << ": .--'                        : ,. :             \n";
    CursorXY(C_MID_X - 22, C_MID_Y - 4);
    cout << ": : _  .--.  ,-.,-.,-. .--.   : :: :.-..-. .--. .--. \n";
    CursorXY(C_MID_X - 22, C_MID_Y - 3);
    cout << ": :; :' .; ; : ,. ,. :' '_.'  : :; :: `; :' '_.': ..'\n";
    CursorXY(C_MID_X - 22, C_MID_Y - 2);
    cout << "`.__.'`.__,_;:_;:_;:_;`.__.'  `.__.'`.__.'`.__.':_;  \n";

    CursorXY(C_MID_X - 1, C_MID_Y + 1);
    cout << "Score : " << score << endl;

    CursorXY(C_MID_X - 35, C_MID_Y + 13);
    cout << "<Enter> GO TO MAIN" << endl;

    ofstream file_name, file_score;
    string name;

    CursorXY(C_MID_X - 1, C_MID_Y + 3);
    cout << "Name : ";
    cin >> name;

    file_name.open("record_name.txt", ios::app);
    file_name << name << endl;
    file_name.close();

    file_score.open("record_score.txt", ios::app);
    file_score << score << endl;
    file_score.close();

}

// 게임 방법 출력 함수
int infoDraw() {
    system("cls");

    cout << "\n\n\n\n\n";
    cout << "                                      [  게임 방법  ]\n\n";
    cout << "                              이동 : 방향키 (↑, ↓, ←, →)\n\n\n";

    cout << "                        뱀을 움직여 아이템을 먹으면 점수가 올라갑니다.\n\n";
    cout << "                      (단, 아이템을 먹을 때마다 뱀의 길이가 길어집니다)\n\n";
    cout << "                   뱀이 벽이나 자신의 몸 일부에 머리를 부딪히면 GAME OVER!\n\n\n\n";

    cout << "                                    [  아이템 소개  ]\n\n";
    textcolor(RED, BLACK);
    cout << "                               ♥";
    textcolor(WHITE, BLACK);
    cout << " : 점수 +10 , 스피드 +5\n\n";

    textcolor(YELLOW, BLACK);
    cout << "                               ★";
    textcolor(WHITE, BLACK);
    cout << " : 점수 +20 , 스피드 +7\n\n";

    cout << "                            ";
    textcolor(WHITE, CYAN);
    cout << "?";
    textcolor(WHITE, BLACK);
    cout << ": 찍찍이와 UP&DOWN 미니게임 한 판!\n\n";


    CursorXY(C_MID_X + 30, C_MID_Y + 13);
    cout << "만든이 : 김민정";

    CursorXY(C_MID_X - 35, C_MID_Y + 13);
    cout << "<B> GO TO MAIN" << endl;

    while (1) {
        int key = keyControl();
        if (key == BACK) return 0;
    }
}

// 타이틀 그리기 함수
void TitleDraw() {

    system("cls");

    textcolor(WHITE, BLACK);
    CursorXY(C_MID_X - 25, C_MID_Y - 7);
    cout << " .--.              .-.            .--.                       \n";
    CursorXY(C_MID_X - 25, C_MID_Y - 6);
    cout << ": .--'             : :.-.        : .--'                      \n";
    CursorXY(C_MID_X - 25, C_MID_Y - 5);
    cout << "`. `. ,-.,-. .--.  : `'.' .--.   : : _  .--.  ,-.,-.,-. .--. \n";
    CursorXY(C_MID_X - 25, C_MID_Y - 4);
    cout << " _`, :: ,. :' .; ; : . `.' '_.'  : :; :' .; ; : ,. ,. :' '_.'\n";
    CursorXY(C_MID_X - 25, C_MID_Y - 3);
    cout << "`.__.':_;:_;`.__,_;:_;:_;`.__.'  `.__.'`.__,_;:_;:_;:_;`.__.'\n";

}

int Random(Snake* snake, Item& item) {

    bool chk_Loop = true;

    while (chk_Loop) {

        item.x = rand() % 51 + 6;
        item.y = rand() % 24 + 3;
        chk_Loop = false;

        for (int i = 0; i < snake->snake_len; i++) { // 뱀의 몸통이 있는 곳인지
            if (item.x == snake[i].x && item.y == snake[i].y) {
                chk_Loop = true;
            }
        }
        if (snake->item_cnt >= 5) {
            for (int i = 6; i <= 26; i++) { // 1단계 왼쪽 위 벽이 있는 자리
                if (item.x == i && item.y == 7) {
                    chk_Loop = true;
                }
            }

            for (int i = 36; i <= 57; i++) { // 1단계 오른쪽 아래 벽이 있는 자리
                if (item.x == i && item.y == 22) {
                    chk_Loop = true;
                }
            }
        }
        if (snake->item_cnt >= 10) {
            for (int i = 3; i <= 11; i++) { // 2단계 오른쪽 위 벽이 있는 자리
                if (item.x == 44 && item.y == i) {
                    chk_Loop = true;
                }
            }

            for (int i = 18; i <= 26; i++) { // 2단계 왼쪽 아래 벽이 있는 자리
                if (item.x == 20 && item.y == i) {
                    chk_Loop = true;
                }
            }
        }
    }
    // snake의 x 좌표가 짝수(x+2, x-2 하여)밖에 안나오니 홀수가 나올 시 +1
    if (item.x % 2 == 1) item.x += 1;
    return 0;
}

int ShowRecord() {

    system("cls");

    CursorXY(C_MID_X, C_MID_Y - 6);
    cout << "[ 게임 기록 ]";
    CursorXY(C_MID_X - 11, C_MID_Y - 4);
    cout << "―――――――――――――――――";
    CursorXY(C_MID_X - 5, C_MID_Y - 3);
    cout << "Name               Score";
    CursorXY(C_MID_X - 11, C_MID_Y - 2);
    cout << "―――――――――――――――――";

    fstream file1("record_name.txt", ios::in);
    fstream file2("record_score.txt", ios::in);

    string str_name;
    string str_score;

    int cnt = 0;

    while (getline(file1, str_name) && getline(file2, str_score)) {
        CursorXY(C_MID_X - 6, C_MID_Y + cnt);
        cout << str_name;
        CursorXY(C_MID_X + 15, C_MID_Y + cnt);
        cout << str_score << endl;
        cnt++;
    }
    file1.close();
    file2.close();

    CursorXY(C_MID_X - 35, C_MID_Y + 13);
    cout << "<B> GO TO MAIN" << endl;

    while (1) { // B(b) 키를 입력하면 빠져나오기
        int key = keyControl();
        if (key == BACK) return 0;
    }
}

// 미니게임 진행 함수
int Mini_Game(Snake* snake) {

    srand((unsigned)time(0));

    system("cls");

    int mouse_num, User = 0;
    int cnt = 0;
    mouse_num = rand() % 10 + 1;

    CursorXY(C_MID_X - 22, C_MID_Y - 6);
    cout << "=====================================================";
    CursorXY(C_MID_X - 22, C_MID_Y - 5);
    cout << "  [찍찍] 기회는 총 3번, 저의 숫자를 맞춰보세요!";
    CursorXY(C_MID_X - 22, C_MID_Y - 4);
    cout << "=====================================================";
    CursorXY(C_MID_X - 22, C_MID_Y - 3);
    cout << " (수의 범위는 1 ~ 10)";

    for (int i = 1; i <= 3; i++) {
        CursorXY(C_MID_X - 20, C_MID_Y);
        cout << "\t[" << i << "번째 기회] " << "숫자를 입력해주세요 : ";
        cin >> User;
        CursorXY(C_MID_X + 19, C_MID_Y);
        cout << "  ";

        if (mouse_num > User) {
            CursorXY(C_MID_X - 15, C_MID_Y + 2);
            cout << "- " << User << " 보다 UP   ";

        }
        else if (mouse_num < User) {
            CursorXY(C_MID_X - 15, C_MID_Y + 2);
            cout << "- " << User << " 보다 Down ";

        }
        else {
            CursorXY(C_MID_X - 22, C_MID_Y + 5);
            cout << "=====================================================";
            CursorXY(C_MID_X - 22, C_MID_Y + 6);
            cout << "  [찍찍] 정답입니다! 점수를 40점 획득합니다.";
            CursorXY(C_MID_X - 22, C_MID_Y + 7);
            cout << "  [찍찍] 3초 뒤 돌아갑니다.";
            CursorXY(C_MID_X - 22, C_MID_Y + 8);
            cout << "=====================================================";
            snake->score += 40;
            break;
        }
        if (i == 3) {
            CursorXY(C_MID_X - 22, C_MID_Y + 5);
            cout << "=====================================================";
            CursorXY(C_MID_X - 22, C_MID_Y + 6);
            cout << "  [찍찍] 제가 생각한 숫자는 [" << mouse_num << "]이었습니다..";
            CursorXY(C_MID_X - 22, C_MID_Y + 7);
            cout << "  [찍찍] 점수가 40점 차감됩니다. 3초 뒤 돌아갑니다.";
            CursorXY(C_MID_X - 22, C_MID_Y + 8);
            cout << "=====================================================";
            snake->score -= 40;
        }
    }
    Sleep(3000);
    return 0;
}

// 아이템1을 먹었을 시
void Item1_Eat(Snake* snake) {

    PlaySound(L"item_bgm.wav", 0, SND_FILENAME | SND_ASYNC);
    snake->score += 10; // 10점 획득
    snake->item_cnt++; // 먹은 갯수 증가
    if (snake->speed > 60) snake->speed -= 5; // 속도 올리기
    if (snake->snake_len < 200)  snake->snake_len++; // 길이 늘리기 
}

// 아이템2을 먹었을 시
void Item2_Eat(Snake* snake) {

    PlaySound(L"item_bgm.wav", 0, SND_FILENAME | SND_ASYNC);
    snake->score += 20; // 20점 획득
    snake->item_cnt++; // 먹은 갯수 증가
    if (snake->speed > 60) snake->speed -= 7; // 속도 올리기
    if (snake->snake_len < 200)  snake->snake_len++; // 길이 늘리기 
}

// 아이템3을 먹었을 시
int Item3_Eat(Snake* snake) {

    system("cls");

    while (1) {

        // 가로 벽 for문으로 생성 시 pc에서 출력 깨짐..
        CursorXY(4, 2);
        cout << "■■■■■■■■■■■■■■■■■■■■■■■■■■■■" << endl;

        CursorXY(4, 27);
        cout << "■■■■■■■■■■■■■■■■■■■■■■■■■■■■" << endl;

        for (int i = 1; i <= 24; i++) {
            //왼쪽 벽
            CursorXY(4, 2 + i);
            cout << "■" << endl;

            // 오른쪽 벽
            CursorXY(58, 2 + i);
            cout << "■" << endl;
        }
        CursorXY(17, 13);
        cout << "성공 : +40점, 실패 : -40점";
        CursorXY(14, 12);
        cout << "미니 게임을 진행하시겠습니까?(y/n) : ";

        string answer;
        cin >> answer;

        if (answer == "y") {
            Mini_Game(snake);
            return 0;
        }
        else if (answer == "n") {
            CursorXY(22, 15);
            cout << "3초 뒤 돌아갑니다..";

            Sleep(3000);
            break;
        }
        else {
            CursorXY(22, 15);
            cout << "(y/n)을 입력해주세요.";
        }
    }
}

int main() {

    srand((unsigned)time(0));
    init();
    PlaySound(L"Game_bgm.wav", 0, SND_FILENAME | SND_ASYNC);

    while (1) {

        TitleDraw();
        int choice_menu = MenuDraw(); // 메뉴 선택

        if (choice_menu == 0) {  // 게임 시작

            PlaySound(0, 0, 0);
            MapDraw();

            Snake snake[200] = {};
            Item item1, item2, item3;

            // 뱀의 처음 시작 위치
            snake[0].x = 30;
            snake[0].y = 15;

            // 아이템의 초기 위치 지정 (snake가 움직일 수 잇는 x좌표:6~56, y좌표:3~26)
            Random(snake, item1);
            Random(snake, item2);
            Random(snake, item3);

            while (check_bump(snake) != 0) { // 뱀의 벽이나 몸에 부딪히지 않았다면

                move_snake(snake);
                Sleep(snake->speed);

                CursorXY(item1.x, item1.y); // 아이템1 출력
                textcolor(RED, BLACK);
                cout << "♥";
                textcolor(WHITE, BLACK);


                if (snake[0].x == item1.x && snake[0].y == item1.y) { // 아이템1을 먹었다면                  
                    Item1_Eat(snake);
                    Random(snake, item1); // 아이템 위치 재선정
                }

                if (snake->speed < 150) { // 속도가 일정 이상 높아졌다면 아이템2 출현

                    CursorXY(item2.x, item2.y); // 아이템2 출력
                    textcolor(YELLOW, BLACK);
                    cout << "★";
                    textcolor(WHITE, BLACK);

                    if (snake[0].x == item2.x && snake[0].y == item2.y) { // 아이템2를 먹었다면
                        Item2_Eat(snake);
                        Random(snake, item2); // 아이템 위치 재선정
                    }
                }
                if (snake->score >= 100) { // 점수가 100 이상일 때 아이템3 출현

                    CursorXY(item3.x, item3.y); // 아이템3 출력
                    textcolor(WHITE, CYAN);
                    cout << "?";
                    textcolor(WHITE, BLACK);

                    if (snake[0].x == item3.x && snake[0].y == item3.y) { // 아이템3를 먹었다면
                        PlaySound(L"Mini_Game.wav", 0, SND_FILENAME | SND_ASYNC);
                        Item3_Eat(snake);
                        Random(snake, item3);

                        Sleep(2000);
                        MapDraw();
                    }
                }
                // 현재 점수, 먹은 아이템 개수 출력
                CursorXY(69, 11);
                cout << "Score : " << snake->score << endl;
                CursorXY(69, 13);
                cout << "Item  : " << snake->item_cnt << endl;

                if (snake->item_cnt >= 5) { // 아이템을 5개 이상 먹었다면 1단계 벽 생성

                   // 출력 밀림
                   /* for (int i = 1; i < 13; i++) {
                        CursorXY(4 + i, 7);
                        cout << "■" << endl;
                        CursorXY(36 + i, 22);
                        cout << "■" << endl;
                    } */

                    CursorXY(4, 7);
                    cout << "■■■■■■■■■■■■" << endl;
                    CursorXY(36, 22);
                    cout << "■■■■■■■■■■■■" << endl;
                }
                if (snake->item_cnt >= 10) { // 아이템을 10개 이상 먹었다면 2단계 벽 생성

                    for (int i = 1; i <= 9; i++) {
                        CursorXY(44, 2 + i);
                        cout << "■" << endl;

                        CursorXY(20, 17 + i);
                        cout << "■" << endl;
                    }
                }
            }
            Game_Over(snake->score); // 게임 오버            

        }
        else if (choice_menu == 1) {  // 게임 방법
            infoDraw();
        }
        else if (choice_menu == 2) {  // 기록 보기
            ShowRecord();
        }
        else if (choice_menu == 3) {  // 게임 종료
            return 0;
        }
    }
}
