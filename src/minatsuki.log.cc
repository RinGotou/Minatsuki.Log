#include "minatsuki.log.h"

namespace minatsuki {
  StandardWriter::~StandardWriter() {
    if (ptr_ != nullptr && ptr_ != stdout) fclose(ptr_);
  }

  void StandardWriter::operator=(StandardWriter &&rhs) {
    if (ptr_ != nullptr) {
      fclose(ptr_);
    }
    ptr_ = rhs.ptr_;
  }

  bool StandardWriter::Write(const char *data, size_t size) {
    size_t counter = 0;
    int flag = 0;
    char const *pos = data;
    while (*pos != '\0' || (size != 0 && counter < size)) {
      flag = fputc(*pos, ptr_);
      counter += 1;
      ++pos;
      if (flag == EOF) break;
    }

    return flag != EOF;
  }

  bool StandardWriter::Write(string &data) {
    int flag = 0;
    for (const auto &unit : data) {
      flag = fputc(unit, ptr_);
      if (flag == EOF) break;
    }
    
    return flag != EOF;
  }
}