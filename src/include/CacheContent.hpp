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
#ifndef CACHE_CONTENT_H_
#define CACHE_CONTENT_H_

#include <string>

/**
 * 包裝實際的檔案
 * 讓其具有過期以及讀檔狀態的概念
 * Example:
 * CacheContent cc = hash.find("some/file.txt");
 * if (cc != hash.end()) {
 *   if (cc.isValid()) callback(cc);
 *   if (cc.isExpired()) doOpenFile();
 *   if (cc.isPending()) callbackAfterFileOpened();
 * }
 *
 */

class CacheContent {
 public:
  CacheContent();
  // copyable
  CacheContent(const CacheContent& other) = default;
  // movable
  CacheContent& operator=(const CacheContent&) = default;
  ~CacheContent();
  // get the content variable
  const std::string getContent();
  // is_ready and is not expired
  bool isValid();
  // not is_ready and is not expired
  bool isPending();
  // read_at is too old
  bool isExpired();

 private:
  // the content, the worker need to read the file and
  // put it here.
  std::string* content;
  // if the worker done read, call the callback and set is_ready
  // to true.
  bool is_ready;
  // time start reading file, if it's too old (expired) then
  // the isValid function should return false and we need to
  // re-load the file into cache.
  time_t read_at;
};

#endif
