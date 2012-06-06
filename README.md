Catalyst, a fast SPDY server
============================

Catalyst is a SPDY server implemented in C. It is meant to be used as sort of a reverse proxy, as it talks to the backend via ZeroMQ. While the performance-critical part of frontend servers is implemented in native C, backends can be written in any language, using the ZeroMQ protocol for communication.

Catalyst is meant to run on Unix and Unix-like systems, and uses an asynchronous event loop courtesy of libev.