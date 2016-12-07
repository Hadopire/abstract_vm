#pragma once

#include <string>
#include <vector>

class ErrorFormatter {
  public:
    ErrorFormatter();
    ErrorFormatter(const std::string & src);
    ErrorFormatter(const ErrorFormatter &);

    ErrorFormatter & operator=(const ErrorFormatter & rhs);

    ~ErrorFormatter();

    void setSource(const std::string & src);
    const std::string format(const std::string & message, size_t line, size_t column) const;

    bool isActive() const;

  private:
    std::vector<std::string> mSplitSrc;
    bool mIsActive{false};
};
