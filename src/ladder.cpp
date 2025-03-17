#include "ladder.h"

#define my_assert(e) \
    {  \
        std::cout << #e << ((e) ? " passed" : " failed") << std::endl; \
    }

// Error function
void error(const std::string & word1, const std::string & word2, const std::string & msg) {
    std::cerr << "Error: " << msg << "[word 1 = " << word1
    << ", word2 = " << word2 << "]\n";
}

// Edit_distance
bool edit_distance_within(const std::string & str1, const std::string & str2, int d) {

    int n = static_cast<int>(str1.size());
    int m = static_cast<int>(str2.size());
    // Check lengths
    if (std::abs(n - m) > d) {
        return false;
    }

    // Make table (n + 1)(m + 1)
    // Edit distance
    const int INF = d + 1;
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, INF));

    // Transofrm
    dp[0][0] = 0;
    for (int i = 1; i <= n && i <= d; i++) {
        dp[i][0] = i;
    }
    for (int j = 1; j <= m && j <= d; j++) {
        dp[0][j] = j;
    }

    for (int i = 1; i <= n; i++) {
        // J must be within these constraints
        int start_j = std::max(1, i - d);
        int end_j = std::min(m, i + d);

        // Track
        bool rowAllGreater = true;

        for (int j = start_j; j <= end_j; j++) {
            if (str1[i - 1] == str2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                // Delete
                int del = dp[i - 1][j];
                // Insert
                int ins = dp[i][j - 1];
                // Replace
                int rep = dp[i - 1][j - 1];
                dp[i][j] = 1 + std::min({del, ins, rep});
            }
            // Not all are greater than d
            if (dp[i][j] <= d) {
                rowAllGreater = false;
            }
        }
        // Exit early if row is > d
        if (rowAllGreater) {
            return false;
        }
    }
    return (dp[n][m] <= d);
}

//is_adjacent
bool is_adjacent(const std::string & word1, const std::string & word2) {
    // Mean distance <= 1
    return edit_distance_within(word1, word2, 1);
}

// Get_neighbors helper
std::vector<std::string> get_neighbors(const std::string & word, const std::set<std::string> & word_list) {
    std::vector<std::string> neighbors;

    // Replacements
    for (int i = 0; i < static_cast<int>(word.size()); ++i) {
        char original = word[i];
        for (char c = 'a'; c <= 'z'; ++c) {
            if (c == original) continue;
            std::string temp = word;
            temp[i] = c;
            if (word_list.find(temp) != word_list.end() && is_adjacent(word, temp)) {
                neighbors.push_back(temp);
            }
        }
    }

    // Insertions
    for (int i = 0; i <= static_cast<int>(word.size()); ++i) {
        for (char c = 'a'; c <= 'z'; ++c) {
            std::string temp = word;
            temp.insert(temp.begin() + i, c);
            if (word_list.find(temp) != word_list.end() && is_adjacent(word, temp)) {
                neighbors.push_back(temp);
            }
        }
    }

    // Deletions
    if (!word.empty()) {
        for (int i = 0; i < static_cast<int>(word.size()); ++i) {
            std::string temp = word;
            temp.erase(temp.begin() + i);
            if (word_list.find(temp) != word_list.end() && is_adjacent(word, temp)) {
                neighbors.push_back(temp);
            }
        }
    }

    return neighbors;
}

// Filter helper function
std::set<std::string> filter_dictionary(const std::set<std::string> & word_list, int minLen, int maxLen) {
    std::set<std::string> filtered;
    // Looper of dictionary
    for (const auto & w : word_list) {
        int len = (int)w.size();
        // Only keep words with a length within minLen, maxLen
        if (len >= minLen && len <= maxLen) {
            filtered.insert(w);
        }
    }
    return filtered;
}

// Generate_word_ladder (BFS)
std::vector<std::string> generate_word_ladder(const std::string & begin_word, const std::string & end_word, const std::set<std::string> & word_list) {
    // If begin == end, treat as invalid
    if (begin_word == end_word) {
        return {};
    }
    
    // Cannot form a ladder if end word is not in dictionary
    if (word_list.find(end_word) == word_list.end()) {
        std::cerr << "End word: " << end_word << " is not in dictionary.\n";
        return {};
    }

    // Only allow for 2 letters smaller/longer than min/max length
    int L1 = (int) begin_word.size();
    int L2 = (int) end_word.size();
    int minLen = std::max(1, std::min(L1, L2) - 2);
    int maxLen = std::max(L1, L2) + 2;

    // Filter dictionary
    std::set<std::string> filtered_word_list = filter_dictionary(word_list, minLen, maxLen);

    // BFS Queue
    std::queue< std::vector<std::string> > ladder_queue;

    // Start ladder containing only begin_word
    ladder_queue.push({ begin_word });

    // Set of words used in ladder
    std::set<std::string> visited;
    visited.insert(begin_word);

    // BFS
    while (!ladder_queue.empty()) {
        // Take the next ladder in front of the queue
        std::vector<std::string> ladder = ladder_queue.front();
        ladder_queue.pop();

        // Last word in ladder
        const std::string & last_word = ladder.back();

        // Generate valid neighbors
        std::vector<std::string> neighbors = get_neighbors(last_word, word_list);
        // Sort alphabetically to expand in order
        std::sort(neighbors.begin(), neighbors.end());
        
        // Check each neighbor
        for (const std::string & candidate : neighbors) {
            // If visited = valid candidate for BFS
            if (visited.find(candidate) == visited.end()) {
                visited.insert(candidate);

                // Extend current ladder
                std::vector<std::string> new_ladder = ladder;
                new_ladder.push_back(candidate);

                // Return immediately if at end_word
                if (candidate == end_word) {
                    return new_ladder;
                }
                ladder_queue.push(new_ladder);
            }
        }
    }
    // No ladder
    return {};
}

// Load words 
void load_words(std::set<std::string> & word_list, const std::string & file_name) {
    // Cannot open file
    std::ifstream in(file_name.c_str());
    if (!in.is_open()) {
        std::cerr << "Cannot open file" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    std::string w;
    while (in >> w) {
        for (std::size_t i = 0; i < w.size(); i++) {
            w[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(w[i])));
        }
        word_list.insert(w);
    }
    in.close();
}

// Print word ladder
void print_word_ladder(const std::vector<std::string> & ladder) {
    if (ladder.empty()) {
        std::cout << "No word ladder found.\n";
        return;
    }
    std::cout << "Word ladder found: ";
    for (std::size_t i = 0; i < ladder.size(); i++) {
        std::cout << ladder[i];
        if (i + 1 < ladder.size()) {
            std::cout << " ";
        } else {
            std::cout << " ";
        }
    }
    std::cout << "\n";
}

void verify_word_ladder() {
    std::set<std::string> word_list;
    load_words(word_list, "src/words.txt");
}