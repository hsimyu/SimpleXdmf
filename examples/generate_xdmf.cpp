#include <simple_xdmf.hpp>

int main() {
    SimpleXdmf gen;

    // Some settings
    gen.setNewLineCodeLF();

    const int nx = 5;
    const int ny = 3;

    gen.beginDomain();
    gen.setName("Domain1");
        gen.beginGrid();
        gen.setName("Grid1");
            gen.beginStructuredTopology("2DCoRectMesh");
            gen.setNumberOfElements(ny, nx);
            gen.setName("Topo1");
            gen.endStructuredTopology();

            gen.beginGeometory("ORIGIN_DXDY");
            gen.setDimensions(ny, nx);
            gen.setName("Geom1");
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

            gen.beginAttribute();
            gen.setCenter("Node");
            gen.setName("Attr1");
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

    gen.generate("test.xmf");

    return 0;
}