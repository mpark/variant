#pragma once

#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <typeinfo>
#include <type_traits>
#include <utility>

#include <exp/type_traits.hpp>
#include <meta/meta.hpp>

#include <mpark/variant/detail/union.hpp>
#include <mpark/variant/unsafe/get.hpp>

#include <mpark/variant/bad_variant_assign.hpp>
#include <mpark/variant/in_place.hpp>
#include <mpark/variant/type_switch.hpp>

namespace mpark {

  namespace detail {

    namespace variant {

      template <typename... Ts>
      class variant : public union_<Ts...> {
        private:
        using super = union_<Ts...>;
        using members = meta::list<Ts...>;

        public:

        //- 20.N.2.1 construction:

        template <std::size_t I, typename... Args>
        explicit constexpr variant(in_place_t (&)(meta::size_t<I>),
                                   Args &&... args)
            : super(meta::size_t<I>{}, std::forward<Args>(args)...), index_(I) {
          static_assert(I < members::size(), "");
          using T = meta::at_c<members, I>;
          static_assert(std::is_constructible<T, Args &&...>{}, "");
        }

        template <typename T, typename... Args>
        explicit constexpr variant(in_place_t (&)(meta::id<T>), Args &&... args)
            : variant(in_place<meta::find_index<members, T>{}>,
                      std::forward<Args>(args)...) {
          static_assert(meta::count<members, T>{} == 1, "");
        }

        //- 20.N.2.3 assignment:

        template <std::size_t I, typename... Args>
        void emplace(Args &&... args) {
          static_assert(I < members::size(), "");
          using T = meta::at_c<members, I>;
          static_assert(std::is_constructible<T, Args &&...>{}, "");
          auto &self = static_cast<mpark::variant<Ts...> &>(*this);
          self.destruct();
          self.template construct<I>(std::forward<Args>(args)...);
        }

        template <typename T, typename... Args>
        void emplace(Args &&... args) {
          static_assert(meta::count<members, T>{} == 1, "");
          emplace<meta::find_index<members, T>{}>(std::forward<Args>(args)...);
        }

        protected:
        using super::super;
        using super::operator[];
        using super::construct;
        using super::destruct;

        constexpr variant() noexcept : index_{meta::npos{}} {}

        constexpr bool valid() const noexcept {
          return index_ != meta::npos{};
        }

        std::size_t index_;

        friend struct unsafe::detail;

      };  // variant

    }  // variant

  }  // detail

  //- 20.N.2 class template `variant`:

  template <typename... Ts> class variant;

  template <> class variant<> {};

  template <typename... Ts>
  class variant : public detail::variant::variant<Ts...> {
    private:
    using super = detail::variant::variant<Ts...>;
    using members = meta::list<Ts...>;

    public:

    //- 20.N.2.1 construction:

    using super::super;

    /* constructs the variant object in an indeterminate state */
    constexpr variant() noexcept : super{} {}

    template <
        std::size_t I,
        typename U,
        typename... Args,
        typename = meta::if_<std::is_constructible<meta::at_c<members, I>,
                                                   std::initializer_list<U> &,
                                                   Args &&...>>>
    explicit constexpr variant(in_place_t (&ip)(meta::size_t<I>),
                               std::initializer_list<U> init,
                               Args &&... args)
        : super(ip, init, std::forward<Args>(args)...) {}

    template <
        typename T,
        typename U,
        typename... Args,
        typename = meta::if_<
            std::is_constructible<T, std::initializer_list<U> &, Args &&...>>>
    explicit constexpr variant(in_place_t (&ip)(meta::id<T>),
                               std::initializer_list<U> init,
                               Args &&... args)
        : super(ip, init, std::forward<Args>(args)...) {}

    template <typename Arg,
              typename = meta::if_<exp::has_best_match<members, Arg &&>>>
    constexpr variant(Arg &&arg)
        : variant(in_place<exp::get_best_match<members, Arg &&>>,
                  std::forward<Arg>(arg)) {}

    variant(const variant &that) {
      static_assert(meta::and_<std::is_copy_constructible<Ts>...>{}, "");
      assert(that.valid());
      type_switch(that)(constructor{*this});
    }

    variant(variant &that) {
      static_assert(meta::and_<std::is_copy_constructible<Ts>...>{}, "");
      assert(that.valid());
      type_switch(that)(constructor{*this});
    }

    variant(const variant &&that) noexcept(
        meta::and_<std::is_nothrow_move_constructible<Ts>...>{}) {
      static_assert(meta::and_<std::is_move_constructible<Ts>...>{}, "");
      assert(that.valid());
      type_switch(std::move(that))(constructor{*this});
    }

    variant(variant &&that) noexcept(
        meta::and_<std::is_nothrow_move_constructible<Ts>...>{}) {
      static_assert(meta::and_<std::is_move_constructible<Ts>...>{}, "");
      assert(that.valid());
      type_switch(std::move(that))(constructor{*this});
    }

    //- 20.N.2.2 destruction:
    ~variant() { destruct(); }

    //- 20.N.2.3 assignment:

    using super::emplace;

    template <std::size_t I, typename U, typename... Args>
    meta::if_<std::is_constructible<meta::at_c<members, I>,
                                    std::initializer_list<U> &,
                                    Args &&...>,
    void> emplace(std::initializer_list<U> init, Args &&... args) {
      super::template emplace<I>(init, std::forward<Args>(args)...);
    }

