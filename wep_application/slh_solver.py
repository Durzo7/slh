#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Dec 27 23:19:21 2020

@author: manu
"""

import multiprocessing
from datetime import datetime

import slh_parser
import slh_drawer
import slh_util
import slh_stage

class solver(object):
    instance = None

    def __init__(self,file_path,status_values,shuffle=False):
        self.status_values = status_values
        self.status_values[0] = 1;

        solver.instance = self
        self.adjacency_matrix = slh_parser.load(file_path)
        self.drawer = slh_drawer.slh_drawer(self)
        slh_util.ordering.size = len(self.adjacency_matrix)
        self.ordering_stack = [slh_util.ordering()]
        self.gap_set = set()
        self.n_gaps = len(self.adjacency_matrix)

        self.status_values[2] = len(self.adjacency_matrix)
        self.status_values[3] = len(self.adjacency_matrix)

    def clear(self):
        self.ordering_stack = [self.ordering_stack[-1]]
        self.gap_set = set()

    def solve(self):
        conn_recv, self.conn = multiprocessing.Pipe(duplex=False)
        drawing_process = multiprocessing.Process(target=self.draw,args=(conn_recv,))
        drawing_process.start()
        self.status_values[5] = 0
        self.status_values[4] = 1
        self.conn.send((self.ordering_stack[-1],-1))

        self.status_values[0] = 2

        start = datetime.now()

        self.status_values[1] = 0
        start = datetime.now()
        slh_stage.stage0()

        self.status_values[1] = 1
        if self.n_gaps>0: slh_stage.stage1()

        self.status_values[1] = 2
        while self.n_gaps>0 and slh_stage.stage2(): slh_stage.stage1()

        self.status_values[1] = 3
        while self.n_gaps>0 and slh_stage.stage3():
            slh_stage.stage1()
            while self.n_gaps>0 and slh_stage.stage2(): slh_stage.stage1()

        elapsed = int((datetime.now()-start).total_seconds())

        self.status_values[6] = elapsed

        with open("./static/graph/solution.tou","w") as file:
            file.write("Elapsed time: {} s\n".format(str(elapsed)))
            for v in self.ordering_stack[-1]: file.write(str(v+1)+" ")

        self.conn.send((None,-1))
        self.status_values[0] = 3
        drawing_process.join()

        self.status_values[0] = 4
        return self.n_gaps==0

    def draw(self,conn):
        img_id = 1
        ordr,stage = conn.recv()

        while (ordr):
            self.drawer.draw(ordr._verts,stage=stage)
            self.drawer.save("./static/img/{}.png".format(img_id))
            self.status_values[5] += 1
            img_id += 1
            ordr,stage = conn.recv()
