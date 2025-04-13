#include "Tetromino.h"

Tetromino::Tetromino(TetrominoType type)
    : m_type{type}, m_posX{3}, m_posY{0}, m_rotationState{0} {
  std::copy(&tetrominoes[type][0][0],
            &tetrominoes[type][0][0] + boxWidth * boxWidth, &m_box[0][0]);
}

void Tetromino::translate(int deltaX, int deltaY) {
  m_posX += deltaX;
  m_posY += deltaY;
}

void Tetromino::rotateClockwise() {
  if (m_type == TetrominoType::O) {
    return;
  }

  int width{m_type == TetrominoType::I ? 4 : 3};

  for (int i = 0; i < width; i++) {
    for (int j = i; j < width; j++) {
      std::swap(m_box[i][j], m_box[j][i]);
    }
  }

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < width / 2; j++) {
      std::swap(m_box[i][j], m_box[i][width - j - 1]);
    }
  }

  m_rotationState = (m_rotationState + 1) % 4;
}

void Tetromino::rotateCounterClockwise() {
  if (m_type == TetrominoType::O) {
    return;
  }
  int width{m_type == TetrominoType::I ? 4 : 3};

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < width / 2; j++) {
      std::swap(m_box[i][j], m_box[i][width - j - 1]);
    }
  }

  for (int j = 0; j < width; j++) {
    for (int i = j + 1; i < width; i++) {
      std::swap(m_box[i][j], m_box[j][i]);
    }
  }

  m_rotationState = (m_rotationState + 3) % 4;
}

std::ostream& operator<<(std::ostream& out, const Tetromino& t) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      out << t.m_box[i][j] << " ";
    }
    out << "\n";
  }
  return out;
}