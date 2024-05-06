#include <mpi.h>
#include <omp.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// Proposed reaction function.
double reaction(const char* seq_code);

std::vector<std::string> read_seq_codes(const std::string& file_path) {
    std::vector<std::string> codes;
    std::ifstream file(file_path);
    std::string line;
    while (std::getline(file, line)) {
        codes.push_back(line);
    }
    return codes;
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    // Set the number of threads to 2 for each process.
    omp_set_num_threads(2);
    //  Dispatch each years to one process.
    std::string years[4] = {"2020", "2021", "2022", "2023"};
    std::string year = years[world_rank];
    std::string file_path = "/data/" + year;
    std::vector<std::string> seq_codes = read_seq_codes(file_path);
    std::ofstream output_file(year + "_seqcode_res.dat");
    #pragma omp parallel for
    for (int i = 0; i < seq_codes.size(); ++i) {
        double result = reaction(seq_codes[i].c_str());
        #pragma omp critical
        {
            output_file << seq_codes[i] << "\t" << result << std::endl;
        }
    }
    output_file.close();
    MPI_Finalize();
    return 0;
}