#pragma once
#include <cstdio>
#include <ctime>
#include <string>
#include <exception>
#include <utility>
#include <type_traits>
#include <list>

#if defined(_MSC_VER)
#pragma warning(disable:4996)
#endif

namespace minatsuki {
  using std::string;
  using std::exception;
  using std::move;
  using std::is_same;
  using std::is_base_of;
  using std::list;

  class Decorator {
  public:
    virtual ~Decorator() {}
    virtual const char *Head() = 0;
    virtual const char *Tail() = 0;
  };

  class StandardDecorator : public Decorator {
  public:
    const char *Head() override {
      auto now = time(nullptr);
      return ctime(&now);
    }

    const char *Tail() override {
      return "\n";
    }
  };

  class Writer {
  public:
    virtual ~Writer() {}
    virtual bool Write(const char *, size_t) = 0;
    virtual bool Write(string &) = 0;
    virtual bool Write(string &&) = 0;
  };

  class StandardWriter : public Writer {
  protected:
    FILE *ptr_;
  public:
    virtual ~StandardWriter();
    StandardWriter() = delete;
    StandardWriter(FILE *ptr) : ptr_(ptr) {}
    StandardWriter(const char *path, const char *mode) :
      ptr_(fopen(path, mode)) {}
    StandardWriter(string path, string mode) :
      ptr_(fopen(path.c_str(), mode.c_str())) {}
    StandardWriter(StandardWriter &) = delete;
    StandardWriter(StandardWriter &&rhs) :
      ptr_(rhs.ptr_) { rhs.ptr_ = nullptr; }

    void operator=(StandardWriter &) = delete;
    void operator=(StandardWriter &&rhs);
    virtual bool Write(const char *data, size_t size = 0) override;
    virtual bool Write(string &data) override;
    virtual bool Write(string &&data) override { return Write(data); }
  };

  class Agent {
  public:
    virtual ~Agent() {}
  };

  template <
    typename _Decorator = StandardDecorator,
    typename _Writer = StandardWriter>
  class CacheAgent : public Agent {
    static_assert(is_base_of<Decorator, _Decorator>::value, "Decorator error");
    static_assert(is_base_of<Writer, _Writer>::value, "Writer error");
  protected:

  };

  template <
    typename _Decorator = StandardDecorator,
    typename _Writer = StandardWriter>
  class RealTimeAgent : public Agent {
    static_assert(is_base_of<Decorator, _Decorator>::value, "Decorator error");
    static_assert(is_base_of<Writer, _Writer>::value, "Writer error");
  protected:

  };


}