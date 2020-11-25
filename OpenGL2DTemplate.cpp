#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glut.h>
#include <ctime>
#include <tchar.h>
#include<windows.h>
#include<mmsystem.h>
#pragma comment(lib, "winmm.lib")
//----------------------------------------------------------------------------------------------
void drawBorder();
void drawLanes();
void drawPlayer();
void drawGoal();
void drawCoins();
void drawPowerUpTime();
void allocateBridges();
void drawBombs();
void allocatePowerUps();
void drawTriangle(int x, int y, int side, float red, float green, float blue);
void drawRecUsingQuads(int x, int y, int w, int h, float red, float green, float blue);
void spe(int k, int x, int y);
void drawCircle(int x, int y, float r,float red, float green, float blue);
void Timer1(int value);
void Timer2(int value);
void print(int x, int y, char* string);
void playCrashSound();
void playSoundWin();
void actWin();
void playClockSound();
void playBombSound();
void playCoinSound();
void actLose();
//----------------------------------------------------------------------------------------------
int windowX=0; // x-coordinate for window location
int windowY=0; // y-coordinate for window location
int windowW=1400;    //Window width
int windowH=750;    // Window height
const int noOfLanes = 3;
int space = windowH - 50-(noOfLanes*10);
int prtSpace = space / (noOfLanes + 1);
int playerX=0;
int playerY=0;
int posX = 30;
int posY=30;
int bridges[noOfLanes];
int coins[noOfLanes+1];
int noOfCoins = 5;
int rotateCoin = 0;
int levelY[noOfLanes + 1];
int timePowerUps[noOfLanes+1];
int bombsPowerUps[noOfLanes ];
int t = 300;
int score = 0;
int currLevel = 0;
int goalX;
int goalY;
int win = 0;
//------------------------------------------------------------------------------------------------
void Anim()
{
	rotateCoin++;
	glutPostRedisplay();
}
void Display() {
	glClear(GL_COLOR_BUFFER_BIT);
	//PlaySound(NULL, NULL,0);
	if (win == 0) {
		if (t >= 0) {
			drawBorder();
			drawLanes();
			drawPlayer();
			drawGoal();
			drawCoins();
			drawPowerUpTime();
			drawBombs();
			glColor3f(0.8, 0.3, 0.8);
			char* p0s[20];
			sprintf((char*)p0s, "Score: %d, Time left:  %d,  ", score, t );
			print(windowW / 2 - 50, windowH - 20, (char*)p0s); 
			
		}
		else
		{
			drawBorder();
			glColor3f(0.4, 0.0, 0.5);
			char* p0s[20];
			sprintf((char*)p0s, "Game Is Over, Your Score is %d", score);
			print(windowW / 2 - 50, windowH / 2, (char*)p0s);
			PlaySound(NULL, 0, 0);
			glutPostRedisplay();
			PlaySound(TEXT("lose.wav"), NULL, SND_SYNC | SND_FILENAME |SND_NOSTOP);
			
		}
	}
	else
	{
		
		drawBorder();
		glColor3f(0.4, 0.0, 0.5);
		char* p0s[20];
		sprintf((char*)p0s, "YOU WON. Finally Sully saved Boo. Your Score is %d", score);
		print(windowW / 2 - 250, windowH / 2, (char*)p0s);
		PlaySound(NULL, 0, 0);
		glutPostRedisplay();
		PlaySound(TEXT("boo.wav"), NULL, SND_FILENAME );
		
	}

	glFlush();
}
void main(int argc, char** argr) {
	srand(time(NULL));
	glutInit(&argc, argr);
	//PlaySound(TEXT("theme.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP | SND_NOSTOP);
	glutInitWindowSize(windowW, windowH);
	glutInitWindowPosition(windowX, windowY);
	//random bridges location
	allocateBridges();
	// random power Ups allocation
	allocatePowerUps();
	//random coins location
	for (int i = 0; i < noOfLanes + 1; i++) {
		if (i == 0)
		{
			int end = windowW - 30 - ceil(0.2 * prtSpace);
			int start = 30 +ceil( 0.65 * prtSpace);
			coins[i] = rand() % (end - start) + start;
			if(coins[i]==bombsPowerUps[i]|coins[i]==timePowerUps[i])
				coins[i] = rand() % (end - start) + start;

		}
		else
		{
			if (i == noOfLanes)
			{
				int end = windowW - 35 - ceil(0.9 * prtSpace);
				int start = 30 + ceil(0.2 * prtSpace);
				coins[i] = rand() % (end - start) + start;
				if (coins[i] == bombsPowerUps[i] | coins[i] == timePowerUps[i])
					coins[i] = rand() % (end - start) + start;
			}
			else
			{
				int end = windowW - 30 - ceil(0.2 * prtSpace);
				int start = 30 + ceil(0.2 * prtSpace);
				coins[i] = rand() % (end - start) + start;
				if (coins[i] == bombsPowerUps[i] | coins[i] == timePowerUps[i])
					coins[i] = rand() % (end - start) + start;
			}
		}
	}
	// know each level y coordinates
	for (int i = 0; i <= noOfLanes; i++)
		levelY[i] = 25 + (prtSpace + 10) * i;



	glutCreateWindow("Maze Inc.");
	glutDisplayFunc(Display);
	glutIdleFunc(Anim);
	glutSpecialFunc(spe);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glClearColor(0.8, 0.3, 0.8, 0.0f);
	gluOrtho2D(0.0, windowW, 0.0, windowH);
	glPointSize(5);
	glutTimerFunc(1*1000, Timer1, 0);
	glutTimerFunc(10*1000, Timer2, 0);
	glutMainLoop();

}
//-----------------------------------------------------------------------------------------------
void  drawBorder()
{
	float red = 0.4;
	float green =0;
	float blue = 0.5;
	
	//right
	drawRecUsingQuads(windowW-25, 0, 25, windowH, red, green, blue);
	drawRecUsingQuads(windowW - 25, 0, 25, windowH, red, green, blue);
	//upper
	drawRecUsingQuads(0, windowH-25, windowW, 25, red, green, blue); 
	//left
	drawRecUsingQuads(0, 0, 25, windowH, red, green, blue);
	//lower
	drawRecUsingQuads(0, 0, windowW, 25, red, green, blue);
	//The Dots
	for(int i=0;i< windowH-20;i+=10)
		drawCircle(12, 12+i, 5, 0.8, 0.3, 0.8);
	for (int i = 0; i < windowH - 20; i += 10)
		drawCircle(windowW-12, 12 + i, 5, 0.8, 0.3, 0.8);
}
void drawLanes()
{

	float red1 = 0.4;
	float green1 =0;
	float blue1 = 0.5;
	float red2 = 0.8;
	float green2 = 0.3;
	float blue2 = 0.8;
	for (int i = 0; i < noOfLanes; i++)
	{
		
		drawRecUsingQuads(25,25+ (prtSpace*(i+1))+(10*i), windowW-50, 10,red1,green1,blue1);
		drawRecUsingQuads(25, 25 + (prtSpace * (i + 1)) + (10 * i), windowW-50, 10, red1, green1, blue1);

		drawRecUsingQuads(bridges[i], 25 + (prtSpace * (i+1)) + (10 * i ), 0.75*prtSpace, 10, red2, green2, blue2);
		drawRecUsingQuads(bridges[i], 25 + (prtSpace * (i + 1)) + (10 * i), 0.75 * prtSpace, 10, red2, green2, blue2);

		drawCircle( bridges[i]-20, 25 + (prtSpace * (i + 1)) + (10 * i)+5, 5, 0.8, 0.3, 0.8);
		drawCircle(bridges[i] + 20+ 0.75 * prtSpace, 25 + (prtSpace * (i + 1)) + (10 * i) + 5, 5, 0.8, 0.3, 0.8);
	
	}
}
void drawRecUsingQuads(int x, int y, int w, int h, float red, float green, float blue)
{
	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glColor3f(red,green, blue);
	glVertex2f(x + w, y);
	glColor3f(red, green, blue);
	glVertex2f(x + w, y + h);
	glColor3f(red, green, blue);
	glVertex2f(x, y + h);
	glColor3f(red, green, blue);
	glEnd();
}
void drawCoins()
{
	
	for (int i = 0; i <= noOfLanes; i++)
	{
		if (coins[i] != 0) {
			glPushMatrix();
			int w = prtSpace * 0.2;
			int h = prtSpace * 0.7;
			int x = coins[i];
			int y = (prtSpace * 0.1) + ((prtSpace + 10) * i) + 25;
			int tx = x + 0.5 * w;
			int ty = y + 0.5 * h; 
			glTranslated(tx, ty, 0);
			glRotatef(rotateCoin, 0, 0, 1);
			glTranslated(-tx, -ty, 0);
			glPushMatrix();


			drawRecUsingQuads(x, y, w, h, 1, 0.9, 0.0);
			drawRecUsingQuads(x, y, w, h, 1, 0.9, 0.0);

			drawRecUsingQuads(x, y + 0.1 * h, w, 3, 0.0, 0.0, 0.0);
			drawRecUsingQuads(x, y + 0.1 * h, w, 3, 0.0, 0.0, 0.0);

			drawRecUsingQuads(x, y + 0.9 * h, w, 3, 0.0, 0.0, 0.0);
			drawRecUsingQuads(x, y + 0.9 * h, w, 3, 0.0, 0.0, 0.0);

			drawRecUsingQuads(x + 0.2 * w, y + 0.3 * h, w * 0.6, 0.5 * h, 0.5, 0.6, 0.6);
			drawRecUsingQuads(x + 0.2 * w, y + 0.3 * h, w * 0.6, 0.5 * h, 0.5, 0.6, 0.6);

			drawCircle(x + 0.5 * w, y + 0.3 * h + 5, 3, 1, 0, 0);
			drawCircle(x + 0.5 * w, y + 0.3 * h + 15, 3, 1, 0, 0);
			drawCircle(x + 0.5 * w, y + 0.3 * h + 25, 3, 1, 0, 0);

			glPopMatrix();
			glPopMatrix();
		}

		

	}


}
void drawCircle(int x, int y, float r, float red, float green, float blue) {
	glPushMatrix();
	glTranslatef(x, y, 0);
	glColor3f(red, green, blue);
	GLUquadric* quadObj = gluNewQuadric();
	gluDisk(quadObj, 0, r, 50, 50);
	glPopMatrix();
}

