#pragma once

#include <string_view>
class TTF_Font;

class Font {
 public:
  Font() = default;
  Font(std::string_view path, int point);
  ~Font();

  Font(Font& other) = delete;
  Font& operator=(Font& other) = delete;
  Font(Font&& other) noexcept;
  Font& operator=(Font&& other) noexcept;

  TTF_Font* get() const;

  Font& load(std::string_view path, int point);

 private:
  TTF_Font* m_font{nullptr};
};
