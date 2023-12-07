#include <forward_list>
#include <vector>
#include <functional>

using namespace std;

template <typename K, typename V>
class HashMap {
    struct Entry {
        const K key;
        V value;
        Entry(const K& k, const V& v) : key(k), value(v) {}
    };

    vector<forward_list<Entry>> buckets;

    size_t hash(const K& key) const {
        return std::hash<K>{}(key) % buckets.size();
    }

public:
    explicit HashMap(size_t size) : buckets(size) {}

    bool put(const K& key, const V& value) {
        size_t index = hash(key);
        for (Entry& entry : buckets[index]) {
            if (entry.key == key) {
                entry.value = value;
                return true;
            }
        }
        buckets[index].emplace_front(key, value);
        return false;
    }

    V* get(const K& key) {
        size_t index = hash(key);
        for (Entry& entry : buckets[index]) {
            if (entry.key == key) {
                return &entry.value;
            }
        }
        return nullptr;
    }

    size_t size() const {
        size_t count = 0;
        for (const auto& bucket : buckets) {
            count += distance(bucket.begin(), bucket.end());
        }
        return count;
    }

    class iterator {
        size_t index;
        typename forward_list<Entry>::iterator lit;
        vector<forward_list<Entry>>& buckets;

    public:
        iterator(size_t index, typename forward_list<Entry>::iterator lit, vector<forward_list<Entry>>& buckets)
            : index(index), lit(lit), buckets(buckets) {}

        iterator& operator++() {
            // Advance the iterator in the current bucket
            ++lit;

            // Check if the current bucket iterator is at the end and there are more buckets to consider
            while (index < buckets.size() && lit == buckets[index].end()) {
                // Move to the next bucket
                ++index;

                // If the next bucket is within the range and not empty, set the local iterator to its beginning
                if (index < buckets.size() && !buckets[index].empty()) {
                    lit = buckets[index].begin();
                }
            }

            return *this;
        }


        bool operator!=(const iterator& o) const {
            return index != o.index || lit != o.lit;
        }

        Entry& operator*() {
            return *lit;
        }
    };

    iterator begin() {
        for (size_t index = 0; index < buckets.size(); ++index) {
            if (!buckets[index].empty()) {
                return iterator(index, buckets[index].begin(), buckets);
            }
        }
        return end();
    }

    iterator end() {
        return iterator(buckets.size(), buckets.back().end(), buckets);
    }
};