void drawPlayer()
{
	glPushMatrix();
	glTranslated(playerX, playerY,0);
	glPushMatrix();
	int headW=prtSpace*0.25;
	int bodyW=prtSpace*0.45;
	//Horns
	drawTriangle(30 + ((bodyW - headW) / 2), 30 + bodyW+headW, 0.25*headW, 1.0, 1.0,1.0);
	drawTriangle(30 + ((bodyW - headW) / 2), 30 + bodyW + headW, 0.25 * headW, 1.0, 1.0, 1.0);
	drawTriangle(30 + ((bodyW - headW) / 2)+headW - 0.25 * headW, 30 + bodyW + headW, 0.25 * headW, 1.0, 1.0, 1.0);
	drawTriangle(30 + ((bodyW - headW) / 2) + headW- 0.25 * headW, 30 + bodyW + headW, 0.25 * headW, 1.0, 1.0, 1.0);
	//Head And Body
	drawRecUsingQuads(30+(bodyW-headW)/2, 30 + bodyW , headW,headW,0.1, 0.8, 0.9);
	drawRecUsingQuads(30 + (bodyW - headW)/2, 30 + bodyW, headW, headW, 0.1, 0.8, 0.9);
	drawRecUsingQuads(30, 30, bodyW, bodyW, 0.1, 0.8, 0.9);
	drawRecUsingQuads(30, 30, bodyW, bodyW, 0.1, 0.8, 0.9);
	//Mouth
	glBegin(GL_LINE_LOOP);
	glLineWidth(6);
	glVertex2f(30 + ((bodyW - headW) / 2) + 0.3 * headW, 30 + bodyW + 0.3 * headW);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2f(30 + ((bodyW - headW) / 2) + 0.3 * headW, 30 + bodyW + 0.3 * headW);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2f(30 + ((bodyW - headW) / 2) + 0.5* headW, 30 + bodyW + 0.3 * headW-4);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2f(30 + ((bodyW - headW) / 2) + 0.7 * headW, 30 + bodyW + 0.3 * headW);
	glColor3f(0.0, 0.0, 0.0);
	glLineWidth(6);
	glEnd();
	//drawRecUsingQuads(30 + ((bodyW - headW) / 2) + 0.3 * headW, 30 + bodyW + 0.4 * headW, 0.4 * headW, 1, 0.0, 0.0, 0.0);
	//drawRecUsingQuads(30 + ((bodyW - headW) / 2) + 0.3 * headW, 30 + bodyW + 0.4 * headW, 0.4 * headW, 1, 0.0, 0.0, 0.0);

	//Dots
		drawCircle(35, 40, 3, 0.4, 0.2, 0.8);
		drawCircle(30+bodyW-5, 40, 3, 0.4, 0.2, 0.8);
		drawCircle(30 + (bodyW/2) , 40, 3, 0.4, 0.2, 0.8);
		drawCircle(35, 40 + bodyW - 20, 3, 0.4, 0.2, 0.8);
		drawCircle(30+bodyW -5, 30 + bodyW - 20, 3, 0.4, 0.2, 0.8);
		drawCircle(30 + bodyW - 15, 40 + bodyW - 20, 3, 0.4, 0.2, 0.8);
		drawCircle(30 + (bodyW /2)- 5, 40 + bodyW - 30, 3, 0.4, 0.2, 0.8);
	

	//Eyes
	glBegin(GL_POINTS);
	glVertex2f(30 + ((bodyW - headW) / 2) + 0.3 * headW, 30 + bodyW + headW - 0.3 * headW);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2f(30 + ((bodyW - headW) / 2) + 0.3 * headW, 30 + bodyW + headW - 0.3 * headW);
	glColor3f(0.0, 0.0, 0.0);
	glEnd();

	glBegin(GL_POINTS);
	glVertex2f(30 + ((bodyW - headW) / 2) + 0.6 * headW, 30 + bodyW + headW - 0.3 * headW);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2f(30 + ((bodyW - headW) / 2) + 0.6 * headW, 30 + bodyW + headW - 0.3 * headW);
	glColor3f(0.0, 0.0, 0.0);
	glEnd();

	glPopMatrix();
	glPopMatrix();
}
void drawGoal()
{
	int w = prtSpace * 0.7;
	int h = w + 15;
	//Attributes for Pink Flower
	int r2 = 0.14 * w;
	int c2X = windowW - (30 + w) + 0.5 * w;
	int c2Y = windowH - (30 + h) + 0.6 * h;
	float red2 = 1;
	float green2 = 0.6;
	float blue2 = 0.7;
	//Attributes for Orange Flower
	int r1 = 0.09* w;
	int c1X = windowW - (30 + w) + 0.2 * w;
	int c1Y= windowH - (30 + h)+0.2*h;
	float red1 = 0.9;
	float green1 = 0.6;
	float blue1 = 0.2;

	goalX = windowW - (30 + w);
	goalY = windowH - (30 + h);
	drawRecUsingQuads(windowW - (30 + w), windowH - (30 +h), w, h, 1.0f, 1.0f, 1.0f);
	drawRecUsingQuads(windowW - (30 + w), windowH - (30 + h), w, h, 1.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
	glLineWidth(7);
	glVertex2d(windowW - (30 + w), windowH - (30 + h));
	glColor3f(0.0, 0.0, 0.0);
	

	
	glVertex2d(windowW - (30 + w)+w, windowH - (30 + h));
	glColor3f(0.0, 0.0, 0.0);
	

	glVertex2d(windowW - (30 + w)+w, windowH - (30 + h)+h);
	glColor3f(0.0, 0.0, 0.0);
	
	
	glVertex2d(windowW - (30 + w), windowH - (30 + h)+h);
	glColor3f(0.0, 0.0, 0.0);
	glLineWidth(7);
	glEnd();
	

	//Drawing  pink flower
	drawCircle(c2X, c2Y, r2, red2, green2, blue2);
	drawCircle(c2X+5, c2Y, r2, red2, green2, blue2);
	drawCircle(c2X, c2Y-7, r2, red2, green2, blue2);
	drawCircle(c2X + 5, c2Y-7, r2, red2, green2, blue2);
	drawCircle(c2X + 3, c2Y - 3, 0.07*w, 1, 0.9, 0.0);

	//Drawing Orange flower
	drawCircle(c1X, c1Y, r1, red1, green1, blue1);
	drawCircle(c1X + 5, c1Y, r1, red1, green1, blue1);
	drawCircle(c1X, c1Y - 7, r1, red1, green1, blue1);
	drawCircle(c1X + 5, c1Y - 7, r1, red1, green1, blue1);
	drawCircle(c1X + 3, c1Y - 3, 0.05 * w, 1, 0.9, 0.0);
	
	
	//Drawing doorknob
	
	drawCircle(windowW - (30 + w) + 0.1 * w, windowH - (30 + h) + 0.5 * h,5,0.6, 0.3, 0.0);
	

	}
void drawPowerUpTime()
{
		
		for (int k = 0; k < noOfLanes + 1; k++)
		{
			if (timePowerUps[k]!=0) {
				int r = prtSpace * 0.2;
				int x = timePowerUps[k];
				int y = (prtSpace * 0.5) + ((prtSpace + 10) * k) + 25;

				glPushMatrix();
				drawCircle(x, y, r, 1, 0.9, 0.0);
				drawTriangle(x - (prtSpace * 0.1) / 2, y, 0.1 * prtSpace, 0, 0, 0.8);
				drawTriangle(x - (prtSpace * 0.1) / 2, y, 0.1 * prtSpace, 0, 0, 0.8);
				glBegin(GL_POINTS);
				glVertex2d(x - (prtSpace * 0.1) / 2 + 0.05*prtSpace-1, y - 5);
				glColor3d(0, 0, 0.8);
				glEnd();

				glPopMatrix();
			}

		}
		
}
void drawBombs()
{
	for (int k = 0; k < noOfLanes; k++)
	{
		if (bombsPowerUps[k] != 0) {
			int r = prtSpace * 0.2;
			int x = bombsPowerUps[k];
			int y = (prtSpace * 0.5) + ((prtSpace + 10) * k) + 25;

			glPushMatrix();
			drawCircle(x, y, r, 0.0, 0.0, 0.0);
			drawRecUsingQuads(x - 0.5 * r, y, r, 1.5*  r, 0.0, 0.0, 0.0);
			drawRecUsingQuads(x -2, y, 2, 2 * r, 0.0, 0.0, 0.0);
			glPopMatrix();
		}

	}
}
void drawTriangle(int x, int y, int side, float red, float green, float blue)
{
	glBegin(GL_TRIANGLES);
	glVertex2f(x, y);
	glColor3f(red, green, blue);
	
	glVertex2f(x+side, y );
	glColor3f(red, green, blue);
	glVertex2f(x + 0.5 * side, sqrt(5) * side + y);
	glColor3f(red, green, blue);
	
	glEnd();

}

void spe(int k, int x, int y)// keyboard special key function takes 3 parameters
							
{
	int yMin = levelY[currLevel];
	int yMax = yMin + prtSpace;
	int coin = coins[currLevel];
	int time = timePowerUps[currLevel];
	int bomb = bombsPowerUps[currLevel];
	int bridge = bridges[currLevel];

	if (k == GLUT_KEY_RIGHT)//if the right arrow is pressed, then the object will be translated in the x axis by 5. (moving right)
	{
		if (posX + 10 < windowW - (30+0.45*prtSpace))
		{
			if ((posY + 0.7 * prtSpace) <= yMax+10) {
				playerX += 10;
				posX += 10;
				if (posX >= coin - 0.2 * prtSpace & posX <= coin + 0.2 * prtSpace) {
					coins[currLevel] = 0;
					score += 10;
					playCoinSound();
				}
				if (posX >= time - 0.2 * prtSpace & posX <= time + 0.2 * prtSpace) {
					timePowerUps[currLevel] = 0;
					t += 20;
					glutPostRedisplay();

					playClockSound();
				}
				if (posX >= bomb - 0.2 * prtSpace & posX <= bomb + 0.2 * prtSpace) {
					bridges[currLevel] = posX - 10;
					bombsPowerUps[currLevel] = 0;
					playBombSound();

				}
				if (posX >= goalX & posX <= goalX + 0.7 * prtSpace & currLevel == noOfLanes) {
					win = 1;

				}
			}
			else
			{
				playCrashSound();
			}


		}
		else
		{
			playCrashSound();
		}

	}
		
		if (k == GLUT_KEY_LEFT)//if the left arrow is pressed, then the object will be translated in the x axis by -5. (moving left)
		{
			if(posX-10>25)

			{
				
				if ((posY + 0.7 * prtSpace) <= yMax+10) {
					playerX -= 10;
					posX -= 10;
					if (posX >= coin - 0.2 * prtSpace & posX <= coin + 0.2 * prtSpace) {
						coins[currLevel] = 0;
						score += 10;
						playCoinSound();
					}
					if (posX >= time - 0.2 * prtSpace & posX <= time + 0.2 * prtSpace) {
						timePowerUps[currLevel] = 0;
						t += 20;
						glutPostRedisplay();
						playClockSound();
					}
					if (posX >= bomb - 0.2 * prtSpace & posX <= bomb + 0.2 * prtSpace) {
						bridges[currLevel] = posX - 10;
						bombsPowerUps[currLevel] = 0;
						playBombSound();

					}
				}
				else
				{
					playCrashSound();
				}
				
			
			}
			else
			{
				playCrashSound();
			}
	}
		
		if (k == GLUT_KEY_UP)//if the up arrow is pressed, then the object will be translated in the y axis by 5. (moving upwords)
		{
			if(posY+10<windowH-(30+0.7*prtSpace))
			{
				if ((posY+0.7*prtSpace) + 10 <= yMax) {
					playerY += 10;
					posY += 10;
				}
				else
				{
					if (posX >= bridge && posX +0.45*prtSpace<= (bridge + 0.75 * prtSpace)) {
						
						playerY += 10;
						posY += 10;
						if(posY>yMax)
							currLevel++;
					}
					else
					{
						playCrashSound();
					}
				}
			}
			else
			{
				playCrashSound();
			}
	}
		
		if (k == GLUT_KEY_DOWN)//if the down arrow is pressed, then the object will be translated in the y axis by -5. (moving downwords)
		{
			if(posY-10>25)
			{
				if (posY - 10 >= yMin)
				{
					playerY -= 10;
					posY -= 10;

				}
				else
				{
					if (posX >= bridges[currLevel-1] && posX + 0.45 * prtSpace <= (bridges[currLevel-1] + 0.75 * prtSpace)) {
						
						playerY -= 10;
						posY -= 10;
						if (posY + 0.65 * prtSpace < yMin)
							currLevel--;
					}
					else
					{
						playCrashSound();
					}
				}
			}
			else
			{
				playCrashSound();
			}
	}
	
	glutPostRedisplay();//redisplay to update the screen with the changes
}
void allocateBridges()
{
	if (t == 30) {
		for (int i = 0; i < noOfLanes; i++)
		{
			int end = windowW - 45 - ceil(0.75 * prtSpace);
			int start = 25;
			bridges[i] = rand() % (end - start) + start;
		}
	}
	else
	{
		for (int i = 0; i < noOfLanes; i++)
		{
			if (i != currLevel) {
				int end = windowW - 45 - ceil(0.75 * prtSpace);
				int start = 25;
				bridges[i] = rand() % (end - start) + start;
			}
		}

	}
}
void Timer1(int value) {
	
		//decrement the Time Left
		t--;
	
	// ask OpenGL to recall the display function to reflect the changes on the window
	glutPostRedisplay();
	// recall the Timer function after 1 seconds (1,000 milliseconds)
	glutTimerFunc(1 * 1000, Timer1, 0);
	
	
}
void Timer2(int value) {
	//reallocate bridges
	allocateBridges();
	//reallocate powerUps
	allocatePowerUps();
	// ask OpenGL to recall the display function to reflect the changes on the window
	glutPostRedisplay();
	
	// recall the Timer function after 10 seconds (10,000 milliseconds)
	glutTimerFunc(10 * 1000, Timer2, 0);

}
void print(int x, int y, char* string)
{
	int len, i;

	//set the position of the text in the window using the x and y coordinates
	glRasterPos2f(x, y);

	//get the length of the string to display
	len = (int)strlen(string);

	//loop to display character by character
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}
void allocatePowerUps()
{
	//random power up time location
	int k = rand() % (noOfLanes + 1);
	int j = rand() % (noOfLanes + 1);
	for (int i = 0; i < noOfLanes + 1; i++) {
		if (i == k | i == j) {
			if (i == 0)
			{
				int end = windowW - 30 - ceil(0.2 * prtSpace);
				int start = 30 + ceil(0.65 * prtSpace);
				timePowerUps[i] = rand() % (end - start) + start;
			}
			else
			{
				if (i == noOfLanes)
				{
					int end = windowW - 35 - ceil(0.9 * prtSpace);
					int start = 30 + ceil(0.2 * prtSpace);
					timePowerUps[i] = rand() % (end - start) + start;
				}
				else
				{
					int end = windowW - 30 - ceil(0.2 * prtSpace);
					int start = 30 + ceil(0.2 * prtSpace);
					timePowerUps[i] = rand() % (end - start) + start;
				}
			}
		}
		else
			timePowerUps[i] = 0;
	}
	//random bombs power up allocation
	k = rand() % (noOfLanes);
	j = rand() % (noOfLanes);
	for (int i = 0; i < noOfLanes; i++) {
		if (i == k | i == j) {
			if (i == 0)
			{
				int end = windowW - 30 - ceil(0.2 * prtSpace);
				int start = 30 + ceil(0.65 * prtSpace);
				bombsPowerUps[i] = rand() % (end - start) + start;
				if (bombsPowerUps[i] == timePowerUps[i])
					bombsPowerUps[i] = rand() % (end - start) + start;
			}
			else
			{
				if (i == noOfLanes)
				{
					int end = windowW - 35 - ceil(0.9 * prtSpace);
					int start = 30 + ceil(0.2 * prtSpace);
					bombsPowerUps[i] = rand() % (end - start) + start;
					if (bombsPowerUps[i] == timePowerUps[i])
						bombsPowerUps[i] = rand() % (end - start) + start;
				}
				else
				{
					int end = windowW - 30 - ceil(0.2 * prtSpace);
					int start = 30 + ceil(0.2 * prtSpace);
					bombsPowerUps[i] = rand() % (end - start) + start;
					if (bombsPowerUps[i] == timePowerUps[i])
						bombsPowerUps[i] = rand() % (end - start) + start;
				}
			}
		}
		else
			bombsPowerUps[i] = 0;
	}
}
void playCrashSound()
{
	PlaySound(NULL, 0, 0);
	PlaySound(TEXT("solid.wav"), NULL,  SND_FILENAME |SND_NOSTOP);
	//PlaySound(TEXT("theme.wav"), NULL, SND_ASYNC | SND_FILENAME );

	
}
void playClockSound()
{
	PlaySound(NULL, 0, 0);
	PlaySound(TEXT("clock.wav"), NULL, SND_FILENAME | SND_NOSTOP );
	//PlaySound(TEXT("theme.wav"), NULL, SND_ASYNC | SND_FILENAME );


}
void playCoinSound()
{

	PlaySound(NULL, 0, 0);
	PlaySound(TEXT("coin.wav"), NULL, SND_FILENAME | SND_NOSTOP);
	//PlaySound(TEXT("theme.wav"), NULL, SND_ASYNC | SND_FILENAME );


}
void playSoundWin()
{
	PlaySound(NULL, 0, 0);
	PlaySound(TEXT("boo.wav"), NULL,SND_FILENAME | SND_LOOP );
}
void playBombSound()
{
	PlaySound(NULL, 0, 0);
	PlaySound(TEXT("bomb.wav"), NULL,  SND_FILENAME);
}
void actWin()
{
	
	drawBorder();
	glColor3f(0.4, 0.0, 0.5);
	char* p0s[20];
	sprintf((char*)p0s, "YOU WON. Finally Sully saved Boo. Your Score is %d", score);
	print(windowW / 2 - 250, windowH / 2, (char*)p0s);
	PlaySound(NULL, 0, 0);
	glutPostRedisplay();
	//PlaySound(TEXT("theme.wav"), NULL, SND_SYNC | SND_FILENAME );
	
}
void actLose()
{
	drawBorder();
	glColor3f(0.4, 0.0, 0.5);
	char* p0s[20];
	sprintf((char*)p0s, "Game Is Over, Your Score is %d", score);
	print(windowW / 2 - 50, windowH / 2, (char*)p0s);
	PlaySound(NULL, 0, 0);
	glutPostRedisplay();
	PlaySound(TEXT("lose.wav"), NULL, SND_SYNC | SND_FILENAME | SND_LOOP);
	
}




