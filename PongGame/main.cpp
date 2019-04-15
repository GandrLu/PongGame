#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <Windows.h>
/// Pong Game by Luzius Koelling 14.04.2019

//// Initialize variables and values

struct s_Position
{
	int m_x;
	int m_y;
};

short const MS_PER_FRAME = 16;
short const AREA_HEIGHT = 10;
short const AREA_WIDTH = 30;

char a_PlayArea[AREA_HEIGHT][AREA_WIDTH];

s_Position * m_CursorPosition;
s_Position * m_CursorLastPosition;
s_Position * m_BallStartPosition;
s_Position * m_BallEndPosition;
s_Position * m_PaddlePosition;

short m_PaddleHalfSize;
short m_FrameThickness;
short m_LastPaddlePosition;
short m_Lifepoints;
short x, y, dx, dy, failureVar, incX, incY, pdx, pdy, ddy, ddx, deltaFastDirection, deltaSlowDirection;


void InitializeGame() {
	m_PaddleHalfSize = 2;
	m_FrameThickness = 1;
	m_LastPaddlePosition = AREA_WIDTH / 2;
	m_Lifepoints = 3;

	m_CursorLastPosition->m_x = 1;
	m_CursorLastPosition->m_y = 1;
	m_BallStartPosition->m_x = AREA_WIDTH / 2;
	m_BallStartPosition->m_y = AREA_HEIGHT - 2;
	m_BallEndPosition->m_x = 8;
	m_BallEndPosition->m_y = 0;
	m_PaddlePosition->m_x = m_BallStartPosition->m_x;
	m_PaddlePosition->m_y = m_BallStartPosition->m_y + 1;
}

//// Helping methods ////

void DrawFrame() {
	for (int j = 0; j < AREA_WIDTH + 2 * m_FrameThickness; j++)
	{
		std::cout << '+';
	}
	std::cout << std::endl;
}

void DrawPaddle() {
	for (int i = m_LastPaddlePosition - m_PaddleHalfSize; i <= m_LastPaddlePosition + m_PaddleHalfSize; i++) {
		a_PlayArea[m_PaddlePosition->m_y][i] = ' ';
	}
	for (int j = m_PaddlePosition->m_x - m_PaddleHalfSize; j <= m_PaddlePosition->m_x + m_PaddleHalfSize; j++) {
		a_PlayArea[m_PaddlePosition->m_y][j] = '#';
	}
	m_LastPaddlePosition = m_PaddlePosition->m_x;
}

int GetSign(int _X) {
	if (_X > 0) {
		return 1;
	}
	else if (_X < 0) {
		return -1;
	}
	else
	{
		return 0;
	}
}

double GetMyCurrentTime() {
	return std::time(0);
}

//// Bresenhams line algorithm ////

void InitializeBresenham() {
	x = m_BallStartPosition->m_x;//_X1
	y = m_BallStartPosition->m_y;//_Y1
	dx = m_BallEndPosition->m_x - m_BallStartPosition->m_x;//_X2 - _X1;
	dy = m_BallEndPosition->m_y - m_BallStartPosition->m_y;//_Y2 - _Y1;
	incX = GetSign(dx);
	incY = GetSign(dy);

	if (dx < 0) {
		dx = -dx;
	}
	if (dy < 0) {
		dy = -dy;
	}

	if (dx > dy) {
		pdx = incX; pdy = 0;
		ddx = incX; ddy = incY;
		deltaSlowDirection = dy; deltaFastDirection = dx;
	}
	else
	{
		pdx = 0; pdy = incY;
		ddx = incX; ddy = incY;
		deltaSlowDirection = dx; deltaFastDirection = dy;
	}

	failureVar = deltaFastDirection / 2;
	m_CursorPosition->m_x = x;
	m_CursorPosition->m_y = y;
}

//// Helping methods for movement calculation ////
// Has to be cleaned up

s_Position CalcutaleNextPosition() {
	s_Position nextPosition;
	float cathetus = m_BallEndPosition->m_x + (m_BallEndPosition->m_x - m_BallStartPosition->m_x);
	//float temp = 10 / b;
	//float angle = 180 - atanf(temp);
	nextPosition.m_x = cathetus;
	nextPosition.m_y = AREA_HEIGHT - 1 - m_CursorPosition->m_y;
	m_BallStartPosition->m_x = m_BallEndPosition->m_x;
	m_BallStartPosition->m_y = m_BallEndPosition->m_y;
	return nextPosition;
}

s_Position CalcutaleNextPositionLeft() {
	s_Position nextPosition;
	float cathetus = -(m_BallEndPosition->m_x);
	//float temp = 10 / cathetus;
	//float angle = 180 - atanf(temp);
	nextPosition.m_x = cathetus;
	nextPosition.m_y = m_BallEndPosition->m_y;
	m_BallStartPosition->m_x = m_CursorPosition->m_x;
	m_BallStartPosition->m_y = m_CursorPosition->m_y;
	return nextPosition;
}

s_Position CalcutaleNextPositionRight() {
	s_Position nextPosition;
	float cathetus = (AREA_WIDTH - 1) - abs(m_BallEndPosition->m_x - (AREA_WIDTH - 1));
	//float temp = 10 / cathetus;
	//float angle = 180 - atanf(temp);
	nextPosition.m_x = cathetus;
	nextPosition.m_y = m_BallEndPosition->m_y;
	m_BallStartPosition->m_x = m_CursorPosition->m_x;
	m_BallStartPosition->m_y = m_CursorPosition->m_y;
	return nextPosition;
}

