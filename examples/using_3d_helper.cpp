#include <simple_xdmf.hpp>

int main() {
    SimpleXdmf gen;

    const float origin_x = 0;
    const float origin_y = 0;
    const float origin_z = 0;

    const float dx = 0.1;
    const float dy = 0.2;
    const float dz = 0.3;

    const int nx = 5;
    const int ny = 3;
    const int nz = 8;

    std::vector<float> values;
    values.resize(nx * ny * nz);

    for(int i = 0; i < values.size(); ++i) {
        values[i] = 10.0f * static_cast<float>(i);
    }

    gen.beginDomain();
        // This method automatically insert Grid and StructuredTopology
        gen.begin3DStructuredGrid("Grid1", "3DCoRectMesh", nx, ny, nz);

            gen.add3DGeometryOrigin("Geom1", origin_x, origin_y, origin_z, dx, dy, dz);
            gen.beginAttribute("Node Scalar1", "Scalar");
                gen.beginDataItem();
                    gen.setDimensions(nx, ny, nz);
                    gen.addVector(values);
                gen.endDataItem();
            gen.endAttribute();

        gen.end3DStructuredGrid();
    gen.endDomain();

    gen.generate("using_3d_helper.xmf");

    return 0;
}