### MPark.Variant                                                                          [variant]

### In general                                                                     [variant.general]

// TODO

#### Header `<mpark/variant.hpp>` synopsis

```cpp
namespace mpark {

  // class template `variant`:
  template <typename... Ts> class variant;

  // class `bad_variant_access`:
  class bad_variant_access;

  // in-place construction tag:
  template <typename T> struct in_place_t {};
  template <typename T> constexpr in_place_t<T> in_place = {};

  // null tag:
  struct null_t {};
  constexpr null_t null = {};

  // element access:
  template <typename T, typename... Ts>
  std::remove_reference_t<T> *get(variant<Ts...> *) noexcept;
  template <typename T, typename... Ts>
  std::remove_reference_t<const T> *get(const variant<Ts...> *) noexcept;

  template <typename T, typename... Ts> T &get(variant<Ts...> &);
  template <typename T, typename... Ts> const T &get(const variant<Ts...> &);
  template <typename T, typename... Ts> T &get(variant<Ts...> &&);
  template <typename T, typename... Ts> const T &&get(const variant<Ts...> &&);

  // visitation:
  template <typename F, typename... Vs> decltype(auto) apply(F &&, Vs &&...);
  template <template <typename...> class F, typename... Vs> decltype(auto) apply(Vs &&...);

  // specialized algorithms:
  template <typename... Ts> void swap(variant<Ts...> &, variant<Ts...> &) noexcept(see below);
  template <typename... Ts, typename... Us> variant<Ts...> variant_cast(const variant<Us...> &);

}  // namespace mpark

// `variant` hash support:
namespace std {

  template <>
  struct hash<mpark::null_t>;

  template <typename... Ts>
  struct hash<mpark::variant<Ts...>>;

}  // namespace std
```

### Class template `variant`                                                       [variant.variant]

```cpp
namespace mpark {

  template <typename... Ts>
  class variant {
    public:

    // `variant` construction:
    constexpr variant() noexcept;  // only if `null_t` is in `Ts...`

    variant(const variant &);
    variant(variant &&) noexcept(see below);
    template <typename... Us>
    variant(const variant<Us...> &);  // only if ...  see below
    template <typename... Us>
    variant(variant<Us...> &&) noexcept(see below);  // only if ...  see below

    template <typename T> constexpr variant(T &&);  // only if ... see below
    template <typename T> constexpr variant(std::initializer_list<T>);  // only if ... see below
    template <typename T, typename... Args>
    explicit constexpr variant(in_place_t<T>, Args &&...);
    template <typename T, typename U, typename... Args>
    explicit constexpr variant(in_place_t<T>,
                               std::initializer_list<U>,
                               Args &&...);  // only if ... see below

    // `variant` destruction:
    ~variant();

    // `variant` assignment:
    variant &operator=(const variant &);
    variant &operator=(variant &&) noexcept(see below);

    template <typename... Us>
    variant &operator=(const variant<Us...> &);  // only if ... see below
    template <typename... Us>
    variant &operator=(variant<Us...> &&) noexcept(see below);  // only if ... see below

    template <typename T> variant &operator=(T &&);  // only if ... see below
    template <typename T> variant &operator=(std::initializer_list<T>);  // only if ... see below

    template <typename T, typename... Args>
    void emplace(Args &&...);
    template <typename T, typename U, typename... Args>
    void emplace(std::initializer_list<U>, Args &&...);  // only if ... see below

    // `variant` swap:
    void swap(variant &) noexcept(see below);

    // `variant` query:
    explicit constexpr operator bool() const noexcept;  // only if `null_t` is in `Ts...`
    const std::type_info &type() const noexcept;
    bool valid() const noexcept;

  };  // variant

}  // namespace mpark
```

### Construction                                                                     [variant.cnstr]

```cpp
constexpr variant() noexcept;  // only if `null_t` is in `Ts...`
```

__Effects:__ Constructs the `variant` holding an instance of `null_t`.

__Remarks:__ This constructor shall not participate in overload resolution unless `null_t` is in
             `Ts...`.

```cpp
variant(const variant &that);
```

Let `U` be the type of the value in `that`.

__Requires:__ `std::is_copy_constructible_v<T>` is `true` for all `T` in `Ts...`, and `that.valid()`
              must be `true`.

