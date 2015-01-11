2048 Engine + AI
================

Author: Shane Barratt

This is a lightweight 2048 Engine and AI written in C. Boards are stored as 64-bit integers, with 4 bits for each spot. Moves are found using a transposition table, and rows/columns are extracted using bit manipulation. The AI is an expectimax AI, and the board evaluation function counts the number of open spaces and if the pieces on the bottom row are in order.