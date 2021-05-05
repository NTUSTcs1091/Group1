/**
 * MIT License
 *
 * Copyright (c) 2021 Tsuhao Chang
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef CACHE_MANAGER_H_
#define CACHE_MANAGER_H_

#include <functional>
#include <map>
#include <mutex>
#include <string>

#include "CacheContent.hpp"

/**
 * Cache管理
 * 維護一key to file的hash並處理race condition
 * Example:
 * CacheManager cm = new CacheManager();
 * cm.get("some/file.txt", callback);
 */

class CacheManager {
 public:
  CacheManager();
  // not copyable
  CacheManager(const CacheManager &other) = delete;
  // not movable
  CacheManager &operator=(const CacheManager &) = delete;
  ~CacheManager();
  // get singleton instance
  static CacheManager *getInstance();
  // 如果該key存在但其CacheContent太舊，reload
  // 如果該key存在但尚未ready，則其尚在讀檔，這段不確定該如何處理讓正在讀檔的worker知道讀檔完須多call一隻callback
  // 如果該key存在且ready，回傳其內容
  void get(const std::string key,
           const std::function<void(const CacheContent &)> callback) const;

 private:
  // singleton
  static CacheManager *instance;
  // mapping key (request) to file
  std::map<std::string, CacheContent *> hash;
  // lock
  std::mutex mtx;
};

#endif
