#include<iostream.h>
#include<stdio.h>
#include<conio.h>
#include<graphics.h>
#include<math.h>
typedef struct pix {
  float x,y;
  int code[4];
}
pixel;
class line_clip {
  pixel pwl,pwh,p1,p2;
  public:
  int and();
  //void divide();
  void edge();
  void show_edge();
  void clip();
  void window();
  void show_window();
  int nonzero();
  void intersection();
  void assigncode();
}
;
int line_clip::and() {
  for (int i=0;i<4;i++) {
    if((p1.code[i]&p2.code[i])==1)
    break; else continue;
  }
  if(i==4)
  return 0; else
  return 1;
}
void line_clip::window() {
  cout<<"Enter lower vertex of window\t";
  cin>>pwl.x>>pwl.y;
  cout<<"Enter upper vertex of window\t";
  cin>>pwh.x>>pwh.y;
}
void line_clip::show_window() {
  setcolor(12);
  rectangle(pwl.x+getmaxx()/2,getmaxy()/2-pwl.y,pwh.x+getmaxx()/2,getmaxy()/2-pwh.y);
}
void line_clip::edge() {
  cout<<"Enter co-ordinates ofstarting point of line";
  cin>>p1.x>>p1.y;
  cout<<"Enter co-ordinates of ending point of line";
  cin>>p2.x>>p2.y;
}
void line_clip::show_edge() {
  line(getmaxx()/2+p1.x,getmaxy()/2-p1.y,getmaxx()/2+p2.x,getmaxy()/2-p2.y);
}
void line_clip::assigncode() {
  for (int i=0;i<4;i++) {
    p1.code[i]=0;
    p2.code[i]=0;
  }
  if(p1.x<pwl.x)
  p1.code[3]=1;
  if(p1.x>pwh.x)
  p1.code[2]=1;
  if(p1.y<pwl.y)
  p1.code[1]=1;
  if(p1.y>pwh.y)
  p1.code[0]=1;
  if(p2.x<pwl.x)
  p1.code[3]=1;
  if(p2.x>pwh.x)
  p1.code[2]=1;
  if(p2.y<pwl.y)
  p1.code[1]=1;
  if(p2.y>pwh.y)
  p1.code[0]=1;
}
void line_clip::intersection() {
  float m;
  m=((p2.y-p1.y)/(p2.x-p1.x));
  if(p1.x<pwl.x&&p2.x>pwl.x) //intersection with left {
  {
    p1.x=pwl.x;
    p1.y=p2.y+(p1.x-p2.x)*m;
  }
  if(p1.x<pwh.x&&p2.x>pwh.x) // with right {
  { 
    p2.x=pwh.x;
    p2.y=p1.y+(p2.x-p1.x)*m;
  }
  if(p1.y<pwh.x&&p2.y>pwh.x) // with upper {
  {
    p2.y=pwh.y;
    p2.x=p1.x+(p2.y-p1.y)/m;
  }
  if(p1.y<pwl.y&&p2.y>pwl.x)   //with lower {
  {
    p1.y=pwl.y;
    p1.x=p2.x+(p1.y-p2.y)/m;
  }
}

void line_clip::clip() {
  assigncode();
  getch();
  if(and()==1) {
    clrscr();
    cleardevice();
    show_window();
    cout<<"Line is completely invisible\n";
  } else {
    intersection();
    clrscr();
    cleardevice();
    show_window();
    setcolor(10);
    show_edge();
  }
}
void main() {
  int gd=DETECT,gm;
  initgraph(&gd,&gm,"c://TURBOC3//BGI");
  line_clip l1;
  l1.window();
  l1.show_window();
  l1.edge();
  l1.show_edge();
  l1.clip();
  getch() ;
}
