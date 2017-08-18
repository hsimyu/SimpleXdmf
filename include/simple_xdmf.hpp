#ifndef SIMPLE_XDMF_HPP_INCLUDED
#define SIMPLE_XDMF_HPP_INCLUDED

#include <string>

class SimpleXdmf {
    private:
        const std::string header = R"(<?xml version="1.0" ?>
<!DOCTYPE Xdmf SYSTEM "Xdmf.dtd" []>)";
        std::string version;
        std::string content;

    public:
        SimpleXdmf(const std::string& _version = "3.0") {
            version = _version;

            content = header;
        }

        enum class DataItemType {Uniform};
        enum class TopologyType {twoDRectMesh};
        enum class GeometryType {Uniform};
        enum class AttributeType {Scalar, Vector};
        enum class AttributeCenter {Node};

        static void addDomain();

        void beginTopology(TopologyType type) {
            content += "<Topology TopologyType=\"2DRectMesh\">";
        }
        void endTopology() {
            content += "</Topology>";
        }

        void beginGeometory(GeometryType type){
            content += "<Geometry GeometryType=\"2DRectMesh\">";
        }
        void endGeometory(){
            content += "</Geometry>";
        }

        void beginDataItem(DataItemType type) {
            content += "<DataItem ItemType=\"Uniform\">";
        }
        void endDataItem() {
            content += "</DataItem>";
        }
};

#endif