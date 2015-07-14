#pragma once

#include <cstddef>
#include <initializer_list>
#include <new>
#include <typeinfo>
#include <type_traits>
#include <utility>

#include <meta/meta.hpp>

#include <mpark/variant/detail/best_match.hpp>
#include <mpark/variant/detail/union.hpp>
#include <mpark/variant/detail/vtable.hpp>

#include <mpark/variant/apply.hpp>
#include <mpark/variant/in_place.hpp>
#include <mpark/variant/null.hpp>

namespace mpark {

  namespace detail {

    namespace variant {

      template <typename... Ts>
      class variant : private union_t<Ts...> {
        private:

        using types = meta::list<Ts...>;

        template <typename T>
        using find_index = meta::find_index<types, T>;

        public:

        template <typename T,
                  typename = std::enable_if_t<has_best_match<T &&, types>{}>>
        constexpr variant(T &&arg)
            : variant(in_place<get_best_match<T &&, types>>,
                      std::forward<T>(arg)) {}

        variant(const variant &that) { apply(constructor{*this}, that); }

        variant(variant &&that) { apply(constructor{*this}, std::move(that)); }

        template <typename T,
                  typename... Args,
                  typename = std::enable_if_t<meta::in<types, T>{}>>
        explicit constexpr variant(in_place_t<T>, Args &&... args)
            : variant(find_index<T>{}, std::forward<Args>(args)...) {}

        ~variant() { destruct(); }

        template <typename T = null_t,
                  typename = std::enable_if_t<meta::in<types, T>{}>>
        explicit constexpr operator bool() const noexcept {
          return index_ != find_index<T>{};
        }

        template <typename T>
        std::enable_if_t<has_best_match<T &&, types>{},
        variant &> operator=(T &&arg) {
          assign<get_best_match<T &&, types>>(std::forward<T>(arg));
          return *this;
        }

        variant &operator=(const variant &that) {
          apply(assigner{*this}, that);
          return *this;
        }

        variant &operator=(variant &&that) {
          apply(assigner{*this}, std::move(that));
          return *this;
        }

        template <typename T, typename Arg>
        void assign(Arg &&arg) {
          static_assert(meta::in<types, T>{},
                        "T must be one of the types in the variant.");
          if (index_ == find_index<T>{}) {
            get(meta::id<T>{}) = std::forward<Arg>(arg);
            return;
          }  // if
          T temp(std::forward<Arg>(arg));
          emplace<T>(std::move(temp));
        }

        template <typename T, typename... Args>
        void emplace(Args &&... args) {
          static_assert(meta::in<types, T>{},
                        "T must be one of the types in the variant.");
          destruct();
          construct<T>(std::forward<Args>(args)...);
        }

        const std::type_info &type() const noexcept {
          return apply([](auto &&elem) -> const std::type_info & {
            using Elem = decltype(elem);
            using T = std::decay_t<Elem>;
            return typeid(T);
          }, *this);
        }

        bool valid() const noexcept { return index_ != meta::npos{}; }

        private:

        struct assigner {

          template <typename Elem>
          void operator()(Elem &&elem) const {
            using T = std::decay_t<Elem>;
            self.assign<T>(std::forward<Elem>(elem));
          }

          variant &self;

        };  // assigner

        struct constructor {

          template <typename Elem>
          void operator()(Elem &&elem) const {
            using T = std::decay_t<Elem>;
            self.construct<T>(std::forward<Elem>(elem));
          }

          variant &self;

        };  // constructor

        template <std::size_t I, typename... Args>
        explicit constexpr variant(meta::size_t<I> idx, Args &&... args)
            : detail::variant::union_t<Ts...>{idx, std::forward<Args>(args)...},
              index_{idx} {}

        template <typename T, typename... Args>
        void construct(Args &&... args) {
          index_ = meta::npos{};
          ::new (this->data()) T(std::forward<Args>(args)...);
          index_ = find_index<T>{};
        }

        void destruct() noexcept {
          if (valid()) {
            apply([](auto &elem) {
              using Elem = decltype(elem);
              using T = std::decay_t<Elem>;
              elem.~T();
            }, *this);
          }
        }

        template <typename T>
        decltype(auto) get(meta::id<T>) & noexcept {
          return (*this)[find_index<T>{}];
        }

        template <typename T>
        decltype(auto) get(meta::id<T>) const & noexcept {
          return (*this)[find_index<T>{}];
        }

        template <typename T>
        decltype(auto) get(meta::id<T>) && noexcept {
          return std::move(*this)[find_index<T>{}];
        }

        template <typename T>
        decltype(auto) get(meta::id<T>) const && noexcept {
          return std::move(*this)[find_index<T>{}];
        }

        std::size_t index_;

        // Allow access to `index_`.
        friend struct apply_impl;

        // Allow access `get`.
        template <typename F, typename... Vs>
        friend struct make_vtable_impl;

      };  // variant

    }  // variant

  }  // detail

  template <typename... Ts>
  class variant : public detail::variant::variant<Ts...> {
    private:

    using super = detail::variant::variant<Ts...>;

    using types = meta::list<Ts...>;

    public:

    template <typename T = null_t,
              typename = std::enable_if_t<meta::in<types, T>{}>>
    constexpr variant()
        : super(null) {}

    using super::super;

    template <typename T,
              typename = std::enable_if_t<
                  std::is_constructible<super, std::initializer_list<T>>{}>>
    constexpr variant(std::initializer_list<T> arg)
        : super(arg) {}

    template <typename T,
              typename U,
              typename... Args,
              typename = std::enable_if_t<meta::in<types, T>{}>>
    explicit constexpr variant(in_place_t<T> tag,
                               std::initializer_list<U> arg,
                               Args &&... args)
        : super(tag, arg, std::forward<Args>(args)...) {}

    using super::operator=;

    template <typename T>
    std::enable_if_t<std::is_assignable<super, std::initializer_list<T>>{},
    variant &> operator=(std::initializer_list<T> arg) {
      super::operator=(arg);
      return *this;
    }

    using super::emplace;

    template <typename T, typename U, typename... Args>
    void emplace(std::initializer_list<U> arg, Args &&... args) {
      super::emplace(arg, std::forward<Args>(args)...);
    }

  };  // variant

}  // namespace mpark
