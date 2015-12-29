// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EXPERIMENTAL_VARIANT_VARIANT_HPP
#define EXPERIMENTAL_VARIANT_VARIANT_HPP

#include <cstddef>
#include <initializer_list>
#include <type_traits>
#include <utility>

#include <experimental/tuple.hpp>
#include <experimental/variant/detail/index_visitor.hpp>
#include <experimental/variant/detail/type_traits.hpp>
#include <experimental/variant/detail/union.hpp>
#include <experimental/variant/detail/unsafe/get.hpp>
#include <experimental/variant/detail/unsafe/visit.hpp>
#include <experimental/variant/detail/vtable.hpp>
#include <experimental/variant/in_place.hpp>
#include <experimental/variant/monostate.hpp>

namespace std {
namespace experimental {

template <typename... Ts>
class variant;

namespace detail {

template <typename... Ts>
class variant_base {
  using alternatives = experimental::variant<Ts...>;
  using storage =
      union_<conjunction<is_trivially_destructible<Ts>...>{}, Ts...>;

  public:
  constexpr bool corrupted_by_exception() const noexcept {
    return index() == tuple_not_found;
  }

  constexpr size_t index() const noexcept { return index_; }

  protected:
  variant_base() : index_(tuple_not_found), storage_() {}

  template <size_t I, typename... Args>
  constexpr variant_base(size_constant<I> index, Args &&... args)
      : index_(index), storage_(index, forward<Args>(args)...) {
    static_assert(I < experimental::tuple_size<alternatives>{}, "");
    using T = experimental::tuple_element_t<I, alternatives>;
    static_assert(is_constructible<T, Args &&...>{}, "");
  }

  variant_base(const variant_base &) = default;
  variant_base(variant_base &&) = default;

  ~variant_base() = default;

  variant_base &operator=(const variant_base &) = default;
  variant_base &operator=(variant_base &&) = default;

  template <size_t I, typename... Args>
  void construct(Args &&... args) {
    storage_.construct(size_constant<I>{}, forward<Args>(args)...);
    index_ = I;
  }

  template <size_t I>
  void destroy() noexcept {
    storage_.destroy(size_constant<I>{});
    index_ = tuple_not_found;
  }

  size_t index_;
  storage storage_;

