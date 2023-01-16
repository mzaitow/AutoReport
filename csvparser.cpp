#include "csvparser.h"
#include <iostream>

using namespace std;

CSVParser::CSVParser(const char *filename)
  : file(filename),
    m_fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()),
    i(m_fileContent.begin()) {
    getContent();
}

const string &CSVParser::getCellContent() {
  return m_cellContent;
}

void CSVParser::getContent() {
  m_cellContent.clear();
  int ch = *i;
  char termChar;

  if(ch == '"') {
      termChar = '"';
  } else {
      termChar = ',';
  }

  while(i != m_fileContent.end() && (ch = *i) != termChar) {
    if(ch == '\n') {
      i -= 1;
    
      return;
    }

    m_cellContent.push_back(static_cast<char>(ch));
    ++i;
  }


}

void CSVParser::nextColumn() {
  if(*i != ',' && *i != '\n')
    throw string("Expected comma-separator");

  ++i;
  getContent();
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
  while(i != m_fileContent.end() && *i != '\n') ++i;
  getForwardCellContent();
}
