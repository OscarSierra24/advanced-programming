package main

import(
	"fmt"
	"os"
	"net"
	"log"
	"io"
	"strings"
)

func main(){
	for i:= 1; i<len(os.Args); i++{
		s := strings.Split(os.Args[i], "=")
		fmt.Printf("%s: %s\n", s[0], s[1])
		conn, err := net.Dial("tcp", s[1])
		if err != nil{
			log.Fatal(err)
		}
		defer conn.Close()
		go mustCopy(os.Stdout, conn)	
	}
	for{}
}

func mustCopy(dst io.Writer, src io.Reader) {
	if _, err := io.Copy(dst, src); err != nil {
        log.Fatal(err)
    }
}