    template <typename T, typename U, typename... Args>
    meta::if_<std::is_constructible<T, std::initializer_list<U> &, Args &&...>,
    void> emplace(std::initializer_list<U> init, Args &&... args) {
      super::template emplace<T>(init, std::forward<Args>(args)...);
    }

    template <std::size_t I, typename Arg>
    void assign(Arg &&arg) {
      static_assert(I < members::size(), "");
      using T = meta::at_c<members, I>;
      static_assert(std::is_constructible<T, Arg &&>{}, "");
      static_assert(std::is_assignable<T &, Arg &&>{}, "");
      if (this->index_ == I) {
        unsafe::get<I>(*this) = std::forward<Arg>(arg);
      } else if (std::is_nothrow_constructible<T, Arg &&>{}) {
        super::template emplace<I>(std::forward<Arg>(arg));
      } else {
        super::template emplace<I>(T(std::forward<Arg>(arg)));
      }  // if
    }

    template <typename T, typename Arg>
    void assign(Arg &&arg) {
      static_assert(meta::count<members, T>{} == 1, "");
      assign<meta::find_index<members, T>{}>(std::forward<Arg>(arg));
    }

    template <typename Arg>
    meta::if_<exp::has_best_match<members, Arg &&>,
    variant &> operator=(Arg &&arg) {
      assign<exp::get_best_match<members, Arg &&>>(std::forward<Arg>(arg));
      return *this;
    }

    variant &operator=(const variant &that) {
      static_assert(meta::and_<std::is_copy_constructible<Ts>...>{}, "");
      static_assert(meta::and_<std::is_copy_assignable<Ts>...>{}, "");
      assert(that.valid());
      type_switch(that)(assigner{*this});
      return *this;
    }

    variant &operator=(variant &that) {
      static_assert(meta::and_<std::is_copy_constructible<Ts>...>{}, "");
      static_assert(meta::and_<std::is_copy_assignable<Ts>...>{}, "");
      assert(that.valid());
      type_switch(that)(assigner{*this});
      return *this;
    }

    variant &operator=(const variant &&that) noexcept(
        meta::and_<std::is_nothrow_move_constructible<Ts>...,
                   std::is_nothrow_move_assignable<Ts>...>{}) {
      static_assert(meta::and_<std::is_move_constructible<Ts>...>{}, "");
      static_assert(meta::and_<std::is_move_assignable<Ts>...>{}, "");
      assert(that.valid());
      type_switch(std::move(that))(assigner{*this});
      return *this;
    }

    variant &operator=(variant &&that) noexcept(
        meta::and_<std::is_nothrow_move_constructible<Ts>...,
                   std::is_nothrow_move_assignable<Ts>...>{}) {
      static_assert(meta::and_<std::is_move_constructible<Ts>...>{}, "");
      static_assert(meta::and_<std::is_move_assignable<Ts>...>{}, "");
      assert(that.valid());
      type_switch(std::move(that))(assigner{*this});
      return *this;
    }

    //- 20.N.2.4 observers:

    constexpr std::size_t index() const noexcept {
      assert(this->valid());
      return this->index_;
    }

    const std::type_info &type() const noexcept {
      assert(this->valid());
      return type_switch(*this)(typer{});
    }

    //- 20.N.2.5 swap:

    void swap(variant &that) noexcept(
        meta::and_<std::is_nothrow_move_constructible<Ts>...,
                   exp::is_nothrow_swappable<Ts>...>{}) {
      assert(this->valid());
      assert(that.valid());
      type_switch(*this, that)(swapper{*this, that});
    }

    private:
    struct assigner {
      template <std::size_t I, typename Arg>
      void operator()(Arg &&arg) const {
        self.assign<I>(std::forward<Arg>(arg));
      }
      variant &self;
    };  // assigner

    struct constructor {
      template <std::size_t I, typename Arg>
      void operator()(Arg &&arg) const {
        self.construct<I>(std::forward<Arg>(arg));
      }
      variant &self;
    };  // constructor

    struct destructor {
      template <std::size_t I, typename Arg>
      void operator()(Arg &) const {
        self.destruct<I>();
      }
      variant &self;
    };  // destructor

    struct swapper {
      template <typename T>
      void operator()(T &lhs, T &rhs) const {
        using std::swap;
        swap(lhs, rhs);
      }

      template <typename T, typename U>
      void operator()(T &, U &) const {
        std::swap(self, other);
      }

      variant &self;
      variant &other;
    };  // swapper

    struct typer {
      template <std::size_t I, typename Arg>
      const std::type_info &operator()(Arg &&) const noexcept {
        using T = meta::at_c<members, I>;
        return typeid(T);
      }
    };  // typer

    template <std::size_t I, typename... Args>
    void construct(Args &&... args) {
      static_assert(I < members::size(), "");
      try {
        this->index_ = meta::npos{};
        super::construct(meta::size_t<I>{}, std::forward<Args>(args)...);
        this->index_ = I;
      } catch (...) {
        throw bad_variant_assign{this};
      }  // try
    }

    template <std::size_t I>
    void destruct() noexcept {
      static_assert(I < members::size(), "");
      super::destruct(meta::size_t<I>{});
    }

    void destruct() noexcept {
      if (this->valid()) {
        type_switch(*this)(destructor{*this});
      }  // if
    }

    friend super;

  };  // variant

  //- 20.N.9 specialized algorithms:
  template <typename... Ts>
  void swap(variant<Ts...> &lhs,
            variant<Ts...> &rhs) noexcept(noexcept(lhs.swap(rhs))) {
    lhs.swap(rhs);
  }

}  // namespace mpark
