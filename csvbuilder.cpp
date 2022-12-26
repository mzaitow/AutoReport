#include "csvbuilder.h"

CSVBuilder::CSVBuilder(const char *filename)
    : m_file(filename), m_rowSeparator('\n'), m_columnSeparator(',') {
  position.column = 0;
  position.row = 0;
}

void CSVBuilder::nextRow() {
  if(position.column != 0)
    m_file << m_columnSeparator;

  m_file << m_cellContent << m_rowSeparator;

  m_cellContent.clear();

  ++position.row;
  position.column = 0;
}

void CSVBuilder::nextColumn() {
  if(position.column != 0)
    m_file << m_columnSeparator;
  
  m_file << m_cellContent;
  ++position.column;

  m_cellContent.clear();
}

void CSVBuilder::setCellContent(const std::string content) {
  m_cellContent = content;
}

CSVBuilder::~CSVBuilder() {
  if(!m_cellContent.empty()) {
    if(position.column != 0)
      m_file << m_columnSeparator;
    
    m_file << m_cellContent;
  }
}
