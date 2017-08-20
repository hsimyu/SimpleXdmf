#include <simple_xdmf.hpp>

int main() {
    SimpleXdmf gen;

    const float origin_x = 0;
    const float origin_y = 0;

    const float dx = 0.1;
    const float dy = 0.2;

    const int nx = 5;
    const int ny = 3;

    gen.beginDomain();
        gen.begin2DStructuredGrid("Grid1", "2DCoRectMesh", ny, nx);
            gen.add2DGeometryOrigin("Geom1", origin_y, origin_x, dy, dx);

            gen.beginAttribute("Node Scalar1", "Scalar");
                gen.beginDataItem();
                    gen.setDimensions(ny, nx);
                    gen.addItem(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
                gen.endDataItem();
            gen.endAttribute();

        gen.end2DStructuredGrid();
    gen.endDomain();

    gen.generate("using_2d_helper.xmf");

    return 0;
}