# Lab 2.5 crawl3.go

Add depth-limiting to the concurrent web crawler from crawl2.go. That is, if the user sets -depth=3, then only URLs reachable by at most 3 links will be fetched.


### Prerequisites

```
Golang

gopl.io/ch5/links 

```

## Testing

```
go run crawl3.go -depth=2 https://google.com
```
