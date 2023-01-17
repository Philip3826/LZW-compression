#include <vector>
#include <list>
template<typename K , typename V>
class HashMap
{
private:
	struct HashNode {
		K key;
		V value;
		HashNode(K key, V value) : key(key) , value(value)
		{}
	};
	std::vector<std::list<HashNode>> data;
	size_t capacity;
	size_t hashCode(K key);
	std::hash<K> hasher;
public:
	HashMap(size_t capacity = 10'000);
	HashMap(HashMap& other) = delete; // to be decided if needed
	HashMap& operator = (HashMap& other) = delete;
	~HashMap();
	HashNode* find(K key)
	{
		size_t index = hashCode(key);
		for (auto& currentNode : data[index])
		{
			if (currentNode.key == key) return &currentNode;
		}

		return nullptr;
	}
	bool contains(K key);
	void insert(K key, V value);
};

template<typename K, typename V>
size_t HashMap<K, V>::hashCode(K key)
{
	return hasher(key) % capacity;
}
template <typename K , typename V>
HashMap<K, V>::HashMap(size_t capacity)
{
	this->capacity = capacity;
	hasher = std::hash<K>();
	data = std::vector<std::list<HashNode>>(capacity, std::list<HashNode>());
}
template<typename K, typename V>
HashMap<K, V>::~HashMap()
{
}

template<typename K, typename V>
bool HashMap<K, V>::contains(K key)
{
	size_t index = hashCode(key);
	for (auto& currentNode : data[index])
	{
		if (currentNode.key == key) return true;
	}

	return false;
}

template<typename K, typename V>
void HashMap<K, V>::insert(K key, V value)
{
	HashNode newEntry(key, value);
	data[hashCode(key)].push_back(newEntry);
}




