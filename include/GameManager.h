#pragma once

#include <cstdint>
#include <queue>
#include <random>

#include "Grid.h"
#include "Tetromino.h"

typedef uint32_t Uint32;

class GameManager {
 public:
  static constexpr Uint32 tickRate{60};

  GameManager() = default;

  void newGame();
  void tick();

  const Grid& getGrid() const { return m_grid; }
  const Tetromino& getTetromino() const { return m_tetromino; }
  TetrominoType getNextTetromino() const { return m_bag.front(); }
  int getScore() const { return m_score; }
  int getLevel() const { return m_level; }
  int getLines() const { return m_lines; }
  bool isGameOver() const { return m_gameOver; }

  bool rotateTetromino(bool clockwise);
  bool moveTetrominoSide(bool left);
  bool moveTetrominoDown();

  void hardDrop();
  void startSoftDrop() { m_softDropActive = true; };
  void endSoftDrop() { m_softDropActive = false; };

 private:
  Grid m_grid{};
  Tetromino m_tetromino{};

  std::queue<TetrominoType> m_bag{};

  Uint32 m_ticks{};

  bool m_gameOver{};
  bool m_softDropActive{};

  int m_lines{};
  int m_level{};
  int m_score{};

  Uint32 m_lockTimerStart{};

  double m_gravity{};
  double m_accumulator{};

  bool canMoveTetrominoDown();
  void placePiece();
  void updateDropDelay();
  void extendBag();
  void pullTetromino();
};
