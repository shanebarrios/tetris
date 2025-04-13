#pragma once

#include "Constants.h"
#include "Tetromino.h"

class Grid {
 public:
  Grid() = default;

  TetrominoType getTile(int i, int j) const { return m_grid[i][j]; }

  void clear();

  void placeTetromino(const Tetromino& t);
  bool checkObstruction(const Tetromino& t) const;
  int clearFullRows();

 private:
  TetrominoType m_grid[Constants::gridRows][Constants::gridCols]{};

  void shiftDown(int bottom);
};
