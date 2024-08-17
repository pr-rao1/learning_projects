package main

import (
	"fmt"
	"log"
	"net/http"
)

func infoHandler(w http.ResponseWriter, r *http.Request) {
	if r.URL.Path != "/info" {
		http.Error(w, "404 not Found", http.StatusNotFound)
		return
	}
	if r.Method != "GET" {
		http.Error(w, "Method not supported", http.StatusNotFound)
		return
	}
	fmt.Fprintln(w, "This website searches a repository of music files and finds if the searched song is available.")
}

func searchHandler(w http.ResponseWriter, r *http.Request) {
	if r.URL.Path != "/search" {
		http.Error(w, "404 not Found", http.StatusNotFound)
		return
	}
	if r.Method != "POST" {
		http.Error(w, "Method not supported", http.StatusNotFound)
		return
	}
	if err := r.ParseForm(); err != nil {
		fmt.Fprintf(w, "Parse form error : %v", err)
		return
	}
	//fmt.Fprintln(w, "POST successfully received")
	title := r.FormValue("title")
	fname := r.FormValue("fname")
	fmt.Fprintf(w, "Title: %s\n", title)
	fmt.Fprintf(w, "First name: %s\n", fname)
}

func main() {
	fileServer := http.FileServer(http.Dir("./pages"))
	http.Handle("/", fileServer)
	http.HandleFunc("/search", searchHandler)
	http.HandleFunc("/info", infoHandler)

	fmt.Println("Starting server at port 8080...")
	if err := http.ListenAndServe(":8080", nil); err != nil {
		log.Fatal(err)
	}

}
