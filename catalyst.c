#include <ev.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <netinet/in.h>

#include <malloc.h>
#include "spdy_session.h"



void accept_cb(struct ev_loop *loop, struct ev_io *watcher, int revents);
void read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents);
void write_cb(struct ev_loop *loop, struct ev_io *watcher, int revents);


	 // every watcher type has its own typedef'd struct
	 // with the name ev_TYPE
	 ev_io stdin_watcher;
	 ev_timer timeout_watcher;

#define BUFSIZE 1024
#define PORT_NO 9001
// all watcher callbacks have a similar signature
// this callback is called when data is readable on stdin
static void
stdin_cb (EV_P_ ev_io *w, int revents)
{
	char buf[BUFSIZE];
	int nread;

	puts ("stdin ready");

	
	if(revents & EV_READ)
	{
		puts ("can read");
		nread = read(0, buf, BUFSIZE);
		printf("read %d\ns", nread);
	}
 
	if(revents & EV_WRITE)
		puts ("can write");

 // for one-shot events, one must manually stop the watcher
 // with its corresponding stop function.
 
 //ev_io_stop (EV_A_ w);

 // this causes all nested ev_loop's to stop iterating
 //ev_unloop (EV_A_ EVUNLOOP_ALL);
}

// another callback, this time for a time-out
static void
timeout_cb (EV_P_ ev_timer *w, int revents)
{
 puts ("timeout");
 // this causes the innermost ev_loop to stop iterating
 ev_unloop (EV_A_ EVUNLOOP_ONE);
}





#define BUFFER_SIZE 1024

int total_clients = 0; // Total number of connected clients


typedef struct
{
  spdy_session_t spdy_session;
  struct ev_io *read_watcher;
  struct ev_io *write_watcher;

} catalyst_connection_t;

/* Accept client requests */
void accept_cb(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	int client_sd;
	if(EV_ERROR & revents)
	{
	perror("got invalid event");
	return;
	}

	// Accept client request
	client_sd = accept(watcher->fd, (struct sockaddr *)&client_addr, &client_len);

	if (client_sd < 0)
	{
	perror("accept error");
	return;
	}

	total_clients ++; // Increment total_clients count
	printf("Successfully connected with client.\n");
	printf("%d client(s) connected.\n", total_clients);

	catalyst_connection_t *connection = (catalyst_connection_t*)malloc(sizeof(catalyst_connection_t));

	connection->read_watcher = (struct ev_io*) malloc (sizeof(struct ev_io));
	connection->write_watcher = (struct ev_io*) malloc (sizeof(struct ev_io));
	spdy_session_create(&connection->spdy_session);

	connection->read_watcher->data = connection;
	connection->write_watcher->data = connection;

	ev_io_init(connection->read_watcher, read_cb, client_sd, EV_READ);
	ev_io_init(connection->write_watcher, write_cb, client_sd, EV_WRITE);
	ev_io_start(loop, connection->read_watcher);
}


void write_cb(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
	printf("can write to client");
	catalyst_connection_t *connection = watcher->data;
	ev_io_stop(loop, connection->write_watcher);
}

