#include "Sim.h"

// special members
Sim::Sim(){std::fill(current_state.begin(), current_state.end(), dead);}
Sim::~Sim(){}

// getters
auto Sim::get_state() const -> state_t
{return current_state;}

auto Sim::get_central_cell() const -> cell_index_t
{return settings::central_index;}

auto Sim::get_neighbor(const cell_index_t cell, const direction_t neigbor) const -> cell_index_t
{
    switch (neigbor)
    {
        case north:
            return cell - settings::board_width;
            break;
        case south:
            return cell + settings::board_height;
            break;
        case east:
            return cell + 1;
            break;
        case west:
            return cell - 1;
            break;
        case northeast:
            return cell - settings::board_width + 1;
            break;
        case northwest:
            return cell - settings::board_width - 1;
            break;
        case southeast:
            return cell + settings::board_width + 1;
            break;
        case southwest:
            return cell + settings::board_width - 1;
            break;
        default: break;
    }
}

auto Sim::get_neighbors(const cell_index_t cell, const direction_vec_t& neighbors) -> cell_index_vec_t
{
    cell_index_vec_t cells;
    cells.reserve( neighbors.size() );
    for ( auto it = neighbors.begin(); it != neighbors.end(); ++it )
        cells.push_back( get_neighbor( cell, *it ) );
    return cells;
}

auto Sim::get_cardinal_neighbors(const cell_index_t cell) -> cell_index_vec_t
{
    const direction_vec_t cardinals{north, south, east, west};
    return get_neighbors(cell, cardinals);
}

auto Sim::get_intercardinal_neighbors(const cell_index_t cell) -> cell_index_vec_t
{
    const direction_vec_t intercardinals{northeast, northwest, southeast, southwest};
    return get_neighbors(cell, intercardinals);
}

auto Sim::get_neighborhood(const cell_index_t i, neighborhood_t &neighbors) -> void
{
    if (i == 0)
        get_top_left_corner_neighborhood(neighbors);
    else if (i == settings::board_width - 1)
        get_top_right_corner_neighborhood(neighbors);
    else if (i == settings::cell_count - settings::board_width)
        get_bottom_left_corner_neighborhood(neighbors);
    else if (i == settings::cell_count - 1)
        get_bottom_right_corner_neighborhood(neighbors);
    else if (i > 0 && i < settings::board_width - 1)
        get_top_edge_neighborhood(i, neighbors);
    else if (i % (settings::board_width) == 0)
        get_left_edge_neighborhood(i, neighbors);
    else if ((i+1) % (settings::board_width) == 0)
        get_right_edge_neighborhood(i, neighbors);
    else if (i > settings::cell_count - settings::board_width)
        get_bottom_edge_neighborhood(i, neighbors);
    else
        __get_neighborhood(i, neighbors);
}

#ifdef MOORE
auto Sim::get_top_left_corner_neighborhood(moore_t &neighbors) -> void
{
    std::fill(neighbors.begin(), neighbors.end() - 3, dead);
    neighbors[5] = current_state[1];
    neighbors[6] = current_state[settings::board_width];
    neighbors[7] = current_state[settings::board_width + 1];
}

auto Sim::get_top_right_corner_neighborhood(moore_t &neighbors) -> void
{
    const cell_index_t target_cell = settings::board_width - 1;
    std::fill(neighbors.begin(), neighbors.end() - 3, dead);
    neighbors[5] = current_state[target_cell - 1];
    neighbors[6] = current_state[target_cell + settings::board_width - 1];
    neighbors[7] = current_state[target_cell + settings::board_width];
}

auto Sim::get_bottom_left_corner_neighborhood(moore_t &neighbors) -> void
{
    const cell_index_t target_cell = settings::cell_count - settings::board_width;
    std::fill(neighbors.begin(), neighbors.end() - 3, dead);
    neighbors[5] = current_state[target_cell - settings::board_width];
    neighbors[6] = current_state[target_cell - settings::board_width + 1];
    neighbors[7] = current_state[target_cell + 1];
}

auto Sim::get_bottom_right_corner_neighborhood(moore_t &neighbors) -> void
{
    const cell_index_t target_cell = settings::cell_count - 1;
    std::fill(neighbors.begin(), neighbors.end() - 3, dead);
    neighbors[5] = current_state[target_cell - settings::board_width];
    neighbors[6] = current_state[target_cell - settings::board_width - 1];
    neighbors[7] = current_state[target_cell - 1];
}

auto Sim::get_top_edge_neighborhood(const cell_index_t i, moore_t &neighbors) -> void
{
    std::fill(neighbors.begin(), neighbors.end() - 5, dead);
    neighbors[3] = current_state[i - 1];
    neighbors[4] = current_state[i + 1];
    neighbors[5] = current_state[i + settings::board_width - 1];
    neighbors[6] = current_state[i + settings::board_width];
    neighbors[7] = current_state[i + settings::board_width + 1];
}

