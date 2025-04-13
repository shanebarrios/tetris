#pragma once

#include <SDL_rect.h>

#include <cstdint>
#include <optional>

class Window;
class Texture;
struct SDL_Renderer;
typedef uint8_t Uint8;

class Renderer {
 public:
  Renderer() = default;
  explicit Renderer(Window& window);
  ~Renderer();

  Renderer(Renderer& other) = delete;
  Renderer& operator=(Renderer& other) = delete;
  Renderer(Renderer&& other) noexcept;
  Renderer& operator=(Renderer&& other) noexcept;

  SDL_Renderer* get() const;

  Renderer& present();
  Renderer& clear();
  Renderer& setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 0xFF);
  Renderer& drawRect(const SDL_Rect& rect);
  Renderer& drawRect(int x, int y, int w, int h);
  Renderer& fillRect(const SDL_Rect& rect);
  Renderer& fillRect(int x, int y, int w, int h);
  Renderer& drawLine(int x1, int y1, int x2, int y2);
  Renderer& drawTexture(Texture& texture,
                        const std::optional<SDL_Rect>& src = std::nullopt,
                        const std::optional<SDL_Rect>& dst = std::nullopt);

 private:
  SDL_Renderer* m_renderer{nullptr};
};
