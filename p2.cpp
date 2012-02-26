//#include<QtGui>
#include<QApplication>
#include<QTextEdit>
#include<QPushButton>
#include<QFont>
#include<QWidget>
#include<QPainter>
#include<QMouseEvent>
#include<QPixmap>
#include<iostream>
#include<QToolButton>
#include<QImage>
using namespace std;
inline int abs(int x){
	return (x>0)?x:(0-x);
}
class Paint;
class Pixmap: public QImage{
	public:
		
		void paintEvent(QPaintEvent* event){

		}
		
		Pixmap(int x,int y):
			QImage(x,y,QImage::Format_RGB32),painter(this),width(x),heigh(y){
//			painter.drawRect(10,20,100,50);
//			painter.drawPoint(15,25);
			fill( (((1<<31)-1)<<1)+1 );
		}

		void drawp(int x,int y,int option=0){
		
//			painter.drawRect(x,y,100,70);
//			cerr<<x<<" "<<y;	
//			painter.drawPoint(x,y);
			if(option==1){
				int st;
				int en;
				int absy=(oy-y)>0?(oy-y):(y-oy);
				int absx=(ox-x)>0?(ox-x):(x-ox);
				
				if( oy!=y ){
					//if(absy==0)
					//	return;
					if(oy>y){
						st=y;
						en=oy;
					}
					else{
						st=oy;
						en=y;
					}
					for(int i=st;i<=en;++i){
						painter.drawPoint((x*(i-oy)+ox*(y-i))/(y-oy),i);
					}
				}
				if (ox!=x){
					if(ox>x){
						st=x;
						en=ox;
					}
					else{
						st=ox;
						en=x;
					}
					for(int i=st;i<=en;++i){
						painter.drawPoint(i,(y*(i-ox)+oy*(x-i))/(x-ox));
					}
				}
			}
			else{
				//painter.fillRect(((x>>3)<<3),((y>>3)<<3),8,8,Qt::SolidPattern);
				painter.drawPoint(x,y);
			}
			ox=x;
			oy=y;
		}
		void drawrec(int x,int y,int xx,int yy,int option){
			
			if(option==1)
				painter.drawRect(x,y,xx-x,yy-y);
			if(option==2)
				painter.drawEllipse(x,y,xx-x,yy-y);
		}
		void paintr(int x,int y,QColor color,int pwidth){
			if(x<0||x>=width||y<0||y>=heigh)
				return ;
			unsigned pcol=pixel(x,y);
			if(color.rgb()==pcol)
				return;
			vector<int> xs;
			vector<int> ys;
	//		xs.resize(heigh*width);
	//		ys.resize(heigh*width);
			int i=0;
			int s=1;
			xs.push_back(x);
			ys.push_back(y);
			width=1000;
			heigh=600;
			QPen pen(color);
			pen.setWidth(1);
			painter.setPen(pen);
			painter.drawPoint(x,y);
			while(i<xs.size()){
				if(xs[i]>0 && pixel(xs[i]-1,ys[i])==pcol ){
					xs.push_back(xs[i]-1);
					ys.push_back(ys[i]);
					painter.drawPoint(xs.back(),ys.back());
				}
				if(xs[i]<width-1 && pixel(xs[i]+1,ys[i])==pcol ){
					xs.push_back(xs[i]+1);
					ys.push_back(ys[i]);
					painter.drawPoint(xs.back(),ys.back());
				}
				if(ys[i]>0 && pixel(xs[i],ys[i]-1)==pcol ){
					xs.push_back(xs[i]);
					ys.push_back(ys[i]-1);
					painter.drawPoint(xs.back(),ys.back());
				}
				if(ys[i]<heigh-1 && pixel(xs[i],ys[i]+1)==pcol ){
					xs.push_back(xs[i]);
					ys.push_back(ys[i]+1);
					painter.drawPoint(xs.back(),ys.back());
				}
				++i;

			}
			pen.setWidth(pwidth);
			painter.setPen(pen);
			cout<<xs.size()<<" "<<ys.size()<<endl;

		}
		/*
			void mousePressEvent(QMouseEvent* event){
			QPainter painter(this);
			painter.drawPoint(event->x(),event->y());
			cerr<<"!!";

			}
		 */
		QPainter painter;
		int ox;
		int oy;
		int width;
		int heigh;

};
class Pbutton :public QPushButton{

