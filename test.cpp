// test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "raylib.h"
#include "raylib-cpp.h"
#include "raymath.h"

using namespace std;

// funcs
float sdCircle(Vector2, Vector2, float);
float sdBox(Vector2, Vector2, Vector2, float);
float vecLen(Vector2);
int twoDtoOneD(int, int, int);
Vector2 oneDtoTwoD(int, int);
float smoothMin(float, float, float);
Vector2 absVec(Vector2);

int main()
{
    // vars
    float amplitude = 1.0f;
    float displace = 0.0f;
    float frequency = 0.2f;

    const int width{ 400 };
    const int height{ 300 };

    Vector2 circle1 = { width/2, height/2 };
    float radius1 = 50;
    bool select1 = false;

    Vector2 rect = { 300, 200 };
    Vector2 rectDims = { 80, 60 };
    float rectRound = 50;
    bool select2 = false;

    float moveSpeed = 60;
    float smoothing = 15;

    Vector2 mouseOffset = { 0, 0 };

    cout << "Hello World!\n";

    InitWindow(width, height, "program");
    SetTargetFPS(60);

    // pixel stuff
    Color* singlePixelColArray = new Color[width * height];

    for (int i = 0; i < width*height; i++)
    {
        Vector2 pt = oneDtoTwoD(i, width);
        float sinValue = amplitude * sin(sdCircle(pt, circle1, radius1) * frequency) + displace;
        singlePixelColArray[i].g = sinValue;
    }

    Image img;
    img.data = singlePixelColArray;
    img.width = width;
    img.height = height;
    img.mipmaps = 1;
    img.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;

    Texture2D tex = LoadTextureFromImage(img);

    Rectangle src{ 0, 0, width, height};
    Rectangle dest{ 0, 0, width, height};
    Vector2 org{ 0, 0 };
    float rot{ 0 };
        
    while (WindowShouldClose() == false)
    {
        for (int i = 0; i < width * height; i++)
        {
            Vector2 pt = oneDtoTwoD(i, width);
            float dst = smoothMin(sdCircle(pt, circle1, radius1), sdBox(pt, rect, rectDims, rectRound), smoothing);
            float sinValue = amplitude * sin(dst * frequency) + displace;

            int colValue = 50;

            if (sinValue > 0)
            {
                colValue = 200;
            }


            if (abs(dst) < 3) // border
            {
                singlePixelColArray[i].r = 0;
                singlePixelColArray[i].g = 0;
                singlePixelColArray[i].b = 0;
            }
            else if (dst < 0) // inside
            {
                singlePixelColArray[i].r = 200;
                singlePixelColArray[i].g = colValue;
                singlePixelColArray[i].b = 0;
            }
            else // outside
            {
                singlePixelColArray[i].r = 250;
                singlePixelColArray[i].g = 100;
                singlePixelColArray[i].b = colValue;
            }
        }

        UpdateTexture(tex, singlePixelColArray);

        // input
        if (IsKeyDown(KEY_A))
        {
            amplitude += GetFrameTime()*2;
            cout << "\nAMPLITUDE: " << amplitude;
        }
        if (IsKeyDown(KEY_D))
        {
            amplitude -= GetFrameTime()*2;
            cout << "\nAMPLITUDE: " << amplitude;
        }
        if (IsKeyDown(KEY_W))
        {
            displace += GetFrameTime()*2;
            cout << "\nDISPLACE: " << displace;
        }
        if (IsKeyDown(KEY_S))
        {
            displace -= GetFrameTime()*2;
            cout << "\nDISPLACE: " << displace;
        }
        if (IsKeyDown(KEY_Q))
        {
            frequency += GetFrameTime()*0.5;
            cout << "\nFREQUENCY: " << frequency;
        }
        if (IsKeyDown(KEY_E))
        {
            frequency -= GetFrameTime()*0.5;
            cout << "\nFREQUENCY: " << frequency;
        }
        if (IsKeyDown(KEY_V))
        {
            printf("\nSDF AT MOUSE: ");
            cout << sdCircle(GetMousePosition(), circle1, radius1);
        }

        // CIRCLE MOVEMENT
        

        // select circle
        if (IsMouseButtonPressed(0))
        {
            if (sdCircle(GetMousePosition(), circle1, radius1) < 0)
            {
                select1 = true;
                mouseOffset = GetMousePosition() - circle1;
                
            }
            else if (sdBox(GetMousePosition(), rect, rectDims, rectRound) < 0)
            {
                select2 = true;
                mouseOffset = GetMousePosition() - rect;
            }
        }
        if (IsMouseButtonReleased(0))
        {
            select1 = false;
            select2 = false;
        }

        // move circle
        if (select1)
        {
            circle1 = GetMousePosition() - mouseOffset;
        }
        else if (select2)
        {
            rect = GetMousePosition() - mouseOffset;
        }

        // control smoothing
        if (IsKeyDown(KEY_ONE))
        {
            smoothing -= GetFrameTime() * 45;
        }
        if (IsKeyDown(KEY_TWO))
        {
            smoothing += GetFrameTime() * 45;
        }
        if (smoothing <= 0)
        {
            smoothing = -0.1f;
        }

        // control rounding
        if (IsKeyDown(KEY_THREE))
        {
            rectRound -= GetFrameTime() * 45;
        }
        if (IsKeyDown(KEY_FOUR))
        {
            rectRound += GetFrameTime() * 45;
        }
        if (rectRound <= 0)
        {
            rectRound = 0.0f;
        }
        

        BeginDrawing();
        DrawTexturePro(tex, src, dest, org, rot, WHITE);

        DrawFPS(10, 10);
        ClearBackground(WHITE);

        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}

// distance fields
float sdCircle(Vector2 point, Vector2 center, float radius)
{
    Vector2 pointTrans = point - center;
    return vecLen(pointTrans) - radius;
}
float sdBox(Vector2 point, Vector2 center, Vector2 b, float rounding = 0.0f)
{
    b.x -= rounding;
    b.y -= rounding;
    Vector2 pointTrans = point - center;
    Vector2 d = absVec(pointTrans) - b;
    return Vector2Length(Vector2Max(d, Vector2Zero())) + min(max(d.x, d.y), 0.0f) - rounding;
}

float vecLen(Vector2 v)
{
    return sqrt((v.x * v.x) + (v.y * v.y));
}

int twoDtoOneD(int row, int rowSize, int col)
{
    return (row * rowSize) + col;
}

Vector2 oneDtoTwoD(int index, int rowSize)
{
    int rowsPassed = 0;
    while (index > rowSize)
    {
        index -= rowSize;
        rowsPassed++;
    }
    Vector2 pos = { index, rowsPassed };
    return pos;

}

Vector2 absVec(Vector2 v)
{
    return { abs(v.x), abs(v.y) };
}

float smoothMin(float a, float b, float k)
{
    float h = max(k - abs(a - b), 0.0f) / k;
    return min(a, b) - h * h * k * (1.0 / 4.0);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
