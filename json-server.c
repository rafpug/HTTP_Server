#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int server = -1;

void cleanup() {
   close(server);
   printf("Server exiting cleanly.\n");
   exit(0);
}


int main(int argc, char *argv[]){

   struct sigaction sa = {0};
   sa.sa_handler = cleanup;
   sigaction(SIGINT, &sa, NULL);

   server = socket(AF_INET6, SOCK_STREAM, 0);
   if (server < 0) {
      perror("Error in creating socket");
      return 1;
   }   

   int off = 0;
   int ret = setsockopt(server, IPPROTO_IPV6, IPV6_V6ONLY, &off, sizeof(off));
   if (ret < 0) {
      perror("Error in setsockopt");
      close(server);
      return 1;
   }

   struct sockaddr_in6 addr = {0};
   addr.sin6_family = AF_INET6;
   if (argc == 2) {
      struct in6_addr inputted_addrv4;
      struct in6_addr inputted_addrv6;
      char ipv4_converted[256];
      
      ret = inet_pton(AF_INET6,argv[1],&inputted_addrv6);
      int ret2 = snprintf(ipv4_converted, sizeof(ipv4_converted), "::fff:%s", argv[1]);
      int ret3;
      
      if (ret2 >= sizeof(ipv4_converted)) {
         ret3 = 0;
      } else {
         ret3 = inet_pton(AF_INET6,ipv4_converted,&inputted_addrv4);
      }

      if (ret == 1) {
         addr.sin6_addr = inputted_addrv6;
      } else if (ret3 == 1) {
         addr.sin6_addr = inputted_addrv4;
      } else {
         addr.sin6_addr = in6addr_any;
      }      
   } else {
      addr.sin6_addr = in6addr_any;
   }
   
   int binded = bind(server, (struct sockaddr *)&addr, sizeof(addr));
   if ( binded < 0) {
      perror("Error in binding");
      close(server);
      return 1;
   }

   struct sockaddr_in6 binded_addr = {0};
   socklen_t len = sizeof(binded_addr);
   ret = getsockname(server, (struct sockaddr *) &binded_addr, &len);
   if ( ret == -1) {
      perror("getsockname failed");
      close(server);
      return 1;
   }

   printf("HTTP server is using TCP port %u\nHTTPS server is using TCP port -1\n", ntohs(binded_addr.sin6_port)); 
   
   ret = listen(server, SOMAXCONN);
   if (ret < 0) {
      perror("listen failed");
      close(server);
      return 1;
   }

   while (1){
      //nothing
   }
      
   close(server);
   printf("test");
   return 0;
}
