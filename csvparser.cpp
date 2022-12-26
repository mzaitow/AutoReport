#include "csvparser.h"

using namespace std;

CSVParser::CSVParser(const char *filename)
  : file(filename) {
  int ch = file.get();
  
  if(ch == '"') {
    getContent('"');
  } else {
    file.seekg(-1, ios::cur);
    getContent(',');
    file.seekg(-1, ios::cur);
  }
}

const string &CSVParser::getCellContent() {
  return m_cellContent;
}

void CSVParser::getContent(const char termChar) {
  m_cellContent.clear();
  int ch;

  while((ch = file.get()) != termChar && ch != EOF) {
    if(ch == '\n') {
      file.seekg(-2, ios::cur);
    
      return;
    }

    m_cellContent.push_back(static_cast<char>(ch));
  }
}

void CSVParser::nextColumn() {
  int ch = file.get();

  if(ch != ',' && ch != '\n')
    throw string("Expected comma-separator");

  ch = file.get();
 
  if(ch == '"') {
    getContent('"');
  } else {
    file.seekg(-1, ios::cur);
    getContent(',');
    file.seekg(-1, ios::cur);
  }
}

const string &CSVParser::getNewlineCellContent() {
  nextRow();
  return getCellContent();
}

const string &CSVParser::getForwardCellContent() {
  nextColumn();
  return getCellContent();
}

void CSVParser::nextRow() {
  int ch;
  
  while((ch = file.get()) != '\n' && ch != EOF);
  file.seekg(-1, ios::cur);
  getForwardCellContent();
}
