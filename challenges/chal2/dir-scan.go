package main

import (
	"fmt"
	"os"
	"path/filepath"
)

// scanDir stands for the directory scanning implementation
func scanDir(dir string) (int, int, int, error) {
	nDir := 0
	nSymLink := 0
	nOthers := 0
	
	err := filepath.Walk(os.Args[1], func(path string, info os.FileInfo, err error) error {
		m:=info.Mode()
		if info.IsDir(){
			nDir++
		} else if m&os.ModeSymlink != 0{
			nSymLink++
		} else{
			nOthers++
		}
		
        return nil
	})
	if err != nil {
        panic(err)
	}
	//return nil

	return nDir, nSymLink, nOthers, err
}

func main() {
	if len(os.Args) < 2 {
		fmt.Println("Usage: ./dir-scan <directory>")
		os.Exit(1)
	}

	nDir, nSymLink, nOthers, err := scanDir(os.Args[1])
	if err != nil{
		fmt.Println("error")
	}else{
		fmt.Printf("Path: %s\n", os.Args[1])
		fmt.Printf("Directories: %d \n", nDir)
		fmt.Printf("Symbolic links: %d\n", nSymLink)
		fmt.Printf("Other files: %d\n", nOthers)	
	}
}
