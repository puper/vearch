/**
 * Copyright 2019 The Gamma Authors.
 *
 * This source code is licensed under the Apache License, Version 2.0 license
 * found in the LICENSE file in the root directory of this source tree.
 */

#ifndef RAW_VECTOR_FACTORY_H_
#define RAW_VECTOR_FACTORY_H_

#include "memory_raw_vector.h"
#include "mmap_raw_vector.h"
#include "raw_vector.h"

#ifdef WITH_ROCKSDB
#include "rocksdb_raw_vector.h"
#endif // WITH_ROCKSDB

#include <string>

namespace tig_gamma {

class RawVectorFactory {
public:
  static RawVector *Create(VectorStorageType type, const std::string &name,
                           int dimension, int max_doc_size,
                           const std::string &root_path,
                           const std::string &store_param) {
    StoreParams store_params;
    if (store_param != "" && store_params.Parse(store_param.c_str()))
      return nullptr;
    LOG(INFO) << "store parameters=" << store_params.ToString();
    switch (type) {
    case Memory:
      return (RawVector *)new MemoryRawVector(name, dimension, max_doc_size,
                                              root_path);
    case Mmap:
      return (RawVector *)new MmapRawVector(name, dimension, max_doc_size,
                                            root_path, store_params);
#ifdef WITH_ROCKSDB
    case RocksDB:
      return (RawVector *)new RocksDBRawVector(name, dimension, max_doc_size,
                                               root_path, store_params);
#endif // WITH_ROCKSDB
    default:
      LOG(ERROR) << "invalid raw feature type:" << type;
      return nullptr;
    }
  }
};
} // namespace tig_gamma

#endif // RAW_VECTOR_FACTORY_H_
