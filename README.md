[![Build Status](https://travis-ci.org/FragJage/SimpleSock.svg?branch=master)](https://travis-ci.org/FragJage/SimpleSock)
[![Build status](https://ci.appveyor.com/api/projects/status/pmlpbnfpc286i6am?svg=true)](https://ci.appveyor.com/project/FragJage/simplesock)
[![Coverage Status](https://coveralls.io/repos/github/FragJage/SimpleSock/badge.svg?branch=master&bust=1)](https://coveralls.io/github/FragJage/SimpleSock?branch=master)
[![Coverity Scan Build](https://scan.coverity.com/projects/7738/badge.svg)](https://scan.coverity.com/projects/7738)

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
 - Security audit with VisualCodeGrepper
 - Dynamic audit with Address Sanitizer.
 - Unit tests replay for each version.

Portability
===========
Unit tests passed successfully on :
 - Windows Seven (CPU Intel Celeron)
 - Linux Ubuntu (CPU Intel Atom)
 - Linux Raspian on Raspberry Pi (CPU ARM)
 - Linux FunPlug on NAS DNS-320 (CPU ARM)

For more informations
=====================
Documentation http://catjage.free.fr/dev/SimpleSock/index.html

Licence
=======
SimpleSock is free software : you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

SimpleSock is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with SimpleSock. If not, see http://www.gnu.org/licenses/.
