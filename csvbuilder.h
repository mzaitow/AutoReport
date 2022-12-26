#ifndef CSVBUILDER_H
#define CSVBUILDER_H

#include <fstream>
#include <string>

class CSVBuilder {
private:
  std::ofstream m_file;
  const char m_rowSeparator;
  const char m_columnSeparator;
  std::string m_cellContent;

  struct Position {
    int row;
    int column;
  } position;
  
public:
  CSVBuilder(const char *filename);
  ~CSVBuilder();
  void nextRow();
  void nextColumn();

  void setCellContent(const std::string content);
};

#endif // CSVBUILDER_H
