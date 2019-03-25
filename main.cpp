#include<iostream>
#include<GL/glut.h>
#include<vector>
using namespace std;

vector<pair<int,int> > p;

int width=700,height=500,j;
int x[2],y[2],i;
const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000

int x_max,y_max,y_min,x_min;

int compute(int x,int y)
{   int code = INSIDE;

    if (x < x_min)
        code |= LEFT;
    else if (x > x_max)
        code |= RIGHT;
    if (y < y_min)
        code |= BOTTOM;
    else if (y > y_max)
        code |= TOP;

    return code;
}

void cs_clip(int i)
{
    int x,y,x1,y1,x2,y2,m;
    x1=p[i].first;
    y1=p[i].second;
    x2=p[i+1].first;
    y2=p[i+1].second;
    m=(y2-y1)/(x2-x1);

    int code1=compute(x1,y1);
    int code2=compute(x2,y2);

  while (true)
    {
        if ((code1 == 0) && (code2 == 0))
            break;
        else if (code1 & code2)
            break;

        else
        {
            int code_out;
            int x, y;

            if (code1 != 0)
                code_out = code1;
            else
                code_out = code2;

            if (code_out & TOP)
            {
                x = x1 + (x2 - x1) * (y_max - y1) / (y2 - y1);
                y = y_max;
            }
            else if (code_out & BOTTOM)
            {
                x = x1 + (x2 - x1) * (y_min - y1) / (y2 - y1);
                y = y_min;
            }
            else if (code_out & RIGHT)
            {
                y = y1 + (y2 - y1) * (x_max - x1) / (x2 - x1);
                x = x_max;
            }
            else if (code_out & LEFT)
            {

                y = y1 + (y2 - y1) * (x_min - x1) / (x2 - x1);
                x = x_min;
            }

            if (code_out == code1)
            {
                x1 = x;
                y1 = y;
                code1 = compute(x1, y1);
            }
            else
            {
                x2 = x;
                y2 = y;
                code2 = compute(x2, y2);
            }

        }
    }

    p[i].first=x1;
    p[i].second=y1;
    p[i+1].first=x2;
    p[i+1].second=y2;

}



void drawline(int j)
{
    glColor3f(1.0,1.0,1.0);
    glBegin(GL_LINES);
     glVertex2i(p[j-2].first,p[j-2].second);
     glVertex2i(p[j-1].first,p[j-1].second);
    glEnd();
    glFlush();
}

void drawClipingArea()
{
    glColor3f(1.0,0.0,1.0);
    glBegin(GL_LINE_LOOP);
    glVertex2i(x[0],y[0]);
    glVertex2i(x[1],y[0]);
    glVertex2i(x[1],y[1]);
    glVertex2i(x[0],y[1]);
    glEnd();
    glFlush();
}

void clip()
{
    x_max = x[1];
    y_min = y[1];
    x_min = x[0];
    y_max = y[0];

    for(unsigned int i=0;i<p.size();i=i+2)
        cs_clip(i);

    glClearColor(0.0,0.0,0.0,0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();

    drawClipingArea();

    for(int j=2;j<p.size()+2;j=j+2)
        drawline(j);

}

void display()
{


}
void menu(int c)
{
    if(c==2)
    {
         glClearColor(0.0,0.0,0.0,0.0);
         glClear(GL_COLOR_BUFFER_BIT);
         glFlush();
         i=0;j=0;
         p.clear();
    }
    else
        clip();


}

void mouse(int button,int state,int m,int n)
{
    if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN && i!=2)
    {
        x[i]=m;
        y[i]=height-n;
        i++;
        if(i==2)
            drawClipingArea();
    }
    else if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN )
    {
        j++;
        p.push_back({m,height-n});
        if(j%2==0)
            drawline(j);
    }

        glutCreateMenu(menu);
        glutAddMenuEntry("clip",1);
        glutAddMenuEntry("clear",2);
        glutAttachMenu(GLUT_RIGHT_BUTTON);

}



 void init()
{
   glClearColor(0.0,0.0,0.0,0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,width,0,height);
    glFlush();
    std::cout<<"Click diagonal points\n";
}

int main(int argc,char **argv)
{

    glutInit(&argc,argv);
    glutInitWindowSize(width,height);
    glutInitWindowPosition(10,10);
    glutCreateWindow("Line Clipping");

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    init();
    glutMainLoop();

    return 0;

}
