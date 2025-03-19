# Simple Golang WebServer
This project is a demonstration of how to build a simple webserver with two routes using Golang.
This emulates a website where it searches a database of songs and checks if any songs of 'X Y' artist are present. (enhancement in progress...)

It has three routes:
Default: localhost:8080/  --> The Main page
localhost:8080/info   --> Contains information about the website
localhost:8080/search.html  --> A form is present to input the artist's name and once "search" button is clicked, the search begins.


Note: This is a leanring project made at a very nascent stage with the help of the below resource:
https://www.youtube.com/watch?v=ASBUp7stqjo&list=PL5dTjWUk_cPYztKD7WxVFluHvpBNM28N9


Procedure to run:
1) Go to the path xxx/top_dir/simple_go_server/src
2) go build
3) go run main.go
4) Allow firewall permissions if prompted, go to a browser and enter localhost:8080/

Debugging:
1) If you see Main page still on localhost:8080 after terminating server, then it might be due to 
    a) Concurrency of the go routine triggering ListenAndServe() subroutine. Use "err != http.ErrServerClosed" when triggering it. Or replace "log.Fatal(err)" with some printf
    b) OS waiting for TIME_WAIT (40-120 seconds) for transmission of residual packets. It is recommended to wait out the time. Else use "SO_REUSEADDR" (for development purpose only).