__Effects:__ Initializes the contained value of the `variant` as if constructing an object of type
             `U` with the argument `get<U>(that)`.

__Throws:__ Any exception thrown by the selected constructor of `U`.

```cpp
variant(variant &&that) noexcept(see below);
```

Let `U` be the type of the value in `that`.

__Requires:__ `std::is_move_constructible_v<T>` is `true` for all `T` in `Ts...`, and `that.valid()`
              must be `true`.

__Effects:__ Initializes the contained value of the `variant` as if constructing an object of type
             `U` with the argument `get<U>(std::move(that))`.

__Throws:__ Any exception thrown by the selected constructor of `U`.

__Remarks:__ The expression inside `noexcept` is equivalent to the logical `AND` of
             `std::is_nothrow_move_constructible_v<T>` for all `T` in `Ts...`.

```cpp
template <typename... Us>
variant(const variant<Us...> &);  // only if ... see below
```

// TODO

```cpp
template <typename... Us>
variant(variant<Us...> &&) noexcept(see below);  // only if ... see below
```

// TODO

```cpp
template <typename T> constexpr variant(T &&t);  // only if ... see below
```

Let `U` be one of the types in `Ts...` for which `T &&` is unambiguously convertible to by standard
overload resolution rules.

__Requires:__ `std::is_constructible_v<U, T &&>` is `true`.

__Effects:__ Equivalent to calling `template <typename T, typename... Args>
             variant(in_place_t<T>, Args &&...)` with the arguments
             `in_place<U>, std::forward<T>(t)`.

__Throws:__ Any exception thrown by the selected constructor of `U`.

__Remarks:__ This constructor shall not participate in overload resolution unless there is a type
             `U` in `Ts...` for which `T &&` is unambiguously convertible to by standard overload
             resolution rules. If the selected constructor of `U` is a `constexpr` constructor,
             this constructor shall be a `constexpr` constructor.

```cpp
template <typename T> constexpr variant(std::initializer_list<T> init);  // only if ... see below
```

Let `U` be one of the types in `Ts...` for which `std::initializer_list<T> &` is unambiguously
convertible to by standard overload resolution rules.

__Requires:__ `std::is_constructible_v<U, std::initializer_list<T> &>` is `true`.

__Effects:__ Equivalent to calling `template <typename T> variant(T &&)` with the argument `init`.

__Throws:__ Any exception thrown by the selected constructor of `U`.

__Remarks:__ This constructor shall not participate in overload resolution unless there is a type
             `U` in `Ts...` for which `std::initializer_list<T> &` is unambiguously convertible to
             by standard overload resolution rules. If the selected constructor of `U` is a
             `constexpr` constructor, this constructor shall be a `constexpr` constructor.

```cpp
template <typename T, typename... Args>
explicit constexpr variant(in_place_t<T>, Args &&... args);
```

__Requires:__ `T` is one of `Ts...`, `std::is_constructible_v<T, Args &&...>` is `true`.

__Effects:__ Initializes the contained value of the `variant` as if constructing an object of type
             `T` with the arguments `std::forward<Args>(args)...`.

__Throws:__ Any exception thrown by the selected constructor of `T`.

__Remarks:__ If the selected constructor of `U` is a `constexpr` constructor, this constructor
             shall be a `constexpr` constructor.

```cpp
template <typename T, typename U, typename... Args>
explicit constexpr variant(in_place_t<T>,
                           std::initializer_list<U> init,
                           Args &&... args);  // only if ... see below
```

__Requires:__ `T` is one of `Ts...`.

__Effects:__ Equivalent to calling `template <typename T, typename... Args>
             variant(in_place_t<T>, Args &&... args);` with the arguments
             `init, std::forward<Args>(args)...`.

__Throws:__ Any exception thrown by the selected constructor of `T`.

__Remarks:__ This constructor shall not participate in overload resolution unless
             `std::is_constructible_v<T, std::initializer_list<U> &, Args &&...>` is `true`.
             If the selected constructor of `U` is a `constexpr` constructor, this constructor
             shall be a `constexpr` constructor.

### Destruction                                                                       [variant.dtor]

```cpp
~variant();
```

__Effects:__ If `valid()` is `true`, calls `get<U>(*this).~U();` where `U` is the type of the value
             in `*this`.

