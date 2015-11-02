// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VARIANT_VARIANT_HPP
#define VARIANT_VARIANT_HPP

#include <cstddef>
#include <initializer_list>
#include <type_traits>
#include <utility>

#include <meta/meta.hpp>

#include <variant/detail/index_visitor.hpp>
#include <variant/detail/type_traits.hpp>
#include <variant/detail/union.hpp>
#include <variant/detail/unsafe/get.hpp>
#include <variant/detail/unsafe/visit.hpp>
#include <variant/detail/vtable.hpp>
#include <variant/in_place.hpp>
#include <variant/monostate.hpp>

namespace std {
namespace experimental {

namespace detail {

template <typename... Ts>
class variant_base {
  using alternatives = meta::list<Ts...>;
  using storage =
      union_<meta::list<Ts...>, meta::and_<is_trivially_destructible<Ts>...>{}>;

  public:
  constexpr bool corrupted_by_exception() const noexcept {
    return index() == meta::npos{};
  }

  constexpr size_t index() const noexcept { return index_; }

  protected:
  variant_base() : index_(meta::npos{}), storage_() {}

  template <size_t I, typename... Args>
  constexpr variant_base(meta::size_t<I> index, Args &&... args)
      : index_(index), storage_(index, forward<Args>(args)...) {
    static_assert(I < alternatives::size(), "");
    using T = meta::at_c<alternatives, I>;
    static_assert(is_constructible<T, Args &&...>{}, "");
  }

  variant_base(const variant_base &) = default;
  variant_base(variant_base &&) = default;

  ~variant_base() = default;

  variant_base &operator=(const variant_base &) = default;
  variant_base &operator=(variant_base &&) = default;

  template <size_t I, typename... Args>
  void construct(Args &&... args) {
    storage_.construct(meta::size_t<I>{}, forward<Args>(args)...);
    index_ = I;
  }

  template <size_t I>
  void destroy() noexcept {
    storage_.destroy(meta::size_t<I>{});
    index_ = meta::npos{};
  }

  size_t index_;
  storage storage_;

  friend unsafe::get_impl;
};  // variant_base

template <typename Ts, bool IsTriviallyCopyable, bool IsTriviallyDestructible>
class variant_impl_;

template <typename... Ts>
class variant_impl_<meta::list<Ts...>,
                    /* IsTriviallyCopyable = */ true,
                    /* IsTriviallyDestructible = */ true>
    : public variant_base<Ts...> {
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
    *this = variant_impl_(meta::size_t<I>{}, forward<Args>(args)...);
  }

