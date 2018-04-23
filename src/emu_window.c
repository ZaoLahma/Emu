#include "emu_window.h"
#include "emu_debug.h"
#include "emu.h"
#include <SDL2/SDL.h>

typedef struct
{
  SDL_Window* window;
  SDL_Renderer* renderer;
} EMUWIN_Ctxt;

static EMUWIN_Ctxt winCtxt;

static void handleSDLWindowEvent(SDL_Event* event);

static void handleSDLWindowEvent(SDL_Event* event)
{
  switch(event->window.event)
  {
    case SDL_WINDOWEVENT_CLOSE:
      EMU_abort();
      break;
    default:
      break;
  }
}

void EMUWIN_init()
{
  memset(&winCtxt, 0, sizeof(winCtxt));

  SDL_Init(SDL_INIT_VIDEO);

  winCtxt.window = SDL_CreateWindow(
                                      "SDL_GAME",
                                      SDL_WINDOWPOS_UNDEFINED,
                                      SDL_WINDOWPOS_UNDEFINED,
                                      640,
                                      480,
                                      SDL_WINDOW_SHOWN
                                   );

  EMU_DEBUG_ASSERT_COND(winCtxt.window);

  winCtxt.renderer = SDL_CreateRenderer(winCtxt.window, -1, SDL_RENDERER_ACCELERATED);

  EMU_DEBUG_ASSERT_COND(winCtxt.renderer);
}

void EMUWIN_run()
{
  SDL_Event event;
  while(SDL_PollEvent(&event) != 0)
  {
    switch(event.type)
    {
      case SDL_WINDOWEVENT:
        handleSDLWindowEvent(&event);
      break;
      default:
      break;
    }
  }
}
