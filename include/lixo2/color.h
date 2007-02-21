/* 
 * color.h
 *
 * Copyright (C) 2004 Universidade Aveiro - Instituto de Telecomunicacoes Polo A
veiro
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Authors:     Diogo Gomes, <dgomes@av.it.pt>
 */
#define RESET (char) 0x1b << "[0m" 
#define BOLD (char) 0x1b << "[1m" 
#define ITALIC (char) 0x1b << "[3m" 

#define FG_BLACK (char) 0x1b << "[30m" 
#define FG_RED (char) 0x1b << "[31m" 
#define FG_GREEN (char) 0x1b << "[32m" 
#define FG_YELLOW (char) 0x1b << "[33m" 
#define FG_BLUE (char) 0x1b << "[34m" 
#define FG_MAGENTA (char) 0x1b << "[35m" 
#define FG_CYAN (char) 0x1b << "[36m" 
#define FG_WHITE (char) 0x1b << "[37m" 

#define BG_BLACK (char) 0x1b << "[40m" 
#define BG_RED (char) 0x1b << "[41m" 
#define BG_GREEN (char) 0x1b << "[42m" 
#define BG_YELLOW (char) 0x1b << "[43m" 
#define BG_BLUE (char) 0x1b << "[44m" 
#define BG_MAGENTA (char) 0x1b << "[45m" 
#define BG_CYAN (char) 0x1b << "[46m" 
#define BG_WHITE (char) 0x1b << "[47m"
