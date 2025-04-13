#include "Renderer.h"

#include <SDL.h>

#include <stdexcept>

#include "Texture.h"
#include "Window.h"

Renderer::Renderer(Window& window) {
  m_renderer = SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED);
  if (!m_renderer) {
    throw std::runtime_error{SDL_GetError()};
  }
}

Renderer::~Renderer() { SDL_DestroyRenderer(m_renderer); }

Renderer::Renderer(Renderer&& other) noexcept : m_renderer{other.m_renderer} {
  other.m_renderer = nullptr;
}

Renderer& Renderer::operator=(Renderer&& other) noexcept {
  if (&other == this) {
    return *this;
  }
  m_renderer = other.m_renderer;
  other.m_renderer = nullptr;
  return *this;
}

SDL_Renderer* Renderer::get() const { return m_renderer; }

Renderer& Renderer::present() {
  SDL_RenderPresent(m_renderer);
  return *this;
}

Renderer& Renderer::clear() {
  SDL_RenderClear(m_renderer);
  return *this;
}

Renderer& Renderer::setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
  SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
  return *this;
}

Renderer& Renderer::drawRect(const SDL_Rect& rect) {
  SDL_RenderDrawRect(m_renderer, &rect);
  return *this;
}

Renderer& Renderer::drawRect(int x, int y, int w, int h) {
  SDL_Rect rect{x, y, w, h};
  SDL_RenderDrawRect(m_renderer, &rect);
  return *this;
}

Renderer& Renderer::fillRect(const SDL_Rect& rect) {
  SDL_RenderFillRect(m_renderer, &rect);
  return *this;
}

Renderer& Renderer::fillRect(int x, int y, int w, int h) {
  SDL_Rect rect{x, y, w, h};
  SDL_RenderFillRect(m_renderer, &rect);
  return *this;
}

Renderer& Renderer::drawLine(int x1, int y1, int x2, int y2) {
  SDL_RenderDrawLine(m_renderer, x1, y1, x2, y2);
  return *this;
}

Renderer& Renderer::drawTexture(Texture& texture,
                                const std::optional<SDL_Rect>& src,
                                const std::optional<SDL_Rect>& dst) {
  SDL_RenderCopy(m_renderer, texture.get(), src ? &(*src) : nullptr,
                 dst ? &(*dst) : nullptr);
  return *this;
}