all: knight-graph grid-graph hcp-encode hcp-decode

knight-graph: knight-graph.c
	gcc knight-graph.c -O2 -o knight-graph

grid-graph: grid-graph.c
	gcc grid-graph.c -O2 -o grid-graph

hcp-encode: hcp-encode.c
	gcc hcp-encode.c -O2 -o hcp-encode

hcp-decode: hcp-decode.c
	gcc hcp-decode.c -O2 -o hcp-decode

clean:
	rm knight-graph grid-graph hcp-encode hcp-decode
