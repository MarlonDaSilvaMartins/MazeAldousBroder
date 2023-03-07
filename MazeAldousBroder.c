#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>


#define N      4	// mesh size (always squared)

enum Side { TOP = 0, RIGHT = 1, BOTTOM = 2, LEFT = 3};

void ResetMaze(bool Maze[N][N], bool Visited[N][N])
{
    int i, j;

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            Maze[i][j] = false;
            Visited[i][j] = false;
        }
    }
}

void ShowMazeValues(bool Maze[N][N])
{
    // for testing purpouses only
    int i, j;

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            printf(" %d ", Maze[i][j]);
        }
        printf("\n");
    }

}

void ShowSolutionValues(bool Solution[N][N])
{
    // for testing purpouses only
    int i, j;

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            printf(" %c ", Solution[i][j] ? '#' : '.');
        }
        printf("\n");
    }

}

void ShowMaze(bool Maze[N][N]){

    int i, j;

    for (i = 0; i < N; i++){

        // ##### TOP #####
        for (j = 0; j < N; j++){
            // top (first line only)
            if (i == 0){
                printf("--");
            }
        }
        printf("\n");
        // FIM TOP

        // #### both sides ####
        for (j = 0; j < N; j++){

            // LEFT for the first column only
            if (j == 0){
                printf("%c", '|');
            }

            // RIGHT for the last column only
            if (j == (N - 1)){
                printf("%c", '|');
            }

            if (j > 0 && j < (N - 1)){
                // check the walls between two intermediate columns
                printf("%s", Maze[i][j] ? "#" : "  ");
            }
        }
        printf("\n");
        // fim both sides

        // #### BOTOM - last line only ####
        if (i == (N - 1)){
            for (j = 0; j < N; j++){
                printf("--");
            }
            printf("\n");
        }
    }
}

void ShowMazeDetails(bool Maze[N][N], bool Visited[N][N], int currCellLine, int currCellColumn, bool ClearScreen){
    if( ClearScreen) system("@cls||clear");

    int i, j;

    // for each line, we have to print 3 levels (top, botton and both sides of each cell)
    for (i = 0; i < N; i++){

        // ##### TOP #####
        for (j = 0; j < N; j++){
            // top (first line only)
            if (i == 0){
                if ((Maze[i][j] & TOP) == TOP){
                    printf("+---");
                }else{
                    printf("+   ");
                }
                if (j == (N - 1)) printf("+");
            }

            // top (second line on)
            if (i > 0){
                if ((Maze[i - 1][j] & BOTTOM) == BOTTOM || (Maze[i][j] & TOP) == TOP){
                    printf("+---");
                }else{
                    //printf("+ = ");
                    printf("+   ");
                }
                if (j == (N - 1)) printf("+");
            }
        }
        printf("\n");
        // FIM TOP

        // #### both sides ####
        for (j = 0; j < N; j++){

            char L = ' ';
            char R = ' ';
            char C = ' ';

            // cell content
            C = ' ';
            if (!Visited[i][j]) C = '*';
            if (i == currCellLine && j == currCellColumn) C = 'C';

            // LEFT and RIGHT for the first column only
            if (j == 0)
            {
                if ((Maze[i][j] & LEFT) == LEFT)  L = '|';
                if ((Maze[i][j] & RIGHT) == RIGHT) R = '|';

                printf("%c %c %c", L, C, R);
            }

            // LEFT and RIGHT for the last column only
            L = 0; R = ' ';
            if (j == (N - 1))
            {
                if ((Maze[i][j] & LEFT) == LEFT)  L = '|';
                if ((Maze[i][j] & RIGHT) == RIGHT) R = '|';
                if ((Maze[i][j - 1] & RIGHT) == RIGHT) L = 0;

                printf("%c %c %c", L, C, R);
            }

            // LEFT and RIGHT for the intermediate columns only
            L = ' '; R = ' '; /*C = '*'; */
            if (j > 0 && j < (N - 1))
            {
                // check the walls between two intermediate columns
                if ((Maze[i][j] & RIGHT) == RIGHT || (Maze[i][j + 1] & LEFT) == LEFT) L = '|';

                printf(" %c %c", C, L);

            }
            //printf("\n");
        }
        printf("\n");
        // fim both sides

        // #### BOTOM - last line only ####
        if (i == (N - 1))
        {
            for (j = 0; j < N; j++)
            {
                if ((Maze[i][j] & BOTTOM) == BOTTOM)
                {
                    printf("+---");
                }
                else
                {
                    printf("+   ");
                }
                if (j == (N - 1)) printf("+");
            }
            printf("\n");
        }

    }

}

