#ifndef VARIANT_VARIANT_HPP
#define VARIANT_VARIANT_HPP

#include <cstddef>
#include <initializer_list>
#include <type_traits>
#include <utility>

#include <meta/meta.hpp>

#include <variant/detail/type_traits.hpp>
#include <variant/detail/union.hpp>
#include <variant/detail/vtable.hpp>
#include <variant/detail/unsafe/get.hpp>
#include <variant/in_place.hpp>
#include <variant/visit.hpp>

namespace std {
namespace experimental {

//- 20.N.M class template `variant`:

template <typename... Ts> class variant;

template <> class variant<> {};

namespace detail {

template <typename... Ts>
class variant_base : public union_<Ts...> {
  private:
  using super = union_<Ts...>;
  using members = meta::list<Ts...>;

  public:

  //- 20.N.2.1 construction:

  template <size_t I, typename... Args>
  explicit constexpr variant_base(in_place_index_t<I>, Args &&... args)
      : super(meta::size_t<I>{}, forward<Args>(args)...), index_(I) {
    static_assert(I < members::size(), "");
    using T = meta::at_c<members, I>;
    static_assert(is_constructible<T, Args &&...>{}, "");
  }

  template <typename T, typename... Args>
  explicit constexpr variant_base(in_place_type_t<T>, Args &&... args)
      : variant_base(in_place_index<meta::find_index<members, T>{}>,
                     forward<Args>(args)...) {
    static_assert(meta::count<members, T>{} == 1, "");
  }

  //- 20.N.2.3 assignment:

  template <size_t I, typename... Args>
  void emplace(Args &&... args) {
    static_assert(I < members::size(), "");
    using T = meta::at_c<members, I>;
    static_assert(is_constructible<T, Args &&...>{}, "");
    auto &self = static_cast<experimental::variant<Ts...> &>(*this);
    self.destroy();
    self.template construct<I>(forward<Args>(args)...);
  }

  template <typename T, typename... Args>
  void emplace(Args &&... args) {
    static_assert(meta::count<members, T>{} == 1, "");
    emplace<meta::find_index<members, T>{}>(forward<Args>(args)...);
  }

  protected:
  explicit constexpr variant_base() = default;

  using super::super;
  using super::operator[];
  using super::construct;
  using super::destroy;

  size_t index_;

  friend struct detail::unsafe::impl;
};  // variant_base

}  // namespace detail

template <typename... Ts>
class variant : public detail::variant_base<Ts...> {
  private:
  using super = detail::variant_base<Ts...>;
  using members = meta::list<Ts...>;

  public:

  //- 20.N.2.1 construction:

  using super::super;

  /* constructs the variant object in an indeterminate state */
  constexpr variant() noexcept : super(in_place_index<0>) {}

  template <size_t I,
            typename U,
            typename... Args,
            typename = meta::if_<is_constructible<meta::at_c<members, I>,
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

  template <typename Arg, typename T = detail::get_best_match<members, Arg &&>>
  constexpr variant(Arg &&arg) : variant(in_place_type<T>, forward<Arg>(arg)) {}

  variant(const variant &that) {
    static_assert(meta::and_<is_copy_constructible<Ts>...>{}, "");
    visit(detail::make_index_visitor<constructor>(*this), that);
  }

  variant(variant &&that) noexcept(
      meta::and_<is_nothrow_move_constructible<Ts>...>{}) {
    static_assert(meta::and_<is_move_constructible<Ts>...>{}, "");
    visit(detail::make_index_visitor<constructor>(*this), move(that));
  }

  //- 20.N.2.2 destruction:
  ~variant() { destroy(); }

  //- 20.N.2.3 assignment:

  using super::emplace;

  template <size_t I, typename U, typename... Args>
  meta::if_<is_constructible<meta::at_c<members, I>,
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

  template <typename Arg, typename T = detail::get_best_match<members, Arg &&>>
  variant &operator=(Arg &&arg) {
    assign<T>(forward<Arg>(arg));
    return *this;
  }

