# SimpleXdmf
A C++ header-only library to produce XDMF format file.
SimpleXdmf does not have complete functionalities for Xdmf, but does not require building with HDF5 (and Boost).

# Installation
Clone it into your project.
```bash
git clone https://github.com/hsimyu/SimpleXdmf.git
```
and copy a header file "simple_xdmf.hpp" to your include directory.

or, if your project already used cmake to build,
add the line below into your CMakeLists.txt.
```cmake
add_subdirectory(SimpleXdmf)
```

# How To Use
```cpp
#include <simple_xdmf.hpp>

int main() {
    SimpleXdmf gen;
    
    gen.beginDomain();
        gen.beginGrid();
            gen.beginTopology();
            gen.endTopology();
            gen.beginGeometry();
            gen.endGeometry();
            gen.beginAttribute();
            gen.endAttribute();
        gen.endGrid();
    gen.endDomain();
    
    gen.generate("output.xmf");
    
    return 0;
}
```

See the examples in the examples directory.

# License
MIT