void ShowMazeSolution(bool Maze[N][N], bool Solution[N][N], bool ClearScreen){
    if (ClearScreen) system("cls||clear");

    int i, j;

    printf("\n");
    // for each line, we have to print 3 levels (top, botton and both sides of each cell)
    for (i = 0; i < N; i++){

        // ##### TOP #####
        for (j = 0; j < N; j++){
            // top (first line only)
            if (i == 0){
                printf("--");
            }
        }
        printf("\n");
        // FIM TOP

        // #### both sides ####
        for (j = 0; j < N; j++){
            char C = ' ';

            // cell content
            if (Solution[i][j]) C = 'O';

            // LEFT for the first column only
            if (j == 0){
                printf("%c", "|");
            }

            // RIGHT for the last column only
            if (j == (N - 1)){
                printf("%c", '|');
            }

            // intermediate columns only
            if (j > 0 && j < (N - 1)){
                printf("%c", C);
            }
            //printf("\n");
        }
        printf("\n");
        // fim both sides

        // #### BOTOM - last line only ####
        if (i == (N - 1)){
            for (j = 0; j < N; j++){
                printf("--");
            }
            printf("\n");
        }
    }
}


void CloseCell(bool Maze[N][N], int Line, int Column, bool Open, bool Guess){

    if (Guess == TOP){
        if (Line > 0){
            Maze[Line - 1][Column] = Open;
        }
    }
    if (Guess == BOTTOM){
        if (Line < (N - 1)){
            Maze[Line + 1][Column] = Open;
        }
    }
    if (Guess == LEFT){
        if (Column > 0){
            Maze[Line][Column - 1] = Open;
        }
    }
    if (Guess == RIGHT){
        if (Column < (N - 1)){
            Maze[Line][Column + 1] = Open;
        }
    }
}

void PickupRandomCell(bool Maze[N][N], int* Line, int* Column)
{
    // returns a random cell from the input maze
    *Line   = rand() % N;
    *Column = rand() % N;

}

bool PickupOpenOrClose(){
    if((rand() % N) % 2 == 0){
        return true;
    }
    return false;
}


void AldousBroderMaze(bool Maze[N][N]){
    bool Visited[N][N];
    int unvisitedCells;
    int currentCellLine=-1, currentCellColumn=-1, neighCellLine=-1, neighCellColumn=-1;
    int guess=-1;
    int cont = 0;

    ResetMaze(Maze, Visited);

    PickupRandomCell(Maze, &currentCellLine, &currentCellColumn);
    unvisitedCells = N * N;

    while (unvisitedCells > 0){
        // pickup a random neighbour from the current cell
        bool open = false;
        bool found = false;
        while (!found){
            // 0: UP
            // 1: DOWN
            // 2: LEFT
            // 3: RIGHT
            guess = rand() % 4;

            switch (guess){
                case 0: // top
                    if (currentCellLine > 0){
                        neighCellLine   = currentCellLine - 1;
                        neighCellColumn = currentCellColumn;
                        found = true;
                        open = PickupOpenOrClose();
                    }
                    break;
                case 1: // DOWN
                    if (currentCellLine < (N - 1)){
                        neighCellLine   = currentCellLine + 1;
                        neighCellColumn = currentCellColumn;
                        found = true;
                        open = PickupOpenOrClose();
                    }
                    break;
                case 2: // LEFT
                    if (currentCellColumn > 0){
                        neighCellLine = currentCellLine;
                        neighCellColumn = currentCellColumn - 1;
                        found = true;
                        open = PickupOpenOrClose();
                    }
                    break;
                case 3: // RIGHT
                    if (currentCellColumn < (N - 1)){
                        neighCellLine = currentCellLine;
                        neighCellColumn = currentCellColumn + 1;
                        found = true;
                        open = PickupOpenOrClose();
                    }
                    break;
                default:
                    break;
            }
        }

        if (!Visited[neighCellLine][neighCellColumn]){
            CloseCell(Maze, currentCellLine, currentCellColumn, open, guess);
            Visited[neighCellLine][neighCellColumn] = true;
            unvisitedCells--;
        }

//        ShowMazeDetails(Maze, Visited,currentCellLine, currentCellColumn, true);
//        printf("\ncont: %d\nunvisited cells: %d\nneighCellLine:   %2d ::: \tcurrentCellLine:   %2d \nneighCellColumn: %2d ::: \tcurrentCellColumn: %2d\n\n", ++cont, unvisitedCells, neighCellLine, currentCellLine, neighCellColumn, currentCellColumn);

        currentCellLine   = neighCellLine;
        currentCellColumn = neighCellColumn;
    }
}

