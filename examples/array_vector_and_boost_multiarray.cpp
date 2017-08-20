
#define USE_BOOST
#include <simple_xdmf.hpp>

int main() {
    SimpleXdmf gen;

    const float origin_x = 0;
    const float origin_y = 0;

    const float dx = 0.1;
    const float dy = 0.2;

    const int nx = 5;
    const int ny = 3;

    const int N = nx * ny;

    gen.beginDomain();
        gen.begin2DStructuredGrid("Grid1", "2DCoRectMesh", nx, ny);

            gen.add2DGeometryOrigin("Geom1", origin_x, origin_y, dx, dy);
            // ----------------------------------------------------------
            std::vector<float> vector_values;
            vector_values.resize(nx * ny);

            for(int i = 0; i < vector_values.size(); ++i) {
                vector_values[i] = 1.0f * static_cast<float>(i);
            }
            gen.beginAttribute("by std::vector", "Scalar");
                gen.beginDataItem();
                    gen.setDimensions(nx, ny);
                    // 1D vector values will be assigned with column-major order in XDMF.
                    gen.addVector(vector_values);
                gen.endDataItem();
            gen.endAttribute();
            // ----------------------------------------------------------
            std::array<float, N> array_values;
            for(int i = 0; i < array_values.size(); ++i) {
                array_values[i] = 1.0f * static_cast<float>(i);
            }

            gen.beginAttribute("by std::array", "Scalar");
                gen.beginDataItem();
                    gen.setDimensions(nx, ny);
                    gen.addArray(array_values);
                gen.endDataItem();
            gen.endAttribute();
            // ----------------------------------------------------------
            float* standard_1d_array_values = new float[N];
            for(int i = 0; i < N; ++i) {
                standard_1d_array_values[i] = 1.0f * static_cast<float>(i);
            }

            gen.beginAttribute("by standard 1D array", "Scalar");
                gen.beginDataItem();
                    gen.setDimensions(nx, ny);
                    gen.addArray(standard_1d_array_values, N);
                gen.endDataItem();
            gen.endAttribute();

            delete [] standard_1d_array_values;
            // ----------------------------------------------------------
            float** standard_2d_array_values = new float*[nx];
            // now we assign values with column-major order
            // array[0][0] = 0, array[1][0] = 1, array[2][0] = 2, ...
            for(int i = 0; i < nx; ++i) {
                standard_2d_array_values[i] = new float[ny];
                for(int j = 0; j < ny; ++j) {
                    standard_2d_array_values[i][j] = 1.0f * static_cast<float>(j * nx + i);
                }
            }
            gen.beginAttribute("by standard 2D array", "Scalar");
                gen.beginDataItem();
                    gen.setDimensions(nx, ny);
                    gen.add2DArray(standard_2d_array_values, nx, ny);
                gen.endDataItem();
            gen.endAttribute();

            for(int i = 0; i < nx; ++i) {
                delete [] standard_2d_array_values[i];
            }
            delete [] standard_2d_array_values;
            // ----------------------------------------------------------
            boost::multi_array<float, 2> bml_array(boost::extents[nx][ny]);
            for(int i = 0; i < bml_array.shape()[0]; ++i) {
                for(int j = 0; j < bml_array.shape()[1]; ++j) {
                    bml_array[i][j] = 1.0f * static_cast<float>(j * nx + i);
                }
            }

            gen.beginAttribute("by boost multi array", "Scalar");
                gen.beginDataItem();
                    gen.setDimensions(nx, ny);
                    gen.addMultiArray(bml_array);
                gen.endDataItem();

                /* If multi_array is initialized with boost::fortran_storage_order(), pass true as the 2nd argument */
                bool is_fortran_storage_order = true;
                gen.beginDataItem();
                    gen.setDimensions(nx, ny);
                    gen.addMultiArray(bml_array, is_fortran_storage_order);
                gen.endDataItem();
            gen.endAttribute();

        gen.end2DStructuredGrid();
    gen.endDomain();

    gen.generate("array_vector_and_boost_multiarray.xmf");

    return 0;
}