	public:
	Pbutton(int s,Paint* paint,QWidget* parent=0):state(s),QPushButton(parent){
		p=paint;
	}
	int state;
	Paint* p;
	void mousePressEvent(QMouseEvent*);
	void mouseReleaseEvent(QMouseEvent*){}
	void mouseMoveEvent(QMouseEvent*){}
	
};
class ColorButton: public QToolButton{
	public:
	ColorButton(int r,int g,int b,Paint* paint,QWidget* parent):color(r,g,b),
	QToolButton(parent){
		p=paint;
	}
	QColor color;
	Paint* p;
	void mousePressEvent(QMouseEvent*);
	void mouseReleaseEvent(QMouseEvent*){}
	void mouseMoveEvent(QMouseEvent*){}

};
class Paint: public QWidget{
	public:
		Paint(int width,int heigh,QObject* parent=0):
			p(width,heigh),counter(0),state(0),totalst(3),color(0,0,0),
			pen(color),pwidth(1){
			pen.setWidth(pwidth);
			resize(1100,600);	
			Pbutton* button;
			for(int i=0;i<=totalst;++i){
				button=new Pbutton(i,this,this);
				button->setGeometry(1000,i*100,100,100);
				buttons.push_back(button);
			}
			ColorButton* cbutton;
			
			cbutton =new ColorButton(255,0,0,this,this);
			cbutton->setGeometry(1000,400,50,50);
			cbutton =new ColorButton(0,255,0,this,this);
			cbutton->setGeometry(1000,450,50,50);
			cbutton =new ColorButton(0,0,255,this,this);
			cbutton->setGeometry(1000,500,50,50);
			cbutton =new ColorButton(0,0,0,this,this);
			cbutton->setGeometry(1050,400,50,50);
			cbutton =new ColorButton(255,255,255,this,this);
			cbutton->setGeometry(1050,450,50,50);
					
		}
		void paintEvent(QPaintEvent* event){
			QPainter painter(this);
			//			Pixmap pixmap(300,300);
			painter.setPen(pen);
			painter.drawImage(0,0,p);
			QPixmap colorp(50,50);
			colorp.fill(color);
			painter.drawPixmap(1050,500,colorp);
			if(state==1)
				painter.drawRect(stx,sty,posx-stx,posy-sty);
			if(state==2)
				painter.drawEllipse(stx,sty,posx-stx,posy-sty);
//			QPixmap so(4,4);
//			so.fill(QColor(255,255,255));
//			painter.drawPixmap(posx-2,posy-2,so);

		}
		void mousePressEvent(QMouseEvent* event){
			if(state==0){
				p.drawp(event->x(),event->y());
			}
			if(state==3){
				p.paintr(event->x(),event->y(),color,pwidth);
			}
			stx=event->x();
			sty=event->y();
			posx=event->x();
			posy=event->y();
			/*
				for(size_t i=0;i<xx.size();++i){
				p.drawp(xx[i],yy[i]);
				}
			 */
			//	xx.clear();
			//	yy.clear();
			update();
		}
		void mouseMoveEvent(QMouseEvent* event){
			if(state==0){
				p.drawp(event->x(),event->y(),1);
			}
			posx=event->x();
			posy=event->y();

			//			update(event->x(),event->y(),1,1);
			update();
			//			xx.push_back(event->x());
			//			yy.push_back(event->y());
			//			cerr<<(counter++)<<" ";

		}
		void mouseReleaseEvent(QMouseEvent* event){
			posx=event->x();
			posy=event->y();
			if(state==1){
				p.drawrec(stx,sty,posx,posy,state);	
			}
			if(state==2){
				p.drawrec(stx,sty,posx,posy,state);
			}
			stx=posx=heigh;
			sty=posy=width;
			update();
		}

		Pixmap p;

		//		vector<int> xx;
		//		vector<int> yy;
		int counter;
		int state;
//		int substate;
		int posx;
		int posy;
		int stx;
		int sty;
		int heigh;
		int width;
		int pwidth;
		int totalst;
		QColor color;
		QPen pen;
		vector<Pbutton*> buttons;
};

void Pbutton::mousePressEvent(QMouseEvent* event){
	for(int i=0;i<=p->totalst;++i){
		if(p->buttons[i]==this)
			p->buttons[i]->setDown(true);
		else
			p->buttons[i]->setDown(false);
	}
	p->state=state;
}
void  ColorButton::mousePressEvent(QMouseEvent* event){
	p->color=color;
	p->pen.setColor(color);
	p->p.painter.setPen(p->pen);
	p->update();
}
int main(int argv,char** args){

	QApplication app(argv,args);
	Paint window(1000,600);

	window.show();

	/*
		QWidget window;
		window.resize(300,300);




		QPushButton button("button",&window);
		button.setGeometry(110,110,60,60);
		QObject::connect(&button,SIGNAL(clicked()),&app,SLOT(quit()));

		window.show();
	 */
	/*
		QTextEdit* textEdit=new QTextEdit;
		QPushButton* button=new QPushButton("button");


		button->resize(300,300);
		button->setFont(QFont("Times",18,QFont::Bold));

		QObject::connect(button,SIGNAL(clicked()),&app,SLOT(quit()));
		button->show();
	 */

	/*
		QVBoxLayout* layout=new QVBoxLayout;
		layout->addWidget(textEdit);
		layout->addWidget(button);

		QWidget window;
		window.setLayout(layout);

		window.show();
	 */
	//textEdit.show();
	//button.show();
	return app.exec();

}
