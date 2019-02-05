// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 156.

// Package geometry defines simple types for plane geometry.
//!+point
package main

import (
	"fmt"
	"math"
	)

type Point struct{ x, y float64 }

// traditional function
func Distance(p, q Point) float64 {
	return math.Hypot(q.X()-p.X(), q.Y()-p.Y())
}

func (p Point) X() float64{
	return p.x;
}

func (p Point) Y() float64{
	return p.y;
}

// same thing, but as a method of the Point type
func (p Point) Distance(q Point) float64 {
	return math.Hypot(q.X()-p.X(), q.Y()-p.Y())
}

//!-point

//!+path

// A Path is a journey connecting the points with straight lines.
type Path []Point

// Distance returns the distance traveled along the path.
func (path Path) Distance() float64 {
	sum := 0.0
	for i := range path {
		if i > 0 {
			sum += path[i-1].Distance(path[i])
		}
	}
	return sum
}

func main(){
	
	printPoint(Point{2.2,3.3,})
}

func printPoint(p Point){
	fmt.Println(p.X(), ",", p.Y())
}

//!-path
