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

    class iterator{

        typename forward_list<Entry>::iterator it;
        size_t index;
        vector<forward_list<Entry>> *buckets;

        iterator () : buckets (&buckets),index(0),it(){}

        Entry& operator*(){ 
            return *it; 
        }
        iterator& operator++(){ 
            // fin de la forwardlist - on cherche le prochain buckket
            if (++it == buckets[index].end()){
                while (buckets[++index]==nullptr ){
                    if (index == sz) {
                        it = end();
                        return *this;
                    }
                }
                it = buckets[index].begin();
            }
            return *this;
        }
        bool operator!=(const iterator &other){
            return index!=other.index || it!=other.it || buckets!=other.buckets;
        }
    };
    iterator begin () {
            size_t index = 0;
            for (;index<buckets.size();++index){
                if (!buckets[index].empty()){
                    break;
                }
            }
            if (sz == 0) return end();

            return iterator(buckets,index,buckets[index].begin());
        }
    iterator end () { return nullptr;}

};

