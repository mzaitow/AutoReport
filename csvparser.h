#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <string>
#include <fstream>

class CSVParser {
private:
  std::ifstream file;

  std::string m_cellContent;
  std::string m_fileContent;
  std::string::iterator i;

  void getContent();
  void getTermChar(const char ch);
  
public:
  CSVParser(const char *filename);
  const std::string &getCellContent();

  void nextRow();
  void nextColumn();

  const std::string &getNewlineCellContent();
  const std::string &getForwardCellContent();
};

#endif // CSVPARSER_H
