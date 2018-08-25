#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "SDL.h"
#include "SDL_ttf.h"

static const int appWidth = 1920;
static const int appHeight = 1080;
static TTF_Font *font[2] = {0};

static void draw_text(SDL_Renderer* render, TTF_Font* ttf, const char* text, const int x, const int y)
{
    SDL_Color c = { 0, 0, 0, 255 };
    SDL_Surface *surface = NULL;
    if(!render || !ttf || !text)
        return;

    surface = TTF_RenderUTF8_Solid(ttf, text, c);
    if(surface) {
        SDL_Texture *texture = SDL_CreateTextureFromSurface(render, surface);
        if(texture) {
            SDL_Rect rt = { x, y, surface->w, surface->h };
            SDL_RenderCopy(render, texture, NULL, &rt);
            SDL_DestroyTexture(texture);
        }
        SDL_FreeSurface(surface);
    }
}

static void draw(SDL_Renderer* render, SDL_Event* pevt)
{
    char sz_x[8] = {0};
    char sz_y[8] = {0};

    if(!render || !pevt)
        return;

    draw_text(render, font[0], "Mouse", 200, 150);
    draw_text(render, font[1], "X", 200, 300);
    draw_text(render, font[1], "Y", 200, 400);

    draw_text(render, font[0], "Keyboard", 200, 550);
    draw_text(render, font[1], "Keycode", 200, 700);

    switch(pevt->type) {
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEMOTION:
            sprintf(sz_x, "%d", pevt->button.x);
            sprintf(sz_y, "%d", pevt->button.y);
            draw_text(render, font[1], ((pevt->type == SDL_MOUSEMOTION) ? "MOTION" : ((pevt->type == SDL_MOUSEBUTTONUP) ? "RELEASED"  : "PRESSED")), 750, 180);
            draw_text(render, font[1], sz_x, 500, 300);
            draw_text(render, font[1], sz_y, 500, 400);
            break;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            draw_text(render, font[1], ((pevt->type == SDL_KEYUP) ? "RELEASED" : "PRESSED"), 750, 580);
            draw_text(render, font[1], SDL_GetKeyName(pevt->key.keysym.sym), 500, 700);
            break;
    }
}


int main(int argc, char *argv[])
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Event event = {0};
    char* appid = NULL;

    int quit = 0;
    int foreground = 1;

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        goto cleanup;
    }

    window = SDL_CreateWindow( "Input sample", 0, 0, appWidth, appHeight, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL );
    if(window==NULL) {
        goto cleanup;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer==NULL) {
        goto cleanup;
    }

    if(TTF_Init() != 0) {
        goto cleanup;
    }

    font[0] = TTF_OpenFont("res/LinBiolinum_R.ttf", 90);
    font[1] = TTF_OpenFont("res/LinBiolinum_R.ttf", 60);
    if (!font[0] || !font[1]) {
        goto cleanup;
    }
    TTF_SetFontStyle(font[0], TTF_STYLE_BOLD);

    while(quit == 0) {
        while ( SDL_PollEvent(&event) ) {
            if(event.type == SDL_APP_DIDENTERFOREGROUND) {
                foreground = 1;
            } else if(event.type == SDL_APP_DIDENTERBACKGROUND) {
                foreground = 0;
            } else if (event.type == SDL_QUIT) {
                quit = 1;
                break;
            }

            /* render at only foreground */
            if(foreground == 1) {
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(renderer);
                draw(renderer, &event);
                SDL_RenderPresent(renderer);
            }

            /* When your apps goes background, your app should prevent CPU burn */
            /* If your apps use cpu too much then webOS system manger may kill your apps */
            else {
                SDL_Delay(30);
            }
        }
    }

cleanup:
    TTF_CloseFont(font[0]);
    TTF_CloseFont(font[1]);
    if(renderer != NULL)
        SDL_DestroyRenderer(renderer);
    if(window != NULL)
        SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();

    return 0;
}


