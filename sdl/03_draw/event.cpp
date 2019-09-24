#include <SDL2/SDL.h>
#include <stdio.h>
#include <unistd.h>



int main(int argc, char *argv[])
{
    SDL_Window *pSDLWindow   = NULL;
    SDL_Renderer *pSDLRender = NULL;
    SDL_Event sdlEvent;
    int quit = 0;
    
    SDL_Init(SDL_INIT_VIDEO); /* 初始化视频系统 */

    /* 创建一个起点(0,0)，大小640x480的可视窗口 */
    pSDLWindow = SDL_CreateWindow("sdl window", 0, 0, 640, 480 ,SDL_WINDOW_SHOWN);
    if(!pSDLWindow)
    {
        printf("SDL_CreateWindow error!\n");
        goto end;
    }

    /* 创建一个渲染器（操作窗口的上下文） */
    pSDLRender = SDL_CreateRenderer(pSDLWindow, -1, 0);
    if(!pSDLRender)
    {
        printf("SDL_CreateRenderer error!\n");
        goto end;
    }

    /* 设置用于绘制窗口的颜色 */
    SDL_SetRenderDrawColor(pSDLRender, 0, 255, 0, 255);

    /* 用当前pSDLRender的颜色去清楚窗口 */
    SDL_RenderClear(pSDLRender);

    /* 更新窗口 */
    SDL_RenderPresent(pSDLRender);

    while(!quit)
    {
        /* 等到事件 */
        SDL_WaitEvent(&sdlEvent);

        switch(sdlEvent.type)
        {
            case SDL_QUIT: /* 退出事件 */
                printf("quit!\n");
                quit = 1;
                break;

            case SDL_MOUSEMOTION: /* 鼠标移动事件 */
                printf("mouse move!\n");
                break;
            
            default:

                break;
        }
    }

end:

    if(pSDLRender)
        SDL_DestroyRenderer(pSDLRender);

    if(pSDLWindow)
        SDL_DestroyWindow(pSDLWindow);

    SDL_Quit();

    return 0;
}