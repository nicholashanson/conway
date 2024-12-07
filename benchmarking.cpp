#include "Sim.h"
#include <chrono>
#include <iostream>
#include <fstream>

int main(){
    Sim sim;

    std::ofstream csv_file;

    csv_file.open( "benchmarking_data.csv" );

    sim.generate_seed();

    for ( unsigned i = 0; i < 200; i++ ) {
        const auto start_time{ std::chrono::steady_clock::now() };
        sim.evolve();
        const auto end_time{ std::chrono::steady_clock::now() };
        const std::chrono::duration<double> elapsed_seconds{ end_time - start_time };
        long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>( elapsed_seconds ).count();

        csv_file << microseconds << ", ";
    }


    // delete last comma
    long pos = csv_file.tellp();
    csv_file.seekp( pos - 2 );
    csv_file.write( " ", 1);

    csv_file.close();

    return 0;
}
