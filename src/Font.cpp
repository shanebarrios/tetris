#include "Font.h"

#include <SDL_ttf.h>

#include <stdexcept>

Font::Font(std::string_view path, int point) { load(path, point); }

Font::~Font() { TTF_CloseFont(m_font); }

Font::Font(Font&& other) noexcept : m_font{other.m_font} {
  other.m_font = nullptr;
}

Font& Font::operator=(Font&& other) noexcept {
  if (&other == this) {
    return *this;
  }
  m_font = other.m_font;
  other.m_font = nullptr;
  return *this;
}

TTF_Font* Font::get() const { return m_font; }

Font& Font::load(std::string_view path, int point) {
  TTF_CloseFont(m_font);
  m_font = TTF_OpenFont(path.data(), point);
  if (!m_font) {
    throw std::runtime_error(TTF_GetError());
  }
  return *this;
}