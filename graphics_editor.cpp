#include <ncurses.h>
#include <iostream>
#include <cmath>

using namespace std;

const int ROWS = 25;
const int COLS = 50;

char canvas[ROWS][COLS];

void clearCanvas()
{
    for(int i=0;i<ROWS;i++)
        for(int j=0;j<COLS;j++)
            canvas[i][j]='_';
}

void setPixel(int x,int y,char ch)
{
    if(x>=0 && x<ROWS && y>=0 && y<COLS)
        canvas[x][y]=ch;
}

void displayCanvas()
{
    clear();

    mvprintw(0,0,"----- 2D Graphics Editor -----");

    for(int i=0;i<ROWS;i++)
    {
        for(int j=0;j<COLS;j++)
        {
            mvaddch(i+2,j,canvas[i][j]);
        }
    }

    refresh();
    getch();
}

void drawLine(int x1,int y1,int x2,int y2,char ch='*')
{
    int dx=abs(x2-x1);
    int dy=abs(y2-y1);

    int sx=(x1<x2)?1:-1;
    int sy=(y1<y2)?1:-1;

    int err=dx-dy;

    while(true)
    {
        setPixel(x1,y1,ch);

        if(x1==x2 && y1==y2)
            break;

        int e2=2*err;

        if(e2>-dy)
        {
            err-=dy;
            x1+=sx;
        }

        if(e2<dx)
        {
            err+=dx;
            y1+=sy;
        }
    }
}

void drawRectangle(int x,int y,int width,int height,char ch='*')
{
    for(int i=y;i<y+width;i++)
    {
        setPixel(x,i,ch);
        setPixel(x+height-1,i,ch);
    }

    for(int i=x;i<x+height;i++)
    {
        setPixel(i,y,ch);
        setPixel(i,y+width-1,ch);
    }
}

void drawTriangle(int x1,int y1,
                  int x2,int y2,
                  int x3,int y3,
                  char ch='*')
{
    drawLine(x1,y1,x2,y2,ch);
    drawLine(x2,y2,x3,y3,ch);
    drawLine(x3,y3,x1,y1,ch);
}

void drawCircle(int xc,int yc,int r,char ch='*')
{
    int x=0;
    int y=r;
    int d=3-2*r;

    while(y>=x)
    {
        setPixel(xc+x,yc+y,ch);
        setPixel(xc-x,yc+y,ch);
        setPixel(xc+x,yc-y,ch);
        setPixel(xc-x,yc-y,ch);

        setPixel(xc+y,yc+x,ch);
        setPixel(xc-y,yc+x,ch);
        setPixel(xc+y,yc-x,ch);
        setPixel(xc-y,yc-x,ch);

        x++;

        if(d>0)
        {
            y--;
            d=d+4*(x-y)+10;
        }
        else
        {
            d=d+4*x+6;
        }
    }
}

int main()
{
    initscr();
    cbreak();
    noecho();

    clearCanvas();

    int choice;

    do
    {
        clear();

        mvprintw(1,2,"===== 2D GRAPHICS EDITOR =====");
        mvprintw(3,2,"1. Draw Rectangle");
        mvprintw(4,2,"2. Draw Line");
        mvprintw(5,2,"3. Draw Triangle");
        mvprintw(6,2,"4. Draw Circle");
        mvprintw(7,2,"5. Delete Rectangle");
        mvprintw(8,2,"6. Display Picture");
        mvprintw(9,2,"7. Clear Canvas");
        mvprintw(10,2,"8. Exit");

        mvprintw(12,2,"Enter Choice: ");
        echo();
        scanw("%d",&choice);
        noecho();

        switch(choice)
        {
            case 1:
            {
                int x,y,w,h;

                mvprintw(14,2,"Top-left x y: ");
                echo();
                scanw("%d%d",&x,&y);

                mvprintw(15,2,"Width Height: ");
                scanw("%d%d",&w,&h);
                noecho();

                drawRectangle(x,y,w,h);
                break;
            }

            case 2:
            {
                int x1,y1,x2,y2;

                mvprintw(14,2,"x1 y1 x2 y2: ");
                echo();
                scanw("%d%d%d%d",&x1,&y1,&x2,&y2);
                noecho();

                drawLine(x1,y1,x2,y2);
                break;
            }

            case 3:
            {
                int x1,y1,x2,y2,x3,y3;

                mvprintw(14,2,"x1 y1: ");
                echo();
                scanw("%d%d",&x1,&y1);

                mvprintw(15,2,"x2 y2: ");
                scanw("%d%d",&x2,&y2);

                mvprintw(16,2,"x3 y3: ");
                scanw("%d%d",&x3,&y3);
                noecho();

                drawTriangle(x1,y1,x2,y2,x3,y3);
                break;
            }

            case 4:
            {
                int xc,yc,r;

                mvprintw(14,2,"Center x y: ");
                echo();
                scanw("%d%d",&xc,&yc);

                mvprintw(15,2,"Radius: ");
                scanw("%d",&r);
                noecho();

                drawCircle(xc,yc,r);
                break;
            }

            case 5:
            {
                int x,y,w,h;

                mvprintw(14,2,"Rectangle x y: ");
                echo();
                scanw("%d%d",&x,&y);

                mvprintw(15,2,"Width Height: ");
                scanw("%d%d",&w,&h);
                noecho();

                drawRectangle(x,y,w,h,'_');
                break;
            }

            case 6:
                displayCanvas();
                break;

            case 7:
                clearCanvas();
                break;

            case 8:
                break;

            default:
                mvprintw(14,2,"Invalid Choice!");
                getch();
        }

    } while(choice != 8);

    endwin();
    return 0;
}
