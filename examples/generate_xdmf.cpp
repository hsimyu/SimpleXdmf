#include <simple_xdmf.hpp>

int main() {
    SimpleXdmf gen;

    gen.beginDomain();
        gen.beginTopology("3DCoRectMesh");
        gen.setNumberOfElements(3, 5);
        gen.setName("Topo1");

            gen.beginDataItem("Uniform");
            gen.setName("Data1");
            gen.addItem();
            gen.addItem();
            gen.endDataItem();

            gen.beginDataItem("Collection");
            gen.setName("Data2");
            gen.addItem();
            gen.endDataItem();

            gen.beginDataItem("Tree");
            gen.setName("Data3");
            gen.endDataItem();

        gen.endTopology();

        gen.beginGeometory();
        gen.setDimensions(3, 5);
        gen.setName("Geom1");
            gen.beginDataItem();
            gen.endDataItem();
        gen.endGeometory();

        gen.beginAttribute();
        gen.setName("Attr1");
            gen.beginDataItem();
            gen.setDimensions(3, 5);
            gen.endDataItem();
        gen.endAttribute();
    gen.endDomain();

    gen.generate("test.xmf");

    return 0;
}