#include "Random.h"
#include "Images.h"
#include "ST7735.h"
#include "tm4c123gh6pm.h"
#define GOTSHOT 6
#define DEAD 7
#define L  0
#define R  1
#define TL 2
#define TR 3
#define BL 4
#define BR 5

int duckx=60;
int ducky=95;
int duckw=24;
int duckh=24;
int duckstate=TR;
int killed=0;
int bullet=3;

#define UNFINISHED 0
#define SUCCESS 1
#define FAIL 2
int mission=UNFINISHED;


int rn=30;

void reflesh(){
	
	if (duckx<100 && duckstate==R && rn!=0)
	{
		duckx++;
		rn--;
	}
	else if (duckx>1  && duckstate==L && rn!=0)
	{
		duckx--;
		rn--;
	}
	else if (ducky>20 && duckx<100 && duckstate==TR && rn!=0)
	{
		ducky--;
		duckx++;
		rn--;
	}
	else if (ducky<90 && duckx<100 && duckstate==BR && rn!=0)
	{
		ducky=ducky+1;
		duckx=duckx+1;
		rn--;
	}
	else if (ducky<90 && duckx>1 && duckstate ==BL && rn!=0)
	{
		ducky++;
		duckx--;
		rn--;
	}
	else if (ducky>20 && duckx>1 && duckstate ==TL && rn!=0)
	{
		ducky--;
		duckx--;
		rn--;
	}
	
	else if(rn==0)
	{
		int rand=Random();
		if(rand<42)
		{
			duckstate=L;
			rn=30;
		}
		else if(rand<84)
		{
			duckstate=R;
			rn=30;
		}
		else if(rand<126)
		{
			duckstate=TL;
			rn=30;
		}
		else if(rand<168)
		{
			duckstate=TR;
			rn=30;
		}
		else if(rand<210)
		{
			
			duckstate=BL;
			rn=30;
		}
		else if(rand<255)
		{
			duckstate=BR;
			rn=30;
		}
		
	}
	else if (ducky ==20)
	{
		duckstate=BL;
		ducky++;
		rn=10;
	}
	else if (ducky ==90&& duckstate!=DEAD)
	{
		duckstate=TR;
		ducky--;
		rn=10;
	}
	
	else if (duckx ==100)
	{
		duckstate=L;
		duckx--;
		rn=10;
	}
	else if (duckx ==1)
	{
		duckstate=R;
		ducky++;
		rn=10;
	}
	if (duckstate==DEAD)
	{
		ducky++;
	}
}

void display_zero_duck()
{
	ST7735_DrawBitmap(56, 145, duckwhite, 20,20);
	ST7735_DrawBitmap(80, 145, duckwhite, 20,20);
}
void display_one_duck()
{
	ST7735_DrawBitmap(56, 145, duckred, 20,20);
	ST7735_DrawBitmap(80, 145, duckwhite, 20,20);
}
void display_two_duck()
{
	ST7735_DrawBitmap(56, 145, duckred, 20,20);
	ST7735_DrawBitmap(80, 145, duckred, 20,20);
}	

void check(int x, int y, int w, int h, int x2, int y2, int w2, int h2)
{
	if( x2+w2/2 < x+w  &&  x2+w/2 >x  &&  y2-h2/2 > y-h  &&  y2-h2/2 < y)
	{
		
		duckstate=GOTSHOT;
		killed++;
		
	}
	else
	{
		
		bullet--;
		if(bullet==0)
		{
			mission=FAIL;
		}
		
	}
}
bool is_pressed=false;
bool is_released=false;
int count=0;
int state=1;
int gotshotcount=0;
 

