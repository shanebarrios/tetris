#include "AssetManager.h"

#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>

#include "Font.h"

namespace {
std::unordered_map<std::string, Font> fonts;
std::unordered_map<std::string, std::string> fontPaths{
    {"arcade", "assets/PressStart2P-Regular.ttf"},
    {"arial", "assets/Arial.ttf"}};
}  // namespace

namespace AssetManager {

Font& getFont(const std::string& font, int point) {
  std::string key{font};
  std::transform(key.begin(), key.end(), key.begin(),
                 [](char c) { return std::tolower(c); });
  auto pathIt = fontPaths.find(key);
  if (pathIt == fontPaths.end()) {
    throw std::runtime_error{"Failed to find font"};
  }
  key += std::to_string(point);
  auto [it, _] = fonts.try_emplace(key, pathIt->second, point);
  return it->second;
}
Font& getDefaultFont() { return getFont("arial", 24); }
}  // namespace AssetManager