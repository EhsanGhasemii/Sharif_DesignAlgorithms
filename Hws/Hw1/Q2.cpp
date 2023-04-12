#include <iostream>
#include <vector>
#include <algorithm>
#include <limits.h>

using namespace std;

int n, k;

struct Node {
    int value, index, next, previous, cost;
};

bool mySort(pair<int,int> a, pair<int,int> b) {
    return a.first < b.first;
}

int main() {
    // get inputs from user
    cin >> n >> k;
    vector<int> x_i(n);
    for (int i = 0; i < n; i++) {
        cin >> x_i[i];
    }

    // convert location of objects to distances
    vector<int> distances(n - 1);
    for (int i = 1; i < n; i++) {
        distances[i - 1] = x_i[i] - x_i[i - 1];
    }
    n--;

    // keep sorted inputs in an array with their indices
    vector<pair<int,int>> x_i_sorted(n);
    for (int i = 0; i < n; i++) {
        x_i_sorted[i] = make_pair(distances[i], i);
    }
    sort(x_i_sorted.begin(), x_i_sorted.end(), mySort);

    // keep inputs with a flag that shows these nodes used or not
    vector<pair<int,int>> flags(n);
    for (int i = 0; i < n; i++) {
        flags[i] = make_pair(distances[i], 0);
    }

    // main body
    int index = 0;
    vector<Node> created_nodes;
    char mode = ' ';
    for (int i = 0; i < k; i++) {

        // find first index that has flag = 0 that means this node has not been used yet
        int min_cost = INT_MAX;
        while (flags[x_i_sorted[index].second].second != 0) {
            index++;
            if (index == n) {
                break;
            }
        }

        // mode is 'index' that means we choose a single node not a subset
        if (index != n) {
            min_cost = x_i_sorted[index].first;
            mode = 'i';
        }

        // mode is 'created_nodes' that means we choose a subset of nodes that have been chosen before
        Node my_tuple = {0, 0, 0, 0, 0};
        for (auto it = created_nodes.begin(); it != created_nodes.end(); it++) {
            int cost = it->cost;
            if (cost < min_cost) {
                min_cost = cost;
                my_tuple = *it;
                mode = 'c';
            }
        }
        if (my_tuple.cost != 0) {
            created_nodes.erase(find(created_nodes.begin(), created_nodes.end(), my_tuple));
            my_tuple.value += my_tuple.cost;
            my_tuple.next++;
            my_tuple.previous--;
            my_tuple.cost *= (-1);
            if (my_tuple.next != n) {
                my_tuple.cost += flags[my_tuple.next].first;
                flags[my_tuple.next].second = 1;
            } else {
                my_tuple.cost = INT_MAX;
            }

            if (my_tuple.previous != -1) {
                my_tuple.cost += flags[my_tuple.previous].first;
                flags[my_tuple.previous].second = 1;
            } else {
                my_tuple.cost = INT_MAX;
            }
        }

        // do processing if we are in mode = 'index'
        if (mode == 'i') {
            // set a complete tuple of information for chosen nodes
            int x_value = x_i_sorted[index].first;
            int x_index = x_i_sorted[index].second;
            int x_next = x_index + 1;
            int x_previous = x_index - 1;
            int x_cost = -x_value;
            flags[x_index].second = 1;
            if (x_next != n) {
                flags[x_next].second = 1;
                x_cost += flags[x_next].first;
            } else {
                x_cost = INT_MAX;
            }

            if (x_previous != -1) {
                flags[x_previous].second = 1;
                x_cost += flags[x_previous].first;
            } else {
                x_cost = INT_MAX;
            }
            my_tuple = {x_value, x_index, x_next, x_previous, x_cost};
        }

        // check if we have a subset of nodes that have overlap with new node that we want to insert
        char c_mode1 = ' ';
        char c_mode2 = ' ';
        vector<Node>::iterator node1;
        vector<Node>::iterator node2;
        for (auto it = created_nodes.begin(); it != created_nodes.end(); it++) {

            // if next of my_tuple has overlap with previous of another nodes/node
            if (my_tuple.next == it->previous) {
                int x_value = my_tuple.value + it->value;
                int x_index = my_tuple.index;
                int x_next = it->next;
                int x_previous = my_tuple.previous;
                int x_cost = it->cost + my_tuple.cost - flags[my_tuple.next].first;
                my_tuple = {x_value, x_index, x_next, x_previous, x_cost};
                node1 = it;
                c_mode1 = 'o';
            }

            // if previous of my_tuple has overlap with next of another nodes/node
            if (my_tuple.previous == it->next) {
                int x_value = my_tuple.value + it->value;
                int x_index = my_tuple.index;
                int x_next = my_tuple.next;
                int x_previous = it->previous;            
                int x_cost = it->cost + my_tuple.cost - flags[my_tuple.previous].first;
                my_tuple = {x_value, x_index, x_next, x_previous, x_cost};
                node2 = it;
                c_mode2 = 'o';
            }
        }
        if (c_mode1 == 'o') {
            created_nodes.erase(node1);
        }
        if (c_mode2 == 'o') {
            created_nodes.erase(node2);
        }

        if (my_tuple.cost != 0) {
            created_nodes.push_back(my_tuple);
        }
    }

    int output = 0;
    for (auto it = created_nodes.begin(); it != created_nodes.end(); it++) {
        output += it->value;
    }
    output *= 2;

    cout << output << endl;

    return 0;
}