  friend unsafe::get_impl;
};  // variant_base

template <bool IsTriviallyCopyable,
          bool IsTriviallyDestructible,
          typename... Ts>
class variant_impl_;

template <typename... Ts>
class variant_impl_</* IsTriviallyCopyable */ true,
                    /* IsTriviallyDestructible */ true,
                    Ts...> : public variant_base<Ts...> {
  using super = variant_base<Ts...>;

  protected:
  using super::super;

  variant_impl_() = default;

  variant_impl_(const variant_impl_ &) = default;
  variant_impl_(variant_impl_ &&) = default;

  ~variant_impl_() = default;

  variant_impl_ &operator=(const variant_impl_ &) = default;
  variant_impl_ &operator=(variant_impl_ &&) = default;

  template <size_t I, typename Arg>
  void assign(Arg &&arg) {
    emplace<I>(forward<Arg>(arg));
  }

  template <size_t I, typename... Args>
  void emplace(Args &&... args) {
    *this = variant_impl_(size_constant<I>{}, forward<Args>(args)...);
  }

  void swap(variant_impl_ &that) noexcept(noexcept(
      std::swap(declval<variant_impl_ &>(), declval<variant_impl_ &>()))) {
    std::swap(*this, that);
  }
};

template <typename... Ts>
class variant_impl_</* IsTriviallyCopyable */ false,
                    /* IsTriviallyDestructible */ true,
                    Ts...> : public variant_impl_<true, true, Ts...> {
  using super = variant_impl_<true, true, Ts...>;

  protected:
  using super::super;

  variant_impl_() = default;

  variant_impl_(const variant_impl_ &that) : variant_impl_{} {
    static_assert(conjunction<is_copy_constructible<Ts>...>{}, "");
    unsafe::visit(make_index_visitor<constructor>(*this), that);
  }

  variant_impl_(variant_impl_ &&that) noexcept(
      conjunction<is_nothrow_move_constructible<Ts>...>{}) {
    static_assert(conjunction<is_move_constructible<Ts>...>{}, "");
    unsafe::visit(make_index_visitor<constructor>(*this), move(that));
  }

  ~variant_impl_() = default;

  variant_impl_ &operator=(const variant_impl_ &that) {
    static_assert(conjunction<is_copy_constructible<Ts>...>{}, "");
    static_assert(conjunction<is_copy_assignable<Ts>...>{}, "");
    unsafe::visit(make_index_visitor<assigner>(*this), that);
    return *this;
  }

  variant_impl_ &operator=(variant_impl_ &&that) noexcept(
      conjunction<is_nothrow_move_constructible<Ts>...,
                  is_nothrow_move_assignable<Ts>...>{}) {
    static_assert(conjunction<is_move_constructible<Ts>...>{}, "");
    static_assert(conjunction<is_move_assignable<Ts>...>{}, "");
    unsafe::visit(make_index_visitor<assigner>(*this), move(that));
    return *this;
  }

  template <size_t I, typename Arg>
  void assign(Arg &&arg) {
    using T = experimental::tuple_element_t<I, experimental::variant<Ts...>>;
    static_assert(is_constructible<T, Arg &&>{}, "");
    static_assert(is_assignable<T &, Arg &&>{}, "");
    if (this->index() == I) {
      unsafe::get<I>(*this) = forward<Arg>(arg);
    } else {
      assign_impl<I>{*this}(forward<Arg>(arg));
    }  // if
  }

  template <size_t I, typename... Args>
  void emplace(Args &&... args) {
    this->index_ = tuple_not_found;
    this->template construct<I>(forward<Args>(args)...);
  }

  void swap(variant_impl_ &that) noexcept(
      conjunction<is_nothrow_move_constructible<Ts>...,
                  detail::is_nothrow_swappable<Ts>...>{}) {
    using namespace detail;
    if (this->index() == that.index()) {
      unsafe::visit(swapper{}, *this, that);
    } else {
      auto temp(move(*this));
      *this = move(that);
      that = move(temp);
    }  // if
  }

  private:

  template <size_t I>
  struct assign_impl {
    using T = experimental::tuple_element_t<I, experimental::variant<Ts...>>;

    void operator()(const T &arg) const {
      (*this)(arg, is_move_constructible<T>{});
    }
    void operator()(T &&arg) const { self.emplace<I>(move(arg)); }

    variant_impl_ &self;

    private:
    void operator()(const T &arg, true_type) const { (*this)(T(arg)); }
    void operator()(const T &arg, false_type) const { self.emplace<I>(arg); }
  };  // assign_impl

  template <size_t I>
  struct assigner {
    template <typename Arg>
    void operator()(Arg &&arg) const {
      self.template assign<I>(forward<Arg>(arg));
    }
    variant_impl_ &self;
  };  // assigner

  template <size_t I>
  struct constructor {
    template <typename Arg>
    void operator()(Arg &&arg) const {
      self.template construct<I>(forward<Arg>(arg));
    }
    variant_impl_ &self;
  };  // constructor

  struct swapper {
    template <typename T>
    void operator()(T &lhs, T &rhs) const {
      using std::swap;
      swap(lhs, rhs);
    }

    template <typename T, typename U>
    void operator()(T &, U &) const {
      assert(false);
    }
  };  // swapper
};

template <typename... Ts>
class variant_impl_</* IsTriviallyCopyable = */ false,
                    /* IsTriviallyDestructible = */ false,
                    Ts...> : public variant_impl_<false, true, Ts...> {
  using super = variant_impl_<false, true, Ts...>;

  protected:
  using super::super;

  variant_impl_() = default;

  variant_impl_(const variant_impl_ &) = default;
  variant_impl_(variant_impl_ &&) = default;

  ~variant_impl_() { destroy(); }

  variant_impl_ &operator=(const variant_impl_ &that) {
    if (this->index() != that.index()) {
      destroy();
    }  // if
    super::operator=(that);
    return *this;
  }

  variant_impl_ &operator=(variant_impl_ &&that) {
    if (this->index() != that.index()) {
      destroy();
    }  // if
    super::operator=(move(that));
    return *this;
  }

  template <size_t I, typename Arg>
  void assign(Arg &&arg) {
    if (this->index() != I) {
      destroy();
    }  // if
    super::template assign<I>(forward<Arg>(arg));
  }

  template <size_t I, typename... Args>
  void emplace(Args &&... args) {
    destroy();
    super::template emplace<I>(forward<Args>(args)...);
  }

  void swap(variant_impl_ &that) noexcept(noexcept(
      declval<variant_impl_ &>().super::swap(declval<variant_impl_ &>()))) {
    super::swap(that);
  }

  private:
  using super::destroy;

  template <size_t I>
  struct destructor {
    template <typename Arg>
    void operator()(Arg &) const noexcept { self.template destroy<I>(); }
    variant_impl_ &self;
  };  // destructor

  void destroy() noexcept {
    if (!this->corrupted_by_exception()) {
      unsafe::visit(make_index_visitor<destructor>(*this), *this);
    }  // if
  }
};

template <typename... Ts>
using variant_impl =
    variant_impl_<conjunction<is_trivially_copyable<Ts>...>{},
                  conjunction<is_trivially_destructible<Ts>...>{},
                  Ts...>;

template <typename... Ts>
class variant : public variant_impl<Ts...> {
  using alternatives = experimental::variant<Ts...>;
  using super = variant_impl<Ts...>;

