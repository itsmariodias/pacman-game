//PACMAN GAME

#include<stdio.h>
#include<stdlib.h>
#include<GL/glut.h>
#include<math.h>

#ifdef _WIN32
#include<windows.h>
#endif
#ifdef linux
#include<unistd.h>
#endif

//xr,yr store location of pacman
float xr = 112, yr = 260;
//flag stores direction key pressed
int flag = 0;
//switch for flip flop
int on = 0;
FILE *fp;

//create 2 matrix to store legal regions and pellet locations
int a[224][288] = {0};
int pellet[224][288] = {0};

//initialize score
int score = 0;

//assigning the legal regions by the x, y values given in the regions.txt file
void assignboundaries()
{
	FILE *file = fopen("regions.txt", "r");
	float x,y;
	while(!feof(file))
	{
		fscanf(file, "%f\t%f\n",&x, &y);
		a[(int)x][(int)y] = 1;
	}

	//storing a pellet location after every 8 pixels
	for(int i=4;i<224;i+=8)
	{
		for(int j=36;j<261;j+=8)
		{
			if(a[i][j])
			{
				pellet[i][j] = 1;
			}
		}
	}

	//extra pixels which were in the cage
	pellet[100][140] = 0;
	pellet[108][140] = 0;
	pellet[116][140] = 0;
	pellet[124][140] = 0;
}

//printing the pellet if not eaten on the display
void pellets()
{

	glColor3f(1, 0.5, 0);

	for(int i=4;i<224;i++)
	{
		for(int j=36;j<261;j++)
		{
			if(pellet[i][j])
			{
				glBegin(GL_POLYGON);
				glVertex2f(i-1,j-1);
				glVertex2f(i+1,j-1);
				glVertex2f(i+1,j+1);
				glVertex2f(i-1,j+1);
				glEnd();
			}
		}
	}

}

//custom delay function
void delay(int milli_seconds)
{
#ifdef _WIN32
Sleep(milli_seconds);
#endif

#ifdef linux
usleep(1000*milli_seconds);
#endif
}

//pacman character
void pacman_closed(){
	glColor3f(1, 1, 0);
	float angle;

	for(int i=0;i<360;i++)
	{
		angle = i*M_PI/180;
		glBegin(GL_LINES);
		glVertex2f(xr, yr);
		glVertex2f(xr + 5*cos(angle), yr + 5*sin(angle));
		glEnd();
	}
}

void pacman_open_left(){
	glColor3f(1, 1, 0);
	float angle;

	for(int i=0;i<360;i++)
	{
		if(i < 135 || i > 225)
		{
			angle = i*M_PI/180;
			glBegin(GL_LINES);
			glVertex2f(xr, yr);
			glVertex2f(xr + 5*cos(angle), yr + 5*sin(angle));
			glEnd();
		}
	}
}

void pacman_open_right(){
	glColor3f(1, 1, 0);
	float angle;

	for(int i=0;i<360;i++)
	{
		if(i > 45 && i < 325)
		{
			angle = i*M_PI/180;
			glBegin(GL_LINES);
			glVertex2f(xr, yr);
			glVertex2f(xr + 5*cos(angle), yr + 5*sin(angle));
			glEnd();
		}
	}
}

void pacman_open_down(){
	glColor3f(1, 1, 0);
	float angle;

	for(int i=0;i<360;i++)
	{
		if(i < 45 || i > 135)
		{
			angle = i*M_PI/180;
			glBegin(GL_LINES);
			glVertex2f(xr, yr);
			glVertex2f(xr + 5*cos(angle), yr + 5*sin(angle));
			glEnd();
		}
	}
}

void pacman_open_up(){
	glColor3f(1, 1, 0);
	float angle;

	for(int i=0;i<360;i++)
	{
		if(i < 225 || i > 325)
		{
			angle = i*M_PI/180;
			glBegin(GL_LINES);
			glVertex2f(xr, yr);
			glVertex2f(xr + 5*cos(angle), yr + 5*sin(angle));
			glEnd();
		}
	}
}

//boundary conditions for pacman
int boundary(int x, int y)
{

	if(a[x][y])
		return 1;
	if(x == 224)
	{
		xr = 0;
		return 1;
	}
	if(x == 0)
	{
		xr = 224;
		return 1;
	}
	return 0;
}

