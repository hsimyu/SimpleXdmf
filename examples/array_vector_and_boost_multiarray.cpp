#include <simple_xdmf.hpp>

int main() {
    SimpleXdmf gen;

    const float origin_x = 0;
    const float origin_y = 0;

    const float dx = 0.1;
    const float dy = 0.2;

    const int nx = 5;
    const int ny = 3;

    std::vector<float> vector_values;
    vector_values.resize(nx * ny);

    for(int i = 0; i < vector_values.size(); ++i) {
        vector_values[i] = 10.0f * static_cast<float>(i);
    }

    const int N = nx * ny;
    std::array<float, N> array_values;
    for(int i = 0; i < array_values.size(); ++i) {
        array_values[i] = 5.0f * static_cast<float>(i);
    }

    float* standard_array_values = new float[N];
    for(int i = 0; i < N; ++i) {
        standard_array_values[i] = 2.0f * static_cast<float>(i);
    }

    gen.beginDomain();
        gen.begin2DStructuredGrid("Grid1", "2DCoRectMesh", ny, nx);

            gen.add2DGeometryOrigin("Geom1", origin_y, origin_x, dy, dx);

            gen.beginAttribute("by std::vector", "Scalar");
                gen.beginDataItem();
                    gen.setDimensions(ny, nx);
                    gen.addVector(vector_values);
                gen.endDataItem();
            gen.endAttribute();

            gen.beginAttribute("by std::array", "Scalar");
                gen.beginDataItem();
                    gen.setDimensions(ny, nx);
                    gen.addArray(array_values);
                gen.endDataItem();
            gen.endAttribute();

            gen.beginAttribute("by standard array", "Scalar");
                gen.beginDataItem();
                    gen.setDimensions(ny, nx);
                    gen.addArray(standard_array_values, N);
                gen.endDataItem();
            gen.endAttribute();

        gen.end2DStructuredGrid();
    gen.endDomain();

    gen.generate("array_vector_and_boost_multiarray.xmf");

    return 0;
}