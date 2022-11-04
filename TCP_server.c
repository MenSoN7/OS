#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

void main() {
  //дескрипторы для пассивного(слушающего) и присоединенного сокетов
  int sockfd, newsockfd;
  int clilen, n;
  // переменная для передачи сообщений
  char line[1000];
  //структуры для размещения полных адресов сервера и клиента
  struct sockaddr_in servaddr, cliaddr;
  //создаем TCP-сокет
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror(NULL);
    close(sockfd);
    exit(1);
  }
  //обнуляем всю структуру перед заполнением
  memset(&servaddr, 0, sizeof(servaddr));
  //заполняем структуру для адреса сервера
  servaddr.sin_family = AF_INET; //семейство
  servaddr.sin_port = htons(51000);//номер порта
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  //связываем созданный сокет с адресом (3 параметр - размер структуры в байтах)
  if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    perror(NULL);
    close(sockfd);
    exit(1);
  }
  //переводим сокет в пассивное состояние
  // второй параметр - кол-во одновременных подключений
  if (listen(sockfd, 1) < 0) {
    perror(NULL);
    close(sockfd);
    exit(1);
  }
  // заносим в clilen максимальную длину ожидаемого адреса клиента
  clilen = sizeof(cliaddr);
  // принимаем соединение на сокете
  // при нормальном завершении в структуре будет адрес, а в clilen - фактическая длина
  if ((newsockfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen)) < 0) {
    perror(NULL);
    close(sockfd);
    exit(1);
  }
  
  //порождаем новый процесс
  pid_t pid = fork();
  if (pid == 0) //дочерний процесс - читает и выводит присылаемые сообщения 
    for (;;) {
      //обнулить строку перед чтением
      memset(line, 0, 1000);
      //принимаем данные
      while ((n = read(newsockfd, line, 999)) > 0) {
        printf("\nClient: %s\n", line);
      }
      if (n < 0) {
        perror(NULL);
        close(sockfd);
        close(newsockfd);
        exit(1);
      }
    }  
  else  // родительский процесс - отправляет сообщения
    for (;;) {
      printf("\nServer: ...\n");
      fflush(stdin);
      //обнуляем перед записью
      memset(line, 0, 1000);
      //заносим в line сообщение
      fgets(line, 1000, stdin);
      //отправляем данные
      while ((n = write(newsockfd, line, strlen(line) + 1)) < 0) {       
        perror(NULL);
        close(sockfd);
        close(newsockfd);
        exit(1);
      }        
  } 
}
