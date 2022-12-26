#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <string>
#include <fstream>

class CSVParser {
private:
  std::string m_cellContent;

  void getContent(const char termChar);
  void getTermChar(const char ch);
  
public:
  std::ifstream file;

  CSVParser(const char *filename);
  const std::string &getCellContent();

  void nextRow();
  void nextColumn();

  const std::string &getNewlineCellContent();
  const std::string &getForwardCellContent();
};

#endif // CSVPARSER_H
