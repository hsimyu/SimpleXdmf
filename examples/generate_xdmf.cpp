#include <simple_xdmf.hpp>

int main() {
    SimpleXdmf gen;

    const int nx = 5;
    const int ny = 3;

    gen.beginDomain("Domain1");
        gen.beginGrid("Grid1");
            gen.beginStructuredTopology("Topo1", "2DCoRectMesh");
            gen.setNumberOfElements(ny, nx);
            gen.endStructuredTopology();

            gen.beginGeometory("Geom1", "ORIGIN_DXDY");
            gen.setDimensions(ny, nx);
                // Origin
                gen.beginDataItem();
                    gen.setDimensions(2);
                    gen.setFormat("XML");
                    gen.addItem(0.0, 0.0);
                gen.endDataItem();

                gen.beginDataItem();
                    gen.setDimensions(2);
                    gen.setFormat("XML");
                    gen.addItem(0.5, 0.5);
                gen.endDataItem();
            gen.endGeometory();

            gen.beginAttribute("Attr1");
            gen.setCenter("Node");
                gen.beginDataItem();
                    gen.setDimensions(ny, nx);
                    gen.setFormat("XML");

                    // Adding from std::vector
                    std::vector<float> node_values;

                    for(int i = 0; i < (nx * ny); ++i) {
                        node_values.emplace_back(i);
                    }

                    gen.addVector(node_values);
                gen.endDataItem();
            gen.endAttribute();
        gen.endGrid();
    gen.endDomain();

    gen.generate("generate_xdmf.xmf");

    return 0;
}