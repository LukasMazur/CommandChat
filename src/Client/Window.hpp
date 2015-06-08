#ifndef INC_CLIEBT_WINDOW_HPP
#define INC_CLIEBT_WINDOW_HPP

#include <string>
#include <set>
#include <utility>
#include <memory>

namespace Client
{
    #include <ncurses.h>
    
    /**
     * Class providing encapsulation about ncurses functions
     */
    class CWindow
    {
        using POS = struct {
            int line;
            int col;
        };
        
        public:
            /**
             * @brief Directions in which a cursor can be moved
             */
            enum class DIR {UP, DOWN, LEFT, RIGHT, TOP, BOTTOM, START};
            
            /**
             * @brief Creates empty window
             */
            CWindow (void);
            /**
             * @brief Creates window
             * @param lines number of lines
             * @param cols number of columns
             * @param y y coordinate of upper left corner
             * @param x x coordinate of upper left corner
             */
            CWindow (int, int, int, int);
            /**
             * @param other
             */
            CWindow & operator= (CWindow &&);
            
            /**
             * @brief Refresh window
             */
            void Refresh (void) const;
            /**
             * @brief Move cursor to specified coordinates
             * @param line
             * @param col
             * @return CWindow & to support fluent interface
             */
            const CWindow & MoveTo (int, int) const;
            /**
             * @brief Move cursor to specified direction
             * @param dir Direction in which move cursor
             * @return CWindow & to support fluent interface
             */
            const CWindow & MoveTo (DIR) const;
            /**
             * @brief Prints string. If longer than screen, it is divided into multiple lines
             * @param str
             * @param attr
             * @return CWindow & to support fluent interface
             */
            const CWindow &  Print (const std::string &, int = A_NORMAL) const;
            /**
             * @brief Prints string. If longer than screen, it is cut
             * @param str
             * @param attr
             * @return CWindow & to support fluent interface
             */
            const CWindow &  PrintLine (const std::string &, int = A_NORMAL) const;
            /**
             * @brief Prints single character
             * @param c character to be printes
             * @param attr
             * @return CWindow & to support fluent interface
             */
            const CWindow &  Print (char, int = A_NORMAL) const;
            /**
             * @brief position of cursor
             * @return POS strcutre containing line and col number
             */
            POS GetPos(void) const;
            /**
             * @brief Enables using special keys
             * @param enable
             */
            void EnableKeypad (bool = true) const;
            /**
             * @brief wait for user to input a char
             * @return int code of character
             */
            int GetCh (void) const;
            /**
             * @brief Clears the window
             */
            void Clear (void) const;
            
        private:
            using pWnd = std::unique_ptr<WINDOW, decltype(&delwin)>;
            
            int lines;
            int cols;
            pWnd window;
    };    
}

#endif
