#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED
#include <iostream>
#include <fstream>

class Logger
{
public:
  static void record(string message)
  {
    static ofstream fout("logger.log");
    fout << message << endl;;
  }
};
#endif // LOGGER_H_INCLUDED