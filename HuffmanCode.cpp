#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <string>
#include <functional> // Required for std::function

using namespace std;

// A node in the Huffman tree
struct Node {
    char ch;        // Character
    int freq;       // Frequency
    Node* left;     // Left child
    Node* right;    // Right child

    Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};

// Comparator for priority queue
struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq; // Higher frequency nodes have lower priority
    }
};

// Function to generate the Huffman codes from the tree
void buildHuffmanCodes(Node* root, string code, unordered_map<char, string>& huffmanCodes) {
    if (!root) return;

    // If a leaf node, store the character and its code
    if (!root->left && !root->right) {
        huffmanCodes[root->ch] = code;
    }

    buildHuffmanCodes(root->left, code + "0", huffmanCodes);
    buildHuffmanCodes(root->right, code + "1", huffmanCodes);
}

// Function to build the Huffman Tree and generate codes
Node* buildHuffmanTree(const string& data, unordered_map<char, string>& huffmanCodes) {
    // Step 1: Count frequency of each character
    unordered_map<char, int> freqTable;
    for (char ch : data) freqTable[ch]++;

    // Step 2: Create a priority queue (min-heap)
    priority_queue<Node*, vector<Node*>, Compare> pq;

    for (const auto& pair : freqTable) {
        pq.push(new Node(pair.first, pair.second));
    }

    // Step 3: Build the Huffman Tree
    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();

        // Merge two smallest nodes
        Node* merged = new Node('\0', left->freq + right->freq);
        merged->left = left;
        merged->right = right;
        pq.push(merged);
    }

    // Step 4: Generate Huffman codes
    Node* root = pq.top();
    buildHuffmanCodes(root, "", huffmanCodes);

    return root;
}

// Function to compress the input string
string compress(const string& data, unordered_map<char, string>& huffmanCodes) {
    string compressed = "";
    for (char ch : data) {
        compressed += huffmanCodes[ch];
    }
    return compressed;
}

// Function to decompress the binary string
string decompress(const string& compressed, Node* root) {
    string decompressed = "";
    Node* current = root;

    for (char bit : compressed) {
        if (bit == '0') current = current->left;
        else current = current->right;

        // If leaf node, append the character
        if (!current->left && !current->right) {
            decompressed += current->ch;
            current = root;
        }
    }
    return decompressed;
}

// Main function
int main() {
    string data = "huffman coding algorithm example";

    // Step 1: Build the Huffman tree and codes
    unordered_map<char, string> huffmanCodes;
    Node* root = buildHuffmanTree(data, huffmanCodes);

    // Step 2: Compress the data
    string compressed = compress(data, huffmanCodes);

    // Display Huffman codes
    cout << "Huffman Codes:" << endl;
    for (const auto& pair : huffmanCodes) {
        cout << pair.first << ": " << pair.second << endl;
    }

    // Step 3: Display original and compressed data
    cout << "\nOriginal string: " << data << endl;
    cout << "Compressed string (binary): " << compressed << endl;

    // Step 4: Decompress the data
    string decompressed = decompress(compressed, root);
    cout << "\nDecompressed string: " << decompressed << endl;

    return 0;
}
