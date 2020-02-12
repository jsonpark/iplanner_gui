#ifndef LAZYGL_UTILS_ASYNC_LOADER_H_
#define LAZYGL_UTILS_ASYNC_LOADER_H_

#include <string>
#include <future>

namespace iplanner
{
class AsyncLoader
{
private:
  enum class LoadStatus
  {
    NOT_LOADED,
    WAITING,
    LOADED,
    FAILED,
  };

public:
  AsyncLoader();

  ~AsyncLoader();

  // Copy constructors do not copy the asynchronously loading texture
  void Copy(const AsyncLoader& rhs);
  AsyncLoader(const AsyncLoader& rhs);
  AsyncLoader& operator = (const AsyncLoader& rhs);

  // Move constructors
  void Move(AsyncLoader&& rhs) noexcept;
  AsyncLoader(AsyncLoader&& rhs) noexcept;
  AsyncLoader& operator = (AsyncLoader&& rhs) noexcept;

  virtual bool LoadFunc(const std::string& filename) = 0;

  bool Load(const std::string& filename);

  const auto& GetFilename() const
  {
    return filename_;
  }

  auto GetFilename()
  {
    return filename_;
  }

  void LoadAsync(const std::string& filename);

  bool IsReady();
  void Get();

  bool IsLoadFinished() const noexcept
  {
    return IsLoaded() || IsLoadFailed();
  }

  bool IsLoaded() const noexcept
  {
    return load_status_ == LoadStatus::LOADED;
  }

  bool IsLoadFailed() const noexcept
  {
    return load_status_ == LoadStatus::FAILED;
  }

private:
  bool LoadAsyncInternal(const std::string& filename)
  {
    return LoadFunc(filename);
  }

  std::string filename_;
  LoadStatus load_status_ = LoadStatus::NOT_LOADED;
  std::future<bool> future_load_async_;
};
}

#endif // LAZYGL_UTILS_CONVERT_GL_H_
