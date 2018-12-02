#include <stdio.h>
#include <string.h>
#include "SDL.h"

#define LOG_ERR(var, ...) \
    do{ \
        char str[260]; \
        snprintf(str, 260,"[ERROR]%s:%d -- "var, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
        OutputDebugStringA(str); \
    } while (0);

#define LOG_INFO(var, ...) \
    do{ \
        char str[260]; \
        snprintf(str, 260,"[INFO]%s:%d -- "var, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
        OutputDebugStringA(str); \
    } while (0);

typedef struct PlayContext_s
{
    int quit;
    SDL_Renderer * renderer;
    SDL_Texture * texture;
    SDL_mutex * mutex;
    unsigned int yuvfmt;
    unsigned int yuv_w;
    unsigned int yuv_h;
    unsigned int yuv_fps;
    char filepath[256];
}PlayContext_t;

static int getfilesize(const char * filepath)
{
    int length = 0;
    FILE * pfile = NULL;

    if (!filepath || strlen(filepath) == 0)
    {
        return -1;
    }
    pfile = fopen(filepath, "rb");
    if (!pfile)
    {
        return -2;
    }
    fseek(pfile, 0, SEEK_END);
    length = ftell(pfile);
    fclose(pfile);
    return length;
}

static int detect_yuv_wh(const char * filepath, int *w, int *h)
{
    int filelen = getfilesize(filepath);
    if (filelen <= 0)
    {
        return -1;
    }
    if (!w || !h)
    {
        return -1;
    }

    if (filelen % (1920 * 1080 * 3 / 2) == 0)
    {
        *w = 1920;
        *h = 1080;
    }
    else if (filelen % (1280 * 720 * 3 / 2) == 0)
    {
        *w = 1280;
        *h = 720;
    }
    else if (filelen % (704 * 576 * 3 / 2) == 0)
    {
        *w = 706;
        *h = 574;
    }
    else if (filelen % (352 * 288 * 3 / 2) == 0)
    {
        *w = 352;
        *h = 288;
    }
    else {
        *w = 352;
        *h = 288;
    }
    return 0;
}

static int YUV_PlayThread(void * arg)
{
    PlayContext_t * PlayCtx = (PlayContext_t *)arg;
    FILE * yuvfile = NULL;
    if (PlayCtx == NULL)
    {
        return -1;
    }
    LOG_INFO("YUVPlayThread start successfully.\n");
    LOG_INFO("####################\n");
    LOG_INFO("# filename : %s\n", PlayCtx->filepath);
    LOG_INFO("# width    : %d\n", PlayCtx->yuv_w);
    LOG_INFO("# height   : %d\n", PlayCtx->yuv_h);
    LOG_INFO("# fps      : %d\n", PlayCtx->yuv_fps);
    LOG_INFO("####################\n\n");
    yuvfile = fopen(PlayCtx->filepath, "rb");
    if (yuvfile == NULL)
    {
        LOG_ERR("open %s failed\n", PlayCtx->filepath);
    }
    unsigned int framesize = PlayCtx->yuv_w*PlayCtx->yuv_h * 3 / 2;
    unsigned char * framebuf = (unsigned char *)malloc(framesize);
    int ret = 0;
    int pitch = PlayCtx->yuv_w;
    if (framebuf == NULL)
    {
        fclose(yuvfile);
    }
    PlayCtx->texture = SDL_CreateTexture(PlayCtx->renderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, PlayCtx->yuv_w, PlayCtx->yuv_h);
    if (PlayCtx->texture == NULL)
    {
        LOG_ERR("SDL_CreateTexture %s\n", SDL_GetError());
    }
    while (1)
    {
        unsigned int cnt;
        if (PlayCtx->quit)
        {
            break;
        }
        if ((cnt = fread(framebuf, 1, framesize, yuvfile)) < framesize)
        {
            ret = fseek(yuvfile, 0, SEEK_SET);
        }
        // 		ret = SDL_LockTexture(PlayCtx->texture, NULL, &framebuf, pitch);
        // 		SDL_UnlockTexture(PlayCtx->texture);
        SDL_LockMutex(PlayCtx->mutex);
        ret = SDL_UpdateTexture(PlayCtx->texture, NULL, framebuf, pitch);
        if (ret)
        {
            LOG_ERR("SDL_UpdateTexture %s\n", SDL_GetError());
            
        }
        ret = SDL_RenderCopy(PlayCtx->renderer, PlayCtx->texture, NULL, NULL);
        if (ret)
        {
            LOG_ERR("SDL_RenderCopy %s\n", SDL_GetError());
        }
        SDL_RenderPresent(PlayCtx->renderer);
        SDL_UnlockMutex(PlayCtx->mutex);
        SDL_Delay(40);
    }
    if (PlayCtx->texture)
    {
        SDL_DestroyTexture(PlayCtx->texture);
        PlayCtx->texture = NULL;
    }
    free(framebuf);
    framebuf = NULL;
    fclose(yuvfile);

    printf("YUVPlayThread exit successfully.\n");
    return 0;
}

int main(int argc, char * argv[])
{
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret)
    {
        printf("SDL_init Failed!\n");
    }
    SDL_Thread * thread = NULL;
    SDL_Window * window = NULL;
    SDL_RendererInfo rendererinfo;
    PlayContext_t PlayCtx = { 0 };

    window = SDL_CreateWindow("SDL YUV Player", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 352, 288, SDL_WINDOW_RESIZABLE);
    if (window == NULL)
    {
        goto Quit;
    }
    SDL_Event event;
    int done = 0;

    int rendercnt = SDL_GetNumRenderDrivers();
    for (int i = 0; i < rendercnt; i++)
    {
        SDL_GetRenderDriverInfo(i, &rendererinfo);
        LOG_INFO("####################\n");
        LOG_INFO("render index:\t%d\n", i);
        LOG_INFO("render name:\t%s\n", rendererinfo.name);
        LOG_INFO("render flags:\t%d\n", rendererinfo.flags);
        LOG_INFO("####################\n");
    }

    PlayCtx.renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (PlayCtx.renderer == NULL)
    {
        LOG_ERR("SDL_CreateRenderer %s\n", SDL_GetError());
    }

    PlayCtx.mutex = SDL_CreateMutex();
    //SDL_RenderDrawRect(PlayCtx.renderer, NULL);
    SDL_RenderClear(PlayCtx.renderer);
    SDL_RenderPresent(PlayCtx.renderer);

    while (!done)
    {
        if (SDL_WaitEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                done = 1;
                break;
            case SDL_WINDOWEVENT:
                switch (event.window.event)
                {
                case SDL_WINDOWEVENT_RESIZED:
                    SDL_LockMutex(PlayCtx.mutex);
                    SDL_DestroyTexture(PlayCtx.texture);
                    SDL_DestroyRenderer(PlayCtx.renderer);
                    PlayCtx.renderer = SDL_CreateRenderer(window, -1, 0);
                    if (PlayCtx.texture)
                    {
                        PlayCtx.texture = SDL_CreateTexture(PlayCtx.renderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, PlayCtx.yuv_w, PlayCtx.yuv_h);
                        if (PlayCtx.texture == NULL)
                        {
                            LOG_ERR("SDL_CreateTexture %s\n", SDL_GetError());
                        }
                    }

					//SDL_RenderDrawRect(PlayCtx.renderer, NULL);
					SDL_RenderClear(PlayCtx.renderer);
					SDL_RenderPresent(PlayCtx.renderer);
                    SDL_UnlockMutex(PlayCtx.mutex);
                    LOG_INFO("11111111111111\n");
                    break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                //printf("capture mouse down event\n");
                break;
            case SDL_DROPFILE:
                strcpy(PlayCtx.filepath, event.drop.file);
                if (detect_yuv_wh(PlayCtx.filepath, &PlayCtx.yuv_w, &PlayCtx.yuv_h) < 0)
                {
                    LOG_ERR("detect_yuv_wh failed!\n");
                    PlayCtx.yuv_w = 352;
                    PlayCtx.yuv_h = 288;
                }
                PlayCtx.yuv_fps = 25;
                thread = SDL_CreateThread(YUV_PlayThread, "YUVPlayThread", (void *)&PlayCtx);
                if (thread == NULL)
                {
                    LOG_ERR("Create YUVPlayThread failed.\n");
                }
                SDL_free(event.drop.file);
                //SDL_EventState(SDL_DROPFILE, SDL_DISABLE);
                break;
            }
        }
        SDL_Delay(0);
    }
    if (thread)
    {
        PlayCtx.quit = 1;
        SDL_WaitThread(thread, NULL);
        thread = NULL;
    }
Quit:
    if (PlayCtx.mutex)
    {
        SDL_DestroyMutex(PlayCtx.mutex);
    }
    if (PlayCtx.renderer)
    {
        SDL_DestroyRenderer(PlayCtx.renderer);
    }
    if (window)
    {
        SDL_DestroyWindow(window);
        window = NULL;
    }
    SDL_Quit();

    return 0;
}