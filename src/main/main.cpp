
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

bool processArguments(int argc,
                      char const *argv[],
                      char *board,
                      int &aiPlayerID,
                      int &numThreads,
                      int &serachDepth) {

    // Initialize arguments
    board[0]    =  0;
    aiPlayerID  = -1;
    numThreads  = -1;
    serachDepth = -1;

    for (int i = 0; i < argc; i++) {
        const char *arg = argv[i];
        if (strncmp(arg, "-b", 2) == 0) {
            // Check if value exists
            if (i >= argc - 1) return false;

            // Check value length
            if (strlen(argv[i + 1]) != 225) return false;

            // Copy board
            strncpy(board, argv[i + 1], 225);
        } else if (strncmp(arg, "-p", 2) == 0) {
            // Check if value exists
            if (i >= argc - 1) return false;

            //
            aiPlayerID = atoi(argv[i + 1]);
        } else if (strncmp(arg, "-t", 2) == 0) {
            // Check if value exists
            if (i >= argc - 1) return false;

            //
            numThreads = atoi(argv[i + 1]);
        } else if (strncmp(arg, "-d", 2) == 0) {
            // Check if value exists
            if (i >= argc - 1) return false;

            serachDepth = atoi(argv[i + 1]);
        }
    }

    if (board[0] == 0) return false;

    // Default values
    if (aiPlayerID == -1)   aiPlayerID  = 1;
    if (numThreads == -1)   numThreads  = 1;
    if (serachDepth == -1)  serachDepth = 4;

    return true;
}

int main(int argc, char const *argv[]) {

    char board[226] = {0};
    int aiPlayerID, numThreads, serachDepth;

    if (!processArguments(argc, argv, board, aiPlayerID, numThreads, serachDepth)) {
        printf("Invalid arguments\n");
        return 1;
    }

    printf("board: %s\n", board);
    printf("aiPlayerID: %d\n", aiPlayerID);
    printf("numThreads: %d\n", numThreads);
    printf("serachDepth: %d\n", serachDepth);

    return 0;
}
