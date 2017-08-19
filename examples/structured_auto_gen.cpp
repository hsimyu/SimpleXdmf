#include <simple_xdmf.hpp>

int main() {
    SimpleXdmf gen;

    // Some settings
    gen.setNewLineCodeLF();

    const int nx = 5;
    const int ny = 3;

    gen.beginDomain();
        gen.beginSet();
        gen.endSet();
        gen.beginTime();
        gen.endTime();
        gen.beginInformation();
        gen.endInformation();
    gen.endDomain();

    gen.generate("structured.xmf");

    return 0;
}