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
        std::string buffer;
        std::string version;
        std::string newLine;
        std::string indent;

        // store current processing tag information for type validation
        enum class TAG {DataItem, Grid, Topology, Geometry, Attribute, Set, Time, Information, Domain};
        TAG current_tag;

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
                buffer += indent;
            }
        }

        void setCurrentTag(const std::string& tag) {
            if (tag == "Grid") {
                current_tag = TAG::Grid;
            } else if (tag == "DataItem") {
                current_tag = TAG::DataItem;
            } else if (tag == "Topology") {
                current_tag = TAG::Topology;
            } else if (tag == "Geometry") {
                current_tag = TAG::Geometry;
            } else if (tag == "Attribute") {
                current_tag = TAG::Attribute;
            } else if (tag == "Set") {
                current_tag = TAG::Set;
            } else if (tag == "Time") {
                current_tag = TAG::Time;
            } else if (tag == "Domain") {
                current_tag = TAG::Domain;
            } else if (tag == "Information") {
                current_tag = TAG::Information;
            }
        }

        void beginElement(const std::string& tag) {
            if (buffer != "") {
                commitBuffer();
            }

            addIndent();
            insertIndent();
            buffer += "<" + tag;

            setCurrentTag(tag);
        }

        void endElement(const std::string& tag) {
            if (buffer != "") {
                commitBuffer();
            }

            insertIndent();
            buffer += "</" + tag;
            commitBuffer();

            backIndent();
        }

        void commitBuffer() {
            content += buffer + ">" + newLine;
            buffer.clear();
        }

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

        bool checkType(const std::string& type) {
            bool isValid = false;
            switch (current_tag) {
                case TAG::Grid:
                    isValid = checkIsValidType<gridTypeLength>(GridType, type);
                    break;
                case TAG::DataItem:
                    isValid = checkIsValidType<dataItemTypeLength>(DataItemType, type);
                    break;
                case TAG::Topology:
                    isValid = checkIsValidType<topologyTypeLength>(TopologyType, type);
                    break;
                case TAG::Geometry:
                    isValid = checkIsValidType<geometryTypeLength>(GeometryType, type);
                    break;
                case TAG::Attribute:
                    isValid = checkIsValidType<attributeTypeLength>(AttributeType, type);
                    break;
                case TAG::Set:
                    isValid = checkIsValidType<setTypeLength>(SetType, type);
                    break;
                case TAG::Time:
                    isValid = checkIsValidType<timeTypeLength>(TimeType, type);
                    break;
                case TAG::Information:
                    isValid = true;
                    break;
                case TAG::Domain:
                    isValid = true;
                    break;
                default:
                    break;
            }

            if (!isValid) {
                std::string tagString = getTagString();

                std::string error_message = "Invalid " + tagString + " type = " + type + " is passed to.";
                throw std::invalid_argument(error_message);
            }

            return isValid;
        }

        std::string getTagString() {
            switch (current_tag) {
                case TAG::Grid:
                    return "Grid";
                case TAG::DataItem:
                    return "DataItem";
                case TAG::Topology:
                    return "Topology";
                case TAG::Geometry:
                    return "Geometry";
                case TAG::Attribute:
                    return "Attribute";
                case TAG::Set:
                    return "Set";
                case TAG::Time:
                    return "Time";
                case TAG::Information:
                    return "Information";
                case TAG::Domain:
                    return "Domain";
                default:
                    return "";
            }
        }

        // Adding Valid Attributes
        void addType(const std::string& type) {
            if (checkType(type)) {
                buffer += " " + getTagString() + "Type=\"" + type + "\"";
            }
        }

    public:
        SimpleXdmf(const std::string& _version = "3.0") {
            version = _version;
            setNewLineCode();
            setIndentSpaceSize();
            beginXdmf();
        }

        void setIndentSpaceSize(const int size = 4) {
            if (size == 0) {
                indent = '\t';
                return;
            }

            indent.clear();
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
            beginElement("Domain");
        };

        void endDomain() {
            endElement("Domain");
        };

        void beginGrid(const std::string& type = "Uniform") {
            beginElement("Grid");
            addType(type);
        }

        void endGrid() {
            endElement("Grid");
        }

        void beginTopology(const std::string& type = "2DCoRectMesh") {
            beginElement("Topology");
            addType(type);
        }

        void endTopology() {
            endElement("Topology");
        }

        void beginGeometory(const std::string& type = "XYZ"){
            beginElement("Geometry");
            addType(type);
        }

        void endGeometory(){
            endElement("Geomerry");
        }

        void beginDataItem(const std::string& type = "Uniform") {
            beginElement("DataItem");
            addType(type);
        }

        void endDataItem() {
            endElement("DataItem");
        }

        void setName(const std::string& name) {
            buffer += " Name=\"" + name + "\"";
        }

        void setFormat() {
            buffer += " Format=\"XML\"";
        }

        void setDimensions() {
            buffer += " Dimensions=\"1 1 2\"";
        }

        void setNumberOfElements() {
            buffer += " NumberOfElements=\"1 1 2\"";
        }
};

#endif