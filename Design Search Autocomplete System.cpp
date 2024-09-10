//1. hashmap, min heap
//2. trie with list of words for each node, min heap (faster)
//3. trie with min heap of words for each node (fastest)

//time: O(1)
class AutocompleteSystem {
private:
    struct Compare {
        bool operator()(const pair<int, string> &a, const pair<int, string> &b) const {
            if(a.first == b.first) return a.second < b.second;
            return a.first > b.first;
        }
    };

    struct Node {
        Node* child[27];
        set<pair<int, string>, Compare> sentences;

        Node() {
            for(int i = 0; i < 27; i++) child[i] = NULL;
        }
    };

    const int MAX_MATCH_COUNT = 3;

    string query;
    Node* root, *curRoot;
    unordered_map<string, int> sentenceFreq;

    void insert(string sentence, int freq) {
        Node* curNode = root;

        int oldFreq = sentenceFreq[sentence];
        sentenceFreq[sentence] += freq;
        int newFreq = sentenceFreq[sentence];

        for(char c: sentence) {
            int index = c - 'a';
            if (c == ' ') index = 26;

            if(curNode->child[index] == NULL) {
                curNode->child[index] = new Node();
            }
            curNode = curNode->child[index];

            curNode->sentences.erase({oldFreq, sentence});
            curNode->sentences.insert({newFreq, sentence});

            if(curNode->sentences.size() > 3) curNode->sentences.erase(prev(curNode->sentences.end()));
        }
    }

    vector<string> search(char c) {
        vector<string> matching;

        int index = c - 'a';
        if(c == ' ') index = 26;

        if(curRoot== NULL) return vector<string>();
        curRoot = curRoot->child[index];
        if(curRoot== NULL) return vector<string>();

        int sentencesSoFar = 0;
        for(auto it = curRoot->sentences.begin(); it != curRoot->sentences.end(); it++) {
            matching.push_back(it->second);
            
            sentencesSoFar++;
            if(sentencesSoFar == MAX_MATCH_COUNT) break;
        }

        return matching;
    }

public:
    AutocompleteSystem(vector<string>& sentences, vector<int>& times) {
        root = new Node();
        for(int index = 0; index < times.size(); index++) insert(sentences[index], times[index]);
        query = "";
        curRoot = root;
    }
    
    vector<string> input(char c) {
        if(c == '#') {
            insert(query, 1);
            query = "";
            curRoot = root;
            return vector<string>();
        }

        query += c;
        return search(c);
    }
};

// my soln - somehow not working

/*
class AutocompleteSystem {
public:
    class TrieNode {
        public:
        unordered_map<char, TrieNode*> children;
        
        // Custom comparator for the priority queue
        //struct Comparator {
        //    bool operator()(pair<int,string>& a, pair<int,string>& b) {
        //        if (a.first == b.first) {
        //            return a.second > b.second; // Compare strings lexicographically if frequencies are the same
        //        }
        //        return a.first > b.first; // Otherwise, compare frequencies
        //    }
        //};
        //priority_queue<pair<int,string>, vector<pair<int,string>>, Comparator> top3;
        
        priority_queue<pair<int,string>, vector<pair<int,string>>, greater<pair<int,string>>> top3;
    
        TrieNode() {
            
        }

        void insert(TrieNode* root, string word, int freq) {
            TrieNode* curr = root;
            for(int i = 0; i < word.length(); i++) {
                char c = word[i];
                if(curr -> children.find(c) == curr -> children.end()) {
                    curr -> children[c] = new TrieNode();
                }
                curr = curr -> children[c];
                curr -> top3.push({freq, word});
                if((curr -> top3).size() > 3) {
                    curr -> top3.pop();
                }
            }
        }

        vector<string> search(TrieNode* root, string word) {
            TrieNode* curr = root;
            for(int i = 0; i < word.length(); i++) {
                char c = word[i];
                if(curr -> children.find(c) == curr -> children.end()) {
                    return {};
                }
                curr = curr -> children[c];
            }
            int n = curr -> top3.size();
            vector<string> result(n);
            vector<int> freq(n);
            for(int i = n-1; i >= 0; i--) {
                result[i] = curr -> top3.top().second;
                freq[i] = curr -> top3.top().first;
                curr -> top3.pop();
            }
            for(int i = n-1; i >= 0; i--) {
                curr -> top3.push({freq[i], result[i]});
            }
            return result;
        }

        void update(TrieNode* root, string word) {
            TrieNode* curr = root;
            for(int i = 0; i < word.length(); i++) {
                char c = word[i];
                if(curr -> children.find(c) == curr -> children.end()) {
                    curr -> children[c] = new TrieNode();
                }
                curr = curr -> children[c];
                vector<pair<int,string>> vec;
                
                bool flag = false;
                while(!curr -> top3.empty()) {
                    pair<int,string> temp = curr -> top3.top();
                    if(temp.second == word) {
                        vec.push_back({temp.first + 1, temp.second});
                        flag = true;
                    }
                    else {
                        vec.push_back({temp.first, temp.second});
                    }
                    curr -> top3.pop();
                }
                if(!flag) {
                    vec.push_back({1, word});
                }
                for(int i = 0; i < vec.size(); i++) {
                    curr -> top3.push({vec[i].first, vec[i].second});
                }
                if((curr -> top3).size() > 3) {
                    curr -> top3.pop();
                }
                
            }
        }
    };

    string str;
    TrieNode* root;
    AutocompleteSystem(vector<string>& sentences, vector<int>& times) {
        str = "";
        root = new TrieNode();
        for(int i = 0; i < sentences.size(); i++) {
            root -> insert(root, sentences[i], times[i]);
        }
    }
    
    vector<string> input(char c) {
        if(c == '#') {
            root -> update(root, str);
            str = "";
            return {};
        }
        else {
            str += c;
            return root -> search(root, str);
        }
    }
};
*/
/**
 * Your AutocompleteSystem object will be instantiated and called as such:
 * AutocompleteSystem* obj = new AutocompleteSystem(sentences, times);
 * vector<string> param_1 = obj->input(c);
 */