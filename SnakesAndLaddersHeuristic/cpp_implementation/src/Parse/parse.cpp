#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "parse.hpp"

const graph& load(const std::string& filepath) {
    std::ifstream input_file_stream(filepath);
    std::string line;
    std::getline(input_file_stream,line);

    if (line=="Graph") return parse_vertex_neighbours(filepath);
    return parse_edge_list(filepath);
}

const graph& parse_edge_list(const std::string& filepath) {
    std::ifstream input_file_stream(filepath);
    std::string line;

    unsigned short n;
    
    while (std::getline(input_file_stream,line)) {
        std::istringstream line_stream(line);
        std::string attribute;
        
        std::getline(line_stream,attribute,':');
        attribute.erase(std::remove(attribute.begin(),attribute.end(),' '),attribute.end());
	
        if (attribute=="DIMENSION") {
            line_stream >> n;
            break;
        }
    }

    graph& G = *(new graph(n));

    while (std::getline(input_file_stream,line)) {
        if (line.back()==13) line.pop_back();
        if (line=="EDGE_DATA_SECTION") break;
    }
    
    while (std::getline(input_file_stream,line) and line.substr(0,2)!="-1") {
        std::istringstream line_stream(line);
        unsigned short v0, v1;
        line_stream >> v0 >> v1;
        G.connect(v0-1,v1-1);
    }

    return G;
}

const graph& parse_vertex_neighbours(const std::string& filepath) {
    std::ifstream input_file_stream(filepath);
    std::string line;
    unsigned short n = std::stoi(filepath.substr(1,filepath.find('.')-1));
    graph& G = *(new graph(n));

    for (int i = 0; i<3; ++i) std::getline(input_file_stream,line);

    while (std::getline(input_file_stream,line) and line[0]!='\n') {
        std::istringstream line_stream(line);
        unsigned short v, n0, n1, n2;
        line_stream >> v >> n0 >> n1 >> n2;
        G.connect(v-1,n0-1);
        G.connect(v-1,n1-1);
        G.connect(v-1,n2-1);
    }

    return G;
}
