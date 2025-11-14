#include <stdio.h>
#include <limits.h>

// A simple alpha-beta search on a fixed tree

int alphabeta(int depth, int nodeIndex, int maximizingPlayer,
              int alpha, int beta, int values[], int height)
{
    if (depth == height)
        return values[nodeIndex];

    if (maximizingPlayer) {
        int best = INT_MIN;

        // left child
        int val = alphabeta(depth + 1, nodeIndex * 2, 0, alpha, beta, values, height);
        best = (best > val) ? best : val;
        alpha = (alpha > best) ? alpha : best;

        if (beta <= alpha) {
            printf("Pruned right child of node %d (MAX)\n", nodeIndex);
            return best;
        }

        // right child
        val = alphabeta(depth + 1, nodeIndex * 2 + 1, 0, alpha, beta, values, height);
        best = (best > val) ? best : val;
        alpha = (alpha > best) ? alpha : best;

        return best;

    } else {
        int best = INT_MAX;

        // left child
        int val = alphabeta(depth + 1, nodeIndex * 2, 1, alpha, beta, values, height);
        best = (best < val) ? best : val;
        beta = (beta < best) ? beta : best;

        if (beta <= alpha) {
            printf("Pruned right child of node %d (MIN)\n", nodeIndex);
            return best;
        }

        // right child
        val = alphabeta(depth + 1, nodeIndex * 2 + 1, 1, alpha, beta, values, height);
        best = (best < val) ? best : val;
        beta = (beta < best) ? beta : best;

        return best;
    }
}

int main() {
    // Leaf nodes in array form (complete binary tree)
    int values[] = {0, 0, 0, 0, 41, 5, 12, 90, 101, 80, 20, 30, 34, 80, 36, 25};

    int height = 4; // tree depth
    int result = alphabeta(0, 1, 1, INT_MIN, INT_MAX, values, height);

    printf("\nFinal Minimax Value at Root = %d\n", result);

    return 0;
}
