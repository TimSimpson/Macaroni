C++ Code Generation
===================
This is a brief overview of the code generation facilities and how the classes
interact.


CodeType
--------
For each Macaroni Element there are different types of code we may write. These include the implementation (which contains the definition of non-inline or template functions), the type definition, etc. This class refers to what type of text we want to write, and also includes some things that aren't really C++, such as the divider between the internal header and implementation Macaroni writes to CPP files.


CodeText
--------
This combines a Macaroni Element and a CodeType which collectively represent a bit of text you might want to write to a file.

It also includes a list of dependencies on that text- i.e. other CodeText elements that must be written before in order for the CodeText to make sense.

For example, if you write a typedef for std::vector like this:

.. code:

    typedef std::vector Vector;

The CodeText which has an Element of "Vector" and a CodeType of "TypeDefinition" would probably be written as the text above, but it would have a dependency on the CodeText for the Element "std::vector" and "TypeDefinition", which if written would be "#include <vector>".


CodeGraph
---------
This a repository for all CodeText instances. It has the ability to generate new CodeText items and handles managing their memory. It has a method "Get" which accepts an Element and a CodeType and returns a CodeText reference (which is unsafe to use after the CodeGraph is destroyed) and may also create all CodeText instances which would be dependencies of the given Element / CodeType pair.

By making CodeText instances live here it is possible to cache them. For example, while only one translation unit may define a class, multiple ones may use that class and need access to the class's Forward Declaration code path. Using CodeGraph the dependencies of this item are only calculated once.


CodeTextSorter
--------------
Given a list of CodeText items, it has the capability to sort these into an order that can be written to a translation unit's source files.


CodeTextProducer
----------------
This is the logic for writing out the CodeText items. It must be consturcted wuith a CppFileWriter (see below). Each call to it's "Write" method must include a CodeText instance, and a Target and Library smart pointer. It uses these later two items to know if an item it is rendering lives in a given target which can affect things, and to properly produce the declspec stuff for libraries.


CppFileWriter
-------------
This has a huge series of functions that write text to a C++ header or implementation file. Because the text itself is somewhat complicated, it's easier to have a seperate class from CodeTextProducer. Methods here will be things such as "EnterNamespace" which would write out the namespace keyword so future code would be defined in a given namespace, or "Comment" which would write a comment.

This has some extra logic in it which contains info on the namespace that is currently active (i.e. if the text "namespace acme{ " just got written then the "acme" namespace is active) as well as what nodes are being used (so if you're inside of class X with nested class Y, writing the type out is just "Y" and not "X::Y").


UnitFileGenerator
-----------------
This handles writing a translation unit. It's constructor accepts a LibraryTarget and normal Target which represents the unit it will be writing a .h and .cpp file for.  It's Generate function then accepts a CodeGraph as well as a Path object to the root directory of where all code is being generated.

This implies the CodeGraph is longer lived which a new UnitFileGenerator is created for each target, and it creates a CodeTextSorter to sort the nodes then two CodeTextProducers to write them.