  public:
  variant() = default;

  template <size_t I, typename... Args>
  explicit constexpr variant(in_place_index_t<I>, Args &&... args)
      : super(size_constant<I>{}, forward<Args>(args)...) {}

  template <typename T, typename... Args>
  explicit constexpr variant(in_place_type_t<T>, Args &&... args)
      : variant(in_place_index<tuple_find<T, alternatives>{}>,
                forward<Args>(args)...) {
    static_assert(tuple_count<T, alternatives>{} == 1, "");
  }

  variant(const variant &) = default;
  variant(variant &&) = default;

  ~variant() = default;

  template <typename Arg, typename T = get_best_match<Arg &&, Ts...>>
  variant &operator=(Arg &&arg) {
    assign<T>(forward<Arg>(arg));
    return *this;
  }

  variant &operator=(const variant &) = default;
  variant &operator=(variant &&) = default;

  template <size_t I, typename... Args>
  void emplace(Args &&... args) {
    super::template emplace<I>(forward<Args>(args)...);
  }

  template <typename T, typename... Args>
  void emplace(Args &&... args) {
    static_assert(tuple_count<T, alternatives>{} == 1, "");
    emplace<tuple_find<T,alternatives>{}>(forward<Args>(args)...);
  }

  private:
  template <size_t I, typename Arg>
  void assign(Arg &&arg) {
    super::template assign<I>(forward<Arg>(arg));
  }

  template <typename T, typename Arg>
  void assign(Arg &&arg) {
    static_assert(tuple_count<T, alternatives>{} == 1, "");
    assign<tuple_find<T, alternatives>{}>(forward<Arg>(arg));
  }
};

}  // namespace detail

//- 20.N.M class template `variant`:

template <typename... Ts>
class variant;

template <>
class variant<> {};

template <typename... Ts>
class variant : public detail::variant<Ts...> {
  using alternatives = variant;
  using super = detail::variant<Ts...>;

  public:
  //- 20.N.2.1 construction:
  using super::super;

  constexpr variant() : variant(in_place_index<0>) {}

  template <size_t I,
            typename U,
            typename... Args,
            typename = enable_if_t<
                is_constructible<experimental::tuple_element_t<I, alternatives>,
                                 initializer_list<U> &,
                                 Args &&...>{}>>
  explicit constexpr variant(in_place_index_t<I> ip,
                             initializer_list<U> init,
                             Args &&... args)
      : super(ip, init, forward<Args>(args)...) {}

  template <typename T,
            typename U,
            typename... Args,
            typename = enable_if_t<
                is_constructible<T, initializer_list<U> &, Args &&...>{}>>
  explicit constexpr variant(in_place_type_t<T> ip,
                             initializer_list<U> init,
                             Args &&... args)
      : super(ip, init, forward<Args>(args)...) {}

  template <typename Arg, typename T = detail::get_best_match<Arg &&, Ts...>>
  constexpr variant(Arg &&arg) : variant(in_place_type<T>, forward<Arg>(arg)) {}

  variant(const variant &that) = default;
  variant(variant &&that) = default;

  //- 20.N.2.2 destruction:
  ~variant() = default;

  //- 20.N.2.3 assignment:
  using super::emplace;
  using super::operator=;

  template <size_t I, typename U, typename... Args>
  enable_if_t<is_constructible<experimental::tuple_element_t<I, alternatives>,
                               initializer_list<U> &,
                               Args &&...>{},
  void> emplace(initializer_list<U> init, Args &&... args) {
    super::template emplace<I>(init, forward<Args>(args)...);
  }

  template <typename T, typename U, typename... Args>
  enable_if_t<is_constructible<T, initializer_list<U> &, Args &&...>{},
  void> emplace(initializer_list<U> init, Args &&... args) {
    super::template emplace<T>(init, forward<Args>(args)...);
  }

  variant &operator=(const variant &that) = default;
  variant &operator=(variant &&that) = default;

  //- 20.N.2.4 observers:
  using super::corrupted_by_exception;
  using super::index;

  //- 20.N.2.5 swap:
  using super::swap;
};  // variant

}  // namespace experimental
}  // namespace std

#endif  // EXPERIMENTAL_VARIANT_VARIANT_HPP
