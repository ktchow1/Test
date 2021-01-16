#include<iostream>
#include<fstream>
#include<string>
#include<yaml-cpp/yaml.h>

// ************************************************ //
// This is a test implemented according to :
// https://github.com/jbeder/yaml-cpp/wiki/Tutorial
// ************************************************ //
void test_yaml0()
{
    // Build yaml-tree
    // 1. both key and value can be either int or string
    // 2. both int and string will be print as string in yaml
    YAML::Node node;

    // (1) scalar
    node[100] = 10000;
    node["101"] = 10000;
    node["101"] = 10001;                    // overwrite
    node["scalar"] = 10002;
    node["scalar"] = "abc";             // overwrite
//  node["group 102"].push_back(10002);     // crash

    // (2) sequence (i.e. heterogenous vector)
    node["seq"].push_back(10);   
    node["seq"].push_back(11);
    node["seq"].push_back(12);
    node["seq"].push_back("abc");
    node["seq"].push_back("ABC");
    node["seq"].push_back(node[101]);   // reference

    // (3a) map (i.e. heterogenous subtree) method1
    node["map"][123]   = 12345;
    node["map"]["abc"] = "abcdef";
    node["map"]["seq"].push_back(10);
    node["map"]["seq"].push_back(11);
    node["map"]["seq"].push_back(12);
    node["map"]["map"]["key0"] = "value0";
    node["map"]["map"]["key1"] = "value1";
    node["map"]["map"]["key2"] = "value2";
    node["map"]["map"]["key3"] = "value3";
    node["map"]["map"]["key4"] = "value4";
    node["map"]["map"]["key5"] = node[101];  // reference again

    // (3b) map (i.e. heterogenous subtree) method2
    YAML::Node n0,n1,n2,n3,n4;
    n0["stock"] = "HSI";  n0["price"] = 10.123;  n0["other"] = "*****";
    n1["stock"] = "BOC";  n1["price"] = 20.234;  n1["other"] = ".....";
    n2["stock"] = "HSB";  n2["price"] = 30.345;  n2["other"] = "/////";
    n3["stock"] = "XYZ";  n3["price"] = 40.456;  n3["other"] = "-----";
    n4["stock"] = "TUV";  n4["price"] = 50.567;  n4["other"] = "#####";
    node["subtree"].push_back(n0);
    node["subtree"].push_back(n1);
    node["subtree"].push_back(n2);
    node["subtree"]["extra0"] = n3; 
    node["subtree"]["extra1"] = n4;
    node["subtree"]["extra2"] = node[101];


    std::cout << "\nWrite yaml-tree into file ...";
    std::ofstream ofs("config.yaml");
    ofs << node;
}

template<typename T>
void access(const YAML::Node& node, const T& key) // can be integer or std::string
{
    // Possible node.Type(): Null / Scalar / Sequence / Map / Undefined
    if (!node[key]) 
    {
        std::cout << "\nnode[" << key << "] is null";
    }
    else if (node[key].Type() == YAML::NodeType::Scalar)
    {
        std::cout << "\nnode[" << key << "] is scalar : " << node[key];
    }
    else if (node[key].Type() == YAML::NodeType::Sequence)
    {
        std::cout << "\nnode[" << key << "] is sequence : ";
        if (node[key][0].Type() != YAML::NodeType::Map)
        {
            for(std::uint32_t n=0; n!=node[key].size(); ++n) std::cout << node[key][n] << " ";
        }
        else
        {
            for(std::uint32_t n=0; n!=node[key].size(); ++n) std::cout << "\n" << node[key][n];
        }
    } 
    else if (node[key].Type() == YAML::NodeType::Map)
    {
        std::cout << "\nnode[" << key << "] is map : ";
        std::cout << "\n" << node[key];
    }
}

struct element
{
    std::uint32_t n;
    std::string s;
    std::vector<std::uint32_t> v;
    std::map<std::string, std::string> m;
};

std::ostream& operator<<(std::ostream& os, const element& x)
{
    os << "\n***************";
    os << "\n*** element ***";
    os << "\n***************";
    os << "\nn = " << x.n;
    os << "\ns = " << x.s;
    os << "\nv = "; for(const auto& y:x.v) os << y << " ";
    os << "\nm = "; for(const auto& y:x.m) os << y.first << ":" << y.second << " ";
    os << "\n***************";
    return os;
}

void parse_into(const YAML::Node& node, element& x)
{
    x.n = node[ 123 ].as<decltype(element::n)>();
    x.s = node["abc"].as<decltype(element::s)>();
    for(std::uint32_t n=0; n!=node["seq"].size(); ++n)
    {
        x.v.push_back(node["seq"][n].as<typename decltype(element::v)::value_type>());
    }
    x.m["key0"] = node["map"]["key0"].as<typename decltype(element::m)::mapped_type>();
    x.m["key1"] = node["map"]["key1"].as<typename decltype(element::m)::mapped_type>();
    x.m["key2"] = node["map"]["key2"].as<typename decltype(element::m)::mapped_type>();
    x.m["key3"] = node["map"]["key3"].as<typename decltype(element::m)::mapped_type>();
    x.m["key4"] = node["map"]["key4"].as<typename decltype(element::m)::mapped_type>();
    x.m["key5"] = node["map"]["key5"].as<typename decltype(element::m)::mapped_type>();
}

void test_yaml1()
{
    std::cout << "\nRead from file into yaml-tree ...";
    YAML::Node node = YAML::LoadFile("config.yaml");

    std::cout << "\nTraverse yaml-tree and print ...";
    std::cout << "\n";
    access(node, 100); 
    access(node, 101); // it can be accessed with integer-key
    access(node, 102); 
    access(node, "scalar"); 
    access(node, "seq"); 
    access(node, "map"); 
    access(node, "subtree"); 
    std::cout << "\n*** using a node as sequence and then as a map will turn it into a map ***";
    std::cout << "\n";

    std::cout << "\nAccess yaml-tree and init my own config-struct ... (we like to use decltype here)";
    element x;
    parse_into(node["map"], x);
    std::cout << "\n" << x;

    // std::uint32_t n = node[100].as<decltype(n)>();
    std::cout << "\n\n";
}

