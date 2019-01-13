#include "utils.h"

int utils::getStateCode(const char *buf) {
  std::string bufString = std::string(buf);
  std::istringstream ism(bufString);
  int code;
  if (ism >> code) {
    return code;
  } else {
    return -1;
  }
}

std::tuple<std::string, unsigned short> utils::getDataURL(const char *buf) {
  std::string bufString = std::string(buf);
  std::regex reg("\\(([0-9]+),([0-9]+),([0-9]+),([0-9]+),([0-9]+),([0-9]+)\\)");
  std::string hostName;
  for (int i = 1; i != 5; i++) {
    std::sregex_token_iterator it(bufString.begin(), bufString.end(), reg, i);
    hostName += it->str();
    if (i != 4) {
      hostName += ".";
    } else {
      hostName += "\r\n";
    }
  }
  std::sregex_token_iterator it5(bufString.begin(), bufString.end(), reg, 5);
  int p1 = std::stoi(it5->str());
  std::sregex_token_iterator it6(bufString.begin(), bufString.end(), reg, 6);
  int p2 = std::stoi(it6->str());
  return {hostName, p1 * 256 + p2};
}

bool utils::command(SOCKET Socket, std::string cmd, std::string &ret, int stateCode) {
  int len = 100;
  send(Socket, cmd.c_str(), int(cmd.length()), 0);
  char buf[len + 1];
  int bufLen = recv(Socket, buf, len, 0);
  buf[bufLen] = '\0';
  ret = std::string(buf);
  if (getStateCode(buf) == stateCode) {
    return true;
  } else {
    std::cout << "utils::command() : The StateCode " << getStateCode(buf) << " is Error!" << std::endl;
    return false;
  }
}

int utils::recvCode(SOCKET Socket) {
  int len = 100;
  char buf[len + 1];
  int bufLen = recv(Socket, buf, len, 0);
  buf[bufLen] = '\0';
  return getStateCode(buf);
}

bool utils::getStringData(SOCKET Socket, std::string &ret) {
  int len = 256;
  char buf[len + 1];
  int recvBytes;
  ret = std::string("");
  while ((recvBytes = recv(Socket, buf, len, 0)) > 0) {
    buf[recvBytes] = '\0';
    ret += buf;
  }
  return true;
}

bool utils::listParser(const std::string &listString, std::vector<struct fileAttr> &fileList) {
  std::istringstream iss(listString);
  std::string x;
  fileList = std::vector<struct fileAttr>(0);
  while (iss >> x && std::regex_match(x, std::regex(std::string("\\d\\d-\\d\\d-\\d\\d")))) {
    if (x.length() != 8) {
      break;
    }
    struct fileAttr tmp;
    strcpy(tmp.date, x.c_str());
    iss >> x;
    strcpy(tmp.time, x.c_str());
    iss >> x;
    if (x == "<DIR>") {
      tmp.isDIR = true;
      tmp.size = 0;
    } else {
      tmp.isDIR = false;
      tmp.size = std::stoi(x);
    }
    iss >> x;
    tmp.name = x;
    fileList.push_back(tmp);
  }
  return true;
}

