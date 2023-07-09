// Copyright (c) Meta Platforms, Inc. and affiliates.
// All rights reserved.
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include "fairseq2/native/extension/module.h"

#include <string_view>

#include <fmt/core.h>

#include <fairseq2/native/data/immutable_string.h>
#include <fairseq2/native/utils/string.h>

namespace py = pybind11;

namespace fairseq2 {

void
def_string(py::module_ &data_module)
{
    py::module_ m = data_module.def_submodule("string");

    // CString
    py::class_<immutable_string>(m, "CString")
        .def(py::init<>())
        .def(py::init<std::string_view>(), py::arg("s"))

        // To be consistent with str, we return the UTF-8 code point length
        // instead of the byte length.
        .def("__len__", &immutable_string::get_code_point_length)

        .def(py::self == py::self)  // NOLINT(misc-redundant-expression)
        .def(py::self != py::self)  // NOLINT(misc-redundant-expression)

        // Equality check with other string-likes.
        .def(
            "__eq__",
            [](const immutable_string &lhs, std::string_view rhs)
            {
                return static_cast<std::string_view>(lhs) == rhs;
            },
            py::is_operator{})
        .def(
            "__ne__",
            [](const immutable_string &lhs, std::string_view rhs)
            {
                return static_cast<std::string_view>(lhs) != rhs;
            },
            py::is_operator{})

        .def(py::hash(py::self))

        .def(
            "__str__",
            [](const immutable_string &self)
            {
                return static_cast<std::string_view>(self);
            })
        .def(
            "__repr__",
            [](const immutable_string &self)
            {
                return fmt::format("CString('{}')", self);
            })

        .def(
            "bytes",
            [](const immutable_string &self)
            {
                return py::bytes(static_cast<std::string_view>(self));
            })

        .def(
            "lstrip",
            [](const immutable_string &self)
            {
                return ltrim(self);
            })
        .def(
            "rstrip",
            [](const immutable_string &self)
            {
                return rtrim(self);
            })
        .def(
            "split",
            [](const immutable_string &self, std::string_view sep)
            {
                if (sep.size() != 1)
                    throw std::invalid_argument{"`sep` must be of length 1."};

                return self.split(sep[0]);
            },
            py::arg("sep") = '\t')

        .def(
            py::pickle(
                [](const immutable_string &self)
                {
                    return static_cast<std::string_view>(self);
                },
                [](std::string_view s)
                {
                    return immutable_string{s};
                }));

    py::implicitly_convertible<std::string_view, immutable_string>();
}

}  // namespace fairseq2
