SimpleSock
==========
Very simple class to manage socket communication TCP or UDP.

Introduction
============
A very simple class to manage socket communication TCP or UDP.
To use, include in your project SimpleSock.cpp and SimpleSock.h.

Features
========
 - Support TCP and UDP Socket
 - Calculate broadcast address
 - Family AF_INET (IPv4)
 - Compile on Linux and Windows, Intel or ARM.

Quality developpment
====================
 - Static audit with CppCheck
 - Dynamic audit with Address Sanitizer.
 - Unit tests replay for each version.

Portability
===========
Unit tests passed successfully on :
 - Windows Seven (CPU Intel Celeron)
 - Linux Ubuntu (CPU Intel Atom)
 - Linux Raspian on Raspberry Pi (CPU ARM)
 - Linux FunPlug on NAS DNS-320 (CPU ARM)
(Compilation directives define LINUX or WIN only necessary for colours in unit tests)

For more informations
=====================
See documentation in doc/html/index.html

Licence
=======
SimpleSock is free software : you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

SimpleSock is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with SimpleSock. If not, see http://www.gnu.org/licenses/.
