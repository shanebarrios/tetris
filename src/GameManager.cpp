#include "GameManager.h"

#include <algorithm>
#include <array>
#include <cmath>

constexpr double softDropSpeed{0.33};
constexpr int linesPerLevel{10};
constexpr int lockDelay{GameManager::tickRate / 2};

constexpr int getClearPoints(int lines, int level) {
  switch (lines) {
    case 1:
      return 100 * level;
    case 2:
      return 300 * level;
    case 3:
      return 500 * level;
    case 4:
      return 800 * level;
    default:
      return 0;
  }
}
void GameManager::newGame() {
  m_grid.clear();
  m_level = 1;
  m_score = 0;
  m_lines = 0;
  m_gameOver = false;
  m_softDropActive = false;
  m_lockTimerStart = 0;
  updateDropDelay();
  m_bag = {};
  pullTetromino();
}

void GameManager::tick() {
  if (m_gameOver) {
    return;
  }
  m_ticks++;
  if (!canMoveTetrominoDown()) {
    if (m_tetromino.getPosY() == 0) {
      m_gameOver = true;
      return;
    }
    if (m_ticks >= m_lockTimerStart + lockDelay) {
      placePiece();
    }
  }
  m_accumulator += (m_softDropActive ? softDropSpeed : m_gravity);
  while (m_accumulator >= 1) {
    moveTetrominoDown();
    m_accumulator -= 1;
  }
}

bool GameManager::moveTetrominoSide(bool left) {
  int deltaX{left ? -1 : 1};
  m_tetromino.translate(deltaX, 0);
  if (m_grid.checkObstruction(m_tetromino)) {
    m_tetromino.translate(-deltaX, 0);
    return false;
  }
  m_lockTimerStart = m_ticks;
  return true;
}

bool GameManager::moveTetrominoDown() {
  m_tetromino.translate(0, 1);
  if (m_grid.checkObstruction(m_tetromino)) {
    m_tetromino.translate(0, -1);
    return false;
  }
  if (m_softDropActive) {
    m_score += 1;
  }
  m_lockTimerStart = m_ticks;
  return true;
}

void GameManager::hardDrop() {
  while (moveTetrominoDown()) {
    m_score += 2;
  }
  placePiece();
}

bool GameManager::canMoveTetrominoDown() {
  m_tetromino.translate(0, 1);
  bool result{m_grid.checkObstruction(m_tetromino)};
  m_tetromino.translate(0, -1);
  return !result;
}

bool GameManager::rotateTetromino(bool clockwise) {
  static constexpr int kickTable[TetrominoType::NumTypes][4][2]{
      {{1, 0}, {1, -1}, {0, 2}, {1, 2}},      // 0 >> 3
      {{-1, 0}, {-1, -1}, {0, 2}, {-1, 2}},   // 0 >> 1
      {{1, 0}, {1, 1}, {0, -2}, {1, -2}},     // 1 >> 0
      {{1, 0}, {1, 1}, {0, -2}, {1, -2}},     // 1 >> 2
      {{-1, 0}, {-1, -1}, {0, 2}, {-1, 2}},   // 2 >> 1
      {{1, 0}, {1, -1}, {0, 2}, {1, 2}},      // 2 >> 3
      {{-1, 0}, {-1, 1}, {0, -2}, {-1, -2}},  // 3 >> 2
      {{-1, 0}, {-1, 1}, {0, -2}, {-1, -2}}   // 3 >> 0
  };

  static constexpr int kickTableI[TetrominoType::NumTypes][4][2]{
      {{-1, 0}, {2, 0}, {-1, -2}, {2, 1}},  // 0 >> 3
      {{-2, 0}, {1, 0}, {-2, 1}, {1, -2}},  // 0 >> 1
      {{2, 0}, {-1, 0}, {2, -1}, {-1, 2}},  // 1 >> 0
      {{-1, 0}, {2, 0}, {-1, -2}, {2, 1}},  // 1 >> 2
      {{1, 0}, {-2, 0}, {1, 2}, {-2, -1}},  // 2 >> 1
      {{2, 0}, {-1, 0}, {2, -1}, {-1, 2}},  // 2 >> 3
      {{-2, 0}, {1, 0}, {-2, 1}, {1, -2}},  // 3 >> 2
      {{1, 0}, {-2, 0}, {1, 2}, {-2, -1}}   // 3 >> 0
  };

  if (m_tetromino.getType() == TetrominoType::O) {
    return false;
  }

  const int startState{m_tetromino.getRotationState()};

  clockwise ? m_tetromino.rotateClockwise()
            : m_tetromino.rotateCounterClockwise();

  if (!m_grid.checkObstruction(m_tetromino)) {
    m_lockTimerStart = m_ticks;
    return true;
  }

  int kickRow{startState * 2 + (clockwise ? 1 : 0)};

  for (int i = 0; i < 4; i++) {
    int deltaX{kickTable[kickRow][i][0]};
    int deltaY{kickTable[kickRow][i][1]};
    if (m_tetromino.getType() == TetrominoType::I) {
      deltaX = kickTableI[kickRow][i][0];
      deltaY = kickTableI[kickRow][i][1];
    }

    m_tetromino.translate(deltaX, deltaY);
    if (!m_grid.checkObstruction(m_tetromino)) {
      m_lockTimerStart = m_ticks;
      return true;
    }
    m_tetromino.translate(-deltaX, -deltaY);
  }
  clockwise ? m_tetromino.rotateCounterClockwise()
            : m_tetromino.rotateClockwise();
  return false;
}

void GameManager::placePiece() {
  m_grid.placeTetromino(m_tetromino);
  int cleared{m_grid.clearFullRows()};
  m_score += getClearPoints(cleared, m_level);
  if ((m_lines + cleared) / linesPerLevel > m_lines / linesPerLevel) {
    m_level++;
    updateDropDelay();
  }
  m_lines += cleared;
  pullTetromino();
}

void GameManager::extendBag() {
  static std::mt19937 m_mt{std::random_device{}()};

  std::array<TetrominoType, NumTypes - 1> tetrominoes{};
  for (int i = TetrominoType::Empty + 1; i < TetrominoType::NumTypes; i++) {
    tetrominoes[i - 1] = static_cast<TetrominoType>(i);
  }
  std::shuffle(tetrominoes.begin(), tetrominoes.end(), m_mt);
  for (TetrominoType t : tetrominoes) {
    m_bag.push(t);
  }
}

void GameManager::pullTetromino() {
  if (m_bag.size() <= 3) {
    extendBag();
  }
  m_tetromino = Tetromino{m_bag.front()};
  m_bag.pop();
}

void GameManager::updateDropDelay() {
  m_gravity = 1 / (tickRate * pow(0.8 - ((m_level - 1) * 0.007), m_level - 1));
}
