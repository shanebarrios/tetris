#include "TetrisRenderer.h"

#include <SDL_rect.h>

#include <optional>
#include <string>
#include <utility>

#include "AssetManager.h"
#include "Constants.h"
#include "Font.h"
#include "GameManager.h"
#include "Grid.h"
#include "Renderer.h"
#include "Tetromino.h"
#include "Texture.h"
#include "Window.h"

using namespace Constants;

constexpr SDL_Color fontColor{0xFF, 0xFF, 0xFF, 0xFF};

TetrisRenderer::TetrisRenderer(Window &window) : m_renderer{Renderer{window}} {
  initLabels();
}

void TetrisRenderer::initLabels() {
  Font &font{AssetManager::getFont("arcade", windowWidth / 32)};
  auto scoreLabel{std::make_shared<UILabel>("SCORE: ", font, fontColor)};
  scoreLabel->setPosition(gridEndX + gridMargin, gridStartY + gridCellWidth);

  auto levelLabel{std::make_shared<UILabel>("LEVEL: ", font, fontColor)};
  levelLabel->setPosition(
      scoreLabel->getX(),
      scoreLabel->getY() + scoreLabel->getHeight() + gridCellWidth);

  m_nextLabel = std::make_shared<UILabel>("NEXT: ", font, fontColor);
  m_nextLabel->setPosition(
      scoreLabel->getX(),
      levelLabel->getY() + levelLabel->getHeight() + gridCellWidth);

  m_levelValue =
      std::make_shared<UILabel>("1", font, fontColor, UIComponent::AnchorRight);
  m_levelValue->setPosition(windowWidth - gridMargin, levelLabel->getY());

  m_scoreValue =
      std::make_shared<UILabel>("0", font, fontColor, UILabel::AnchorRight);
  m_scoreValue->setPosition(windowWidth - gridMargin, scoreLabel->getY());

  m_ui.add(scoreLabel);
  m_ui.add(levelLabel);
  m_ui.add(m_nextLabel);
  m_ui.add(m_scoreValue);
  m_ui.add(m_levelValue);
}

void TetrisRenderer::fillCell(TetrominoType type, int x, int y) {
  static constexpr Uint8 tetrominoColors[TetrominoType::NumTypes - 1][3] = {
      {0, 255, 255}, {0, 0, 255},   {255, 165, 0}, {255, 255, 0},
      {0, 255, 0},   {128, 0, 128}, {255, 0, 0}};

  SDL_Rect rect{x, y, gridCellWidth, gridCellWidth};
  m_renderer.setDrawColor(tetrominoColors[type - 1][0],
                          tetrominoColors[type - 1][1],
                          tetrominoColors[type - 1][2]);
  m_renderer.fillRect(rect);
  m_renderer.setDrawColor(0, 0, 0);
  m_renderer.drawRect(rect);
}

void TetrisRenderer::fillGridCell(TetrominoType t, int row, int col) {
  int x{gridStartX + col * gridCellWidth};
  int y{gridStartY + row * gridCellWidth};
  fillCell(t, x, y);
}

void TetrisRenderer::drawGrid(const GameManager &gm) {
  m_renderer.setDrawColor(0, 0, 0);
  m_renderer.fillRect(gridStartX, gridStartY, gridWidth, gridHeight);
  m_renderer.setDrawColor(0x44, 0x44, 0x44);
  for (int x = gridStartX; x <= gridEndX; x += gridCellWidth) {
    m_renderer.drawLine(x, gridStartY, x, gridEndY);
  }
  for (int y = gridStartY; y <= gridEndY; y += gridCellWidth) {
    m_renderer.drawLine(gridStartX, y, gridEndX, y);
  }

  const Grid &grid{gm.getGrid()};
  for (int i = 0; i < gridRows; i++) {
    for (int j = 0; j < gridCols; j++) {
      TetrominoType type{grid.getTile(i, j)};
      if (type) {
        fillGridCell(type, i, j);
      }
    }
  }
}

void TetrisRenderer::drawTetromino(const GameManager &gm) {
  const Tetromino &tetromino{gm.getTetromino()};

  for (int i = 0; i < Tetromino::boxWidth; i++) {
    for (int j = 0; j < Tetromino::boxWidth; j++) {
      int tile{tetromino.getTile(i, j)};
      if (tile) {
        fillGridCell(tetromino.getType(), i + tetromino.getPosY(),
                     j + tetromino.getPosX());
      }
    }
  }
}

void TetrisRenderer::drawText(const GameManager &gm) {
  m_levelValue->setText(std::to_string(gm.getLevel()));
  m_scoreValue->setText(std::to_string(gm.getScore()));
  m_ui.draw();
}

void TetrisRenderer::drawNext(const GameManager &gm) {
  Tetromino tetromino{gm.getNextTetromino()};
  for (int i = 0; i < Tetromino::boxWidth; i++) {
    for (int j = 0; j < Tetromino::boxWidth; j++) {
      if (tetromino.getTile(i, j)) {
        fillCell(tetromino.getType(),
                 m_nextLabel->getX() + gridMargin + j * gridCellWidth,
                 m_nextLabel->getY() + m_nextLabel->getHeight() +
                     2 * gridMargin + i * gridCellWidth);
      }
    }
  }
}

void TetrisRenderer::render(const GameManager &gm) {
  m_renderer.setDrawColor(0x20, 0x20, 0x20);
  m_renderer.clear();
  drawGrid(gm);
  drawTetromino(gm);
  drawText(gm);
  drawNext(gm);
  m_renderer.present();
}
