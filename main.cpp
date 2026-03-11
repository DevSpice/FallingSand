#include <iostream>
#include "./SourceFiles/Gamestate.cpp"
#include "./SourceFiles/Particles.cpp"
#include <raylib.h>

using namespace std;
using namespace fallingsandgame;

int main () {

    const int SCREEN_WIDTH = Width;
    const int SCREEN_HEIGHT = Height;
    const int SCALE_FACTOR = 4;
    
    unordered_map<Element, Color> colorsDict = {{Element::WATER, Color(BLUE)}, {Element::CONCRETE, Color(GRAY)},
                                                {Element::STEAM, Color(LIGHTGRAY)}, {Element::FIRE, Color(ORANGE)},
                                                {Element::SAND, Color(BEIGE)}, {Element::NONE, Color(BLACK)}};

    cout << "Hello World" << endl;

    auto displayWidth = Width*SCALE_FACTOR;
    auto displayHeight = Height*SCALE_FACTOR;
    InitWindow(displayWidth, displayHeight, "Falling Sands! A C++ Final Project by Houle & Klaes"); // Top left is (0,0)
    SetTargetFPS(60);
    // According to Google, we can use a render texture to draw pixel by pixel, then size it up
    // to a larger size afterwards so that we can 
    RenderTexture2D pixelatedScreen = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    // Set up user interaction variables;
    bool interacted = false;
    Coord interactPos;
    Element userElement = Element::WATER;
    GameState gs = GameState(8);
    while (WindowShouldClose() == false){
        interacted = false;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            interacted = true;
            auto interactVec = GetMousePosition();
            interactPos = Coord{int(interactVec.x), int(interactVec.y)};
        }

        // Calculate which element they are placing.
        if (IsKeyPressed(KEY_ONE)) {
            userElement = Element::NONE;
        }
        else if (IsKeyPressed(KEY_TWO)) {
            userElement = Element::CONCRETE;
        }
        else if (IsKeyPressed(KEY_THREE)) {
            userElement = Element::WATER;
        }
        else if (IsKeyPressed(KEY_FOUR)) {
            userElement = Element::STEAM;
        }
        else if (IsKeyPressed(KEY_FIVE)) {
            userElement = Element::FIRE;
        }
        else if (IsKeyPressed(KEY_SIX)) {
            userElement = Element::SAND;
        }

        if (interacted) {
            gs.ApplyUserInteraction(interactPos, 1.0f/SCALE_FACTOR, userElement);
            // cout << "Trying to interact!!!" << endl;
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
                (Rectangle){ 0, 0, (float)displayWidth, (float)displayHeight },
                (Vector2){ 0, 0 }, 0.0f, WHITE);
        EndDrawing();
    }

    CloseWindow();
}