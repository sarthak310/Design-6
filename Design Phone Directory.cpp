class PhoneDirectory {
    int maxNumbers;
    priority_queue<int, vector<int>, greater<int>> pq;
    unordered_set<int> uset;
public:
    PhoneDirectory(int maxNumbers) {
        maxNumbers = maxNumbers;
        for(int i = 0; i < maxNumbers; i++) {
            pq.push(i);
            uset.insert(i);
        }
    }
    
    int get() {
        if(uset.empty()) return -1;
        int curr = pq.top();
        pq.pop();
        uset.erase(curr);
        return curr;
    }
    
    bool check(int number) {
        if(uset.find(number) != uset.end()) return true;
        return false;
    }
    
    void release(int number) {
        if(uset.find(number) == uset.end()) {
            pq.push(number);
            uset.insert(number);
        }
    }
};

/**
 * Your PhoneDirectory object will be instantiated and called as such:
 * PhoneDirectory* obj = new PhoneDirectory(maxNumbers);
 * int param_1 = obj->get();
 * bool param_2 = obj->check(number);
 * obj->release(number);
 */