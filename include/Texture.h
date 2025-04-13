#pragma once

#include <SDL_pixels.h>

#include <string_view>

class Renderer;
class Font;
struct SDL_Texture;
struct SDL_Surface;

class Texture {
 public:
  Texture() = default;
  Texture(Renderer& renderer, std::string_view path);
  Texture(Renderer& renderer, Font& font, std::string_view text,
          SDL_Color color = SDL_Color{0, 0, 0, 0xFF});
  ~Texture();

  Texture(Texture& other) = delete;
  Texture& operator=(Texture& other) = delete;
  Texture(Texture&& other) noexcept;
  Texture& operator=(Texture&& other) noexcept;

  SDL_Texture* get() const;
  int getWidth() const;
  int getHeight() const;

  Texture& loadFromSurface(Renderer& renderer, SDL_Surface* surface);
  Texture& loadImage(Renderer& renderer, std::string_view path);
  Texture& loadText(Renderer& renderer, const Font& font, std::string_view text,
                    SDL_Color color = SDL_Color{0, 0, 0, 0xFF});

 private:
  SDL_Texture* m_texture{nullptr};
  int m_width{};
  int m_height{};
};
