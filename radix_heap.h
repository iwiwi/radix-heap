#include <algorithm>
#include <cassert>
#include <climits>
#include <cstdint>
#include <type_traits>
#include <vector>
#include <limits>

namespace radix_heap {
namespace internal {
template<typename T>
inline constexpr size_t num_bits() {
  return sizeof(T) * CHAR_BIT;
}

template<typename KeyType, bool IsSigned> class encoder_impl_integer;

template<typename KeyType>
class encoder_impl_integer<KeyType, false> {
 public:
  typedef KeyType key_type;
  typedef KeyType unsigned_key_type;

  inline static constexpr unsigned_key_type encode(key_type x) {
    // puts("UNSIGNED");
    return x;
  }

  inline static constexpr key_type decode(unsigned_key_type x) {
    return x;
  }
};

template<typename KeyType>
class encoder_impl_integer<KeyType, true> {
 public:
  typedef KeyType key_type;
  typedef typename std::make_unsigned<KeyType>::type unsigned_key_type;

  inline static constexpr unsigned_key_type encode(key_type x) {
    //puts("SIGNED");
    return static_cast<unsigned_key_type>(x) ^
        (unsigned_key_type(1) << unsigned_key_type(num_bits<unsigned_key_type>() - 1));
  }

  inline static constexpr key_type decode(unsigned_key_type x) {
    return static_cast<key_type>
        (x ^ (unsigned_key_type(1) << (num_bits<unsigned_key_type>() - 1)));
  }
};

template<typename KeyType, typename UnsignedKeyType>
class encoder_impl_decimal {
public:
  typedef KeyType key_type;
  typedef UnsignedKeyType unsigned_key_type;

  inline static constexpr unsigned_key_type encode(key_type x) {
    return raw_cast<key_type, unsigned_key_type>(x) ^
        ((-(raw_cast<key_type, unsigned_key_type>(x) >> (num_bits<unsigned_key_type>() - 1))) |
         (unsigned_key_type(1) << (num_bits<unsigned_key_type>() - 1)));
  }

  inline static constexpr key_type decode(unsigned_key_type x) {
    return raw_cast<unsigned_key_type, key_type>
        (x ^ (((x >> (num_bits<unsigned_key_type>() - 1)) - 1) |
              (unsigned_key_type(1) << (num_bits<unsigned_key_type>() - 1))));
  }

 private:
  template<typename T, typename U>
  union raw_cast {
   public:
    constexpr raw_cast(T t) : t_(t) {}
    operator U() const { return u_; }

   private:
    T t_;
    U u_;
  };
};

template<typename KeyType>
class encoder : public encoder_impl_integer<KeyType, std::is_signed<KeyType>::value> {};

template<>
class encoder<float> : public encoder_impl_decimal<float, uint32_t> {};

template<>
class encoder<double> : public encoder_impl_decimal<double, uint64_t> {};

template<typename T>
inline constexpr size_t find_bucket(T x, T last) {
  // TODO: template meta programming
  return sizeof(T) == 8 ?
      (x == last ? 0 : 64 - __builtin_clzll(static_cast<uint64_t>(x ^ last))) :
      (x == last ? 0 : 32 - __builtin_clz(static_cast<uint32_t>(x ^ last)));
}
}  // namespace internal

template<typename KeyType, typename EncoderType = internal::encoder<KeyType>>
class radix_heap {
 public:
  typedef KeyType key_type;
  typedef EncoderType encoder_type;
  typedef typename encoder_type::unsigned_key_type unsigned_key_type;

  radix_heap() : size_(0), last_(), buckets_() {}

  void push(key_type key) {
    const unsigned_key_type x = encoder_type::encode(key);
    assert(last_ <= x);
    ++size_;
    buckets_[internal::find_bucket(x, last_)].emplace_back(x);
  }

  key_type top() {
    pull();
    return encoder_type::decode(last_);
  }

  void pop() {
    pull();
    buckets_[0].pop_back();
    --size_;
  }

  size_t size() const {
    return size_;
  }

  bool empty() const {
    return size_ == 0;
  }