//determine which direction pacman moves in
void move_pacman()
{
	if(flag == 0)
		pacman_open_left();
	else if(flag == 1)
	{
		if(boundary(xr, yr-4))
		{
			yr -= 4;
			if(on)
			{
				pacman_open_up();
				on = 0;
			}
			else
			{
				pacman_closed();
				on = 1;
			}
		}
		else
		{
			pacman_open_up();
			on = 0;
		}
	}
	else if(flag == 2)
	{
		if(boundary(xr, yr+4))
		{
			yr += 4;
			if(on)
			{
				pacman_open_down();
				on = 0;
			}
			else
			{
				pacman_closed();
				on = 1;
			}
		}
		else
		{
			pacman_open_down();
			on = 0;
		}
	}
	else if(flag == 3)
	{
		if(boundary(xr-4, yr))
		{
			xr -= 4;
			if(on)
			{
				pacman_open_left();
				on = 0;
			}
			else
			{
				pacman_closed();
				on = 1;
			}
		}
		else
		{
			pacman_open_left();
			on = 0;
		}
	}
	else if(flag == 4)
	{
		if(boundary(xr+4, yr))
		{
			xr += 4;
			if(on)
			{
				pacman_open_right();
				on = 0;
			}
			else
			{
				pacman_closed();
				on = 1;
			}
		}
		else
		{
			pacman_open_right();
			on = 0;
		}
	}
	//checking if pacman has eaten a pellet or not
	if(pellet[(int)xr][(int)yr])
	{
		pellet[(int)xr][(int)yr] = 0;
		score++;
		printf("Score = %d\n", score);
	}
}

//to print the word 'score'
void score_word()
{
	glLineWidth(6);
	glColor3f(1, 1, 1);

	//S
	glBegin(GL_LINE_STRIP);
	glVertex2f(15, 5);
	glVertex2f(5, 5);
	glVertex2f(5, 11);
	glVertex2f(14, 11);
	glVertex2f(14, 18);
	glVertex2f(4, 18);
	glEnd();

	//C
	glBegin(GL_LINE_STRIP);
	glVertex2f(30, 5);
	glVertex2f(20, 5);
	glVertex2f(20, 18);
	glVertex2f(30, 18);
	glEnd();

	//O
	glBegin(GL_LINE_LOOP);
	glVertex2f(45, 5);
	glVertex2f(35, 5);
	glVertex2f(35, 18);
	glVertex2f(45, 18);
	glEnd();

	//R
	glBegin(GL_LINE_STRIP);
	glVertex2f(50, 19);
	glVertex2f(50, 5);
	glVertex2f(60, 5);
	glVertex2f(60, 11);
	glVertex2f(50, 11);
	glVertex2f(60, 19);
	glEnd();

	//E
	glBegin(GL_LINE_STRIP);
	glVertex2f(75, 5);
	glVertex2f(65, 5);
	glVertex2f(65, 18);
	glVertex2f(75, 18);
	glEnd();

	//middle of E
	glBegin(GL_LINES);
	glVertex2f(65, 11);
	glVertex2f(75, 11);
	glEnd();
}

//to display the given digit
void print_digit(int d, int x, int y)
{
	glLineWidth(6);
	switch(d)
	{
		case 0:
				glBegin(GL_LINE_LOOP);
				glVertex2f(x, y);
				glVertex2f(x-10, y);
				glVertex2f(x-10, y+13);
				glVertex2f(x, y+13);
				glEnd();
				break;
		case 1:
				glBegin(GL_LINE_STRIP);
				glVertex2f(x-1, y-1);
				glVertex2f(x-1, y+14);
				glEnd();
				break;
		case 2:
				glBegin(GL_LINE_STRIP);
				glVertex2f(x-11, y);
				glVertex2f(x, y);
				glVertex2f(x, y+7);
				glVertex2f(x-10, y+7);
				glVertex2f(x-10, y+13);
				glVertex2f(x+1, y+13);
				glEnd();
				break;
		case 3:
				glBegin(GL_LINE_STRIP);
				glVertex2f(x-11, y);
				glVertex2f(x, y);
				glVertex2f(x, y+13);
				glVertex2f(x-11, y+13);
				glEnd();
				glBegin(GL_LINES);
				glVertex2f(x, y+6);
				glVertex2f(x-11, y+6);
				glEnd();
				break;
		case 4:
				glBegin(GL_LINE_STRIP);
				glVertex2f(x-10, y-1);
				glVertex2f(x-10, y+7);
				glVertex2f(x, y+7);
				glVertex2f(x, y+14);
				glEnd();
				glBegin(GL_LINES);
				glVertex2f(x, y-1);
				glVertex2f(x, y+7);
				glEnd();
				break;
		case 5:
				glBegin(GL_LINE_STRIP);
				glVertex2f(x+1, y);
				glVertex2f(x-10, y);
				glVertex2f(x-10, y+7);
				glVertex2f(x, y+7);
				glVertex2f(x, y+13);
				glVertex2f(x-11, y+13);
				glEnd();
				break;
		case 6:
				glBegin(GL_LINE_STRIP);
				glVertex2f(x+1, y);
				glVertex2f(x-10, y);
				glVertex2f(x-10, y+7);
				glVertex2f(x, y+7);
				glVertex2f(x, y+13);
				glVertex2f(x-10, y+13);
				glVertex2f(x-10, y+7);
				glEnd();
				break;
		case 7:
				glBegin(GL_LINE_STRIP);
				glVertex2f(x-11, y);
				glVertex2f(x, y);
				glVertex2f(x, y+14);
				glEnd();
				break;
		case 8:
				glBegin(GL_LINE_LOOP);
				glVertex2f(x, y);
				glVertex2f(x-10, y);
				glVertex2f(x-10, y+13);
				glVertex2f(x, y+13);
				glEnd();
				glBegin(GL_LINES);
				glVertex2f(x, y+6);
				glVertex2f(x-11, y+6);
				glEnd();
				break;
		case 9:
				glBegin(GL_LINE_STRIP);
				glVertex2f(x, y+7);
				glVertex2f(x, y);
				glVertex2f(x-10, y);
				glVertex2f(x-10, y+7);
				glVertex2f(x, y+7);
				glVertex2f(x, y+13);
				glVertex2f(x-11, y+13);
				glEnd();
				break;
	}
}

