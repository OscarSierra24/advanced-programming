// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 254.
//!+

// Chat is a server that lets clients chat with each other.
package main

import (
	"bufio"
	"fmt"
	"log"
	"net"
	"strings"
)

//!+broadcaster
type client chan<- string // an outgoing message channel

var (
	entering = make(chan client)
	leaving  = make(chan client)
	messages = make(chan string) // all incoming client messages
)
var users []string

func getUsers() string{
	return "users: " + strings.Join(users, ", ")
}

func broadcaster() {
	clients := make(map[client]bool) // all connected clients
	for {
		select {
		case msg := <-messages:
			fmt.Println("estoy aqui")
			// Broadcast incoming message to all
			// clients' outgoing message channels.
			for cli := range clients {
				cli <- msg
			}

		case cli := <-entering:
			clients[cli] = true

		case cli := <-leaving:
			delete(clients, cli)
			close(cli)
		}
	}
}

//!-broadcaster

//!+handleConn
func handleConn(conn net.Conn, user string) {
	user = user[:len(user)-1]
	ch := make(chan string) // outgoing client messages
	go clientWriter(conn, ch, user)
	who := conn.RemoteAddr().String()

	users = append(users, user)
	
	ch <- "You are " + user
	messages <- user + " has arrived"
	entering <- ch

	input := bufio.NewScanner(conn)
	for input.Scan() {
		fmt.Println(input.Text())
		if strings.Compare(input.Text(), "/users") == 0{
			ch <- getUsers()
			continue
		}
		messages <- user + ": " + input.Text()
	}
	// NOTE: ignoring potential errors from input.Err()

	leaving <- ch
	messages <- who + " has left"
	conn.Close()
}

func clientWriter(conn net.Conn, ch <-chan string, user string) {
	for msg := range ch {
		fmt.Fprintln(conn, msg) // NOTE: ignoring network errors
	}
}

//!-handleConn

//!+main
func main() {
	listener, err := net.Listen("tcp", "localhost:8000")
	if err != nil {
		log.Fatal(err)
	}

	go broadcaster()
	for {
		conn, err := listener.Accept()
		message, _ := bufio.NewReader(conn).ReadString('\n')
		if err != nil {
			log.Print(err)
			continue
		}
		go handleConn(conn, string(message))
	}
}

//!-main
