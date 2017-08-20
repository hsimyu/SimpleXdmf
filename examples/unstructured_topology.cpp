#include <simple_xdmf.hpp>

int main() {
    SimpleXdmf gen;

    const int nx = 5;
    const int ny = 3;

    gen.beginDomain();
    gen.setName("Domain1");
        gen.beginGrid();
        gen.setName("Grid1");
            gen.beginUnstructuredTopology("Polyvertex");
            gen.setNumberOfElements(ny, nx);
            gen.setName("Topo1");
            gen.endUnstructuredTopology();
        gen.endGrid();
    gen.endDomain();

    gen.generate("unstructured_topology.xmf");

    return 0;
}