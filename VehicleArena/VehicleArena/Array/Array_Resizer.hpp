// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Array_Forward.hpp>
#include <VehicleArena/Array/Array_Shape.hpp>
#include <VehicleArena/Os/Os.hpp>
#include <functional>
#include <iostream>

namespace VA {

class ArrayResizer {
    std::function<void(const ArrayShape&)> resize_;
    ArrayShape shape_;
    bool done_;
    ArrayResizer& operator = (const ArrayResizer&) = delete;
public:
    explicit ArrayResizer(std::function<void(ArrayShape)> resize):
        resize_(resize),
        done_(true) {}
    ~ArrayResizer() {
        if (!done_) {
            verbose_abort("Missing () in array.{resize|reshape}");
        }
    }
    ArrayResizer& operator [] (size_t size) {
        if (done_) {
            shape_.clear();
            done_ = false;
        }
        shape_.append(size);
        return *this;
    }
    void operator [] (const ArrayShape& shape) {
        if (shape.ndim() > 0) {
            (*this)[shape(0)];
            (*this)[shape.erased_first()];
        }
    }
    void operator () (size_t size) {
        (*this)[size];
        (*this)();
    }
    void operator () (size_t size0, size_t size1) {
        (*this)[size0];
        (*this)[size1];
        (*this)();
    }
    void operator () (const ArrayShape& shape) {
        (*this)[shape];
        (*this)();
    }
    void operator () () {
        resize_(shape_);
        done_ = true;
    }
};

}
