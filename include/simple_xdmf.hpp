#ifndef SIMPLE_XDMF_HPP_INCLUDED
#define SIMPLE_XDMF_HPP_INCLUDED

#include <array>
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

        // for checking valid types
        static constexpr int dataItemTypeLength = 6;
        static constexpr int gridTypeLength = 4;
        static constexpr int topologyTypeLength = 6;
        static constexpr int geometryTypeLength = 6;
        static constexpr int attributeTypeLength = 5;
        static constexpr int attributeCenterLength = 5;
        static constexpr int setTypeLength = 4;
        static constexpr int timeTypeLength = 4;
        static constexpr int formatTypeLength = 3;
        static constexpr int numberTypeLength = 5;
        static constexpr int precisionTypeLength = 4;

        std::array<std::string, dataItemTypeLength> DataItemType {"Uniform", "Collection", "Tree", "HyperSlab", "Coordinates", "Function"};
        std::array<std::string, gridTypeLength> GridType {"Uniform", "Collection", "Tree", "Subset"};
        std::array<std::string, topologyTypeLength> TopologyType {"2DSMesh", "2DRectMesh", "2DCoRectMesh", "3DSMesh", "3DRectMesh", "3DCoRectMesh"};
        std::array<std::string, geometryTypeLength> GeometryType {"XYZ", "XY", "X_Y_Z", "VXVYVZ", "ORIGIN_DXDYDZ", "ORIGIN_DXDY"};
        std::array<std::string, attributeTypeLength> AttributeType {"Scalar", "Vector", "Tensor", "Tensor6", "Matrix"};
        std::array<std::string, attributeCenterLength> AttributeCenter {"Node", "Edge", "Face", "Cell", "Grid"};
        std::array<std::string, setTypeLength> SetType {"Node", "Edge", "Face", "Cell"};
        std::array<std::string, timeTypeLength> TimeType {"Single", "HyperSlab", "List", "Range"};
        std::array<std::string, formatTypeLength> FormatType {"XML", "HDF", "Binary"};
        std::array<std::string, numberTypeLength> NumberType {"Float", "Int", "UInt", "Char", "UChar"};
        std::array<std::string, precisionTypeLength> PrecisionType {"1", "2", "4", "8"};

        template<int N>
        bool checkIsValidType(const std::array<std::string, N>& valid_types, const std::string& specified_type) {
            bool is_valid = false;

            for(const auto& t : valid_types) {
                if (t == specified_type) {
                    is_valid = true;
                }
            }

            return is_valid;
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

        void beginDomain() {
            beginElement();
            content += "<Domain>" + newLine;
        };

        void endDomain() {
            insertIndent();
            content += "</Domain>" + newLine;
            endElement();
        };

        void beginGrid(const std::string& type = "Uniform") {
            beginElement();

            if (checkIsValidType<gridTypeLength>(GridType, type)) {
                content += "<Grid GridType=\"" + type + "\">" + newLine;
            } else {
                std::string error_message = "Invalid Grid type = " + type + " is passed to beginGrid().";
                throw std::invalid_argument(error_message);
            }
        }

        void endGrid() {
            insertIndent();
            content += "</Grid>" + newLine;
            endElement();
        }

        void beginTopology(const std::string& type = "2DCoRectMesh") {
            beginElement();

            if (checkIsValidType<topologyTypeLength>(TopologyType, type)) {
                content += "<Topology TopologyType=\"" + type + "\">" + newLine;
            } else {
                std::string error_message = "Invalid Topology type = " + type + " is passed to beginTopology().";
                throw std::invalid_argument(error_message);
            }
        }

        void endTopology() {
            insertIndent();
            content += "</Topology>" + newLine;
            endElement();
        }

        void beginGeometory(const std::string& type = "XYZ"){
            beginElement();

            if (checkIsValidType<geometryTypeLength>(GeometryType, type)) {
                content += "<Geometry GeometryType=\"" + type + "\">" + newLine;
            } else {
                std::string error_message = "Invalid Geometry type = " + type + " is passed to beginGeometry().";
                throw std::invalid_argument(error_message);
            }
        }

        void endGeometory(){
            insertIndent();
            content += "</Geometry>" + newLine;
            endElement();
        }

        void beginDataItem(const std::string& type = "Uniform") {
            beginElement();

            if (checkIsValidType<dataItemTypeLength>(DataItemType, type)) {
                content += "<DataItem ItemType=\"" + type + "\">" + newLine;
            } else {
                std::string error_message = "Invalid DataItem type = " + type + " is passed to beginDataItem().";
                throw std::invalid_argument(error_message);
            }
        }

        void endDataItem() {
            insertIndent();
            content += "</DataItem>" + newLine;
            endElement();
        }
};

#endif