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
   exit(0);
}


int main(int argc, char *arv[]){

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
   addr.sin6_addr = in6addr_any;
   
   int binded = bind(server, (struct sockaddr *)&addr, sizeof(addr));
   if ( binded < 0) {
      perror("Error in binding");
      close(server);
      return 1;
   }
   
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