auto Sim::get_right_edge_neighborhood(const cell_index_t i, moore_t &neighbors) -> void
{
    std::fill(neighbors.begin(), neighbors.end() - 5, dead);
    neighbors[3] = current_state[i - settings::board_width - 1];
    neighbors[4] = current_state[i - settings::board_width];
    neighbors[5] = current_state[i - 1];
    neighbors[6] = current_state[i + settings::board_width - 1];
    neighbors[7] = current_state[i + settings::board_width];
}

auto Sim::get_bottom_edge_neighborhood(const cell_index_t i, moore_t &neighbors) -> void
{
    std::fill(neighbors.begin(), neighbors.end() - 5, dead);
    neighbors[3] = current_state[i - settings::board_width - 1];
    neighbors[4] = current_state[i - settings::board_width];
    neighbors[5] = current_state[i - settings::board_width + 1];
    neighbors[6] = current_state[i - 1];
    neighbors[7] = current_state[i + 1];
}

auto Sim::get_left_edge_neighborhood(const cell_index_t i, moore_t &neighbors) -> void
{
    std::fill(neighbors.begin(), neighbors.end() - 5, dead);
    neighbors[3] = current_state[i - settings::board_width];
    neighbors[4] = current_state[i - settings::board_width + 1];
    neighbors[5] = current_state[i + 1];
    neighbors[6] = current_state[i + settings::board_width];
    neighbors[7] = current_state[i + settings::board_width + 1];
}

auto Sim::__get_neighborhood(const cell_index_t i, moore_t &neighbors) -> void
{
    neighbors[0] = current_state[i - settings::board_width - 1];
    neighbors[1] = current_state[i - settings::board_width];
    neighbors[2] = current_state[i - settings::board_width + 1];
    neighbors[3] = current_state[i - 1];
    neighbors[4] = current_state[i + 1];
    neighbors[5] = current_state[i + settings::board_width - 1];
    neighbors[6] = current_state[i + settings::board_width];
    neighbors[7] = current_state[i + settings::board_width + 1];
}
#endif

#ifdef VON_NEUMANN
auto Sim::get_top_left_corner_neighborhood(von_neumann_t &neighbors) -> void
{
    neighbors[0] = current_state[1];
    neighbors[1] = current_state[settings::board_width];
    neighbors[2] = dead;
    neighbors[3] = dead;
}

auto Sim::get_top_right_corner_neighborhood(von_neumann_t &neighbors) -> void
{
    const cell_index_t target_cell = settings::board_width - 1;
    neighbors[0] = current_state[target_cell - 1];
    neighbors[1] = current_state[target_cell + settings::board_width];
    neighbors[2] = dead;
    neighbors[3] = dead;
}

auto Sim::get_bottom_left_corner_neighborhood(von_neumann_t &neighbors) -> void
{
    const cell_index_t target_cell = settings::cell_count - settings::board_width;
    neighbors[0] = current_state[target_cell - settings::board_width];
    neighbors[1] = current_state[target_cell + 1];
    neighbors[2] = dead;
    neighbors[3] = dead;
}

auto Sim::get_bottom_right_corner_neighborhood(von_neumann_t &neighbors) -> void
{
    const cell_index_t target_cell = settings::cell_count - 1;
    neighbors[0] = current_state[target_cell - settings::board_width];
    neighbors[1] = current_state[target_cell - 1];
    neighbors[2] = dead;
    neighbors[3] = dead;
}

auto Sim::get_top_edge_neighborhood(const cell_index_t i, von_neumann_t &neighbors) -> void
{
    neighbors[0] = current_state[i + settings::board_width];
    neighbors[1] = current_state[i - 1];
    neighbors[2] = current_state[i + 1];
    neighbors[3] = dead;
}

auto Sim::get_right_edge_neighborhood(const cell_index_t i, von_neumann_t &neighbors) -> void
{
    neighbors[0] = current_state[i - 1];
    neighbors[1] = current_state[i - settings::board_width];
    neighbors[2] = current_state[i + settings::board_width];
    neighbors[3] = dead;
}

auto Sim::get_bottom_edge_neighborhood(const cell_index_t i, von_neumann_t &neighbors) -> void
{
    neighbors[0] = current_state[i - settings::board_width];
    neighbors[1] = current_state[i - 1];
    neighbors[2] = current_state[i + 1];
    neighbors[3] = dead;
}

auto Sim::get_left_edge_neighborhood(const cell_index_t i, von_neumann_t &neighbors) -> void
{
    neighbors[0] = current_state[i - settings::board_width];
    neighbors[1] = current_state[i + 1];
    neighbors[2] = current_state[i + settings::board_width];
    neighbors[4] = dead;
}