//// Game loop methods ////

void ProcessInput() {
	if (GetAsyncKeyState('A') && m_PaddlePosition->m_x > m_PaddleHalfSize) {
		--m_PaddlePosition->m_x;
	}
	else if (GetAsyncKeyState('D') && m_PaddlePosition->m_x < AREA_WIDTH - m_PaddleHalfSize - 1) {
		++m_PaddlePosition->m_x;
	}
	else if (GetAsyncKeyState(VK_ESCAPE)) {
		m_Lifepoints = -1;
	}
}

void Update() {
	if (m_CursorPosition->m_y == 0
		|| a_PlayArea[m_CursorPosition->m_y - 1][m_CursorPosition->m_x] == '#'
		|| a_PlayArea[m_CursorPosition->m_y][m_CursorPosition->m_x] == '#') {
		*m_BallEndPosition = CalcutaleNextPosition();
		InitializeBresenham();
	}
	else if (m_CursorPosition->m_x == 0) {
		*m_BallEndPosition = CalcutaleNextPositionLeft();
		InitializeBresenham();
	}
	else if (m_CursorPosition->m_x == AREA_WIDTH - 1) {
		*m_BallEndPosition = CalcutaleNextPositionRight();
		InitializeBresenham();
	}
	else if (m_CursorPosition->m_y == AREA_HEIGHT - 1) {
		--m_Lifepoints;
		short tempBallPos = m_CursorPosition->m_x;
		m_BallStartPosition->m_x = m_PaddlePosition->m_x;
		m_BallStartPosition->m_y = AREA_HEIGHT - 2;
		m_BallEndPosition->m_x = 8;
		m_BallEndPosition->m_y = 0;
		InitializeBresenham();
		m_CursorPosition->m_x = tempBallPos;
		m_CursorPosition->m_y = AREA_HEIGHT - 1;
	}
	a_PlayArea[m_CursorPosition->m_y][m_CursorPosition->m_x] = 'o';
	a_PlayArea[m_CursorLastPosition->m_y][m_CursorLastPosition->m_x] = ' ';
	m_CursorLastPosition->m_x = m_CursorPosition->m_x;
	m_CursorLastPosition->m_y = m_CursorPosition->m_y;

	failureVar -= deltaSlowDirection;
	if (failureVar < 0) {
		failureVar += deltaFastDirection;
		x += ddx;
		y += ddy;
	}
	else
	{
		x += pdx;
		y += pdy;
	}
	m_CursorPosition->m_x = x;
	m_CursorPosition->m_y = y;
}

void Render() {
	system("cls");

	std::cout << "   __    _          __  " << std::endl;
	std::cout << "  |  \\  / \\  |\\  | /    |" << std::endl;
	std::cout << "  |__/  | |  | \\ | | _  |" << std::endl;
	std::cout << "  |     \\_/  |  \\| \\_/  #" << std::endl;

	DrawFrame();
	DrawPaddle();

	for (int i = 0; i < AREA_HEIGHT; i++)
	{
		std::cout << '+';
		for (int j = 0; j < AREA_WIDTH; j++)
		{
			std::cout << a_PlayArea[i][j];
		}
		std::cout << '+' << std::endl;
	}

	DrawFrame();
	std::cout << "Lifes: " << m_Lifepoints << std::endl;
	std::cout << "---------------------------------" << std::endl;
	std::cout << "Press A + D to move left and right" << std::endl;
	std::cout << "Press ESC to exit game" << std::endl;
	std::cout << "---------------------------------" << std::endl;
	std::cout << sizeof(m_CursorPosition) << std::endl;
	std::cout << sizeof(a_PlayArea) << std::endl;

	//// Debugging ////

	//std::cout << "x: " << m_CursorPosition->m_x <<	"  y: " << m_CursorPosition->m_y << std::endl;
	//std::cout << "x: " << m_CursorLastPosition->m_x << "  y: " << m_CursorLastPosition->m_y << std::endl;
	//std::cout << "End x: " << m_BallEndPosition->m_x << "  y: " << m_BallEndPosition->m_y << std::endl;
	//std::cout << "Start x: " << m_BallStartPosition->m_x << "  y: " << m_BallStartPosition->m_y << std::endl;
	//std::cout << "x: " << x <<	"  y: " << y << std::endl;
}

int main() {
	m_CursorPosition = new s_Position();
	m_CursorLastPosition = new s_Position();
	m_BallStartPosition = new s_Position();
	m_BallEndPosition = new s_Position();
	m_PaddlePosition = new s_Position();
	
	InitializeGame();
	InitializeBresenham();

	while (m_Lifepoints >= 0)
	{
		double start = GetMyCurrentTime();
		ProcessInput();
		Update();
		Render();

		Sleep(start + MS_PER_FRAME - GetMyCurrentTime());
	}

	delete m_CursorPosition;
	delete m_CursorLastPosition;
	delete m_BallStartPosition;
	delete m_BallEndPosition;
	delete m_PaddlePosition;
	return 0;
}