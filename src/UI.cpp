#include "UI.h"

#include <SDL_ttf.h>

#include "Font.h"

UIComponent::UIComponent(Anchor anchor) : m_anchor{anchor} {}

void UIComponent::draw(Renderer& renderer) {
  if (m_shouldUpdate) {
    update(renderer);
    m_shouldUpdate = false;
  }
  renderer.drawTexture(m_texture, std::nullopt, m_dstRect);
}

void UIComponent::setPosition(int x, int y) {
  m_x = x;
  m_y = y;
  updateAlignment();
}

void UIComponent::updateAlignment() {
  switch (m_anchor) {
    case AnchorLeft:
      m_dstRect.x = m_x;
      m_dstRect.y = m_y;
      break;
    case AnchorRight:
      m_dstRect.x = m_x - m_dstRect.w;
      m_dstRect.y = m_y;
      break;
  }
}

int UIComponent::getX() const { return m_x; }
int UIComponent::getY() const { return m_y; }
int UIComponent::getWidth() const { return m_dstRect.w; }
int UIComponent::getHeight() const { return m_dstRect.h; }

UILabel::UILabel(std::string_view text, const Font& font, SDL_Color color,
                 Anchor anchor)
    : UIComponent{anchor}, m_font{font}, m_text{text}, m_color{color} {
  TTF_SizeText(m_font.get().get(), m_text.c_str(), &m_dstRect.w, &m_dstRect.h);
}

void UILabel::setText(std::string_view text) {
  if (m_text == text) {
    return;
  }
  m_text = text;
  m_shouldUpdate = true;
}

void UILabel::setColor(SDL_Color color) {
  if (m_color.r == color.r && m_color.g == color.g && m_color.b == color.b &&
      m_color.a == color.a) {
    return;
  }
  m_color = color;
  m_shouldUpdate = true;
}

void UILabel::setFont(Font& font) {
  m_font = font;
  m_shouldUpdate = true;
}

void UILabel::update(Renderer& renderer) {
  m_texture.loadText(renderer, m_font, m_text, m_color);
  m_dstRect.w = m_texture.getWidth();
  m_dstRect.h = m_texture.getHeight();
  updateAlignment();
}

UISystem::UISystem(Renderer& renderer) : m_renderer{renderer} {}

void UISystem::add(std::shared_ptr<UIComponent> component) {
  m_components.push_back(component);
}

void UISystem::remove(std::shared_ptr<UIComponent> component) {
  m_components.erase(
      std::remove(m_components.begin(), m_components.end(), component));
}

void UISystem::draw() {
  for (auto& component : m_components) {
    component->draw(m_renderer);
  }
}