//separating the digits to display them
void scoreboard()
{
	int d3, d2, d1;

	d3 = score/100;
	d2 = (score%100)/10;
	d1 = score%10;

	print_digit(d3, 99, 5);
	print_digit(d2, 114, 5);
	print_digit(d1, 129, 5);
}

//for the outer walls of the game
void outerwalls()
{
	//bottom
	glLineWidth(2);
	glColor3f(0, 0, 1);
	glBegin(GL_LINE_STRIP);
	glVertex2f(0, 151);
	glVertex2f(40, 151);
	glVertex2f(40, 176);
	glVertex2f(0, 176);
	glVertex2f(0, 271);
	glVertex2f(223, 271);
	glVertex2f(223, 176);
	glVertex2f(183, 176);
	glVertex2f(183, 151);
	glVertex2f(223, 151);
	glEnd();

	//top
	glBegin(GL_LINE_STRIP);
	glVertex2f(0, 128);
	glVertex2f(40, 128);
	glVertex2f(40, 103);
	glVertex2f(0, 103);
	glVertex2f(0, 24);
	glVertex2f(223, 24);
	glVertex2f(223, 103);
	glVertex2f(183, 103);
	glVertex2f(183, 128);
	glVertex2f(223, 128);
	glEnd();
}

//for inner walls of the game
void innerwalls()
{
	//bottom
	glLineWidth(2);
	glColor3f(0, 0, 1);
	glBegin(GL_LINE_STRIP);
	glVertex2f(0, 148);
	glVertex2f(43, 148);
	glVertex2f(43, 179);
	glVertex2f(3, 179);
	glVertex2f(3, 220);
	glVertex2f(19, 220);
	glVertex2f(19, 227);
	glVertex2f(3, 227);
	glVertex2f(3, 268);
	glVertex2f(220, 268);
	glVertex2f(220, 227);
	glVertex2f(204, 227);
	glVertex2f(204, 220);
	glVertex2f(220, 220);
	glVertex2f(220, 179);
	glVertex2f(180, 179);
	glVertex2f(180, 148);
	glVertex2f(223, 148);
	glEnd();

	//top
	glBegin(GL_LINE_STRIP);
	glVertex2f(0, 131);
	glVertex2f(43, 131);
	glVertex2f(43, 100);
	glVertex2f(3, 100);
	glVertex2f(3, 27);
	glVertex2f(108, 27);
	glVertex2f(108, 59);
	glVertex2f(115, 59);
	glVertex2f(115, 27);
	glVertex2f(220, 27);
	glVertex2f(220, 100);
	glVertex2f(180, 100);
	glVertex2f(180, 131);
	glVertex2f(223, 131);
	glEnd();
}

//L shaped blocks
void L_blocks()
{
	glLineWidth(2);
	glBegin(GL_LINE_LOOP);
	glVertex2f(20, 196);
	glVertex2f(43, 196);
	glVertex2f(43, 227);
	glVertex2f(36, 227);
	glVertex2f(36, 203);
	glVertex2f(20, 203);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2f(180, 196);
	glVertex2f(203, 196);
	glVertex2f(203, 203);
	glVertex2f(187, 203);
	glVertex2f(187, 227);
	glVertex2f(180, 227);
	glEnd();
}

