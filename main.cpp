#include <iostream>
#include "./SourceFiles/Gamestate.cpp"
#include "./SourceFiles/Particles.cpp"
#include <raylib.h>

using namespace std;
using namespace fallingsandgame;

static const int    TOOLBAR_HEIGHT  = 72;   // px in screen-space
static const int    BTN_COUNT       = 6;
static const int    BTN_PADDING     = 12;
static const int    BTN_SWATCH_SIZE = 20;   // colored square inside button
static const Color  TOOLBAR_BG      = { 18, 18, 22, 230 };   // near-black, slightly transparent
static const Color  BTN_IDLE        = { 38, 38, 46, 255 };
static const Color  BTN_HOVER       = { 58, 58, 70, 255 };
static const Color  BTN_SELECTED    = { 80, 80, 255, 255 };   // bright indigo highlight
static const Color  TEXT_DIM        = { 140, 140, 155, 255 };
static const Color  TEXT_BRIGHT     = { 230, 230, 240, 255 };


struct ElementInfo {
    Element   elem;
    const char* label;
    const char* key;
    Color     swatch;
};

static const ElementInfo ELEMENTS[BTN_COUNT] = {
    { Element::NONE,     "Erase",    "1", BLACK          },
    { Element::CONCRETE, "Concrete", "2", GRAY           },
    { Element::WATER,    "Water",    "3", BLUE           },
    { Element::STEAM,    "Steam",    "4", LIGHTGRAY      },
    { Element::FIRE,     "Fire",     "5", ORANGE         },
    { Element::SAND,     "Sand",     "6", BEIGE          },
};


//  DrawToolbar
//  Renders the bottom element-picker panel and
//  returns the Element the user clicked (or
//  current if no click occurred).
Element DrawToolbar(int screenW, int screenH, Element current) {
    Element chosen = current;

    // Panel background
    DrawRectangle(0, screenH - TOOLBAR_HEIGHT, screenW, TOOLBAR_HEIGHT, TOOLBAR_BG);
    // Thin top border line for separation
    DrawRectangle(0, screenH - TOOLBAR_HEIGHT, screenW, 1, { 70, 70, 90, 255 });

    // Calculate button width so they fill the bar evenly
    int totalPadding  = BTN_PADDING * (BTN_COUNT + 1);
    int btnWidth      = (screenW - totalPadding) / BTN_COUNT;
    int btnHeight     = TOOLBAR_HEIGHT - BTN_PADDING * 2;
    int btnY          = screenH - TOOLBAR_HEIGHT + BTN_PADDING;

    for (int i = 0; i < BTN_COUNT; i++) {
        int btnX = BTN_PADDING + i * (btnWidth + BTN_PADDING);
        Rectangle btnRect = { (float)btnX, (float)btnY, (float)btnWidth, (float)btnHeight };

        bool isSelected = (ELEMENTS[i].elem == current);
        bool isHovered  = CheckCollisionPointRec(GetMousePosition(), btnRect);

        // Button background
        Color bg = isSelected ? BTN_SELECTED : (isHovered ? BTN_HOVER : BTN_IDLE);
        DrawRectangleRounded(btnRect, 0.25f, 6, bg);

        // Selected: glowing outer ring
        if (isSelected) {
            DrawRectangleRoundedLines(
                { btnRect.x - 2, btnRect.y - 2, btnRect.width + 4, btnRect.height + 4 },
                0.3f, 6, { 130, 130, 255, 180 }
            );
        }

        // Color swatch (centered horizontally, upper portion)
        int swatchX = btnX + (btnWidth  - BTN_SWATCH_SIZE) / 2;
        int swatchY = btnY + 6;

        // Special "Erase" swatch: draw a small X instead of solid black
        if (ELEMENTS[i].elem == Element::NONE) {
            DrawRectangle(swatchX, swatchY, BTN_SWATCH_SIZE, BTN_SWATCH_SIZE, { 50, 50, 58, 255 });
            // Draw cross
            DrawLine(swatchX + 3, swatchY + 3,
                     swatchX + BTN_SWATCH_SIZE - 3, swatchY + BTN_SWATCH_SIZE - 3, WHITE);
            DrawLine(swatchX + BTN_SWATCH_SIZE - 3, swatchY + 3,
                     swatchX + 3, swatchY + BTN_SWATCH_SIZE - 3, WHITE);
        } else {
            DrawRectangle(swatchX, swatchY, BTN_SWATCH_SIZE, BTN_SWATCH_SIZE, ELEMENTS[i].swatch);
            DrawRectangleLines(swatchX, swatchY, BTN_SWATCH_SIZE, BTN_SWATCH_SIZE, { 0, 0, 0, 80 });
        }

        // Label
        Color labelColor = isSelected ? TEXT_BRIGHT : TEXT_DIM;
        int   fontSize   = 9;
        int   labelW     = MeasureText(ELEMENTS[i].label, fontSize);
        DrawText(ELEMENTS[i].label,
                 btnX + (btnWidth - labelW) / 2,
                 swatchY + BTN_SWATCH_SIZE + 4,
                 fontSize, labelColor);

        // Keyboard shortcut hint
        int keyFontSize = 8;
        int keyW        = MeasureText(ELEMENTS[i].key, keyFontSize);
        Color keyColor  = isSelected ? TEXT_BRIGHT : TEXT_DIM;
        DrawText(ELEMENTS[i].key,
                 btnX + (btnWidth - keyW) / 2,
                 swatchY + BTN_SWATCH_SIZE + 16,
                 keyFontSize, keyColor);

        // Click detection (only the game area passes mouse to game; toolbar captures it)
        if (isHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            chosen = ELEMENTS[i].elem;
        }
    }

    return chosen;
}

int main () {

    const int SCREEN_WIDTH = Width;
    const int SCREEN_HEIGHT = Height;
    const int SCALE_FACTOR = 4;
    
    unordered_map<Element, Color> colorsDict = {{Element::WATER, Color(BLUE)}, {Element::CONCRETE, Color(GRAY)},
                                                {Element::STEAM, Color(LIGHTGRAY)}, {Element::FIRE, Color(ORANGE)},
                                                {Element::SAND, Color(BEIGE)}, {Element::NONE, Color(BLACK)}};

    GameState game{};

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
    GameState gs = GameState();
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

                userElement = DrawToolbar(displayWidth, displayHeight, userElement);

        EndDrawing();
    }

    CloseWindow();
}