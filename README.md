# Mandelbrot Project - CS455 System Programing

Project to explore interprocess communication and parallel processing. 

Mandy_Client Application outputs an image of the Mandelbrot set with specified center and scale to file or to pipe.

- The client uses a network socket a unix domain socket with a specified file
- The client will use UDP to make requests to a server which calculates regions of the Mandelbrot set.  The request provides center, scale, number of segments, and image size.
- The client has a mechanism to specify the number of segments to use in the real and imaginary axes.

Mandy_Server Application listens at the UDP socket and uses a separate thread per request to service the request. The returned information is a packet of RGB data representing the Mandelbrot set in the specified region. 

The client/server (completed project) uses C and runs on a POSIX system.  Unit testing is found in /testing.

## To be completed
- Fix internet sockets w/ graphics
- Implement timeout on client
