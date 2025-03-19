package main

import (
	"context"
	"fmt"
	"log"
	"net/http"
	"os"
	"os/signal"
	"syscall"
	"time"
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

	server := &http.Server{
		Addr:    ":8080",
		Handler: fileServer, // File server instead of ServeMux
	}

	fmt.Println("Starting server at port 8080...")
	if err := server.ListenAndServe(); err != nil {
		log.Fatal(err)
	}

	stop := make(chan os.Signal, 1)
	signal.Notify(stop, os.Interrupt, syscall.SIGTERM)

	<-stop // Wait for termination signal
	fmt.Println("\nShutting down server...")

	// Graceful shutdown with a timeout
	ctx, cancel := context.WithTimeout(context.Background(), 5*time.Second)
	defer cancel()

	if err := server.Shutdown(ctx); err != nil {
		fmt.Println("Server shutdown failed:", err)
	} else {
		fmt.Println("Server stopped gracefully.")
	}

}