### Assignment                                                                      [variant.assign]

```cpp
variant &operator=(const variant &that);
```

Let `U` be the type of the value in `that`.

__Requires:__ `std::is_copy_constructible_v<T>` and `std::is_copy_assignable_v<T>` is `true` for all
              `T` in `Ts...`, and `that.valid()` must be `true`.

__Effects:__ If the type of the value in `*this` and `that` are both `U`, calls
             `get<U>(*this) = get<U>(that);` Otherwise, constructs a temporary object `temp` of
             type `U` with the argument `get<U>(that)`, destructs the value in `*this`, then
             initializes the contained value of `*this` as if constructing an object type `U` with
             the argument `std::move(temp)`.

__Returns:__ `*this`

__Exception Safety:__ If the type of the value in `*this` and `that` are both `U`, the state of the
                      value in `*this` is determined by the exception safety specification of the
                      selected assignment operator of `U`. Otherwise, if an exception is thrown
                      during the call to the copy constructor of `U`, `*this` is unaffected.
                      If an exception is thrown during the call to the move constructor of `U`,
                      no copy assignment takes place and `valid()` will be `false`.

```cpp
variant &operator=(variant &&that) noexcept(see below);
```

__Requires:__ `std::is_move_constructible_v<T>` and `std::is_move_assignable_v<T>` is `true` for all
              `T` in `Ts...`, and `that.valid()` must be `true`.

__Effects:__ If the type of the value in `*this` and `that` are both `U`, calls
             `get<U>(*this) = get<U>(std::move(that));` Otherwise, constructs a temporary object
             `temp` of type `U` with the argument `get<U>(std::move(that))`, destructs the
             value in `*this`, then initializes the contained value of `*this` as if constructing
             an object type `U` with the argument `std::move(temp)`.

__Returns:__ `*this`

__Remarks:__ The expression inside `noexcept` is equivalent to the logical `AND` of
             `std::is_nothrow_move_constructible_v<T>` &&
             `std::is_nothrow_move_assignable_v<T>` for all `T` in `Ts...`.

__Exception Safety:__ If the type of the value in `*this` and `that` are both `U`, the state of the
                      value in `*this` is determined by the exception safety specification of the
                      selected assignment operator of `U`. Otherwise, if an exception is thrown
                      during the call to the copy constructor of `U`, `*this` is unaffected.
                      If an exception is thrown during the call to the move constructor of `U`,
                      no copy assignment takes place and `valid()` will be `false`.

```cpp
template <typename... Us>
variant &operator=(const variant<Us...> &);  // only if ... see below
```

// TODO

```cpp
template <typename... Us>
variant &operator=(variant<Us...> &&) noexcept(see below);  // only if ... see below
```

// TODO

```cpp
template <typename T> variant &operator=(T &&t);  // only if ... see below
```

Let `U` be one of the types in `Ts...` for which `T &&` is unambiguously convertible to by standard
overload resolution rules.

__Requires:__ `std::is_constructible_v<U, T &&>` and `std::is_assignable_v<U &, T &&>` is `true`.

__Effects:__ If the type of the value in `*this` is `U`, calls
             `get<U>(*this) = std::forward<T>(t);` Otherwise, constructs a temporary object `temp`
             of type `U` with the argument `std::forward<T>(t)`, destructs the value in `*this`,
             then initializes the contained value of `*this` as if constructing an object
             type `U` with the argument `std::move(temp)`.

__Returns:__ `*this`

__Remarks:__ This constructor shall not participate in overload resolution unless there is a type
             `U` in `Ts...` for which `T &&` is unambiguously convertible to by standard overload
             resolution rules.

__Exception Safety:__ If the type of the value in `*this` is `U`, the state of the value in `*this`
                      is determined by the exception safety specification of the selected assignment
                      operator of `U`. Otherwise, if an exception is thrown during the construction
                      of the temporary object, `*this` is unaffected. If an exception is thrown
                      during the initialization of the value in `*this`, no assignment takes place
                      and `valid()` will be `false`.

```cpp
template <typename T> variant &operator=(std::initializer_list<T> init);  // only if ... see below
```

Let `U` be one of the types in `Ts...` for which `std::initializer_list<T> &` is unambiguously
convertible to by standard overload resolution rules.

