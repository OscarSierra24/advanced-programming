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
	//"strings"
	"flag"
	"strconv"
	"bytes"
	"time"
)

//!+broadcaster
type client chan<- string // an outgoing message channel

var host *string //server host
var port *int //server port
var users map[string]bool //stores users currently online

var (
	entering = make(chan client)
	leaving  = make(chan client)
	messages = make(chan string) // all incoming client messages
)

func getUsers() string{ 
	var buf bytes.Buffer

	for k, _ := range users { 
		buf.WriteString(k + " ")
	}

	return buf.String()
}

func getTime() string{
	var buf bytes.Buffer
	buf.WriteString("Local time: ")
	buf.WriteString(time.Now().Location().String())
	buf.WriteString(" ")
	buf.WriteString(time.Now().Format("15:04:05"))
	
	return buf.String()
}

func broadcaster() {
	clients := make(map[client]bool) // all connected clients
	for {
		select {
		case msg := <-messages:
			//fmt.Println("estoy aqui")
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
	
	users[user] = true

	ch := make(chan string) // outgoing client messages
	go clientWriter(conn, ch, user)

	ch <- "You are " + user
	messages <- user + " has arrived"
	entering <- ch

	//--------------client message handler----------------
	input := bufio.NewScanner(conn)
	for input.Scan() {
		fmt.Println(input.Text())
		switch text := input.Text(); text {
		case "/users":
			ch <- getUsers()

		case "/time":
			ch <- getTime()
		
		default:
			messages <- user + ": " + input.Text()
		}
	}
	//----------------------------------------------------

	delete(users, user)
	fmt.Println("user" , user, "has left\n")

	leaving <- ch
	messages <- user + " has left"
	
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
	
	host = flag.String("host", "No host", "host ip")
	port = flag.Int("port", 8000, "port number")
	
	flag.Parse()
	
	fmt.Println(getTime())

	users = make(map[string]bool)

	fmt.Println("host: ", *host)
	fmt.Println("port: ", *port)
	fmt.Println("")

	

	listener, err := net.Listen("tcp", (*host + ":" + strconv.Itoa(*port)))	
	if err != nil {
		log.Fatal(err)
	}

	go broadcaster()
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err)
			continue
		}

		user, _ := bufio.NewReader(conn).ReadString('\n')
		user = user[:len(user)-1]
		
		fmt.Println("new user login attempt: ",	 user);
		if _, ok := users[user]; ok { //checks if user is already connected
			fmt.Println("login attempt failed\n");
			fmt.Fprintf(conn, "Sorry, that username is already chosen. Please, choose another one")
			conn.Close()
		} else {
			fmt.Println("login succeded\n")
			go handleConn(conn, user)			
		}
	}
}