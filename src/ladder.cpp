#include "ladder.h"
#include <cmath>
#include <cctype>

#define my_assert(e) \
    {  \
        std::cout << #e << ((e) ? " passed" : " failed") << std::endl; \
    }

// Error function
void error(const std::string & word1, const std::string & word2, const std::string & msg) {
    std::cerr << "Error: " << msg << "[word 1 = " << word1
    << ", word2 = " << word2 << "]\n";
}

//is_adjacent
bool is_adjacent(const std::string & word1, const std::string & word2) {
    
    // Check Lengths
    int len1 = static_cast<int>(word1.size());
    int len2 = static_cast<int>(word2.size());
    int diff = len1 - len2;

    // Cannot be adjavent if lengths differ more than 1
    if (std::abs(diff) > 1) {
        return false;
    }

    // Must differ in exactly one position if same length
    if (len1 == len2) {
        int mismatches = 0;
        for (int i = 0; i < len1; i++) {
            if (word1[i] != word2[i]) {
                mismatches++;
                if (mismatches > 1) {
                    return false;
                }
            }
        }
        // True if mismatch is 1
        return (mismatches == 1);
    }

    // If Word2 is one character longer, check if word1 can be formed
    if (len1 + 1 == len2) {
        // Check
        int i = 0, j = 0;
        bool foundExtra = false;
        // Word2 should match word1 except by one char
        while (i < len1 && j < len2) {
            if (word1[i] == word2[j]) {
                i++;
                j++;
            } else {
                // Mismatch
                if (foundExtra) {
                    return false;
                }
                foundExtra = true;
                j++;
            }
        }
        return true;
    }

    // Word1 is one character longer
    if (len1 - 1 == len2) {
        // Flip 
        return is_adjacent(word2, word1);
    }
    // Not adjacent
    return false;
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

    // BFS Queue
    std::queue< std::vector<std::string> > ladder_queue;

    // Start ladder containing only begin_word
    ladder_queue.push({ begin_word });

    // Set of words used in ladder
    std::set<std::string> visited;
    visited.insert(begin_word);

    // BFS
    while (!ladder_queue.empty()) {
        std::vector<std::string> ladder = ladder_queue.front();
        ladder_queue.pop();

        // Last word in ladder
        const std::string & last_word = ladder.back();

        // Scan dictionary
        for (std::set<std::string>::const_iterator it = word_list.begin(); it != word_list.end(); ++it) {
            const std::string & candidate = *it;

            // Extend ladder if neighbor is not visited
            if (is_adjacent(last_word, candidate) && visited.find(candidate) == visited.end()) {
                visited.insert(candidate);
                
                // Make a copy of the ladder, then add to candidate
                std::vector<std::string> new_ladder = ladder;
                new_ladder.push_back(candidate);

                // Return a new ladder if candidate == end_word
                if (candidate == end_word) {
                    return new_ladder;
                }
                
                // Enqueue to explore further
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
        std::cerr << " Cannot open file" << std::endl;
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
        std::cout << "No ladder.\n";
        return;
    }

    for (std::size_t i = 0; i < ladder.size(); i++) {
        std::cout << ladder[i];
        if (i + 1 < ladder.size()) {
            std::cout << " => ";
        }
    }
    std::cout << "\n";
}

void verify_word_ladder() {
    std::set<std::string> word_list;
    load_words(word_list, "src/words.txt");
    my_assert(generate_word_ladder("cat",   "dog",   word_list).size() == 4);
    my_assert(generate_word_ladder("marty", "curls", word_list).size() == 6);
    my_assert(generate_word_ladder("code",  "data",  word_list).size() == 6);
    my_assert(generate_word_ladder("work",  "play",  word_list).size() == 6);
    my_assert(generate_word_ladder("sleep", "awake", word_list).size() == 8);
    my_assert(generate_word_ladder("car",   "cheat", word_list).size() == 4);
}