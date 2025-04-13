#include "Texture.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <stdexcept>
#include <utility>

#include "Font.h"
#include "Renderer.h"

Texture::Texture(Renderer& renderer, std::string_view path) {
  loadImage(renderer, path);
}

Texture::Texture(Renderer& renderer, Font& font, std::string_view text,
                 SDL_Color color) {
  loadText(renderer, font, text, color);
}

Texture::~Texture() { SDL_DestroyTexture(m_texture); }
Texture::Texture(Texture&& other) noexcept
    : m_texture{other.m_texture},
      m_width{other.m_width},
      m_height{other.m_height} {
  other.m_texture = nullptr;
}

Texture& Texture::operator=(Texture&& other) noexcept {
  if (&other == this) {
    return *this;
  }
  m_texture = other.m_texture;
  m_width = other.m_width;
  m_height = other.m_height;
  other.m_texture = nullptr;
  return *this;
}

SDL_Texture* Texture::get() const { return m_texture; }

int Texture::getWidth() const { return m_width; }
int Texture::getHeight() const { return m_height; }

Texture& Texture::loadFromSurface(Renderer& renderer, SDL_Surface* surface) {
  SDL_DestroyTexture(m_texture);
  if (!surface) {
    throw std::runtime_error{IMG_GetError()};
  }
  m_texture = SDL_CreateTextureFromSurface(renderer.get(), surface);
  m_width = surface->w;
  m_height = surface->h;
  SDL_FreeSurface(surface);
  if (!m_texture) {
    throw std::runtime_error{SDL_GetError()};
  }
  return *this;
}

Texture& Texture::loadImage(Renderer& renderer, std::string_view path) {
  return loadFromSurface(renderer, IMG_Load(path.data()));
}

Texture& Texture::loadText(Renderer& renderer, const Font& font,
                           std::string_view text, SDL_Color color) {
  return loadFromSurface(renderer,
                         TTF_RenderText_Solid(font.get(), text.data(), color));
}