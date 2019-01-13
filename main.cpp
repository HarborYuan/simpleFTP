#include "socket.h"
int main() {
  Socket ftp = Socket(std::string("127.0.0.1"), 21);
  if (ftp.isConnected) {
    std::cout << "Successful connected to the server." << std::endl;
  } else {
    std::cout << "Error when connecting to the server." << std::endl;
    return 1;
  }
  ftp.login("anonymous", "123345678@qq.com");
  ftp.list("/");
  ftp.getFile("/file2.txt");
  ftp.getFile("/dir1/file1.1.txt");
  ftp.~Socket();
  return 0;
}