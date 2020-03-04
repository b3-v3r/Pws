package main 

import (
	"fmt"
	"io/ioutil"
	"io"
	"os"
	"log"
	"net/http"
)

func ReadFile( filename string ) (string, bool) {
	file, err := os.Open( filename)

	if err != nil {
		return " ", true
	}
	
	text, err := ioutil.ReadAll( file )
	
	return string(text), false
}


func data_handler( w http.ResponseWriter, r *http.Request ) {

	fmt.Println("Getting data json ")
	file_text, err := ReadFile( "../stats/current_stat.json")

	if err {
		w.WriteHeader(404)
		return
	}

	w.WriteHeader(200)
	io.WriteString( w, file_text)
}

func main() {
	http.HandleFunc( "/get_json", data_handler )

	http.Handle("/", http.FileServer(http.Dir(".")))

	log.Fatal( http.ListenAndServe(":2020", nil))
}
