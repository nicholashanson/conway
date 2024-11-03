//#define VON_NEUMANN
#define MOORE

#include <memory>
#include <vector>
#include <array>
#include "settings.h"
#include <iostream>
#include <algorithm>
#include <numeric>
#include <random>

typedef enum {
    north,
    south,
    east,
    west,
    southeast,
    northeast,
    southwest,
    northwest
} direction_t;

typedef enum {
    dead = 0,
    live = 1
} status_t;

// scalar types
typedef int count_t;
typedef int cell_state;
typedef int dimension_t;
typedef int cell_index_t;

// iterators
typedef int *cell_state_p;

// aggregate types
typedef std::vector<direction_t> direction_vec_t;
typedef std::vector<cell_index_t> cell_index_vec_t;
typedef std::array<status_t, settings::board_width*settings::board_height> board_t;
typedef cell_index_vec_t seed_t;
typedef board_t state_t;

#ifdef MOORE
typedef std::array<status_t, 8> moore_t;
typedef moore_t neighborhood_t;
#endif
#ifdef VON_NEUMANN
typedef std::array<status_t, 4> von_neumann_t;
typedef von_neumann_t neighborhood_t;
#endif


