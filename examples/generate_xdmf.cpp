#include <simple_xdmf.hpp>

int main() {
    SimpleXdmf gen;

    gen.beginDomain();
        gen.beginTopology("3DCoRectMesh");
        gen.setNumberOfElements(3, 5);
        gen.setName("Topo1");

            gen.beginDataItem("Uniform");
            gen.setName("Data1");

            std::vector<float> values{1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
            gen.addItem(values);
            gen.endDataItem();

            gen.beginDataItem("Collection");
            gen.setName("Data2");

            std::vector<float> values2{1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.05, 12.4};
            gen.addItem(values2);

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