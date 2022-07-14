#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Feb 26 15:02:39 2021

@author: manu
"""

import slh_solver
import slh_util
import slh_transform
import sortedcontainers


def stage0():
    solver = slh_solver.solver.instance

    period = len(solver.adjacency_matrix)*9//200
    count = 0

    while slh_transform.closing_transformation(solver.ordering_stack[-1]):
        if count == period:
            solver.conn.send((solver.ordering_stack[-1],solver.status_values[1]))
            solver.status_values[4] += 1
            count = 0

        else: count += 1

    solver.n_gaps = solver.ordering_stack[-1].n_gaps()
    solver.status_values[3] = solver.n_gaps


def stage1():
    solver = slh_solver.solver.instance
    aux_ordering_stack = solver.ordering_stack[:]
    n_gaps = solver.n_gaps

    period = solver.n_gaps//20
    count = 0

    while len(solver.ordering_stack)<slh_util.ordering.size**2 and aux_ordering_stack:
        opt,n_ordr = slh_transform.floating_flo_transformation(aux_ordering_stack[-1])

        if n_ordr:
            solver.ordering_stack.append(n_ordr)

            if opt:
                n_gaps -= opt

                if solver.status_values[1] == 1:
                    if count==period or n_gaps==0:
                        solver.conn.send((n_ordr,solver.status_values[1]))
                        solver.status_values[4] += 1
                        solver.status_values[3] = n_gaps
                        count = 0

                    else: count += 1

                if n_gaps==0:
                    solver.n_gaps = 0
                    return

                solver.clear()
                aux_ordering_stack = []

            aux_ordering_stack.append(n_ordr)

        else: aux_ordering_stack.pop()

    solver.n_gaps = n_gaps


def stage2():
    solver = slh_solver.solver.instance
    ordr = solver.ordering_stack.pop()
    n_gaps = solver.n_gaps
    gap = ordr.next_gap()
    visited = sortedcontainers.SortedList()
    opt,n_ordr = slh_transform.opening_4flo(ordr,gap,visited)

    while n_ordr:
        solver.ordering_stack.append(n_ordr)
        solver.clear()
        solver.gap_set.add(gap)
        solver.n_gaps = n_gaps-opt

        stage1()

        if n_gaps>solver.n_gaps:
            solver.conn.send((solver.ordering_stack[-1],solver.status_values[1]))
            solver.status_values[4] += 1
            solver.status_values[3] = solver.n_gaps
            if solver.n_gaps==0: return False
            solver.clear()
            return True

        visited.add(n_ordr)
        opt,n_ordr = slh_transform.opening_4flo(ordr,gap,visited)

    solver.ordering_stack.append(ordr)
    solver.n_gaps = n_gaps

    return False


def stage3():
    solver = slh_solver.solver.instance
    visited = sortedcontainers.SortedList(solver.ordering_stack)

    while len(solver.ordering_stack)<=slh_util.ordering.size**3:
        opt,n_ordr = slh_transform.opening_transformation(solver.ordering_stack[-1],visited)
        if not n_ordr: break

        solver.ordering_stack.append(n_ordr)
        visited.add(n_ordr)
        aux_ordr_pair_stack = [(n_ordr.n_gaps(),n_ordr)]

        while len(solver.ordering_stack)<=slh_util.ordering.size**3 and aux_ordr_pair_stack:
            opt,n_ordr = slh_transform.floating_opt_transformation(aux_ordr_pair_stack[-1][1],visited)

            if n_ordr:
                solver.ordering_stack.append(n_ordr)
                visited.add(n_ordr)
                aux_ordr_pair_stack.append((aux_ordr_pair_stack[-1][0]-opt,n_ordr))

            elif aux_ordr_pair_stack[-1][0]<solver.n_gaps:
                solver.conn.send((aux_ordr_pair_stack[-1][1],solver.status_values[1]))
                solver.status_values[4] += 1
                solver.n_gaps = aux_ordr_pair_stack[-1][0]
                solver.status_values[3] = solver.n_gaps
                if aux_ordr_pair_stack[-1][0]==0: return False
                solver.clear()
                return True

            else: aux_ordr_pair_stack.pop()

    return False
