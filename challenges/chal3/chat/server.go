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
	"flag"
	"strconv"
	"bytes"
	"time"
)

//!+broadcaster
type client chan<- string // an outgoing message channel

var host *string //server host
var port *int //server port
var users map[string]net.Conn //stores users currently online

var (
	entering = make(chan client)
	leaving  = make(chan client)
	messages = make(chan string) // all incoming client messages
)

func getUsers() string{ 
	var buf bytes.Buffer
	buf.WriteString("irc-server > ");
	for k, _ := range users { 
		buf.WriteString(k + " ")
	}

	return buf.String()
}

func getUserInfo(user string) string{
	if _, ok := users[user]; ok { //checks if user is already connected
		return ("irc-server > Username: " + user + " Ip: " +  users[user].RemoteAddr().String())
	} else {
		return "There's no user with that name"
	}
	
}

func getTime() string{
	var buf bytes.Buffer
	buf.WriteString("irc-server > Local time: ")
	buf.WriteString(time.Now().Location().String())
	buf.WriteString(" ")
	buf.WriteString(time.Now().Format("15:04:05"))
	
	return buf.String()
}

func sendPrivateMessage(originUser, destinyUser string, text []string) {
	if _, ok := users[destinyUser]; ok { //checks if user is already connected
		fmt.Fprintln(users[destinyUser], originUser + "> " + strings.Join(text," "))
	} else {
		fmt.Fprintln(users[originUser], "There's no user with that name")
	}
	
}

func broadcaster() {
	clients := make(map[client]bool) // all connected clients
	for {
		select {
		case msg := <-messages:
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
	
	users[user] = conn

	ch := make(chan string) // outgoing client messages
	go clientWriter(conn, ch, user)

	ch <- "irc-server > Welcome to the Simple IRC Server"
	ch <- "Your user [" + user +"] is successfully logged in"; 
	messages <- "New connected user ["+ user + "]"
	entering <- ch
	fmt.Println("irc-server > new connected user [" + user + "]");

	//--------------client message handler----------------
	input := bufio.NewScanner(conn)
	for input.Scan() {
		text := input.Text()
		tLength := len(text)
		
		//fmt.Println(text) //uncomment this to see incoming messages

		if tLength <= 0 {
			messages <- user + "> " + input.Text()	 
			continue
		}
		if strings.Compare(string(text[0]), "/") != 0 {
			messages <- user + "> " + input.Text()	 
			continue
		}

		//splits the message when / was detected at the 0 index	
		commandText := strings.Split(strings.Trim(text, " "), " ")
		
		if len(commandText) == 1 {
			switch commandText[0] {
			case "/users":
				ch <- getUsers()
			case "/time":
				ch <- getTime()
			default:
				messages <- user + "> " + text	 	
			}
		} else if len(commandText) == 2 {
			if strings.Compare(commandText [0],  "/user") == 0{
				ch <- getUserInfo(commandText[1])
			} else {
				messages <- user + "> " + text 	
			}
		} else if len(commandText) >= 3 {
			if strings.Compare(commandText [0],  "/msg") == 0{
				sendPrivateMessage(user, commandText[1], commandText[2:])
			}
		}

	}
	//----------------------------------------------------

	delete(users, user)

	fmt.Println("irc-server > ["+user+"] left")

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
	
	users = make(map[string]net.Conn)

	fmt.Println("irc-server > Simple IRC Server started at", *host, *port);
	fmt.Println("irc-server > Ready for receiving new clients");

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
		
		//fmt.Println("irc-server > new user login attempt: ",	 user);
		if _, ok := users[user]; ok { //checks if user is already connected
			//fmt.Println("login attempt failed\n");
			fmt.Fprintf(conn, "irc-server > Sorry, that username is already chosen. Please, choose another one")
			conn.Close()
		} else {
			//fmt.Println("login succeded\n")
			go handleConn(conn, user)			
		}
	}
}