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

#define DEFAULT_OUTPUT "Minatsuki.log"

namespace minatsuki {
  using std::string;
  using std::exception;
  using std::move;
  using std::is_same;
  using std::is_base_of;
  using std::list;

  using CharList = list<char>;

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
    virtual ~StandardWriter() {
      if (ptr_ != nullptr && ptr_ != stdout) fclose(ptr_);
    }

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
    virtual bool WriteLine(const char *, size_t) = 0;
    virtual bool WriteLine(string &) = 0;
    virtual bool WriteLine(string &&) = 0;
    virtual bool WriteLine(exception *e) = 0;
  };

  template <
    typename _Decorator = StandardDecorator,
    typename _Writer = StandardWriter>
  class CacheAgent : public Agent {
    static_assert(is_base_of<Decorator, _Decorator>::value, "Decorator error");
    static_assert(is_base_of<Writer, _Writer>::value, "Writer error");
  protected:
    const char *dest_;
    const char *mode_;
    list<CharList> cache_;
    _Decorator decorator_;

  protected:
    void CopyToCache(const char *data, size_t size);
    void CopyToCache(string &data);

  public:
    virtual ~CacheAgent();

    CacheAgent() : 
      dest_(DEFAULT_OUTPUT),
      mode_("a+"), cache_(), decorator_() {}

    CacheAgent(const char *dest, const char *mode) :
      dest_(dest), mode_(mode), cache_(), decorator_() {}

    CacheAgent(string dest, string mode) :
      dest_(dest.c_str()), mode_(mode.c_str()),
      cache_(), decorator_() {}

    bool WriteLine(const char *data, size_t size = 0) override;
    bool WriteLine(string &data) override;
    bool WriteLine(string &&data) override { return WriteLine(data); }
    bool WriteLine(exception *e) override;
  };

  template<typename _Decorator, typename _Writer>
  void CacheAgent<_Decorator, _Writer>::CopyToCache(const char *data, size_t size) {
    size_t counter = 0;
    char const *pos = data;

    cache_.push_back(CharList());

    auto &dest_list = cache_.back();

    while (*pos != '\0' || (size != 0 && counter < size)) {
      dest_list.push_back(*pos);
      ++pos;
      counter += 1;
    }
  }

  template<typename _Decorator, typename _Writer>
  void CacheAgent<_Decorator, _Writer>::CopyToCache(string &data) {
    cache_.push_back(CharList());

    auto &dest_list = cache_.back();

    for (const auto &unit : data) {
      dest_list.push_back(*pos);
    }
  }

  template<typename _Decorator, typename _Writer>
  CacheAgent<_Decorator, _Writer>::~CacheAgent() {

  }

  template<typename _Decorator, typename _Writer>
  bool CacheAgent<_Decorator, _Writer>::WriteLine(const char *data, size_t size) {
    CopyToCache(data, size);
    return true;
  }

  template<typename _Decorator, typename _Writer>
  bool CacheAgent<_Decorator, _Writer>::WriteLine(string &data) {
    CopyToCache(data);
    return true;
  }

  template<typename _Decorator, typename _Writer>
  bool CacheAgent<_Decorator, _Writer>::WriteLine(exception *e) {
    CopyToCache(e->what());
    return true;
  }

  template <
    typename _Decorator = StandardDecorator,
    typename _Writer = StandardWriter>
  class RealTimeAgent : public Agent {
    static_assert(is_base_of<Decorator, _Decorator>::value, "Decorator error");
    static_assert(is_base_of<Writer, _Writer>::value, "Writer error");
  protected:

  };


}