# Tylers Game of Life

By Tyler Lewis
   tylewis@chapman.edu

   2021

My take on Conway’s Game of Life

Simulation of cells who reproduce and die, much like organisms do in the real world. Rules of the game vary between different peoples implementations.

Rules of how cells reproduce and die are as follows:
> A cell with one or fewer neighbors will be empty in the next generation
> A cell with two neighbors retains its state in the next generation
> A cell with three neighbors will contain a live cell in the next generation
> A cell with four or more neighbors will be empty in the next generation
>
> All births and deaths take place simultaneously


Use makefile to run:

> make all

then...

> ./tylerLyfe.exe
