class ProductOfNumbers {
public:
    vector<int> prefixProduct;
    ProductOfNumbers() {
        prefixProduct.push_back(1);
    }

    // add number at the back
    // current TC = O(1)
    // expected TC = O(1)
    void add(int num) {
        // if this is the first element we are adding
        if (num == 0) {
            prefixProduct.clear();
            prefixProduct.push_back(1);
        } else {
            prefixProduct.push_back(prefixProduct.back() * num);
        }
    }

    // return product of last k numbers
    // current TC = O(k)
    // expected TC = O(1)
    int getProduct(int k) {
        if (k >= prefixProduct.size())
            return 0;
        return prefixProduct.back() / prefixProduct[prefixProduct.size() - k - 1];
    }
};

/**
 * Your ProductOfNumbers object will be instantiated and called as such:
 * ProductOfNumbers* obj = new ProductOfNumbers();
 * obj->add(num);
 * int param_2 = obj->getProduct(k);
 */
