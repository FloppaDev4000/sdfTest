#include "raylib.h";
#include <iostream>;

using namespace std;

void printC(Color);

int main3() {
	
	const int width{ 300 };
	const int height{ 200 };

	Color c = { 0, 0, 0 , 255 };

    InitWindow(width, height, "program");
    SetTargetFPS(60);

	while (WindowShouldClose() == false)
	{
        if (IsKeyDown(KEY_A)) // this kinda doesnt do anything rn
        {
            c.r -= 1;
            printC(c);
        }
        if (IsKeyDown(KEY_D))
        {
            c.r += 1;
            printC(c);
        }

        if (IsKeyDown(KEY_W))
        {
            c.g += 1;
            printC(c);
        }
        if (IsKeyDown(KEY_S))
        {
            c.g -= 1;
            printC(c);
        }

        if (IsKeyDown(KEY_Q))
        {
            c.b -= 1;
            printC(c);
        }
        if (IsKeyDown(KEY_E))
        {
            c.b += 1;
            printC(c);
        }

		BeginDrawing();

		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
                DrawPixel(x, y, c);
			}
		}

		DrawFPS(10, 10);

		EndDrawing();
	}
	return 0;
}

void printC(Color c)
{
    cout << "COLOR: " << (int)c.r << " " << (int)c.g << " " << (int)c.b << " " << (int)c.a << "\n";
}