  void swap(variant_impl_ &that) noexcept(noexcept(
      std::swap(declval<variant_impl_ &>(), declval<variant_impl_ &>()))) {
    std::swap(*this, that);
  }
};

template <typename... Ts>
class variant_impl_<meta::list<Ts...>,
                    /* IsTriviallyCopyable = */ false,
                    /* IsTriviallyDestructible = */ true>
    : public variant_impl_<meta::list<Ts...>, true, true> {
  using super = variant_impl_<meta::list<Ts...>, true, true>;

  protected:
  using super::super;

  variant_impl_() = default;

  variant_impl_(const variant_impl_ &that) : variant_impl_{} {
    static_assert(meta::and_<is_copy_constructible<Ts>...>{}, "");
    unsafe::visit(make_index_visitor<constructor>(*this), that);
  }

  variant_impl_(variant_impl_ &&that) noexcept(
      meta::and_<is_nothrow_move_constructible<Ts>...>{}) {
    static_assert(meta::and_<is_move_constructible<Ts>...>{}, "");
    unsafe::visit(make_index_visitor<constructor>(*this), move(that));
  }

  ~variant_impl_() = default;

  variant_impl_ &operator=(const variant_impl_ &that) {
    static_assert(meta::and_<is_copy_constructible<Ts>...>{}, "");
    static_assert(meta::and_<is_copy_assignable<Ts>...>{}, "");
    unsafe::visit(make_index_visitor<assigner>(*this), that);
    return *this;
  }

  variant_impl_ &operator=(variant_impl_ &&that) noexcept(
      meta::and_<is_nothrow_move_constructible<Ts>...,
                 is_nothrow_move_assignable<Ts>...>{}) {
    static_assert(meta::and_<is_move_constructible<Ts>...>{}, "");
    static_assert(meta::and_<is_move_assignable<Ts>...>{}, "");
    unsafe::visit(make_index_visitor<assigner>(*this), move(that));
    return *this;
  }

  template <size_t I, typename Arg>
  void assign(Arg &&arg) {
    using T = meta::at_c<meta::list<Ts...>, I>;
    static_assert(is_constructible<T, Arg &&>{}, "");
    static_assert(is_assignable<T &, Arg &&>{}, "");
    if (this->index() == I) {
      unsafe::get<I>(*this) = forward<Arg>(arg);
    } else {
      // TODO(mpark): Something feels off about this pattern. Reinvestigate.
      struct emplacer {
        void operator()(const T &arg) const { self.emplace<I>(T(arg)); }
        void operator()(T &&arg) const { self.emplace<I>(move(arg)); }
        variant_impl_ &self;
      };  // emplacer
      emplacer{*this}(forward<Arg>(arg));
    }  // if
  }

  template <size_t I, typename... Args>
  void emplace(Args &&... args) {
    this->index_ = meta::npos{};
    this->template construct<I>(forward<Args>(args)...);
  }

  void swap(variant_impl_ &that) noexcept(
      meta::and_<is_nothrow_move_constructible<Ts>...,
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
class variant_impl_<meta::list<Ts...>,
                    /* IsTriviallyCopyable = */ false,
                    /* IsTriviallyDestructible = */ false>
    : public variant_impl_<meta::list<Ts...>, false, true> {
  using super = variant_impl_<meta::list<Ts...>, false, true>;

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
    variant_impl_<meta::list<Ts...>,
                  meta::and_<is_trivially_copyable<Ts>...>{},
                  meta::and_<is_trivially_destructible<Ts>...>{}>;

template <typename... Ts>
class variant : public variant_impl<Ts...> {
  using alternatives = meta::list<Ts...>;
  using super = variant_impl<Ts...>;

  public:
  variant() = default;

  template <size_t I, typename... Args>
  explicit constexpr variant(in_place_index_t<I>, Args &&... args)
      : super(meta::size_t<I>{}, forward<Args>(args)...) {}

  template <typename T, typename... Args>
  explicit constexpr variant(in_place_type_t<T>, Args &&... args)
      : variant(in_place_index<meta::find_index<alternatives, T>{}>,
                forward<Args>(args)...) {
    static_assert(meta::count<alternatives, T>{} == 1, "");
  }

  variant(const variant &) = default;
  variant(variant &&) = default;

  ~variant() = default;

  template <typename Arg, typename T = get_best_match<alternatives, Arg &&>>
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
    static_assert(meta::count<alternatives, T>{} == 1, "");
    emplace<meta::find_index<alternatives, T>{}>(forward<Args>(args)...);
  }

  private:
  template <size_t I, typename Arg>
  void assign(Arg &&arg) {
    super::template assign<I>(forward<Arg>(arg));
  }

  template <typename T, typename Arg>
  void assign(Arg &&arg) {
    static_assert(meta::count<alternatives, T>{} == 1, "");
    assign<meta::find_index<alternatives, T>{}>(forward<Arg>(arg));
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
  using alternatives = meta::list<Ts...>;
  using super = detail::variant<Ts...>;

  public:
  //- 20.N.2.1 construction:
  using super::super;

  constexpr variant() : variant(in_place_index<0>) {}

  template <size_t I,
            typename U,
            typename... Args,
            typename = meta::if_<is_constructible<meta::at_c<alternatives, I>,
                                                  initializer_list<U> &,
                                                  Args &&...>>>
  explicit constexpr variant(in_place_index_t<I> ip,
                             initializer_list<U> init,
                             Args &&... args)
      : super(ip, init, forward<Args>(args)...) {}

  template <typename T,
            typename U,
            typename... Args,
            typename = meta::if_<
                is_constructible<T, initializer_list<U> &, Args &&...>>>
  explicit constexpr variant(in_place_type_t<T> ip,
                             initializer_list<U> init,
                             Args &&... args)
      : super(ip, init, forward<Args>(args)...) {}

  template <typename Arg,
            typename T = detail::get_best_match<alternatives, Arg &&>>
  constexpr variant(Arg &&arg) : variant(in_place_type<T>, forward<Arg>(arg)) {}

  variant(const variant &that) = default;
  variant(variant &&that) = default;

  //- 20.N.2.2 destruction:
  ~variant() = default;

  //- 20.N.2.3 assignment:
  using super::emplace;
  using super::operator=;

  template <size_t I, typename U, typename... Args>
  meta::if_<is_constructible<meta::at_c<alternatives, I>,
                             initializer_list<U> &,
                             Args &&...>,
  void> emplace(initializer_list<U> init, Args &&... args) {
    super::template emplace<I>(init, forward<Args>(args)...);
  }

  template <typename T, typename U, typename... Args>
  meta::if_<is_constructible<T, initializer_list<U> &, Args &&...>,
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

#endif  // VARIANT_VARIANT_HPP
