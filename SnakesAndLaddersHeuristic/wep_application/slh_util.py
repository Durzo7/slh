#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Dec 28 13:24:48 2020

@author: manu
"""

import array
import bitarray

import slh_solver


class adjacency_matrix(object):
    def __init__(self,n):
        self._matrix = [bitarray.bitarray('0'*n) for i in range(n)]
        self._lists = [array.array('H') for i in range(n)]
        
    def __getitem__(self,key):
        return self._matrix[key[0]][key[1]]
        
    def __len__(self):
        return len(self._matrix)
    
    def connect(self,v0,v1):
        if v0!=v1 and not self[v0,v1]:
            self._matrix[v0][v1] = 1
            self._matrix[v1][v0] = 1
            self._lists[v0].append(v1)
            self._lists[v1].append(v0)
            
    def sort(self):
        for i in range(len(self._lists)): self._lists[i] = sorted(self._lists[i])
    
    def get_neighbours(self,v):
        return self._lists[v]


class pair(object):
    def __init__(self,v0,v1):
        self.v0 = v0
        self.v1 = v1
    
    def __eq__(self,other):
        return ((self.v0==other.v0 or self.v0==other.v1) and
                (self.v1==other.v1 or self.v1==other.v0))
    
    def __hash__(self):
        return pair.cantor(self.v0,self.v1) if self.v0<self.v1 else pair.cantor(self.v1,self.v0)
    
    def __getitem__(self,i):
        if i==0: return self.v0
        if i==1: return self.v1
        raise IndexError
        
    @staticmethod
    def cantor(x,y):
        return (x+y)*(x+y+1)//2+x

    
class ordering(object):
    size = 0
    
    def __init__(self,other=None):   
        if other:
            self._verts = array.array('H',other._verts)
            self._indxs = array.array('H',other._indxs)
        else:
            self._verts = array.array('H',range(ordering.size))
            self._indxs = array.array('H',range(ordering.size))
    
    def __getitem__(self,i):
        return self._verts[i]
    
    def __eq__(self,other):
        dir0 = self.next_of(0)<self.prev_of(0)
        dir1 = other.next_of(0)<other.prev_of(0)
        
        itr0 = ordering_iterator(self,self.index_of(0),direction=dir0)
        itr1 = ordering_iterator(other,other.index_of(0),direction=dir1)
        
        next(itr0)
        next(itr1)
        
        while True:
            try:
                if next(itr0)!=next(itr1): return False
            
            except StopIteration: return True
    
    def __lt__(self,other):
        dir0 = self.next_of(0)<self.prev_of(0)
        dir1 = other.next_of(0)<other.prev_of(0)
        
        itr0 = ordering_iterator(self,self.index_of(0),direction=dir0)
        itr1 = ordering_iterator(other,other.index_of(0),direction=dir1)
        
        next(itr0)
        next(itr1)
        
        while True:
            try:
                if itr0.value()<itr1.value(): return True
                if next(itr0)>next(itr1): return False
            
            except StopIteration: return False
    
    def __gt__(self,other):
        dir0 = self.next_of(0)<self.prev_of(0)
        dir1 = other.next_of(0)<other.prev_of(0)
        
        itr0 = ordering_iterator(self,self.index_of(0),direction=dir0)
        itr1 = ordering_iterator(other,other.index_of(0),direction=dir1)
        
        next(itr0)
        next(itr1)
        
        while True:
            try:
                if itr0.value()>itr1.value(): return True
                if next(itr0)<next(itr1): return False
            
            except StopIteration: return False
            
    def __ne__(self,other):
        return not self==other
            
    def __le__(self,other):
        return not self>other
    
    def __ge__(self,other):
        return not self<other
    
    def __iter__(self):
        return ordering_iterator(self,0)
        
    def index_of(self,v):
        return self._indxs[v]
    
    def prev_of(self,v):
        return self._verts[self._indxs[v]-1 if self._indxs[v] else ordering.size-1]
    
    def next_of(self,v):
        return self._verts[(self._indxs[v]+1)%ordering.size]
    
    def distance(self,v0,v1):
        i0 = self.index_of(v0)
        i1 = self.index_of(v1)
        
        return ordering.size-i0+i1-1 if i0>i1 else i1-i0-1
    
    def is_between(self,v,v0,v1):
        i = self.index_of(v)
        i0 = self.index_of(v0)
        i1 = self.index_of(v1)
        
        if (i0<i1 and i>i0 and i<i1): return True
        if (i0>i1 and (i>i0 or i<i1)): return True
        
        return False
    
    def next_gap(self,itr=None):
        M_adj = slh_solver.solver.instance.adjacency_matrix
        if not itr: itr = iter(self)
        
        while True:
            try:
                v0 = next(itr)
                v1 = itr.value()
                
                if not M_adj[v0,v1]: return pair(v0,v1)
                
            except StopIteration: return None
            
    def n_gaps(self):
        itr = iter(self)
        cont = 0
        
        while (self.next_gap(itr)): cont+=1
        
        return cont
    
    def is_hamiltonian(self):
        return not self.next_gap()
    
    def reverse_segment(self,v0,v1):
        itr0 = ordering_iterator(self,self.index_of(v0),True)
        itr1 = ordering_iterator(self,self.index_of(v1),False)
        
        n_itr = self.distance(v0,v1)//2+1
        
        for _ in range(n_itr):
            self._indxs[itr0.value()] = itr1._i
            self._indxs[itr1.value()] = itr0._i
            self._verts[itr0._i],self._verts[itr1._i] = itr1.value(),itr0.value()
            next(itr0)
            next(itr1)
            
    def swap_segments(self,v00,v01,v10,v11):
        i00 = self.index_of(v00)
        i01 = self.index_of(v01)
        i10 = self.index_of(v10)
        i11 = self.index_of(v11)
        
        n_verts = array.array('H')
        
        if i10<i11: n_verts += self._verts[i10:i11+1]
        else:
            n_verts += self._verts[i10:]
            n_verts += self._verts[:i11+1]
            
        if i01<i10: n_verts += self._verts[i01+1:i10]
        else:
            n_verts += self._verts[i01+1:]
            n_verts += self._verts[:i10]
            
        if i00<i01: n_verts += self._verts[i00:i01+1]
        else:
            n_verts += self._verts[i00:]
            n_verts += self._verts[:i01+1]
            
        if i11<i00: n_verts += self._verts[i11+1:i00]
        else:
            n_verts += self._verts[i11+1:]
            n_verts += self._verts[:i00]
            
        self._verts = n_verts
        
        for i in range(ordering.size): self._indxs[self._verts[i]] = i
    
class ordering_iterator(object):
    def __init__(self,ordering,i,direction=True):
        self._ord = ordering
        self._i = i
        self._fi = i
        self._ef = False
        self._dir = direction
    
    def __iter__(self):
        return self
    
    def __next__(self):
        value = self.value()
        
        if self._i==self._fi:
            if self._ef: raise StopIteration
            else: self._ef = True
        
        if self._dir: self += 1
        else: self -= 1
        
        return value
        
    
    def __iadd__(self,n):
        self._i = (self._i+n)%ordering.size
        return self
    
    def __isub__(self,n):
        self._i = ordering.size+self._i-n if self._i<n else self._i-n
        return self
    
    def __add__(self,n):
        itr = ordering_iterator(self._ord,self._i,self._dir)
        itr += n
        return itr
    
    def __sub__(self,n):
        itr = ordering_iterator(self._ord,self._i,self._dir)
        itr -= n
        return itr
    
    def __eq__(self,other):
        return self._i==other._i
    
    def value(self):
        return self._ord[self._i]
        