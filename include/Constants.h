#pragma once

namespace Constants {
inline constexpr int gridRows{20};
inline constexpr int gridCols{10};

inline constexpr int windowWidth{800};
inline constexpr int windowHeight{800};

inline constexpr int gridMargin{16};
inline constexpr int gridHeight{(windowHeight - gridMargin * 2) / gridRows *
                                gridRows};
inline constexpr int gridCellWidth{gridHeight / gridRows};
inline constexpr int gridWidth{gridCellWidth * gridCols};

inline constexpr int gridStartX{gridMargin};
inline constexpr int gridStartY{gridMargin};
inline constexpr int gridEndX{gridStartX + gridWidth};
inline constexpr int gridEndY{gridStartY + gridHeight};
}  // namespace Constants