auto Sim::__get_neighborhood(const cell_index_t i, von_neumann_t &neighbors) -> void
{
    neighbors[0] = current_state[i - settings::board_width];
    neighbors[1] = current_state[i - 1];
    neighbors[2] = current_state[i + 1];
    neighbors[3] = current_state[i + settings::board_width];
}
#endif

// stats
auto Sim::total_cells() const -> count_t
{return current_state.size();}

auto Sim::total_live_cells() const -> count_t
{
    return std::count_if(current_state.begin(), current_state.end(),
        []( const status_t& status ){
            return live == status;
    });
}

auto Sim::live_neighbor_count(const neighborhood_t &neighbors) const -> count_t
{
    return std::count_if(neighbors.begin(), neighbors.end(),
        []( const status_t& status ){
            return live == status;
    });
}

// checkers
auto Sim::board_is_dead() const -> bool
{
    return std::none_of(current_state.begin(), current_state.end(),
        []( const status_t& status ){
            return live == status;
    });
}

auto Sim::cell_is_live(const cell_index_t cell) const -> bool
{return live == current_state[cell];}

auto Sim::cell_is_dead(const cell_index_t cell) const -> bool
{return dead == current_state[cell];}

auto Sim::cells_are_live(const cell_index_vec_t& cells) const -> bool
{
    return std::none_of(cells.cbegin(), cells.cend(),
        [this](const cell_index_t& index){
            return dead == current_state[index];
    });
}

auto Sim::cells_are_live(const count_t cells_to_check, ...) const -> bool
{
    va_list cells;
    va_start( cells, cells_to_check );
    for ( int i = 0; i < cells_to_check; i++ )
        if ( dead == current_state[ va_arg( cells, cell_index_t ) ] )
            return false;
    return true;
}

auto Sim::cells_are_live(const cell_index_t cell, const cell_index_vec_t& cells) const -> bool
{
    if ( dead == current_state[cell] )
        return false;
    return cells_are_live( cells );
}

auto Sim::cells_are_dead(const count_t cells_to_check, ...) const -> bool
{
    va_list cells;
    va_start( cells, cells_to_check );
    for ( int i = 0; i < cells_to_check; i++ )
        if ( live == current_state[ va_arg( cells, cell_index_t ) ] )
            return false;
    return true;
}

auto Sim::is_edge_cell(const cell_index_t i) const -> bool
{
    if ( i == 0 )
        return true;
    else if ( i == settings::board_width - 1 )
        return true;
    else if ( i == settings::cell_count - settings::board_width )
        return true;
    else if ( i == settings::cell_count - 1 )
        return true;
    else if ( i > 0 && i < settings::board_width - 1 )
        return true;
    else if ( i % ( settings::board_width) == 0 )
        return true;
    else if ( ( i+1 ) % ( settings::board_width ) == 0 )
        return true;
    else if ( i > settings::cell_count - settings::board_width )
        return true;
    else
        return false;
}

// actions
auto Sim::seed(const count_t cells_to_seed, ...) -> void
{
    va_list cells;
    va_start( cells, cells_to_seed );
    for ( int i = 0; i < cells_to_seed; i++ )
        current_state[ va_arg( cells, cell_index_t ) ] = live;
    va_end( cells );
}

auto Sim::seed(const cell_index_vec_t& cells) -> void
{
    std::for_each(cells.begin(), cells.end(),
        [this]( const cell_index_t& i ){
            this->current_state[i] = live;
    });
}

auto Sim::seed(const cell_index_t cell, const cell_index_vec_t& cells) -> void
{
    current_state[cell] = live;
    seed(cells);
}

auto Sim::generate_seed() -> void
{
    std::random_device rd;
    std::default_random_engine generator( rd() );
    std::uniform_int_distribution<int> distribution( 0, 1 );
    for ( int i = 0; i < settings::cell_count; i++ )
        if ( 1 == distribution( generator ) )
            current_state[i] = live;
}

void Sim::evolve()
{
    state_t new_state;
    new_state = current_state;
    neighborhood_t neighbors;
    for ( cell_index_t i = 0; i < settings::cell_count; i++ ){
        get_neighborhood(i, neighbors);
        count_t live_neighbors = live_neighbor_count( neighbors );
        if ( live == current_state[i] )
            switch ( live_neighbors ) {
                case 2:
                    break;
                case 3:
                    break;
                default:
                    new_state[i] = dead;
                    break;
            }
        else // this cell is dead
            switch ( live_neighbors ) {
                case 3:
                    new_state[i] = live;
                    break;
                default: break;
            }
    }
    current_state = new_state;
}
