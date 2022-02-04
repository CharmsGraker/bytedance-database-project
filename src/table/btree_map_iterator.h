//
// Created by nekonoyume on 2022/1/24.
//

#ifndef INC_2_3TREE_BTREE_MAP_ITERATOR_H
#define INC_2_3TREE_BTREE_MAP_ITERATOR_H

#include <stack>
#include "btree_map.h"

template<typename K, typename V, typename Node>
class Iterator {
protected:
    std::stack <pair<Node *, int>> q; // to identify idx child
public:
    virtual bool hasNext() {
        return !q.empty();
    }

    virtual std::pair <K, vector<V>> next() = 0;
};





template<typename K, typename V, typename Node>
class InOrderIterator : public Iterator<K, V, Node> {
    Node *cache_leaf = nullptr;
    int cursor = 0;
    typedef vector <V> valueContainer;

private:
    Node *searchAndPush(Node *node,int k, int *idx) {
        // Find the first key greater than or equal to k
        if(!node) {
            return nullptr;
        }
        int i = 0;
        this->q.push({node,i});

        while (i < node->n && k > node->keys[i])
            i++;

        // If the found key is equal to k, return this node
        if (node->keys[i] == k) {
            if (idx) {
                *idx = i;
            }
            return node;
        }

        // If key is not found here and this is a leaf node
        if (node->leaf) {
            return nullptr;
        }


        // Go to the appropriate child
        return searchAndPush(node->C[i],k, idx);
    }
public:
    InOrderIterator(BTree<V> *tree) {
        if (tree->empty()) {
            printf("btree is empty\n");
            return;
        }
        this->q.push({tree->root, 0});
        cache_leaf = nullptr;
        cursor = 0;
    }

    // for search all key above k
    InOrderIterator(BTree<V> *tree, K k) {
        int idx=-1;

        if(searchAndPush(tree->root, k, &idx)== nullptr) {
            while(!this->q.empty()) {
                this->q.pop();
            }
        }
    }

    InOrderIterator(BTreeNode<V> *node, int idx) {
        if (!node) {
            printf("invalid node\n");
            return;
        }
        this->q.push({node, idx});
        cache_leaf = nullptr;
        cursor = 0;
    }

    std::pair <K, valueContainer> next() override {
        if (cache_leaf && cursor < cache_leaf->n) {
            /** hit leaf, return the key[idx] of keys*/
            std::pair <K, valueContainer> kv = {cache_leaf->keys[cursor], cache_leaf->values[cursor]};
            ++cursor;
            return kv;
        } else {
            while (!this->q.empty()) {
                std::pair < Node * , int > member = this->q.top();
                this->q.pop();

                Node *cur = member.first;
                int i = member.second;
                if (cur->leaf) {
                    cache_leaf = cur;
                    cursor = 0;
//                    printf("n=%d,",cache_leaf->n);
                    std::pair <K, valueContainer> pair = {cache_leaf->keys[cursor],
                                                          cache_leaf->values[cursor]};
                    ++cursor;
                    return pair;
                } else if (i < cur->n + 1) {
                    std::pair <K, valueContainer> kv_val;
                    if (i > 0) {
                        kv_val = {cur->keys[i - 1], cur->values[i - 1]};
                    }
                    /** the stack may hold invalid node,
                     * to avoid this, check before push next child */
                    if (i + 1 <= cur->n)
                        this->q.push({cur, i + 1});
                    this->q.push({cur->C[i], 0});
                    if (i > 0) {
                        return kv_val;
                    }
                }
            }
        }
        return {1, {V()}};
    }

    bool hasNext() override {
        return (cache_leaf != nullptr && cursor < cache_leaf->n) || (!this->q.empty());
    }
};

#endif //INC_2_3TREE_BTREE_MAP_ITERATOR_H
