#ifdef TEST
BOOST_AUTO_TEST_CASE(BoardSizeTest)
{
    Sim sim;
    count_t total_cells = settings::board_width*settings::board_height;
    BOOST_CHECK_EQUAL(sim.total_cells(), total_cells);
}

BOOST_AUTO_TEST_CASE(LiveCellCount)
{
    Sim sim;
    cell_pos_t central_cell = sim.get_central_cell();
    cell_pos_t east_neighbor = sim.get_neighbor(central_cell, east);
    sim.seed(2, central_cell, east_neighbor);
    BOOST_CHECK_EQUAL(sim.total_live_cells(), 2);
}

BOOST_AUTO_TEST_CASE(UnderPopulationTest)
{
    Sim sim;
    cell_pos_t central_cell = sim.get_central_cell();
    sim.seed(1, central_cell);
    sim.evolve();
    BOOST_CHECK(sim.board_is_dead());
}

BOOST_AUTO_TEST_CASE(ThreeLiveNeighborsReproductionTest)
{
    Sim sim;
    cell_pos_t central_cell = sim.get_central_cell();
    cell_pos_t right_cell = sim.get_neighbor(central_cell, east);
    cell_pos_t left_cell = sim.get_neighbor(central_cell, west);
    cell_pos_t top_cell = sim.get_neighbor(central_cell, north);
    sim.seed(3, right_cell, left_cell, top_cell);
    BOOST_CHECK_EQUAL(sim.total_live_cells(), 3);
    sim.evolve();
    BOOST_CHECK(sim.cell_is_live(central_cell));
}

BOOST_AUTO_TEST_CASE(UnderpopulationTest)
{
    Sim sim;
    cell_pos_t central_cell = sim.get_central_cell();
    cell_pos_t right_cell = sim.get_neighbor(central_cell, east);
    sim.seed(2, central_cell, right_cell);
    sim.evolve();
    BOOST_CHECK(sim.cells_are_dead(2, central_cell, right_cell));
}

BOOST_AUTO_TEST_CASE(GetNeighborsVectorTest)
{
    Sim sim;
    cell_pos_t central_cell = sim.get_central_cell();
    direction_vec_t directions{north, south, east, west};
    cell_pos_vec_t neighbors = sim.get_neighbors(central_cell, directions);
    BOOST_CHECK_EQUAL(neighbors.size(), 4);
}

BOOST_AUTO_TEST_CASE(OvercrowdingTest)
{
    Sim sim;
    cell_pos_t central_cell = sim.get_central_cell();
    cell_pos_vec_t neighbors = sim.get_cardinal_neighbors(central_cell);
    sim.seed(central_cell, neighbors);
    sim.evolve();
    BOOST_CHECK(sim.cell_is_dead(central_cell));
}

#ifdef MOORE

BOOST_AUTO_TEST_CASE(MooreNeighborhoodTest)
{
    moore_t neighbors;
    BOOST_CHECK_EQUAL(neighbors.size(), 8);
}

BOOST_AUTO_TEST_CASE(MooreZeroLiveNeighborsTest)
{
    Sim sim;
    cell_pos_t central_cell = sim.get_central_cell();
    neighborhood_t neighbors;
    sim.get_neighborhood(central_cell, neighbors);
    count_t live_neighbors = sim.live_neighbor_count(neighbors);
    BOOST_CHECK_EQUAL(live_neighbors, 0);
}

BOOST_AUTO_TEST_CASE(MooreOneLiveNeighborsTest)
{
    Sim sim;
    cell_pos_t central_cell = sim.get_central_cell();
    cell_pos_t east_neighbor = sim.get_neighbor(central_cell, east);
    sim.seed(2, central_cell, east_neighbor);
    neighborhood_t neighbors;
    sim.get_neighborhood(central_cell, neighbors);
    count_t live_neighbors = sim.live_neighbor_count(neighbors);
    BOOST_CHECK_EQUAL(live_neighbors, 1);
}

BOOST_AUTO_TEST_CASE(MooreTwoLiveNeighborsSurvivalTest)
{
    Sim sim;
    cell_pos_t central_cell = sim.get_central_cell();
    BOOST_CHECK(!sim.is_edge_cell(central_cell));
    cell_pos_t right_cell = sim.get_neighbor(central_cell, east);
    cell_pos_t left_cell = sim.get_neighbor(central_cell, west);
    sim.seed(3, left_cell, central_cell, right_cell);
    BOOST_CHECK_EQUAL(sim.total_live_cells(), 3);
    neighborhood_t neighbors;
    sim.evolve();
    cell_pos_t top_cell = sim.get_neighbor(central_cell, north);
    cell_pos_t bottom_cell = sim.get_neighbor(central_cell, south);
    BOOST_CHECK(sim.cell_is_live(central_cell));
    BOOST_CHECK(sim.cell_is_live(top_cell));
    BOOST_CHECK(sim.cell_is_live(bottom_cell));
    BOOST_CHECK_EQUAL(sim.total_live_cells(), 3);
}

