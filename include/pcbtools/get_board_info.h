#ifndef GET_BOARD_INFO_H
#define GET_BOARD_INFO_H
#include "board.h"
#include <filesystem>
namespace pcb_tools {
board get_board_info(const std::filesystem::path &gerber_file);
}
#endif// GET_BOARD_INFO_H
