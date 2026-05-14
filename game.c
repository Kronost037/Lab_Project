#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

#define CHECK(x) do { if(!x) goto ERROR; } while(0)

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

typedef SDL_AppResult GameStatus;

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *playerTexture;
    SDL_Texture *skyTexture;
} MyGame;

GameStatus SDL_AppInit(void **appstate, int argc, char **argv) {
    (void) argc;
    (void) argv;
    
    MyGame *game = (MyGame *)SDL_calloc(1, sizeof(MyGame));
    CHECK(game);
    
    bool display = SDL_CreateWindowAndRenderer("Hello World", SCREEN_WIDTH, SCREEN_HEIGHT, 0, &game->window, &game->renderer);
    CHECK(display);

    game->playerTexture = IMG_LoadTexture(game->renderer, "assets/player.png");
    CHECK(game->playerTexture);

    game->skyTexture = IMG_LoadTexture(game->renderer, "assets/Clouds/Clouds 3/1.png");
    CHECK(game->skyTexture);

    *appstate = game;
    return SDL_APP_CONTINUE;
    
 ERROR:
    SDL_Log("ERROR: APPInit FAILED: %s", SDL_GetError());
    
    if(game) {
        SDL_free(game);
    }
    
    return SDL_APP_FAILURE;
}

GameStatus SDL_AppEvent(void *appstate, SDL_Event *event) {
    MyGame *game = (MyGame *)appstate;
    (void)game;
    
    switch(event->type) {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
    case SDL_EVENT_KEY_DOWN: {
        switch(event->key.key) {
        case SDLK_ESCAPE:
            return SDL_APP_SUCCESS;
        }
    }
    }
    
    return SDL_APP_CONTINUE;
}

GameStatus SDL_AppIterate(void *appstate) {
    MyGame *game = (MyGame *) appstate;
    
    SDL_SetRenderDrawColor(game->renderer, 250, 250, 250, 255);
    SDL_RenderClear(game->renderer);

    int w, h;
    SDL_GetRenderOutputSize(game->renderer, &w, &h);
    SDL_FRect dst_rect = { 0.0f, 0.0f, (float)w, (float)h };
    
    SDL_RenderTexture(game->renderer, game->skyTexture, NULL, &dst_rect);
    
    SDL_FRect playerRect = {100.0f, 100.0f, 50.0f, 50.0f};
    SDL_RenderTexture(game->renderer, game->playerTexture, NULL, &playerRect);
    
    SDL_RenderPresent(game->renderer);
    
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    if(appstate) {
        MyGame *game = (MyGame *) appstate;

        SDL_DestroyTexture(game->skyTexture);
        SDL_DestroyTexture(game->playerTexture);
        SDL_DestroyRenderer(game->renderer);
        SDL_DestroyWindow(game->window);
        
        SDL_free(game);
    }
    
    (void) result;
    SDL_Quit();
}