//T shaped blocks
void T_blocks()
{
	glLineWidth(2);
	glBegin(GL_LINE_LOOP);
	glVertex2f(60, 76);
	glVertex2f(67, 76);
	glVertex2f(67, 100);
	glVertex2f(91, 100);
	glVertex2f(91, 107);
	glVertex2f(67, 107);
	glVertex2f(67, 131);
	glVertex2f(60, 131);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2f(84, 76);
	glVertex2f(139, 76);
	glVertex2f(139, 83);
	glVertex2f(115, 83);
	glVertex2f(115, 107);
	glVertex2f(108, 107);
	glVertex2f(108, 83);
	glVertex2f(84, 83);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2f(156, 76);
	glVertex2f(163, 76);
	glVertex2f(163, 131);
	glVertex2f(156, 131);
	glVertex2f(156, 107);
	glVertex2f(132, 107);
	glVertex2f(132, 100);
	glVertex2f(156, 100);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2f(84, 172);
	glVertex2f(139, 172);
	glVertex2f(139, 179);
	glVertex2f(115, 179);
	glVertex2f(115, 203);
	glVertex2f(108, 203);
	glVertex2f(108, 179);
	glVertex2f(84, 179);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2f(20, 244);
	glVertex2f(60, 244);
	glVertex2f(60, 220);
	glVertex2f(67, 220);
	glVertex2f(67, 244);
	glVertex2f(91, 244);
	glVertex2f(91, 251);
	glVertex2f(20, 251);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2f(84, 220);
	glVertex2f(139, 220);
	glVertex2f(139, 227);
	glVertex2f(115, 227);
	glVertex2f(115, 251);
	glVertex2f(108, 251);
	glVertex2f(108, 227);
	glVertex2f(84, 227);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2f(132, 244);
	glVertex2f(156, 244);
	glVertex2f(156, 220);
	glVertex2f(163, 220);
	glVertex2f(163, 244);
	glVertex2f(203, 244);
	glVertex2f(203, 251);
	glVertex2f(132, 251);
	glEnd();
}

//Square shaped blocks
void Square_blocks()
{
	glLineWidth(2);
	glBegin(GL_LINE_LOOP);
	glVertex2f(20, 44);
	glVertex2f(43, 44);
	glVertex2f(43, 59);
	glVertex2f(20, 59);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2f(60, 44);
	glVertex2f(91, 44);
	glVertex2f(91, 59);
	glVertex2f(60, 59);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2f(20, 76);
	glVertex2f(43, 76);
	glVertex2f(43, 83);
	glVertex2f(20, 83);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2f(132, 44);
	glVertex2f(163, 44);
	glVertex2f(163, 59);
	glVertex2f(132, 59);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2f(180, 44);
	glVertex2f(203, 44);
	glVertex2f(203, 59);
	glVertex2f(180, 59);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2f(180, 76);
	glVertex2f(203, 76);
	glVertex2f(203, 83);
	glVertex2f(180, 83);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2f(60, 148);
	glVertex2f(67, 148);
	glVertex2f(67, 179);
	glVertex2f(60, 179);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2f(156, 148);
	glVertex2f(163, 148);
	glVertex2f(163, 179);
	glVertex2f(156, 179);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2f(60, 196);
	glVertex2f(91, 196);
	glVertex2f(91, 203);
	glVertex2f(60, 203);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2f(132, 196);
	glVertex2f(163, 196);
	glVertex2f(163, 203);
	glVertex2f(132, 203);
	glEnd();
}

//ghost cage
void cage()
{
	glLineWidth(2);
	glBegin(GL_LINE_LOOP);
	glVertex2f(103, 127);
	glVertex2f(87, 127);
	glVertex2f(87, 152);
	glVertex2f(136, 152);
	glVertex2f(136, 127);
	glVertex2f(120, 127);
	glVertex2f(120, 124);
	glVertex2f(139, 124);
	glVertex2f(139, 155);
	glVertex2f(84, 155);
	glVertex2f(84, 124);
	glVertex2f(103, 124);
	glEnd();
}

void disp()
{
	glutSwapBuffers();
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	score_word();
	scoreboard();

	outerwalls();
	innerwalls();
	Square_blocks();
	T_blocks();
	L_blocks();
	cage();

	pellets();

	move_pacman();

	glFlush();
	//continously update display after every 10ms
	delay(100);
	glutPostRedisplay();
}

//to store which key was pressed to control pacman
void specialkey(int key, int x, int y)
{
	switch(key)
	{
	case GLUT_KEY_UP:
		flag = 1;
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:
		flag = 2;
		glutPostRedisplay();
		break;
	case GLUT_KEY_LEFT:
		flag = 3;
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:
		flag = 4;
		glutPostRedisplay();
		break;
	}
}

int main(int argv, char **argc)
{
	//initialise the legal regions only once to conserve memory
	assignboundaries();
	glutInit(&argv, argc);
	glutInitWindowSize(448, 576);
	glutCreateWindow("Pac-Man Game");
	gluOrtho2D(0, 224, 288, 0);
	glutDisplayFunc(disp);
	glutSpecialFunc(specialkey);
	glutMainLoop();
	return 0;
}
