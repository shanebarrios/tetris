
#include <string>

class Font;

namespace AssetManager {

Font& getFont(const std::string& font, int point);
Font& getDefaultFont();
}  // namespace AssetManager