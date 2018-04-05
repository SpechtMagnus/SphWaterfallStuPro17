#include "cui/CUI.h"

int main(int argc, char** argv) {
	MPI_Init(&argc, &argv);

	int mpi_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

	// generate slave_comm and slave_comm_size for simulation
	int color = 1337;
	if (mpi_rank == 0) {
		color = MPI_UNDEFINED;
	}
	MPI_Comm_split(MPI_COMM_WORLD, color, 0, &slave_comm);
	if (mpi_rank != 0) {
		MPI_Comm_size(slave_comm, &slave_comm_size);
	}

	// start cui and command handling
	CommandHandler command_handler = CommandHandler(mpi_rank);
	if (mpi_rank == 0) {
		CUI cui = CUI(command_handler);
		cui.start();
	}
	else {
		command_handler.start();
	}

	MPI_Finalize();

	return 0;
}
