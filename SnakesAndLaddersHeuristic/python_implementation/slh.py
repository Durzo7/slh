#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Sep  2 17:10:19 2021

@author: manu
"""

import argparse
import slh_solver


if __name__ == "__main__":

    argparser = argparse.ArgumentParser(description='"Snakes and Ladders" Heristic for the Hamiltonian Cycle Problem')
    argparser.add_argument("seed", help="random seed (0 -> normal mode)", type=int)
    argparser.add_argument("filepath", help='".hcp" graph filepath')
    args = argparser.parse_args()
    
    solver = slh_solver.solver(args.filepath)
    if args.seed: solver.ordering_stack[0].shuffle(args.seed)
    solver.solve()
    solver.write(args.filepath.split("/")[-1]+".sol")
    