__Requires:__ `std::is_constructible_v<U, std::initializer_list<T> &>` and
              `std::is_assignable_v<U &, std::initialier_list<T> &>` is `true`.

__Effects:__ Equivalent to calling `template <typename T> operator=(T &&)` with the argument `init`.

__Remarks:__ This constructor shall not participate in overload resolution unless there is a type
             `U` in `Ts...` for which `std::initializer_list<T> &` is unambiguously convertible to
             by standard overload resolution rules.

```cpp
template <typename T, typename... Args>
void emplace(Args &&... args);
```

__Requires:__ `T` is one of `Ts...`, `std::is_constructible_v<T, Args &&...>` is `true`.

__Effects:__ Destructs the value in `*this`, then initializes the contained value of `*this` as if
             constructing an object type `U` with the arguments `std::forward<Args>(args)...`.

__Returns:__ `*this`

__Throws:__ Any exception thrown by the selected constructor of `T`.

__Exception Safety:__ If an exception is thrown during the initialization of the value in `*this`,
                      no initialization takes place and `valid()` will be `false`.

```cpp
template <typename T, typename U, typename... Args>
void emplace(std::initializer_list<U> init, Args &&... args);  // only if ... see below
```

__Requires:__ `T` is one of `Ts...`.

__Effects:__ Equivalent to calling `template <typename T, typename... Args>
             emplace(Args &&... args);` with the arguments `init, std::forward<Args>(args)...`.

__Returns:__ `*this`

__Throws:__ Any exception thrown by the selected constructor of `T`.

__Remarks:__ This constructor shall not participate in overload resolution unless
             `std::is_constructible_v<T, std::initializer_list<U> &, Args &&...>` is `true`.

__Exception Safety:__ If an exception is thrown during the initialization of the value in `*this`,
                      no initialization takes place and `valid()` will be `false`.

### Query                                                                            [variant.query]

```cpp
explicit constexpr operator bool() const noexcept;  // only if `null_t` is in `Ts...`
```

__Requires:__ `valid()` must be `true`.

__Returns:__ `true` if `*this` is null, otherwise `false`.

__Remarks:__ This constructor shall not participate in overload resolution unless `null_t` is in
             `Ts...`.

```cpp
const std::type_info &type() const noexcept;
```

__Requires:__ `valid()` must be `true`.

__Returns:__ `typeid(U)` where the type of the value in `*this` is `U`.

```cpp
bool valid() const noexcept;
```

__Returns:__ `true` if `*this` contains a value, otherwise `false`.

### Swap                                                                              [variant.swap]

```cpp
void swap(variant &that) noexcept(see below);
```

__Requires:__ `std::is_move_constructible_v<T>` is `true` for all `T` in `Ts...`, `T &` shall be
              [Swappable] and `valid()` and `that.valid()` must be both `true`.

[Swappable]: http://en.cppreference.com/w/cpp/concept/Swappable

__Effects:__ If the type of the value in `*this` and `that` are both `U`, calls
             `swap(get<U>(*this), get<U>(that));` Otherwise, calls `std::swap(*this, that);`.

__Remarks:__ The expression inside `noexcept` is equivalent to the logical `AND` of
             `std::is_nothrow_move_constructible_v<Ts>...` and
             `noexcept(swap(std::declval<Ts &>(), std::declval<Ts &>()))...` where `std::swap` is
             in scope.

__Exception Safety:__ If an exception is thrown during the call to
                      `swap(get<U>(*this), get<U>(that)`, the state of the value in `*this` and
                      `that` is determined by the exception safety guarantees of `swap` for `U &`.
                      If an exception is thrown during the call to `swap(*this, that)`, the state of
                      the value in `*this` and `that` is determined by the exception safety
                      guarantee of `variant`'s move constructor and assignment operator.

### Element access                                                                    [variant.elem]

// TODO

### Visitation                                                                       [variant.visit]

// TODO

### Specialized algorithms                                                         [variant.special]

```cpp
template <typename... Ts>
void swap(variant<Ts...> &lhs, variant<Ts...> &rhs) noexcept(noexcept(lhs.swap(rhs)));
```

__Effects:__ Calls `lhs.swap(rhs)`.

### Hash support                                                                      [variant.hash]

// TODO
