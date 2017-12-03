#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MIN_PORT 1024 + 1
#define MAX_PORT 64000 - 1
#define NUM_SERVERS 3
#define SERVER_PORT_FILE "server_port"
#define CLIENT_PORT_FILE "http_port"
#define HTTP_EOP "\r\n\r\n\0"

short randPort(short minPort, short maxPort) { return rand() % (maxPort - minPort) + minPort; }

short bindSocket(int *socket_fd)
{
  short port = 0;
  const int enable = 1;
  struct sockaddr_in addr;

  *socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  setsockopt(*socket_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  do {
    port = randPort(MIN_PORT, MAX_PORT);
    addr.sin_port = htons(port);
  } while (bind(*socket_fd, (const struct sockaddr *)&addr, sizeof(addr)) < 0);

  return port;
}

void writePortToFile(char *path, short value)
{
  FILE *file = fopen(path, "w");
  fprintf(file, "%d", value);
  fclose(file);
}

void initSocket(int *socket_fd, char *file_path)
{
  short port = bindSocket(socket_fd);
  writePortToFile(file_path, port);
  listen(*socket_fd, SOMAXCONN);
}

void connectServers(int socket_fd, int *server_handles)
{
  int i;
  for (i = 0; i < NUM_SERVERS; i++) {
    server_handles[i] = accept(socket_fd, NULL, NULL);
  }
}

char *subString(char *string, const char *substring, char *boundary)
{
  char *ptr = strstr(string, substring);
  return ptr != NULL && ptr < boundary ? ptr : NULL;
}

char *receiveHTTP(int connection, bool is_response)
{
  char *http_delimiter_ptr = NULL;
  char *buff = NULL;
  int recvd = 0;
  int buff_size = 1;
  bool done = false;

  buff = realloc(NULL, buff_size);
  memset(buff, '\0', buff_size);

  while (!done) {
    recvd += recv(connection, buff + recvd, buff_size - recvd, 0);

    http_delimiter_ptr = subString(buff, HTTP_EOP, buff + recvd);
    if (is_response && http_delimiter_ptr) {
      http_delimiter_ptr = subString(http_delimiter_ptr + strlen(HTTP_EOP), HTTP_EOP, buff + recvd);
    }
    done = http_delimiter_ptr != NULL;

    if (recvd == buff_size) {
      buff_size *= 2;
      buff = realloc(buff, buff_size);
      memset(buff + recvd, '\0', buff_size - recvd);
    }
  }
  return buff;
}

int writeToSocket(char *string, int connection)
{
  int msg_len = strlen(string);
  int bytes_sent = 0;

  while (bytes_sent < msg_len) {
    bytes_sent += write(connection, string, msg_len - bytes_sent);
  }

  return bytes_sent;
}

void handleClients(int client_fd, int *server_handles)
{
  char *request = NULL;
  char *response = NULL;
  int client_handle = 0;
  int current_server = -1;

  while (true) {
    client_handle = accept(client_fd, NULL, NULL);
    current_server = (current_server + 1) % NUM_SERVERS;

    request = receiveHTTP(client_handle, false);
    writeToSocket(request, server_handles[current_server]);

    response = receiveHTTP(server_handles[current_server], true);
    writeToSocket(response, client_handle);

    free(request);
    free(response);
  }
}

int main()
{
  int server_fd = 0;
  int client_fd = 0;
  int server_handles[NUM_SERVERS];

  srandom(time(NULL));

  initSocket(&server_fd, SERVER_PORT_FILE);
  initSocket(&client_fd, CLIENT_PORT_FILE);

  connectServers(server_fd, server_handles);
  handleClients(client_fd, server_handles);

  close(server_fd);
  close(client_fd);
  return 0;
}