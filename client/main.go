package main

import (
	"fmt"
	"io"
	"io/ioutil"
	"log"
	"net/http"
	"os"
	"strconv"
	"syscall"
)

func ReadFile(filename string) (string, bool) {
	file, err := os.Open(filename)

	if err != nil {
		return " ", true
	}

	text, err := ioutil.ReadAll(file)

	return string(text), false
}

func data_handler(w http.ResponseWriter, r *http.Request) {

	file_text, err := ReadFile("../stats/current_stat.json")

	if err {
		w.WriteHeader(404)
		return
	}

	val, _ := strconv.Atoi(os.Args[1])
	syscall.Kill(val, syscall.SIGINT)

	w.WriteHeader(200)
	io.WriteString(w, file_text)

}


func main() {

	if len(os.Args) != 2 {
		fmt.Println("This programm not start byhand")
		os.Exit(0)
	}

	http.HandleFunc("/get_json", data_handler)
	cdir, _ := os.Getwd()
	test := fmt.Sprintf("%s%s", cdir, "/../client/")

	err := os.Chdir(test)

	if err != nil {
		fmt.Println("test")
	}


	http.Handle("/icons", http.FileServer(http.Dir("icons/") ) )
	http.Handle("/", http.FileServer(http.Dir(".") ) )

	log.Fatal(http.ListenAndServe(":2020", nil))
}
