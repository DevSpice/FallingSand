#include <iostream>
#include "./Headers/Gamestate.h"
#include <raylib.h>

using namespace std;
using namespace fallingsandgame;

int main () {

    const int SCREEN_WIDTH = Width;
    const int SCREEN_HEIGHT = Height;
    
    GameState game{};

    cout << "Hello World" << endl;

    InitWindow(1920, 1080, "My first RAYLIB program!"); // Top left is (0,0)
    SetTargetFPS(60);
    // According to Google, we can use a render texture to draw pixel by pixel, then size it up
    // to a larger size afterwards so that we can 
    RenderTexture2D pixelatedScreen = LoadRenderTexture(SCREEN_HEIGHT, SCREEN_HEIGHT);
    while (WindowShouldClose() == false){
        
        BeginTextureMode(pixelatedScreen);
            ClearBackground(BLACK);
            DrawCircle(20,20,20, WHITE);
        EndTextureMode();
        BeginDrawing();
            ClearBackground(BLACK);
        DrawTexturePro(pixelatedScreen.texture,
                (Rectangle){ 0, 0, (float)pixelatedScreen.texture.width, (float)-pixelatedScreen.texture.height }, // -height to flip Y
                (Rectangle){ 0, 0, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT },
                (Vector2){ 0, 0 }, 0.0f, WHITE);
        EndDrawing();
    }

    CloseWindow();
}