void display_duck()
{
	if(duckstate==R)
	{
					if(state==1)
		{
			ST7735_DrawBitmap(duckx, ducky, horizontal1bmp, duckw,duckh);
			count++;
		}
		else if(state==2)
		{
			ST7735_DrawBitmap(duckx, ducky, horizontal2bmp, duckw,duckh);
			count++;
		}
		else if(state==3)
		{
			ST7735_DrawBitmap(duckx, ducky, horizontal3bmp, duckw,duckh);
			count++;
		}
		else if (state==4)
		{
			ST7735_DrawBitmap(duckx, ducky, horizontal2bmp, duckw,duckh);
			count++;
		}
		
		if(count==100)
		{
			if(state==1)
			{
				state=2;
			}
			else if(state==2)
			{
				state=3;
			}				
			else if(state==3)
			{
				state=4;
			}
			else if (state==4)
			{
				state=1;
			}
			count=0;
		}
	}
	
	
	else if(duckstate==BR)
	{
		ST7735_DrawBitmap(duckx, ducky, horizontal2bmp, duckw,duckh);
	}
	else if(duckstate==BL)
	{
		ST7735_DrawBitmap(duckx, ducky, horizontal2bmpl, duckw,duckh);
	}
	
	else if(duckstate==L)
	{
		if(state==1)
		{
			ST7735_DrawBitmap(duckx, ducky, horizontal1bmpl, duckw,duckh);
			count++;
		}
		else if(state==2)
		{
			ST7735_DrawBitmap(duckx, ducky, horizontal2bmpl, duckw,duckh);
			count++;
		}
		else if(state==3)
		{
			ST7735_DrawBitmap(duckx, ducky, horizontal3bmpl, duckw,duckh);
			count++;
		}
		else if (state==4)
		{
			ST7735_DrawBitmap(duckx, ducky, horizontal2bmpl, duckw,duckh);
			count++;
		}
		
		if(count==100)
		{
			if(state==1)
			{
				state=2;
			}
			else if(state==2)
			{
				state=3;
			}				
			else if(state==3)
			{
				state=4;
			}
			else if (state==4)
			{
				state=1;
			}
			count=0;
		}
	}
	else if(duckstate==TR)
	{
		if(state==1)
		{
			ST7735_DrawBitmap(duckx, ducky, up1, duckw,duckh);
			count++;
		}
		else if(state==2)
		{
			ST7735_DrawBitmap(duckx, ducky, up2, duckw,duckh);
			count++;
		}
		else if(state==3)
		{
			ST7735_DrawBitmap(duckx, ducky, up3, duckw,duckh);
			count++;
		}
		else if (state==4)
		{
			ST7735_DrawBitmap(duckx, ducky, up2, duckw,duckh);
			count++;
		}
		
		if(count==100)
		{
			if(state==1)
			{
				state=2;
			}
			else if(state==2)
			{
				state=3;
			}				
			else if(state==3)
			{
				state=4;
			}
			else if (state==4)
			{
				state=1;
			}
			count=0;
		}
	}
	else if(duckstate==TL)
	{
		if(state==1)
		{
			ST7735_DrawBitmap(duckx, ducky, up1l, duckw,duckh);
			count++;
		}
		else if(state==2)
		{
			ST7735_DrawBitmap(duckx, ducky, up2l, duckw,duckh);
			count++;
		}
		else if(state==3)
		{
			ST7735_DrawBitmap(duckx, ducky, up3l, duckw,duckh);
			count++;
		}
		else if (state==4)
		{
			ST7735_DrawBitmap(duckx, ducky, up2l, duckw,duckh);
			count++;
		}
		
		if(count==100)
		{
			if(state==1)
			{
				state=2;
			}
			else if(state==2)
			{
				state=3;
			}				
			else if(state==3)
			{
				state=4;
			}
			else if (state==4)
			{
				state=1;
			}
			count=0;
		}
	}

	else if(duckstate==GOTSHOT)
	{
		if(gotshotcount<300)
		{
			ST7735_DrawBitmap(duckx, ducky, hit1, duckw,duckh);
			gotshotcount++;
			
		}
		if(gotshotcount==300)
		{
			duckstate=DEAD;
		}
		
	}
	
	else if(duckstate==DEAD)
	{
		ST7735_DrawBitmap(duckx, ducky, fall1, duckw,duckh);
	}
}


const unsigned short* peb[]={peb1,peb2,peb3,peb4,peb5,peb4,peb5,peb6,peb6,peb7,peb8,peb9,peb10,peb11,peb12,peb13,peb14};
void beginning()
{
	int timer=0;
	int list_length=17;
	int ptime=3000000;
	ST7735_DrawBitmap(0, 149, perfect_background_bmp, 128,150);
	display_zero_duck();
	for(int i=0;i<list_length;i++)
	{
		ST7735_DrawBitmap(0, 120, peb[i], 80,50);
		while(timer<ptime){timer++;} timer=0;
	}
}
const unsigned short* got[]={got1,got2,got3,got3,got2,got1};
void pick_duck()
{
	int timer=0;
	int list_length=6;
	int ptime=3000000;
	ST7735_DrawBitmap(0, 149, perfect_background_bmp, 128,150);
	if(killed==1)
	{display_one_duck();}
	if(killed==2)
	{display_two_duck();}
	
	for(int i=0;i<list_length;i++)
	{
		ST7735_DrawBitmap(duckx, 95, got[i], 26,29);
		while(timer<ptime){timer++;} timer=0;
	}
	ST7735_DrawBitmap(0, 149, perfect_background_bmp, 128,150);
	if(killed==1)
	{display_one_duck();}
	if(killed==2)
	{display_two_duck();}
}
	

void display_bullet()
{
	
	if(bullet==2)
	{
		ST7735_FillRect(7,128,8,9,ST7735_BLACK);
	}
	else if(bullet==1)
	{
		ST7735_FillRect(7,128,13,9,ST7735_BLACK);

	}
	else if(bullet==0)
	{
		ST7735_FillRect(7,128,21,9,ST7735_BLACK);

	}
}

const unsigned short* laughing[]={laugh0,laugh1,laugh2,laugh1,laugh2,laugh1,laugh2,laugh1,laugh2};
void display_laugh()
{
	int timer=0;
	int list_length=9;
	int ptime=3000000;
	for(int i=0;i<list_length;i++)
	{
		ST7735_DrawBitmap(51, 97, laughing[i], 20,30);
		while(timer<ptime){timer++;} timer=0;
	}
}

