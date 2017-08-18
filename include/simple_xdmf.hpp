#ifndef SIMPLE_XDMF_HPP_INCLUDED
#define SIMPLE_XDMF_HPP_INCLUDED

#include <string>
#include <fstream>

class SimpleXdmf {
    private:
        const std::string header = R"(
<?xml version="1.0" ?>
<!DOCTYPE Xdmf SYSTEM "Xdmf.dtd" []>
)";
        std::string content;
        std::string version;
        std::string newLine;
        std::string indent;

        unsigned int currentIndentation = 0;
        bool endEdit = false;

        void addIndent() {
            ++currentIndentation;
        }

        void backIndent() {
            if (currentIndentation > 0) --currentIndentation;
        }

        void insertIndent() {
            for(unsigned int i = 0; i < currentIndentation; ++i) {
                content += indent;
            }
        }

        void beginElement() {
            addIndent();
            insertIndent();
        }

        void endElement() {
            backIndent();
        }

    public:
        SimpleXdmf(const std::string& _version = "3.0") {
            version = _version;
            setNewLineCode();
            setIndent();
            beginXdmf();
        }

        void setIndent(const int size = 4) {
            if (size == 0) {
                indent = '\t';
                return;
            }

            for (int i = 0; i < size; ++i) {
                indent += ' ';
            }
        }

        void setNewLineCode() {
            const char CR = '\r';
            const char LF = '\n';
            const char* CRLF = "\r\n";

            newLine = LF;
        }

        void generate(const std::string file_name) {
            if(!endEdit) endXdmf();

            std::ofstream ofs(file_name, std::ios::out);
            ofs << content << std::endl;
        }

        void beginXdmf() {
            endEdit = false;
            content = header + "<Xdmf Version=\"" + version + "\">" + newLine;
        }

        void endXdmf() {
            content += "</Xdmf>";
            endEdit = true;
        }

        enum class DataItemType {Uniform};
        enum class TopologyType {_2DSMesh, _2DRectMesh, _2DCoRectMesh, _3DSMesh, _3DRectMesh, _3DCoRectMesh};
        enum class GeometryType {Uniform};
        enum class AttributeType {Scalar, Vector};
        enum class AttributeCenter {Node};

        void beginDomain() {
            beginElement();
            content += "<Domain>" + newLine;
        };

        void endDomain() {
            insertIndent();
            content += "</Domain>" + newLine;
            endElement();
        };

        void beginTopology(TopologyType type = TopologyType::_2DCoRectMesh) {
            beginElement();

            using TType = TopologyType;
            std::string typeString;

            switch(type) {
                case TType::_2DSMesh:
                    typeString = "2DSMesh";
                case TType::_2DRectMesh:
                    typeString = "2DRectMesh";
                    break;
                case TType::_2DCoRectMesh:
                    typeString = "2DCoRectMesh";
                    break;
                case TType::_3DSMesh:
                    typeString = "3DSMesh";
                    break;
                case TType::_3DRectMesh:
                    typeString = "3DRectMesh";
                    break;
                case TType::_3DCoRectMesh:
                    typeString = "3DCoRectMesh";
                    break;
            }

            content += "<Topology TopologyType=\"" + typeString + "\">" + newLine;
        }

        void endTopology() {
            insertIndent();
            content += "</Topology>" + newLine;
            endElement();
        }

        void beginGeometory(GeometryType type = GeometryType::Uniform){
            beginElement();
            content += "<Geometry GeometryType=\"2DRectMesh\">" + newLine;
        }
        void endGeometory(){
            insertIndent();
            content += "</Geometry>" + newLine;
            endElement();
        }

        void beginDataItem(DataItemType type = DataItemType::Uniform) {
            beginElement();
            content += "<DataItem ItemType=\"Uniform\">" + newLine;
        }
        void endDataItem() {
            insertIndent();
            content += "</DataItem>" + newLine;
            endElement();
        }
};

#endif