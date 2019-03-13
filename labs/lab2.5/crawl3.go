// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 241.

// Crawl2 crawls web links starting with the command-line arguments.
//
// This version uses a buffered channel as a counting semaphore
// to limit the number of concurrent calls to links.Extract.
package main

import (
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"

	"gopl.io/ch5/links"
)

// tokens is a counting semaphore used to
// enforce a limit of 20 concurrent requests.
var tokens = make(chan struct{}, 20)
var seen = make(map[string]bool)
var done = make(chan bool)

func main() {
	url := os.Args[2]
	depthArg := strings.Split(os.Args[1], "=")
	
	if strings.Compare(depthArg[0], "-depth") != 0 { 
		log.Fatal("Usage: ./crawl3 -depth=2 https://google.com")
	}
	depth, err := strconv.Atoi(depthArg[1]) 
	if err != nil {
		log.Fatal("There was an issue with depth parameter, make sure depth is an integer number.")
	}
	
	go crawls(depth, url, done)
	<- done
}

func crawls(depth int, url string, done chan bool) {
	defer func(){done <- true}()

	if depth <= 0 {
		return
	}

	_, v := seen[url]
	if v {
		return
	} else {
		seen[url] = true
	}

	links := crawl(url)
	doneRecursion := make(chan bool)
	for _, link := range links {
		go crawls(depth-1, link, doneRecursion)
		<- doneRecursion
	}
}

func crawl(url string) []string {
	fmt.Println(url)
	tokens <- struct{}{} // acquire a token
	list, err := links.Extract(url)
	<-tokens // release the token
	if err != nil {
		log.Print(err)
	}
	return list
}