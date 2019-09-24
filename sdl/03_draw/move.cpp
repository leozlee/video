#include <SDL2/SDL.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[])
{
    SDL_Window *pSDLWindow = NULL;
    SDL_Renderer *pSDLRender = NULL;
    SDL_Event SDLEvent;
    SDL_Texture *pSDLTexture = NULL;
    SDL_Rect SDLRect;
    int quit = 0;
    
    SDL_Init(SDL_INIT_VIDEO);

    //創建窗口
    pSDLWindow = SDL_CreateWindow("sdl window", 0, 0, 640, 480 ,SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
    if(!pSDLWindow)
    {
        printf("SDL_CreateWindow error!\n");
        goto end;
    }

    //創建渲染器
    pSDLRender = SDL_CreateRenderer(pSDLWindow, -1, 0);
    if(!pSDLRender)
    {
        printf("SDL_CreateRenderer error!\n");
        goto end;
    }

    /* 创建纹理 */
    pSDLTexture = SDL_CreateTexture(pSDLRender, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 640, 480);
    if(!pSDLTexture)
    {
        printf("SDL_CreateTexture error!\n");
        goto end;
    }

    /* 方块 */
    SDLRect.w = 30;
    SDLRect.h = 30;

    SDL_SetRenderDrawColor(pSDLRender, 0, 255, 0, 255);

    SDL_RenderClear(pSDLRender);

    SDL_RenderPresent(pSDLRender);

    while(!quit)
    {
        SDL_WaitEvent(&SDLEvent);

        switch(SDLEvent.type)
        {
            case SDL_QUIT:
                printf("quit!\n");
                quit = 1;
                break;

            case SDL_MOUSEMOTION:
                printf("mouse move!\n");
                break;
            
            default:

                break;
        }

        SDLRect.x = rand() % 640;
        SDLRect.y = rand() % 480;

        SDL_SetRenderTarget(pSDLRender, pSDLTexture); /* 设置render的操作对象纹理 */
        SDL_SetRenderDrawColor(pSDLRender, 0, 0, 0, 0); /* 设置纹理的颜色 */
        SDL_RenderClear(pSDLRender); /* 将画面更新到纹理上 */

        SDL_RenderDrawRect(pSDLRender, &SDLRect); /* 使用render来绘制方块 */
        SDL_SetRenderDrawColor(pSDLRender, 255, 0, 0, 0); /* 绘制方块的颜色 */
        SDL_RenderFillRect(pSDLRender, &SDLRect); /* 将方块刷到纹理上 */

        SDL_SetRenderTarget(pSDLRender, NULL); /* 改变render的操作对象为window */
        SDL_RenderCopy(pSDLRender, pSDLTexture, NULL, NULL);

        SDL_RenderPresent(pSDLRender); /* 显示 */
    }

end:

    if(pSDLRender)
        SDL_DestroyRenderer(pSDLRender);

    if(pSDLWindow)
        SDL_DestroyWindow(pSDLWindow);

    SDL_Quit();

    return 0;
}