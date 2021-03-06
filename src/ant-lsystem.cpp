#include <bits/stdc++.h>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>

using ll = long long;
using ld = long double;
using ull = unsigned long long;

using namespace std;

HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
DWORD dwWritten;
HWND myconsole = GetConsoleWindow();
HDC dc = GetDC(myconsole);

int XSIZE = 100;
int YSIZE = 100;

float A = -M_PI;
float B = M_PI;

int MAX_LENGTH = 30;
int AXIOM_ITERATIONS = 5;
int MAX_CHAMBER_SIZE = 5;

float PROB = 100.0f;
float PROB_DECAY = 0.999;

struct Nest
{
    float heading;
    int x,y;

    Nest()
    {
        heading = 0;
        x = XSIZE/2;
        y = 0;
    }
};

void DDA(int X0, int X1, int Y0, int Y1)
{
    /*
        Implementation of DDA c/o GeeksForGeeks
        https://www.geeksforgeeks.org/dda-line-generation-algorithm-computer-graphics/
    */

    int dx = X1 - X0;
    int dy = Y1 - Y0;
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    float Xinc = dx/(float) steps;
    float Yinc = dy/(float) steps;
    float X = X0;
    float Y = Y0;
    for (int i = 0; i <= steps; i++)
    {
        FillConsoleOutputCharacter(hOutput, '#', 1, {X,Y}, &dwWritten);

        X += Xinc;
        Y += Yinc;
    }
}

float change_heading(Nest nest, char direction)
{
    float new_heading;

    float helper = 100.0f - PROB;

    if (direction == 'L')
    {
        new_heading = nest.heading + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(A/2-nest.heading)));
    } else
    {
        new_heading = nest.heading + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(B/2-nest.heading)));
    }
    return new_heading;
}
void build_chamber(Nest &nest)
{
    int chamber_length = 5 + rand()%MAX_CHAMBER_SIZE;
    int t = 2 + rand()%3;

    while (chamber_length >= 3 && t>0)
    {
        DDA(nest.x, nest.x+chamber_length*sin(-M_PI/2), nest.y, nest.y+chamber_length*cos(-M_PI/2));
        DDA(nest.x, nest.x+chamber_length*sin(M_PI/2), nest.y, nest.y+chamber_length*cos(M_PI/2));
        nest.y+=1;
        chamber_length-=2;
        t--;
    }


}

void read_system(Nest nest, string lsystem)
{
    stack<Nest> s;

    for (int i = 0; i < lsystem.length(); i++)
    {
        int length = rand()%MAX_LENGTH;
        int prob_r = rand()%100;
        switch(lsystem[i])
        {
            case 'L':
                nest.heading = change_heading(nest, 'L');
                break;
            case 'R':
                nest.heading = change_heading(nest, 'R');
                break;
            case 'F':

                DDA(nest.x, nest.x+length*sin(nest.heading), nest.y, nest.y+length*cos(nest.heading));
                nest.x+=length*sin(nest.heading);
                nest.y+=length*cos(nest.heading);
                break;
            case '[':
                s.push(nest);
                break;
            case ']':
                nest.heading = s.top().heading;
                nest.x = s.top().x;
                nest.y = s.top().y;
                s.pop();
                break;
            case 'B':
                if (prob_r <= PROB)
                build_chamber(nest);
                break;
            default:
                break;
        }

        PROB *= PROB_DECAY;
        A+=0.001;
        B-=0.001;

        if (PROB < 10.00f)
        {
            break;
        }
    }
}

int main()
{
    srand(time(NULL));

    /*
    Some other examples of l-system strings that look deccent:
    [LFRFB]RRFBLFB
    [LF[RF]BLF]RRFBLFB
    [[RRFB]LFBRFB]FBRFBLFB
    [RFB]LFBLFBRFB
    [RFB][LFBLFBRFB]LFBLFB[LLFB]RFB
    */

    string lsystem = "[LF[RF]BLF]RRFBLFB";

    string tmp = "";

    for (int i = 0; i < AXIOM_ITERATIONS; i++)
    {
        for (int j = 0; j < lsystem.length(); j++)
        {
            if (lsystem[j]!='F')
            { tmp+=lsystem[j]; }
            else { tmp+=lsystem; }
        }
    }

    Nest nest;
    read_system(nest, tmp);
}