  void clear() {
    size_ = 0;
    last_ = key_type();
    for (int i = 0; i <= internal::num_bits<unsigned_key_type>(); ++i) {
      buckets_[i].clear();
    }
  }

  void swap(radix_heap<KeyType, EncoderType> &a) {
    std::swap(size_, a.size_);
    std::swap(last_, a.last_);
    for (int i = 0; i <= internal::num_bits<unsigned_key_type>(); ++i) {
      buckets_[i].swap(a.buckets_[i]);
    }
  }

 private:
  size_t size_;
  unsigned_key_type last_;
  std::vector<unsigned_key_type>
      buckets_[internal::num_bits<unsigned_key_type>() + 1];

  void pull() {
    assert(size_ > 0);
    if (!buckets_[0].empty()) return;
    size_t i;
    for (i = 1; buckets_[i].empty(); ++i);
    last_ = *std::min_element(buckets_[i].begin(), buckets_[i].end());
    for (unsigned_key_type x : buckets_[i]) {
      buckets_[internal::find_bucket(x, last_)].emplace_back(x);
    }
    buckets_[i].clear();
  }
};

template<typename KeyType, typename ValueType, typename EncoderType = internal::encoder<KeyType>>
class pair_radix_heap {
 public:
  typedef KeyType key_type;
  typedef ValueType value_type;
  typedef EncoderType encoder_type;
  typedef typename encoder_type::unsigned_key_type unsigned_key_type;

  pair_radix_heap() : size_(0), last_(), buckets_() {}

  void push(key_type key, const value_type &value) {
    const unsigned_key_type x = encoder_type::encode(key);
    assert(last_ <= x);
    ++size_;
    buckets_[internal::find_bucket(x, last_)].emplace_back(x, value);
  }

  void push(key_type key, value_type &&value) {
    const unsigned_key_type x = encoder_type::encode(key);
    assert(last_ <= x);
    ++size_;
    buckets_[internal::find_bucket(x, last_)].emplace_back(x, std::move(value));
  }

  template <class... Args>
  void emplace(key_type key, Args&&... args) {
    const unsigned_key_type x = encoder_type::encode(key);
    assert(last_ <= x);
    ++size_;
    buckets_[internal::find_bucket(x, last_)].emplace_back(
        std::piecewise_construct, std::forward_as_tuple(x), std::forward_as_tuple(args...));
  }

  key_type top_key() {
    pull();
    return encoder_type::decode(last_);
  }

  value_type &top_value() {
    pull();
    return buckets_[0].back().second;
  }

  void pop() {
    pull();
    buckets_[0].pop_back();
    --size_;
  }

  size_t size() const {
    return size_;
  }

  bool empty() const {
    return size_ == 0;
  }

  void clear() {
    size_ = 0;
    last_ = key_type();
    for (int i = 0; i <= internal::num_bits<unsigned_key_type>(); ++i) {
      buckets_[i].clear();
    }
  }

  void swap(pair_radix_heap<KeyType, ValueType, EncoderType> &a) {
    std::swap(size_, a.size_);
    std::swap(last_, a.last_);
    for (int i = 0; i <= internal::num_bits<unsigned_key_type>(); ++i) {
      buckets_[i].swap(a.buckets_[i]);
    }
  }

 private:
  size_t size_;
  unsigned_key_type last_;
  std::vector<std::pair<unsigned_key_type, value_type>>
      buckets_[internal::num_bits<unsigned_key_type>() + 1];

  void pull() {
    assert(size_ > 0);
    if (!buckets_[0].empty()) return;
    size_t i;
    for (i = 1; buckets_[i].empty(); ++i);

    last_ = std::numeric_limits<unsigned_key_type>::max();
    for (size_t j = 0; j < buckets_[i].size(); ++j) {
      last_ = std::min(last_, buckets_[i][j].first);
    }

    for (size_t j = 0; j < buckets_[i].size(); ++j) {
      const size_t k = internal::find_bucket(buckets_[i][j].first, last_);
      buckets_[k].emplace_back(std::move(buckets_[i][j]));
    }
    buckets_[i].clear();
  }
};
}  // namespace radix_heap
