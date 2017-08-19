#include <simple_xdmf.hpp>

int main() {
    SimpleXdmf gen;

    const int nx = 5;
    const int ny = 3;

    gen.beginDomain();
        gen.beginGrid();
        gen.setName("Grid1");
        gen.endGrid();

        gen.beginGrid();
        gen.setName("Grid2");
        gen.setReferenceFromName("Grid1"); // set Xpath as Reference attribute
        gen.endGrid();

        gen.beginGrid();
        gen.setName("Grid3");
            gen.addReferenceFromName("Grid3"); // as inner reference element
        gen.endGrid();

        gen.beginGrid();
        gen.setName("Grid4");
        gen.setReference("/Xdmf/Domain/Grid2"); // or directly setting
        gen.endGrid();
    gen.endDomain();

    gen.generate("reference_management.xmf");
    return 0;
}