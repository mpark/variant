#pragma once

#include <tuple>
#include <type_traits>
#include <utility>

#include <exp/tuple.hpp>

#include <mpark/variant/detail/overload.hpp>
#include <mpark/variant/detail/vtable.hpp>

namespace mpark {

  namespace detail {

    namespace variant {

      template <typename R, typename... Vs>
      class TypeSwitch {
        public:
        TypeSwitch(std::tuple<Vs...> &&vs) : vs_(std::move(vs)) {}

        template <typename F>
        decltype(auto) operator()(F &&f) const && {
          return dispatch(exp::priority<2>{}, std::forward<F>(f));
        }

        template <typename... Fs>
        decltype(auto) operator()(Fs &&... fs) const && {
          return std::move(*this)(overload(std::forward<Fs>(fs)...));
        }

        private:
        template <typename F,
                  typename S = R,
                  typename = meta::if_<meta::not_<std::is_same<S, deduce_t>>>>
        decltype(auto) dispatch(exp::priority<2>, F &&f) const {
          return dispatch<S>(std::forward<F>(f));
        }

        template <typename F,
                  typename S = typename std::decay_t<F>::result_type>
        decltype(auto) dispatch(exp::priority<1>, F &&f) const {
          return dispatch<S>(std::forward<F>(f));
        }

        template <typename F,
                  typename S = R,
                  typename = meta::if_<std::is_same<S, deduce_t>>>
        decltype(auto) dispatch(exp::priority<0>, F &&f) const {
          return dispatch<deduce_t>(std::forward<F>(f));
        }

        template <typename S, typename F>
        decltype(auto) dispatch(F &&f) const {
          static constexpr auto vtable = make_vtable<S, F &&, Vs...>();
          return exp::apply([&](Vs... vs) -> decltype(auto) {
            return at(vtable, {vs.index()...})(std::forward<F>(f),
                                               static_cast<Vs>(vs)...);
          }, std::move(this->vs_));
        }

        std::tuple<Vs...> vs_;
      };  // TypeSwitch

    }  // namespace variant

  }  // namespace detail

  //- 20.N.11 visitation:
  template <typename R = detail::variant::deduce_t, typename... Vs>
  detail::variant::TypeSwitch<R, Vs &&...> type_switch(Vs &&... vs) {
    return std::forward_as_tuple(std::forward<Vs>(vs)...);
  }

}  // namespace mpark
