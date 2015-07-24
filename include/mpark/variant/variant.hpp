#pragma once

#include <cstddef>
#include <initializer_list>
#include <new>
#include <tuple>
#include <typeinfo>
#include <type_traits>
#include <utility>

#include <meta/meta.hpp>

#include <mpark/variant/detail/best_match.hpp>
#include <mpark/variant/detail/union.hpp>
#include <mpark/variant/detail/vtable.hpp>
#include <mpark/variant/detail/ref_wrapper.hpp>

#include <mpark/variant/apply.hpp>
#include <mpark/variant/in_place.hpp>
#include <mpark/variant/null.hpp>

namespace mpark {

  namespace detail {

    namespace variant {

      template <typename T>
      struct storage {
        private:

        static meta::id<ref_wrapper<T>> impl(std::true_type);
        static meta::id<T> impl(std::false_type);

        public:

        using type = meta::_t<decltype(impl(std::is_reference<T>{}))>;

      };  // storage

      template <typename... Ts>
      class variant : private union_t<meta::_t<storage<Ts>>...> {
        private:

        using super = union_t<meta::_t<storage<Ts>>...>;

        template <typename T>
        using find_index = meta::find_index<meta::list<Ts...>, T>;

        public:

        template <typename T,
                  typename = std::enable_if_t<
                      has_best_match<meta::list<Ts...>, T &&>{}>>
        constexpr variant(T &&arg)
            : variant(in_place<get_best_match<meta::list<Ts...>, T &&>>,
                      std::forward<T>(arg)) {}

        variant(const variant &that) {
          apply<constructor>(std::forward_as_tuple(*this), that);
        }

        variant(variant &&that) {
          apply<constructor>(std::forward_as_tuple(*this), std::move(that));
        }

        template <
            typename T,
            typename... Args,
            typename = std::enable_if_t<meta::in<meta::list<Ts...>, T>{} &&
                                        std::is_constructible<T, Args &&...>{}>>
        explicit constexpr variant(in_place_t<T>, Args &&... args)
            : variant(find_index<T>{}, std::forward<Args>(args)...) {
          static_assert(meta::in<meta::list<Ts...>, T>{}, "");
          static_assert(std::is_constructible<T, Args &&...>{}, "");
        }

        ~variant() { destruct(); }

        template <typename T = null_t,
                  typename = std::enable_if_t<meta::in<meta::list<Ts...>, T>{}>>
        explicit constexpr operator bool() const noexcept {
          return index_ != find_index<T>{};
        }

        template <typename T>
        std::enable_if_t<has_best_match<meta::list<Ts...>, T &&>{},
        variant &> operator=(T &&arg) {
          assign<get_best_match<meta::list<Ts...>, T &&>>(std::forward<T>(arg));
          return *this;
        }

        variant &operator=(const variant &that) {
          apply<assigner>(std::forward_as_tuple(*this), that);
          return *this;
        }

        variant &operator=(variant &&that) {
          apply<assigner>(std::forward_as_tuple(*this), std::move(that));
          return *this;
        }

        template <typename T, typename... Args>
        void emplace(Args &&... args) {
          static_assert(meta::in<meta::list<Ts...>, T>{}, "");
          static_assert(std::is_constructible<T, Args &&...>{}, "");
          destruct();
          construct<T>(std::forward<Args>(args)...);
        }

        void swap(variant &that) { apply(swapper{*this, that}, *this, that); }

        const std::type_info &type() const noexcept {
          return apply<typer>(std::forward_as_tuple(), *this);
        }

        bool valid() const noexcept { return index_ != meta::npos{}; }

        private:

        template <typename T>
        struct assigner {

          template <typename U>
          void operator()(U &&u) const {
            self.assign<T>(std::forward<U>(u));
          }

          variant &self;

        };  // assigner

        template <typename T>
        struct constructor {

          template <typename U>
          void operator()(U &&u) const {
            self.construct<T>(std::forward<U>(u));
          }

          variant &self;

        };  // constructor

        template <typename T>
        struct destructor {

          template <typename U>
          void operator()(U &) const {
            self.destruct<T>();
          }

          variant &self;

        };  // destructor

        struct swapper {

          template <typename T>
          void operator()(T &lhs, T &rhs) const {
            using std::swap;
            swap(lhs, rhs);
          }

          template <typename Lhs, typename Rhs>
          void operator()(Lhs &, Rhs &) const {
            std::swap(self, other);
          }

          variant &self;

          variant &other;

        };  // swapper

        template <typename T>
        struct typer {

          template <typename U>
          const std::type_info &operator()(U &&) const noexcept {
            return typeid(T);
          }

        };  // typer

        template <std::size_t I, typename... Args>
        explicit constexpr variant(meta::size_t<I> idx, Args &&... args)
            : super(idx, std::forward<Args>(args)...), index_(idx) {}