/* Read client message */
void read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents){
	char buffer[BUFFER_SIZE];
	ssize_t read;

	if(EV_ERROR & revents)
	{
	perror("got invalid event");
	return;
	}

	catalyst_connection_t *connection = watcher->data;
	spdy_session_t *spdy_session = &connection->spdy_session;

	printf("start of callback: avail_to_parse=%u, addr=%x\n", spdy_session->avail_to_parse, &spdy_session->avail_to_parse);

	uint8_t *buffer_cursor = spdy_session->parse_buffer + spdy_session->avail_to_parse;
	size_t buffer_space = SPDY_SESSION_PARSE_BUFFER_SIZE - spdy_session->avail_to_parse;

	printf("allowing read of %zu\n", buffer_space);

	//memcpy(, test_packet_syn_stream, 5);

	// Receive message from client socket
	read = recv(watcher->fd, buffer_cursor, buffer_space, 0);

	printf("read res: %d\n", read);

	if(read < 0)
	{
	perror("read error");
	return;
	}

	if(read == 0)
	{
		// Stop and free watcher if client socket is closing, FIME: free spdy
		ev_io_stop(loop,watcher);
		free(connection);
		perror("peer might closing");
		total_clients --; // Decrement total_clients count
		printf("%d client(s) connected.\n", total_clients);
		return;
	}
	else
	{
		spdy_session->avail_to_parse += read;
		int res = spdy_session_parse_next_frame(&connection->spdy_session);
		printf("spdy_session_parse_next_frame result: %d\n", res);
		if(res == 0) {
			uint8_t packed_frame[SPDY_SESSION_PARSE_BUFFER_SIZE];
			uint32_t packed_size;

		  if(spdy_session->received_frame_count > 1)
		  {
		  	ev_io_start(loop, connection->write_watcher);

		  	if(spdy_session->last_frame.frame_type == SPDY_CONTROL_FRAME && spdy_session->last_frame.control_frame_type == SPDY_CONTROL_SYN_STREAM)
		  	{
				  packed_size = spdy_frame_pack_rst_stream(packed_frame, SPDY_SESSION_PARSE_BUFFER_SIZE,
	                                                 spdy_session->last_frame.control_header.syn_stream.stream_id /*stream_id*/,
	                                                 1 /*status*/,
	                                                 0 /*flags*/);
			  	
			  	res = send(watcher->fd, packed_frame, packed_size, 0);
			  	printf("send() result for choke RSTSTREAM: %d, size was: %d \n", res, packed_size);
		  	}
		  	else
			  {
			  	printf("received an additional frame, but it was no SYN_STREAM\n");
			  }
		  }
		  else
		  {
		  	printf("first frame, looks good, sending reply.\n");

		  	const uint8_t test_headers[] = {0x78, 0xbb, 0xdf, 0xa2, 0x51, 0xb2, 0x62, 0x60, 0x66, 0xe0, 0x41, 0x0e, 0x24, 0x06, 0x2e, 0x84, 0x1d, 0x0c, 0x6c, 0x10, 0xe5, 0x0c, 0x6c, 0xc0, 0x64, 0xac, 0xe0, 0xef, 0xcd, 0xc0, 0x0e, 0xd5, 0xc8, 0xc0, 0x01, 0x33, 0x0f, 0x00, 0x00, 0x00, 0xff, 0xff};

		  	spdy_headers_t my_headers;
		  	spdy_headers_create(&my_headers);

		  	spdy_headers_add(&my_headers, "hello", "world");
		  	size_t zipped_header_len;
		  	uint8_t *zipped_headers = spdy_headers_deflate(&my_headers, &spdy_session->deflate_zstrm, &zipped_header_len);

		  	packed_size = spdy_frame_pack_syn_reply(packed_frame, 150,
                                                1 /*stream_id*/,
                                                zipped_headers /*headers*/,
                                                zipped_header_len,
                                                0 /*flags*/);
        printf("zipped_header_len: %u, packed_size: %u", zipped_header_len, packed_size);
        uint32_t n;
        for(n=0; n<packed_size; n++) {
			    printf("%02x ",(uint8_t)packed_frame[n]);
			  }
  			printf("\n");
		  	res = send(watcher->fd, packed_frame, packed_size, 0);
		  	printf("send() result for SYNREPLY: %d\n", res);


		  	const uint8_t test_data[] = "This is SPDY.";
			  packed_size = spdy_frame_pack_data(packed_frame, 40,
                                          1 /*stream_id*/,
                                          test_data /*data*/,
                                          sizeof(test_data)-1,
                                          1 /*flags*/);

		  	res = send(watcher->fd, packed_frame, packed_size, 0);
		  	printf("send() result for DATA_FIN: %d\n", res);
		  }


		}
	}

	printf("totally done!\n");

}




static void
sigint_cb (struct ev_loop *loop, struct ev_signal *w, int revents)
{
  ev_unloop (loop, EVUNLOOP_ALL);
}











int
main (void)
{
	// use the default event loop unless you have special needs
	struct ev_loop *loop = ev_default_loop (0);

	// initialise an io watcher, then start it
	// this one will watch for stdin to become readable
	ev_io_init (&stdin_watcher, stdin_cb, /*STDIN_FILENO*/ 0, EV_READ);
	ev_io_start (loop, &stdin_watcher);




	int sd;
	struct sockaddr_in addr;
	int addr_len = sizeof(addr);
	struct ev_io w_accept;

	// Create server socket
	if( (sd = socket(PF_INET, SOCK_STREAM, 0)) < 0 )
	{
	perror("socket error");
	return -1;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT_NO);
	addr.sin_addr.s_addr = INADDR_ANY;

	// Bind socket to address
	if (bind(sd, (struct sockaddr*) &addr, sizeof(addr)) != 0)
	{
	perror("bind error");
	}

	// Start listing on the socket
	if (listen(sd, 2) < 0)
	{
		perror("listen error");
		return -1;
	}
	printf("Listening on port %d\n", PORT_NO);

	// Initialize and start a watcher to accepts client requests
	ev_io_init(&w_accept, accept_cb, sd, EV_READ);
	ev_io_start(loop, &w_accept);




	// exit on SIGINT

  struct ev_signal signal_watcher;
  ev_signal_init (&signal_watcher, sigint_cb, SIGINT);
  ev_signal_start (loop, &signal_watcher);


	// initialise a timer watcher, then start it
	// simple non-repeating 5.5 second timeout
	//ev_timer_init (&timeout_watcher, timeout_cb, 5.5, 0.);
	//ev_timer_start (loop, &timeout_watcher);

	// now wait for events to arrive
	ev_loop (loop, 0);

	puts("cya");
	close(sd);

	// unloop was called, so exit
	return 0;
}