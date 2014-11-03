Modern-openZWave
==========

A modern implementation of the openzwave library in C++11

This project is a full rewrite of the MinOZW example in the OpenZWave library. The idea is to allow programmers to quickly implement this and use ZWave without any troubles. All the including files EXCEPT the Makefile fall under the following license.

Installation:
-----
For ubuntu/debian users install this:
        
    sudo apt-get install subversion libudev-dev

Then run ./install.sh (this assumes you have sudo installed and sudo rights on the current user). 
After installing you have the ModernOZW executable :) The installation (compilation) might take some time on devices like the Raspberry pi.

Todo:
-----
- Comment and document code
- Complete the hook system
- Json communication to other programs

License:
-----


    Copyright (C) 2014  Merijn Testroote

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

