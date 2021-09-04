#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Dec 28 23:31:39 2020

@author: manu
"""

import math
from PIL import Image, ImageDraw, ImageFont


class slh_drawer(object):
    def __init__(self,solver):
        self.solver = solver
        self.image_size = 4096
        self.circle_size = 4000
        self.vertex_coordinates = slh_drawer.get_vertex_coordinates(
            len(self.solver.adjacency_matrix),
            self.image_size,
            self.circle_size
        )
        self.image = None


    def show(self):
        self.image.show()


    def save(self,path):
        self.image.save(path)


    def draw(self,ordering,background_color=None,layer=None,stage=-1):
        if layer == "snakes": image = self.draw_snakes(ordering,stage=stage)
        elif layer == "ladders": image = self.draw_ladders(ordering)
        else: image = Image.alpha_composite(self.draw_ladders(ordering),self.draw_snakes(ordering,stage=stage))

        image = image.resize((self.image_size//4,self.image_size//4),resample=Image.LANCZOS)

        if background_color:
            background = Image.new('RGBA',(self.image_size//4,self.image_size//4),color=background_color)
            image = Image.alpha_composite(background,image)

        self.image = image


    def draw_snakes(self,ordering,stage=-1):
        image = Image.new('RGBA',(self.image_size,self.image_size))
        drawer = ImageDraw.Draw(image)
        font = ImageFont.truetype("./static/font/cmunss.ttf",150)

        vertex = dict(zip(ordering,self.vertex_coordinates))

        margin = (self.image_size-self.circle_size)/2
        circle_box = (margin,margin,self.image_size-margin,self.image_size-margin)

        snakes = 0

        for i in range(len(ordering)):
            ni = 0 if i==len(ordering)-1 else i+1
            curr_vertex = vertex[ordering[i]]
            next_vertex = vertex[ordering[ni]]
            if self.solver.adjacency_matrix[ordering[i],ordering[ni]]: snakes += 1

            drawer.arc(
                circle_box,
                curr_vertex[1],
                next_vertex[1],
                fill = "green" if self.solver.adjacency_matrix[ordering[i],ordering[ni]] else "red",
                width = 10
            )

        drawer.text(
            (0,self.image_size-50),
            "Stage {}".format("-" if stage<0 else str(stage)),
            fill="gray",
            anchor="ls",
            font=font
        )

        drawer.text(
            (self.image_size,self.image_size-50),
            "Snakes {}/{}".format(snakes,len(ordering)),
            fill="gray",
            anchor="rs",
            font=font
        )

        return image


    def draw_ladders(self,ordering):
        image = Image.new('RGBA',(self.image_size,self.image_size))
        drawer = ImageDraw.Draw(image)

        vertex = dict(zip(ordering,self.vertex_coordinates))

        for i in range(len(ordering)):
            pi = len(ordering)-1 if i==0 else i-1
            ni = 0 if i==len(ordering)-1 else i+1
            curr_vertex = vertex[ordering[i]]

            neighbours = [n for n in self.solver.adjacency_matrix.get_neighbours(ordering[i]) if n>ordering[i]]
            if (prev:=ordering[pi]) in neighbours: neighbours.remove(prev)
            if (nex:=ordering[ni]) in neighbours: neighbours.remove(nex)

            for n in neighbours:
                neig_vertex = vertex[n]

                drawer.line(
                    (curr_vertex[0],neig_vertex[0]),
                    fill = "orange",
                    width = 2
                )

        return image


    @staticmethod
    def get_vertex_coordinates(n,image_size,circle_size):
        center = image_size/2
        radious = circle_size/2
        alpha = 2*math.pi/n
        beta = 360/n
        vertex_coordinates = []

        for k in range(n):
            vertex_coordinates.append((
                (radious*math.cos(k*alpha)+center,radious*math.sin(k*alpha)+center),
                k*beta
            ))

        return vertex_coordinates
