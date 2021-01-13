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

    node[100] = 10000;
    node["101"] = 10000;
    node["101"] = 10002;                    // overwrite
//  node["101"].push_back(10002);           // crash
    node["group 102"] = 10003;
    node["group 102"] = "abcde";            // overwrite
    node["group 103"].push_back(200);       // sequence
    node["group 103"].push_back(201);
    node["group 103"].push_back(202);
    node["group 103"].push_back("abc");
    node["group 103"].push_back("def");
    node["group 103"].push_back(node[101]); // reference

    YAML::Node n0,n1,n2,n3,n4;
    n0["stock"] = "HSI";  n0["price"] = 10.123;  n0["other"] = "!@#$%";
    n1["stock"] = "BOC";  n1["price"] = 30.987;  n1["other"] = "ABCDE";
    n2["stock"] = "HSB";  n2["price"] = 56.780;  n2["other"] = "UVWXY";
    n3["stock"] = "XYZ";  n3["price"] = 67.890;  n3["other"] = "-----";
    n4["stock"] = "xyz";  n4["price"] = 5555;    n4["other"] = "STOCK";
    node["vector"].push_back(n0);
    node["vector"].push_back(n1);
    node["vector"].push_back(n2);

    YAML::Node nn;
    nn["layer0"] = n0;                     // build on reference
    nn["layer1"] = n1;
    nn["layer2"] = n2;
    nn["layer3"] = n3;                     // build on existing node
    nn["layerX"]["abc"] = 456;             // build on temporary node
    nn["layerX"]["dd0"] = 455;
    nn["layerX"]["dd1"] = 457;
    nn["layerX"]["dd2"]["x0"] = "xxx-";
    nn["layerX"]["dd2"]["x1"] = "xx--";
    nn["layerX"]["dd2"]["x2"] = "x---";
    nn["layerX"]["dd2"]["x3"] = "----";
    nn["layerX"]["dd2"]["x4"] = "....";
    nn["layerX"]["dd2"]["x5"] = n4;
    node["tree"] = nn;

    std::ofstream ofs("config.yaml");
    ofs << node;
    std::cout << "\n\n";
}

void test_yaml1()
{
    std::cout << "\nLoading config ...";

    YAML::Node node = YAML::LoadFile("config.yaml");
    if (node[100])
    {
        std::uint32_t n = node[100].as<decltype(n)>();
        std::string   s = node[100].as<decltype(s)>();
        std::cout << "\n" << n << " " << s;
    }
    std::cout << "\n\n";
}

