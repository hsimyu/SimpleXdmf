#ifndef SIMPLE_XDMF_HPP_INCLUDED
#define SIMPLE_XDMF_HPP_INCLUDED

#include <iostream>
#include <array>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>

class SimpleXdmf {
    private:
        const std::string header = R"(<?xml version="1.0" ?>
<!DOCTYPE Xdmf SYSTEM "Xdmf.dtd" []>
)";
        std::string content;
        std::string buffer;
        std::string version;
        std::string newLine;
        bool endEdit = false;
        unsigned int innerElementPerLine = 10;


        // indent management
        std::string indent;
        unsigned int currentIndentation = 0;
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


        // store current processing tag information for type validation
        enum class TAG {DataItem, Grid, Topology, Geometry, Attribute, Set, Time, Information, Domain, Inner};
        TAG current_tag;

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
            } else if (tag == "Inner") {
                current_tag = TAG::Inner;
            }
        }


        // Reference Management
        // xpathMap[Name] => Xpath
        std::map<std::string, std::string> xpathMap;

        void addNewXpath(const std::string& name, const std::string& xpath) {
            if (xpathMap.count(name) > 0) {
                std::cerr << "[ERROR] Duplicate Name attribute \"" << name << "\" may cause incorrect Xpath reference. " << std::endl;
            } else {
                xpathMap[name] = xpath;
            }
        }

        std::string getXpath(const std::string& name) {
            if (xpathMap.count(name) > 0) {
                return xpathMap[name];
            } else {
                std::cerr << "[ERROR] Non-existente Name \"" << name << "\" passed to getXpath(). " << std::endl;
                return "";
            }
        }


        // Structure Management
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

        void endOneLineElement(const std::string& tag) {
            if (buffer != "") {
                commitOneLineBuffer();
            }
            backIndent();
        }

        void commitOneLineBuffer() {
            content += buffer + "/>" + newLine;
            buffer.clear();
        }

        void beginInnerElement() {
            if (buffer != "") {
                commitBuffer();
            }
            addIndent();
            insertIndent();

            setCurrentTag("Inner");
        }

        void endInnerElement() {
            if (buffer != "") {
                commitInnerBuffer();
            }
            backIndent();
        }

        void commitInnerBuffer() {
            content += buffer;
            buffer.clear();
        }


        // Type checking
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
                case TAG::Inner:
                    return "Inner";
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

        // for convinience
        void convertFromVariadicArgsToStringInternal(const std::stringstream& ss) {}

        template<typename First, typename... Rests>
        void convertFromVariadicArgsToStringInternal(std::stringstream& ss, First&& first, Rests&&... rest) {
            ss << first;

            constexpr std::size_t parameter_pack_size = sizeof...(Rests);
            if (parameter_pack_size > 0) {
                ss << " ";
                convertFromVariadicArgsToStringInternal(ss, std::forward<Rests>(rest)...);
            }
        }

        template<typename... Args>
        std::string convertFromVariadicArgsToString(Args&&... args) {
            std::stringstream ss;
            convertFromVariadicArgsToStringInternal(ss, std::forward<Args>(args)...);
            return ss.str();
        }

    public:
        SimpleXdmf(const std::string& _version = "3.0") {
            version = _version;
            setNewLineCodeCRLF();
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

        void setNewLineCodeLF() {
            constexpr char LF = '\n';
            newLine = LF;
        }

        void setNewLineCodeCR() {
            constexpr char CR = '\r';
            newLine = CR;
        }

        void setNewLineCodeCRLF() {
            constexpr const char* CRLF = "\r\n";
            newLine = CRLF;
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

        void beginStructuredTopology(const std::string& type = "2DCoRectMesh") {
            beginElement("Topology");
            addType(type);
        }

        void endStructuredTopology() {
            endOneLineElement("Topology");
        }

        void beginGeometory(const std::string& type = "XYZ"){
            beginElement("Geometry");
            addType(type);
        }

        void endGeometory(){
            endElement("Geometry");
        }

        void beginAttribute(const std::string& type = "Scalar"){
            beginElement("Attribute");
            addType(type);
        }

        void endAttribute(){
            endElement("Attribute");
        }

        void beginDataItem(const std::string& type = "Uniform") {
            beginElement("DataItem");
            addType(type);
        }

        void endDataItem() {
            endElement("DataItem");
        }

        void beginSet(const std::string& type = "Node") {
            beginElement("Set");
            addType(type);
        }

        void endSet() {
            endElement("Set");
        }

        void beginTime(const std::string& type = "Single") {
            beginElement("Time");
            addType(type);
        }

        void endTime() {
            endElement("Time");
        }

        void beginInformation() {
            beginElement("Information");
        }

        void endInformation() {
            endElement("Information");
        }

        template<typename T>
        void addVector(const std::vector<T>& values) {
            beginInnerElement();

            std::stringstream ss;
            auto size = values.size();
            for(size_t i = 1; i <= size; ++i) {
                ss << values[i - 1];

                if (i < size) {
                    ss << " ";

                    if (i % innerElementPerLine == 0) {
                        buffer += ss.str() + newLine;
                        insertIndent();
                        ss.str("");
                        ss.clear(std::stringstream::goodbit);
                    }
                }
            }

            if (ss.str() != "") {
                buffer += ss.str() + newLine;
            }

            endInnerElement();
        }

        void addItemInternal(std::stringstream& ss) {}

        template<typename First, typename... Rests>
        void addItemInternal(std::stringstream& ss, First&& first, Rests&&... rests) {
            ss << first << " ";
            addItemInternal(ss, rests...);
        }

        template<typename... Args>
        void addItem(Args&&... args) {
            beginInnerElement();

            std::stringstream ss;
            addItemInternal(ss, std::forward<Args>(args)...);
            buffer += ss.str() + newLine;

            endInnerElement();
        }

        // --- Attirbute Setting Functions ---
        void setName(const std::string& name) {
            addNewXpath(name, "test");
            buffer += " Name=\"" + name + "\"";
        }

        void setFormat(const std::string& type = "XML") {
            if (checkIsValidType<formatTypeLength>(FormatType, type)) {
                buffer += " Format=\"" + type + "\"";
            } else {
                std::string error_message = "Invalid Format type = " + type + " is passed to setFormat().";
                throw std::invalid_argument(error_message);
            }
        }

        void setPrecision(const std::string& type = "4") {
            if (checkIsValidType<precisionTypeLength>(PrecisionType, type)) {
                buffer += " Precision=\"" + type + "\"";
            } else {
                std::string error_message = "Invalid Precision type = " + type + " is passed to setPrecision().";
                throw std::invalid_argument(error_message);
            }
        }

        void setNumberType(const std::string& type = "Float") {
            if (checkIsValidType<numberTypeLength>(NumberType, type)) {
                buffer += " NumberType=\"" + type + "\"";
            } else {
                std::string error_message = "Invalid Number type = " + type + " is passed to setNumberType().";
                throw std::invalid_argument(error_message);
            }
        }

        void setCenter(const std::string& type = "Node") {
            if (current_tag != TAG::Attribute) {
                std::cerr << "[ERROR] setCenter() cannot be called when current Tag is not Attribute." << std::endl;
                return;
            }

            if (checkIsValidType<attributeCenterLength>(AttributeCenter, type)) {
                buffer += " Center=\"" + type + "\"";
            } else {
                std::string error_message = "Invalid Center type = " + type + " is passed to setCenter().";
                throw std::invalid_argument(error_message);
            }
        }

        void setFunction(const std::string& func) {
            if (current_tag != TAG::DataItem) {
                std::cerr << "[ERROR] setFunction() cannot be called when current Tag is not DataItem." << std::endl;
                return;
            }
            buffer += " Function=\"" + func + "\"";
        }

        void setSection(const std::string& sect) {
            if (current_tag != TAG::Grid) {
                std::cerr << "[ERROR] setSection() cannot be called when current Tag is not Grid." << std::endl;
                return;
            }

            if (sect == "DataItem" || sect == "All") {
                buffer += " Selection=\"" + sect + "\"";
            } else {
                std::string error_message = "Invalid Section type = " + sect + " is passed to setSection().";
                throw std::invalid_argument(error_message);
            }
        }

        void setValue(const std::string& value) {
            if (current_tag != TAG::Time && current_tag != TAG::Information) {
                std::cerr << "[ERROR] setValue() cannot be called when current Tag is not Time and Information." << std::endl;
                return;
            }
            buffer += " Value=\"" + value + "\"";
        }

        template<typename... Args>
        void setDimensions(Args&&... args) {
            std::string dimString = convertFromVariadicArgsToString(std::forward<Args>(args)...);
            buffer += " Dimensions=\"" + dimString + "\"";
        }

        template<typename... Args>
        void setNumberOfElements(Args&&... args) {
            std::string dimString = convertFromVariadicArgsToString(std::forward<Args>(args)...);
            buffer += " NumberOfElements=\"" + dimString + "\"";
        }

        // Reference Attribute management
        void setReference(const std::string& xpath) {
            buffer += " Reference=\"" + xpath + "\"";
        }

        void setReferenceFromName(const std::string& name) {
            auto xpath = getXpath(name);
            buffer += " Reference=\"" + xpath + "\"";
        }

        void addReferenceFromName(const std::string& name) {
            auto xpath = getXpath(name);
            addItem(xpath);
        }
};

#endif