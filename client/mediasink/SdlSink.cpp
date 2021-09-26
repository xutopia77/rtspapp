
#include "SdlSink.h"
#include "ffmpegParse.h"

#define EVENT_FREASH (SDL_USEREVENT + 1)
// bool QUITFREASH = false;

using namespace std;

SdlSink::SdlSink()
{
}

SdlSink::~SdlSink()
{
}

/**
 * sdl的接收frame应该由pause，delay_time控制，由这些参数决定是否要去刷新
*/
void SdlSink::acceptFrame(std::shared_ptr<XAvframe>& frame){
    sinkFrame = frame;
    SDL_Event event;
    event.type = EVENT_FREASH;
    SDL_PushEvent(&event);
}

bool SdlSink::start(shared_ptr<MediaCtrl> ctrl){
    mctrl = ctrl;
    SDL_Thread *threadRun = nullptr;
    int rst = 0;
    rst = SDL_Init(SDL_INIT_VIDEO);
    if (rst != 0){
        LOG_ERR("rst = %d\n", rst);
        return -1;
    }
    threadRun = SDL_CreateThread(sdlSinkRunThread, "sdlRun", this);
    return true;
}

static bool createWindow(int screen_w, int screen_h, SDL_Window** win, SDL_Renderer** ren){
    SDL_Renderer *render = nullptr;
    SDL_Window *window = nullptr;
    window = SDL_CreateWindow("YUV player",
                              30, 30,
                                screen_w, screen_h,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (window == nullptr){
        LOG_ERR("create window failed\n");
        return false;
    }    

    render = SDL_CreateRenderer(window, -1, 0);
    if (render == nullptr){
        LOG_ERR("create render failed\n");
        return false;
    }
    SDL_SetRenderDrawColor(render, 255, 0, 100, 200);

    *win = window;
    *ren = render;
    return true;
}

/*c -, x=, z +, space 暂停， ESC 退出
    
*/
void SdlSink::sinkRun(){

    LOG_INFO("sink sdl start\n");
    AppCfg *cfg = gpAppCfg;
    SDL_Event event;
    SDL_Rect rec;
    size_t len = 0;
    rec.x = 10;
    rec.y = 10;
    // rec.w = cfg->screen_w - 20;
    // rec.h = cfg->screen_h - 20;
    char showText[1024];
    SDL_Renderer *render = nullptr;
    SDL_Window *window = nullptr;

    while (true){
        SDL_WaitEvent(&event);

        switch (event.type)
        {
        case EVENT_FREASH:{
            if( sinkFrame != nullptr){
    
                if(window == nullptr){
                    screen_w = sinkFrame->width;
                    screen_h = sinkFrame->height;
                    bool bre = createWindow(screen_w, screen_h, &window, &render);
                    if(!bre){
                        fatal("create window false\n");
                    }
                }

                SDL_Texture* texture;
                rec.w = sinkFrame->width;
                rec.h = sinkFrame->height;
                texture = SDL_CreateTexture(render, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_TARGET, rec.w, rec.h);
                SDL_RenderClear(render);
                SDL_UpdateTexture(texture, nullptr, sinkFrame->frame, rec.w);
                SDL_RenderCopy(render, texture, nullptr, &rec);
                sprintf(showText,"mode:%s",cfg->sourceType==1?"file":"rtsp");
                SDLTest_DrawString(render, 10, 10, showText);
                sprintf(showText,"pix fmt:%s,%dx%d",sinkFrame->pix_fmt.c_str(),sinkFrame->width, sinkFrame->height);
                SDLTest_DrawString(render, 10, 18, showText);
                SDL_RenderPresent(render);
                SDL_DestroyTexture(texture);
                // LOG_DBG("freash\n");
            }else{
                SDL_Log("not find value frame\n");
            }
        }
        break;
        case SDL_QUIT:{
            SDL_Log("recive a SDL_QUIT event\n");
            goto EXIT;
        }
        break;
        case SDL_WINDOWEVENT:{
            SDL_GetWindowSize(window, &screen_w, &screen_h);
            rec.w = screen_w - 20;
            rec.h = screen_h - 20;
            // SDL_Log("recive a SDL_WINDOWEVENT event %d\n", event.window.event);
            if(event.window.event == SDL_WINDOWEVENT_RESIZED){
                // SDL_Log("type : SDL_WINDOWEVENT_RESIZED\n");
            }
            // SDL_Log("\n");
        }
        break;
        case SDL_KEYUP:{
            if (event.key.keysym.sym == SDLK_SPACE)
            {
                PAUSE = PAUSE ? false : true;
            }
            else if (event.key.keysym.sym == SDLK_ESCAPE){
                SDL_Log("ESC is pressed , it will exit \n");
                goto EXIT;
            }
            else if (event.key.keysym.sym == SDLK_z)
            {
                delayTime += step;
                delayTime = delayTime > 80 ? 80 : delayTime;
                SDL_Log("z is pressed , delayTime = %u \n", delayTime);
            }
            else if (event.key.keysym.sym == SDLK_x)
            {
                delayTime = 40;
            }
            else if (event.key.keysym.sym == SDLK_c)
            {
                delayTime -= step;
                delayTime = delayTime > 0 ? delayTime : step;
                SDL_Log("z is pressed , delayTime = %u \n", delayTime);
            }
        }
        break;
        default:
            break;
        }
    }
EXIT:
    if (nullptr != render)
        SDL_DestroyRenderer(render);
    if (nullptr != window)
        SDL_DestroyWindow(window);
    
    SDL_Quit();
}