BOOST_AUTO_TEST_CASE(MooreBlinkerTest)
{
    Sim sim;
    cell_pos_t central_cell = sim.get_central_cell();
    cell_pos_t right_cell = sim.get_neighbor(central_cell, east);
    cell_pos_t left_cell = sim.get_neighbor(central_cell, west);
    sim.seed(3, central_cell, right_cell, left_cell);
    state_t initial_state = sim.get_state();
    sim.evolve();
    state_t other_state = sim.get_state();
    sim.evolve();
    state_t initial_state_2 = sim.get_state();
    BOOST_CHECK(std::equal(initial_state.begin(), initial_state.end(), initial_state_2.begin()));
    sim.evolve();
    state_t other_state_2 = sim.get_state();
    BOOST_CHECK(std::equal(other_state_2.begin(), other_state_2.end(), other_state.begin()));
}


BOOST_AUTO_TEST_CASE(MooreGliderInitiationTest)
{
    Sim sim;
    cell_pos_t central_cell = sim.get_central_cell();
    direction_vec_t directions{north, east, southeast, south, southwest};
    cell_pos_vec_t neighbors = sim.get_neighbors(central_cell, directions);
    sim.seed(neighbors);
    BOOST_CHECK(sim.cells_are_live(neighbors));
}

BOOST_AUTO_TEST_CASE(MooreGliderSecondStateTest)
{
    Sim sim;
    cell_pos_t central_cell_before = sim.get_central_cell();
    BOOST_CHECK(!sim.is_edge_cell(central_cell_before));
    direction_vec_t directions_before{north, east, southeast, south, southwest};
    cell_pos_vec_t neighbors_before = sim.get_neighbors(central_cell_before, directions_before);
    sim.seed(neighbors_before);
    BOOST_CHECK_EQUAL(sim.total_live_cells(), 5);
    sim.evolve();
    BOOST_CHECK_EQUAL(sim.total_live_cells(), 5);
    cell_pos_t central_cell_after = sim.get_neighbor(sim.get_central_cell(), south);
    direction_vec_t directions_after{northwest, northeast, east, south};
    cell_pos_vec_t neighbors_after = sim.get_neighbors(central_cell_after, directions_after);
    BOOST_CHECK(sim.cell_is_live(central_cell_after));
    BOOST_CHECK(sim.cells_are_live(neighbors_after));
}

BOOST_AUTO_TEST_CASE(MooreStaticBoatTest)
{
    Sim sim;
    direction_vec_t directions{north, south, east, west, northwest};
    cell_pos_vec_t neighbors = sim.get_neighbors(sim.get_central_cell(), directions);
    sim.seed(neighbors);
    sim.evolve();
    BOOST_CHECK_EQUAL(sim.total_live_cells(), 5);
    BOOST_CHECK(sim.cells_are_live(neighbors));
}

BOOST_AUTO_TEST_CASE(MooreOvercrowdingTest)
{
    Sim sim;
    cell_pos_t central_cell = sim.get_central_cell();
    cell_pos_vec_t intercardinals = sim.get_intercardinal_neighbors(central_cell);
    sim.seed(central_cell, intercardinals);
    sim.evolve();
    BOOST_CHECK(sim.cell_is_dead(central_cell));
}

BOOST_AUTO_TEST_CASE(MooreFourLiveNeighborsDestructionTest)
{
    Sim sim;
    cell_pos_t central_cell = sim.get_central_cell();
    direction_vec_t directions{north, south, east, west};
    cell_pos_vec_t neighbors = sim.get_neighbors(central_cell, directions);
    sim.seed(central_cell, neighbors);
    BOOST_CHECK_EQUAL(sim.total_live_cells(), 5);
    BOOST_CHECK(sim.cells_are_live(central_cell, neighbors));
    neighborhood_t neighborhood;
    sim.get_neighborhood(central_cell, neighborhood);
    BOOST_CHECK_EQUAL(sim.live_neighbor_count(neighborhood), 4);
    sim.evolve();
    BOOST_CHECK_EQUAL(sim.total_live_cells(), 8);
    sim.get_neighborhood(central_cell, neighborhood);
    BOOST_CHECK_EQUAL(sim.live_neighbor_count(neighborhood), 8);
    BOOST_CHECK(sim.cell_is_dead(central_cell));
}