  variant &operator=(const variant &that) {
    static_assert(meta::and_<is_copy_constructible<Ts>...>{}, "");
    static_assert(meta::and_<is_copy_assignable<Ts>...>{}, "");
    visit(detail::make_index_visitor<assigner>(*this), that);
    return *this;
  }

  variant &operator=(variant &&that) noexcept(
      meta::and_<is_nothrow_move_constructible<Ts>...,
                 is_nothrow_move_assignable<Ts>...>{}) {
    static_assert(meta::and_<is_move_constructible<Ts>...>{}, "");
    static_assert(meta::and_<is_move_assignable<Ts>...>{}, "");
    visit(detail::make_index_visitor<assigner>(*this), move(that));
    return *this;
  }

  //- 20.N.2.4 observers:

  constexpr bool corrupted_by_exception() const noexcept {
    return index() == meta::npos{};
  }

  constexpr size_t index() const noexcept { return this->index_; }

  //- 20.N.2.5 swap:

  void swap(variant &that) noexcept(
      meta::and_<is_nothrow_move_constructible<Ts>...,
                 detail::is_nothrow_swappable<Ts>...>{}) {
    visit(swapper{*this, that}, *this, that);
  }

  private:
  template <size_t I>
  struct assigner {
    template <typename Arg>
    void operator()(Arg &&arg) const { self.assign<I>(forward<Arg>(arg)); }
    variant &self;
  };  // assigner

  template <size_t I>
  struct constructor {
    template <typename Arg>
    void operator()(Arg &&arg) const { self.construct<I>(forward<Arg>(arg)); }
    variant &self;
  };  // constructor

  template <size_t I>
  struct destructor {
    template <typename Arg>
    void operator()(Arg &) const { self.destroy<I>(); }
    variant &self;
  };  // destructor

  struct swapper {
    template <typename T>
    void operator()(T &lhs, T &rhs) const {
      using std::swap;
      swap(lhs, rhs);
    }
    template <typename T, typename U>
    void operator()(T &, U &) const { std::swap(self, other); }
    variant &self;
    variant &other;
  };  // swapper

  template <size_t I, typename Arg>
  void assign(Arg &&arg) {
    static_assert(meta::not_<meta::or_<std::is_reference<Ts>...>>{}, "");
    static_assert(I < members::size(), "");
    using T = meta::at_c<members, I>;
    static_assert(is_constructible<T, Arg &&>{}, "");
    static_assert(is_assignable<T &, Arg &&>{}, "");
    if (this->index_ == I) {
      detail::unsafe::get<I>(*this) = forward<Arg>(arg);
    } else {
      struct assignment {
        void operator()(const T &arg) const { self.emplace<I>(T(arg)); }
        void operator()(T &&arg) const { self.emplace<I>(move(arg)); }
        variant &self;
      };  // assignment
      assignment{*this}(forward<Arg>(arg));
    }  // if
  }

  template <typename T, typename Arg>
  void assign(Arg &&arg) {
    static_assert(meta::count<members, T>{} == 1, "");
    assign<meta::find_index<members, T>{}>(forward<Arg>(arg));
  }

  template <size_t I, typename... Args>
  void construct(Args &&... args) {
    static_assert(I < members::size(), "");
    this->index_ = meta::npos{};
    super::construct(meta::size_t<I>{}, forward<Args>(args)...);
    this->index_ = I;
  }

  template <size_t I>
  void destroy() noexcept {
    static_assert(I < members::size(), "");
    super::destroy(meta::size_t<I>{});
  }

  void destroy() noexcept {
    if (!corrupted_by_exception()) {
      visit(detail::make_index_visitor<destructor>(*this), *this);
    }  // if
  }

  friend super;
};  // variant

}  // namespace experimental
}  // namespace std

#endif  // VARIANT_VARIANT_HPP
