// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 227.

// Netcat is a simple read/write client for TCP servers.
package main

import (
	"io"
	"log"
	"net"
	"os"
	"fmt"
	"strings"
	"flag"
)

var server *string 
var username *string 

//!+
func main() {

	server = flag.String("server", "No server", "server ip:port")
	username = flag.String("user", "No user", "username") 
	
	flag.Parse()

	if strings.Compare(*username, "No user") == 0 {
		log.Fatal("Please, provide a username")
	}

	conn, err := net.Dial("tcp", *server)
	if err != nil {
		log.Fatal(err)
	}

	fmt.Fprintf(conn,  *username + "\n")

	done := make(chan struct{})
	go func() {
		io.Copy(os.Stdout, conn) // NOTE: ignoring errors
		log.Println("done")
		done <- struct{}{} // signal the main goroutine
	}()
	mustCopy(conn, os.Stdin)
	conn.Close()
	<-done // wait for background goroutine to finish
}

//!-

func mustCopy(dst io.Writer, src io.Reader) {
	if _, err := io.Copy(dst, src); err != nil {
		log.Fatal(err)
	}
}
