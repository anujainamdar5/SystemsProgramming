#include<iostream.h>
#include<stdio.h>
#include<conio.h>
#include<math.h>
#include<graphics.h>
class trans {
	int n;
	double mat[12][3];
	public:
	void quad();
	void getdata();
	void drawobj(char str[10]);
	void identity();
	void matmul(trans poly, trans mat);
	void translation(int x, int y);
	void rotation(double angle, int direction);
	void scaling(float sx,float sy);
	void comp_rot(trans org);
	void reflection(trans org);
	void shear();
	void comp_shear(trans org);
	trans() {
		for (int i=0;i<5;i++)
		for (int j=0;j<3;j++)
		mat[i][j]=0;
	}
}
;
void trans::quad() {
	setcolor(8);
	line(getmaxx()/2,0,getmaxx()/2,getmaxy());
	line(0,getmaxy()/2,getmaxx(),getmaxy()/2);
}
void trans::getdata() {
	cout<<"Enter number of vertices\t";
	cin>>n;
	cout<<"\nEnter vertices\n";
	for (int i=0;i<n;i++) {
		cout<<"x co-odrinate\t";
		cin>>mat[i][0];
		cout<<"y co-odrinate\t";
		cin>>mat[i][1];
		mat[i][2]=1;
	}
}
void trans::drawobj(char str[10]) {
	for (int i=0;i<n-1;i++)
	line(getmaxx()/2+mat[i][0],getmaxy()/2-mat[i][1],getmaxx()/2+mat[i+1][0],getmaxy()/2-mat[i+1][1]);
	line(getmaxx()/2+mat[n-1][0],getmaxy()/2-mat[n-1][1],getmaxx()/2+mat[0][0],getmaxy()/2-mat[0][1]);
	outtextxy(getmaxx()/2+mat[2][0],getmaxy()/2-mat[2][1],str);
}
void trans::identity() {
	for (int i=0;i<3;i++)
	for (int j=0;j<3;j++) {
		if(i==j)
		mat[i][j]=1; else
		mat[i][j]=0;
	}
}
void trans::translation(int x, int y) {
	mat[2][0]=x;
	mat[2][1]=y;
}
void trans::rotation(double angle, int direction) {
	angle=angle*(3.14/180);
	mat[0][0]=cos(angle);
	mat[1][1]=cos(angle);
	if(direction==1) {
		mat[0][1]=-sin(angle);
		mat[1][0]=sin(angle);
	} else {
		mat[0][1]=sin(angle);
		mat[1][0]=-sin(angle);
	}
}
void trans::scaling(float sx,float sy) {
	mat[0][0]=sx;
	mat[1][1]=sy;
}
void trans::matmul(trans poly, trans t1) {
	int i,j,k,sum;
	n=poly.n;
	for (i=0;i<n;i++) {
		for (j=0;j<3;j++) {
			sum=0;
			for (k=0;k<n;k++) {
				sum=sum+poly.mat[i][k]*t1.mat[k][j];
				mat[i][j]=sum;
			}
		}
	}
}
void trans::comp_rot(trans org) {
	trans ta,r,tb,ans1,ans2,ans;
	double angle;
	int xm,ym,dir;
	cout<<"Enter xm\t";
	cin>>xm;
	cout<<"Enter ym\t";
	cin>>ym;
	ta.identity();
	ta.translation(-xm,-ym);
	cout<<"\n";
	cout<<"Angle=\t";
	cin>>angle;
	cout<<"Press 1 for clockwise rotation or 0 for anti-clockwise rotation.\n";
	cin>>dir;
	r.identity();
	r.rotation(angle,dir);
	tb.identity();
	tb.translation(xm,ym);
	ans.matmul(org,ta);
	ans1.matmul(ans,r);
	ans2.matmul(ans1,tb);
	setcolor(4);
	ans.drawobj("Image 1");
	setcolor(1);
	ans1.drawobj("Image 2");
	setcolor(12);
	ans2.drawobj("Final");
}
void trans::reflection(trans org) {
	trans r,ans,t1,t2,a1,a2;
	int op,m,c;
	float angle;
	cout<<"Relection along:\n1. x axis\n2. y axis\n3.x=y\n4.y=-x\n5. origin\n6. y=mx+c\n";
	cin>>op;
	r.identity();
	switch(op) {
		case 1: r.mat[1][1]=-1;
		ans.matmul(org,r);
		break;
		case 2: r.mat[0][0]=-1;
		ans.matmul(org,r);
		break;
		case 3: r.mat[0][0]=0;
		r.mat[1][1]=0;
		r.mat[0][1]=1;
		r.mat[1][0]=1;
		ans.matmul(org,r);
		break;
		case 4: r.mat[0][0]=0;
		r.mat[1][1]=0;
		r.mat[0][1]=-1;
		r.mat[1][0]=-1;
		ans.matmul(org,r);
		break;
		case 5: r.mat[0][0]=-1;
		r.mat[1][1]=-1;
		ans.matmul(org,r);
		break;
		case 6: cout<<"Enter the coefficents of line y=mx+c\n";
		cout<<"m=\t";
		cin>>m;
		cout<<"c=\t";
		cin>>c;
		t1.identity();
		t1.mat[2][1]=-c;
		a1.matmul(org,t1);
		t1.identity();
		angle=acos(1/sqrt(1+m*m));
		angle=angle*(180/3.14);
		//cout<<angle;
		t1.rotation(angle,1);
		a2.matmul(a1,t1);
		t1.identity();
		t1.mat[1][1]=-1;
		a1.matmul(a2,t1);
		t1.identity();
		t1.rotation(angle,0);
		a2.matmul(a1,t1);
		t1.identity();
		t1.mat[2][1]=c;
		ans.matmul(a2,t1);
		break;
		default:cout<<"Please enter a valid option.";
	}
	setcolor(5);
	ans.drawobj("Reflected Image");
}
void trans::shear() {
	int op;
	cout<<"Menu:\n1. x shear\n2. y shear\n";
	cin>>op;
	if(op==1) {
		cout<<"\nEnter the factor\t";
		cin>>mat[1][0];
	} else if(op==2) {
		cout<<"\nEnter the factor\t";
		cin>>mat[0][1];
	} else
	cout<<"\nEnter correct option.\n";
}
void trans:: comp_shear(trans org) {
	trans t1,t2,ans,ans1,t3,ans2;
	int tx,ty;
	cout<<"Enter tx\t";
	cin>>tx;
	cout<<"Enter ty\t";
	cin>>ty;
	t1.identity();
	t1.translation(-tx,-ty);
	ans.matmul(org,t1);
	setcolor(12);
	ans.drawobj("Image 1");
	t2.shear();
	ans1.matmul(ans,t2);
	setcolor(7);
	ans1.drawobj("Image 2");
	t3.identity();
	t3.translation(tx,ty);
	ans2.matmul(org,t1);
	setcolor(9);
	ans2.drawobj("Transformed Image");
}
void main() {
	int tx,ty,angle,option,dir;
	float sx,sy;
	trans poly,t1,t_ans,r1,r_ans,s1,s_ans,cm,ref;
	int gd=DETECT,gm;
	detectgraph(&gd,&gm);
	initgraph(&gd,&gm,"C:\\TURBOC3\\BGI");
	poly.getdata();
	do {
		clrscr();
		cleardevice();
		poly.quad();
		setcolor(10);
		poly.drawobj("Original Image");
		cout<<"\nMenu:\n1. Translation\n2. Rotation\n3. Scaling";
		cout<<"\n4. Composite rotation\n5. Reflection\n6. Shearing\n7.Exit\n";
		cin>>option;
		switch(option) {
			case 1: //clrscr();
			t1.identity();
			cout<<"Enter tx\t";
			cin>>tx;
			cout<<"Enter ty\t";
			cin>>ty;
			t1.translation(tx,ty);
			t_ans.matmul(poly,t1);
			setcolor(4);
			t_ans.drawobj("Transformed Image");
			getch();
			break;
			case 2: r1.identity();
			cout<<"\nEnter angle\t";
			cin>>angle;
			cout<<"Press 1 for clockwise and\n 0 for anti-clockwise \nrotation.\n";
			cin>>dir;
			r1.rotation(angle,dir);
			r_ans.matmul(poly,r1);
			setcolor(1);
			r_ans.drawobj("Transformed Image");
			getch();
			break;
			case 3: s1.identity();
			cout<<"Enter Sx\t";
			cin>>sx;
			cout<<"Enter Sy\t";
			cin>>sy;
			s1.scaling(sx,sy);
			s_ans.matmul(poly,s1);
			setcolor(14);
			s_ans.drawobj("Transformed Image");
			getch();
			break;
			case 4:
			cm.comp_rot(poly);
			getch();
			break;
			case 5:
			ref.reflection(poly);
			getch();
			break;
			case 6: t1.identity();
			t1.shear();
			s_ans.matmul(poly,t1);
			setcolor(14);
			s_ans.drawobj("Transformed Image");
			getch();
			break;
			case 8:
			t1.comp_shear(poly);
			getch();
			break;
			default: "Please enter a valid option.";
		}
	}
	while(option!=7);
	getch();
	closegraph();
}
