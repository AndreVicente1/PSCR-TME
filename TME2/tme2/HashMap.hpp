using namespace std;
template <typename K, typename V>
class HashMap{
    struct Entry{
        
        const K key;
        V value;
        Entry(const K& k, V& v) : key(k), value(v) {}
    };

    size_t sz;
    vector<list<Entry>> buckets;

    size_t hash(const K& key) const {
        // You can implement your own hash function here or use std::hash.
        return std::hash<K>{}(key) % capacity;
    }

    public:
        HashMap(size_t initialCapacity = 16) : sz(initialCapacity), bucket(initialCapacity) {}
        ~HashMap() {}

        void insert(const K& key, V& value) {
            size_t index = hash(key);
            for (Entry& entry : buckets[index]) {
                if (entry.key == key) {
                    // Key already exists, update the value.
                    entry.value = value;
                    return;
                }
            }

            // Key does not exist in this bucket, insert a new entry.
            buckets[index].emplace_back(key, value);
        }

};