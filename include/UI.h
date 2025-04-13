#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include "AssetManager.h"
#include "Renderer.h"
#include "Texture.h"

class UIComponent {
 public:
  enum Anchor {
    AnchorLeft,
    AnchorRight,
  };
  virtual ~UIComponent() = default;

  virtual void draw(Renderer& renderer);

  void setPosition(int x, int y);
  int getX() const;
  int getY() const;
  int getWidth() const;
  int getHeight() const;

 protected:
  explicit UIComponent(Anchor anchor);
  Texture m_texture{};
  SDL_Rect m_dstRect{};
  Anchor m_anchor{};

  int m_x{0};
  int m_y{0};

  bool m_shouldUpdate{true};

  virtual void update(Renderer& renderer) = 0;
  void updateAlignment();
};

class UILabel : public UIComponent {
 public:
  explicit UILabel(std::string_view text = "",
                   const Font& font = AssetManager::getDefaultFont(),
                   SDL_Color color = {0, 0, 0, 0xFF}, Anchor = AnchorLeft);

  void setText(std::string_view text);
  void setColor(SDL_Color color);
  void setFont(Font& font);

 private:
  std::reference_wrapper<const Font> m_font;
  std::string m_text{};
  SDL_Color m_color{};

  void update(Renderer& renderer) override;
};

class UISystem {
 public:
  explicit UISystem(Renderer& renderer);
  void add(std::shared_ptr<UIComponent> component);
  void remove(std::shared_ptr<UIComponent> component);
  void draw();

 private:
  std::vector<std::shared_ptr<UIComponent>> m_components{};
  Renderer& m_renderer;
};
