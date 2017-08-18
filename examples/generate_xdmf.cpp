#include <simple_xdmf.hpp>

int main() {
    SimpleXdmf gen;

    gen.beginDomain();
        gen.beginTopology("3DCoRectMesh");
        gen.setDimensions();
        gen.setName("Topo1");
            gen.beginDataItem("Uniform");
            gen.setName("Data1");
            gen.endDataItem();
            gen.beginDataItem("Collection");
            gen.setName("Data2");
            gen.endDataItem();
            gen.beginDataItem("Tree");
            gen.setName("Data3");
            gen.endDataItem();
        gen.endTopology();
        gen.beginGeometory();
        gen.setName("Geom1");
            gen.beginDataItem();
            gen.endDataItem();
        gen.endGeometory();
    gen.endDomain();

    gen.generate("test.xmf");

    return 0;
}