BOOST_AUTO_TEST_CASE(MooreStillLifeBlockTest)
{
    Sim sim;
    cell_pos_t central_cell = sim.get_central_cell();
    direction_vec_t directions{east, southeast, south};
    cell_pos_vec_t neighbors = sim.get_neighbors(central_cell, directions);
    sim.seed(central_cell, neighbors);
    neighborhood_t neighborhood;
    sim.get_neighborhood(central_cell, neighborhood);
    BOOST_CHECK_EQUAL(sim.live_neighbor_count(neighborhood), 3);
    sim.get_neighborhood(neighbors[0], neighborhood);
    BOOST_CHECK_EQUAL(sim.live_neighbor_count(neighborhood), 3);
    sim.get_neighborhood(neighbors[1], neighborhood);
    BOOST_CHECK_EQUAL(sim.live_neighbor_count(neighborhood), 3);
    sim.get_neighborhood(neighbors[2], neighborhood);
    BOOST_CHECK_EQUAL(sim.live_neighbor_count(neighborhood), 3);
    BOOST_CHECK_EQUAL(sim.total_live_cells(), 4);
    sim.evolve();
    sim.get_neighborhood(central_cell, neighborhood);
    BOOST_CHECK_EQUAL(sim.live_neighbor_count(neighborhood), 3);
    sim.get_neighborhood(neighbors[0], neighborhood);
    BOOST_CHECK_EQUAL(sim.live_neighbor_count(neighborhood), 3);
    sim.get_neighborhood(neighbors[1], neighborhood);
    BOOST_CHECK_EQUAL(sim.live_neighbor_count(neighborhood), 3);
    sim.get_neighborhood(neighbors[2], neighborhood);
    BOOST_CHECK_EQUAL(sim.live_neighbor_count(neighborhood), 3);
    BOOST_CHECK_EQUAL(sim.total_live_cells(), 4);
    BOOST_CHECK(sim.cells_are_live(central_cell, neighbors));
}
#endif // MOORE

#ifdef VON_NEUMANN
BOOST_AUTO_TEST_CASE(VonNeumannNeighborhoodTest)
{
    Sim sim;
    cell_pos_t central_cell = sim.get_central_cell();
    cell_pos_t right_cell = sim.get_neighbor(central_cell, east);
    cell_pos_t left_cell = sim.get_neighbor(central_cell, west);
    sim.seed(3, left_cell, central_cell, right_cell);
    neighborhood_t central_cell_neighbors;
    sim.get_neighborhood(central_cell, central_cell_neighbors);
    neighborhood_t right_cell_neighbors;
    sim.get_neighborhood(right_cell, right_cell_neighbors);
    neighborhood_t left_cell_neighbors;
    sim.get_neighborhood(left_cell, left_cell_neighbors);
    BOOST_CHECK_EQUAL(sim.live_neighbor_count(central_cell_neighbors), 2);
    BOOST_CHECK_EQUAL(sim.live_neighbor_count(right_cell_neighbors), 1);
    BOOST_CHECK_EQUAL(sim.live_neighbor_count(left_cell_neighbors), 1);
}

BOOST_AUTO_TEST_CASE(VonNeumannTwoLiveNeighborsSurvivalTest)
{
    Sim sim;
    cell_pos_t central_cell = sim.get_central_cell();
    cell_pos_t right_cell = sim.get_neighbor(central_cell, east);
    cell_pos_t left_cell = sim.get_neighbor(central_cell, west);
    sim.seed(3, left_cell, central_cell, right_cell);
    BOOST_CHECK_EQUAL(sim.total_live_cells(), 3);
    sim.evolve();
    BOOST_CHECK_EQUAL(sim.total_live_cells(), 1);
    BOOST_CHECK(sim.cell_is_live(central_cell));
    BOOST_CHECK(sim.cell_is_dead(right_cell));
    BOOST_CHECK(sim.cell_is_dead(left_cell));
}

BOOST_AUTO_TEST_CASE(VonNeumannThreeLiveNeighborsSurvivalTest)
{
    Sim sim;
    cell_pos_t central_cell = sim.get_central_cell();
    cell_pos_t right_cell = sim.get_neighbor(central_cell, east);
    cell_pos_t left_cell = sim.get_neighbor(central_cell, west);
    cell_pos_t top_cell = sim.get_neighbor(central_cell, north);
    sim.seed(4, central_cell, right_cell, left_cell, top_cell);
    sim.evolve();
    BOOST_CHECK(sim.cell_is_live(central_cell));
    BOOST_CHECK_EQUAL(sim.total_live_cells(), 1);
}

BOOST_AUTO_TEST_CASE(VonNeumannOvercrowdingTest)
{
    Sim sim;
    cell_pos_t central_cell = sim.get_central_cell();
    cell_pos_vec_t cardinals = sim.get_cardinal_neighbors(central_cell);
    sim.seed(central_cell, cardinals);
    sim.evolve();
    BOOST_CHECK_EQUAL(sim.total_live_cells(), 0);
}

#endif // VON_NEUMANN

#endif // TEST