bool IsValidPosition(int Line, int Column)
{
    //checks wether the position is within the maze
    if ((Line >= 0 && Line <= (N - 1)) && (Column >= 0 && Column <= (N - 1))){
        return true;
    }

    return false;
}

bool MazeSolving(bool Maze[N][N], bool Solution[N][N], int LineSource, int ColumnSource, int LineDest, int ColumnDest){

    // check if the destination was reached
    if (LineSource == LineDest && ColumnSource == ColumnDest){
        Solution[LineSource][ColumnSource] = true;
        return true;
    }

    // is it a valid position?
    if (!IsValidPosition(LineSource, ColumnSource)){
        return false;
    }

    // is it already part of the path?
    if (Solution[LineSource][ColumnSource]){
        return false;
    }

    // marks current cell as part of the path
    Solution[LineSource][ColumnSource] = true;
    //ShowSolutionValues(Solution);
    ShowMazeSolution(Maze, Solution, true);
    printf("\n");
    sleep(0.1);


    // tries north, if possible
    if (LineSource > 0){
        // is the top cell open?
        if (Maze[LineSource+1][ColumnSource] == false){
            if (MazeSolving(Maze, Solution, LineSource - 1, ColumnSource, LineDest, ColumnDest)) return true;
        }
    }

    // tries south, if possible
    if (LineSource < (N - 1)){
        // is the bottom cell open?
        if (Maze[LineSource-1][ColumnSource] == false){
            if (MazeSolving(Maze, Solution, LineSource + 1, ColumnSource, LineDest, ColumnDest)) return true;
        }
    }

    // tries east, if possible
    if (ColumnSource < (N - 1)){
        // is the right cell open?
        if (Maze[LineSource][ColumnSource+1] == false){
            if (MazeSolving(Maze, Solution, LineSource, ColumnSource + 1, LineDest, ColumnDest)) return true;
        }
    }

    // tries west, if possible
    if (ColumnSource > 0){
        // is the left cell open?
        if (Maze[LineSource][ColumnSource-1] == false){
            if (MazeSolving(Maze, Solution, LineSource, ColumnSource - 1, LineDest, ColumnDest)) return true;
        }
    }

    /*
    // move forward into x direction, if possible!
    if (MazeSolving(Maze, Solution, LineSource + 1, ColumnSource, LineDest, ColumnDest)) return true;
    // move forward into y direction
    if (MazeSolving(Maze, Solution, LineSource, ColumnSource + 1, LineDest, ColumnDest)) return true;
    */

    // if none direction solved the problem, unmark current cell and return.
    Solution[LineSource][ColumnSource] = false;

    return false;
}

int main(){
    bool Maze[N][N];
    bool Solution[N][N];

    srand(time(NULL));

    int i, j;
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            Solution[i][j] = false;

    AldousBroderMaze(Maze);
//
//    ShowMazeValues(Maze);
//    ShowMaze(Maze);
//
    bool Solved = MazeSolving(Maze, Solution, 0, 0, N - 1, N - 1);
    if (Solved){
        ShowMaze(Maze); printf("\n");
        ShowMazeSolution(Maze, Solution, false);
    }else{
        printf("\n::: There is no solution for tis maze! \n");
    }


    i = 0, j = 0;
    for (i = 0; i < N; i++){
        for (j = 0; j < N; j++) {
            printf("%d", Maze[i][j]);
        }
        printf("\n");
    }

    return (0);
}
