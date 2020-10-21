# Mandelbrot Project - CS455 System Programing

Project to explore interprocess communication. 

Client Application which displays the Mandelbrot set with the current functionality and the following modifications.

- The client can either use a network socket with a given ip address and port or unix domain socket with a specified file
- The client will use UDP to make requests to a server which calculates regions of the Mandelbrot set.  The request will provide real_min, real_max, imaginary_min,imaginary_max, steps_real,steps_imaginary
- The client will have a mechanism to specify the number of segments to use in the real and imaginary axes.
- The client should be robust, and use a timeout to request another packet if the time elapses.  Some mechanism should be used to ignore duplicate packets.

Server Application which listens at the UDP socket and uses a separate thread per request to  service the request.  The returned information is a packet of RGB (or HSI) data representing the Mandelbrot set in the specified region. 

The client/server (completed project) must use C and run on a POSIX system.  You will be graded on the design/development process and the documentation of the testing of the project.