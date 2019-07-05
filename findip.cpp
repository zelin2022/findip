/*
* Filename: findip.cpp
* Author: Zelin Liu
* Date: 2019/05/22
* Description: a quick one to find ip address of any hostname, can find IPV4 and IPV6
* Instruction: compile with "gcc findip.cpp -o findip" then run using "./findip xxx" xxx is honstname such as "google.ca"
*/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char** argv){
  struct addrinfo hints, *res, *p;
  int status;
  char ipstr[INET6_ADDRSTRLEN];

  // argument count check
  if (argc != 2) {
    fprintf(stderr,"wrong argument count, run with 1 queried hostnames\n");
    return 1;
  }

  // set up hints
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6 to force version
  hints.ai_socktype = SOCK_STREAM; // some interesting results when you use SOCK_DGRAM here and test on hostnames like "google.ca", they have different ip?

  // call getaddrinfo(), 0 succeed, else failed
  if ((status = getaddrinfo(argv[1], NULL, &hints, &res)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    return 2;
  }

  printf("IP addresses for %s:\n", argv[1]);

  // res is a pointer to a linked list
  for(p = res;p != NULL; p = p->ai_next) {
    void *addr;
    char ipver[] ="     ";
    // get the pointer to the address itself,
    // different fields in IPv4 and IPv6:
    if (p->ai_family == AF_INET) { // IPv4
      struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
      addr = &(ipv4->sin_addr);
      strcpy(ipver, "IPv4");
    } else { // IPv6
      struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
      addr = &(ipv6->sin6_addr);
      strcpy(ipver, "IPv6");
    }

    // convert the IP to a string and print it:
    inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
    printf("  %s: %s\n", ipver, ipstr);
    printf("socktype:%d protocol:%d \n", p->ai_socktype, p->ai_protocol);
  }

  freeaddrinfo(res); // free the linked list

  return 0;
}
