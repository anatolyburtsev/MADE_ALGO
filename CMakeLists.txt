cmake_minimum_required(VERSION 3.15)
project(made_algo)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -fno-omit-frame-pointer -mno-omit-leaf-frame-pointer")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fno-omit-frame-pointer -mno-omit-leaf-frame-pointer")
set(CMAKE_CXX_STANDARD 14)

add_executable(task1_2 algo1/task1_2.cpp)
add_executable(task2_3 algo1/task2_3.cpp)
add_executable(task6_4 algo1/task6_4.cpp)
add_executable(task8_2 algo1/task8_2.cpp)
add_executable(task7_1 algo1/task7_1.cpp)
add_executable(task11_1 algo1/task11_1.cpp)
add_executable(task16 algo1/task16.cpp)
add_executable(dynprog algo2/hw_01_02_dyn_prog/ztask_hw01_01_grasshopper.cpp)
add_executable(dynprog2 algo2/hw_01_02_dyn_prog/ztask_hw01_02_turtle.cpp)
add_executable(dynprog3 algo2/hw_01_02_dyn_prog/ztask_hw01_03_longest_inc_seq.cpp)
add_executable(dynprog4 algo2/hw_01_02_dyn_prog/ztask_hw01_04_levenstein_distance.cpp)
add_executable(dynprog5 algo2/hw_01_02_dyn_prog/ztask_hw02_01_pair_matching_general_graph.cpp)
add_executable(dynprog7 algo2/hw_01_02_dyn_prog/ztask_hw02_03_pair_matching_tree.cpp)
add_executable(dynprog6 algo2/hw_01_02_dyn_prog/ztask_hw02_04_hamilton_cycle.cpp)
add_executable(dynprog22 algo2/hw_01_02_dyn_prog/ztask_hw02_02_removing_brackets.cpp)
add_executable(dynprog25 algo2/hw_01_02_dyn_prog/ztask_hw02_05_domino.cpp)
add_executable(dynprog31 algo2/hw_03_04_number_theory/ztask_hw03_01_factorization.cpp)
add_executable(dynprog32 algo2/hw_03_04_number_theory/ztask_hw03_02_several_funcs.cpp)
add_executable(dynprog32_c algo2/hw_03_04_number_theory/ztask_hw03_02_several_funcs_cache.cpp)
add_executable(dynprog33 algo2/hw_03_04_number_theory/ztask_hw03_03_crack_RSA.cpp)
add_executable(dynprog34 algo2/hw_03_04_number_theory/ztask_hw03_04_point_on_plane.cpp)
add_executable(dynprog35 algo2/hw_03_04_number_theory/ztask_hw03_05_chinese_reminder_theorem.cpp)
add_executable(dynprog51 algo2/hw_05_06_intervals/hw05_01_rsq_simple.cpp)
add_executable(dynprog511 algo2/hw_05_06_intervals/hw05_01_rsq_simple_simple.cpp)
add_executable(dynprog52 algo2/hw_05_06_intervals/hw05_02_fenwick_tree.cpp)
add_executable(dynprog53 algo2/hw_05_06_intervals/hw05_03_segment_tree.cpp)
add_executable(dynprog54 algo2/hw_05_06_intervals/hw05_04_sparse_matrix_simple_vector.cpp)
add_executable(dp61 algo2/hw_05_06_intervals/hw06_01_multidim_fenwick.cpp)
add_executable(dp63 algo2/hw_05_06_intervals/hw06_03_LCA.cpp)
add_executable(dp65 algo2/hw_05_06_intervals/hw06_05_LCA_min.cpp minonpath.in)
add_executable(dp81 algo2/hw_08_09_flow/hw08_01_max_flow.cpp)
add_executable(dp82 algo2/hw_08_09_flow/hw08_02_min_cut.cpp)
add_executable(dp83 algo2/hw_08_09_flow/hw08_03_two_paths.cpp)
add_executable(dp91 algo2/hw_08_09_flow/hw09_01_min_cost.cpp)
add_executable(dp92 algo2/hw_08_09_flow/hw09_02_assignment_problem.cpp)
add_executable(dp922 algo2/hw_08_09_flow/hw09_02_assignment_problem_dijkstra.cpp)
add_executable(dp111 algo2/hw_11_string/hw11_01_suffix_array.cpp)

