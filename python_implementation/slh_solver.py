#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Dec 27 23:19:21 2020

@author: manu
"""

from datetime import datetime
import progressbar

import slh_parser
import slh_drawer
import slh_util
import slh_stage

class solver(object):
    instance = None
    
    def __init__(self,file_path):
        solver.instance = self
        self.adjacency_matrix = slh_parser.load(file_path)
        self.drawer = slh_drawer.slh_drawer(self)
        slh_util.ordering.size = len(self.adjacency_matrix)
        self.ordering_stack = [slh_util.ordering()]
        self.gap_set = set()
        self.n_gaps = len(self.adjacency_matrix)
        
        self.elapsed = "-"
        self.stage = "-"
        
    def clear(self):
        self.ordering_stack = [self.ordering_stack[-1]]
        self.gap_set = set()
        
    def show(self,background_color=None,layer=None):
        self.drawer.draw(self.ordering_stack[-1]._verts,background_color,layer)
        self.drawer.show()
        
    def write(self,path):
        with open(path,"w") as f:
            f.writelines([
                "Stage: {}\nElapsed Time: {}s\n".format(self.stage,self.elapsed),
                " ".join([str(v+1) for v in self.ordering_stack[-1]._verts])+"\n",
            ])
        
    def solve(self):
        
        widgets = [
            progressbar.SimpleProgress(format='Snakes: %(value_s)s/%(max_value_s)s '),
            progressbar.Percentage(),
            progressbar.Bar(),
            progressbar.Variable("stage",format="Stage {formatted_value}"),
            progressbar.Timer()
        ]
        self.bar = progressbar.ProgressBar(
            widgets=widgets,
            max_value=len(self.adjacency_matrix)
        ).start()
        
        start = datetime.now()
        
        self.stage = "0"
        self.bar.update(stage="0")
        slh_stage.stage0()
        
        if self.n_gaps>0:
            self.stage = "1"
            self.bar.update(stage="1")
            slh_stage.stage1()
        
        if self.n_gaps!=0:
            self.stage = "2"
            self.bar.update(stage="2")
            
        while self.n_gaps>0 and slh_stage.stage2(): slh_stage.stage1()
        
        if self.n_gaps!=0:
            self.stage = "3"
            self.bar.update(stage="3")
            
        while self.n_gaps>0 and slh_stage.stage3():
            slh_stage.stage1()
            while self.n_gaps>0 and slh_stage.stage2(): slh_stage.stage1()
        
        self.elapsed = str((datetime.now()-start).total_seconds())
        self.bar.finish()
        
        return self.n_gaps==0