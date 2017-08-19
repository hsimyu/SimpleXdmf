#include <simple_xdmf.hpp>

int main() {
    SimpleXdmf gen;

    // Some settings
    gen.setNewLineCodeLF();

    const int nx = 5;
    const int ny = 3;

    gen.beginDomain();
        gen.beginGrid();
        gen.setName("Grid1");
        gen.endGrid();

        gen.beginGrid();
        gen.setName("Grid2");
        gen.setReferenceFromName("Grid1");
        gen.endGrid();

        gen.beginGrid();
        gen.setName("Grid3");
        gen.setReferenceFromName("Grid3");
            gen.addReferenceFromName("Grid3");
        gen.endGrid();
    gen.endDomain();

    gen.generate("reference_management.xmf");

    return 0;
}