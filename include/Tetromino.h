#pragma once

#include <iostream>

namespace TetrominoData {};
enum TetrominoType { Empty = 0, I, J, L, O, S, T, Z, NumTypes };

class Tetromino {
 public:
  static constexpr int boxWidth{4};

  explicit Tetromino(TetrominoType type = TetrominoType::Empty);

  int getTile(int row, int col) const { return m_box[row][col]; }
  TetrominoType getType() const { return m_type; }
  int getPosX() const { return m_posX; }
  int getPosY() const { return m_posY; }
  int getRotationState() const { return m_rotationState; }

  void translate(int deltaX, int deltaY);

  void rotateClockwise();
  void rotateCounterClockwise();

  friend std::ostream& operator<<(std::ostream& out, const Tetromino& t);

 private:
  static constexpr int tetrominoes[TetrominoType::NumTypes][boxWidth][boxWidth]{
      {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // Empty
      {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // I
      {{1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // J
      {{0, 0, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // L
      {{0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // O
      {{0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // S
      {{0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // T
      {{1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}   // Z

  };

  TetrominoType m_type;
  int m_box[boxWidth][boxWidth];
  int m_posX;
  int m_posY;
  int m_rotationState;
};
