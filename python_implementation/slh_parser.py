#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Dec 28 10:30:01 2020

@author: manu
"""

import slh_util
import re


def load(file_path):
    re_format1 = re.compile(r"EDGE_DATA_FORMAT : (?P<format>[A-Z_]+)")
    re_format2 = re.compile(r"Vertex  Neighbours")
    file_format = ""
    
    with open(file_path,'r') as file_object:
        lines = file_object.readlines()
        
        if re.match(re_format1, lines[4]): 
            file_format = re.match(re_format1,lines[4]).group("format")
        elif re.match(re_format2, lines[1]): 
            file_format = "VERTEX_NEIGHBOURS"
        
    if file_format:
        if file_format == "EDGE_LIST": 
            return parse_edge_list(file_path)
        elif file_format == "VERTEX_NEIGHBOURS": 
            return parse_vertex_neighbours(file_path)

            
def parse_edge_list(file_path):
    re_dim = re.compile(r"DIMENSION : (?P<dimension>\d+)")
    re_edge = re.compile(r"(?P<vertex1>\d+)\s+(?P<vertex2>\d+)")
    
    with open(file_path,'r') as file_object:
        lines = file_object.readlines()
        
        if (match_dim:=re.match(re_dim,lines[3])):
            dim = int(match_dim.group("dimension"))
            adjacency_matrix = slh_util.adjacency_matrix(dim)
            
            for line in lines[6:-2]:
                if (edge:=re.search(re_edge,line)):
                    adjacency_matrix.connect(int(edge.group("vertex1"))-1,int(edge.group("vertex2"))-1)
    
    adjacency_matrix.sort()
    return adjacency_matrix
                    
        
def parse_vertex_neighbours(file_path):
    re_dim = re.compile(r"(?P<dimension>\d+)")
    re_vertex = re.compile(r"(?P<vertex>\d+)\s+(?P<neighbours>(\d+\s)+)")
    re_neigh = re.compile(r"(\d+)")
    
    with open(file_path,'r') as file_object:
        lines = file_object.readlines()
        
        if (match_dim:=re.match(re_dim,lines[-2])):
            dim = int(match_dim.group("dimension"))
            adjacency_matrix = slh_util.adjacency_matrix(dim)
            
            for line in lines[3:-1]:
                if (vertex:=re.search(re_vertex,line)):
                    v = int(vertex.group("vertex"))
                    
                    for n in re.findall(re_neigh, vertex.group("neighbours")):
                        adjacency_matrix.connect(v-1,int(n)-1)
    
    adjacency_matrix.sort()
    return adjacency_matrix
        
                