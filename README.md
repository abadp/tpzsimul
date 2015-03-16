# Overview #

TOPAZ is a general-purpose interconnection network simulator that allows the modeling a wide variety of message routers with different tradeoffs between speed and precision. TOPAZ comes from [SICOSYS](http://www.atc.unican.es/SICOSYS/) simulator, which was originally conceived to obtain results are very close to those obtained by using HDL description of networks components by hardware simulators but at lower computational cost. In order to make the tool easily comprehensible, extensible and reusable, the design of the tool is object-oriented and its implementation is in C++ language. For the models provided, approximately 110 classes, distributed in about 50,000 lines of code have been necessary. The simulator has support for parallel execution using standard POSIX threads. The portability is high: can be used in **any UNIX** platform with a C++ standard compiler.

# Announcements #

  * _02/10/2012_ Initial release publicly available (See [here](http://code.google.com/p/tpzsimul/source/checkout))
  * _05/09/2012_ Paper presented at NOCs
  * _07/29/2012_ Updated to latest GEM5 version
  * _05/09/2012_ Updated to latest ruby API changes in GEM5
  * _04/16/2013_ Uploaded [LIGERO](http://goo.gl/cDJtI) router. Fully connected crossbar performance with scalable low-cost network?
  * _04/17/2013_ Updated Topaz-GEM5 interface to support latest GEM5 stable version (Changeset 926e24224171)
  * _10/11/2013_ Updated Topaz-GEM5 interface to support latest GEM5 development version (Changeset ff839efdd92a)
  * _09/03/2014_ Updated Topaz-GEM5 interface to support latest GEM5 development version (Changeset 1e2f39859382)
  * _12/17/2014_ Updated Topaz-GEM5 interface to support latest GEM5 stable version (Changeset bdb307e8be54 in gem5-dev repo )

# Documentation #
  * [Getting Started](http://code.google.com/p/tpzsimul/wiki/GStarted) A quick guide to understad how to get the sources and compile the simulator.
  * [Simulator Configuration](http://code.google.com/p/tpzsimul/wiki/SimulationConfiguration) How to configure the simulator and most common command line options
  * [SGML Configuration](http://code.google.com/p/tpzsimul/wiki/SGML) A quick review of SGML configuration files. How to define Routers, Netwkorks and Simulations
  * [Components Available](http://code.google.com/p/tpzsimul/wiki/Out_of_the_box) A list of the components availables in the _tip_
  * [GEM5 Integration](http://code.google.com/p/tpzsimul/wiki/GEM5Integration) A small review of **GEM5** integration and an introductory HOW-TO  for using GEM5 with TOPAZ networks

# Support #
In order to allow other users to interact, please submit your questions **only** to the [topaz-discuss list](http://groups.google.com/group/topaz-discuss) .

# References #
If you use this tool in your research, please cite the following paper:

P.Abad, P.Prieto, L.Menezo, A.Colaso, V.Puente, J.A. Gregorio _TOPAZ: An Open-Source Interconnection Network Simulator for Chip Multiprocessors and Supercomputers_, **NOCS 2012** ([pdf](https://sites.google.com/site/atcgalerna/home-1/publications/files/NOCS-2012_Topaz.pdf?attredirects=0))
