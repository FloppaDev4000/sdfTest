// test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// includes
#include <iostream>
#include "raylib.h"
#include "raymath.h"

// namespace
using namespace std;

// funcs
float sdCircle(Vector2, Vector2, float);
float sdBox(Vector2, Vector2, Vector2, float);
float vecLen(Vector2);
int twoDtoOneD(int, int, int);
Vector2 oneDtoTwoD(int, int);
float smoothMin(float, float, float);
Vector2 absVec(Vector2);
void drawPixelScaled(int, int, Color, int, int);

// main method
int main()
{
    // sine parameters
    float amplitude = 1.0f;
    float displace = 0.0f;
    float frequency = 0.2f;

    // window scale
    const int windowScale = 2;

    // resolution vars
    const int width{ 300 * windowScale};
    const int height{ 200 * windowScale};
    const int resScale = 1*windowScale; // divide resolution by this amount

    // circle data
    Vector2 circle1 = { width/2, height/2 };
    float radius1 = 50*windowScale;
    bool select1 = false;

    // box data
    Vector2 rect = { 300*windowScale, 200*windowScale };
    Vector2 rectDims = { 80*windowScale, 60*windowScale };
    float rectRound = 50*windowScale;
    float maxRound = min(rectDims.x, rectDims.y);
    bool select2 = false;

    // for smooth min function
    float smoothing = 15*windowScale;

    // is drawing shape outlines
    bool isDebug = false;

    // for shape mouse selection
    Vector2 mouseOffset = { 0, 0 };

    InitWindow(width, height, "program");
    SetTargetFPS(60);


    // main loop   
    while (WindowShouldClose() == false)
    {

        // -------------------------------------------- INPUT

        // sine params input
        if (IsKeyDown(KEY_A)) // this kinda doesnt do anything rn
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

        //----------------------- SHAPE MOVEMENT
        
        if (IsMouseButtonPressed(0)) // select shape
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
        if (IsMouseButtonReleased(0)) // deselect shape
        {
            select1 = false;
            select2 = false;
        }

        // move shapes
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
        if (smoothing <= 0) // prevent negative smoothing (0.0f causes problem cuz divide by zero)
        {
            smoothing = 0.0f;
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
        else if (rectRound >= maxRound)
        {
            rectRound = maxRound;
        }

        // toggle debug
        if (IsKeyPressed(KEY_SPACE))
        {
            isDebug = !isDebug;
        }
        

        BeginDrawing();

        Color c = { 0, 0, 0 , 255 };
        float dst;
        float sinValue;
        unsigned char colValue = 50;

        Color prevXCol;
        Color prevYCol;

        // loop over every pixel to get colour
        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++)
            {

                Vector2 pt = { x, y };

                dst = smoothMin(sdCircle(pt, circle1, radius1), sdBox(pt, rect, rectDims, rectRound), smoothing); // circle and rect
                //dst = sdCircle(pt, circle1, radius1); // circle only
                //dst = sdBox(pt, rect, rectDims, rectRound); // rect only

                // get sin from dst calculation
                sinValue = amplitude * sin(dst * frequency) + displace;
           
                // switch between 2 values based on result
                colValue = 50;
                if (sinValue > 0)
                {
                    colValue = 200;
                }
                
                if (abs(dst) < 3*windowScale) // border
                {
                    c = { 0, 0, 0 , 255 };
                }
                else if (dst < 0) // inside
                {
                    unsigned char colValue2 = (unsigned char)200.0 + (dst * 6);
                    if (dst * 6 < -150)
                    {
                        colValue2 = 25;
                    }
                    
                    c = { 200, colValue2, 50, 255 };
                }
                else // outside
                {

                    c = { 250, 100, colValue, 255 };
                }

                // draw four pixels for each calculation (50% resolution scale)
                drawPixelScaled(x, y, c, resScale, resScale);
                y += resScale - 1;
            }
            x += resScale - 1;
        }

        // shape outlines ( unaffected by res scale )
        if (isDebug)
        {
            DrawCircleLines(circle1.x, circle1.y, radius1, BLACK);
            Vector2 alteredRect = rect - rectDims;
            Vector2 alteredDims = rectDims * 2;
            Rectangle r = { alteredRect.x, alteredRect.y, alteredDims.x, alteredDims.y };
            DrawRectangleRoundedLines(r, rectRound / maxRound, 2, BLACK);
        }

        DrawFPS(10, 10);

        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}

//----------SIGNED DISTANCE FIELDS
// circle distance field
float sdCircle(Vector2 point, Vector2 center, float radius)
{
    Vector2 pointTrans = point - center;
    return vecLen(pointTrans) - radius;
}

// box distance field
float sdBox(Vector2 point, Vector2 center, Vector2 b, float rounding = 0.0f)
{
    b.x -= rounding;
    b.y -= rounding;
    Vector2 pointTrans = point - center;
    Vector2 d = absVec(pointTrans) - b;
    return Vector2Length(Vector2Max(d, Vector2Zero())) + min(max(d.x, d.y), 0.0f) - rounding;
}

// length of vector
float vecLen(Vector2 v)
{
    return sqrt((v.x * v.x) + (v.y * v.y));
}

// convert 2D matrix entry to array index
int twoDtoOneD(int row, int rowSize, int col)
{
    return (row * rowSize) + col;
}

// convert array index to 2D matrix entry
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

// absolute vector value
Vector2 absVec(Vector2 v)
{
    return { abs(v.x), abs(v.y) };
}

// smooth minimum of 2 floats
float smoothMin(float a, float b, float k)
{
    if (k == 0.0f)
    {
        return min(a, b);
    }
    float h = max(k - abs(a - b), 0.0f) / k;
    return min(a, b) - h * h * k * (1.0 / 4.0);
}

// draw a big pixel
void drawPixelScaled(int x, int y, Color c, int xSpan, int ySpan)
{
    for (int i = 0; i < xSpan; i++)
    {
        for (int j = 0; j < ySpan; j++)
        {
            DrawPixel(x + i, y + j, c);
        }
    }
}
