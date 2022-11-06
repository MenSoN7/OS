all:
	gcc TCP_client.c -o TCP_client
	gcc TCP_server.c -o TCP_server
	gcc client_UDP.c -o client_UDP
	gcc server_UDP.c -o server_UDP
