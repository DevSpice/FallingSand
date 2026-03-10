#include <iostream>
#include "./SourceFiles/Gamestate.cpp"
#include "./SourceFiles/Particles.cpp"
#include <raylib.h>

using namespace std;
using namespace fallingsandgame;

int main () {

    const int SCREEN_WIDTH = Width;
    const int SCREEN_HEIGHT = Height;
    
    unordered_map<Element, Color> colorsDict = {{Element::WATER, Color(BLUE)}, {Element::NONE, Color(BLACK)}};

    GameState game{};

    cout << "Hello World" << endl;

    auto displayWidth = 1920;
    auto displayHeight = 1080;
    InitWindow(displayWidth, displayHeight, "My first RAYLIB program!"); // Top left is (0,0)
    SetTargetFPS(60);
    // According to Google, we can use a render texture to draw pixel by pixel, then size it up
    // to a larger size afterwards so that we can 
    RenderTexture2D pixelatedScreen = LoadRenderTexture(SCREEN_HEIGHT, SCREEN_HEIGHT);
    // Set up user interaction variables;
    bool interacted = false;
    Coord interactPos;
    Element userElement = Element::WATER;
    GameState gs = GameState();
    while (WindowShouldClose() == false){
        interacted = false;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            interacted = true;
            auto interactVec = GetMousePosition();
            interactPos = Coord{int(interactVec.x), int(interactVec.y)};
        }

        if (interacted) {
            gs.ApplyUserInteraction(interactPos, (1.0f*SCREEN_WIDTH)/displayWidth, userElement);
        }

        gs.Tick();
        
        BeginTextureMode(pixelatedScreen);
            ClearBackground(BLACK);
            for (int x = 0; x < pixelatedScreen.texture.width; x++) {
                for (int y = 0; y < pixelatedScreen.texture.height; y++) {
                    auto elemToDraw = gs.GetPixelElement(x, y);
                    DrawPixel(x, y, colorsDict[elemToDraw]);
                }
            }
            //DrawCircle(20,20,20, WHITE);
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