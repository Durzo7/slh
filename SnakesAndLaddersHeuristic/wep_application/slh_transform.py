#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Feb 18 12:30:57 2021

@author: manu
"""

import slh_solver
import slh_util


def isomorphism_gamma(ordr,x,b):
    ordr.reverse_segment(x,b)


def isomorphism_kappa(ordr,x,e,c,a,b,f,d,y):
    ordr.swap_segments(b,f,d,y)
    ordr.reverse_segment(x,e)
    ordr.reverse_segment(c,a)
    ordr.reverse_segment(b,f)

        
def closing_2opt_type1(ordr,gap):
    M_adj = slh_solver.solver.instance.adjacency_matrix
    
    # Forward
    x = gap.v1
    
    for a in M_adj.get_neighbours(x):
        b = ordr.prev_of(a)
        
        if x!=b and not M_adj[a,b]:
            isomorphism_gamma(ordr,x,b)
            return True
        
    # Backward
    x = gap.v0
    
    for a in M_adj.get_neighbours(x):
        b = ordr.next_of(a)
        
        if x!=b and not M_adj[a,b]:
            isomorphism_gamma(ordr,b,x)
            return True
        
    return False


def closing_2opt_type2(ordr,gap):
    M_adj = slh_solver.solver.instance.adjacency_matrix
    
    # Symmetric
    x = gap.v1
    y = gap.v0
    
    for a in M_adj.get_neighbours(x):
        b = ordr.prev_of(a)
        
        if x!=b and M_adj[y,b]:
            isomorphism_gamma(ordr,x,b)
            return True
    
    return False


def closing_3opt(ordr,gap):
    M_adj = slh_solver.solver.instance.adjacency_matrix
    
    # Symmetric
    x = gap.v1
    y = gap.v0
    
    for a in M_adj.get_neighbours(x):
        c = ordr.prev_of(a)
        
        if x!=c and ordr.distance(a,y)>=2:
            for b in M_adj.get_neighbours(y):
                if ordr.is_between(b,a,y):
                    d = ordr.next_of(b)
                    
                    if y!=d and M_adj[c,d]:
                        isomorphism_gamma(ordr,x,c)
                        isomorphism_gamma(ordr,d,y)
                        return True
                    
    return False


cls_tfrms = [
    closing_2opt_type1,
    closing_2opt_type2,
    closing_3opt
]

def closing_transformation(ordr):    
    for cls_tfrm in cls_tfrms:
        itr = iter(ordr)
        
        while gap:=ordr.next_gap(itr):
            if cls_tfrm(ordr,gap): return True
            
    return False


def floating_2flo(ordr,gap):
    M_adj = slh_solver.solver.instance.adjacency_matrix
    gap_set = slh_solver.solver.instance.gap_set
    
    # Forward
    x = gap.v1
    y = gap.v0
    
    for a in M_adj.get_neighbours(x):
        b = ordr.prev_of(a)
        
        if x!=b and slh_util.pair(y,b) not in gap_set:
            opt = M_adj[y,b] + (not M_adj[a,b])
            
            n_ordr = slh_util.ordering(ordr)
            isomorphism_gamma(n_ordr,x,b)
            
            return opt,n_ordr
        
    # Backward
    x = gap.v0
    y = gap.v1
    
    for a in M_adj.get_neighbours(x):
        b = ordr.next_of(a)
        
        if x!=b and slh_util.pair(y,b) not in gap_set:
            opt = M_adj[y,b] + (not M_adj[a,b])
            
            n_ordr = slh_util.ordering(ordr)
            isomorphism_gamma(n_ordr,b,x)
            
            return opt,n_ordr
    
    return 0,None
    

def floating_3flo(ordr,gap):
    M_adj = slh_solver.solver.instance.adjacency_matrix
    gap_set = slh_solver.solver.instance.gap_set
    
    # Forward
    x = gap.v1
    y = gap.v0
    
    for a in M_adj.get_neighbours(x):
        c = ordr.prev_of(a)
        
        if x!=c and ordr.distance(a,y)>=2:
            # (y,b) is a ladder, Symmetric
            for b in M_adj.get_neighbours(y):
                if ordr.is_between(b,a,y):
                    d = ordr.next_of(b)
                    
                    if y!=d and slh_util.pair(c,d) not in gap_set:
                        opt = M_adj[c,d] + (not M_adj[a,c]) + (not M_adj[b,d])
                        
                        n_ordr = slh_util.ordering(ordr)
                        isomorphism_gamma(n_ordr,x,c)
                        isomorphism_gamma(n_ordr,d,y)
                        
                        return opt,n_ordr
            
            # (c,d) is a ladder
            for d in M_adj.get_neighbours(c):
                if ordr.is_between(d,a,y):
                    b = ordr.prev_of(d)
                    
                    if a!=b and slh_util.pair(y,b) not in gap_set:
                        opt = M_adj[y,b] + (not M_adj[a,c]) + (not M_adj[b,d])
                        
                        n_ordr = slh_util.ordering(ordr)
                        isomorphism_gamma(n_ordr,x,c)
                        isomorphism_gamma(n_ordr,d,y)
                        
                        return opt,n_ordr
                    
    # Backward
    x = gap.v0
    y = gap.v1
    
    for a in M_adj.get_neighbours(x):
        c = ordr.next_of(a)
        
        if x!=c and ordr.distance(y,a)>=2:
            # (b,y) is a ladder, already tested
            # (c,d) is a ladder
            for d in M_adj.get_neighbours(c):
                if ordr.is_between(d,y,a):
                    b = ordr.next_of(d)
                    
                    if a!=b and slh_util.pair(y,b) not in gap_set:
                        opt = M_adj[y,b] + (not M_adj[a,c]) + (not M_adj[b,d])
                        
                        n_ordr = slh_util.ordering(ordr)
                        isomorphism_gamma(n_ordr,c,x)
                        isomorphism_gamma(n_ordr,y,d)
                        
                        return opt,n_ordr
                    
    return 0,None


def floating_4flo_type1(ordr,gap):
    M_adj = slh_solver.solver.instance.adjacency_matrix
    gap_set = slh_solver.solver.instance.gap_set

    # Symmetric
    x = gap.v1
    y = gap.v0

    for a in M_adj.get_neighbours(x):
        b = ordr.next_of(a)
        
        if M_adj[y,b] and ordr.distance(x,a)>=2 and ordr.distance(b,y)>=2:
            itr = slh_util.ordering_iterator(ordr,ordr.index_of(x))+2
            end = slh_util.ordering_iterator(ordr,ordr.index_of(a))
            
            while itr!=end:
                c = next(itr)
                
                for d in M_adj.get_neighbours(c):
                    if ordr.is_between(d,b,y):
                        e = ordr.prev_of(c)
                        f = ordr.next_of(d)
                        
                        if y!=f and slh_util.pair(e,f) not in gap_set:
                            opt = M_adj[e,f] + (not M_adj[c,e]) + (not M_adj[a,b]) + (not M_adj[d,f])
                            
                            n_ordr = slh_util.ordering(ordr)
                            isomorphism_gamma(n_ordr,x,e)
                            isomorphism_gamma(n_ordr,c,a)
                            isomorphism_gamma(n_ordr,b,d)
                            isomorphism_gamma(n_ordr,f,y)
                            
                            return opt, n_ordr
                        
    return 0,None
                

def floating_4flo_type2(ordr,gap):
    M_adj = slh_solver.solver.instance.adjacency_matrix
    gap_set = slh_solver.solver.instance.gap_set
    
    # Forward
    x = gap.v1
    y = gap.v0
    
    for a in M_adj.get_neighbours(x):
        b = ordr.next_of(a)
        
        if ordr.distance(x,a)>=2 and ordr.distance(b,y)>=2:
            # (b,e) is a ladder
            for e in M_adj.get_neighbours(b):
                if ordr.is_between(e,x,a):
                    c = ordr.next_of(e)
                    
                    if a!=c:
                        for d in M_adj.get_neighbours(c):
                            if ordr.is_between(d,b,y):
                                f = ordr.prev_of(d)
                                
                                if b!=f and slh_util.pair(y,f) not in gap_set:
                                    opt = M_adj[y,f] + (not M_adj[c,e]) + (not M_adj[a,b]) + (not M_adj[d,f])
                                    
                                    n_ordr = slh_util.ordering(ordr)
                                    isomorphism_kappa(n_ordr,x,e,c,a,b,f,d,y)
                                    
                                    return opt,n_ordr
                                
            # (y,f) is a ladder
            for f in M_adj.get_neighbours(y):
                if ordr.is_between(f,b,y):
                    d = ordr.next_of(f)
                    
                    if d!=y:
                        for c in M_adj.get_neighbours(d):
                            if ordr.is_between(c,x,a):
                                e = ordr.prev_of(c)
                                
                                if x!=e and slh_util.pair(b,e) not in gap_set:
                                    opt = M_adj[b,e] + (not M_adj[c,e]) + (not M_adj[a,b]) + (not M_adj[d,f])
                                    
                                    n_ordr = slh_util.ordering(ordr)
                                    isomorphism_kappa(n_ordr,x,e,c,a,b,f,d,y)
                                    
                                    return opt,n_ordr
    
    # Backward
    x = gap.v0
    y = gap.v1
    
    for a in M_adj.get_neighbours(x):
        b = ordr.prev_of(a)
        
        if ordr.distance(a,x)>=2 and ordr.distance(y,b)>=2:
            # (b,e) is a ladder
            for e in M_adj.get_neighbours(b):
                if ordr.is_between(e,a,x):
                    c = ordr.prev_of(e)
                    
                    if a!=c:
                        for d in M_adj.get_neighbours(c):
                            if ordr.is_between(d,y,b):
                                f = ordr.next_of(d)
                                
                                if b!=f and slh_util.pair(y,f) not in gap_set:
                                    opt = M_adj[y,f] + (not M_adj[c,e]) + (not M_adj[a,b]) + (not M_adj[d,f])
                                    
                                    n_ordr = slh_util.ordering(ordr)
                                    isomorphism_kappa(n_ordr,e,x,a,c,f,b,y,d)
                                    
                                    return opt,n_ordr
                                
            # (y,f) is a ladder
            for f in M_adj.get_neighbours(y):
                if ordr.is_between(f,y,b):
                    d = ordr.prev_of(f)
                    
                    if d!=y:
                        for c in M_adj.get_neighbours(d):
                            if ordr.is_between(c,a,x):
                                e = ordr.next_of(c)
                                
                                if x!=e and slh_util.pair(b,e) not in gap_set:
                                    opt = M_adj[b,e] + (not M_adj[c,e]) + (not M_adj[a,b]) + (not M_adj[d,f])
                                    
                                    n_ordr = slh_util.ordering(ordr)
                                    isomorphism_kappa(n_ordr,e,x,a,c,f,b,y,d)
                                    
                                    return opt,n_ordr
        
    return 0,None


def floating_5flo(ordr,gap):
    M_adj = slh_solver.solver.instance.adjacency_matrix
    gap_set = slh_solver.solver.instance.gap_set
    
    # Forward
    x = gap.v1
    y = gap.v0
    
    for a in M_adj.get_neighbours(x):
        f = ordr.next_of(a)
        
        if ordr.distance(x,a)>=2 and ordr.distance(f,y)>=5:
            for b in M_adj.get_neighbours(y):
                if ordr.is_between(b,f,y) and ordr.distance(b,y)>=3:
                    g = ordr.prev_of(b)
                    
                    if f!=g:
                        for e in M_adj.get_neighbours(f):
                            if ordr.is_between(e,x,a):
                                c = ordr.next_of(e)
                                
                                if a!=c:
                                    for d in M_adj.get_neighbours(c):
                                        if ordr.is_between(d,b,y):
                                            j = ordr.prev_of(d)
                                            h = ordr.next_of(d)
                                            
                                            if b!=j and y!=h and slh_util.pair(g,h) not in gap_set:
                                                opt = M_adj[g,h]+(not M_adj[c,e])+(not M_adj[a,f])+(not M_adj[b,g])+(not M_adj[d,h])
                                                
                                                n_ordr = slh_util.ordering(ordr)
                                                isomorphism_kappa(n_ordr,x,e,c,a,f,j,d,y)
                                                isomorphism_gamma(n_ordr,j,b)
                                                isomorphism_gamma(n_ordr,g,d)
                                                
                                                return opt,n_ordr
    
    # Backward
    x = gap.v0
    y = gap.v1
    
    for a in M_adj.get_neighbours(x):
        f = ordr.prev_of(a)
        
        if ordr.distance(a,x)>=2 and ordr.distance(y,f)>=5:
            for b in M_adj.get_neighbours(y):
                if ordr.is_between(b,y,f) and ordr.distance(y,b)>=3:
                    g = ordr.next_of(b)
                    
                    if f!=g:
                        for e in M_adj.get_neighbours(f):
                            if ordr.is_between(e,a,x):
                                c = ordr.prev_of(e)
                                
                                if a!=c:
                                    for d in M_adj.get_neighbours(c):
                                        if ordr.is_between(d,y,b):
                                            j = ordr.next_of(d)
                                            h = ordr.prev_of(d)
                                            
                                            if b!=j and y!=h and slh_util.pair(g,h) not in gap_set:
                                                opt = M_adj[g,h]+(not M_adj[c,e])+(not M_adj[a,f])+(not M_adj[b,g])+(not M_adj[d,h])
                                                
                                                n_ordr = slh_util.ordering(ordr)
                                                isomorphism_kappa(n_ordr,e,x,a,c,j,f,y,d)
                                                isomorphism_gamma(n_ordr,b,j)
                                                isomorphism_gamma(n_ordr,d,g)
                                                
                                                return opt,n_ordr
                                
    return 0,None


flo_tfrms = [
    floating_2flo,
    floating_3flo,
    floating_4flo_type1,
    floating_4flo_type2,
    floating_5flo
]

def floating_flo_transformation(ordr):
    for flo_tfrm in flo_tfrms:
        itr = iter(ordr)
        
        while gap:=ordr.next_gap(itr):
            opt,n_ordr = flo_tfrm(ordr,gap)
            
            if n_ordr:
                slh_solver.solver.instance.gap_set.add(gap)
                return opt,n_ordr
        
    return 0,None


def floating_2opt(ordr,gap,visited):
    M_adj = slh_solver.solver.instance.adjacency_matrix
    
    # Forward
    x = gap.v1
    y = gap.v0
    
    for a in M_adj.get_neighbours(x):
        b = ordr.prev_of(a)
        
        if x!=b:
            opt = M_adj[y,b] + (not M_adj[a,b])
            
            if opt:
                n_ordr = slh_util.ordering(ordr)
                isomorphism_gamma(n_ordr,x,b)
                
                if n_ordr not in visited: return opt,n_ordr
        
    # Backward
    x = gap.v0
    y = gap.v1
    
    for a in M_adj.get_neighbours(x):
        b = ordr.next_of(a)
        
        if x!=b:
            opt = M_adj[y,b] + (not M_adj[a,b])
            
            if opt:
                n_ordr = slh_util.ordering(ordr)
                isomorphism_gamma(n_ordr,b,x)
                
                if n_ordr not in visited: return opt,n_ordr
    
    return 0,None
    

def floating_3opt(ordr,gap,visited):
    M_adj = slh_solver.solver.instance.adjacency_matrix
    
    # Forward
    x = gap.v1
    y = gap.v0
    
    for a in M_adj.get_neighbours(x):
        c = ordr.prev_of(a)
        
        if x!=c and ordr.distance(a,y)>=2:
            # (y,b) is a ladder, Symmetric
            for b in M_adj.get_neighbours(y):
                if ordr.is_between(b,a,y):
                    d = ordr.next_of(b)
                    
                    if y!=d:
                        opt = M_adj[c,d] + (not M_adj[a,c]) + (not M_adj[b,d])
                        
                        if opt:
                            n_ordr = slh_util.ordering(ordr)
                            isomorphism_gamma(n_ordr,x,c)
                            isomorphism_gamma(n_ordr,d,y)
                            
                            if n_ordr not in visited: return opt,n_ordr
            
            # (c,d) is a ladder
            for d in M_adj.get_neighbours(c):
                if ordr.is_between(d,a,y):
                    b = ordr.prev_of(d)
                    
                    if a!=b:
                        opt = M_adj[y,b] + (not M_adj[a,c]) + (not M_adj[b,d])
                        
                        if opt:
                            n_ordr = slh_util.ordering(ordr)
                            isomorphism_gamma(n_ordr,x,c)
                            isomorphism_gamma(n_ordr,d,y)
                            
                            if n_ordr not in visited: return opt,n_ordr
                    
    # Backward
    x = gap.v0
    y = gap.v1
    
    for a in M_adj.get_neighbours(x):
        c = ordr.next_of(a)
        
        if x!=c and ordr.distance(y,a)>=2:
            # (b,y) is a ladder, already tested
            # (c,d) is a ladder
            for d in M_adj.get_neighbours(c):
                if ordr.is_between(d,y,a):
                    b = ordr.next_of(d)
                    
                    if a!=b:
                        opt = M_adj[y,b] + (not M_adj[a,c]) + (not M_adj[b,d])
                        
                        if opt:
                            n_ordr = slh_util.ordering(ordr)
                            isomorphism_gamma(n_ordr,c,x)
                            isomorphism_gamma(n_ordr,y,d)
                            
                            if n_ordr not in visited: return opt,n_ordr
                    
    return 0,None


def floating_4opt_type1(ordr,gap,visited):
    M_adj = slh_solver.solver.instance.adjacency_matrix

    # Symmetric
    x = gap.v1
    y = gap.v0

    for a in M_adj.get_neighbours(x):
        b = ordr.next_of(a)
        
        if M_adj[y,b] and ordr.distance(x,a)>=2 and ordr.distance(b,y)>=2:
            itr = slh_util.ordering_iterator(ordr,ordr.index_of(x))+2
            end = slh_util.ordering_iterator(ordr,ordr.index_of(a))
            
            while itr!=end:
                c = next(itr)
                
                for d in M_adj.get_neighbours(c):
                    if ordr.is_between(d,b,y):
                        e = ordr.prev_of(c)
                        f = ordr.next_of(d)
                        
                        if y!=f:
                            opt = M_adj[e,f] + (not M_adj[c,e]) + (not M_adj[a,b]) + (not M_adj[d,f])
                            
                            if opt:
                                n_ordr = slh_util.ordering(ordr)
                                isomorphism_gamma(n_ordr,x,e)
                                isomorphism_gamma(n_ordr,c,a)
                                isomorphism_gamma(n_ordr,b,d)
                                isomorphism_gamma(n_ordr,f,y)
                                
                                if n_ordr not in visited: return opt, n_ordr
                        
    return 0,None
                

def floating_4opt_type2(ordr,gap,visited):
    M_adj = slh_solver.solver.instance.adjacency_matrix
    
    # Forward
    x = gap.v1
    y = gap.v0
    
    for a in M_adj.get_neighbours(x):
        b = ordr.next_of(a)
        
        if ordr.distance(x,a)>=2 and ordr.distance(b,y)>=2:
            # (b,e) is a ladder
            for e in M_adj.get_neighbours(b):
                if ordr.is_between(e,x,a):
                    c = ordr.next_of(e)
                    
                    if a!=c:
                        for d in M_adj.get_neighbours(c):
                            if ordr.is_between(d,b,y):
                                f = ordr.prev_of(d)
                                
                                if b!=f:
                                    opt = M_adj[y,f] + (not M_adj[c,e]) + (not M_adj[a,b]) + (not M_adj[d,f])
                                    
                                    if opt:
                                        n_ordr = slh_util.ordering(ordr)
                                        isomorphism_kappa(n_ordr,x,e,c,a,b,f,d,y)
                                        
                                        if n_ordr not in visited: return opt,n_ordr
                                
            # (y,f) is a ladder
            for f in M_adj.get_neighbours(y):
                if ordr.is_between(f,b,y):
                    d = ordr.next_of(f)
                    
                    if d!=y:
                        for c in M_adj.get_neighbours(d):
                            if ordr.is_between(c,x,a):
                                e = ordr.prev_of(c)
                                
                                if x!=e:
                                    opt = M_adj[b,e] + (not M_adj[c,e]) + (not M_adj[a,b]) + (not M_adj[d,f])
                                    
                                    if opt:
                                        n_ordr = slh_util.ordering(ordr)
                                        isomorphism_kappa(n_ordr,x,e,c,a,b,f,d,y)
                                        
                                        if n_ordr not in visited: return opt,n_ordr
    
    # Backward
    x = gap.v0
    y = gap.v1
    
    for a in M_adj.get_neighbours(x):
        b = ordr.prev_of(a)
        
        if ordr.distance(a,x)>=2 and ordr.distance(y,b)>=2:
            # (b,e) is a ladder
            for e in M_adj.get_neighbours(b):
                if ordr.is_between(e,a,x):
                    c = ordr.prev_of(e)
                    
                    if a!=c:
                        for d in M_adj.get_neighbours(c):
                            if ordr.is_between(d,y,b):
                                f = ordr.next_of(d)
                                
                                if b!=f:
                                    opt = M_adj[y,f] + (not M_adj[c,e]) + (not M_adj[a,b]) + (not M_adj[d,f])
                                    
                                    if opt:
                                        n_ordr = slh_util.ordering(ordr)
                                        isomorphism_kappa(n_ordr,e,x,a,c,f,b,y,d)
                                        
                                        if n_ordr not in visited: return opt,n_ordr
                                
            # (y,f) is a ladder
            for f in M_adj.get_neighbours(y):
                if ordr.is_between(f,y,b):
                    d = ordr.prev_of(f)
                    
                    if d!=y:
                        for c in M_adj.get_neighbours(d):
                            if ordr.is_between(c,a,x):
                                e = ordr.next_of(c)
                                
                                if x!=e:
                                    opt = M_adj[b,e] + (not M_adj[c,e]) + (not M_adj[a,b]) + (not M_adj[d,f])
                                    
                                    if opt:
                                        n_ordr = slh_util.ordering(ordr)
                                        isomorphism_kappa(n_ordr,e,x,a,c,f,b,y,d)
                                        
                                        if n_ordr not in visited: return opt,n_ordr
        
    return 0,None


def floating_5opt(ordr,gap,visited):
    M_adj = slh_solver.solver.instance.adjacency_matrix
    
    # Forward
    x = gap.v1
    y = gap.v0
    
    for a in M_adj.get_neighbours(x):
        f = ordr.next_of(a)
        
        if ordr.distance(x,a)>=2 and ordr.distance(f,y)>=5:
            for b in M_adj.get_neighbours(y):
                if ordr.is_between(b,f,y) and ordr.distance(b,y)>=3:
                    g = ordr.prev_of(b)
                    
                    if f!=g:
                        for e in M_adj.get_neighbours(f):
                            if ordr.is_between(e,x,a):
                                c = ordr.next_of(e)
                                
                                if a!=c:
                                    for d in M_adj.get_neighbours(c):
                                        if ordr.is_between(d,b,y):
                                            j = ordr.prev_of(d)
                                            h = ordr.next_of(d)
                                            
                                            if b!=j and y!=h:
                                                opt = M_adj[g,h]+(not M_adj[c,e])+(not M_adj[a,f])+(not M_adj[b,g])+(not M_adj[d,h])
                                                
                                                if opt:
                                                    n_ordr = slh_util.ordering(ordr)
                                                    isomorphism_kappa(n_ordr,x,e,c,a,f,j,d,y)
                                                    isomorphism_gamma(n_ordr,j,b)
                                                    isomorphism_gamma(n_ordr,g,d)
                                                    
                                                    if n_ordr not in visited: return opt,n_ordr
    
    # Backward
    x = gap.v0
    y = gap.v1
    
    for a in M_adj.get_neighbours(x):
        f = ordr.prev_of(a)
        
        if ordr.distance(a,x)>=2 and ordr.distance(y,f)>=5:
            for b in M_adj.get_neighbours(y):
                if ordr.is_between(b,y,f) and ordr.distance(y,b)>=3:
                    g = ordr.next_of(b)
                    
                    if f!=g:
                        for e in M_adj.get_neighbours(f):
                            if ordr.is_between(e,a,x):
                                c = ordr.prev_of(e)
                                
                                if a!=c:
                                    for d in M_adj.get_neighbours(c):
                                        if ordr.is_between(d,y,b):
                                            j = ordr.next_of(d)
                                            h = ordr.prev_of(d)
                                            
                                            if b!=j and y!=h:
                                                opt = M_adj[g,h]+(not M_adj[c,e])+(not M_adj[a,f])+(not M_adj[b,g])+(not M_adj[d,h])
                                                
                                                if opt:
                                                    n_ordr = slh_util.ordering(ordr)
                                                    isomorphism_kappa(n_ordr,e,x,a,c,j,f,y,d)
                                                    isomorphism_gamma(n_ordr,b,j)
                                                    isomorphism_gamma(n_ordr,d,g)
                                                    
                                                    if n_ordr not in visited: return opt,n_ordr
                                
    return 0,None


flo_opt_tfrms = [
    floating_2opt,
    floating_3opt,
    floating_4opt_type1,
    floating_4opt_type2,
    floating_5opt
]

def floating_opt_transformation(ordr,visited):
    for flo_opt_tfrm in flo_opt_tfrms:
        itr = iter(ordr)
        
        while gap:=ordr.next_gap(itr):
            opt,n_ordr = flo_opt_tfrm(ordr,gap,visited)
            
            if n_ordr: return opt,n_ordr
        
    return 0,None


def opening_4flo(ordr,gap,visited):
    M_adj = slh_solver.solver.instance.adjacency_matrix
    
    # Forward
    x = gap.v1
    y = gap.v0
    
    for a in M_adj.get_neighbours(x):
        b = ordr.next_of(a)
        
        if ordr.distance(x,a)>=2 and ordr.distance(b,y)>=2:
            itr = slh_util.ordering_iterator(ordr,ordr.index_of(x))+2
            end = slh_util.ordering_iterator(ordr,ordr.index_of(a))
            
            while itr!=end:
                c = next(itr)
                
                for d in M_adj.get_neighbours(c):
                    if ordr.is_between(d,b,y):
                        e = ordr.prev_of(c)
                        f = ordr.prev_of(d)
                        
                        if b!=f:
                            n_ordr = slh_util.ordering(ordr)
                            isomorphism_kappa(n_ordr,x,e,c,a,b,f,d,y)
                            
                            if n_ordr not in visited:
                                opt = -1 + M_adj[b,e] + M_adj[y,f] + (not M_adj[c,e]) + (not M_adj[a,b]) + (not M_adj[d,f])
                                
                                return opt,n_ordr
                            
    # Backward
    x = gap.v0
    y = gap.v1
    
    for a in M_adj.get_neighbours(x):
        b = ordr.prev_of(a)
        
        if ordr.distance(a,x)>=2 and ordr.distance(y,b)>=2:
            itr = slh_util.ordering_iterator(ordr,ordr.index_of(x),False)-2
            end = slh_util.ordering_iterator(ordr,ordr.index_of(a))
            
            while itr!=end:
                c = next(itr)
                
                for d in M_adj.get_neighbours(c):
                    if ordr.is_between(d,y,b):
                        e = ordr.next_of(c)
                        f = ordr.next_of(d)
                        
                        if b!=f:
                            n_ordr = slh_util.ordering(ordr)
                            isomorphism_kappa(n_ordr,e,x,a,c,f,b,y,d)
                            
                            if n_ordr not in visited:
                                opt = -1 + M_adj[b,e] + M_adj[y,f] + (not M_adj[c,e]) + (not M_adj[a,b]) + (not M_adj[d,f])
                                
                                return opt,n_ordr
                            
    return 0,None
                                

def opening_transformation(ordr,visited):
    itr = iter(ordr)
    
    while gap:=ordr.next_gap(itr):
        opt,n_ordr = opening_4flo(ordr,gap,visited)
        
        if n_ordr: return opt,n_ordr
        
    return 0,None
