// Clock2 is a concurrent TCP server that periodically writes the time.
package main

import (
	"io"
	"log"
	"net"
	"time"
	"os"
)

func handleConn(c net.Conn) {
	defer c.Close()
	slocation := os.Getenv("TZ");
	location, _:= time.LoadLocation(os.Getenv("TZ"))
	for {
		_, err := io.WriteString(c, (slocation + "			:" + time.Now().In(location).Format("15:04:05\n")))
		if err != nil {
			return // e.g., client disconnected
		}
		time.Sleep(1 * time.Second)
	}
}

func main(){
	listener, err := net.Listen("tcp", ("localhost:"+os.Args[2]))
	if err != nil {
		log.Fatal(err)
	}
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err) // e.g., connection aborted
			continue
		}
		go handleConn(conn) // handle connections concurrently
	}
}