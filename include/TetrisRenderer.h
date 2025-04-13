#pragma once

#include "Font.h"
#include "Renderer.h"
#include "Tetromino.h"
#include "Texture.h"
#include "UI.h"

class Window;
class GameManager;
class TetrisRenderer {
 public:
  explicit TetrisRenderer(Window& window);

  void render(const GameManager& gm);

 private:
  Renderer m_renderer{};
  UISystem m_ui{m_renderer};

  std::shared_ptr<UILabel> m_levelValue{};
  std::shared_ptr<UILabel> m_scoreValue{};
  std::shared_ptr<UILabel> m_nextLabel{};

  void initLabels();

  void fillCell(TetrominoType t, int x, int y);
  void fillGridCell(TetrominoType t, int row, int col);
  void drawGrid(const GameManager& gm);
  void drawTetromino(const GameManager& gm);
  void drawText(const GameManager& gm);
  void drawNext(const GameManager& gm);
};