        template <typename T, typename Arg>
        void assign(Arg &&arg) {
          static_assert(meta::in<meta::list<Ts...>, T>{}, "");
          static_assert(std::is_assignable<T &, Arg &&>{}, "");
          static_assert(std::is_constructible<T, Arg &&>{}, "");
          if (index_ == find_index<T>{}) {
            index_ = meta::npos{};
            get(meta::id<T>{}) = std::forward<Arg>(arg);
            index_ = find_index<T>{};
          } else {
            emplace<T>(T(std::forward<Arg>(arg)));
          }  // if
        }

        template <typename T, typename... Args>
        void construct(Args &&... args) {
          static_assert(meta::in<meta::list<Ts...>, T>{}, "");
          index_ = meta::npos{};
          super::construct(find_index<T>{}, std::forward<Args>(args)...);
          index_ = find_index<T>{};
        }

        void destruct() noexcept {
          if (valid()) {
            apply<destructor>(std::forward_as_tuple(*this), *this);
          }  // if
        }

        template <typename T>
        void destruct() noexcept {
          static_assert(meta::in<meta::list<Ts...>, T>{}, "");
          super::destruct(find_index<T>{});
        }

        template <typename T>
        T &get(meta::id<T>) & noexcept {
          static_assert(meta::in<meta::list<Ts...>, T>{}, "");
          return (*this)[find_index<T>{}];
        }

        template <typename T>
        const T &get(meta::id<T>) const & noexcept {
          static_assert(meta::in<meta::list<Ts...>, T>{}, "");
          return (*this)[find_index<T>{}];
        }

        template <typename T>
        T &&get(meta::id<T>) && noexcept {
          static_assert(meta::in<meta::list<Ts...>, T>{}, "");
          return std::move(*this)[find_index<T>{}];
        }

        template <typename T>
        const T &&get(meta::id<T>) const && noexcept {
          static_assert(meta::in<meta::list<Ts...>, T>{}, "");
          return std::move(*this)[find_index<T>{}];
        }

        std::size_t index_;

        // Allow access to `index_`.
        friend struct apply_impl;

        // Allow access `get`.
        template <typename... Vs>
        friend struct make_vtable_impl;

      };  // variant

    }  // variant

  }  // detail

  template <typename... Ts>
  class variant : public detail::variant::variant<Ts...> {
    private:

    using super = detail::variant::variant<Ts...>;

    public:

    template <typename T = null_t,
              typename = std::enable_if_t<std::is_constructible<super, T>{}>>
    constexpr variant()
        : super(null) {}

    using super::super;

    template <typename T,
              typename = std::enable_if_t<
                  std::is_constructible<super, std::initializer_list<T> &>{}>>
    constexpr variant(std::initializer_list<T> arg)
        : super(arg) {}

    template <
        typename T,
        typename U,
        typename... Args,
        typename = std::enable_if_t<
            std::is_constructible<T, std::initializer_list<U> &, Args &&...>{}>>
    explicit constexpr variant(in_place_t<T> tag,
                               std::initializer_list<U> arg,
                               Args &&... args)
        : super(tag, arg, std::forward<Args>(args)...) {}

    using super::operator=;

    template <typename T>
    std::enable_if_t<std::is_assignable<super &, std::initializer_list<T> &>{},
    variant &> operator=(std::initializer_list<T> arg) {
      super::operator=(arg);
      return *this;
    }

    using super::emplace;

    template <typename T, typename U, typename... Args>
    std::enable_if_t<
        std::is_constructible<T, std::initializer_list<U> &, Args &&...>{},
    void> emplace(std::initializer_list<U> arg, Args &&... args) {
      super::emplace(arg, std::forward<Args>(args)...);
    }

  };  // variant

  template <typename... Ts>
  void swap(variant<Ts...> &lhs, variant<Ts...> &rhs) {
    lhs.swap(rhs);
  }

}  // namespace mpark

namespace std {

  template <>
  struct hash<mpark::null_t> {

    using argument_type = mpark::null_t;
    using result_type = std::size_t;

    result_type operator()(const argument_type &) const {
      return 0u;
    }

  };  // hash<mpark::null_t>

  template <typename... Ts>
  struct hash<mpark::variant<Ts...>> {

    using argument_type = mpark::variant<Ts...>;
    using result_type = std::size_t;

    result_type operator()(const argument_type &v) const {
      return mpark::apply<hasher>(std::forward_as_tuple(), v);
    }

    private:

    template <typename T>
    struct hasher {

      template <typename U>
      result_type operator()(U &&u) const {
        return std::hash<T>()(std::forward<U>(u));
      }

    };  // hasher

  };  // hash<mpark::variant<Ts...>>

}  // namespace std
