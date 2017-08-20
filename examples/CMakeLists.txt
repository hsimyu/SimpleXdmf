set(EXAMPLE_GENERATE_XDMF_SRC "generate_xdmf.cpp")
add_executable(example_generate_xdmf ${EXAMPLE_GENERATE_XDMF_SRC})

set(EXAMPLE_REFERENCE_MANAGEMENT_SRC "reference_management.cpp")
add_executable(example_reference_management ${EXAMPLE_REFERENCE_MANAGEMENT_SRC})

set(EXAMPLE_UNSTRUCTURED_SRC "unstructured_topology.cpp")
add_executable(example_unstructured_topology ${EXAMPLE_UNSTRUCTURED_SRC})

set(EXAMPLE_USING_2D_HELPER "using_2d_helper.cpp")
add_executable(example_using_2d_helper ${EXAMPLE_USING_2D_HELPER})

set(EXAMPLE_USING_3D_HELPER "using_3d_helper.cpp")
add_executable(example_using_3d_helper ${EXAMPLE_USING_3D_HELPER})