#ifndef SIM_H
#define SIM_H
#include <cstdarg>
#include <algorithm>
#include "definitions.h"

class Sim
{
    public:
        Sim();
        virtual ~Sim();

        // getters
        auto get_state() const -> state_t;
        auto get_central_cell() const -> cell_index_t;
        auto get_neighbor(const cell_index_t, const direction_t) const -> cell_index_t;
        auto get_cardinal_neighbors(const cell_index_t) -> cell_index_vec_t;
        auto get_intercardinal_neighbors(const cell_index_t) -> cell_index_vec_t;
        auto get_neighbors(const cell_index_t,const direction_vec_t&) -> cell_index_vec_t;
        auto get_neighbors(const cell_index_t,const count_t, ...) const -> cell_index_vec_t;
        #ifdef MOORE
        auto get_neighborhood(const cell_index_t,neighborhood_t&) -> void;
        auto get_top_left_corner_neighborhood(neighborhood_t&) -> void;
        auto get_top_right_corner_neighborhood(neighborhood_t&) -> void;
        auto get_bottom_right_corner_neighborhood(neighborhood_t&) -> void;
        auto get_bottom_left_corner_neighborhood(neighborhood_t&) -> void;
        auto get_top_edge_neighborhood(const cell_index_t, neighborhood_t&) -> void;
        auto get_right_edge_neighborhood(const cell_index_t, neighborhood_t&) -> void;
        auto get_bottom_edge_neighborhood(const cell_index_t, neighborhood_t&) -> void;
        auto get_left_edge_neighborhood(const cell_index_t, neighborhood_t&) -> void;
        #endif
        #ifdef VON_NEUMANN
        auto get_neighborhood(const cell_index_t,neighborhood_t&) -> void;
        auto get_top_left_corner_neighborhood(von_neumann_t&) -> void;
        auto get_top_right_corner_neighborhood(von_neumann_t&) -> void;
        auto get_bottom_right_corner_neighborhood(von_neumann_t&) -> void;
        auto get_bottom_left_corner_neighborhood(von_neumann_t&) -> void;
        auto get_top_edge_neighborhood(const cell_index_t, von_neumann_t&) -> void;
        auto get_right_edge_neighborhood(const cell_index_t, von_neumann_t&) -> void;
        auto get_bottom_edge_neighborhood(const cell_index_t, von_neumann_t&) -> void;
        auto get_left_edge_neighborhood(const cell_index_t, neighborhood_t&) -> void;
        #endif

        // stats
        auto total_cells() const -> count_t;
        auto total_live_cells() const -> count_t;
        auto live_neighbor_count(const neighborhood_t&) const -> count_t;

        // checkers
        auto board_is_dead() const -> bool;
        auto cell_is_live(const cell_index_t) const -> bool;
        auto cell_is_dead(const cell_index_t) const -> bool;
        auto cells_are_live(const cell_index_vec_t&) const -> bool;
        auto cells_are_live(const count_t, ...) const -> bool;
        auto cells_are_live(const cell_index_t, const cell_index_vec_t&) const -> bool;
        auto cells_are_dead(const count_t, ...) const -> bool;
        auto is_edge_cell(const cell_index_t) const -> bool;

        // actions
        auto seed(const count_t, ...) -> void;
        auto seed(const cell_index_t,const cell_index_vec_t&) -> void;
        auto seed(const cell_index_vec_t&) -> void;
        auto generate_seed() -> void;
        auto evolve() -> void;

    protected:

    private:
        auto __get_neighborhood(const cell_index_t, neighborhood_t&) -> void;

        state_t current_state;
};

#endif // SIM_H
