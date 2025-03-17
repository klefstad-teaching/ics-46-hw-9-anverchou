#include "../src/dijkstras.h"        
#include "../src/ladder.h"            

static Graph build_small_graph() {
    Graph G;
    G.numVertices = 4;
    G.resize(4);
    G[0].push_back(Edge(0, 1, 3));
    G[0].push_back(Edge(0, 2, 1));
    G[2].push_back(Edge(2, 1, 1));
    G[1].push_back(Edge(1, 3, 2));
    G[2].push_back(Edge(2, 3, 7));
    G[3].push_back(Edge(3, 0, 5));

    return G;
}

TEST(DijkstraTests, BasicSmallGraph) {
    Graph G = build_small_graph();

    std::vector<int> previous(G.numVertices, -1);
    std::vector<int> dist = dijkstra_shortest_path(G, 0, previous);
    EXPECT_EQ(dist[0], 0);
    EXPECT_EQ(dist[1], 2);
    EXPECT_EQ(dist[2], 1);
    EXPECT_EQ(dist[3], 4);

    std::vector<int> path = extract_shortest_path(dist, previous, 3);
    ASSERT_EQ(path.size(), 4u);
    EXPECT_EQ(path[0], 0);
    EXPECT_EQ(path[1], 2);
    EXPECT_EQ(path[2], 1);
    EXPECT_EQ(path[3], 3);
}

TEST(LadderTests, Adjacency) {
    EXPECT_TRUE(is_adjacent("cat", "cot"));
    EXPECT_TRUE(is_adjacent("cate", "date"));
    EXPECT_FALSE(is_adjacent("cat", "cut"));
    EXPECT_FALSE(is_adjacent("cat", "cat"));   

    EXPECT_TRUE(is_adjacent("cat", "cart"));
    EXPECT_TRUE(is_adjacent("chat", "hat"));   
    EXPECT_FALSE(is_adjacent("cat", "carts")); 
}

static std::set<std::string> build_small_dictionary() {
    std::set<std::string> dict;
    dict.insert("cat");
    dict.insert("cot");
    dict.insert("dot");
    dict.insert("dog");
    return dict;
}

TEST(LadderTests, SmallLadderCatDog) {
    std::set<std::string> dict = build_small_dictionary();

    std::vector<std::string> ladder = generate_word_ladder("cat", "dog", dict);

    ASSERT_EQ(ladder.size(), 4u);
    EXPECT_EQ(ladder[0], "cat");
    EXPECT_EQ(ladder[1], "cot");
    EXPECT_EQ(ladder[2], "dot");
    EXPECT_EQ(ladder[3], "dog");
}

TEST(LadderTests, SameWord) {
    std::set<std::string> dict = build_small_dictionary();
    std::vector<std::string> ladder = generate_word_ladder("cat", "cat", dict);
    EXPECT_TRUE(ladder.empty());
}

TEST(LadderTests, FullDictionaryTest) {
    std::set<std::string> dict;
    load_words(dict, "words.txt");
    ASSERT_FALSE(dict.empty()); 

    if (dict.find("cat") != dict.end() &&
        dict.find("cot") != dict.end() &&
        dict.find("dot") != dict.end() &&
        dict.find("dog") != dict.end())
    {
        std::vector<std::string> ladder = generate_word_ladder("cat", "dog", dict);
        EXPECT_EQ(ladder.size(), 4u);
    }
    else {
        SUCCEED();
    }
}
