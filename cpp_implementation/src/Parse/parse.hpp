#pragma once

#include <string>
#include "graph.hpp"

const graph& load(const std::string& filepath);

const graph& parse_edge_list(const std::string& filepath);

const graph& parse_vertex_neighbours(const std::string& filepath);
