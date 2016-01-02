// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0.(See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EXPERIMENTAL_VARIANT_HPP
#define EXPERIMENTAL_VARIANT_HPP

#include <cassert>
#include <cstddef>
#include <functional>
#include <stdexcept>
#include <type_traits>
#include <utility>

#include <experimental/lib.hpp>
#include <experimental/tuple.hpp>

namespace std {
namespace experimental {
inline namespace fundamentals_v3 {

//- class `variant`:
template <typename... Ts> class variant;

//- in-place construction:

/* `in_place_index_t<I>` is an empty class type used to disambiguate the
   overloads of constructors and member functions of that take arguments
   (possibly a parameter pack) for index-based in-place construction. */
template <size_t I> struct in_place_index_t {};
template <size_t I> constexpr in_place_index_t<I> in_place_index{};

/* `in_place_type_t<I>` is an empty class type used to disambiguate the
   overloads of constructors and member functions of that take arguments
   (possibly a parameter pack) for type-based in-place construction. */
template <typename T> struct in_place_type_t {};
template <typename T> constexpr in_place_type_t<T> in_place_type{};

//- class `bad_variant_access`:

/* exception indicating an invalid access to a `variant` object

   The class `bad_variant_access` defines the type of objects thrown as
   exceptions to report the situation where an invalid attempt is made to
   access the value of a `variant` via `get`. */
class bad_variant_access : public exception {
  public:
  bad_variant_access() = default;
  bad_variant_access(const bad_variant_access &) = default;
  bad_variant_access &operator=(const bad_variant_access &) = default;
  virtual const char *what() const noexcept { return "bad_variant_access"; }
};  // bad_variant_access

//- unsafe operations

namespace unsafe {

template <typename... Ts> class storage_base;

template <typename T>
struct is_storage_base : false_type {};

template <typename... Ts>
struct is_storage_base<storage_base<Ts...>> : true_type {};

template <typename... Ts>
constexpr storage_base<Ts...> &as_storage_base(storage_base<Ts...> &that) { return that; }

template <typename... Ts>
constexpr const storage_base<Ts...> &as_storage_base(const storage_base<Ts...> &that) { return that; }

template <typename... Ts>
constexpr storage_base<Ts...> &&as_storage_base(storage_base<Ts...> &&that) { return move(that); }

template <typename... Ts>
constexpr const storage_base<Ts...> &&as_storage_base(const storage_base<Ts...> &&that) { return move(that); }

//- unsafe element access

struct access {

template <size_t I, typename Storage>
static constexpr auto &&get(Storage &&s) {
  using S = decay_t<Storage>;
  static_assert(is_storage_base<S>::value, "");
  using V = lib::repack_t<S, variant>;
  static_assert(I < experimental::tuple_size<V>::value, "");
  using T = experimental::tuple_element_t<I, V>;
  assert(!s.corrupted_by_exception());
  using R = lib::qualify_as_t<T, Storage &&>;
  return static_cast<R>(forward<Storage>(s).union_[lib::size_constant<I>{}]);
}

template <typename T, typename Storage>
static constexpr auto &&get(Storage &&s) {
  using S = decay_t<Storage>;
  static_assert(is_storage_base<S>::value, "");
  using V = lib::repack_t<S, variant>;
  static_assert(tuple_count<T, V>::value == 1, "");
  return get<tuple_find<T, V>::value>(forward<Storage>(s));
}

};  // access

template <size_t I, typename... Ts>
constexpr auto &&get(storage_base<Ts...> &s) { return access::get<I>(s); }

template <size_t I, typename... Ts>
constexpr auto &&get(const storage_base<Ts...> &s) { return access::get<I>(s); }

template <size_t I, typename... Ts>
constexpr auto &&get(storage_base<Ts...> &&s) { return access::get<I>(move(s)); }

template <size_t I, typename... Ts>
constexpr auto &&get(const storage_base<Ts...> &&s) { return access::get<I>(move(s)); }

template <typename T, typename... Ts>
constexpr auto &&get(storage_base<Ts...> &s) { return access::get<T>(s); }

template <typename T, typename... Ts>
constexpr auto &&get(const storage_base<Ts...> &s) { return access::get<T>(s); }

template <typename T, typename... Ts>
constexpr auto &&get(storage_base<Ts...> &&s) { return access::get<T>(move(s)); }

template <typename T, typename... Ts>
constexpr auto &&get(const storage_base<Ts...> &&s) { return access::get<T>(move(s)); }

//- unsafe visitation

struct visitation {

// Base case for `at`.
template <typename T>
static constexpr const T &at_impl(const T &elem, const size_t *) {
  return elem;
}

// Recursive case for `at_impl`.
template <typename T, size_t N>
static constexpr auto &&at_impl(const array<T, N> &elems, const size_t *index) {
  return at_impl(elems[*index], index + 1);
}

template <typename T, size_t N>
static constexpr auto &&at(const array<T, N> &elems, const size_t (&indices)[lib::rank<array<T, N>>::value]) {
  return at_impl(elems, begin(indices));
}

template <typename... Ts>
static constexpr auto make_array_impl(true_type, Ts &&... ts) {
  return funds_v2::make_array(forward<Ts>(ts)...);
}

template <typename... Ts>
static constexpr void make_array_impl(false_type, Ts &&...) {
  static_assert(lib::false_t<Ts...>::value,
                "The return types of all handlers must be equivalent for return type deduction of `visit`.");
}

template <typename T, typename... Ts>
static constexpr auto make_array(T &&t, Ts &&... ts) {
  return make_array_impl(cpp17::conjunction<is_same<T, Ts>...>{}, forward<T>(t), forward<Ts>(ts)...);
}

template <typename F, typename... Ss>
struct vtable {

template <size_t... Is>
static constexpr decltype(auto) dispatch(F f, Ss... ss) {
  return cpp17::invoke(static_cast<F>(f), index_sequence<Is...>{}, unsafe::get<Is>(static_cast<Ss>(ss))...);
}

// Base case for `make_impl`.
template <size_t... Is>
static constexpr auto make_impl(index_sequence<Is...>) {
  return &dispatch<Is...>;
}

// Recursive case for `make_impl`.
template <size_t... Is, size_t... Js, typename... Ls>
static constexpr auto make_impl(index_sequence<Is...>, index_sequence<Js...>, Ls... ls) {
  return make_array(make_impl(index_sequence<Is..., Js>{}, ls...)...);
}

// `make`.
static constexpr auto make() {
  return make_impl(index_sequence<>{},
                   make_index_sequence<experimental::tuple_size<lib::repack_t<decay_t<Ss>, variant>>::value>{}...);
}

};  // vtable

template <typename F, typename... Ss>
static constexpr auto make_vtable() { return vtable<F, Ss...>::make(); }

template <typename F, typename... Ss>
static constexpr decltype(auto) indexed_visit(F &&f, Ss &&... ss) {
  static_assert(cpp17::conjunction<is_storage_base<decay_t<Ss>>...>::value, "");
  constexpr auto vtable = make_vtable<F &&, Ss &&...>();
  return at(vtable, {ss.index()...})(forward<F>(f), forward<Ss>(ss)...);
}

template <typename F>
struct visitor {
  template <size_t... Is, typename... Args>
  constexpr decltype(auto) operator()(index_sequence<Is...>, Args &&... args) const {
    return cpp17::invoke(static_cast<F>(f), forward<Args>(args)...);
  }
  F f;
};

template <typename F, typename... Ss>
static constexpr decltype(auto) visit(F &&f, Ss &&... ss) {
  static_assert(cpp17::conjunction<is_storage_base<decay_t<Ss>>...>::value, "");
  return indexed_visit(visitor<F &&>{forward<F>(f)}, forward<Ss>(ss)...);
}

};  // visitation

template <typename F, typename... Ss>
constexpr decltype(auto) indexed_visit(F &&f, Ss &&... ss) {
  return visitation::indexed_visit(forward<F>(f), as_storage_base(forward<Ss>(ss))...);
}

template <typename F, typename... Ss>
constexpr decltype(auto) visit(F &&f, Ss &&... ss) {
  return visitation::visit(forward<F>(f), as_storage_base(forward<Ss>(ss))...);
}

//- class `storage`

template <typename T>
struct ref {
  public:
  static_assert(is_reference<T>::value, "`ref<T>` only holds references.");

  constexpr ref(T t) noexcept : ref_(static_cast<T>(t)) {}

  template <typename U = T, typename = enable_if_t<is_lvalue_reference<U>::value>>
  constexpr ref(remove_reference_t<T> &&) = delete;

  ref(const ref &) = default;
  ref(ref &&) = default;

  ~ref() = default;

  ref &operator=(const ref &) = default;
  ref &operator=(ref &&) = default;

  constexpr T get() const noexcept { return static_cast<T>(ref_); }
  constexpr operator T() const noexcept { return static_cast<T>(ref_); }

  private:
  T ref_;
};  // ref

/* `union_t` is a variadic recursive `union` which overlaps the N
   specified types. Compile-time indices are used to determine the element
   that needs to be constructed or retrieved. */

template <bool is_trivially_destructible, typename... Ts>
class union_impl;

/* Base case for `union_impl`. */
template <bool is_trivially_destructible>
class union_impl<is_trivially_destructible> {};

/* Recursive case for `union_impl`. */
#define UNION_IMPL(is_trivially_destructible, destructor)                                                    \
  template <typename T, typename... Ts>                                                                      \
  class union_impl<is_trivially_destructible, T, Ts...> {                                                    \
    public:                                                                                                  \
    using head = conditional_t<is_reference<T>::value, ref<T>, T>;                                           \
    using tail = union_impl<is_trivially_destructible, Ts...>;                                               \
                                                                                                             \
    explicit constexpr union_impl() noexcept : data_() {}                                                    \
                                                                                                             \
    /* Construct the element at index I. */                                                                  \
                                                                                                             \
    template <typename... Args>                                                                              \
    explicit constexpr union_impl(lib::size_constant<0>, Args &&... args) : head_(forward<Args>(args)...) {} \
                                                                                                             \
    template <size_t I, typename... Args>                                                                    \
    explicit constexpr union_impl(lib::size_constant<I>, Args &&... args)                                    \
        : tail_(lib::size_constant<I - 1>{}, forward<Args>(args)...) {}                                      \
                                                                                                             \
    union_impl(const union_impl &) = default;                                                                \
    union_impl(union_impl &&) = default;                                                                     \
                                                                                                             \
    destructor                                                                                               \
                                                                                                             \
    union_impl &operator=(const union_impl &) = default;                                                     \
    union_impl &operator=(union_impl &&) = default;                                                          \
                                                                                                             \
    /* Access the element at index I. */                                                                     \
                                                                                                             \
    constexpr T &operator[](lib::size_constant<0>) noexcept { return head_; }                                \
                                                                                                             \
    template <size_t I>                                                                                      \
    constexpr auto &&operator[](lib::size_constant<I>) noexcept {                                            \
      return static_cast<tail &>(tail_)[lib::size_constant<I - 1>{}];                                        \
    }                                                                                                        \
                                                                                                             \
    constexpr const T &operator[](lib::size_constant<0>) const noexcept { return head_; }                    \
                                                                                                             \
    template <size_t I>                                                                                      \
    constexpr auto &&operator[](lib::size_constant<I>) const noexcept {                                      \
      return static_cast<const tail &>(tail_)[lib::size_constant<I - 1>{}];                                  \
    }                                                                                                        \
                                                                                                             \
    /* Construct the element at index I. */                                                                  \
                                                                                                             \
    template <typename... Args>                                                                              \
    void construct(lib::size_constant<0>, Args &&... args) {                                                 \
      ::new (&head_) head(forward<Args>(args)...);                                                           \
    }                                                                                                        \
                                                                                                             \
    template <size_t I, typename... Args>                                                                    \
    void construct(lib::size_constant<I>, Args &&... args) {                                                 \
      tail_.construct(lib::size_constant<I - 1>{}, forward<Args>(args)...);                                  \
    }                                                                                                        \
                                                                                                             \
    /* Destruct the element at index I. */                                                                   \
                                                                                                             \
    void destroy(lib::size_constant<0>) noexcept { head_.~head(); }                                          \
                                                                                                             \
    template <size_t I>                                                                                      \
    void destroy(lib::size_constant<I>) noexcept {                                                           \
      tail_.destroy(lib::size_constant<I - 1>{});                                                            \
    }                                                                                                        \
                                                                                                             \
    private:                                                                                                 \
    union {                                                                                                  \
      uint8_t data_;                                                                                         \
      head head_;                                                                                            \
      tail tail_;                                                                                            \
    };                                                                                                       \
  };

/* is_trivially_destructible, destructor */
UNION_IMPL(true , ~union_impl() = default;)
UNION_IMPL(false, ~union_impl() {}        )

#undef UNION_IMPL

template <typename... Ts>
using union_t = union_impl<cpp17::conjunction<is_trivially_destructible<Ts>...>::value, Ts...>;

template <typename... Ts>
class storage_base {
  public:
  constexpr bool corrupted_by_exception() const noexcept { return index() == tuple_not_found; }

  constexpr size_t index() const noexcept { return index_; }

  protected:
  storage_base() : index_(tuple_not_found), union_() {}

  template <size_t I, typename... Args>
  constexpr storage_base(lib::size_constant<I> index, Args &&... args)
      : index_(index), union_(index, forward<Args>(args)...) {
    using V = variant<Ts...>;
    static_assert(I < experimental::tuple_size<V>::value, "");
    using T = experimental::tuple_element_t<I, V>;
    static_assert(is_constructible<T, Args &&...>::value, "");
  }

  storage_base(const storage_base &) = default;
  storage_base(storage_base &&) = default;

  ~storage_base() = default;

  storage_base &operator=(const storage_base &) = default;
  storage_base &operator=(storage_base &&) = default;

  template <size_t I, typename... Args>
  void construct(Args &&... args) {
    union_.construct(lib::size_constant<I>{}, forward<Args>(args)...);
    index_ = I;
  }

  template <size_t I>
  void destroy() noexcept {
    union_.destroy(lib::size_constant<I>{});
    index_ = tuple_not_found;
  }

  protected:
  size_t index_;
  union_t<Ts...> union_;

  friend struct access;
};

template <bool is_trivially_copyable, bool is_trivially_destructible, typename... Ts>
class storage_impl;

template <typename... Ts>
class storage_impl</* is_trivially_copyable */ true,
                   /* is_trivially_destructible */ true,
                   Ts...> : public storage_base<Ts...> {
  using super = storage_base<Ts...>;

  protected:
  using super::super;

  storage_impl() = default;

  storage_impl(const storage_impl &) = default;
  storage_impl(storage_impl &&) = default;

  ~storage_impl() = default;

  storage_impl &operator=(const storage_impl &) = default;
  storage_impl &operator=(storage_impl &&) = default;

  template <size_t I, typename Arg>
  void assign(Arg &&arg) {
    emplace<I>(forward<Arg>(arg));
  }

  template <size_t I, typename... Args>
  void emplace(Args &&... args) {
    *this = storage_impl(lib::size_constant<I>{}, forward<Args>(args)...);
  }

  void swap(storage_impl &that) noexcept(noexcept(std::swap(declval<storage_impl &>(), declval<storage_impl &>()))) {
    std::swap(*this, that);
  }
};

template <typename... Ts>
class storage_impl</* is_trivially_copyable */ true,
                   /* is_trivially_destructible */ false,
                   Ts...> {
  static_assert(lib::false_t<Ts...>::value, "A type cannot be trivally copyable but not trivially destructible.");
};

template <typename... Ts>
class storage_impl</* is_trivially_copyable */ false,
                   /* is_trivially_destructible */ true,
                   Ts...> : public storage_impl<true, true, Ts...> {
  using super = storage_impl<true, true, Ts...>;

  protected:
  using super::super;

  storage_impl() = default;

  storage_impl(const storage_impl &that) : storage_impl{} {
    static_assert(cpp17::conjunction<is_copy_constructible<Ts>...>::value, "");
    unsafe::indexed_visit(constructor{*this}, that);
  }

  storage_impl(storage_impl &&that) noexcept(cpp17::conjunction<is_nothrow_move_constructible<Ts>...>::value) {
    static_assert(cpp17::conjunction<is_move_constructible<Ts>...>::value, "");
    unsafe::indexed_visit(constructor{*this}, move(that));
  }

  ~storage_impl() = default;

  storage_impl &operator=(const storage_impl &that) {
    static_assert(cpp17::conjunction<is_copy_constructible<Ts>...>::value, "");
    static_assert(cpp17::conjunction<is_copy_assignable<Ts>...>::value, "");
    unsafe::indexed_visit(assigner{*this}, that);
    return *this;
  }

  storage_impl &operator=(storage_impl &&that) noexcept(
      cpp17::conjunction<is_nothrow_move_constructible<Ts>..., is_nothrow_move_assignable<Ts>...>::value) {
    static_assert(cpp17::conjunction<is_move_constructible<Ts>...>::value, "");
    static_assert(cpp17::conjunction<is_move_assignable<Ts>...>::value, "");
    unsafe::indexed_visit(assigner{*this}, move(that));
    return *this;
  }

  template <size_t I, typename Arg>
  void assign(Arg &&arg) {
    using T = experimental::tuple_element_t<I, variant<Ts...>>;
    static_assert(is_constructible<T, Arg &&>::value, "");
    static_assert(is_assignable<T &, Arg &&>::value, "");
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

  void swap(storage_impl &that) noexcept(
      cpp17::conjunction<is_nothrow_move_constructible<Ts>..., lib::is_nothrow_swappable<Ts>...>::value) {
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
    using T = experimental::tuple_element_t<I, variant<Ts...>>;

    void operator()(const T &arg) const { (*this)(arg, is_move_constructible<T>{}); }
    void operator()(T &&arg) const { self.emplace<I>(move(arg)); }

    storage_impl &self;

    private:
    void operator()(const T &arg, true_type) const { (*this)(T(arg)); }
    void operator()(const T &arg, false_type) const { self.emplace<I>(arg); }
  };  // assign_impl

  struct assigner {
    template <size_t I, typename Arg>
    void operator()(index_sequence<I>, Arg &&arg) const {
      self.template assign<I>(forward<Arg>(arg));
    }
    storage_impl &self;
  };  // assigner

  struct constructor {
    template <size_t I, typename Arg>
    void operator()(index_sequence<I>, Arg &&arg) const {
      self.template construct<I>(forward<Arg>(arg));
    }
    storage_impl &self;
  };  // constructor

  struct swapper {
    template <typename T>
    void operator()(T &lhs, T &rhs) const {
      using std::swap;
      swap(lhs, rhs);
    }

    template <typename T, typename U>
    void operator()(T &, U &) const { assert(false); }
  };  // swapper
};

template <typename... Ts>
class storage_impl</* is_trivially_copyable = */ false,
                   /* is_trivially_destructible = */ false,
                   Ts...> : public storage_impl<false, true, Ts...> {
  using super = storage_impl<false, true, Ts...>;

  protected:
  using super::super;

  storage_impl() = default;

  storage_impl(const storage_impl &) = default;
  storage_impl(storage_impl &&) = default;

  ~storage_impl() { destroy(); }

  storage_impl &operator=(const storage_impl &that) {
    if (this->index() != that.index()) {
      destroy();
    }  // if
    super::operator=(that);
    return *this;
  }

  storage_impl &operator=(storage_impl &&that) {
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

  void swap(storage_impl &that) noexcept(noexcept(declval<storage_impl &>().super::swap(declval<storage_impl &>()))) {
    super::swap(that);
  }

  private:
  using super::destroy;

  struct destructor {
    template <size_t I, typename Arg>
    void operator()(index_sequence<I>, Arg &) const noexcept {
      self.template destroy<I>();
    }
    storage_impl &self;
  };  // destructor

  void destroy() noexcept {
    if (!this->corrupted_by_exception()) {
      unsafe::indexed_visit(destructor{*this}, *this);
    }  // if
  }
};

template <typename... Ts>
using storage = storage_impl<cpp17::conjunction<is_trivially_copyable<Ts>...>::value,
                             cpp17::conjunction<is_trivially_destructible<Ts>...>::value,
                             Ts...>;

}  // namespace unsafe

//- class `variant`:

template <typename... Ts>
struct overload_set;

template <>
struct overload_set<> { void operator()() const; };

template <typename T, typename... Ts>
struct overload_set<T, Ts...> : overload_set<Ts...> {
  using super = overload_set<Ts...>;

  using super::operator();

  lib::id<T> operator()(T) const;

  template <typename U = T, typename = enable_if_t<is_lvalue_reference<U>::value>>
  void operator()(remove_reference_t<T> &&) const = delete;
};

template <typename T, typename... Ts>
using get_best_match = typename result_of_t<overload_set<Ts...>(T)>::type;

template <typename... Ts>
class variant_base : public unsafe::storage<Ts...> {
  using super = unsafe::storage<Ts...>;
  using V = variant<Ts...>;

  public:
  variant_base() = default;

  template <size_t I, typename... Args>
  explicit constexpr variant_base(in_place_index_t<I>, Args &&... args)
      : super(lib::size_constant<I>{}, forward<Args>(args)...) {}

  template <typename T, typename... Args>
  explicit constexpr variant_base(in_place_type_t<T>, Args &&... args)
      : variant_base(in_place_index<tuple_find<T, V>::value>, forward<Args>(args)...) {
    static_assert(tuple_count<T, V>::value == 1, "");
  }

  variant_base(const variant_base &) = default;
  variant_base(variant_base &&) = default;

  ~variant_base() = default;

  template <typename Arg, typename T = get_best_match<Arg &&, Ts...>>
  variant_base &operator=(Arg &&arg) {
    assign<T>(forward<Arg>(arg));
    return *this;
  }

  variant_base &operator=(const variant_base &) = default;
  variant_base &operator=(variant_base &&) = default;

  template <size_t I, typename... Args>
  void emplace(Args &&... args) {
    super::template emplace<I>(forward<Args>(args)...);
  }

  template <typename T, typename... Args>
  void emplace(Args &&... args) {
    static_assert(tuple_count<T, V>::value == 1, "");
    emplace<tuple_find<T, V>::value>(forward<Args>(args)...);
  }

  private:
  template <size_t I, typename Arg>
  void assign(Arg &&arg) {
    super::template assign<I>(forward<Arg>(arg));
  }

  template <typename T, typename Arg>
  void assign(Arg &&arg) {
    static_assert(tuple_count<T, V>::value == 1, "");
    assign<tuple_find<T, V>::value>(forward<Arg>(arg));
  }
};

template <typename... Ts>
class variant;

template <>
class variant<> {};

template <typename... Ts>
class variant : public variant_base<Ts...> {
  using super = variant_base<Ts...>;

  public:
  //- construction:
  using super::super;

  constexpr variant() : variant(in_place_index<0>) {}

  template <size_t I,
            typename U,
            typename... Args,
            typename = enable_if_t<
                is_constructible<experimental::tuple_element_t<I, variant>, initializer_list<U> &, Args &&...>::value>>
  explicit constexpr variant(in_place_index_t<I> ip, initializer_list<U> init, Args &&... args)
      : super(ip, init, forward<Args>(args)...) {}

  template <typename T,
            typename U,
            typename... Args,
            typename = enable_if_t<is_constructible<T, initializer_list<U> &, Args &&...>::value>>
  explicit constexpr variant(in_place_type_t<T> ip, initializer_list<U> init, Args &&... args)
      : super(ip, init, forward<Args>(args)...) {}

  template <typename Arg, typename T = get_best_match<Arg &&, Ts...>>
  constexpr variant(Arg &&arg) : variant(in_place_type<T>, forward<Arg>(arg)) {}

  variant(const variant &that) = default;
  variant(variant &&that) = default;

  //- destruction:
  ~variant() = default;

  //- assignment:
  using super::emplace;
  using super::operator=;

  template <size_t I, typename U, typename... Args>
  enable_if_t<is_constructible<experimental::tuple_element_t<I, variant>, initializer_list<U> &, Args &&...>::value,
  void> emplace(initializer_list<U> init, Args &&... args) {
    super::template emplace<I>(init, forward<Args>(args)...);
  }

  template <typename T, typename U, typename... Args>
  enable_if_t<is_constructible<T, initializer_list<U> &, Args &&...>::value,
  void> emplace(initializer_list<U> init, Args &&... args) {
    super::template emplace<T>(init, forward<Args>(args)...);
  }

  variant &operator=(const variant &that) = default;
  variant &operator=(variant &&that) = default;

  //- observation:
  using super::corrupted_by_exception;
  using super::index;

  //- swap:
  using super::swap;
};  // variant

//- element access:

template <size_t I, typename... Ts>
constexpr bool holds_alternative(const variant<Ts...> &v) noexcept {
  return v.index() == I;
}

template <typename T, typename... Ts>
constexpr bool holds_alternative(const variant<Ts...> &v) noexcept {
  using V = variant<Ts...>;
  static_assert(tuple_count<T, V>::value == 1, "");
  return holds_alternative<tuple_find<T, V>::value>(v);
}

namespace access {

template <size_t I, typename Variant>
constexpr auto *get_if(Variant *v) {
  using V = decay_t<Variant>;
  static_assert(I < experimental::tuple_size<V>::value, "");
  using T = experimental::tuple_element_t<I, V>;
  assert(v);
  if (!holds_alternative<I>(*v)) {
    using R = add_pointer_t<lib::qualify_as_t<T, Variant>>;
    return static_cast<R>(nullptr);
  }  // if
  return &unsafe::get<I>(*v);
}

template <typename T, typename Variant>
constexpr auto *get_if(Variant *v) {
  using V = decay_t<Variant>;
  static_assert(tuple_count<T, V>::value == 1, "");
  return get_if<tuple_find<T, V>::value>(v);
}

template <size_t I, typename Variant>
constexpr auto &&get(Variant &&v) {
  using V = decay_t<Variant>;
  using T = experimental::tuple_element_t<I, V>;
  using R = lib::qualify_as_t<T, Variant &&>;
  auto *result = get_if<I>(&v);
  return static_cast<R>(*(result ? result : throw bad_variant_access{}));
}

template <typename T, typename Variant>
constexpr auto &&get(Variant &&v) {
  using V = decay_t<Variant>;
  static_assert(tuple_count<T, V>::value == 1, "");
  return get<tuple_find<T, V>::value>(forward<Variant>(v));
}

}  // namespace access

template <size_t I, typename... Ts>
constexpr auto *get_if(variant<Ts...> *v) noexcept { return access::get_if<I>(v); }

template <size_t I, typename... Ts>
constexpr auto *get_if(const variant<Ts...> *v) noexcept { return access::get_if<I>(v); }

template <typename T, typename... Ts>
constexpr auto *get_if(variant<Ts...> *v) noexcept { return access::get_if<T>(v); }

template <typename T, typename... Ts>
constexpr auto *get_if(const variant<Ts...> *v) noexcept { return access::get_if<T>(v); }

template <size_t I, typename... Ts>
constexpr auto &&get(variant<Ts...> &v) { return access::get<I>(v); }

template <size_t I, typename... Ts>
constexpr auto &&get(const variant<Ts...> &v) { return access::get<I>(v); }

template <size_t I, typename... Ts>
constexpr auto &&get(variant<Ts...> &&v) { return access::get<I>(move(v)); }

template <size_t I, typename... Ts>
constexpr auto &&get(const variant<Ts...> &&v) { return access::get<I>(move(v)); }

template <typename T, typename... Ts>
constexpr auto &&get(variant<Ts...> &v) { return access::get<T>(v); }

template <typename T, typename... Ts>
constexpr auto &&get(const variant<Ts...> &v) { return access::get<T>(v); }

template <typename T, typename... Ts>
constexpr auto &&get(variant<Ts...> &&v) { return access::get<T>(move(v)); }

template <typename T, typename... Ts>
constexpr auto &&get(const variant<Ts...> &&v) { return access::get<T>(move(v)); }

//- relational operators:

namespace rel {

struct equal {
  template <typename T>
  constexpr bool operator()(const T &lhs, const T &rhs) const {
    return lhs == rhs;
  }

  template <typename T, typename U>
  constexpr bool operator()(const T &, const U &) const {
    assert(false);
    return false;
  }
};  // equal

struct less {
  template <typename T>
  constexpr bool operator()(const T &lhs, const T &rhs) const {
    return lhs < rhs;
  }

  template <typename T, typename U>
  constexpr bool operator()(const T &, const U &) const {
    assert(false);
    return false;
  }
};  // less

}  // namespace rel

template <typename... Ts>
constexpr bool operator==(const variant<Ts...> &lhs, const variant<Ts...> &rhs) {
  return lhs.index() == rhs.index() && unsafe::visit(rel::equal{}, lhs, rhs);
}

template <typename... Ts>
constexpr bool operator!=(const variant<Ts...> &lhs, const variant<Ts...> &rhs) {
  return !(lhs == rhs);
}

template <typename... Ts>
constexpr bool operator<(const variant<Ts...> &lhs, const variant<Ts...> &rhs) {
  return lhs.index() < rhs.index() || (lhs.index() == rhs.index() && unsafe::visit(rel::less{}, lhs, rhs));
}

template <typename... Ts>
constexpr bool operator>(const variant<Ts...> &lhs, const variant<Ts...> &rhs) {
  return rhs < lhs;
}

template <typename... Ts>
constexpr bool operator<=(const variant<Ts...> &lhs, const variant<Ts...> &rhs) {
  return !(lhs > rhs);
}

template <typename... Ts>
constexpr bool operator>=(const variant<Ts...> &lhs, const variant<Ts...> &rhs) {
  return !(lhs < rhs);
}

//- swap support:

template <typename... Ts>
void swap(variant<Ts...> &lhs, variant<Ts...> &rhs) noexcept(noexcept(lhs.swap(rhs))) {
  lhs.swap(rhs);
}

//- visitation:

template <typename F, typename... Vs>
constexpr decltype(auto) visit(F &&f, Vs &&... vs) {
  return lib::any_of({vs.corrupted_by_exception()...}, true) ? throw bad_variant_access{}
                                                             : unsafe::visit(forward<F>(f), forward<Vs>(vs)...);
}

}  // namespace fundamentals_v3
}  // namespace experimental

//- hash support:

template <typename... Ts>
struct hash<experimental::variant<Ts...>> {
  using argument_type = experimental::variant<Ts...>;
  using result_type = size_t;

  result_type operator()(const argument_type &v) const { return experimental::unsafe::indexed_visit(hasher{}, v); }

  private:
  struct hasher {
    template <size_t I, typename Arg>
    result_type operator()(index_sequence<I>, const Arg &arg) const {
      using T = experimental::tuple_element_t<I, experimental::variant<Ts...>>;
      return hash<T>{}(arg);
    }
  };  // hasher
};  // hash<experimental::variant<Ts...>>

}  // namespace std

#endif  // EXPERIMENTAL_VARIANT_HPP
