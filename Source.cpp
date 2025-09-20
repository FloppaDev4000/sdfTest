/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

-- Copyright (c) 2020-2024 Jeffery Myers
--
--This software is provided "as-is", without any express or implied warranty. In no event
--will the authors be held liable for any damages arising from the use of this software.

--Permission is granted to anyone to use this software for any purpose, including commercial
--applications, and to alter it and redistribute it freely, subject to the following restrictions:

--  1. The origin of this software must not be misrepresented; you must not claim that you
--  wrote the original software. If you use this software in a product, an acknowledgment
--  in the product documentation would be appreciated but is not required.
--
--  2. Altered source versions must be plainly marked as such, and must not be misrepresented
--  as being the original software.
--
--  3. This notice may not be removed or altered from any source distribution.

*/

#include "raylib.h"


int main2()
{
    //The height and with of the window
    //the texture in this case will have the same height and width
    //making it cover the full window
    const  int height{ 1000 };
    const int width{ 1000 };

    //simple window nothing special about it 
    InitWindow(width, height, "Single Pixel");

    //Array of Color sturcts made on the heap 
    //Can do this on stack but you can run out of memory fast 
    //See Color struct in raylib.h line 247 for more info 
    Color* singlePixelColorArray = new Color[height * width];


    for (int i = 0; i < height * width; i++)
    {
        //You can pass in a pre defined color like the line below
        //singlePixelColorArray[i] = RED;
        //the following is to just be verbose to show what is happenign 
        //and to show that every pixel is being manualy set

        unsigned char red = (unsigned char)GetRandomValue(0, 255);
        unsigned char blue = (unsigned char)GetRandomValue(0, 255);
        unsigned char green = (unsigned char)GetRandomValue(0, 255);
        unsigned char alpha = (unsigned char)GetRandomValue(0, 255);

        singlePixelColorArray[i].r = red;
        singlePixelColorArray[i].g = green;
        singlePixelColorArray[i].b = blue;
        singlePixelColorArray[i].a = alpha;
    }

    //Creating the image from the pixel data by using the Image struct see raylib.h line 263 for more info
    Image singlePixelImage;
    singlePixelImage.data = singlePixelColorArray; //This is why the array had to be 1D this is were we pass in our pixel values
    singlePixelImage.width = width;  // sets the width of the image
    singlePixelImage.height = height; // sets the heigh of the image
    singlePixelImage.mipmaps = 1;     // sets mipmaps low is higher res image but dont hold me to that i never mess with that value 
    singlePixelImage.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8; // this is how the image expexts the pixal data to be set in see raylib.h line 883 for more infor 

    //creating a texture from the image we just made 
    Texture2D singlePixelTexture = LoadTextureFromImage(singlePixelImage);

    //these are the values that will be passed in to the DrawTexturePro() function to draw the texture to the window 
    Rectangle src{ 0,0,width,height };
    Rectangle dest{ 0,0,width,height };
    Vector2 org{ 0,0 };
    float rot{ 0 };

    while (!WindowShouldClose())
    {
        //-----uncomment to change every pixel in the texture per frame loop 
        // 
        //for (int i = 0; i < height * width; i++)
        //{
        //    //You can pass in a pre defined color like the line below
        //    //singlePixelColorArray[i] = RED;
        //    //the following is to just be verbose to show what is happenign 
        //    //and to show that every pixel is being manualy set

        //    unsigned char red = (int)GetRandomValue(0, 255);
        //    unsigned char blue = (int)GetRandomValue(0, 255);
        //    unsigned char green = (int)GetRandomValue(0, 255);
        //    unsigned char alpha = (int)GetRandomValue(0, 255);

        //    singlePixelColorArray[i].r = red;
        //    singlePixelColorArray[i].g = green;
        //    singlePixelColorArray[i].b = blue;
        //    singlePixelColorArray[i].a = alpha;
        //}

        //UpdateTexture(singlePixelTexture, singlePixelColorArray);

        //nothing special just a normal draw call 
        BeginDrawing();
        DrawTexturePro(singlePixelTexture, src, dest, org, rot, WHITE);
        DrawFPS(10, 10); //fps to show performance
        ClearBackground(WHITE);
        EndDrawing();
    }

    //unloading the texture we made and closing the window 
    UnloadTexture(singlePixelTexture);
    CloseWindow();
    return 0;
}