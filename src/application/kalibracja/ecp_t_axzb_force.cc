#include "ecp_t_axzb_force.h"
#include "ecp_t_acq_force.h"
#include "ecp_t_calib_axzb.h"

namespace mrrocpp {
namespace ecp {
namespace common {
namespace task {

// KONSTRUKTORY
axzb_force::axzb_force(lib::configurator &_config) : calib_axzb(_config)
{
	ofp.number_of_measures = config.value<int>("measures_count");
	ofp.magical_c = config.value<double>("magical_c");
	std::string K_file_path = config.value<std::string>("K_file_path");
	std::string kk_file_path = config.value<std::string>("kk_file_path");
	std::string M_file_path = config.value<std::string>("M_file_path");
	std::string mm_file_path = config.value<std::string>("mm_file_path");

	//run a task to get the data if needed
	if (config.value<int>("acquire")) {
	acq_force* acq_task = new acq_force(_config);
	acq_task->write_data(K_file_path, kk_file_path, M_file_path, mm_file_path, ofp.number_of_measures);
	delete acq_task;
	}

	//load the data
	// translation vector (from robot base to tool frame) - received from MRROC
	ofp.k = gsl_vector_calloc (3*ofp.number_of_measures);
	// rotation matrix (from robot base to tool frame) - received from MRROC
	ofp.K = gsl_matrix_calloc (3*ofp.number_of_measures, 3);
	// translation vector (from chessboard base to camera frame)
	ofp.m = gsl_vector_calloc (3*ofp.number_of_measures);
	// rotation matrix (from chessboard base to camera frame)
	ofp.M = gsl_matrix_calloc (3*ofp.number_of_measures, 3);
	FILE *FP;
	FP = fopen(K_file_path.c_str(),"r");
	gsl_matrix_fscanf(FP,ofp.K);
	fclose(FP);
	FP = fopen(kk_file_path.c_str(),"r");
	gsl_vector_fscanf(FP,ofp.k);
	fclose(FP);
	FP = fopen(M_file_path.c_str(),"r");
	gsl_matrix_fscanf(FP,ofp.M);
	fclose(FP);
	FP = fopen(mm_file_path.c_str(),"r");
	gsl_vector_fscanf(FP,ofp.m);
	fclose(FP);
}

void axzb_force::main_task_algorithm(void)
{
	calib_axzb::main_task_algorithm();
	ecp_termination_notice();
}

task* return_created_ecp_task (lib::configurator &_config)
{
	return new axzb_force(_config);
}

} // namespace task
} // namespace common
} // namespace ecp
} // namespace mrrocpp