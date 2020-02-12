#include "iplanner/utils/async_loader.h"

#include <iostream>

namespace iplanner
{
AsyncLoader::AsyncLoader() = default;

AsyncLoader::~AsyncLoader() = default;


// Copy constructors
void AsyncLoader::Copy(const AsyncLoader& rhs)
{
  switch (rhs.load_status_)
  {
  case LoadStatus::WAITING:
    load_status_ = LoadStatus::NOT_LOADED;
    filename_ = "";
    break;

  default:
    load_status_ = rhs.load_status_;
    filename_ = rhs.filename_;
  }
}

AsyncLoader::AsyncLoader(const AsyncLoader& rhs)
{
  Copy(rhs);
}

AsyncLoader& AsyncLoader::operator = (const AsyncLoader& rhs)
{
  Copy(rhs);
  return *this;
}


// Move constructors
void AsyncLoader::Move(AsyncLoader&& rhs) noexcept
{
  filename_ = std::move(rhs.filename_);
  load_status_ = rhs.load_status_;
  future_load_async_ = std::move(rhs.future_load_async_);

  rhs.load_status_ = LoadStatus::NOT_LOADED;
}

AsyncLoader::AsyncLoader(AsyncLoader&& rhs) noexcept
{
  Move(std::move(rhs));
}

AsyncLoader& AsyncLoader::operator = (AsyncLoader&& rhs) noexcept
{
  Move(std::move(rhs));
  return *this;
}


bool AsyncLoader::Load(const std::string& filename)
{
  // When asynchronous load is active, it can't be stopped and return reporting failure
  if (load_status_ == LoadStatus::WAITING)
  {
    std::cout << "AsyncLoader: Load() called while being loaded asynchronously" << std::endl;
    return false;
  }

  auto success = LoadFunc(filename);

  if (success)
    load_status_ = LoadStatus::LOADED;
  else
    load_status_ = LoadStatus::FAILED;

  return success;
}

void AsyncLoader::LoadAsync(const std::string& filename)
{
  filename_ = filename;
  load_status_ = LoadStatus::WAITING;
  future_load_async_ = std::async(std::launch::async, &AsyncLoader::LoadAsyncInternal, this, filename);
}

bool AsyncLoader::IsReady()
{
  return load_status_ == LoadStatus::WAITING &&
    future_load_async_.wait_for(std::chrono::duration<double>(0.)) == std::future_status::ready;
}

void AsyncLoader::Get()
{
  if (IsReady())
  {
    auto success = future_load_async_.get();
    if (success)
      load_status_ = LoadStatus::LOADED;
    else
      load_status_ = LoadStatus::FAILED;
  }
}
}
