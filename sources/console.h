/**
 * Copyright 2024 fruit-bat
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <cstring>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ioctl.h>

void conctrl_save_cursor_pos();
void conctrl_restore_cursor_pos();
void conctrl_mono_mode();
void conctrl_mono80_mode();
void conctrl_home();
void conctrl_cls();
void conctrl_clear_to_end_of_line();
void conctrl_clear_to_end_of_screen();
void conctrl_black_text_on_white();
void conctrl_white_text_on_black();
void conctrl_move_cursor(const uint8_t x, const uint8_t y);
int conctrl_kbhit();
int conctrl_getch();

#define ESC ((char)0x1b)

int conctrl_kbhit(void) {
	int characters_buffered;
	ioctl(STDIN_FILENO, FIONREAD, &characters_buffered);
	return characters_buffered;
}

int conctrl_getch(void) {
	if (!conctrl_kbhit()) return 0;
    const int c = getchar();
    switch(c) {
        case 10: return 13;
        default: return c;
    }
}

void conctrl_save_cursor_pos() {
    std::cout << ESC << "[s";
}

void conctrl_restore_cursor_pos() {
    std::cout << ESC << "[u";
}

void conctrl_mono_mode() {
    // ESC[=0h	40 x 25 monochrome (text)
    std::cout << ESC << "[=0h";
}

void conctrl_mono80_mode() {
    // ESC[=2h	80 x 25 monochrome (text)
    std::cout << ESC << "[=2h";
}

void conctrl_home() {
    std::cout << ESC << "[H";
}

void conctrl_cls(){
    std::cout << ESC << "[2J" << ESC << "[H";
}

void conctrl_black_text_on_white() {
    std::cout << ESC << "[0m";
    std::cout << ESC << "[7m";
}

void conctrl_white_text_on_black() {
    std::cout << ESC << "[0m";
}

void conctrl_move_cursor(const uint8_t row, const uint8_t col) {
    std::cout << ESC << "[" << (unsigned)row << ';' <<  (unsigned)col << 'H';
}

void conctrl_clear_to_end_of_line() {
    std::cout << ESC << "[0K";
}

void conctrl_clear_to_end_of_screen() {
    std::cout << ESC << "[0J";
}

// See https://www.seasip.info/Cpm/cpm86esc.html	
//     https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797
//     https://mdfs.net/Archive/info-cpm/1985/01/19/053100.htm
//     https://www.cse.psu.edu/~kxc104/class/cmpen472/16f/hw/hw8/vt100ansi.htm#:~:text=Cursor%20Home%20%5B%7B,upper%20left%20of%20the%20screen.
//
// Buffer console output so we can execute control sequences
// TODO need a way to reset these
static uint8_t conb[4];
static unsigned coni = 0; 

void console_reset() {
    coni = 0; 
}

unsigned console_el() {
    switch(conb[1]) {
        case 'Y':
        case 'd':
        case 'e':
        case 'f':
        case 'g':
        case 'h':
        case 'l':
        case '=':
            return 4;
        case 'a': 
        case 'b':
        case 'c':
            return 3;
        default: return 2;
    }
}

void console_output_buf() {
    for(int i = 0; i < coni; ++i) {
        std::cout << '(' << std::hex << (unsigned)conb[i] << ')';
        if (i > 0) {
            std::cout << (char)conb[i];
        }
    }
}

void console_exeb() {

    switch(conb[1]) {
// ESC 0
// disable the status line (containing the clock).
// ESC 1
// enable the status line.
// ESC A
// cursor up, unless at the top
// ESC B
// cursor down, unless at the bottom
// ESC C
// cursor right, unless at the right-hand edge
// ESC D
// cursor left, unless at the left-hand edge
    case 'E': { // ESC E - clear screen and home cursor
        conctrl_cls();
        break;
    }
// ESC H
// home cursor
// ESC I
// cursor up, scroll down if at top
// ESC J
// delete to end of screen
// ESC K
// delete to end of line
// ESC L
// insert a line, moving current line and all lines below down by 1
// ESC M
// delete current line, moving all lines below up by 1
// ESC N
// delete character at cursor position, moving all characters right of it one character to the left
    case 'Y': case '=': { // ESC Y r c - Move cursor to row r-32, column c-32
        conctrl_move_cursor(
            conb[2] - 32,
            conb[3] - 32
        );
        break;
    }
// ESC a m
// Select screen mode m
// ESC b c
// Set foreground colour c
// ESC c c
// Set background colour c
// ESC d h l
// Redirect CONIN
// ESC e h l
// Redirect CONOUT
// ESC f h l
// Redirect AUXIN
// ESC g h l
// Redirect AUXOUT
// ESC h h l
// Redirect LST
// All redirections combine h and l to form a device number:

//                  0, 1 = CRT
//                  2,3  = COM1-2
//                  4,5,6= LPT1-3
//                  7    = CRT
//                  8    = light pen
//                  9,10 = CRT
	case 'j': { // ESC j - Save cursor position
        conctrl_save_cursor_pos();
        break;
    }
    case 'k': { // ESC k - Restore cursor position
        conctrl_restore_cursor_pos();
        break;
    }
// ESC l
// Erase all characters on the current line
// ESC m
// Enable cursor
// ESC n
// Disable cursor
// ESC o
// Erase characters up to the cursor position


    case 'p': { // ESC p - Select black text on white background
        conctrl_black_text_on_white();
        break;
    }
    case 'q': { // ESC q - Select white text on black background
        conctrl_white_text_on_black();
        break;
    }
// ESC r
// Select bright text
// ESC s
// Select flashing text
// ESC t
// Cancel flashing text
// ESC u
// Cancel bright text
// ESC v
// Allow wrapping at end of line
// ESC w
// Do not wrap at end of line
    case 'x': { // ESC x - Mode MONO
        conctrl_mono_mode();
        break;
    }
    case 'y': { // ESC y - Mode CO80
        conctrl_mono80_mode();
        break;
    }


// ESC / bh bl
// Call INT 10h, function 0Bh. ie:
// ESC / 00h n - set border
// ESC / 01h n - set CGA palette

        default: {
            console_output_buf();
            break;
        }
    }

    coni = 0;
}

void console_chkb() {
    if (coni >= 2 && conb[0] == 0x1b) {
        unsigned el = console_el();
        if (coni >= el) {
            console_exeb();
        }
    }
}

inline void console_pushc(const uint8_t c) {
    if (coni < sizeof(conb)) {
        conb[coni++] = c;
    }
    console_chkb();
}

void console_putc(const uint8_t c) {
    const uint8_t a = c & 127; // remove parity?
    if (a == 0x1b || coni > 0) {
        console_pushc(a);
    }
    else {
//         Cursor left (bs) .............  08h     08
//         Cursor right .................  0Ch     12
//         Cursor down (lf) .............  0Ah     10
//         Cursor up ....................  0Bh     12
//         Home cursor ..................  1Eh     30
//         Clear screen & home cursor ...  1Ah     26
//         Carriage return ..............  0Dh     13 
//         Clear EOL (Ctl-X) ............  18h     24
//         Clear EOS (Ctl-W) ............  17h     23
        switch(a) {
            case 0x08:
            case 0x0c:
            case 0x0a:
            case 0x0b: {
                putchar(a);
             //   std::cout << '[' << std::hex << (unsigned)a << ']';
                break; 
            }
            case 0x17: { // Clear EOS (Ctl-W) ............  17h     23
                conctrl_clear_to_end_of_screen();
                break;
            }
            case 0x18: { // Clear EOL (Ctl-X) ............  18h     24
                conctrl_clear_to_end_of_line();
                break;
            }            
            case 0x1a: { // Clear screen & home cursor ...  1Ah     26
                conctrl_cls();
                break;
            }
            case 0x1e: { // Home cursor ..................  1Eh     30
                conctrl_home();
                break;
            }
            default: {
                putchar(a);
        //    if (a < 32) std::cout << '[' << std::hex << (unsigned)a << ']';
                break;
            }
        }
    }
}
