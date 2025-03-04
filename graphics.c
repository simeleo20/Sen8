#include <stdio.h>
#include "core.h"
#include "graphics.h"
#include "customLog.h"
#include "os.h"
#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
EM_JS(int, getInnerWidth, (), {
    return window.innerWidth;
});
EM_JS(int, getInnerHeight, (), {
    return window.innerHeight;
});
#endif


int screenWidth = 256;
int screenHeight = 240;
extern core cCore;
extern const rgb palette[];

float newScreenWidth;
float newScreenHeight;

u8 previousScreen[240][256];

RenderTexture2D renderTexture;

void putPixel(u8 x, u8 y, u8 color)
{
    DrawPixel(x, y, (Color){palette[color].r, palette[color].g, palette[color].b, 255});
}
void drawScreen()
{
    for(int y = 0; y < 240; y++)
    {
        for(int x = 0; x < 256; x++)
        {
            if(cCore.ram.screen[y][x] != previousScreen[y][x])
            {
                putPixel(x, y, cCore.ram.screen[y][x]);
            }
            previousScreen[y][x] = cCore.ram.screen[y][x];

        }
    }
}
void setupPreviousScreen()
{
    for(int y = 0; y < 240; y++)
    {
        for(int x = 0; x < 256; x++)
        {
            previousScreen[y][x] = -1;
        }
    }
}

Vector2 calcMousePosition()
{
    Vector2 mousePosition = GetMousePosition();
    Vector2 result;
    result.x = (mousePosition.x - ((float)GetScreenWidth() - newScreenWidth)/2) * screenWidth / newScreenWidth;
    result.y = (mousePosition.y - ((float)GetScreenHeight()- newScreenHeight)/2) * screenHeight / newScreenHeight;
    return result;
}
bool btn(u8 btncode)
{
    switch (btncode)
    {
    case BUTTON_UP:
        return IsKeyDown(KEY_UP); 
        break;
    case BUTTON_DOWN:
        return IsKeyDown(KEY_DOWN); 
        break;
    case BUTTON_LEFT:
        return IsKeyDown(KEY_LEFT); 
        break;
    case BUTTON_RIGHT:
        return IsKeyDown(KEY_RIGHT); 
        break;
    case BUTTON_A:
        return IsKeyDown(KEY_Z); 
        break;
    case BUTTON_B:
        return IsKeyDown(KEY_X); 
        break;
    case BUTTON_MOUSEL:
        return IsMouseButtonDown(MOUSE_LEFT_BUTTON); 
        break;
    case BUTTON_MOUSER:
        return IsMouseButtonDown(MOUSE_RIGHT_BUTTON); 
        break;
    default:
        break;
    }
    return false;
}
int getKeyPressed()
{
    return GetKeyPressed();
}
int getCharPressed()
{
    return GetCharPressed();
}

void UpdateDrawFrame(void)
{
    #if defined(PLATFORM_WEB)
    SetWindowSize(GetRenderWidth(), GetRenderHeight());
    #endif
    osLoop();
    corePPUDraw();

    // Instead of using BeginDrawing() we render to the render texture. Everything else stays unchanged
    BeginTextureMode(renderTexture);
    //ClearBackground(RAYWHITE);
    drawScreen();
    
    // We need to end the texture mode separately
    EndTextureMode();

    //calc the new screen size without deformation
    float tryheight = (float)GetScreenWidth() * screenHeight / screenWidth;
    float trywidth = (float)GetScreenHeight() * screenWidth / screenHeight;
    if(tryheight <= GetScreenHeight()){
        newScreenWidth = GetScreenWidth();
        newScreenHeight = tryheight;
    }else{
        newScreenWidth = trywidth;
        newScreenHeight = GetScreenHeight();
    }
    
    // Let's draw the texture. The source rect is the size of the texture, the destination rect is of the same size as the screen. For some reason, the texture was flipped vertically, so I had to invert the source rects "height" to flip the UV.
    BeginDrawing();
    
    DrawTexturePro(
        renderTexture.texture,
        (Rectangle){ 0, 0, (float)renderTexture.texture.width, (float)-renderTexture.texture.height },
        (Rectangle){ ((float)GetScreenWidth() - newScreenWidth)/2, ((float)GetScreenHeight()- newScreenHeight)/2, newScreenWidth, newScreenHeight },
        (Vector2){ 0, 0 },
        0,
        WHITE);
    EndDrawing();
    

    coreLoop();
    
    // VBLANK
    coreVBLANK();
    

    //printf("Width: %d, Height: %d\n", GetScreenWidth(), GetScreenHeight());
    //Vector2 mousePosition = calcMousePosition();
    //printf("Mouse Position: [%f, %f]\n", mousePosition.x, mousePosition.y);
}

int main(void)
{
    
    resetScreen();
    SetTraceLogCallback(CustomLog);
    
    #if defined(PLATFORM_WEB)
        InitWindow(getInnerWidth(),getInnerHeight(), "Fantasy Console");
    #else
        InitWindow(256, 240, "Fantasy Console");
    #endif
    // This should use the flag FLAG_FULLSCREEN_MODE which results in a possible ToggleFullscreen() call later on
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetExitKey(KEY_NULL);
    // Request a texture to render to. The size is the screen size of the raylib example.
    renderTexture = LoadRenderTexture(screenWidth, screenHeight);
    coreSetup();
    setupPreviousScreen();
    #if defined(PLATFORM_WEB)
    ToggleBorderlessWindowed();
    printf("Hello, world!\n");
    EM_ASM(
            // Make a directory other than '/'
            FS.mkdir('/Sen8');
            // Then mount with IDBFS type
            FS.mount(IDBFS, {
                autoPersist: true
            }, '/Sen8');
                
            FS.syncfs(true, function (err) {
                if (err) {
                    console.error('Error during initial sync:', err);
                } else {
                    console.log('Initial sync complete');
                }
            });
            
        );
        //ChangeDirectory("/Sen8");
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    
    {
        UpdateDrawFrame();
    }
#endif

    // Unload the texture handle again to make a clean exit.
    UnloadRenderTexture(renderTexture);


    CloseWindow();

    return 0;
}


