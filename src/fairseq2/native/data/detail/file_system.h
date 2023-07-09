// Copyright (c) Meta Platforms, Inc. and affiliates.
// All rights reserved.
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <optional>
#include <string>
#include <vector>

#include "fairseq2/native/data/data.h"

namespace fairseq2::detail {

data_list
list_files(const std::string &pathname, const std::optional<std::string> &pattern);

}
