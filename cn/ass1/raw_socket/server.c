#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 12345
#define BUF_SIZE 1024

unsigned short checksum(void *b, int len) {
  unsigned short *buf = b;
  unsigned int sum = 0;
  unsigned short result;
  for (sum = 0; len > 1; len -= 2)
    sum += *buf++;
  if (len == 1)
    sum += *(unsigned char *)buf;
  sum = (sum >> 16) + (sum & 0xFFFF);
  sum += (sum >> 16);
  result = ~sum;
  return result;
}

int main() {
  int sockfd;
  struct sockaddr_in client_addr;
  char buffer[BUF_SIZE];
  char packet[BUF_SIZE];

  sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
  if (sockfd < 0) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  int one = 1;
  const int *val = &one;
  if (setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, val, sizeof(one)) < 0) {
    perror("Error setting IP_HDRINCL");
    exit(EXIT_FAILURE);
  }

  memset(&client_addr, 0, sizeof(client_addr));
  client_addr.sin_family = AF_INET;
  client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  printf("Server ready to receive messages\n");
  char messageSent[BUF_SIZE];

  while (1) {
    memset(buffer, 0, BUF_SIZE);
    struct sockaddr_in sender;
    socklen_t sender_len = sizeof(sender);

    ssize_t bytes_received = recvfrom(sockfd, buffer, BUF_SIZE, 0,
                                    (struct sockaddr *)&sender, &sender_len);

    if (bytes_received < 0) {
      perror("recvfrom failed");
      continue;
    }

    struct iphdr *ip_header = (struct iphdr *)buffer;
    int ip_header_len = ip_header->ihl * 4;
    char *payload = buffer + ip_header_len;

    if(strcmp(payload, messageSent)!=0)
    printf("Client (%s): %s\n", inet_ntoa(sender.sin_addr), payload);

    if (strncmp(payload, "exit", 4) == 0) {
      printf("Client disconnected.\n");
      break;
    }

    printf("Enter your response: ");
    char response[BUF_SIZE - sizeof(struct iphdr)];
    fgets(response, sizeof(response), stdin);
    response[strcspn(response, "\n")] = '\0';

    if (strcmp(response, "exit") == 0) {
      printf("Server disconnected.\n");
      break;
    }

    memset(packet, 0, BUF_SIZE);
    struct iphdr *resp_ip = (struct iphdr *)packet;

    resp_ip->ihl = 5;
    resp_ip->version = 4;
    resp_ip->tos = 0;
    resp_ip->id = htonl(12345);
    resp_ip->frag_off = 0;
    resp_ip->ttl = 255;
    resp_ip->protocol = IPPROTO_RAW;
    resp_ip->saddr = inet_addr("127.0.0.1");
    resp_ip->daddr = sender.sin_addr.s_addr;

    strcpy(packet + sizeof(struct iphdr), response);
    int payload_len = strlen(response);
    int total_len = sizeof(struct iphdr) + payload_len;
    resp_ip->tot_len = htons(total_len);

    resp_ip->check = 0;
    resp_ip->check = checksum((unsigned short *)packet, sizeof(struct iphdr));

    if (sendto(sockfd, packet, total_len, 0, (struct sockaddr *)&sender,
             sender_len) < 0) {
      perror("sendto failed");
      continue;
    }

    printf("Response sent: %s\n", response);
    strcpy(messageSent, response);
  }

  close(sockfd);
  return 0;
}
