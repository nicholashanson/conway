namespace settings {
    const int board_width = 30;
    const int board_height = 30;
    constexpr int cell_count = board_width*board_height;
    constexpr int central_index = (cell_count / 2) % board_width == 0 ? cell_count / 2 + board_width / 2 : cell_count / 2;
    const int square_size = 20;
}