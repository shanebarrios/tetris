#include "Grid.h"

void Grid::clear()
{
  for (int i = 0; i < Constants::gridRows; i++)
  {
    for (int j = 0; j < Constants::gridCols; j++)
    {
      m_grid[i][j] = TetrominoType::Empty;
    }
  }
}

void Grid::placeTetromino(const Tetromino &t)
{
  for (int i = 0; i < Tetromino::boxWidth; i++)
  {
    for (int j = 0; j < Tetromino::boxWidth; j++)
    {
      int occupied = t.getTile(i, j);
      if (occupied)
      {
        m_grid[t.getPosY() + i][t.getPosX() + j] = t.getType();
      }
    }
  }
}

bool Grid::checkObstruction(const Tetromino &t) const
{
  for (int i = 0; i < Tetromino::boxWidth; i++)
  {
    for (int j = 0; j < Tetromino::boxWidth; j++)
    {
      int occupied = t.getTile(i, j);
      if (!occupied)
      {
        continue;
      }
      int tileX = t.getPosX() + j;
      int tileY = t.getPosY() + i;
      if (tileX >= Constants::gridCols || tileX < 0)
      {
        return true;
      }
      if (tileY >= Constants::gridRows || tileY < 0)
      {
        return true;
      }
      if (m_grid[tileY][tileX] != TetrominoType::Empty)
      {
        return true;
      }
    }
  }
  return false;
}

int Grid::clearFullRows()
{
  int count{0};
  for (int i = Constants::gridRows - 1; i >= 0;)
  {
    bool fullRow{true};
    for (int j = 0; j < Constants::gridCols; j++)
    {
      if (!m_grid[i][j])
      {
        fullRow = false;
        break;
      }
    }
    if (fullRow)
    {
      shiftDown(i);
      count++;
    }
    else
    {
      i--;
    }
  }
  return count;
}

void Grid::shiftDown(int bottom)
{
  for (int i = bottom; i > 0; i--)
  {
    for (int j = 0; j <= Constants::gridCols; j++)
    {
      m_grid[i][j] = m_grid[i - 1][j];
    }
  }
}
