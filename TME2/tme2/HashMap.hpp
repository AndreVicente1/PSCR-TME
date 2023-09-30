#include <forward_list>
using namespace std;
template <typename K, typename V>
class HashMap{
    struct Entry{
        
        const K key;
        V value;
        Entry(const K& k, V& v) : key(k), value(v) {}
    };

    size_t sz;
    vector<forward_list<Entry>> buckets;

    size_t hash(const K& key) const {
        size_t h = std::hash<K>{}(key);
        return h % buckets.size();
    }

    public:
        HashMap(size_t size) : buckets(size) {}
        ~HashMap() {}

        bool put(const K& key,V& value) {
            size_t index = hash(key);
            for (Entry& entry : buckets[index]) {
                if (entry.key == key) {
                    // la clé existe deja, on incremente de 1 la valeur
                    entry.value = entry.value+1;
                    return true;
                }
            }

            // Kla clé n'existe pas, on insert le nouvbvel élement
            buckets[index].emplace_front(Entry(key, value));
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
                count += std::distance(bucket.begin(), bucket.end());
            }
            return count;
    }

};