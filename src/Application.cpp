#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <iostream>
#include <stdexcept>

#include "Constants.h"
#include "GameManager.h"
#include "Renderer.h"
#include "TetrisRenderer.h"
#include "Window.h"

void init() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    throw std::runtime_error{SDL_GetError()};
  }
  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    throw std::runtime_error{IMG_GetError()};
  }
  if (TTF_Init() < 0) {
    throw std::runtime_error{TTF_GetError()};
  }
}

void close() {
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}

bool handleEvent(const SDL_Event& e, GameManager& gm) {
  if (e.type == SDL_QUIT) {
    return false;
  }
  if (!gm.isGameOver() && e.type == SDL_KEYDOWN) {
    switch (e.key.keysym.sym) {
      case SDLK_UP:
        if (!e.key.repeat) {
          gm.rotateTetromino(true);
        }
        break;
      case SDLK_DOWN:
        gm.startSoftDrop();
        break;
      case SDLK_LEFT:
        gm.moveTetrominoSide(true);
        break;
      case SDLK_RIGHT:
        gm.moveTetrominoSide(false);
        break;
      case SDLK_SPACE:
        gm.hardDrop();
    }
  }
  if (e.type == SDL_KEYUP) {
    switch (e.key.keysym.sym) {
      case SDLK_DOWN:
        gm.endSoftDrop();
    }
  }
  return true;
}

int main(int argc [[maybe_unused]], char** argv [[maybe_unused]]) {
  init();

  Window window{"Tetris", Constants::windowWidth, Constants::windowHeight};
  window.show();
  TetrisRenderer renderer{window};

  GameManager gm{};
  gm.newGame();
  constexpr Uint32 tickDelayMs{1000 / GameManager::tickRate};

  bool running{true};
  Uint32 previous{SDL_GetTicks()};
  SDL_Event e;
  while (running) {
    while (SDL_PollEvent(&e)) {
      running = handleEvent(e, gm);
    }
    Uint32 current{SDL_GetTicks()};
    while (current - previous >= tickDelayMs) {
      gm.tick();
      previous += tickDelayMs;
    }
    renderer.render(gm);
  }

  close();
  return 0;
}